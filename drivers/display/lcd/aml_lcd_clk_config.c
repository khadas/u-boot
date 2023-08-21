/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/lcd/aml_lcd_clk_config.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <malloc.h>
#include <amlogic/aml_lcd.h>
#include "aml_lcd_reg.h"
#include "aml_lcd_common.h"
#include "aml_lcd_clk_config.h"
#include "aml_lcd_clk_ctrl.h"

static struct lcd_clk_config_s clk_conf = { /* unit: Hz */
	/* IN-OUT parameters */
	.fin = FIN_FREQ,
	.fout = 0,

	/* pll parameters */
	.pll_mode = 0, /* txl */
	.pll_od_fb = 0,
	.pll_m = 0,
	.pll_n = 0,
	.pll_od1_sel = 0,
	.pll_od2_sel = 0,
	.pll_od3_sel = 0,
	.pll_tcon_div_sel = 0,
	.pll_level = 0,
	.pll_frac_half_shift = 0,
	.ss_level = 0,
	.ss_freq = 0,
	.ss_mode = 0,
	.div_sel = 0,
	.xd = 0,
	.pll_fout = 0,

	/* clk path node parameters */
	.div_sel_max = 0,
	.xd_max = 0,

	.data = NULL,
};

struct lcd_clk_config_s *get_lcd_clk_config(void)
{
	return &clk_conf;
}

/* **********************************
 * lcd controller operation
 * ********************************** */
static unsigned long long error_abs(unsigned long long a, unsigned long long b)
{
	if (a >= b)
		return (a - b);
	else
		return (b - a);
}

#define PLL_CLK_CHECK_MAX    2000000 /* Hz */
static int lcd_clk_msr_check(struct lcd_clk_config_s *cconf)
{
	unsigned int encl_clk_msr;

	if (!cconf)
		return 0;
	if (cconf->data->enc_clk_msr_id == -1)
		return 0;

	encl_clk_msr = clk_util_clk_msr(cconf->data->enc_clk_msr_id) * 1000000;
	if (error_abs(cconf->fout, encl_clk_msr) >= PLL_CLK_CHECK_MAX) {
		LCDERR("%s: expected:%d, msr:%d\n",
			__func__, cconf->fout, encl_clk_msr);
		return -1;
	}

	return 0;
}

static int lcd_pll_wait_lock(unsigned int reg, unsigned int lock_bit)
{
	unsigned int pll_lock;
	int wait_loop = PLL_WAIT_LOCK_CNT; /* 200 */
	int ret = 0;

	do {
		udelay(50);
		pll_lock = lcd_hiu_getb(reg, lock_bit, 1);
		wait_loop--;
	} while ((pll_lock == 0) && (wait_loop > 0));
	if (pll_lock == 0)
		ret = -1;
	LCDPR("%s: pll_lock=%d, wait_loop=%d\n",
		__func__, pll_lock, (PLL_WAIT_LOCK_CNT - wait_loop));

	return ret;
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
		pll_lock = lcd_hiu_getb(pll_ctrl, 31, 1);
		wait_loop--;
	} while ((pll_lock != 1) && (wait_loop > 0));

	if (pll_lock == 1) {
		goto pll_lock_end_g12a;
	} else {
		LCDPR("path: %d, pll try 1, lock: %d\n", path, pll_lock);
		lcd_hiu_setb(pll_ctrl3, 1, 31, 1);
		wait_loop = PLL_WAIT_LOCK_CNT_G12A;
		do {
			udelay(50);
			pll_lock = lcd_hiu_getb(pll_ctrl, 31, 1);
			wait_loop--;
		} while ((pll_lock != 1) && (wait_loop > 0));
	}

	if (pll_lock == 1) {
		goto pll_lock_end_g12a;
	} else {
		LCDPR("path: %d, pll try 2, lock: %d\n", path, pll_lock);
		lcd_hiu_write(pll_ctrl6, 0x55540000);
		wait_loop = PLL_WAIT_LOCK_CNT_G12A;
		do {
			udelay(50);
			pll_lock = lcd_hiu_getb(pll_ctrl, 31, 1);
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

static int lcd_pll_ss_level_generate(struct lcd_clk_config_s *cconf)
{
	unsigned int dep_sel, str_m, err, min, done = 0;
	unsigned long long target, ss_ppm, dep_base;

	if (!cconf)
		return -1;

	target = cconf->ss_level;
	target *= 1000;
	min = cconf->data->ss_dep_base * 5;
	dep_base = cconf->data->ss_dep_base;
	for (str_m = 1; str_m <= cconf->data->ss_str_m_max; str_m++) { //str_m
		for (dep_sel = 1; dep_sel <= cconf->data->ss_dep_sel_max; dep_sel++) { //dep_sel
			ss_ppm = dep_sel * str_m * dep_base;
			if (ss_ppm > target)
				break;
			err = target - ss_ppm;
			if (err < min) {
				min = err;
				cconf->ss_dep_sel = dep_sel;
				cconf->ss_str_m = str_m;
				cconf->ss_ppm = ss_ppm;
				done++;
			}
		}
	}
	if (done == 0) {
		LCDERR("%s: invalid ss_level %d\n", __func__, cconf->ss_level);
		return -1;
	}

	if (lcd_debug_print_flag == 2) {
		LCDPR("%s: dep_sel=%d, str_m=%d, error=%d\n",
			__func__, cconf->ss_dep_sel, cconf->ss_str_m, min);
	}

	return 0;
}

static void lcd_set_pll_ss_level_gxtvbb(unsigned int level)
{
	unsigned int reg3, reg4, reg5, reg6;
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();

	if (cconf->pll_fvco >= 5500000000ULL && cconf->pll_fvco <= 6000000000ULL) {
		reg3 = pll_ss_reg_gxtvbb_high[level][0];
		reg4 = pll_ss_reg_gxtvbb_high[level][1];
		reg5 = pll_ss_reg_gxtvbb_high[level][2];
		reg6 = pll_ss_reg_gxtvbb_high[level][3];
	} else {
		reg3 = pll_ss_reg_gxtvbb_low[level][0];
		reg4 = pll_ss_reg_gxtvbb_low[level][1];
		reg5 = pll_ss_reg_gxtvbb_low[level][2];
		reg6 = pll_ss_reg_gxtvbb_low[level][3];
	}

	lcd_hiu_write(HHI_HPLL_CNTL3, reg3);
	lcd_hiu_write(HHI_HPLL_CNTL4, reg4);
	lcd_hiu_write(HHI_HPLL_CNTL5, reg5);
	lcd_hiu_write(HHI_HPLL_CNTL6, reg6);

	LCDPR("set pll spread spectrum: %s\n", lcd_ss_level_table_gxtvbb[level]);
}

static void lcd_set_pll_gxtvbb(struct lcd_clk_config_s *cconf)
{
	unsigned int pll_ctrl, pll_ctrl2;
	int ret, cnt = 0;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);
	pll_ctrl = ((1 << LCD_PLL_EN_GXTVBB) |
		(1 << 27) | /* DPLL_BGP_EN */
		(cconf->pll_n << LCD_PLL_N_GXTVBB) |
		(cconf->pll_m << LCD_PLL_M_GXTVBB));

	pll_ctrl2 = ((cconf->pll_od3_sel << LCD_PLL_OD3_GXTVBB) |
		(cconf->pll_od2_sel << LCD_PLL_OD2_GXTVBB) |
		(cconf->pll_od1_sel << LCD_PLL_OD1_GXTVBB));
	pll_ctrl2 |= ((1 << 14) | (cconf->pll_frac << 0));

set_pll_retry_gxtvbb:
	lcd_hiu_write(HHI_HPLL_CNTL, pll_ctrl | (1 << LCD_PLL_RST_GXTVBB));
	lcd_hiu_write(HHI_HPLL_CNTL2, pll_ctrl2);
	if (cconf->pll_fvco >= 5500000000ULL && cconf->pll_fvco <= 6000000000ULL) {
		lcd_hiu_write(HHI_HPLL_CNTL3, 0x12dc5081);
		lcd_hiu_write(HHI_HPLL_CNTL4, 0x801da72c);
		lcd_hiu_write(HHI_HPLL_CNTL5, 0x71486980);
		lcd_hiu_write(HHI_HPLL_CNTL6, 0x00002a55);
	} else {
		lcd_hiu_write(HHI_HPLL_CNTL3, 0x0d5c5091);
		lcd_hiu_write(HHI_HPLL_CNTL4, 0x801da72c);
		lcd_hiu_write(HHI_HPLL_CNTL5, 0x71486980);
		lcd_hiu_write(HHI_HPLL_CNTL6, 0x00002a55);
	}
	lcd_hiu_write(HHI_HPLL_CNTL, pll_ctrl);

	ret = lcd_pll_wait_lock(HHI_HPLL_CNTL, LCD_PLL_LOCK_GXTVBB);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_gxtvbb;
		LCDERR("hpll lock failed\n");
	}
	if (cconf->ss_level > 0)
		lcd_set_pll_ss_level_gxtvbb(cconf->ss_level);
}

static void lcd_set_pll_ss_level_txl(unsigned int level)
{
	unsigned int pll_ctrl3, pll_ctrl4;
	unsigned int dep_sel = 0, str_m = 0;

	pll_ctrl3 = lcd_hiu_read(HHI_HPLL_CNTL3);
	pll_ctrl4 = lcd_hiu_read(HHI_HPLL_CNTL4);
	pll_ctrl3 &= ~((0xf << 10) | (1 << 14));
	pll_ctrl4 &= ~(0x3 << 2);

	if (level > 0) {
		dep_sel = pll_ss_reg_txl[level][0];
		str_m = pll_ss_reg_txl[level][1];
		pll_ctrl3 |= ((1 << 14) | ((dep_sel & 0xf) << 10));
		pll_ctrl4 |= ((str_m & 0x3) << 2);
	}
	pll_ctrl3 |= pll_ss_reg_txl[level][0];
	pll_ctrl4 |= pll_ss_reg_txl[level][1];

	lcd_hiu_write(HHI_HPLL_CNTL3, pll_ctrl3);
	lcd_hiu_write(HHI_HPLL_CNTL4, pll_ctrl4);

	LCDPR("set pll spread spectrum: %s\n", lcd_ss_level_table_txl[level]);
}

static void lcd_set_pll_txl(struct lcd_clk_config_s *cConf)
{
	unsigned int pll_ctrl, pll_ctrl2, pll_ctrl3;
	int ret, cnt = 0;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);
	pll_ctrl = ((1 << LCD_PLL_EN_TXL) |
		(cConf->pll_n << LCD_PLL_N_TXL) |
		(cConf->pll_m << LCD_PLL_M_TXL));
	pll_ctrl2 = 0x800ca000;
	pll_ctrl2 |= ((1 << 12) | (cConf->pll_frac << 0));
	pll_ctrl3 = 0x860330c4 | (cConf->pll_od_fb << 30);
	pll_ctrl3 |= ((cConf->pll_od3_sel << LCD_PLL_OD3_TXL) |
		(cConf->pll_od2_sel << LCD_PLL_OD2_TXL) |
		(cConf->pll_od1_sel << LCD_PLL_OD1_TXL));

set_pll_retry_txl:
	lcd_hiu_write(HHI_HPLL_CNTL, pll_ctrl);
	lcd_hiu_write(HHI_HPLL_CNTL2, pll_ctrl2);
	lcd_hiu_write(HHI_HPLL_CNTL3, pll_ctrl3);
	if (cConf->pll_mode & LCD_PLL_MODE_SPECIAL_CNTL)
		lcd_hiu_write(HHI_HPLL_CNTL4, 0x0d160000);
	else
		lcd_hiu_write(HHI_HPLL_CNTL4, 0x0c8e0000);
	lcd_hiu_write(HHI_HPLL_CNTL5, 0x001fa729);
	lcd_hiu_write(HHI_HPLL_CNTL6, 0x01a31500);
	lcd_hiu_setb(HHI_HPLL_CNTL, 1, LCD_PLL_RST_TXL, 1);
	lcd_hiu_setb(HHI_HPLL_CNTL, 0, LCD_PLL_RST_TXL, 1);

	ret = lcd_pll_wait_lock(HHI_HPLL_CNTL, LCD_PLL_LOCK_TXL);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_txl;
		LCDERR("hpll lock failed\n");
	}
	if (cConf->ss_level > 0)
		lcd_set_pll_ss_level_txl(cConf->ss_level);
}

static void lcd_set_pll_ss_level_txlx(unsigned int level)
{
	unsigned int pll_ctrl3, pll_ctrl4, pll_ctrl5;
	unsigned int dep_sel = 0, str_m = 0;

	pll_ctrl3 = lcd_hiu_read(HHI_HPLL_CNTL3);
	pll_ctrl4 = lcd_hiu_read(HHI_HPLL_CNTL4);
	pll_ctrl5 = lcd_hiu_read(HHI_HPLL_CNTL5);
	pll_ctrl3 &= ~((0xf << 10) | (1 << 14));
	pll_ctrl4 &= ~(0x3 << 2);
	pll_ctrl5 &= ~(0x3 << 30);

	if (level > 0) {
		dep_sel = pll_ss_reg_txlx[level][0];
		str_m = pll_ss_reg_txlx[level][1];
		pll_ctrl3 |= ((1 << 14) | ((dep_sel & 0xf) << 10));
		pll_ctrl4 |= (((str_m >> 0) & 0x3) << 2);
		pll_ctrl5 |= (((str_m >> 2) & 0x3) << 30);
	}

	lcd_hiu_write(HHI_HPLL_CNTL3, pll_ctrl3);
	lcd_hiu_write(HHI_HPLL_CNTL4, pll_ctrl4);
	lcd_hiu_write(HHI_HPLL_CNTL5, pll_ctrl5);

	LCDPR("set pll spread spectrum: %s\n", lcd_ss_level_table_txlx[level]);
}

static void lcd_set_pll_txlx(struct lcd_clk_config_s *cConf)
{
	unsigned int pll_ctrl, pll_ctrl2, pll_ctrl3;
	int ret, cnt = 0;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);
	pll_ctrl = ((1 << LCD_PLL_EN_TXLX) |
		(cConf->pll_n << LCD_PLL_N_TXLX) |
		(cConf->pll_m << LCD_PLL_M_TXLX));
	pll_ctrl2 = 0x800ca000;
	pll_ctrl2 |= ((1 << 12) | (cConf->pll_frac << 0));
	pll_ctrl3 = 0x860030c4 | (cConf->pll_od_fb << 30);
	pll_ctrl3 |= ((cConf->pll_od3_sel << LCD_PLL_OD3_TXLX) |
		(cConf->pll_od2_sel << LCD_PLL_OD2_TXLX) |
		(cConf->pll_od1_sel << LCD_PLL_OD1_TXLX));

set_pll_retry_txlx:
	lcd_hiu_write(HHI_HPLL_CNTL, pll_ctrl);
	lcd_hiu_write(HHI_HPLL_CNTL2, pll_ctrl2);
	lcd_hiu_write(HHI_HPLL_CNTL3, pll_ctrl3);
	lcd_hiu_write(HHI_HPLL_CNTL4, 0x0c8e0000);
	lcd_hiu_write(HHI_HPLL_CNTL5, 0x001fa729);
	lcd_hiu_write(HHI_HPLL_CNTL6, 0x01a31500);
	lcd_hiu_setb(HHI_HPLL_CNTL, 1, LCD_PLL_RST_TXLX, 1);
	lcd_hiu_setb(HHI_HPLL_CNTL, 0, LCD_PLL_RST_TXLX, 1);

	ret = lcd_pll_wait_lock(HHI_HPLL_CNTL, LCD_PLL_LOCK_TXLX);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_txlx;
		LCDERR("hpll lock failed\n");
	}

	if (cConf->ss_level > 0)
		lcd_set_pll_ss_level_txlx(cConf->ss_level);
}

static void lcd_set_pll_ss_level_txhd(unsigned int level)
{
	unsigned int pll_ctrl3, pll_ctrl4, pll_ctrl5;
	unsigned int dep_sel = 0, str_m = 0;

	pll_ctrl3 = lcd_hiu_read(HHI_HPLL_CNTL3);
	pll_ctrl4 = lcd_hiu_read(HHI_HPLL_CNTL4);
	pll_ctrl5 = lcd_hiu_read(HHI_HPLL_CNTL5);
	pll_ctrl3 &= ~((0xf << 10) | (1 << 14) | (1 << 18));
	pll_ctrl4 &= ~(0x3 << 2);
	pll_ctrl5 &= ~(0x3 << 30);

	if (level == 0) {
		pll_ctrl3 |= (1 << 18);
	} else {
		dep_sel = pll_ss_reg_txhd[level][0];
		str_m = pll_ss_reg_txhd[level][1];
		pll_ctrl3 |= ((1 << 14) | ((dep_sel & 0xf) << 10));
		pll_ctrl4 |= (((str_m >> 0) & 0x3) << 2);
		pll_ctrl5 |= (((str_m >> 2) & 0x3) << 30);
	}

	lcd_hiu_write(HHI_HPLL_CNTL3, pll_ctrl3);
	lcd_hiu_write(HHI_HPLL_CNTL4, pll_ctrl4);
	lcd_hiu_write(HHI_HPLL_CNTL5, pll_ctrl5);

	LCDPR("set pll spread spectrum: %s\n", lcd_ss_level_table_txhd[level]);
}

static void lcd_set_pll_ss_advance_txhd(unsigned int freq, unsigned int mode)
{
	unsigned int pll_ctrl4, pll_ctrl6;

	pll_ctrl4 = lcd_hiu_read(HHI_HDMI_PLL_CNTL4);
	pll_ctrl6 = lcd_hiu_read(HHI_HDMI_PLL_CNTL6);
	pll_ctrl6 &= ~(0x7 << 0); /* ss_freq */
	pll_ctrl6 |= (freq << 0);
	pll_ctrl4 &= ~(0x3 << 30); /* ss_mode */
	pll_ctrl4 |= (mode << 30);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL4, pll_ctrl4);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL6, pll_ctrl6);

	LCDPR("set pll spread spectrum freq=%d, mode=%d\n", freq, mode);
}

static void lcd_set_pll_txhd(struct lcd_clk_config_s *cconf)
{
	unsigned int pll_ctrl, pll_ctrl2, pll_ctrl3, pll_ctrl6;
	int ret, cnt = 0;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);
	pll_ctrl = ((1 << LCD_PLL_EN_TXHD) |
		(cconf->pll_n << LCD_PLL_N_TXHD) |
		(cconf->pll_m << LCD_PLL_M_TXHD));
	if (cconf->pll_fvco > 4500000000ULL)
		pll_ctrl2 = 0x800ca000; /* bit[18]=1 */
	else
		pll_ctrl2 = 0x8008a000; /* bit[18]=0 */
	pll_ctrl2 |= ((1 << 12) | (cconf->pll_frac << 0));
	pll_ctrl3 = 0x860730c4 | (cconf->pll_od_fb << 30);
	pll_ctrl3 |= ((cconf->pll_od3_sel << LCD_PLL_OD3_TXHD) |
		(cconf->pll_od2_sel << LCD_PLL_OD2_TXHD) |
		(cconf->pll_od1_sel << LCD_PLL_OD1_TXHD));
	pll_ctrl6 = (0x01a31500 | (cconf->pll_tcon_div_sel << 30));

set_pll_retry_txhd:
	lcd_hiu_write(HHI_HPLL_CNTL, pll_ctrl);
	lcd_hiu_write(HHI_HPLL_CNTL2, pll_ctrl2);
	lcd_hiu_write(HHI_HPLL_CNTL3, pll_ctrl3);
	lcd_hiu_write(HHI_HPLL_CNTL4, 0x0a960000); /* 0x0c8e0000 */
	lcd_hiu_write(HHI_HPLL_CNTL5, 0x001fa729);
	lcd_hiu_write(HHI_HPLL_CNTL6, pll_ctrl6);
	lcd_hiu_setb(HHI_HPLL_CNTL, 1, LCD_PLL_RST_TXHD, 1);
	lcd_hiu_setb(HHI_HPLL_CNTL, 0, LCD_PLL_RST_TXHD, 1);

	ret = lcd_pll_wait_lock(HHI_HPLL_CNTL, LCD_PLL_LOCK_TXHD);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_txhd;
		LCDERR("hpll lock failed\n");
	}

	if (cconf->ss_level > 0) {
		lcd_set_pll_ss_level_txhd(cconf->ss_level);
		lcd_set_pll_ss_advance_txhd(cconf->ss_freq, cconf->ss_mode);
	}
}

static void lcd_set_pll_axg(struct lcd_clk_config_s *cconf)
{
	unsigned int pll_ctrl, pll_ctrl1, pll_ctrl2;
	int ret, cnt = 0;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);

	pll_ctrl = ((1 << LCD_PLL_EN_AXG) |
		(cconf->pll_n << LCD_PLL_N_AXG) |
		(cconf->pll_m << LCD_PLL_M_AXG) |
		(cconf->pll_od1_sel << LCD_PLL_OD_AXG));
	pll_ctrl1 = 0xc084a000;
	pll_ctrl1 |= ((1 << 12) | (cconf->pll_frac << 0));
	pll_ctrl2 = 0xb75020be | (cconf->pll_od_fb << 19);

set_pll_retry_axg:
	lcd_hiu_write(HHI_GP0_PLL_CNTL, pll_ctrl);
	lcd_hiu_write(HHI_GP0_PLL_CNTL1, pll_ctrl1);
	lcd_hiu_write(HHI_GP0_PLL_CNTL2, pll_ctrl2);
	lcd_hiu_write(HHI_GP0_PLL_CNTL3, 0x0a59a288);
	lcd_hiu_write(HHI_GP0_PLL_CNTL4, 0xc000004d);
	if (cconf->pll_fvco >= 1632000000ULL)
		lcd_hiu_write(HHI_GP0_PLL_CNTL5, 0x00058000);
	else
		lcd_hiu_write(HHI_GP0_PLL_CNTL5, 0x00078000);
	lcd_hiu_setb(HHI_GP0_PLL_CNTL, 1, LCD_PLL_RST_AXG, 1);
	lcd_hiu_setb(HHI_GP0_PLL_CNTL, 0, LCD_PLL_RST_AXG, 1);

	ret = lcd_pll_wait_lock(HHI_GP0_PLL_CNTL, LCD_PLL_LOCK_AXG);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_axg;
		LCDERR("hpll lock failed\n");
	}
}

static void lcd_set_gp0_pll_g12a(struct lcd_clk_config_s *cConf)
{
	unsigned int pll_ctrl, pll_ctrl1, pll_ctrl3, pll_ctrl4, pll_ctrl6;
	int ret, cnt = 0;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);

	pll_ctrl = ((1 << LCD_PLL_EN_GP0_G12A) |
		(cConf->pll_n << LCD_PLL_N_GP0_G12A) |
		(cConf->pll_m << LCD_PLL_M_GP0_G12A) |
		(cConf->pll_od1_sel << LCD_PLL_OD_GP0_G12A));
	pll_ctrl1 = (cConf->pll_frac << 0);
	if (cConf->pll_frac) {
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
	lcd_hiu_write(HHI_GP0_PLL_CNTL0, pll_ctrl);
	lcd_hiu_write(HHI_GP0_PLL_CNTL1, pll_ctrl1);
	lcd_hiu_write(HHI_GP0_PLL_CNTL2, 0x00);
	lcd_hiu_write(HHI_GP0_PLL_CNTL3, pll_ctrl3);
	lcd_hiu_write(HHI_GP0_PLL_CNTL4, pll_ctrl4);
	lcd_hiu_write(HHI_GP0_PLL_CNTL5, 0x39272000);
	lcd_hiu_write(HHI_GP0_PLL_CNTL6, pll_ctrl6);
	lcd_hiu_setb(HHI_GP0_PLL_CNTL0, 1, LCD_PLL_RST_GP0_G12A, 1);
	udelay(100);
	lcd_hiu_setb(HHI_GP0_PLL_CNTL0, 0, LCD_PLL_RST_GP0_G12A, 1);

	ret = lcd_pll_wait_lock_g12a(1);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_gp0_pll_retry_g12a;
		LCDERR("hpll lock failed\n");
	}
}

static void lcd_set_hpll_g12a(struct lcd_clk_config_s *cConf)
{
	unsigned int pll_ctrl, pll_ctrl1, pll_ctrl3, pll_ctrl4, pll_ctrl6;
	int ret, cnt = 0;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);

	pll_ctrl = ((1 << LCD_PLL_EN_HPLL_G12A) |
		(1 << 25) | /* clk out gate */
		(cConf->pll_n << LCD_PLL_N_HPLL_G12A) |
		(cConf->pll_m << LCD_PLL_M_HPLL_G12A) |
		(cConf->pll_od1_sel << LCD_PLL_OD1_HPLL_G12A) |
		(cConf->pll_od2_sel << LCD_PLL_OD2_HPLL_G12A) |
		(cConf->pll_od3_sel << LCD_PLL_OD3_HPLL_G12A));
	pll_ctrl1 = (cConf->pll_frac << 0);
	if (cConf->pll_frac) {
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
	lcd_hiu_write(HHI_HDMI_PLL_CNTL0, pll_ctrl);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL1, pll_ctrl1);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL2, 0x00);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL3, pll_ctrl3);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL4, pll_ctrl4);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL5, 0x39272000);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL6, pll_ctrl6);
	lcd_hiu_setb(HHI_HDMI_PLL_CNTL0, 1, LCD_PLL_RST_HPLL_G12A, 1);
	udelay(100);
	lcd_hiu_setb(HHI_HDMI_PLL_CNTL0, 0, LCD_PLL_RST_HPLL_G12A, 1);

	ret = lcd_pll_wait_lock_g12a(0);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_hpll_pll_retry_g12a;
		LCDERR("hpll lock failed\n");
	}
}

static void lcd_set_gp0_pll_g12b(struct lcd_clk_config_s *cConf)
{
	unsigned int pll_ctrl, pll_ctrl1, pll_ctrl3, pll_ctrl4, pll_ctrl6;
	int ret, cnt = 0;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);

	pll_ctrl = ((1 << LCD_PLL_EN_GP0_G12A) |
		(cConf->pll_n << LCD_PLL_N_GP0_G12A) |
		(cConf->pll_m << LCD_PLL_M_GP0_G12A) |
		(cConf->pll_od1_sel << LCD_PLL_OD_GP0_G12A));
	pll_ctrl1 = (cConf->pll_frac << 0);
	if (cConf->pll_frac) {
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
	lcd_hiu_write(HHI_GP0_PLL_CNTL0, pll_ctrl);
	lcd_hiu_write(HHI_GP0_PLL_CNTL1, pll_ctrl1);
	lcd_hiu_write(HHI_GP0_PLL_CNTL2, 0x00);
	lcd_hiu_write(HHI_GP0_PLL_CNTL3, pll_ctrl3);
	lcd_hiu_write(HHI_GP0_PLL_CNTL4, pll_ctrl4);
	lcd_hiu_write(HHI_GP0_PLL_CNTL5, 0x39272000);
	lcd_hiu_write(HHI_GP0_PLL_CNTL6, pll_ctrl6);
	lcd_hiu_setb(HHI_GP0_PLL_CNTL0, 1, LCD_PLL_RST_GP0_G12A, 1);
	udelay(100);
	lcd_hiu_setb(HHI_GP0_PLL_CNTL0, 0, LCD_PLL_RST_GP0_G12A, 1);

	ret = lcd_pll_wait_lock(HHI_GP0_PLL_CNTL0, LCD_PLL_LOCK_GP0_G12A);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_gp0_pll_retry_g12b;
		LCDERR("hpll lock failed\n");
	}
}

static void lcd_set_hpll_g12b(struct lcd_clk_config_s *cConf)
{
	unsigned int pll_ctrl, pll_ctrl1, pll_ctrl3, pll_ctrl4, pll_ctrl6;
	int ret, cnt = 0;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);

	pll_ctrl = ((1 << LCD_PLL_EN_HPLL_G12A) |
		(1 << 25) | /* clk out gate */
		(cConf->pll_n << LCD_PLL_N_HPLL_G12A) |
		(cConf->pll_m << LCD_PLL_M_HPLL_G12A) |
		(cConf->pll_od1_sel << LCD_PLL_OD1_HPLL_G12A) |
		(cConf->pll_od2_sel << LCD_PLL_OD2_HPLL_G12A) |
		(cConf->pll_od3_sel << LCD_PLL_OD3_HPLL_G12A));
	pll_ctrl1 = (cConf->pll_frac << 0);
	if (cConf->pll_frac) {
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
	lcd_hiu_write(HHI_HDMI_PLL_CNTL0, pll_ctrl);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL1, pll_ctrl1);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL2, 0x00);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL3, pll_ctrl3);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL4, pll_ctrl4);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL5, 0x39272000);
	lcd_hiu_write(HHI_HDMI_PLL_CNTL6, pll_ctrl6);
	lcd_hiu_setb(HHI_HDMI_PLL_CNTL0, 1, LCD_PLL_RST_HPLL_G12A, 1);
	udelay(100);
	lcd_hiu_setb(HHI_HDMI_PLL_CNTL0, 0, LCD_PLL_RST_HPLL_G12A, 1);

	ret = lcd_pll_wait_lock(HHI_HDMI_PLL_CNTL0, LCD_PLL_LOCK_HPLL_G12A);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_hpll_pll_retry_g12b;
		LCDERR("hpll lock failed\n");
	}
}

static void lcd_set_pll_ss_level_tl1(unsigned int level)
{
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();
	unsigned int pll_ctrl2;

	pll_ctrl2 = lcd_hiu_read(HHI_TCON_PLL_CNTL2);
	pll_ctrl2 &= ~((0xf << 16) | (0xf << 28));

	if (level > 0) {
		cconf->ss_dep_sel = pll_ss_reg_tl1[level][0];
		cconf->ss_str_m = pll_ss_reg_tl1[level][1];
		pll_ctrl2 |= ((cconf->ss_dep_sel << 28) | (cconf->ss_str_m << 16));
		cconf->ss_ppm = cconf->ss_dep_sel * cconf->ss_str_m * cconf->data->ss_dep_base;
		LCDPR("set pll spread spectrum: level %d, %dppm\n", level, cconf->ss_ppm);
	} else {
		cconf->ss_en = 0;
		LCDPR("set pll spread spectrum: disable\n");
	}

	lcd_hiu_write(HHI_TCON_PLL_CNTL2, pll_ctrl2);
}

static void lcd_set_pll_ss_advance_tl1(unsigned int freq, unsigned int mode)
{
	unsigned int pll_ctrl2;

	pll_ctrl2 = lcd_hiu_read(HHI_TCON_PLL_CNTL2);
	pll_ctrl2 &= ~(0x7 << 24); /* ss_freq */
	pll_ctrl2 |= (freq << 24);
	pll_ctrl2 &= ~(0x3 << 22); /* ss_mode */
	pll_ctrl2 |= (mode << 22);
	lcd_hiu_write(HHI_TCON_PLL_CNTL2, pll_ctrl2);

	LCDPR("set pll spread spectrum: freq=%d, mode=%d\n", freq, mode);
}

static void lcd_set_pll_tl1(struct lcd_clk_config_s *cConf)
{
	unsigned int pll_ctrl, pll_ctrl1;
	unsigned int tcon_div[5][3] = {
		/* div_mux, div2/4_sel, div4_bypass */
		{1, 0, 1},  /* div1 */
		{0, 0, 1},  /* div2 */
		{0, 1, 1},  /* div4 */
		{0, 0, 0},  /* div8 */
		{0, 1, 0},  /* div16 */
	};
	unsigned int tcon_div_sel = cConf->pll_tcon_div_sel;
	int ret, cnt = 0;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);
	pll_ctrl = ((0x3 << 17) | /* gate ctrl */
		(tcon_div[tcon_div_sel][2] << 16) |
		(cConf->pll_n << LCD_PLL_N_TL1) |
		(cConf->pll_m << LCD_PLL_M_TL1) |
		(cConf->pll_od3_sel << LCD_PLL_OD3_TL1) |
		(cConf->pll_od2_sel << LCD_PLL_OD2_TL1) |
		(cConf->pll_od1_sel << LCD_PLL_OD1_TL1));
	pll_ctrl1 = (1 << 28) |
		(tcon_div[tcon_div_sel][0] << 22) |
		(tcon_div[tcon_div_sel][1] << 21) |
		((1 << 20) | /* sdm_en */
		(cConf->pll_frac << 0));

set_pll_retry_tl1:
	lcd_hiu_write(HHI_TCON_PLL_CNTL0, pll_ctrl);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 1, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 1, LCD_PLL_EN_TL1, 1);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL1, pll_ctrl1);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL2, 0x0000110c);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL3, 0x10051400);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL4, 0x0100c0, 0, 24);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL4, 0x8300c0, 0, 24);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 1, 26, 1);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 0, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL2, 0x0000300c);

	ret = lcd_pll_wait_lock(HHI_TCON_PLL_CNTL0, LCD_PLL_LOCK_TL1);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_tl1;
		LCDERR("hpll lock failed\n");
	} else {
		udelay(100);
		lcd_hiu_setb(HHI_TCON_PLL_CNTL2, 1, 5, 1);
	}

	if (cConf->ss_level > 0) {
		lcd_set_pll_ss_level_tl1(cConf->ss_level);
		lcd_set_pll_ss_advance_tl1(cConf->ss_freq, cConf->ss_mode);
	}
}

static void lcd_prbs_set_pll_vx1_tl1(void)
{
	int cnt = 0, ret;

	if (lcd_debug_print_flag == 6)
		LCDPR("%s\n", __func__);

lcd_prbs_retry_pll_vx1_tl1:
	lcd_hiu_write(HHI_TCON_PLL_CNTL0, 0x000f04f7);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 1, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 1, LCD_PLL_EN_TL1, 1);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL1, 0x10110000);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL2, 0x00001108);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL3, 0x10051400);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL4, 0x010100c0);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL4, 0x038300c0);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 1, 26, 1);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 0, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL2, 0x00003008);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL2, 0x00003028);
	udelay(10);

	ret = lcd_pll_wait_lock(HHI_TCON_PLL_CNTL0, HHI_TCON_PLL_CNTL1);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto lcd_prbs_retry_pll_vx1_tl1;
		LCDERR("hpll lock failed\n");
	}

	/* pll_div */
	/* Disable the div output clock */
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 19, 1);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);

	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 18, 1);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 16, 2);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 0, 14);

	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 2, 16, 2);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 1, 15, 1);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0x739c, 0, 15);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);

	/* Enable the final output clock */
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 1, 19, 1);
}

static void lcd_prbs_set_pll_lvds_tl1(void)
{
	int cnt = 0, ret;

	if (lcd_debug_print_flag == 6)
		LCDPR("%s\n", __func__);

lcd_prbs_retry_pll_lvds_tl1:
	lcd_hiu_write(HHI_TCON_PLL_CNTL0, 0x008e049f);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL0, 0x208e049f);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL0, 0x3006049f);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL1, 0x10000000);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL2, 0x00001102);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL3, 0x10051400);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL4, 0x010100c0);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL4, 0x038300c0);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL0, 0x348e049f);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL0, 0x148e049f);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL2, 0x00003002);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL2, 0x00003022);
	udelay(10);

	ret = lcd_pll_wait_lock(HHI_TCON_PLL_CNTL0, LCD_PLL_LOCK_TL1);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto lcd_prbs_retry_pll_lvds_tl1;
		LCDERR("hpll lock failed\n");
	}

	/* pll_div */
	/* Disable the div output clock */
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 19, 1);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);

	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 18, 1);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 16, 2);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 0, 14);

	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 1, 16, 2);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 1, 15, 1);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0x3c78, 0, 15);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);

	/* Enable the final output clock */
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 1, 19, 1);
}

void lcd_prbs_config_clk_tl1(unsigned int lcd_prbs_mode)
{
	if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
		lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
		lcd_prbs_set_pll_vx1_tl1();
	} else if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
		lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
		lcd_prbs_set_pll_lvds_tl1();
	} else {
		LCDERR("%s: unsupport lcd_prbs_mode %d\n",
		       __func__, lcd_prbs_mode);
		return;
	}

	lcd_hiu_setb(HHI_VIID_CLK_DIV, 0, VCLK2_XD, 8);
	udelay(5);

	/* select vid_pll_clk */
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_CLK_IN_SEL, 3);
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 1, VCLK2_EN, 1);
	udelay(5);

	/* [15:12] encl_clk_sel, select vclk2_div1 */
	lcd_hiu_setb(HHI_VIID_CLK_DIV, 8, ENCL_CLK_SEL, 4);
	/* release vclk2_div_reset and enable vclk2_div */
	lcd_hiu_setb(HHI_VIID_CLK_DIV, 1, VCLK2_XD_EN, 2);
	udelay(5);

	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 1, VCLK2_DIV1_EN, 1);
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 1, VCLK2_SOFT_RST, 1);
	udelay(10);
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_SOFT_RST, 1);
	udelay(5);

	/* enable CTS_ENCL clk gate */
	lcd_hiu_setb(HHI_VID_CLK_CNTL2, 1, ENCL_GATE_VCLK, 1);

	if (lcd_debug_print_flag == 6)
		LCDPR("%s ok\n", __func__);
}

static void lcd_set_pll_ss_level_t5w(unsigned int level)
{
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();
	unsigned int pll_ctrl2;
	int ret;

	pll_ctrl2 = lcd_hiu_read(HHI_TCON_PLL_CNTL2);
	pll_ctrl2 &= ~((0xf << 16) | (0xf << 28));

	if (level > 0) {
		ret = lcd_pll_ss_level_generate(cconf);
		if (ret == 0) {
			cconf->ss_en = 1;
			pll_ctrl2 |= ((cconf->ss_dep_sel << 28) | (cconf->ss_str_m << 16));
			LCDPR("set pll spread spectrum: level: %d, %dppm\n",
				cconf->ss_level, cconf->ss_ppm);
		}
	} else {
		cconf->ss_en = 0;
		LCDPR("set pll spread spectrum: disable\n");
	}

	lcd_hiu_write(HHI_TCON_PLL_CNTL2, pll_ctrl2);
}

static void lcd_set_pll_t5w(struct lcd_clk_config_s *cconf)
{
	unsigned int pll_ctrl, pll_ctrl1;
	unsigned int tcon_div[5][3] = {
		/* div_mux, div2/4_sel, div4_bypass */
		{1, 0, 1},  /* div1 */
		{0, 0, 1},  /* div2 */
		{0, 1, 1},  /* div4 */
		{0, 0, 0},  /* div8 */
		{0, 1, 0},  /* div16 */
	};
	unsigned int tcon_div_sel = cconf->pll_tcon_div_sel;
	int ret, cnt = 0;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);
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

set_pll_retry_t5w:
	lcd_hiu_write(HHI_TCON_PLL_CNTL0, pll_ctrl);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 1, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 1, LCD_PLL_EN_TL1, 1);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL1, pll_ctrl1);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL2, 0x0000110c);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL3, 0x10051400);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL4, 0x0100c0, 0, 24);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL4, 0x8300c0, 0, 24);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 1, 26, 1);
	udelay(10);
	lcd_hiu_setb(HHI_TCON_PLL_CNTL0, 0, LCD_PLL_RST_TL1, 1);
	udelay(10);
	lcd_hiu_write(HHI_TCON_PLL_CNTL2, 0x0000300c);

	ret = lcd_pll_wait_lock(HHI_TCON_PLL_CNTL0, LCD_PLL_LOCK_TL1);
	if (ret) {
		if (cnt++ < PLL_RETRY_MAX)
			goto set_pll_retry_t5w;
		LCDERR("hpll lock failed\n");
	} else {
		udelay(100);
		lcd_hiu_setb(HHI_TCON_PLL_CNTL2, 1, 5, 1);
	}

	if (cconf->ss_level > 0) {
		lcd_set_pll_ss_level_t5w(cconf->ss_level);
		lcd_set_pll_ss_advance_tl1(cconf->ss_freq, cconf->ss_mode);
	}
}

static void lcd_prbs_config_clk_t5w(unsigned int lcd_prbs_mode)
{
	unsigned int reg_vid2_clk_div, reg_vid2_clk_ctrl, reg_vid_clk_ctrl2;

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);

	reg_vid2_clk_div = HHI_VIID_CLK0_DIV;
	reg_vid2_clk_ctrl = HHI_VIID_CLK0_CTRL;
	reg_vid_clk_ctrl2 = HHI_VID_CLK0_CTRL2;

	if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
		lcd_hiu_setb(reg_vid2_clk_ctrl, 0, VCLK2_EN, 1);
		lcd_prbs_set_pll_vx1_tl1();
	} else if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
		lcd_hiu_setb(reg_vid2_clk_ctrl, 0, VCLK2_EN, 1);
		lcd_prbs_set_pll_lvds_tl1();
	} else {
		LCDERR("%s: unsupport lcd_prbs_mode %d\n",
		       __func__, lcd_prbs_mode);
		return;
	}

	lcd_hiu_setb(reg_vid2_clk_div, 0, VCLK2_XD, 8);
	udelay(5);

	/* select vid_pll_clk */
	lcd_hiu_setb(reg_vid2_clk_ctrl, 0, VCLK2_CLK_IN_SEL, 3);
	lcd_hiu_setb(reg_vid2_clk_ctrl, 1, VCLK2_EN, 1);
	udelay(5);

	/* [15:12] encl_clk_sel, select vclk2_div1 */
	lcd_hiu_setb(reg_vid2_clk_div, 8, ENCL_CLK_SEL, 4);
	/* release vclk2_div_reset and enable vclk2_div */
	lcd_hiu_setb(reg_vid2_clk_div, 1, VCLK2_XD_EN, 2);
	udelay(5);

	lcd_hiu_setb(reg_vid2_clk_ctrl, 1, VCLK2_DIV1_EN, 1);
	lcd_hiu_setb(reg_vid2_clk_ctrl, 1, VCLK2_SOFT_RST, 1);
	udelay(10);
	lcd_hiu_setb(reg_vid2_clk_ctrl, 0, VCLK2_SOFT_RST, 1);
	udelay(5);

	/* enable CTS_ENCL clk gate */
	lcd_hiu_setb(reg_vid_clk_ctrl2, 1, ENCL_GATE_VCLK, 1);

	if (lcd_debug_print_flag == 6)
		LCDPR("%s ok\n", __func__);
}

static void lcd_set_vid_pll_div(struct lcd_clk_config_s *cConf)
{
	unsigned int shift_val, shift_sel;
	int i;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);

	/* Disable the div output clock */
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 19, 1);
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);

	i = 0;
	while (lcd_clk_div_table[i][0] != CLK_DIV_SEL_MAX) {
		if (cConf->div_sel == lcd_clk_div_table[i][0])
			break;
		i++;
	}
	if (lcd_clk_div_table[i][0] == CLK_DIV_SEL_MAX)
		LCDERR("invalid clk divider\n");
	shift_val = lcd_clk_div_table[i][1];
	shift_sel = lcd_clk_div_table[i][2];

	if (shift_val == 0xffff) { /* if divide by 1 */
		lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 1, 18, 1);
	} else {
		lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 18, 1);
		lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 16, 2);
		lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);
		lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 0, 14);

		lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, shift_sel, 16, 2);
		lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 1, 15, 1);
		lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, shift_val, 0, 15);
		lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 0, 15, 1);
	}
	/* Enable the final output clock */
	lcd_hiu_setb(HHI_VID_PLL_CLK_DIV, 1, 19, 1);
}

static void lcd_set_vclk_crt(struct lcd_config_s *pconf)
{
	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);

#ifdef CONFIG_AML_LCD_PXP
	/* setup the XD divider value */
	lcd_hiu_setb(HHI_VIID_CLK_DIV, clk_conf.xd, VCLK2_XD, 8);
	udelay(5);
	/* select vid_pll_clk */
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 7, VCLK2_CLK_IN_SEL, 3);
#else
	/* setup the XD divider value */
	lcd_hiu_setb(HHI_VIID_CLK_DIV, (clk_conf.xd - 1), VCLK2_XD, 8);
	udelay(5);
	/* select vid_pll_clk */
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, clk_conf.data->vclk_sel,
		VCLK2_CLK_IN_SEL, 3);
#endif
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 1, VCLK2_EN, 1);
	udelay(2);

	/* [15:12] encl_clk_sel, select vclk2_div1 */
	lcd_hiu_setb(HHI_VIID_CLK_DIV, 8, ENCL_CLK_SEL, 4);
	/* release vclk2_div_reset and enable vclk2_div */
	lcd_hiu_setb(HHI_VIID_CLK_DIV, 1, VCLK2_XD_EN, 2);
	udelay(5);

	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 1, VCLK2_DIV1_EN, 1);
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 1, VCLK2_SOFT_RST, 1);
	udelay(10);
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_SOFT_RST, 1);
	udelay(5);

	/* enable CTS_ENCL clk gate */
	lcd_hiu_setb(HHI_VID_CLK_CNTL2, 1, ENCL_GATE_VCLK, 1);
}

static void lcd_set_vclk_crt_t5w(struct lcd_config_s *pconf)
{
	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);

#ifdef CONFIG_AML_LCD_PXP
	/* setup the XD divider value */
	lcd_hiu_setb(HHI_VIID_CLK0_DIV, 0, VCLK2_XD, 8);
	udelay(5);
	/* select vid_pll_clk */
	lcd_hiu_setb(HHI_VIID_CLK0_CTRL, 7, VCLK2_CLK_IN_SEL, 3);
#else
	/* setup the XD divider value */
	lcd_hiu_setb(HHI_VIID_CLK0_DIV, (clk_conf.xd - 1), VCLK2_XD, 8);
	udelay(5);
	/* select vid_pll_clk */
	lcd_hiu_setb(HHI_VIID_CLK0_CTRL, clk_conf.data->vclk_sel,
		VCLK2_CLK_IN_SEL, 3);
#endif
	lcd_hiu_setb(HHI_VIID_CLK0_CTRL, 1, VCLK2_EN, 1);
	udelay(2);

	/* [15:12] encl_clk_sel, select vclk2_div1 */
	lcd_hiu_setb(HHI_VIID_CLK0_DIV, 8, ENCL_CLK_SEL, 4);
	/* release vclk2_div_reset and enable vclk2_div */
	lcd_hiu_setb(HHI_VIID_CLK0_DIV, 1, VCLK2_XD_EN, 2);
	udelay(5);

	lcd_hiu_setb(HHI_VIID_CLK0_CTRL, 1, VCLK2_DIV1_EN, 1);
	lcd_hiu_setb(HHI_VIID_CLK0_CTRL, 1, VCLK2_SOFT_RST, 1);
	udelay(10);
	lcd_hiu_setb(HHI_VIID_CLK0_CTRL, 0, VCLK2_SOFT_RST, 1);
	udelay(5);

	/* enable CTS_ENCL clk gate */
	lcd_hiu_setb(HHI_VID_CLK0_CTRL2, 1, ENCL_GATE_VCLK, 1);
}

static void lcd_set_dsi_meas_clk(void)
{
	lcd_hiu_setb(HHI_VDIN_MEAS_CLK_CNTL, 0, 21, 3);
	lcd_hiu_setb(HHI_VDIN_MEAS_CLK_CNTL, 0, 12, 7);
	lcd_hiu_setb(HHI_VDIN_MEAS_CLK_CNTL, 1, 20, 1);
}

static void lcd_set_dsi_phy_clk(int sel)
{
	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);

	lcd_hiu_setb(HHI_MIPIDSI_PHY_CLK_CNTL, sel, 12, 3);
	lcd_hiu_setb(HHI_MIPIDSI_PHY_CLK_CNTL, 1, 8, 1);
	lcd_hiu_setb(HHI_MIPIDSI_PHY_CLK_CNTL, 0, 0, 7);
}

static void lcd_set_tcon_clk(struct lcd_config_s *pconf)
{
	unsigned int val;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);

	switch (pconf->lcd_basic.lcd_type) {
	case LCD_LVDS:
		lcd_hiu_write(HHI_DIF_TCON_CNTL0, 0x0);
		lcd_hiu_write(HHI_DIF_TCON_CNTL0, 0x80000000);
		lcd_hiu_write(HHI_DIF_TCON_CNTL1, 0x0);
		lcd_hiu_write(HHI_DIF_TCON_CNTL2, 0x0);
		break;
	case LCD_MLVDS:
		val = pconf->lcd_control.mlvds_config->pi_clk_sel;
		/*val = (~val) & 0x3ff;*/
		lcd_hiu_write(HHI_DIF_TCON_CNTL0, (val << 12));
		lcd_hiu_write(HHI_DIF_TCON_CNTL0, ((1 << 31) | (val << 12)));

		val = pconf->lcd_control.mlvds_config->clk_phase & 0xfff;
		lcd_hiu_write(HHI_DIF_TCON_CNTL1, val);
		lcd_hiu_write(HHI_DIF_TCON_CNTL2, 0x0);

		/* tcon_clk 50M */
		lcd_hiu_write(HHI_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (7 << 0));
		break;
	default:
		break;
	}
}

static void lcd_set_tcon_clk_tl1(struct lcd_config_s *pconf)
{
	unsigned int val;

	if (lcd_debug_print_flag == 2)
		LCDPR("%s\n", __func__);

	switch (pconf->lcd_basic.lcd_type) {
	case LCD_MLVDS:
		val = pconf->lcd_control.mlvds_config->clk_phase & 0xfff;
		lcd_hiu_setb(HHI_TCON_PLL_CNTL1, (val & 0xf), 24, 4);
		lcd_hiu_setb(HHI_TCON_PLL_CNTL4, ((val >> 4) & 0xf), 28, 4);
		lcd_hiu_setb(HHI_TCON_PLL_CNTL4, ((val >> 8) & 0xf), 24, 4);

		/* tcon_clk */
		if (pconf->lcd_timing.lcd_clk >= 100000000) /* 25M */
			lcd_hiu_write(HHI_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (0xf << 0));
		else /* 12.5M */
			lcd_hiu_write(HHI_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (0x1f << 0));
		break;
	case LCD_P2P:
		/* tcon_clk 50M */
		lcd_hiu_write(HHI_TCON_CLK_CNTL, (1 << 7) | (1 << 6) | (7 << 0));
		break;
	default:
		break;
	}
}

static void lcd_set_tcon_clk_t5(struct lcd_config_s *pconf)
{
	if (pconf->lcd_basic.lcd_type != LCD_MLVDS &&
	    pconf->lcd_basic.lcd_type != LCD_P2P)
		return;

	lcd_set_tcon_clk_tl1(pconf);

	/* global reset tcon */
	lcd_reset_setb(RESET1_MASK, 0, 4, 1);
	lcd_reset_setb(RESET1_LEVEL, 0, 4, 1);
	udelay(1);
	lcd_reset_setb(RESET1_LEVEL, 1, 4, 1);
	udelay(2);
	LCDPR("reset tcon\n");
}

/* ****************************************************
 * lcd clk parameters calculate
 * ****************************************************
 */
static unsigned long long clk_vid_pll_div_calc(unsigned long long clk,
		unsigned int div_sel, int dir)
{
	unsigned long long clk_ret;

	switch (div_sel) {
	case CLK_DIV_SEL_1:
		clk_ret = clk;
		break;
	case CLK_DIV_SEL_2:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk, 2);
		else
			clk_ret = clk * 2;
		break;
	case CLK_DIV_SEL_3:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk, 3);
		else
			clk_ret = clk * 3;
		break;
	case CLK_DIV_SEL_3p5:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk * 2, 7);
		else
			clk_ret = lcd_do_div(clk * 7, 2);
		break;
	case CLK_DIV_SEL_3p75:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk * 4, 15);
		else
			clk_ret = lcd_do_div(clk * 15, 4);
		break;
	case CLK_DIV_SEL_4:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk, 4);
		else
			clk_ret = clk * 4;
		break;
	case CLK_DIV_SEL_5:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk, 5);
		else
			clk_ret = clk * 5;
		break;
	case CLK_DIV_SEL_6:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk, 6);
		else
			clk_ret = clk * 6;
		break;
	case CLK_DIV_SEL_6p25:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk * 4, 25);
		else
			clk_ret = lcd_do_div(clk * 25, 4);
		break;
	case CLK_DIV_SEL_7:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk, 7);
		else
			clk_ret = clk * 7;
		break;
	case CLK_DIV_SEL_7p5:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk * 2, 15);
		else
			clk_ret = lcd_do_div(clk * 15, 2);
		break;
	case CLK_DIV_SEL_12:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk, 12);
		else
			clk_ret = clk * 12;
		break;
	case CLK_DIV_SEL_14:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk, 14);
		else
			clk_ret = clk * 14;
		break;
	case CLK_DIV_SEL_15:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk, 15);
		else
			clk_ret = clk * 15;
		break;
	case CLK_DIV_SEL_2p5:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk * 2, 5);
		else
			clk_ret = lcd_do_div(clk * 5, 2);
		break;
	case CLK_DIV_SEL_4p67:
		if (dir == CLK_DIV_I2O)
			clk_ret = lcd_do_div(clk * 3, 14);
		else
			clk_ret = lcd_do_div(clk * 14, 3);
		break;
	default:
		clk_ret = clk;
		LCDERR("clk_div_sel: Invalid parameter\n");
		break;
	}

	return clk_ret;
}

static int lcd_pll_get_frac(struct lcd_clk_config_s *cconf, unsigned long long pll_fvco)
{
	unsigned int od_fb, frac_range, frac, offset;
	unsigned long long fvco_calc, temp;

	od_fb = cconf->pll_od_fb;
	frac_range = cconf->data->pll_frac_range;

	fvco_calc = lcd_do_div(pll_fvco, od_fb_table[od_fb]);
	temp = cconf->fin;
	temp = lcd_do_div((temp * cconf->pll_m), cconf->pll_n);
	if (fvco_calc >= temp) {
		temp = fvco_calc - temp;
		offset = 0;
	} else {
		temp = temp - fvco_calc;
		offset = 1;
	}
	if (temp >= (2 * cconf->fin)) {
		LCDERR("%s: pll changing %lldHz is too much\n", __func__, temp);
		return -1;
	}

	frac = lcd_do_div((temp * frac_range * cconf->pll_n * 10), cconf->fin) + 5;
	frac /= 10;
	if (cconf->pll_mode & LCD_PLL_MODE_FRAC_SHIFT) {
		if ((frac == (frac_range >> 1)) || (frac == (frac_range >> 2))) {
			frac |= 0x66;
			cconf->pll_frac_half_shift = 1;
		} else {
			cconf->pll_frac_half_shift = 0;
		}
	}
	cconf->pll_frac = frac | (offset << cconf->data->pll_frac_sign_bit);
	if (lcd_debug_print_flag)
		LCDPR("%s: 0x%x\n", __func__, cconf->pll_frac);

	return 0;
}

static int check_pll_gxtvbb(struct lcd_clk_config_s *cconf, unsigned long long pll_fout)
{
	struct lcd_clk_data_s *data = cconf->data;
	unsigned int m, n;
	unsigned int od1_sel, od2_sel, od3_sel, od1, od2, od3;
	unsigned long long pll_fod2_in, pll_fod3_in, pll_fvco, temp;
	unsigned int od_fb = 0, pll_frac, frac_range;
	int done;

	done = 0;
	if (pll_fout > data->pll_out_fmax || pll_fout < data->pll_out_fmin)
		return done;

	frac_range = data->pll_frac_range;
	for (od3_sel = data->pll_od_sel_max; od3_sel > 0; od3_sel--) {
		od3 = od_table[od3_sel - 1];
		pll_fod3_in = pll_fout * od3;
		for (od2_sel = od3_sel; od2_sel > 0; od2_sel--) {
			od2 = od_table[od2_sel - 1];
			pll_fod2_in = pll_fod3_in * od2;
			for (od1_sel = od2_sel; od1_sel > 0; od1_sel--) {
				od1 = od_table[od1_sel - 1];
				pll_fvco = pll_fod2_in * od1;
				if ((pll_fvco < data->pll_vco_fmin) ||
					(pll_fvco > data->pll_vco_fmax)) {
					continue;
				}
				cconf->pll_od1_sel = od1_sel - 1;
				cconf->pll_od2_sel = od2_sel - 1;
				cconf->pll_od3_sel = od3_sel - 1;
				cconf->pll_fout = pll_fout;
				if (lcd_debug_print_flag == 2) {
					LCDPR("od1=%d, od2=%d, od3=%d, pll_fvco=%lld\n",
						(od1_sel - 1), (od2_sel - 1),
						(od3_sel - 1), pll_fvco);
				}
				cconf->pll_fvco = pll_fvco;
				n = 1;
				od_fb = cconf->pll_od_fb;
				pll_fvco = lcd_do_div(pll_fvco, od_fb_table[od_fb]);
				m = lcd_do_div(pll_fvco, cconf->fin);
				temp = cconf->fin;
				temp *= m;
				temp = pll_fvco - temp;
				pll_frac = lcd_do_div((temp * frac_range * 10), cconf->fin) + 5;
				pll_frac /= 10;
				cconf->pll_m = m;
				cconf->pll_n = n;
				cconf->pll_frac = pll_frac;
				if (lcd_debug_print_flag == 2)
					LCDPR("m=%d, n=%d, frac=0x%x\n", m, n, pll_frac);
				done = 1;
				break;
			}
		}
	}
	return done;
}

static void lcd_clk_generate_gxtvbb(struct lcd_config_s *pconf)
{
	unsigned long long pll_fout, clk_div_in;
	unsigned int clk_div_out;
	unsigned int clk_div_sel, xd, tmp_clk;
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();
	int done;

	done = 0;
	cconf->fout = pconf->lcd_timing.lcd_clk;
	cconf->err_fmin = MAX_ERROR;

	if (cconf->fout > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, cconf->fout);
		goto generate_clk_done_gxtvbb;
	}

	switch (pconf->lcd_basic.lcd_type) {
	case LCD_TTL:
		clk_div_sel = CLK_DIV_SEL_1;
		cconf->xd_max = CRT_VID_DIV_MAX;
		for (xd = 1; xd <= cconf->xd_max; xd++) {
			clk_div_out = cconf->fout * xd;
			if (clk_div_out > cconf->data->div_out_fmax)
				continue;
			if (lcd_debug_print_flag == 2) {
				LCDPR("fout=%d, xd=%d, clk_div_out=%d\n",
					cconf->fout, xd, clk_div_out);
			}
			clk_div_in = clk_vid_pll_div_calc(clk_div_out,
					clk_div_sel, CLK_DIV_O2I);
			if (clk_div_in > cconf->data->div_in_fmax)
				continue;
			cconf->xd = xd;
			cconf->div_sel = clk_div_sel;
			pll_fout = clk_div_in;
			if (lcd_debug_print_flag == 2) {
				LCDPR("clk_div_sel=%s(index %d), pll_fout=%lld\n",
					lcd_clk_div_sel_table[clk_div_sel],
					clk_div_sel, pll_fout);
			}
			done = check_pll_gxtvbb(cconf, pll_fout);
			if (done)
				goto generate_clk_done_gxtvbb;
		}
		break;
	case LCD_LVDS:
		clk_div_sel = CLK_DIV_SEL_7;
		xd = 1;
		clk_div_out = cconf->fout * xd;
		if (clk_div_out > cconf->data->div_out_fmax)
			goto generate_clk_done_gxtvbb;
		if (lcd_debug_print_flag == 2) {
			LCDPR("fout=%d, xd=%d, clk_div_out=%d\n",
				cconf->fout, xd, clk_div_out);
		}
		clk_div_in = clk_vid_pll_div_calc(clk_div_out,
				clk_div_sel, CLK_DIV_O2I);
		if (clk_div_in > cconf->data->div_in_fmax)
			goto generate_clk_done_gxtvbb;
		cconf->xd = xd;
		cconf->div_sel = clk_div_sel;
		pll_fout = clk_div_in;
		if (lcd_debug_print_flag == 2) {
			LCDPR("clk_div_sel=%s(index %d), pll_fout=%lld\n",
				lcd_clk_div_sel_table[clk_div_sel],
				clk_div_sel, pll_fout);
		}
		done = check_pll_gxtvbb(cconf, pll_fout);
		if (done)
			goto generate_clk_done_gxtvbb;
		break;
	case LCD_VBYONE:
		cconf->div_sel_max = CLK_DIV_SEL_MAX;
		cconf->xd_max = CRT_VID_DIV_MAX;
		pll_fout = pconf->lcd_timing.bit_rate;
		clk_div_in = pll_fout;
		if (clk_div_in > cconf->data->div_in_fmax)
			goto generate_clk_done_gxtvbb;
		if (lcd_debug_print_flag == 2)
			LCDPR("pll_fout=%lld, clk_div_in=%lld\n", pll_fout, clk_div_in);

		for (clk_div_sel = CLK_DIV_SEL_1; clk_div_sel < CLK_DIV_SEL_MAX; clk_div_sel++) {
			clk_div_out = clk_vid_pll_div_calc(clk_div_in,
						clk_div_sel, CLK_DIV_I2O);
			if (clk_div_out > cconf->data->div_out_fmax)
				continue;
			if (lcd_debug_print_flag == 2) {
				LCDPR("clk_div_out=%d, clk_div_sel=%s(%d)\n",
					clk_div_out,
					lcd_clk_div_sel_table[clk_div_sel],
					clk_div_sel);
			}

			done = 0;
			for (xd = 1; xd <= cconf->xd_max; xd++) {
				tmp_clk = cconf->fout * xd;
				if (tmp_clk > clk_div_out)
					break;
				if (tmp_clk == clk_div_out) {
					cconf->xd = xd;
					cconf->div_sel = clk_div_sel;
					done = 1;
					if (lcd_debug_print_flag == 2)
						LCDPR("fout=%d, xd=%d\n", cconf->fout, xd);
					break;
				}
			}

			if (done)
				break;
		}
		done = check_pll_gxtvbb(cconf, pll_fout);
		break;
	default:
		break;
	}

generate_clk_done_gxtvbb:
	if (done) {
		pconf->lcd_timing.pll_ctrl =
			(cconf->pll_od1_sel << PLL_CTRL_OD1) |
			(cconf->pll_od2_sel << PLL_CTRL_OD2) |
			(cconf->pll_od3_sel << PLL_CTRL_OD3) |
			(cconf->pll_n << PLL_CTRL_N)         |
			(cconf->pll_m << PLL_CTRL_M);
		pconf->lcd_timing.div_ctrl =
			(cconf->div_sel << DIV_CTRL_DIV_SEL) |
			(cconf->xd << DIV_CTRL_XD);
		pconf->lcd_timing.clk_ctrl =
			(cconf->pll_frac << CLK_CTRL_FRAC);
	} else {
		pconf->lcd_timing.pll_ctrl =
			(1 << PLL_CTRL_OD1) |
			(1 << PLL_CTRL_OD2) |
			(1 << PLL_CTRL_OD3) |
			(1 << PLL_CTRL_N)   |
			(50 << PLL_CTRL_M);
		pconf->lcd_timing.div_ctrl =
			(CLK_DIV_SEL_1 << DIV_CTRL_DIV_SEL) |
			(7 << DIV_CTRL_XD);
		pconf->lcd_timing.clk_ctrl = (0 << CLK_CTRL_FRAC);
		LCDERR("Out of clock range, reset to default setting\n");
	}
}

static void lcd_pll_frac_generate_gxtvbb(struct lcd_config_s *pconf)
{
	unsigned long long pll_fout, pll_fvco, clk_div_in;
	unsigned int clk_div_out, clk_div_sel, enc_clk;
	unsigned int od1, od2, od3;
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();
	int ret;

	enc_clk = pconf->lcd_timing.lcd_clk;
	clk_div_sel = cconf->div_sel;
	od1 = od_table[cconf->pll_od1_sel];
	od2 = od_table[cconf->pll_od2_sel];
	od3 = od_table[cconf->pll_od3_sel];

	if (lcd_debug_print_flag == 2) {
		LCDPR("m=%d, n=%d, od1=%d, od2=%d, od3=%d, clk_div_sel=%s(index %d), xd=%d\n",
			cconf->pll_m, cconf->pll_n,
			cconf->pll_od1_sel, cconf->pll_od2_sel,
			cconf->pll_od3_sel,
			lcd_clk_div_sel_table[clk_div_sel],
			clk_div_sel, cconf->xd);
	}
	if (enc_clk > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, enc_clk);
		return;
	}
	if (lcd_debug_print_flag == 2)
		LCDPR("%s pclk=%d\n", __func__, enc_clk);

	clk_div_out = enc_clk * cconf->xd;
	if (clk_div_out > cconf->data->div_out_fmax) {
		LCDERR("%s: wrong clk_div_out value %dHz\n", __func__, clk_div_out);
		return;
	}

	clk_div_in = clk_vid_pll_div_calc(clk_div_out, clk_div_sel, CLK_DIV_O2I);
	if (clk_div_in > cconf->data->div_in_fmax) {
		LCDERR("%s: wrong clk_div_in value %lldHz\n", __func__, clk_div_in);
		return;
	}

	pll_fout = clk_div_in;
	if (pll_fout > cconf->data->pll_out_fmax || pll_fout < cconf->data->pll_out_fmin) {
		LCDERR("%s: wrong pll_fout value %lldHz\n", __func__, pll_fout);
		return;
	}
	if (lcd_debug_print_flag == 2)
		LCDPR("%s pll_fout=%lld\n", __func__, pll_fout);

	pll_fvco = pll_fout * od1 * od2 * od3;
	if (pll_fvco < cconf->data->pll_vco_fmin || pll_fvco > cconf->data->pll_vco_fmax) {
		LCDERR("%s: wrong pll_fvco value %lldHz\n", __func__, pll_fvco);
		return;
	}
	if (lcd_debug_print_flag == 2)
		LCDPR("%s pll_fvco=%lld\n", __func__, pll_fvco);

	ret = lcd_pll_get_frac(cconf, pll_fvco);
	if (ret == 0) {
		cconf->fout = enc_clk;
		cconf->pll_fout = pll_fout;
		cconf->pll_fvco = pll_fvco;
		pconf->lcd_timing.clk_ctrl &= ~(0x1ffffff);
		pconf->lcd_timing.clk_ctrl |=
			(cconf->pll_frac << CLK_CTRL_FRAC) |
			(cconf->pll_frac_half_shift << CLK_CTRL_FRAC_SHIFT);
	}
}

static int check_pll_txl(struct lcd_clk_config_s *cconf, unsigned long long pll_fout)
{
	struct lcd_clk_data_s *data = cconf->data;
	unsigned int m, n;
	unsigned int od1_sel, od2_sel, od3_sel, od1, od2, od3;
	unsigned long long pll_fod2_in, pll_fod3_in, pll_fvco, temp;
	unsigned int od_fb = 0, frac_range, pll_frac;
	int done;

	done = 0;
	if ((pll_fout > data->pll_out_fmax) ||
		(pll_fout < data->pll_out_fmin)) {
		return done;
	}
	frac_range = data->pll_frac_range;
	for (od3_sel = data->pll_od_sel_max; od3_sel > 0; od3_sel--) {
		od3 = od_table[od3_sel - 1];
		pll_fod3_in = pll_fout * od3;
		for (od2_sel = od3_sel; od2_sel > 0; od2_sel--) {
			od2 = od_table[od2_sel - 1];
			pll_fod2_in = pll_fod3_in * od2;
			for (od1_sel = od2_sel; od1_sel > 0; od1_sel--) {
				od1 = od_table[od1_sel - 1];
				pll_fvco = pll_fod2_in * od1;
				if ((pll_fvco < data->pll_vco_fmin) ||
					(pll_fvco > data->pll_vco_fmax)) {
					continue;
				}
				cconf->pll_od1_sel = od1_sel - 1;
				cconf->pll_od2_sel = od2_sel - 1;
				cconf->pll_od3_sel = od3_sel - 1;
				cconf->pll_fout = pll_fout;
				if (lcd_debug_print_flag == 2) {
					LCDPR("od1=%d, od2=%d, od3=%d, pll_fvco=%lld\n",
						(od1_sel - 1), (od2_sel - 1),
						(od3_sel - 1), pll_fvco);
				}
				cconf->pll_fvco = pll_fvco;
				n = 1;
				od_fb = cconf->pll_od_fb;
				pll_fvco = lcd_do_div(pll_fvco, od_fb_table[od_fb]);
				m = lcd_do_div(pll_fvco, cconf->fin);
				temp = cconf->fin;
				temp *= m;
				temp = pll_fvco - temp;
				pll_frac = lcd_do_div((temp * frac_range * 10), cconf->fin) + 5;
				pll_frac /= 10;
				if (cconf->pll_mode & LCD_PLL_MODE_FRAC_SHIFT) {
					if ((pll_frac == (frac_range >> 1)) ||
						(pll_frac == (frac_range >> 2))) {
						pll_frac |= 0x66;
						cconf->pll_frac_half_shift = 1;
					} else {
						cconf->pll_frac_half_shift = 0;
					}
				}
				cconf->pll_m = m;
				cconf->pll_n = n;
				cconf->pll_frac = pll_frac;
				if (lcd_debug_print_flag == 2)
					LCDPR("m=%d, n=%d, frac=0x%x\n", m, n, pll_frac);
				done = 1;
				break;
			}
		}
	}
	return done;
}

static int check_pll_vco(struct lcd_clk_config_s *cconf, unsigned long long pll_fvco)
{
	struct lcd_clk_data_s *data = cconf->data;
	unsigned int m, n;
	unsigned int od_fb = 0, pll_frac;
	unsigned long long temp;
	int done = 0;

	if ((pll_fvco < data->pll_vco_fmin) || (pll_fvco > data->pll_vco_fmax)) {
		if (lcd_debug_print_flag == 2)
			LCDPR("pll_fvco %lld is out of range\n", pll_fvco);
		return done;
	}

	cconf->pll_fvco = pll_fvco;
	n = 1;
	od_fb = cconf->pll_od_fb;
	pll_fvco = lcd_do_div(pll_fvco, od_fb_table[od_fb]);
	m = lcd_do_div(pll_fvco, cconf->fin);
	temp = cconf->fin;
	temp *= m;
	temp = pll_fvco - temp;
	pll_frac = lcd_do_div((temp * data->pll_frac_range * 10), cconf->fin) + 5;
	pll_frac /= 10;
	if (cconf->pll_mode & LCD_PLL_MODE_FRAC_SHIFT) {
		if ((pll_frac == (data->pll_frac_range >> 1)) ||
			(pll_frac == (data->pll_frac_range >> 2))) {
			pll_frac |= 0x66;
			cconf->pll_frac_half_shift = 1;
		} else {
			cconf->pll_frac_half_shift = 0;
		}
	}
	cconf->pll_m = m;
	cconf->pll_n = n;
	cconf->pll_frac = pll_frac;
	if (lcd_debug_print_flag == 2)
		LCDPR("m=%d, n=%d, frac=0x%x, pll_fvco=%lld\n", m, n, pll_frac, pll_fvco);
	done = 1;

	return done;
}

#define PLL_FVCO_ERR_MAX    2000
static int check_pll_od(struct lcd_clk_config_s *cconf, unsigned long long pll_fout)
{
	struct lcd_clk_data_s *data = cconf->data;
	unsigned int od1_sel, od2_sel, od3_sel, od1, od2, od3;
	unsigned long long pll_fod2_in, pll_fod3_in, pll_fvco;
	int done = 0;

	if ((pll_fout > data->pll_out_fmax) ||
		(pll_fout < data->pll_out_fmin)) {
		return done;
	}
	for (od3_sel = data->pll_od_sel_max; od3_sel > 0; od3_sel--) {
		od3 = od_table[od3_sel - 1];
		pll_fod3_in = pll_fout * od3;
		for (od2_sel = od3_sel; od2_sel > 0; od2_sel--) {
			od2 = od_table[od2_sel - 1];
			pll_fod2_in = pll_fod3_in * od2;
			for (od1_sel = od2_sel; od1_sel > 0; od1_sel--) {
				od1 = od_table[od1_sel - 1];
				pll_fvco = pll_fod2_in * od1;
				if ((pll_fvco < data->pll_vco_fmin) ||
					(pll_fvco > data->pll_vco_fmax)) {
					continue;
				}
				if (error_abs(pll_fvco, cconf->pll_fvco) < PLL_FVCO_ERR_MAX) {
					cconf->pll_od1_sel = od1_sel - 1;
					cconf->pll_od2_sel = od2_sel - 1;
					cconf->pll_od3_sel = od3_sel - 1;
					cconf->pll_fout = pll_fout;

					if (lcd_debug_print_flag == 2) {
						LCDPR("od1=%d, od2=%d, od3=%d\n",
							(od1_sel - 1), (od2_sel - 1),
							(od3_sel - 1));
					}
					done = 1;
					break;
				}
			}
		}
	}
	return done;
}

static void lcd_clk_generate_txl(struct lcd_config_s *pconf)
{
	unsigned long long pll_fout, pll_fvco, clk_div_in, bit_rate;
	unsigned int clk_div_out, tmp_clk;
	unsigned int clk_div_sel, xd, tcon_div_sel = 0, phy_div = 1;
	unsigned int od1, od2, od3;
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();
	int done;

	done = 0;
	cconf->fout = pconf->lcd_timing.lcd_clk;
	cconf->err_fmin = MAX_ERROR;

	if (cconf->fout > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, cconf->fout);
		goto generate_clk_done_txl;
	}

	bit_rate = pconf->lcd_timing.bit_rate;
	cconf->pll_mode = pconf->lcd_timing.clk_auto;

	switch (pconf->lcd_basic.lcd_type) {
	case LCD_TTL:
		clk_div_sel = CLK_DIV_SEL_1;
		cconf->xd_max = CRT_VID_DIV_MAX;
		for (xd = 1; xd <= cconf->xd_max; xd++) {
			clk_div_out = cconf->fout * xd;
			if (clk_div_out > cconf->data->div_out_fmax)
				continue;
			if (lcd_debug_print_flag == 2) {
				LCDPR("fout=%d, xd=%d, clk_div_out=%d\n",
					cconf->fout, xd, clk_div_out);
			}
			clk_div_in = clk_vid_pll_div_calc(clk_div_out,
					clk_div_sel, CLK_DIV_O2I);
			if (clk_div_in > cconf->data->div_in_fmax)
				continue;
			cconf->xd = xd;
			cconf->div_sel = clk_div_sel;
			pll_fout = clk_div_in;
			if (lcd_debug_print_flag == 2) {
				LCDPR("clk_div_sel=%s(index %d), pll_fout=%lld\n",
					lcd_clk_div_sel_table[clk_div_sel],
					clk_div_sel, pll_fout);
			}
			done = check_pll_txl(cconf, pll_fout);
			if (done)
				goto generate_clk_done_txl;
		}
		break;
	case LCD_LVDS:
		clk_div_sel = CLK_DIV_SEL_7;
		xd = 1;
		clk_div_out = cconf->fout * xd;
		if (clk_div_out > cconf->data->div_out_fmax)
			goto generate_clk_done_txl;
		if (lcd_debug_print_flag == 2) {
			LCDPR("fout=%d, xd=%d, clk_div_out=%d\n",
				cconf->fout, xd, clk_div_out);
		}
		clk_div_in = clk_vid_pll_div_calc(clk_div_out,
				clk_div_sel, CLK_DIV_O2I);
		if (clk_div_in > cconf->data->div_in_fmax)
			goto generate_clk_done_txl;
		cconf->xd = xd;
		cconf->div_sel = clk_div_sel;
		pll_fout = clk_div_in;
		if (lcd_debug_print_flag == 2) {
			LCDPR("clk_div_sel=%s(index %d), pll_fout=%lld\n",
				lcd_clk_div_sel_table[clk_div_sel],
				clk_div_sel, pll_fout);
		}
		done = check_pll_txl(cconf, pll_fout);
		if (done == 0)
			goto generate_clk_done_txl;
		done = 0;
		if (pconf->lcd_control.lvds_config->dual_port)
			phy_div = 2;
		else
			phy_div = 1;
		od1 = od_table[cconf->pll_od1_sel];
		od2 = od_table[cconf->pll_od2_sel];
		od3 = od_table[cconf->pll_od3_sel];
		for (tcon_div_sel = 0; tcon_div_sel < 5; tcon_div_sel++) {
			if (tcon_div_table[tcon_div_sel] == phy_div * od1 * od2 * od3) {
				cconf->pll_tcon_div_sel = tcon_div_sel;
				done = 1;
				break;
			}
		}
		break;
	case LCD_VBYONE:
		cconf->div_sel_max = CLK_DIV_SEL_MAX;
		cconf->xd_max = CRT_VID_DIV_MAX;
		pll_fout = bit_rate;
		clk_div_in = pll_fout;
		if (clk_div_in > cconf->data->div_in_fmax)
			goto generate_clk_done_txl;
		if (lcd_debug_print_flag == 2)
			LCDPR("pll_fout=%lld, clk_div_in=%lld\n", pll_fout, clk_div_in);

		for (clk_div_sel = CLK_DIV_SEL_1; clk_div_sel < CLK_DIV_SEL_MAX; clk_div_sel++) {
			clk_div_out = clk_vid_pll_div_calc(clk_div_in,
						clk_div_sel, CLK_DIV_I2O);
			if (clk_div_out > cconf->data->div_out_fmax)
				continue;
			if (lcd_debug_print_flag == 2) {
				LCDPR("clk_div_out=%d, clk_div_sel=%s(%d)\n",
					clk_div_out,
					lcd_clk_div_sel_table[clk_div_sel],
					clk_div_sel);
			}

			done = 0;
			for (xd = 1; xd <= cconf->xd_max; xd++) {
				tmp_clk = cconf->fout * xd;
				if (tmp_clk > clk_div_out)
					break;
				if (tmp_clk == clk_div_out) {
					cconf->xd = xd;
					cconf->div_sel = clk_div_sel;
					done = 1;
					if (lcd_debug_print_flag == 2)
						LCDPR("fout=%d, xd=%d\n", cconf->fout, xd);
					break;
				}
			}

			if (done)
				break;
		}

		done = check_pll_txl(cconf, pll_fout);
		if (done == 0)
			goto generate_clk_done_txl;
		done = 0;
		od1 = od_table[cconf->pll_od1_sel];
		od2 = od_table[cconf->pll_od2_sel];
		od3 = od_table[cconf->pll_od3_sel];
		for (tcon_div_sel = 0; tcon_div_sel < 5; tcon_div_sel++) {
			if (tcon_div_table[tcon_div_sel] == od1 * od2 * od3) {
				cconf->pll_tcon_div_sel = tcon_div_sel;
				done = 1;
				break;
			}
		}
		break;
	case LCD_MLVDS:
		/* must go through div4 for clk phase */
		for (tcon_div_sel = 3; tcon_div_sel < 5; tcon_div_sel++) {
			pll_fvco = bit_rate * tcon_div_table[tcon_div_sel];
			done = check_pll_vco(cconf, pll_fvco);
			if (done == 0)
				continue;
			cconf->xd_max = CRT_VID_DIV_MAX;
			for (xd = 1; xd <= cconf->xd_max; xd++) {
				clk_div_out = cconf->fout * xd;
				if (clk_div_out > cconf->data->div_out_fmax)
					continue;
				if (lcd_debug_print_flag == 2) {
					LCDPR("fout=%d, xd=%d, clk_div_out=%d\n",
						cconf->fout, xd, clk_div_out);
				}
				for (clk_div_sel = CLK_DIV_SEL_1;
					clk_div_sel < CLK_DIV_SEL_MAX;
					clk_div_sel++) {
					clk_div_in = clk_vid_pll_div_calc(
						clk_div_out, clk_div_sel, CLK_DIV_O2I);
					if (clk_div_in > cconf->data->div_in_fmax)
						continue;
					cconf->xd = xd;
					cconf->div_sel = clk_div_sel;
					cconf->pll_tcon_div_sel = tcon_div_sel;
					pll_fout = clk_div_in;
					if (lcd_debug_print_flag == 2) {
						LCDPR("clk_div_sel=%s(%d)\n",
							lcd_clk_div_sel_table[clk_div_sel],
							clk_div_sel);
						LCDPR("pll_fout=%lld, tcon_div_sel=%d\n",
							pll_fout, tcon_div_sel);
					}
					done = check_pll_od(cconf, pll_fout);
					if (done)
						goto generate_clk_done_txl;
				}
			}
		}
		break;
	case LCD_P2P:
		for (tcon_div_sel = 0; tcon_div_sel < 5; tcon_div_sel++) {
			pll_fvco = bit_rate * tcon_div_table[tcon_div_sel];
			done = check_pll_vco(cconf, pll_fvco);
			if (done == 0)
				continue;
			cconf->xd_max = CRT_VID_DIV_MAX;
			for (xd = 1; xd <= cconf->xd_max; xd++) {
				clk_div_out = cconf->fout * xd;
				if (clk_div_out > cconf->data->div_out_fmax)
					continue;
				if (lcd_debug_print_flag == 2) {
					LCDPR("fout=%d, xd=%d, clk_div_out=%d\n",
						cconf->fout, xd, clk_div_out);
				}
				for (clk_div_sel = CLK_DIV_SEL_1;
					clk_div_sel < CLK_DIV_SEL_MAX;
					clk_div_sel++) {
					clk_div_in = clk_vid_pll_div_calc(
						clk_div_out, clk_div_sel, CLK_DIV_O2I);
					if (clk_div_in > cconf->data->div_in_fmax)
						continue;
					cconf->xd = xd;
					cconf->div_sel = clk_div_sel;
					cconf->pll_tcon_div_sel = tcon_div_sel;
					pll_fout = clk_div_in;
					if (lcd_debug_print_flag == 2) {
						LCDPR("clk_div_sel=%s(%d)\n",
							lcd_clk_div_sel_table[clk_div_sel],
							clk_div_sel);
						LCDPR("pll_fout=%lld, tcon_div_sel=%d\n",
							pll_fout, tcon_div_sel);
					}
					done = check_pll_od(cconf, pll_fout);
					if (done)
						goto generate_clk_done_txl;
				}
			}
		}
		if (done)
			goto generate_clk_done_txl;
		break;
	default:
		break;
	}

generate_clk_done_txl:
	if (done) {
		pconf->lcd_timing.pll_ctrl =
			(cconf->pll_od1_sel << PLL_CTRL_OD1) |
			(cconf->pll_od2_sel << PLL_CTRL_OD2) |
			(cconf->pll_od3_sel << PLL_CTRL_OD3) |
			(cconf->pll_n << PLL_CTRL_N)         |
			(cconf->pll_m << PLL_CTRL_M);
		pconf->lcd_timing.div_ctrl =
			(cconf->div_sel << DIV_CTRL_DIV_SEL) |
			(cconf->xd << DIV_CTRL_XD);
		pconf->lcd_timing.clk_ctrl =
			(cconf->pll_frac << CLK_CTRL_FRAC) |
			(cconf->pll_frac_half_shift << CLK_CTRL_FRAC_SHIFT);
	} else {
		pconf->lcd_timing.pll_ctrl =
			(1 << PLL_CTRL_OD1) |
			(1 << PLL_CTRL_OD2) |
			(1 << PLL_CTRL_OD3) |
			(1 << PLL_CTRL_N)   |
			(50 << PLL_CTRL_M);
		pconf->lcd_timing.div_ctrl =
			(CLK_DIV_SEL_1 << DIV_CTRL_DIV_SEL) |
			(7 << DIV_CTRL_XD);
		pconf->lcd_timing.clk_ctrl = (0 << CLK_CTRL_FRAC);
		LCDERR("Out of clock range, reset to default setting\n");
	}
}

static void lcd_pll_frac_generate_txl(struct lcd_config_s *pconf)
{
	unsigned long long pll_fout, pll_fvco, clk_div_in;
	unsigned int clk_div_out, clk_div_sel, enc_clk;
	unsigned int od1, od2, od3;
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();
	int ret;

	enc_clk = pconf->lcd_timing.lcd_clk;
	clk_div_sel = cconf->div_sel;
	od1 = od_table[cconf->pll_od1_sel];
	od2 = od_table[cconf->pll_od2_sel];
	od3 = od_table[cconf->pll_od3_sel];

	if (lcd_debug_print_flag == 2) {
		LCDPR("m=%d, n=%d, od1=%d, od2=%d, od3=%d, clk_div_sel=%s(index %d), xd=%d\n",
			cconf->pll_m, cconf->pll_n,
			cconf->pll_od1_sel, cconf->pll_od2_sel,
			cconf->pll_od3_sel,
			lcd_clk_div_sel_table[clk_div_sel],
			clk_div_sel, cconf->xd);
	}
	if (enc_clk > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, enc_clk);
		return;
	}
	if (lcd_debug_print_flag == 2)
		LCDPR("%s pclk=%d\n", __func__, enc_clk);

	clk_div_out = enc_clk * cconf->xd;
	if (clk_div_out > cconf->data->div_out_fmax) {
		LCDERR("%s: wrong clk_div_out value %dHz\n", __func__, clk_div_out);
		return;
	}

	clk_div_in = clk_vid_pll_div_calc(clk_div_out, clk_div_sel, CLK_DIV_O2I);
	if (clk_div_in > cconf->data->div_in_fmax) {
		LCDERR("%s: wrong clk_div_in value %lldHz\n", __func__, clk_div_in);
		return;
	}

	pll_fout = clk_div_in;
	if (pll_fout > cconf->data->pll_out_fmax || pll_fout < cconf->data->pll_out_fmin) {
		LCDERR("%s: wrong pll_fout value %lldHz\n", __func__, pll_fout);
		return;
	}
	if (lcd_debug_print_flag == 2)
		LCDPR("%s pll_fout=%lld\n", __func__, pll_fout);

	pll_fvco = pll_fout * od1 * od2 * od3;
	if (pll_fvco < cconf->data->pll_vco_fmin || pll_fvco > cconf->data->pll_vco_fmax) {
		LCDERR("%s: wrong pll_fvco value %lldHz\n", __func__, pll_fvco);
		return;
	}
	if (lcd_debug_print_flag == 2)
		LCDPR("%s pll_fvco=%lld\n", __func__, pll_fvco);

	ret = lcd_pll_get_frac(cconf, pll_fvco);
	if (ret == 0) {
		cconf->fout = enc_clk;
		cconf->pll_fout = pll_fout;
		cconf->pll_fvco = pll_fvco;
		pconf->lcd_timing.clk_ctrl &= ~(0x1ffffff);
		pconf->lcd_timing.clk_ctrl |=
			(cconf->pll_frac << CLK_CTRL_FRAC) |
			(cconf->pll_frac_half_shift << CLK_CTRL_FRAC_SHIFT);
	}
}

static void lcd_clk_generate_txhd(struct lcd_config_s *pconf)
{
	unsigned long long pll_fout, pll_fvco, bit_rate;
	unsigned int clk_div_in, clk_div_out;
	unsigned int clk_div_sel, xd, tcon_div_sel = 0, phy_div = 1;
	unsigned int od1, od2, od3;
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();
	int done = 0;

	cconf->fout = pconf->lcd_timing.lcd_clk;
	cconf->err_fmin = MAX_ERROR;

	if (cconf->fout > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, cconf->fout);
		goto generate_clk_done_txhd;
	}

	switch (pconf->lcd_basic.lcd_type) {
	case LCD_LVDS:
		clk_div_sel = CLK_DIV_SEL_7;
		xd = 1;
		clk_div_out = cconf->fout * xd;
		if (clk_div_out > cconf->data->div_out_fmax)
			goto generate_clk_done_txhd;
		if (lcd_debug_print_flag == 2) {
			LCDPR("fout=%d, xd=%d, clk_div_out=%d\n",
				cconf->fout, xd, clk_div_out);
		}
		clk_div_in = clk_vid_pll_div_calc(clk_div_out,
				clk_div_sel, CLK_DIV_O2I);
		if (clk_div_in > cconf->data->div_in_fmax)
			goto generate_clk_done_txhd;
		cconf->xd = xd;
		cconf->div_sel = clk_div_sel;
		pll_fout = clk_div_in;
		if (lcd_debug_print_flag == 2) {
			LCDPR("clk_div_sel=%s(index %d), pll_fout=%lld\n",
				lcd_clk_div_sel_table[clk_div_sel],
				clk_div_sel, pll_fout);
		}
		done = check_pll_txl(cconf, pll_fout);
		if (done == 0)
			goto generate_clk_done_txhd;
		done = 0;
		if (pconf->lcd_control.lvds_config->dual_port)
			phy_div = 2;
		else
			phy_div = 1;
		od1 = od_table[cconf->pll_od1_sel];
		od2 = od_table[cconf->pll_od2_sel];
		od3 = od_table[cconf->pll_od3_sel];
		for (tcon_div_sel = 0; tcon_div_sel < 2; tcon_div_sel++) {
			if (tcon_div_table_txhd[tcon_div_sel] == phy_div * od1 * od2 * od3) {
				cconf->pll_tcon_div_sel = tcon_div_sel;
				done = 1;
				break;
			}
		}
		break;
	case LCD_MLVDS:
		bit_rate = pconf->lcd_timing.bit_rate;
		/* must go through div4 for clk phase */
		for (tcon_div_sel = 0; tcon_div_sel < 2; tcon_div_sel++) {
			pll_fvco = bit_rate * tcon_div_table_txhd[tcon_div_sel];
			done = check_pll_vco(cconf, pll_fvco);
			if (done) {
				clk_div_sel = CLK_DIV_SEL_1;
				cconf->xd_max = CRT_VID_DIV_MAX;
				for (xd = 1; xd <= cconf->xd_max; xd++) {
					clk_div_out = cconf->fout * xd;
					if (clk_div_out > cconf->data->div_out_fmax)
						continue;
					if (lcd_debug_print_flag == 2) {
						LCDPR("fout=%d, xd=%d, clk_div_out=%d\n",
							cconf->fout, xd, clk_div_out);
					}
					clk_div_in = clk_vid_pll_div_calc(clk_div_out,
							clk_div_sel, CLK_DIV_O2I);
					if (clk_div_in > cconf->data->div_in_fmax)
						continue;
					cconf->xd = xd;
					cconf->div_sel = clk_div_sel;
					cconf->pll_tcon_div_sel = tcon_div_sel;
					pll_fout = clk_div_in;
					if (lcd_debug_print_flag == 2) {
						LCDPR("clk_div_sel=%s(index %d), pll_fout=%lld\n",
							lcd_clk_div_sel_table[clk_div_sel],
							clk_div_sel, pll_fout);
						LCDPR("tcon_div_sel=%d\n", tcon_div_sel);
					}
					done = check_pll_od(cconf, pll_fout);
					if (done)
						goto generate_clk_done_txhd;
				}
			}
		}
		break;
	default:
		break;
	}

generate_clk_done_txhd:
	if (done) {
		pconf->lcd_timing.pll_ctrl =
			(cconf->pll_od1_sel << PLL_CTRL_OD1) |
			(cconf->pll_od2_sel << PLL_CTRL_OD2) |
			(cconf->pll_od3_sel << PLL_CTRL_OD3) |
			(cconf->pll_n << PLL_CTRL_N)         |
			(cconf->pll_m << PLL_CTRL_M);
		pconf->lcd_timing.div_ctrl =
			(cconf->div_sel << DIV_CTRL_DIV_SEL) |
			(cconf->xd << DIV_CTRL_XD);
		pconf->lcd_timing.clk_ctrl =
			(cconf->pll_frac << CLK_CTRL_FRAC);
	} else {
		pconf->lcd_timing.pll_ctrl =
			(1 << PLL_CTRL_OD1) |
			(1 << PLL_CTRL_OD2) |
			(1 << PLL_CTRL_OD3) |
			(1 << PLL_CTRL_N)   |
			(50 << PLL_CTRL_M);
		pconf->lcd_timing.div_ctrl =
			(CLK_DIV_SEL_1 << DIV_CTRL_DIV_SEL) |
			(7 << DIV_CTRL_XD);
		pconf->lcd_timing.clk_ctrl = (0 << CLK_CTRL_FRAC);
		LCDERR("Out of clock range, reset to default setting\n");
	}
}

static int check_pll_axg(struct lcd_clk_config_s *cconf, unsigned long long pll_fout)
{
	struct lcd_clk_data_s *data = cconf->data;
	unsigned int m, n, od_sel, od;
	unsigned long long pll_fvco, temp;
	unsigned int od_fb = 0, pll_frac;
	int done = 0;

	if ((pll_fout > data->pll_out_fmax) ||
		(pll_fout < data->pll_out_fmin)) {
		return done;
	}
	for (od_sel = data->pll_od_sel_max; od_sel > 0; od_sel--) {
		od = od_table[od_sel - 1];
		pll_fvco = pll_fout * od;
		if ((pll_fvco < data->pll_vco_fmin) ||
			(pll_fvco > data->pll_vco_fmax)) {
			continue;
		}
		cconf->pll_od1_sel = od_sel - 1;
		cconf->pll_fout = pll_fout;
		if (lcd_debug_print_flag == 2)
			LCDPR("od_sel=%d, pll_fvco=%lld\n", (od_sel - 1), pll_fvco);

		cconf->pll_fvco = pll_fvco;
		n = 1;
		od_fb = cconf->pll_od_fb;
		pll_fvco = lcd_do_div(pll_fvco, od_fb_table[od_fb]);
		m = lcd_do_div(pll_fvco, cconf->fin);
		temp = cconf->fin;
		temp *= m;
		temp = pll_fvco - temp;
		pll_frac = lcd_do_div((temp * data->pll_frac_range * 10), cconf->fin) + 5;
		pll_frac /= 10;
		cconf->pll_m = m;
		cconf->pll_n = n;
		cconf->pll_frac = pll_frac;
		if (lcd_debug_print_flag == 2)
			LCDPR("pll_m=%d, pll_n=%d, pll_frac=0x%x\n", m, n, pll_frac);
		done = 1;
		break;
	}
	return done;
}

static void lcd_clk_generate_axg(struct lcd_config_s *pconf)
{
	unsigned long long pll_fout;
	unsigned long long dsi_bit_rate_max = 0, dsi_bit_rate_min = 0;
	unsigned int xd;
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();
	int done;

	done = 0;
	cconf->fout = pconf->lcd_timing.lcd_clk;
	cconf->err_fmin = MAX_ERROR;

	if (cconf->fout > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, cconf->fout);
		goto generate_clk_done_axg;
	}

	switch (pconf->lcd_basic.lcd_type) {
	case LCD_MIPI:
		cconf->xd_max = CRT_VID_DIV_MAX;
		dsi_bit_rate_max = pconf->lcd_control.mipi_config->bit_rate_max;
		dsi_bit_rate_max = dsi_bit_rate_max * 1000000;
		dsi_bit_rate_min = dsi_bit_rate_max - cconf->fout;

		for (xd = 1; xd <= cconf->xd_max; xd++) {
			pll_fout = cconf->fout;
			pll_fout = pll_fout * xd;
			if ((pll_fout > dsi_bit_rate_max) ||
				(pll_fout < dsi_bit_rate_min)) {
				continue;
			}
			if (lcd_debug_print_flag == 2)
				LCDPR("fout=%d, xd=%d\n", cconf->fout, xd);

			cconf->phy_clk = pll_fout;
			pconf->lcd_timing.bit_rate = cconf->phy_clk;
			pconf->lcd_control.mipi_config->clk_factor = xd;
			cconf->xd = xd;
			done = check_pll_axg(cconf, pll_fout);
			if (done)
				goto generate_clk_done_axg;
		}
		break;
	default:
		break;
	}

generate_clk_done_axg:
	if (done) {
		pconf->lcd_timing.pll_ctrl =
			(cconf->pll_od1_sel << PLL_CTRL_OD1) |
			(cconf->pll_n << PLL_CTRL_N) |
			(cconf->pll_m << PLL_CTRL_M);
		pconf->lcd_timing.div_ctrl =
			(CLK_DIV_SEL_1 << DIV_CTRL_DIV_SEL) |
			(cconf->xd << DIV_CTRL_XD);
		pconf->lcd_timing.clk_ctrl = (cconf->pll_frac << CLK_CTRL_FRAC);
	} else {
		pconf->lcd_timing.pll_ctrl =
			(1 << PLL_CTRL_OD1) |
			(1 << PLL_CTRL_N)   |
			(50 << PLL_CTRL_M);
		pconf->lcd_timing.div_ctrl =
			(CLK_DIV_SEL_1 << DIV_CTRL_DIV_SEL) |
			(7 << DIV_CTRL_XD);
		pconf->lcd_timing.clk_ctrl = (0 << CLK_CTRL_FRAC);
		LCDERR("Out of clock range, reset to default setting!\n");
	}
}

static void lcd_pll_frac_generate_axg(struct lcd_config_s *pconf)
{
	unsigned long long pll_fout, pll_fvco;
	unsigned int od, enc_clk;
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();
	int ret;

	enc_clk = pconf->lcd_timing.lcd_clk;
	od = od_table[cconf->pll_od1_sel];

	if (lcd_debug_print_flag == 2) {
		LCDPR("m=%d, n=%d, od=%d, xd=%d\n",
			cconf->pll_m, cconf->pll_n, cconf->pll_od1_sel, cconf->xd);
	}
	if (enc_clk > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, enc_clk);
		return;
	}
	if (lcd_debug_print_flag == 2)
		LCDPR("%s pclk=%d\n", __func__, enc_clk);

	pll_fout = enc_clk;
	pll_fout = pll_fout * cconf->xd;
	if (pll_fout > cconf->data->pll_out_fmax || pll_fout < cconf->data->pll_out_fmin) {
		LCDERR("%s: wrong pll_fout value %lldHz\n", __func__, pll_fout);
		return;
	}
	if (lcd_debug_print_flag == 2)
		LCDPR("%s pll_fout=%lld\n", __func__, pll_fout);

	pll_fvco = pll_fout * od;
	if (pll_fvco < cconf->data->pll_vco_fmin || pll_fvco > cconf->data->pll_vco_fmax) {
		LCDERR("%s: wrong pll_fvco value %lldHz\n", __func__, pll_fvco);
		return;
	}
	if (lcd_debug_print_flag == 2)
		LCDPR("%s pll_fvco=%lld\n", __func__, pll_fvco);

	ret = lcd_pll_get_frac(cconf, pll_fvco);
	if (ret == 0) {
		cconf->fout = enc_clk;
		cconf->pll_fout = pll_fout;
		cconf->pll_fvco = pll_fvco;
		pconf->lcd_timing.clk_ctrl &= ~(0x1ffffff);
		pconf->lcd_timing.clk_ctrl |=
			(cconf->pll_frac << CLK_CTRL_FRAC) |
			(cconf->pll_frac_half_shift << CLK_CTRL_FRAC_SHIFT);
	}
}

static void lcd_clk_generate_hpll_g12a(struct lcd_config_s *pconf)
{
	unsigned long long pll_fout;
	unsigned int clk_div_sel, xd;
	unsigned long long dsi_bit_rate_max = 0, dsi_bit_rate_min = 0;
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();
	int done;

	done = 0;
	cconf->fout = pconf->lcd_timing.lcd_clk;
	cconf->err_fmin = MAX_ERROR;

	if (cconf->fout > cconf->data->xd_out_fmax) {
		LCDERR("%s: wrong lcd_clk value %dHz\n", __func__, cconf->fout);
		goto generate_clk_done_g12a;
	}

	switch (pconf->lcd_basic.lcd_type) {
	case LCD_MIPI:
		cconf->xd_max = CRT_VID_DIV_MAX;
		dsi_bit_rate_max = pconf->lcd_control.mipi_config->bit_rate_max;
		dsi_bit_rate_max = dsi_bit_rate_max * 1000000;
		dsi_bit_rate_min = dsi_bit_rate_max - cconf->fout;

		clk_div_sel = CLK_DIV_SEL_1;
		for (xd = 1; xd <= cconf->xd_max; xd++) {
			pll_fout = cconf->fout;
			pll_fout = pll_fout * xd;
			if ((pll_fout > dsi_bit_rate_max) ||
				(pll_fout < dsi_bit_rate_min)) {
				continue;
			}
			if (lcd_debug_print_flag == 2)
				LCDPR("fout=%d, xd=%d\n", cconf->fout, xd);

			cconf->phy_clk = pll_fout;
			pconf->lcd_timing.bit_rate = cconf->phy_clk;
			pconf->lcd_control.mipi_config->clk_factor = xd;
			cconf->xd = xd;
			cconf->div_sel = clk_div_sel;
			done = check_pll_txl(cconf, pll_fout);
			if (done)
				goto generate_clk_done_g12a;
		}
		break;
	default:
		break;
	}

generate_clk_done_g12a:
	if (done) {
		pconf->lcd_timing.pll_ctrl =
			(cconf->pll_od1_sel << PLL_CTRL_OD1) |
			(cconf->pll_od2_sel << PLL_CTRL_OD2) |
			(cconf->pll_od3_sel << PLL_CTRL_OD3) |
			(cconf->pll_n << PLL_CTRL_N)         |
			(cconf->pll_m << PLL_CTRL_M);
		pconf->lcd_timing.div_ctrl =
			(cconf->div_sel << DIV_CTRL_DIV_SEL) |
			(cconf->xd << DIV_CTRL_XD);
		pconf->lcd_timing.clk_ctrl = (cconf->pll_frac << CLK_CTRL_FRAC);
	} else {
		pconf->lcd_timing.pll_ctrl =
			(1 << PLL_CTRL_OD1) |
			(1 << PLL_CTRL_OD2) |
			(1 << PLL_CTRL_OD3) |
			(1 << PLL_CTRL_N)   |
			(50 << PLL_CTRL_M);
		pconf->lcd_timing.div_ctrl =
			(CLK_DIV_SEL_1 << DIV_CTRL_DIV_SEL) |
			(7 << DIV_CTRL_XD);
		pconf->lcd_timing.clk_ctrl = (0 << CLK_CTRL_FRAC);
		LCDERR("Out of clock range, reset to default setting\n");
	}
}

/* ****************************************************
 * lcd clk match function
 * ****************************************************
 */
static void lcd_clk_set_gxtvbb(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	lcd_set_pll_gxtvbb(&clk_conf);
	lcd_set_vid_pll_div(&clk_conf);
}

static void lcd_clk_set_txl(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	lcd_set_pll_txl(&clk_conf);
	lcd_set_vid_pll_div(&clk_conf);
}

static void lcd_clk_set_txlx(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	lcd_set_pll_txlx(&clk_conf);
	lcd_set_vid_pll_div(&clk_conf);
}

static void lcd_clk_set_axg(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	lcd_set_pll_axg(&clk_conf);
	lcd_set_dsi_meas_clk();
}

static void lcd_clk_set_txhd(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	lcd_set_tcon_clk(pconf);
	lcd_set_pll_txhd(&clk_conf);
	lcd_set_vid_pll_div(&clk_conf);
}

static void lcd_clk_set_g12a_path0(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	lcd_set_hpll_g12a(&clk_conf);
	lcd_set_vid_pll_div(&clk_conf);
	lcd_set_dsi_meas_clk();
	lcd_set_dsi_phy_clk(0);
}

static void lcd_clk_set_g12a_path1(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	lcd_set_gp0_pll_g12a(&clk_conf);
	lcd_set_dsi_meas_clk();
	lcd_set_dsi_phy_clk(1);
}

static void lcd_clk_set_g12b_path0(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	lcd_set_hpll_g12b(&clk_conf);
	lcd_set_vid_pll_div(&clk_conf);
	lcd_set_dsi_meas_clk();
	lcd_set_dsi_phy_clk(0);
}

static void lcd_clk_set_g12b_path1(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	lcd_set_gp0_pll_g12b(&clk_conf);
	lcd_set_dsi_meas_clk();
	lcd_set_dsi_phy_clk(1);
}

static void lcd_clk_set_tl1(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	lcd_set_tcon_clk_tl1(pconf);
	lcd_set_pll_tl1(&clk_conf);
	lcd_set_vid_pll_div(&clk_conf);
}

static void lcd_clk_set_t5(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);
	lcd_set_tcon_clk_t5(pconf);
	lcd_set_pll_tl1(&clk_conf);
	lcd_set_vid_pll_div(&clk_conf);
}

static void lcd_clk_set_t5w(struct lcd_config_s *pconf)
{
	lcd_hiu_setb(HHI_VIID_CLK0_CTRL, 0, VCLK2_EN, 1);
	lcd_set_tcon_clk_t5(pconf);
	lcd_set_pll_t5w(&clk_conf);
	lcd_set_vid_pll_div(&clk_conf);
}

static void lcd_clk_config_init_print_dft(void)
{
	struct lcd_clk_data_s *data = clk_conf.data;

	LCDPR("lcd clk config:\n"
		"pll_m_max:           %d\n"
		"pll_m_min:           %d\n"
		"pll_n_max:           %d\n"
		"pll_n_min:           %d\n"
		"pll_od_fb:           %d\n"
		"pll_frac_range:      %d\n"
		"pll_od_sel_max:      %d\n"
		"pll_ref_fmax:        %d\n"
		"pll_ref_fmin:        %d\n"
		"pll_vco_fmax:        %lld\n"
		"pll_vco_fmin:        %lld\n"
		"pll_out_fmax:        %lld\n"
		"pll_out_fmin:        %lld\n"
		"div_in_fmax:         %lld\n"
		"div_out_fmax:        %d\n"
		"xd_out_fmax:         %d\n"
		"ss_level_max:        %d\n"
		"ss_dep_base:         %d\n"
		"ss_dep_sel_max:      %d\n"
		"ss_str_m_max:        %d\n"
		"ss_freq_max:         %d\n"
		"ss_mode_max:         %d\n\n",
		data->pll_m_max, data->pll_m_min,
		data->pll_n_max, data->pll_n_min,
		data->pll_od_fb, data->pll_frac_range,
		data->pll_od_sel_max,
		data->pll_ref_fmax, data->pll_ref_fmin,
		data->pll_vco_fmax, data->pll_vco_fmin,
		data->pll_out_fmax, data->pll_out_fmin,
		data->div_in_fmax, data->div_out_fmax,
		data->xd_out_fmax, data->ss_level_max,
		data->ss_dep_base, data->ss_dep_sel_max,
		data->ss_str_m_max,
		data->ss_freq_max, data->ss_mode_max);
}

static void lcd_clk_config_init_print_axg(void)
{
	struct lcd_clk_data_s *data = clk_conf.data;

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

static void lcd_clk_config_print_dft(void)
{
	LCDPR("lcd clk config:\n"
		"pll_mode:             %d\n"
		"pll_m:                %d\n"
		"pll_n:                %d\n"
		"pll_frac:             0x%03x\n"
		"pll_frac_half_shift:  %d\n"
		"pll_fvco:             %lldHz\n"
		"pll_od1:              %d\n"
		"pll_od2:              %d\n"
		"pll_od3:              %d\n"
		"pll_tcon_div_sel:     %d\n"
		"pll_out:              %lldHz\n"
		"div_sel:              %s(index %d)\n"
		"xd:                   %d\n"
		"fout:                 %dHz\n\n",
		clk_conf.pll_mode, clk_conf.pll_m, clk_conf.pll_n,
		clk_conf.pll_frac, clk_conf.pll_frac_half_shift,
		clk_conf.pll_fvco,
		clk_conf.pll_od1_sel, clk_conf.pll_od2_sel,
		clk_conf.pll_od3_sel, clk_conf.pll_tcon_div_sel,
		clk_conf.pll_fout,
		lcd_clk_div_sel_table[clk_conf.div_sel],
		clk_conf.div_sel, clk_conf.xd,
		clk_conf.fout);
	if (clk_conf.data && clk_conf.data->ss_support) {
		printf("ss_level:   %d\n"
			"ss_dep_sel: %d\n"
			"ss_str_m:   %d\n"
			"ss_ppm:     %d\n"
			"ss_freq:    %d\n"
			"ss_mode:    %d\n"
			"ss_en:      %d\n\n",
			clk_conf.ss_level, clk_conf.ss_dep_sel,
			clk_conf.ss_str_m, clk_conf.ss_ppm,
			clk_conf.ss_freq, clk_conf.ss_mode, clk_conf.ss_en);
	}
}

static void lcd_clk_config_print_axg(void)
{
	LCDPR("lcd clk config:\n"
		"pll_m:        %d\n"
		"pll_n:        %d\n"
		"pll_frac:     0x%03x\n"
		"pll_fvco:     %lldHz\n"
		"pll_od:       %d\n"
		"pll_out:      %lldHz\n"
		"xd:           %d\n"
		"fout:         %dHz\n\n",
		clk_conf.pll_m, clk_conf.pll_n,
		clk_conf.pll_frac, clk_conf.pll_fvco,
		clk_conf.pll_od1_sel, clk_conf.pll_fout,
		clk_conf.xd, clk_conf.fout);
}

static void lcd_clk_config_print_g12a(void)
{
	if (clk_conf.data->vclk_sel) {
		LCDPR("lcd clk config:\n"
			"vclk_sel      %d\n"
			"pll_m:        %d\n"
			"pll_n:        %d\n"
			"pll_frac:     0x%03x\n"
			"pll_fvco:     %lldHz\n"
			"pll_od:       %d\n"
			"pll_out:      %lldHz\n"
			"xd:           %d\n"
			"fout:         %dHz\n\n",
			clk_conf.data->vclk_sel,
			clk_conf.pll_m, clk_conf.pll_n,
			clk_conf.pll_frac, clk_conf.pll_fvco,
			clk_conf.pll_od1_sel, clk_conf.pll_fout,
			clk_conf.xd, clk_conf.fout);
	} else {
		LCDPR("lcd clk config:\n"
			"vclk_sel        %d\n"
			"pll_m:          %d\n"
			"pll_n:          %d\n"
			"pll_frac:       0x%03x\n"
			"pll_fvco:       %lldHz\n"
			"pll_od1:        %d\n"
			"pll_od2:        %d\n"
			"pll_od3:        %d\n"
			"pll_out:        %lldHz\n"
			"div_sel:        %s(index %d)\n"
			"xd:             %d\n"
			"fout:           %dHz\n\n",
			clk_conf.data->vclk_sel,
			clk_conf.pll_m, clk_conf.pll_n,
			clk_conf.pll_frac, clk_conf.pll_fvco,
			clk_conf.pll_od1_sel, clk_conf.pll_od2_sel,
			clk_conf.pll_od3_sel, clk_conf.pll_fout,
			lcd_clk_div_sel_table[clk_conf.div_sel],
			clk_conf.div_sel, clk_conf.xd,
			clk_conf.fout);
	}
}

/* ****************************************************
 * lcd clk function api
 * ****************************************************
 */
void lcd_clk_generate_parameter(struct lcd_config_s *pconf)
{
	unsigned int ss_level;
	unsigned int ss_freq;
	unsigned int ss_mode;

	if (clk_conf.data == NULL) {
		LCDERR("%s: clk config data is null\n", __func__);
		return;
	}

	if (clk_conf.data->clk_generate_parameter)
		clk_conf.data->clk_generate_parameter(pconf);

	ss_level = pconf->lcd_timing.ss_level & 0xff;
	clk_conf.ss_level = (ss_level >= clk_conf.data->ss_level_max) ?
				0 : ss_level;
	ss_freq = (pconf->lcd_timing.ss_level >> 8) & 0xf;
	clk_conf.ss_freq = (ss_freq >= clk_conf.data->ss_freq_max) ?
				0 : ss_freq;
	ss_mode = (pconf->lcd_timing.ss_level >> 12) & 0xf;
	clk_conf.ss_mode = (ss_mode >= clk_conf.data->ss_mode_max) ?
				0 : ss_mode;
}

void lcd_get_ss(void)
{
	unsigned int temp;

	if (clk_conf.data == NULL) {
		printf("lcd clk config data is null\n");
		return;
	}
	if (clk_conf.data->ss_support == 0 || clk_conf.data->ss_level_max == 0) {
		printf("lcd spread spectrum is not support\n");
		return;
	}

	if (clk_conf.data->ss_support == 1) { //fix table
		temp = (clk_conf.ss_level >= clk_conf.data->ss_level_max) ?
			0 : clk_conf.ss_level;
		if (clk_conf.data->ss_level_table) {
			printf("ss_level: %s\n",
				clk_conf.data->ss_level_table[temp]);
		}
		temp = (clk_conf.ss_freq >= clk_conf.data->ss_freq_max) ?
			0 : clk_conf.ss_freq;
		if (clk_conf.data->ss_freq_table) {
			printf("ss_freq: %s\n",
				clk_conf.data->ss_freq_table[temp]);
		}
		temp = (clk_conf.ss_mode >= clk_conf.data->ss_mode_max) ?
			0 : clk_conf.ss_mode;
		if (clk_conf.data->ss_mode_table) {
			printf("ss_mode: %s\n",
				clk_conf.data->ss_mode_table[temp]);
		}
	} else if (clk_conf.data->ss_support == 2) {
		printf("ss_level: %d, %dppm, dep_sel=%d, str_m=%d\n",
			clk_conf.ss_level, clk_conf.ss_ppm,
			clk_conf.ss_dep_sel, clk_conf.ss_str_m);
		printf("ss_freq: %s\n", lcd_ss_freq_table_tl1[clk_conf.ss_freq]);
		printf("ss_mode: %s\n", lcd_ss_mode_table_tl1[clk_conf.ss_mode]);
	}
}

int lcd_set_ss(unsigned int level, unsigned int freq, unsigned int mode)
{
	if (clk_conf.data == NULL) {
		LCDERR("%s: clk config data is null\n", __func__);
		return -1;
	}
	if (level < 0xff) {
		if (level >= clk_conf.data->ss_level_max) {
			LCDERR("%s: ss_level %d is out of support (max %d)\n",
				__func__, level,
				(clk_conf.data->ss_level_max - 1));
			return -1;
		}
	}
	if (freq < 0xff) {
		if (freq >= clk_conf.data->ss_freq_max) {
			LCDERR("%s: ss_freq %d is out of support (max %d)\n",
				__func__, freq,
				(clk_conf.data->ss_freq_max - 1));
			return -1;
		}
	}
	if (mode < 0xff) {
		if (mode >= clk_conf.data->ss_mode_max) {
			LCDERR("%s: ss_mode %d is out of support (max %d)\n",
				__func__, mode,
				(clk_conf.data->ss_mode_max - 1));
			return -1;
		}
	}

	if (clk_conf.data->set_ss_level) {
		if (level < 0xff) {
			clk_conf.ss_level = level;
			clk_conf.data->set_ss_level(level);
		}
	}

	if (clk_conf.data->set_ss_advance) {
		if ((freq == 0xff) && (mode == 0xff))
			goto lcd_set_ss_end;
		if (freq < 0xff)
			clk_conf.ss_freq = freq;
		if (mode < 0xff)
			clk_conf.ss_mode = mode;
		clk_conf.data->set_ss_advance(clk_conf.ss_freq,
			clk_conf.ss_mode);
	}

lcd_set_ss_end:
	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);
	return 0;
}

/* for frame rate change */
void lcd_clk_update(struct lcd_config_s *pconf)
{
	struct lcd_clk_ctrl_s *table;
	unsigned int val;
	int i = 0;

	if (clk_conf.data == NULL) {
		LCDERR("%s: clk config data is null\n", __func__);
		return;
	}
	if (clk_conf.data->pll_frac_generate)
		clk_conf.data->pll_frac_generate(pconf);

	if (clk_conf.data->pll_ctrl_table == NULL)
		return;
	table = clk_conf.data->pll_ctrl_table;
	while (i < LCD_CLK_CTRL_CNT_MAX) {
		if (table[i].flag == LCD_CLK_CTRL_END)
			break;
		if (table[i].flag == LCD_CLK_CTRL_FRAC) {
			val = lcd_hiu_read(table[i].reg);
			lcd_hiu_setb(table[i].reg, clk_conf.pll_frac,
				table[i].bit, table[i].len);
			if (lcd_debug_print_flag) {
				LCDPR("%s: pll_frac reg 0x%x: 0x%08x->0x%08x\n",
					__func__, table[i].reg,
					val, lcd_hiu_read(table[i].reg));
			}
		}
		i++;
	}

	LCDPR("%s: pll_frac=0x%x\n", __func__, clk_conf.pll_frac);
}

/* for timing change */
void lcd_clk_set(struct lcd_config_s *pconf)
{
	int cnt = 0;

	if (!clk_conf.data) {
		LCDERR("%s: clk config data is null\n", __func__);
		return;
	}

lcd_clk_set_retry:
	if (clk_conf.data->clk_set)
		clk_conf.data->clk_set(pconf);
	if (clk_conf.data->vclk_crt_set)
		clk_conf.data->vclk_crt_set(pconf);
#ifdef CONFIG_AML_LCD_PXP
	return;
#endif
	mdelay(10);

	while (lcd_clk_msr_check(&clk_conf)) {
		if (cnt++ >= 10) {
			LCDERR("%s timeout\n", __func__);
			break;
		}
		goto lcd_clk_set_retry;
	}

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);
}

void lcd_clk_disable(void)
{
	struct lcd_clk_ctrl_s *table;
	int i = 0;

	lcd_hiu_setb(HHI_VID_CLK_CNTL2, 0, ENCL_GATE_VCLK, 1);

	/* close vclk2_div gate: 0x104b[4:0] */
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, 0, 5);
	lcd_hiu_setb(HHI_VIID_CLK_CNTL, 0, VCLK2_EN, 1);

	if (clk_conf.data == NULL)
		return;
	if (clk_conf.data->pll_ctrl_table == NULL)
		return;
	table = clk_conf.data->pll_ctrl_table;
	while (i < LCD_CLK_CTRL_CNT_MAX) {
		if (table[i].flag == LCD_CLK_CTRL_END)
			break;
		if (table[i].flag == LCD_CLK_CTRL_EN) {
			lcd_hiu_setb(table[i].reg, 0,
				table[i].bit, table[i].len);
		} else if (table[i].flag == LCD_CLK_CTRL_RST) {
			lcd_hiu_setb(table[i].reg, 1,
				table[i].bit, table[i].len);
		}
		i++;
	}

	LCDPR("%s\n", __func__);
}

static void lcd_clk_config_init_print(void)
{
	if (clk_conf.data == NULL) {
		LCDERR("%s: clk config data is null\n", __func__);
		return;
	}

	if (clk_conf.data->clk_config_init_print)
		clk_conf.data->clk_config_init_print();
}

void lcd_clk_config_print(void)
{
	if (clk_conf.data == NULL) {
		LCDERR("%s: clk config data is null\n", __func__);
		return;
	}

	if (clk_conf.data->clk_config_print)
		clk_conf.data->clk_config_print();
}

/* ****************************************************
 * lcd clk config
 * ****************************************************
 */
static struct lcd_clk_data_s lcd_clk_data_gxtvbb = {
	.pll_od_fb = 1,
	.pll_m_max = 511,
	.pll_m_min = 2,
	.pll_n_max = 1,
	.pll_n_min = 1,
	.pll_frac_range = (1 << 10),
	.pll_frac_sign_bit = 11,
	.pll_od_sel_max = 3,
	.pll_ref_fmax = 25000000,
	.pll_ref_fmin = 5000000,
	.pll_vco_fmax = 6000000000ULL,
	.pll_vco_fmin = 3000000000ULL,
	.pll_out_fmax = 3100000000ULL,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 3100000000ULL,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 620000000,

	.clk_path_valid = 0,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = LCD_CLK_MSR_INVALID,
	.pll_ctrl_table = pll_ctrl_table_gxtvbb,

	.ss_support = 1,
	.ss_level_max = sizeof(lcd_ss_level_table_gxtvbb) / sizeof(char *),
	.ss_freq_max = 0,
	.ss_mode_max = 0,
	.ss_level_table = lcd_ss_level_table_gxtvbb,
	.ss_freq_table = NULL,
	.ss_mode_table = NULL,

	.clk_generate_parameter = lcd_clk_generate_gxtvbb,
	.pll_frac_generate = lcd_pll_frac_generate_gxtvbb,
	.set_ss_level = lcd_set_pll_ss_level_gxtvbb,
	.set_ss_advance = NULL,
	.clk_set = lcd_clk_set_gxtvbb,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_gxl = {
	.pll_od_fb = 1,
	.pll_m_max = 511,
	.pll_m_min = 2,
	.pll_n_max = 1,
	.pll_n_min = 1,
	.pll_frac_range = (1 << 10),
	.pll_frac_sign_bit = 11,
	.pll_od_sel_max = 3,
	.pll_ref_fmax = 25000000,
	.pll_ref_fmin = 5000000,
	.pll_vco_fmax = 6000000000ULL,
	.pll_vco_fmin = 3000000000ULL,
	.pll_out_fmax = 3100000000ULL,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 3100000000ULL,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 620000000,

	.clk_path_valid = 0,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = LCD_CLK_MSR_INVALID,
	.pll_ctrl_table = pll_ctrl_table_txl,

	.ss_support = 0,
	.ss_level_max = 0,
	.ss_freq_max = 0,
	.ss_mode_max = 0,
	.ss_level_table = NULL,
	.ss_freq_table = NULL,
	.ss_mode_table = NULL,

	.clk_generate_parameter = lcd_clk_generate_txl,
	.pll_frac_generate = lcd_pll_frac_generate_txl,
	.set_ss_level = NULL,
	.set_ss_advance = NULL,
	.clk_set = lcd_clk_set_txl,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_txl = {
	.pll_od_fb = 1,
	.pll_m_max = 511,
	.pll_m_min = 2,
	.pll_n_max = 1,
	.pll_n_min = 1,
	.pll_frac_range = (1 << 10),
	.pll_frac_sign_bit = 11,
	.pll_od_sel_max = 3,
	.pll_ref_fmax = 25000000,
	.pll_ref_fmin = 5000000,
	.pll_vco_fmax = 6000000000ULL,
	.pll_vco_fmin = 3000000000ULL,
	.pll_out_fmax = 3100000000ULL,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 3100000000ULL,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 620000000,

	.clk_path_valid = 0,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = LCD_CLK_MSR_INVALID,
	.pll_ctrl_table = pll_ctrl_table_txl,

	.ss_support = 1,
	.ss_level_max = sizeof(lcd_ss_level_table_txl) / sizeof(char *),
	.ss_freq_max = 0,
	.ss_mode_max = 0,
	.ss_level_table = lcd_ss_level_table_txl,
	.ss_freq_table = NULL,
	.ss_mode_table = NULL,

	.clk_generate_parameter = lcd_clk_generate_txl,
	.pll_frac_generate = lcd_pll_frac_generate_txl,
	.set_ss_level = lcd_set_pll_ss_level_txl,
	.set_ss_advance = NULL,
	.clk_set = lcd_clk_set_txl,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_txlx = {
	.pll_od_fb = 0,
	.pll_m_max = 511,
	.pll_m_min = 2,
	.pll_n_max = 1,
	.pll_n_min = 1,
	.pll_frac_range = (1 << 10),
	.pll_frac_sign_bit = 11,
	.pll_od_sel_max = 3,
	.pll_ref_fmax = 25000000,
	.pll_ref_fmin = 5000000,
	.pll_vco_fmax = 6000000000ULL,
	.pll_vco_fmin = 3000000000ULL,
	.pll_out_fmax = 3100000000ULL,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 3100000000ULL,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 620000000,

	.clk_path_valid = 0,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = LCD_CLK_MSR_INVALID,
	.pll_ctrl_table = pll_ctrl_table_txl,

	.ss_support = 1,
	.ss_level_max = sizeof(lcd_ss_level_table_txlx) / sizeof(char *),
	.ss_freq_max = 0,
	.ss_mode_max = 0,
	.ss_level_table = lcd_ss_level_table_txlx,
	.ss_freq_table = NULL,
	.ss_mode_table = NULL,

	.clk_generate_parameter = lcd_clk_generate_txl,
	.pll_frac_generate = lcd_pll_frac_generate_txl,
	.set_ss_level = lcd_set_pll_ss_level_txlx,
	.set_ss_advance = NULL,
	.clk_set = lcd_clk_set_txlx,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_axg = {
	.pll_od_fb = 0,
	.pll_m_max = 511,
	.pll_m_min = 2,
	.pll_n_max = 1,
	.pll_n_min = 1,
	.pll_frac_range = (1 << 10),
	.pll_frac_sign_bit = 11,
	.pll_od_sel_max = 3,
	.pll_ref_fmax = 25000000,
	.pll_ref_fmin = 5000000,
	.pll_vco_fmax = 1920000000ULL,
	.pll_vco_fmin = 960000000,
	.pll_out_fmax = 1920000000ULL,
	.pll_out_fmin = 240000000,
	.div_in_fmax = 0,
	.div_out_fmax = 1920000000ULL,
	.xd_out_fmax = 200000000,

	.clk_path_valid = 0,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = LCD_CLK_MSR_INVALID,
	.pll_ctrl_table = pll_ctrl_table_axg,

	.ss_support = 0,
	.ss_level_max = 0,
	.ss_freq_max = 0,
	.ss_mode_max = 0,
	.ss_level_table = NULL,
	.ss_freq_table = NULL,
	.ss_mode_table = NULL,

	.clk_generate_parameter = lcd_clk_generate_axg,
	.pll_frac_generate = lcd_pll_frac_generate_axg,
	.set_ss_level = NULL,
	.set_ss_advance = NULL,
	.clk_set = lcd_clk_set_axg,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_axg,
	.clk_config_print = lcd_clk_config_print_axg,
	.prbs_clk_config = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_txhd = {
	.pll_od_fb = 0,
	.pll_m_max = 511,
	.pll_m_min = 2,
	.pll_n_max = 1,
	.pll_n_min = 1,
	.pll_frac_range = (1 << 10),
	.pll_frac_sign_bit = 11,
	.pll_od_sel_max = 3,
	.pll_ref_fmax = 25000000,
	.pll_ref_fmin = 5000000,
	.pll_vco_fmax = 6000000000ULL,
	.pll_vco_fmin = 3000000000ULL,
	.pll_out_fmax = 3100000000ULL,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 3100000000ULL,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 400000000,

	.clk_path_valid = 0,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = LCD_CLK_MSR_INVALID,
	.pll_ctrl_table = pll_ctrl_table_txhd,

	.ss_support = 1,
	.ss_level_max = sizeof(lcd_ss_level_table_txhd) / sizeof(char *),
	.ss_freq_max = sizeof(lcd_ss_freq_table_tl1) / sizeof(char *),
	.ss_mode_max = sizeof(lcd_ss_mode_table_tl1) / sizeof(char *),
	.ss_level_table = lcd_ss_level_table_txhd,
	.ss_freq_table = lcd_ss_freq_table_tl1,
	.ss_mode_table = lcd_ss_mode_table_tl1,

	.clk_generate_parameter = lcd_clk_generate_txhd,
	.pll_frac_generate = lcd_pll_frac_generate_txl,
	.set_ss_level = lcd_set_pll_ss_level_txhd,
	.set_ss_advance = lcd_set_pll_ss_advance_txhd,
	.clk_set = lcd_clk_set_txhd,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_g12a_path0 = {
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
	.pll_out_fmax = 6000000000ULL,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 0,
	.div_out_fmax = 1000000000,
	.xd_out_fmax = 200000000,

	.clk_path_valid = 1,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = LCD_CLK_MSR_INVALID,
	.pll_ctrl_table = pll_ctrl_table_g12a_path0,

	.ss_support = 0,
	.ss_level_max = 0,
	.ss_freq_max = 0,
	.ss_mode_max = 0,
	.ss_level_table = NULL,
	.ss_freq_table = NULL,
	.ss_mode_table = NULL,

	.clk_generate_parameter = lcd_clk_generate_hpll_g12a,
	.pll_frac_generate = lcd_pll_frac_generate_txl,
	.set_ss_level = NULL,
	.set_ss_advance = NULL,
	.clk_set = lcd_clk_set_g12a_path0,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_axg,
	.clk_config_print = lcd_clk_config_print_g12a,
	.prbs_clk_config = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_g12a_path1 = {
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
	.pll_out_fmin = 187500000,
	.div_in_fmax = 0,
	.div_out_fmax = 1000000000,
	.xd_out_fmax = 200000000,

	.clk_path_valid = 1,
	.vclk_sel = 1,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = LCD_CLK_MSR_INVALID,
	.pll_ctrl_table = pll_ctrl_table_g12a_path1,

	.ss_support = 0,
	.ss_level_max = 0,
	.ss_freq_max = 0,
	.ss_mode_max = 0,
	.ss_level_table = NULL,
	.ss_freq_table = NULL,
	.ss_mode_table = NULL,

	.clk_generate_parameter = lcd_clk_generate_axg,
	.pll_frac_generate = lcd_pll_frac_generate_axg,
	.set_ss_level = NULL,
	.set_ss_advance = NULL,
	.clk_set = lcd_clk_set_g12a_path1,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_axg,
	.clk_config_print = lcd_clk_config_print_g12a,
	.prbs_clk_config = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_g12b_path0 = {
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
	.pll_out_fmax = 6000000000ULL,
	.pll_out_fmin = 187500000,
	.div_in_fmax = 0,
	.div_out_fmax = 1000000000,
	.xd_out_fmax = 200000000,

	.clk_path_valid = 1,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = LCD_CLK_MSR_INVALID,
	.pll_ctrl_table = pll_ctrl_table_g12a_path0,

	.ss_support = 0,
	.ss_level_max = 0,
	.ss_freq_max = 0,
	.ss_mode_max = 0,
	.ss_level_table = NULL,
	.ss_freq_table = NULL,
	.ss_mode_table = NULL,

	.clk_generate_parameter = lcd_clk_generate_hpll_g12a,
	.pll_frac_generate = lcd_pll_frac_generate_txl,
	.set_ss_level = NULL,
	.set_ss_advance = NULL,
	.clk_set = lcd_clk_set_g12b_path0,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_axg,
	.clk_config_print = lcd_clk_config_print_g12a,
	.prbs_clk_config = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_g12b_path1 = {
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
	.pll_out_fmin = 187500000,
	.div_in_fmax = 0,
	.div_out_fmax = 1000000000,
	.xd_out_fmax = 200000000,

	.clk_path_valid = 1,
	.vclk_sel = 1,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = LCD_CLK_MSR_INVALID,
	.pll_ctrl_table = pll_ctrl_table_g12a_path1,

	.ss_support = 0,
	.ss_level_max = 0,
	.ss_freq_max = 0,
	.ss_mode_max = 0,
	.ss_level_table = NULL,
	.ss_freq_table = NULL,
	.ss_mode_table = NULL,

	.clk_generate_parameter = lcd_clk_generate_axg,
	.pll_frac_generate = lcd_pll_frac_generate_axg,
	.set_ss_level = NULL,
	.set_ss_advance = NULL,
	.clk_set = lcd_clk_set_g12b_path1,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_axg,
	.clk_config_print = lcd_clk_config_print_g12a,
	.prbs_clk_config = NULL,
};

static struct lcd_clk_data_s lcd_clk_data_tl1 = {
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
	.pll_vco_fmax = 6024000000ULL,
	.pll_vco_fmin = 3384000000ULL,
	.pll_out_fmax = 3100000000ULL,
	.pll_out_fmin = 211500000,
	.div_in_fmax = 3100000000ULL,
	.div_out_fmax = 750000000,
	.xd_out_fmax = 750000000,

	.clk_path_valid = 0,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = 129,
	.pll_ctrl_table = pll_ctrl_table_tl1,

	.ss_support = 2,
	.ss_level_max = sizeof(lcd_ss_level_table_tl1) / sizeof(char *),
	.ss_freq_max = sizeof(lcd_ss_freq_table_tl1) / sizeof(char *),
	.ss_mode_max = sizeof(lcd_ss_mode_table_tl1) / sizeof(char *),
	.ss_dep_base = 500, //ppm
	.ss_dep_sel_max = 12,
	.ss_str_m_max = 10,
	.ss_level_table = lcd_ss_level_table_tl1,
	.ss_freq_table = lcd_ss_freq_table_tl1,
	.ss_mode_table = lcd_ss_mode_table_tl1,

	.clk_generate_parameter = lcd_clk_generate_txl,
	.pll_frac_generate = lcd_pll_frac_generate_txl,
	.set_ss_level = lcd_set_pll_ss_level_tl1,
	.set_ss_advance = lcd_set_pll_ss_advance_tl1,
	.clk_set = lcd_clk_set_tl1,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = lcd_prbs_config_clk_tl1,
};

static struct lcd_clk_data_s lcd_clk_data_tm2 = {
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

	.clk_path_valid = 0,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = 129,
	.pll_ctrl_table = pll_ctrl_table_tl1,

	.ss_support = 2,
	.ss_level_max = sizeof(lcd_ss_level_table_tl1) / sizeof(char *),
	.ss_freq_max = sizeof(lcd_ss_freq_table_tl1) / sizeof(char *),
	.ss_mode_max = sizeof(lcd_ss_mode_table_tl1) / sizeof(char *),
	.ss_dep_base = 500, //ppm
	.ss_dep_sel_max = 12,
	.ss_str_m_max = 10,
	.ss_level_table = lcd_ss_level_table_tl1,
	.ss_freq_table = lcd_ss_freq_table_tl1,
	.ss_mode_table = lcd_ss_mode_table_tl1,

	.clk_generate_parameter = lcd_clk_generate_txl,
	.pll_frac_generate = lcd_pll_frac_generate_txl,
	.set_ss_level = lcd_set_pll_ss_level_tl1,
	.set_ss_advance = lcd_set_pll_ss_advance_tl1,
	.clk_set = lcd_clk_set_tl1,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = lcd_prbs_config_clk_tl1,
};

static struct lcd_clk_data_s lcd_clk_data_t5 = {
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

	.clk_path_valid = 0,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = 129,
	.pll_ctrl_table = pll_ctrl_table_tl1,

	.ss_support = 2,
	.ss_level_max = sizeof(lcd_ss_level_table_tl1) / sizeof(char *),
	.ss_freq_max = sizeof(lcd_ss_freq_table_tl1) / sizeof(char *),
	.ss_mode_max = sizeof(lcd_ss_mode_table_tl1) / sizeof(char *),
	.ss_dep_base = 500, //ppm
	.ss_dep_sel_max = 12,
	.ss_str_m_max = 10,
	.ss_level_table = lcd_ss_level_table_tl1,
	.ss_freq_table = lcd_ss_freq_table_tl1,
	.ss_mode_table = lcd_ss_mode_table_tl1,

	.clk_generate_parameter = lcd_clk_generate_txl,
	.pll_frac_generate = lcd_pll_frac_generate_txl,
	.set_ss_level = lcd_set_pll_ss_level_tl1,
	.set_ss_advance = lcd_set_pll_ss_advance_tl1,
	.clk_set = lcd_clk_set_t5,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = lcd_prbs_config_clk_tl1,
};

static struct lcd_clk_data_s lcd_clk_data_t5d = {
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
	.xd_out_fmax = 400000000,

	.clk_path_valid = 0,
	.vclk_sel = 0,
	.enc_clk_msr_id = 9,
	.fifo_clk_msr_id = 129,
	.pll_ctrl_table = pll_ctrl_table_tl1,

	.ss_support = 2,
	.ss_level_max = sizeof(lcd_ss_level_table_tl1) / sizeof(char *),
	.ss_freq_max = sizeof(lcd_ss_freq_table_tl1) / sizeof(char *),
	.ss_mode_max = sizeof(lcd_ss_mode_table_tl1) / sizeof(char *),
	.ss_dep_base = 500, //ppm
	.ss_dep_sel_max = 12,
	.ss_str_m_max = 10,
	.ss_level_table = lcd_ss_level_table_tl1,
	.ss_freq_table = lcd_ss_freq_table_tl1,
	.ss_mode_table = lcd_ss_mode_table_tl1,

	.clk_generate_parameter = lcd_clk_generate_txl,
	.pll_frac_generate = lcd_pll_frac_generate_txl,
	.set_ss_level = lcd_set_pll_ss_level_tl1,
	.set_ss_advance = lcd_set_pll_ss_advance_tl1,
	.clk_set = lcd_clk_set_t5,
	.vclk_crt_set = lcd_set_vclk_crt,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = lcd_prbs_config_clk_tl1,
};

static struct lcd_clk_data_s lcd_clk_data_t5w = {
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

	.clk_path_valid = 0,
	.vclk_sel = 0,
	.enc_clk_msr_id = 6,
	.fifo_clk_msr_id = 129,
	.pll_ctrl_table = pll_ctrl_table_tl1,

	.ss_support = 2,
	.ss_level_max = 60,
	.ss_freq_max = 6,
	.ss_mode_max = 2,
	.ss_dep_base = 500, //ppm
	.ss_dep_sel_max = 12,
	.ss_str_m_max = 10,
	.ss_level_table = NULL,
	.ss_freq_table = lcd_ss_freq_table_tl1,
	.ss_mode_table = lcd_ss_mode_table_tl1,

	.clk_generate_parameter = lcd_clk_generate_txl,
	.pll_frac_generate = lcd_pll_frac_generate_txl,
	.set_ss_level = lcd_set_pll_ss_level_t5w,
	.set_ss_advance = lcd_set_pll_ss_advance_tl1,
	.clk_set = lcd_clk_set_t5w,
	.vclk_crt_set = lcd_set_vclk_crt_t5w,
	.clk_config_init_print = lcd_clk_config_init_print_dft,
	.clk_config_print = lcd_clk_config_print_dft,
	.prbs_clk_config = lcd_prbs_config_clk_t5w,
};

static void lcd_clk_config_chip_init(struct lcd_clk_config_s *cConf)
{
	struct aml_lcd_drv_s *lcd_drv = aml_lcd_get_driver();

	switch (lcd_drv->chip_type) {
	case LCD_CHIP_GXTVBB:
		cConf->data = &lcd_clk_data_gxtvbb;
		break;
	case LCD_CHIP_GXL:
	case LCD_CHIP_GXM:
		cConf->data = &lcd_clk_data_gxl;
		break;
	case LCD_CHIP_TXL:
		cConf->data = &lcd_clk_data_txl;
		break;
	case LCD_CHIP_TXLX:
		cConf->data = &lcd_clk_data_txlx;
		break;
	case LCD_CHIP_AXG:
		cConf->data = &lcd_clk_data_axg;
		break;
	case LCD_CHIP_TXHD:
		cConf->data = &lcd_clk_data_txhd;
		break;
	case LCD_CHIP_G12A:
	case LCD_CHIP_SM1:
		if (lcd_drv->lcd_config->lcd_clk_path)
			cConf->data = &lcd_clk_data_g12a_path1;
		else
			cConf->data = &lcd_clk_data_g12a_path0;
		break;
	case LCD_CHIP_G12B:
		if (lcd_drv->lcd_config->lcd_clk_path)
			cConf->data = &lcd_clk_data_g12b_path1;
		else
			cConf->data = &lcd_clk_data_g12b_path0;
		break;
	case LCD_CHIP_TL1:
		cConf->data = &lcd_clk_data_tl1;
		break;
	case LCD_CHIP_TM2:
		cConf->data = &lcd_clk_data_tm2;
		break;
	case LCD_CHIP_T5:
		cConf->data = &lcd_clk_data_t5;
		break;
	case LCD_CHIP_T5D:
		cConf->data = &lcd_clk_data_t5d;
		break;
	case LCD_CHIP_T5W:
		cConf->data = &lcd_clk_data_t5w;
		break;
	default:
		LCDPR("%s invalid chip type\n", __func__);
		break;
	}

	if (cConf->data)
		cConf->pll_od_fb = cConf->data->pll_od_fb;
	if (lcd_debug_print_flag > 0)
		lcd_clk_config_init_print();
}

void lcd_clk_config_probe(void)
{
	lcd_clk_config_chip_init(&clk_conf);
}
