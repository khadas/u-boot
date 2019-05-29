// SPDX-License-Identifier: GPL-2.0+
/*
 * Meson GXL and GXM USB2 PHY driver
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
#include <asm/arch/cpu_id.h>


#include <linux/compat.h>
#include <linux/ioport.h>

#define MESON_CPU_MAJOR_ID_A1	0x2C
#define P_AO_RTI_GEN_PWR_SLEEP0 0xfe007808
#define P_AO_RTI_GEN_PWR_ISO0 0xfe007804
#define P_HHI_MEM_PD_REG0 0xfe007850

#define RESET_COMPLETE_TIME				500
#define MESON_CPU_MAJOR_ID_SM1	0x2B

char name[32];

struct phy_aml_usb2_priv {
	/* controller */
	unsigned int base_addr;
	unsigned int reset_addr;
	/* role */
	int id_mode;
	/* vbus call back */
	unsigned int usb_phy2_base_addr;
	unsigned int u2_port_num;
	unsigned int usbphy_reset_bit[8];
	unsigned int usbctrl_reset_bit;
	unsigned int u2_ctrl_sleep_shift;
	unsigned int u2_hhi_mem_pd_mask;
	unsigned int u2_ctrl_iso_shift;
	unsigned int u2_hhi_mem_pd_shift;
	unsigned int clktree_usb_bus_ctrl;
	unsigned int usb_phy2_pll_base_addr[4];
};

static int Rev_flag = 0;

/*Rev_flag == 0XB, g12b and revB, tl1 */
/*Rev_flag == 1, sm1 */
static void phy_aml_usb2_check_rev (void)
{
	cpu_id_t cpu_id = get_cpu_id();

	if (cpu_id.family_id == MESON_CPU_MAJOR_ID_G12B) {
		if (cpu_id.chip_rev == 0xb)
			Rev_flag = 0xb;
		else
			Rev_flag = 0;
	} else if (cpu_id.family_id == MESON_CPU_MAJOR_ID_SM1){
		Rev_flag = MESON_CPU_MAJOR_ID_SM1;
	}else if (cpu_id.family_id == MESON_CPU_MAJOR_ID_A1){
		Rev_flag = MESON_CPU_MAJOR_ID_A1;
	}
	return;
}

static int phy_aml_usb2_get_rev_type (void)
{
	return Rev_flag;
}


static void set_usb_pll(struct phy *phy, uint32_t volatile *phy2_pll_base)
{
	unsigned int pll_set1, pll_set2, pll_set3;
	unsigned int tuning_disconnect_threshold = 0x34;

	dev_read_u32(phy->dev, "pll-setting-1", &pll_set1);
	dev_read_u32(phy->dev, "pll-setting-2", &pll_set2);
	dev_read_u32(phy->dev, "pll-setting-3", &pll_set3);
	debug("pll1=0x%08x, pll2=0x%08x, pll-setting-3 =0x%08x\n",
		pll_set1, pll_set2, pll_set3);

	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x40))
		= (pll_set1 | USB_PHY2_RESET | USB_PHY2_ENABLE);
	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x44)) =
		pll_set2;
	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x48)) =
		pll_set3;
	udelay(100);
	(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x40))
		= (((pll_set1) | (USB_PHY2_ENABLE))
			& (~(USB_PHY2_RESET)));

	dev_read_u32(phy->dev, "disconnect-threshold", &tuning_disconnect_threshold);
	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0xc)) =
		tuning_disconnect_threshold;
	debug("tuning_disconnect_threshold=0x%x\n", tuning_disconnect_threshold);
}

static int phy_aml_usb2_phy_init(struct phy *phy)
{
	struct u2p_aml_regs *u2p_aml_reg;
	struct phy_aml_usb2_priv *priv = dev_get_priv(phy->dev);
	u2p_r0_t dev_u2p_r0;
	u2p_r1_t dev_u2p_r1;
	int i;
	int cnt, node, rev_type;
	int time_dly = RESET_COMPLETE_TIME;
	int usbphy_reset_bit[8] = {0};
	int usbctrl_reset_bit;
	int u2_sleep_shift;
	int u2_mem_pd_mask;
	int u2_iso_shift;
	int u2_pd_shift;
	unsigned int  u2portnum = 0;

	phy_aml_usb2_check_rev();
	rev_type = phy_aml_usb2_get_rev_type();
	if ((rev_type == MESON_CPU_MAJOR_ID_SM1) || (rev_type == MESON_CPU_MAJOR_ID_A1)) {
		u2_sleep_shift = dev_read_u32_default(phy->dev, "u2-ctrl-sleep-shift", -1);
		if (u2_sleep_shift != -1)
			priv->u2_ctrl_sleep_shift = u2_sleep_shift;
		else
			goto sm1erro;

		u2_pd_shift = dev_read_u32_default(phy->dev, "u2-hhi-mem-pd-shift", -1);
		if (u2_pd_shift != -1)
			priv->u2_hhi_mem_pd_shift = u2_pd_shift;
		else
			goto sm1erro;

		u2_mem_pd_mask = dev_read_u32_default(phy->dev, "u2-hhi-mem-pd-mask", -1);
		if (u2_mem_pd_mask != -1)
			priv->u2_hhi_mem_pd_mask = u2_mem_pd_mask;
		else
			goto sm1erro;

		u2_iso_shift = dev_read_u32_default(phy->dev, "u2-ctrl-iso-shift", -1);
		if (u2_iso_shift != -1)
			priv->u2_ctrl_iso_shift = u2_iso_shift;
		else
			goto sm1erro;
		writel((readl(P_AO_RTI_GEN_PWR_SLEEP0) & (~(0x1 << priv->u2_ctrl_sleep_shift))),
			P_AO_RTI_GEN_PWR_SLEEP0);
		writel((readl(P_AO_RTI_GEN_PWR_ISO0) & (~(0x1 << priv->u2_ctrl_iso_shift))),
			P_AO_RTI_GEN_PWR_ISO0);
		writel((readl(P_HHI_MEM_PD_REG0)
			& (~(priv->u2_hhi_mem_pd_mask << priv->u2_hhi_mem_pd_shift))), P_HHI_MEM_PD_REG0);
	}

	priv->reset_addr = dev_read_addr_index(phy->dev, 1);
	if (priv->reset_addr == FDT_ADDR_T_NONE) {
		pr_err("Coun't get usb_phy2_pll_base_addr[%d]\n", i);
		return -1;
	}

	dev_read_u32(phy->dev, "portnum", &u2portnum);
	priv->u2_port_num = u2portnum;

	priv->clktree_usb_bus_ctrl = dev_read_addr_index(phy->dev, 2 + u2portnum);
	if (priv->clktree_usb_bus_ctrl == FDT_ADDR_T_NONE)
		pr_err("Coun't get clktree_usb_bus_ctrl addr index %d\n", 2 + u2portnum);
	else
		*(volatile unsigned int *)priv->clktree_usb_bus_ctrl |= (1 << 8) | (1 << 9);

	usbctrl_reset_bit = dev_read_u32_default(phy->dev, "usb-reset-bit", -1);
	if (usbctrl_reset_bit != -1)
		priv->usbctrl_reset_bit = usbctrl_reset_bit;
	else
		priv->usbctrl_reset_bit = 2;
	*(volatile unsigned int *)priv->reset_addr = (1 << priv->usbctrl_reset_bit);

	udelay(time_dly);

	for (i = 0; i < u2portnum; i++) {
		memset(name, 0, 32 * sizeof(char));
		sprintf(name, "phy2%d-reset-level-bit", i);
		usbphy_reset_bit[i] = dev_read_u32_default(phy->dev, name, -1);
		if (usbphy_reset_bit[i] != -1)
			priv->usbphy_reset_bit[i] = usbphy_reset_bit[i];
		else
			priv->usbphy_reset_bit[i] = 16 + i;
	}

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

	for (i = 0; i < u2portnum; i++) {
		u2p_aml_reg = (struct u2p_aml_regs *)((ulong)priv->base_addr + i * PHY_REGISTER_SIZE);
		dev_u2p_r0.d32 = u2p_aml_reg->u2p_r0;
		dev_u2p_r0.b.host_device= 1;
		dev_u2p_r0.b.POR= 0;
		u2p_aml_reg->u2p_r0  = dev_u2p_r0.d32;
		udelay(10);
		*(volatile unsigned int *)priv->reset_addr = (1 << priv->usbphy_reset_bit[i]);
		udelay(50);

		/* wait for phy ready */
		dev_u2p_r1.d32  = u2p_aml_reg->u2p_r1;
		cnt = 0;
		while (dev_u2p_r1.b.phy_rdy != 1) {
			dev_u2p_r1.d32 = u2p_aml_reg->u2p_r1;
			/*we wait phy ready max 1ms, common is 100us*/
			if (cnt > 200)
				break;
			else {
				cnt++;
				udelay(5);
			}
		}
	}

	for (i = 0; i < u2portnum; i++) {
		debug("------set usb pll\n");
		set_usb_pll(phy, priv->usb_phy2_pll_base_addr[i]);
	}
	return 0;
sm1erro:
	pr_err("Coun't get sm1 power shift parmeters\n");
	return -1;
}

static void phy_aml_usb2_reset(struct phy_aml_usb2_priv *priv)
{
	return;
}

static int phy_aml_usb2_tuning(struct phy *phy, int port)
{
	struct phy_aml_usb2_priv *priv = dev_get_priv(phy->dev);
	unsigned long phy_reg_base;
	unsigned int pll_set1, pll_set2, pll_set3, pll_set4;
	unsigned int rev_type = 0;

	if (port > 2)
		return 0;

	dev_read_u32(phy->dev, "pll-setting-4", &pll_set1);
	dev_read_u32(phy->dev, "pll-setting-5", &pll_set2);
	dev_read_u32(phy->dev, "pll-setting-7", &pll_set4);
	dev_read_u32(phy->dev, "pll-setting-6", &pll_set3);

	debug("pll1=0x%08x, pll2=0x%08x, pll-setting-3 =0x%08x\n",
			pll_set2, pll_set1, pll_set3);


	phy_reg_base = priv->usb_phy2_pll_base_addr[port];

	rev_type = phy_aml_usb2_get_rev_type();
	if ((rev_type == 0xb) || (rev_type == MESON_CPU_MAJOR_ID_SM1)
	    || (rev_type == MESON_CPU_MAJOR_ID_A1)) {
		(*(volatile uint32_t *)(phy_reg_base + 0x50)) = pll_set1;
		(*(volatile uint32_t *)(phy_reg_base + 0x54)) = 0x2a;
		(*(volatile uint32_t *)(phy_reg_base + 0x34)) = pll_set3 & (0x1f << 16);
		return 0;
	}

	(*(volatile uint32_t *)(phy_reg_base + 0x10)) = pll_set2;
	(*(volatile uint32_t *)(phy_reg_base + 0x50)) = pll_set1;
	(*(volatile uint32_t *)(phy_reg_base + 0x38)) = pll_set4;
	(*(volatile uint32_t *)(phy_reg_base + 0x34)) = pll_set3;
	return 0;
}


static void
phy_aml_usb2_set_host_mode(struct phy_aml_usb2_priv *priv)
{
	return;
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
};

int phy_aml_usb2_probe(struct udevice *dev)
{
	return 0;
}

static const struct udevice_id phy_aml_usb2_phy_ids[] = {
	{ .compatible = "amlogic, amlogic-new-usb2-v2" },
	{ }
};

U_BOOT_DRIVER(meson_gxl_usb2_phy) = {
	.name = "amlogic_new_usb2_phy",
	.id = UCLASS_PHY,
	.of_match = phy_aml_usb2_phy_ids,
	.probe = phy_aml_usb2_probe,
	.ops = &amlogic_usb2_phy_ops,
	.priv_auto_alloc_size = sizeof(struct phy_aml_usb2_priv),
};
