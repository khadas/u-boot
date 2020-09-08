
/*
 * arch/arm/include/asm/arch-sc2/acs.h
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
