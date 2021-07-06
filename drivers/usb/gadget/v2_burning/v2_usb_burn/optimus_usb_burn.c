// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include "../v2_sdc_burn/optimus_sdc_burn_i.h"
#include "../v2_sdc_burn/optimus_led.h"
#include <dm/pinctrl.h>
#include <emmc_partitions.h>
#include <amlogic/aml_mmc.h>
#include <amlogic/storage.h>
#define BLOCK_SIZE 512

extern int optimus_burn_with_cfg_file(const char* cfgFile);

int usb_burn_erase_data(unsigned char init_flag)
{
	int ret = 0, i;
	struct mmc *mmc;
	struct partitions *part_info = NULL;

	init_flag = init_flag;

	mmc = find_mmc_device(1);
	if (!mmc) {
		printf("[%s]  no mmc devices available\n", __func__);
		return -1;
	}

	mmc->has_init = 0;
	pinctrl_select_state(mmc->dev, "default");

	ret = mmc_init(mmc);
	if (ret != 0) {
		DWN_ERR("mmc init failed, ret %d\n", ret);
		return -__LINE__;
	}

	for (i = 0; ; i++) {
		part_info = get_partition_info_by_num(i);
		if (!part_info) {
			break;
		}
		if (!strcmp("reserved", part_info->name)
				|| !strcmp("bootloader", part_info->name)) {
			printf("Part:%s is skiped\n", part_info->name);
			continue;
		}
		if (part_info->size == 0) {
			printf("Part:%s size is 0\n", part_info->name);
			continue;
		}
		if (part_info->mask_flags & PART_PROTECT_FLAG) {
			printf("Part:%s is protected\n", part_info->name);
			continue;
		}
		ret = blk_derase(mmc_get_blk_desc(mmc),
				part_info->offset / BLOCK_SIZE,
				part_info->size / BLOCK_SIZE);
		printf("Erased: %s %s\n",
				part_info->name,
				(ret == 0) ? "OK" : "ERR");
	}
	return 0;
}

// added by scy
int optimus_burn_package_in_usb(const char* sdc_cfg_file)
{
    int rcode = 0;

    DWN_MSG("usb start\n");
    rcode = run_command("usb start", 0);
    if (rcode) {
        DWN_ERR("Fail in init usb host, Does usb host not plugged in?\n");
        return __LINE__;
    }

#if 1//this asserted by 'run update' and 'aml_check_is_ready_for_sdc_produce'
    rcode = do_fat_get_fileSz(sdc_cfg_file);
    if (!rcode) {
        DWN_ERR("The [%s] not exist in udisk\n", sdc_cfg_file);
        return __LINE__;
    }
#endif//#if 0

    rcode = optimus_device_probe("usb", "0");
    if (rcode) {
        DWN_ERR("Fail to detect device usb 0\n");
        return __LINE__;
    }

    rcode = optimus_burn_with_cfg_file(sdc_cfg_file);

    return rcode;
}


// added by scy
int do_usb_burn(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    int rcode = 0;
    const char* sdc_cfg_file = argv[1];

    setenv("usb_update","1");

    if (argc < 2 ) {
        cmd_usage(cmdtp);
        return __LINE__;
    }

    optimus_work_mode_set(OPTIMUS_WORK_MODE_UDISK_PRODUCE);
    show_logo_to_report_burning();//indicate enter flow of burning! when 'run update'
    if (optimus_led_open(LED_TYPE_PWM)) {
        DWN_ERR("Fail to open led for burn\n");
        return __LINE__;
    }
    optimus_led_show_in_process_of_burning();

    rcode = optimus_burn_package_in_usb(sdc_cfg_file);

    return rcode;
}

// added by scy
U_BOOT_CMD(
   usb_burn,      //command name
   5,               //maxargs
   0,               //repeatable
   do_usb_burn,   //command function
   "Burning with amlogic format package in usb ",           //description
   "argv: [sdc_burn_cfg_file]\n"//usage
   "    -aml_sdc_burn.ini is usually used configure file\n"
);
