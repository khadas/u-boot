/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __AML_LCD_TV_H__
#define __AML_LCD_TV_H__
#include <amlogic/media/vout/lcd/lcd_vout.h>

//**********************************
//lcd driver version
//**********************************
#define LCD_DRV_TYPE      "tv"

//**********************************

void lcd_tv_config_update(struct aml_lcd_drv_s *pdrv);
void lcd_tv_driver_init_pre(struct aml_lcd_drv_s *pdrv);
int lcd_tv_driver_init(struct aml_lcd_drv_s *pdrv);
void lcd_tv_driver_disable(struct aml_lcd_drv_s *pdrv);

#endif
