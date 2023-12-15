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

static char *lcd_clk_div_sel_table[] = {
	"1",
	"2",
	"3",
	"3.5",
	"3.75",
	"4",
	"5",
	"6",
	"6.25",
	"7",
	"7.5",
	"12",
	"14",
	"15",
	"2.5",
	"4.67",
	"invalid",
};

static const unsigned int od_table[6] = {1, 2, 4, 8, 16, 32};

static void lcd_pll_frac_set_gp0(struct aml_lcd_drv_s *pdrv, unsigned int frac)
{
	struct lcd_clk_config_s *cconf;
	unsigned int val;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	val = lcd_ana_read(HHI_GP0_PLL_CNTL1);
	lcd_ana_setb(HHI_GP0_PLL_CNTL1, frac, 0, 19);
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("%s: reg 0x%x: 0x%08x->0x%08x\n",
			__func__, HHI_GP0_PLL_CNTL1,
			val, lcd_ana_read(HHI_GP0_PLL_CNTL1));
	}
}

static void lcd_pll_frac_set_hpll(struct aml_lcd_drv_s *pdrv, unsigned int frac)
{
	struct lcd_clk_config_s *cconf;
	unsigned int val;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	val = lcd_ana_read(HHI_HDMI_PLL_CNTL1);
	lcd_ana_setb(HHI_HDMI_PLL_CNTL1, frac, 0, 19);
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("%s: reg 0x%x: 0x%08x->0x%08x\n",
			__func__, HHI_HDMI_PLL_CNTL1,
			val, lcd_ana_read(HHI_HDMI_PLL_CNTL1));
	}
}

#define PLL_WAIT_LOCK_CNT_G12A    1000
static int lcd_pll_wait_lock_g12a(int path)
{
	unsigned int pll_ctrl, pll_ctrl3, pll_ctrl6;
	unsigned int pll_lock;
	int wait_loop = PLL_WAIT_LOCK_CNT_G12A; /* 200 */
	int ret = 0;

	if (path) {
		pll_ctrl = HHI_GP0_PLL_CNTL0;
		pll_ctrl3 = HHI_GP0_PLL_CNTL3;
		pll_ctrl6 = HHI_GP0_PLL_CNTL6;
	} else {
		pll_ctrl = HHI_HDMI_PLL_CNTL0;
		pll_ctrl3 = HHI_HDMI_PLL_CNTL3;
		pll_ctrl6 = HHI_HDMI_PLL_CNTL6;
	}
	do {
		udelay(50);
		pll_lock = lcd_ana_getb(pll_ctrl, 31, 1);
		wait_loop--;
	} while ((pll_lock != 1) && (wait_loop > 0));

	if (pll_lock == 1) {
		goto pll_lock_end_g12a;
	} else {
		LCDPR("path: %d, pll try 1, lock: %d\n", path, pll_lock);
		lcd_ana_setb(pll_ctrl3, 1, 31, 1);
		wait_loop = PLL_WAIT_LOCK_CNT_G12A;
		do {
			udelay(50);
			pll_lock = lcd_ana_getb(pll_ctrl, 31, 1);
			wait_loop--;
		} while ((pll_lock != 1) && (wait_loop > 0));
	}

	if (pll_lock == 1) {
		goto pll_lock_end_g12a;
	} else {
		LCDPR("path: %d, pll try 2, lock: %d\n", path, pll_lock);
		lcd_ana_write(pll_ctrl6, 0x55540000);
		wait_loop = PLL_WAIT_LOCK_CNT_G12A;
		do {
			udelay(50);
			pll_lock = lcd_ana_getb(pll_ctrl, 31, 1);
			wait_loop--;
		} while ((pll_lock != 1) && (wait_loop > 0));
	}

	if (pll_lock != 1)
		ret = -1;

pll_lock_end_g12a:
	LCDPR("%s: path=%d, pll_lock=%d, wait_loop=%d\n",
		__func__, path, pll_lock, (PLL_WAIT_LOCK_CNT_G12A - wait_loop));
	return ret;
}

static void lcd_set_gp0_pll_g12a(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl, pll_ctrl1, pll_ctrl3, pll_ctrl4, pll_ctrl6;
	int ret, cnt = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("%s\n", __func__);

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	pll_ctrl = ((1 << LCD_PLL_EN_GP0_G12A) |
		(cconf->pll_n << LCD_PLL_N_GP0_G12A) |
		(cconf->pll_m << LCD_PLL_M_GP0_G12A) |
		(cconf->pll_od1_sel << LCD_PLL_OD_GP0_G12A));
	pll_ctrl1 = (cconf->pll_frac << 0);
	if (cconf->pll_frac) {
		pll_ctrl |= (1 << 27);
		pll_ctrl3 = 0x6a285c00;
		pll_ctrl4 = 0x65771290;
		pll_ctrl6 = 0x56540000;
	} else {
		pll_ctrl3 = 0x48681c00;
		pll_ctrl4 = 0x33771290;
		pll_ctrl6 = 0x56540000;
	}

set_gp0_pll_retry_g12a:
	lcd_ana_write(HHI_GP0_PLL_CNTL0, pll_ctrl);
	lcd_ana_write(HHI_GP0_PLL_CNTL1, pll_ctrl1);
	lcd_ana_write(HHI_GP0_PLL_CNTL2, 0x00);
	lcd_ana_write(HHI_GP0_PLL_CNTL3, pll_ctrl3);
	lcd_ana_write(HHI_GP0_PLL_CNTL4, pll_ctrl4);
	lcd_ana_write(HHI_GP0_PLL_CNTL5, 0x39272000);
	lcd_ana_write(HHI_GP0_PLL_CNTL6, pll_ctrl6);
	lcd_ana_setb(HHI_GP0_PLL_CNTL0, 1, LCD_PLL_RST_GP0_G12A, 1);
	udelay(100);
	lcd_ana_setb(HHI_GP0_PLL_CNTL0, 0, LCD_PLL_RST_GP0_G12A, 1);

	ret = lcd_pll_wait_lock_g12a(1);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_gp0_pll_retry_g12a;
		LCDERR("gp0_pll lock failed\n");
	}
}

static void lcd_set_hpll_g12a(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl, pll_ctrl1, pll_ctrl3, pll_ctrl4, pll_ctrl6;
	int ret, cnt = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("%s\n", __func__);

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	pll_ctrl = ((1 << LCD_PLL_EN_HPLL_G12A) |
		(1 << 25) | /* clk out gate */
		(cconf->pll_n << LCD_PLL_N_HPLL_G12A) |
		(cconf->pll_m << LCD_PLL_M_HPLL_G12A) |
		(cconf->pll_od1_sel << LCD_PLL_OD1_HPLL_G12A) |
		(cconf->pll_od2_sel << LCD_PLL_OD2_HPLL_G12A) |
		(cconf->pll_od3_sel << LCD_PLL_OD3_HPLL_G12A));
	pll_ctrl1 = (cconf->pll_frac << 0);
	if (cconf->pll_frac) {
		pll_ctrl |= (1 << 27);
		pll_ctrl3 = 0x6a285c00;
		pll_ctrl4 = 0x65771290;
		pll_ctrl6 = 0x56540000;
	} else {
		pll_ctrl3 = 0x48681c00;
		pll_ctrl4 = 0x33771290;
		pll_ctrl6 = 0x56540000;
	}

set_hpll_pll_retry_g12a:
	lcd_ana_write(HHI_HDMI_PLL_CNTL0, pll_ctrl);
	lcd_ana_write(HHI_HDMI_PLL_CNTL1, pll_ctrl1);
	lcd_ana_write(HHI_HDMI_PLL_CNTL2, 0x00);
	lcd_ana_write(HHI_HDMI_PLL_CNTL3, pll_ctrl3);
	lcd_ana_write(HHI_HDMI_PLL_CNTL4, pll_ctrl4);
	lcd_ana_write(HHI_HDMI_PLL_CNTL5, 0x39272000);
	lcd_ana_write(HHI_HDMI_PLL_CNTL6, pll_ctrl6);
	lcd_ana_setb(HHI_HDMI_PLL_CNTL0, 1, LCD_PLL_RST_HPLL_G12A, 1);
	udelay(100);
	lcd_ana_setb(HHI_HDMI_PLL_CNTL0, 0, LCD_PLL_RST_HPLL_G12A, 1);

	ret = lcd_pll_wait_lock_g12a(0);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_hpll_pll_retry_g12a;
		LCDERR("hpll lock failed\n");
	}
}

static void lcd_set_gp0_pll_g12b(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl, pll_ctrl1, pll_ctrl3, pll_ctrl4, pll_ctrl6;
	int ret, cnt = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("%s\n", __func__);

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	pll_ctrl = ((1 << LCD_PLL_EN_GP0_G12A) |
		(cconf->pll_n << LCD_PLL_N_GP0_G12A) |
		(cconf->pll_m << LCD_PLL_M_GP0_G12A) |
		(cconf->pll_od1_sel << LCD_PLL_OD_GP0_G12A));
	pll_ctrl1 = (cconf->pll_frac << 0);
	if (cconf->pll_frac) {
		pll_ctrl |= (1 << 27);
		pll_ctrl3 = 0x6a285c00;
		pll_ctrl4 = 0x65771290;
		pll_ctrl6 = 0x56540000;
	} else {
		pll_ctrl3 = 0x48681c00;
		pll_ctrl4 = 0x33771290;
		pll_ctrl6 = 0x56540000;
	}

set_gp0_pll_retry_g12b:
	lcd_ana_write(HHI_GP0_PLL_CNTL0, pll_ctrl);
	lcd_ana_write(HHI_GP0_PLL_CNTL1, pll_ctrl1);
	lcd_ana_write(HHI_GP0_PLL_CNTL2, 0x00);
	lcd_ana_write(HHI_GP0_PLL_CNTL3, pll_ctrl3);
	lcd_ana_write(HHI_GP0_PLL_CNTL4, pll_ctrl4);
	lcd_ana_write(HHI_GP0_PLL_CNTL5, 0x39272000);
	lcd_ana_write(HHI_GP0_PLL_CNTL6, pll_ctrl6);
	lcd_ana_setb(HHI_GP0_PLL_CNTL0, 1, LCD_PLL_RST_GP0_G12A, 1);
	udelay(100);
	lcd_ana_setb(HHI_GP0_PLL_CNTL0, 0, LCD_PLL_RST_GP0_G12A, 1);

	ret = lcd_pll_wait_lock(HHI_GP0_PLL_CNTL0, LCD_PLL_LOCK_GP0_G12A);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_gp0_pll_retry_g12b;
		LCDERR("gp0_pll lock failed\n");
	}
}

static void lcd_set_hpll_g12b(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int pll_ctrl, pll_ctrl1, pll_ctrl3, pll_ctrl4, pll_ctrl6;
	int ret, cnt = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("%s\n", __func__);

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	pll_ctrl = ((1 << LCD_PLL_EN_HPLL_G12A) |
		(1 << 25) | /* clk out gate */
		(cconf->pll_n << LCD_PLL_N_HPLL_G12A) |
		(cconf->pll_m << LCD_PLL_M_HPLL_G12A) |
		(cconf->pll_od1_sel << LCD_PLL_OD1_HPLL_G12A) |
		(cconf->pll_od2_sel << LCD_PLL_OD2_HPLL_G12A) |
		(cconf->pll_od3_sel << LCD_PLL_OD3_HPLL_G12A));
	pll_ctrl1 = (cconf->pll_frac << 0);
	if (cconf->pll_frac) {
		pll_ctrl |= (1 << 27);
		pll_ctrl3 = 0x6a285c00;
		pll_ctrl4 = 0x65771290;
		pll_ctrl6 = 0x56540000;
	} else {
		pll_ctrl3 = 0x48681c00;
		pll_ctrl4 = 0x33771290;
		pll_ctrl6 = 0x56540000;
	}

set_hpll_pll_retry_g12b:
	lcd_ana_write(HHI_HDMI_PLL_CNTL0, pll_ctrl);
	lcd_ana_write(HHI_HDMI_PLL_CNTL1, pll_ctrl1);
	lcd_ana_write(HHI_HDMI_PLL_CNTL2, 0x00);
	lcd_ana_write(HHI_HDMI_PLL_CNTL3, pll_ctrl3);
	lcd_ana_write(HHI_HDMI_PLL_CNTL4, pll_ctrl4);
	lcd_ana_write(HHI_HDMI_PLL_CNTL5, 0x39272000);
	lcd_ana_write(HHI_HDMI_PLL_CNTL6, pll_ctrl6);
	lcd_ana_setb(HHI_HDMI_PLL_CNTL0, 1, LCD_PLL_RST_HPLL_G12A, 1);
	udelay(100);
	lcd_ana_setb(HHI_HDMI_PLL_CNTL0, 0, LCD_PLL_RST_HPLL_G12A, 1);

	ret = lcd_pll_wait_lock(HHI_HDMI_PLL_CNTL0, LCD_PLL_LOCK_HPLL_G12A);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_hpll_pll_retry_g12b;
		LCDERR("hpll lock failed\n");
	}
}

static void lcd_set_dsi_meas_clk(void)
{
	lcd_clk_setb(HHI_VDIN_MEAS_CLK_CNTL, 0, 21, 3);
	lcd_clk_setb(HHI_VDIN_MEAS_CLK_CNTL, 0, 12, 7);
	lcd_clk_setb(HHI_VDIN_MEAS_CLK_CNTL, 1, 20, 1);
}

static void lcd_set_dsi_phy_clk(int sel)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
		LCDPR("%s\n", __func__);

	lcd_ana_setb(HHI_MIPIDSI_PHY_CLK_CNTL, sel, 12, 3);
	lcd_ana_setb(HHI_MIPIDSI_PHY_CLK_CNTL, 1, 8, 1);
	lcd_ana_setb(HHI_MIPIDSI_PHY_CLK_CNTL, 0, 0, 7);
}

static void lcd_clk_set_g12a_path0(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	lcd_set_hpll_g12a(pdrv);
	lcd_set_vid_pll_div_dft(cconf);
	lcd_set_dsi_meas_clk();
	lcd_set_dsi_phy_clk(0);
}

static void lcd_clk_set_g12a_path1(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	lcd_set_gp0_pll_g12a(pdrv);
	lcd_set_dsi_meas_clk();
	lcd_set_dsi_phy_clk(1);
}

static void lcd_clk_set_g12b_path0(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	lcd_set_hpll_g12b(pdrv);
	lcd_set_vid_pll_div_dft(cconf);
	lcd_set_dsi_meas_clk();
	lcd_set_dsi_phy_clk(0);
}

static void lcd_clk_set_g12b_path1(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	lcd_set_gp0_pll_g12b(pdrv);
	lcd_set_dsi_meas_clk();
	lcd_set_dsi_phy_clk(1);
}

static void lcd_clk_disable_gp0(struct aml_lcd_drv_s *pdrv)
{
	lcd_clk_setb(HHI_VID_CLK_CNTL2, 0, 3, 1);

	/* close vclk2_div gate: 0x104b[4:0] */
	lcd_clk_setb(HHI_VIID_CLK_CNTL, 0, 0, 5);
	lcd_clk_setb(HHI_VIID_CLK_CNTL, 0, 19, 1);

	lcd_ana_setb(HHI_GP0_PLL_CNTL0, 0, 28, 1);  //disable
	lcd_ana_setb(HHI_GP0_PLL_CNTL0, 1, 29, 1);  //reset
}

static void lcd_clk_disable_hpll(struct aml_lcd_drv_s *pdrv)
{
	lcd_clk_setb(HHI_VID_CLK_CNTL2, 0, 3, 1);

	/* close vclk2_div gate: 0x104b[4:0] */
	lcd_clk_setb(HHI_VIID_CLK_CNTL, 0, 0, 5);
	lcd_clk_setb(HHI_VIID_CLK_CNTL, 0, 19, 1);

	lcd_ana_setb(HHI_HDMI_PLL_CNTL0, 0, 28, 1);  //disable
	lcd_ana_setb(HHI_HDMI_PLL_CNTL0, 1, 29, 1);  //reset
}

static void lcd_pll_frac_generate_g12a(struct aml_lcd_drv_s *pdrv)
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

static void lcd_clk_generate_g12a(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned long long pll_fout, bit_rate_max = 0, bit_rate_min = 0, tmp;
	unsigned int xd;
	int done = 0;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	cconf->fout = pconf->timing.lcd_clk;
	cconf->err_fmin = MAX_ERROR;

	if (cconf->fout > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, cconf->fout);
		goto generate_clk_done_g12a;
	}

	switch (pconf->basic.lcd_type) {
	case LCD_MIPI:
		cconf->xd_max = CRT_VID_DIV_MAX;
		bit_rate_max = pconf->control.mipi_cfg.local_bit_rate_max;
		bit_rate_min = pconf->control.mipi_cfg.local_bit_rate_min;
		tmp = bit_rate_max - cconf->fout;
		if (tmp >= bit_rate_min)
			bit_rate_min = tmp;

		for (xd = 1; xd <= cconf->xd_max; xd++) {
			pll_fout = cconf->fout;
			pll_fout *= xd;
			if (pll_fout > bit_rate_max || pll_fout < bit_rate_min)
				continue;
			if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
				LCDPR("fout=%d, xd=%d\n", cconf->fout, xd);

			cconf->phy_clk = pll_fout;
			pconf->timing.bit_rate = cconf->phy_clk;
			pconf->control.mipi_cfg.clk_factor = xd;
			cconf->xd = xd;
			done = check_pll_1od(cconf, pll_fout);
			if (done)
				goto generate_clk_done_g12a;
		}
		break;
	default:
		break;
	}

generate_clk_done_g12a:
	if (done) {
		pconf->timing.pll_ctrl =
			(cconf->pll_od1_sel << PLL_CTRL_OD1) |
			(cconf->pll_n << PLL_CTRL_N) |
			(cconf->pll_m << PLL_CTRL_M);
		pconf->timing.div_ctrl =
			(CLK_DIV_SEL_1 << DIV_CTRL_DIV_SEL) |
			(cconf->xd << DIV_CTRL_XD);
		pconf->timing.clk_ctrl = (cconf->pll_frac << CLK_CTRL_FRAC);
		cconf->done = 1;
	} else {
		pconf->timing.pll_ctrl =
			(1 << PLL_CTRL_OD1) |
			(1 << PLL_CTRL_N)   |
			(50 << PLL_CTRL_M);
		pconf->timing.div_ctrl =
			(CLK_DIV_SEL_1 << DIV_CTRL_DIV_SEL) |
			(7 << DIV_CTRL_XD);
		pconf->timing.clk_ctrl = (0 << CLK_CTRL_FRAC);
		cconf->done = 0;
		LCDERR("Out of clock range\n");
	}
}

static void lcd_clk_generate_hpll_g12a(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned long long pll_fout, bit_rate_max = 0, bit_rate_min = 0, tmp;
	unsigned int clk_div_sel, xd;
	int done = 0;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	cconf->fout = pconf->timing.lcd_clk;
	cconf->err_fmin = MAX_ERROR;

	if (cconf->fout > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, cconf->fout);
		goto generate_clk_done_hpll_g12a;
	}

	switch (pconf->basic.lcd_type) {
	case LCD_MIPI:
		cconf->xd_max = CRT_VID_DIV_MAX;
		bit_rate_max = pconf->control.mipi_cfg.local_bit_rate_max;
		bit_rate_min = pconf->control.mipi_cfg.local_bit_rate_min;
		tmp = bit_rate_max - cconf->fout;
		if (tmp >= bit_rate_min)
			bit_rate_min = tmp;

		clk_div_sel = CLK_DIV_SEL_1;
		for (xd = 1; xd <= cconf->xd_max; xd++) {
			pll_fout = cconf->fout;
			pll_fout *= xd;
			if (pll_fout > bit_rate_max || pll_fout < bit_rate_min)
				continue;
			if (lcd_debug_print_flag & LCD_DBG_PR_ADV2)
				LCDPR("fout=%d, xd=%d\n", cconf->fout, xd);

			cconf->phy_clk = pll_fout;
			pconf->timing.bit_rate = cconf->phy_clk;
			pconf->control.mipi_cfg.clk_factor = xd;
			cconf->xd = xd;
			cconf->div_sel = clk_div_sel;
			cconf->pll_div_fout = pll_fout;
			done = check_pll_3od(cconf, pll_fout);
			if (done)
				goto generate_clk_done_hpll_g12a;
		}
		break;
	default:
		break;
	}

generate_clk_done_hpll_g12a:
	if (done) {
		pconf->timing.pll_ctrl =
			(cconf->pll_od1_sel << PLL_CTRL_OD1) |
			(cconf->pll_od2_sel << PLL_CTRL_OD2) |
			(cconf->pll_od3_sel << PLL_CTRL_OD3) |
			(cconf->pll_n << PLL_CTRL_N)         |
			(cconf->pll_m << PLL_CTRL_M);
		pconf->timing.div_ctrl =
			(cconf->div_sel << DIV_CTRL_DIV_SEL) |
			(cconf->xd << DIV_CTRL_XD);
		pconf->timing.clk_ctrl = (cconf->pll_frac << CLK_CTRL_FRAC);
		cconf->done = 1;
	} else {
		pconf->timing.pll_ctrl =
			(1 << PLL_CTRL_OD1) |
			(1 << PLL_CTRL_OD2) |
			(1 << PLL_CTRL_OD3) |
			(1 << PLL_CTRL_N)   |
			(50 << PLL_CTRL_M);
		pconf->timing.div_ctrl =
			(CLK_DIV_SEL_1 << DIV_CTRL_DIV_SEL) |
			(7 << DIV_CTRL_XD);
		pconf->timing.clk_ctrl = (0 << CLK_CTRL_FRAC);
		cconf->done = 0;
		LCDERR("Out of clock range\n");
	}
}

static void lcd_clk_config_init_print_g12a(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	struct lcd_clk_data_s *data;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	data = cconf->data;
	LCDPR("lcd clk config data init:\n"
		"vclk_sel:          %d\n"
		"pll_m_max:         %d\n"
		"pll_m_min:         %d\n"
		"pll_n_max:         %d\n"
		"pll_n_min:         %d\n"
		"pll_od_fb:         %d\n"
		"pll_frac_range:    %d\n"
		"pll_od_sel_max:    %d\n"
		"pll_ref_fmax:      %d\n"
		"pll_ref_fmin:      %d\n"
		"pll_vco_fmax:      %lld\n"
		"pll_vco_fmin:      %lld\n"
		"pll_out_fmax:      %lld\n"
		"pll_out_fmin:      %lld\n"
		"xd_out_fmax:       %d\n\n",
		data->vclk_sel,
		data->pll_m_max, data->pll_m_min,
		data->pll_n_max, data->pll_n_min,
		data->pll_od_fb, data->pll_frac_range,
		data->pll_od_sel_max,
		data->pll_ref_fmax, data->pll_ref_fmin,
		data->pll_vco_fmax, data->pll_vco_fmin,
		data->pll_out_fmax, data->pll_out_fmin,
		data->xd_out_fmax);
}

static void lcd_clk_config_print_g12a(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return;

	if (cconf->data->vclk_sel) {
		LCDPR("lcd clk config:\n"
			"vclk_sel      %d\n"
			"pll_m:        %d\n"
			"pll_n:        %d\n"
			"pll_frac:     0x%x\n"
			"pll_fvco:     %lldHz\n"
			"pll_od:       %d\n"
			"pll_out:      %lldHz\n"
			"xd:           %d\n"
			"fout:         %dHz\n\n",
			cconf->data->vclk_sel,
			cconf->pll_m, cconf->pll_n,
			cconf->pll_frac, cconf->pll_fvco,
			cconf->pll_od1_sel, cconf->pll_fout,
			cconf->xd, cconf->fout);
	} else {
		LCDPR("lcd clk config:\n"
			"vclk_sel        %d\n"
			"pll_m:          %d\n"
			"pll_n:          %d\n"
			"pll_frac:       0x%x\n"
			"pll_fvco:       %lldHz\n"
			"pll_od1:        %d\n"
			"pll_od2:        %d\n"
			"pll_od3:        %d\n"
			"pll_out:        %lldHz\n"
			"div_sel:        %s(index %d)\n"
			"xd:             %d\n"
			"fout:           %dHz\n\n",
			cconf->data->vclk_sel,
			cconf->pll_m, cconf->pll_n,
			cconf->pll_frac, cconf->pll_fvco,
			cconf->pll_od1_sel, cconf->pll_od2_sel,
			cconf->pll_od3_sel, cconf->pll_fout,
			lcd_clk_div_sel_table[cconf->div_sel],
			cconf->div_sel, cconf->xd,
			cconf->fout);
	}
}

static struct lcd_clk_data_s lcd_clk_data_g12a_path0 = { //HPLL
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
	.pll_out_fmax = 750000000,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 0,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 200000000,
	.od_cnt = 3,
	.have_tcon_div = 0,
	.have_pll_div = 1,
	.phy_clk_location = 2,

	.vclk_sel = 0,
	.enc_clk_msr_id = 9,

	.ss_support = 0,

	.clk_parameter_init = NULL,
	.clk_generate_parameter = lcd_clk_generate_hpll_g12a,
	.pll_frac_generate = lcd_pll_frac_generate_dft,
	.set_ss_level = NULL,
	.set_ss_advance = NULL,
	.clk_ss_enable = NULL,
	.pll_frac_set = lcd_pll_frac_set_hpll,
	.clk_set = lcd_clk_set_g12a_path0,
	.vclk_crt_set = lcd_set_vclk_crt_dft,
	.clk_disable = lcd_clk_disable_hpll,
	.clktree_set = NULL,
	.clk_config_init_print = lcd_clk_config_init_print_g12a,
	.clk_config_print = lcd_clk_config_print_g12a,
	.prbs_clk_config = NULL,
	.prbs_test = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_g12a_path1 = {  //GP0
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
	.pll_out_fmax = 750000000,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 0,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 200000000,
	.od_cnt = 1,
	.have_tcon_div = 0,
	.have_pll_div = 0,
	.phy_clk_location = 1,

	.vclk_sel = 1,
	.enc_clk_msr_id = 9,

	.ss_support = 0,

	.clk_parameter_init = NULL,
	.clk_generate_parameter = lcd_clk_generate_g12a,
	.pll_frac_generate = lcd_pll_frac_generate_g12a,
	.set_ss_level = NULL,
	.set_ss_advance = NULL,
	.clk_ss_enable = NULL,
	.pll_frac_set = lcd_pll_frac_set_gp0,
	.clk_set = lcd_clk_set_g12a_path1,
	.vclk_crt_set = lcd_set_vclk_crt_dft,
	.clk_disable = lcd_clk_disable_gp0,
	.clktree_set = NULL,
	.clk_config_init_print = lcd_clk_config_init_print_g12a,
	.clk_config_print = lcd_clk_config_print_g12a,
	.prbs_clk_config = NULL,
	.prbs_test = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_g12b_path0 = {  //HPLL
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
	.pll_out_fmax = 750000000,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 0,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 200000000,
	.od_cnt = 3,
	.have_tcon_div = 0,
	.have_pll_div = 1,
	.phy_clk_location = 2,

	.vclk_sel = 0,
	.enc_clk_msr_id = 9,

	.ss_support = 0,

	.clk_parameter_init = NULL,
	.clk_generate_parameter = lcd_clk_generate_hpll_g12a,
	.pll_frac_generate = lcd_pll_frac_generate_dft,
	.set_ss_level = NULL,
	.set_ss_advance = NULL,
	.clk_ss_enable = NULL,
	.pll_frac_set = lcd_pll_frac_set_hpll,
	.clk_set = lcd_clk_set_g12b_path0,
	.vclk_crt_set = lcd_set_vclk_crt_dft,
	.clk_disable = lcd_clk_disable_hpll,
	.clktree_set = NULL,
	.clk_config_init_print = lcd_clk_config_init_print_g12a,
	.clk_config_print = lcd_clk_config_print_g12a,
	.prbs_clk_config = NULL,
	.prbs_test = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_g12b_path1 = { //GP0
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
	.pll_out_fmax = 750000000,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 0,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 200000000,
	.od_cnt = 1,
	.have_tcon_div = 0,
	.have_pll_div = 0,
	.phy_clk_location = 1,

	.vclk_sel = 1,
	.enc_clk_msr_id = 9,

	.ss_support = 0,

	.clk_parameter_init = NULL,
	.clk_generate_parameter = lcd_clk_generate_g12a,
	.pll_frac_generate = lcd_pll_frac_generate_g12a,
	.set_ss_level = NULL,
	.set_ss_advance = NULL,
	.clk_ss_enable = NULL,
	.pll_frac_set = lcd_pll_frac_set_gp0,
	.clk_set = lcd_clk_set_g12b_path1,
	.vclk_crt_set = lcd_set_vclk_crt_dft,
	.clk_disable = lcd_clk_disable_gp0,
	.clktree_set = NULL,
	.clk_config_init_print = lcd_clk_config_init_print_g12a,
	.clk_config_print = lcd_clk_config_print_g12a,
	.prbs_clk_config = NULL,
	.prbs_test = NULL,
};

void lcd_clk_config_chip_init_g12a(struct aml_lcd_drv_s *pdrv, struct lcd_clk_config_s *cconf)
{
	if (pdrv->clk_path) {
		cconf->data = &lcd_clk_data_g12a_path1;
		cconf->pll_od_fb = lcd_clk_data_g12a_path1.pll_od_fb;
	} else {
		cconf->data = &lcd_clk_data_g12a_path0;
		cconf->pll_od_fb = lcd_clk_data_g12a_path0.pll_od_fb;
	}
}

void lcd_clk_config_chip_init_g12b(struct aml_lcd_drv_s *pdrv, struct lcd_clk_config_s *cconf)
{
	if (pdrv->clk_path) {
		cconf->data = &lcd_clk_data_g12b_path1;
		cconf->pll_od_fb = lcd_clk_data_g12b_path1.pll_od_fb;
	} else {
		cconf->data = &lcd_clk_data_g12b_path0;
		cconf->pll_od_fb = lcd_clk_data_g12b_path0.pll_od_fb;
	}
}
