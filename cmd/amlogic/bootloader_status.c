// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <common.h>
#include <linux/kernel.h>
#include <amlogic/aml_efuse.h>
#include <amlogic/cpu_id.h>
#include <amlogic/storage.h>
#include <partition_table.h>
#include <fastboot.h>
#include <emmc_partitions.h>
#include <asm/arch/efuse.h>
#include <amlogic/aml_rollback.h>
#include <part.h>
#include <cli.h>

#if defined(CONFIG_EFUSE_OBJ_API) && defined(CONFIG_CMD_EFUSE)
extern efuse_obj_field_t efuse_field;
#endif//#ifdef CONFIG_EFUSE_OBJ_API

int __attribute__((weak)) store_logic_read(const char *name, loff_t off, size_t size, void *buf)
{ return store_read(name, off, size, buf);}

typedef boot_img_hdr_t boot_img_hdr;

#define debugP(fmt...) //printf("[DbgBootSta]L%d:", __LINE__),printf(fmt)
#define errorP(fmt...) printf("ErrBootSta(L%d):", __LINE__),printf(fmt)
#define wrnP(fmt...)   printf("wrn:"fmt)
#define MsgP(fmt...)   printf("[BootSta]"fmt)

#define BOOTLOADER_OFFSET		512
#define BOOTLOADER_MAX_SIZE		(4 * 1024 * 1024)

//check SWPL-31296 for details
static int do_get_bootloader_status(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	bool printStat = false;
	bool saveenv   = false;
	debugP("Initial value for argc=%d\n", argc);
	while (argc > 1 && **(argv + 1) == '-') {
		char *arg = *++argv;

		--argc;
		while (*++arg) {
			switch (*arg) {
				case 'p':		/* print */
					printStat = true;
					break;
				case 's':		/* saveenv */
					saveenv = true;
					break;
				default:
					return CMD_RET_USAGE;
			}
		}
	}
	debugP("Final value for argc=%d\n", argc);
	//1,forUpgrade_socType, cpu familyId
	const cpu_id_t cpuid = get_cpu_id();
	const int familyId	 = cpuid.family_id;
	env_set_hex("forUpgrade_socType", familyId);

	//2,forUpgrade_secureBoot
	const bool secureboot = IS_FEAT_BOOT_VERIFY();
	env_set("forUpgrade_secureBoot", secureboot ? "true" : "false");

	//3,forUpgrade_robustOta
	bool supportRobustOta = false;
	switch (familyId) {
		case 0x32://sc2
		case 0x36://t7
		case 0x37://s4
		case 0x38://t3
		case 0x3A://s4d
			supportRobustOta = true;
			break;
		default:
		{
			if (familyId > 0x3A) {
				supportRobustOta = true;
			}
		}
		break;
	}
	env_set("forUpgrade_robustOta", supportRobustOta ? "true" : "false");

	//4,forUpgrade_flashType
	const char* BootDevices[] = {
		"BOOT_EMMC",		"BOOT_SD",
		"BOOT_NAND_NFTL",	"BOOT_NAND_MTD",
		"BOOT_SNAND",		"BOOT_SNOR",
	};
	const char* bootDevice = "BOOT_NONE";
	enum boot_type_e  bootType = store_get_type();
	int i = 0;
	for (; i < ARRAY_SIZE(BootDevices); ++i) {
		if ((1<<i) != bootType) continue;
		bootDevice = BootDevices[i];
		break;
	}
	env_set("forUpgrade_flashType", bootDevice);

	//5,forUpgrade_bootloaderCopies, how many copies supported
	int bootCopies = 1;
	switch (bootType) {
		case BOOT_EMMC: bootCopies = 3; break;
		default:break;
	}
	env_set_ulong("forUpgrade_bootloaderCopies", bootCopies);

	//6,forUpgrade_bootloaderIndex
	//for emmc, 0/1/2 is user/boot0/boot1
	const int bootCpyIndex = store_bootup_bootidx("bootloader");
	env_set_ulong("forUpgrade_bootloaderIndex", bootCpyIndex);

	//7,get first boot index, for defendkey
	const int firstBootCpyIndex = store_boot_copy_start();
	env_set_ulong("forUpgrade_1stBootIndex", firstBootCpyIndex);

	if (printStat) run_command("printenv forUpgrade_socType forUpgrade_secureBoot "
			" forUpgrade_robustOta forUpgrade_flashType forUpgrade_bootloaderCopies "
			" forUpgrade_bootloaderIndex forUpgrade_1stBootIndex", 0);

	if (saveenv) {
#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
		run_command("update_env_part -p forUpgrade_robustOta forUpgrade_bootloaderIndex", 0
			);
#else
		run_command("saveenv", 0);
#endif
	}


	return CMD_RET_SUCCESS;
}

static void run_recovery_from_flash(void) {
	env_set("dolby_status","0");
	run_command("run init_display", 0);
	run_command("run storeargs", 0);
	run_command("get_rebootmode", 0);
	run_command("if test ${reboot_mode} = quiescent; then "\
		"setenv bootconfig ${bootconfig} androidboot.quiescent=1; fi;", 0);
	run_command("if test ${reboot_mode} = recovery_quiescent; then "\
		"setenv bootconfig ${bootconfig} androidboot.quiescent=1; fi;", 0);
	run_command("run recovery_from_flash", 0);
}

static void run_recovery_from_cache(void) {

	char *loadaddr_kernel = env_get("loadaddr_kernel");
	if (loadaddr_kernel != NULL) {
		env_set("loadaddr",loadaddr_kernel);
	} else {
		env_set("loadaddr","0x01080000");
	}
	env_set("dolby_status","0");
	run_command("run bcb_cmd;", 0);
	run_command("run init_display", 0);
	run_command("run storeargs", 0);
	run_command("get_rebootmode", 0);

	run_command("if test ${reboot_mode} = quiescent; then "\
		"setenv bootconfig ${bootconfig} androidboot.quiescent=1; fi;", 0);
	run_command("if test ${reboot_mode} = recovery_quiescent; then "\
		"setenv bootconfig ${bootconfig} androidboot.quiescent=1; fi;", 0);
	run_command("if ext4load mmc 1:2 ${dtb_mem_addr} /recovery/dtb.img; then echo cache dtb.img loaded; fi;", 0);

	run_command("if test ${reboot_vendor_boot} = true; then "\
		"setenv vendor_boot_mode true; fi;", 0);

	run_command("if test ${vendor_boot_mode} = true; then "\
		"setenv bootargs ${bootargs} ${fs_type} aml_dt=${aml_dt} recovery_part=${recovery_part} recovery_offset=${recovery_offset};"\
		"imgread kernel ${recovery_part} ${loadaddr} ${recovery_offset};"\
		"else "\
		"if ext4load mmc 1:2 ${loadaddr} /recovery/recovery.img; then echo cache recovery.img loaded; fi;"\
		"fi;", 0);

	env_set("check_result","recovery_succ");
	env_set("recovery_mode", "true");
	run_command("bootm ${loadaddr}", 0);
	env_set("check_result","recovery_fail");
	env_set("reboot_status","reboot_recovery");
	run_command("saveenv", 0);
	run_command("reboot", 0);//need reboot old bootloader
}

int write_bootloader_back(const char *bootloaderindex, int dstindex)
{
	int iRet = 0;
	int copy = 0;
	int ret = -1;
	unsigned char* buffer = NULL;
	int capacity_boot = 0;

	if (strcmp(bootloaderindex, "1") == 0) {
		copy = 1;
	} else if (strcmp(bootloaderindex, "2") == 0) {
		copy = 2;
	} else if (strcmp(bootloaderindex, "0") == 0) {
		copy = 0;
	}

#ifdef CONFIG_MMC_MESON_GX
	struct mmc *mmc = NULL;

	if (store_get_type() == BOOT_EMMC)
		mmc = find_mmc_device(1);

	if (mmc)
		capacity_boot = mmc->capacity_boot;
#endif

	printf("write_bootloader_back_capacity_boot: %x\n", capacity_boot);

	buffer = (unsigned char *)malloc(capacity_boot);
	if (!buffer)
	{
		printf("ERROR! fail to allocate memory ...\n");
		goto exit;
	}
	memset(buffer, 0, capacity_boot);
	iRet = store_boot_read("bootloader", copy, 0, buffer);
	if (iRet) {
		errorP("Fail read bootloader from rsv with sz\n");
		goto exit;
	}
	iRet = store_boot_write("bootloader", dstindex, 0, buffer);
	if (iRet) {
		printf("Failed to write bootloader\n");
		goto exit;
	} else {
		ret = 0;
	}

exit:
	if (buffer)
	{
		free(buffer);
		//buffer = NULL;
	}
	return ret;
}

//bootloader write protect
static void bootloader_wp(void)
{
#ifdef CONFIG_MMC_MESON_GX
	if (store_get_type() == BOOT_EMMC) {//emmc device
		if (IS_FEAT_BOOT_VERIFY()) { //secure boot enable
			if (BOOTLOADER_MODE_ADVANCE_INIT) { //new arch chip
				env_set("bootloader_wp", "1");
			}
		}
	}
#endif
}

static void aml_recovery(void) {
	char *mode = NULL;
	char command[32];
	char miscbuf[4096] = {0};

	run_command("get_rebootmode", 0);

	//get reboot_mode
	mode = env_get("reboot_mode");
	if (mode == NULL) {
		wrnP("can not get reboot mode, so skip recovery check\n");
	} else {
		if ((!strcmp(mode, "factory_reset")) || (!strcmp(mode, "update"))) {
			env_set("dolby_status","0");
		}
	}

#ifdef CONFIG_BOOTLOADER_CONTROL_BLOCK
	int ret = 0;
	extern int boot_info_open_partition(char *miscbuf);
	ret = boot_info_open_partition(miscbuf);
	if (ret != 0) {
		wrnP("open misc partition failed, so skip recovery check");
		return;
	}
#endif

	//if run recovery, need disable dolby
	memcpy(command, miscbuf, 32);
	if (!memcmp(command, "boot-recovery", strlen("boot-recovery"))) {
		env_set("dolby_status","0");
		return;
	}
}

void update_rollback(void)
{
	char *slot = NULL;
	int ret = -1;
	int gpt_flag = -1;

#ifdef CONFIG_MMC_MESON_GX
	struct mmc *mmc = NULL;

	if (store_get_type() == BOOT_EMMC)
		mmc = find_mmc_device(1);

	if (mmc)
		gpt_flag = aml_gpt_valid(mmc);
#endif
	if (gpt_flag == 0)
		ret = 0;

#if defined(CONFIG_EFUSE_OBJ_API) && defined(CONFIG_CMD_EFUSE)
	run_command("efuse_obj get FEAT_DISABLE_EMMC_USER", 0);

	//dis_user_flag = run_command("efuse_obj get FEAT_DISABLE_EMMC_USER", 0);
	if (*efuse_field.data == 1)
		ret = 0;
#endif//#ifdef CONFIG_EFUSE_OBJ_API

	slot = env_get("slot-suffixes");
	if (!slot) {
		run_command("get_valid_slot", 0);
		slot = env_get("slot-suffixes");
	}
	if (strcmp(slot, "0") == 0) {
		if (ret != 0) {
			wrnP("normal mode\n");
			write_bootloader_back("2", 0);
			env_set("expect_index", "0");
		} else {
			wrnP("gpt or disable user bootloader mode\n");
			write_bootloader_back("2", 1);
			env_set("expect_index", "1");
		}
		wrnP("back to slot b\n");
		run_command("set_roll_flag 1", 0);
		run_command("set_active_slot b", 0);
	} else if (strcmp(slot, "1") == 0) {
		if (ret != 0) {
			wrnP("normal mode\n");
			write_bootloader_back("1", 0);
			env_set("expect_index", "0");
		} else {
			wrnP("gpt or disable user bootloader mode\n");
			write_bootloader_back("2", 1);
			env_set("expect_index", "1");
		}
		wrnP("back to slot a\n");
		run_command("set_roll_flag 1", 0);
		run_command("set_active_slot a", 0);
	}
	env_set("update_env", "1");
	env_set("reboot_status", "reboot_next");
	env_set("update_dts_gpt", "0");
	env_set("write_boot", "0");
	run_command("saveenv", 0);
	run_command("reset", 0);
}

static int write_bootloader(int i)
{
	unsigned char *buffer = NULL;
	int capacity_boot = 0x2000 * 512;
	int iRet = 0;
	char partname[32] = {0};
	char *slot_name = NULL;

#ifdef CONFIG_MMC_MESON_GX
	struct mmc *mmc = NULL;

	if (store_get_type() == BOOT_EMMC)
		mmc = find_mmc_device(1);

	if (mmc)
		capacity_boot = mmc->capacity_boot;
#endif

	printf("capacity_boot: 0x%x\n", capacity_boot);
	buffer = (unsigned char *)malloc(capacity_boot);
	if (!buffer) {
		printf("ERROR! fail to allocate memory ...\n");
		return -1;
	}
	memset(buffer, 0, capacity_boot);

	slot_name = env_get("active_slot");
	if (slot_name && (strcmp(slot_name, "_a") == 0))
		strcpy((char *)partname, "bootloader_a");
	else if (slot_name && (strcmp(slot_name, "_b") == 0))
		strcpy((char *)partname, "bootloader_b");
	else
		strcpy((char *)partname, "bootloader_up");

	iRet = store_logic_read(partname, 0, BOOTLOADER_MAX_SIZE - BOOTLOADER_OFFSET, buffer);
	if (iRet) {
		errorP("Fail to read 0x%xB from part[%s] at offset 0\n",
					BOOTLOADER_MAX_SIZE - BOOTLOADER_OFFSET, partname);
		free(buffer);
		return -1;
	}

	iRet = store_boot_write("bootloader", i, BOOTLOADER_MAX_SIZE - BOOTLOADER_OFFSET, buffer);
	if (iRet) {
		printf("Failed to write boot0\n");
		free(buffer);
		return -1;
	}

	free(buffer);
	return 0;
}

static int update_gpt(int flag)
{
	int ret = 0;
#ifdef CONFIG_MMC_MESON_GX
	unsigned char *buffer = NULL;
	int capacity_boot = 0x2000 * 512;
	int iRet = 0;
	struct mmc *mmc = NULL;
	struct blk_desc *dev_desc;

	if (store_get_type() == BOOT_EMMC)
		mmc = find_mmc_device(1);

	if (mmc)
		capacity_boot = mmc->capacity_boot;

	printf("capacity_boot: 0x%x\n", capacity_boot);
	buffer = (unsigned char *)malloc(capacity_boot);
	if (!buffer) {
		printf("ERROR! fail to allocate memory ...\n");
		return -1;
	}
	memset(buffer, 0, capacity_boot);

	iRet = store_boot_read("bootloader", 0, 0, buffer);
	if (iRet) {
		printf("Failed to read boot0\n");
		ret = -1;
		goto exit;
	}

	if (mmc) {
		printf("try to read gpt data from bootloader.img\n");
		int erase_flag = 0;

		dev_desc = blk_get_dev("mmc", 1);
		if (!dev_desc || dev_desc->type == DEV_TYPE_UNKNOWN) {
			printf("invalid mmc device\n");
			ret = -1;
			goto exit;
		}

		if (flag == 1 || flag == 2) {
			printf("update from dts to gpt, erase first\n");
			erase_gpt_part_table(dev_desc);
		}

		if (is_valid_gpt_buf(dev_desc, buffer + 0x3DFE00)) {
			printf("printf normal bootloader.img, no gpt partition table\n");
			ret = -1;
			goto exit;
		} else {
			erase_flag = check_gpt_change(dev_desc, buffer + 0x3DFE00);

			if (erase_flag == 3) {
				printf("Important partition changes, refused to upgrade\n");
				ret = 1;
				goto exit;
			}

			if (flag == 2) {
#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
				env_set("dts_to_gpt", "1");
				run_command("update_env_part -p dts_to_gpt;", 0);
#else
				run_command("defenv_reserve;setenv dts_to_gpt 1;saveenv;", 0);
#endif
			}

			if (write_mbr_and_gpt_partitions(dev_desc, buffer + 0x3DFE00)) {
				printf("%s: writing GPT partitions failed\n", __func__);
				ret = 1;
				goto exit;
			}

			if (mmc_device_init(mmc) != 0) {
				printf(" update gpt partition table fail\n");
				ret = 2;
				goto exit;
			}
			printf("%s: writing GPT partitions ok\n", __func__);
		}
	}

	if (flag == 1 || flag == 2) {
		printf("update from dts to gpt, backup old bootloader\n");
		char *slot = NULL;

		slot = env_get("slot-suffixes");
		if (!slot) {
			run_command("get_valid_slot", 0);
			slot = env_get("slot-suffixes");
		}
		if (strcmp(slot, "0") == 0) {
			printf("active is a, b is old, don't need backup\n");
		} else if (strcmp(slot, "1") == 0) {
			printf("active is b, a is old, backup boot0 to boot1\n");
			iRet = write_bootloader_back("1", 2);
			if (iRet != 0) {
				printf("Failed to write boot1\n");
				ret = 3;
				goto exit;
			}
		}
		iRet = store_boot_write("bootloader", 1, 0, buffer);
		if (iRet) {
			printf("Failed to write boot0\n");
			ret = 4;
			goto exit;
		}
	}

exit:
	if (buffer)
		free(buffer);

	if (mmc && ret > 0) {
		dev_desc = blk_get_dev("mmc", 1);
		if (dev_desc && dev_desc->type != DEV_TYPE_UNKNOWN) {
			printf("valid mmc device, erase gpt\n");
			erase_gpt_part_table(dev_desc);
		}
		if (ret == 1 || ret == 2 || ret == 3) {
			printf("rollback\n");
			update_rollback();
		} else if (ret == 4) {
			printf("write back boot0, rollback\n");
			write_bootloader_back("2", 1);
			update_rollback();
		}
	}
#endif
	return ret;
}

static int do_secureboot_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]) {
	int match_flag = 0;
	char *rebootstatus = NULL;
	char *checkresult = NULL;
	char *bootloaderindex = NULL;
	char *expect_index = NULL;
	char *robustota = NULL;
	char *update_env = NULL;
	char *rebootmode = NULL;
	int gpt_flag = -1;
	int ret = -1;

#ifdef CONFIG_MMC_MESON_GX
	struct mmc *mmc = NULL;
	//int capacity_boot = 0;

	if (store_get_type() == BOOT_EMMC)
		mmc = find_mmc_device(1);
#endif
	//unsupport update dt in boothal, update dt in uboot
	run_command("update_dt;", 0);

	bootloader_wp();

	//set default env before check
	env_set("recovery_mode", "false");

	//if recovery mode, need disable dv, if factoryreset, need default uboot env
	aml_recovery();

#ifdef CONFIG_MMC_MESON_GX
	if (mmc)
		gpt_flag = aml_gpt_valid(mmc);
#endif
	if (gpt_flag == 0)
		ret = 0;

#if defined(CONFIG_EFUSE_OBJ_API) && defined(CONFIG_CMD_EFUSE)
	run_command("efuse_obj get FEAT_DISABLE_EMMC_USER", 0);

	if (*efuse_field.data == 1) {
		wrnP("efuse_field.data == 1\n");
		ret = 0;
		env_set("nocs_mode", "true");
	} else {
		wrnP("efuse_field.data != 1\n");
		env_set("nocs_mode", "false");
	}
#endif//#ifdef CONFIG_EFUSE_OBJ_API

	run_command("get_rebootmode", 0);
	rebootmode = env_get("reboot_mode");
	if (!rebootmode) {
		wrnP("can not get reboot mode, so skip secure check\n");
		return -1;
	}
	printf("rebootmode is %s\n", rebootmode);
	if (rebootmode && (strcmp(rebootmode, "rescueparty") == 0)) {
		printf("rebootmode is rescueparty, need rollback\n");
		char *slot;

#ifdef CONFIG_FASTBOOT
		struct misc_virtual_ab_message message;

		set_mergestatus_cancel(&message);
#endif

		slot = env_get("slot-suffixes");
		if (!slot) {
			run_command("get_valid_slot", 0);
			slot = env_get("slot-suffixes");
		}
		if (strcmp(slot, "0") == 0) {
			if (ret != 0) {
				wrnP("normal mode\n");
				write_bootloader_back("2", 0);
				env_set("expect_index", "0");
			} else {
				wrnP("gpt or disable user bootloader mode\n");
				write_bootloader_back("2", 1);
				env_set("expect_index", "1");
			}
			wrnP("back to slot b\n");
			run_command("set_roll_flag 1", 0);
			run_command("set_active_slot b", 0);
		} else if (strcmp(slot, "1") == 0) {
			if (ret != 0) {
				wrnP("normal mode\n");
				write_bootloader_back("1", 0);
				env_set("expect_index", "0");
			} else {
				wrnP("gpt or disable user bootloader mode\n");
				write_bootloader_back("2", 1);
				env_set("expect_index", "1");
			}
			wrnP("back to slot a\n");
			run_command("set_roll_flag 1", 0);
			run_command("set_active_slot a", 0);
		}

		env_set("update_env", "1");
		env_set("reboot_status", "reboot_next");
		run_command("saveenv", 0);
		run_command("reset", 0);
	}

	char *write_boot = env_get("write_boot");

	if (has_boot_slot == 0 && write_boot && (!strcmp(write_boot, "1"))) {
		printf("non ab for kernel 5.15 update bootloader\n");
		write_bootloader(1);
		write_bootloader(2);
		env_set("write_boot", "0");
		env_set("upgrade_step", "1");
		run_command("saveenv", 0);
		run_command("reset", 0);
	}

	//check_result init
	checkresult = env_get("check_result");
	if (checkresult == NULL) {
		env_set("check_result","succ");
	}

	//reboot_status init
	rebootstatus = env_get("reboot_status");
	if (rebootstatus == NULL) {
		env_set("reboot_status","reboot_init");
		rebootstatus = env_get("reboot_status");
	}

	if (rebootstatus == NULL) {
		printf("rebootstatus is NULL, skip check\n");
		return -1;
	}

	//check reboot_end
	if (!strcmp(rebootstatus, "reboot_end")) {
		env_set("reboot_status","reboot_init");
		run_command("saveenv", 0);
	}

	//get boot status
	run_command("amlbootsta -p -s", 0);

	//get forUpgrade_robustOta and check if support robustota
	robustota = env_get("forUpgrade_robustOta");
	if ((robustota == NULL) || !strcmp(robustota, "false")) {
		return -1;
	}

	//get bootloader index
	bootloaderindex = env_get("forUpgrade_bootloaderIndex");
	if (bootloaderindex == NULL) {
		wrnP("can not get bootloader index, so skip secure check\n");
		return -1;
	}

	int rc = 0;
	int update_flag = -1;
	char *update_dts_gpt = NULL;

	if (write_boot && !strcmp(write_boot, "1")) {
		printf("need to write boot0\n");
		rc = write_bootloader(1);
		if (rc) {
			printf("write boot0 fail, need to rollback!\n");
			update_rollback();
		} else {
			update_flag = update_gpt(0);
			printf("write boot0 success, need to reset!\n");
		}
	}

	update_dts_gpt = env_get("update_dts_gpt");

	if (rc == 0 && update_dts_gpt && !strcmp(update_dts_gpt, "1")) {
		printf("update from dts to gpt\n");
		update_flag = update_gpt(1);
		env_set("update_dts_gpt", "0");
		env_set("dts_to_gpt", "1");
		run_command("saveenv", 0);
	}

	if ((write_boot && !strcmp(write_boot, "1")) || (update_flag != -1)) {
		printf("reset......\n");
		env_set("write_boot", "0");
		env_set("reboot_status", "reboot_next");
		env_set("expect_index", "1");
		env_set("update_env", "1");
		run_command("saveenv", 0);
		run_command("reset", 0);
	}

#ifdef CONFIG_MMC_MESON_GX
#ifdef CONFIG_CMD_BOOTCTOL_VAB
	if (mmc && is_android_image() && (!strcmp(rebootstatus, "reboot_next"))) {
		struct blk_desc *dev_desc = mmc_get_blk_desc(mmc);

		if (dev_desc && !strcmp(bootloaderindex, "0")) {
			printf("try to read gpt data from bootloader.img\n");
			update_flag = update_gpt(2);
			if (update_flag != -1) {
				printf("reset......\n");
				env_set("reboot_status", "reboot_next");
				env_set("expect_index", "1");
				env_set("update_env", "1");
#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
				run_command("update_env_part -p reboot_status;", 0);
				run_command("update_env_part -p expect_index;", 0);
				run_command("update_env_part -p update_env;", 0);
#else
				run_command("saveenv", 0);
#endif
				run_command("reset", 0);
			}
		}
	}
#endif

	char *fastboot_step = env_get("fastboot_step");

	if (mmc && fastboot_step && (strcmp(fastboot_step, "1") == 0)) {
		printf("reboot to new bootloader burned by fastboot\n");
		env_set("update_env", "1");
		env_set("fastboot_step", "2");
		run_command("saveenv", 0);
		if (rebootmode && (strcmp(rebootmode, "fastboot") == 0))
			run_command("reboot next,bootloader", 0);
		else
			run_command("reboot next", 0);
	}
	if (mmc && fastboot_step && (strcmp(fastboot_step, "2") == 0)) {
		struct blk_desc *dev_desc = mmc_get_blk_desc(mmc);

		if (dev_desc && ((ret == 0 && !strcmp(bootloaderindex, "1")) ||
			(ret != 0 && !strcmp(bootloaderindex, "0")))) {
			printf("new bootloader error, please fastboot to another one\n");
			env_set("fastboot_step", "0");
#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
			run_command("update_env_part -p fastboot_step;", 0);
#else
			run_command("defenv_reserve;setenv fastboot_step 0;saveenv;", 0);
#endif

			cli_init();
			cli_loop();
		}
	}
#endif

	//no secure check need
	if (!strcmp(rebootstatus, "reboot_init")) {
		printf("rebootstatus is reboot_init, skip check\n");
		return -1;
	}

	//get expect index
	expect_index = env_get("expect_index");
	if (expect_index == NULL) {
		wrnP("can not get expect index, so skip secure check\n");
		return -1;
	}

	wrnP("expect_index is : %s\n", expect_index);

	match_flag = strcmp(bootloaderindex, expect_index);

	//ignore reboot next check if power off during reboot next to finish
	if (rebootmode && (strcmp(rebootmode, "cold_boot") == 0)) {
		if (!strcmp(rebootstatus, "reboot_finish")) {
			match_flag = 0;
		}
	}


	//first reboot, command from recovery, need reboot next
	if (!strcmp(rebootstatus,"reboot_next")) {
		wrnP("--secure check reboot_next---\n");
		//bootloader index, expect == current, no need reboot next
		if (match_flag == 0) {
			wrnP("current index is expect, no need reboot next, run cache recovery\n");
			if (has_boot_slot == 1) {
				wrnP("ab mode\n");
			} else {
				run_recovery_from_cache();
				return 0;
			}
		} else {
			wrnP("now ready start reboot next\n");
			if (has_boot_slot == 1) {
#ifdef CONFIG_MMC_MESON_GX
				if (mmc != NULL)
					gpt_flag = aml_gpt_valid(mmc);
#endif
				if (gpt_flag == 0)
					ret = 0;

#if defined(CONFIG_EFUSE_OBJ_API) && defined(CONFIG_CMD_EFUSE)
				run_command("efuse_obj get FEAT_DISABLE_EMMC_USER", 0);

				if (*efuse_field.data == 1)
					ret = 0;
#endif//#ifdef CONFIG_EFUSE_OBJ_API
				if (ret == 0) {
					wrnP("gpt or disable user bootloader mode, write boot1 to boot0\n");
					write_bootloader_back("2", 1);
#ifdef CONFIG_FASTBOOT
					struct misc_virtual_ab_message message;

					set_mergestatus_cancel(&message);
#endif
					char *slot;

					slot = env_get("slot-suffixes");
					if (!slot) {
						run_command("get_valid_slot", 0);
						slot = env_get("slot-suffixes");
					}
					if (strcmp(slot, "0") == 0) {
						wrnP("back to slot b\n");
						run_command("set_roll_flag 1", 0);
						run_command("set_active_slot b", 0);
					} else if (strcmp(slot, "1") == 0) {
						wrnP("back to slot a\n");
						run_command("set_roll_flag 1", 0);
						run_command("set_active_slot a", 0);
					}
					env_set("expect_index", "1");
				} else {
					write_bootloader_back(bootloaderindex, 0);
#ifdef CONFIG_FASTBOOT
					struct misc_virtual_ab_message message;
					set_mergestatus_cancel(&message);
#endif
					if (strcmp(bootloaderindex, "1") == 0) {
						wrnP("back to slot a\n");
						run_command("set_roll_flag 1", 0);
						run_command("set_active_slot a", 0);
					} else if (strcmp(bootloaderindex, "2") == 0) {
						wrnP("back to slot b\n");
						run_command("set_roll_flag 1", 0);
						run_command("set_active_slot b", 0);
					}
					env_set("expect_index", "0");
				}

				env_set("update_env", "1");
				env_set("reboot_status", "reboot_next");
				run_command("saveenv", 0);
				run_command("reset", 0);
			} else {
				env_set("reboot_status","reboot_finish");
				run_command("saveenv", 0);
				run_command("get_rebootmode", 0);
				run_command("if test ${reboot_mode} = quiescent; then reboot next,quiescent; else reboot next; fi;", 0);
			}
			return 0;
		}
	} else if (!strcmp(rebootstatus,"reboot_finish")) {//second reboot, reboot next from uboot
		wrnP("--secure check reboot_finish---\n");
		env_set("reboot_status","reboot_end");
		run_command("saveenv", 0);

		if (match_flag == 0) {
			wrnP("reboot next succ, bootloader secure check pass......\n");
			if (has_boot_slot == 1) {
				printf("ab mode, default all uboot env\n");
				update_env = env_get("update_env");
				if (!update_env) {
					errorP("can not get update_env\n");
					return -1;
				}
				if (strcmp(update_env, "1") == 0) {
					printf("ab mode, default all uboot env\n");
					run_command("defenv_reserv;saveenv;", 0);
					run_command("run bcb_cmd", 0);
					env_set("update_env","0");

					if (strcmp(bootloaderindex, "2") == 0) {
						wrnP("rom always boot as boot0--> boot1\n");
						wrnP("So if boot1 is ok, write it to boot0\n");
						run_command("copy_slot_bootable 2 1", 0);
					}
					run_command("saveenv", 0);
				}
			} else {
				run_recovery_from_cache();
				return 0;
			}
		} else {
			//bootloader check failed, run recovery show error
			wrnP("reboot next fail, bootloader secure check fail(curr:%s, expect:%s)......\n",bootloaderindex, expect_index);
			env_set("check_result","bootloader_fail");
			run_command("saveenv", 0);
			if (has_boot_slot == 1) {
				wrnP("ab mode\n");
#ifdef CONFIG_FASTBOOT
				struct misc_virtual_ab_message message;
				set_mergestatus_cancel(&message);
#endif
				if (strcmp(bootloaderindex, "1") == 0) {
					wrnP("back to slot a\n");
					run_command("set_roll_flag 1", 0);
					run_command("set_active_slot a", 0);
				} else if (strcmp(bootloaderindex, "2") == 0) {
					wrnP("back to slot b\n");
					run_command("set_roll_flag 1", 0);
					run_command("set_active_slot b", 0);
				}

#ifdef CONFIG_MMC_MESON_GX
				if (mmc != NULL)
					gpt_flag = aml_gpt_valid(mmc);
#endif

				if (gpt_flag == 0)
					ret = 0;

#if defined(CONFIG_EFUSE_OBJ_API) && defined(CONFIG_CMD_EFUSE)
				run_command("efuse_obj get FEAT_DISABLE_EMMC_USER", 0);

				if (*efuse_field.data == 1)
					ret = 0;
#endif//#ifdef CONFIG_EFUSE_OBJ_API

				if (ret == 0) {
					wrnP("gpt or disable user bootloader mode\n");
					env_set("update_env","0");
					env_set("reboot_status","reboot_init");
					run_command("saveenv", 0);
				} else {
					write_bootloader_back(bootloaderindex, 0);
					env_set("update_env","1");
					env_set("reboot_status","reboot_next");
					env_set("expect_index","0");
					run_command("saveenv", 0);
					run_command("reset", 0);
				}

			} else {
				run_recovery_from_flash();
				return 0;
			}
		}
	} else if (!strcmp(rebootstatus,"reboot_recovery")) {
			//recovery check failed, run recovery show error
			wrnP("--secure check reboot_recovery---\n");
			env_set("reboot_status","reboot_end");
			run_command("saveenv", 0);
			run_recovery_from_flash();
			return 0;
	} else {
		env_set("check_result","succ");
	}

	return 0;
}


static int do_update_uboot_env(cmd_tbl_t *cmdtp,
	int flag,
	int argc,
	char * const argv[])
{
	const int write_boot = env_get_ulong("write_boot", 10, 0);
	const int update_dts_gpt = env_get_ulong("update_dts_gpt", 10, 0);

	//skip update env if need update bootloader
	if (write_boot || update_dts_gpt) {
		run_command("echo write_boot ${write_boot}, update_dts_gpt ${update_dts_gpt}", 0);
		return 0;
	}

	//default uboot env need before anyone use it
	//after factory reset
	if (env_get("default_env")) {
		printf("factory reset, need default all uboot env.\n");
		run_command("defenv_reserv; setenv upgrade_step 2; saveenv;", 0);
	}

	//after ab update
	char *update_env = env_get("update_env");

	if (update_env && (strcmp(update_env, "1") == 0)) {
		printf("ab mode, default all uboot env\n");
		run_command("defenv_reserv; setenv upgrade_step 2; saveenv;", 0);
	}

	//after recovery update
	char *upgrade_step = env_get("upgrade_step");

	if (upgrade_step && (strcmp(upgrade_step, "1") == 0)) {
		printf("after recovery update, need update uboot env\n");
		run_command("defenv_reserv; setenv upgrade_step 2; saveenv;", 0);
	}

	return 0;
}

U_BOOT_CMD_COMPLETE(
	amlbootsta, 3, 0,	do_get_bootloader_status,
	"get bootloader status in env",
	"[-p] print bootloader status\n"
	"[-s] saveenv after generate bootloader status\n",
	var_complete
);

U_BOOT_CMD_COMPLETE(
	amlsecurecheck, 1, 0,	do_secureboot_check,
	"try bootloader/dtb/recovery secure check",
	""
	"",
	var_complete
);

U_BOOT_CMD(aml_update_env,
	1,	0, do_update_uboot_env,
	"aml_update_env",
	"\nThis command will update uboot env\n"
	"So you can execute command: aml_update_env"
);


