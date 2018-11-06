/*
 * drivers/amlogic/media/vout/lcd/lcd_extern/lcd_extern.h
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

#ifndef _LCD_EXTERN_H_
#define _LCD_EXTERN_H_
#include <amlogic/media/vout/lcd/lcd_extern.h>

#define EXTPR(fmt, args...)     printf("lcd extern: "fmt"", ## args)
#define EXTERR(fmt, args...)    printf("lcd extern: error: "fmt"", ## args)

#define LCD_EXTERN_DRIVER		"lcd_extern"

struct aml_lcd_extern_pinmux_s {
	unsigned reg;
	unsigned mux;
};

extern void udelay(unsigned long usec);
extern void mdelay(unsigned long msec);

extern void lcd_extern_i2c_bus_print(unsigned char i2c_bus);
extern int lcd_extern_i2c_write(unsigned char i2c_bus, unsigned i2c_addr,
		unsigned char *buff, unsigned len);
extern int lcd_extern_i2c_read(unsigned char i2c_bus, unsigned i2c_addr,
		unsigned char *buff, unsigned int len);

extern char *lcd_extern_get_dts_prop(int nodeoffset, char *propname);
extern int lcd_extern_get_dts_child(int index);

extern int lcd_ext_gpio_set(unsigned char index, int value);
extern unsigned int lcd_ext_gpio_input_get(unsigned char index);
extern void lcd_extern_pinmux_set(int status);

extern int lcd_extern_default_probe(struct aml_lcd_extern_driver_s *ext_drv);
extern int lcd_extern_mipi_default_probe(struct aml_lcd_extern_driver_s *ext_drv);

#ifdef CONFIG_AML_LCD_EXTERN_I2C_T5800Q
extern int lcd_extern_i2c_T5800Q_probe(struct aml_lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_I2C_DLPC3439
extern int lcd_extern_i2c_DLPC3439_probe(struct aml_lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_I2C_RT6947
extern int lcd_extern_i2c_RT6947_probe(struct aml_lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_I2C_ANX6862_7911
extern int lcd_extern_i2c_ANX6862_7911_probe(struct aml_lcd_extern_driver_s *ext_drv);
#endif

#ifdef CONFIG_AML_LCD_EXTERN_SPI_LD070WS2
extern int lcd_extern_spi_LD070WS2_probe(struct aml_lcd_extern_driver_s *ext_drv);
#endif

#ifdef CONFIG_AML_LCD_EXTERN_MIPI_TV070WSM
extern int lcd_extern_mipi_TV070WSM_probe(struct aml_lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_MIPI_P070ACB
extern int lcd_extern_mipi_p070acb_probe(struct aml_lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_MIPI_ST7701
extern int lcd_extern_mipi_st7701_probe(struct aml_lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_MIPI_TL050FHV02CT
extern int lcd_extern_mipi_tl050fhv02ct_probe(struct aml_lcd_extern_driver_s *ext_drv);
#endif

#endif

