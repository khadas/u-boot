// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/gpio.h>
#include <fdtdec.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include <amlogic/media/vout/lcd/bl_ldim.h>
#include "../../lcd_common.h"
#include "ldim_drv.h"
#include "ldim_dev_drv.h"

static int global_on_flag;

static int global_hw_init_on(struct ldim_dev_driver_s *dev_drv)
{
	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	ldim_set_duty_pwm(&dev_drv->ldim_pwm_config);
	ldim_set_duty_pwm(&dev_drv->analog_pwm_config);

	dev_drv->pinmux_ctrl(dev_drv, 1);
	mdelay(2);
	ldim_gpio_set(dev_drv, dev_drv->en_gpio, dev_drv->en_gpio_on);
	mdelay(20);

	return 0;
}

static int global_hw_init_off(struct ldim_dev_driver_s *dev_drv)
{
	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	dev_drv->pinmux_ctrl(dev_drv, 0);
	ldim_gpio_set(dev_drv, dev_drv->en_gpio, dev_drv->en_gpio_off);

	return 0;
}

static unsigned int global_get_value(unsigned int dim_max, unsigned int dim_min, unsigned int level)
{
	unsigned int val;

	val = dim_min + ((level * (dim_max - dim_min)) / LD_DATA_MAX);

	return val;
}

static int global_smr(struct aml_ldim_driver_s *ldim_drv, unsigned short *buf, unsigned char len)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;
	unsigned short val;

	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	val = global_get_value(dev_drv->dim_max, dev_drv->dim_min, buf[0]);
	dev_drv->ldim_pwm_config.pwm_duty = val;

	if (global_on_flag == 0) {
		if (lcd_debug_print_flag)
			LDIMPR("%s: on_flag=%d\n", __func__, global_on_flag);
		return 0;
	}

	if (len != 1) {
		LDIMERR("%s: data len %d invalid\n", __func__, len);
		return -1;
	}

	ldim_set_duty_pwm(&dev_drv->ldim_pwm_config);

	return 0;
}

static void global_dim_range_update(struct ldim_dev_driver_s *dev_drv)
{
	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return;
	}

	dev_drv->dim_max = dev_drv->ldim_pwm_config.pwm_duty_max;
	dev_drv->dim_min = dev_drv->ldim_pwm_config.pwm_duty_min;
}

static int global_power_on(struct aml_ldim_driver_s *ldim_drv)
{
	global_hw_init_on(ldim_drv->dev_drv);
	global_on_flag = 1;

	LDIMPR("%s: ok\n", __func__);
	return 0;
}

static int global_power_off(struct aml_ldim_driver_s *ldim_drv)
{
	global_on_flag = 0;
	global_hw_init_off(ldim_drv->dev_drv);

	LDIMPR("%s: ok\n", __func__);
	return 0;
}

static int global_ldim_driver_update(struct ldim_dev_driver_s *dev_drv)
{
	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	dev_drv->ldim_pwm_config.pwm_duty_max = dev_drv->dim_max;
	dev_drv->ldim_pwm_config.pwm_duty_min = dev_drv->dim_min;
	dev_drv->dim_range_update = global_dim_range_update;

	dev_drv->power_on = global_power_on;
	dev_drv->power_off = global_power_off;
	dev_drv->dev_smr = global_smr;

	return 0;
}

int ldim_dev_global_probe(struct aml_ldim_driver_s *ldim_drv)
{
	global_on_flag = 0;
	global_ldim_driver_update(ldim_drv->dev_drv);

	return 0;
}

int ldim_dev_global_remove(struct aml_ldim_driver_s *ldim_drv)
{
	return 0;
}

