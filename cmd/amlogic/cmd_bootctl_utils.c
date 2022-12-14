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
#include <amlogic/storage.h>

#include "cmd_bootctl_utils.h"
#ifndef getenv
#define getenv env_get
#endif

#define MISC_BUFSIZE_AVB	2080
#define MISC_BUFSIZE_VAB	2080
#define MISC_BUFSIZE_NORMAL	1088

static size_t g_miscbuf_size;

int aml_get_boot_mode(void)
{
#if defined(CONFIG_UNIFY_BOOTLOADER) && !defined(CONFIG_SYSTEM_RTOS)
// If need enable unify bootloader function, we need do:
// 1. Add #define CONFIG_UNIFY_BOOTLOADER in board/amlogic/configs/xxx.h
// 2. Add below dts config
//      cur-sys-type = "systype-xxx"; xxx is rdk or atv or yocto
//    in "arch/arm64/boot/dts/amlogic/xx.dts" top node
	char cmdbuf[128] = {0};
	int ret = 0;
	char *temp_env = NULL;

	memset(cmdbuf, 0, sizeof(cmdbuf));
	sprintf(cmdbuf, "fdt get value env-cur-sys-type / cur-sys-type;");

	ret = run_command(cmdbuf, 0);
	if (ret)
		printf("%s fdt get error\n", __func__);
	else
		temp_env = getenv("env-cur-sys-type");

	if (strcmp(temp_env, "systype-atv") == 0) {
		printf("we are run in atv\n");
		return BOOT_MODE_VAB;
	} else if (strcmp(temp_env, "systype-rdk") == 0) {
		printf("we are run in rdk\n");
		return BOOT_MODE_AVB;
	} else if (strcmp(temp_env, "systype-yocto") == 0) {
		printf("we are run in yocto\n");
		return BOOT_MODE_AVB;
	}

	printf("we are run in %s fall to avb\n", temp_env);
	return BOOT_MODE_AVB;
#else
#if defined(CONFIG_CMD_BOOTCTOL_AVB) && defined(CONFIG_CMD_BOOTCTOL_VAB)
	printf("boot in VAB mode\n");
	return BOOT_MODE_VAB;
#endif

#if defined(CONFIG_CMD_BOOTCTOL_AVB)
	printf("boot in AVB mode\n");
	return BOOT_MODE_AVB;
#endif
	return BOOT_MODE_NORMAL;
#endif
}

int boot_info_open_partition(char *miscbuf)
{
	char *partition = "misc";

	if (g_miscbuf_size <= 0) {
		int boot_mode = aml_get_boot_mode();

		switch (boot_mode) {
		case BOOT_MODE_VAB:
			g_miscbuf_size = MISC_BUFSIZE_VAB;
			break;
		case BOOT_MODE_AVB:
			g_miscbuf_size = MISC_BUFSIZE_AVB;
			break;
		default:
			g_miscbuf_size = MISC_BUFSIZE_NORMAL;
		break;
		}
	}

	printf("%s Start read %s partition datas\n", __func__, partition);
	if (store_read((const char *)partition,
		0, g_miscbuf_size, (unsigned char *)miscbuf) < 0) {
		printf("failed to store read %s.\n", partition);
		return -1;
	}

	return 0;
}

