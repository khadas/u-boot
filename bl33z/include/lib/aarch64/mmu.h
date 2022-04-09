/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2018 Amlogic, Inc. All rights reserved.
 */

#ifndef _ASM_ARMV8_MMU_H_
#define _ASM_ARMV8_MMU_H_

#define BIT(nr)			(1 << nr)
typedef signed long long s64;
typedef unsigned long long u64;

#ifdef __ASSEMBLY__
#define _AC(X, Y)	X
#else
#define _AC(X, Y)	(X##Y)
#endif

#define UL(x)		_AC(x, UL)

/***************************************************************/
/*
 * The following definitions are related each other, shoud be calculated specifically.
 */
#define VA_BITS			(42)	/* 36 bits virtual address == 64G*/

/* PAGE_SHIFT determines the page size */
//#undef  PAGE_SIZE
//#define PAGE_SHIFT	16
//#define PAGE_SIZE		BIT(PAGE_SHIFT)
//#define PAGE_MASK		(~(PAGE_SIZE - 1))

/*
 * section address mask and size definitions.
 */
#define SECTION_SHIFT		29
#define SECTION_SIZE		(UL(1) << SECTION_SHIFT)
#define SECTION_MASK		(~(SECTION_SIZE - 1))
/***************************************************************/

/*
 * Memory types
 */
#define MT_DEVICE_NGNRNE	0
#define MT_DEVICE_NGNRE		1
#define MT_DEVICE_GRE		2
#define MT_NORMAL_NC		3
#define MT_NORMAL		4

#define MEMORY_ATTRIBUTES	((0x00 << (MT_DEVICE_NGNRNE * 8)) |	\
				(0x04 << (MT_DEVICE_NGNRE * 8)) |		\
				(0x0c << (MT_DEVICE_GRE * 8)) |		\
				(0x44 << (MT_NORMAL_NC * 8)) |		\
				(UL(0xff) << (MT_NORMAL * 8)))

/*
 * Hardware page table definitions.
 *
 * Level 2 descriptor (PMD).
 */
#define PMD_TYPE_MASK		(3 << 0)
#define PMD_TYPE_FAULT		(0 << 0)
#define PMD_TYPE_TABLE		(3 << 0)
#define PMD_TYPE_SECT		BIT(0)

/*
 * Section
 */
#define PMD_SECT_S			(3 << 8)
#define PMD_SECT_AF			BIT(10)
#define PMD_SECT_NG			BIT(11)
#define PMD_SECT_PXN		(UL(1) << 53)
#define PMD_SECT_UXN		(UL(1) << 54)

/*
 * AttrIndx[2:0]
 */
#define PMD_ATTRINDX(t)		((t) << 2)
#define PMD_ATTRINDX_MASK	(7 << 2)

/*
 * TCR flags.
 */
#define TCR_T0SZ(x)		((64 - (x)) << 0)
#define TCR_IRGN_NC		(0 << 8)
#define TCR_IRGN_WBWA		BIT(8)
#define TCR_IRGN_WT		(2 << 8)
#define TCR_IRGN_WBNWA		(3 << 8)
#define TCR_IRGN_MASK		(3 << 8)
#define TCR_ORGN_NC		(0 << 10)
#define TCR_ORGN_WBWA		BIT(10)
#define TCR_ORGN_WT		(2 << 10)
#define TCR_ORGN_WBNWA		(3 << 10)
#define TCR_ORGN_MASK		(3 << 10)
#define TCR_SHARED_NON		(0 << 12)
#define TCR_SHARED_OUTER	BIT(12)
#define TCR_SHARED_INNER	(2 << 12)
#define TCR_TG0_4K		(0 << 14)
#define TCR_TG0_64K		BIT(14)
#define TCR_TG0_16K		(2 << 14)
#define TCR_EL1_IPS_BITS	(UL(0) << 32)	/* 32 bits physical address */
#define TCR_EL2_IPS_BITS	(3 << 16)	/* 42 bits physical address */
#define TCR_EL3_IPS_BITS	(3 << 16)	/* 42 bits physical address */

/* PTWs cacheable, inner/outer WBWA and non-shareable */
#define TCR_FLAGS		(TCR_TG0_64K |		\
				TCR_SHARED_NON |	\
				TCR_ORGN_WBWA |		\
				TCR_IRGN_WBWA |		\
				TCR_T0SZ(VA_BITS))


/*
 * SCTLR_EL1/SCTLR_EL2/SCTLR_EL3 bits definitions
 */
#define CR_M		BIT(0)	/* MMU enable			*/
#define CR_A		BIT(1)	/* Alignment abort enable	*/
#define CR_C		BIT(2)	/* Dcache enable		*/
#define CR_SA		BIT(3)	/* Stack Alignment Check Enable	*/
#define CR_I		BIT(12)	/* Icache enable		*/
#define CR_WXN		BIT(19)	/* Write Permision Imply XN	*/
#define CR_EE		BIT(25)	/* Exception (Big) Endian	*/

#define PGTABLE_SIZE	(0x10000)
#endif /* _ASM_ARMV8_MMU_H_ */
