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

static struct lcd_clk_ctrl_s pll_ctrl_table_t7[] = {
	/* flag             reg                      bit              len*/
	{LCD_CLK_CTRL_EN,   ANACTRL_TCON_PLL0_CNTL0, LCD_PLL_EN_TL1,   1},
	{LCD_CLK_CTRL_RST,  ANACTRL_TCON_PLL0_CNTL0, LCD_PLL_RST_TL1,  1},
	{LCD_CLK_CTRL_FRAC, ANACTRL_TCON_PLL0_CNTL1,               0, 17},
	{LCD_CLK_CTRL_END,  LCD_CLK_REG_END,                       0,  0},
};

static unsigned int tcon_div[5][3] = {
	/* div_mux, div2/4_sel, div4_bypass */
	{1, 0, 1},  /* div1 */
	{0, 0, 1},  /* div2 */
	{0, 1, 1},  /* div4 */
	{0, 0, 0},  /* div8 */
	{0, 1, 0},  /* div16 */
};

static unsigned int lcd_clk_div_table[][3] = {
	/* divider,        shift_val,  shift_sel */
	{CLK_DIV_SEL_1,    0xffff,     0,},
	{CLK_DIV_SEL_2,    0x0aaa,     0,},
	{CLK_DIV_SEL_3,    0x0db6,     0,},
	{CLK_DIV_SEL_3p5,  0x36cc,     1,},
	{CLK_DIV_SEL_3p75, 0x6666,     2,},
	{CLK_DIV_SEL_4,    0x0ccc,     0,},
	{CLK_DIV_SEL_5,    0x739c,     2,},
	{CLK_DIV_SEL_6,    0x0e38,     0,},
	{CLK_DIV_SEL_6p25, 0x0000,     3,},
	{CLK_DIV_SEL_7,    0x3c78,     1,},
	{CLK_DIV_SEL_7p5,  0x78f0,     2,},
	{CLK_DIV_SEL_12,   0x0fc0,     0,},
	{CLK_DIV_SEL_14,   0x3f80,     1,},
	{CLK_DIV_SEL_15,   0x7f80,     2,},
	{CLK_DIV_SEL_2p5,  0x5294,     2,},
	{CLK_DIV_SEL_4p67, 0x0ccc,     1,},
	{CLK_DIV_SEL_MAX,  0xffff,     0,},
};

static void lcd_pll_ss_enable(struct aml_lcd_drv_s *pdrv, int status)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl2, offset;
	unsigned int level, flag;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	level = cconf->ss_level;
	offset = cconf->pll_offset;
	pll_ctrl2 = lcd_ana_read(ANACTRL_TCON_PLL0_CNTL2 + offset);
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
		LCDPR("[%d]: pll ss enable: %dppm\n", pdrv->index, (level * 1000));
	} else {
		cconf->ss_en = 0;
		pll_ctrl2 &= ~(1 << 15);
		LCDPR("[%d]: pll ss disable\n", pdrv->index);
	}
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, pll_ctrl2);
}

static void lcd_set_pll_ss_level(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl2, offset;
	unsigned int level, dep_sel, str_m;
	unsigned int data[2] = {0, 0};
	int ret;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	level = cconf->ss_level;
	offset = cconf->pll_offset;
	pll_ctrl2 = lcd_ana_read(ANACTRL_TCON_PLL0_CNTL2 + offset);
	pll_ctrl2 &= ~((1 << 15) | (0xf << 16) | (0xf << 28));

	if (level > 0) {
		cconf->ss_en = 1;
		ret = lcd_pll_ss_level_generate(data, level, 500);
		if (ret == 0) {
			dep_sel = data[0];
			str_m = data[1];
			dep_sel = (dep_sel > 10) ? 10 : dep_sel;
			str_m = (str_m > 10) ? 10 : str_m;
			pll_ctrl2 |= ((1 << 15) | (dep_sel << 28) | (str_m << 16));
		}
	} else {
		cconf->ss_en = 0;
	}

	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, pll_ctrl2);

	if (level > 0) {
		LCDPR("[%d]: set pll spread spectrum: %dppm\n",
		      pdrv->index, (level * 1000));
	} else {
		LCDPR("[%d]: set pll spread spectrum: disable\n", pdrv->index);
	}
}

static void lcd_set_pll_ss_advance(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl2, offset;
	unsigned int freq, mode;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	freq = cconf->ss_freq;
	mode = cconf->ss_mode;
	offset = cconf->pll_offset;
	pll_ctrl2 = lcd_ana_read(ANACTRL_TCON_PLL0_CNTL2 + offset);
	pll_ctrl2 &= ~(0x7 << 24); /* ss_freq */
	pll_ctrl2 |= (freq << 24);
	pll_ctrl2 &= ~(0x3 << 22); /* ss_mode */
	pll_ctrl2 |= (mode << 22);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, pll_ctrl2);

	LCDPR("[%d]: set pll spread spectrum: freq=%d, mode=%d\n",
	      pdrv->index, freq, mode);
}

static void lcd_set_pll_t7(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl, pll_ctrl1, pll_stts, offset;
	unsigned int tcon_div_sel;
	int ret, cnt = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	tcon_div_sel = cconf->pll_tcon_div_sel;
	pll_ctrl = ((0x3 << 17) | /* gate ctrl */
		(tcon_div[tcon_div_sel][2] << 16) |
		(cconf->pll_n << LCD_PLL_N_TL1) |
		(cconf->pll_m << LCD_PLL_M_TL1) |
		(cconf->pll_od3_sel << LCD_PLL_OD3_T7) |
		(cconf->pll_od2_sel << LCD_PLL_OD2_T7) |
		(cconf->pll_od1_sel << LCD_PLL_OD1_T7));
	pll_ctrl1 = (1 << 28) |
		(tcon_div[tcon_div_sel][0] << 22) |
		(tcon_div[tcon_div_sel][1] << 21) |
		((1 << 20) | /* sdm_en */
		(cconf->pll_frac << 0));

	offset = cconf->pll_offset;
	switch (cconf->pll_id) {
	case 1:
		pll_stts = ANACTRL_TCON_PLL1_STS;
		break;
	case 2:
		pll_stts = ANACTRL_TCON_PLL2_STS;
		break;
	case 0:
	default:
		pll_stts = ANACTRL_TCON_PLL0_STS;
		break;
	}

set_pll_retry_t7:
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0 + offset, pll_ctrl);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 1, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 1, LCD_PLL_EN_TL1, 1);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL1 + offset, pll_ctrl1);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, 0x0000110c);
	udelay(10);
	if (cconf->pll_fvco < 3800000000ULL)
		lcd_ana_write(ANACTRL_TCON_PLL0_CNTL3 + offset, 0x10051100);
	else
		lcd_ana_write(ANACTRL_TCON_PLL0_CNTL3 + offset, 0x10051400);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL4 + offset, 0x0100c0, 0, 24);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL4 + offset, 0x8300c0, 0, 24);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 1, 26, 1);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 0, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, 0x0000300c);

	ret = lcd_pll_wait_lock(pll_stts, LCD_PLL_LOCK_T7);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_t7;
		LCDERR("[%d]: pll lock failed\n", pdrv->index);
	} else {
		udelay(100);
		lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL2 + offset, 1, 5, 1);
	}

	if (cconf->ss_level > 0) {
		lcd_set_pll_ss_level(pdrv);
		lcd_set_pll_ss_advance(pdrv);
	}
}

static void lcd_set_pll_t3(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl, pll_ctrl1, pll_stts, offset;
	unsigned int tcon_div_sel;
	int ret, cnt = 0;

	if (pdrv->index) /* clk_path1 invalid */
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	tcon_div_sel = cconf->pll_tcon_div_sel;
	pll_ctrl = ((0x3 << 17) | /* gate ctrl */
		(tcon_div[tcon_div_sel][2] << 16) |
		(cconf->pll_n << LCD_PLL_N_TL1) |
		(cconf->pll_m << LCD_PLL_M_TL1) |
		(cconf->pll_od3_sel << LCD_PLL_OD3_T7) |
		(cconf->pll_od2_sel << LCD_PLL_OD2_T7) |
		(cconf->pll_od1_sel << LCD_PLL_OD1_T7));
	pll_ctrl1 = (1 << 28) |
		(tcon_div[tcon_div_sel][0] << 22) |
		(tcon_div[tcon_div_sel][1] << 21) |
		((1 << 20) | /* sdm_en */
		(cconf->pll_frac << 0));

	offset = cconf->pll_offset;
	pll_stts = ANACTRL_TCON_PLL0_STS;

set_pll_retry_t3:
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0 + offset, pll_ctrl);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 1, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 1, LCD_PLL_EN_TL1, 1);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL1 + offset, pll_ctrl1);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, 0x0000110c);
	udelay(10);
	if (cconf->pll_fvco < 3800000000ULL)
		lcd_ana_write(ANACTRL_TCON_PLL0_CNTL3 + offset, 0x10051100);
	else
		lcd_ana_write(ANACTRL_TCON_PLL0_CNTL3 + offset, 0x10051400);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL4 + offset, 0x0100c0, 0, 24);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL4 + offset, 0x8300c0, 0, 24);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 1, 26, 1);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 0, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, 0x0000300c);

	ret = lcd_pll_wait_lock(pll_stts, LCD_PLL_LOCK_T7);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_t3;
		LCDERR("[%d]: pll lock failed\n", pdrv->index);
	} else {
		udelay(100);
		lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL2 + offset, 1, 5, 1);
	}

	if (cconf->ss_level > 0) {
		lcd_set_pll_ss_level(pdrv);
		lcd_set_pll_ss_advance(pdrv);
	}
}

static void lcd_set_dsi_meas_clk(int index)
{
	if (index) {
		lcd_clk_setb(CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL, 7, 12, 7);
		lcd_clk_setb(CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL, 0, 21, 3);
		lcd_clk_setb(CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL, 1, 20, 1);
	} else {
		lcd_clk_setb(CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL, 7, 0, 7);
		lcd_clk_setb(CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL, 0, 9, 3);
		lcd_clk_setb(CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL, 1, 8, 1);
	}
}

static void lcd_set_dsi_phy_clk(int index)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", index, __func__);
	if (index) {
		lcd_clk_setb(CLKCTRL_MIPIDSI_PHY_CLK_CTRL, 0, 16, 7);
		lcd_clk_setb(CLKCTRL_MIPIDSI_PHY_CLK_CTRL, 0, 25, 3);
		lcd_clk_setb(CLKCTRL_MIPIDSI_PHY_CLK_CTRL, 1, 24, 1);
	} else {
		lcd_clk_setb(CLKCTRL_MIPIDSI_PHY_CLK_CTRL, 0, 0, 7);
		lcd_clk_setb(CLKCTRL_MIPIDSI_PHY_CLK_CTRL, 0, 12, 3);
		lcd_clk_setb(CLKCTRL_MIPIDSI_PHY_CLK_CTRL, 1, 8, 1);
	}
}

static void lcd_set_phy_dig_div(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int reg_edp_clk_div, reg_dphy_tx_ctrl1;
	unsigned int port_sel, bit_div_en, bit_div0, bit_div1, bit_rst;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	reg_edp_clk_div = COMBO_DPHY_EDP_PIXEL_CLK_DIV;
	switch (cconf->pll_id) {
	case 1:
		reg_dphy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL1;
		port_sel = 1;
		bit_div_en = 25;
		bit_div0 = 8;
		bit_div1 = 12;
		bit_rst = 20;
		break;
	case 2:
		reg_dphy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1;
		port_sel = 2;
		bit_div_en = 26;
		bit_div0 = 0;
		bit_div1 = 4;
		bit_rst = 7;
		break;
	case 0:
	default:
		reg_dphy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1;
		port_sel = 0;
		bit_div_en = 24;
		bit_div0 = 0;
		bit_div1 = 4;
		bit_rst = 19;
		break;
	}

	lcd_reset_setb(RESETCTRL_RESET1_MASK, 0, bit_rst, 1);
	lcd_reset_setb(RESETCTRL_RESET1_LEVEL, 0, bit_rst, 1);
	udelay(1);
	lcd_reset_setb(RESETCTRL_RESET1_LEVEL, 1, bit_rst, 1);
	udelay(10);

	// Enable dphy clock
	lcd_combo_dphy_setb(reg_dphy_tx_ctrl1, 1, 0, 1);

	switch (pdrv->config.basic.lcd_type) {
	case LCD_EDP:
		if (port_sel == 2) {
			LCDERR("[%d]: %s: invalid port: %d\n",
			       pdrv->index, __func__, port_sel);
			return;
		}
		// Disable edp_div clock
		lcd_combo_dphy_setb(reg_edp_clk_div, 0, bit_div_en, 1);
		lcd_combo_dphy_setb(reg_edp_clk_div, cconf->edp_div0, bit_div0, 4);
		lcd_combo_dphy_setb(reg_edp_clk_div, cconf->edp_div1, bit_div1, 4);
		// Enable edp_div clock
		lcd_combo_dphy_setb(reg_edp_clk_div, 1, bit_div_en, 1);
		// sel edp_div clock
		lcd_combo_dphy_setb(reg_dphy_tx_ctrl1, 1, 4, 1);
		break;
	case LCD_MIPI:
	case LCD_VBYONE:
		if (port_sel == 2) {
			LCDERR("[%d]: %s: invalid port: %d\n",
			       pdrv->index, __func__, port_sel);
			return;
		}
		// sel pll clock
		lcd_combo_dphy_setb(reg_dphy_tx_ctrl1, 0, 4, 1);
		break;
	default:
		// sel pll clock
		lcd_combo_dphy_setb(reg_dphy_tx_ctrl1, 0, 4, 1);
		break;
	}

	// sel tcon_pll clock
	lcd_combo_dphy_setb(reg_dphy_tx_ctrl1, 0, 5, 1);
}

static void lcd_set_vid_pll_div_t7(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int reg_vid_pll_div, reg_vid2_clk_ctrl;
	unsigned int shift_val, shift_sel;
	int i;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	switch (cconf->pll_id) {
	case 1:
		reg_vid_pll_div = COMBO_DPHY_VID_PLL1_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK1_CTRL;
		break;
	case 2:
		reg_vid_pll_div = COMBO_DPHY_VID_PLL2_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK2_CTRL;
		break;
	case 0:
	default:
		reg_vid_pll_div = COMBO_DPHY_VID_PLL0_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;
		break;
	}

	lcd_clk_setb(reg_vid2_clk_ctrl, 0, VCLK2_EN, 1);
	udelay(5);

	/* Disable the div output clock */
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 19, 1);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 15, 1);

	i = 0;
	while (lcd_clk_div_table[i][0] != CLK_DIV_SEL_MAX) {
		if (cconf->div_sel == lcd_clk_div_table[i][0])
			break;
		i++;
	}
	if (lcd_clk_div_table[i][0] == CLK_DIV_SEL_MAX)
		LCDERR("[%d]: invalid clk divider\n", pdrv->index);
	shift_val = lcd_clk_div_table[i][1];
	shift_sel = lcd_clk_div_table[i][2];

	if (shift_val == 0xffff) { /* if divide by 1 */
		lcd_combo_dphy_setb(reg_vid_pll_div, 1, 18, 1);
	} else {
		lcd_combo_dphy_setb(reg_vid_pll_div, 0, 18, 1);
		lcd_combo_dphy_setb(reg_vid_pll_div, 0, 16, 2);
		lcd_combo_dphy_setb(reg_vid_pll_div, 0, 15, 1);
		lcd_combo_dphy_setb(reg_vid_pll_div, 0, 0, 14);

		lcd_combo_dphy_setb(reg_vid_pll_div, shift_sel, 16, 2);
		lcd_combo_dphy_setb(reg_vid_pll_div, 1, 15, 1);
		lcd_combo_dphy_setb(reg_vid_pll_div, shift_val, 0, 15);
		lcd_combo_dphy_setb(reg_vid_pll_div, 0, 15, 1);
	}
	/* Enable the final output clock */
	lcd_combo_dphy_setb(reg_vid_pll_div, 1, 19, 1);
}

static void lcd_clk_set_t7(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

#ifdef CONFIG_AML_LCD_PXP
	return;
#endif

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	lcd_set_pll_t7(pdrv);
	lcd_set_phy_dig_div(pdrv);
	lcd_set_vid_pll_div_t7(pdrv);

	if (pdrv->config.basic.lcd_type == LCD_MIPI) {
		lcd_set_dsi_meas_clk(pdrv->index);
		lcd_set_dsi_phy_clk(pdrv->index);
	}
}

/* tcon run base clk, include register access */
static void lcd_set_tcon_clk_t3(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int val;

	if (pconf->basic.lcd_type != LCD_MLVDS &&
	    pconf->basic.lcd_type != LCD_P2P)
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	switch (pconf->basic.lcd_type) {
	case LCD_MLVDS:
		val = pconf->control.mlvds_cfg.clk_phase & 0xfff;
		lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL1, (val & 0xf), 24, 4);
		lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL4, ((val >> 4) & 0xf), 28, 4);
		lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL4, ((val >> 8) & 0xf), 24, 4);

		/* tcon_clk */
		if (pconf->timing.lcd_clk >= 100000000) /* 25M */
			lcd_clk_write(CLKCTRL_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (0xf << 0));
		else /* 12.5M */
			lcd_clk_write(CLKCTRL_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (0x1f << 0));
		break;
	case LCD_P2P:
		/* tcon_clk 50M */
		lcd_clk_write(CLKCTRL_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (7 << 0));
		break;
	default:
		break;
	}

	/* global reset tcon */
	lcd_reset_setb(RESETCTRL_RESET2_MASK, 0, 5, 1);
	lcd_reset_setb(RESETCTRL_RESET2_LEVEL, 0, 5, 1);
	udelay(1);
	lcd_reset_setb(RESETCTRL_RESET2_LEVEL, 1, 5, 1);
	udelay(2);
	LCDPR("reset tcon\n");
}

static void lcd_set_vid_pll_div_t3(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int shift_val, shift_sel;
	int i;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	if (cconf->pll_id) {
		/* only crt_video valid for clk path1 */
		lcd_clk_setb(CLKCTRL_VIID_CLK1_CTRL, 0, VCLK2_EN, 1);
		udelay(5);
		return;
	}

	lcd_clk_setb(CLKCTRL_VIID_CLK0_CTRL, 0, VCLK2_EN, 1);
	udelay(5);

	/* Disable the div output clock */
	lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, 0, 19, 1);
	lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, 0, 15, 1);

	i = 0;
	while (lcd_clk_div_table[i][0] != CLK_DIV_SEL_MAX) {
		if (cconf->div_sel == lcd_clk_div_table[i][0])
			break;
		i++;
	}
	if (lcd_clk_div_table[i][0] == CLK_DIV_SEL_MAX)
		LCDERR("[%d]: invalid clk divider\n", pdrv->index);
	shift_val = lcd_clk_div_table[i][1];
	shift_sel = lcd_clk_div_table[i][2];

	if (shift_val == 0xffff) { /* if divide by 1 */
		lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, 1, 18, 1);
	} else {
		lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, 0, 18, 1);
		lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, 0, 20, 1);/*div8_25*/
		lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, 0, 16, 2);
		lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, 0, 15, 1);
		lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, 0, 0, 15);

		lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, shift_sel, 16, 2);
		lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, 1, 15, 1);
		lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, shift_val, 0, 15);
		lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, 0, 15, 1);

	}
	/* Enable the final output clock */
	lcd_ana_setb(ANACTRL_VID_PLL_CLK_DIV, 1, 19, 1);
}

static void lcd_clk_set_t3(struct aml_lcd_drv_s *pdrv)
{
	if (pdrv->index == 0) /* tcon_clk invalid for lcd1 */
		lcd_set_tcon_clk_t3(pdrv);
	lcd_set_pll_t3(pdrv);
	lcd_set_vid_pll_div_t3(pdrv);
}

static void lcd_set_vclk_crt(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int reg_vid2_clk_div, reg_vid2_clk_ctrl, reg_vid_clk_ctrl2;
	unsigned int venc_clk_sel_bit = 0xff;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	switch (cconf->pll_id) {
	case 1:
		reg_vid2_clk_div = CLKCTRL_VIID_CLK1_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK1_CTRL;
		reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK1_CTRL2;
		break;
	case 2:
		reg_vid2_clk_div = CLKCTRL_VIID_CLK2_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK2_CTRL;
		reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK2_CTRL2;
		venc_clk_sel_bit = 25;
		break;
	case 0:
	default:
		reg_vid2_clk_div = CLKCTRL_VIID_CLK0_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;
		reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK0_CTRL2;
		venc_clk_sel_bit = 24;
		break;
	}

	lcd_clk_write(reg_vid_clk_ctrl2, 0);
	lcd_clk_write(reg_vid2_clk_ctrl, 0);
	lcd_clk_write(reg_vid2_clk_div, 0);
	udelay(5);

	if (venc_clk_sel_bit < 0xff)
		lcd_clk_setb(CLKCTRL_HDMI_VID_PLL_CLK_DIV, 0, venc_clk_sel_bit, 1);

#ifdef CONFIG_AML_LCD_PXP
	/* setup the XD divider value */
	lcd_clk_setb(reg_vid2_clk_div, cconf->xd, VCLK2_XD, 8);
	udelay(5);
	/* select vid_pll_clk */
	lcd_clk_setb(reg_vid2_clk_ctrl, 7, VCLK2_CLK_IN_SEL, 3);
#else
	/* setup the XD divider value */
	lcd_clk_setb(reg_vid2_clk_div, (cconf->xd - 1), VCLK2_XD, 8);
	udelay(5);
	/* select vid_pll_clk */
	lcd_clk_setb(reg_vid2_clk_ctrl, cconf->data->vclk_sel,
		     VCLK2_CLK_IN_SEL, 3);
#endif
	lcd_clk_setb(reg_vid2_clk_ctrl, 1, VCLK2_EN, 1);
	udelay(2);

	/* [15:12] encl_clk_sel, select vclk2_div1 */
	lcd_clk_setb(reg_vid2_clk_div, 8, ENCL_CLK_SEL, 4);
	/* release vclk2_div_reset and enable vclk2_div */
	lcd_clk_setb(reg_vid2_clk_div, 1, VCLK2_XD_EN, 2);
	udelay(5);

	lcd_clk_setb(reg_vid2_clk_ctrl, 1, VCLK2_DIV1_EN, 1);
	lcd_clk_setb(reg_vid2_clk_ctrl, 1, VCLK2_SOFT_RST, 1);
	udelay(10);
	lcd_clk_setb(reg_vid2_clk_ctrl, 0, VCLK2_SOFT_RST, 1);
	udelay(5);

	/* enable CTS_ENCL clk gate */
	lcd_clk_setb(reg_vid_clk_ctrl2, 1, ENCL_GATE_VCLK, 1);
}

static void lcd_clk_disable(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	struct lcd_clk_ctrl_s *table;
	unsigned int reg_vid_clk_ctrl2, reg_vid2_clk_ctrl, offset;
	int i = 0;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	switch (cconf->pll_id) {
	case 1:
		reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK1_CTRL2;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK1_CTRL;
		break;
	case 2:
		reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK2_CTRL2;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK2_CTRL;
		break;
	case 0:
	default:
		reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK0_CTRL2;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;
		break;
	}
	offset = cconf->pll_offset;

	lcd_clk_setb(reg_vid_clk_ctrl2, 0, ENCL_GATE_VCLK, 1);

	/* close vclk2_div gate: [4:0] */
	lcd_clk_setb(reg_vid2_clk_ctrl, 0, 0, 5);
	lcd_clk_setb(reg_vid2_clk_ctrl, 0, VCLK2_EN, 1);

	if (!cconf->data->pll_ctrl_table)
		return;
	table = cconf->data->pll_ctrl_table;
	while (i < LCD_CLK_CTRL_CNT_MAX) {
		if (table[i].flag == LCD_CLK_CTRL_END)
			break;
		if (table[i].flag == LCD_CLK_CTRL_EN)
			lcd_ana_setb(table[i].reg + offset, 0, table[i].bit, table[i].len);
		else if (table[i].flag == LCD_CLK_CTRL_RST)
			lcd_ana_setb(table[i].reg + offset, 1, table[i].bit, table[i].len);
		i++;
	}
}

static void lcd_prbs_set_pll_vx1_t7(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_stts, offset;
	unsigned int reg_vid_pll_div, reg_vid2_clk_ctrl;
	int cnt = 0, ret;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	offset = cconf->pll_offset;
	switch (cconf->pll_id) {
	case 1:
		pll_stts = ANACTRL_TCON_PLL1_STS;
		reg_vid_pll_div = COMBO_DPHY_VID_PLL1_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK1_CTRL;
		break;
	case 2:
		pll_stts = ANACTRL_TCON_PLL2_STS;
		reg_vid_pll_div = COMBO_DPHY_VID_PLL2_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK2_CTRL;
		break;
	case 0:
	default:
		pll_stts = ANACTRL_TCON_PLL0_STS;
		reg_vid_pll_div = COMBO_DPHY_VID_PLL0_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;
		break;
	}

lcd_prbs_retry_pll_vx1_t7:
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0 + offset, 0x000f04f7);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 1, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 1, LCD_PLL_EN_TL1, 1);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL1 + offset, 0x10110000);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, 0x00001108);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL3 + offset, 0x10051400);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL4 + offset, 0x010100c0);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL4 + offset, 0x038300c0);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 1, 26, 1);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0 + offset, 0, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, 0x00003008);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, 0x00003028);
	udelay(10);

	ret = lcd_pll_wait_lock(pll_stts, LCD_PLL_LOCK_T7);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto lcd_prbs_retry_pll_vx1_t7;
		LCDERR("[%d]: pll lock failed\n", pdrv->index);
	}

	/* pll_div */
	lcd_clk_setb(reg_vid2_clk_ctrl, 0, VCLK2_EN, 1);
	udelay(5);

	/* Disable the div output clock */
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 19, 1);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 15, 1);

	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 18, 1);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 16, 2);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 15, 1);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 0, 14);

	lcd_combo_dphy_setb(reg_vid_pll_div, 2, 16, 2);
	lcd_combo_dphy_setb(reg_vid_pll_div, 1, 15, 1);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0x739c, 0, 15);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 15, 1);

	/* Enable the final output clock */
	lcd_combo_dphy_setb(reg_vid_pll_div, 1, 19, 1);
}

static void lcd_prbs_set_pll_lvds_t7(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_stts, offset;
	unsigned int reg_vid_pll_div, reg_vid2_clk_ctrl;
	int cnt = 0, ret;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	offset = cconf->pll_offset;
	switch (cconf->pll_id) {
	case 1:
		pll_stts = ANACTRL_TCON_PLL1_STS;
		reg_vid_pll_div = COMBO_DPHY_VID_PLL1_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK1_CTRL;
		break;
	case 2:
		pll_stts = ANACTRL_TCON_PLL2_STS;
		reg_vid_pll_div = COMBO_DPHY_VID_PLL2_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK2_CTRL;
		break;
	case 0:
	default:
		pll_stts = ANACTRL_TCON_PLL0_STS;
		reg_vid_pll_div = COMBO_DPHY_VID_PLL0_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;
		break;
	}

lcd_prbs_retry_pll_lvds_t7:
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0 + offset, 0x008e049f);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0 + offset, 0x208e049f);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0 + offset, 0x3006049f);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL1 + offset, 0x10000000);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, 0x00001102);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL3 + offset, 0x10051400);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL4 + offset, 0x010100c0);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL4 + offset, 0x038300c0);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0 + offset, 0x348e049f);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0 + offset, 0x148e049f);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, 0x00003002);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2 + offset, 0x00003022);
	udelay(10);

	ret = lcd_pll_wait_lock(pll_stts, LCD_PLL_LOCK_T7);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto lcd_prbs_retry_pll_lvds_t7;
		LCDERR("[%d]: pll lock failed\n", pdrv->index);
	}

	/* pll_div */
	lcd_clk_setb(reg_vid2_clk_ctrl, 0, VCLK2_EN, 1);
	udelay(5);

	/* Disable the div output clock */
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 19, 1);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 15, 1);

	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 18, 1);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 16, 2);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 15, 1);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 0, 14);

	lcd_combo_dphy_setb(reg_vid_pll_div, 1, 16, 2);
	lcd_combo_dphy_setb(reg_vid_pll_div, 1, 15, 1);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0x3c78, 0, 15);
	lcd_combo_dphy_setb(reg_vid_pll_div, 0, 15, 1);

	/* Enable the final output clock */
	lcd_combo_dphy_setb(reg_vid_pll_div, 1, 19, 1);
}

static void lcd_prbs_config_clk_t7(struct aml_lcd_drv_s *pdrv, unsigned int lcd_prbs_mode)
{
	struct lcd_clk_config_s *cconf;
	unsigned int reg_vid2_clk_div, reg_vid2_clk_ctrl, reg_vid_clk_ctrl2;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	switch (cconf->pll_id) {
	case 1:
		reg_vid2_clk_div = CLKCTRL_VIID_CLK1_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK1_CTRL;
		reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK1_CTRL2;
		break;
	case 2:
		reg_vid2_clk_div = CLKCTRL_VIID_CLK2_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK2_CTRL;
		reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK2_CTRL2;
		break;
	case 0:
	default:
		reg_vid2_clk_div = CLKCTRL_VIID_CLK0_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;
		reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK0_CTRL2;
		break;
	}

	if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
		lcd_prbs_set_pll_vx1_t7(pdrv);
	} else if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
		lcd_prbs_set_pll_lvds_t7(pdrv);
	} else {
		LCDERR("[%d]: %s: unsupport lcd_prbs_mode %d\n",
		       pdrv->index, __func__, lcd_prbs_mode);
		return;
	}

	lcd_clk_setb(reg_vid2_clk_div, 0, VCLK2_XD, 8);
	udelay(5);

	/* select vid_pll_clk */
	lcd_clk_setb(reg_vid2_clk_ctrl, 0, VCLK2_CLK_IN_SEL, 3);
	lcd_clk_setb(reg_vid2_clk_ctrl, 1, VCLK2_EN, 1);
	udelay(5);

	/* [15:12] encl_clk_sel, select vclk2_div1 */
	lcd_clk_setb(reg_vid2_clk_div, 8, ENCL_CLK_SEL, 4);
	/* release vclk2_div_reset and enable vclk2_div */
	lcd_clk_setb(reg_vid2_clk_div, 1, VCLK2_XD_EN, 2);
	udelay(5);

	lcd_clk_setb(reg_vid2_clk_ctrl, 1, VCLK2_DIV1_EN, 1);
	lcd_clk_setb(reg_vid2_clk_ctrl, 1, VCLK2_SOFT_RST, 1);
	udelay(10);
	lcd_clk_setb(reg_vid2_clk_ctrl, 0, VCLK2_SOFT_RST, 1);
	udelay(5);

	/* enable CTS_ENCL clk gate */
	lcd_clk_setb(reg_vid_clk_ctrl2, 1, ENCL_GATE_VCLK, 1);

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s ok\n", pdrv->index, __func__);
}

static void lcd_prbs_set_pll_vx1_t3(struct aml_lcd_drv_s *pdrv)
{
	unsigned int pll_stts;
	unsigned int reg_vid_pll_div, reg_vid2_clk_ctrl;
	int cnt = 0, ret;

	pll_stts = ANACTRL_TCON_PLL0_STS;
	reg_vid_pll_div = ANACTRL_VID_PLL_CLK_DIV;
	reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;

lcd_prbs_retry_pll_vx1_t3:
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0, 0x000f04f7);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0, 1, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0, 1, LCD_PLL_EN_TL1, 1);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL1, 0x10110000);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2, 0x00001108);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL3, 0x10051400);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL4, 0x010100c0);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL4, 0x038300c0);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0, 1, 26, 1);
	udelay(10);
	lcd_ana_setb(ANACTRL_TCON_PLL0_CNTL0, 0, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2, 0x00003008);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2, 0x00003028);
	udelay(10);

	ret = lcd_pll_wait_lock(pll_stts, LCD_PLL_LOCK_T7);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto lcd_prbs_retry_pll_vx1_t3;
		LCDERR("pll lock failed\n");
	}

	/* pll_div */
	lcd_clk_setb(reg_vid2_clk_ctrl, 0, VCLK2_EN, 1);
	udelay(5);

	/* Disable the div output clock */
	lcd_ana_setb(reg_vid_pll_div, 0, 19, 1);
	lcd_ana_setb(reg_vid_pll_div, 0, 15, 1);

	lcd_ana_setb(reg_vid_pll_div, 0, 18, 1);
	lcd_ana_setb(reg_vid_pll_div, 0, 16, 2);
	lcd_ana_setb(reg_vid_pll_div, 0, 15, 1);
	lcd_ana_setb(reg_vid_pll_div, 0, 0, 14);

	lcd_ana_setb(reg_vid_pll_div, 2, 16, 2);
	lcd_ana_setb(reg_vid_pll_div, 1, 15, 1);
	lcd_ana_setb(reg_vid_pll_div, 0x739c, 0, 15);
	lcd_ana_setb(reg_vid_pll_div, 0, 15, 1);

	/* Enable the final output clock */
	lcd_ana_setb(reg_vid_pll_div, 1, 19, 1);
}

static void lcd_prbs_set_pll_lvds_t3(struct aml_lcd_drv_s *pdrv)
{
	unsigned int pll_stts;
	unsigned int reg_vid_pll_div, reg_vid2_clk_ctrl;
	int cnt = 0, ret;

	pll_stts = ANACTRL_TCON_PLL0_STS;
	reg_vid_pll_div = ANACTRL_VID_PLL_CLK_DIV;
	reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;

lcd_prbs_retry_pll_lvds_t3:
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0, 0x008e049f);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0, 0x208e049f);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0, 0x3006049f);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL1, 0x10000000);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2, 0x00001102);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL3, 0x10051400);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL4, 0x010100c0);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL4, 0x038300c0);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0, 0x348e049f);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL0, 0x148e049f);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2, 0x00003002);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL0_CNTL2, 0x00003022);
	udelay(10);

	ret = lcd_pll_wait_lock(pll_stts, LCD_PLL_LOCK_T7);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto lcd_prbs_retry_pll_lvds_t3;
		LCDERR("pll lock failed\n");
	}

	/* pll_div */
	lcd_clk_setb(reg_vid2_clk_ctrl, 0, VCLK2_EN, 1);
	udelay(5);

	/* Disable the div output clock */
	lcd_ana_setb(reg_vid_pll_div, 0, 19, 1);
	lcd_ana_setb(reg_vid_pll_div, 0, 15, 1);

	lcd_ana_setb(reg_vid_pll_div, 0, 18, 1);
	lcd_ana_setb(reg_vid_pll_div, 0, 16, 2);
	lcd_ana_setb(reg_vid_pll_div, 0, 15, 1);
	lcd_ana_setb(reg_vid_pll_div, 0, 0, 14);

	lcd_ana_setb(reg_vid_pll_div, 1, 16, 2);
	lcd_ana_setb(reg_vid_pll_div, 1, 15, 1);
	lcd_ana_setb(reg_vid_pll_div, 0x3c78, 0, 15);
	lcd_ana_setb(reg_vid_pll_div, 0, 15, 1);

	/* Enable the final output clock */
	lcd_ana_setb(reg_vid_pll_div, 1, 19, 1);
}

static void lcd_prbs_config_clk_t3(struct aml_lcd_drv_s *pdrv, unsigned int lcd_prbs_mode)
{
	struct lcd_clk_config_s *cconf;
	unsigned int reg_vid2_clk_div, reg_vid2_clk_ctrl, reg_vid_clk_ctrl2;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	switch (cconf->pll_id) {
	case 1:
		reg_vid2_clk_div = CLKCTRL_VIID_CLK1_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK1_CTRL;
		reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK1_CTRL2;
		break;
	case 0:
	default:
		reg_vid2_clk_div = CLKCTRL_VIID_CLK0_DIV;
		reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;
		reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK0_CTRL2;
		break;
	}

	if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
		lcd_prbs_set_pll_vx1_t3(pdrv);
	} else if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
		lcd_prbs_set_pll_lvds_t3(pdrv);
	} else {
		LCDERR("%s: unsupport lcd_prbs_mode %d\n",
		       __func__, lcd_prbs_mode);
		return;
	}

	lcd_clk_setb(reg_vid2_clk_div, 0, VCLK2_XD, 8);
	udelay(5);

	/* select vid_pll_clk */
	lcd_clk_setb(reg_vid2_clk_ctrl, 0, VCLK2_CLK_IN_SEL, 3);
	lcd_clk_setb(reg_vid2_clk_ctrl, 1, VCLK2_EN, 1);
	udelay(5);

	/* [15:12] encl_clk_sel, select vclk2_div1 */
	lcd_clk_setb(reg_vid2_clk_div, 8, ENCL_CLK_SEL, 4);
	/* release vclk2_div_reset and enable vclk2_div */
	lcd_clk_setb(reg_vid2_clk_div, 1, VCLK2_XD_EN, 2);
	udelay(5);

	lcd_clk_setb(reg_vid2_clk_ctrl, 1, VCLK2_DIV1_EN, 1);
	lcd_clk_setb(reg_vid2_clk_ctrl, 1, VCLK2_SOFT_RST, 1);
	udelay(10);
	lcd_clk_setb(reg_vid2_clk_ctrl, 0, VCLK2_SOFT_RST, 1);
	udelay(5);

	/* enable CTS_ENCL clk gate */
	lcd_clk_setb(reg_vid_clk_ctrl2, 1, ENCL_GATE_VCLK, 1);

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s ok\n", pdrv->index, __func__);
}

static int lcd_prbs_test_t7(struct aml_lcd_drv_s *pdrv, unsigned int ms,
			    unsigned int mode_flag)
{
	struct lcd_clk_config_s *cconf;
	unsigned int reg_phy_tx_ctrl0, reg_phy_tx_ctrl1, reg_ctrl_out, bit_width;
	int encl_msr_id, fifo_msr_id;
	unsigned int lcd_prbs_mode;
	unsigned int val1, val2, timeout;
	unsigned int cnt = 0;
	unsigned int clk_err_cnt = 0;
	int i, j, ret;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return -1;

	switch (pdrv->index) {
	case 0:
		reg_phy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0;
		reg_phy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1;
		reg_ctrl_out = COMBO_DPHY_RO_EDP_LVDS_TX_PHY0_CNTL1;
		bit_width = 8;
		break;
	case 1:
		reg_phy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL0;
		reg_phy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL1;
		reg_ctrl_out = COMBO_DPHY_RO_EDP_LVDS_TX_PHY1_CNTL1;
		bit_width = 8;
		break;
	case 2:
		reg_phy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0;
		reg_phy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1;
		reg_ctrl_out = COMBO_DPHY_RO_EDP_LVDS_TX_PHY2_CNTL1;
		bit_width = 10;
		break;
	default:
		LCDERR("[%d]: %s: invalid drv_index\n",
			pdrv->index, __func__);
		return -1;
	}
	encl_msr_id = cconf->data->enc_clk_msr_id;
	fifo_msr_id = -1;

	timeout = (ms > 1000) ? 1000 : ms;

	for (i = 0; i < LCD_PRBS_MODE_MAX; i++) {
		if ((mode_flag & (1 << i)) == 0)
			continue;

		lcd_combo_dphy_write(reg_phy_tx_ctrl0, 0);
		lcd_combo_dphy_write(reg_phy_tx_ctrl1, 0);

		cnt = 0;
		clk_err_cnt = 0;
		lcd_prbs_mode = (1 << i);
		LCDPR("[%d]: lcd_prbs_mode: %d\n", pdrv->index, lcd_prbs_mode);
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_encl_clk_check_std = 136;
			lcd_fifo_clk_check_std = 48;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_encl_clk_check_std = 594;
			lcd_fifo_clk_check_std = 297;
		}
		if (!cconf->data->prbs_clk_config) {
			LCDERR("[%d]: %s: prbs_clk_config is null\n",
			       pdrv->index, __func__);
			goto lcd_prbs_test_t7_end;
		}
		cconf->data->prbs_clk_config(pdrv, lcd_prbs_mode);
		udelay(500);

		/* set fifo_clk_sel: div 10 */
		lcd_combo_dphy_write(reg_phy_tx_ctrl0, (3 << 5));
		/* set cntl_ser_en:  10-channel */
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 0x3ff, 16, 10);
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 1, 2, 1);
		/* decoupling fifo enable, gated clock enable */
		lcd_combo_dphy_write(reg_phy_tx_ctrl1, (1 << 6) | (1 << 0));
		/* decoupling fifo write enable after fifo enable */
		lcd_combo_dphy_setb(reg_phy_tx_ctrl1, 1, 7, 1);
		/* prbs_err en */
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 1, 13, 1);
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 1, 12, 1);

		while (cnt++ < timeout) {
			val1 = lcd_combo_dphy_getb(reg_ctrl_out, bit_width, bit_width);
			udelay(1000);
			ret = 1;
			for (j = 0; j < 20; j++) {
				udelay(5);
				val2 = lcd_combo_dphy_getb(reg_ctrl_out, bit_width, bit_width);
				if (val2 != val1) {
					ret = 0;
					break;
				}
			}
			if (ret) {
				LCDERR("[%d]: prbs check error 1, val:0x%03x, cnt:%d\n",
				       pdrv->index, val2, cnt);
				goto lcd_prbs_test_t7_err;
			}
			if (lcd_combo_dphy_getb(reg_ctrl_out, 0, bit_width)) {
				LCDERR("[%d]: prbs check error 2, cnt:%d\n",
				       pdrv->index, cnt);
				goto lcd_prbs_test_t7_err;
			}
			if (lcd_prbs_clk_check(lcd_encl_clk_check_std, encl_msr_id,
					       lcd_fifo_clk_check_std, fifo_msr_id,
					       cnt))
				clk_err_cnt++;
			else
				clk_err_cnt = 0;
			if (clk_err_cnt >= 10) {
				LCDERR("[%d]: prbs check error 3(clkmsr), cnt:%d\n",
				       pdrv->index, cnt);
				goto lcd_prbs_test_t7_err;
			}
		}

		lcd_combo_dphy_write(reg_phy_tx_ctrl0, 0);
		lcd_combo_dphy_write(reg_phy_tx_ctrl1, 0);

		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_LVDS);
			LCDPR("[%d]: lvds prbs check ok\n", pdrv->index);
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_VX1);
			LCDPR("[%d]: vx1 prbs check ok\n", pdrv->index);
		} else {
			LCDPR("[%d]: prbs check: unsupport mode\n", pdrv->index);
		}
		continue;

lcd_prbs_test_t7_err:
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err |= LCD_PRBS_MODE_LVDS;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err |= LCD_PRBS_MODE_VX1;
		}
	}

lcd_prbs_test_t7_end:
	lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 0, 12, 2);

	printf("\n[[%d]: lcd prbs result]:\n", pdrv->index);
	printf("  lvds prbs performed: %d, error: %d\n"
	       "  vx1 prbs performed: %d, error: %d\n",
	       (lcd_prbs_performed & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_performed & LCD_PRBS_MODE_VX1) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_VX1) ? 1 : 0);

	return 0;
}

static int lcd_prbs_test_t3(struct aml_lcd_drv_s *pdrv, unsigned int ms,
			    unsigned int mode_flag)
{
	struct lcd_clk_config_s *cconf;
	unsigned int reg_phy_tx_ctrl0, reg_phy_tx_ctrl1;
	int encl_msr_id, fifo_msr_id;
	unsigned int lcd_prbs_mode;
	unsigned int val1, val2, timeout;
	unsigned int cnt = 0;
	unsigned int clk_err_cnt = 0;
	int i, j, ret;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return -1;

	switch (pdrv->index) {
	case 0:
		reg_phy_tx_ctrl0 = ANACTRL_LVDS_TX_PHY_CNTL0;
		reg_phy_tx_ctrl1 = ANACTRL_LVDS_TX_PHY_CNTL1;
		break;
	case 1:
		reg_phy_tx_ctrl0 = ANACTRL_LVDS_TX_PHY_CNTL2;
		reg_phy_tx_ctrl1 = ANACTRL_LVDS_TX_PHY_CNTL3;
		break;
	default:
		LCDERR("[%d]: %s: invalid drv_index\n",
			pdrv->index, __func__);
		return -1;
	}
	encl_msr_id = cconf->data->enc_clk_msr_id;
	fifo_msr_id = -1;

	timeout = (ms > 1000) ? 1000 : ms;

	for (i = 0; i < LCD_PRBS_MODE_MAX; i++) {
		if ((mode_flag & (1 << i)) == 0)
			continue;

		lcd_ana_write(reg_phy_tx_ctrl0, 0);
		lcd_ana_write(reg_phy_tx_ctrl1, 0);

		cnt = 0;
		clk_err_cnt = 0;
		lcd_prbs_mode = (1 << i);
		LCDPR("[%d]: lcd_prbs_mode: %d\n", pdrv->index, lcd_prbs_mode);
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_encl_clk_check_std = 136;
			lcd_fifo_clk_check_std = 48;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_encl_clk_check_std = 594;
			lcd_fifo_clk_check_std = 297;
		}
		if (!cconf->data->prbs_clk_config) {
			LCDERR("[%d]: %s: prbs_clk_config is null\n",
			       pdrv->index, __func__);
			goto lcd_prbs_test_t3_end;
		}
		cconf->data->prbs_clk_config(pdrv, lcd_prbs_mode);
		udelay(500);

		/* set fifo_clk_sel: div 10 */
		lcd_ana_write(reg_phy_tx_ctrl0, (3 << 6));
		/* set cntl_ser_en:  8-channel to 1 */
		lcd_ana_setb(reg_phy_tx_ctrl0, 0xfff, 16, 12);
		lcd_ana_setb(reg_phy_tx_ctrl0, 1, 2, 1);
		/* decoupling fifo enable, gated clock enable */
		lcd_ana_write(reg_phy_tx_ctrl1, (1 << 30) | (1 << 24));
		/* decoupling fifo write enable after fifo enable */
		lcd_ana_setb(reg_phy_tx_ctrl1, 1, 31, 1);
		/* prbs_err en */
		lcd_ana_setb(reg_phy_tx_ctrl0, 1, 13, 1);
		lcd_ana_setb(reg_phy_tx_ctrl0, 1, 12, 1);

		while (cnt++ < timeout) {
			val1 = lcd_ana_getb(reg_phy_tx_ctrl1, 12, 12);
			udelay(1000);
			ret = 1;
			for (j = 0; j < 20; j++) {
				udelay(5);
				val2 = lcd_ana_getb(reg_phy_tx_ctrl1, 12, 12);
				if (val2 != val1) {
					ret = 0;
					break;
				}
			}
			if (ret) {
				LCDERR("[%d]: prbs check error 1, val:0x%03x, cnt:%d\n",
				       pdrv->index, val2, cnt);
				goto lcd_prbs_test_t3_err;
			}
			if (lcd_ana_getb(reg_phy_tx_ctrl1, 0, 12)) {
				LCDERR("[%d]: prbs check error 2, cnt:%d\n",
				       pdrv->index, cnt);
				goto lcd_prbs_test_t3_err;
			}
			if (lcd_prbs_clk_check(lcd_encl_clk_check_std, encl_msr_id,
					       lcd_fifo_clk_check_std, fifo_msr_id,
					       cnt))
				clk_err_cnt++;
			else
				clk_err_cnt = 0;
			if (clk_err_cnt >= 10) {
				LCDERR("[%d]: prbs check error 3(clkmsr), cnt:%d\n",
				       pdrv->index, cnt);
				goto lcd_prbs_test_t3_err;
			}
		}

		lcd_ana_write(reg_phy_tx_ctrl0, 0);
		lcd_ana_write(reg_phy_tx_ctrl1, 0);

		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_LVDS);
			LCDPR("[%d]: lvds prbs check ok\n", pdrv->index);
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_VX1);
			LCDPR("[%d]: vx1 prbs check ok\n", pdrv->index);
		} else {
			LCDPR("[%d]: prbs check: unsupport mode\n", pdrv->index);
		}
		continue;

lcd_prbs_test_t3_err:
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err |= LCD_PRBS_MODE_LVDS;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err |= LCD_PRBS_MODE_VX1;
		}
	}

lcd_prbs_test_t3_end:
	lcd_ana_setb(reg_phy_tx_ctrl0, 0, 12, 2);

	printf("\n[[%d]: lcd prbs result]:\n", pdrv->index);
	printf("  lvds prbs performed: %d, error: %d\n"
	       "  vx1 prbs performed: %d, error: %d\n",
	       (lcd_prbs_performed & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_performed & LCD_PRBS_MODE_VX1) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_VX1) ? 1 : 0);

	return 0;
}

static struct lcd_clk_data_s lcd_clk_data_t7 = {
	.pll_od_fb = 0,
	.pll_m_max = 511,
	.pll_m_min = 2,
	.pll_n_max = 1,
	.pll_n_min = 1,
	.pll_frac_range = (1 << 17),
	.pll_frac_sign_bit = 18,
	.pll_od_sel_max = 3,
	.pll_ref_fmax = 25000000,
	.pll_ref_fmin = 5000000,
	.pll_vco_fmax = 6000000000ULL,
	.pll_vco_fmin = 3000000000ULL,
	.pll_out_fmax = 3100000000ULL,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 3100000000ULL,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 750000000,

	.vclk_sel = 0,
	.enc_clk_msr_id = 222,
	.pll_ctrl_table = pll_ctrl_table_t7,

	.ss_support = 2,

	.clk_generate_parameter = lcd_clk_generate_dft,
	.pll_frac_generate = lcd_pll_frac_generate_dft,
	.set_ss_level = lcd_set_pll_ss_level,
	.set_ss_advance = lcd_set_pll_ss_advance,
	.clk_ss_enable = lcd_pll_ss_enable,
	.clk_set = lcd_clk_set_t7,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_disable = lcd_clk_disable,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = lcd_prbs_config_clk_t7,
	.prbs_test = lcd_prbs_test_t7,
};

static struct lcd_clk_data_s lcd_clk_data_t3 = {
	.pll_od_fb = 0,
	.pll_m_max = 511,
	.pll_m_min = 2,
	.pll_n_max = 1,
	.pll_n_min = 1,
	.pll_frac_range = (1 << 17),
	.pll_frac_sign_bit = 18,
	.pll_od_sel_max = 3,
	.pll_ref_fmax = 25000000,
	.pll_ref_fmin = 5000000,
	.pll_vco_fmax = 6000000000ULL,
	.pll_vco_fmin = 3000000000ULL,
	.pll_out_fmax = 3100000000ULL,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 3100000000ULL,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 750000000,

	.vclk_sel = 0,
	.enc_clk_msr_id = 222,
	.pll_ctrl_table = pll_ctrl_table_t7,

	.ss_support = 2,

	.clk_generate_parameter = lcd_clk_generate_dft,
	.pll_frac_generate = lcd_pll_frac_generate_dft,
	.set_ss_level = lcd_set_pll_ss_level,
	.set_ss_advance = lcd_set_pll_ss_advance,
	.clk_ss_enable = lcd_pll_ss_enable,
	.clk_set = lcd_clk_set_t3,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_disable = lcd_clk_disable,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = lcd_prbs_config_clk_t3,
	.prbs_test = lcd_prbs_test_t3,
};

void lcd_clk_config_chip_init_t7(struct aml_lcd_drv_s *pdrv, struct lcd_clk_config_s *cconf)
{
	cconf->data = &lcd_clk_data_t7;
	cconf->pll_od_fb = lcd_clk_data_t7.pll_od_fb;
	switch (pdrv->index) {
	case 2:
		cconf->data->enc_clk_msr_id = 220;
		cconf->pll_id = 2;
		cconf->pll_offset = (0xa << 2);
		break;
	case 1:
		cconf->data->enc_clk_msr_id = 221;
		cconf->pll_id = 1;
		cconf->pll_offset = (0x5 << 2);
		break;
	case 0:
	default:
		cconf->data->enc_clk_msr_id = 222;
		cconf->pll_id = 0;
		cconf->pll_offset = 0;
		break;
	}
	cconf->data->enc_clk_msr_id = -1;
}

void lcd_clk_config_chip_init_t3(struct aml_lcd_drv_s *pdrv, struct lcd_clk_config_s *cconf)
{
	cconf->data = &lcd_clk_data_t3;
	cconf->pll_od_fb = lcd_clk_data_t3.pll_od_fb;
	switch (pdrv->index) {
	case 1:
		cconf->data->enc_clk_msr_id = 221;
		cconf->pll_id = 1;
		break;
	case 0:
	default:
		cconf->data->enc_clk_msr_id = 222;
		cconf->pll_id = 0;
		break;
	}
	cconf->data->enc_clk_msr_id = -1;
}

