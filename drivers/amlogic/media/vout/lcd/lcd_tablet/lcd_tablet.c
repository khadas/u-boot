// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/gpio.h>
#include <fdtdec.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "../lcd_reg.h"
#include "../lcd_common.h"
#include "lcd_tablet.h"
#include "mipi_dsi_util.h"

static int check_lcd_output_mode(struct aml_lcd_drv_s *pdrv, char *mode, unsigned int frac)
{
	char str[10];

	if (frac) {
		LCDERR("don't support frac\n");
		return -1;
	}

	if (pdrv->index == 0)
		sprintf(str, "panel");
	else
		sprintf(str, "panel%d", pdrv->index);
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("[%d]: %s: outputmode=%s, support mode=%s\n",
		      pdrv->index, __func__, mode, str);
	}

	if (strcmp(mode, str) != 0) {
		//LCDERR("outputmode[%s] is not support\n", mode);
		return -1;
	}

	return 0;
}

static void lcd_list_support_mode(struct lcd_config_s *pconf)
{
	printf("panel\n");
}

static void lcd_config_init(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int h_period, v_period, clk, sync_duration;

	h_period = pconf->basic.h_period;
	v_period = pconf->basic.v_period;
	clk = pconf->timing.lcd_clk;

	if (clk < 200) { /* regard as frame_rate */
		sync_duration = clk * 100;
		pconf->timing.lcd_clk = clk * h_period * v_period;
	} else { /* regard as pixel clock */
		sync_duration = ((clk / h_period) * 100) / v_period;
	}
	pconf->timing.lcd_clk_dft = pconf->timing.lcd_clk;
	pconf->timing.h_period_dft = pconf->basic.h_period;
	pconf->timing.v_period_dft = pconf->basic.v_period;
	pconf->timing.sync_duration_num = sync_duration;
	pconf->timing.sync_duration_den = 100;

	lcd_timing_init_config(pconf);
	lcd_tablet_config_update(pdrv);
	lcd_clk_generate_parameter(pdrv);
}

static int lcd_config_check(struct aml_lcd_drv_s *pdrv, char *mode, unsigned int frac)
{
	int ret;

	ret = check_lcd_output_mode(pdrv, mode, frac);
	if (ret)
		return -1;

	return 0;
}

int lcd_mode_tablet_init(struct aml_lcd_drv_s *pdrv)
{
	pdrv->list_support_mode = lcd_list_support_mode;
	pdrv->outputmode_check = check_lcd_output_mode;
	pdrv->config_check = lcd_config_check;
	pdrv->driver_init_pre = lcd_tablet_driver_init_pre;
	pdrv->driver_init = lcd_tablet_driver_init;
	pdrv->driver_disable = lcd_tablet_driver_disable;

	lcd_config_init(pdrv);

	return 0;
}


