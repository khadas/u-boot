// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>

/***************************************************
 * lcd_0
 ***************************************************/
static char lcd0_cpu_gpio[LCD_CPU_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOX_16", /* LCD_PWREN */
	"GPIOT_22", /* LCD_RST */
	"invalid", /* ending flag */
};

static char lcd0_bl_gpio[BL_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOD_5", /* enable */
	"GPIOD_9",  /* pwm_n */
	"invalid", /* ending flag */
};

static struct lcd_power_step_s lcd0_power_on_step[] = {
	{LCD_POWER_TYPE_CPU,    0, 1, 100,}, /* panel vcc */
	{LCD_POWER_TYPE_CPU,    1, 0, 10,},  /* reset low */
	{LCD_POWER_TYPE_CPU,    1, 1, 20,},  /* reset high */
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_MAX,    0, 0, 0,},  /* ending flag */
};

static struct lcd_power_step_s lcd0_power_off_step[] = {
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 50,},
	{LCD_POWER_TYPE_CPU,    1, 0, 10,},  /* reset low */
	{LCD_POWER_TYPE_CPU,    0, 0, 500,}, /* panel vcc */
	{LCD_POWER_TYPE_MAX,    0, 0, 0,},   /* ending flag */
};

struct ext_lcd_config_s ext_lcd0_config[LCD_NUM_MAX] = {
	{/* rgb*/
	"rgb_0", LCD_RGB, 6,
	/* basic timing */
	1024, 600, 1312, 635, 40, 140, 0, 5, 23, 0,
	/* clk_attr */
	0, 0, 1, 51200000, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* rgb_attr */
	0, 1, 1, 1, 0, 0, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* cmd init */
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	100, 255, 10, 128, 128,
	BL_CTRL_MAX, 0, 1, 0, 200, 200,
	BL_PWM_NEGATIVE, BL_PWM_E, 180, 100, 25, 1, 1,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},
	{.panel_type = "invalid"},
};

static struct lcd_pinmux_ctrl_s lcd0_pinmux_ctrl[LCD_PINMUX_MAX] = {
	{
		.name = "lcd_rgb_on_pin",
		.pinmux_set = {
			{0xb, 0x55555555},  //GPIOT
			{0xc, 0x55555555},
			{0xd, 0x55555},
			{LCD_PINMUX_END, 0x0}
		},
		.pinmux_clr = {
			{0xb, 0xffffffff},
			{0xc, 0xffffffff},
			{0xd, 0xfffff},
			{LCD_PINMUX_END, 0x0}
		},
	},
	{
		.name = "invalid",
	},
};

static struct lcd_pinmux_ctrl_s lcd0_bl_pinmux_ctrl[BL_PINMUX_MAX] = {
	{
		.name = "bl_pwm_vs_on_pin",
		.pinmux_set = {{0x8, 0x30000}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{0x8, 0xf0000}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "invalid",
	},
};

/***************************************************
 * lcd default config
 ***************************************************/
static struct lcd_dft_config_s lcd_dft_conf[] = {
	{//index 0
		.lcd_gpio = lcd0_cpu_gpio,
		.key_valid = 0,
		.clk_path = 0,
		.mode = LCD_MODE_TABLET,
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
