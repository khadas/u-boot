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
#include <amlogic/media/vout/lcd/lcd_i2c_dev.h>

#define EXTPR(fmt, args...)     printf("lcd extern: "fmt"", ## args)
#define EXTERR(fmt, args...)    printf("lcd extern: error: "fmt"", ## args)

#define LCD_EXTERN_DRIVER		"lcd_extern"

struct aml_lcd_extern_pinmux_s {
	unsigned reg;
	unsigned mux;
};

#ifdef CONFIG_SYS_I2C_AML
extern struct i2c_platform g_aml_i2c_plat;
#endif
extern void udelay(unsigned long usec);
extern void mdelay(unsigned long msec);

#ifdef CONFIG_OF_LIBFDT
char *aml_lcd_extern_get_dts_prop(int nodeoffset, char *propname);
int aml_lcd_extern_get_dts_child(int index);
#endif

extern int lcd_ext_gpio_set(unsigned char index, int value);
extern unsigned int lcd_ext_gpio_input_get(unsigned char index);
void lcd_extern_pinmux_set(struct lcd_extern_config_s *extconf, int status);

int lcd_extern_default_probe(struct lcd_extern_driver_s *ext_drv);
int lcd_extern_mipi_default_probe(struct lcd_extern_driver_s *ext_drv);

#ifdef CONFIG_AML_LCD_EXTERN_I2C_T5800Q
int lcd_extern_i2c_T5800Q_probe(struct lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_I2C_ANX6345
int lcd_extern_i2c_anx6345_probe(struct lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_I2C_DLPC3439
int lcd_extern_i2c_DLPC3439_probe(struct lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_I2C_RT6947
int lcd_extern_i2c_RT6947_probe(struct lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_I2C_CS602
int lcd_extern_i2c_CS602_probe(struct lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_I2C_ANX6862_7911
int lcd_extern_i2c_ANX6862_7911_probe(struct lcd_extern_driver_s *ext_drv);
#endif

#ifdef CONFIG_AML_LCD_EXTERN_SPI_LD070WS2
int lcd_extern_spi_LD070WS2_probe(struct lcd_extern_driver_s *ext_drv);
#endif

#ifdef CONFIG_AML_LCD_EXTERN_MIPI_TV070WSM
int lcd_extern_mipi_TV070WSM_probe(struct lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_MIPI_KD080D13
int lcd_extern_mipi_KD080D13_probe(struct lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_MIPI_TV070WSM
int aml_lcd_extern_mipi_TV070WSM_probe(struct lcd_extern_driver_s *ext_drv);
#endif

#ifdef CONFIG_AML_LCD_EXTERN_MIPI_ST7701
int lcd_extern_mipi_st7701_probe(struct lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_MIPI_P070ACB
int lcd_extern_mipi_p070acb_probe(struct lcd_extern_driver_s *ext_drv);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_MIPI_TL050FHV02CT
int lcd_extern_mipi_tl050fhv02ct_probe(struct lcd_extern_driver_s *ext_drv);
#endif

#endif

