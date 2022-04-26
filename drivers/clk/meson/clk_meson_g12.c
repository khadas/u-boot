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
#include <dt-bindings/clock/g12-clkc.h>
#include "clk_meson.h"

/* clk81 gates */
static struct meson_gate gates[] = {
		{CLKID_MIPI_DSI_HOST,	G12_HHI_GCLK_MPEG0, 3},
		{CLKID_ETH_PHY,		G12_HHI_GCLK_MPEG0, 4},
		{CLKID_SPICC0,		G12_HHI_GCLK_MPEG0, 8},
		{CLKID_I2C,		G12_HHI_GCLK_MPEG0, 9},
		{CLKID_UART0,		G12_HHI_GCLK_MPEG0, 13},
		{CLKID_SPICC1,		G12_HHI_GCLK_MPEG0, 14},
		{CLKID_MIPI_DSI_PHY,	G12_HHI_GCLK_MPEG0, 20},
		{CLKID_SD_EMMC_A,	G12_HHI_GCLK_MPEG0, 24},
		{CLKID_SD_EMMC_B,	G12_HHI_GCLK_MPEG0, 25},
		{CLKID_SD_EMMC_C,	G12_HHI_GCLK_MPEG0, 26},
		{CLKID_ETH_CORE,	G12_HHI_GCLK_MPEG1, 3},
		{CLKID_ADC,		G12_HHI_GCLK_MPEG1, 13},
		{CLKID_UART1,		G12_HHI_GCLK_MPEG1, 16},
		{CLKID_HTX_HDCP22,	G12_HHI_GCLK_MPEG2, 3},
		{CLKID_HTX_PCLK,	G12_HHI_GCLK_MPEG2, 4},
		{CLKID_UART2,		G12_HHI_GCLK_MPEG2, 15},
		{CLKID_VCLK2_VENCI0,	G12_HHI_GCLK_OTHER, 1},
		{CLKID_VCLK2_VENCI1,	G12_HHI_GCLK_OTHER, 2},
		{CLKID_VCLK2_VENCP0,	G12_HHI_GCLK_OTHER, 3},
		{CLKID_VCLK2_VENCP1,	G12_HHI_GCLK_OTHER, 4},
		{CLKID_VCLK2_VENCT0,	G12_HHI_GCLK_OTHER, 5},
		{CLKID_VCLK2_VENCT1,	G12_HHI_GCLK_OTHER, 6},
		{CLKID_VCLK2_OTHER,	G12_HHI_GCLK_OTHER, 7},
		{CLKID_VCLK2_ENCI,	G12_HHI_GCLK_OTHER, 8},
		{CLKID_VCLK2_ENCP,	G12_HHI_GCLK_OTHER, 9},
		{CLKID_VCLK2_ENCT,	G12_HHI_GCLK_OTHER, 22},
		{CLKID_VCLK2_ENCL,	G12_HHI_GCLK_OTHER, 23},
		{CLKID_VCLK2_VENCLMMC,	G12_HHI_GCLK_OTHER, 24},
		{CLKID_VCLK2_VENCL,	G12_HHI_GCLK_OTHER, 25},
		{CLKID_VCLK2_OTHER1,	G12_HHI_GCLK_OTHER, 26},
		{CLKID_SPICC0_GATE,	G12_HHI_SPICC_CLK_CNTL, 6},
		{CLKID_SPICC1_GATE,	G12_HHI_SPICC_CLK_CNTL, 22},
		{CLKID_SARADC_GATE,	G12_AO_SAR_CLK, 8},
		{CLKID_AO_I2C,		G12_AO_CLK_GATE0, 2},
		{CLKID_SD_EMMC_A_P0_GATE,       G12_HHI_SD_EMMC_CLK_CNTL, 7},
		{CLKID_SD_EMMC_B_P0_GATE,       G12_HHI_SD_EMMC_CLK_CNTL, 23},
		{CLKID_SD_EMMC_C_P0_GATE, G12_HHI_NAND_CLK_CNTL, 7},
};

static unsigned int spicc_parents[] = {CLKID_XTAL, CLKID_CLK81, CLKID_FCLK_DIV4,
CLKID_FCLK_DIV3, CLKID_UNREALIZED, CLKID_FCLK_DIV5, CLKID_FCLK_DIV7, CLKID_UNREALIZED};

static unsigned int saradc_parents[] = {CLKID_XTAL, CLKID_AO_CLK81};
static unsigned int sd_emmc_parents[] = {CLKID_XTAL, CLKID_FCLK_DIV2, CLKID_FCLK_DIV3,
		CLKID_FCLK_DIV5, CLKID_FCLK_DIV7, CLKID_UNREALIZED, CLKID_UNREALIZED, CLKID_UNREALIZED};


static struct meson_mux muxes[] = {
		{CLKID_SPICC0_MUX, G12_HHI_SPICC_CLK_CNTL, 7,  7, spicc_parents, ARRAY_SIZE(spicc_parents)},
		{CLKID_SPICC1_MUX, G12_HHI_SPICC_CLK_CNTL, 23, 7, spicc_parents, ARRAY_SIZE(spicc_parents)},
		{CLKID_SARADC_MUX, G12_AO_SAR_CLK, 9, 3, saradc_parents, ARRAY_SIZE(saradc_parents)},
		{CLKID_SD_EMMC_A_P0_MUX, G12_HHI_SD_EMMC_CLK_CNTL, 9, 7, sd_emmc_parents, ARRAY_SIZE(sd_emmc_parents)},
		{CLKID_SD_EMMC_B_P0_MUX, G12_HHI_SD_EMMC_CLK_CNTL, 25, 7, sd_emmc_parents, ARRAY_SIZE(sd_emmc_parents)},
		{CLKID_SD_EMMC_C_P0_MUX, G12_HHI_NAND_CLK_CNTL, 9, 7, sd_emmc_parents, ARRAY_SIZE(sd_emmc_parents)},
};

static struct meson_div divs[] = {
		{CLKID_SPICC0_DIV, G12_HHI_SPICC_CLK_CNTL, 0,  6, CLKID_SPICC0_MUX},
		{CLKID_SPICC1_DIV, G12_HHI_SPICC_CLK_CNTL, 16, 6, CLKID_SPICC1_MUX},
		{CLKID_SARADC_DIV, G12_AO_SAR_CLK, 0, 8, CLKID_SARADC_MUX},
		{CLKID_SD_EMMC_A_P0_DIV, G12_HHI_SD_EMMC_CLK_CNTL, 0, 7, CLKID_SD_EMMC_A_P0_MUX},
		{CLKID_SD_EMMC_B_P0_DIV, G12_HHI_SD_EMMC_CLK_CNTL, 16, 7, CLKID_SD_EMMC_B_P0_MUX},
		{CLKID_SD_EMMC_C_P0_DIV, G12_HHI_NAND_CLK_CNTL, 0, 7, CLKID_SD_EMMC_C_P0_MUX},
};

static struct parm meson_fixed_pll_parm[3] = {
	{G12_HHI_FIX_PLL_CNTL0, 0, 8}, /* pm */
	{G12_HHI_FIX_PLL_CNTL0, 10, 5}, /* pn */
	{G12_HHI_FIX_PLL_CNTL0, 16, 2}, /* pod */
};

static struct parm meson_sys_pll_parm[3] = {
	{G12_HHI_SYS_PLL_CNTL0, 0, 8}, /* pm */
	{G12_HHI_SYS_PLL_CNTL0, 10, 5}, /* pn */
	{G12_HHI_SYS_PLL_CNTL0, 16, 2}, /* pod */
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
	/* clk81 has realized in rom code*/
	case CLKID_CLK81:
	case CLKID_AO_CLK81:
		rate = CLK81_RATE;
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
	{ .compatible = "amlogic,g12a-clkc" },
	{ .compatible = "amlogic,g12a-aoclkc" },
	{ }
};

U_BOOT_DRIVER(meson_clk) = {
	.name		= "meson-clk-g12a",
	.id			= UCLASS_CLK,
	.of_match	= meson_clk_ids,
	.priv_auto_alloc_size = sizeof(struct meson_clk),
	.ops		= &meson_clk_ops,
	.probe		= meson_clk_probe,
};
