/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_watchdog.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <command.h>
#include <asm/arch/watchdog.h>

static int do_watchdog(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *cmd;
	char *endp;
	int timeout;

	if (argc != 2)
		return CMD_RET_USAGE;

	cmd = argv[1];

	if (strcmp(cmd, "off") == 0) {
		/* disable watchdog */
		watchdog_disable();
		return CMD_RET_SUCCESS;
	}

	timeout = simple_strtoul(cmd, &endp, 0);
	if (endp == cmd)
		return CMD_RET_USAGE;
	if (timeout < 0)
		return CMD_RET_USAGE;

	/* enable the watchdog and set timeout */
	timeout *= 1000;
	watchdog_init(timeout);

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	watchdog, 2, 0,	do_watchdog,
	"enable or disable watchdog",
	"<timeout>	- enable watchdog with `timeout' seconds timeout\n"
	"watchdog off		- disable watchdog\n"
);

