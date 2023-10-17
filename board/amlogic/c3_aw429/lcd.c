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
	"GPIOA_4", /* 12V */
	"GPIOA_3", /* reset */
	"invalid", /* ending flag */
};

static char lcd0_bl_gpio[BL_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOA_2", /* enable */
	"GPIOA_5",  /* pwm_n */
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

static struct lcd_power_step_s lcd0_power_on_step_LT8616[] = {
	{LCD_POWER_TYPE_CPU,    0, 1, 100,}, /* panel vcc */
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 0,},
	{LCD_POWER_TYPE_MAX,    0, 0, 0,},  /* ending flag */
};

static struct lcd_power_step_s lcd0_power_off_step_LT8616[] = {
	{LCD_POWER_TYPE_SIGNAL, 0, 0, 50,},
	{LCD_POWER_TYPE_CPU,    0, 0, 500,}, /* panel vcc */
	{LCD_POWER_TYPE_MAX,    0, 0, 0,},   /* ending flag */
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
	lcd0_power_on_step, lcd0_power_off_step,
	/* backlight */
	100, 255, 10, 128, 128,
	BL_CTRL_PWM, 0, 1, 0, 200, 200,
	BL_PWM_NEGATIVE, BL_PWM_E, 180, 100, 25, 1, 1,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{/* bt656*/
	"bt_0", LCD_BT656, 8,
	/* basic timing */
	1280, 720, 1650, 750, 40, 220, 1, 5, 20, 1,
	/* clk_attr */
	4, 0, 1, 74250000, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* bt_attr */
	0, 0, 0, 0, 0, 0, 0, 0, Rsv_val, Rsv_val,
	/* cmd init */
	NULL, NULL,
	/* power step */
	lcd0_power_on_step_LT8616, lcd0_power_off_step_LT8616,
	/* backlight */
	100, 255, 10, 128, 128,
	BL_CTRL_MAX, 0, 1, 0, 200, 200,
	BL_PWM_NEGATIVE, BL_PWM_E, 180, 100, 25, 1, 1,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{/* bt1120*/
	"bt_1", LCD_BT1120, 8,
	/* basic timing */
	1920, 1080, 2200, 1125, 44, 148, 1, 5, 36, 1,
	/* clk_attr */
	4, 0, 1, 148500000, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* mipi_attr */
	0, 0, 0, 0, 0, 0, 0, 0, Rsv_val, Rsv_val,
	/* cmd init */
	NULL, NULL,
	/* power step */
	lcd0_power_on_step_LT8616, lcd0_power_off_step_LT8616,
	/* backlight */
	100, 255, 10, 128, 128,
	BL_CTRL_MAX, 0, 1, 0, 200, 200,
	BL_PWM_NEGATIVE, BL_PWM_E, 180, 100, 25, 1, 1,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	10, 10, Rsv_val},

	{/* rgb*/
	"rgb_0", LCD_RGB, 8,
	/* basic timing */
	1920, 1080, 2200, 1125, 44, 148, 1, 5, 36, 1,
	/* clk_attr */
	4, 0, 1, 148500000, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* rgb_attr */
	0, 0, 1, 1, 0, 0, Rsv_val, Rsv_val, Rsv_val, Rsv_val,
	/* cmd init */
	NULL, NULL,
	/* power step */
	lcd0_power_on_step_LT8616, lcd0_power_off_step_LT8616,
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
		.name = "lcd_bt656_on_pin",
		.pinmux_set = {
			{0x0, 0x444},   //GPIOB_0~2
			{0x1, 0x44444}, //GPIOB_8~12
			{0xb, 0x4000},  //GPIOA_3
			{LCD_PINMUX_END, 0x0}
		},
		.pinmux_clr = {
			{0x0, 0xfff},
			{0x1, 0xfffff},
			{0xb, 0xf000},
			{LCD_PINMUX_END, 0x0}
		},
	},
	{
		.name = "lcd_bt1120_on_pin",
		.pinmux_set = {
			{0x0, 0x444},      //GPIOB_0~2
			{0x1, 0x44444},    //GPIOB_8~12
			{0x3, 0x44444444}, //GPIOX_0~7
			{0xb, 0x4000},     //GPIOA_3
			{LCD_PINMUX_END, 0x0}
		},
		.pinmux_clr = {
			{0x0, 0xfff},
			{0x1, 0xfffff},
			{0x3, 0xffffffff},
			{0xb, 0xf000},
			{LCD_PINMUX_END, 0x0}
		},
	},
	{
		.name = "lcd_rgb_on_pin",
		.pinmux_set = {
			{0x0, 0x444},      //GPIOB_0~2
			{0x1, 0x44444},    //GPIOB_8~12
			{0x3, 0x44444444}, //GPIOX_0~7
			{0x4, 0x444000},   //GPIOX_11~13
			{LCD_PINMUX_END, 0x0}
		},
		.pinmux_clr = {
			{0x0, 0xfff},
			{0x1, 0xfffff},
			{0x3, 0xffffffff},
			{0x4, 0xfff000},
			{LCD_PINMUX_END, 0x0}
		},
	},
	{
		.name = "lcd_rgb_de_on_pin",
		.pinmux_set = {
			{0x4, 0x400},   //GPIOX_10
			{LCD_PINMUX_END, 0x0}
		},
		.pinmux_clr = {
			{0x4, 0xf00},
			{LCD_PINMUX_END, 0x0}
		},
	},
	{
		.name = "lcd_rgb_sync_on_pin",
		.pinmux_set = {
			{0x4, 0x44},   //GPIOX_8~9
			{LCD_PINMUX_END, 0x0}
		},
		.pinmux_clr = {
			{0x4, 0xff},
			{LCD_PINMUX_END, 0x0}
		},
	},
	{
		.name = "invalid",
	},
};

static struct lcd_pinmux_ctrl_s lcd0_bl_pinmux_ctrl[BL_PINMUX_MAX] = {
	{
		.name = "bl_pwm_on_pin", /*GPIOH_12*/
		.pinmux_set = {{0x8, 0x40000}, {LCD_PINMUX_END, 0x0} },
		.pinmux_clr = {{0x8, 0xf0000}, {LCD_PINMUX_END, 0x0} },
	},
	{
		.name = "bl_pwm_vs_on_pin", //GPIOH_12
		.pinmux_set = {{0x8, 0x30000}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{0x8, 0xf0000}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "invalid",
	},
};

#ifdef CONFIG_AML_LCD_EXTERN
static char lcd0_ext_gpio[LCD_EXTERN_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOA_2",  /* int */
	"GPIOA_5",  /* reset */
	"invalid",  /* ending flag */
};

static struct lcd_pinmux_ctrl_s lcd0_ext_pinmux_ctrl[LCD_PINMUX_NUM] = {
	{
		.name = "invalid",
	},
};

static unsigned char init_on_table[LCD_EXTERN_INIT_ON_MAX] = {
	0xf0, 2, 1, 1,
	0xfd, 1, 10,
	0xf0, 2, 1, 0,
	0xfd, 1, 10,
	0xf0, 2, 1, 1,
	0xfd, 1, 10, /* delay 10ms */
	0xff, 0, /* ending */
};

static unsigned char init_off_table[LCD_EXTERN_INIT_OFF_MAX] = {
	0xf0, 2, 1, 0,
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
		.name = "invalid",
		/* LCD_EXTERN_I2C, LCD_EXTERN_SPI, LCD_EXTERN_MAX */
		.type = LCD_EXTERN_MAX,
		.status = 0, /* 0=disable, 1=enable */
		.i2c_addr = 0x72, /* 7bit i2c address */
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
	},
};

void lcd_config_bsp_init(void)
{
	struct aml_lcd_data_s *pdata = aml_lcd_get_data();

	if (pdata)
		pdata->dft_conf[0] = &lcd_dft_conf[0];
}
