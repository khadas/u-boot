// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <linux/string.h>

void __asan_init(void)
{
}

/* make compiler shutup */
void __asan_handle_no_return(void)
{
}

static __always_inline void check_memory_region_inline(unsigned long addr,
						size_t size, bool write,
						unsigned long ret_ip)
{
	if (unlikely(size == 0))
		return;

	/* TODO: */
#if 0
	if (unlikely((void *)addr <
		kasan_shadow_to_mem((void *)KASAN_SHADOW_START))) {
		kasan_report(addr, size, write, ret_ip);
		return;
	}

	if (likely(!memory_is_poisoned(addr, size)))
		return;

	kasan_report(addr, size, write, ret_ip);
#endif
}

static void check_memory_region(unsigned long addr,
				size_t size, bool write,
				unsigned long ret_ip)
{
	check_memory_region_inline(addr, size, write, ret_ip);
}

#define DEFINE_ASAN_LOAD_STORE(size)					\
	void __asan_load##size(unsigned long addr)			\
	{								\
		check_memory_region_inline(addr, size, false, _RET_IP_);\
	}								\
	__alias(__asan_load##size)					\
	void __asan_load##size##_noabort(unsigned long);		\
	void __asan_store##size(unsigned long addr)			\
	{								\
		check_memory_region_inline(addr, size, true, _RET_IP_);	\
	}								\
	__alias(__asan_store##size)					\
	void __asan_store##size##_noabort(unsigned long);		\

DEFINE_ASAN_LOAD_STORE(1);
DEFINE_ASAN_LOAD_STORE(2);
DEFINE_ASAN_LOAD_STORE(4);
DEFINE_ASAN_LOAD_STORE(8);
DEFINE_ASAN_LOAD_STORE(16);

void __asan_loadN(unsigned long addr, size_t size)
{
	check_memory_region(addr, size, false, _RET_IP_);
}

__alias(__asan_loadN)
void __asan_loadN_noabort(unsigned long, size_t);

void __asan_storeN(unsigned long addr, size_t size)
{
	check_memory_region(addr, size, true, _RET_IP_);
}
__alias(__asan_storeN)
void __asan_storeN_noabort(unsigned long, size_t);

#undef memset
void *memset(void *addr, int c, size_t len)
{
	check_memory_region((unsigned long)addr, len, true, _RET_IP_);

	return __memset(addr, c, len);
}

#undef memmove
void *memmove(void *dest, const void *src, size_t len)
{
	check_memory_region((unsigned long)src, len, false, _RET_IP_);
	check_memory_region((unsigned long)dest, len, true, _RET_IP_);

	return __memmove(dest, src, len);
}

#undef memcpy
void *memcpy(void *dest, const void *src, size_t len)
{
	check_memory_region((unsigned long)src, len, false, _RET_IP_);
	check_memory_region((unsigned long)dest, len, true, _RET_IP_);

	return __memcpy(dest, src, len);
}

/* TODO: add malloc / free asan check */

