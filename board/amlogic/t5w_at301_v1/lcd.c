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
	"GPIOH_7", /* 12V */
	"GPIOH_3", /* sel_lvds */
	"invalid", /* ending flag */
};

static char lcd0_bl_gpio[BL_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOH_13", /* enable */
	"GPIOH_12",  /* pwm_d */
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

static struct lcd_power_step_s lcd0_power_on_step_p2p[] = {
	{LCD_POWER_TYPE_CPU,    0, 1, 300,}, /* panel vcc */
	{LCD_POWER_TYPE_EXTERN, 0, 0, 50,}, /* pmu i2c init */
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},  /* ending flag */
};

struct ext_lcd_config_s ext_lcd0_config[LCD_NUM_MAX] = {
	{/* normal*/
	"lvds_0",LCD_LVDS,8,
	/* basic timing */
	1920,1080,2200,1125,44,148,0,5,36,0,
	/* clk_attr */
	0,0,1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* lvds_attr */
	1,1,0,0,0,0x3,0x0,Rsv_val,Rsv_val,Rsv_val,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	60,255,10,128,128,
	BL_CTRL_PWM,0,1,0,200,200,
	BL_PWM_POSITIVE,BL_PWM_C,180,100,25,1,0,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{/* for HDMI convert*/
	"lvds_1",LCD_LVDS,8,
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

	{/*public 2-region vx1 : 3840x2160@60hz 8lane */
	"vbyone_0",LCD_VBYONE,10,
	/* basic timing */
	3840,2160,4400,2250,33,477,0,6,81,0,
	/* clk_attr */
	2,0,1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* vbyone_attr */
	8,2,4,4,0xf,0x1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	60,255,10,128,128,
	BL_CTRL_PWM,0,1,0,200,200,
	BL_PWM_POSITIVE,BL_PWM_C,180,100,25,1,0,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{/*public 1-region vx1 : 3840x2160@60hz 8lane */
	"vbyone_1",LCD_VBYONE,10,
	/* basic timing */
	3840,2160,4400,2250,33,477,0,6,81,0,
	/* clk_attr */
	2,0,1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* vbyone_attr */
	8,1,4,4,0xf,0x1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	60,255,10,128,128,
	BL_CTRL_PWM,0,1,0,200,200,
	BL_PWM_POSITIVE,BL_PWM_C,180,100,25,1,0,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{/*public p2p ceds : 3840x2160@60hz 12lane */
	"p2p_0",LCD_P2P,8,
	/* basic timing */
	3840,2160,5000,2250,16,29,0,6,81,0,
	/* clk_attr */
	2,0,1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* p2p attr */
	0x0,12,0x76543210,0xba98,0,0,0xf,0x1,Rsv_val,Rsv_val,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step_p2p, lcd0_power_off_step,
	/* backlight */
	60,255,10,128,128,
	BL_CTRL_MAX,0,1,0,200,200,
	BL_PWM_POSITIVE,BL_PWM_C,180,100,25,1,0,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{/*public p2p ceds : 3840x2160@60hz 6lane */
	"p2p_1",LCD_P2P,8,
	/* basic timing */
	3840,2160,4400,2250,16,29,0,6,81,0,
	/* clk_attr */
	2,0,1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* p2p attr */
	0x11,12,0x76543210,0xba98,0,0,0x5,0x1,Rsv_val,Rsv_val,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step_p2p, lcd0_power_off_step,
	/* backlight */
	60,255,10,128,128,
	BL_CTRL_MAX,0,1,0,200,200,
	BL_PWM_POSITIVE,BL_PWM_C,180,100,25,1,0,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{/* 1920*1080*/
	"mlvds_0",LCD_MLVDS,8,
	/* basic timing */
	1920,1080,2200,1125,44,148,0,5,36,0,
	/* clk_attr */
	2,0,1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* minilvds_attr */
	6,0x76543210,0xba98,0x660,0,0,0xf,0x0,Rsv_val,Rsv_val,
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

	{/* 1366*768*/
	"mlvds_1",LCD_MLVDS,8,
	/* basic timing */
	1366,768,1560,806,56,64,0,3,28,0,
	/* clk_attr */
	2,0,1,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* minilvds_attr */
	6,0x76543210,0xba98,0x660,0,0,0xf,0x0,Rsv_val,Rsv_val,
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

	{.panel_type = "invalid"},
};

static struct lcd_pinmux_ctrl_s lcd0_pinmux_ctrl[LCD_PINMUX_MAX] = {
	{
		.name = "lcd_vbyone_pin", //GPIOH_0/8
		.pinmux_set = {{5, 0x00000004}, {6, 0x00000004}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{5, 0x0000000f}, {6, 0x0000000f}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "lcd_minilvds_pin", //GPIOH_0~6
		.pinmux_set = {{5, 0x01111111}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{5, 0x0fffffff}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "lcd_p2p_pin", //GPIOH_0~6
		.pinmux_set = {{5, 0x01111112}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{5, 0x0fffffff}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "lcd_p2p_usit_pin", //GPIOH_0~6
		.pinmux_set = {{5, 0x01111113}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{5, 0x0fffffff}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "ST6451D06-3", //GPIOH_0~6,9~12,14
		.pinmux_set = {{5, 0x01111112}, {6, 0x01001110}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{5, 0x0fffffff}, {6, 0x0f00fff0}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "ST5461D18-2", //GPIOH_0~6,9~12,14
		.pinmux_set = {{5, 0x01111112}, {6, 0x01001110}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{5, 0x0fffffff}, {6, 0x0f00fff0}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "invalid",
	},
};

static struct lcd_pinmux_ctrl_s lcd0_bl_pinmux_ctrl[BL_PINMUX_MAX] = {
	{
		.name = "bl_pwm_on_pin", /*GPIOH_12*/
		.pinmux_set = {{6, 0x00040000}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{6, 0x000f0000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "bl_pwm_vs_on_pin", /*GPIOH_12*/
		.pinmux_set = {{6, 0x00030000}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{6, 0x000f0000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "bl_pwm_combo_0_on_pin", /*GPIOH_12*/
		.pinmux_set = {{6, 0x00040000}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{6, 0x000f0000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "bl_pwm_combo_1_on_pin", /*GPIOH_13*/
		.pinmux_set = {{6, 0x00400000}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{6, 0x00f00000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "bl_pwm_combo_0_vs_on_pin", /*GPIOH_12*/
		.pinmux_set = {{6, 0x00030000}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{6, 0x000f0000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "bl_pwm_combo_1_vs_on_pin", /*GPIOH_13*/
		.pinmux_set = {{6, 0x00300000}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{6, 0x00f00000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "invalid",
	},
};

#ifdef CONFIG_AML_LCD_BL_LDIM
static struct lcd_pinmux_ctrl_s lcd0_ldim_pinmux_ctrl[BL_PINMUX_MAX] = {
	{
		.name = "ldim_pwm_pin", /*GPIOH_12*/
		.pinmux_set = {{6, 0x00030000}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{6, 0x000f0000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "ldim_pwm_vs_pin", /*GPIOH_12*/
		.pinmux_set = {{6, 0x00030000}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{6, 0x000f0000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "analog_pwm_pin", /*GPIOH_12*/
		.pinmux_set = {{6, 0x00030000}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{6, 0x000f0000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "invalid",
		.pinmux_set = {{LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{LCD_PINMUX_END, 0x0}},
	},
};
#endif

#ifdef CONFIG_AML_LCD_EXTERN
static char lcd0_ext_gpio[LCD_EXTERN_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"invalid", /* ending flag */
};

static struct lcd_pinmux_ctrl_s lcd0_ext_pinmux_ctrl[LCD_PINMUX_NUM] = {
	{
		.name = "invalid",
	},
};

static unsigned char init_on_table[LCD_EXTERN_INIT_ON_MAX] = {
	0xc0, 2, 0x01, 0x2b,
	0xc0, 2, 0x02, 0x05,
	0xc0, 2, 0x03, 0x00,
	0xc0, 2, 0x04, 0x00,
	0xc0, 2, 0x05, 0x0c,
	0xc0, 2, 0x06, 0x04,
	0xc0, 2, 0x07, 0x21,
	0xc0, 2, 0x08, 0x0f,
	0xc0, 2, 0x09, 0x04,
	0xc0, 2, 0x0a, 0x00,
	0xc0, 2, 0x0b, 0x04,
	0xc0, 2, 0xff, 0x00,
	0xfd, 1, 100, /* delay 100ms */

	0xc1, 2, 0x01, 0xca,
	0xc1, 2, 0x02, 0x3b,
	0xc1, 2, 0x03, 0x33,
	0xc1, 2, 0x04, 0x05,
	0xc1, 2, 0x05, 0x2c,
	0xc1, 2, 0x06, 0xf2,
	0xc1, 2, 0x07, 0x9c,
	0xc1, 2, 0x08, 0x1b,
	0xc1, 2, 0x09, 0x82,
	0xc1, 2, 0x0a, 0x3d,
	0xc1, 2, 0x0b, 0x20,
	0xc1, 2, 0x0c, 0x11,
	0xc1, 2, 0x0d, 0xc4,
	0xc1, 2, 0x0e, 0x1a,
	0xc1, 2, 0x0f, 0x31,
	0xc1, 2, 0x10, 0x4c,
	0xc1, 2, 0x11, 0x12,
	0xc1, 2, 0x12, 0x90,
	0xc1, 2, 0x13, 0xf7,
	0xc1, 2, 0x14, 0x0c,
	0xc1, 2, 0x15, 0x20,
	0xc1, 2, 0x16, 0x13,
	0xff, 0, /* ending */
};

static unsigned char init_off_table[LCD_EXTERN_INIT_OFF_MAX] = {
	0xff, 0,  /* ending */
};

static struct lcd_extern_common_s ext_common_dft = {
	.key_valid = 0,
	.ext_num = 1,
	.ext_gpio = lcd0_ext_gpio,

	.i2c_bus = LCD_EXTERN_I2C_BUS_2,
	.i2c_sck_gpio = 0xff,
	.i2c_sck_gpio_off = 0,
	.i2c_sda_gpio = 0xff,
	.i2c_sda_gpio_off = 0,
	.ext_pinmux = lcd0_ext_pinmux_ctrl,
};

static struct lcd_extern_config_s ext_config_dtf[LCD_EXTERN_DEV_MAX] = {
	{
		.index = 0,
		.name = "i2c_ANX6862_7911",
		.type = LCD_EXTERN_I2C, /* LCD_EXTERN_I2C, LCD_EXTERN_SPI, LCD_EXTERN_MAX */
		.status = 1, /* 0=disable, 1=enable */
		.i2c_addr = 0x20, /* 7bit i2c address */
		.i2c_addr2 = 0x74, /* 7bit i2c address, 0xff for none */
		.i2c_addr3 = LCD_EXTERN_I2C_BUS_1, /* LCD_EXTERN_I2C_BUS_0/1/2/3/4 */
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

/***************************************************
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
#ifdef CONFIG_AML_LCD_BL_LDIM
		.ldim_pinmux = lcd0_ldim_pinmux_ctrl,
#endif
	},
};

void lcd_config_bsp_init(void)
{
	struct aml_lcd_data_s *pdata = aml_lcd_get_data();

	if (pdata)
		pdata->dft_conf[0] = &lcd_dft_conf[0];
}
