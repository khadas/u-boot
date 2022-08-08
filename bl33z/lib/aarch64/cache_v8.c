// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */
/*
 * Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License Agreement
 * (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 * only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission from
 * Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <mmu.h>
#include <cache_v8.h>
#include <serial.h>
#include <io.h>
#include "ram_compress.h"
#include "platform_def.h"

void set_pgtable_section(u64 *page_table, u64 index, u64 section,
						 u64 memory_type)
{
	u64 value;

	value = section | PMD_TYPE_SECT | PMD_SECT_AF;
	value |= PMD_ATTRINDX(memory_type);
	page_table[index] = value;
}

/* to activate the MMU we need to set up virtual memory */
static void mmu_setup(unsigned long ddr_size)
{
	u64 i, j;
	u64 *page_table = (u64 *)CONFIG_AML_MMU_ADDR;
	unsigned long gap_start = 1UL * (1024 * 3 + 512) << 20;	//reversed from 3.5G to 4.0g
	unsigned long gap_size = 1UL * 512 << 20;	//gap used space: 512MB

	/* Setup an identity-mapping for all spaces */
	for (i = 0; i < (PGTABLE_SIZE >> 3); i++) {
		set_pgtable_section(page_table, i, i << SECTION_SHIFT,
							MT_DEVICE_NGNRNE);
	}

	/* Setup an identity-mapping for all RAM space */
	unsigned long nTotal = readl(PREG_STICKY_REG8) & AMLOGIC_USERAM_MASK;
	nTotal = ((nTotal - 1) / 512 + 1) * 512;

	serial_puts("\naml log : DDR size is ");
	serial_put_dec(nTotal);
	serial_puts("MB\n");

	nTotal = (nTotal << 20);
	unsigned long start = 0;
	unsigned long end = start + (nTotal ? nTotal : ddr_size);

	if ((end >> (SECTION_SHIFT - 2)) & 1) {
		/* odd multiple of 128MB, align to 256M */
		end += (SECTION_SIZE >> 2);
	}
	if ((end >> (SECTION_SHIFT - 1)) & 1) {
		/* odd multiple of 256MB, align to 512M */
		end += (SECTION_SIZE >> 1);
	}

	if (nTotal > gap_start) {
		//serial_puts("DRAM have reg space(512MB)!\n");
		end = gap_start;
	}
	for (j = (start >> SECTION_SHIFT); j < (end >> SECTION_SHIFT); j++) {
		set_pgtable_section(page_table, j, j << SECTION_SHIFT, MT_NORMAL);
		if (0) {
			serial_puts("index = 0x");
			serial_put_hex(j, 32);
			serial_puts(" sec=0x");
			serial_put_hex(j << SECTION_SHIFT, 64);
			serial_puts(" attb = 0x");
			serial_put_hex(MT_NORMAL, 16);
			serial_puts("\n");
		}
	}

	if (nTotal > gap_start) {
		serial_puts("mmu skip reg space(512MB)!\n");
		end = nTotal + gap_size;
		for (; j < ((end >> SECTION_SHIFT) - 1); j++) {
			set_pgtable_section(page_table, (j + 1), (j + 1) << SECTION_SHIFT, MT_NORMAL);
			if (0) {
				serial_puts("index = 0x");
				serial_put_hex(j, 32);
				serial_puts(" sec=0x");
				serial_put_hex((j + 1) << SECTION_SHIFT, 64);
				serial_puts(" attb = 0x");
				serial_put_hex(MT_NORMAL, 16);
				serial_puts("\n");
			}
		}
	}

	/* load TTBR0 */
	__asm_set_ttbr_tcr_mair((u64)page_table, TCR_FLAGS | TCR_EL2_IPS_BITS,
							MEMORY_ATTRIBUTES);
	serial_puts("asm_set_sctlr\n");
	/* enable the mmu */
	__asm_set_sctlr(__asm_get_sctlr() | CR_M);
	serial_puts("mmu setup ok.\n");
}

/*
 * Performs a invalidation of the entire data cache at all levels
 */
void invalidate_dcache_all(void)
{
	__asm_invalidate_dcache_all();
}

/*
 * Performs a clean & invalidation of the entire data cache at all levels
 */
void flush_dcache_all(void)
{
	__asm_flush_dcache_all();
}

void enable_dcache(unsigned long ddr_size)
{
	/* The data cache is not active unless the mmu is enabled */
	if (!(__asm_get_sctlr() & CR_M)) {
		__asm_invalidate_dcache_all();
		__asm_invalidate_tlb_all();
		mmu_setup(ddr_size);
	}
	__asm_set_sctlr(__asm_get_sctlr() | CR_C);
	serial_puts("enable_dcache OK.\n");
}

void disable_dcache(void)
{
	unsigned long sctlr;

	sctlr = __asm_get_sctlr();

	if (!(sctlr & CR_C))
		return;

	__asm_set_sctlr(sctlr & ~(CR_C | CR_M));

	__asm_flush_dcache_all();
	__asm_invalidate_tlb_all();
}

void enable_icache(void)
{
	__asm_invalidate_icache_all();
	//serial_puts("currentel:0x");
	//serial_put_dec(__asm_get_currentel());
	//serial_puts(", SCTLR_EL2:0x");
	//serial_put_hex((unsigned long)__asm_get_sctlr(), 32);
	//serial_puts("\n");
	__asm_set_sctlr(__asm_get_sctlr() | CR_I);
}

void disable_icache(void)
{
	__asm_set_sctlr(__asm_get_sctlr() & ~CR_I);
	__asm_invalidate_icache_all();
}

void enable_caches(unsigned long ddr_size)
{
	enable_icache();
	enable_dcache(ddr_size);
}

void disable_caches(void)
{
	disable_icache();
	disable_dcache();
}
