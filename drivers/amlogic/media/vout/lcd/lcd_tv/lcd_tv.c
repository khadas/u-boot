// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/gpio.h>
#include <fdtdec.h>
#include <amlogic/media/vout/aml_vmode.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "../lcd_reg.h"
#include "../lcd_common.h"
#include "lcd_tv.h"

/* ************************************************** *
   lcd mode function
 * ************************************************** */
#define ACTIVE_FRAME_RATE_CNT     3
#define LCD_STD_FRAME_RATE_MAX    5
static struct lcd_duration_s lcd_std_fr[] = {
	{60,  60,     1,    0},
	{59,  60000,  1001, 1},
	{50,  50,     1,    0},
	{48,  48,     1,    0},
	{47,  48000,  1001, 1},
	{60,  60,     1,    0}
};

static struct lcd_duration_s lcd_std_fr_high[] = {
	{120, 120,    1,    0},
	{119, 120000, 1001, 1},
	{100, 100,    1,    0},
	{96,  96,     1,    0},
	{95,  96000,  1001, 1},
	{120, 120,    1,    0}
};

struct lcd_vmode_info_s {
	char *name;
	enum vmode_e mode;
	unsigned int width;
	unsigned int height;
	unsigned int frame_rate;
	unsigned int frac;
	struct lcd_duration_s *duration;
};

enum lcd_vmode_e {
	LCD_VMODE_600P = 0,
	LCD_VMODE_768P,
	LCD_VMODE_1080P,
	LCD_VMODE_2160P,
	LCD_VMODE_3840_1080P,
	LCD_VMODE_MAX,
};

static struct lcd_vmode_info_s lcd_vmode_info[] = {
	{
		.name              = "600p",
		.mode              = VMODE_LCD,
		.width             = 1024,
		.height            = 600,
		.frame_rate        = 60,
		.frac              = 0,
		.duration          = lcd_std_fr,
	},
	{
		.name              = "768p",
		.mode              = VMODE_LCD,
		.width             = 1366,
		.height            = 768,
		.frame_rate        = 60,
		.frac              = 0,
		.duration          = lcd_std_fr,
	},
	{
		.name              = "1080p",
		.mode              = VMODE_LCD,
		.width             = 1920,
		.height            = 1080,
		.frame_rate        = 60,
		.frac              = 0,
		.duration          = lcd_std_fr,
	},
	{
		.name              = "2160p",
		.mode              = VMODE_LCD,
		.width             = 3840,
		.height            = 2160,
		.frame_rate        = 60,
		.frac              = 0,
		.duration          = lcd_std_fr,
	},
	{
		.name              = "3840x1080p",
		.mode              = VMODE_LCD,
		.width             = 3840,
		.height            = 1080,
		.frame_rate        = 120,
		.frac              = 0,
		.duration          = lcd_std_fr_high,
	},
	{
		.name              = "invalid",
		.mode              = VMODE_INIT_NULL,
		.width             = 1920,
		.height            = 1080,
		.frame_rate        = 60,
		.frac              = 0,
		.duration          = lcd_std_fr,
	},
};

static int lcd_output_vmode_init(struct aml_lcd_drv_s *pdrv)
{
	int i, count = ARRAY_SIZE(lcd_vmode_info) - 1;

	if (!pdrv)
		return -1;

	for (i = 0; i < count; i++) {
		if (pdrv->config.basic.h_active == lcd_vmode_info[i].width &&
		    pdrv->config.basic.v_active == lcd_vmode_info[i].height) {
			pdrv->output_vmode = i;
			pdrv->std_duration = lcd_vmode_info[i].duration;
			return 0;
		}
	}

	pdrv->output_vmode = LCD_VMODE_MAX;
	pdrv->std_duration = lcd_std_fr;
	LCDERR("[%d]: %s: unsupport resolution: %dx%d\n",
	       pdrv->index, __func__,
	       pdrv->config.basic.h_active, pdrv->config.basic.v_active);
	return -1;
}

static int lcd_outputmode_is_mached(struct aml_lcd_drv_s *pdrv, const char *mode)
{
	char temp[30], *p;
	int n;

	p = strchr(mode, 'p');
	if (!p)
		return -1;
	n = p - mode + 1;
	strncpy(temp, mode, n);
	temp[n] = '\0';
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: outputmode=%s, lcd_vmode=%s\n", pdrv->index, mode, temp);

	if (pdrv->output_vmode >= LCD_VMODE_MAX)
		return -1;

	if (strcmp(temp, lcd_vmode_info[pdrv->output_vmode].name) == 0)
		return 0;

	LCDERR("[%d]: %s: invalid mode: %s\n", pdrv->index, __func__, mode);
	return -1;
}

static int lcd_outputmode_to_frame_rate(struct aml_lcd_drv_s *pdrv, const char *mode)
{
	int frame_rate = 0;
	char temp[30], *p;
	int n, i;

	p = strchr(mode, 'p');
	if (p == NULL)
		return 0;
	n = p - mode + 1;
	strncpy(temp, mode+n, (strlen(mode)-n));
	p = strchr(temp, 'h');
	if (p == NULL)
		return 0;
	*p = '\0';
	frame_rate = (int)simple_strtoul(temp, NULL, 10);
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: outputmode=%s, frame_rate=%d\n", pdrv->index, mode, frame_rate);

	for (i = 0; i < LCD_STD_FRAME_RATE_MAX; i++) {
		if (frame_rate == pdrv->std_duration[i].frame_rate)
			return frame_rate;
	}
	return 0; /* invalid frame_rate */
}

static unsigned int lcd_std_frame_rate_index(struct lcd_vmode_info_s *info)
{
	unsigned int i;

	for (i = 0; i < LCD_STD_FRAME_RATE_MAX; i++) {
		if (info->frame_rate == info->duration[i].frame_rate)
			return i;
	}

	LCDERR("%s: invalid frame_rate: %d\n", __func__, info->frame_rate);
	return LCD_STD_FRAME_RATE_MAX;
}

static int check_lcd_output_mode(struct aml_lcd_drv_s *pdrv, char *mode,
				 unsigned int frac)
{
	struct lcd_vmode_info_s *info;
	int frame_rate;
	int ret;

	if (!mode)
		return -1;

	if (pdrv->output_vmode >= LCD_VMODE_MAX)
		return -1;

	ret = lcd_outputmode_is_mached(pdrv, mode);
	if (ret)
		return -1;

	info = &lcd_vmode_info[pdrv->output_vmode];
	frame_rate = lcd_outputmode_to_frame_rate(pdrv, mode);
	if (frame_rate == 0) {
		LCDERR("[%d]: %s: frame_rate is not support\n",
		       pdrv->index, __func__);
		return -1;
	}
	if (frac) {
		if (frame_rate != 60 && frame_rate != 48 &&
		    frame_rate != 120 && frame_rate != 96) {
			LCDERR("[%d]: %s: don't support frac under mode %s\n",
			       pdrv->index, __func__, mode);
			return -1;
		}
		info->frac = 1;
	} else {
		info->frac = 0;
	}
	info->frame_rate = frame_rate;

	return 0;
}

static void lcd_list_support_mode(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_vmode_info_s *info;
	int i;

	info = &lcd_vmode_info[pdrv->output_vmode];
	for (i = 0; i < ACTIVE_FRAME_RATE_CNT; i++)
		printf("%s%dhz\n", info->name, pdrv->std_duration[i].frame_rate);
}

static void lcd_config_init(struct aml_lcd_drv_s *pdrv)
{
	if (pdrv->config.timing.lcd_clk == 0) {/* default 0 for 60hz */
		pdrv->config.timing.lcd_clk = 60;
	} else {
		LCDPR("[%d]: custom clk: %d\n",
		      pdrv->index, pdrv->config.timing.lcd_clk);
	}

	lcd_basic_timing_range_update(pdrv);
	lcd_timing_init_config(&pdrv->config);
	lcd_output_vmode_init(pdrv);
}

static int lcd_outputmode_check(struct aml_lcd_drv_s *pdrv, char *mode, unsigned int frac)
{
	int ret;

	ret = check_lcd_output_mode(pdrv, mode, frac);

	return ret;
}

static int lcd_config_check(struct aml_lcd_drv_s *pdrv, char *mode, unsigned int frac)
{
	struct lcd_config_s *pconf = &pdrv->config;
	struct lcd_vmode_info_s *info;
	int index, ret, frame_rate;

	ret = check_lcd_output_mode(pdrv, mode, frac);
	if (ret)
		return -1;

	info = &lcd_vmode_info[pdrv->output_vmode];
	frame_rate = lcd_outputmode_to_frame_rate(pdrv, mode);
	if (frame_rate == 0) {
		LCDERR("[%d]: %s: frame_rate is not support\n",
		       pdrv->index, __func__);
		return -1;
	}

	info->frame_rate = frame_rate;
	index = lcd_std_frame_rate_index(info);
	if (info->frac) {
		if (index < LCD_STD_FRAME_RATE_MAX)
			index++;
	}
	pconf->timing.sync_duration_num = pdrv->std_duration[index].duration_num;
	pconf->timing.sync_duration_den = pdrv->std_duration[index].duration_den;

	/* update clk & timing config */
	lcd_vmode_change(pdrv);
	lcd_tv_config_update(pdrv);
	lcd_clk_generate_parameter(pdrv);

	return 0;
}

int lcd_mode_tv_init(struct aml_lcd_drv_s *pdrv)
{
	pdrv->list_support_mode = lcd_list_support_mode;
	pdrv->outputmode_check = lcd_outputmode_check;
	pdrv->config_check = lcd_config_check;
	pdrv->driver_init_pre = lcd_tv_driver_init_pre;
	pdrv->driver_init = lcd_tv_driver_init;
	pdrv->driver_disable = lcd_tv_driver_disable;

	lcd_config_init(pdrv);

	return 0;
}


