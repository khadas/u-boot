// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <asm/gpio.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "lcd_bl.h"
#include "../lcd_reg.h"
#include "../lcd_common.h"

#define PWM_REG_MAX    0xffffffff

struct bl_pwm_clkctrl_s {
	unsigned int reg;
	unsigned int bit_div;
	unsigned int bit_sel;
	unsigned int bit_en;
};

struct bl_pwm_misc_s {
	unsigned int reg;
	unsigned int bit_pre_div;
	unsigned int bit_clk_sel;
	unsigned int bit_clk_en;
	unsigned int bit_pwm_en;
	unsigned int val_pwm_en;
};

struct bl_pwm_ctrl_config_s {
	unsigned int pwm_div_flag; /*1:div in clktree*/
	unsigned int pwm_vs_flag; /*1:8ch*/
	struct bl_pwm_clkctrl_s *pwm_clk;
	struct bl_pwm_misc_s *pwm_misc;
	unsigned int *pwm_reg;
	unsigned int pwm_cnt;
	struct bl_pwm_clkctrl_s *pwm_ao_clk;
	struct bl_pwm_misc_s *pwm_ao_misc;
	unsigned int *pwm_ao_reg;
	unsigned int pwm_ao_cnt;
};

static struct bl_pwm_ctrl_config_s *bl_pwm_ctrl_conf;

static struct bl_pwm_misc_s pwm_misc_dft[] = {
	/* pwm_reg,    bit_pre_div, bit_clk_sel, bit_clk_en, bit_pwm_en, val_en*/
	{PWM_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWM_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWM_MISC_REG_CD,   8,       4,          15,         0,          1,},
	{PWM_MISC_REG_CD,   16,      6,          23,         1,          1,},
	{PWM_MISC_REG_EF,   8,       4,          15,         0,          1,},
	{PWM_MISC_REG_EF,   16,      6,          23,         1,          1,},
	{PWM_REG_MAX,       0,       0,          0,          0,          0,}
};

static unsigned int pwm_reg_dft[] = {
	PWM_PWM_A,
	PWM_PWM_B,
	PWM_PWM_C,
	PWM_PWM_D,
	PWM_PWM_E,
	PWM_PWM_F,
	PWM_REG_MAX
};

static struct bl_pwm_misc_s pwm_ao_misc_dft[] = {
	/* pwm_reg,       bit_pre_div, bit_clk_sel, bit_clk_en, bit_pwm_en, val_en*/
	{AO_PWM_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{AO_PWM_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWM_REG_MAX,          0,       0,          0,          0,          0,}
};

static unsigned int pwm_ao_reg_dft[] = {
	AO_PWM_PWM_A,
	AO_PWM_PWM_B,
	PWM_REG_MAX
};

static struct bl_pwm_clkctrl_s pwm_clk_ctrl_t7[] = {
	/* pwm_reg,                bit_div, bit_sel, bit_en*/
	{CLKCTRL_PWM_CLK_AB_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_AB_CTRL,   16,      25,      24},
	{CLKCTRL_PWM_CLK_CD_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_CD_CTRL,   16,      25,      24},
	{CLKCTRL_PWM_CLK_EF_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_EF_CTRL,   16,      25,      24},
	{PWM_REG_MAX,               0,       0,       0}
};

static struct bl_pwm_misc_s pwm_misc_t7[] = {
	/* pwm_reg,      bit_pre_div, bit_clk_sel, bit_clk_en, bit_pwm_en, val_en*/
	{PWMAB_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWMAB_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWMCD_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWMCD_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWMEF_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWMEF_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWM_REG_MAX,         0,       0,          0,          0,          0,}
};

static unsigned int pwm_reg_t7[] = {
	PWMAB_PWM_A,
	PWMAB_PWM_B,
	PWMCD_PWM_A,
	PWMCD_PWM_B,
	PWMEF_PWM_A,
	PWMEF_PWM_B,
	PWM_REG_MAX
};

static struct bl_pwm_clkctrl_s pwm_ao_clk_ctrl_t7[] = {
	/* pwm_reg,                    bit_div, bit_sel, bit_en*/
	{CLKCTRL_PWM_CLK_AO_AB_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_AO_AB_CTRL,   16,      25,      24},
	{CLKCTRL_PWM_CLK_AO_CD_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_AO_CD_CTRL,   16,      25,      24},
	{CLKCTRL_PWM_CLK_AO_EF_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_AO_EF_CTRL,   16,      25,      24},
	{CLKCTRL_PWM_CLK_AO_GH_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_AO_GH_CTRL,   16,      25,      24},
	{PWM_REG_MAX,                  0,       0,       0}
};

static struct bl_pwm_misc_s pwm_ao_misc_t7[] = {
	/* pwm_reg,          bit_pre_div, bit_clk_sel, bit_clk_en, bit_pwm_en, val_en*/
	{PWM_AO_AB_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWM_AO_AB_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWM_AO_CD_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWM_AO_CD_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWM_AO_EF_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWM_AO_EF_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWM_AO_GH_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWM_AO_GH_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWM_REG_MAX,             0,       0,          0,          0,          0,}
};

static unsigned int pwm_ao_reg_t7[] = {
	PWM_AO_AB_PWM_A,
	PWM_AO_AB_PWM_B,
	PWM_AO_CD_PWM_A,
	PWM_AO_CD_PWM_B,
	PWM_AO_EF_PWM_A,
	PWM_AO_EF_PWM_B,
	PWM_AO_GH_PWM_A,
	PWM_AO_GH_PWM_B,
	PWM_REG_MAX
};

static struct bl_pwm_clkctrl_s pwm_clk_ctrl_t3[] = {
	/* pwm_reg,                bit_div, bit_sel, bit_en*/
	{CLKCTRL_PWM_CLK_AB_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_AB_CTRL,   16,      25,      24},
	{CLKCTRL_PWM_CLK_CD_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_CD_CTRL,   16,      25,      24},
	{CLKCTRL_PWM_CLK_EF_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_EF_CTRL,   16,      25,      24},
	{CLKCTRL_PWM_CLK_GH_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_GH_CTRL,   16,      25,      24},
	{CLKCTRL_PWM_CLK_IJ_CTRL,   0,       9,       8},
	{CLKCTRL_PWM_CLK_IJ_CTRL,   16,      25,      24},
	{PWM_REG_MAX,               0,       0,       0}
};

static struct bl_pwm_misc_s pwm_misc_t3[] = {
	/* pwm_reg,      bit_pre_div, bit_clk_sel, bit_clk_en, bit_pwm_en, val_en*/
	{PWMAB_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWMAB_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWMCD_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWMCD_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWMEF_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWMEF_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWMGH_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWMGH_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWMIJ_MISC_REG_AB,   8,       4,          15,         0,          1,},
	{PWMIJ_MISC_REG_AB,   16,      6,          23,         1,          1,},
	{PWM_REG_MAX,         0,       0,          0,          0,          0,}
};

static unsigned int pwm_reg_t3[] = {
	PWMAB_PWM_A,
	PWMAB_PWM_B,
	PWMCD_PWM_A,
	PWMCD_PWM_B,
	PWMEF_PWM_A,
	PWMEF_PWM_B,
	PWMGH_PWM_A,
	PWMGH_PWM_B,
	PWMIJ_PWM_A,
	PWMIJ_PWM_B,
	PWM_REG_MAX
};

static struct bl_pwm_ctrl_config_s bl_pwm_ctrl_conf_dft = {
	.pwm_div_flag = 0,
	.pwm_vs_flag = 0,
	.pwm_clk = NULL,
	.pwm_misc = pwm_misc_dft,
	.pwm_reg = pwm_reg_dft,
	.pwm_cnt = 6,
	.pwm_ao_clk = NULL,
	.pwm_ao_misc = pwm_ao_misc_dft,
	.pwm_ao_reg = pwm_ao_reg_dft,
	.pwm_ao_cnt = 2,
};

static struct bl_pwm_ctrl_config_s bl_pwm_ctrl_conf_tl1 = {
	.pwm_div_flag = 0,
	.pwm_vs_flag = 1,
	.pwm_clk = NULL,
	.pwm_misc = pwm_misc_dft,
	.pwm_reg = pwm_reg_dft,
	.pwm_cnt = 6,
	.pwm_ao_clk = NULL,
	.pwm_ao_misc = pwm_ao_misc_dft,
	.pwm_ao_reg = pwm_ao_reg_dft,
	.pwm_ao_cnt = 2,
};

static struct bl_pwm_ctrl_config_s bl_pwm_ctrl_conf_t7 = {
	.pwm_div_flag = 0,
	.pwm_vs_flag = 1,
	.pwm_clk = pwm_clk_ctrl_t7,
	.pwm_misc = pwm_misc_t7,
	.pwm_reg = pwm_reg_t7,
	.pwm_cnt = 6,
	.pwm_ao_clk = pwm_ao_clk_ctrl_t7,
	.pwm_ao_misc = pwm_ao_misc_t7,
	.pwm_ao_reg = pwm_ao_reg_t7,
	.pwm_ao_cnt = 8,
};

static struct bl_pwm_ctrl_config_s bl_pwm_ctrl_conf_t3 = {
	.pwm_div_flag = 1,
	.pwm_vs_flag = 1,
	.pwm_clk = pwm_clk_ctrl_t3,
	.pwm_misc = pwm_misc_t3,
	.pwm_reg = pwm_reg_t3,
	.pwm_cnt = 10,
	.pwm_ao_clk = NULL,
	.pwm_ao_misc = NULL,
	.pwm_ao_reg = NULL,
	.pwm_ao_cnt = 0,
};

static char *bl_pwm_name[] = {
	"PWM_A",
	"PWM_B",
	"PWM_C",
	"PWM_D",
	"PWM_E",
	"PWM_F",
	"PWM_G",
	"PWM_H",
	"PWM_I",
	"PWM_J"
};

static char *bl_pwm_ao_name[] = {
	"PWM_AO_A",
	"PWM_AO_B",
	"PWM_AO_C",
	"PWM_AO_D",
	"PWM_AO_E",
	"PWM_AO_F",
	"PWM_AO_G",
	"PWM_AO_H"
};

static char bl_pwm_vs_name[] = {"PWM_VS"};
static char bl_pwm_invalid_name[] = {"invalid"};

enum bl_pwm_port_e bl_pwm_str_to_num(const char *str)
{
	enum bl_pwm_port_e pwm_port = BL_PWM_MAX;
	int i, cnt;

	if (!bl_pwm_ctrl_conf)
		return BL_PWM_MAX;

	cnt = bl_pwm_ctrl_conf->pwm_cnt;
	if (cnt > ARRAY_SIZE(bl_pwm_name))
		cnt = ARRAY_SIZE(bl_pwm_name);
	for (i = 0; i < cnt; i++) {
		if (strcmp(str, bl_pwm_name[i]) == 0) {
			pwm_port = i + BL_PWM_A;
			return pwm_port;
		}
	}

	cnt = bl_pwm_ctrl_conf->pwm_ao_cnt;
	if (cnt > ARRAY_SIZE(bl_pwm_ao_name))
		cnt = ARRAY_SIZE(bl_pwm_ao_name);
	for (i = 0; i < cnt; i++) {
		if (strcmp(str, bl_pwm_ao_name[i]) == 0) {
			pwm_port = i + BL_PWM_AO_A;
			return pwm_port;
		}
	}

	if (strcmp(str, bl_pwm_vs_name) == 0) {
		pwm_port = BL_PWM_VS;
		return pwm_port;
	}

	return BL_PWM_MAX;
}

char *bl_pwm_num_to_str(unsigned int num)
{
	unsigned int temp, cnt;

	if (num < BL_PWM_AO_A) {
		temp = num - BL_PWM_A;
		cnt = ARRAY_SIZE(bl_pwm_name);
		if (temp >= cnt)
			return bl_pwm_invalid_name;
		return bl_pwm_name[temp];
	} else if (num < BL_PWM_VS) {
		temp = num - BL_PWM_AO_A;
		cnt = ARRAY_SIZE(bl_pwm_ao_name);
		if (temp >= cnt)
			return bl_pwm_invalid_name;
		return bl_pwm_ao_name[temp];
	} else if (num == BL_PWM_VS) {
		return bl_pwm_vs_name;
	}

	return bl_pwm_invalid_name;
}

static void bl_pwm_set_duty(struct bl_pwm_config_s *bl_pwm)
{
	unsigned int *pwm_reg;
	int port;

	if (!bl_pwm_ctrl_conf)
		return;

	switch (bl_pwm->pwm_port) {
	case BL_PWM_A:
	case BL_PWM_B:
	case BL_PWM_C:
	case BL_PWM_D:
	case BL_PWM_E:
	case BL_PWM_F:
	case BL_PWM_G:
	case BL_PWM_H:
	case BL_PWM_I:
	case BL_PWM_J:
		port = bl_pwm->pwm_port - BL_PWM_A;
		if (port >= bl_pwm_ctrl_conf->pwm_cnt) {
			BLERR("invalid pwm_port 0x%x\n", bl_pwm->pwm_port);
			return;
		}
		pwm_reg = bl_pwm_ctrl_conf->pwm_reg;
		break;
	case BL_PWM_AO_A:
	case BL_PWM_AO_B:
	case BL_PWM_AO_C:
	case BL_PWM_AO_D:
	case BL_PWM_AO_E:
	case BL_PWM_AO_F:
	case BL_PWM_AO_G:
	case BL_PWM_AO_H:
		port = bl_pwm->pwm_port - BL_PWM_AO_A;
		if (port >= bl_pwm_ctrl_conf->pwm_ao_cnt) {
			BLERR("invalid pwm_port 0x%x\n", bl_pwm->pwm_port);
			return;
		}
		pwm_reg = bl_pwm_ctrl_conf->pwm_ao_reg;
		break;
	default:
		return;
	}

	lcd_cbus_write(pwm_reg[port], (bl_pwm->pwm_hi << 16) | bl_pwm->pwm_lo);
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		BLPR("pwm_reg=0x%08x\n", lcd_cbus_read(pwm_reg[port]));
}

void bl_set_pwm(struct bl_pwm_config_s *bl_pwm)
{
	unsigned int port = bl_pwm->pwm_port;
	unsigned int vs[8], ve[8], sw, n, i, pol = 0;

	if (!bl_pwm_ctrl_conf)
		return;

	switch (bl_pwm->pwm_method) {
	case BL_PWM_POSITIVE:
		bl_pwm->pwm_hi = bl_pwm->pwm_level;
		bl_pwm->pwm_lo = bl_pwm->pwm_cnt - bl_pwm->pwm_level;
		pol = 0;
		break;
	case BL_PWM_NEGATIVE:
		bl_pwm->pwm_lo = bl_pwm->pwm_level;
		bl_pwm->pwm_hi = bl_pwm->pwm_cnt - bl_pwm->pwm_level;
		pol = 1;
		break;
	default:
		BLERR("port %d: invalid pwm_method %d\n",
		       port, bl_pwm->pwm_method);
		break;
	}

	switch (port) {
	case BL_PWM_VS:
		bl_pwm->pwm_hi = bl_pwm->pwm_level;
		n = bl_pwm->pwm_freq;
		sw = (bl_pwm->pwm_cnt * 10 / n + 5) / 10;
		bl_pwm->pwm_hi = (bl_pwm->pwm_hi * 10 / n + 5) / 10;
		bl_pwm->pwm_hi = (bl_pwm->pwm_hi > 1) ? bl_pwm->pwm_hi : 1;
		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
			BLPR("n=%d, sw=%d, pwm_high=%d\n", n, sw, bl_pwm->pwm_hi);
		for (i = 0; i < n; i++) {
			vs[i] = 1 + (sw * i);
			ve[i] = vs[i] + bl_pwm->pwm_hi - 1;
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				BLPR("vs[%d]=%d, ve[%d]=%d\n", i, vs[i], i, ve[i]);
		}
		for (i = n; i < 8; i++) {
			vs[i] = 0x1fff;
			ve[i] = 0x1fff;
		}
		lcd_vcbus_write(VPU_VPU_PWM_V0, (pol << 31) | (ve[0] << 16) | (vs[0]));
		lcd_vcbus_write(VPU_VPU_PWM_V1, (ve[1] << 16) | (vs[1]));
		lcd_vcbus_write(VPU_VPU_PWM_V2, (ve[2] << 16) | (vs[2]));
		lcd_vcbus_write(VPU_VPU_PWM_V3, (ve[3] << 16) | (vs[3]));
		if (bl_pwm_ctrl_conf->pwm_vs_flag) {
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				BLPR("pwm_vs_flag support\n");
			lcd_vcbus_setb(VPU_VPU_PWM_H0, 1, 31, 1);
			lcd_vcbus_setb(VPU_VPU_PWM_V0, vs[4], 0, 13);
			lcd_vcbus_setb(VPU_VPU_PWM_V0, ve[4], 16, 13);
			lcd_vcbus_write(VPU_VPU_PWM_V1, (ve[5] << 16) | (vs[5]));
			lcd_vcbus_write(VPU_VPU_PWM_V2, (ve[6] << 16) | (vs[6]));
			lcd_vcbus_write(VPU_VPU_PWM_V3, (ve[7] << 16) | (vs[7]));
			lcd_vcbus_setb(VPU_VPU_PWM_H0, 0, 31, 1);
		}
		break;
	default:
		bl_pwm_set_duty(bl_pwm);
		break;
	}
}

static unsigned int bl_level_mapping(struct bl_config_s *bconf, unsigned int level)
{
	unsigned int mid = bconf->level_mid;
	unsigned int mid_map =bconf->level_mid_mapping;
	unsigned int max = bconf->level_max;
	unsigned int min = bconf->level_min;

	if (mid == mid_map)
		return level;

	level = level > max ? max : level;
	if ((level >= mid) && (level <= max))
		level = (((level - mid) * (max - mid_map)) / (max - mid)) + mid_map;
	else if ((level >= min) && (level < mid))
		level = (((level - min) * (mid_map - min)) / (mid - min)) + min;
	else
		level = 0;

	return level;
}

void bl_pwm_set_level(struct aml_bl_drv_s *bdrv,
		     struct bl_pwm_config_s *bl_pwm, unsigned int level)
{
	unsigned int min = bl_pwm->level_min;
	unsigned int max = bl_pwm->level_max;
	unsigned int pwm_max = bl_pwm->pwm_max;
	unsigned int pwm_min = bl_pwm->pwm_min;

	level = bl_level_mapping(&bdrv->config, level);
	max = bl_level_mapping(&bdrv->config, max);
	min = bl_level_mapping(&bdrv->config, min);
	if ((max <= min) || (level < min))
		bl_pwm->pwm_level = pwm_min;
	else
		bl_pwm->pwm_level =
		(pwm_max - pwm_min) * (level - min) / (max - min) + pwm_min;

	if (bl_pwm->pwm_duty_max > 255)
		bl_pwm->pwm_duty = bl_pwm->pwm_level * 4095 / bl_pwm->pwm_cnt;
	else if (bl_pwm->pwm_duty_max > 100)
		bl_pwm->pwm_duty = bl_pwm->pwm_level * 255 / bl_pwm->pwm_cnt;
	else
		bl_pwm->pwm_duty =
		((bl_pwm->pwm_level * 1000 / bl_pwm->pwm_cnt) + 5) / 10;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		BLPR("pwm_port 0x%x: level=%d, level_max=%d, level_min=%d, pwm_max=%d, pwm_min=%d, pwm_level=%d, duty=%d%%\n",
		     bl_pwm->pwm_port, level, max, min,
		     pwm_max, pwm_min, bl_pwm->pwm_level,
		     bl_pwm->pwm_duty);
	}

	if (bdrv->state > 0)
		bl_set_pwm_gpio_check(bdrv, bl_pwm);
	bl_set_pwm(bl_pwm);
}

void bl_pwm_en(struct bl_pwm_config_s *bl_pwm, int flag)
{
	struct bl_pwm_clkctrl_s *pwm_clk;
	struct bl_pwm_misc_s *pwm_misc;
	unsigned int port, pre_div;

	if (!bl_pwm_ctrl_conf)
		return;

	pre_div = bl_pwm->pwm_pre_div;
	switch (bl_pwm->pwm_port) {
	case BL_PWM_A:
	case BL_PWM_B:
	case BL_PWM_C:
	case BL_PWM_D:
	case BL_PWM_E:
	case BL_PWM_F:
	case BL_PWM_G:
	case BL_PWM_H:
	case BL_PWM_I:
	case BL_PWM_J:
		port = bl_pwm->pwm_port - BL_PWM_A;
		if (port >= bl_pwm_ctrl_conf->pwm_cnt) {
			BLERR("invalid pwm_port 0x%x\n", bl_pwm->pwm_port);
			return;
		}
		pwm_clk = bl_pwm_ctrl_conf->pwm_clk;
		pwm_misc = bl_pwm_ctrl_conf->pwm_misc;
		break;
	case BL_PWM_AO_A:
	case BL_PWM_AO_B:
	case BL_PWM_AO_C:
	case BL_PWM_AO_D:
	case BL_PWM_AO_E:
	case BL_PWM_AO_F:
	case BL_PWM_AO_G:
	case BL_PWM_AO_H:
		port = bl_pwm->pwm_port - BL_PWM_AO_A;
		if (port >= bl_pwm_ctrl_conf->pwm_ao_cnt) {
			BLERR("invalid pwm_port 0x%x\n", bl_pwm->pwm_port);
			return;
		}
		pwm_clk = bl_pwm_ctrl_conf->pwm_ao_clk;
		pwm_misc = bl_pwm_ctrl_conf->pwm_ao_misc;
		break;
	default:
		return;
	}

	if (flag) {
		if (pwm_clk) {
			if (bl_pwm_ctrl_conf->pwm_div_flag)
				lcd_cbus_setb(pwm_clk[port].reg, pre_div, pwm_clk[port].bit_div, 2);
			else
				lcd_cbus_setb(pwm_clk[port].reg, 0, pwm_clk[port].bit_div, 2);
			lcd_cbus_setb(pwm_clk[port].reg, 0, pwm_clk[port].bit_sel, 8);
			lcd_cbus_setb(pwm_clk[port].reg, 1, pwm_clk[port].bit_en, 1);
		}
		if (bl_pwm_ctrl_conf->pwm_div_flag == 0)
			lcd_cbus_setb(pwm_misc[port].reg, pre_div, pwm_misc[port].bit_pre_div, 7);
		lcd_cbus_setb(pwm_misc[port].reg, pre_div, pwm_misc[port].bit_pre_div, 7);
		/* pwm clk_sel */
		lcd_cbus_setb(pwm_misc[port].reg, 0, pwm_misc[port].bit_clk_sel, 2);
		/* pwm clk_en */
		lcd_cbus_setb(pwm_misc[port].reg, 1, pwm_misc[port].bit_clk_en, 1);
		/* pwm enable */
		lcd_cbus_setb(pwm_misc[port].reg, pwm_misc[port].val_pwm_en,
			      pwm_misc[port].bit_pwm_en, 1);
	} else {
		/* pwm clk_disable */
		lcd_cbus_setb(pwm_misc[port].reg, 0, pwm_misc[port].bit_clk_en, 1);
		if (pwm_clk)
			lcd_cbus_setb(pwm_clk[port].reg, 0, pwm_clk[port].bit_en, 1);
	}
}

void bl_pwm_config_init(struct bl_pwm_config_s *bl_pwm)
{
	unsigned int freq, pre_div, cnt;
	int i;

	if (!bl_pwm) {
		BLERR("%s: bl_pwm is NULL\n", __func__);
		return;
	}
	if (bl_pwm->pwm_port >= BL_PWM_MAX)
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		BLPR("%s pwm_port 0x%x: freq = %u\n",
		     __func__, bl_pwm->pwm_port, bl_pwm->pwm_freq);
	}
	freq = bl_pwm->pwm_freq;
	switch (bl_pwm->pwm_port) {
	case BL_PWM_VS:
		cnt = lcd_vcbus_read(ENCL_VIDEO_MAX_LNCNT) + 1;
		bl_pwm->pwm_cnt = cnt;
		bl_pwm->pwm_pre_div = 0;
		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
			BLPR("pwm_cnt = %u\n", bl_pwm->pwm_cnt);
		break;
	default:
		for (i = 0; i < 0x7f; i++) {
			pre_div = i;
			cnt = XTAL_FREQ_HZ / (freq * (pre_div + 1)) - 2;
			if (cnt <= 0xffff) /* 16bit */
				break;
		}
		bl_pwm->pwm_cnt = cnt;
		bl_pwm->pwm_pre_div = pre_div;
		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
			BLPR("pwm_cnt = %u, pwm_pre_div = %u\n", cnt, pre_div);
		break;
	}

	if (bl_pwm->pwm_duty_max > 255) {
		bl_pwm->pwm_max = (bl_pwm->pwm_cnt * bl_pwm->pwm_duty_max / 4095);
		bl_pwm->pwm_min = (bl_pwm->pwm_cnt * bl_pwm->pwm_duty_min / 4095);
	} else if (bl_pwm->pwm_duty_max > 100) {
		bl_pwm->pwm_max = (bl_pwm->pwm_cnt * bl_pwm->pwm_duty_max / 255);
		bl_pwm->pwm_min = (bl_pwm->pwm_cnt * bl_pwm->pwm_duty_min / 255);
	} else {
		bl_pwm->pwm_max = (bl_pwm->pwm_cnt * bl_pwm->pwm_duty_max / 100);
		bl_pwm->pwm_min = (bl_pwm->pwm_cnt * bl_pwm->pwm_duty_min / 100);
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		BLPR("pwm_max = %u, pwm_min = %u\n",
		      bl_pwm->pwm_max, bl_pwm->pwm_min);
}

void bl_pwm_reg_print(struct bl_pwm_config_s *bl_pwm)
{
	unsigned int *pwm_reg;
	int port;

	if (!bl_pwm_ctrl_conf)
		return;

	switch (bl_pwm->pwm_port) {
	case BL_PWM_A:
	case BL_PWM_B:
	case BL_PWM_C:
	case BL_PWM_D:
	case BL_PWM_E:
	case BL_PWM_F:
	case BL_PWM_G:
	case BL_PWM_H:
	case BL_PWM_I:
	case BL_PWM_J:
		port = bl_pwm->pwm_port - BL_PWM_A;
		if (port >= bl_pwm_ctrl_conf->pwm_cnt) {
			BLERR("invalid pwm_port 0x%x\n", bl_pwm->pwm_port);
			return;
		}
		pwm_reg = bl_pwm_ctrl_conf->pwm_reg;
		break;
	case BL_PWM_AO_A:
	case BL_PWM_AO_B:
	case BL_PWM_AO_C:
	case BL_PWM_AO_D:
	case BL_PWM_AO_E:
	case BL_PWM_AO_F:
	case BL_PWM_AO_G:
	case BL_PWM_AO_H:
		port = bl_pwm->pwm_port - BL_PWM_AO_A;
		if (port >= bl_pwm_ctrl_conf->pwm_ao_cnt) {
			BLERR("invalid pwm_port 0x%x\n", bl_pwm->pwm_port);
			return;
		}
		pwm_reg = bl_pwm_ctrl_conf->pwm_ao_reg;
		break;
	default:
		return;
	}

	BLPR("pwm_reg    = 0x%08x\n", lcd_cbus_read(pwm_reg[port]));
}

int aml_bl_pwm_reg_config_init(struct aml_lcd_data_s *pdata)
{
	switch (pdata->chip_type) {
	case LCD_CHIP_G12A:
	case LCD_CHIP_G12B:
	case LCD_CHIP_SM1:
		bl_pwm_ctrl_conf = &bl_pwm_ctrl_conf_dft;
		break;
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
		bl_pwm_ctrl_conf = &bl_pwm_ctrl_conf_tl1;
		break;
	case LCD_CHIP_T7:
		bl_pwm_ctrl_conf = &bl_pwm_ctrl_conf_t7;
		break;
	case LCD_CHIP_T3:
		bl_pwm_ctrl_conf = &bl_pwm_ctrl_conf_t3;
		break;
	default:
		bl_pwm_ctrl_conf = NULL;
		break;
	}

	return 0;
}
