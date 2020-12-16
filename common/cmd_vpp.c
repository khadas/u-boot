/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_vpp.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <command.h>
#include <vpp.h>
#include <amlogic/dolby_vision.h>
#include <asm/cpu_id.h>

static int do_vpp_pq(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	short brightness, contrast, sat, hue;

	if (argc < 1) {
		printf("[vpp] pq val error !!!\n");
		return cmd_usage(cmdtp);
	}

	if (strcmp(argv[0], "pq") == 0) {
		if (argc == 5) {
			brightness = simple_strtol(argv[1], NULL, 10);
			contrast = simple_strtol(argv[2], NULL, 10);
			sat = simple_strtol(argv[3], NULL, 10);
			hue = simple_strtol(argv[4], NULL, 10);

			printf("[vpp] bri:%d, cont:%d,sat: %d, hue: %d\n",
				brightness, contrast, sat, hue);
			vpp_pq_init(brightness, contrast, sat, hue);
		} else {
			printf("[vpp]arg cnt  error: %x !!!\n", argc);
			return cmd_usage(cmdtp);
		}
	} else {
		printf("[vpp]arg val  error !!!\n");
		return cmd_usage(cmdtp);
	}

	return CMD_RET_SUCCESS;
}

static int do_hdr_packet(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{

	if (argc < 1) {
		printf("[vpp] hdr packet error !!!\n");
		return cmd_usage(cmdtp);
	}
	if (!check_dolby_vision_on()) {
		printf("hdr_packet\n");
		hdr_tx_pkt_cb();
	}

	return CMD_RET_SUCCESS;
}

static int do_vpp_init(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	vpp_init();
	return CMD_RET_SUCCESS;

}

static cmd_tbl_t cmd_vpp_sub[] = {
	U_BOOT_CMD_MKENT(pq, 5, 1, do_vpp_pq, "", ""),
	U_BOOT_CMD_MKENT(hdrpkt, 1, 1, do_hdr_packet, "", ""),
	U_BOOT_CMD_MKENT(init, 2, 0, do_vpp_init, "", ""),
};

static int do_vpp(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *c;

	/* Strip off leading 'bmp' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_vpp_sub[0], ARRAY_SIZE(cmd_vpp_sub));

	if (c) {
		return c->cmd(cmdtp, flag, argc, argv);
	} else {
		cmd_usage(cmdtp);
		return 1;
	}
}

U_BOOT_CMD(vpp, CONFIG_SYS_MAXARGS, 0, do_vpp,
	"vpp sub-system",
	"osd+video pq value  brightness/contrast/saturation/hue parameters\n"
	"vpp init     - init vpp\n"
);
