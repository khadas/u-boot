/*
* Copyright (C) 2017 Amlogic, Inc. All rights reserved.
* *
This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* *
This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
* *
You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
* *
Description:
*/

#include "../v2_burning_i.h"
#include "../v2_sdc_burn/optimus_sdc_burn_i.h"
#include "../v2_sdc_burn/optimus_led.h"

#ifndef CONFIG_AML_SYS_RECOVERY_CLEAR_USR_DATA
#define CONFIG_AML_SYS_RECOVERY_CLEAR_USR_DATA  "data"
#endif// #ifndef CONFIG_AML_SYS_RECOVERY_CLEAR_USR_DATA
extern int sdc_burn_aml_keys(HIMAGE hImg, const int keyOverWrite, int licenseKey, int imgKey);

static int optimus_sysrec_check_whole_img_before_burn(const char* partName)
{
        //TODO:
        return 0;
}

#ifdef CONFIG_AML_SYS_RECOVERY_CLEAR_USR_DATA
//clear data parts then the parts will formatted when firtsboot
//As fill half parttition need so much time, I just clear 2M
static int optimus_sysrec_clear_usr_data_parts(void)
{
        const char* const _usrDataParts[] = {CONFIG_AML_SYS_RECOVERY_CLEAR_USR_DATA,};
        const int   dataPartsNum          = sizeof(_usrDataParts)/sizeof(const char*);
        int partIndex = 0;
        int ret = 0;
        char cmdbuf[128];

        for (partIndex = 0; partIndex < dataPartsNum; ++partIndex)
        {
            sprintf(cmdbuf, "store erase partition %s", _usrDataParts[partIndex]);
            ret = run_command(cmdbuf, 0);
            if (ret) { DWN_WRN("Wrn:failed in cmd:%s\n", cmdbuf); }
        }

        return ret;
}
#endif//#ifdef CONFIG_AML_SYS_RECOVERY_CLEAR_USR_DATA

/*
 *.partName: aml_sysrecovery
 *.needVerify: 1 then verify partitions that contain verify file; 0 then not to verify for faster burning
 */
static int optimus_sysrec_burn_package_from_partition(const char* partName, const unsigned needVerifyWhileBurn,
                const unsigned verifyPackageBeforeBurn)
{
        extern ConfigPara_t g_sdcBurnPara ;
        ConfigPara_t* pSdcCfgPara = &g_sdcBurnPara;
        __hdle hUiProgress = NULL;
        HIMAGE hImg = NULL;
        int ret = 0;
        int hasBootloader = 0;
        u64 datapartsSz = 0;

        if (verifyPackageBeforeBurn)
        {
                ret = optimus_sysrec_check_whole_img_before_burn(partName);
                if (ret) {
                        DWN_ERR("Failed in crc check the burning package.\n");
                        return __LINE__;
                }
        }

        hImg = image_open("store", "0", AML_SYS_RECOVERY_PART, "");
        if (!hImg) {
                DWN_ERR("Fail to open image in part %s\n", AML_SYS_RECOVERY_PART);
                ret = __LINE__; goto _finish;
        }

        if (video_res_prepare_for_upgrade(hImg)) {
                DWN_ERR("Fail when prepare bm res or init video for upgrade\n");
                ret = __LINE__; goto _finish;
        }
        show_logo_to_report_burning();

        //update dtb for keyman
        ret = optimus_sdc_burn_dtb_load(hImg);
        if (ITEM_NOT_EXIST != ret && ret) {
            DWN_ERR("Fail in load dtb for sdc_burn\n");
            ret = __LINE__; goto _finish;
        }
        optimus_storage_init(0);

        hUiProgress = optimus_progress_ui_request_for_sdc_burn();
        if (!hUiProgress) {
                DWN_ERR("request progress handle failed!\n");
                ret = __LINE__; goto _finish;
        }
        optimus_progress_ui_direct_update_progress(hUiProgress, UPGRADE_STEPS_AFTER_DISK_INIT_OK);

        datapartsSz = optimus_img_decoder_get_data_parts_size(hImg, &hasBootloader);
        DWN_MSG("datapartsSz=[%8u]MB\n", (unsigned)(datapartsSz >> 20));
        ret = optimus_progress_ui_set_smart_mode(hUiProgress, datapartsSz,
                        UPGRADE_STEPS_FOR_BURN_DATA_PARTS_IN_PKG(!pSdcCfgPara->burnEx.bitsMap.mediaPath));
        if (ret) {
                DWN_ERR("Fail to set smart mode\n");
                ret = __LINE__; goto _finish;
        }

        pSdcCfgPara->burnParts.burn_num = 0;
        ret = optimus_sdc_burn_partitions(pSdcCfgPara, hImg, hUiProgress, needVerifyWhileBurn);
        if (ret) {
                DWN_ERR("Fail when burn partitions\n");
                ret = __LINE__; goto _finish;
        }

        optimus_progress_ui_direct_update_progress(hUiProgress, UPGRADE_STPES_AFTER_BURN_DATA_PARTS_OK);

#if 1
        if (hasBootloader)
        {//burn bootloader
                ret = optimus_burn_bootlader(hImg);
                if (ret) {
                        DWN_ERR("Fail in burn bootloader\n");
                        goto _finish;
                }
                ret = optimus_set_burn_complete_flag();
                if (ret) {
                        DWN_ERR("Fail in set_burn_complete_flag\n");
                        ret = __LINE__; goto _finish;
                }
        }
#endif
#if CONFIG_SUPPORT_SDC_KEYBURN
        ret = sdc_burn_aml_keys(hImg, 0, 0, 1);
        if (ret) {
            DWN_ERR("Fail in recovery keys from img\n");
            ret = __LINE__; goto _finish;
        }
#endif// #if CONFIG_SUPPORT_SDC_KEYBURN
#ifdef CONFIG_AML_SYS_RECOVERY_CLEAR_USR_DATA
        optimus_sysrec_clear_usr_data_parts();
#endif// #ifdef CONFIG_AML_SYS_RECOVERY_CLEAR_USR_DATA

        optimus_progress_ui_direct_update_progress(hUiProgress, UPGRADE_STEPS_AFTER_BURN_BOOTLOADER_OK);

_finish:
        image_close(hImg);
        if (hUiProgress) optimus_progress_ui_report_upgrade_stat(hUiProgress, !ret);
        optimus_report_burn_complete_sta(ret, 1/*pSdcCfgPara->custom.rebootAfterBurn*/);
        if (hUiProgress) optimus_progress_ui_release(hUiProgress);
        //optimus_storage_exit();//temporary not exit storage driver when failed as may continue burning after burn
        return ret;
}

static int do_aml_sysrecovery(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int rcode = 0;
    unsigned needVerify         = (1 < argc) ? simple_strtoul(argv[1], NULL, 0) : 0;
    unsigned verifyPackageBeforeBurn = (2 < argc) ? simple_strtoul(argv[2], NULL, 0) : 0;

    if (argc < 2 ) {
        cmd_usage(cmdtp);
        return __LINE__;
    }

    show_logo_to_report_burning();//indicate enter flow of burning! when 'run update'
    if (optimus_led_open(LED_TYPE_PWM)) {
        DWN_ERR("Fail to open led for burn\n");
        return __LINE__;
    }
    optimus_led_show_in_process_of_burning();

    optimus_work_mode_set(OPTIMUS_WORK_MODE_SYS_RECOVERY);
    optimus_buf_manager_init(16*1024);
    rcode = optimus_sysrec_burn_package_from_partition(AML_SYS_RECOVERY_PART, needVerify, verifyPackageBeforeBurn);

    return rcode;
}

U_BOOT_CMD(
   aml_sysrecovery,     //command name
   3,                   //maxargs
   0,                   //repeatable
   do_aml_sysrecovery,         //command function
   "Burning with amlogic format package from partition sysrecovery",           //description
   "argv: needVerify [,checkWholeImgBeforeBurn]\n"//usage
   "    --@needVerify: 0 then skip to verify the partition even have verify file."
   "    --@checkWholeImgBeforeBurn: 1 then crc32 check the burn package in partition sysrecovery before actual burn"
   "   eg:'aml_sysrecovery 0': burn from partition aml_sysrecovery without verify"
);

