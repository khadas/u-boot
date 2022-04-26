// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/io.h>
#include <bitfield.h>
#include <dm.h>
#include <errno.h>
#include <regmap.h>
#include <power/regulator.h>
#include <clk.h>
#include <asm/arch/usb.h>
#include <amlogic/cpu_id.h>

#include <linux/compat.h>
#include <linux/ioport.h>

#define RESET_COMPLETE_TIME				500

static int usb_get_priv_data (struct phy *phy){
	struct phy_aml_usb2_priv *priv = dev_get_priv(phy->dev);
	unsigned int  dwc2_reg, u2portnum = 0;
	int ret, i;

	priv->reset_addr = dev_read_addr_index(phy->dev, 1);
	if (priv->reset_addr == FDT_ADDR_T_NONE) {
		pr_err("Coun't get usb_phy2_pll_base_addr[1]: reset addr\n");
		return -1;
	}

	dev_read_u32(phy->dev, "portnum", &u2portnum);
	priv->u2_port_num = u2portnum;

	ret = dev_read_u32(phy->dev, "dwc2_a_reg", &dwc2_reg);
	if (ret) {
		pr_err("Coun't get dwc2_a_reg!!!!\n");
		return -1;
	}
	priv->dwc2_a_addr = dwc2_reg;

	for (i = 2; i < (u2portnum + 2); i++) {
		priv->usb_phy2_pll_base_addr[i-2] = dev_read_addr_index(phy->dev, i);
		if (priv->usb_phy2_pll_base_addr[i-2] == FDT_ADDR_T_NONE) {
			pr_err("Coun't get usb_phy2_pll_base_addr[%d]\n", i-2);
			return -1;
		}
	}

	priv->base_addr = dev_read_addr(phy->dev);

	debug("usb2 phy: portnum=%d, addr1= 0x%08x, addr2= 0x%08x\n",
		u2portnum, priv->usb_phy2_pll_base_addr[0],
		priv->usb_phy2_pll_base_addr[1]);
	debug("dwc2_a base addr: 0x%08x\n", priv->dwc2_a_addr);

	return 0;

}

static int phy_aml_usb2_getinfo(struct phy *phy)
{
	return usb_get_priv_data(phy);
}

static int phy_aml_usb2_phy_init(struct phy *phy)
{
	int ret = -1;
	ret = usb_get_priv_data(phy);
	if (ret !=0)
		return ret;

	return usb2_phy_init(phy);
}

static int phy_aml_usb2_tuning(struct phy *phy, int port) {
	struct phy_aml_usb2_priv *priv = dev_get_priv(phy->dev);
	unsigned long phy_reg_base;

	phy_reg_base = priv->usb_phy2_pll_base_addr[port];
	if (phy_reg_base == 0)
		return 0;
	return usb2_phy_tuning(phy_reg_base, port);
}

static int phy_aml_usb2_power_on(struct phy *phy)
{
	return 0;
}

static int phy_aml_usb2_power_off(struct phy *phy)
{
	return 0;
}

struct phy_ops amlogic_usb2_phy_ops = {
	.init = phy_aml_usb2_phy_init,
	.power_on = phy_aml_usb2_power_on,
	.power_off = phy_aml_usb2_power_off,
	.tuning = phy_aml_usb2_tuning,
	.getinfo = phy_aml_usb2_getinfo,
};

static int phy_aml_usb2_probe(struct udevice *dev)
{
	return 0;
}

static const struct udevice_id phy_aml_usb2_phy_ids[] = {
	{ .compatible = "amlogic, amlogic-new-usb2-v2" },
	{ }
};

U_BOOT_DRIVER(meson_usb2_phy) = {
	.name = "amlogic_new_usb2_phy",
	.id = UCLASS_PHY,
	.of_match = phy_aml_usb2_phy_ids,
	.probe = phy_aml_usb2_probe,
	.ops = &amlogic_usb2_phy_ops,
	.priv_auto_alloc_size = sizeof(struct phy_aml_usb2_priv),
};
