/*
 * drivers/amlogic/media/vout/lcd/aml_lcd_clk_ctrl.h
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _LCD_CLK_CONFIG_CTRL_H
#define _LCD_CLK_CONFIG_CTRL_H

#include "lcd_reg.h"
#include "lcd_clk_config.h"

/* G12A */
/* ******** register bit ******** */
/* PLL_CNTL bit: GP0 */
#define LCD_PLL_LOCK_GP0_G12A        31
#define LCD_PLL_EN_GP0_G12A          28
#define LCD_PLL_RST_GP0_G12A         29
#define LCD_PLL_OD_GP0_G12A          16
#define LCD_PLL_N_GP0_G12A           10
#define LCD_PLL_M_GP0_G12A           0

/* ******** frequency limit (unit: kHz) ******** */
#define PLL_OD_FB_GP0_G12A           0
#define PLL_FRAC_RANGE_GP0_G12A      (1 << 17)
#define PLL_OD_SEL_MAX_GP0_G12A      5
#define PLL_VCO_MIN_GP0_G12A         (3000 * 1000)
#define PLL_VCO_MAX_GP0_G12A         (6000 * 1000)

/* PLL_CNTL bit: hpll */
#define LCD_PLL_LOCK_HPLL_G12A       31
#define LCD_PLL_EN_HPLL_G12A         28
#define LCD_PLL_RST_HPLL_G12A        29
#define LCD_PLL_N_HPLL_G12A          10
#define LCD_PLL_M_HPLL_G12A          0

#define LCD_PLL_OD3_HPLL_G12A        20
#define LCD_PLL_OD2_HPLL_G12A        18
#define LCD_PLL_OD1_HPLL_G12A        16

/* ******** frequency limit (unit: kHz) ******** */
#define PLL_OD_FB_HPLL_G12A          0
#define PLL_FRAC_RANGE_HPLL_G12A     (1 << 17)
#define PLL_OD_SEL_MAX_HPLL_G12A     3
#define PLL_VCO_MIN_HPLL_G12A        (3000 * 1000)
#define PLL_VCO_MAX_HPLL_G12A        (6000 * 1000)

/* video */
#define PLL_M_MIN_G12A               2
#define PLL_M_MAX_G12A               511
#define PLL_N_MIN_G12A               1
#define PLL_N_MAX_G12A               1
#define PLL_FREF_MIN_G12A            (5 * 1000)
#define PLL_FREF_MAX_G12A            (25 * 1000)
#define CRT_VID_CLK_IN_MAX_G12A      (6000 * 1000)
#define ENCL_CLK_IN_MAX_G12A         (200 * 1000)


/* **********************************
 * Spread Spectrum
 * **********************************
 */
#define LCD_SS_STEP_BASE            500 /* ppm */

#define SS_LEVEL_MAX_GP0_G12A       0
#define SS_LEVEL_MAX_HPLL_G12A      0

/* **********************************
 * pll control
 * **********************************
 */
static struct lcd_clk_ctrl_s pll_ctrl_table_g12a_path0[] = {
	/* flag             reg                 bit                   len*/
	{LCD_CLK_CTRL_EN,   HHI_HDMI_PLL_CNTL0, LCD_PLL_EN_HPLL_G12A,  1},
	{LCD_CLK_CTRL_FRAC, HHI_HDMI_PLL_CNTL1,                    0, 19},
	{LCD_CLK_CTRL_END,  LCD_CLK_REG_END,                       0,  0},
};

static struct lcd_clk_ctrl_s pll_ctrl_table_g12a_path1[] = {
	/* flag             reg                bit                  len*/
	{LCD_CLK_CTRL_EN,   HHI_GP0_PLL_CNTL0, LCD_PLL_EN_GP0_G12A,  1},
	{LCD_CLK_CTRL_FRAC, HHI_GP0_PLL_CNTL1,                   0, 19},
	{LCD_CLK_CTRL_END,  LCD_CLK_REG_END,                     0,  0},
};

/* **********************************
 * pll & clk parameter
 * ********************************** */
/* ******** clk calculation ******** */
#define PLL_WAIT_LOCK_CNT           200
 /* frequency unit: kHz */
#define FIN_FREQ                    (24 * 1000)
/* clk max error */
#define MAX_ERROR                   (2 * 1000)

/* ******** register bit ******** */
/* divider */
#define CRT_VID_DIV_MAX             255

static const unsigned int od_fb_table[2] = {1, 2};

static const unsigned int od_table[6] = {1, 2, 4, 8, 16, 32};

/* static const unsigned int pi_div_table[2] = {2, 4}; */

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
	"invalid",
};

/* g9tv, g9bb, gxbb divider */
#define CLK_DIV_I2O     0
#define CLK_DIV_O2I     1
enum div_sel_e {
	CLK_DIV_SEL_1 = 0,
	CLK_DIV_SEL_2,    /* 1 */
	CLK_DIV_SEL_3,    /* 2 */
	CLK_DIV_SEL_3p5,  /* 3 */
	CLK_DIV_SEL_3p75, /* 4 */
	CLK_DIV_SEL_4,    /* 5 */
	CLK_DIV_SEL_5,    /* 6 */
	CLK_DIV_SEL_6,    /* 7 */
	CLK_DIV_SEL_6p25, /* 8 */
	CLK_DIV_SEL_7,    /* 9 */
	CLK_DIV_SEL_7p5,  /* 10 */
	CLK_DIV_SEL_12,   /* 11 */
	CLK_DIV_SEL_14,   /* 12 */
	CLK_DIV_SEL_15,   /* 13 */
	CLK_DIV_SEL_2p5,  /* 14 */
	CLK_DIV_SEL_MAX,
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
	{CLK_DIV_SEL_MAX,  0xffff,     0,},
};

#endif
