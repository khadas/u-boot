/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/lcd/lcd_tablet/lcd_tablet.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __AML_LCD_TABLET_H__
#define __AML_LCD_TABLET_H__

//**********************************
//lcd driver version
//**********************************
#define LCD_DRV_TYPE      "tablet"

//**********************************

extern void lcd_tablet_config_update(struct lcd_config_s *pconf);
extern void lcd_tablet_driver_init_pre(void);
extern int lcd_tablet_driver_init(void);
extern void lcd_tablet_driver_disable(void);

#endif
