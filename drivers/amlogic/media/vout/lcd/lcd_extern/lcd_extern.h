/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _LCD_EXTERN_H_
#define _LCD_EXTERN_H_
#include <amlogic/media/vout/lcd/lcd_extern.h>
#include <amlogic/media/vout/lcd/lcd_i2c_dev.h>

#define EXTPR(fmt, args...)     printf("lcd extern: "fmt"", ## args)
#define EXTERR(fmt, args...)    printf("lcd extern: error: "fmt"", ## args)

#define LCD_EXTERN_DRIVER		"lcd_extern"

void udelay(unsigned long usec);
void mdelay(unsigned long msec);

#ifdef CONFIG_OF_LIBFDT
char *lcd_extern_get_dts_prop(int nodeoffset, char *propname);
int lcd_extern_get_dts_child(char *dtaddr, char *snode, int index);
#endif

int lcd_extern_gpio_get(struct lcd_extern_driver_s *ext_drv, unsigned char index);
int lcd_ext_gpio_set(struct lcd_extern_driver_s *ext_drv, unsigned char index, int value);
void lcd_extern_pinmux_set(struct lcd_extern_driver_s *ext_drv, int status);

int lcd_extern_default_probe(struct lcd_extern_driver_s *edrv,
			     struct lcd_extern_dev_s *ext_dev);
int lcd_extern_mipi_default_probe(struct lcd_extern_driver_s *edrv,
				  struct lcd_extern_dev_s *ext_dev);

#ifdef CONFIG_AML_LCD_EXTERN_I2C_RT6947
int lcd_extern_i2c_RT6947_probe(struct lcd_extern_driver_s *edrv,
				struct lcd_extern_dev_s *ext_dev);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_I2C_ANX6862_7911
int lcd_extern_i2c_ANX6862_7911_probe(struct lcd_extern_driver_s *edrv,
				      struct lcd_extern_dev_s *ext_dev);
#endif

#endif

