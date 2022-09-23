// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/io.h>
#include <dm.h>
#include <errno.h>
#include <led.h>
#include <asm/gpio.h>
#include <dm/lists.h>
#include <malloc.h>
#include <errno.h>

#define DEFAULT_T0H	(200)
#define DEFAULT_T0L	(800)
#define DEFAULT_T1H	(800)
#define DEFAULT_T1L	(200)
#define DEFAULT_TRES	(8000)
/* 4 led by defalt */
#define DEFAULT_LED_COUNTS	(4)

struct unipolar_led_timing {
	/*timing count in ns */
	unsigned int t0l;
	unsigned int t0h;
	unsigned int t1l;
	unsigned int t1h;
	unsigned int tres;
};

struct led_unipolar_gpio_priv {
	struct gpio_desc gpio;
	/* led counts*/
	unsigned int counts;
	/* timming setting */
	struct unipolar_led_timing timing;
	/* data frame with counts x 3(r,g,b) */
	unsigned char *frame;
	const unsigned char *defcolor;
	unsigned int effect;
};

static void unipolar_rz_0(struct led_unipolar_gpio_priv *priv)
{
	struct gpio_desc *gpio = &priv->gpio;
	/* TOH min 0.2us */
	dm_gpio_set_value(gpio, 1);
	/* TOL min 0.8us  */
	dm_gpio_set_value(gpio, 0);
	dm_gpio_set_value(gpio, 0);
}

static void unipolar_rz_1(struct led_unipolar_gpio_priv *priv)
{
	struct gpio_desc *gpio = &priv->gpio;
	/* T1H min 0.6us */
	dm_gpio_set_value(gpio, 1);
	dm_gpio_set_value(gpio, 1);
	/* T1L min 0.2us */
	dm_gpio_set_value(gpio, 0);
}

static void unipolar_rz_res(struct led_unipolar_gpio_priv *priv)
{
	struct gpio_desc *gpio = &priv->gpio;
	struct unipolar_led_timing *timing = &priv->timing;
	/* Trst, min 80us */
	dm_gpio_set_value(gpio, 0);
	udelay(timing->tres / 1000);
}

static void byte_to_unipolar_rz(struct led_unipolar_gpio_priv *priv, unsigned char d)
{
	int i = 0;

	for (i = 0; i < 8; ++i)	{
		if ((d >> i) & 0x1)
			unipolar_rz_1(priv);
		else
			unipolar_rz_0(priv);
	}
}

static void set_led_rgb(struct led_unipolar_gpio_priv *priv,
	unsigned char r, unsigned char g, unsigned char b)
{
	byte_to_unipolar_rz(priv, r);
	byte_to_unipolar_rz(priv, g);
	byte_to_unipolar_rz(priv, b);
}

static int unipolar_gpio_leds_onoff(struct led_unipolar_gpio_priv *priv, enum led_state_t state)
{
	int ret = 0, rgb;
	unsigned int i, counts = priv->counts;

	rgb = (state == LEDST_OFF) ? 0 : 0xff;
	for (i = 0; i < counts; ++i)
		set_led_rgb(priv, rgb, rgb, rgb);

	unipolar_rz_res(priv);
	priv->effect = state;
	return ret;
}

static int unipolar_gpio_leds_effect(struct led_unipolar_gpio_priv *priv, unsigned int effect)
{
	int ret = 0;
	unsigned int i, counts = priv->counts;
	unsigned char *rgb = (unsigned char *)priv->defcolor;

	/* send frame rest */
	unipolar_rz_res(priv);
	if (effect < 14) {
		for (i = 0; i < effect; ++i)
			set_led_rgb(priv, 255, 0, 255);
	} else {
		for (i = 0; i < counts; i++)
			set_led_rgb(priv, rgb[i * 3 + (effect % 3 + 0) % 3],
					rgb[i * 3 + (effect % 3 + 1) % 3],
					rgb[i * 3 + (effect % 3 + 2) % 3]);
	}
	unipolar_rz_res(priv);
	priv->effect = effect;
	return ret;
}

static int unipolar_gpio_led_set_state(struct udevice *dev, enum led_state_t state)
{
	struct led_unipolar_gpio_priv *priv = dev_get_priv(dev);
	int ret;

	if (!dm_gpio_is_valid(&priv->gpio))
		return -EREMOTEIO;
	switch (state) {
	case LEDST_OFF:
	case LEDST_ON:
		ret = unipolar_gpio_leds_onoff(priv, state);
		break;
	case LEDST_TOGGLE:
		printf("%s: effect %d\n", __func__, priv->effect);
		ret = unipolar_gpio_leds_effect(priv, ++priv->effect);
		break;
	/*
	 * To satisfy the coding style check
	 * WARNING: ENOSYS means 'invalid syscall nr' and nothing else
	 * using error number directly.
	 */
	default:
		ret = -38;
	}

	return ret;
}

static enum led_state_t unipolar_gpio_led_get_state(struct udevice *dev)
{
	struct led_unipolar_gpio_priv *priv = dev_get_priv(dev);
	int ret;

	ret = priv->effect;
	return ret ? LEDST_ON : LEDST_OFF;
}

static int led_unipolar_gpio_probe(struct udevice *dev)
{
	struct led_uc_plat *uc_plat = dev_get_uclass_platdata(dev);
	struct led_unipolar_gpio_priv *priv = dev_get_priv(dev);
	struct unipolar_led_timing *timing = &priv->timing;
	int ret = 0;
	size_t sz;

	priv->frame = NULL;
	priv->effect = 0;
	/* Ignore the top-level LED node */
	if (!uc_plat->label)
		goto _out;

	/* gpio */
	ret = gpio_request_by_name(dev, "gpios", 0, &priv->gpio, GPIOD_IS_OUT);
	if (ret)
		goto _out;

	/* timing */
	timing->t0h = dev_read_u32_default(dev, "t0h", DEFAULT_T0H);
	timing->t0l = dev_read_u32_default(dev, "t0l", DEFAULT_T0L);
	timing->t1h = dev_read_u32_default(dev, "t1h", DEFAULT_T1H);
	timing->t1l = dev_read_u32_default(dev, "t1l", DEFAULT_T1L);
	timing->tres = dev_read_u32_default(dev, "tres", DEFAULT_TRES);

	/* led and frame */
	priv->counts = dev_read_u32_default(dev, "counts", DEFAULT_LED_COUNTS);
	/* rgb */
	sz = priv->counts * 3;
	priv->frame = malloc(sz);
	if (!priv->frame) {
		ret = -ENOSPC;
		goto _out;
	}
	/* default all white */
	memset(priv->frame, 0xff, sz);

	priv->defcolor = dev_read_u8_array_ptr(dev, "color-defaults", sz);
	if (!priv->defcolor) {
		ret = -ENODATA;
		goto _out;
	}

_out:
	if (priv->frame)
		free(priv->frame);

	return ret;
}

static int led_unipolar_gpio_remove(struct udevice *dev)
{
	/*
	 * The GPIO driver may have already been removed. We will need to
	 * address this more generally.
	 */
#ifndef CONFIG_SANDBOX
	struct led_unipolar_gpio_priv *priv = dev_get_priv(dev);

	if (dm_gpio_is_valid(&priv->gpio))
		dm_gpio_free(dev, &priv->gpio);
#endif
	if (priv->frame)
		free(priv->frame);

	return 0;
}

static int led_unipolar_gpio_bind(struct udevice *parent)
{
	struct udevice *dev;
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
		ret = device_bind_driver_to_node(parent, "unipolar_gpio_led",
						 ofnode_get_name(node),
						 node, &dev);
		if (ret)
			return ret;
		uc_plat = dev_get_uclass_platdata(dev);
		uc_plat->label = label;
	}

	return 0;
}

static const struct led_ops unipolar_gpio_led_ops = {
	.set_state	= unipolar_gpio_led_set_state,
	.get_state	= unipolar_gpio_led_get_state,
};

static const struct udevice_id led_unipolar_gpio_ids[] = {
	{ .compatible = "unipolar-gpio-leds" },
	{ }
};

U_BOOT_DRIVER(unipolar_led_gpio) = {
	.name	= "unipolar_gpio_led",
	.id	= UCLASS_LED,
	.of_match = led_unipolar_gpio_ids,
	.ops	= &unipolar_gpio_led_ops,
	.priv_auto_alloc_size = sizeof(struct led_unipolar_gpio_priv),
	.bind	= led_unipolar_gpio_bind,
	.probe	= led_unipolar_gpio_probe,
	.remove	= led_unipolar_gpio_remove,
};
