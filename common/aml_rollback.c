// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <config.h>
#include <asm/arch/io.h>
#include <partition_table.h>
#include <libavb.h>
#include <version.h>
#include <fb_mmc.h>
#include <mmc.h>
#include <amlogic/aml_mmc.h>
#include <amlogic/aml_rollback.h>

int write_bootloader_back(int copy, int dstindex)
{
	int ret = -1;
	unsigned char *buffer = NULL;
	char str[128] = {0};
	u64 addr;
	u64 size = 0x2000 * 512 - 512;
	int map = 0;

	buffer = (unsigned char *)malloc(size);
	if (!buffer) {
		printf("ERROR! fail to allocate memory ...\n");
		goto exit;
	}
	memset(buffer, 0, size);
	addr = (unsigned long)buffer;

	if (copy == 0) {
		sprintf(str, "amlmmc switch 1 user");
		ret = run_command(str, 0);
	} else if (copy == 1) {
		sprintf(str, "amlmmc switch 1 boot0");
		ret = run_command(str, 0);
	} else if (copy == 2) {
		sprintf(str, "amlmmc switch 1 boot1");
		ret = run_command(str, 0);
	} else {
		printf("invail copy index\n");
		goto exit;
	}
	if (ret != 0) {
		printf("amlmmc cmd %s failed\n", str);
		goto exit;
	}

	sprintf(str, "amlmmc read bootloader 0x%llx  0x200  0x%llx", addr, size);
	printf("command: %s\n", str);
	ret = run_command(str, 0);
	if (ret != 0) {
		printf("amlmmc cmd %s failed\n", str);
		goto exit;
	}

	if (dstindex == 0)
		map = AML_BL_USER;
	else if (dstindex == 1)
		map = AML_BL_BOOT0;
	else if (dstindex == 2)
		map = AML_BL_BOOT1;

	if (map) {
		ret = amlmmc_write_bootloader(1, map, size, buffer);
		if (ret) {
			printf("update error");
			goto exit;
		}
	}

	sprintf(str, "amlmmc switch 1 user");
	ret = run_command(str, 0);

exit:
	if (buffer) {
		free(buffer);
		buffer = NULL;
	}
	return ret;
}

void update_rollback(void)
{
	char *slot = NULL;

	slot = getenv("slot-suffixes");
	if (!slot) {
		run_command("get_valid_slot", 0);
		slot = getenv("slot-suffixes");
	}
	if (slot && (strcmp(slot, "0") == 0)) {
		printf("back to slot b\n");
		run_command("set_roll_flag 1", 0);
		run_command("set_active_slot b", 0);
		setenv("default_env", "1");
		run_command("saveenv", 0);
		if (write_bootloader_back(2, 0) == 0) {
			printf("rollback ok\n");
			run_command("reset", 0);
		} else {
			printf("rollback failed\n");
			run_command("run storeargs; run update;", 0);
		}
	} else if (slot && (strcmp(slot, "1") == 0)) {
		printf("back to slot a\n");
		run_command("set_roll_flag 1", 0);
		run_command("set_active_slot a", 0);
		setenv("default_env", "1");
		run_command("saveenv", 0);
		if (write_bootloader_back(1, 0) == 0) {
			printf("rollback ok\n");
			run_command("reset", 0);
		} else {
			printf("rollback failed\n");
			run_command("run storeargs; run update;", 0);
		}
	}
}

