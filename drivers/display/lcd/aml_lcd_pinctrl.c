/*
 * drivers/display/lcd/aml_lcd_pinctrl.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the named License,
 * or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <common.h>
#include <errno.h>
#include <dm.h>
#include <asm/gpio.h>
#include <amlogic/aml_lcd.h>
#include "aml_lcd_reg.h"
#include "aml_lcd_common.h"

/* **************************************
 * pinmux
 ****************************************
 */
struct lcd_pinmux_reg_s {
	unsigned int *reg_table;
	unsigned int reg_cnt;
	unsigned int *reg_ao_table;
	unsigned int reg_ao_cnt;
};

static struct lcd_pinmux_reg_s *lcd_pinmux_reg;

static unsigned int pinmux_reg_t5w[] = {
	PERIPHS_PIN_MUX_0,
	PERIPHS_PIN_MUX_1,
	PERIPHS_PIN_MUX_2,
	PERIPHS_PIN_MUX_3,
	PERIPHS_PIN_MUX_4,
	PERIPHS_PIN_MUX_5,
	PERIPHS_PIN_MUX_6,
	PERIPHS_PIN_MUX_7,
	PERIPHS_PIN_MUX_8,
	PERIPHS_PIN_MUX_9,
	PERIPHS_PIN_MUX_A,
	PERIPHS_PIN_MUX_B,
	PERIPHS_PIN_MUX_C,
	PERIPHS_PIN_MUX_D,
	PERIPHS_PIN_MUX_E,
	PERIPHS_PIN_MUX_F,
	PERIPHS_PIN_MUX_G,
	PERIPHS_PIN_MUX_H
};

static unsigned int pinmux_ao_reg_t5w[] = {
	AO_RTI_PINMUX_REG0,
	AO_RTI_PINMUX_REG1
};

static struct lcd_pinmux_reg_s lcd_pinmux_reg_t5w = {
	.reg_table = pinmux_reg_t5w,
	.reg_cnt = sizeof(pinmux_reg_t5w) / sizeof(unsigned int),
	.reg_ao_table = pinmux_ao_reg_t5w,
	.reg_ao_cnt = sizeof(pinmux_ao_reg_t5w) / sizeof(unsigned int),
};

int lcd_pinmux_probe(unsigned int cpu_type)
{
	switch (cpu_type) {
	case LCD_CHIP_T5W:
		lcd_pinmux_reg = &lcd_pinmux_reg_t5w;
		break;
	default:
		lcd_pinmux_reg = NULL;
		break;
	}

	return 0;
}

static inline unsigned int lcd_pinmux_reg_get(unsigned int n)
{
	unsigned int index, _reg = REG_ADDR_INVALID;

	if (lcd_pinmux_reg) {
		if (n >= 0xa0) {
			index = n - 0xa0;
			if (index >= lcd_pinmux_reg->reg_ao_cnt) {
				LCDERR("%s: reg 0x%x invalid\n", __func__, n);
				return REG_ADDR_INVALID;
			}
			_reg = lcd_pinmux_reg->reg_ao_table[index];
		} else {
			index = n;
			if (index >= lcd_pinmux_reg->reg_cnt) {
				LCDERR("%s: reg %d invalid\n", __func__, n);
				return REG_ADDR_INVALID;
			}
			_reg = lcd_pinmux_reg->reg_table[index];
		}
	} else {
		if (n >= 0xa0) {
			index = n - 0xa0;
			if (index > 1) {
				LCDERR("%s: reg 0x%x invalid\n", __func__, n);
				return REG_ADDR_INVALID;
			}
			_reg = AO_RTI_PINMUX_REG0 + (n << 2);
		} else {
			index = n;
			if (index > 15) {
				LCDERR("%s: reg %d invalid\n", __func__, n);
				return REG_ADDR_INVALID;
			}
			_reg = PERIPHS_PIN_MUX_0 + (n << 2);
		}
	}

	return _reg;
}

void lcd_pinmux_set_mask(unsigned int n, unsigned int _mask)
{
	unsigned int _reg;

	_reg = lcd_pinmux_reg_get(n);
	if (_reg == REG_ADDR_INVALID)
		return;

	lcd_periphs_write(_reg, (lcd_periphs_read(_reg) | (_mask)));
}

void lcd_pinmux_clr_mask(unsigned int n, unsigned int _mask)
{
	unsigned int _reg;

	_reg = lcd_pinmux_reg_get(n);
	if (_reg == REG_ADDR_INVALID)
		return;

	lcd_periphs_write(_reg, (lcd_periphs_read(_reg) & (~(_mask))));
}

void lcd_pinmux_set(int status)
{
	struct aml_lcd_drv_s *lcd_drv = aml_lcd_get_driver();
	struct lcd_config_s *pconf;
	int i;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	pconf = lcd_drv->lcd_config;
	if (status) {
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (pconf->pinmux_clr[i][0] == LCD_PINMUX_END)
				break;
			if (lcd_debug_print_flag) {
				LCDPR("pinmux_clr: %d, 0x%08x\n",
					pconf->pinmux_clr[i][0],
					pconf->pinmux_clr[i][1]);
			}
			lcd_pinmux_clr_mask(pconf->pinmux_clr[i][0], pconf->pinmux_clr[i][1]);
			i++;
		}
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (pconf->pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			if (lcd_debug_print_flag) {
				LCDPR("pinmux_set: %d, 0x%08x\n",
					pconf->pinmux_set[i][0],
					pconf->pinmux_set[i][1]);
			}
			lcd_pinmux_set_mask(pconf->pinmux_set[i][0], pconf->pinmux_set[i][1]);
			i++;
		}
	} else {
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (pconf->pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			if (lcd_debug_print_flag) {
				LCDPR("pinmux_clr: %d, 0x%08x\n",
					pconf->pinmux_set[i][0],
					pconf->pinmux_set[i][1]);
			}
			lcd_pinmux_clr_mask(pconf->pinmux_set[i][0], pconf->pinmux_set[i][1]);
			i++;
		}
	}
}

/* **************************************
 * gpio
 ****************************************
 */
int aml_lcd_gpio_name_map_num(const char *name)
{
	int gpio;
#if defined(CONFIG_DM_GPIO)
	int ret;
#endif

#if defined(CONFIG_DM_GPIO)
	ret = gpio_lookup_name(name, NULL, NULL, (unsigned int *)&gpio);
	if (ret) {
		LCDERR("gpio: wrong name %s\n", name);
		return LCD_GPIO_MAX;
	}
#else
	/* turn the gpio name into a gpio number */
	gpio = simple_strtoul(name, NULL, 10);
	if (gpio < 0) {
		LCDERR("gpio: wrong name %s\n", name);
		return LCD_GPIO_MAX;
	}
#endif
	if (lcd_debug_print_flag)
		LCDPR("gpio: %s, %d\n", name, gpio);
	return gpio;
}

int aml_lcd_gpio_set(int gpio, int value)
{
	int ret = 0;

	if (gpio >= LCD_GPIO_MAX)
		return -1;
	if (lcd_debug_print_flag)
		LCDPR("gpio: %d, value: %d\n", gpio, value);
	/* grab the pin before we tweak it */
	ret = gpio_request(gpio, "aml_lcd_gpio");
	if (ret && ret != -EBUSY) {
		LCDERR("gpio: requesting pin %u failed\n", gpio);
		return -1;
	}

	/* finally, let's do it: set direction and exec command */
	switch (value) {
	case LCD_GPIO_OUTPUT_LOW:
	case LCD_GPIO_OUTPUT_HIGH:
		ret = gpio_direction_output(gpio, value);
		break;
	case LCD_GPIO_INPUT:
	default:
		ret = gpio_direction_input(gpio);
		break;
	}

	return 0;
}

unsigned int aml_lcd_gpio_input_get(int gpio)
{
	unsigned int value;

	if (gpio >= LCD_GPIO_MAX)
		return 0;
	gpio_direction_input(gpio);
	value = gpio_get_value(gpio);
	return value;
}

