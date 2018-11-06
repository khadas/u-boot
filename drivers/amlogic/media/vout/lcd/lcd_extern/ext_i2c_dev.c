/*
 * drivers/amlogic/media/vout/lcd/lcd_extern/ext_i2c_dev.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/gpio.h>
#include <fdtdec.h>
#include <i2c.h>
#include <dm.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include <amlogic/media/vout/lcd/lcd_extern.h>
#include "lcd_extern.h"
#include "../lcd_common.h"

struct aml_lcd_extern_i2c_match_s {
	unsigned char bus_id;
	unsigned char bus_sys;
	char *bus_str;
};

static struct aml_lcd_extern_i2c_match_s lcd_extern_i2c_match_table[] = {
	{LCD_EXTERN_I2C_BUS_0,   LCD_EXT_I2C_BUS_0,   "i2c_0/a"},
	{LCD_EXTERN_I2C_BUS_1,   LCD_EXT_I2C_BUS_1,   "i2c_1/b"},
	{LCD_EXTERN_I2C_BUS_2,   LCD_EXT_I2C_BUS_2,   "i2c_2/c"},
	{LCD_EXTERN_I2C_BUS_3,   LCD_EXT_I2C_BUS_3,   "i2c_3/d"},
	{LCD_EXTERN_I2C_BUS_4,   LCD_EXT_I2C_BUS_4,   "i2c_4/ao"},
	{LCD_EXTERN_I2C_BUS_MAX, LCD_EXT_I2C_BUS_MAX, "i2c_invalid"},
};

void lcd_extern_i2c_bus_print(unsigned char i2c_bus)
{
	int i, temp = ARRAY_SIZE(lcd_extern_i2c_match_table) - 1;

	for (i = 0; i < ARRAY_SIZE(lcd_extern_i2c_match_table); i++) {
		if (lcd_extern_i2c_match_table[i].bus_id == i2c_bus) {
			temp = i;
			break;
		}
	}

	EXTPR("i2c_bus = %s(%d)\n",
		lcd_extern_i2c_match_table[temp].bus_str, temp);
}

unsigned char lcd_extern_i2c_bus_get_sys(unsigned char i2c_bus)
{
	int i, ret = LCD_EXT_I2C_BUS_MAX;

	for (i = 0; i < ARRAY_SIZE(lcd_extern_i2c_match_table); i++) {
		if (lcd_extern_i2c_match_table[i].bus_id == i2c_bus) {
			ret = lcd_extern_i2c_match_table[i].bus_sys;
			break;
		}
	}

	if (lcd_debug_print_flag)
		EXTPR("%s: %d->%d\n", __func__, i2c_bus, ret);
	return ret;
}

int lcd_extern_i2c_write(unsigned char i2c_bus, unsigned i2c_addr,
		unsigned char *buff, unsigned len)
{
	unsigned char sys_i2c_bus;
	struct udevice *i2c_dev;
	int i, ret = 0;

	sys_i2c_bus = lcd_extern_i2c_bus_get_sys(i2c_bus);
	ret = i2c_get_chip_for_busnum(sys_i2c_bus, i2c_addr, 1, &i2c_dev);
	if (ret) {
		EXTERR("no sys_i2c_bus %d find\n", sys_i2c_bus);
		return ret;
	}

	if (lcd_debug_print_flag) {
		printf("%s:", __func__);
		for (i = 0; i < len; i++)
			printf(" 0x%02x", buff[i]);
		printf(" [addr 0x%02x]\n", i2c_addr);
	}

	ret = dm_i2c_write(i2c_dev, i2c_addr, buff, len);
	if (ret) {
		EXTERR("i2c write failed [addr 0x%02x]\n", i2c_addr);
		return ret;
	}

	return 0;
}

int lcd_extern_i2c_read(unsigned char i2c_bus, unsigned i2c_addr,
		unsigned char *buff, unsigned int len)
{
	unsigned char sys_i2c_bus;
	struct udevice *i2c_dev;
	int ret = 0;

	sys_i2c_bus = lcd_extern_i2c_bus_get_sys(i2c_bus);
	ret = i2c_get_chip_for_busnum(sys_i2c_bus, i2c_addr, 1, &i2c_dev);
	if (ret) {
		EXTERR("no sys_i2c_bus %d find\n", sys_i2c_bus);
		return ret;
	}

	ret = dm_i2c_write(i2c_dev, i2c_addr, buff, 1);
	if (ret) {
		EXTERR("i2c write failed [addr 0x%02x]\n", i2c_addr);
		return ret;
	}
	ret = dm_i2c_read(i2c_dev, i2c_addr, buff, len);
	if (ret) {
		EXTERR("i2c read failed [addr 0x%02x]\n", i2c_addr);
		return ret;
	}

	return 0;
}
