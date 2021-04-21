/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

struct meson8_pmx_data {
	bool is_gpio;
	unsigned int reg;
	unsigned int bit;
};

#define PMX_DATA(r, b, g)						\
	{								\
		.reg = r,						\
		.bit = b,						\
		.is_gpio = g,						\
	}

#define GROUP(grp, r, b)						\
	{								\
		.name = #grp,						\
		.pins = grp ## _pins,					\
		.num_pins = ARRAY_SIZE(grp ## _pins),			\
		.data = (const struct meson8_pmx_data[]){		\
			PMX_DATA(r, b, false),				\
		},							\
	 }

#define GPIO_GROUP(gpio)						\
	{								\
		.name = #gpio,						\
		.pins = (const unsigned int[]){ gpio },			\
		.num_pins = 1,						\
		.data = (const struct meson8_pmx_data[]){		\
			PMX_DATA(0, 0, true),				\
		},							\
	}

extern const struct pinctrl_ops meson8_pinctrl_ops;
