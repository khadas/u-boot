// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

/* For this first generation of pinctrl driver every pinmux group can be
 * enabled by a specific bit in the first register range. When all groups for
 * a given pin are disabled the pin acts as a GPIO.
 */
#include <common.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <linux/io.h>

#include "pinctrl-meson.h"
#include "pinctrl-meson8-pmx.h"

static const struct pinconf_param meson8_pinconf_params[] = {
	{ "bias-disable",	PIN_CONFIG_BIAS_DISABLE,	0 },
	{ "bias-pull-up",	PIN_CONFIG_BIAS_PULL_UP,	1 },
	{ "bias-pull-down",	PIN_CONFIG_BIAS_PULL_DOWN,	1 },
	{ "input-enable",	PIN_CONFIG_INPUT_ENABLE,	1 },
	{ "output-high",	PIN_CONFIG_OUTPUT,		1 },
	{ "output-low",		PIN_CONFIG_OUTPUT,		0 },
};

/**
 * meson8_pmx_disable_other_groups() - disable other groups using a given pin
 *
 * @priv:	meson pin controller device
 * @pin:	number of the pin
 * @sel_group:	index of the selected group, or -1 if none
 *
 * The function disables all pinmux groups using a pin except the
 * selected one. If @sel_group is -1 all groups are disabled, leaving
 * the pin in GPIO mode.
 */
static void meson8_pmx_disable_other_groups(struct meson_pinctrl *priv,
					      unsigned int pin, int sel_group)
{
	struct meson_pmx_group *group;
	struct meson8_pmx_data *pmx_data;
	void __iomem *addr;
	int i, j;

	for (i = 0; i < priv->data->num_groups; i++) {
		group = &priv->data->groups[i];
		pmx_data = (struct meson8_pmx_data *)group->data;
		if (pmx_data->is_gpio || i == sel_group)
			continue;

		for (j = 0; j < group->num_pins; j++) {
			if (group->pins[j] == pin) {
				/* We have found a group using the pin */
				debug("pinmux: disabling %s\n", group->name);
				addr = priv->reg_mux + pmx_data->reg * 4;
				writel(readl(addr) & ~BIT(pmx_data->bit), addr);
			}
		}
	}
}

static int meson8_pmx_group_set(struct udevice *dev,
				  unsigned group_selector,
				  unsigned func_selector)
{
	struct meson_pinctrl *priv = dev_get_priv(dev);
	const struct meson_pmx_group *group;
	const struct meson_pmx_func *func;
	struct meson8_pmx_data *pmx_data;
	void __iomem *addr;
	int i;

	group = &priv->data->groups[group_selector];
	func = &priv->data->funcs[func_selector];
	pmx_data = (struct meson8_pmx_data *)group->data;

	debug("pinmux: set group %s, func %s\n", group->name, func->name);

	/*
	 * Disable groups using the same pins.
	 * The selected group is not disabled to avoid glitches.
	 */
	for (i = 0; i < group->num_pins; i++) {
		meson8_pmx_disable_other_groups(priv,
						  group->pins[i],
						  group_selector);
	}

	/* Function 0 (GPIO) doesn't need any additional setting */
	if (func_selector) {
		addr = priv->reg_mux + pmx_data->reg * 4;
		writel(readl(addr) | BIT(pmx_data->bit), addr);
	}

	return 0;
}

static int meson8_pmx_request_gpio(struct udevice *dev, int offset)
{
	struct meson_pinctrl *priv = dev_get_priv(dev);

	meson8_pmx_disable_other_groups(priv, offset, -1);

	return 0;
}

const struct pinctrl_ops meson8_pinctrl_ops = {
	.get_groups_count = meson_pinctrl_get_groups_count,
	.get_group_name = meson_pinctrl_get_group_name,
	.get_functions_count = meson_pinmux_get_functions_count,
	.get_function_name = meson_pinmux_get_function_name,
	.pinmux_group_set = meson8_pmx_group_set,
	.set_state = pinctrl_generic_set_state,
	.set_gpio_mux = meson8_pmx_request_gpio,
	.pinconf_num_params = ARRAY_SIZE(meson8_pinconf_params),
	.pinconf_params = meson8_pinconf_params,
	.pinconf_group_set = meson_pinconf_group_set,
};
