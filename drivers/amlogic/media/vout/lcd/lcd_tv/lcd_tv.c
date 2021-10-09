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
static unsigned int lcd_std_frame_rate[][3] = {
	{60, 60,    1},
	{59, 60000, 1001},
	{50, 50,    1},
	{48, 48,    1},
	{47, 48000, 1001},
	{60, 60,    1}
};

struct lcd_vmode_info_s {
	char *name;
	enum vmode_e mode;
	unsigned int width;
	unsigned int height;
	unsigned int frame_rate;
	unsigned int frac;
};

enum lcd_vmode_e {
	LCD_VMODE_600P = 0,
	LCD_VMODE_768P,
	LCD_VMODE_1080P,
	LCD_VMODE_2160P,
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
	},
	{
		.name              = "768p",
		.mode              = VMODE_LCD,
		.width             = 1366,
		.height            = 768,
		.frame_rate        = 60,
		.frac              = 0,
	},
	{
		.name              = "1080p",
		.mode              = VMODE_LCD,
		.width             = 1920,
		.height            = 1080,
		.frame_rate        = 60,
		.frac              = 0,
	},
	{
		.name              = "2160p",
		.mode              = VMODE_LCD,
		.width             = 3840,
		.height            = 2160,
		.frame_rate        = 60,
		.frac              = 0,
	},
	{
		.name              = "invalid",
		.mode              = VMODE_INIT_NULL,
		.width             = 1920,
		.height            = 1080,
		.frame_rate        = 60,
		.frac              = 0,
	},
};

static int lcd_vmode_is_mached(struct lcd_config_s *pconf, int index)
{
	if ((pconf->basic.h_active == lcd_vmode_info[index].width) &&
		(pconf->basic.v_active == lcd_vmode_info[index].height))
		return 0;
	else
		return -1;
}

static int lcd_outputmode_to_vmode(const char *mode)
{
	int lcd_vmode = LCD_VMODE_MAX;
	int i, count = ARRAY_SIZE(lcd_vmode_info) - 1;
	char temp[30], *p;
	int n;

	p = strchr(mode, 'p');
	if (p == NULL)
		return LCD_VMODE_MAX;
	n = p - mode + 1;
	strncpy(temp, mode, n);
	temp[n] = '\0';
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("outputmode=%s, lcd_vmode=%s\n", mode, temp);

	for (i = 0; i < count; i++) {
		if (strcmp(temp, lcd_vmode_info[i].name) == 0) {
			lcd_vmode = i;
			break;
		}
	}
	return lcd_vmode;
}

static int lcd_outputmode_to_frame_rate(const char *mode)
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
	n = (int)simple_strtoul(temp, NULL, 10);
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("outputmode=%s, frame_rate=%d\n", mode, n);

	for (i = 0; i < LCD_STD_FRAME_RATE_MAX; i++) {
		if (n == lcd_std_frame_rate[i][0]) {
			frame_rate = n;
			break;
		}
	}
	return frame_rate;
}

static unsigned int lcd_std_frame_rate_index(unsigned int frame_rate)
{
	unsigned int i;

	for (i = 0; i < LCD_STD_FRAME_RATE_MAX; i++) {
		if (frame_rate == lcd_std_frame_rate[i][0])
			return i;
	}

	LCDERR("%s: invalid frame_rate: %d\n", __func__, frame_rate);
	return LCD_STD_FRAME_RATE_MAX;
}

static int check_lcd_output_mode(struct aml_lcd_drv_s *pdrv, char *mode,
				 unsigned int frac)
{
	int lcd_vmode, frame_rate;
	int ret;

	if (!mode)
		return LCD_VMODE_MAX;

	lcd_vmode = lcd_outputmode_to_vmode(mode);
	if (lcd_vmode >= LCD_VMODE_MAX) {
		LCDERR("%s: outputmode %s is not support\n", __func__, mode);
		return LCD_VMODE_MAX;
	}

	ret = lcd_vmode_is_mached(&pdrv->config, lcd_vmode);
	if (ret) {
		LCDERR("outputmode[%s] and panel_type is not match\n",
			lcd_vmode_info[lcd_vmode].name);
		return LCD_VMODE_MAX;
	}

	frame_rate = lcd_outputmode_to_frame_rate(mode);
	if (frame_rate == 0) {
		LCDERR("%s: frame_rate is not support\n", __func__);
		return LCD_VMODE_MAX;
	}
	if (frac) {
		if (frame_rate != 60 && frame_rate != 48) {
			LCDERR("%s: don't support frac under mode %s\n",
			       __func__, mode);
			return LCD_VMODE_MAX;
		}
		lcd_vmode_info[lcd_vmode].frac = 1;
	} else {
		lcd_vmode_info[lcd_vmode].frac = 0;
	}
	lcd_vmode_info[lcd_vmode].frame_rate = frame_rate;

	return lcd_vmode;
}

static void lcd_list_support_mode(struct lcd_config_s *pconf)
{
	int i, j;
	char str[30];

	for (i = 0; i < (ARRAY_SIZE(lcd_vmode_info) - 1); i++) {
		if ((pconf->basic.h_active == lcd_vmode_info[i].width) &&
		    (pconf->basic.v_active == lcd_vmode_info[i].height)) {
			for (j = 0; j < ACTIVE_FRAME_RATE_CNT; j++) {
				sprintf(str, "%s%dhz",
					lcd_vmode_info[i].name,
					lcd_std_frame_rate[j][0]);
				printf("%s\n", str);
			}
			break;
		}
	}
}

static void lcd_config_init(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int clk;

	if (pconf->timing.lcd_clk == 0) {/* default 0 for 60hz */
		pconf->timing.lcd_clk = 60;
	} else {
		LCDPR("[%d]: custome clk: %d\n",
		      pdrv->index, pconf->timing.lcd_clk);
	}
	clk = pconf->timing.lcd_clk;
	if (clk < 200) { /* regard as frame_rate */
		pconf->timing.lcd_clk = clk * pconf->basic.h_period *
			pconf->basic.v_period;
	} else /* regard as pixel clock */
		pconf->timing.lcd_clk = clk;
	pconf->timing.lcd_clk_dft = pconf->timing.lcd_clk;
	pconf->timing.h_period_dft = pconf->basic.h_period;
	pconf->timing.v_period_dft = pconf->basic.v_period;
	pconf->timing.sync_duration_num =
		((pconf->timing.lcd_clk / pconf->basic.h_period) * 100) /
		pconf->basic.v_period;
	pconf->timing.sync_duration_den = 100;

	lcd_timing_init_config(pconf);
}

static int lcd_outputmode_check(struct aml_lcd_drv_s *pdrv, char *mode, unsigned int frac)
{
	int lcd_vmode;

	lcd_vmode = check_lcd_output_mode(pdrv, mode, frac);
	if (lcd_vmode >= LCD_VMODE_MAX)
		return -1;

	return 0;
}

static int lcd_config_check(struct aml_lcd_drv_s *pdrv, char *mode, unsigned int frac)
{
	struct lcd_config_s *pconf = &pdrv->config;
	int index, lcd_vmode, frame_rate;

	lcd_vmode = check_lcd_output_mode(pdrv, mode, frac);
	if (lcd_vmode >= LCD_VMODE_MAX)
		return -1;

	frame_rate = lcd_outputmode_to_frame_rate(mode);
	if (frame_rate == 0) {
		LCDERR("[%d]: %s: frame_rate is not support\n",
		       pdrv->index, __func__);
		return -1;
	}

	index = lcd_std_frame_rate_index(frame_rate);
	if (lcd_vmode_info[lcd_vmode].frac) {
		if (index < LCD_STD_FRAME_RATE_MAX)
			index++;
	}
	pconf->timing.sync_duration_num = lcd_std_frame_rate[index][1];
	pconf->timing.sync_duration_den = lcd_std_frame_rate[index][2];

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


