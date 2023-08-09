#include <common.h>
#include <command.h>
#include <linux/delay.h>

static int do_mdelay(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	ulong delay_ms;

	if (argc != 2)
		return CMD_RET_USAGE;

	delay_ms = simple_strtoul(argv[1], NULL, 10);
	udelay(delay_ms * 1000);  // Convert milliseconds to microseconds

	return 0;
}

U_BOOT_CMD(
	mdelay, 2, 0, do_mdelay,
	"Delay execution for specified number of milliseconds",
	"milliseconds"
);

static int do_udelay(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	ulong delay_us;

	if (argc != 2)
		return CMD_RET_USAGE;

	delay_us = simple_strtoul(argv[1], NULL, 10);
	udelay(delay_us);

	return 0;
}

U_BOOT_CMD(
	udelay, 2, 0, do_udelay,
	"Delay execution for specified number of microseconds",
	"microseconds"
);
