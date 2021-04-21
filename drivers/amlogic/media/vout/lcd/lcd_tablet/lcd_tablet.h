/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __AML_LCD_TABLET_H__
#define __AML_LCD_TABLET_H__
#include <amlogic/media/vout/lcd/lcd_vout.h>

//**********************************
//lcd driver version
//**********************************
#define LCD_DRV_TYPE      "tablet"

//**********************************

void lcd_tablet_config_update(struct aml_lcd_drv_s *pdrv);
void lcd_tablet_driver_init_pre(struct aml_lcd_drv_s *pdrv);
int lcd_tablet_driver_init(struct aml_lcd_drv_s *pdrv);
void lcd_tablet_driver_disable(struct aml_lcd_drv_s *pdrv);

#endif
