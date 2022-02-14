// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/gpio.h>
#include <fdtdec.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include <amlogic/media/vout/lcd/lcd_extern.h>
#include "lcd_extern.h"
#include "../lcd_common.h"
#include "../lcd_reg.h"

#define EXT_DEFAULT_NAME	"ext_default"

/******************** mipi command ********************/
/* format:  data_type, cmd_size, data.... */
/*	data_type=0xff,
 *		0 < cmd_size < 0xff means delay ms,
 *		cmd_size=0 or 0xff means ending.
 *	data_type=0xf0, for gpio control
 *		data0=gpio_index, data1=gpio_value.
 *		data0=gpio_index, data1=gpio_value, data2=delay.
 *	data_type=0xfd, for delay ms
 *		data0=delay, data_1=delay, ..., data_n=delay.
 */
static unsigned char mipi_init_on_table[] = {
	0x05, 1, 0x11,
	0xfd, 1, 200, /* delay 200ms */
	0x05, 1, 0x29,
	0xfd, 1, 20,  /* delay 20ms */
	0xff, 0,      /* ending */
};

static unsigned char mipi_init_off_table[] = {
	0x05, 1, 0x28, /* display off */
	0xfd, 1, 10,   /* delay 10ms */
	0x05, 1, 0x10, /* sleep in */
	0xfd, 1, 150,  /* delay 150ms */
	0xff, 0,       /* ending */
};

static int lcd_extern_driver_update(struct lcd_extern_driver_s *edrv,
				    struct lcd_extern_dev_s *edev)
{
	if (edev->config.table_init_loaded == 0) {
		edev->config.cmd_size = LCD_EXT_CMD_SIZE_DYNAMIC;
		edev->config.table_init_on  = &mipi_init_on_table[0];
		edev->config.table_init_on_cnt  = sizeof(mipi_init_on_table);
		edev->config.table_init_off = &mipi_init_off_table[0];
		edev->config.table_init_off_cnt  = sizeof(mipi_init_off_table);
		EXTERR("%s(%d): tablet_init is invalid\n",
		       edev->config.name, edev->dev_index);
		return -1;
	}

	return 0;
}

int lcd_extern_mipi_default_probe(struct lcd_extern_driver_s *edrv,
				  struct lcd_extern_dev_s *edev)
{
	int ret = 0;

	if (!edrv) {
		EXTERR("%s: %s ext_drv is null\n", __func__, EXT_DEFAULT_NAME);
		return -1;
	}
	if (!edev) {
		EXTERR("[%d]: %s: %s ext_dev is null\n", edrv->index, __func__, EXT_DEFAULT_NAME);
		return -1;
	}

	if (edev->config.type != LCD_EXTERN_MIPI) {
		EXTERR("[%d]: %s: invalid dev: %s(%d) type %d\n",
		       edrv->index, __func__,
		       edev->config.name, edev->dev_index,
		       edev->config.type);
		return -1;
	}
	if (edev->config.cmd_size != LCD_EXT_CMD_SIZE_DYNAMIC) {
		EXTERR("[%d]: %s: %s(%d): cmd_size %d is invalid\n",
			edrv->index, __func__,
			edev->config.name,
			edev->dev_index,
			edev->config.cmd_size);
		return -1;
	}

	ret = lcd_extern_driver_update(edrv, edev);

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		EXTPR("[%d]: %s: %d %s\n",
		      edrv->index, __func__, edev->dev_index,
		      (ret == 0) ? "ok" : "fail");
	}
	return ret;
}
