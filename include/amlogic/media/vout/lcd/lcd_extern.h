/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _INC_AML_LCD_EXTERN_H_
#define _INC_AML_LCD_EXTERN_H_
#include <amlogic/media/vout/lcd/lcd_vout.h>

enum lcd_extern_type_e {
	LCD_EXTERN_I2C = 0,
	LCD_EXTERN_SPI,
	LCD_EXTERN_MIPI,
	LCD_EXTERN_MAX,
};

#define LCD_EXTERN_I2C_BUS_A          0
#define LCD_EXTERN_I2C_BUS_B          1
#define LCD_EXTERN_I2C_BUS_C          2
#define LCD_EXTERN_I2C_BUS_D          3
#define LCD_EXTERN_I2C_BUS_AO         4

#define LCD_EXTERN_I2C_BUS_0          LCD_EXTERN_I2C_BUS_A
#define LCD_EXTERN_I2C_BUS_1          LCD_EXTERN_I2C_BUS_B
#define LCD_EXTERN_I2C_BUS_2          LCD_EXTERN_I2C_BUS_C
#define LCD_EXTERN_I2C_BUS_3          LCD_EXTERN_I2C_BUS_D
#define LCD_EXTERN_I2C_BUS_4          LCD_EXTERN_I2C_BUS_AO
#define LCD_EXTERN_I2C_BUS_MAX        5


#define LCD_EXTERN_INIT_ON_MAX        3000
#define LCD_EXTERN_INIT_OFF_MAX       100

#define LCD_EXTERN_GPIO_NUM_MAX       6
#define LCD_EXTERN_GPIO_LEN_MAX       LCD_CPU_GPIO_NAME_MAX
#define LCD_EXTERN_INDEX_INVALID      0xff
#define LCD_EXTERN_NAME_LEN_MAX       30

#define LCD_EXTERN_DEV_MAX            10

struct lcd_extern_driver_s;

/* for bsp lcd.c default config without dts */
struct lcd_extern_common_s {
	unsigned char key_valid;
	unsigned int ext_num;
	char (*ext_gpio)[LCD_CPU_GPIO_NAME_MAX];

	unsigned char i2c_bus;
	unsigned char i2c_sck_gpio;
	unsigned char i2c_sck_gpio_off;
	unsigned char i2c_sda_gpio;
	unsigned char i2c_sda_gpio_off;
	struct lcd_pinmux_ctrl_s *ext_pinmux;
};

struct lcd_extern_config_s {
	unsigned char index;
	char name[LCD_EXTERN_NAME_LEN_MAX];
	enum lcd_extern_type_e type;
	unsigned char status;

	unsigned char i2c_addr;
	unsigned char i2c_addr2;
	unsigned char i2c_addr3;
	unsigned char i2c_addr4;

	unsigned char spi_gpio_cs;
	unsigned char spi_gpio_clk;
	unsigned char spi_gpio_data;
	unsigned char spi_clk_pol;
	unsigned short spi_clk_freq; /*KHz */
	unsigned short spi_delay_us;

	unsigned char cmd_size;
	unsigned char table_init_loaded; /* internal use */
	unsigned int table_init_on_cnt;
	unsigned int table_init_off_cnt;
	unsigned char *table_init_on;
	unsigned char *table_init_off;
};

struct lcd_extern_multi_list_s {
	unsigned int index;
	unsigned int type;
	unsigned char data_len;
	unsigned char *data_buf;
	struct lcd_extern_multi_list_s *next;
};

struct lcd_extern_dev_s {
	int dev_index;
	struct lcd_extern_config_s config;
	struct lcd_extern_multi_list_s *multi_list_header;
	int (*reg_read)(struct lcd_extern_driver_s *ext_drv,
			struct lcd_extern_dev_s *ext_dev,
			unsigned char reg, unsigned char *buf);
	int (*reg_write)(struct lcd_extern_driver_s *ext_drv,
			struct lcd_extern_dev_s *ext_dev,
			unsigned char *buf, unsigned int len);
	int (*power_on)(struct lcd_extern_driver_s *ext_drv,
			struct lcd_extern_dev_s *ext_dev);
	int (*power_off)(struct lcd_extern_driver_s *ext_drv,
			 struct lcd_extern_dev_s *ext_dev);
};

//global API
struct lcd_extern_driver_s {
	int index;
	unsigned char key_valid;
	char gpio_name[LCD_EXTERN_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX];
	unsigned int dev_cnt;
	struct lcd_extern_dev_s *dev[LCD_EXTERN_DEV_MAX];
	struct aml_lcd_data_s *data;

	unsigned char i2c_bus;
	unsigned char i2c_sck_gpio;
	unsigned char i2c_sck_gpio_off;
	unsigned char i2c_sda_gpio;
	unsigned char i2c_sda_gpio_off;
	unsigned char pinmux_valid;
	unsigned int pinmux_set[LCD_PINMUX_NUM][2];
	unsigned int pinmux_clr[LCD_PINMUX_NUM][2];

	void (*info_print)(struct lcd_extern_driver_s *ext_drv);
	/* only for debug */
	void (*power_ctrl)(struct lcd_extern_driver_s *ext_drv, int status);
};

struct lcd_extern_driver_s *lcd_extern_get_driver(int drv_index);
struct lcd_extern_dev_s *lcd_extern_get_dev(struct lcd_extern_driver_s *edrv, int dev_index);
int lcd_extern_probe(char *dtaddr, int load_id);
int lcd_extern_remove(void);
int lcd_extern_drv_index_add(int drv_index, int dev_index);
int lcd_extern_drv_index_remove(int drv_index, int dev_index);
int lcd_extern_init(void);

#endif

