/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _INC_AML_LCD_I2C_DEV_H_
#define _INC_AML_LCD_I2C_DEV_H_

unsigned char aml_lcd_i2c_bus_get_str(const char *str);
void aml_lcd_i2c_bus_print(unsigned char i2c_bus);
unsigned char aml_lcd_i2c_bus_get_sys(unsigned char i2c_bus);
int aml_lcd_i2c_write(unsigned char i2c_bus, unsigned int i2c_addr,
			 unsigned char *buff, unsigned int len);
int aml_lcd_i2c_read(unsigned char i2c_bus, unsigned i2c_addr,
			unsigned char *buff, unsigned int len);

#endif

