/*
 * \file        aml_v3_burning.c
 * \brief       v3 burning producing case entry
 *
 * \version     1.0.0
 * \date        2018年12月20日
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2018 Amlogic. All Rights Reserved.
 *
 */
#include "include/v3_tool_def.h"
#include <mmc.h>
#ifndef BOOT_DEVICE_USB
#define BOOT_DEVICE_SD                  4
#define BOOT_DEVICE_USB                 5
#endif// #ifndef BOOT_DEVICE_USB
extern void serial_initialize(void);

extern void board_init_mem(void);

static unsigned _get_romcode_boot_id(void)
{
	FB_DBG("cfg0 0x%08x\n", readl(P_AO_SEC_GP_CFG0));
    const unsigned boot_id = readl(P_AO_SEC_GP_CFG0) & 0xf;

    return boot_id;
}

static int is_boot_device_usb(void)
{
    return BOOT_DEVICE_USB == _get_romcode_boot_id();
}

static int is_bl1_usb_protocol_DNL(void)
{
#ifdef SYSCTRL_SEC_STATUS_REG1
    const unsigned cfg9 = readl(SYSCTRL_SEC_STATUS_REG1);
    FB_DBG("SYSCTRL_SEC_STATUS_REG1 0x%x\n", cfg9);
    return !(cfg9 & (1U<<12));
#else
    const unsigned cfg9 = readl(P_AO_SEC_SD_CFG9);
    FB_DBG("cfg9 0x%x\n", cfg9);
    return cfg9 & (1U<<15);
#endif// #ifdef SYSCTRL_SEC_STATUS_REG1
}

int aml_v3_factory_usb_burning(int flag, bd_t* bis)
{
    if (!is_boot_device_usb()) return 1;
    if (!is_bl1_usb_protocol_DNL()) return 1;

    bis = bis;//avoid compiling warnning
    if ( !flag ) {
        serial_initialize();//init for write memory
#ifdef CONFIG_GENERIC_MMC
        FB_MSG("MMC init for dnl\n");
        mmc_initialize(bis);
#endif
        set_default_env(NULL, 0);
        board_init_mem();
    }
#ifdef CONFIG_SILENT_CONSOLE
    /* enable console output */
    gd->flags &= ~GD_FLG_SILENT;
#endif
    //pull down and sleep in bl2-->tpl,
    //to improve pc compatibility
    f_dwc_otg_pullup(0);
    udelay(2*1000*1000);

    v3tool_work_mode_set(V3TOOL_WORK_MODE_USB_PRODUCE);
    optimus_clear_ovd_register();//clear OVD register for normal reboot
    return run_command("adnl", 0);
}

