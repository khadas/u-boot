/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _AML_LCD_BL_H
#define _AML_LCD_BL_H
#include <amlogic/media/vout/lcd/aml_lcd.h>

void bl_set_pwm_gpio_check(struct aml_bl_drv_s *bdrv, struct bl_pwm_config_s *bl_pwm);

enum bl_pwm_port_e bl_pwm_str_to_num(const char *str);
char *bl_pwm_num_to_str(unsigned int num);
void bl_set_pwm(struct bl_pwm_config_s *bl_pwm);
void bl_pwm_set_level(struct aml_bl_drv_s *bdrv,
		     struct bl_pwm_config_s *bl_pwm, unsigned int level);
void bl_pwm_en(struct bl_pwm_config_s *bl_pwm, int flag);
void bl_pwm_config_init(struct bl_pwm_config_s *bl_pwm);
void bl_pwm_reg_print(struct bl_pwm_config_s *bl_pwm);

#endif

