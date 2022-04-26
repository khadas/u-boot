// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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

	if (arg < 1 || arg > 4) {
		debug("pin %u: invalid drive-strength [1-4]: %d\n", pin, arg);
		return -EINVAL;
	}

	arg = arg - 1;

	meson_drive_calc_reg_and_bit(drive_bank, pin, &reg, &bit);

	clrsetbits_le32(priv->reg_drive + reg, 0x3 << bit, (arg & 0x3) << bit);

	return 0;
}
