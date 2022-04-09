// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */
/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <arch.h>
#include <stdio.h>
#include <serial.h>
#include <timer.h>
#include <watchdog.h>
#include <string.h>
#include <cache_v8.h>
#include "ram_compress.h"
#include "platform_def.h"

struct ram_compress_full ram_compress_data = {
	.store_phy_addr = (void *)CONFIG_COMPRESSED_DATA_ADDR,
	.full_memsize   = CONFIG_DDR_TOTAL_SIZE,
	.section_count  = CONFIG_COMPRESS_SECTION,
	.sections       = {
		{
			/* compress1 space before reg space: start ~ 3.5G */
			.phy_addr      = (void *)CONFIG_SECTION5_ADDR,
			.section_size  = CONFIG_SECTION5_SIZE,
			.section_index = 5,
			.compress_type = RAM_COMPRESS_NORMAL,
			.copy_dest_addr  = (void *)0,
		},
		{
			/* memory at the begin of ddr, include bl2e/x */
			.phy_addr      = (void *)CONFIG_SECTION1_ADDR,
			.section_size  = CONFIG_SECTION1_SIZE,
			.section_index = 1,
			.compress_type = RAM_COMPRESS_SET,
			.copy_dest_addr  = (void *)0,
			.set_value     = 0x0,
		},
		{
			/* memory before bl31 */
			.phy_addr      = (void *)CONFIG_SECTION2_ADDR,
			.section_size  = CONFIG_SECTION2_SIZE,
			.section_index = 2,
			.compress_type = RAM_COMPRESS_NORMAL,
			.copy_dest_addr  = (void *)0,
		},
		{
			/* memory in bl31 and bl32 */
			.phy_addr      = (void *)CONFIG_SECTION3_ADDR,
			.section_size  = CONFIG_SECTION3_SIZE,
			.section_index = 3,
			.compress_type = RAM_COMPRESS_SET,
			.copy_dest_addr  = (void *)0,
			.set_value     = 0x0,
		},
		{
			/* memory after bl32, and before compress1 start */
			.phy_addr      = (void *)CONFIG_SECTION4_ADDR,
			.section_size  = CONFIG_SECTION4_SIZE,
			.section_index = 4,
			.compress_type = RAM_COMPRESS_NORMAL,
			.copy_dest_addr  = (void *)0,
		},
		/*******************************************************/
		{
			/* memory extend in reg space : 3.5GB ~ 4.0GB */
			.phy_addr      = (void *)CONFIG_SECTION6_ADDR,
			.section_size  = CONFIG_SECTION6_SIZE,
			.section_index = 6,
			.compress_type = RAM_COMPRESS_SET,
			.copy_dest_addr  = (void *)0,
			.set_value     = 0x0,
		},
		{
			/* memory extend after reg space : 4.0GB ~ END */
			.phy_addr      = (void *)CONFIG_SECTION7_ADDR,
			.section_size  = CONFIG_SECTION7_SIZE,
			.section_index = 7,
			.compress_type = RAM_COMPRESS_NORMAL,
			.copy_dest_addr  = (void *)0,
		}
		/*******************************************************/
	},
};

unsigned char debug_enable = 1;
/*******************************************************************************
 * The only thing to do in BL33Z is to load ramdump images and pass control to
 * BL33. The memory occupied by BL33Z will be reclaimed by BL33 stages. BL33Z runs
 * entirely in S-EL2. Since arm standard c libraries are not PIC, printf et al
 * are not available. We rely on assertions to signal error conditions.
 ******************************************************************************/
void bl33z_main(void)
{
	struct ram_compress_full *rcf = &ram_compress_data;

	/* [BL33Z stage]: get reboot mode, compress ddr memory. */
	serial_puts("\nHello world, Now in BL33Z.\n");
	aml_ramdump_compress(rcf, NULL, 0, 0);

	/* [BL33  stage]: Launch u-boot 2019, no params needed. */
	serial_puts("\nRUN back to bl33 uboot!\n");
	return;
}