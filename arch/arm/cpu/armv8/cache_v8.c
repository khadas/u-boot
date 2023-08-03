/*
 * (C) Copyright 2013
 * David Feng <fenghua@phytium.com.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/system.h>
#include <asm/armv8/mmu.h>

DECLARE_GLOBAL_DATA_PTR;

#define DDR_SKIP_ZERO_ADDR_START_64KB_MAP	0x10000

#ifndef CONFIG_SYS_DCACHE_OFF

/*
 *  With 4k page granule, a virtual address is split into 4 lookup parts
 *  spanning 9 bits each:
 *
 *    _______________________________________________
 *   |       |       |       |       |       |       |
 *   |   0   |  Lv0  |  Lv1  |  Lv2  |  Lv3  |  off  |
 *   |_______|_______|_______|_______|_______|_______|
 *     63-48   47-39   38-30   29-21   20-12   11-00
 *
 *             mask        page size
 *
 *    Lv0: FF8000000000       --
 *    Lv1:   7FC0000000       1G
 *    Lv2:     3FE00000       2M
 *    Lv3:       1FF000       4K
 *    off:          FFF
 */

u64 get_tcr(int el, u64 *pips, u64 *pva_bits)
{
	u64 max_addr = 0;
	u64 ips, va_bits;
	u64 tcr;
	int i;

	/* Find the largest address we need to support */
	for (i = 0; mem_map[i].size || mem_map[i].attrs; i++)
		max_addr = max(max_addr, mem_map[i].virt + mem_map[i].size);

	/* Calculate the maximum physical (and thus virtual) address */
	if (max_addr > (1ULL << 44)) {
		ips = 5;
		va_bits = 48;
	} else  if (max_addr > (1ULL << 42)) {
		ips = 4;
		va_bits = 44;
	} else  if (max_addr > (1ULL << 40)) {
		ips = 3;
		va_bits = 42;
	} else  if (max_addr > (1ULL << 36)) {
		ips = 2;
		va_bits = 40;
	} else  if (max_addr > (1ULL << 32)) {
		ips = 1;
		va_bits = 36;
	} else {
		ips = 0;
		va_bits = 32;
	}

	if (el == 1) {
		tcr = TCR_EL1_RSVD | (ips << 32) | TCR_EPD1_DISABLE;
	} else if (el == 2) {
		tcr = TCR_EL2_RSVD | (ips << 16);
	} else {
		tcr = TCR_EL3_RSVD | (ips << 16);
	}

	/* PTWs cacheable, inner/outer WBWA and inner shareable */
	tcr |= TCR_TG0_4K | TCR_SHARED_INNER | TCR_ORGN_WBWA | TCR_IRGN_WBWA;
	tcr |= TCR_T0SZ(va_bits);

	if (pips)
		*pips = ips;
	if (pva_bits)
		*pva_bits = va_bits;

	return tcr;
}

#define MAX_PTE_ENTRIES 512

static int pte_type(u64 *pte)
{
	return *pte & PTE_TYPE_MASK;
}

/* Returns the LSB number for a PTE on level <level> */
static int level2shift(int level)
{
	/* Page is 12 bits wide, every level translates 9 bits */
	return (12 + 9 * (3 - level));
}

static u64 *find_pte(u64 addr, int level)
{
	int start_level = 0;
	u64 *pte;
	u64 idx;
	u64 va_bits;
	int i;

	debug("addr=%llx level=%d\n", addr, level);

	get_tcr(0, NULL, &va_bits);
	if (va_bits < 39)
		start_level = 1;

	if (level < start_level)
		return NULL;

	/* Walk through all page table levels to find our PTE */
	pte = (u64 *)gd->arch.tlb_addr;
	for (i = start_level; i < 4; i++) {
		idx = (addr >> level2shift(i)) & 0x1FF;
		pte += idx;
		debug("idx=%llx PTE %p at level %d: %llx\n", idx, pte, i, *pte);

		/* Found it */
		if (i == level)
			return pte;
		/* PTE is no table (either invalid or block), can't traverse */
		if (pte_type(pte) != PTE_TYPE_TABLE)
			return NULL;
		/* Off to the next level */
		pte = (u64 *)(*pte & 0x0000fffffffff000ULL);
	}

	/* Should never reach here */
	return NULL;
}

/* Returns and creates a new full table (512 entries) */
static u64 *create_table(void)
{
	u64 *new_table = (u64 *)gd->arch.tlb_fillptr;
	u64 pt_len = MAX_PTE_ENTRIES * sizeof(u64);

	/* Allocate MAX_PTE_ENTRIES pte entries */
	gd->arch.tlb_fillptr += pt_len;

	if (gd->arch.tlb_fillptr - gd->arch.tlb_addr > gd->arch.tlb_size)
		panic("Insufficient RAM for page table: 0x%lx > 0x%lx. "
		      "Please increase the size in get_page_table_size()",
			gd->arch.tlb_fillptr - gd->arch.tlb_addr,
			gd->arch.tlb_size);

	/* Mark all entries as invalid */
	memset(new_table, 0, pt_len);

	return new_table;
}

static void set_pte_table(u64 *pte, u64 *table)
{
	/* Point *pte to the new table */
	debug("Setting %p to addr=%p\n", pte, table);
	*pte = PTE_TYPE_TABLE | (ulong)table;
}

/* Splits a block PTE into table with subpages spanning the old block */
static void split_block(u64 *pte, int level)
{
	u64 old_pte = *pte;
	u64 *new_table;
	u64 i = 0;
	/* level describes the parent level, we need the child ones */
	int levelshift = level2shift(level + 1);

	if (pte_type(pte) != PTE_TYPE_BLOCK)
		panic("PTE %p (%llx) is not a block. Some driver code wants to "
		      "modify dcache settings for an range not covered in "
		      "mem_map.", pte, old_pte);

	new_table = create_table();
	debug("Splitting pte %p (%llx) into %p\n", pte, old_pte, new_table);

	for (i = 0; i < MAX_PTE_ENTRIES; i++) {
		new_table[i] = old_pte | (i << levelshift);

		/* Level 3 block PTEs have the table type */
		if ((level + 1) == 3)
			new_table[i] |= PTE_TYPE_TABLE;

		debug("Setting new_table[%lld] = %llx\n", i, new_table[i]);
	}

	/* Set the new table into effect */
	set_pte_table(pte, new_table);
}


/* Add one mm_region map entry to the page tables */
static void add_map(struct mm_region *map)
{
	u64 *pte;
	u64 virt = map->virt;
	u64 phys = map->phys;
	u64 size = map->size;
	u64 attrs = map->attrs | PTE_TYPE_BLOCK | PTE_BLOCK_AF;
	u64 blocksize;
	int level;
	u64 *new_table;

	if ((virt == 0) && (phys == 0)) {
		virt += DDR_SKIP_ZERO_ADDR_START_64KB_MAP;
		phys += DDR_SKIP_ZERO_ADDR_START_64KB_MAP;
		size -= DDR_SKIP_ZERO_ADDR_START_64KB_MAP;
	}
	while (size) {
		pte = find_pte(virt, 0);
		if (pte && (pte_type(pte) == PTE_TYPE_FAULT)) {
			debug("Creating table for virt 0x%llx\n", virt);
			new_table = create_table();
			set_pte_table(pte, new_table);
		}

		for (level = 1; level < 4; level++) {
			pte = find_pte(virt, level);
			if (!pte)
				panic("pte not found\n");

			blocksize = 1ULL << level2shift(level);
			debug("Checking if pte fits for virt=%llx size=%llx blocksize=%llx\n",
			      virt, size, blocksize);
			if (size >= blocksize && !(virt & (blocksize - 1))) {
				/* Page fits, create block PTE */
				debug("Setting PTE %p to block virt=%llx\n",
				      pte, virt);
				if (level == 3)
					*pte = phys | attrs | PTE_TYPE_PAGE;
				else
					*pte = phys | attrs;
				virt += blocksize;
				phys += blocksize;
				size -= blocksize;
				break;
			} else if (pte_type(pte) == PTE_TYPE_FAULT) {
				/* Page doesn't fit, create subpages */
				debug("Creating subtable for virt 0x%llx blksize=%llx\n",
				      virt, blocksize);
				new_table = create_table();
				set_pte_table(pte, new_table);
			} else if (pte_type(pte) == PTE_TYPE_BLOCK) {
				debug("Split block into subtable for virt 0x%llx blksize=0x%llx\n",
				      virt, blocksize);
				split_block(pte, level);
			}
		}
	}
}

void setup_pgtables(void)
{
	int i;

	if (!gd->arch.tlb_fillptr || !gd->arch.tlb_addr)
		panic("Page table pointer not setup.");

	/*
	 * Allocate the first level we're on with invalidate entries.
	 * If the starting level is 0 (va_bits >= 39), then this is our
	 * Lv0 page table, otherwise it's the entry Lv1 page table.
	 */
	create_table();

	/* Now add all MMU table entries one after another to the table */
	for (i = 0; mem_map[i].size || mem_map[i].attrs; i++)
		add_map(&mem_map[i]);
}

static void setup_all_pgtables(void)
{
	u64 tlb_addr = gd->arch.tlb_addr;
	u64 tlb_size = gd->arch.tlb_size;

	/* Reset the fill ptr */
	gd->arch.tlb_fillptr = tlb_addr;

	/* Create normal system page tables */
	setup_pgtables();

	/* Create emergency page tables */
	gd->arch.tlb_size -= (uintptr_t)gd->arch.tlb_fillptr -
			     (uintptr_t)gd->arch.tlb_addr;
	gd->arch.tlb_addr = gd->arch.tlb_fillptr;
	setup_pgtables();
	gd->arch.tlb_emerg = gd->arch.tlb_addr;
	gd->arch.tlb_addr = tlb_addr;
	gd->arch.tlb_size = tlb_size;
}

/* to activate the MMU we need to set up virtual memory */
void mmu_setup(void)
{

	int el;

	/* Set up page tables only once */
	if (!gd->arch.tlb_fillptr) {
#if defined(CONFIG_SYS_MEM_TOP_HIDE)
		bd_mem_map[0].size =  gd->bd->bi_dram[0].start + gd->bd->bi_dram[0].size + CONFIG_SYS_MEM_TOP_HIDE;
#else
		bd_mem_map[0].size =  gd->bd->bi_dram[0].start + gd->bd->bi_dram[0].size;
#endif
		setup_all_pgtables();
	}

	el = current_el();
	set_ttbr_tcr_mair(el, gd->arch.tlb_addr, get_tcr(el, NULL, NULL),
			  MEMORY_ATTRIBUTES);

	/* enable the mmu */
	set_sctlr(get_sctlr() | CR_M);
}

/*
 * Performs a invalidation of the entire data cache at all levels
 */
void invalidate_dcache_all(void)
{
	__asm_invalidate_dcache_all();
}

void __weak flush_l3_cache(void)
{
}

/*
 * Performs a clean & invalidation of the entire data cache at all levels
 */
void flush_dcache_all(void)
{
	__asm_flush_dcache_all();
	flush_l3_cache();
}

/*
 * Invalidates range in all levels of D-cache/unified cache
 */
void invalidate_dcache_range(unsigned long start, unsigned long stop)
{
	__asm_flush_dcache_range(start, stop);
}

/*
 * Flush range(clean & invalidate) from all levels of D-cache/unified cache
 */
void flush_dcache_range(unsigned long start, unsigned long stop)
{
	__asm_flush_dcache_range(start, stop);
}

void dcache_enable(void)
{
	/* The data cache is not active unless the mmu is enabled */
	if (!(get_sctlr() & CR_M)) {
		invalidate_dcache_all();
		__asm_invalidate_tlb_all();
		mmu_setup();
	}

	set_sctlr(get_sctlr() | CR_C);
	gd->flags |= GD_FLG_CACHE_EN;
}

void dcache_disable(void)
{
	uint32_t sctlr;

	sctlr = get_sctlr();

	/* if cache isn't enabled no need to disable */
	if (!(sctlr & CR_C))
		return;

	set_sctlr(sctlr & ~(CR_C|CR_M));
	gd->flags &= ~GD_FLG_CACHE_EN;

	__asm_flush_dcache_all();
	flush_l3_cache();
	__asm_invalidate_tlb_all();
}

int dcache_status(void)
{
	return (get_sctlr() & CR_C) != 0;
}

#else	/* CONFIG_SYS_DCACHE_OFF */

void invalidate_dcache_all(void)
{
}

void flush_dcache_all(void)
{
}

void invalidate_dcache_range(unsigned long start, unsigned long stop)
{
}

void flush_dcache_range(unsigned long start, unsigned long stop)
{
}

void dcache_enable(void)
{
}

void dcache_disable(void)
{
}

int dcache_status(void)
{
	return 0;
}

#endif	/* CONFIG_SYS_DCACHE_OFF */

#ifndef CONFIG_SYS_ICACHE_OFF

void icache_enable(void)
{
	__asm_invalidate_icache_all();
	set_sctlr(get_sctlr() | CR_I);
}

void icache_disable(void)
{
	set_sctlr(get_sctlr() & ~CR_I);
}

int icache_status(void)
{
	return (get_sctlr() & CR_I) != 0;
}

void invalidate_icache_all(void)
{
	__asm_invalidate_icache_all();
}

#else	/* CONFIG_SYS_ICACHE_OFF */

void icache_enable(void)
{
}

void icache_disable(void)
{
}

int icache_status(void)
{
	return 0;
}

void invalidate_icache_all(void)
{
}

#endif	/* CONFIG_SYS_ICACHE_OFF */

/*
 * Enable dCache & iCache, whether cache is actually enabled
 * depend on CONFIG_SYS_DCACHE_OFF and CONFIG_SYS_ICACHE_OFF
 */
void __weak enable_caches(void)
{
	icache_enable();
	dcache_enable();
}

/*
 * Flush range from all levels of d-cache/unified-cache
 */
void flush_cache(unsigned long start, unsigned long size)
{
	flush_dcache_range(start, start + size);
}
