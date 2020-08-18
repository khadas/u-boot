/*
 * \file        bootloader_status.c
 * \brief       Generate bootloader status in env for upgrade
 *
 * \version     1.0.0
 * \date        08/18/2020
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2020 Amlogic. All Rights Reserved.
 *
 */
#include <config.h>
#include <common.h>
#include <linux/kernel.h>
#include <amlogic/aml_efuse.h>
#include <amlogic/cpu_id.h>
#include <amlogic/storage.h>

#ifndef IS_FEAT_BOOT_VERIFY
#define IS_FEAT_BOOT_VERIFY() 0
#endif// #ifndef IS_FEAT_BOOT_VERIFY
int __attribute__((weak)) store_logic_read(const char *name, loff_t off, size_t size, void *buf)
{ return store_read(name, off, size, buf);}

typedef struct andr_img_hdr boot_img_hdr;

#define debugP(fmt...) //printf("[DbgBootSta]L%d:", __LINE__),printf(fmt)
#define errorP(fmt...) printf("ErrBootSta(L%d):", __LINE__),printf(fmt)
#define wrnP(fmt...)   printf("wrn:"fmt)
#define MsgP(fmt...)   printf("[BootSta]"fmt)

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
		case 0x32:
			supportRobustOta = true;
			break;
		default:break;
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

	if (saveenv) run_command("saveenv", 0);

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD_COMPLETE(
	amlbootsta, 3, 0,	do_get_bootloader_status,
	"get bootloader status in env",
	"[-p] print bootloader status\n"
	"[-s] saveenv after generate bootloader status\n",
	var_complete
);


