
/*
 * arch/arm/cpu/armv8/txl/firmware/acs/acs.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
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
#include <version.h>
#include <asm/arch/acs.h>
#include <asm/arch/timing.h>
#include "timing.c"

dev_param_hdr_t __param_hdr __attribute__ ((section(".dev_header"))) = {
	.magic = DEV_PARAM_MAGIC,
	.version = DEV_PARAM_VERSION,

	.bl2_regs_magic = "bl2r_",
	.bl2_regs_length = sizeof(__bl2_reg),

	.board_clk_magic = "bclk_",
	.board_clk_length = sizeof(__board_clk_setting),

	.opt_reg_magic = "ops__",
	.opt_reg_length = sizeof(__bl2_ops_reg),

	.sto_set_magic = "store",
	.sto_set_length = sizeof(__store_para),

	.ddr_set_magic = "ddrs_",
	.ddr_set_length = sizeof(__ddr_setting),
};

