// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <errno.h>
#include <dm.h>
#include <asm/gpio.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "lcd_reg.h"
#include "lcd_common.h"

int lcd_gpio_name_map_num(const char *name)
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
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("gpio: %s, %d\n", name, gpio);
	return gpio;
}

int lcd_gpio_set(int gpio, int value)
{
	int ret = 0;

	if (gpio >= LCD_GPIO_MAX)
		return -1;
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
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

unsigned int lcd_gpio_input_get(int gpio)
{
	unsigned int value;

	if (gpio >= LCD_GPIO_MAX)
		return 0;
	gpio_direction_input(gpio);
	value = gpio_get_value(gpio);
	return value;
}

