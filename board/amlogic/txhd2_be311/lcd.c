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
	BL_PWM_POSITIVE,BL_PWM_D,180,100,25,1,0,
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
	BL_PWM_POSITIVE,BL_PWM_C,180,100,25,1,0,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{/* for HDMI convert*/
	"mlvds_0", LCD_MLVDS, 8,
	/* basic timing */
	1920, 1080, 2200, 1125, 44, 148, 0, 5, 36, 0,
	/* clk_attr */
	2, 3, 1, 0, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* mlvds_attr */
	6, 0x123f4567, 0x0000fff0, 0x660, 0x0, 0, 3, 0, 0, 0,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	60, 255, 10, 128, 128,
	BL_CTRL_MAX, 0, 1, 0, 200, 200,
	BL_PWM_POSITIVE, BL_PWM_C, 180, 100, 25, 1, 0,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},
};

static struct lcd_pinmux_ctrl_s lcd0_pinmux_ctrl[LCD_PINMUX_MAX] = {
	{
		.name = "lcd_minilvds_pin", //GPIOH_0~6,8~12
		.pinmux_set = {{5, 0x01111111}, {6, 0x00011111}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{5, 0x0fffffff}, {6, 0x000fffff}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "invalid",
	},
};

static struct lcd_pinmux_ctrl_s lcd0_bl_pinmux_ctrl[BL_PINMUX_MAX] = {
	{
		.name = "invalid",
	},
};

#ifdef CONFIG_AML_LCD_EXTERN
static char lcd_ext_gpio[LCD_EXTERN_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"invalid", /* ending flag */
};

static struct lcd_pinmux_ctrl_s lcd_ext_pinmux_ctrl[LCD_PINMUX_NUM] = {
	{
		.name = "invalid",
	},
};

static unsigned char init_on_table[LCD_EXTERN_INIT_ON_MAX] = {
	0xb0, 49, 0x00,
	0x3f, 0x11, 0xe1, 0x16, 0x03, 0x0b, 0x0b, 0x0a, 0x0a, 0x0b,
	0xdb, 0x58, 0x43, 0x10, 0x03, 0xbd, 0x03, 0x9a, 0x02, 0xe7,
	0x02, 0x98, 0x02, 0x73, 0x02, 0x3a, 0x02, 0x13, 0x01, 0xb5,
	0x01, 0x8c, 0x01, 0x24, 0x00, 0xf1, 0x00, 0x99, 0x00, 0x1a,
	0x00, 0x0d, 0x00, 0x8f, 0x00, 0x90, 0x44, 0x8d,
	0xff, 0, //end flag
};

static unsigned char init_off_table[LCD_EXTERN_INIT_OFF_MAX] = {
	0xff, 0,  /* ending */
};

static struct lcd_extern_common_s ext_common_dft = {
	.key_valid = 0,
	.ext_num = 1,
	.ext_gpio = lcd_ext_gpio,

	.i2c_bus = LCD_EXTERN_I2C_BUS_3,
	.i2c_sck_gpio = 0xff,
	.i2c_sck_gpio_off = 0,
	.i2c_sda_gpio = 0xff,
	.i2c_sda_gpio_off = 0,
	.ext_pinmux = lcd_ext_pinmux_ctrl,
};

static struct lcd_extern_config_s ext_config_dtf[LCD_EXTERN_DEV_MAX] = {
	{
		.index = 0,
		.name = "ext_default",
		/* LCD_EXTERN_I2C, LCD_EXTERN_SPI, LCD_EXTERN_MAX */
		.type = LCD_EXTERN_I2C,
		.status = 1, /* 0=disable, 1=enable */
		.i2c_addr = 0x20, /* 7bit i2c address */
		.i2c_addr2 = 0xff, /* 7bit i2c address, 0xff for none */
		.i2c_addr3 = 0xff,
		.i2c_addr4 = 0xff,

		.cmd_size = 0xff,
		.table_init_on = init_on_table,
		.table_init_off = init_off_table,
	},
	{
		.index = LCD_EXTERN_INDEX_INVALID,
	},
};
#endif

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
#ifdef CONFIG_AML_LCD_EXTERN
		.ext_common = &ext_common_dft,
		.ext_conf = ext_config_dtf,
#endif
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
