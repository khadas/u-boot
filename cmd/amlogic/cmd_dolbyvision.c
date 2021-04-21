// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <amlogic/media/dv/dolby_vision.h>

static int do_dolbyvision_process(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	dolby_vision_process();
	return 1;
}

static int do_dolbyvision_set(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	apply_stb_core_settings();
	return 1;
}

static int do_hdmi_package(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	send_hdmi_pkt();
	return 1;
}

static int do_read_reg(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	unsigned long tmp_addr = 0;

	tmp_addr = simple_strtoul(argv[1], NULL, 16);
	dv_read_reg((unsigned int)tmp_addr);
	printf("addr:%lx, value: %x\n", tmp_addr,
		dv_read_reg((unsigned int)tmp_addr));
	return 1;
}

static int do_write_reg(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	unsigned long tmp_addr = 0;
	unsigned long tmp_value = 0;

	tmp_addr = simple_strtoul(argv[1], NULL, 16);
	tmp_value = simple_strtoul(argv[2], NULL, 16);
	dv_write_reg((unsigned int)tmp_addr,(unsigned int)tmp_value);
	printf("addr:%lx, value: %x\n", tmp_addr,
		dv_read_reg((unsigned int)tmp_addr));
	return 1;
}

static int do_dolbyvision_debug(cmd_tbl_t *cmdtp, int flag, int argc,
			char *const argv[])
{
	int ret = 0;
	unsigned long value = 0;

	if (argc == 2) {
		value = simple_strtoul(argv[1], NULL, 16);
		switch (value) {
		case 0:
			dolbyvision_debug(0);
			break;
		case 1:
			dolbyvision_debug(1);
			break;
		case 2:
			dolbyvision_dump_setting();
			break;
		default:
			cmd_usage(cmdtp);
			break;
		}
	} else {
		return cmd_usage(cmdtp);
	}

	return ret;
}

static cmd_tbl_t cmd_dovi_sub[] = {
	U_BOOT_CMD_MKENT(process, 1, 1, do_dolbyvision_process, "", ""),
	U_BOOT_CMD_MKENT(pkg, 1, 1, do_hdmi_package, "", ""),
	U_BOOT_CMD_MKENT(set, 1, 1, do_dolbyvision_set, "", ""),
	U_BOOT_CMD_MKENT(rv, 2, 1, do_read_reg, "", ""),
	U_BOOT_CMD_MKENT(wv, 3, 1, do_write_reg, "", ""),
	U_BOOT_CMD_MKENT(debug, 2, 0, do_dolbyvision_debug, "", ""),
};

static int do_dolby_vision(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2)
		return cmd_usage(cmdtp);

	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_dovi_sub[0], ARRAY_SIZE(cmd_dovi_sub));

	if (c)
		return  c->cmd(cmdtp, flag, argc, argv);
	else
		return cmd_usage(cmdtp);
}

U_BOOT_CMD(dovi, CONFIG_SYS_MAXARGS, 0, do_dolby_vision,
	"Dolby_vision sub-system",
	"set------------  apply core settings \n"
	"dovi pkg--------------send hdmi pkt\n"
	"dovi rv---------------Dolby_vision read registers\n"
	"dovi wv---------------Dolby_vision write registers\n"
	"dovi debug------------0: disable debug; 1:enable debug; 2: dump setting\n"
);
