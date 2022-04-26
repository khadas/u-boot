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
#include <dt-bindings/clock/a1-clkc.h>
#include "clk_meson.h"

/* change it later */
#define SYS_CLK		64000000

/* clk81 gates */
static struct meson_gate gates[] = {
	{CLKID_SPIFC_XTAL_GATE,	A1_SPIFC_CLK_CTRL, 15},
	{CLKID_SPIFC_GATE, A1_SPIFC_CLK_CTRL, 8},
	{CLKID_SPICC_A_GATE, A1_SPICC_CLK_CTRL, 8},
	{CLKID_SAR_ADC_GATE, A1_SAR_ADC_CLK_CTRL, 8},
	{CLKID_SD_EMMC_XTAL_GATE, A1_SD_EMMC_CLK_CTRL, 15},
	{CLKID_SD_EMMC_GATE, A1_SD_EMMC_CLK_CTRL, 8},
	{CLKID_I2C_M_A, A1_SYS_CLK_EN0, 23},
	{CLKID_I2C_M_B, A1_SYS_CLK_EN0, 22},
	{CLKID_I2C_M_C, A1_SYS_CLK_EN0, 21},
	{CLKID_I2C_M_D, A1_SYS_CLK_EN0, 20},
};

static unsigned int spifc_parents[] = {CLKID_FCLK_DIV2, CLKID_FCLK_DIV3,
CLKID_FCLK_DIV5, CLKID_UNREALIZED};

static unsigned int saradc_parents[] = {CLKID_XTAL, CLKID_SYS_CLK};
static unsigned int sd_emmc_parents[] = {CLKID_FCLK_DIV2, CLKID_FCLK_DIV3,
CLKID_FCLK_DIV5, CLKID_UNREALIZED};

static struct meson_mux muxes[] = {
		{CLKID_SPIFC_MUX, A1_SPIFC_CLK_CTRL, 9,  0x3, spifc_parents, ARRAY_SIZE(spifc_parents)},
		{CLKID_SPICC_A_MUX, A1_SPICC_CLK_CTRL, 9,  0x7, spifc_parents, ARRAY_SIZE(spifc_parents)},
		{CLKID_SARADC_MUX, A1_SAR_ADC_CLK_CTRL, 9,  0x1, saradc_parents, ARRAY_SIZE(saradc_parents)},
		{CLKID_SD_EMMC_MUX, A1_SD_EMMC_CLK_CTRL, 9,  0x3, sd_emmc_parents, ARRAY_SIZE(sd_emmc_parents)},
};

static struct meson_div divs[] = {
		{CLKID_SPIFC_DIV, A1_SPIFC_CLK_CTRL, 0,  8, CLKID_SPIFC_MUX},
		{CLKID_SPICC_A_DIV, A1_SPICC_CLK_CTRL, 0,  8, CLKID_SPICC_A_MUX},
		{CLKID_SARADC_DIV, A1_SAR_ADC_CLK_CTRL, 0,  8, CLKID_SARADC_MUX},
		{CLKID_SD_EMMC_DIV, A1_SD_EMMC_CLK_CTRL, 0, 8, CLKID_SD_EMMC_MUX},
};

static struct parm meson_fixed_pll_parm[3] = {
	{A1_ANACTRL_FIXPLL_CTRL0, 0, 8}, /* pm */
	{A1_ANACTRL_FIXPLL_CTRL0, 10, 5}, /* pn */
	/* {ANACTRL_FIXPLL_CTRL0, 16, 2},  pod */
};

static struct parm meson_sys_pll_parm[3] = {
	{A1_ANACTRL_SYSPLL_CTRL0, 0, 8}, /* pm */
	{A1_ANACTRL_SYSPLL_CTRL0, 10, 5}, /* pn */
	/* {A1_ANACTRL_SYSPLL_CTRL0, 16, 2},  pod */
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
	struct parm *pm, *pn;
	unsigned long parent_rate_mhz = clk_get_rate(&priv->clkin)/1000000;
	u16 n, m;
	u32 reg;

	switch (id) {
	case CLKID_FIXED_PLL:
		pm = &meson_fixed_pll_parm[0];
		pn = &meson_fixed_pll_parm[1];
		/*pod = &meson_fixed_pll_parm[2];*/
		break;
	case CLKID_SYS_PLL:
		pm = &meson_sys_pll_parm[0];
		pn = &meson_sys_pll_parm[1];
		/*pod = &meson_sys_pll_parm[2];*/
		break;
	default:
		return -ENOENT;
	}

	reg = readl(priv->addr + pn->reg_off);
	n = PARM_GET(pn->width, pn->shift, reg);

	reg = readl(priv->addr + pm->reg_off);
	m = PARM_GET(pm->width, pm->shift, reg);

	/* there is no OD in A1
	 reg = readl(priv->addr + pod->reg_off);
	od = PARM_GET(pod->width, pod->shift, reg);
	((parent_rate_mhz * m / n) >> od) * 1000000;
	*/

	return (parent_rate_mhz * m / n) * 1000000;
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
	case CLKID_FCLK_DIV5:
		rate = meson_pll_get_rate(clk, CLKID_FIXED_PLL) / 5;
		break;
	case CLKID_FCLK_DIV7:
		rate = meson_pll_get_rate(clk, CLKID_FIXED_PLL) / 7;
		break;
	/* sys clk has realized in rom code*/
	case CLKID_SYS_CLK:
		rate = SYS_CLK;
		break;
	default:
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
	.set_parent = meson_clk_set_parent,
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
	{ .compatible = "amlogic,a1-clkc" },
	{ }
};

U_BOOT_DRIVER(meson_clk) = {
	.name		= "meson-clk-a1",
	.id			= UCLASS_CLK,
	.of_match	= meson_clk_ids,
	.priv_auto_alloc_size = sizeof(struct meson_clk),
	.ops		= &meson_clk_ops,
	.probe		= meson_clk_probe,
};
