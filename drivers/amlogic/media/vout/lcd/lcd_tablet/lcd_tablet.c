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

static void lcd_list_support_mode(struct aml_lcd_drv_s *pdrv)
{
	printf("panel\n");
}

static void lcd_config_init(struct aml_lcd_drv_s *pdrv)
{
	lcd_basic_timing_range_update(pdrv);
	lcd_timing_init_config(&pdrv->config);
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


