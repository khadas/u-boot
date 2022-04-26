// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <dm.h>
#include <wdt.h>

static int do_watchdog(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *cmd;
	char *endp;
	int timeout, ret;
	struct udevice *watchdog_devp;

	if (argc != 2)
		return CMD_RET_USAGE;
	cmd = argv[1];

	ret = uclass_get_device_by_name(UCLASS_WDT, "watchdog", &watchdog_devp);
	if (ret < 0) {
		printf("Failed to find watchdog node, check device tree.\n");
		return ret;
	}
	if (strcmp(cmd, "off") == 0) {
		/* disable watchdog */
		wdt_stop(watchdog_devp);
		return CMD_RET_SUCCESS;
	}

	if (strcmp(cmd, "ping") == 0) {
		wdt_reset(watchdog_devp);
		return CMD_RET_SUCCESS;
	}
	timeout = simple_strtoul(cmd, &endp, 0);
	if (endp == cmd)
		return CMD_RET_USAGE;
	if (timeout < 0)
		return CMD_RET_USAGE;

	/* enable the watchdog and set timeout */
	wdt_start(watchdog_devp, timeout * 1000, 0);
	wdt_reset(watchdog_devp);

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	watchdog, 2, 0,	do_watchdog,
	"enable or disable watchdog",
	"<timeout>	- enable watchdog with `timeout' seconds timeout\n"
	"watchdog off	- disable watchdog\n"
	"watchdog ping	- ping watchdog\n"
);

