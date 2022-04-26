// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/arch/clock.h>
#include <asm/io.h>
#include <clk-uclass.h>
#include <div64.h>
#include <dm.h>
#include <dt-bindings/clock/s4-clkc.h>
#include "clk_meson.h"

/* change it later */
#define SYS_CLK		166666666

/* clk81 gates, sys_clk */
static struct meson_gate gates[] = {
	{CLKID_SPICC_A_GATE, S4_CLKCTRL_SPICC_CLK_CTRL, 6},
	{CLKID_SAR_ADC_GATE, S4_CLKCTRL_SAR_CLK_CTRL, 8},
	{CLKID_SD_EMMC_A_GATE, S4_CLKCTRL_SD_EMMC_CLK_CTRL, 7},
	{CLKID_SD_EMMC_B_GATE, S4_CLKCTRL_SD_EMMC_CLK_CTRL, 23},
	{CLKID_SD_EMMC_C_GATE, S4_CLKCTRL_NAND_CLK_CTRL, 7},
};

static unsigned int spicc_parents[] = {CLKID_XTAL, CLKID_SYS_CLK,
CLKID_FCLK_DIV4, CLKID_FCLK_DIV3, CLKID_FCLK_DIV2,
CLKID_FCLK_DIV5, CLKID_FCLK_DIV7, CLKID_UNREALIZED};

static unsigned int saradc_parents[] = {CLKID_XTAL, CLKID_SYS_CLK};

static unsigned int sd_emmc_parents[] = {CLKID_XTAL, CLKID_FCLK_DIV2,
CLKID_FCLK_DIV3, CLKID_UNREALIZED, CLKID_UNREALIZED,
CLKID_UNREALIZED, CLKID_UNREALIZED, CLKID_UNREALIZED};

static struct meson_mux muxes[] = {
	{CLKID_SPICC_A_MUX, S4_CLKCTRL_SPICC_CLK_CTRL, 7,  0x7, spicc_parents, ARRAY_SIZE(spicc_parents)},
	{CLKID_SARADC_MUX, S4_CLKCTRL_SAR_CLK_CTRL, 9, 0x3, saradc_parents, ARRAY_SIZE(saradc_parents)},
	{CLKID_SD_EMMC_A_MUX, S4_CLKCTRL_SD_EMMC_CLK_CTRL, 9, 0x7, sd_emmc_parents, ARRAY_SIZE(sd_emmc_parents)},
	{CLKID_SD_EMMC_B_MUX, S4_CLKCTRL_SD_EMMC_CLK_CTRL, 25, 0x7, sd_emmc_parents, ARRAY_SIZE(sd_emmc_parents)},
	{CLKID_SD_EMMC_C_MUX, S4_CLKCTRL_NAND_CLK_CTRL, 9, 0x7, sd_emmc_parents, ARRAY_SIZE(sd_emmc_parents)},
};

static struct meson_div divs[] = {
	{CLKID_SPICC_A_DIV, S4_CLKCTRL_SPICC_CLK_CTRL, 0, 6, CLKID_SPICC_A_MUX},
	{CLKID_SARADC_DIV, S4_CLKCTRL_SAR_CLK_CTRL, 0, 8, CLKID_SARADC_MUX},
	{CLKID_SD_EMMC_A_DIV, S4_CLKCTRL_SD_EMMC_CLK_CTRL, 0, 7, CLKID_SD_EMMC_A_MUX},
	{CLKID_SD_EMMC_B_DIV, S4_CLKCTRL_SD_EMMC_CLK_CTRL, 16, 7, CLKID_SD_EMMC_B_MUX},
	{CLKID_SD_EMMC_C_DIV, S4_CLKCTRL_NAND_CLK_CTRL, 0, 7, CLKID_SD_EMMC_C_MUX},
};

static struct parm meson_fixed_pll_parm[3] = {
	{S4_ANACTRL_FIXPLL_CTRL0, 0, 8}, /* pm */
	{S4_ANACTRL_FIXPLL_CTRL0, 10, 5}, /* pn */
	{S4_ANACTRL_FIXPLL_CTRL0, 16, 2}, /* pod */
};

static struct parm meson_sys_pll_parm[3] = {
	{S4_ANACTRL_SYSPLL_CTRL0, 0, 8}, /* pm */
	{S4_ANACTRL_SYSPLL_CTRL0, 10, 5}, /* pn */
	{S4_ANACTRL_SYSPLL_CTRL0, 16, 3}, /* pod */
};

static int meson_clk_enable(struct clk *clk)
{
	return meson_set_gate_by_id(clk, gates, ARRAY_SIZE(gates), true);
}

static int meson_clk_disable(struct clk *clk)
{
	return meson_set_gate_by_id(clk, gates, ARRAY_SIZE(gates), false);
}

static ulong meson_pll_get_rate(struct clk *clk, unsigned long id)
{
	struct meson_clk *priv = dev_get_priv(clk->dev);
	struct parm *pm, *pn, *pod;
	unsigned long parent_rate_mhz = clk_get_rate(&priv->clkin)/1000000;
	u16 n, m, od;
	u32 reg;

	switch (id) {
	case CLKID_FIXED_PLL:
		pm = &meson_fixed_pll_parm[0];
		pn = &meson_fixed_pll_parm[1];
		pod = &meson_fixed_pll_parm[2];
		break;
	case CLKID_SYS_PLL:
		pm = &meson_sys_pll_parm[0];
		pn = &meson_sys_pll_parm[1];
		pod = &meson_sys_pll_parm[2];
		break;
	default:
		return -ENOENT;
	}

	reg = readl(priv->addr + pn->reg_off);
	n = PARM_GET(pn->width, pn->shift, reg);

	reg = readl(priv->addr + pm->reg_off);
	m = PARM_GET(pm->width, pm->shift, reg);

	/* there is OD in C1 */
	 reg = readl(priv->addr + pod->reg_off);
	od = PARM_GET(pod->width, pod->shift, reg);

	return ((parent_rate_mhz * m / n) >> od) * 1000000;
}

static ulong meson_clk_get_rate_by_id(struct clk *clk, ulong id)
{
	ulong rate;
	struct meson_clk *priv = dev_get_priv(clk->dev);

	switch (id) {
	case CLKID_XTAL:
		rate = clk_get_rate(&priv->clkin);
		break;
	case CLKID_FIXED_PLL:
	case CLKID_SYS_PLL:
		rate = meson_pll_get_rate(clk, id);
		break;
	case CLKID_FCLK_DIV2:
		rate = meson_pll_get_rate(clk, CLKID_FIXED_PLL) / 2;
		break;
	case CLKID_FCLK_DIV3:
		rate = meson_pll_get_rate(clk, CLKID_FIXED_PLL) / 3;
		break;
	case CLKID_FCLK_DIV4:
		rate = meson_pll_get_rate(clk, CLKID_FIXED_PLL) / 4;
		break;
	case CLKID_FCLK_DIV5:
		rate = meson_pll_get_rate(clk, CLKID_FIXED_PLL) / 5;
		break;
	case CLKID_FCLK_DIV7:
		rate = meson_pll_get_rate(clk, CLKID_FIXED_PLL) / 7;
		break;
	case CLKID_FCLK_DIV2P5:
		rate = (meson_pll_get_rate(clk, CLKID_FIXED_PLL) * 2) / 5;
		break;
	/* sys clk has realized in rom code*/
	case CLKID_SYS_CLK:
		rate = SYS_CLK;
		break;
	default:
		/* only for DIV type, others such as GATE/MUX is always 0 */
		rate = priv->actual_rate;
		break;
	}

	return rate;
}

static ulong meson_clk_get_rate(struct clk *clk)
{
	return meson_clk_get_rate_by_id(clk, clk->id);
}

static ulong meson_clk_set_rate(struct clk *clk, ulong rate)
{
	ulong div_parent, mux_parent, parent_rate;
	unsigned int div_val;
	struct meson_clk *priv = dev_get_priv(clk->dev);
	unsigned int i;
	struct meson_div *div = NULL;

	for (i = 0; i < ARRAY_SIZE(divs); i++) {
		if (clk->id == divs[i].index)
			div = &divs[i];
	}
	div_parent = div->parent_index;
	mux_parent = meson_clk_get_mux_parent(clk, muxes,
					ARRAY_SIZE(muxes), div_parent);
	parent_rate = meson_clk_get_rate_by_id(clk, mux_parent);

	div_val = DIV_ROUND_CLOSEST(parent_rate, rate) - 1;

	priv->actual_rate = DIV_ROUND_CLOSEST(parent_rate, div_val + 1);
	meson_clk_set_div(priv, div, div_val);

	return 0;
}

static int meson_clk_set_parent(struct clk* clk, struct clk* parent_clk)
{
	return meson_mux_set_parent_by_id(clk, muxes, ARRAY_SIZE(muxes), parent_clk);
}

static struct clk_ops meson_clk_ops = {
	.disable	= meson_clk_disable,
	.enable		= meson_clk_enable,
	.get_rate	= meson_clk_get_rate,
	.set_rate 	= meson_clk_set_rate,
	.set_parent 	= meson_clk_set_parent,
};

static int meson_clk_probe(struct udevice *dev)
{
	struct meson_clk *priv = dev_get_priv(dev);

	clk_get_by_name(dev, "xtal", &priv->clkin);
	priv->addr = dev_read_addr_ptr(dev);

	debug("meson-clk: probed at addr %p\n", priv->addr);

	return 0;
}

static const struct udevice_id meson_clk_ids[] = {
	{ .compatible = "amlogic,s4-clkc" },
	{ }
};

U_BOOT_DRIVER(meson_clk) = {
	.name		= "meson-clk-s4",
	.id		= UCLASS_CLK,
	.of_match	= meson_clk_ids,
	.priv_auto_alloc_size = sizeof(struct meson_clk),
	.ops		= &meson_clk_ops,
	.probe		= meson_clk_probe,
};
