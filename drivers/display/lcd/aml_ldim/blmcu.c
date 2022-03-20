// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <asm/arch/gpio.h>
#ifdef CONFIG_OF_LIBFDT
#include <libfdt.h>
#endif
#include <amlogic/aml_lcd.h>
#include <amlogic/aml_ldim.h>
#include "../aml_lcd_reg.h"
#include "../aml_lcd_common.h"
#include "ldim_drv.h"
#include "ldim_dev_drv.h"

static int blmcu_on_flag;

struct blmcu {
	int cs_hold_delay;
	int cs_clk_delay;
	struct spi_slave *spi;
};

struct blmcu *bl_mcu;

static int blmcu_hw_init_on(void)
{
	struct aml_ldim_driver_s *ldim_drv = aml_ldim_get_driver();

	LDIMPR("%s\n", __func__);

	/* step 1: system power_on */
	ldim_set_gpio(ldim_drv->ldev_conf->en_gpio, ldim_drv->ldev_conf->en_gpio_on);

	/* step 2: delay for internal logic stable */
	mdelay(10);

	/* step 3: Generate external VSYNC to VSYNC/PWM pin */
	ldim_set_duty_pwm(&ldim_drv->ldev_conf->ldim_pwm_config);
	ldim_set_duty_pwm(&ldim_drv->ldev_conf->analog_pwm_config);
	ldim_drv->pinmux_ctrl(1);

	return 0;
}

static int blmcu_hw_init_off(void)
{
	struct aml_ldim_driver_s *ldim_drv = aml_ldim_get_driver();

	ldim_drv->pinmux_ctrl(0);
	ldim_set_gpio(ldim_drv->ldev_conf->en_gpio, ldim_drv->ldev_conf->en_gpio_off);

	return 0;
}

static int blmcu_smr(unsigned short *buf, unsigned char len)
{
	if (blmcu_on_flag == 0) {
		LDIMPR("%s: on_flag=%d\n", __func__, blmcu_on_flag);
		return 0;
	}

	//ret = ldim_spi_write(dev_drv->spi_info.spi, bl_mcu->tbuf, bl_mcu->tbuf_size);
	LDIMPR("%s: unnecessary send duty to mcu in uboot\n", __func__);

	return 0;
}

static int blmcu_power_on(void)
{
	blmcu_hw_init_on();
	blmcu_on_flag = 1;

	LDIMPR("%s: ok\n", __func__);
	return 0;
}

static int blmcu_power_off(void)
{
	blmcu_on_flag = 0;
	blmcu_hw_init_off();

	LDIMPR("%s: ok\n", __func__);
	return 0;
}

static int blmcu_ldim_driver_update(struct aml_ldim_driver_s *ldim_drv)
{
	ldim_drv->device_power_on = blmcu_power_on;
	ldim_drv->device_power_off = blmcu_power_off;
	ldim_drv->device_bri_update = blmcu_smr;
	return 0;
}

int ldim_dev_blmcu_probe(struct aml_ldim_driver_s *ldim_drv)
{
	if (!ldim_drv->spi_info->spi) {
		LDIMERR("%s: spi is null\n", __func__);
		return -1;
	}

	bl_mcu = (struct blmcu *)malloc(sizeof(struct blmcu));
	if (!bl_mcu) {
		LDIMERR("blmcu malloc error\n");
		return -1;
	}
	memset(bl_mcu, 0, sizeof(struct blmcu));

	blmcu_on_flag = 0;

	bl_mcu->spi = ldim_drv->spi_info->spi;
	bl_mcu->cs_hold_delay = ldim_drv->ldev_conf->cs_hold_delay;
	bl_mcu->cs_clk_delay = ldim_drv->ldev_conf->cs_clk_delay;

	blmcu_ldim_driver_update(ldim_drv);

	printf("%s: ok\n", __func__);

	return 0;
}

int ldim_dev_blmcu_remove(struct aml_ldim_driver_s *ldim_drv)
{
	if (bl_mcu) {
		free(bl_mcu);
		bl_mcu = NULL;
	}
	return 0;
}

