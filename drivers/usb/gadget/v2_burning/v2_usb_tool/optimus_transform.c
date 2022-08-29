/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/usb/gadget/v2_burning/v2_usb_tool/optimus_transform.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "../v2_burning_i.h"
#include "usb_pcd.h"
#include "platform.h"
#include <partition_table.h>
#include <asm/cpu_id.h>
#include <amlogic/aml_efuse.h>

#define MYDBG(fmt ...) printf("OPT]"fmt)

#ifdef CONFIG_CMD_AML
#define USB_BURN_POWER_CONTROL  1
#endif// #ifdef CONFIG_CMD_AML

static inline int str2long(const char *p, unsigned long *num)
{
	char *endptr;
	*num = simple_strtoul(p, &endptr, 0);
	return (*p != '\0' && *endptr == '\0') ? 1 : 0;
}

static int opimus_func_write_bootloader(unsigned long addr)
{
        int ret = 0;
        loff_t size = 0;

        size = 0x60000;//FIXME: 256K at most ??

        ret = store_boot_write((u8*)addr, (loff_t)0, size);

        return ret;
}

//[0]write_raw_img [1]part_name [2]address, [3]offset, [4]size
int opimus_func_write_raw_img(int argc, char *argv[], char *info)
{
        int ret = 0;
        u64 addr;
        u64 off, size;
        const char* partName = argv[1];

        if (strcmp(partName, "bootloader") == 0)
        {
                addr = simple_strtoul(argv[2], NULL, 0);
                return opimus_func_write_bootloader(addr);
        }

        addr = simple_strtoull(argv[2], NULL, 0);
        off  = simple_strtoull(argv[3], NULL, 0);
        size = simple_strtoul(argv[4], NULL, 0);

        printf("write_raw_img part %s offset 0x%x, size 0x%x, addr 0x%llx\n", argv[1], (u32)off, (u32)size, addr);
        ret = store_write_ops((u8*)partName, (u8*)addr, off, size);

        return ret;
}


int optimus_simg2part (int argc, char * const argv[], char *info)
{
        int ret = -1;
        const char* partition_name = argv[1];
        u8* simg_addr = (u8*)simple_strtoul(argv[2], NULL, 16);
        u32 pktSz     = simple_strtoul(argv[3], NULL, 0);
        const unsigned memAddrTop = OPTIMUS_DOWNLOAD_SPARSE_INFO_FOR_VERIFY;//this address backup the chunk info, don't overwrite it!

        if (argc < 4) {
                sprintf(info, "failed: used simg2part partName memAddr, pktSz]\n");
                DWN_ERR(info);
                return __LINE__;
        }
        if (!pktSz || !simg_addr) {
                sprintf(info, "simg_addr or pktSz error\n");
                DWN_ERR(info);
                return __LINE__;
        }

        if ((unsigned long)simg_addr + pktSz > memAddrTop) {
                sprintf(info, "failed:simg_addr(0x%p) + pktSz(0x%x) > memAddrTop(0x%x)\n", simg_addr, pktSz, memAddrTop);
                DWN_ERR(info);
                return __LINE__;
        }

        ret = optimus_parse_img_download_info(partition_name, pktSz, "sparse", "store", 0);
        if (ret) {
                sprintf(info, "failed:init download info for part(%s)\n", partition_name);
                DWN_ERR(info);
                return __LINE__;
        }
        unsigned writeLen = optimus_download_img_data(simg_addr, pktSz, info);
        if (writeLen != pktSz) {
                DWN_ERR("failed when burn simg!, want(0x%x), write(0x%x)\n", pktSz, writeLen);
                return __LINE__;
        }

        return ret;
}

int optimus_mem_md (int argc, char * const argv[], char *info)
{
	ulong	addr, length = 0x100;
	int	size;
	int rc = 0;

	if ((size = cmd_get_data_size(argv[0], 4)) < 0)
		return 1;

	/* Address is specified since argc > 1
	*/
	addr = simple_strtoul(argv[1], NULL, 16);

	/* If another parameter, it is the length to display.
	 * Length is the number of objects, not number of bytes.
	 */
	if (argc > 2)
		length = simple_strtoul(argv[2], NULL, 16);

	/* Print the lines. */
	print_buffer(addr, (void*)addr, size, length, 16/size);
	strcpy(info, "success");
	return rc;
}

int set_low_power_for_usb_burn(int arg, char* buff)
{
    if (OPTIMUS_WORK_MODE_USB_PRODUCE == optimus_work_mode_get()) {
        return 0;//just return ok as usb producing mode as LCD not initialized yet!
    }

#if defined(CONFIG_VIDEO_AMLLCD)
    int ret1=0;
    //axp to low power off LCD, no-charging
    MYDBG("To close LCD\n");
    ret1 = run_command("video dev disable", 0);
    if (ret1) {
        if (buff) sprintf(buff, "Fail to close back light") ;
        printf("Fail to close back light\n");
        /*return __LINE__;*/
    }
#endif// #if defined(CONFIG_VIDEO_AMLLCD)

#if USB_BURN_POWER_CONTROL
    int ret2=0;
    //limit vbus current to 500mA, i.e, if hub is 4A, 8 devices at most, arg3 to not set_env as it's not inited yet!!
    MYDBG("set_usbcur_limit 500 0\n");
    ret2 = run_command("set_usbcur_limit 500 0", 0);
    if (ret2) {
        if (buff) sprintf(buff, "Fail to set_usb_cur_limit") ;
        printf("Fail to set_usb_cur_limit\n");
        return __LINE__;
    }
#endif//#if USB_BURN_POWER_CONTROL

    return 0;
}

int cb_4_dis_connect_intr(void)
{
    if (optimus_burn_complete(OPTIMUS_BURN_COMPLETE__QUERY))
    {
        close_usb_phy_clock(0);//disconnect to avoid k200 platfrom which can't relally poweroff
        DWN_MSG("User Want poweroff after disconnect\n");
        optimus_poweroff();
    }

    return 0;
}

static int _cpu_temp_in_valid_range(int argc, char* argv[], char* errInfo)
{
        int ret = 0;
        int minTemp = 0;
        int maxTemp = 0;
        unsigned long env_cpu_temp = 0;

        if (3 > argc) {
                sprintf(errInfo, "argc %d < 3 is invalid\n", argc);
                return __LINE__;
        }
        minTemp = simple_strtol(argv[1], NULL, 0);
        maxTemp = simple_strtol(argv[2], NULL, 0);
        if (minTemp <=0 || maxTemp <= 0 || minTemp >= maxTemp) {
                sprintf(errInfo, "Invalid:minTemp=%d, maxTemp=%d\n", minTemp, maxTemp);
                return __LINE__;
        }
        ret = run_command("read_temp", 0);
        if (ret < 0) {
                sprintf(errInfo, "cmd[cpu_temp] failed\n");
                return __LINE__;
        }

        env_cpu_temp = getenv_ulong("tempa", 10, 0);
        ret = (env_cpu_temp >= minTemp && env_cpu_temp <= maxTemp) ? 0 : __LINE__;
        if (!ret) {
                sprintf(errInfo, "%lu", env_cpu_temp);
        }
        else{
                sprintf(errInfo, "%lu is out of temp range[%d, %d], errInfo[%s]\n",
                        env_cpu_temp, minTemp, maxTemp, getenv_optimus("err_info_tempa"));
        }

        return ret;
}

static int _get_chipid(char* buff)
{
    int i = 0;
    unsigned char chipid[16];
    int ret = get_chip_id(chipid, 16);
    if ( ret ) {
        DWN_ERR("_get_chipid %d", ret);
        return ret;
    }

    buff[0] = ':';
    for (; i < 12; ++i) {
        sprintf(buff + 1 + i*2, "%02x", chipid[15-i]);
    }
    buff[1 + i*2] = '\0';
    return 0;
}

int optimus_working (const char *cmd, char* buff)
{
        static char cmdBuf[CMD_BUFF_SIZE] = {0};
        int ret = 0;
        int argc = 33;
        char *argv[CONFIG_SYS_MAXARGS + 1];	/* NULL terminated	*/
        /*printf("reboot_mode [%8x, %8x]\n", P_AO_RTI_STATUS_REG1);*/
        const char* optCmd = NULL;

        memset(buff, 0, CMD_BUFF_SIZE);
        memcpy(cmdBuf, cmd, CMD_BUFF_SIZE);
        if ((argc = cli_simple_parse_line(cmdBuf, argv)) == 0)
        {
                strcpy(buff, "failed:no command at all");
                printf("no command at all\n");
                return -1;	/* no command at all */
        }
        optCmd = argv[0];

        if (!strcmp("low_power", optCmd))
        {
                ret = set_low_power_for_usb_burn(1, buff);
        }
        else if(strcmp(optCmd, "disk_initial") == 0)
        {
                unsigned  erase = argc > 1 ? simple_strtoul(argv[1], NULL, 0) : 0;

                ret = optimus_storage_init(erase);
        }
        else if(!strcmp(optCmd, "bootloader_is_old"))
        {
                ret = is_tpl_loaded_from_usb();
                if (ret)sprintf(buff, "Failed, bootloader is new\n") ;
        }
        else if(!strcmp(optCmd, "erase_bootloader"))
        {
                ret = optimus_erase_bootloader("usb");

                if (ret)sprintf(buff, "Failed to erase bootloader\n") ;
        }
        else if(strcmp(optCmd, "write_raw_img") == 0)
        {
                ret = opimus_func_write_raw_img(argc, argv, buff);
        }
        else if(strcmp(optCmd, "simg2part") == 0)
        {
                ret = optimus_simg2part(argc, argv, buff);
        }
        else if(strcmp(optCmd, "reset") == 0)
        {
                close_usb_phy_clock(0);
                optimus_reset(OPTIMUS_BURN_COMPLETE__REBOOT_NORMAL);
        }
        else if(strcmp(optCmd, "poweroff") == 0)
        {
                optimus_poweroff();
        }
        else if(strncmp(optCmd, "md", 2) == 0)
        {
                ret = optimus_mem_md(argc, argv, buff);
        }
        else if(!strcmp(optCmd, "download") || !strcmp("upload", optCmd))
        {
                ret = optimus_parse_download_cmd(argc, argv);
        }
        else if(!strcmp("key", optCmd))
        {
                ret = v2_key_command(argc, argv, buff);
        }
        else if(!strcmp("verify", optCmd))
        {
                ret = optimus_media_download_verify(argc, argv, buff);
        }
        else if(!strcmp("save_setting", optCmd))
        {
                ret = optimus_set_burn_complete_flag();
        }
        else if(!strcmp("burn_complete", optCmd))
        {
                unsigned choice = simple_strtoul(argv[1], NULL, 0);//0 is poweroff, 1 is reset system

                if (OPTIMUS_BURN_COMPLETE__POWEROFF_AFTER_DISCONNECT != choice) {//disconnect except OPTIMUS_BURN_COMPLETE__POWEROFF_AFTER_DISCONNECT
                        close_usb_phy_clock(0);//some platform can't poweroff but dis-connect needed by pc
                }
                ret = optimus_burn_complete(choice);
        }
        else if(!strcmp(optCmd, "support_tempcontrol"))
        {
#ifndef CONFIG_CMD_CPU_TEMP
                ret = __LINE__;
                sprintf(buff + 7, "cpu temp control cmd NOT supported.\n");
#else
                ret = 0;
                sprintf(buff + 7, "cpu temp control cmd DO supported.\n");
#endif// #ifdef CONFIG_CMD_CPU_TEMP
        }
        else if(!strcmp(optCmd, "tempcontrol"))
        {
                ret = _cpu_temp_in_valid_range(argc, argv, buff + 7);
        }
        else if(!strcmp(optCmd, "get_chipid"))
        {
                ret = _get_chipid(buff + 7);
        } else if(!strcmp(optCmd, "secureboot_en")) {
                const bool secureboot = IS_FEAT_BOOT_VERIFY();
                strncpy(buff + 7, secureboot ? "secure" : "normal", 7);
                ret = 0;
        }
        else
        {
                int flag = 0;
                ret = run_command(cmd, flag);
                DWN_MSG("ret = %d\n", ret);
                /*ret = ret < 0 ? ret : 0;*/
        }

        if (ret)
        {
                memcpy(buff, "failed:", strlen("failed:"));//use memcpy but not strcpy to not overwrite storage/key info
        }
        else
        {
                memcpy(buff, "success", strlen("success"));//use memcpy but not strcpy to not overwrite storage/key info
        }

        printf("[info]%s\n",buff);
        mdelay(20);
        return ret;
}

