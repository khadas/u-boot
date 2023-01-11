/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __AML_UASAN_H__
#define __AML_UASAN_H__

/* for red zone between each section */
#define SECTION_RED_ZONE_NUM		8
#define MEM_SECTION_RED_ZONE_SIZE	256
#define MEM_SECTION_RED_ZONE_MASK	0xff

#define UASAN_SHADOW_SCALE_SHIFT	3

#define UASAN_SHADOW_SCALE_SIZE		(1UL << UASAN_SHADOW_SCALE_SHIFT)
#define UASAN_SHADOW_MASK		(UASAN_SHADOW_SCALE_SIZE - 1)

#define UASAN_FREE_PAGE			0xFF  /* page was freed */
#define UASAN_PAGE_REDZONE		0xFE  /* redzone for kmalloc_large allocations */
#define UASAN_MALLOC_REDZONE		0xFC  /* redzone inside slub object */
#define UASAN_MALLOC_FREE		0xFB  /* object was freed (kmem_cache_free/kfree) */
#define UASAN_GLOBAL_REDZONE		0xFA  /* redzone for global variable */

/*
 * Stack redzone shadow values
 * (Those are compiler's ABI, don't change them)
 */
#define UASAN_STACK_LEFT		0xF1
#define UASAN_STACK_MID			0xF2
#define UASAN_STACK_RIGHT		0xF3
#define UASAN_STACK_PARTIAL		0xF4
#define UASAN_USE_AFTER_SCOPE		0xF8

/*
 * alloca redzone shadow values
 */
#define UASAN_ALLOCA_LEFT		0xCA
#define UASAN_ALLOCA_RIGHT		0xCB

#define UASAN_ALLOCA_REDZONE_SIZE	32

#define UASAN_STACK_SIZE		(128 * 1024)

#define UASAN_UBOOT_SIZE		(256 * 1024 * 1024UL)

#ifndef __ASSEMBLY__

struct uasan_report_info {
	const void *access_addr;
	const void *first_bad_addr;
	size_t access_size;
	int is_write;
	unsigned long ip;
};

/* The layout of struct dictated by compiler */
struct uasan_source_location {
	const char *filename;
	int line_no;
	int column_no;
};

/* The layout of struct dictated by compiler */
struct uasan_global {
	/* Address of the beginning of the global variable. */
	const void *beg;
	/* Size of the global variable. */
	size_t size;
	/* Size of the variable + size of the red zone. 32 bytes aligned */
	size_t size_with_redzone;
	const void *name;
	/* Name of the module where the global variable is declared. */
	const void *module_name;
	unsigned long has_dynamic_init;	/* This needed for C++ */
#if KASAN_ABI_VERSION >= 4
	struct uasan_source_location *location;
#endif
#if KASAN_ABI_VERSION >= 5
	char *odr_indicator;
#endif
};

/* Used for constructor calls. */
typedef void (*ctor_fn_t)(void);

unsigned long mem_to_shadow(const void *mem_addr);
void uasan_poison_object(unsigned long addr, unsigned long size, unsigned tag);

extern const char __init_array_start[];
extern const char __init_array_end[];
void uasan_alloc(void *ptr, unsigned long size);
void uasan_free(void *ptr, unsigned long size);

#endif

#endif
