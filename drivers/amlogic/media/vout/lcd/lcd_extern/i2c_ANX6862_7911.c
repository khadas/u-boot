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

#define LCD_EXTERN_NAME			"i2c_ANX6862_7911"

//#define LCD_EXTERN_I2C_ADDR		(0x20) //7bit address
//#define LCD_EXTERN_I2C_ADDR2		(0x74) //7bit address

static unsigned char ANX6862_NVM_wr[] = {0xff, 0x80};
static unsigned char ANX7911_NVM_wr[] = {0x00, 0x0a};

#define ANX6862_REG_CNT   12
#define ANX7911_REG_CNT   23
#if 0
static unsigned char ANX6862_7911_init[] = {
	0xc0, 2, 0x01, 0x2b,
	0xc0, 2, 0x02, 0x05,
	0xc0, 2, 0x03, 0x00,
	0xc0, 2, 0x04, 0x00,
	0xc0, 2, 0x05, 0x0c,
	0xc0, 2, 0x06, 0x04,
	0xc0, 2, 0x07, 0x21,
	0xc0, 2, 0x08, 0x0e,
	0xc0, 2, 0x09, 0x04,
	0xc0, 2, 0x0a, 0x00,
	0xc0, 2, 0x0b, 0x04,
	0xc0, 2, 0xff, 0x00,
	0xfd, 1, 50, /* delay 50ms */

	0xc1, 2, 0x01, 0xca,
	0xc1, 2, 0x02, 0x3b,
	0xc1, 2, 0x03, 0x33,
	0xc1, 2, 0x04, 0x05,
	0xc1, 2, 0x05, 0x2c,
	0xc1, 2, 0x06, 0xf2,
	0xc1, 2, 0x07, 0x9c,
	0xc1, 2, 0x08, 0x1b,
	0xc1, 2, 0x09, 0x82,
	0xc1, 2, 0x0a, 0x3d,
	0xc1, 2, 0x0b, 0x20,
	0xc1, 2, 0x0c, 0x11,
	0xc1, 2, 0x0d, 0xc4,
	0xc1, 2, 0x0e, 0x1a,
	0xc1, 2, 0x0f, 0x31,
	0xc1, 2, 0x10, 0x4c,
	0xc1, 2, 0x11, 0x12,
	0xc1, 2, 0x12, 0x90,
	0xc1, 2, 0x13, 0xf7,
	0xc1, 2, 0x14, 0x0c,
	0xc1, 2, 0x15, 0x20,
	0xc1, 2, 0x16, 0x13,
	0xff, 0,
};
#endif

static int lcd_extern_power_cmd_dynamic_size(struct lcd_extern_driver_s *edrv,
					     struct lcd_extern_dev_s *edev,
					     unsigned char *table,
					     unsigned char flag)
{
	int i = 0, j, step = 0, max_len;
	unsigned char type, cmd_size, type_chk, type_other;
	unsigned char i2c_bus, i2c_addr;
	int delay_bypass, delay_ms, ret = 0;

	if (!table)
		return -1;

	i2c_bus = edrv->i2c_bus;
	max_len = edev->config.table_init_on_cnt;
	if (flag) {
		i2c_addr = edev->config.i2c_addr2;
		type_chk = LCD_EXT_CMD_TYPE_CMD2;
		type_other = LCD_EXT_CMD_TYPE_CMD;
	} else {
		i2c_addr = edev->config.i2c_addr;
		type_chk = LCD_EXT_CMD_TYPE_CMD;
		type_other = LCD_EXT_CMD_TYPE_CMD2;
	}

	delay_bypass = 0;
	while ((i + 1) < max_len) {
		type = table[i];
		if (type == LCD_EXT_CMD_TYPE_END)
			break;
		cmd_size = table[i+1];
		if (cmd_size == 0)
			goto power_cmd_dynamic_next;
		if ((i + 2 + cmd_size) > max_len)
			break;

		if (type == type_chk) {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
					__func__, step, type, table[i+1]);
			}
			ret = aml_lcd_i2c_write(i2c_bus, i2c_addr,
				&table[i+2], cmd_size);
			delay_bypass = 0;
		} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
			if (delay_bypass)
				goto power_cmd_dynamic_next;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
					__func__, step, type, table[i+1]);
			}
			delay_ms = 0;
			for (j = 0; j < cmd_size; j++)
				delay_ms += table[i+2+j];
			if (delay_ms > 0)
				mdelay(delay_ms);
		} else if (type == type_other) {
			delay_bypass = 1;
		} else {
			delay_bypass = 1;
			EXTERR("%s(%d: %s): type 0x%02x invalid\n",
				__func__, edev->config.index,
				edev->config.name, type);
		}
power_cmd_dynamic_next:
		i += (cmd_size + 2);
		step++;
	}

	return ret;
}

static int lcd_extern_power_cmd_fixed_size(struct lcd_extern_driver_s *edrv,
					   struct lcd_extern_dev_s *edev,
					   unsigned char *table,
					   unsigned char flag)
{
	int i = 0, j, step = 0, max_len = LCD_EXTERN_INIT_ON_MAX;
	unsigned char type, cmd_size, type_chk, type_other;
	unsigned char i2c_bus, i2c_addr;
	int delay_bypass, delay_ms, ret = 0;

	if (table == NULL)
		return -1;

	i2c_bus = edrv->i2c_bus;
	cmd_size = edev->config.cmd_size;
	max_len = edev->config.table_init_on_cnt;
	if (flag) {
		i2c_addr = edev->config.i2c_addr2;
		type_chk = LCD_EXT_CMD_TYPE_CMD2;
		type_other = LCD_EXT_CMD_TYPE_CMD;
	} else {
		i2c_addr = edev->config.i2c_addr;
		type_chk = LCD_EXT_CMD_TYPE_CMD;
		type_other = LCD_EXT_CMD_TYPE_CMD2;
	}

	if (cmd_size < 2) {
		EXTERR("%s: invalid cmd_size %d\n", __func__, cmd_size);
		return -1;
	}

	delay_bypass = 0;
	while ((i + cmd_size) <= max_len) {
		type = table[i];
		if (type == LCD_EXT_CMD_TYPE_END)
			break;
		if (type == type_chk) {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
					__func__, step, type, cmd_size);
			}
			ret = aml_lcd_i2c_write(i2c_bus, i2c_addr,
				&table[i+1], (cmd_size-1));
			delay_bypass = 0;
		} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
			if (delay_bypass)
				goto power_cmd_fixed_next;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
					__func__, step, type, cmd_size);
			}
			delay_ms = 0;
			for (j = 0; j < (cmd_size - 1); j++)
				delay_ms += table[i+1+j];
			if (delay_ms > 0)
				mdelay(delay_ms);
		} else if (type == type_other) {
			delay_bypass = 1;
		} else {
			delay_bypass = 1;
			EXTERR("%s(%d: %s): type 0x%02x invalid\n",
				__func__, edev->config.index,
				edev->config.name, type);
		}
power_cmd_fixed_next:
		i += cmd_size;
		step++;
	}

	return ret;
}

static int lcd_extern_power_cmd(struct lcd_extern_driver_s *edrv,
				struct lcd_extern_dev_s *edev, unsigned char flag)
{
	unsigned char *table;
	unsigned char cmd_size;
	int ret = 0;

	table = edev->config.table_init_on;
	cmd_size = edev->config.cmd_size;
	if (cmd_size < 1) {
		EXTERR("%s: cmd_size %d is invalid\n", __func__, cmd_size);
		return -1;
	}
	if (!table) {
		EXTERR("%s: init_on_table is NULL\n", __func__);
		return -1;
	}

	if (cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC)
		ret = lcd_extern_power_cmd_dynamic_size(edrv, edev, table, flag);
	else
		ret = lcd_extern_power_cmd_fixed_size(edrv, edev, table, flag);

	return ret;
}

static int lcd_extern_check_reg_dynamic_size(struct lcd_extern_driver_s *edrv,
					     struct lcd_extern_dev_s *edev,
					     unsigned char *table,
					     unsigned char *chk_table,
					     int cnt, unsigned char flag)
{
	int i = 0, step = 0, max_len;
	unsigned char type, cmd_size, type_chk, reg;
	int ret = 0;

	if (!table)
		return -1;

	max_len = edev->config.table_init_on_cnt;
	type_chk = (flag) ? LCD_EXT_CMD_TYPE_CMD2 : LCD_EXT_CMD_TYPE_CMD;

	while ((i + 1) < max_len) {
		type = table[i];
		if (type == LCD_EXT_CMD_TYPE_END)
			break;
		cmd_size = table[i+1];
		if (cmd_size == 0) {
			i += 2;
			continue;
		}
		if ((i + 2 + cmd_size) > max_len)
			break;

		if (type == type_chk) {
			reg = table[i+2];
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("%s: step %d: reg 0x%02x, val 0x%02x,0x%02x\n",
					__func__, step,
					reg, table[i+3], chk_table[reg]);
			}
			if (chk_table[reg] != table[i+3])
				return -1;
		}
		i += (cmd_size + 2);
		step++;
	}

	return ret;
}

static int lcd_extern_check_reg_fixed_size(struct lcd_extern_driver_s *edrv,
					   struct lcd_extern_dev_s *edev,
					   unsigned char *table,
					   unsigned char *chk_table,
					   int cnt, unsigned char flag)
{
	int i = 0, step = 0, max_len;
	unsigned char type, cmd_size, type_chk, reg;

	cmd_size = edev->config.cmd_size;
	max_len = edev->config.table_init_on_cnt;
	type_chk = (flag) ? LCD_EXT_CMD_TYPE_CMD2 : LCD_EXT_CMD_TYPE_CMD;

	while ((i + cmd_size) <= max_len) {
		type = table[i];
		if (type == LCD_EXT_CMD_TYPE_END)
			break;
		if (type == type_chk) {
			reg = table[i+1];
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("%s: step %d: reg 0x%02x, val 0x%02x,0x%02x\n",
					__func__, step,
					reg, table[i+2], chk_table[reg]);
			}
			if (chk_table[reg] != table[i+2])
				return -1;
		}
		i += cmd_size;
		step++;
	}

	return 0;
}

static int lcd_extern_init_check(struct lcd_extern_driver_s *edrv,
				 struct lcd_extern_dev_s *edev, unsigned char flag)
{
	unsigned char *table, *chk_table, cmd_size;
	unsigned char i2c_bus, i2c_addr;
	int cnt, ret = 0;

	table = edev->config.table_init_on;
	cmd_size = edev->config.cmd_size;
	i2c_bus = edrv->i2c_bus;
	if (flag)
		i2c_addr = edev->config.i2c_addr2;
	else
		i2c_addr = edev->config.i2c_addr;

	cnt = (flag) ? ANX7911_REG_CNT : ANX6862_REG_CNT;
	if (cmd_size < 1) {
		EXTERR("%s: cmd_size %d is invalid\n", __func__, cmd_size);
		return -1;
	}
	if (!table) {
		EXTERR("%s: init_table %d is NULL\n", __func__, flag);
		return -1;
	}

	chk_table = malloc(cnt * sizeof(unsigned char));
	if (chk_table == NULL) {
		EXTERR("%s: failed to alloc chk_table, not enough memory\n",
			LCD_EXTERN_NAME);
		return -1;
	}
	memset(chk_table, 0, cnt);

	ret = aml_lcd_i2c_read(i2c_bus, i2c_addr, chk_table, cnt);
	if (ret) {
		EXTERR("%s: i2c read error\n", __func__);
		return -1;
	}

	if (cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC)
		ret = lcd_extern_check_reg_dynamic_size(table, chk_table, cnt, flag);
	else
		ret = lcd_extern_check_reg_fixed_size(table, chk_table, cnt, flag);

	return ret;
}

static int lcd_extern_power_on(struct lcd_extern_driver_s *edrv,
			       struct lcd_extern_dev_s *edev)
{
	int ret = 0;

	lcd_extern_pinmux_set(edrv, 1);

	/* check voltage is init or not */
	/* step1: ANX6862 */
	ret = lcd_extern_init_check(edrv, edev, 0);
	if (ret) {
		EXTPR("ANX6862: need init voltage and NVM write\n");
		/* init voltage */
		lcd_extern_power_cmd(edrv, edev, 0);
		/* NVM write */
		aml_lcd_i2c_write(edrv->i2c_bus, edev->config.i2c_addr,
			ANX6862_NVM_wr, 2);
	}

	/* step2: ANX7911 */
	ret = lcd_extern_init_check(edrv, edev, 1);
	if (ret) {
		EXTPR("ANX7911: need init voltage and NVM write\n");
		/* init voltage */
		lcd_extern_power_cmd(edrv, edev, 1);
		/* NVM write */
		aml_lcd_i2c_write(edrv->i2c_bus, edev->config.i2c_addr2,
			ANX7911_NVM_wr, 2);
	}

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
		return -1;
	}

	edev->power_on  = lcd_extern_power_on;
	edev->power_off = lcd_extern_power_off;

	return 0;
}

int lcd_extern_i2c_ANX6862_7911_probe(struct lcd_extern_driver_s *edrv,
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

	ret = lcd_extern_driver_update(edrv, edrv);

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		EXTPR("[%d]: %s: %d %s\n",
		      edrv->index, __func__, edev->dev_index,
		      (ret == 0) ? "ok" : "fail");
	}
	return ret;
}

