// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2025 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <regmap.h>
#include <syscon.h>

#include "pinctrl-rockchip.h"

static int rv1126b_set_mux(struct rockchip_pin_bank *bank, int pin, int mux)
{
	struct rockchip_pinctrl_priv *priv = bank->priv;
	int iomux_num = (pin / 8);
	struct regmap *regmap;
	int reg, ret, mask;
	u8 bit;
	u32 data;

	debug("setting mux of GPIO%d-%d to %d\n", bank->bank_num, pin, mux);

	regmap = priv->regmap_base;
	reg = bank->iomux[iomux_num].offset;
	if ((pin % 8) >= 4)
		reg += 0x4;
	bit = (pin % 4) * 4;
	mask = 0xf;

	if (bank->recalced_mask & BIT(pin))
		rockchip_get_recalced_mux(bank, pin, &reg, &bit, &mask);
	data = (mask << (bit + 16));
	data |= (mux & mask) << bit;

	debug("iomux write reg = %x data = %x\n", reg, data);

	ret = regmap_write(regmap, reg, data);

	return ret;
}

#define RV1126B_DRV_BITS_PER_PIN		8
#define RV1126B_DRV_PINS_PER_REG		2
#define RV1126B_DRV_GPIO0_A_OFFSET		0x100
#define RV1126B_DRV_GPIO0_C_OFFSET		0x8120
#define RV1126B_DRV_GPIO_OFFSET(GPION)		(0x8100 + GPION * 0x8040)

static int rv1126b_calc_drv_reg_and_bit(struct rockchip_pin_bank *bank,
				       int pin_num, struct regmap **regmap,
				       int *reg, u8 *bit)
{
	struct rockchip_pinctrl_priv *priv = bank->priv;

	*regmap = priv->regmap_base;
	switch (bank->bank_num) {
	case 0:
		if (pin_num < 16)
			*reg = RV1126B_DRV_GPIO0_A_OFFSET;
		else
			*reg = RV1126B_DRV_GPIO0_C_OFFSET - 0x20;
		break;

	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		*reg = RV1126B_DRV_GPIO_OFFSET(bank->bank_num);
		break;

	default:
		dev_err(info->dev, "unsupported bank_num %d\n", bank->bank_num);

		return -EINVAL;
	}

	*reg += ((pin_num / RV1126B_DRV_PINS_PER_REG) * 4);
	*bit = pin_num % RV1126B_DRV_PINS_PER_REG;
	*bit *= RV1126B_DRV_BITS_PER_PIN;

	return 0;
}

static int rv1126b_set_drive(struct rockchip_pin_bank *bank,
			    int pin_num, int strength)
{
	struct regmap *regmap;
	int reg, ret;
	u32 data;
	u8 bit;
	int rmask_bits = RV1126B_DRV_BITS_PER_PIN;

	ret = rv1126b_calc_drv_reg_and_bit(bank, pin_num, &regmap, &reg, &bit);
	if (ret)
		return ret;

	/* enable the write to the equivalent lower bits */
	data = ((1 << rmask_bits) - 1) << (bit + 16);
	ret = (1 << (strength + 1)) - 1;
	data |= (ret << bit);
	ret = regmap_write(regmap, reg, data);

	return ret;
}

#define RV1126B_PULL_BITS_PER_PIN		2
#define RV1126B_PULL_PINS_PER_REG		8
#define RV1126B_PULL_GPIO0_A_OFFSET		0x300
#define RV1126B_PULL_GPIO0_C_OFFSET		0x8308
#define RV1126B_PULL_GPIO_OFFSET(GPION)		(0x8300 + GPION * 0x8010)

static int rv1126b_calc_pull_reg_and_bit(struct rockchip_pin_bank *bank,
					int pin_num, struct regmap **regmap,
					int *reg, u8 *bit)
{
	struct rockchip_pinctrl_priv *priv = bank->priv;

	*regmap = priv->regmap_base;
	switch (bank->bank_num) {
	case 0:
		if (pin_num < 16)
			*reg = RV1126B_PULL_GPIO0_A_OFFSET;
		else
			*reg = RV1126B_PULL_GPIO0_C_OFFSET - 0x8;
		break;

	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		*reg = RV1126B_PULL_GPIO_OFFSET(bank->bank_num);
		break;

	default:
		dev_err(info->dev, "unsupported bank_num %d\n", bank->bank_num);

		return -EINVAL;
	}

	*reg += ((pin_num / RV1126B_PULL_PINS_PER_REG) * 4);
	*bit = pin_num % RV1126B_PULL_PINS_PER_REG;
	*bit *= RV1126B_PULL_BITS_PER_PIN;

	return 0;
}

static int rv1126b_set_pull(struct rockchip_pin_bank *bank,
			   int pin_num, int pull)
{
	struct regmap *regmap;
	int reg, ret;
	u8 bit, type;
	u32 data;

	if (pull == PIN_CONFIG_BIAS_PULL_PIN_DEFAULT)
		return -ENOTSUPP;

	ret = rv1126b_calc_pull_reg_and_bit(bank, pin_num, &regmap, &reg, &bit);
	if (ret)
		return ret;
	type = bank->pull_type[pin_num / 8];

	ret = rockchip_translate_pull_value(type, pull);
	if (ret < 0) {
		debug("unsupported pull setting %d\n", pull);

		return ret;
	}

	/* enable the write to the equivalent lower bits */
	data = ((1 << RV1126B_PULL_BITS_PER_PIN) - 1) << (bit + 16);
	data |= (ret << bit);
	ret = regmap_write(regmap, reg, data);

	return ret;
}

#define RV1126B_SMT_BITS_PER_PIN		1
#define RV1126B_SMT_PINS_PER_REG		8
#define RV1126B_SMT_GPIO0_A_OFFSET		0x500
#define RV1126B_SMT_GPIO0_C_OFFSET		0x8508
#define RV1126B_SMT_GPIO_OFFSET(GPION)		(0x8500 + GPION * 0x8010)

static int rv1126b_calc_schmitt_reg_and_bit(struct rockchip_pin_bank *bank,
					   int pin_num,
					   struct regmap **regmap,
					   int *reg, u8 *bit)
{
	struct rockchip_pinctrl_priv *priv = bank->priv;

	*regmap = priv->regmap_base;
	switch (bank->bank_num) {
	case 0:
		if (pin_num < 16)
			*reg = RV1126B_SMT_GPIO0_A_OFFSET;
		else
			*reg = RV1126B_SMT_GPIO0_C_OFFSET - 0x8;
		break;

	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		*reg = RV1126B_SMT_GPIO_OFFSET(bank->bank_num);
		break;

	default:
		dev_err(info->dev, "unsupported bank_num %d\n", bank->bank_num);
		return -EINVAL;
	}

	*reg += ((pin_num / RV1126B_SMT_PINS_PER_REG) * 4);
	*bit = pin_num % RV1126B_SMT_PINS_PER_REG;
	*bit *= RV1126B_SMT_BITS_PER_PIN;

	return 0;
}

static int rv1126b_set_schmitt(struct rockchip_pin_bank *bank,
			      int pin_num, int enable)
{
	struct regmap *regmap;
	int reg, ret;
	u32 data;
	u8 bit;

	ret = rv1126b_calc_schmitt_reg_and_bit(bank, pin_num, &regmap, &reg, &bit);
	if (ret)
		return ret;

	/* enable the write to the equivalent lower bits */
	data = ((1 << RV1126B_SMT_BITS_PER_PIN) - 1) << (bit + 16);
	data |= (enable << bit);
	ret = regmap_write(regmap, reg, data);

	return ret;
}

static struct rockchip_pin_bank rv1126b_pin_banks[] = {
	PIN_BANK_IOMUX_4_OFFSET(0, 32, "gpio0",
				      0x0, 0x8, 0x8010, 0x8018),
	PIN_BANK_IOMUX_4_OFFSET(1, 32, "gpio1",
				      0x10020, 0x10028, 0x10030, 0x10038),
	PIN_BANK_IOMUX_4_OFFSET(2, 32, "gpio2",
				      0x18040, 0x18048, 0x18050, 0x18058),
	PIN_BANK_IOMUX_4_OFFSET(3, 32, "gpio3",
				      0x20060, 0x20068, 0x20070, 0x20078),
	PIN_BANK_IOMUX_4_OFFSET(4, 32, "gpio4",
				      0x28080, 0x28088, 0x28090, 0x28098),
	PIN_BANK_IOMUX_4_OFFSET(5, 32, "gpio5",
				      0x300a0, 0x300a8, 0x300b0, 0x300b8),
	PIN_BANK_IOMUX_4_OFFSET(6, 32, "gpio6",
				      0x380c0, 0x380c8, 0x380d0, 0x380d8),
	PIN_BANK_IOMUX_4_OFFSET(7, 32, "gpio7",
				      0x400e0, 0x400e8, 0x400f0, 0x400f8),
};
static struct rockchip_pin_ctrl rv1126b_pin_ctrl __maybe_unused = {
	.pin_banks		= rv1126b_pin_banks,
	.nr_banks		= ARRAY_SIZE(rv1126b_pin_banks),
	.nr_pins		= 256,
	.set_mux		= rv1126b_set_mux,
	.set_pull		= rv1126b_set_pull,
	.set_drive		= rv1126b_set_drive,
	.set_schmitt		= rv1126b_set_schmitt,
};

static const struct udevice_id rv1126b_pinctrl_ids[] = {
	{
		.compatible = "rockchip,rv1126b-pinctrl",
		.data = (ulong)&rv1126b_pin_ctrl
	},
	{ }
};

U_BOOT_DRIVER(pinctrl_rv1126b) = {
	.name		= "rockchip_rv1126b_pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= rv1126b_pinctrl_ids,
	.priv_auto_alloc_size = sizeof(struct rockchip_pinctrl_priv),
	.ops		= &rockchip_pinctrl_ops,
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	.bind		= dm_scan_fdt_dev,
#endif
	.probe		= rockchip_pinctrl_probe,
};
