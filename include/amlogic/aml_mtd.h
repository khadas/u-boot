/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Header file of amlogic mtd driver
 *
 * Copyright (C) 2018 Amlogic Corporation
 *
 * Licensed under the GPL-2 or later.
 *
 */

#ifndef __AMLMTD_H_
#define __AMLMTD_H_
#define BOOT_TOTAL_PAGES			1024

#ifndef CONFIG_SPI_FLASH
#define CONFIG_DISCRETE_BOOTLOADER
#endif

/* spinor adds discrete modem base on advanced burning */
#ifdef CONFIG_SPI_FLASH_AML_ADVANCED
#define CONFIG_DISCRETE_BOOTLOADER
#endif

#if (defined(CONFIG_SPI_NAND) || defined(CONFIG_MTD_SPI_NAND)) && !defined(CONFIG_DISCRETE_BOOTLOADER)
#error SPI NAND only support discrete boot mode!
#endif

#ifdef CONFIG_DISCRETE_BOOTLOADER
#define CONFIG_BL2_COPY_NUM			8
#define CONFIG_TPL_SIZE_PER_COPY	0x200000
#ifdef CONFIG_SPI_FLASH
#define CONFIG_TPL_COPY_NUM			1
#else
#define CONFIG_TPL_COPY_NUM			4
#endif
#endif

/* Mapping conversion between phyAddr and logicAddr,do not open it temporarily */
//#define CONFIG_MTD_LOGIC_MAP

#define BOOT_LOADER			"bootloader"
#define BOOT_BL2			"bl2"
#define BOOT_SPL			"spl"
#define BOOT_BL2E                       "bl2e"
#define BOOT_BL2X                       "bl2x"
#define BOOT_DDRFIP                     "ddrfip"
#define BOOT_DEVFIP                     "devfip"
#define BOOT_TPL			"tpl"
#define BOOT_FIP			"fip"
#define MAX_MTD_CNT			2

#endif/* __AMLMTD_H_ */
