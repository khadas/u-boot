/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_viuprobe.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <command.h>
#include <asm/arch/bl31_apis.h>

static int do_viu_probe_enable(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	set_viu_probe_enable();
	return 0;
}

U_BOOT_CMD(
	viu_probe,	2,	1,	do_viu_probe_enable,
	"enable viu probe in no secure chip",
	"enable viu probe in no secure chip"
);

