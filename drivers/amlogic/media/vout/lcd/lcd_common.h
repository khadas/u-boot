/*
 * driver/amlogic/media/vout/lcd/lcd_common.h
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

#ifndef _AML_LCD_COMMON_H
#define _AML_LCD_COMMON_H
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "lcd_clk_config.h"
#include "lcd_unifykey.h"

/* 20181106: init version */
/* 20201211: support t5/t5d/t7 */
#define LCD_DRV_VERSION    "20201211"

#define LCD_STATUS_IF_ON      (1 << 0)
#define LCD_STATUS_ENCL_ON    (1 << 1)
#define VPP_OUT_SATURATE      (1 << 0)

extern unsigned int lcd_debug_test;

void mdelay(unsigned long n);

void lcd_display_init_test(void);
void lcd_display_init_reg_dump(void);
void lcd_edp_debug(void);

/* lcd common */
int lcd_type_str_to_type(const char *str);
char *lcd_type_type_to_str(int type);
int lcd_mode_str_to_mode(const char *str);
char *lcd_mode_mode_to_str(int mode);

void lcd_pinmux_set(int status);

int lcd_power_load_from_dts(struct lcd_config_s *pconf,
		char *dt_addr, int child_offset);
int lcd_power_load_from_unifykey(struct lcd_config_s *pconf,
		unsigned char *buf, int key_len, int len);
int lcd_pinmux_load_config(char *dt_addr, struct lcd_config_s *pconf);
void lcd_timing_init_config(struct lcd_config_s *pconf);
int lcd_vmode_change(struct lcd_config_s *pconf);

/* lcd phy */
void lcd_lvds_phy_set(struct lcd_config_s *pconf, int status);
void lcd_vbyone_phy_set(struct lcd_config_s *pconf, int status);
void lcd_mlvds_phy_set(struct lcd_config_s *pconf, int status);
void lcd_p2p_phy_set(struct lcd_config_s *pconf, int status);
void lcd_mipi_phy_set(struct lcd_config_s *pconf, int status);
int lcd_phy_probe(void);
void lcd_phy_tcon_chpi_bbc_init_tl1(struct lcd_config_s *pconf);

/* lcd tcon */
void lcd_tcon_info_print(void);
int lcd_tcon_enable(struct lcd_config_s *pconf);
void lcd_tcon_disable(void);
int lcd_tcon_probe(char *dt_addr, struct lcd_drv_s *lcd_drv, int load_id);

/* lcd gpio */
int lcd_gpio_name_map_num(const char *name);
int lcd_gpio_set(int gpio, int value);
unsigned int lcd_gpio_input_get(int gpio);

/* lcd debug */
void aml_lcd_debug_test(unsigned int num);
void lcd_mute_setting(unsigned char flag);
void lcd_info_print(void);
void lcd_reg_print(void);
void aml_lcd_vbyone_rst(void);
void aml_lcd_vbyone_cdr(void);
void lcd_debug_probe(struct lcd_drv_s *lcd_drv);
int lcd_prbs_test(unsigned int s, unsigned int mode_flag);

/* lcd driver */
int get_lcd_tv_config(char *dt_addr, int load_id);
int get_lcd_tablet_config(char *dt_addr, int load_id);

void lcd_wait_vsync(void);

/* aml_bl driver */
void bl_config_print(void);
void bl_pwm_config_update(struct bl_config_s *bconf);
void bl_set_level(unsigned int level);
unsigned int bl_get_level(void);
void bl_power_ctrl(int status, int delay_flag);
int bl_config_load(char *dt_addr, int load_id);
#ifdef CONFIG_AML_LOCAL_DIMMING
int ldim_config_load_from_dts(char *dt_addr, int child_offset);
int ldim_config_load_from_unifykey(unsigned char *para);
int ldim_config_load(char *dt_addr);
#endif

#endif

