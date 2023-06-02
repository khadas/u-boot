/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-t5d/acs.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __ACS_H
#define __ACS_H

#define CHIP_PARAM_MAGIC		0x50696863  //"chiP"
#define DEV_PARAM_MAGIC			0x50766564  //"devP"

#define CHIP_PARAM_VERSION		0x1
#define DEV_PARAM_VERSION		0x1
#ifndef __ASSEMBLY__
typedef struct dev_param_hdr {
	unsigned int		magic;
	unsigned int		version;

	char				bl2_regs_magic[6];
	unsigned short		bl2_regs_length;
	unsigned *bl2_regs_sta;

	char				board_clk_magic[6];
	unsigned short		board_clk_length;
	pll_set_t  *board_clk_sta;//board_clk_set_t

	char				opt_reg_magic[6];
	unsigned short		opt_reg_length;
	unsigned  *opt_reg_sta;//register_ops_t

	char				sto_set_magic[6];
	unsigned short		sto_set_length;
	unsigned  *sto_set_sta;//storage_parameter_t

	char				ddr_set_magic[6];
	unsigned short		ddr_set_length;
	ddr_set_ps0_only_t *ddr_set_sta;

	char				ddr_2acs_magic[6];
	unsigned short		ddr_2acs_length;
	unsigned int		*ddr_2acs_data_p;

	unsigned int		RFU[4];
} __attribute__ ((packed)) dev_param_hdr_t;

#endif
#endif
