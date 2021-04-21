/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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

	char				board_clk_magic[6];
	unsigned short		board_clk_length;

	char				opt_reg_magic[6];
	unsigned short		opt_reg_length;

	char				sto_set_magic[6];
	unsigned short		sto_set_length;

	char				ddr_set_magic[6];
	unsigned short		ddr_set_length;

	unsigned int		RFU[4];
} __attribute__ ((packed)) dev_param_hdr_t;

#endif
#endif
