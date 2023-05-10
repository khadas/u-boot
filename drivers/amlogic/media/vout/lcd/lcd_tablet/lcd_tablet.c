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

#define CONNECTOR_NUM_MAX 10
#define OUTPUTMODE_NUM_MAX 3

static char *connector_name_list[CONNECTOR_NUM_MAX] = {
	"LVDS_A",
	"LVDS_B",
	"LVDS_C",
	"VBYONE_A",
	"VBYONE_B",
	"MIPI_A",
	"MIPI_B",
	"EDP_A",
	"EDP_B",
	"null"
};

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

static int lcd_reserve_match_opt_mode_idx(struct aml_lcd_drv_s *pdrv)
{
	char panel_str[10];
	char out_mode_str[12];
	char *out_mode_name;
	unsigned int index = 0;

	if (pdrv->index == 0)
		sprintf(panel_str, "panel");
	else
		sprintf(panel_str, "panel%d", pdrv->index);

	for (index = 0; index < OUTPUTMODE_NUM_MAX; index++) {
		if (index == 0)
			sprintf(out_mode_str, "outputmode");
		else
			sprintf(out_mode_str, "outputmode%d", index + 1);

		out_mode_name = env_get(out_mode_str);
		if (!out_mode_name)
			continue;

		if (strcmp(out_mode_name, panel_str) == 0)
			return index;
	}
	return -1;
}

static void lcd_set_connector(struct aml_lcd_drv_s *pdrv, char *opt_mode)
{
	char *buf;
	int name_idx = 0, cnt_idx  = 0;
	unsigned int cnt_used = 0;

	cnt_idx = lcd_reserve_match_opt_mode_idx(pdrv);
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("%s: panel[%d] match outputmode[%d]\n", __func__, pdrv->index, cnt_idx);

	if (cnt_idx < 0)
		return;

	buf = (char *)malloc(64 * sizeof(char));
	memset(buf, 0, 64);

	switch (pdrv->config.basic.lcd_type) {
	case LCD_LVDS:
	case LCD_MLVDS:
		name_idx = 0;
		break;
	case LCD_VBYONE:
	case LCD_P2P:
		name_idx = 3;
		break;
	case LCD_MIPI:
		name_idx = 5;
		break;
	case LCD_EDP:
		name_idx = 7;
		break;
	default:
		name_idx = CONNECTOR_NUM_MAX;
	}

	cnt_used = (name_idx + pdrv->index) < CONNECTOR_NUM_MAX ?
				name_idx + pdrv->index : CONNECTOR_NUM_MAX - 1;

	if (cnt_idx == 0)
		snprintf(buf, 63, "setenv connector_type %s", connector_name_list[cnt_used]);
	else
		snprintf(buf, 63, "setenv connector%d_type %s", cnt_idx,
				connector_name_list[cnt_used]);

	run_command(buf, 0);
	LCDPR("%s:[%d]->%s\n", __func__, cnt_idx, connector_name_list[cnt_used]);

	free(buf);
}

static int lcd_config_check(struct aml_lcd_drv_s *pdrv, char *mode, unsigned int frac)
{
	int ret;

	ret = check_lcd_output_mode(pdrv, mode, frac);
	if (ret)
		return -1;

	lcd_set_connector(pdrv, mode);

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


