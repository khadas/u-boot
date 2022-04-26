/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _AML_LCD_COMMON_H
#define _AML_LCD_COMMON_H
#include <div64.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "lcd_clk_config.h"
#include "lcd_unifykey.h"

/* 20181106: init version */
/* 20201211: support t5/t5d/t7 */
/* 20210222: support multi driver index */
/* 20210409: update lvds & vbyone regs for t7*/
/* 20210412: add t3 support*/
/* 20210514: add extern driver support*/
/* 20210518: update t7 & t3 lvds lane setting*/
/* 20210616: fix t3 clk ss mistake*/
/* 20210625: add tcon multi lut support*/
/* 20211009: support 59 & 47 frame rate for tv mode*/
#define LCD_DRV_VERSION    "20211009"

extern unsigned int lcd_debug_test;
extern unsigned long clk_util_clk_msr(unsigned long clk_mux);

void mdelay(unsigned long n);

static inline unsigned int lcd_do_div(unsigned long long num, unsigned int den)
{
	unsigned long long ret = num;

	do_div(ret, den);

	return (unsigned int)ret;
}

void lcd_display_init_test(struct aml_lcd_drv_s *pdrv);
void lcd_display_init_reg_dump(struct aml_lcd_drv_s *pdrv);

/* lcd common */
int lcd_type_str_to_type(const char *str);
char *lcd_type_type_to_str(int type);
int lcd_mode_str_to_mode(const char *str);
char *lcd_mode_mode_to_str(int mode);

int lcd_get_config(char *dt_addr, int load_id, struct aml_lcd_drv_s *pdrv);
void lcd_timing_init_config(struct lcd_config_s *pconf);
int lcd_vmode_change(struct aml_lcd_drv_s *pdrv);
void lcd_pinmux_set(struct aml_lcd_drv_s *pdrv, int status);
void lcd_vbyone_config_set(struct aml_lcd_drv_s *pdrv);
void lcd_mlvds_config_set(struct aml_lcd_drv_s *pdrv);
void lcd_p2p_config_set(struct aml_lcd_drv_s *pdrv);
void lcd_mipi_dsi_config_set(struct aml_lcd_drv_s *pdrv);
void lcd_edp_config_set(struct aml_lcd_drv_s *pdrv);

void lcd_set_venc_timing(struct aml_lcd_drv_s *pdrv);
void lcd_set_venc(struct aml_lcd_drv_s *pdrv);

/* lcd phy */
void lcd_phy_tcon_chpi_bbc_init_tl1(struct lcd_config_s *pconf);
void lcd_phy_set(struct aml_lcd_drv_s *pdrv, int status);
int lcd_phy_probe(struct aml_lcd_drv_s *pdrv);
int lcd_phy_config_init(struct aml_lcd_data_s *pdata);

/*lcd vbyone*/
void lcd_vbyone_enable(struct aml_lcd_drv_s *pdrv);
void lcd_vbyone_disable(struct aml_lcd_drv_s *pdrv);
void lcd_vbyone_sw_reset(struct aml_lcd_drv_s *pdrv);
void lcd_vbyone_wait_timing_stable(struct aml_lcd_drv_s *pdrv);
void lcd_vbyone_cdr_training_hold(struct aml_lcd_drv_s *pdrv, int flag);
void lcd_vbyone_wait_hpd(struct aml_lcd_drv_s *pdrv);
void lcd_vbyone_wait_stable(struct aml_lcd_drv_s *pdrv);
void lcd_vbyone_hw_filter(struct aml_lcd_drv_s *pdrv, int flag);

/* lcd tcon */
#ifdef CONFIG_AML_LCD_TCON
void lcd_tcon_info_print(void);
int lcd_tcon_enable(struct aml_lcd_drv_s *pdrv);
void lcd_tcon_disable(struct aml_lcd_drv_s *pdrv);
int lcd_tcon_probe(char *dt_addr, struct aml_lcd_drv_s *pdrv, int load_id);
#endif

/* lcd gpio */
int lcd_gpio_name_map_num(const char *name);
int lcd_gpio_set(int gpio, int value);
unsigned int lcd_gpio_input_get(int gpio);

/* lcd debug */
void aml_lcd_debug_test(struct aml_lcd_drv_s *pdrv, unsigned int num);
void lcd_mute_setting(struct aml_lcd_drv_s *pdrv, unsigned char flag);
void lcd_info_print(struct aml_lcd_drv_s *pdrv);
void lcd_reg_print(struct aml_lcd_drv_s *pdrv);
void lcd_vbyone_rst(struct aml_lcd_drv_s *pdrv);
void lcd_vbyone_cdr(struct aml_lcd_drv_s *pdrv);
void lcd_debug_probe(struct aml_lcd_drv_s *pdrv);
int lcd_prbs_test(struct aml_lcd_drv_s *pdrv, unsigned int s, unsigned int mode_flag);

/* lcd driver */
#ifdef CONFIG_AML_LCD_TV
int lcd_mode_tv_init(struct aml_lcd_drv_s *pdrv);
#endif
#ifdef CONFIG_AML_LCD_TABLET
int lcd_mode_tablet_init(struct aml_lcd_drv_s *pdrv);
int lcd_mipi_dsi_init_table_detect(char *dt_addr, int child_offset,
				   struct dsi_config_s *dconf, int flag);
int lcd_mipi_dsi_init_table_check_bsp(struct dsi_config_s *dconf, int flag);
void mipi_dsi_print_info(struct lcd_config_s *pconf);
void mipi_dsi_config_init(struct lcd_config_s *pconf);
void mipi_dsi_link_off(struct aml_lcd_drv_s *pdrv);
void mipi_dsi_tx_ctrl(struct aml_lcd_drv_s *pdrv, int flag);
int dptx_edid_dump(struct aml_lcd_drv_s *pdrv);
int dptx_edid_timing_probe(struct aml_lcd_drv_s *pdrv);
void dptx_dpcd_dump(struct aml_lcd_drv_s *pdrv);
void edp_tx_ctrl(struct aml_lcd_drv_s *pdrv, int flag);
#endif
void lcd_wait_vsync(struct aml_lcd_drv_s *pdrv);

/* aml_bl driver */
int aml_bl_probe(char *dtaddr, int load_id);
int aml_bl_remove(void);
int aml_bl_index_add(int drv_index, int conf_index);
int aml_bl_index_remove(int drv_index);
int aml_bl_init(void);
void aml_bl_driver_enable(int index);
void aml_bl_driver_disable(int index);
void aml_bl_set_level(int index, unsigned int level);
unsigned int aml_bl_get_level(int index);
void aml_bl_config_print(int index);
int aml_bl_pwm_reg_config_init(struct aml_lcd_data_s *pdata);

#endif

