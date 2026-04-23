/*
 * SPDX-License-Identifier:     GPL-2.0+
 *
 * (C) Copyright 2025 Wesion Technology Co., Ltd
 */

#include <common.h>
#include <dwc3-uboot.h>
#include <usb.h>
#include <i2c.h>
#include <dm.h>
#include <linux/usb/phy-rockchip-usbdp.h>
#include <asm/io.h>
#include <rockusb.h>

#define TP_I2C_BUS_NUM 0
#define MCU_I2C_BUS_NUM 2

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_USB_DWC3
#define CRU_BASE		0x27200000
#define CRU_SOFTRST_CON47	0x0abc
#define U3PHY_BASE		0x2b010000

static struct dwc3_device dwc3_device_data = {
	.maximum_speed = USB_SPEED_SUPER,
	.base = 0x23000000,
	.dr_mode = USB_DR_MODE_PERIPHERAL,
	.index = 0,
	.dis_u2_susphy_quirk = 1,
	.dis_u1u2_quirk = 1,
	.usb2_phyif_utmi_width = 16,
};

int rk_board_init(void)
{
	int ret = 0;
	int res = 0;
	struct udevice *bus;
	struct udevice *dev;
	uchar linebuf[1];
	u8 value = 0;
	run_command("gpio set 83", 0);//GPIO2_C3 vcc 5v

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
				value = 1;
			} else if (linebuf[0] == 0x79) {//new ts050
				env_set("lcd_panel","newts050");
				value = 1;
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
					value = 1;
				}
			} else {
				env_set("lcd_panel","null");
			}
		}
	}

	run_command("gpio set 78", 0);//GPIO2_B6 TYPEC0_PWR_EN

	env_set_ulong("mipi_lcd_exist", value);
	printf("mipi_lcd_exist : %d\n", value);

	run_command("kbi usid", 0);//export usid to env

	/*Add bootmode to env*/
	ret = uclass_get_device_by_seq(UCLASS_I2C, MCU_I2C_BUS_NUM, &bus);
	if (ret) {
		printf("%s: No bus %d\n", __func__, MCU_I2C_BUS_NUM);
		return 0;
	}

	ret = i2c_get_chip(bus, 0x18, 1, &dev);
	if (!ret) {
		res = dm_i2c_read(dev, 0x20, linebuf, 1);
		if (!res) {
			printf("boot_mode=0x%x\n", linebuf[0]);
			if (linebuf[0] == 0x00){	//boot from spi
				env_set("bootmode","spi");
			} else if (linebuf[0] == 0x1) {		//boot from emmc
				env_set("bootmode","emmc");
			}
		}
	}

    return 0;
}

int usb_gadget_handle_interrupts(int index)
{
	dwc3_uboot_handle_interrupt(0);
	return 0;
}

bool rkusb_usb3_capable(void)
{
	return true;
}

static void usb_reset_otg_controller(void)
{
	writel(0x00200020, CRU_BASE + CRU_SOFTRST_CON47);
	mdelay(1);
	writel(0x00200000, CRU_BASE + CRU_SOFTRST_CON47);
	mdelay(1);
}

int board_usb_init(int index, enum usb_init_type init)
{
	u32 ret = 0;

	usb_reset_otg_controller();

#if defined(CONFIG_SUPPORT_USBPLUG)
	dwc3_device_data.maximum_speed = USB_SPEED_HIGH;

	if (rkusb_switch_usb3_enabled()) {
		dwc3_device_data.maximum_speed = USB_SPEED_SUPER;
		ret = rockchip_u3phy_uboot_init(U3PHY_BASE);
		if (ret) {
			rkusb_force_to_usb2(true);
			dwc3_device_data.maximum_speed = USB_SPEED_HIGH;
		}
	}
#else
	ret = rockchip_u3phy_uboot_init(U3PHY_BASE);
	if (ret) {
		rkusb_force_to_usb2(true);
		dwc3_device_data.maximum_speed = USB_SPEED_HIGH;
	}
#endif

	return dwc3_uboot_init(&dwc3_device_data);
}

#if defined(CONFIG_SUPPORT_USBPLUG)
int board_usb_cleanup(int index, enum usb_init_type init)
{
	dwc3_uboot_exit(index);
	return 0;
}
#endif

#endif
