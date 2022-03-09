/*
 * (C) Copyright 2018
 * Amlogic. Inc. yi.zhou@amlogic.com
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <amlogic/dolby_vision.h>

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
	int enable = 0;

	switch (argc) {
	case 1:
		dolbyvision_dump_setting();
		break;
	case 2:
		enable = simple_strtoul(argv[1], NULL, 10);
		dolbyvision_debug(enable);
		break;
	default:
		return cmd_usage(cmdtp);
	}

	return ret;
}

static cmd_tbl_t cmd_hdmi_sub[] = {
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

	c = find_cmd_tbl(argv[0], &cmd_hdmi_sub[0], ARRAY_SIZE(cmd_hdmi_sub));

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
	"dovi debug------------debug dolby vision\n"
);
