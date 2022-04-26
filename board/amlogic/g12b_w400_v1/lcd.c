// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#ifdef CONFIG_AML_LCD_EXTERN
//#include "lcd_extern.h"
#endif

static char lcd_cpu_gpio[LCD_CPU_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOZ_9", /* panel rst */
	"GPIOZ_8", /* panel power */
	"invalid", /* ending flag */
};

static struct lcd_power_step_s lcd_power_on_step[] = {
	{LCD_POWER_TYPE_CPU,   1,0,100,}, /* lcd power */
	{LCD_POWER_TYPE_CPU,   0,0,10,}, /* lcd_reset */
	{LCD_POWER_TYPE_CPU,   0,1,20,}, /* lcd_reset */
	{LCD_POWER_TYPE_SIGNAL,0,0,0,},  /* signal */
	{LCD_POWER_TYPE_MAX,   0,0,0,},  /* ending flag */
};
static struct lcd_power_step_s lcd_power_off_step[] = {
	{LCD_POWER_TYPE_SIGNAL,0,0,50,},  /* signal */
	{LCD_POWER_TYPE_CPU,   0,0,10,}, /* lcd_reset */
	{LCD_POWER_TYPE_CPU,   1,1,100,}, /* power off */
	{LCD_POWER_TYPE_MAX,   0,0,0,},   /* ending flag */
};

static struct lcd_power_step_s lcd_power_on_step_P070ACB[] = {
	{LCD_POWER_TYPE_CPU,   1,0,200,}, /* lcd power */
	{LCD_POWER_TYPE_SIGNAL,0,0,0,},  /* signal */
	{LCD_POWER_TYPE_MAX,   0,0,0,},  /* ending flag */
};
static struct lcd_power_step_s lcd_power_off_step_P070ACB[] = {
	{LCD_POWER_TYPE_SIGNAL,0,0,0,},  /* signal */
	{LCD_POWER_TYPE_CPU,   0,0,20,}, /* lcd_reset */
	{LCD_POWER_TYPE_CPU,   1,1,100,}, /* power off */
	{LCD_POWER_TYPE_MAX,   0,0,0,},   /* ending flag */
};

static struct lcd_power_step_s lcd_power_on_step_TL050FHV02CT[] = {
	{LCD_POWER_TYPE_CPU,   1,0,200,}, /* lcd power */
	{LCD_POWER_TYPE_CPU,   0,1,20,}, /* lcd reset: 1 */
	{LCD_POWER_TYPE_CPU,   0,0,10,}, /* lcd reset: 0 */
	{LCD_POWER_TYPE_CPU,   0,1,20,}, /* lcd reset: 1 */
	{LCD_POWER_TYPE_SIGNAL,0,0,0,},  /* signal */
	{LCD_POWER_TYPE_MAX,   0,0,0,},  /* ending flag */
};
static struct lcd_power_step_s lcd_power_off_step_TL050FHV02CT[] = {
	{LCD_POWER_TYPE_SIGNAL,0,0,0,},  /* signal */
	{LCD_POWER_TYPE_CPU,   0,0,20,}, /* lcd_reset */
	{LCD_POWER_TYPE_CPU,   1,1,100,}, /* power off */
	{LCD_POWER_TYPE_MAX,   0,0,0,},   /* ending flag */
};

static struct lcd_power_step_s lcd_power_on_step_TL070WSH27[] = {
	{LCD_POWER_TYPE_CPU,   1,0,100,}, /* lcd power */
	{LCD_POWER_TYPE_CPU,   0,0,10,}, /* lcd reset: 0 */
	{LCD_POWER_TYPE_CPU,   0,1,20,}, /* lcd reset: 1 */
	{LCD_POWER_TYPE_SIGNAL,0,0,0,},  /* signal */
	{LCD_POWER_TYPE_MAX,   0,0,0,},  /* ending flag */
};
static struct lcd_power_step_s lcd_power_off_step_TL070WSH27[] = {
	{LCD_POWER_TYPE_SIGNAL,0,0,10,},  /* signal */
	{LCD_POWER_TYPE_CPU,   0,0,10,}, /* lcd_reset */
	{LCD_POWER_TYPE_CPU,   1,1,100,}, /* power off */
	{LCD_POWER_TYPE_MAX,   0,0,0,},   /* ending flag */
};

static unsigned char mipi_init_on_table[DSI_INIT_ON_MAX] = {//table size < 100
	0x05, 1, 0x11,
	0xfd, 1, 20,
	0x05, 1, 0x29,
	0xfd, 1, 20,
	0xff, 0,   //ending
};
static unsigned char mipi_init_off_table[DSI_INIT_OFF_MAX] = {//table size < 50
	0x05, 1, 0x28,
	0xfd, 1, 10,
	0x05, 1, 0x10,
	0xfd, 1, 10,
	0xff, 0,   //ending
};

static unsigned char mipi_init_on_table_P070ACB[DSI_INIT_ON_MAX] = {//table size < 100
	0xfd, 1, 10,
	0xf0, 3, 0, 1, 30, /* reset high, delay 30ms */
	0xf0, 3, 0, 0, 10, /* reset low, delay 10ms */
	0xf0, 3, 0, 1, 30, /* reset high, delay 30ms */
	0xfc, 2, 0x04, 3,  /* check_reg, check_cnt */
	0xff, 0,   //ending
};
static unsigned char mipi_init_off_table_P070ACB[DSI_INIT_OFF_MAX] = {//table size < 50
	0xff, 0,   //ending
};

static unsigned char mipi_init_on_table_TL050FHV02CT[DSI_INIT_ON_MAX] = {//table size < 100
	0xff, 0,   //ending
};
static unsigned char mipi_init_off_table_TL050FHV02CT[DSI_INIT_OFF_MAX] = {//table size < 50
	0xff, 0,   //ending
};

static unsigned char mipi_init_on_table_TL070WSH27[DSI_INIT_ON_MAX] = {//table size < 100
	0x05, 1, 0x11,
	0xfd, 1, 100,
	0x05, 1, 0x29,
	0xfd, 1, 20,
	0xff, 0,   //ending
};
static unsigned char mipi_init_off_table_TL070WSH27[DSI_INIT_OFF_MAX] = {//table size < 50
	0x05, 1, 0x28,
	0xfd, 1, 100,
	0x05, 1, 0x10,
	0xfd, 1, 10,
	0xff, 0,   //ending
};

static char lcd_bl_gpio[BL_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"GPIOH_4", /* BL_EN */
	"GPIOH_5", /* BL_PWM */
	"invalid", /* ending flag */
};

struct ext_lcd_config_s ext_lcd_config[LCD_NUM_MAX] = {
	{/* B080XAN01*/
	"lcd_0",LCD_MIPI,8,
	/* basic timing */
	768,1024,948,1140,64,56,0,50,30,0,
	/* clk_attr */
	0,0,1,64843200,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* mipi_attr */
	4,550,0,1,0,2,1,0,Rsv_val,Rsv_val,
	/* cmd init */
	mipi_init_on_table, mipi_init_off_table,
	/* power step */
	lcd_power_on_step, lcd_power_off_step,
	/* backlight */
	100,255,10,128,128,
	BL_CTRL_PWM,0,1,0,200,200,
	BL_PWM_NEGATIVE,BL_PWM_F,180,100,25,1,1,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{/* P070ACB_FT*/
	"lcd_1",LCD_MIPI,8,
	/* basic timing */
	600,1024,770,1070,10,80,0,6,20,0,
	/* clk_attr */
	0,0,1,49434000,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* mipi_attr */
	4,400,0,1,0,2,0,0,Rsv_val,1,
	/* cmd init */
	mipi_init_on_table_P070ACB, mipi_init_off_table_P070ACB,
	/* power step */
	lcd_power_on_step_P070ACB, lcd_power_off_step_P070ACB,
	/* backlight */
	100,255,10,128,128,
	BL_CTRL_PWM,0,1,0,200,200,
	BL_PWM_NEGATIVE,BL_PWM_F,180,100,25,1,1,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{/* TL050FHV02CT*/
	"lcd_2",LCD_MIPI,8,
	/* basic timing */
	1080,1920,1125,2100,5,30,0,44,108,0,
	/* clk_attr */
	0,0,1,118125000,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* mipi_attr */
	4,960,0,1,0,2,1,0,Rsv_val,2,
	/* cmd init */
	mipi_init_on_table_TL050FHV02CT, mipi_init_off_table_TL050FHV02CT,
	/* power step */
	lcd_power_on_step_TL050FHV02CT, lcd_power_off_step_TL050FHV02CT,
	/* backlight */
	100,255,10,128,128,
	BL_CTRL_PWM,0,1,0,200,200,
	BL_PWM_NEGATIVE,BL_PWM_F,180,100,25,1,1,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{/* TL070WSH27*/
	"lcd_3",LCD_MIPI,8,
	/* basic timing */
	1024,600,1250,630,80,100,0,5,20,0,
	/* clk_attr */
	0,0,1,47250000,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	/* mipi_attr */
	4,300,0,1,0,2,1,0,Rsv_val,Rsv_val,
	/* cmd init */
	mipi_init_on_table_TL070WSH27, mipi_init_off_table_TL070WSH27,
	/* power step */
	lcd_power_on_step_TL070WSH27, lcd_power_off_step_TL070WSH27,
	/* backlight */
	100,255,10,128,128,
	BL_CTRL_PWM,0,1,0,200,200,
	BL_PWM_NEGATIVE,BL_PWM_F,180,100,25,1,1,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	Rsv_val,Rsv_val,Rsv_val,Rsv_val,
	10,10,Rsv_val},

	{.panel_type = "invalid"},
};

static struct lcd_pinmux_ctrl_s lcd_pinmux_ctrl[LCD_PINMX_MAX] = {
	{
		.name = "lcd_pin",
		.pinmux_set = {{LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "invalid",
	},
};

static struct lcd_pinmux_ctrl_s lcd_bl_pinmux_ctrl[BL_PINMUX_MAX] = {
	{
		.name = "bl_pwm_on_pin", //GPIOH_5
		.pinmux_set = {{11, 0x00400000}, {LCD_PINMUX_END, 0x0}},
		.pinmux_clr = {{11, 0x00f00000}, {LCD_PINMUX_END, 0x0}},
	},
	{
		.name = "invalid",
	},
};

#ifdef CONFIG_AML_LCD_EXTERN
static char lcd_ext_gpio[LCD_EXTERN_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX] = {
	"invalid", /* ending flag */
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
	.lcd_ext_key_valid = 0,
	.lcd_ext_num = 1,
	.pinmux_set = {{LCD_PINMUX_END, 0x0} },
	.pinmux_clr = {{LCD_PINMUX_END, 0x0} },
};

struct lcd_extern_config_s ext_config_dtf[LCD_EXTERN_NUM_MAX] = {
	{
		.index = 0,
		.name = "invalid",
		/* LCD_EXTERN_I2C, LCD_EXTERN_SPI, LCD_EXTERN_MAX */
		.type = LCD_EXTERN_MAX,
		.status = 0, /* 0=disable, 1=enable */
		.i2c_addr = 0x20, /* 7bit i2c address */
		.i2c_addr2 = 0x74, /* 7bit i2c address, 0xff for none */
		/* LCD_EXTERN_I2C_BUS_0/1/2/3/4 */
		.i2c_bus = LCD_EXTERN_I2C_BUS_1,
		.cmd_size = 0xff,
		.table_init_on = init_on_table,
		.table_init_off = init_off_table,
	},
	{
		.index = LCD_EXTERN_INDEX_INVALID,
	},
};
#endif

#ifdef CONFIG_AML_BL_EXTERN
static unsigned char bl_ext_init_on[BL_EXTERN_INIT_ON_MAX] = {
	0xff, 0,   //ending flag
};

static unsigned char bl_ext_init_off[BL_EXTERN_INIT_OFF_MAX] = {
	0xff, 0,   //ending flag
};

struct bl_extern_config_s bl_extern_config_dtf = {
	.index = BL_EXTERN_INDEX_INVALID,
	.name = "none",
	.type = BL_EXTERN_MAX,
	.i2c_addr = 0xff,
	.i2c_bus = BL_EXTERN_I2C_BUS_MAX,
	.dim_min = 10,
	.dim_max = 255,

	.init_loaded = 0,
	.cmd_size = 0xff,
	.init_on = bl_ext_init_on,
	.init_off = bl_ext_init_off,
	.init_on_cnt = sizeof(bl_ext_init_on),
	.init_off_cnt = sizeof(bl_ext_init_off),
};
#endif

/***************************************************
 * lcd default config
 ***************************************************/
static struct lcd_dft_config_s lcd_dft_conf = {
	.lcd_gpio = lcd_cpu_gpio,
	.key_valid = 0,
	.clk_path = 0,
	.mode = LCD_MODE_TABLET,
	.ext_lcd = ext_lcd_config,
	.lcd_pinmux = lcd_pinmux_ctrl,

#ifdef CONFIG_AML_LCD_EXTERN
	.ext_gpio = lcd_ext_gpio[0],
	.ext_common = &ext_common_dft,
	.ext_conf = ext_config_dtf,
#endif
	.bl_gpio = lcd_bl_gpio,
	.bl_pinmux = lcd_bl_pinmux_ctrl,
};

void lcd_config_bsp_init(void)
{
	struct aml_lcd_data_s *pdata = aml_lcd_get_data();

	if (pdata)
		pdata->dft_conf[0] = &lcd_dft_conf;
}

