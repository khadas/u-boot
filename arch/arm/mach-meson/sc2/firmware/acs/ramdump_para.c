/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */
#include "ramdump_para.h"

ram_compress_full_t __ramdump_data __attribute__ ((section(".ramdump_param"))) = {
	.store_phy_addr = (void *)CONFIG_COMPRESSED_DATA_ADDR,
	.full_memsize   = CONFIG_DDR_TOTAL_SIZE,
	.section_count  = CONFIG_COMPRESS_SECTION,
	.sections       = {
		{
			/* memory afer compressed data address */
			.phy_addr      = (void *)CONFIG_SECTION0_ADDR,
			.section_size  = CONFIG_SECTION0_SIZE,
			.section_index = 6,
			.compress_type = RAM_COMPRESS_NORMAL,
			.copy_dest_addr  = 0,
		},
		{
			/* memory at the begin of ddr */
			.phy_addr      = (void *)CONFIG_SECTION1_ADDR,
			.section_size  = CONFIG_SECTION1_SIZE,
			.section_index = 1,
			.compress_type = RAM_COMPRESS_NORMAL,
			.copy_dest_addr  = 0,
		},
		{
			/* memory of bl2e and blex */
			.phy_addr      = (void *)CONFIG_SECTION2_ADDR,
			.section_size  = CONFIG_SECTION2_SIZE,
			.section_index = 2,
			.compress_type = RAM_COMPRESS_NORMAL,
			.copy_dest_addr  = (void *)(CONFIG_BL32_LOAD_ADDR),
			.copy_size_limit = CONFIG_BL32_LOAD_SIZE,
		},
		{
			/* memory before bl31 */
			.phy_addr      = (void *)CONFIG_SECTION3_ADDR,
			.section_size  = CONFIG_SECTION3_SIZE,
			.section_index = 3,
			.compress_type = RAM_COMPRESS_NORMAL,
			.copy_dest_addr  = 0,
		},
		{
			/* memory in bl31 and bl32 */
			.phy_addr      = (void *)CONFIG_SECTION4_ADDR,
			.section_size  = CONFIG_SECTION4_SIZE,
			.section_index = 4,
			.compress_type = RAM_COMPRESS_SET,
			.copy_dest_addr  = 0,
			.set_value     = 0x0,
		},
		{
			/* memory after bl32, and before compress addr */
			.phy_addr      = (void *)CONFIG_SECTION5_ADDR,
			.section_size  = CONFIG_SECTION5_SIZE,
			.section_index = 5,
			.compress_type = RAM_COMPRESS_NORMAL,
			.copy_dest_addr  = 0,
		}
	},
};
