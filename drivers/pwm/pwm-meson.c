/*
 * Pwm controller driver for Amlogic Meson AXG SOC
 *
 * Copyright (c) 2018 Amlogic, Inc. All rights reserved.
 * Auther: Bichao Zheng <bichao.zheng@amlogic.com>
 *
 * SPDX-License-Identifier: (GPL-2.0+ or MIT)
 */

#include <common.h>
#include <dm.h>
#include <pwm.h>
#include <regmap.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/sizes.h>
#include <linux/printk.h>
#include <div64.h>

#include "pwm-meson.h"

struct meson_pwm_priv{
	struct meson_pwm_reg *regs;
	struct meson_pwm_state *pwm_state;
};

static u64 meson_pwm_clock_get_rate(void)
{
	return 24000000;
}

static int pwm_meson_get_polarity(struct udevice *dev, uint channel)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
#if 0
	struct meson_pwm_reg *regs = priv->regs;

	unsigned int tmp, val;

	switch (channel) {
		case MESON_PWM0:
		case MESON_PWM2:
			val = 0x1 << 26;
			break;

		case MESON_PWM1:
		case MESON_PWM3:
			val = 0x1 << 27;
			break;

		default:
			pr_err("Id is invalid\n");
			return 0;
	}

	tmp = readl(&regs->miscr);
	tmp = tmp & val;
	if (tmp == 0)
		return 0;
	else
		return 1;
#else
	struct meson_pwm_state *pwm_state = priv->pwm_state;

	return pwm_state[channel].polarity;
#endif
}

static void pwm_constant_enable(struct udevice *dev, uint channel)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_reg *regs = priv->regs;

	switch (channel) {
		case MESON_PWM0:
		case MESON_PWM2:
			setbits_le32(&regs->miscr, 1 << 28);
			break;

		case MESON_PWM1:
		case MESON_PWM3:
			setbits_le32(&regs->miscr, 1 << 29);
			break;

		default:
			pr_err("Id is invalid\n");
			break;
		}
}

static void pwm_constant_disable(struct udevice *dev, uint channel)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_reg *regs = priv->regs;

	switch (channel) {
		case MESON_PWM0:
		case MESON_PWM2:
			clrbits_le32(&regs->miscr, 1 << 28);
			break;

		case MESON_PWM1:
		case MESON_PWM3:
			clrbits_le32(&regs->miscr, 1 << 29);
			break;

		default:
			pr_err("Id is invalid\n");
			break;
		}
}

static void pwm_meson_config(struct udevice *dev, unsigned channel)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_reg *regs = priv->regs;
	struct meson_pwm_state *pwm_state = priv->pwm_state;

	switch (channel) {
		case MESON_PWM0:
			/*set div and clock enable*/
			setbits_le32(&regs->miscr, (pwm_state[channel].pre_div << 8 | 1 << 15));
			/*set duty*/
			writel((pwm_state[channel].hi << 16 | pwm_state[channel].lo), &regs->dar);
			break;

		case MESON_PWM1:
			/*set div and clock enable*/
			setbits_le32(&regs->miscr, (pwm_state[channel].pre_div << 16 | 1 << 23));
			/*set duty*/
			writel((pwm_state[channel].hi << 16 | pwm_state[channel].lo), &regs->dbr);
			break;

		default:
			break;
	}
}

static void pwm_meson_config_ext(struct udevice *dev, unsigned channel)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_reg *regs = priv->regs;
	struct meson_pwm_state *pwm_state = priv->pwm_state;

	switch (channel) {
		case MESON_PWM2:
			/*set div and clock enable*/
			/*setbits_le32(&regs->miscr, (pwm_state[channel].pre_div << 8 | 1 << 15));*/
			/*set duty*/
			writel((pwm_state[channel].hi << 16 | pwm_state[channel].lo), &regs->da2r);
			break;

		case MESON_PWM3:
			/*set div and clock enable*/
			/*setbits_le32(&regs->miscr, (pwm_state[channel].pre_div << 16 | 1 << 23));*/
			/*set duty*/
			writel((pwm_state[channel].hi << 16 | pwm_state[channel].lo), &regs->db2r);
			break;

		default:
			break;
	}
}

static int meson_pwm_cacl(struct udevice *dev, uint channel, uint period,
			uint duty)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_state *pwm_state = priv->pwm_state;
	unsigned int pre_div, cnt, duty_cnt, inv;
	unsigned long fin_freq = -1;
	u64 fin_ps;

	inv = pwm_meson_get_polarity(dev, channel);
	if (inv)
		duty = period - duty;

	fin_freq = meson_pwm_clock_get_rate();
	fin_ps = (u64)NSEC_PER_SEC * 1000;
	do_div(fin_ps, fin_freq);

	for (pre_div = 0; pre_div < 0x7f; pre_div++) {
		cnt = DIV_ROUND_CLOSEST_ULL((u64)period * 1000,
				fin_ps * (pre_div + 1));
		if (cnt <= 0xffff)
			break;
	}

	if (pre_div >= 0x7f) {
		pr_err("unable to get period pre_div\n");
		return -EINVAL;
	}

	if (duty == period) {
		pwm_state[channel].pre_div = pre_div;
		pwm_state[channel].hi = cnt;
		pwm_state[channel].lo = 0;
	} else if (duty == 0) {
		pwm_state[channel].pre_div = pre_div;
		pwm_state[channel].hi = 0;
		pwm_state[channel].lo = cnt;
	} else {
		/* Then check is we can have the duty with the same pre_div */
		duty_cnt = DIV_ROUND_CLOSEST_ULL((u64)duty * 1000,
					fin_ps * (pre_div + 1));
		if (duty_cnt > 0xffff) {
			pr_err("unable to get duty cycle\n");
			return -EINVAL;
		}

		if (duty_cnt == 0)
			duty_cnt = 1;

		if (cnt == duty_cnt)
			duty_cnt -= 1;

		pwm_state[channel].pre_div = pre_div;
		pwm_state[channel].hi = duty_cnt - 1;
		pwm_state[channel].lo = cnt - duty_cnt - 1;
	}

	if (duty == period || duty == 0)
		pwm_constant_enable(dev, channel);
	else
		pwm_constant_disable(dev, channel);

	return 0;
}

static int meson_pwm_set_config(struct udevice *dev, uint channel, uint period_ns,
				uint duty_ns)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_state *pwm_state = priv->pwm_state;

	if ((duty_ns < 0) || (period_ns <= 0)) {
		pr_err("Not available duty_ns period_ns error\n");
		return -1;
	}

	if (duty_ns > period_ns) {
		pr_err("Not available duty_ns period_ns error\n");
		return -1;
	}

	if (pwm_state[channel].period != period_ns ||
	   pwm_state[channel].duty_cycle != duty_ns) {
		meson_pwm_cacl(dev, channel, period_ns, duty_ns);
		switch (channel) {
			case MESON_PWM0:
			case MESON_PWM1:
			pwm_meson_config(dev, channel);
			break;

			case MESON_PWM2:
			case MESON_PWM3:
			pwm_meson_config_ext(dev, channel);
			break;

		default:
			break;
		}
	}

	pwm_state[channel].period = period_ns;
	pwm_state[channel].duty_cycle = duty_ns;

	return 0;
};

static void meson_pwm_meson_enable(struct udevice *dev, unsigned channel)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_reg *regs = priv->regs;
	struct meson_pwm_state *pwm_state = priv->pwm_state;
	unsigned int val, orig;

	switch (channel) {
		case MESON_PWM0:
			val = 1 << 0;
			break;
		case MESON_PWM1:
			val = 1 << 1;
			break;
		case MESON_PWM2:
			val = 1 << 25;
			break;
		case MESON_PWM3:
			val = 1 << 24;
			break;
		default:
			pr_err("channel is not legal\n");
			return;
	}

	orig = readl(&regs->miscr);
	orig |= val;
	writel(orig, &regs->miscr);
	pwm_state[channel].enabled = 1;
}

static void meson_pwm_meson_disable(struct udevice *dev, unsigned channel)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_reg *regs = priv->regs;
	struct meson_pwm_state *pwm_state = priv->pwm_state;
	unsigned int val, orig;

	switch (channel) {
	case MESON_PWM0:
		val = 1 << 0;
		break;
	case MESON_PWM1:
		val = 1 << 1;
		break;
	case MESON_PWM2:
		val = 1 << 25;
		break;
	case MESON_PWM3:
		val = 1 << 24;
		break;
	default:
		pr_err("channel is not legal\n");
		return;
	}

	orig = readl(&regs->miscr);
	orig &= ~val;
	writel(orig, &regs->miscr);
	pwm_state[channel].enabled = 0;
}

static int meson_pwm_set_enable(struct udevice *dev, uint channel, bool enable)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_state *pwm_state = priv->pwm_state;

	if (pwm_state[channel].enabled != enable) {
		if (enable)
			meson_pwm_meson_enable(dev,channel);
		else
			meson_pwm_meson_disable(dev,channel);
	}

	return 0;
}

static int meson_pwm_set_invert(struct udevice *dev, uint channel, bool polarity)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_state *pwm_state = priv->pwm_state;
#if 0
	struct meson_pwm_reg *regs = priv->regs;

	switch (channel) {
	case MESON_PWM0:
		if (polarity)
			setbits_le32(&regs->miscr, 0x01 << 26);
		else
			clrbits_le32(&regs->miscr, 0x01 << 26);
		break;

	case MESON_PWM1:
		if (polarity)
			setbits_le32(&regs->miscr, 0x01 << 27);
		else
			clrbits_le32(&regs->miscr, 0x01 << 27);
		break;

	default:
		pr_err("Id is invalid\n");
		break;
	}
#endif
	if (polarity)
		pwm_state[channel].polarity = 1;
	else
		pwm_state[channel].polarity = 0;
	return 0;
};

static int meson_pwm_set_times(struct udevice *dev, uint channel, uint times)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_reg *regs = priv->regs;

	if ((times > 256) || (times <= 0)) {
		pr_err("Not available times error\n");
		return -1;
	}

	switch (channel) {
	case MESON_PWM0:
		clrbits_le32(&regs->tr, 0xff << 24);
		setbits_le32(&regs->tr, (times - 1) << 24);
		break;

	case MESON_PWM1:
		clrbits_le32(&regs->tr, 0xff << 8);
		setbits_le32(&regs->tr, (times - 1) << 8);
		break;

	case MESON_PWM2:
		clrbits_le32(&regs->tr, 0xff << 16);
		setbits_le32(&regs->tr, (times - 1) << 16);
		break;

	case MESON_PWM3:
		clrbits_le32(&regs->tr, 0xff << 0);
		setbits_le32(&regs->tr, (times - 1) << 0);
		break;

	default:
		pr_err("Id is invalid\n");
		break;
	}

	return 0;
};

static int meson_pwm_set_blink_times(struct udevice *dev, uint channel, uint times)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_reg *regs = priv->regs;

	if ((times > 16) || (times <= 0)) {
		pr_err("Not available times error\n");
		return -1;
	}

	switch (channel) {
	case MESON_PWM0:
	case MESON_PWM2:
		clrbits_le32(&regs->br, 0xf);
		setbits_le32(&regs->br, times - 1);
		break;

	case MESON_PWM1:
	case MESON_PWM3:
		clrbits_le32(&regs->br, 0xf << 4);
		setbits_le32(&regs->br, (times - 1) << 4);
		break;

	default:
		pr_err("Id is invalid\n");
		break;
	}

	return 0;
};

static int meson_pwm_blink_enable(struct udevice *dev, uint channel, bool enable)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);
	struct meson_pwm_reg *regs = priv->regs;

	switch (channel) {
	case MESON_PWM0:
	case MESON_PWM2:
		if (enable)
			setbits_le32(&regs->br, 1 << 8);
		else
			clrbits_le32(&regs->br, 1 << 8);
		break;

	case MESON_PWM1:
	case MESON_PWM3:
		if (enable)
			setbits_le32(&regs->br, 1 << 9);
		else
			clrbits_le32(&regs->br, 1 << 9);
		break;

	default:
		pr_err("Id is invalid\n");
		break;
	}

	return 0;
};

static int meson_pwm_probe(struct udevice *dev)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);

	priv->regs = (struct meson_pwm_reg *)dev_read_addr(dev);
	priv->pwm_state = (struct meson_pwm_state *)calloc(4, sizeof(struct meson_pwm_state));

	return 0;
}

int meson_pwm_remove(struct udevice *dev)
{
	struct meson_pwm_priv *priv = dev_get_priv(dev);

	free(priv->pwm_state);

	return 0;
}

static const struct pwm_ops meson_pwm_ops = {
	.set_config = meson_pwm_set_config,
	.set_enable = meson_pwm_set_enable,
	.set_invert = meson_pwm_set_invert,
	.set_times = meson_pwm_set_times,
	.set_blink_times = meson_pwm_set_blink_times,
	.set_blink_enable = meson_pwm_blink_enable,
};

static const struct udevice_id meson_pwm_ids[] = {
		{.compatible = "amlogic,g12a-ee-pwm"},
		{.compatible = "amlogic,g12a-ao-pwm"},
		{}
};

U_BOOT_DRIVER(meson_pwm) = {
	.name = "meson_pwm",
	.id = UCLASS_PWM,
	.of_match = meson_pwm_ids,
	.ops = &meson_pwm_ops,
	.probe = meson_pwm_probe,
	.remove = meson_pwm_remove,
	.priv_auto_alloc_size = sizeof(struct meson_pwm_priv),
};
