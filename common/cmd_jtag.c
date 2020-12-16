/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_jtag.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <command.h>
#include <malloc.h>
#include <asm/arch/bl31_apis.h>
#include <asm/cpu_id.h>
#include <amlogic/jtag.h>

#define SWD_AP_AO     10

static int get_jtag_sel(const char *argv)
{
	int sel;
	if (strcmp(argv, "apao") == 0)
		sel = JTAG_A53_AO;
	else if (strcmp(argv, "apee") == 0)
		sel = JTAG_A53_EE;
	else if (strcmp(argv, "scpao") == 0)
		sel = JTAG_M3_AO;
	else if (strcmp(argv, "scpee") == 0)
		sel = JTAG_M3_EE;
	else if (strcmp(argv, "swd_apao") == 0)
		sel = SWD_AP_AO;
	else
		sel = -1;

	return sel;
}

int do_jtagon(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int sel;
	if (argc < 2) {
		printf("invalid argument count!\n");
		return -1;
	}
	sel = get_jtag_sel(argv[1]);
	if (sel < 0) {
		printf("invalid argument!\n");
		return -1;
	}

	jtag_set_pinmux(sel, 1);
	if ((get_cpu_id().family_id == MESON_CPU_MAJOR_ID_GXM) && (sel >1)) {
		if (argv[2]) {
			int tmp = simple_strtoul(argv[2], NULL, 10);
			sel = sel | (tmp << CLUSTER_BIT);
		} else {
			printf("gxm: set A53 jtag to cluster0 by default!\n");
		}
	}
	aml_set_jtag_state(JTAG_STATE_ON, sel);
	return 0;
}

U_BOOT_CMD(
	jtagon,	3,	1,	do_jtagon,
	"enable jtag",
	"jtagon [apao|swd_apao|apee|scpao|scpee] [0|1]\n"
	" [apao|apee|scpao|scpee]  - ap or scp jtag connect to ao or ee domain\n"
	" [swd_apao]               - ap swd connect to ao domain, only tm2 platforms support SWD mode \n"
	" [0|1]                    - special for gxm, ap jtag to cluster 0 or cluster 1\n"
);

int do_jtagoff(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int sel;
	if (argc < 2) {
		printf("invalid argument count!\n");
		return -1;
	}
	sel = get_jtag_sel(argv[1]);
	if (sel < 0) {
		printf("invalid argument!\n");
		return -1;
	}

	jtag_set_pinmux(sel, 0);
	aml_set_jtag_state(JTAG_STATE_OFF, sel);
	return 0;
}

U_BOOT_CMD(
	jtagoff,	2,	1,	do_jtagoff,
	"disable jtag",
	"jtagoff [apao|swd_apao|apee|scpao|scpee]"
);
