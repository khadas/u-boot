// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <version.h>
#include <linux/compiler.h>

static int do_bootloader_version(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char s_version[64];
	strcpy(s_version, "01.01.");
	strcat(s_version, U_BOOT_DATE_TIME);
	printf("s_version: %s\n", s_version);
	env_set("bootloader_version", s_version);
	return 0;
}

U_BOOT_CMD(
	get_bootloaderversion,	1,		0,	do_bootloader_version,
	"print bootloader version",
	""
);
