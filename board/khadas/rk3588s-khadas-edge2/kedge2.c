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

#define HW_VERSION_ADC_VALUE_TOLERANCE  0x28
#define HW_VERSION_ADC_VAL_EDGE2_V11    0x2b3
#define HW_VERSION_ADC_VAL_EDGE2_V12    0x56d
#define HW_VERSION_ADC_VAL_EDGE2_V13    0x807

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

static int set_hw_version(void)
{
	unsigned int val = 0;
	int ret;
	struct udevice *dev;
	int current_channel = 5;

	ret = uclass_get_device_by_name(UCLASS_ADC, "saradc", &dev);
	if(ret)
		return ret;
	udelay(100);
	ret = adc_start_channel(dev, current_channel);
	if(ret)
		return ret;
	ret = adc_channel_data(dev, current_channel, &val);
	if(ret)
		return ret;

	if ((val >= HW_VERSION_ADC_VAL_EDGE2_V11 - HW_VERSION_ADC_VALUE_TOLERANCE) &&
		(val <= HW_VERSION_ADC_VAL_EDGE2_V11 + HW_VERSION_ADC_VALUE_TOLERANCE)){
		env_set("hwver", "EDGE2.V11");
	} else if ((val >= HW_VERSION_ADC_VAL_EDGE2_V12 - HW_VERSION_ADC_VALUE_TOLERANCE) &&
		(val <= HW_VERSION_ADC_VAL_EDGE2_V12 + HW_VERSION_ADC_VALUE_TOLERANCE)) {
		env_set("hwver", "EDGE2.V12");
	} else if ((val >= HW_VERSION_ADC_VAL_EDGE2_V13 - HW_VERSION_ADC_VALUE_TOLERANCE) &&
		(val <= HW_VERSION_ADC_VAL_EDGE2_V13 + HW_VERSION_ADC_VALUE_TOLERANCE)) {
		env_set("hwver", "EDGE2.V13");
	} else {
		env_set("hwver", "Unknow");
	}

	current_channel = -1;

	return 0;
}

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

	set_hw_version();

	run_command("kbi usid", 0);//export usid to env

	return 0;
}
