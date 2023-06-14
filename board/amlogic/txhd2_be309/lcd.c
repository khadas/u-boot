/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * board/amlogic/t5d_skt_v1/lcd.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>

/***************************************************
 * lcd_0
 ***************************************************/
static char lcd0_cpu_gpio[LCD_CPU_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOZ_3", /* panel_pwr */
	"invalid", /* ending flag */
};

static char lcd0_bl_gpio[BL_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOZ_2", /* bl_en */
	"GPIOZ_1", /* bl_pwm */
	"invalid", /* ending flag */
};

static struct lcd_power_step_s lcd0_power_on_step[] = {
	{LCD_POWER_TYPE_CPU,    0, 1, 50,}, /* panel vcc */
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},  /* ending flag */
};

static struct lcd_power_step_s lcd0_power_off_step[] = {
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_CPU,   0, 0, 500,}, /* panel vcc */
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},   /* ending flag */
};

struct ext_lcd_config_s ext_lcd0_config[LCD_NUM_MAX] = {
	{/* normal*/
	"lvds_0",LCD_LVDS,8,
	/* basic timing */
	1920,1080,2200,1125,44,148,0,5,36,0,
	/* clk_attr */
	2,0,1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* lvds_attr */
	1,1,0,0,0,0x3,0x0,Rsv_val,Rsv_val,Rsv_val,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	60,255,10,128,128,
	BL_CTRL_PWM,0,1,0,200,200,
	BL_PWM_POSITIVE, BL_PWM_B, 180, 100, 25, 1, 0,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{/* for HDMI convert*/
	"lvds_1",LCD_LVDS,8,
	/* basic timing */
	1920,1080,2200,1125,44,148,0,5,36,0,
	/* clk_attr */
	4,0,1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* lvds_attr */
	1,1,0,0,0,0x3,0x0,Rsv_val,Rsv_val,Rsv_val,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	60,255,10,128,128,
	BL_CTRL_PWM,0,1,0,200,200,
	BL_PWM_POSITIVE,BL_PWM_VS,3,100,25,1,0,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{/* for HDMI convert*/
	"mipi_0",LCD_MIPI,8,
	/* basic timing */
	1920,1080,2200,1125,44,148,0,5,36,0,
	/* clk_attr */
	1,0,1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* lvds_attr */
	1,1,0,0,0,0xf,0x0,Rsv_val,Rsv_val,Rsv_val,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	60,255,10,128,128,
	BL_CTRL_MAX,0,1,0,200,200,
	BL_PWM_POSITIVE, BL_PWM_B, 180, 100, 25, 1, 0,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},
};

static struct lcd_pinmux_ctrl_s lcd0_pinmux_ctrl[LCD_PINMUX_MAX] = {
	{
		.name = "invalid",
	},
};

static struct lcd_pinmux_ctrl_s lcd0_bl_pinmux_ctrl[BL_PINMUX_MAX] = {
	{
		.name = "bl_pwm_on_pin", //GPIOZ_1
		.pinmux_set = {{4, 0x40}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{4, 0xf0}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "bl_pwm_vs_on_pin", //GPIOZ_1
		.pinmux_set = {{4, 0x30}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{4, 0xf0}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "invalid",
	},
};

/*********************************************
 * lcd default config
 ***************************************************/
static struct lcd_dft_config_s lcd_dft_conf[] = {
	{//index 0
		.lcd_gpio = lcd0_cpu_gpio,
		.key_valid = 0,
		.clk_path = 0,
		.mode = LCD_MODE_TV,
		.ext_lcd = ext_lcd0_config,
		.lcd_pinmux = lcd0_pinmux_ctrl,

		.bl_gpio = lcd0_bl_gpio,
		.bl_pinmux = lcd0_bl_pinmux_ctrl,
	},
};

void lcd_config_bsp_init(void)
{
	struct aml_lcd_data_s *pdata = aml_lcd_get_data();

	if (pdata)
		pdata->dft_conf[0] = &lcd_dft_conf[0];
}
