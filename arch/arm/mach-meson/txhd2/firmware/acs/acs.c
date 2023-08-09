/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/t5d/firmware/acs/acs.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */
#include <version.h>
#include <asm/arch/timing.h>
#include <asm/arch/acs.h>
#include "timing.c"
//main acs struct

dev_param_hdr_t __param_hdr __attribute__ ((section(".dev_header"))) = {
	.magic = DEV_PARAM_MAGIC,
	.version = DEV_PARAM_VERSION,

	.bl2_regs_magic = "bl2r_",
	.bl2_regs_length = sizeof(__bl2_reg),
	//.bl2_regs_length = sizeof(__bl2_ddr_reg_data),
	.bl2_regs_sta = (unsigned *)&__bl2_reg,//__bl2_ddr_reg_data[0],

	.board_clk_magic = "bclk_",
	.board_clk_length = sizeof(__pll_setting),
	.board_clk_sta = &__pll_setting,//__board_clk_setting,

	.opt_reg_magic = "ops__",
	//.opt_reg_length = sizeof(__bl2_ops_reg),
	//.opt_reg_sta = &__bl2_ops_reg[0],

	.sto_set_magic = "store",
	//.sto_set_length = sizeof(__store_para),
	//.sto_set_sta = &__store_para,

	.ddr_set_magic = "ddrs_",
	.ddr_set_length = sizeof(__ddr_setting),
	.ddr_set_sta = &__ddr_setting[0],

	.ddr_2acs_magic = "ddr2ac",
	.ddr_2acs_length = sizeof(__bl2_ddr_reg_data),
	.ddr_2acs_data_p = (unsigned int *)(&__bl2_ddr_reg_data),
};
