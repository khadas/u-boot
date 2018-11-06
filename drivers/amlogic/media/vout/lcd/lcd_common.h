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
#define LCD_DRV_VERSION    "20181106"

#define VPP_OUT_SATURATE            (1 << 0)

/* -------------------------- */
/* lvsd phy parameters define */
/* -------------------------- */
#define LVDS_PHY_CNTL1_G9TV    0x606cca80
#define LVDS_PHY_CNTL2_G9TV    0x0000006c
#define LVDS_PHY_CNTL3_G9TV    0x00000800

#define LVDS_PHY_CNTL1_TXHD    0x6c60ca80
#define LVDS_PHY_CNTL2_TXHD    0x00000070
#define LVDS_PHY_CNTL3_TXHD    0x03ff0c00
/* -------------------------- */

/* -------------------------- */
/* vbyone phy parameters define */
/* -------------------------- */
#define VX1_PHY_CNTL1_G9TV            0x6e0ec900
#define VX1_PHY_CNTL1_G9TV_PULLUP     0x6e0f4d00
#define VX1_PHY_CNTL2_G9TV            0x0000007c
#define VX1_PHY_CNTL3_G9TV            0x00ff0800
/* -------------------------- */

/* -------------------------- */
/* minilvds phy parameters define */
/* -------------------------- */
#define MLVDS_PHY_CNTL1_TXHD   0x6c60ca80
#define MLVDS_PHY_CNTL2_TXHD   0x00000070
#define MLVDS_PHY_CNTL3_TXHD   0x03ff0c00
/* -------------------------- */

/* ******** MIPI_DSI_PHY ******** */
/* bit[15:11] */
#define MIPI_PHY_LANE_BIT        11
#define MIPI_PHY_LANE_WIDTH      5

/* MIPI-DSI */
#define DSI_LANE_0              (1 << 4)
#define DSI_LANE_1              (1 << 3)
#define DSI_LANE_CLK            (1 << 2)
#define DSI_LANE_2              (1 << 1)
#define DSI_LANE_3              (1 << 0)
#define DSI_LANE_COUNT_1        (DSI_LANE_CLK | DSI_LANE_0)
#define DSI_LANE_COUNT_2        (DSI_LANE_CLK | DSI_LANE_0 | DSI_LANE_1)
#define DSI_LANE_COUNT_3        (DSI_LANE_CLK | DSI_LANE_0 |\
					DSI_LANE_1 | DSI_LANE_2)
#define DSI_LANE_COUNT_4        (DSI_LANE_CLK | DSI_LANE_0 |\
					DSI_LANE_1 | DSI_LANE_2 | DSI_LANE_3)
/* -------------------------- */

extern void mdelay(unsigned long n);
extern unsigned int lcd_debug_load_flag;

/* lcd common */
extern int lcd_type_str_to_type(const char *str);
extern char *lcd_type_type_to_str(int type);
extern int lcd_mode_str_to_mode(const char *str);
extern char *lcd_mode_mode_to_str(int mode);

extern void lcd_pinmux_set(int status);

extern unsigned int lcd_lvds_channel_on_value(struct lcd_config_s *pconf);
extern int lcd_power_load_from_dts(struct lcd_config_s *pconf,
		const void *dt_blob, int child_offset);
extern int lcd_power_load_from_unifykey(struct lcd_config_s *pconf,
		unsigned char *buf, int key_len, int len);
extern int lcd_pinmux_load_config(const void *dt_blob, struct lcd_config_s *pconf);
extern void lcd_timing_init_config(struct lcd_config_s *pconf);
extern int lcd_vmode_change(struct lcd_config_s *pconf);

/* lcd tcon */
extern void lcd_tcon_reg_table_print(void);
extern void lcd_tcon_reg_readback_print(void);
extern void lcd_tcon_info_print(void);
extern int lcd_tcon_enable(struct lcd_config_s *pconf);
extern void lcd_tcon_disable(void);
extern int lcd_tcon_probe(const void *dt_blob, struct aml_lcd_drv_s *lcd_drv, int load_id);

/* lcd gpio */
extern void lcd_cpu_gpio_info_print(void);
extern int lcd_gpio_probe(const char *name, int index);
extern int lcd_gpio_set(int gpio, int value);
extern unsigned int lcd_gpio_input_get(int gpio);

/* lcd debug */
extern void lcd_debug_test(unsigned int num);
extern void lcd_mute_setting(unsigned char flag);
extern void lcd_info_print(void);
extern void lcd_reg_print(void);
extern void lcd_debug_probe(struct aml_lcd_drv_s *lcd_drv);

/* lcd driver */
extern int get_lcd_tv_config(const void *dt_blob, int load_id);
extern int get_lcd_tablet_config(const void *dt_blob, int load_id);

extern void lcd_wait_vsync(void);

/* aml_bl driver */
extern void bl_config_print(void);
extern void bl_pwm_config_update(struct bl_config_s *bconf);
extern void bl_set_level(unsigned int level);
extern unsigned int bl_get_level(void);
extern void bl_power_ctrl(int status);
extern int bl_config_load(const void *dt_blob, int load_id);
#ifdef CONFIG_AML_LOCAL_DIMMING
extern int ldim_config_load_from_dts(const void *dt_blob, int child_offset);
extern int ldim_config_load_from_unifykey(unsigned char *para);
extern int ldim_config_load(const void *dt_blob);
#endif

#endif

