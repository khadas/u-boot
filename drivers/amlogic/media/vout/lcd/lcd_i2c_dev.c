// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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
	{LCD_EXT_I2C_BUS_0,   "i2c_0"},
	{LCD_EXT_I2C_BUS_1,   "i2c_1"},
	{LCD_EXT_I2C_BUS_2,   "i2c_2"},
	{LCD_EXT_I2C_BUS_3,   "i2c_3"},
	{LCD_EXT_I2C_BUS_4,   "i2c_4"},
	{LCD_EXT_I2C_BUS_0,   "i2c_a"},
	{LCD_EXT_I2C_BUS_1,   "i2c_b"},
	{LCD_EXT_I2C_BUS_2,   "i2c_c"},
	{LCD_EXT_I2C_BUS_3,   "i2c_d"},
	{LCD_EXT_I2C_BUS_4,   "i2c_ao"},
	{LCD_EXT_I2C_BUS_0,   "i2c_bus_0"},
	{LCD_EXT_I2C_BUS_1,   "i2c_bus_1"},
	{LCD_EXT_I2C_BUS_2,   "i2c_bus_2"},
	{LCD_EXT_I2C_BUS_3,   "i2c_bus_3"},
	{LCD_EXT_I2C_BUS_4,   "i2c_bus_4"},
	{LCD_EXT_I2C_BUS_0,   "i2c_bus_a"},
	{LCD_EXT_I2C_BUS_1,   "i2c_bus_b"},
	{LCD_EXT_I2C_BUS_2,   "i2c_bus_c"},
	{LCD_EXT_I2C_BUS_3,   "i2c_bus_d"},
	{LCD_EXT_I2C_BUS_4,   "i2c_bus_ao"},
	{LCD_EXT_I2C_BUS_MAX, "invalid"}
};

unsigned char aml_lcd_i2c_bus_get_str(const char *str)
{
	unsigned char i2c_bus = LCD_EXT_I2C_BUS_MAX;
	int i;

	for (i = 0; i < ARRAY_SIZE(lcd_i2c_match_table); i++) {
		if (strcmp(lcd_i2c_match_table[i].bus_str, str) == 0) {
			i2c_bus = lcd_i2c_match_table[i].bus_id;
			break;
		}
	}

	if (i2c_bus == LCD_EXT_I2C_BUS_MAX)
		LCDI2C_ERR("%s: invalid i2c_bus: %s\n", __func__, str);

	return i2c_bus;
}

void aml_lcd_i2c_bus_print(unsigned char i2c_bus)
{
	char *str = NULL;
	int i;

	for (i = 0; i < ARRAY_SIZE(lcd_i2c_match_table); i++) {
		if (lcd_i2c_match_table[i].bus_id == i2c_bus) {
			str = lcd_i2c_match_table[i].bus_str;
			break;
		}
	}

	LCDI2C_PR("i2c_bus = %s(%d)\n", str, i2c_bus);
}

#ifdef CONFIG_DM_I2C
int aml_lcd_i2c_write(unsigned char i2c_bus, unsigned int i2c_addr,
			 unsigned char *buff, unsigned int len)
{
	unsigned char aml_i2c_bus;
	struct udevice *bus;
	struct udevice *i2c_dev;
	int i, ret = 0;
	unsigned char data = 0;

	aml_i2c_bus = i2c_bus;
	ret = uclass_get_device_by_seq(UCLASS_I2C, aml_i2c_bus, &bus);
	if (ret) {
		LCDI2C_ERR("no sys aml_i2c_bus %d find\n", aml_i2c_bus);
		return ret;
	}
	ret = i2c_get_chip(bus, i2c_addr, 1, &i2c_dev);
	if (ret) {
		LCDI2C_ERR("no sys aml_i2c_bus %d find\n", aml_i2c_bus);
		return ret;
	}

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		printf("%s:", __func__);
		for (i = 0; i < len; i++)
			printf(" 0x%02x", buff[i]);
		printf(" [addr 0x%02x]\n", i2c_addr);
	}

	if (len < 1) {
		LCDI2C_ERR("invalid len %d\n", len);
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
	struct udevice *bus;
	struct udevice *i2c_dev;
	int ret = 0, i;

	aml_i2c_bus = i2c_bus;
	ret = uclass_get_device_by_seq(UCLASS_I2C, aml_i2c_bus, &bus);
	if (ret) {
		LCDI2C_ERR("no sys aml_i2c_bus %d find\n", aml_i2c_bus);
		return ret;
	}
	ret = i2c_get_chip(bus, i2c_addr, 1, &i2c_dev);
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
		LCDI2C_ERR("invalid len %d\n", len);
		return -1;
	}

	ret = dm_i2c_read(i2c_dev, buff[0], buff, len);
	if (ret) {
		LCDI2C_ERR("i2c read failed [addr 0x%02x]\n", i2c_addr);
		return ret;
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		printf("%s:", __func__);
		for (i = 0; i < len; i++)
			printf(" 0x%02x", buff[i]);
		printf(" [addr 0x%02x]\n", i2c_addr);
	}

	return 0;
}

#else
int aml_lcd_i2c_write(unsigned char i2c_bus, unsigned int i2c_addr,
			 unsigned char *buff, unsigned int len)
{
	LCDI2C_ERR("no CONFIG_DM_I2C\n");
	return -1;
}

int aml_lcd_i2c_read(unsigned char i2c_bus, unsigned int i2c_addr,
			unsigned char *buff, unsigned int len)
{
	LCDI2C_ERR("no CONFIG_DM_I2C\n");
	return -1;
}
#endif
