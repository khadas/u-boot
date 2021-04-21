/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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
