// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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

