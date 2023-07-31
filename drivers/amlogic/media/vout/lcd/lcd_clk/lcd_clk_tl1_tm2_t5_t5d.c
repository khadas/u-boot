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

static struct lcd_clk_ctrl_s pll_ctrl_table_tl1[] = {
	/* flag             reg                 bit              len*/
	{LCD_CLK_CTRL_EN,   HHI_TCON_PLL_CNTL0, LCD_PLL_EN_TL1,   1},
	{LCD_CLK_CTRL_RST,  HHI_TCON_PLL_CNTL0, LCD_PLL_RST_TL1,  1},
	{LCD_CLK_CTRL_FRAC, HHI_TCON_PLL_CNTL1,               0, 17},
	{LCD_CLK_CTRL_END,  LCD_CLK_REG_END,                  0,  0},
};

static unsigned int pll_ss_reg_tl1[][2] = {
	/* dep_sel,  str_m  */
	{ 0,          0}, /* 0: disable */
	{ 4,          1}, /* 1: +/-0.1% */
	{ 4,          2}, /* 2: +/-0.2% */
	{ 4,          3}, /* 3: +/-0.3% */
	{ 4,          4}, /* 4: +/-0.4% */
	{ 4,          5}, /* 5: +/-0.5% */
	{ 4,          6}, /* 6: +/-0.6% */
	{ 4,          7}, /* 7: +/-0.7% */
	{ 4,          8}, /* 8: +/-0.8% */
	{ 4,          9}, /* 9: +/-0.9% */
	{ 4,         10}, /* 10: +/-1.0% */
	{ 11,         4}, /* 11: +/-1.1% */
	{ 12,         4}, /* 12: +/-1.2% */
	{ 10,         5}, /* 13: +/-1.25% */
	{ 8,          7}, /* 14: +/-1.4% */
	{ 6,         10}, /* 15: +/-1.5% */
	{ 8,          8}, /* 16: +/-1.6% */
	{ 11,         6}, /* 17: +/-1.65% */
	{ 8,          9}, /* 18: +/-1.8% */
	{ 11,         7}, /* 19: +/-1.925% */
	{ 10,         8}, /* 20: +/-2.0% */
	{ 12,         7}, /* 21: +/-2.1% */
	{ 11,         8}, /* 22: +/-2.2% */
	{ 9,         10}, /* 23: +/-2.25% */
	{ 12,         8}, /* 24: +/-2.4% */
	{ 10,        10}, /* 25: +/-2.5% */
	{ 10,        10}, /* 26: +/-2.5% */
	{ 12,         9}, /* 27: +/-2.7% */
	{ 11,        10}, /* 28: +/-2.75% */
	{ 11,        10}, /* 29: +/-2.75% */
	{ 12,        10}, /* 30: +/-3.0% */
};

static unsigned int tcon_div[5][3] = {
	/* div_mux, div2/4_sel, div4_bypass */
	{1, 0, 1},  /* div1 */
	{0, 0, 1},  /* div2 */
	{0, 1, 1},  /* div4 */
	{0, 0, 0},  /* div8 */
	{0, 1, 0},  /* div16 */
};

static void lcd_set_pll_ss_level(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int level, pll_ctrl2;
	unsigned int dep_sel, str_m;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	level = cconf->ss_level;
	pll_ctrl2 = lcd_ana_read(HHI_TCON_PLL_CNTL2);
	pll_ctrl2 &= ~((1 << 15) | (0xf << 16) | (0xf << 28));

	if (level > 0) {
		cconf->ss_en = 1;
		dep_sel = pll_ss_reg_tl1[level][0];
		str_m = pll_ss_reg_tl1[level][1];
		dep_sel = (dep_sel > 10) ? 10 : dep_sel;
		str_m = (str_m > 10) ? 10 : str_m;
		pll_ctrl2 |= ((1 << 15) | (dep_sel << 28) | (str_m << 16));
	} else {
		cconf->ss_en = 0;
	}
	lcd_ana_write(HHI_TCON_PLL_CNTL2, pll_ctrl2);

	LCDPR("set pll spread spectrum: %s\n", lcd_ss_level_table_tl1[level]);
}

static void lcd_set_pll_ss_advance(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl2;
	unsigned int freq, mode;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	freq = cconf->ss_freq;
	mode = cconf->ss_mode;
	pll_ctrl2 = lcd_ana_read(HHI_TCON_PLL_CNTL2);
	pll_ctrl2 &= ~(0x7 << 24); /* ss_freq */
	pll_ctrl2 |= (freq << 24);
	pll_ctrl2 &= ~(0x3 << 22); /* ss_mode */
	pll_ctrl2 |= (mode << 22);
	lcd_ana_write(HHI_TCON_PLL_CNTL2, pll_ctrl2);

	LCDPR("set pll spread spectrum: freq=%d, mode=%d\n", freq, mode);
}

static void lcd_set_pll(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl, pll_ctrl1;
	unsigned int tcon_div_sel;
	int ret, cnt = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("%s\n", __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	tcon_div_sel = cconf->pll_tcon_div_sel;
	pll_ctrl = ((0x3 << 17) | /* gate ctrl */
		(tcon_div[tcon_div_sel][2] << 16) |
		(cconf->pll_n << LCD_PLL_N_TL1) |
		(cconf->pll_m << LCD_PLL_M_TL1) |
		(cconf->pll_od3_sel << LCD_PLL_OD3_TL1) |
		(cconf->pll_od2_sel << LCD_PLL_OD2_TL1) |
		(cconf->pll_od1_sel << LCD_PLL_OD1_TL1));
	pll_ctrl1 = (1 << 28) |
		(tcon_div[tcon_div_sel][0] << 22) |
		(tcon_div[tcon_div_sel][1] << 21) |
		((1 << 20) | /* sdm_en */
		(cconf->pll_frac << 0));

set_pll_retry_tl1:
	lcd_ana_write(HHI_TCON_PLL_CNTL0, pll_ctrl);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, LCD_PLL_EN_TL1, 1);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL1, pll_ctrl1);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL2, 0x0000110c);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL3, 0x10051400);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL4, 0x0100c0, 0, 24);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL4, 0x8300c0, 0, 24);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 26, 1);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 0, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL2, 0x0000300c);

	ret = lcd_pll_wait_lock(HHI_TCON_PLL_CNTL0, LCD_PLL_LOCK_TL1);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_tl1;
		LCDERR("hpll lock failed\n");
	} else {
		udelay(100);
		lcd_ana_setb(HHI_TCON_PLL_CNTL2, 1, 5, 1);
	}

	if (cconf->ss_level > 0) {
		lcd_set_pll_ss_level(pdrv);
		lcd_set_pll_ss_advance(pdrv);
	}
}

static void lcd_set_pll_txhd2(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl, pll_ctrl1;
	int ret, cnt = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("%s\n", __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	pll_ctrl =
		(cconf->pll_n << 10) |
		(cconf->pll_m << 0) |
		(cconf->pll_od1_sel << 16) |
		(cconf->pll_od2_sel << 18) |
		(cconf->pll_od3_sel << 20) |
		(3 << 24);

	pll_ctrl1 = cconf->pll_frac;
	LCDPR("pll_m=0x%x, pll_n=0x%x, frac=0x%x, od1=%d, od2=%d, od3=%d\n",
	cconf->pll_m, cconf->pll_n, cconf->pll_frac,
	cconf->pll_od1_sel, cconf->pll_od2_sel, cconf->pll_od3_sel);

	lcd_ana_write(HHI_TCON_PLL_CNTL0, 1 << 29);
	lcd_ana_write(HHI_TCON_PLL_CNTL0, pll_ctrl);
	lcd_ana_write(HHI_TCON_PLL_CNTL1, pll_ctrl1);
set_pll_retry_txhd2:
	lcd_ana_write(HHI_TCON_PLL_CNTL2, 0x01000000);
	lcd_ana_write(HHI_TCON_PLL_CNTL3, 0x00258000);
	lcd_ana_write(HHI_TCON_PLL_CNTL4, 0x05501000);
	lcd_ana_write(HHI_TCON_PLL_CNTL5, 0x00150500);
	lcd_ana_write(HHI_TCON_PLL_CNTL6, 0x50450000);
	udelay(50);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 28, 1);
	udelay(50);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 29, 1);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 0, 24, 2);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 23, 1);
	udelay(50);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 15, 1);
	lcd_ana_write(HHI_TCON_PLL_CNTL6, 0x50440000);

	ret = lcd_pll_wait_lock(HHI_TCON_PLL_STS, 31);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_txhd2;
		LCDERR("hpll lock failed\n");
	}
}

static void lcd_set_clk_phase_txhd2(unsigned int phase_value)
{
	// set clock phase value
	lcd_ana_setb(HHI_TCON_PLL_CNTL1, phase_value, 20, 12);

	// set clock phase load sequence
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 25, 1);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 0, 23, 1);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 0, 25, 1);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 23, 1);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 25, 1);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 0, 25, 1);
}

static void lcd_pll_ss_enable(struct aml_lcd_drv_s *pdrv, int status)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl2;
	unsigned int level, flag;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	level = cconf->ss_level;
	pll_ctrl2 = lcd_ana_read(HHI_TCON_PLL_CNTL2);
	if (status) {
		if (level > 0)
			flag = 1;
		else
			flag = 0;
	} else {
		flag = 0;
	}
	if (flag) {
		cconf->ss_en = 1;
		pll_ctrl2 |= (1 << 15);
		LCDPR("pll ss enable: %s\n", lcd_ss_level_table_tl1[level]);
	} else {
		cconf->ss_en = 0;
		pll_ctrl2 &= ~(1 << 15);
		LCDPR("pll ss disable\n");
	}
	lcd_ana_write(HHI_TCON_PLL_CNTL2, pll_ctrl2);
}

static void lcd_set_tcon_clk_tl1(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int val;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	switch (pconf->basic.lcd_type) {
	case LCD_MLVDS:
		val = pconf->control.mlvds_cfg.clk_phase & 0xfff;
		lcd_ana_setb(HHI_TCON_PLL_CNTL1, (val & 0xf), 24, 4);
		lcd_ana_setb(HHI_TCON_PLL_CNTL4, ((val >> 4) & 0xf), 28, 4);
		lcd_ana_setb(HHI_TCON_PLL_CNTL4, ((val >> 8) & 0xf), 24, 4);

		/* tcon_clk */
		if (pconf->timing.lcd_clk >= 100000000) /* 25M */
			lcd_clk_write(HHI_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (0xf << 0));
		else /* 12.5M */
			lcd_clk_write(HHI_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (0x1f << 0));
		break;
	case LCD_P2P:
		/* tcon_clk 50M */
		lcd_clk_write(HHI_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (7 << 0));
		break;
	default:
		break;
	}
}

static void lcd_set_dsi_phy_clk(int index)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", index, __func__);
	lcd_clk_setb(HHI_MIPIDSI_PHY_CLK_CNTL, 0, 0, 7);
	lcd_clk_setb(HHI_MIPIDSI_PHY_CLK_CNTL, 0, 12, 3);
	lcd_clk_setb(HHI_MIPIDSI_PHY_CLK_CNTL, 1, 8, 1);
}

static void lcd_clk_set_tl1(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	lcd_set_tcon_clk_tl1(pdrv);
	lcd_set_pll(pdrv);
	lcd_set_vid_pll_div_dft(cconf);
}

static void lcd_set_tcon_clk_t5(struct aml_lcd_drv_s *pdrv)
{
	if (pdrv->config.basic.lcd_type != LCD_MLVDS &&
	    pdrv->config.basic.lcd_type != LCD_P2P)
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	lcd_set_tcon_clk_tl1(pdrv);

	/* global reset tcon */
	lcd_reset_setb(RESET1_MASK, 0, 4, 1);
	lcd_reset_setb(RESET1_LEVEL, 0, 4, 1);
	udelay(1);
	lcd_reset_setb(RESET1_LEVEL, 1, 4, 1);
	udelay(2);
	LCDPR("reset tcon\n");
}

static void lcd_set_tcon_clk_txhd2(struct aml_lcd_drv_s *pdrv)
{
	unsigned int val = 0;
	struct lcd_config_s *pconf = &pdrv->config;

	if (pdrv->config.basic.lcd_type != LCD_MLVDS)
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	switch (pconf->basic.lcd_type) {
	case LCD_MLVDS:
		val = pconf->control.mlvds_cfg.clk_phase & 0xfff;
		lcd_set_clk_phase_txhd2(val);

		/* tcon_clk */
		if (pconf->timing.lcd_clk >= 100000000) /* 25M */
			lcd_clk_write(HHI_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (0xf << 0));
		else /* 12.5M */
			lcd_clk_write(HHI_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (0x1f << 0));
		break;
	default:
		break;
	}

	/* global reset tcon */
	lcd_reset_setb(RESET1_MASK, 0, 4, 1);
	lcd_reset_setb(RESET1_LEVEL, 0, 4, 1);
	udelay(1);
	lcd_reset_setb(RESET1_LEVEL, 1, 4, 1);
	udelay(2);
	LCDPR("reset tcon\n");
}

static void lcd_clk_set_t5(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	lcd_set_tcon_clk_t5(pdrv);
	lcd_set_pll(pdrv);
	lcd_set_vid_pll_div_dft(cconf);
}

static void lcd_clk_set_txhd2(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	lcd_set_pll_txhd2(pdrv);
	lcd_set_tcon_clk_txhd2(pdrv);
	lcd_set_vid_pll_div_txhd2(cconf);

	if (pdrv->config.basic.lcd_type == LCD_MIPI) {
		// lcd_set_dsi_meas_clk(pdrv->index);
		lcd_set_dsi_phy_clk(pdrv->index);
	}
}

static void lcd_prbs_set_pll_vx1(struct aml_lcd_drv_s *pdrv)
{
	int cnt = 0, ret;

lcd_prbs_retry_pll_vx1_tl1:
	lcd_ana_write(HHI_TCON_PLL_CNTL0, 0x000f04f7);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, LCD_PLL_EN_TL1, 1);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL1, 0x10110000);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL2, 0x00001108);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL3, 0x10051400);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL4, 0x010100c0);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL4, 0x038300c0);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 26, 1);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 0, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL2, 0x00003008);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL2, 0x00003028);
	udelay(10);

	ret = lcd_pll_wait_lock(HHI_TCON_PLL_CNTL0, LCD_PLL_LOCK_TL1);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto lcd_prbs_retry_pll_vx1_tl1;
		LCDERR("pll lock failed\n");
	}

	/* pll_div */
	lcd_clk_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	udelay(5);

	/* Disable the div output clock */
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 19, 1);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);

	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 18, 1);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 16, 2);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 0, 14);

	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 2, 16, 2);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 1, 15, 1);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0x739c, 0, 15);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);

	/* Enable the final output clock */
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 1, 19, 1);
}

static void lcd_prbs_set_pll_lvds(struct aml_lcd_drv_s *pdrv)
{
	int cnt = 0, ret;

lcd_prbs_retry_pll_lvds_tl1:
	lcd_ana_write(HHI_TCON_PLL_CNTL0, 0x008e049f);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL0, 0x208e049f);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL0, 0x3006049f);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL1, 0x10000000);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL2, 0x00001102);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL3, 0x10051400);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL4, 0x010100c0);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL4, 0x038300c0);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL0, 0x348e049f);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL0, 0x148e049f);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL2, 0x00003002);
	udelay(10);
	lcd_ana_write(HHI_TCON_PLL_CNTL2, 0x00003022);
	udelay(10);

	ret = lcd_pll_wait_lock(HHI_TCON_PLL_CNTL0, LCD_PLL_LOCK_TL1);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto lcd_prbs_retry_pll_lvds_tl1;
		LCDERR("pll lock failed\n");
	}

	/* pll_div */
	lcd_clk_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	udelay(5);

	/* Disable the div output clock */
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 19, 1);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);

	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 18, 1);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 16, 2);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 0, 14);

	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 1, 16, 2);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 1, 15, 1);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0x3c78, 0, 15);
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);

	/* Enable the final output clock */
	lcd_ana_setb(HHI_VID_PLL_CLK_DIV, 1, 19, 1);
}

static void lcd_prbs_set_pll_lvds_txhd2(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	int cnt = 0, ret;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	//3840 / 8 / 10 = 48M
	//3840 / 4 / 4.67 = 137M
	lcd_ana_write(HHI_TCON_PLL_CNTL0, 1 << 29);
	lcd_ana_write(HHI_TCON_PLL_CNTL0, 0x031484a0);
	lcd_ana_write(HHI_TCON_PLL_CNTL1, 0x00008000);
lcd_prbs_retry_pll_lvds_txhd2:
	lcd_ana_write(HHI_TCON_PLL_CNTL2, 0x01000000);
	lcd_ana_write(HHI_TCON_PLL_CNTL3, 0x00258000);
	lcd_ana_write(HHI_TCON_PLL_CNTL4, 0x05501000);
	lcd_ana_write(HHI_TCON_PLL_CNTL5, 0x00150500);
	lcd_ana_write(HHI_TCON_PLL_CNTL6, 0x50450000);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 28, 1);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 29, 1);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 0, 24, 2);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 23, 1);
	udelay(10);
	lcd_ana_setb(HHI_TCON_PLL_CNTL0, 1, 15, 1);
	lcd_ana_write(HHI_TCON_PLL_CNTL6, 0x50440000);

	ret = lcd_pll_wait_lock(HHI_TCON_PLL_STS, 31);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto lcd_prbs_retry_pll_lvds_txhd2;
		LCDERR("[%d]: hpll lock failed\n", pdrv->index);
	}

	/* pll_div */
	lcd_clk_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	udelay(10);

	/* Disable the div output clock */
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 19, 1);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 15, 1);

	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 18, 1);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 16, 2);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 15, 1);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 0, 14);

	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL0_DIV, 1, 16, 2);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL0_DIV, 1, 15, 1);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL0_DIV, 0x0ccc, 0, 15);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 15, 1);

	/* Enable the final output clock */
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL0_DIV, 1, 19, 1);
}

static void lcd_prbs_config_clk(struct aml_lcd_drv_s *pdrv, unsigned int lcd_prbs_mode)
{
	if (pdrv->data->chip_type == LCD_CHIP_TXHD2) {
		lcd_clk_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
		lcd_prbs_set_pll_lvds_txhd2(pdrv);
	} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
		lcd_prbs_set_pll_vx1(pdrv);
	} else if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
		lcd_prbs_set_pll_lvds(pdrv);
	} else {
		LCDERR("%s: unsupport lcd_prbs_mode %d\n",
		       __func__, lcd_prbs_mode);
		return;
	}

	lcd_clk_setb(HHI_VIID_CLK_DIV, 0, VCLK2_XD, 8);
	udelay(5);

	/* select vid_pll_clk */
	lcd_clk_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_CLK_IN_SEL, 3);
	lcd_clk_setb(HHI_VIID_CLK_CNTL, 1, VCLK2_EN, 1);
	udelay(5);

	/* [15:12] encl_clk_sel, select vclk2_div1 */
	lcd_clk_setb(HHI_VIID_CLK_DIV, 8, ENCL_CLK_SEL, 4);
	/* release vclk2_div_reset and enable vclk2_div */
	lcd_clk_setb(HHI_VIID_CLK_DIV, 1, VCLK2_XD_EN, 2);
	udelay(5);

	lcd_clk_setb(HHI_VIID_CLK_CNTL, 1, VCLK2_DIV1_EN, 1);
	lcd_clk_setb(HHI_VIID_CLK_CNTL, 1, VCLK2_SOFT_RST, 1);
	udelay(10);
	lcd_clk_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_SOFT_RST, 1);
	udelay(5);

	/* enable CTS_ENCL clk gate */
	lcd_clk_setb(HHI_VID_CLK_CNTL2, 1, ENCL_GATE_VCLK, 1);

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("%s ok\n", __func__);
}

static int lcd_prbs_test(struct aml_lcd_drv_s *pdrv, unsigned int ms, unsigned int mode_flag)
{
	struct lcd_clk_config_s *cconf;
	unsigned int reg_phy_tx_ctrl0, reg_phy_tx_ctrl1;
	int encl_msr_id, fifo_msr_id;
	unsigned int lcd_prbs_mode;
	unsigned int val1, val2, s, timeout;
	unsigned int cnt = 0;
	unsigned int clk_err_cnt = 0;
	int i, j, ret;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return -1;

	reg_phy_tx_ctrl0 = HHI_LVDS_TX_PHY_CNTL0;
	reg_phy_tx_ctrl1 = HHI_LVDS_TX_PHY_CNTL1;
	encl_msr_id = 9;
	fifo_msr_id = 129;

	s = ms / 1000;
	s = (s == 0) ? 1 : ((s > 1800) ? 1800 : s);
	timeout = s * 200;
	for (i = 0; i < LCD_PRBS_MODE_MAX; i++) {
		if ((mode_flag & (1 << i)) == 0)
			continue;

		lcd_ana_write(reg_phy_tx_ctrl0, 0);
		lcd_ana_write(reg_phy_tx_ctrl1, 0);

		cnt = 0;
		clk_err_cnt = 0;
		lcd_prbs_mode = (1 << i);
		LCDPR("lcd_prbs_mode: %d\n", lcd_prbs_mode);
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_encl_clk_check_std = 136;
			lcd_fifo_clk_check_std = 48;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_encl_clk_check_std = 594;
			lcd_fifo_clk_check_std = 297;
		}
		if (cconf->data->prbs_clk_config) {
			cconf->data->prbs_clk_config(pdrv, lcd_prbs_mode);
		} else {
			LCDERR("%s: prbs_clk_config is null\n", __func__);
			goto lcd_prbs_test_end;
		}
		mdelay(20);

		lcd_ana_write(reg_phy_tx_ctrl0, 0x000000c0);
		lcd_ana_setb(reg_phy_tx_ctrl0, 0xfff, 16, 12);
		lcd_ana_setb(reg_phy_tx_ctrl0, 1, 2, 1);
		lcd_ana_write(reg_phy_tx_ctrl1, 0x41000000);
		lcd_ana_setb(reg_phy_tx_ctrl1, 1, 31, 1);

		lcd_ana_write(reg_phy_tx_ctrl0, 0xfff20c4);
		lcd_ana_setb(reg_phy_tx_ctrl0, 1, 12, 1);
		val1 = lcd_ana_getb(reg_phy_tx_ctrl1, 12, 12);

		while (cnt++ < timeout) {
			mdelay(5);
			ret = 1;
			for (j = 0; j < 5; j++) {
				val2 = lcd_ana_getb(reg_phy_tx_ctrl1, 12, 12);
				if (val2 != val1) {
					ret = 0;
					break;
				}
			}
			if (ret) {
				LCDERR("prbs check error 1, val:0x%03x, cnt:%d\n",
				       val2, cnt);
				goto lcd_prbs_test_err;
			}
			val1 = val2;
			if (lcd_ana_getb(reg_phy_tx_ctrl1, 0, 12)) {
				LCDERR("prbs check error 2, cnt:%d\n", cnt);
				goto lcd_prbs_test_err;
			}
			if (lcd_prbs_clk_check(lcd_encl_clk_check_std, encl_msr_id,
					       lcd_fifo_clk_check_std, fifo_msr_id,
					       cnt))
				clk_err_cnt++;
			else
				clk_err_cnt = 0;
			if (clk_err_cnt >= 10) {
				LCDERR("prbs check error 3(clkmsr), cnt:%d\n",
				       cnt);
				goto lcd_prbs_test_err;
			}
		}

		lcd_ana_write(reg_phy_tx_ctrl0, 0);
		lcd_ana_write(reg_phy_tx_ctrl1, 0);

		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_LVDS);
			LCDPR("lvds prbs check ok\n");
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_VX1);
			LCDPR("vx1 prbs check ok\n");
		} else {
			LCDPR("prbs check: unsupport mode\n");
		}
		continue;

lcd_prbs_test_err:
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err |= LCD_PRBS_MODE_LVDS;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err |= LCD_PRBS_MODE_VX1;
		}
	}

lcd_prbs_test_end:
	lcd_ana_setb(reg_phy_tx_ctrl0, 0, 12, 2);

	printf("\n[lcd prbs result]:\n");
	printf("  lvds prbs performed: %d, error: %d\n"
	       "  vx1 prbs performed: %d, error: %d\n",
	       (lcd_prbs_performed & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_performed & LCD_PRBS_MODE_VX1) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_VX1) ? 1 : 0);

	return 0;
}

static int lcd_clk_prbs_test_txhd2(struct aml_lcd_drv_s *pdrv,
				 unsigned int ms, unsigned int mode_flag)
{
	struct lcd_clk_config_s *cconf = get_lcd_clk_config(pdrv);
	unsigned int combo_dphy_ctrl0, combo_dphy_ctrl1, bit_width;
	int encl_msr_id, fifo_msr_id;
	unsigned int lcd_prbs_cnt;
	unsigned int val1, val2, timeout;
	unsigned int clk_err_cnt = 0;
	int j, ret;

	if (!cconf)
		return -1;
	if (!(mode_flag & LCD_PRBS_MODE_LVDS)) {
		LCDPR("%s: not support\n", __func__);
		goto lcd_prbs_test_err_txhd2;
	}

	//bit[15:0]: reg_hi_edp_lvds_tx_phy0_cntl0
	combo_dphy_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0;
	//bit[31:24]: reg_hi_edp_lvds_tx_phy0_cntl1
	//bit[19:0]: ro_hi_edp_lvds_tx_phy0_cntl1_o
	combo_dphy_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1;
	bit_width = 10;

	encl_msr_id = cconf->data->enc_clk_msr_id;
	fifo_msr_id = -1;

	timeout = (ms > 1000) ? 1000 : ms;

	lcd_combo_dphy_write(combo_dphy_ctrl0, 0);
	lcd_combo_dphy_write(combo_dphy_ctrl1, 0);

	lcd_prbs_cnt = 0;
	clk_err_cnt = 0;
	LCDPR("[%d]: lcd_prbs_mode: 0x%lx\n", pdrv->index, LCD_PRBS_MODE_LVDS);
	lcd_encl_clk_check_std = 136000000;
	lcd_fifo_clk_check_std = 48000000;

	lcd_prbs_config_clk(pdrv, LCD_PRBS_MODE_LVDS);
	udelay(500);

	/* set fifo_clk_sel: div 10 */
	// COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0[7:6]: Fifo_clk_sel
	lcd_combo_dphy_write(combo_dphy_ctrl0, (3 << 6));
	/* set cntl_ser_en:  10-channel */
	lcd_combo_dphy_setb(combo_dphy_ctrl0, 0x3ff, 16, 10);
	lcd_combo_dphy_setb(combo_dphy_ctrl0, 1, 2, 1);
	/* decoupling fifo enable, gated clock enable */
	lcd_combo_dphy_write(combo_dphy_ctrl1, (1 << 30) | (1 << 24));
	/* decoupling fifo write enable after fifo enable */
	lcd_combo_dphy_setb(combo_dphy_ctrl1, 1, 31, 1);

	/* cntl_prbs_en & cntl_prbs_err_en*/
	lcd_combo_dphy_setb(combo_dphy_ctrl0, 1, 13, 1);
	lcd_combo_dphy_setb(combo_dphy_ctrl0, 1, 12, 1);

	while (lcd_prbs_flag) {
		if (lcd_prbs_cnt++ >= timeout)
			break;
		ret = 1;
		val1 = lcd_combo_dphy_getb(combo_dphy_ctrl1, bit_width, bit_width);
		udelay(1000);

		for (j = 0; j < 20; j++) {
			val2 = lcd_combo_dphy_getb(combo_dphy_ctrl1, bit_width, bit_width);
			udelay(5);
			if (val2 != val1) {
				ret = 0;
				break;
			}
		}
		if (ret) {
			LCDERR("[%d]: prbs error 1, val:0x%03x, cnt:%d\n",
					pdrv->index, val2, lcd_prbs_cnt);
			goto lcd_prbs_test_err_txhd2;
		}
		if (lcd_combo_dphy_getb(combo_dphy_ctrl1, 0, bit_width)) {
			LCDERR("[%d]: prbs error 2, cnt:%d\n", pdrv->index, lcd_prbs_cnt);
			goto lcd_prbs_test_err_txhd2;
		}

		if (lcd_prbs_clk_check(lcd_encl_clk_check_std, encl_msr_id,
					lcd_fifo_clk_check_std, fifo_msr_id, lcd_prbs_cnt))
			clk_err_cnt++;
		else
			clk_err_cnt = 0;
		if (clk_err_cnt >= 10) {
			LCDERR("[%d]: prbs error 3(clkmsr), cnt:%d\n", pdrv->index, lcd_prbs_cnt);
			goto lcd_prbs_test_err_txhd2;
		}
	}

	lcd_combo_dphy_write(combo_dphy_ctrl0, 0);
	lcd_combo_dphy_write(combo_dphy_ctrl1, 0);

	lcd_prbs_performed = LCD_PRBS_MODE_LVDS;
	lcd_prbs_err = 0;
	lcd_prbs_flag = 0;
	LCDPR("[%d]: lvds prbs check ok\n", pdrv->index);
	return 0;

lcd_prbs_test_err_txhd2:
	lcd_prbs_performed = LCD_PRBS_MODE_LVDS;
	lcd_prbs_err = LCD_PRBS_MODE_LVDS;
	lcd_prbs_flag = 0;
	return -1;
}

static struct lcd_clk_data_s lcd_clk_data_tl1 = {
	.pll_od_fb = PLL_OD_FB_TL1,
	.pll_m_max = PLL_M_MAX,
	.pll_m_min = PLL_M_MIN,
	.pll_n_max = PLL_N_MAX,
	.pll_n_min = PLL_N_MIN,
	.pll_frac_range = PLL_FRAC_RANGE_TL1,
	.pll_frac_sign_bit = PLL_FRAC_SIGN_BIT_TL1,
	.pll_od_sel_max = PLL_OD_SEL_MAX_TL1,
	.pll_ref_fmax = PLL_FREF_MAX,
	.pll_ref_fmin = PLL_FREF_MIN,
	.pll_vco_fmax = PLL_VCO_MAX_TL1,
	.pll_vco_fmin = PLL_VCO_MIN_TL1,
	.pll_out_fmax = CLK_DIV_IN_MAX_TL1,
	.pll_out_fmin = PLL_VCO_MIN_TL1 / 16,
	.div_in_fmax = CLK_DIV_IN_MAX_TL1,
	.div_out_fmax = CRT_VID_CLK_IN_MAX_TL1,
	.xd_out_fmax = ENCL_CLK_IN_MAX_TL1,

	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.pll_ctrl_table = pll_ctrl_table_tl1,

	.ss_support = 1,

	.clk_generate_parameter = lcd_clk_generate_dft,
	.pll_frac_generate = lcd_pll_frac_generate_dft,
	.set_ss_level = lcd_set_pll_ss_level,
	.set_ss_advance = lcd_set_pll_ss_advance,
	.clk_ss_enable = lcd_pll_ss_enable,
	.clk_set = lcd_clk_set_tl1,
	.vclk_crt_set = lcd_set_vclk_crt_dft,
	.clk_disable = lcd_clk_disable_dft,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = lcd_prbs_config_clk,
	.prbs_test = lcd_prbs_test,
};

static struct lcd_clk_data_s lcd_clk_data_tm2 = {
	.pll_od_fb = PLL_OD_FB_TL1,
	.pll_m_max = PLL_M_MAX,
	.pll_m_min = PLL_M_MIN,
	.pll_n_max = PLL_N_MAX,
	.pll_n_min = PLL_N_MIN,
	.pll_frac_range = PLL_FRAC_RANGE_TL1,
	.pll_frac_sign_bit = PLL_FRAC_SIGN_BIT_TL1,
	.pll_od_sel_max = PLL_OD_SEL_MAX_TL1,
	.pll_ref_fmax = PLL_FREF_MAX,
	.pll_ref_fmin = PLL_FREF_MIN,
	.pll_vco_fmax = PLL_VCO_MAX_TM2,
	.pll_vco_fmin = PLL_VCO_MIN_TM2,
	.pll_out_fmax = CLK_DIV_IN_MAX_TL1,
	.pll_out_fmin = PLL_VCO_MIN_TL1 / 16,
	.div_in_fmax = CLK_DIV_IN_MAX_TL1,
	.div_out_fmax = CRT_VID_CLK_IN_MAX_TL1,
	.xd_out_fmax = ENCL_CLK_IN_MAX_TL1,

	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.pll_ctrl_table = pll_ctrl_table_tl1,

	.ss_support = 1,

	.clk_generate_parameter = lcd_clk_generate_dft,
	.pll_frac_generate = lcd_pll_frac_generate_dft,
	.set_ss_level = lcd_set_pll_ss_level,
	.set_ss_advance = lcd_set_pll_ss_advance,
	.clk_ss_enable = lcd_pll_ss_enable,
	.clk_set = lcd_clk_set_tl1,
	.vclk_crt_set = lcd_set_vclk_crt_dft,
	.clk_disable = lcd_clk_disable_dft,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = lcd_prbs_config_clk,
	.prbs_test = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_t5 = {
	.pll_od_fb = PLL_OD_FB_TL1,
	.pll_m_max = PLL_M_MAX,
	.pll_m_min = PLL_M_MIN,
	.pll_n_max = PLL_N_MAX,
	.pll_n_min = PLL_N_MIN,
	.pll_frac_range = PLL_FRAC_RANGE_TL1,
	.pll_frac_sign_bit = PLL_FRAC_SIGN_BIT_TL1,
	.pll_od_sel_max = PLL_OD_SEL_MAX_TL1,
	.pll_ref_fmax = PLL_FREF_MAX,
	.pll_ref_fmin = PLL_FREF_MIN,
	.pll_vco_fmax = PLL_VCO_MAX_TM2,
	.pll_vco_fmin = PLL_VCO_MIN_TM2,
	.pll_out_fmax = CLK_DIV_IN_MAX_TL1,
	.pll_out_fmin = PLL_VCO_MIN_TL1 / 16,
	.div_in_fmax = CLK_DIV_IN_MAX_TL1,
	.div_out_fmax = CRT_VID_CLK_IN_MAX_TL1,
	.xd_out_fmax = ENCL_CLK_IN_MAX_TL1,

	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.pll_ctrl_table = pll_ctrl_table_tl1,

	.ss_support = 1,

	.clk_generate_parameter = lcd_clk_generate_dft,
	.pll_frac_generate = lcd_pll_frac_generate_dft,
	.set_ss_level = lcd_set_pll_ss_level,
	.set_ss_advance = lcd_set_pll_ss_advance,
	.clk_ss_enable = lcd_pll_ss_enable,
	.clk_set = lcd_clk_set_t5,
	.vclk_crt_set = lcd_set_vclk_crt_dft,
	.clk_disable = lcd_clk_disable_dft,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = lcd_prbs_config_clk,
	.prbs_test = lcd_prbs_test,
};

static struct lcd_clk_data_s lcd_clk_data_t5d = {
	.pll_od_fb = PLL_OD_FB_TL1,
	.pll_m_max = PLL_M_MAX,
	.pll_m_min = PLL_M_MIN,
	.pll_n_max = PLL_N_MAX,
	.pll_n_min = PLL_N_MIN,
	.pll_frac_range = PLL_FRAC_RANGE_TL1,
	.pll_frac_sign_bit = PLL_FRAC_SIGN_BIT_TL1,
	.pll_od_sel_max = PLL_OD_SEL_MAX_TL1,
	.pll_ref_fmax = PLL_FREF_MAX,
	.pll_ref_fmin = PLL_FREF_MIN,
	.pll_vco_fmax = PLL_VCO_MAX_TM2,
	.pll_vco_fmin = PLL_VCO_MIN_TM2,
	.pll_out_fmax = CLK_DIV_IN_MAX_T5D,
	.pll_out_fmin = PLL_VCO_MIN_TL1 / 16,
	.div_in_fmax = CLK_DIV_IN_MAX_T5D,
	.div_out_fmax = CRT_VID_CLK_IN_MAX_T5D,
	.xd_out_fmax = ENCL_CLK_IN_MAX_T5D,

	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.pll_ctrl_table = pll_ctrl_table_tl1,

	.ss_support = 1,

	.clk_generate_parameter = lcd_clk_generate_dft,
	.pll_frac_generate = lcd_pll_frac_generate_dft,
	.set_ss_level = lcd_set_pll_ss_level,
	.set_ss_advance = lcd_set_pll_ss_advance,
	.clk_ss_enable = lcd_pll_ss_enable,
	.clk_set = lcd_clk_set_t5,
	.vclk_crt_set = lcd_set_vclk_crt_dft,
	.clk_disable = lcd_clk_disable_dft,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = lcd_prbs_config_clk,
	.prbs_test = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_txhd2 = {
	.pll_od_fb = PLL_OD_FB_TL1,
	.pll_m_max = PLL_M_MAX,
	.pll_m_min = PLL_M_MIN,
	.pll_n_max = PLL_N_MAX,
	.pll_n_min = PLL_N_MIN,
	.pll_frac_range = PLL_FRAC_RANGE_TL1,
	.pll_frac_sign_bit = PLL_FRAC_SIGN_BIT_TL1,
	.pll_od_sel_max = PLL_OD_SEL_MAX_TL1,
	.pll_ref_fmax = PLL_FREF_MAX,
	.pll_ref_fmin = PLL_FREF_MIN,
	.pll_vco_fmax = PLL_VCO_MAX_TM2,
	.pll_vco_fmin = PLL_VCO_MIN_TM2,
	.pll_out_fmax = CLK_DIV_IN_MAX_T5D,
	.pll_out_fmin = PLL_VCO_MIN_TL1 / 16,
	.div_in_fmax = CLK_DIV_IN_MAX_T5D,
	.div_out_fmax = CRT_VID_CLK_IN_MAX_T5D,
	.xd_out_fmax = ENCL_CLK_IN_MAX_T5D,

	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.pll_ctrl_table = pll_ctrl_table_tl1,

	.ss_support = 0,

	.clk_generate_parameter = lcd_clk_generate_dft,
	.pll_frac_generate = lcd_pll_frac_generate_dft,
	.set_ss_level = lcd_set_pll_ss_level,
	.set_ss_advance = NULL,
	.clk_ss_enable = NULL,
	.clk_set = lcd_clk_set_txhd2,
	.vclk_crt_set = lcd_set_vclk_crt_dft,
	.clk_disable = lcd_clk_disable_dft,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = NULL,
	.prbs_test = lcd_clk_prbs_test_txhd2,
};

void lcd_clk_config_chip_init_tl1(struct aml_lcd_drv_s *pdrv, struct lcd_clk_config_s *cconf)
{
	cconf->data = &lcd_clk_data_tl1;
	cconf->pll_od_fb = lcd_clk_data_tl1.pll_od_fb;
}

void lcd_clk_config_chip_init_tm2(struct aml_lcd_drv_s *pdrv, struct lcd_clk_config_s *cconf)
{
	cconf->data = &lcd_clk_data_tm2;
	cconf->pll_od_fb = lcd_clk_data_tm2.pll_od_fb;
}

void lcd_clk_config_chip_init_t5(struct aml_lcd_drv_s *pdrv, struct lcd_clk_config_s *cconf)
{
	cconf->data = &lcd_clk_data_t5;
	cconf->pll_od_fb = lcd_clk_data_t5.pll_od_fb;
}

void lcd_clk_config_chip_init_t5d(struct aml_lcd_drv_s *pdrv, struct lcd_clk_config_s *cconf)
{
	cconf->data = &lcd_clk_data_t5d;
	cconf->pll_od_fb = lcd_clk_data_t5d.pll_od_fb;
}

void lcd_clk_config_chip_init_txhd2(struct aml_lcd_drv_s *pdrv, struct lcd_clk_config_s *cconf)
{
	cconf->data = &lcd_clk_data_txhd2;
	cconf->pll_od_fb = lcd_clk_data_txhd2.pll_od_fb;
}
