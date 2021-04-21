/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __AMLMTD_H_
#define __AMLMTD_H_
#define BOOT_TOTAL_PAGES			1024

#define CONFIG_BL2_COPY_NUM			8
#define CONFIG_TPL_SIZE_PER_COPY	0x200000
#define CONFIG_NOR_TPL_COPY_NUM		1 //for spi-nor
#define CONFIG_NAND_TPL_COPY_NUM	4 // for slc & spi-nand

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
