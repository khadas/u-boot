// SPDX-License-Identifier: GPL-2.0+
/*
 * Meson g12b, revB USB2 PHY driver
 *
 * Copyright (C) 2017 Martin Blumenstingl <martin.blumenstingl@googlemail.com>
 * Copyright (C) 2018 BayLibre, SAS
 * Author: Neil Armstrong <narmstron@baylibre.com>
 */

#include <common.h>
#include <asm/io.h>
#include <bitfield.h>
#include <dm.h>
#include <errno.h>
#include <generic-phy.h>
#include <regmap.h>
#include <power/regulator.h>
#include <clk.h>
#include <asm/arch/usb.h>
#include <amlogic/cpu_id.h>

#include <linux/compat.h>
#include <linux/ioport.h>
#include <asm-generic/gpio.h>

#define M31_2_RESET_LEVEL_BIT	8
#define	M31_1_RESET_LEVEL_BIT	9
#define	M31_0_RESET_LEVEL_BIT	10

#define M31_2_UTMI_RESET_LEVEL_BIT	0
#define	M31_1_UTMI_RESET_LEVEL_BIT	1
#define	M31_0_UTMI_RESET_LEVEL_BIT	2

#define CRG_U3DRD_0_RESET_LEVEL_BIT 6
#define CRG_U3DRD_1_RESET_LEVEL_BIT 5
#define CRG_U3DRD_2_RESET_LEVEL_BIT 3

#define	USB_RESET_BIT			4

#define RESET_BASE 0xFE002000

#define M31_SETTING 0x1E30CEB9
#define M31_PHY_DEVICE_SETTING 0x1E30CE89
#define M31_PHY_0_BASE  0XFE02A000
#define M31_PHY_1_BASE  0XFE076000
#define M31_PHY_2_BASE  0XFE074000

void usb_reset(unsigned int reset_addr, int bit)
{
	*(unsigned int *)(unsigned long)reset_addr = (1 << bit);
}

void usb_aml_detect_operation(int argc, char * const argv[])
{
}

int m31_phy_init(unsigned int phy_num)
{
	if (phy_num == 0) {
		*(unsigned int *)(unsigned long)M31_PHY_0_BASE = M31_SETTING;
		udelay(20);
		usb_reset(RESET_BASE, USB_RESET_BIT);
		udelay(20);
		usb_reset(RESET_BASE, M31_0_RESET_LEVEL_BIT | M31_0_UTMI_RESET_LEVEL_BIT);
		udelay(20);
		usb_reset(RESET_BASE, CRG_U3DRD_0_RESET_LEVEL_BIT);
		udelay(20);
	} else if (phy_num == 1) {
		*(unsigned int *)(unsigned long)M31_PHY_1_BASE = M31_SETTING;
		udelay(20);
		usb_reset(RESET_BASE, M31_1_RESET_LEVEL_BIT | M31_1_UTMI_RESET_LEVEL_BIT);
		udelay(20);
		usb_reset(RESET_BASE, CRG_U3DRD_1_RESET_LEVEL_BIT);
		udelay(20);
	} else if (phy_num == 2) {
		*(unsigned int *)(unsigned long)M31_PHY_2_BASE = M31_SETTING;
		udelay(20);
		usb_reset(RESET_BASE, M31_2_RESET_LEVEL_BIT | M31_2_UTMI_RESET_LEVEL_BIT);
		udelay(20);
		usb_reset(RESET_BASE, CRG_U3DRD_2_RESET_LEVEL_BIT);
		udelay(20);
	}

	return 0;
}

/**************************************************************/
/*           device mode config                               */
/**************************************************************/
void usb_device_mode_init(int phy_num) {
	printf("usb_device_mode_init\n");

	*(unsigned int *)(unsigned long)M31_PHY_1_BASE = M31_PHY_DEVICE_SETTING;
	udelay(20);

	//step 1: usb controller reset
	usb_reset(RESET_BASE, USB_RESET_BIT);
	udelay(10);
	//step 2: phy21 reset
	usb_reset(RESET_BASE, M31_1_RESET_LEVEL_BIT | M31_1_UTMI_RESET_LEVEL_BIT);
	udelay(50);

	usb_reset(RESET_BASE, CRG_U3DRD_1_RESET_LEVEL_BIT);
	udelay(20);
}
