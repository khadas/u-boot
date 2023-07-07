/*
 * SPDX-License-Identifier:     GPL-2.0+
 *
 * (C) Copyright 2022 Wesion Technology Co., Ltd
 */

#include <common.h>
#include <dwc3-uboot.h>
#include <usb.h>
#include <i2c.h>
#include <dm.h>
#include <adc.h>

#define TP_I2C_BUS_NUM 6

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_USB_DWC3
static struct dwc3_device dwc3_device_data = {
	.maximum_speed = USB_SPEED_HIGH,
	.base = 0xfc000000,
	.dr_mode = USB_DR_MODE_PERIPHERAL,
	.index = 0,
	.dis_u2_susphy_quirk = 1,
	.usb2_phyif_utmi_width = 16,
};

int usb_gadget_handle_interrupts(void)
{
	dwc3_uboot_handle_interrupt(0);
	return 0;
}

int board_usb_init(int index, enum usb_init_type init)
{
	return dwc3_uboot_init(&dwc3_device_data);
}
#endif

int rk_board_late_init(void)
{
	// Set FAN test
	run_command("i2c dev 2; i2c mw 18 96 1", 0);

	// Set Green LED on
	run_command("gpio set 138; gpio clear 139; gpio clear 140", 0);

	return 0;
}

int rk_board_init(void)
{
	int ret = 0;
	int res = 0;
	struct udevice *bus;
	struct udevice *dev;
	unsigned int val;
	uchar linebuf[1];

	run_command("gpio set 130", 0);//GPIO4_A2 vcc 5v

	ret = uclass_get_device_by_seq(UCLASS_I2C, TP_I2C_BUS_NUM, &bus);
	if (ret) {
		printf("%s: No bus %d\n", __func__, TP_I2C_BUS_NUM);
		return 0;
	}

	ret = i2c_get_chip(bus, 0x38, 1, &dev);
	if (!ret) {
		res = dm_i2c_read(dev, 0xA8, linebuf, 1);
		if (!res) {
			printf("TP05 id=0x%x\n", linebuf[0]);
			if (linebuf[0] == 0x51){//old ts050
				env_set("lcd_panel","ts050");
			} else if (linebuf[0] == 0x79) {//new ts050
				env_set("lcd_panel","newts050");
			}
		}
	}
	if (ret || res) {
		ret = i2c_get_chip(bus, 0x14, 1, &dev);
		if (!ret) {
			res = dm_i2c_read(dev, 0x9e, linebuf, 1);
			if (!res) {
				printf("TP10 id=0x%x\n", linebuf[0]);
				if (linebuf[0] == 0x00) {//TS101
					env_set("lcd_panel","ts101");
				}
			} else {
				env_set("lcd_panel","null");
			}
		}
	}

	ret = adc_channel_single_shot("saradc", 2, &val);
	if (ret) {
		printf("%s adc_channel_single_shot fail! ret=%d\n", __func__, ret);
		return -1;
	}

	if (val < 50) {
		run_command("gpio set 105", 1); //pogo_power_enable
	}

	return 0;
}
