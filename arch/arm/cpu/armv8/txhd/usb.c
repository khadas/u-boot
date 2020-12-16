/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/txhd/usb.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

//#include <common.h>
//#include <asm/cache.h>
#include <asm/arch/usb-new.h>
#include <asm/arch/romboot.h>


static struct amlogic_usb_config * g_usb_cfg[BOARD_USB_MODE_MAX][USB_PHY_PORT_MAX];

struct amlogic_usb_config * board_usb_start(int mode,int index)
{
	printf("USB3.0 XHCI init start\n");

	if (mode < 0 || mode >= BOARD_USB_MODE_MAX||!g_usb_cfg[mode][index])
		return 0;

	writel((1 << 2),P_RESET1_REGISTER);

	return g_usb_cfg[mode][index];
}

int board_usb_stop(int mode,int index)
{
	printf("board_usb_stop cfg: %d\n",mode);

	return 0;
}

int usb_index = 0;
void board_usb_init(struct amlogic_usb_config * usb_cfg,int mode)
{
	if (mode < 0 || mode >= BOARD_USB_MODE_MAX || !usb_cfg)
		return ;

	if (mode == BOARD_USB_MODE_HOST) {
		if (usb_index >= USB_PHY_PORT_MAX)
			return;
		g_usb_cfg[mode][usb_index] = usb_cfg;
		usb_index++;
	} else
		g_usb_cfg[mode][0] = usb_cfg;
	printf("register usb cfg[%d][%d] = %p\n",mode,(mode==BOARD_USB_MODE_HOST)?usb_index:0,usb_cfg);
}

int get_usb_count(void)
{
    return  usb_index;
}
