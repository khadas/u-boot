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
#include <fastboot.h>
#include <u-boot/sha1.h>
#include <asm/arch/efuse.h>
#include <stdlib.h>
#include "cmd_bootctl_wrapper.h"
#include "cmd_bootctl_utils.h"

#ifndef getenv
#define getenv env_get
#endif

static bootctl_func_handles *curr_bootctl_handles;

static bootctl_func_handles *select_bootctl_cmd_func(void)
{
	if (curr_bootctl_handles) {
		return curr_bootctl_handles;
	};

	if (aml_get_boot_mode() == BOOT_MODE_AVB)
		curr_bootctl_handles = get_bootctl_cmd_func_avb();
	else if (aml_get_boot_mode() == BOOT_MODE_VAB)
		curr_bootctl_handles = get_bootctl_cmd_func_vab();
	else
		curr_bootctl_handles = get_bootctl_cmd_func();

	return curr_bootctl_handles;
}

static int do_GetValidSlot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	bootctl_func_handles *func_handles = NULL;

	func_handles = select_bootctl_cmd_func();

	if (func_handles && func_handles->do_GetValidSlot_func)
		ret = func_handles->do_GetValidSlot_func(cmdtp, flag, argc, argv);

	return ret;
}

static int do_SetActiveSlot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	bootctl_func_handles *func_handles = NULL;

	func_handles = select_bootctl_cmd_func();

	if (func_handles && func_handles->do_SetActiveSlot_func)
		ret = func_handles->do_SetActiveSlot_func(cmdtp, flag, argc, argv);

	return ret;
}

static int do_SetRollFlag(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	bootctl_func_handles *func_handles = NULL;

	func_handles = select_bootctl_cmd_func();

	if (func_handles && func_handles->do_SetRollFlag_func)
		ret = func_handles->do_SetRollFlag_func(cmdtp, flag, argc, argv);

	return ret;
}

static int do_CopySlot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	bootctl_func_handles *func_handles = NULL;

	func_handles = select_bootctl_cmd_func();

	if (func_handles && func_handles->do_CopySlot_func)
		ret = func_handles->do_CopySlot_func(cmdtp, flag, argc, argv);

	return ret;
}

static int do_SetUpdateTries(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	bootctl_func_handles *func_handles = NULL;

	func_handles = select_bootctl_cmd_func();

	if (func_handles && func_handles->do_SetUpdateTries_func)
		ret = func_handles->do_SetUpdateTries_func(cmdtp, flag, argc, argv);

	return ret;
}

int do_GetSystemMode(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	bootctl_func_handles *func_handles = NULL;

	func_handles = select_bootctl_cmd_func();

	if (func_handles && func_handles->do_GetSystemMode_func)
		ret = func_handles->do_GetSystemMode_func(cmdtp, flag, argc, argv);

	return ret;
}

int do_GetAvbMode(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	bootctl_func_handles *func_handles = NULL;

	func_handles = select_bootctl_cmd_func();

	if (func_handles && func_handles->do_GetAvbMode_func)
		ret = func_handles->do_GetAvbMode_func(cmdtp, flag, argc, argv);

	return ret;
}

U_BOOT_CMD(get_valid_slot, 2, 0, do_GetValidSlot,
	"get_valid_slot",
	"\nThis command will choose valid slot to boot up which saved in misc\n"
	"partition by mark to decide whether execute command!\n"
	"So you can execute command: get_valid_slot");

U_BOOT_CMD(set_active_slot, 2, 1, do_SetActiveSlot,
	"set_active_slot",
	"\nThis command will set active slot\n"
	"So you can execute command: set_active_slot a");

U_BOOT_CMD(set_roll_flag, 2, 1, do_SetRollFlag,
	"set_roll_flag",
	"\nThis command will set active slot\n"
	"So you can execute command: set_active_slot a");

U_BOOT_CMD(copy_slot_bootable, 3, 1, do_CopySlot,
	"copy_slot_bootable",
	"\nThis command will set active slot\n"
	"So you can execute command: copy_slot_bootable 2 1");

U_BOOT_CMD(update_tries, 2, 0, do_SetUpdateTries,
	"update_tries",
	"\nThis command will change tries_remaining in misc\n"
	"So you can execute command: update_tries");

U_BOOT_CMD(get_system_as_root_mode, 1, 0, do_GetSystemMode,
	"get_system_as_root_mode",
	"\nThis command will get system_as_root_mode\n"
	"So you can execute command: get_system_as_root_mode");

U_BOOT_CMD(get_avb_mode, 1, 0, do_GetAvbMode,
	"get_avb_mode",
	"\nThis command will get avb mode\n"
	"So you can execute command: get_avb_mode");

