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

static char *lcd_ss_freq_table_dft[] = {
	"0, 29.5KHz",
	"1, 31.5KHz",
	"2, 50KHz",
	"3, 75KHz",
	"4, 100KHz",
	"5, 150KHz",
	"6, 200KHz",
};

static char *lcd_ss_mode_table_dft[] = {
	"0, center ss",
	"1, up ss",
	"2, down ss",
};

struct lcd_clk_config_s *get_lcd_clk_config(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	if (!pdrv)
		return NULL;

	if (!pdrv->clk_conf) {
		LCDERR("[%d]: %s: clk_config is null\n", pdrv->index, __func__);
		return NULL;
	}
	cconf = (struct lcd_clk_config_s *)pdrv->clk_conf;
	if (!cconf->data) {
		LCDERR("[%d]: %s: clk config data is null\n",
		       pdrv->index, __func__);
		return NULL;
	}

	return cconf;
}

/* ****************************************************
 * lcd clk function api
 * ****************************************************
 */
void lcd_clk_generate_parameter(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int ss_level;
	unsigned int ss_freq;
	unsigned int ss_mode;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf || !cconf->data)
		return;

	if (cconf->data->clk_generate_parameter)
		cconf->data->clk_generate_parameter(pdrv);

	ss_level = pconf->timing.ss_level & 0xff;
	cconf->ss_level = (ss_level >= cconf->data->ss_level_max) ?
				ss_level >= cconf->data->ss_level_max : ss_level;

	ss_freq = (pconf->timing.ss_level >> 8) & 0xff;
	cconf->ss_freq = (ss_freq >= cconf->data->ss_freq_max) ?
				cconf->data->ss_freq_max : ss_freq;

	ss_mode = (pconf->timing.ss_level >> 12) & 0xff;
	cconf->ss_mode = (ss_mode >= cconf->data->ss_mode_max) ? 0 : ss_mode;
	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2) {
		LCDPR("[%d]: %s: ss_level=%d, ss_freq=%d, ss_mode=%d\n",
		      pdrv->index, __func__,
		      cconf->ss_level, cconf->ss_freq, cconf->ss_mode);
	}
}

void lcd_get_ss(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf || !cconf->data)
		return;
	if (cconf->data->ss_support == 0) {
		printf("[%d]: spread spectrum is not support\n", pdrv->index);
		return;
	}

	printf("ss_level: %d, %dppm, dep_sel=%d, str_m=%d\n",
		cconf->ss_level, cconf->ss_ppm,
		cconf->ss_dep_sel, cconf->ss_str_m);
	printf("ss_freq: %d, %s\n", cconf->ss_freq, lcd_ss_freq_table_dft[cconf->ss_freq]);
	printf("ss_mode: %d, %s\n", cconf->ss_mode, lcd_ss_mode_table_dft[cconf->ss_mode]);
}

int lcd_set_ss(struct aml_lcd_drv_s *pdrv, unsigned int level,
	       unsigned int freq, unsigned int mode)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf || !cconf->data)
		return -1;
	if (cconf->data->ss_support == 0) {
		LCDERR("[%d]: %s: not support\n", pdrv->index, __func__);
		return -1;
	}

	if (level < 0xff) {
		if (level > cconf->data->ss_level_max) {
			LCDERR("%s: ss_level %d is out of support (max %d)\n",
			       __func__, level, cconf->data->ss_level_max);
			return -1;
		}
	}
	if (freq < 0xff) {
		if (freq > cconf->data->ss_freq_max) {
			LCDERR("%s: ss_freq %d is out of support (max %d)\n",
			       __func__, freq, cconf->data->ss_freq_max);
			return -1;
		}
	}
	if (mode < 0xff) {
		if (mode > cconf->data->ss_mode_max) {
			LCDERR("%s: ss_mode %d is out of support (max %d)\n",
			       __func__, mode, cconf->data->ss_mode_max);
			return -1;
		}
	}

	if (cconf->data->set_ss_level) {
		if (level < 0xff) {
			cconf->ss_level = level;
			cconf->data->set_ss_level(pdrv);
		}
	}

	if (cconf->data->set_ss_advance) {
		if (freq == 0xff && mode == 0xff)
			goto lcd_set_ss_end;
		if (freq < 0xff)
			cconf->ss_freq = freq;
		if (mode < 0xff)
			cconf->ss_mode = mode;
		cconf->data->set_ss_advance(pdrv);
	}

lcd_set_ss_end:
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	return 0;
}

/* for frame rate change */
void lcd_update_clk(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	struct lcd_clk_ctrl_s *table;
	unsigned int offset, reg, val;
	int i = 0;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf || !cconf->data)
		return;

	if (cconf->data->pll_frac_generate)
		cconf->data->pll_frac_generate(pdrv);

	offset = cconf->pll_offset;

	if (!cconf->data->pll_ctrl_table)
		return;
	table = cconf->data->pll_ctrl_table;
	while (i < LCD_CLK_CTRL_CNT_MAX) {
		if (table[i].flag == LCD_CLK_CTRL_END)
			break;
		if (table[i].flag == LCD_CLK_CTRL_FRAC) {
			reg = table[i].reg + offset;
			val = lcd_ana_read(reg);
			lcd_ana_setb(reg, cconf->pll_frac, table[i].bit, table[i].len);
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("[%d]: %s: pll_frac reg 0x%x: 0x%08x->0x%08x\n",
					pdrv->index, __func__, reg,
					val, lcd_ana_read(reg));
			}
		}
		i++;
	}

	LCDPR("[%d]: %s: pll_frac=0x%x\n", pdrv->index, __func__, cconf->pll_frac);
}

/* for timing change */
void lcd_set_clk(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	int cnt = 0;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf || !cconf->data)
		return;

#ifdef CONFIG_AML_LCD_PXP
	if (cconf->data->vclk_crt_set)
		cconf->data->vclk_crt_set(pdrv);
	return;
#endif
lcd_set_clk_retry:
	if (cconf->data->clk_set)
		cconf->data->clk_set(pdrv);
	if (cconf->data->vclk_crt_set)
		cconf->data->vclk_crt_set(pdrv);
	mdelay(10);

	while (lcd_clk_msr_check(cconf->data->enc_clk_msr_id, cconf->fout)) {
		if (cnt++ >= 10) {
			LCDERR("[%d]: %s timeout\n", pdrv->index, __func__);
			break;
		}
		goto lcd_set_clk_retry;
	}

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
}

void lcd_disable_clk(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf || !cconf->data)
		return;

	if (cconf->data->clk_disable)
		cconf->data->clk_disable(pdrv);

	LCDPR("[%d]: %s\n", pdrv->index, __func__);
}

void lcd_clk_config_print(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf || !cconf->data)
		return;

	if (cconf->data->clk_config_print)
		cconf->data->clk_config_print(pdrv);
}

int aml_lcd_prbs_test(struct aml_lcd_drv_s *pdrv, unsigned int ms, unsigned int mode_flag)
{
	struct lcd_clk_config_s *cconf;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf || !cconf->data)
		return 0;

	if (cconf->data->prbs_test)
		cconf->data->prbs_test(pdrv, ms, mode_flag);
	return 0;
}

void lcd_clk_config_chip_init(struct aml_lcd_drv_s *pdrv, struct lcd_clk_config_s *cconf)
{
	unsigned int i;
	struct lcd_clk_config_s *pclk_conf;
	unsigned int clk_mode = pdrv->config.timing.clk_mode;
	unsigned int loop_num = 1;

	if (clk_mode == LCD_CLK_MODE_INDEPENDENCE)
		loop_num = 2;

	for (i = 0; i < loop_num; i++) {
		pclk_conf = &cconf[i];
		pclk_conf->pll_id = 0;
		pclk_conf->pll_offset = 0;
		pclk_conf->fin = FIN_FREQ;
	}
	switch (pdrv->data->chip_type) {
	case LCD_CHIP_G12A:
	case LCD_CHIP_SM1:
		lcd_clk_config_chip_init_g12a(pdrv, cconf);
		break;
	case LCD_CHIP_G12B:
		lcd_clk_config_chip_init_g12b(pdrv, cconf);
		break;
#ifndef CONFIG_AMLOGIC_REMOVE_OLD
	case LCD_CHIP_TL1:
		lcd_clk_config_chip_init_tl1(pdrv, cconf);
		break;
#endif
	case LCD_CHIP_TM2:
		lcd_clk_config_chip_init_tm2(pdrv, cconf);
		break;
	case LCD_CHIP_T5:
		lcd_clk_config_chip_init_t5(pdrv, cconf);
		break;
	case LCD_CHIP_T5D:
		lcd_clk_config_chip_init_t5d(pdrv, cconf);
		break;
	case LCD_CHIP_T7:
		lcd_clk_config_chip_init_t7(pdrv, cconf);
		break;
	case LCD_CHIP_T5M: //the same as t3, but only support 1 driver
	case LCD_CHIP_T3: /* only one pll */
		lcd_clk_config_chip_init_t3(pdrv, cconf);
		break;
	case LCD_CHIP_T3X: /* only one pll */
		lcd_clk_config_chip_init_t3x(pdrv, cconf);
		break;
	case LCD_CHIP_T5W:
		lcd_clk_config_chip_init_t5w(pdrv, cconf);
		break;
	case LCD_CHIP_C3:
		lcd_clk_config_chip_init_c3(pdrv, cconf);
		break;
	case LCD_CHIP_A4:
		lcd_clk_config_chip_init_a4(pdrv, cconf);
		break;
	case LCD_CHIP_TXHD2:
		lcd_clk_config_chip_init_txhd2(pdrv, cconf);
		break;
	default:
		LCDPR("[%d]: %s: invalid chip type\n", pdrv->index, __func__);
		return;
	}

	cconf->pll_od_fb = cconf->data->pll_od_fb;
	if (lcd_debug_print_flag & LCD_DBG_PR_ADV2) {
		if (cconf->data->clk_config_init_print)
			cconf->data->clk_config_init_print(pdrv);
	}
}

void lcd_clk_config_probe(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_clk_config_s *cconf;
	unsigned int size = sizeof(struct lcd_clk_config_s);
	unsigned int clk_mode = pdrv->config.timing.clk_mode;

	if (clk_mode == LCD_CLK_MODE_INDEPENDENCE)
		size = 2 * sizeof(struct lcd_clk_config_s);

	if (!pdrv->clk_conf) {
		cconf = (struct lcd_clk_config_s *)malloc(size);
		if (!cconf) {
			LCDERR("[%d]: %s: Not enough memory\n", pdrv->index, __func__);
			return;
		}
		pdrv->clk_conf = (void *)cconf;
	} else {
		cconf = (struct lcd_clk_config_s *)pdrv->clk_conf;
	}
	memset(cconf, 0, size);

	lcd_clk_config_chip_init(pdrv, cconf);
}
