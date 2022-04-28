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
	"GPIOY_0", /* 12V */
	"GPIOY_4", /* rst */
	"invalid", /* ending flag */
};

static char lcd0_bl_gpio[BL_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOY_12", /* enable */
	"GPIOY_1",  /* pwm_e */
	"invalid", /* ending flag */
};

static struct lcd_power_step_s lcd0_power_on_step[] = {
	{LCD_POWER_TYPE_CPU,    0, 1, 50,}, /* panel vcc */
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},  /* ending flag */
};

static struct lcd_power_step_s lcd0_power_off_step[] = {
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_CPU,   0, 0, 200,}, /* panel vcc */
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},   /* ending flag */
};

static struct lcd_power_step_s lcd0_power_on_step_mipi[] = {
	{LCD_POWER_TYPE_CPU,    1, 0, 0,}, /* rst */
	{LCD_POWER_TYPE_CPU,    0, 0, 500,}, /* panel vcc */
	{LCD_POWER_TYPE_CPU,    0, 1, 100,}, /* panel vcc */
	{LCD_POWER_TYPE_CPU,    1, 1, 20,}, /* rst */
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},  /* ending flag */
};

static struct lcd_power_step_s lcd0_power_off_step_mipi[] = {
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_CPU,    1, 0, 0,}, /* rst */
	{LCD_POWER_TYPE_CPU,   0, 0, 200,}, /* panel vcc */
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},   /* ending flag */
};

static unsigned char mipi_init_on_table_dft[DSI_INIT_ON_MAX] = {//table size < 100
	0x05, 1, 0x11,
	0xfd, 1, 100,
	0x05, 1, 0x29,
	0xfd, 1, 20,
	0xff, 0,   //ending
};

static unsigned char mipi_init_off_table_dft[DSI_INIT_OFF_MAX] = {//table size < 50
	0x05, 1, 0x28,
	0xfd, 1, 100,
	0x05, 1, 0x10,
	0xfd, 1, 10,
	0xff, 0,   //ending
};

struct ext_lcd_config_s ext_lcd0_config[LCD_NUM_MAX] = {
	{/* normal*/
	"lvds_0", LCD_LVDS, 8,
	/* basic timing */
	1366, 768, 1569, 806, 56, 64, 0, 3, 28, 0,
	/* clk_attr */
	2, 0, 1, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* lvds_attr */
	1, 0, 0, 0, 0, 0x3, 0x0, Rsv_val, Rsv_val, Rsv_val,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	60, 255, 10, 128, 128,
	BL_CTRL_PWM, 0, 1, 0, 200, 200,
	BL_PWM_POSITIVE, BL_PWM_E, 180, 100, 25, 1, 0,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{/*public 2-region vx1 : 3840x2160@60hz 8lane */
	"vbyone_0", LCD_VBYONE, 10,
	/* basic timing */
	3840, 2160, 4400, 2250, 33, 477, 0, 6, 81, 0,
	/* clk_attr */
	2, 0, 1, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* vbyone_attr */
	8, 2, 4, 4, 0x7, 0x1, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	60, 255, 10, 128, 128,
	BL_CTRL_PWM, 0, 1, 0, 200, 200,
	BL_PWM_POSITIVE, BL_PWM_E, 180, 100, 25, 1, 0,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{/*public 1-region vx1 : 3840x2160@60hz 8lane */
	"vbyone_1", LCD_VBYONE, 10,
	/* basic timing */
	3840, 2160, 4400, 2250, 33, 477, 0, 6, 81, 0,
	/* clk_attr */
	2, 0, 1, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* vbyone_attr */
	8, 1, 4, 4, 0x7, 0x1, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	60, 255, 10, 128, 128,
	BL_CTRL_PWM, 0, 1, 0, 200, 200,
	BL_PWM_POSITIVE, BL_PWM_E, 180, 100, 25, 1, 0,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{"edp_0", LCD_EDP, 8,
	/* basic timing */
	1920, 1080, 2200, 1120, 44, 148, 0, 5, 30, 0,
	/* clk_attr */
	1, 0, 1, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* edp_attr */
	2, 1, 0, 0, 0, 0, 0, 0, 0x5, 0x1,
	NULL, NULL,
	/* power step */
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	60, 255, 10, 128, 128,
	BL_CTRL_PWM, 0, 1, 0, 200, 200,
	BL_PWM_POSITIVE, BL_PWM_E, 180, 100, 25, 1, 0,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{/* TL070WSH27*/
	"mipi_0", LCD_MIPI, 8,
	/* basic timing */
	1024, 600, 1250, 630, 80, 100, 0, 5, 20, 0,
	/* clk_attr */
	0, 0, 1, 47250000, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* mipi_attr */
	4, 300, 0, 1, 0, 2, 1, 0, Rsv_val, Rsv_val,
	/* cmd init */
	mipi_init_on_table_dft, mipi_init_off_table_dft,
	/* power step */
	lcd0_power_on_step_mipi, lcd0_power_off_step_mipi,
	/* backlight */
	100, 255, 10, 128, 128,
	BL_CTRL_PWM, 0, 1, 0, 200, 200,
	BL_PWM_NEGATIVE, BL_PWM_E, 180, 100, 25, 1, 1,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{.panel_type = "invalid"},
};

static struct lcd_pinmux_ctrl_s lcd0_pinmux_ctrl[LCD_PINMUX_MAX] = {
	{
		.name = "lcd_vbyone_pin", //GPIOY_10/12
		.pinmux_set = {{0x14, 0x00030300}, {LCD_PINMUX_END, 0x0} },

		.pinmux_clr = {{0x14, 0x000f0f00}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "lcd_edp_pin", //GPIOY_10
		.pinmux_set = {{0x14, 0x00000400}, {LCD_PINMUX_END, 0x0} },

		.pinmux_clr = {{0x14, 0x00000f00}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "invalid",
	},
};

static struct lcd_pinmux_ctrl_s lcd0_bl_pinmux_ctrl[BL_PINMUX_MAX] = {
	{
		.name = "bl_pwm_on_pin", /*GPIOY_1*/
		.pinmux_set = {{0x13, 0x00000030}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{0x13, 0x000000f0}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "invalid",
	},
};

#ifdef CONFIG_AML_LCD_BL_LDIM
static struct lcd_pinmux_ctrl_s lcd0_ldim_pinmux_ctrl[BL_PINMUX_MAX] = {
	{
		.name = "ldim_pwm_pin", /*GPIOY_1*/
		.pinmux_set = {{0x13, 0x00000030}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{0x13, 0x000000f0}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "ldim_pwm_vs_pin", //GPIOY_14
		.pinmux_set = {{0x14, 0x03000000}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{0x14, 0x0f000000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "invalid",
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
	0xff, 0, /* ending */
};

static unsigned char init_off_table[LCD_EXTERN_INIT_OFF_MAX] = {
	0xff, 0,  /* ending */
};

struct lcd_extern_common_s ext_common_dft = {
	.key_valid = 0,
	.ext_num = 1,
	.ext_gpio = lcd0_ext_gpio,
	.i2c_bus = LCD_EXTERN_I2C_BUS_1,
	.ext_pinmux = lcd0_ext_pinmux_ctrl,
};

struct lcd_extern_config_s ext_config_dtf[LCD_EXTERN_DEV_MAX] = {
	{
		.index = 0,
		.name = "invalid",
		/* LCD_EXTERN_I2C, LCD_EXTERN_SPI, LCD_EXTERN_MAX */
		.type = LCD_EXTERN_MAX,
		.status = 0, /* 0=disable, 1=enable */
		.i2c_addr = 0x20, /* 7bit i2c address */
		.i2c_addr2 = 0x74, /* 7bit i2c address, 0xff for none */
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
 * lcd_1
 ***************************************************/
static char lcd1_cpu_gpio[LCD_CPU_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOY_14", /* 12V */
	"GPIOY_6", /* rst */
	"invalid", /* ending flag */
};

static char lcd1_bl_gpio[BL_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOY_13", /* enable */
	"GPIOY_8", /* pwm_f */
	"invalid", /* ending flag */
};

static struct lcd_power_step_s lcd1_power_on_step[] = {
	{LCD_POWER_TYPE_CPU,   0, 1, 50,}, /* panel vcc */
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},  /* ending flag */
};

static struct lcd_power_step_s lcd1_power_off_step[] = {
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_CPU,   0, 0, 200,}, /* panel vcc */
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},   /* ending flag */
};

static struct lcd_power_step_s lcd1_power_on_step_mipi[] = {
	{LCD_POWER_TYPE_CPU,    1, 0, 0,}, /* rst */
	{LCD_POWER_TYPE_CPU,    0, 0, 500,}, /* panel vcc */
	{LCD_POWER_TYPE_CPU,    0, 1, 100,}, /* panel vcc */
	{LCD_POWER_TYPE_CPU,    1, 1, 20,}, /* rst */
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},  /* ending flag */
};

static struct lcd_power_step_s lcd1_power_off_step_mipi[] = {
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_CPU,    1, 0, 0,}, /* rst */
	{LCD_POWER_TYPE_CPU,   0, 0, 200,}, /* panel vcc */
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},   /* ending flag */
};

struct ext_lcd_config_s ext_lcd1_config[LCD_NUM_MAX] = {
	{/* normal*/
	"lvds_0", LCD_LVDS, 8,
	/* basic timing */
	1366, 768, 1569, 806, 56, 64, 0, 3, 28, 0,
	/* clk_attr */
	2, 0, 1, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* lvds_attr */
	1, 0, 0, 0, 0, 0x3, 0x0, Rsv_val, Rsv_val, Rsv_val,
	NULL, NULL,
	/* power step */
	lcd1_power_on_step, lcd1_power_off_step,
	/* backlight */
	60, 255, 10, 128, 128,
	BL_CTRL_PWM, 0, 1, 0, 200, 200,
	BL_PWM_POSITIVE, BL_PWM_E, 180, 100, 25, 1, 0,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{/*public 2-region vx1 : 3840x2160@60hz 8lane */
	"vbyone_0", LCD_VBYONE, 10,
	/* basic timing */
	3840, 2160, 4400, 2250, 33, 477, 0, 6, 81, 0,
	/* clk_attr */
	2, 0, 1, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* vbyone_attr */
	8, 2, 4, 4, 0x7, 0x1, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	NULL, NULL,
	/* power step */
	lcd1_power_on_step, lcd1_power_off_step,
	/* backlight */
	60, 255, 10, 128, 128,
	BL_CTRL_PWM, 0, 1, 0, 200, 200,
	BL_PWM_POSITIVE, BL_PWM_F, 180, 100, 25, 1, 0,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{/*public 1-region vx1 : 3840x2160@60hz 8lane */
	"vbyone_1", LCD_VBYONE, 10,
	/* basic timing */
	3840, 2160, 4400, 2250, 33, 477, 0, 6, 81, 0,
	/* clk_attr */
	2, 0, 1, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* vbyone_attr */
	8, 1, 4, 4, 0x7, 0x1, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	NULL, NULL,
	/* power step */
	lcd1_power_on_step, lcd1_power_off_step,
	/* backlight */
	60, 255, 10, 128, 128,
	BL_CTRL_PWM, 0, 1, 0, 200, 200,
	BL_PWM_POSITIVE, BL_PWM_F, 180, 100, 25, 1, 0,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{"edp_0", LCD_EDP, 8,
	/* basic timing */
	1920, 1080, 2200, 1120, 44, 148, 0, 5, 30, 0,
	/* clk_attr */
	1, 0, 1, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* edp_attr */
	2, 1, 0, 0, 0, 0, 0, 0, 0x5, 0x1,
	NULL, NULL,
	/* power step */
	lcd1_power_on_step, lcd1_power_off_step,
	/* backlight */
	60, 255, 10, 128, 128,
	BL_CTRL_PWM, 0, 1, 0, 200, 200,
	BL_PWM_POSITIVE, BL_PWM_F, 180, 100, 25, 1, 0,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{/* TL070WSH27*/
	"mipi_0", LCD_MIPI, 8,
	/* basic timing */
	1024, 600, 1250, 630, 80, 100, 0, 5, 20, 0,
	/* clk_attr */
	0, 0, 1, 47250000, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* mipi_attr */
	4, 300, 0, 1, 0, 2, 1, 0, Rsv_val, Rsv_val,
	/* cmd init */
	mipi_init_on_table_dft, mipi_init_off_table_dft,
	/* power step */
	lcd1_power_on_step_mipi, lcd1_power_off_step_mipi,
	/* backlight */
	100, 255, 10, 128, 128,
	BL_CTRL_PWM, 0, 1, 0, 200, 200,
	BL_PWM_NEGATIVE, BL_PWM_F, 180, 100, 25, 1, 1,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{.panel_type = "invalid"},
};

static struct lcd_pinmux_ctrl_s lcd1_pinmux_ctrl[LCD_PINMX_MAX] = {
	{
		.name = "lcd_vbyone_pin", //GPIOY_11/13
		.pinmux_set = {{0x14, 0x00303000}, {LCD_PINMUX_END, 0x0} },

		.pinmux_clr = {{0x14, 0x00f0f000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "lcd_edp_pin", //GPIOY_11
		.pinmux_set = {{0x14, 0x00004000}, {LCD_PINMUX_END, 0x0} },

		.pinmux_clr = {{0x14, 0x0000f000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "invalid",
	},
};

static struct lcd_pinmux_ctrl_s lcd1_bl_pinmux_ctrl[BL_PINMUX_MAX] = {
	{
		.name = "bl_pwm_on_pin", /*GPIOY_8*/
		.pinmux_set = {{0x14, 0x00000003}, {LCD_PINMUX_END, 0x0} },

		.pinmux_clr = {{0x14, 0x0000000f}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "invalid",
	},
};

/***************************************************
 * lcd_2
 ***************************************************/
static char lcd2_cpu_gpio[LCD_CPU_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOY_14",
	"invalid", /* ending flag */
};

static char lcd2_bl_gpio[BL_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"invalid", /* ending flag */
};

static struct lcd_power_step_s lcd2_power_on_step[] = {
	{LCD_POWER_TYPE_CPU,   0, 1, 20,}, /* panel vcc */
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},  /* ending flag */
};

static struct lcd_power_step_s lcd2_power_off_step[] = {
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_CPU,   0, 0, 200,}, /* panel vcc */
	{LCD_POWER_TYPE_MAX,   0, 0, 0,},   /* ending flag */
};

struct ext_lcd_config_s ext_lcd2_config[LCD_NUM_MAX] = { //only support lvds
	{/* normal*/
	"lvds_0", LCD_LVDS, 8,
	/* basic timing */
	1366, 768, 1569, 806, 56, 64, 0, 3, 28, 0,
	/* clk_attr */
	2, 0, 1, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* lvds_attr */
	1, 0, 0, 0, 0, 0x3, 0x0, Rsv_val, Rsv_val, Rsv_val,
	NULL, NULL,
	/* power step */
	lcd2_power_on_step, lcd2_power_off_step,
	/* backlight */
	60, 255, 10, 128, 128,
	BL_CTRL_MAX, 0, 1, 0, 200, 200,
	BL_PWM_POSITIVE, BL_PWM_E, 180, 100, 25, 1, 0,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{/* for HDMI convert*/
	"lvds_1", LCD_LVDS, 8,
	/* basic timing */
	1920, 1080, 2200, 1125, 44, 148, 0, 5, 36, 0,
	/* clk_attr */
	4, 0, 1, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* lvds_attr */
	1, 1, 0, 0, 0, 0x3, 0x0, Rsv_val, Rsv_val, Rsv_val,
	NULL, NULL,
	/* power step */
	lcd2_power_on_step, lcd2_power_off_step,
	/* backlight */
	60, 255, 10, 128, 128,
	BL_CTRL_MAX, 0, 1, 0, 200, 200,
	BL_PWM_POSITIVE, BL_PWM_MAX, 180, 100, 25, 1, 0,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{.panel_type = "invalid"},
};

static struct lcd_pinmux_ctrl_s lcd2_pinmux_ctrl[LCD_PINMX_MAX] = {
	{
		.name = "invalid",
	},
};

static struct lcd_pinmux_ctrl_s lcd2_bl_pinmux_ctrl[BL_PINMUX_MAX] = {
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
	{//index 1
		.lcd_gpio = lcd1_cpu_gpio,
		.key_valid = 0,
		.clk_path = 0,
		.mode = LCD_MODE_TABLET,
		.ext_lcd = ext_lcd1_config,
		.lcd_pinmux = lcd1_pinmux_ctrl,

#ifdef CONFIG_AML_LCD_EXTERN
		.ext_common = &ext_common_dft,
		.ext_conf = ext_config_dtf,
#endif
		.bl_gpio = lcd1_bl_gpio,
		.bl_pinmux = lcd1_bl_pinmux_ctrl,
	},
	{//index 2
		.lcd_gpio = lcd2_cpu_gpio,
		.key_valid = 0,
		.clk_path = 0,
		.mode = LCD_MODE_TABLET,
		.ext_lcd = ext_lcd2_config,
		.lcd_pinmux = lcd2_pinmux_ctrl,

#ifdef CONFIG_AML_LCD_EXTERN
		.ext_common = &ext_common_dft,
		.ext_conf = ext_config_dtf,
#endif
		.bl_gpio = lcd2_bl_gpio,
		.bl_pinmux = lcd2_bl_pinmux_ctrl,
	}
};

void lcd_config_bsp_init(void)
{
	struct aml_lcd_data_s *pdata = aml_lcd_get_data();

	if (pdata) {
		pdata->dft_conf[0] = &lcd_dft_conf[0];
		pdata->dft_conf[1] = &lcd_dft_conf[1];
		pdata->dft_conf[2] = &lcd_dft_conf[2];
	}
}
