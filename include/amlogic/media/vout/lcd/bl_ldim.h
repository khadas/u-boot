/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef INC_AML_BL_LDIM_H
#define INC_AML_BL_LDIM_H

#include <common.h>
#include <linux/list.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include <spi.h>

#define LD_DATA_MIN      10
#define LDIM_DATA_MAX    0xfff
#define LDIM_DATA_MIN    0x7f

struct aml_ldim_driver_s;

enum ldim_dev_type_e {
	LDIM_DEV_TYPE_NORMAL = 0,
	LDIM_DEV_TYPE_SPI,
	LDIM_DEV_TYPE_I2C,
	LDIM_DEV_TYPE_MAX,
};

struct ldim_config_s {
	unsigned char row;
	unsigned char col;
	unsigned char dev_index;
};

#define LDIM_SPI_NAME_MAX    30
struct ldim_spi_dev_info_s {
	char modalias[20];
	char spi_name[LDIM_SPI_NAME_MAX];
	int mode;
	int max_speed_hz;
	int bus_num;
	int chip_select;
	int wordlen;

	struct spi_slave *spi;
};

#define LDIM_DEV_NAME_MAX    30
#define LDIM_INIT_ON_MAX     1000
#define LDIM_INIT_OFF_MAX    24
struct ldim_dev_driver_s {
	unsigned char index;
	char name[LDIM_DEV_NAME_MAX];
	char pinmux_name[LDIM_DEV_NAME_MAX];
	char gpio_name[BL_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX];
	unsigned char key_valid;
	unsigned char type;
	unsigned char probe_flag;

	int cs_hold_delay;
	int cs_clk_delay;
	int en_gpio;
	int en_gpio_on;
	int en_gpio_off;
	int lamp_err_gpio;
	unsigned char fault_check;
	unsigned char write_check;
	unsigned char pinmux_flag;
	unsigned char chip_cnt;

	unsigned int bl_row;
	unsigned int bl_col;
	unsigned int dim_min;
	unsigned int dim_max;
	unsigned int zone_num;
	unsigned short *bl_mapping;
	unsigned int mcu_header;
	unsigned int mcu_dim;

	unsigned char init_loaded;
	unsigned char cmd_size;
	unsigned char *init_on;
	unsigned char *init_off;
	unsigned int init_on_cnt;
	unsigned int init_off_cnt;

	struct bl_pwm_config_s ldim_pwm_config;
	struct bl_pwm_config_s analog_pwm_config;
	struct ldim_spi_dev_info_s spi_info;

	void (*dim_range_update)(struct ldim_dev_driver_s *dev_drv);
	int (*pinmux_ctrl)(struct ldim_dev_driver_s *dev_drv, int status);
	int (*reg_write)(struct ldim_dev_driver_s *dev_drv, unsigned char chip_id,
			 unsigned char reg, unsigned char *buf, unsigned int len);
	int (*reg_read)(struct ldim_dev_driver_s *dev_drv, unsigned char chip_id,
			unsigned char reg, unsigned char *buf, unsigned int len);
	void (*config_print)(struct ldim_dev_driver_s *dev_drv);

	int (*power_on)(struct aml_ldim_driver_s *ldim_drv);
	int (*power_off)(struct aml_ldim_driver_s *ldim_drv);
	int (*dev_smr)(struct aml_ldim_driver_s *ldim_drv,
		       unsigned short *buf, unsigned char len);
};

/*******global API******/
struct aml_ldim_driver_s {
	int valid_flag;
	int ldim_on_flag;

	unsigned int data_min;
	unsigned int data_max;
	unsigned int brightness_level;
	unsigned int litgain;

	struct aml_lcd_data_s *data;
	struct ldim_config_s config;
	struct ldim_dev_driver_s *dev_drv;
	unsigned short *bl_matrix;

	int (*power_on)(struct aml_ldim_driver_s *ldim_drv);
	int (*power_off)(struct aml_ldim_driver_s *ldim_drv);
	int (*set_level)(struct aml_ldim_driver_s *ldim_drv, unsigned int level);
	void (*config_print)(struct aml_ldim_driver_s *ldim_drv);
};

struct aml_ldim_driver_s *aml_ldim_get_driver(void);

/* flag: 0=dts, 1=bsp, 2=unifykey */
int aml_ldim_probe(char *dt_addr, int child_offset, unsigned char *key_buf, int flag);

#endif /* INC_AML_BL_LDIM_H */
