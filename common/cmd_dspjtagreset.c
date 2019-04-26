/*
 * common/cmd_reboot.c
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

#include <common.h>
#include <command.h>
#include <asm/arch/cpu.h>
#include <asm/arch-tm2/secure_apb.h>
#include <asm/arch-tm2/timer.h>
#include <asm/arch-tm2/mailbox.h>

#include <serial.h>

#define writel32(val, reg)		(*((volatile uint32_t *)((uintptr_t)(reg)))) =  (val)
#define readl32(reg)			(*((volatile uint32_t *)((uintptr_t)(reg))))

void dsp_jtag_init(unsigned int dspid) {
	init_dsp_jtag(dspid);
}

static int do_dspjtagrest(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int dspid;

	int ret=0;
	if (argc <= 1) {
		printf("plese input dspjtagrest args: id !\n");
		return CMD_RET_USAGE;
	}
	dspid = simple_strtoul(argv[1], NULL, 16);
	printf("dsp%d jtag reset \n",dspid);
	dsp_jtag_init(dspid);
	return ret;
}

U_BOOT_CMD(
	dspjtagreset,	2,	1,	do_dspjtagrest,
	"excute dsp jtag rest",
	"\narg[0]: cmd\n"
	"arg[1]: dspid \n"
);




