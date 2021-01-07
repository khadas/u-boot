// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * drivers/amlogic/media/vout/lcd/lcd_i2c_dev.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the named License,
 * or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/gpio.h>
#include <fdtdec.h>
#include <i2c.h>
#include <dm.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include <amlogic/media/vout/lcd/lcd_i2c_dev.h>
#include "lcd_common.h"

#define LCDI2C_PR(fmt, args...)     printf("lcd_i2c: "fmt"", ## args)
#define LCDI2C_ERR(fmt, args...)    printf("lcd_i2c: error: "fmt"", ## args)

struct lcd_i2c_match_s {
	unsigned char bus_id;
	char *bus_str;
};

static struct lcd_i2c_match_s lcd_i2c_match_table[] = {
	{LCD_EXT_I2C_BUS_0,   "i2c_0/a"},
	{LCD_EXT_I2C_BUS_1,   "i2c_1/b"},
	{LCD_EXT_I2C_BUS_2,   "i2c_2/c"},
	{LCD_EXT_I2C_BUS_3,   "i2c_3/d"},
	{LCD_EXT_I2C_BUS_4,   "i2c_4/ao"},
	{LCD_EXT_I2C_BUS_MAX, "i2c_invalid"},
};

void aml_lcd_i2c_bus_print(unsigned char i2c_bus)
{
	int i, temp = ARRAY_SIZE(lcd_i2c_match_table) - 1;

	for (i = 0; i < ARRAY_SIZE(lcd_i2c_match_table); i++) {
		if (lcd_i2c_match_table[i].bus_id == i2c_bus) {
			temp = i;
			break;
		}
	}

	LCDI2C_PR("i2c_bus = %s(%d)\n", lcd_i2c_match_table[temp].bus_str, temp);
}

int aml_lcd_i2c_write(unsigned char i2c_bus, unsigned int i2c_addr,
			 unsigned char *buff, unsigned int len)
{
	unsigned char aml_i2c_bus;
	struct udevice *i2c_dev;
	int i, ret = 0;
	unsigned char data = 0;

	aml_i2c_bus = i2c_bus;
	ret = i2c_get_chip_for_busnum(aml_i2c_bus, i2c_addr, 1, &i2c_dev);
	if (ret) {
		LCDI2C_ERR("no sys aml_i2c_bus %d find\n", aml_i2c_bus);
		return ret;
	}

	if (lcd_debug_print_flag) {
		printf("%s:", __func__);
		for (i = 0; i < len; i++)
			printf(" 0x%02x", buff[i]);
		printf(" [addr 0x%02x]\n", i2c_addr);
	}

	if (len < 1) {
		LCDI2C_ERR("invailed len %d\n", len);
		return -1;
	}
	if (len == 1)
		ret = dm_i2c_write(i2c_dev, buff[0], &data, len);
	else if (len > 1)
		ret = dm_i2c_write(i2c_dev, buff[0], &buff[1], len - 1);

	if (ret) {
		LCDI2C_ERR("i2c write failed [addr 0x%02x]\n", i2c_addr);
		return ret;
	}

	return 0;
}

int aml_lcd_i2c_read(unsigned char i2c_bus, unsigned int i2c_addr,
			unsigned char *buff, unsigned int len)
{
	unsigned char aml_i2c_bus;
	struct udevice *i2c_dev;
	int ret = 0, i;

	aml_i2c_bus = i2c_bus;
	ret = i2c_get_chip_for_busnum(aml_i2c_bus, i2c_addr, 1, &i2c_dev);
	if (ret) {
		LCDI2C_ERR("no sys aml_i2c_bus %d find\n", aml_i2c_bus);
		return ret;
	}

#if 0
	ret = i2c_write(i2c_dev, buff[0], &buff[1], 1);
	if (ret) {
		LCDI2C_ERR("i2c write failed [addr 0x%02x]\n", i2c_addr);
		return ret;
	}
#endif
	if (len < 1) {
		LCDI2C_ERR("invalied len %d\n", len);
		return -1;
	}

	ret = dm_i2c_read(i2c_dev, buff[0], buff, len);
	if (ret) {
		LCDI2C_ERR("i2c read failed [addr 0x%02x]\n", i2c_addr);
		return ret;
	}
	if (lcd_debug_print_flag) {
		printf("%s:", __func__);
		for (i = 0; i < len; i++)
			printf(" 0x%02x", buff[i]);
		printf(" [addr 0x%02x]\n", i2c_addr);
	}

	return 0;
}

