// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <linux/string.h>
#include <asm/armv8/mmu.h>
#include <hexdump.h>

DECLARE_GLOBAL_DATA_PTR;

/* Shadow layout customization. */
#define SHADOW_BYTES_PER_BLOCK	1
#define SHADOW_BLOCKS_PER_ROW	16
#define SHADOW_BYTES_PER_ROW	(SHADOW_BLOCKS_PER_ROW * SHADOW_BYTES_PER_BLOCK)
#define SHADOW_ROWS_AROUND_ADDR	2

void __asan_init(void)
{
}

unsigned long mem_to_shadow(const void *mem_addr)
{
	unsigned long off;

	if ((unsigned long)mem_addr >= gd->phy_mem_high)
		return 0;
	if ((unsigned long)mem_addr < gd->phy_mem_low)
		return 0;

	off = (unsigned long)mem_addr - gd->phy_mem_low;
	return gd->shadow_addr + (off >> UASAN_SHADOW_SCALE_SHIFT);
}

static unsigned long shadow_to_mem(const void *mem_addr)
{
	return gd->phy_mem_low +
	       (((unsigned long)mem_addr - gd->shadow_addr) << UASAN_SHADOW_SCALE_SHIFT);
}

void uasan_poison_object(unsigned long addr, unsigned long size, unsigned tag)
{
	if (!tag || tag > 0xff)
		tag = UASAN_FREE_PAGE;

	__memset((void *)addr, size, tag);
}

/*
 * Poisons the shadow memory for 'size' bytes starting from 'addr'.
 * Memory addresses should be aligned to UASAN_SHADOW_SCALE_SIZE.
 */
static void uasan_poison_shadow(const void *address, size_t size, u8 value)
{
	void *shadow_start, *shadow_end;

	shadow_start = (void *)mem_to_shadow(address);
	shadow_end = (void *)mem_to_shadow(address + size);

	memset(shadow_start, value, shadow_end - shadow_start);
}

void uasan_unpoison_shadow(const void *address, size_t size)
{
	uasan_poison_shadow(address, size, 0);

	if (size & UASAN_SHADOW_MASK) {
		u8 *shadow = (u8 *)mem_to_shadow(address + size);
		*shadow = size & UASAN_SHADOW_MASK;
	}
}

__weak int is_register(unsigned long addr)
{
	if (addr > gd->phy_mem_high)
		return 1;
	else
		return 0;
}

/* make compiler shutup */
void __asan_handle_no_return(void)
{
}

static const void *find_first_bad_addr(const void *addr, size_t size)
{
	u8 shadow_val = *(u8 *)mem_to_shadow(addr);
	const void *first_bad_addr = addr;

	while (!shadow_val && first_bad_addr < addr + size) {
		first_bad_addr += UASAN_SHADOW_SCALE_SIZE;
		shadow_val = *(u8 *)mem_to_shadow(first_bad_addr);
	}
	return first_bad_addr;
}

static const char *get_shadow_bug_type(struct uasan_report_info *info)
{
	const char *bug_type = "unknown-crash";
	u8 *shadow_addr;

	info->first_bad_addr = find_first_bad_addr(info->access_addr,
						info->access_size);

	shadow_addr = (u8 *)mem_to_shadow(info->first_bad_addr);

	/*
	 * If shadow byte value is in [0, UASAN_SHADOW_SCALE_SIZE) we can look
	 * at the next shadow byte to determine the type of the bad access.
	 */
	if (*shadow_addr > 0 && *shadow_addr <= UASAN_SHADOW_SCALE_SIZE - 1)
		shadow_addr++;

	switch (*shadow_addr) {
	case 0 ... UASAN_SHADOW_SCALE_SIZE - 1:
		/*
		 * In theory it's still possible to see these shadow values
		 * due to a data race in the kernel code.
		 */
		bug_type = "out-of-bounds";
		break;
	case UASAN_PAGE_REDZONE:
	case UASAN_MALLOC_REDZONE:
		bug_type = "malloc-out-of-bounds";
		break;
	case UASAN_GLOBAL_REDZONE:
		bug_type = "global-out-of-bounds";
		break;
	case UASAN_STACK_LEFT:
	case UASAN_STACK_MID:
	case UASAN_STACK_RIGHT:
	case UASAN_STACK_PARTIAL:
		bug_type = "stack-out-of-bounds";
		break;
	case UASAN_FREE_PAGE:
	case UASAN_MALLOC_FREE:
		bug_type = "use-after-free";
		break;
	case UASAN_USE_AFTER_SCOPE:
		bug_type = "use-after-scope";
		break;
	case UASAN_ALLOCA_LEFT:
	case UASAN_ALLOCA_RIGHT:
		bug_type = "alloca-out-of-bounds";
		break;
	}

	return bug_type;
}

static bool addr_has_shadow(struct uasan_report_info *info)
{
	return ((unsigned long)info->access_addr >= gd->phy_mem_low);
}

static const char *get_wild_bug_type(struct uasan_report_info *info)
{
	const char *bug_type = "unknown-crash";

	if ((unsigned long)info->access_addr < PAGE_SIZE)
		bug_type = "null-ptr-deref";

	return bug_type;
}

static const char *get_bug_type(struct uasan_report_info *info)
{
	if (addr_has_shadow(info))
		return get_shadow_bug_type(info);
	return get_wild_bug_type(info);
}

static void print_error_description(struct uasan_report_info *info)
{
	const char *bug_type = get_bug_type(info);
	const char *sym = NULL;
	unsigned long base, end;

	sym = symbol_lookup(info->ip - gd->relocaddr, &base, &end);

	if (sym)
		printf("BUG: UASAN: %s in %s\n",
			bug_type, sym);
	else
		printf("BUG: UASAN: %s in unknown function:%lx\n",
			bug_type, info->ip);

	printf("%s of size %zu at addr %p\n",
		info->is_write ? "Write" : "Read", info->access_size,
		info->access_addr);
}

static void print_address_description(void *addr)
{
	/* TODO: for malloc */
}

static bool row_is_guilty(const void *row, const void *guilty)
{
	return (row <= guilty) && (guilty < row + SHADOW_BYTES_PER_ROW);
}

static int shadow_pointer_offset(const void *row, const void *shadow)
{
	/* The length of ">ff00ff00ff00ff00: " is
	 *    3 + (BITS_PER_LONG / 8) * 2 chars.
	 */
	return 3 + (BITS_PER_LONG / 8) * 2 + (shadow - row) * 2 +
		(shadow - row) / SHADOW_BYTES_PER_BLOCK + 1;
}

static void print_shadow_for_address(const void *addr)
{
	int i;
	const void *shadow = (const void *)mem_to_shadow(addr);
	const void *shadow_row;

	shadow_row = (void *)round_down((unsigned long)shadow,
					SHADOW_BYTES_PER_ROW)
		- SHADOW_ROWS_AROUND_ADDR * SHADOW_BYTES_PER_ROW;

	printf("Memory state around the buggy address: %p\n", shadow);

	for (i = -SHADOW_ROWS_AROUND_ADDR; i <= SHADOW_ROWS_AROUND_ADDR; i++) {
		const void *kaddr = (const void *)shadow_to_mem(shadow_row);
		char buffer[4 + (BITS_PER_LONG / 8) * 2];
		char shadow_buf[SHADOW_BYTES_PER_ROW];

		snprintf(buffer, sizeof(buffer),
			(i == 0) ? ">%p: " : " %p: ", kaddr);
		/*
		 * We should not pass a shadow pointer to generic
		 * function, because generic functions may try to
		 * access uasan mapping for the passed address.
		 */
		memcpy(shadow_buf, shadow_row, SHADOW_BYTES_PER_ROW);
		print_hex_dump(buffer,
			DUMP_PREFIX_NONE, SHADOW_BYTES_PER_ROW, 1,
			shadow_buf, SHADOW_BYTES_PER_ROW, 0);

		if (row_is_guilty(shadow_row, shadow))
			printf("%*c\n",
				shadow_pointer_offset(shadow_row, shadow),
				'^');

		shadow_row += SHADOW_BYTES_PER_ROW;
	}
}

void uasan_report(unsigned long addr, unsigned long size,
		  int write, unsigned long ip)
{
	struct uasan_report_info info = {};

	info.access_addr = (void *)addr;
	info.access_size = size;
	info.is_write = write;
	info.ip = ip;

	gd->in_asan_report = 1;	/* avoid recursion */
	printf("==================================================================\n");
	print_error_description(&info);
	if (!addr_has_shadow(&info)) {
		stack_dump();
	} else {
		print_address_description((void *)info.access_addr);
		printf("\n");
		print_shadow_for_address(info.first_bad_addr);
		stack_dump();
	}
	printf("==================================================================\n");
	gd->in_asan_report = 0;
}

static __always_inline bool memory_is_poisoned_1(unsigned long addr)
{
	s8 *shadow_addr = (s8 *)mem_to_shadow((void *)addr);
	s8 shadow_value;

	if (!shadow_addr) /* ignore access for memory not in uasan range */
		return false;

	shadow_value = *shadow_addr;
	if (unlikely(shadow_value)) {
		s8 last_accessible_byte = addr & UASAN_SHADOW_MASK;

		return unlikely(last_accessible_byte >= shadow_value);
	}

	return false;
}

static __always_inline bool memory_is_poisoned_2(unsigned long addr)
{
	u16 *shadow_addr = (u16 *)mem_to_shadow((void *)addr);

	if (!shadow_addr)
		return false;

	if (unlikely(*shadow_addr)) {
		if (memory_is_poisoned_1(addr + 1))
			return true;

		/*
		 * If single shadow byte covers 2-byte access, we don't
		 * need to do anything more. Otherwise, test the first
		 * shadow byte.
		 */
		if (likely(((addr + 1) & UASAN_SHADOW_MASK) != 0))
			return false;

		return unlikely(*(u8 *)shadow_addr);
	}

	return false;
}

static __always_inline bool memory_is_poisoned_4(unsigned long addr)
{
	u16 *shadow_addr = (u16 *)mem_to_shadow((void *)addr);

	if (!shadow_addr)
		return false;

	if (unlikely(*shadow_addr)) {
		if (memory_is_poisoned_1(addr + 3))
			return true;

		/*
		 * If single shadow byte covers 4-byte access, we don't
		 * need to do anything more. Otherwise, test the first
		 * shadow byte.
		 */
		if (likely(((addr + 3) & UASAN_SHADOW_MASK) >= 3))
			return false;

		return unlikely(*(u8 *)shadow_addr);
	}

	return false;
}

static __always_inline bool memory_is_poisoned_8(unsigned long addr)
{
	u16 *shadow_addr = (u16 *)mem_to_shadow((void *)addr);

	if (!shadow_addr)
		return false;

	if (unlikely(*shadow_addr)) {
		if (memory_is_poisoned_1(addr + 7))
			return true;

		/*
		 * If single shadow byte covers 8-byte access, we don't
		 * need to do anything more. Otherwise, test the first
		 * shadow byte.
		 */
		if (likely(IS_ALIGNED(addr, UASAN_SHADOW_SCALE_SIZE)))
			return false;

		return unlikely(*(u8 *)shadow_addr);
	}

	return false;
}

static __always_inline bool memory_is_poisoned_16(unsigned long addr)
{
	u32 *shadow_addr = (u32 *)mem_to_shadow((void *)addr);

	if (!shadow_addr)
		return false;

	if (unlikely(*shadow_addr)) {
		u16 shadow_first_bytes = *(u16 *)shadow_addr;

		if (unlikely(shadow_first_bytes))
			return true;

		/*
		 * If two shadow bytes covers 16-byte access, we don't
		 * need to do anything more. Otherwise, test the last
		 * shadow byte.
		 */
		if (likely(IS_ALIGNED(addr, UASAN_SHADOW_SCALE_SIZE)))
			return false;

		return memory_is_poisoned_1(addr + 15);
	}

	return false;
}

static __always_inline unsigned long bytes_is_zero(const u8 *start,
					size_t size)
{
	while (size) {
		if (unlikely(*start))
			return (unsigned long)start;
		start++;
		size--;
	}

	return 0;
}

static __always_inline unsigned long memory_is_zero(const void *start,
						const void *end)
{
	unsigned int words;
	unsigned long ret;
	unsigned int prefix = (unsigned long)start % 8;

	if (end - start <= 16)
		return bytes_is_zero(start, end - start);

	if (prefix) {
		prefix = 8 - prefix;
		ret = bytes_is_zero(start, prefix);
		if (unlikely(ret))
			return ret;
		start += prefix;
	}

	words = (end - start) / 8;
	while (words) {
		if (unlikely(*(u64 *)start))
			return bytes_is_zero(start, 8);
		start += 8;
		words--;
	}

	return bytes_is_zero(start, (end - start) % 8);
}

static __always_inline bool memory_is_poisoned_n(unsigned long addr,
						size_t size)
{
	unsigned long ret;
	const void *start, *end;

	start = (const void *)mem_to_shadow((const void *)addr);
	end   = (const void *)mem_to_shadow((const void *)addr + size - 1);
	if (!start || !end)
		return false;

	ret = memory_is_zero(start, end + 1);

	if (unlikely(ret)) {
		unsigned long last_byte = addr + size - 1;
		s8 *last_shadow = (s8 *)mem_to_shadow((const void *)last_byte);

		if (unlikely(ret != (unsigned long)last_shadow ||
			((long)(last_byte & UASAN_SHADOW_MASK) >= *last_shadow)))
			return true;
	}
	return false;
}

static __always_inline bool memory_is_poisoned(unsigned long addr, size_t size)
{
	if (__builtin_constant_p(size)) {
		switch (size) {
		case 1:
			return memory_is_poisoned_1(addr);
		case 2:
			return memory_is_poisoned_2(addr);
		case 4:
			return memory_is_poisoned_4(addr);
		case 8:
			return memory_is_poisoned_8(addr);
		case 16:
			return memory_is_poisoned_16(addr);
		default:
			break;
		}
	}

	return memory_is_poisoned_n(addr, size);
}

void check_memory_region_inline(unsigned long addr,
				size_t size, bool write,
				unsigned long ret_ip)
{
	if (unlikely(size == 0))
		return;

	if (unlikely(!gd->uasan_enabled))
		return;

	if (gd->in_asan_report)
		return;

	if (addr >= gd->phy_mem_high && !is_register(addr)) {
		uasan_report(addr, size, write, ret_ip);
		return;
	}

	if (likely(!memory_is_poisoned(addr, size)))
		return;

	uasan_report(addr, size, write, ret_ip);
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

static void register_global(struct uasan_global *global)
{
	size_t aligned_size = round_up(global->size, UASAN_SHADOW_SCALE_SIZE);
	const void *rel_beg = global->beg + gd->relocaddr;

	debug("  global beg:%p, size:%ld, rsize:%ld, asize:%ld, rel_beg:%p\n",
		global->beg, global->size, global->size_with_redzone,
		aligned_size, rel_beg);
	uasan_unpoison_shadow(rel_beg, global->size);

	uasan_poison_shadow(rel_beg + aligned_size,
		global->size_with_redzone - aligned_size,
		UASAN_GLOBAL_REDZONE);
}

void __asan_register_globals(struct uasan_global *globals, size_t size)
{
	int i;

	debug("  global:%p, size:%ld\n", globals, size);
	for (i = 0; i < size; i++)
		register_global(&globals[i]);
}

/* TODO: add malloc / free asan check */

void uasan_alloc(void *ptr, unsigned long size)
{
	const void *addr;

	if (!ptr || !size)
		return;

	/* chunk  head */
	addr = ptr;
	uasan_unpoison_shadow(addr, 2 * sizeof(void *));

	/* left redzone */
	addr += 2 * sizeof(void *);
	uasan_poison_shadow(addr, UASAN_ALLOCA_REDZONE_SIZE,
			    UASAN_MALLOC_REDZONE);

	/* object */
	addr += UASAN_ALLOCA_REDZONE_SIZE;
	uasan_unpoison_shadow(addr, size);

	/* right redzone */
	addr += size;
	addr = (void  *)round_up((unsigned long)addr, UASAN_SHADOW_SCALE_SIZE);
	uasan_poison_shadow(addr, UASAN_ALLOCA_REDZONE_SIZE,
			    UASAN_MALLOC_REDZONE);
}

void uasan_free(void *ptr, unsigned long size)
{
	if (!ptr || !size)
		return;

	/* chunk header + fp/bk prt */
	uasan_unpoison_shadow(ptr, 4 * sizeof(void *));

	/* end of this object */
	uasan_poison_shadow(ptr + 4 * sizeof(void *),
			    size - 4 * sizeof(void *),
			    UASAN_MALLOC_FREE);
}

void __asan_unpoison_stack_memory(const void *addr, size_t size)
{
	uasan_unpoison_shadow(addr, size);
}

void __asan_poison_stack_memory(const void *addr, size_t size)
{
	uasan_poison_shadow(addr, round_up(size, UASAN_SHADOW_SCALE_SIZE),
			    UASAN_USE_AFTER_SCOPE);
}

/* Emitted by compiler to poison alloca()ed objects. */
void __asan_alloca_poison(unsigned long addr, size_t size)
{
	size_t rounded_up_size = round_up(size, UASAN_SHADOW_SCALE_SIZE);
	size_t padding_size = round_up(size, UASAN_ALLOCA_REDZONE_SIZE) -
			rounded_up_size;
	size_t rounded_down_size = round_down(size, UASAN_SHADOW_SCALE_SIZE);

	const void *left_redzone = (const void *)(addr -
			UASAN_ALLOCA_REDZONE_SIZE);
	const void *right_redzone = (const void *)(addr + rounded_up_size);

	uasan_unpoison_shadow((const void *)(addr + rounded_down_size),
			      size - rounded_down_size);
	uasan_poison_shadow(left_redzone, UASAN_ALLOCA_REDZONE_SIZE,
			UASAN_ALLOCA_LEFT);
	uasan_poison_shadow(right_redzone,
			padding_size + UASAN_ALLOCA_REDZONE_SIZE,
			UASAN_ALLOCA_RIGHT);
}

/* Emitted by compiler to unpoison alloca()ed areas when the stack unwinds. */
void __asan_allocas_unpoison(const void *stack_top, const void *stack_bottom)
{
	if (unlikely(!stack_top || stack_top > stack_bottom))
		return;

	uasan_unpoison_shadow(stack_top, stack_bottom - stack_top);
}
