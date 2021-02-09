/*
 * driver/amlogic/media/vout/lcd/backlight/lcd_bl.h
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

#ifndef _AML_LCD_BL_H
#define _AML_LCD_BL_H
#include <amlogic/media/vout/lcd/aml_lcd.h>

void bl_set_pwm_gpio_check(struct aml_bl_drv_s *bdrv, struct bl_pwm_config_s *bl_pwm);

enum bl_pwm_port_e bl_pwm_str_to_num(const char *str);
char *bl_pwm_num_to_str(unsigned int num);
void bl_pwm_set_duty(struct bl_pwm_config_s *bl_pwm);
void bl_pwm_set_level(struct aml_bl_drv_s *bdrv,
		     struct bl_pwm_config_s *bl_pwm, unsigned int level);
void bl_pwm_en(struct bl_pwm_config_s *bl_pwm, int flag);
void bl_pwm_config_init(struct bl_pwm_config_s *bl_pwm);
void bl_pwm_reg_print(struct bl_pwm_config_s *bl_pwm);

#ifdef CONFIG_AML_LOCAL_DIMMING
int ldim_config_load_from_dts(char *dt_addr, int child_offset);
int ldim_config_load_from_unifykey(unsigned char *para);
int ldim_config_load(char *dt_addr);
#endif

#endif

