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

#define LCD_EXTERN_NAME			"i2c_oled"

static int lcd_extern_power_cmd_dynamic_size(struct lcd_extern_driver_s *edrv,
					     struct lcd_extern_dev_s *edev,
					     unsigned char *table)
{
	int i = 0, j, step = 0, delay_ms;
	unsigned char type, cmd_size;
	int ret = 0;
	int max_len;

	max_len = edev->config.table_init_on_cnt;
	while ((i + 1) < max_len) {
		type = table[i];
		if (type == LCD_EXT_CMD_TYPE_END)
			break;
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			EXTPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
				__func__, step, type, table[i + 1]);
		}
		cmd_size = table[i + 1];
		if (cmd_size == 0)
			goto power_cmd_dynamic_next;
		if ((i + 2 + cmd_size) > max_len)
			break;

		if (type == LCD_EXT_CMD_TYPE_NONE) {
			/* do nothing */
		} else if (type == LCD_EXT_CMD_TYPE_GPIO) {
			if (cmd_size < 2) {
				EXTERR("step %d: invalid cmd_size %d for GPIO\n",
					step, cmd_size);
				goto power_cmd_dynamic_next;
			}
			lcd_ext_gpio_set(edrv, table[i + 2], table[i + 3]);
			if (cmd_size > 2) {
				if (table[i + 4] > 0)
					mdelay(table[i + 4]);
			}
		} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
			delay_ms = 0;
			for (j = 0; j < cmd_size; j++)
				delay_ms += table[i + 2 + j];
			if (delay_ms > 0)
				mdelay(delay_ms);
		} else if (type == LCD_EXT_CMD_TYPE_CMD) {
			ret = aml_lcd_i2c_write(edrv->i2c_bus,
				edev->config.i2c_addr, &table[i + 2], cmd_size);
		} else if (type == LCD_EXT_CMD_TYPE_CMD2) {
			ret = aml_lcd_i2c_write(edrv->i2c_bus,
				edev->config.i2c_addr2, &table[i + 2], cmd_size);
		} else {
			EXTERR("%s(%d: %s): type 0x%02x invalid\n",
				__func__, edev->config.index, edev->config.name, type);
		}
power_cmd_dynamic_next:
		i += (cmd_size + 2);
		step++;
	}

	return ret;
}

static int lcd_extern_power_cmd(struct lcd_extern_driver_s *edrv,
				struct lcd_extern_dev_s *edev, unsigned char *table)
{
	int cmd_size;
	int ret = 0;

	cmd_size = edev->config.cmd_size;
	if (cmd_size != LCD_EXT_CMD_SIZE_DYNAMIC) {
		EXTERR("%s: cmd_size %d is invalid\n", __func__, cmd_size);
		return -1;
	}
	if (!table)
		return -1;

	ret = lcd_extern_power_cmd_dynamic_size(edrv, edev, table);

	return ret;
}

static int lcd_extern_power_on(struct lcd_extern_driver_s *edrv,
			       struct lcd_extern_dev_s *edev)
{
	int ret = 0;

	lcd_extern_pinmux_set(edrv, 1);

	lcd_extern_power_cmd(edrv, edev, edev->config.table_init_on);

	EXTPR("%s\n", __func__);
	return ret;
}

static int lcd_extern_power_off(struct lcd_extern_driver_s *edrv,
				struct lcd_extern_dev_s *edev)
{
	lcd_extern_power_cmd(edrv, edev, edev->config.table_init_off);
	lcd_extern_pinmux_set(edrv, 0);
	return 0;
}

static int lcd_extern_driver_update(struct lcd_extern_driver_s *edrv,
				    struct lcd_extern_dev_s *edev)
{
	if (edev->config.table_init_loaded == 0) {
		EXTERR("%s(%d): tablet_init is invalid\n",
		       edev->config.name, edev->dev_index);
	}

	edev->power_on  = lcd_extern_power_on;
	edev->power_off = lcd_extern_power_off;

	return 0;
}

int lcd_extern_i2c_oled_probe(struct lcd_extern_driver_s *edrv,
				struct lcd_extern_dev_s *edev)
{
	int ret = 0;

	if (!edrv) {
		EXTERR("%s: %s ext_drv is null\n", __func__, LCD_EXTERN_NAME);
		return -1;
	}
	if (!edev) {
		EXTERR("%s: %s ext_dev is null\n", __func__, LCD_EXTERN_NAME);
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
