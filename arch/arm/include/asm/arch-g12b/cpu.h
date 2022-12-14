/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _CPU_H
#define _CPU_H
/* #include <config.h> */
/* #include <asm/plat-cpu.h> */
/* #include <asm/arch/ddr.h> */

#if 0
#define CONFIG_AML_MESON 1
#define CONFIG_AML_MESON_GX 1
#define CONFIG_AML_MESON_G12A 1
#endif

/*enable viu probe*/
#if 0
#define CONFIG_CMD_VIU_PROBE    1
#endif

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(96*1024*1024)

/* for uboot environment use */
/* #define CONFIG_DDR_SIZE					1024 */

/* dram */
#define PHYS_SDRAM_1_BASE					0x00000000UL
#define PHYS_SDRAM_1_SIZE					(1024 << 20)
#define CONFIG_SYS_SDRAM_BASE				PHYS_SDRAM_1_BASE
#define CONFIG_SYS_INIT_SP_ADDR				(0x08000000)

#define CONFIG_SYS_TEXT_BASE		0x01000000 /*16MB rsv*/
#define CONFIG_SYS_LOAD_ADDR		(PHYS_SDRAM_1_BASE + CONFIG_SYS_TEXT_BASE)

/* console/print buffer */
#define CONFIG_SYS_CBSIZE		512
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

/* Generic Timer Definitions */
#define COUNTER_FREQUENCY		(0x1800000)	/* 24MHz */

/* support board late init */
#define CONFIG_BOARD_LATE_INIT
/* use "hush" command parser */
#if 0
#define CONFIG_SYS_HUSH_PARSER
#endif

#if 0
#define CONFIG_DTB_MEM_ADDR 0x1000000
#endif

#if 0
#define CONFIG_AML_CUSTOMER_ID  (0)
#endif

#define AML_CUSTOMER_ID  (0)

/* #define CONFIG_SILENT_CONSOLE */
#define CONFIG_SILENT_CONSOLE_UPDATE_ON_RELOC
#define CONFIG_SILENT_U_BOOT_ONLY

/* 2018.03.19 new compress solution, only support BL33 LZ4 compress */
#define CONFIG_AML_BL33_COMPRESS_ENABLE 1

/* 2018.03.19 G12A only support v3 format for normal & secure boot */
#define CONFIG_AML_SECURE_BOOT_V3 1

#define NR_CPUS		4

#define CPU_ID_REG		P_AO_SEC_SD_CFG8
#define PACKAGE_ID_MASK 	0xFF

#endif /* _CPU_H */
