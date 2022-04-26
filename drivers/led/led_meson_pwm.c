// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <led.h>
#include <pwm.h>
#include <dm/lists.h>
#include <div64.h>

struct led_pwm_priv {
	struct udevice *pwm;
};

struct led_pwm_child_priv {
	struct udevice *pwm;
	uint max_brightness;
	uint default_brightness;
	uint brightness;
	uint channel;
	uint period_ns;
};

static int pwm_led_set_state(struct udevice *dev, enum led_state_t state)
{
	return 0;
}

static enum led_state_t pwm_led_get_state(struct udevice *dev)
{
	return 0;
}

static int pwm_led_set_brightness(struct udevice *dev, int brightness)
{
	struct led_pwm_child_priv *priv = dev_get_parent_platdata(dev);
	unsigned long long duty =  priv->period_ns;
	int ret;

	duty *= brightness;
	do_div(duty, priv->max_brightness);
	ret = pwm_set_config(priv->pwm, priv->channel, priv->period_ns, duty);
	if (ret)
		return ret;

	ret = pwm_set_enable(priv->pwm, priv->channel, 1);
	if (ret)
		return ret;

	priv->brightness = brightness;

	return 0;
}

static int pwm_led_get_brightness(struct udevice *dev)
{
	struct led_pwm_child_priv *priv = dev_get_parent_platdata(dev);

	return priv->brightness;
}

static int led_pwm_probe(struct udevice *dev)
{
	return 0;
}

static int led_pwm_remove(struct udevice *dev)
{
	return 0;
}

static int led_pwm_bind(struct udevice *parent)
{
	struct udevice *dev;
	struct led_pwm_child_priv *child_priv;
	struct ofnode_phandle_args args;
	ofnode node;
	int ret;

	dev_for_each_subnode(node, parent) {
		struct led_uc_plat *uc_plat;
		const char *label;

		label = ofnode_read_string(node, "label");
		if (!label) {
			debug("%s: node %s has no label\n", __func__,
			      ofnode_get_name(node));
			return -EINVAL;
		}
		ret = device_bind_driver_to_node(parent, "pwm_led",
						 ofnode_get_name(node),
						 node, &dev);
		if (ret)
			return ret;

		child_priv = dev_get_parent_platdata(dev);
		ret = ofnode_read_u32(node, "max-brightness", &child_priv->max_brightness);
		if (ret)
			return ret;

		ret = ofnode_read_u32(node, "default-brightness", &child_priv->default_brightness);
		if (ret)
			return ret;

		ret = dev_read_phandle_with_args(dev, "pwms", "#pwm-cells", 0, 0,&args);
		if (ret) {
			debug("%s: Cannot get PWM phandle: ret=%d\n", __func__, ret);
			return ret;
		}

		child_priv->channel = args.args[0];
		child_priv->period_ns = args.args[1];
		ret = uclass_get_device_by_ofnode(UCLASS_PWM, args.node, &child_priv->pwm);
		if (ret) {
			debug("%s: Cannot get PWM: ret=%d\n", __func__, ret);
			return ret;
		}

		uc_plat = dev_get_uclass_platdata(dev);
		uc_plat->label = label;
		pwm_led_set_brightness(dev,child_priv->default_brightness);
	}
	return 0;
}

static const struct led_ops pwm_led_ops = {
	.set_state	= pwm_led_set_state,
	.get_state	= pwm_led_get_state,
	.set_brightness = pwm_led_set_brightness,
	.get_brightness = pwm_led_get_brightness,
};

static const struct udevice_id led_pwm_ids[] = {
	{ .compatible = "pwm-leds" },
	{ }
};

U_BOOT_DRIVER(led_pwm) = {
	.name	= "pwm_led",
	.id	= UCLASS_LED,
	.of_match = led_pwm_ids,
	.ops	= &pwm_led_ops,
	.priv_auto_alloc_size = sizeof(struct led_pwm_priv),
	.per_child_platdata_auto_alloc_size = sizeof(struct led_pwm_child_priv),
	.bind	= led_pwm_bind,
	.probe	= led_pwm_probe,
	.remove	= led_pwm_remove,
};
