/*
 * Second generation of pinmux driver for Amlogic Meson-AXG SoC.
 *
 * Copyright (c) 2017 Baylibre SAS.
 * Author:  Jerome Brunet  <jbrunet@baylibre.com>
 *
 * Copyright (c) 2017 Amlogic, Inc. All rights reserved.
 * Author: Xingyu Chen <xingyu.chen@amlogic.com>
 *
 * SPDX-License-Identifier: (GPL-2.0+ or MIT)
 */

/*
 * This new generation of pinctrl IP is mainly adopted by the
 * Meson-AXG SoC and later series, which use 4-width continuous
 * register bit to select the function for each pin.
 *
 * The value 0 is always selecting the GPIO mode, while other
 * values (start from 1) for selecting the function mode.
 */
#include <common.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <linux/io.h>

#include "pinctrl-meson.h"
#include "pinctrl-meson-axg-pmx.h"

static const struct pinconf_param meson_axg_pinconf_params[] = {
	{ "bias-disable",	PIN_CONFIG_BIAS_DISABLE,	0 },
	{ "bias-pull-up",	PIN_CONFIG_BIAS_PULL_UP,	1 },
	{ "bias-pull-down",	PIN_CONFIG_BIAS_PULL_DOWN,	1 },
	{ "input-enable",	PIN_CONFIG_INPUT_ENABLE,	1 },
	{ "output-high",	PIN_CONFIG_OUTPUT,		1 },
	{ "output-low",		PIN_CONFIG_OUTPUT,		0 },
	{ "drive-strength",	PIN_CONFIG_DRIVE_STRENGTH,	2 },
};

static int meson_axg_pmx_get_bank(struct meson_pinctrl *priv,
			unsigned int pin,
			struct meson_pmx_bank **bank)
{
	int i;
	struct meson_axg_pmx_data *pmx = priv->data->pmx_data;

	for (i = 0; i < pmx->num_pmx_banks; i++)
		if (pin >= pmx->pmx_banks[i].first &&
				pin <= pmx->pmx_banks[i].last) {
			*bank = &pmx->pmx_banks[i];
			return 0;
		}

	return -EINVAL;
}

static int meson_pmx_calc_reg_and_offset(struct meson_pmx_bank *bank,
			unsigned int pin, unsigned int *reg,
			unsigned int *offset)
{
	int shift;

	shift = pin - bank->first;

	*reg = bank->reg + (bank->offset + (shift << 2)) / 32;
	*offset = (bank->offset + (shift << 2)) % 32;

	return 0;
}

static int meson_axg_pmx_update_function(struct meson_pinctrl *priv,
			unsigned int pin, unsigned int func)
{
	int ret;
	unsigned int reg;
	unsigned int offset;
	struct meson_pmx_bank *bank;
	void __iomem *addr;

	ret = meson_axg_pmx_get_bank(priv, pin, &bank);
	if (ret)
		return ret;

	meson_pmx_calc_reg_and_offset(bank, pin, &reg, &offset);

	addr = priv->reg_mux + reg * 4;
	writel((readl(addr) & ~(0xf << offset)) | ((func & 0xf) << offset),
		addr);

	return ret;
}

static int meson_axg_pmx_set_mux(struct udevice *dev,
			unsigned int group_selector, unsigned int func_selector)
{
	int i;
	int ret;
	struct meson_pinctrl *priv = dev_get_priv(dev);
	struct meson_pmx_func *func = &priv->data->funcs[func_selector];
	struct meson_pmx_group *group = &priv->data->groups[group_selector];
	struct meson_pmx_axg_data *pmx_data =
		(struct meson_pmx_axg_data *)group->data;

	debug("pinmux: set group %s, func %s\n", group->name, func->name);

	for (i = 0; i < group->num_pins; i++) {
		ret = meson_axg_pmx_update_function(priv, group->pins[i],
			pmx_data->func);
		if (ret)
			return ret;
	}

	return 0;
}

static int meson_axg_pmx_request_gpio(struct udevice *dev, int offset)
{
	struct meson_pinctrl *priv = dev_get_priv(dev);

	return meson_axg_pmx_update_function(priv, offset, 0);
}

const struct pinctrl_ops meson_axg_pinctrl_ops = {
	.get_groups_count = meson_pinctrl_get_groups_count,
	.get_group_name = meson_pinctrl_get_group_name,
	.get_functions_count = meson_pinmux_get_functions_count,
	.get_function_name = meson_pinmux_get_function_name,
	.pinmux_group_set = meson_axg_pmx_set_mux,
	.set_state = pinctrl_generic_set_state,
	.set_gpio_mux = meson_axg_pmx_request_gpio,
	.pinconf_num_params = ARRAY_SIZE(meson_axg_pinconf_params),
	.pinconf_params	= meson_axg_pinconf_params,
	.pinconf_group_set = meson_pinconf_group_set,
};
