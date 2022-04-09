
/*
 * plat/txl/include/cache.h
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
*/

#ifndef __BL33Z_CACHE_H_
#define __BL33Z_CACHE_H_

unsigned long __asm_get_sctlr(void);
void __asm_set_sctlr(unsigned long);
void __asm_flush_dcache_all(void);
void __asm_invalidate_dcache_all(void);
void __asm_invalidate_icache_all(void);
void __asm_flush_dcache_range(unsigned long, unsigned long);
void __asm_set_ttbr_tcr_mair(unsigned long, unsigned long, unsigned long);
void __asm_invalidate_tlb_all(void);

void enable_icache(void);
void enable_dcache(unsigned long ddr_size);
void disable_icache(void);
void disable_dcache(void);
void enable_caches(unsigned long ddr_size);
void disable_caches(void);
#endif /*__BL33Z_CACHE_H_*/
