/*
 *
 * Copyright (C) 2018 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#ifndef PINCONF_MESON_G12A_H
#define PINCONF_MESON_G12A_H

struct meson_pinctrl;

struct meson_drive_bank {
	const char *name;
	unsigned int first;
	unsigned int last;
	unsigned int reg;
	unsigned int bit;
};

struct meson_drive_data {
	struct meson_drive_bank *drive_banks;
	unsigned int num_drive_banks;
};

#define BANK_DRIVE(n, f, l, r, o)					\
	{								\
		.name   = n,						\
		.first	= f,						\
		.last	= l,						\
		.reg	= r,						\
		.bit	= o,						\
	}

extern int meson_pinconf_set_drive_strength(struct meson_pinctrl *priv,
		unsigned int pin, u16 arg);
#endif /*PINCONF_MESON_G12A_H*/
