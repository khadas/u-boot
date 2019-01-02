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

#include <linux/io.h>
#include "pinctrl-meson.h"
#include "pinconf-meson-g12a.h"

static int meson_get_drive_bank(struct meson_pinctrl *priv, unsigned int pin,
				struct meson_drive_bank **bank)
{
	int i;
	struct meson_drive_data *drive = priv->data->drv_data;

	for (i = 0; i < drive->num_drive_banks; i++) {
		if (pin >= drive->drive_banks[i].first &&
			pin <= drive->drive_banks[i].last) {
			*bank = &drive->drive_banks[i];
			return 0;
		}
	}

	return -EINVAL;
}

static void meson_drive_calc_reg_and_bit(struct meson_drive_bank *drive_bank,
				unsigned int pin, unsigned int *reg,
				unsigned int *bit)
{
	int shift = pin - drive_bank->first;

	*reg = (drive_bank->reg + (drive_bank->bit + (shift << 1)) / 32) * 4;
	*bit = (drive_bank->bit + (shift << 1)) % 32;
}

int meson_pinconf_set_drive_strength(struct meson_pinctrl *priv,
				unsigned int pin, u16 arg)
{
	struct meson_drive_bank *drive_bank;
	unsigned int reg, bit;
	int ret;

	debug("pin %u: set drive-strength\n", pin);

	ret = meson_get_drive_bank(priv, pin, &drive_bank);
	if (ret)
		return ret;

	if (arg >= 4) {
		debug("pin %u: invalid drive-strength [0-3]: %d\n", pin, arg);
		return -EINVAL;
	}
	meson_drive_calc_reg_and_bit(drive_bank, pin, &reg, &bit);

	clrsetbits_le32(priv->reg_drive + reg, 0x3 << bit, (arg & 0x3) << bit);

	return 0;
}
