// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <dm.h>
#include <misc.h>
#include <malloc.h>
#include <asm/arch/bl31_apis.h>
#include <amlogic/cpu_id.h>
#include <amlogic/jtag.h>
#include <linux/ctype.h>

static struct udevice *jtag_devp;
static int jtagon_id = INVALID_ID;

struct jtag_id_desc {
	unsigned short id;
	const char *core_type;
	const char *jtag_type;
	const char *alias;
};

static struct jtag_id_desc jtag_id_data[] = {
	{JTAG_SELECT_ID(AP,   JTAG_A,  0), "ap", "jtag_a", "apao"},
	{JTAG_SELECT_ID(AP,   JTAG_B,  0), "ap", "jtag_b", "apee"},
	{JTAG_SELECT_ID(AP,   SWD_A,   0), "ap",  "swd_a", "swd_apao"},

	{JTAG_SELECT_ID(SCP,  JTAG_A,  0), "scp", "jtag_a", "scpao"},
	{JTAG_SELECT_ID(SCP,  JTAG_B,  0), "scp", "jtag_b", "scpee"},
	{JTAG_SELECT_ID(SCP,  SWD_A,   0), "scp", "swd_a"},

	{JTAG_SELECT_ID(SP,   JTAG_A,  0), "sp", "jtag_a"},
	{JTAG_SELECT_ID(SP,   JTAG_B,  0), "sp", "jtag_b"},
	{JTAG_SELECT_ID(SP,   SWD_A,   0), "sp", "swd_a"},

	{JTAG_SELECT_ID(DSPA, JTAG_A,  0), "dspa", "jtag_a"},
	{JTAG_SELECT_ID(DSPA, JTAG_B,  0), "dspa", "jtag_b"},
	{JTAG_SELECT_ID(DSPA, SWD_A,   0), "dspa", "swd_a"},

	{JTAG_SELECT_ID(DSPB, JTAG_A,  0), "dspb", "jtag_a"},
	{JTAG_SELECT_ID(DSPB, JTAG_B,  0), "dspb", "jtag_b"},
	{JTAG_SELECT_ID(DSPB, SWD_A,   0), "dspb", "swd_a"},

	{JTAG_SELECT_ID(DIF, JTAG_A,  0), "dif", "jtag_a"},
	{JTAG_SELECT_ID(DIF, JTAG_B,  0), "dif", "jtag_b"},

	{JTAG_SELECT_ID(AOCPU, JTAG_A,  0), "aocpu", "jtag_a"},
	{JTAG_SELECT_ID(AOCPU, JTAG_B,  0), "aocpu", "jtag_b"},

	{JTAG_SELECT_ID(AUCPU, JTAG_A,  0), "aucpu", "jtag_a"},
	{JTAG_SELECT_ID(AUCPU, JTAG_B,  0), "aucpu", "jtag_b"},

	{JTAG_SELECT_ID(SOCBRG, JTAG_A,  0), "socbrg", "jtag_a"},
	{JTAG_SELECT_ID(SOCBRG, JTAG_B,  0), "socbrg", "jtag_b"},

	{JTAG_SELECT_ID(DEMOD, JTAG_A,  0), "demod", "jtag_a"},
	{JTAG_SELECT_ID(DEMOD, JTAG_B,  0), "demod", "jtag_b"},

	{JTAG_SELECT_ID(SECPU, JTAG_A,  0), "secpu", "jtag_a"},
	{JTAG_SELECT_ID(SECPU, JTAG_B,  0), "secpu", "jtag_b"},

	{JTAG_SELECT_ID(MOPA, JTAG_A,  0), "mopa", "jtag_a"},
	{JTAG_SELECT_ID(MOPB, JTAG_A,  0), "mopb", "jtag_a"},
};

static int jtag_id_find(const char *core_type, const char *jtag_type)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(jtag_id_data); i++) {
		if ((!jtag_id_data[i].core_type) || (!jtag_id_data[i].jtag_type))
			continue;
		if ((strcmp(core_type, jtag_id_data[i].core_type) == 0) &&
		    (strcmp(jtag_type, jtag_id_data[i].jtag_type) == 0)) {
			return jtag_id_data[i].id;
		}
	}

	return INVALID_ID;
}

static int jtag_id_find_by_alias(const char *name)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(jtag_id_data); i++) {
		if (!jtag_id_data[i].alias)
			continue;
		if (strcmp(name, jtag_id_data[i].alias) == 0)
			return jtag_id_data[i].id;
	}

	return INVALID_ID;
}

static int jtag_id_get(int argc, char * const* argv)
{
	int jtag_id = INVALID_ID;
	int cluster_type;

	switch (argc) {
	case 2:
		jtag_id = jtag_id_find_by_alias(argv[1]);
		break;
	case 3:
		if (isdigit(argv[2][0])) {
			jtag_id = jtag_id_find_by_alias(argv[1]);
			if (jtag_id < 0)
				return jtag_id;

			cluster_type = simple_strtoul(argv[2], NULL, 10);
			if ((cluster_type & (~CLUSTER_TYPE_MASK)) != 0) {
				printf("invalid cluster type.\n");
				return INVALID_ID;
			}
			jtag_id = CLUSTER_TYPE_UPDATE(jtag_id, cluster_type);
		} else {
			jtag_id = jtag_id_find(argv[1], argv[2]);
		}
		break;
	case 4:
		jtag_id = jtag_id_find(argv[1], argv[2]);
		if (jtag_id < 0)
			return jtag_id;

		cluster_type = simple_strtoul(argv[3], NULL, 10);
		if ((cluster_type & (~CLUSTER_TYPE_MASK)) != 0) {
			printf("invalid cluster type.\n");
			return INVALID_ID;
		}
		jtag_id = CLUSTER_TYPE_UPDATE(jtag_id, cluster_type);
		break;
	default:
		printf("invalid argument count!\n");
		return INVALID_ID;
	}

	return jtag_id;
}

int do_jtagon(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int jtag_id;
	int ret = 0;

	jtag_id = jtag_id_get(argc, argv);
	if (jtag_id < 0) {
		printf("can't find jtag id.\n");
		return -1;
	}

	ret = uclass_get_device_by_name(UCLASS_MISC, "jtag", &jtag_devp);
	if (ret) {
		printf("failed to find jtag node, check device tree.\n");
		return ret;
	}

	if (misc_ioctl(jtag_devp, JTAG_SETPINMUX, &jtag_id) < 0) {
		printf("failed to set jtag pinmux.\n");
		return -1;
	}

	if (misc_ioctl(jtag_devp, JTAG_EANBLE, &jtag_id) < 0) {
		printf("failed to enable jtag.\n");
		return -1;
	}

	jtagon_id = jtag_id;

	return 0;
}

U_BOOT_CMD(
	jtagon, 4, 1, do_jtagon,
	"enable jtag",
	"jtagon <core_type> <jtag_type> [cluster_type]\n"
	"core_type: [ap | scp | sp | dspa | dspb | dif | aocpu | aucpu | socbrg | demod | secpu | mopa | mopb]\n"
	"jtag_type: [jtag_a | jtag_b | swd_a | swd_b]\n"
	"cluster_type: [0 | 1]"
);

int do_jtagoff(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (jtagon_id == INVALID_ID) {
		printf("jtag is not enable!\n");
		return -1;
	}

	if (misc_ioctl(jtag_devp, JTAG_CLRPINMUX, &jtagon_id) < 0) {
		printf("failed to clear jtag pinmux.\n");
		return -1;
	}

	if (misc_ioctl(jtag_devp, JTAG_DISABLE, &jtagon_id) < 0) {
		printf("failed to disable jtag\n");
		return -1;
	}

	jtagon_id = INVALID_ID;

	return 0;
}

U_BOOT_CMD(
	jtagoff, 1, 1, do_jtagoff,
	"disable jtag",
	"jtagoff"
);
