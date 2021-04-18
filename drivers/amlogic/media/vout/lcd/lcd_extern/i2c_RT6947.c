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

#define LCD_EXTERN_NAME			"i2c_RT6947"
//#define LCD_EXTERN_I2C_ADDR		(0x66 >> 1) //7bit address

#define GAMMA_EEPROM_WRITE

#define LCD_EXTERN_CMD_SIZE        LCD_EXT_CMD_SIZE_DYNAMIC
static unsigned char gamma_init[] = {
	0xc0, 89, 0x00,
		0xA2, 0xD0, 0x80, 0x00, 0x10, 0x10, 0x59, 0x19, 0x4B, 0x0B,
		0x3A, 0x13, 0x27, 0x2F, 0xC2, 0xC9, 0x29, 0xD2, 0x7A, 0x20,
		0xA1, 0xC0, 0x15, 0x01, 0x36, 0x10, 0x10, 0xCB, 0x0A, 0x10,
		0x2C, 0x1C, 0xA0, 0xFF, 0x00, 0x00, 0x0B, 0x02, 0x00, 0x00,
		0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10,
		0x80, 0x00, 0x60, 0x00, 0x00, 0x00, 0x0D, 0x0D, 0x20, 0x02,
		0x00, 0x20, 0x02, 0x00, 0x20, 0x02, 0x00, 0x20, 0x02, 0x00,
		0x20, 0x02, 0x00, 0x20, 0x02, 0x00, 0x20, 0x02, 0x00, 0x20,
		0x00, 0xFF, 0x00, 0x00, 0x0B, 0x02, 0x00, 0x00,
	0xff, 0,  /* ending */
};

#ifdef GAMMA_EEPROM_WRITE
static unsigned char mtp_en[]    = {0x64,0x01};
static unsigned char eeprom_wr[] = {0xFF,0x80};
#endif

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
		if (lcd_debug_print_flag) {
			EXTPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
				__func__, step, type, table[i+1]);
		}
		cmd_size = table[i+1];
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
			lcd_ext_gpio_set(table[i+2], table[i+3]);
			if (cmd_size > 2) {
				if (table[i+4] > 0)
					mdelay(table[i+4]);
			}
		} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
			delay_ms = 0;
			for (j = 0; j < cmd_size; j++)
				delay_ms += table[i+2+j];
			if (delay_ms > 0)
				mdelay(delay_ms);
		} else if ((type == LCD_EXT_CMD_TYPE_CMD) ||
			   (type == LCD_EXT_CMD_TYPE_CMD_BIN)) {
			ret = aml_lcd_i2c_write(edrv->i2c_bus,
				edev->config.i2c_addr, &table[i+2], cmd_size);
		} else if ((type == LCD_EXT_CMD_TYPE_CMD2) ||
			   (type == LCD_EXT_CMD_TYPE_CMD2_BIN)) {
			ret = aml_lcd_i2c_write(edrv->i2c_bus,
				edev->config.i2c_addr2, &table[i+2], cmd_size);
		} else if (type == LCD_EXT_CMD_TYPE_CMD_DELAY) {
			ret = aml_lcd_i2c_write(edrv->i2c_bus,
				edev->config.i2c_addr, &table[i+2], (cmd_size-1));
			if (table[i+1+cmd_size] > 0)
				mdelay(table[i+1+cmd_size]);
		} else if (type == LCD_EXT_CMD_TYPE_CMD2_DELAY) {
			ret = aml_lcd_i2c_write(edrv->i2c_bus,
				edev->config.i2c_addr2, &table[i+2], (cmd_size-1));
			if (table[i+1+cmd_size] > 0)
				mdelay(table[i+1+cmd_size]);
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

static int lcd_extern_power_cmd_fixed_size(struct lcd_extern_driver_s *edrv,
					   struct lcd_extern_dev_s *edev,
					   unsigned char *table)
{
	int i = 0, j, step = 0, delay_ms;
	unsigned char type, cmd_size;
	int ret = 0;
	int max_len;

	max_len = edev->config.table_init_on_cnt;
	cmd_size = edev->config.cmd_size;
	if (cmd_size < 2) {
		EXTERR("%s: invalid cmd_size %d\n", __func__, cmd_size);
		return -1;
	}

	while ((i + cmd_size) <= max_len) {
		type = table[i];
		if (type == LCD_EXT_CMD_TYPE_END)
			break;
		if (lcd_debug_print_flag) {
			EXTPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
				__func__, step, type, cmd_size);
		}
		if (type == LCD_EXT_CMD_TYPE_NONE) {
			/* do nothing */
		} else if (type == LCD_EXT_CMD_TYPE_GPIO) {
			lcd_ext_gpio_set(table[i+1], table[i+2]);
			if (cmd_size > 3) {
				if (table[i+3] > 0)
					mdelay(table[i+3]);
			}
		} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
			delay_ms = 0;
			for (j = 0; j < (cmd_size - 1); j++)
				delay_ms += table[i+1+j];
			if (delay_ms > 0)
				mdelay(delay_ms);
		} else if ((type == LCD_EXT_CMD_TYPE_CMD) ||
			   (type == LCD_EXT_CMD_TYPE_CMD_BIN)) {
			ret = aml_lcd_i2c_write(edrv->i2c_bus,
				edev->config.i2c_addr, &table[i+1], (cmd_size-1));
		} else if ((type == LCD_EXT_CMD_TYPE_CMD2) ||
			   (type == LCD_EXT_CMD_TYPE_CMD2_BIN)) {
			ret = aml_lcd_i2c_write(edrv->i2c_bus,
				edev->config.i2c_addr2, &table[i+1], (cmd_size-1));
		} else if (type == LCD_EXT_CMD_TYPE_CMD_DELAY) {
			ret = aml_lcd_i2c_write(edrv->i2c_bus,
				edev->config.i2c_addr, &table[i+1], (cmd_size-2));
			if (table[i+cmd_size-1] > 0)
				mdelay(table[i+cmd_size-1]);
		} else if (type == LCD_EXT_CMD_TYPE_CMD2_DELAY) {
			ret = aml_lcd_i2c_write(edrv->i2c_bus,
				edev->config.i2c_addr2, &table[i+1], (cmd_size-2));
			if (table[i+cmd_size-1] > 0)
				mdelay(table[i+cmd_size-1]);
		} else {
			EXTERR("%s(%d: %s): type 0x%02x invalid\n",
				__func__, edev->config.index, edev->config.name, type);
		}
		i += cmd_size;
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
	if (cmd_size < 1) {
		EXTERR("%s: cmd_size %d is invalid\n", __func__, cmd_size);
		return -1;
	}
	if (!table)
		return -1;

	if (cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC)
		ret = lcd_extern_power_cmd_dynamic_size(edrv, edev, table);
	else
		ret = lcd_extern_power_cmd_fixed_size(edrv, edev, table);

	return ret;
}

#ifdef GAMMA_EEPROM_WRITE
static int lcd_extern_init_check(struct lcd_extern_driver_s *edrv,
				struct lcd_extern_dev_s *edev, int len)
{
	int ret = 0;
	unsigned char *chk_table;
	int i;

	chk_table = (unsigned char *)malloc(sizeof(unsigned char) * len);
	if (!chk_table) {
		EXTERR("%s: failed to alloc chk_table, not enough memory\n", LCD_EXTERN_NAME);
		ret = lcd_extern_power_cmd(edrv, edev, edev->config.table_init_on);
		return ret;
	}
	memset(chk_table, 0, len);

	ret = aml_lcd_i2c_read(edrv->i2c_bus, edev->config.i2c_addr, chk_table, len);
	if (ret == 0) {
		for (i = 0; i < len; i++) {
			if (chk_table[i] != edev->config.table_init_on[i+3])
				return -1;
		}
	}

	return 0;
}
#endif

static int lcd_extern_power_on(struct lcd_extern_driver_s *edrv,
			       struct lcd_extern_dev_s *edev)
{
	int ret = 0;
#ifdef GAMMA_EEPROM_WRITE
	int len;
#endif

	lcd_extern_pinmux_set(edrv, 1);

#ifdef GAMMA_EEPROM_WRITE
	len = edev->config.table_init_on[1] - 2;
	/* check gamma is init or not */
	ret = lcd_extern_init_check(len);
	if (ret) {
		EXTPR("RT6947: need init gamma and mtp write\n");
		lcd_extern_power_cmd(edev->config.table_init_on);
		/* enable mtp */
		len = sizeof(mtp_en) / sizeof(unsigned char);
		aml_lcd_i2c_write(edrv->i2c_bus, edev->config.i2c_addr, mtp_en, len);
		/* write eeprom */
		len = sizeof(eeprom_wr) / sizeof(unsigned char);
		aml_lcd_i2c_write(edrv->i2c_bus, edev->config.i2c_addr, eeprom_wr, len);
	}
#else
	lcd_extern_power_cmd(edev->config.table_init_on);
#endif

	EXTPR("%s\n", __func__);
	return ret;
}

static int lcd_extern_power_off(struct lcd_extern_driver_s *edrv,
				struct lcd_extern_dev_s *edev)
{
	lcd_extern_pinmux_set(edrv, 0);
	return 0;
}

static int lcd_extern_driver_update(struct lcd_extern_driver_s *edrv,
				    struct lcd_extern_dev_s *edev)
{
	if (edev->config.table_init_loaded == 0) {
		EXTERR("%s(%d): tablet_init is invalid\n",
		       edev->config.name, edev->dev_index);
		edev->config->cmd_size = LCD_EXTERN_CMD_SIZE;
		edev->config->table_init_on = gamma_init;
		edev->config->table_init_on_cnt = sizeof(gamma_init);
	}

	edev->power_on  = lcd_extern_power_on;
	edev->power_off = lcd_extern_power_off;

	return 0;
}

int lcd_extern_i2c_RT6947_probe(struct lcd_extern_driver_s *edrv,
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

	ret = lcd_extern_driver_update(edrv, edev);

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		EXTPR("[%d]: %s: %d %s\n",
		      edrv->index, __func__, edev->dev_index,
		      (ret == 0) ? "ok" : "fail");
	}
	return ret;
}

