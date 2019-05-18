/*
 * Driver for keys on TCA6408 I2C IO expander
 *
 * Copyright (C) 2019 Texas Instruments
 *
 * Author : waylon <waylon@khadas.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <i2c.h>
#include <khadas_tca6408.h>

#define CHIP_ADDR			   0x20

#define TCA6408_INPUT          0
#define TCA6408_OUTPUT         1
#define TCA6408_INVERT         2
#define TCA6408_DIRECTION      3

//#define TCA6408_DEBUG
#ifdef TCA6408_DEBUG
    #define debug_info(msg...) printf(msg);
#else
    #define debug_info(msg...)
#endif

struct tca6408_gpio_chip {
	uint8_t addr;
	uint8_t reg_output;
	uint8_t reg_direction;
};
static struct tca6408_gpio_chip chip;

static int tca6408_write_reg(u8 reg, u8 val)
{
	int error;

	error = i2c_write(chip.addr, reg, 1, &val, 1);
	if (error < 0) {
		printf("%s failed, reg: %d, val: %d, error: %d\n",
			__func__, reg, val, error);
		return error;
	}

	return 0;
}

static int tca6408_read_reg(u8 reg, u8 *val)
{
	int retval;

	retval = i2c_read(chip.addr, reg, 1, val, 1);
	if (retval < 0) {
		printf("%s failed, reg: %d, error: %d\n",
			__func__, reg, retval);
		return retval;
	}

	return 0;
}

int tca6408_output_set_value(u8 value, u8 mask)
{
	int error;
	u8 reg;

	debug_info("%s, value = 0x%x, mask = 0x%x\n", __func__ ,value, mask);

	// set direction output
	error = tca6408_read_reg(TCA6408_DIRECTION, &reg);
	if (error)
		return error;

	reg &= ~(mask);

	error = tca6408_write_reg(TCA6408_DIRECTION, reg);
	if (error)
		return error;

	// read value
	error = tca6408_read_reg(TCA6408_OUTPUT, &chip.reg_output);
	if (error)
		return error;

	chip.reg_output &= ~(mask);
	chip.reg_output |= (value & mask);

	error = tca6408_write_reg(TCA6408_OUTPUT, chip.reg_output);
	if (error)
		return error;

	return 0;
}

int tca6408_output_get_value(u8 *value)
{
	int error;

	debug_info("%s\n", __func__);

	error = tca6408_read_reg(TCA6408_OUTPUT, value);
	if (error)
		return error;

	debug_info("value = 0x%x\n", *value);

	return 0;
}

// read GPIO
int tca6408_get_value(u8 *value, u8 mask)
{
	int error;
	u8 reg;

	// set direction input
	error = tca6408_read_reg(TCA6408_DIRECTION, &reg);
	if (error)
		return error;

	reg |= mask;

	error = tca6408_write_reg(TCA6408_DIRECTION, reg);
	if (error)
		return error;

	// read value
	error = tca6408_read_reg(TCA6408_INPUT, &reg);
	if (error)
		return error;

	*value = (reg & mask) ? 1 : 0;

	return 0;
}

static int tca6408_setup_registers(struct tca6408_gpio_chip *chip)
{
	int error;

	debug_info("%s\n", __func__);

	/* ensure that keypad pins are set to output */
	error = tca6408_write_reg(TCA6408_DIRECTION, 0x00);
	if (error)
		return error;
	error = tca6408_write_reg(TCA6408_OUTPUT, 0x00);
	if (error)
		return error;

	error = tca6408_read_reg(TCA6408_OUTPUT, &chip->reg_output);
	if (error)
		return error;

	error = tca6408_read_reg(TCA6408_DIRECTION, &chip->reg_direction);
	if (error)
		return error;

	debug_info("%s:direction=0x%x, output=0x%x\n", __func__, chip->reg_direction, chip->reg_output);

	return 0;
}

int tca6408_gpio_init(void)
{
	int error;

	debug_info("%s\n", __func__);

	memset(&chip, 0, sizeof(chip));

	chip.addr = CHIP_ADDR;

	error = tca6408_setup_registers(&chip);
	if (error)
		return error;

	debug_info("%s init success.\n", __func__);

	return 0;
}
