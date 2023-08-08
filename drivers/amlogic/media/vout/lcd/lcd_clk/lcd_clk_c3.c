// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "../lcd_reg.h"
#include "../lcd_common.h"
#include "lcd_clk_config.h"
#include "lcd_clk_ctrl.h"
#include "lcd_clk_utils.h"

static struct lcd_clk_ctrl_s pll_ctrl_table_c3[] = {
	/* flag             reg                 bit              len*/
	{LCD_CLK_CTRL_EN,   ANACTRL_GP0PLL_CTRL0, 28,   1},
	{LCD_CLK_CTRL_RST,  ANACTRL_GP0PLL_CTRL0, 29,   1},
	{LCD_CLK_CTRL_FRAC, ANACTRL_GP0PLL_CTRL1, 0,    17},
	{LCD_CLK_CTRL_END,  LCD_CLK_REG_END,      0,    0},
};

static const unsigned int od_table[6] = {1, 2, 4, 8, 16, 32};

static void lcd_set_pll(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl, pll_ctrl1;
	int ret, cnt = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	pll_ctrl = ((cconf->pll_od1_sel << 16) |
		(cconf->pll_n << 10) |
		(cconf->pll_m << 0));
	pll_ctrl1 = cconf->pll_frac;

set_pll_retry_c3:
	lcd_ana_write(ANACTRL_GP0PLL_CTRL0, pll_ctrl);
	lcd_ana_write(ANACTRL_GP0PLL_CTRL1, pll_ctrl1);
	lcd_ana_write(ANACTRL_GP0PLL_CTRL2, 0x00023041);
	lcd_ana_write(ANACTRL_GP0PLL_CTRL3, 0x48681c00);
	lcd_ana_write(ANACTRL_GP0PLL_CTRL4, 0x88770290);
	lcd_ana_write(ANACTRL_GP0PLL_CTRL5, 0x3927200a);
	lcd_ana_setb(ANACTRL_GP0PLL_CTRL0, 3, 28, 2);
	lcd_ana_setb(ANACTRL_GP0PLL_CTRL0, 0, 29, 1);
	udelay(20);
	lcd_ana_write(ANACTRL_GP0PLL_CTRL2, 0x00023001);
	udelay(20);

	ret = lcd_pll_wait_lock(ANACTRL_GP0PLL_STS, 31);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_c3;
		LCDERR("[%d]: pll lock failed\n", pdrv->index);
	}
}

static void lcd_set_dsi_phy_clk(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	lcd_clk_setb(CLKCTRL_MIPIDSI_PHY_CLK_CTRL, (cconf->phy_div - 1), 0, 7);
	lcd_clk_setb(CLKCTRL_MIPIDSI_PHY_CLK_CTRL, 1, 12, 3);
	lcd_clk_setb(CLKCTRL_MIPIDSI_PHY_CLK_CTRL, 1, 8, 1);
}

static void lcd_clk_set_c3(struct aml_lcd_drv_s *pdrv)
{
	lcd_set_pll(pdrv);
	if (pdrv->config.basic.lcd_type == LCD_MIPI)
		lcd_set_dsi_phy_clk(pdrv);
}

static void lcd_set_vclk_crt(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("%s\n", __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

#ifdef CONFIG_AML_LCD_PXP //143M
		lcd_clk_setb(CLKCTRL_VOUTENC_CLK_CTRL, 1, 16, 7);
		lcd_clk_setb(CLKCTRL_VOUTENC_CLK_CTRL, 7, 25, 3);
#else
		lcd_clk_setb(CLKCTRL_VOUTENC_CLK_CTRL, (cconf->xd - 1), 16, 7);
		lcd_clk_setb(CLKCTRL_VOUTENC_CLK_CTRL, cconf->data->vclk_sel, 25, 3);
#endif
	lcd_clk_setb(CLKCTRL_VOUTENC_CLK_CTRL, 1, 24, 1);
}

static void lcd_clk_disable_c3(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	struct lcd_clk_ctrl_s *table;
	int i = 0;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	lcd_clk_setb(CLKCTRL_VOUTENC_CLK_CTRL, 0, 24, 1);

	if (!cconf->data->pll_ctrl_table)
		return;
	table = cconf->data->pll_ctrl_table;
	while (i < LCD_CLK_CTRL_CNT_MAX) {
		if (table[i].flag == LCD_CLK_CTRL_END)
			break;
		if (table[i].flag == LCD_CLK_CTRL_EN)
			lcd_ana_setb(table[i].reg, 0, table[i].bit, table[i].len);
		else if (table[i].flag == LCD_CLK_CTRL_RST)
			lcd_ana_setb(table[i].reg, 1, table[i].bit, table[i].len);
		i++;
	}
}

static void lcd_pll_frac_generate_c3(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned long long pll_fout, pll_fvco;
	unsigned int enc_clk, od;
	int ret;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	enc_clk = pconf->timing.lcd_clk;
	od = od_table[cconf->pll_od1_sel];
	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2) {
		LCDPR("m=%d, n=%d, od=%d, xd=%d\n",
			cconf->pll_m, cconf->pll_n, cconf->pll_od1_sel, cconf->xd);
	}
	if (enc_clk > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, enc_clk);
		return;
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("%s pclk=%d\n", __func__, enc_clk);

	pll_fout = enc_clk;
	pll_fout *= cconf->xd;
	if (pll_fout > cconf->data->pll_out_fmax || pll_fout < cconf->data->pll_out_fmin) {
		LCDERR("%s: wrong pll_fout value %lldHz\n", __func__, pll_fout);
		return;
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("%s pll_fout=%lld\n", __func__, pll_fout);

	pll_fvco = pll_fout * od;
	if (pll_fvco < cconf->data->pll_vco_fmin || pll_fvco > cconf->data->pll_vco_fmax) {
		LCDERR("%s: wrong pll_fvco value %lldHz\n", __func__, pll_fvco);
		return;
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("%s pll_fvco=%lld\n", __func__, pll_fvco);

	ret = lcd_pll_get_frac(cconf, pll_fvco);
	if (ret == 0) {
		cconf->fout = enc_clk;
		cconf->pll_fout = pll_fout;
		cconf->pll_fvco = pll_fvco;
		pconf->timing.clk_ctrl &= ~(0x1ffffff);
		pconf->timing.clk_ctrl |=
			(cconf->pll_frac << CLK_CTRL_FRAC) |
			(cconf->pll_frac_half_shift << CLK_CTRL_FRAC_SHIFT);
	}
}

static void lcd_clk_generate_c3(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned long long pll_fout, bit_rate, bit_rate_max = 0, bit_rate_min = 0, tmp;
	unsigned int xd, phy_div;
	int done = 0;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	cconf->fout = pconf->timing.lcd_clk;
	cconf->err_fmin = MAX_ERROR;

	if (cconf->fout > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, cconf->fout);
		goto generate_clk_done_c3;
	}

	switch (pconf->basic.lcd_type) {
	case LCD_MIPI:
		cconf->xd_max = CRT_VID_DIV_MAX_C3;
		bit_rate_max = pconf->control.mipi_cfg.local_bit_rate_max;
		bit_rate_min = pconf->control.mipi_cfg.local_bit_rate_min;
		tmp = bit_rate_max - cconf->fout;
		if (tmp >= bit_rate_min)
			bit_rate_min = tmp;
		if (lcd_debug_print_flag & LCD_DBG_PR_ADV2) {
			LCDPR("fout=%d, dsi_bit_rate max=%lld, min=%lld, xd_max=%d\n",
				 cconf->fout, bit_rate_max,
				 bit_rate_min, cconf->xd_max);
		}

		for (xd = 1; xd <= cconf->xd_max; xd++) {
			done = 0;
			pll_fout = cconf->fout;
			pll_fout *= xd;
			if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
				LCDPR("pll_fout=%lld, xd=%d\n", pll_fout, xd);
			for (phy_div = 1; phy_div <= PHY_CLK_DIV_MAX_C3; phy_div++) {
				bit_rate = lcd_do_div(pll_fout, phy_div);
				if (bit_rate <= bit_rate_max &&
				    bit_rate >= bit_rate_min) {
					done = 1;
					break;
				}
			}
			if (done == 0)
				continue;
			if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
				LCDPR("bit_rate=%lld, phy_div=%d\n", bit_rate, phy_div);

			cconf->phy_clk = bit_rate;
			pconf->timing.bit_rate = cconf->phy_clk;
			pconf->control.mipi_cfg.clk_factor = xd / phy_div;
			cconf->xd = xd;
			cconf->phy_div = phy_div;
			done = check_pll_1od(cconf, pll_fout);
			if (done)
				goto generate_clk_done_c3;
		}
		break;
	default:
		cconf->xd_max = CRT_VID_DIV_MAX_C3;
		for (xd = 1; xd <= cconf->xd_max; xd++) {
			done = 0;
			pll_fout = cconf->fout;
			pll_fout *= xd;
			if (lcd_debug_print_flag & LCD_DBG_PR_ADV2) {
				LCDPR("fout=%d, pll_fout=%lld, xd=%d\n",
					cconf->fout, pll_fout, xd);
			}
			cconf->xd = xd;
			done = check_pll_1od(cconf, pll_fout);
			if (done)
				goto generate_clk_done_c3;
		}
		break;
	}

generate_clk_done_c3:
	if (done) {
		pconf->timing.pll_ctrl =
			(cconf->pll_od1_sel << PLL_CTRL_OD1) |
			(cconf->pll_n << PLL_CTRL_N) |
			(cconf->pll_m << PLL_CTRL_M);
		pconf->timing.div_ctrl =
			(CLK_DIV_SEL_1 << DIV_CTRL_DIV_SEL) |
			(cconf->xd << DIV_CTRL_XD);
		pconf->timing.clk_ctrl =
			(cconf->pll_frac << CLK_CTRL_FRAC);
		cconf->done = 1;
	} else {
		cconf->done = 0;
		LCDERR("Out of clock range\n");
	}
}

static void lcd_clk_config_print_c3(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	LCDPR("lcd clk config:\n"
		"vclk_sel      %d\n"
		"pll_m:        %d\n"
		"pll_n:        %d\n"
		"pll_frac:     0x%x\n"
		"pll_fvco:     %lldHz\n"
		"pll_od:       %d\n"
		"pll_out:      %lldHz\n"
		"xd:           %d\n"
		"phy_div:      %d\n"
		"fout:         %dHz\n\n",
		cconf->data->vclk_sel,
		cconf->pll_m, cconf->pll_n,
		cconf->pll_frac, cconf->pll_fvco,
		cconf->pll_od1_sel, cconf->pll_fout,
		cconf->xd, cconf->phy_div, cconf->fout);
}

static struct lcd_clk_data_s lcd_clk_data_c3 = {
	.pll_od_fb = 0,
	.pll_m_max = 511,
	.pll_m_min = 2,
	.pll_n_max = 1,
	.pll_n_min = 1,
	.pll_frac_range = (1 << 17),
	.pll_frac_sign_bit = 18,
	.pll_od_sel_max = 5,
	.pll_ref_fmax = 25000000,
	.pll_ref_fmin = 5000000,
	.pll_vco_fmax = 6000000000ULL,
	.pll_vco_fmin = 3000000000ULL,
	.pll_out_fmax = 6000000000ULL,
	.pll_out_fmin = 375000000,
	.div_in_fmax = 1600000000,
	.div_out_fmax = 1600000000,
	.xd_out_fmax = 200000000,

	.vclk_sel = 4, //gp0_pll
	.enc_clk_msr_id = -1,
	.pll_ctrl_table = pll_ctrl_table_c3,

	.ss_support = 0,

	.clk_generate_parameter = lcd_clk_generate_c3,
	.pll_frac_generate = lcd_pll_frac_generate_c3,
	.set_ss_level = NULL,
	.set_ss_advance = NULL,
	.clk_ss_enable = NULL,
	.clk_set = lcd_clk_set_c3,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_disable = lcd_clk_disable_c3,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_c3,
	.prbs_clk_config = NULL,
	.prbs_test = NULL,
};

void lcd_clk_config_chip_init_c3(struct aml_lcd_drv_s *pdrv, struct lcd_clk_config_s *cconf)
{
	cconf->data = &lcd_clk_data_c3;
	cconf->pll_od_fb = lcd_clk_data_c3.pll_od_fb;
}
