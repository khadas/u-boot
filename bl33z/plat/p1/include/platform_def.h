/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2018 Amlogic, Inc. All rights reserved.
 */

#ifndef __PLATFORM_DEF_H__
#define __PLATFORM_DEF_H__

#include <arch.h>

/*******************************************************************************
 * Platform binary types for linking
 ******************************************************************************/
#define PLATFORM_LINKER_FORMAT		"elf64-littleaarch64"
#define PLATFORM_LINKER_ARCH		aarch64

#define CONFIG_DEBUG_BL33Z			1
#define CONFIG_ENABLE_CACHE			0
#define CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS

/********************************************************************************
 * For all used regs
 ********************************************************************************/
/*
 * ISA_TIMERE use SYSCTRL_TIMERE
 * WATCHDOG_CNTL use RESETCTRL_WATCHDOG_CTRL0
 * WATCHDOG_TCNT use RESETCTRL_WATCHDOG_CNT
 * WATCHDOG_RST use RESETCTRL_WATCHDOG_CLR
 */
#define ISA_TIMERE				    ((0x003b  << 2) + 0xfe010000)
#define WATCHDOG_CNTL			    ((0x0040  << 2) + 0xfe002000)
#define WATCHDOG_TCNT			    ((0x0042  << 2) + 0xfe002000)
#define WATCHDOG_RST			    ((0x0043  << 2) + 0xfe002000)

/*
 * SEC_AO_SEC_GP_CFG0 use SYSCTRL_SEC_STATUS_REG4
 * AO_SEC_GP_CFG0 use SYSCTRL_SEC_STATUS_REG4
 * AO_RTI_STATUS_REG3 use SYSCTRL_SEC_STICKY_REG1
 * AO_SEC_SD_CFG15 use SYSCTRL_SEC_STATUS_REG2
 */
#define SEC_AO_SEC_GP_CFG0          ((0x00c4  << 2) + 0xfe010000)
#define AO_SEC_GP_CFG0			    ((0x00c4  << 2) + 0xfe010000) /* DDR total */
#define AO_RTI_STATUS_REG3		    ((0x00e1  << 2) + 0xfe010000) /* reboot mode */
#define AO_SEC_SD_CFG15             ((0x00df  << 2) + 0xfe010000) /* get_rebootmode */

/* PREG_STICKY_REG8  use SYSCTRL_STICKY_REG6
 * P_AO_SEC_GP_CFG12 use SYSCTRL_DEBUG_REG6
 * P_AO_SEC_GP_CFG13 use SYSCTRL_DEBUG_REG7
 */
#define PREG_STICKY_REG8		    ((0x00b6  << 2) + 0xfe010000) /* boot status*/
#define P_AO_SEC_GP_CFG12		    ((0x0096  << 2) + 0xfe010000) /* ram compress size */
#define P_AO_SEC_GP_CFG13		    ((0x0097  << 2) + 0xfe010000) /* mdump start addrs */

/*
 * P_AO_RTI_PINMUX_REG0 use PADCTRL_PIN_MUX_REG0
 * P_AO_TIMEBASE_CNTL1 use CLKTREE_TIMEBASE_CTRL1
 * AO_UART_WFIFO use UART_A_WFIFO
 */
#define P_AO_RTI_PINMUX_REG0	    ((0x0000  << 2) + 0xfe004000)
#define P_AO_TIMEBASE_CNTL1		    ((0x0016  << 2) + 0xfe000800)
#define AO_UART_WFIFO			    ((0x0000  << 2) + 0xfe078000)

#define SYSCTRL_SEC_STATUS_REG4     ((0x00c4  << 2) + 0xfe010000)
/*
 * Not AO watch dog
 */
#define P_WATCHDOG_CNTL				(volatile uint32_t *)(WATCHDOG_CNTL)
#define P_WATCHDOG_TCNT				(volatile uint32_t *)(WATCHDOG_TCNT)
#define P_WATCHDOG_RESET			(volatile uint32_t *)(WATCHDOG_RST)
#define P_ISA_TIMERE				(volatile uint32_t *)(ISA_TIMERE)

/********************************************************************************
 * For ramdump section map
 ********************************************************************************/
/*
 *   Full Memory lay out for RAM compress:
 *
 *         MEM_EXTEND end -> +--------+
 *                           |        |
 *                           |        |
 *                           |   7    |
 *                           |        |
 *                           |        |
 *                           |        |
 *          REG_SPACE end -> +--------+ -- IGNORE_SIZE
 *                           ||||||||||
 *                           ||||6||||| <- reg space(3.5g ~ 4.0g)
 *                           ||||||||||
 *                DDR_TOP -> +--------+
 *                           |        |
 *                           |        |
 *                           |   5    |
 *                           |        |
 *                           |        |
 *                           |~~~~~~~~| <- store compressing data
 *                           |~~~~~~~~|
 *                           |~~~~~~~~|
 *                           |~~~~~~~~|
 *                           |~~~~~~~~|
 *  COMPRESSED_DATA start -> +--------+
 *                           |        |
 *                           |   4    |
 *                           |        |
 *     BL32_LOAD_ADDR end -> +--------+ -- IGNORE_SIZE
 *                           ||||||||||
 *                           ||||3||||| <- load bl31 bl32(3+48 MB)
 *                           ||||||||||
 *   BL31_LOAD_ADDR start -> +--------+
 *                           |        |
 *                           |   2    |
 *                           |        |
 *     BL2E_LOAD_ADDR end -> +--------+
 *                           ||||||||||
 *                           ||||1||||| <- load bl2x bl2e
 *                           ||||||||||
 *   BL2X_LOAD_ADDR start -> +        +
 *                           |        |
 *                           |   1    |
 *                           |        |
 *    COMPRESS_START_ADDR -> +--------+
 *
 */

/********************************************************************************
 * For ramdump section info
 ********************************************************************************/
#define CONFIG_DDR_TOTAL_SIZE		(1UL << 32)			/* get from cpu reg */

#define	CONFIG_DDR_SIZE_RANGE1		((1UL << 32) - 0x20000000)	/* RANGE 1: 0.0G - 3.5G */
#define	CONFIG_DDR_SIZE_RANGE2		(1UL << 32)			/* RANGE 2: 4.0G - 7.0G */
#define	CONFIG_DDR_SIZE_RANGE3		((1UL << 32) + 0xC0000000)	/* RANGE 3: 7.0G - end  */

#define CONFIG_SMALL_COMPRESS_ADDR	(0x0c000000)
#define CONFIG_BIG_COMPRESS_ADDR	(1UL << 32)
#if (CONFIG_DDR_TOTAL_SIZE <= CONFIG_DDR_SIZE_RANGE3)
#define CONFIG_COMPRESSED_DATA_ADDR	(CONFIG_SMALL_COMPRESS_ADDR)/* section6 start addr */
#else
#define CONFIG_COMPRESSED_DATA_ADDR	(CONFIG_BIG_COMPRESS_ADDR)	/* section6 start addr */
#endif

#define CONFIG_COMPRESS_START_ADDR	(0x00000000)		/* ddr start addr */
#define CONFIG_BL2X_LOAD_ADDR		(0x007ffe00)		/* bl2x start addr */
//#define CONFIG_BL2X_LOAD_ADDR		(0x00800000)		/* bl2x start addr */
#define CONFIG_BL2E_END_ADDR		(0x01280000)        /* bl2x end addr */
#define CONFIG_BL33Z_RSVMEM		    (0x01800000)		/* reserved memory 24MB */
#define CONFIG_BL31_LOAD_ADDR		(0x05000000)		/* bl31 start addr */
#define CONFIG_BL31_LOAD_SIZE		(0x00300000)		/* bl31 size: 3MB */
#define CONFIG_BL32_LOAD_ADDR		(CONFIG_BL31_LOAD_ADDR + CONFIG_BL31_LOAD_SIZE)
#define CONFIG_BL32_LOAD_SIZE		(0x02000000)		/* bl32 size: 32MB(rsvmem dump)*/
#define CONFIG_ACS_DDR_LOAD_ADDR	(0x05200000)
#define	CONFIG_ACS_DDR_LOAD_SIZE	(0x00000200)		/* 512 Bytes */

#define	CONFIG_DDR_REG_SPACE_ADDR	(0xE0000000)		/* ADDR: 3.5G */
#define	CONFIG_DDR_REG_SPACE_END	(1UL << 32)		    /* ADDR: 4.0G */
#define	CONFIG_DDR_EXTEND_FLAG		((0X1C0000000)UL)	/* ADDR: 7.0G */
#define CONFIG_COMPRESSED_2_DATA_ADDR	(1UL << 32)		/* ADDR: 4.0G, (4.0G ~ end)  */

//#define CONFIG_SECTION0_ADDR		(CONFIG_COMPRESSED_DATA_ADDR) /* compress begin here */
//#define CONFIG_SECTION0_SIZE		(CONFIG_DDR_TOTAL_SIZE - CONFIG_COMPRESSED_DATA_ADDR)
#define CONFIG_SECTION1_ADDR		(CONFIG_COMPRESS_START_ADDR) /* 0 */
#define CONFIG_SECTION1_SIZE		(CONFIG_SECTION2_ADDR - CONFIG_SECTION1_ADDR) /* 24MB */
#define CONFIG_SECTION2_ADDR		(CONFIG_BL33Z_RSVMEM)	/* new kernel loadaddr */
#define CONFIG_SECTION2_SIZE		(CONFIG_BL31_LOAD_ADDR - CONFIG_SECTION2_ADDR)
#define CONFIG_SECTION3_ADDR		(CONFIG_BL31_LOAD_ADDR)
#define CONFIG_SECTION3_SIZE		(CONFIG_SECTION4_ADDR - CONFIG_SECTION3_ADDR)
#define CONFIG_SECTION4_ADDR		(CONFIG_BL32_LOAD_ADDR + CONFIG_BL32_LOAD_SIZE)
#define CONFIG_SECTION4_SIZE		(CONFIG_SECTION5_ADDR - CONFIG_SECTION4_ADDR)

#define CONFIG_SECTION5_ADDR		(CONFIG_SMALL_COMPRESS_ADDR)
#define CONFIG_SECTION6_ADDR		(CONFIG_DDR_REG_SPACE_ADDR)
#define CONFIG_SECTION7_ADDR		(CONFIG_BIG_COMPRESS_ADDR)

#if (CONFIG_DDR_TOTAL_SIZE <= CONFIG_DDR_REG_SPACE_ADDR)
/* For ddr <= 3.5GB */
#define CONFIG_SECTION5_SIZE		(CONFIG_DDR_TOTAL_SIZE - CONFIG_SECTION5_ADDR)
#define CONFIG_SECTION6_SIZE		(0)
#define CONFIG_SECTION7_SIZE		(0)
#else
/* For ddr > 3.5GB */
#define CONFIG_SECTION5_SIZE		(CONFIG_SECTION6_ADDR - CONFIG_SECTION5_ADDR)
#define CONFIG_SECTION6_SIZE		(CONFIG_SECTION7_ADDR - CONFIG_SECTION6_ADDR)
#define CONFIG_SECTION7_SIZE		(CONFIG_DDR_TOTAL_SIZE - CONFIG_SECTION6_ADDR) /* +512M */
#endif

/********************************************************************************
 * For mmu calculate ddr size
 ********************************************************************************/
#define REG_SPACE_START_ADDR		(0xE0000000)	/* reg space: 3.5g ~ 4.0g */
#define REG_SPACE_RANGE_SIZE		(0x20000000)	/* reg space: 3.5g ~ 4.0g */
#define REG_SPACE_END_ADDR			(1UL << 32)
#define REG_BIG_ADDR_FLAG			(0x00000080)
#define REG_BIG_ADDR_MASE			(0xFFFFFF00)
#define REG_BIG_SIZE_MASK			(0x0000007F)

/********************************************************************************
 * For MDUMP STORE
 ********************************************************************************/
#ifndef CONFIG_MDUMP_MESSAGE_SIZE
#define CONFIG_MDUMP_MESSAGE_SIZE	65536
#endif
#define MDUMP_STORE_BLOCK			0x0100000
#define MDUMP_BLOCK_SIZE			4096
#define MDUMP_BUFFER_SIG			0x4842444dU	/* MDBH */

/********************************************************************************
 * For memory layout for runtime
 ********************************************************************************/
/*
 * Memory layout:
 * --> 0x00000000  --> code start
 * --> 0x00004000  --> code end
 * --> 0x013F0000  --> stack bottom
 * --> 0x01400000  --> stack top / mmu base
 * --> 0x01420000  --> mmu end / compress scrach buffer
 * --> 0x01480000  --> compress scrach end / first compress out buffer
 * --> 0x014E0000  --> first compress out end
 */

/* CODE space */
#define BL33Z_LOAD_ADDR				0x00400000	/* code start */
#define BL33Z_LOAD_SIZE				0x00004000	/* 16KB code */
#define BL33Z_LIMIT					(BL33Z_LOAD_ADDR + BL33Z_LOAD_SIZE) //16KB

/* STACK space */
#define PLATFORM_STACK_BOTTOM		(PLATFORM_STACK_POSI - PLATFORM_STACK_SIZE)
#define PLATFORM_STACK_SIZE			0x00010000	/* 64KB stack size */
#define PLATFORM_STACK_POSI			0x00800000	/* stack top addr */

/* MMU space */
#define CONFIG_AML_MMU_ADDR			(PLATFORM_STACK_POSI)
#define COMFIG_AML_MMU_SIZE			(128 << 10)	/* 128KB mmu */
#define BL33Z_LIMIT_MMU				(CONFIG_AML_MMU_ADDR + COMFIG_AML_MMU_SIZE)

/* TMPWORK IN space */
#define CONFIG_RAMDUMP_TMPWORK_ADDR	(CONFIG_AML_MMU_ADDR + \
							COMFIG_AML_MMU_SIZE)
#define CONFIG_RAMDUMP_TMPWORK_SIZE	(384 << 10)

/* TMPWORK OUT space */
#define CONFIG_RAMDUMP_TMPOUT_ADDR	(CONFIG_RAMDUMP_TMPWORK_ADDR + \
							CONFIG_RAMDUMP_TMPWORK_SIZE)
#define CONFIG_RAMDUMP_TMPOUT_SIZE	(384 << 10)

#endif /* __PLATFORM_DEF_H__ */
