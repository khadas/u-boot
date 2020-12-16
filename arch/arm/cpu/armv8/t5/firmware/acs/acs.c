/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/t5/firmware/acs/acs.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <asm/arch/acs.h>
#include <asm/arch/timing.h>
#include "timing.c"

//main acs struct
acs_set_t __acs_set={
					.acs_magic		= "acs__",
					.chip_type		= 0x0,
					.version 		= 1,
					.acs_set_length	= sizeof(__acs_set),

					.ddr_magic		= "ddrs_",
					.ddr_set_version= 1,
					.ddr_set_length	= sizeof(__ddr_setting),
					.ddr_set_addr	= (unsigned long)(&__ddr_setting),

					.ddr_reg_magic	= "ddrr_",
					.ddr_reg_version= 1,
					.ddr_reg_length	= sizeof(__ddr_reg),
					.ddr_reg_addr	= (unsigned long)(&__ddr_reg),

					.pll_magic		= "pll__",
					.pll_set_version= 1,
					.pll_set_length	= sizeof(__pll_setting),
					.pll_set_addr	= (unsigned long)(&__pll_setting),

					.sto_magic		= "store",
					.sto_set_version= 1,
					.sto_set_length	= 0,
					.sto_set_addr	= 0,

					.bl2_regs_magic	= "bl2r_",
					.bl2_regs_version = 1,
					.bl2_regs_length = sizeof(__bl2_reg),
					.bl2_regs_addr	= (unsigned long)(&__bl2_reg),

					.rsv_magic		= "rsv0_",
					.rsv_set_version= 1,
					.rsv_set_length	= 0,
					.rsv_set_addr	= 0,
					.board_id		= {0,},
					.ddr_struct_size = {0,},
					.ddr_struct_org_size = sizeof(ddr_set_t),
};
