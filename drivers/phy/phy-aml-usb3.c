// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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
#include <asm-generic/gpio.h>

#include <linux/compat.h>
#include <linux/bitfield.h>

static int phy_aml_usb3_power_info(struct phy *phy){
#ifdef CONFIG_DM_GPIO
	int ret;
	//struct gpio_desc desc;
	const char *usb_vbus_gpioname;
	const void *fdt = gd->fdt_blob;
	struct phy_aml_usb3_priv *priv = dev_get_priv(phy->dev);

	usb_vbus_gpioname = fdt_getprop(fdt, dev_of_offset(phy->dev), "gpio-vbus-power", NULL);
	if (!usb_vbus_gpioname) {
		printf("No ----gpio-vbus-power\n");
		return 0;
	}

	debug("usb_vbus_gpioname=%s\n", usb_vbus_gpioname);

	ret = dm_gpio_lookup_name(usb_vbus_gpioname, &priv->desc);
	if (ret) {
		printf("%s: not found\n", usb_vbus_gpioname);
		return ret;
	}

	ret = dm_gpio_request(&priv->desc, usb_vbus_gpioname);
	if (ret) {
		dm_gpio_free(phy->dev, &priv->desc);
		dm_gpio_request(&priv->desc, usb_vbus_gpioname);
		pr_info("%s: after usb stop, retry request gpio\n",
			usb_vbus_gpioname);
	}

	ret = dm_gpio_set_dir_flags(&priv->desc, GPIOD_IS_OUT);
	if (ret) {
		pr_err("%s: failed to set direction\n", usb_vbus_gpioname);
		return ret;
	}
#endif
		return 0;
}

static int phy_aml_usb3_power_on(struct phy *phy){
#ifdef CONFIG_DM_GPIO
	int ret;
	struct phy_aml_usb3_priv *priv = dev_get_priv(phy->dev);

	ret = phy_aml_usb3_power_info(phy);
	if (ret)
		return 0;

	if (priv)
		dm_gpio_set_value(&priv->desc, 1);
#endif
		return 0;
}

static int phy_aml_usb3_power_off(struct phy *phy)
{
	return 0;
}

static int phy_aml_usb3_getinfo(struct phy *phy)
{
	unsigned int u3portnum;
	struct phy_aml_usb3_priv *priv = dev_get_priv(phy->dev);

	dev_read_u32(phy->dev, "portnum", &u3portnum);
	priv->usb3_port_num = u3portnum;

	priv->base_addr = dev_read_addr(phy->dev);
	if (priv->base_addr == FDT_ADDR_T_NONE) {
		pr_err("Coun't get usb3 base addr\n");
		return -1;
	}

	phy_aml_usb3_power_info(phy);
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
	debug("usb3_phy: portnum=%d, base addr = 0x%08x\n",
		u3portnum, priv->base_addr);
	if (u3portnum == 0) {
		usb_aml_reg = (struct usb_aml_regs *)((ulong)priv->base_addr);
		r1.d32 = usb_aml_reg->usb_r1;
		r1.b.u3h_fladj_30mhz_reg = 0x20;
		usb_aml_reg->usb_r1 = r1.d32;
		udelay(100);
	}

	for (i = 0; i < u3portnum; i++) {
		usb_aml_reg = (struct usb_aml_regs *)((ulong)(priv->base_addr + i*PHY_REGISTER_SIZE));

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
	}
	usb_save_phy_dev(1, phy);
	return 0;
}


struct phy_ops phy_aml_usb3_phy_ops = {
	.init = phy_aml_usb3_init,
	.power_on = phy_aml_usb3_power_on,
	.power_off = phy_aml_usb3_power_off,
	.getinfo = phy_aml_usb3_getinfo,
};

int phy_aml_usb3_probe(struct udevice *dev)
{
	return 0;
}

static const struct udevice_id phy_aml_usb3_phy_ids[] = {
	{ .compatible = "amlogic, amlogic-new-usb3-v2" },
	{ }
};

U_BOOT_DRIVER(meson_usb3_phy) = {
	.name = "amlogic_new_usb3_phy",
	.id = UCLASS_PHY,
	.of_match = phy_aml_usb3_phy_ids,
	.probe = phy_aml_usb3_probe,
	.ops = &phy_aml_usb3_phy_ops,
	.priv_auto_alloc_size = sizeof(struct phy_aml_usb3_priv),
};
