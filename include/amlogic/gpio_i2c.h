/*
 * Amlogic I2C controller Diver
 *
 * Copyright (C) 2018 Amlogic Corporation
 *
 * Licensed under the GPL-2 or later.
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
};

#endif /* __GPIO_I2C_H__ */
