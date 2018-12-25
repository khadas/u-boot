// SPDX-License-Identifier: GPL-2.0+
/*
 * Meson GXL USB3 PHY driver
 *
 * Copyright (C) 2018 Martin Blumenstingl <martin.blumenstingl@googlemail.com>
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
#include <clk.h>
#include <asm/arch/usb.h>
#include <linux/libfdt.h>
//#include <asm/gpio.h>
#include <asm-generic/gpio.h>

#include <linux/compat.h>
#include <linux/bitfield.h>

struct phy_aml_usb3_priv {
	unsigned int base_addr;
	unsigned int usb3_port_num;
};

static int
phy_aml_usb3_set_host_mode(struct phy_aml_usb3_priv *priv)
{
	return 0;
}

static int phy_aml_usb3_power_on(struct phy *phy){
	int ret;
	struct gpio_desc desc;
	const char *usb_vbus_gpioname;
	const void *fdt = gd->fdt_blob;

	usb_vbus_gpioname = fdt_getprop(fdt, dev_of_offset(phy->dev), "gpio-vbus-power", NULL);
	pr_info("usb_vbus_gpioname=%s\n", usb_vbus_gpioname);

	ret = dm_gpio_lookup_name(usb_vbus_gpioname, &desc);
	if (ret) {
		printf("%s: not found\n", usb_vbus_gpioname);
		return ret;
	}

	ret = dm_gpio_request(&desc, usb_vbus_gpioname);
	if (ret) {
		dm_gpio_free(phy->dev, &desc);
		dm_gpio_request(&desc, usb_vbus_gpioname);
		pr_info("%s: after usb stop, retry request gpio\n",
			usb_vbus_gpioname);
	}

	ret = dm_gpio_set_dir_flags(&desc, GPIOD_IS_OUT);
	if (ret) {
		pr_err("%s: failed to set direction\n", usb_vbus_gpioname);
		return ret;
	}

		dm_gpio_set_value(&desc, 1);
		return 0;
}


static int phy_aml_usb3_power_off(struct phy *phy)
{
	return 0;
}

static int phy_aml_usb3_init(struct phy *phy)
{
	usb_r1_t r1 = {.d32 = 0};
	usb_r2_t r2 = {.d32 = 0};
	usb_r3_t r3 = {.d32 = 0};
	int i;
	unsigned int u3portnum;
	struct phy_aml_usb3_priv *priv = dev_get_priv(phy->dev);
	struct usb_aml_regs *usb_aml_reg;
#if 0
	u32 data = 0;
#endif

	dev_read_u32(phy->dev, "portnum", &u3portnum);
	priv->usb3_port_num = u3portnum;

	priv->base_addr = dev_read_addr(phy->dev);
	if (priv->base_addr == FDT_ADDR_T_NONE) {
		pr_err("Coun't get usb3 base addr\n");
		return -1;
	}
	pr_info("usb3_phy: portnum=%d, base addr = 0x%08x\n",
		u3portnum, priv->base_addr);
	if (u3portnum == 0) {
		usb_aml_reg = (struct usb_aml_regs *)((ulong)priv->base_addr);
		r1.d32 = usb_aml_reg->usb_r1;
		r1.b.u3h_fladj_30mhz_reg = 0x20;
		usb_aml_reg->usb_r1 = r1.d32;
		udelay(100);
	}

	for (i = 0; i < u3portnum; i++) {
		usb_aml_reg = (struct usb_aml_regs *)((ulong)priv->base_addr+i*PHY_REGISTER_SIZE);

		r3.d32 = usb_aml_reg->usb_r3;
		r3.b.p30_ssc_en = 1;
		r3.b.p30_ref_ssp_en = 1;
		usb_aml_reg->usb_r3 = r3.d32;
		udelay(2);
		r2.d32 = usb_aml_reg->usb_r2;
		r2.b.p30_pcs_tx_deemph_3p5db = 0x15;
		r2.b.p30_pcs_tx_deemph_6db = 0x20;
		usb_aml_reg->usb_r2 = r2.d32;
		udelay(2);
		r1.d32 = usb_aml_reg->usb_r1;
		r1.b.u3h_host_port_power_control_present = 1;
		r1.b.u3h_fladj_30mhz_reg = 32;
		usb_aml_reg->usb_r1 = r1.d32;
		udelay(2);
#if 0
		/*
		* WORKAROUND: There is SSPHY suspend bug due to which USB enumerates
		* in HS mode instead of SS mode. Workaround it by asserting
		* LANE0.TX_ALT_BLOCK.EN_ALT_BUS to enable TX to use alt bus mode
		*/
		data = cr_bus_read(0x102d);
		data |= (1 << 7);
		cr_bus_write(0x102D, data);

		data = cr_bus_read(0x1010);
		data &= ~0xff0;
		data |= 0x20;
		cr_bus_write(0x1010, data);

		/*
		* Fix RX Equalization setting as follows
		* LANE0.RX_OVRD_IN_HI. RX_EQ_EN set to 0
		* LANE0.RX_OVRD_IN_HI.RX_EQ_EN_OVRD set to 1
		* LANE0.RX_OVRD_IN_HI.RX_EQ set to 3
		* LANE0.RX_OVRD_IN_HI.RX_EQ_OVRD set to 1
		*/
		data = cr_bus_read(0x1006);
		data &= ~(1 << 6);
		data |= (1 << 7);
		data &= ~(0x7 << 8);
		data |= (0x3 << 8);
		data |= (0x1 << 11);
		cr_bus_write(0x1006, data);

		/*
		* Set EQ and TX launch amplitudes as follows
		* LANE0.TX_OVRD_DRV_LO.PREEMPH set to 22
		* LANE0.TX_OVRD_DRV_LO.AMPLITUDE set to 127
		 * LANE0.TX_OVRD_DRV_LO.EN set to 1.
		*/
		data = cr_bus_read(0x1002);
		data &= ~0x3f80;
		data |= (0x16 << 7);
		data &= ~0x7f;
		data |= (0x7f | (1 << 14));
		cr_bus_write(0x1002, data);

		/*
		* TX_FULL_SWING  to 127
		*/
		data = cr_bus_read(0x30);
		data &= ~(0xf << 4);
		cr_bus_write(0x30, data);

		/*
		* TX_FULL_SWING  to 127
		*/
		r1.d32 = usb_aml_reg->usb_r1;
		r1.b.p30_pcs_tx_swing_full = 127;
		r1.b.u3h_fladj_30mhz_reg = 0x20;
		usb_aml_reg->usb_r1 = r1.d32;
		udelay(2);

		/*
		* TX_DEEMPH_3_5DB  to 22
		*/
		r2.d32 = usb_aml_reg->usb_r2;
		r2.b.p30_pcs_tx_deemph_3p5db = 22;
		usb_aml_reg->usb_r2 = r2.d32;

		udelay(2);
		/*
		* LOS_BIAS	to 0x5
		* LOS_LEVEL to 0x9
		*/
		r3.d32 = usb_aml_reg->usb_r3;
		r3.b.p30_los_bias = 0x5;
		r3.b.p30_los_level = 0x9;
		r3.b.p30_ssc_en = 1;
		r3.b.p30_ssc_range = 2;
		usb_aml_reg->usb_r3 = r3.d32;
#endif
	}

	return 0;
}


struct phy_ops phy_aml_usb3_phy_ops = {
	.init = phy_aml_usb3_init,
	.power_on = phy_aml_usb3_power_on,
	.power_off = phy_aml_usb3_power_off,
};

int phy_aml_usb3_probe(struct udevice *dev)
{

	return 0;
}

static const struct udevice_id phy_aml_usb3_phy_ids[] = {
	{ .compatible = "amlogic, amlogic-new-usb3-v2" },
	{ }
};

U_BOOT_DRIVER(meson_gxl_usb3_phy) = {
	.name = "amlogic_new_usb3_phy",
	.id = UCLASS_PHY,
	.of_match = phy_aml_usb3_phy_ids,
	.probe = phy_aml_usb3_probe,
	.ops = &phy_aml_usb3_phy_ops,
	.priv_auto_alloc_size = sizeof(struct phy_aml_usb3_priv),
};
