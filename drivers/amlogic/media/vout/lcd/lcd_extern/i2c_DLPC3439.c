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

#define LCD_EXTERN_NAME			"i2c_DLPC3439"
#define LCD_EXTERN_TYPE			LCD_EXTERN_I2C
#define LCD_EXTERN_I2C_ADDR		(0x36 >> 1) //7bit address

static struct lcd_extern_config_s *ext_config;

	/** Write: ImageCrop: 1920x1080
	W 36 10 00 00 00 00 80 07 38 04 **/
static unsigned char data_1[] = {0x10, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x07, 0x38, 0x04};
	/** Write: DisplaySize: 1920x1080
	W 36 12 80 07 38 04 **/
static unsigned char data_2[] = {0x12, 0x80, 0x07, 0x38, 0x04};
	/** Write: InputImageSize: 1920x1080
	W 36 2e 80 07 38 04 **/
static unsigned char data_3[] = {0x2e, 0x80, 0x07, 0x38, 0x04};
	/** Write: InputSourceSelect; 0 = External Video Port
	W 36 05 00 **/
static unsigned char data_4[] = {0x05, 0x00};
	/** Write: VideoSourceFormatSelect: 0x43=RGB888
	W 36 07 43 **/
static unsigned char data_5[] = {0x07, 0x43};

static int lcd_extern_power_on(void)
{
	int ret = 0;

	lcd_extern_pinmux_set(ext_config, 1);

	aml_lcd_i2c_write(ext_config->i2c_bus, ext_config->i2c_addr, data_1, 9);
	aml_lcd_i2c_write(ext_config->i2c_bus, ext_config->i2c_addr, data_2, 5);
	aml_lcd_i2c_write(ext_config->i2c_bus, ext_config->i2c_addr, data_3, 5);
	aml_lcd_i2c_write(ext_config->i2c_bus, ext_config->i2c_addr, data_4, 2);
	aml_lcd_i2c_write(ext_config->i2c_bus, ext_config->i2c_addr, data_5, 2);

	EXTPR("%s\n", __func__);
	return ret;
}

static int lcd_extern_power_off(void)
{
	int ret = 0;

	lcd_extern_pinmux_set(ext_config, 0);
	return ret;
}

static int lcd_extern_driver_update(struct aml_lcd_extern_driver_s *ext_drv)
{
	int ret = 0;

	if (ext_drv == NULL) {
		EXTERR("%s driver is null\n", LCD_EXTERN_NAME);
		return -1;
	}

	ext_drv->power_on  = lcd_extern_power_on;
	ext_drv->power_off = lcd_extern_power_off;

	return ret;
}

int lcd_extern_i2c_DLPC3439_probe(struct aml_lcd_extern_driver_s *ext_drv)
{
	int ret = 0;

	ext_config = ext_drv->config;
	ret = lcd_extern_driver_update(ext_drv);

	if (lcd_debug_print_flag)
		EXTPR("%s: %d\n", __func__, ret);
	return ret;
}

