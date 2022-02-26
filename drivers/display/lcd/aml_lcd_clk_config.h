/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/lcd/aml_lcd_clk_config.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _LCD_CLK_CONFIG_H
#define _LCD_CLK_CONFIG_H

#include <linux/types.h>
#include <amlogic/aml_lcd.h>

/* **********************************
 * clk config
 * ********************************** */
#define LCD_PLL_MODE_DEFAULT         (1 << 0)
#define LCD_PLL_MODE_SPECIAL_CNTL    (1 << 1)
#define LCD_PLL_MODE_FRAC_SHIFT      (1 << 2)

#define PLL_RETRY_MAX		20
#define LCD_CLK_CTRL_EN      0
#define LCD_CLK_CTRL_RST     1
#define LCD_CLK_CTRL_FRAC    2
#define LCD_CLK_CTRL_END     0xffff

#define LCD_CLK_REG_END      0xffff
#define LCD_CLK_CTRL_CNT_MAX 10
struct lcd_clk_ctrl_s {
	unsigned int flag;
	unsigned int reg;
	unsigned int bit;
	unsigned int len;
};

#define LCD_CLK_MSR_INVALID   0xffffffff

struct lcd_clk_data_s {
	/* clk path node parameters */
	unsigned int pll_od_fb;
	unsigned int pll_m_max;
	unsigned int pll_m_min;
	unsigned int pll_n_max;
	unsigned int pll_n_min;
	unsigned int pll_frac_range;
	unsigned int pll_frac_sign_bit;
	unsigned int pll_od_sel_max;
	unsigned int pll_ref_fmax;
	unsigned int pll_ref_fmin;
	unsigned int pll_vco_fmax;
	unsigned int pll_vco_fmin;
	unsigned int pll_out_fmax;
	unsigned int pll_out_fmin;
	unsigned int div_in_fmax;
	unsigned int div_out_fmax;
	unsigned int xd_out_fmax;

	unsigned char clk_path_valid;
	unsigned char vclk_sel;
	int enc_clk_msr_id;
	int fifo_clk_msr_id;
	struct lcd_clk_ctrl_s *pll_ctrl_table;

	unsigned int ss_level_max;
	unsigned int ss_freq_max;
	unsigned int ss_mode_max;
	char **ss_level_table;
	char **ss_freq_table;
	char **ss_mode_table;

	void (*clk_generate_parameter)(struct lcd_config_s *pconf);
	void (*pll_frac_generate)(struct lcd_config_s *pconf);
	void (*set_ss_level)(unsigned int level);
	void (*set_ss_advance)(unsigned int freq, unsigned int mode);
	void (*clk_set)(struct lcd_config_s *pconf);
	void (*vclk_crt_set)(struct lcd_config_s *pconf);
	void (*clk_config_init_print)(void);
	void (*clk_config_print)(void);
	void (*prbs_clk_config)(unsigned int lcd_prbs_mode);
};

struct lcd_clk_config_s { /* unit: kHz */
	/* IN-OUT parameters */
	unsigned int fin;
	unsigned int fout;

	/* pll parameters */
	unsigned int pll_mode; /* txl */
	unsigned int pll_od_fb;
	unsigned int pll_m;
	unsigned int pll_n;
	unsigned int pll_fvco;
	unsigned int pll_od1_sel;
	unsigned int pll_od2_sel;
	unsigned int pll_od3_sel;
	unsigned int pll_tcon_div_sel;
	unsigned int pll_level;
	unsigned int pll_frac;
	unsigned int pll_frac_half_shift;
	unsigned int pll_fout;
	unsigned int ss_level;
	unsigned int ss_freq;
	unsigned int ss_mode;
	unsigned int div_sel;
	unsigned int xd;
	unsigned int div_sel_max;
	unsigned int xd_max;
	unsigned int err_fmin;

	struct lcd_clk_data_s *data;
};

/* ******** api ******** */
extern struct lcd_clk_config_s *get_lcd_clk_config(void);
extern void lcd_clk_config_print(void);

extern void lcd_get_ss(void);
extern int lcd_set_ss(unsigned int level, unsigned int freq, unsigned int mode);
extern void lcd_clk_update(struct lcd_config_s *pconf);
extern void lcd_clk_set(struct lcd_config_s *pconf);
extern void lcd_clk_disable(void);

extern void lcd_clk_generate_parameter(struct lcd_config_s *pconf);
extern void lcd_clk_config_probe(void);

extern unsigned long clk_util_clk_msr(int index);

#endif
