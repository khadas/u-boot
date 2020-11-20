// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * include/amlogic/aml_lcd_i2c_dev.h
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

#ifndef _INC_AML_LCD_I2C_DEV_H_
#define _INC_AML_LCD_I2C_DEV_H_

void lcd_extern_i2c_bus_print(unsigned char i2c_bus);
unsigned char aml_lcd_extern_i2c_bus_get_sys(unsigned char i2c_bus);
int lcd_extern_i2c_bus_change(unsigned int i2c_bus);
int lcd_extern_i2c_bus_recovery(void);
int lcd_extern_i2c_write(unsigned char i2c_bus, unsigned int i2c_addr,
			 unsigned char *buff, unsigned int len);
int lcd_extern_i2c_read(unsigned char i2c_bus, unsigned i2c_addr,
			unsigned char *buff, unsigned int len);

#endif

