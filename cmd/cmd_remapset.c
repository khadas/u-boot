// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <asm/arch/cpu.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/timer.h>
#include <asm/arch/bl31_apis.h>

#include <serial.h>



static int do_remapset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	int dspid = 0;
	int addr_0 = 0;
	int addr_1 = 0;
	int reg_sel = 0;

	dspid    = simple_strtoul(argv[1], NULL, 16);
	addr_0   = simple_strtoul(argv[2], NULL, 16);
	addr_1   = simple_strtoul(argv[3], NULL, 16);
	reg_sel  = simple_strtoul(argv[4], NULL, 16);

	remap_set(dspid, addr_0, addr_1, reg_sel);

	return ret;
}

U_BOOT_CMD(
	remapset, 5, 1, do_remapset,
	"do_remapset the address will be mapped",
	"\narg[0]: cmd\n"
	"arg[1]: dspid \n"
	"arg[2]: load address!\n"
	"arg[3]: target address!\n"
	"arg[4]: register type select: 0: 16-bit  1: 18-bit  2: 20-bit !\n"
);



