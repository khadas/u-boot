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
#define UASAN_KMALLOC_REDZONE		0xFC  /* redzone inside slub object */
#define UASAN_KMALLOC_FREE		0xFB  /* object was freed (kmem_cache_free/kfree) */
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
	bool is_write;
	unsigned long ip;
};

unsigned long mem_to_shadow(const void *mem_addr);
void uasan_poison_object(unsigned long addr, unsigned long size, unsigned tag);

#endif

#endif
