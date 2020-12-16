/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-tm2/acs.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __ACS_H
#define __ACS_H

#ifndef __ASSEMBLY__
typedef struct acs_setting{
		char				acs_magic[5];	//acs setting magic word, make sure this piece of data was right.
		unsigned char		chip_type;		//chip type
		unsigned short		version;		//version of acs_setting struct, for PC tool use.
		unsigned long		acs_set_length;	//length of current struct.

		//ddr setting part, 16 bytes
		char				ddr_magic[5];		//magic word to indicate that following 12 bytes was ddr setting.
		unsigned char		ddr_set_version;	//struct version, for PC tool use.
		unsigned short		ddr_set_length;		//length of ddr struct.
		unsigned long		ddr_set_addr;		//address of ddr setting.

		char				ddr_reg_magic[5];
		unsigned char		ddr_reg_version;
		unsigned short		ddr_reg_length;
		unsigned long		ddr_reg_addr;

		char				pll_magic[5];
		unsigned char		pll_set_version;
		unsigned short		pll_set_length;
		unsigned long		pll_set_addr;

		char				sto_magic[5];
		unsigned char		sto_set_version;
		unsigned short		sto_set_length;
		unsigned long		sto_set_addr;

		char				bl2_regs_magic[5];
		unsigned char		bl2_regs_version;
		unsigned short		bl2_regs_length;
		unsigned long		bl2_regs_addr;

		char				rsv_magic[5];
		unsigned char		rsv_set_version;
		unsigned short		rsv_set_length;
		unsigned long		rsv_set_addr;
		char				board_id[12];
		unsigned short		ddr_struct_size[12];
		unsigned long		ddr_struct_org_size;
}__attribute__ ((packed)) acs_set_t;

#endif
#endif
