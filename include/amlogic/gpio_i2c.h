/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * include/amlogic/gpio_i2c.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __GPIO_I2C_H__
#define __GPIO_I2C_H__

/*
 * @i2c_index: Controller Index.
 * @reg: Controller registers address.
 * @clock_rate: clock source rate.
 * @div_factor: divider factor.
 * @i2c_index: Filter delay count ,if not, i2c frequecy
 * is not correct.
 */

struct meson_gpio_i2c_platdata {
	const char sda[10];
	const char scl[10];
	unsigned int clock_rate;
	char is_odpin;
};

#endif /* __GPIO_I2C_H__ */
