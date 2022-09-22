
/*
 * arch/arm/include/asm/arch-g12a/cpu.h
 *
 * Copyright (C) 2018 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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

/* for uboot environment use */
#define CONFIG_DDR_SIZE					2048

/* dram */
#define PHYS_SDRAM_1_BASE					0x00000000UL
#define PHYS_SDRAM_1_SIZE					0x80000000UL
#define CONFIG_SYS_SDRAM_BASE				PHYS_SDRAM_1_BASE

#define CONFIG_SYS_LOAD_ADDR		(PHYS_SDRAM_1_BASE + CONFIG_SYS_TEXT_BASE)

/* console/print buffer */
#define CONFIG_SYS_CBSIZE		512
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

/* Generic Timer Definitions */
#define COUNTER_FREQUENCY		(0x1800000)	/* 24MHz */

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

#define CONFIG_OF_LIBFDT_OVERLAY 1
#define CONFIG_CMD_DTIMG 1

//#define CONFIG_SILENT_CONSOLE
//#define CONFIG_SILENT_CONSOLE_UPDATE_ON_RELOC
//#define CONFIG_SILENT_U_BOOT_ONLY

/* 2018.03.19 new compress solution, only support BL33 LZ4 compress */
#define CONFIG_AML_BL33_COMPRESS_ENABLE 1

/* 2018.03.19 G12A only support v3 format for normal & secure boot */
#define CONFIG_AML_SECURE_BOOT_V3 1

#define NR_CPUS		4

#define CPU_ID_REG		SYSCTRL_SEC_STATUS_REG0
#define PACKAGE_ID_MASK 	0xFF

#define MIPI_DSI_REGISTER 1
/*C1 SoC secure boot solution default not support AES algorithm*/
//#define CONFIG_AML_CRYPTO_AES 1


/*Flag for enable AES key generate to EFUSE pattern, with this flag only
AES key will be generated to EFUSE pattern, it can be used for use case
like the AES key not ready when secure boot enabled for the first time.
*/
//#define CONFIG_AML_EFUSE_GEN_AES_ONLY 1
#define CONFIG_SYS_NONCACHED_MEMORY     (1 << 20)       /* 1 MiB */

#define CONFIG_AVB2_KPUB_FROM_FIP 1

#define CONFIG_KEY_PRESERVE

#endif /* _CPU_H */
