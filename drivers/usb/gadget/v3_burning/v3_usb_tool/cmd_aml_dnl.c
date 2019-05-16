/*
 * \file        cmd_usb_tool.c
 * \brief       amlogic DNL protocl entry
 *
 * \version     1.0.0
 * \date        2018/12/17
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2018 Amlogic. All Rights Reserved.
 *
 */
#include <common.h>
#include <command.h>
#include <console.h>
#include <g_dnl.h>

extern int aml_dnl_register(const char *name);
extern void aml_dnl_unregister(void);

static int do_aml_DNL(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
    int ret;

    ret = aml_dnl_register("usb_dnl_amlogic");
    if (ret)
        return ret;

    while (1) {
        if (ctrlc())
            break;
        usb_gadget_handle_interrupts(0);
    }

    aml_dnl_unregister();
    return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
        adnl,	1,	0,	do_aml_DNL,
        "use Amlogic DNL protocol",
        "\n"
        "    - run as a Amlogic DNL device"
);

