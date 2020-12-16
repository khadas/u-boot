/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_dspjtagreset.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
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




