/*
 * (C) Copyright 2024 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <asm/gpio.h>
#include <power/pmic.h>
#include <power/rk801_pmic.h>
#include <power/regulator.h>

#define RK801_BUCK_VSEL_MASK		0x7f
#define RK801_LDO_VSEL_MASK		0x3f
#define ENABLE_MASK(id)			(BIT(4 + (id)) | BIT(id))
#define ENABLE_VAL(id)			(BIT(4 + (id)) | BIT(id))
#define DISABLE_VAL(id)			(BIT(4 + (id)) | 0)

enum {
	PM_SLEEP = 0,
	PM_RUNTIME = 1,
};

struct runtime_device {
	int reg_src;
	int reg_dst;
};

struct regulator_desc {
	int id;
	unsigned int min_uV;
	unsigned int uV_step;
	const struct linear_range *linear_ranges;
	int n_linear_ranges;
	unsigned int vsel_reg;
	unsigned int vsel_mask;
	unsigned int enable_reg;
	unsigned int enable_mask;
	unsigned int enable_val;
	unsigned int disable_val;
	unsigned int ramp_delay;
};

struct linear_range {
	unsigned int min;
	unsigned int min_sel;
	unsigned int max_sel;
	unsigned int step;
};

#define REGULATOR_LINEAR_RANGE(_min_uV, _min_sel, _max_sel, _step_uV)	\
{									\
	.min		= _min_uV,					\
	.min_sel	= _min_sel,					\
	.max_sel	= _max_sel,					\
	.step		= _step_uV,					\
}

static const struct linear_range rk801_buck1_voltage_ranges[] = {
	REGULATOR_LINEAR_RANGE(500000,   0, 80, 12500),	/* 0.5v - 1.5v */
	REGULATOR_LINEAR_RANGE(1800000, 81, 82, 400000),/* 1.8v - 2.2v */
	REGULATOR_LINEAR_RANGE(3300000, 83, 83, 0),	/* 3.3v */
	REGULATOR_LINEAR_RANGE(5000000, 84, 84, 0),	/* 5.0v */
	REGULATOR_LINEAR_RANGE(5250000, 85, 85, 0),	/* 5.25v */
};

static const struct linear_range rk801_buck2_voltage_ranges[] = {
	REGULATOR_LINEAR_RANGE(800000,  0, 2, 50000),	/* 0.8v - 0.9v */
	REGULATOR_LINEAR_RANGE(1800000, 3, 4, 400000),	/* 1.8v - 2.2v */
	REGULATOR_LINEAR_RANGE(3300000, 5, 5, 0),	/* 3.3v */
	REGULATOR_LINEAR_RANGE(5000000, 6, 6, 0),	/* 5.0v */
	REGULATOR_LINEAR_RANGE(5250000, 7, 7, 0),	/* 5.25v */
};

static const struct linear_range rk801_buck4_voltage_ranges[] = {
	REGULATOR_LINEAR_RANGE(500000,   0, 80, 12500),	/* 0.5v - 1.5v */
	REGULATOR_LINEAR_RANGE(1800000, 81, 82, 400000),/* 1.8v - 2.2v */
	REGULATOR_LINEAR_RANGE(2500000, 83, 83, 0),	/* 2.5v */
	REGULATOR_LINEAR_RANGE(2800000, 84, 84, 0),	/* 2.8v */
	REGULATOR_LINEAR_RANGE(3000000, 85, 85, 0),	/* 3.0v */
	REGULATOR_LINEAR_RANGE(3300000, 86, 86, 0),	/* 3.3v */
};

static const struct linear_range rk801_ldo1_voltage_ranges[] = {
	REGULATOR_LINEAR_RANGE(500000, 0, 58, 50000),	/* 0.5v - 3.4v */
};

static const struct linear_range rk801_ldo2_voltage_ranges[] = {
	REGULATOR_LINEAR_RANGE(500000, 0, 58, 50000),	/* 0.5v - 3.4v */
};

static const struct regulator_desc rk801_desc[] = {
	{
		.id = RK801_ID_DCDC1,
		.linear_ranges = rk801_buck1_voltage_ranges,
		.n_linear_ranges = ARRAY_SIZE(rk801_buck1_voltage_ranges),
		.vsel_reg = RK801_BUCK1_ON_VSEL_REG,
		.vsel_mask = RK801_BUCK_VSEL_MASK,
		.enable_reg = RK801_POWER_EN0_REG,
		.enable_mask = ENABLE_MASK(RK801_ID_DCDC1),
		.enable_val = ENABLE_VAL(RK801_ID_DCDC1),
		.disable_val = DISABLE_VAL(RK801_ID_DCDC1),
		.ramp_delay = 1, // TODO: +32
	}, {
		.id = RK801_ID_DCDC2,
		.linear_ranges = rk801_buck2_voltage_ranges,
		.n_linear_ranges = ARRAY_SIZE(rk801_buck2_voltage_ranges),
		.vsel_reg = RK801_BUCK2_ON_VSEL_REG,
		.vsel_mask = RK801_BUCK_VSEL_MASK,
		.enable_reg = RK801_POWER_EN0_REG,
		.enable_mask = ENABLE_MASK(RK801_ID_DCDC2),
		.enable_val = ENABLE_VAL(RK801_ID_DCDC2),
		.disable_val = DISABLE_VAL(RK801_ID_DCDC2),
		.ramp_delay = 1,
	}, {
		.id = RK801_ID_DCDC3,
		.enable_reg = RK801_POWER_EN0_REG,
		.enable_mask = ENABLE_MASK(RK801_ID_DCDC3),
		.enable_val = ENABLE_VAL(RK801_ID_DCDC3),
		.disable_val = DISABLE_VAL(RK801_ID_DCDC3),
	}, {
		.id = RK801_ID_DCDC4,
		.linear_ranges = rk801_buck4_voltage_ranges,
		.n_linear_ranges = ARRAY_SIZE(rk801_buck4_voltage_ranges),
		.vsel_reg = RK801_BUCK4_ON_VSEL_REG,
		.vsel_mask = RK801_BUCK_VSEL_MASK,
		.enable_reg = RK801_POWER_EN0_REG,
		.enable_mask = ENABLE_MASK(RK801_ID_DCDC4),
		.enable_val = ENABLE_VAL(RK801_ID_DCDC4),
		.disable_val = DISABLE_VAL(RK801_ID_DCDC4),
		.ramp_delay = 1,
	}, {
		.id = RK801_ID_LDO1,
		.linear_ranges = rk801_ldo1_voltage_ranges,
		.n_linear_ranges = ARRAY_SIZE(rk801_ldo1_voltage_ranges),
		.vsel_reg = RK801_LDO1_ON_VSEL_REG,
		.vsel_mask = RK801_LDO_VSEL_MASK,
		.enable_reg = RK801_POWER_EN1_REG,
		.enable_mask = ENABLE_MASK(0),
		.enable_val = ENABLE_VAL(0),
		.disable_val = DISABLE_VAL(0),
		.ramp_delay = 1,
	}, {
		.id = RK801_ID_LDO2,
		.linear_ranges = rk801_ldo2_voltage_ranges,
		.n_linear_ranges = ARRAY_SIZE(rk801_ldo2_voltage_ranges),
		.vsel_reg = RK801_LDO2_ON_VSEL_REG,
		.vsel_mask = RK801_LDO_VSEL_MASK,
		.enable_reg = RK801_POWER_EN1_REG,
		.enable_mask = ENABLE_MASK(1),
		.enable_val = ENABLE_VAL(1),
		.disable_val = DISABLE_VAL(1),
		.ramp_delay = 1,
	}, {
		.id = RK801_ID_SWITCH,
		.n_linear_ranges = 0,
		.enable_reg = RK801_POWER_EN1_REG,
		.enable_mask = ENABLE_MASK(2),
		.enable_val = ENABLE_VAL(2),
		.disable_val = DISABLE_VAL(2),
		.ramp_delay = 1,
	},
};

static int linear_range_get_value(const struct linear_range *r,
				  unsigned int selector, unsigned int *val)
{
	if (r->min_sel > selector || r->max_sel < selector)
		return -EINVAL;

	*val = r->min + (selector - r->min_sel) * r->step;

	return 0;
}

static int linear_range_get_value_array(const struct linear_range *r, int ranges,
					unsigned int selector, unsigned int *uV)
{
	int i;

	for (i = 0; i < ranges; i++) {
		if (r[i].min_sel <= selector && r[i].max_sel >= selector)
			return linear_range_get_value(&r[i], selector, uV);
	}

	return -EINVAL;
}

static unsigned int linear_range_get_max_value(const struct linear_range *r)
{
	return r->min + (r->max_sel - r->min_sel) * r->step;
}

static int linear_range_get_selector_high(const struct linear_range *r,
					  unsigned int val,
					  unsigned int *selector,
					  bool *found)
{
	*found = false;

	if (linear_range_get_max_value(r) < val)
		return -EINVAL;

	if (r->min > val) {
		*selector = r->min_sel;
		return 0;
	}

	*found = true;

	if (r->step == 0)
		*selector = r->max_sel;
	else
		*selector = DIV_ROUND_UP(val - r->min, r->step) + r->min_sel;

	return 0;
}

int regulator_map_voltage_linear_range(const struct regulator_desc *desc,
				       int min_uV, int max_uV)
{
	const struct linear_range *range;
	int ret = -EINVAL;
	unsigned int sel;
	bool found;
	uint voltage, i;

	if (!desc->n_linear_ranges)
		return -EINVAL;

	for (i = 0; i < desc->n_linear_ranges; i++) {
		range = &desc->linear_ranges[i];

		ret = linear_range_get_selector_high(range, min_uV, &sel,
						     &found);
		if (ret)
			continue;

		ret = sel;

		/*
		 * Map back into a voltage to verify we're still in bounds.
		 * If we are not, then continue checking rest of the ranges.
		 */
		if (linear_range_get_value_array(desc->linear_ranges,
					desc->n_linear_ranges, sel, &voltage))
			continue;

		if (voltage >= min_uV && voltage <= max_uV)
			break;
	}

	if (i == desc->n_linear_ranges)
		return -EINVAL;

	return ret;
}

static const struct regulator_desc *rk801_get_desc(struct udevice *dev)
{
	struct dm_regulator_uclass_platdata *uc_pdata;
	int i, id;

	/* Why? Because: RK801_ID_DCDC3=4, RK801_ID_DCDC4=3 */
	uc_pdata = dev_get_uclass_platdata(dev);
	if (uc_pdata->type == REGULATOR_TYPE_BUCK) {
		switch (dev->driver_data) {
		case 1:
			id = RK801_ID_DCDC1;
			break;
		case 2:
			id = RK801_ID_DCDC2;
			break;
		case 3:
			id = RK801_ID_DCDC3;
			break;
		case 4:
			id = RK801_ID_DCDC4;
			break;
		default:
			id = -EINVAL;
		}
	} else if (uc_pdata->type == REGULATOR_TYPE_LDO) {
		switch (dev->driver_data) {
		case 1:
			id = RK801_ID_LDO1;
			break;
		case 2:
			id = RK801_ID_LDO2;
			break;
		default:
			id = -EINVAL;
		}
	} else {
		id = RK801_ID_SWITCH;
	}

	if (id != -EINVAL) {
		for (i = 0; i < ARRAY_SIZE(rk801_desc); i++) {
			if (rk801_desc[i].id == id)
				return &rk801_desc[i];
		}
	}

	return NULL;
}

static int rk801_regulator_get_value(struct udevice *dev, bool runtime)
{
	const struct regulator_desc *desc = rk801_get_desc(dev);
	struct udevice *pmic = dev->parent;
	int sel, val, vsel_reg, ret;
	uint uV;

	if (!desc)
		return -ENODEV;

	if (desc->id == RK801_ID_DCDC3)
		return -ENOSYS;

	if (runtime)
		vsel_reg = desc->vsel_reg;
	else
		vsel_reg = desc->vsel_reg + RK801_SLP_REG_OFFSET;

	val = pmic_reg_read(pmic, vsel_reg);
	if (val < 0)
		return val;

	sel = (val & desc->vsel_mask) >> (ffs(desc->vsel_mask) - 1);
	ret = linear_range_get_value_array(desc->linear_ranges,
					   desc->n_linear_ranges, sel, &uV);

	debug("%s, %s, desc[%d]: reg=%02x, ret=%d, sel=0x%02x(%d), uV=%d\n",
	      __func__, dev->name, desc->id, vsel_reg, ret, sel, sel, uV);

	return ret ? ret : uV;
}

static int rk801_regulator_set_value(struct udevice *dev, int uV, bool runtime)
{
	const struct regulator_desc *desc = rk801_get_desc(dev);
	struct udevice *pmic = dev->parent;
	struct rk801_priv *priv = dev_get_priv(pmic);
	const struct gpio_desc *gpio = &priv->pwrctrl_gpio;
	uint reg, reg0, reg1, sel;
	int ret, gpio_level;

	if (!desc)
		return -ENODEV;

	if (desc->id == RK801_ID_DCDC3)
		return -ENOSYS;

	if (priv->req_pwrctrl_dvs) {
		reg0 = desc->vsel_reg;
		reg1 = desc->vsel_reg + RK801_SLP_REG_OFFSET;

		gpio_level = dm_gpio_get_value(gpio);
		reg = (gpio_level == 1) ? reg0 : reg1;

		sel = regulator_map_voltage_linear_range(desc, uV, uV);
		if (sel < 0)
			return sel;

		sel <<= ffs(desc->vsel_mask) - 1;

		debug("%s, %s, desc[%d]: reg=%02x, uV=%d, sel=0x%02x(%d), gpio=%d\n",
		      __func__, dev->name, desc->id, reg, uV, sel, sel, gpio_level);

		ret = pmic_clrsetbits(pmic, reg, desc->vsel_mask, sel);
		if (ret)
			return ret;

		udelay(40); /* hw sync */

		dm_gpio_set_value(gpio, !gpio_level);

		if (reg == reg0)
			ret = pmic_clrsetbits(pmic, reg1, desc->vsel_mask, sel);
		else
			ret = pmic_clrsetbits(pmic, reg0, desc->vsel_mask, sel);

		/* if sleep mode: set pwrctrl pin inactive anyway */
		if (!runtime) {
			dm_gpio_set_value(gpio, 0);
			udelay(40); /* hw sync */
		}

		return ret;
	} else {
		if (runtime)
			reg = desc->vsel_reg;
		else
			reg = desc->vsel_reg + RK801_SLP_REG_OFFSET;

		sel = regulator_map_voltage_linear_range(desc, uV, uV);
		if (sel < 0)
			return sel;

		debug("%s, %s, desc[%d]: reg=%02x, uV=%d, sel=0x%02x(%d)\n",
		      __func__, dev->name, desc->id, reg, uV, sel, sel);

		sel <<= ffs(desc->vsel_mask) - 1;

		return pmic_clrsetbits(pmic, reg, desc->vsel_mask, sel);
	}
}

static int rk801_regulator_get_enable(struct udevice *dev, bool runtime)
{
	const struct regulator_desc *desc = rk801_get_desc(dev);
	struct udevice *pmic = dev->parent;
	int val;

	if (!desc)
		return -ENODEV;

	if (runtime) {
		val = pmic_reg_read(pmic, desc->enable_reg);
		if (val < 0)
			return val;

		val &= desc->enable_mask;

		return val == desc->enable_val;
	} else {
		val = pmic_reg_read(pmic, RK801_POWER_SLP_EN_REG);
		if (val < 0)
			return val;

		return (val & BIT(desc->id));
	}
}

static int rk801_regulator_set_enable(struct udevice *dev, bool enable)
{
	const struct regulator_desc *desc = rk801_get_desc(dev);
	struct udevice *pmic = dev->parent;
	int val;

	if (!desc)
		return -ENODEV;

	val = enable ? desc->enable_val : desc->disable_val;

	debug("%s, %s, desc[%d]: reg=%02x, mask=%02x, enable=%d, val=0x%02x(%d)\n",
	      __func__, dev->name, desc->id, desc->enable_reg,
	      desc->enable_mask, enable, val, val);

	return pmic_clrsetbits(pmic, desc->enable_reg, desc->enable_mask, val);
}

static int rk801_regulator_set_suspend_enable(struct udevice *dev, bool enable)
{
	const struct regulator_desc *desc = rk801_get_desc(dev);
	struct udevice *pmic = dev->parent;

	if (!desc)
		return -ENODEV;

	debug("%s, %s, desc[%d]: reg=%02x, mask=%02lx, enable=0x%02x, val=0x%02lx\n",
	      __func__, dev->name, desc->id, RK801_POWER_SLP_EN_REG,
	      BIT(desc->id), enable, BIT(desc->id));

	return pmic_clrsetbits(pmic, RK801_POWER_SLP_EN_REG,
			       BIT(desc->id), BIT(desc->id));
}

static int buck_ldo_get_value(struct udevice *dev)
{
	return rk801_regulator_get_value(dev, PM_RUNTIME);
}

static int buck_ldo_set_value(struct udevice *dev, int uV)
{
	return rk801_regulator_set_value(dev, uV, PM_RUNTIME);
}

static int buck_ldo_get_enable(struct udevice *dev)
{
	return rk801_regulator_get_enable(dev, PM_RUNTIME);
}

static int buck_ldo_set_enable(struct udevice *dev, bool enable)
{
	struct rk801_priv *priv = dev_get_priv(dev->parent);
	int ret;

	ret = rk801_regulator_set_enable(dev, enable);
	if (ret)
		return ret;

	if (priv->req_pwrctrl_dvs)
		return rk801_regulator_set_suspend_enable(dev, enable);

	return 0;
}

static int buck_ldo_get_suspend_value(struct udevice *dev)
{
	return rk801_regulator_get_value(dev, PM_SLEEP);
}

static int buck_ldo_set_suspend_value(struct udevice *dev, int uV)
{
	return rk801_regulator_set_value(dev, uV, PM_SLEEP);
}

static int buck_ldo_get_suspend_enable(struct udevice *dev)
{
	return rk801_regulator_get_enable(dev, PM_SLEEP);
}

static int buck_ldo_set_suspend_enable(struct udevice *dev, bool enable)
{
	struct rk801_priv *priv = dev_get_priv(dev->parent);
	int ret;

	ret = rk801_regulator_set_suspend_enable(dev, enable);
	if (ret)
		return ret;

	if (priv->req_pwrctrl_dvs)
		return rk801_regulator_set_enable(dev, enable);

	return 0;
}

int buck_ldo_get_ramp_delay(struct udevice *dev, int old_uV, int new_uV)
{
	const struct regulator_desc *desc = rk801_get_desc(dev);
	struct dm_regulator_uclass_platdata *uc_pdata;

	if (!desc)
		return 0;

	uc_pdata = dev_get_uclass_platdata(dev);
	if (uc_pdata->type != REGULATOR_TYPE_BUCK)
		return 0;

	return abs(new_uV - old_uV) / 1000 + 32;
}

static int switch_set_enable(struct udevice *dev, bool enable)
{
	return buck_ldo_set_enable(dev, enable);
}

static int switch_get_enable(struct udevice *dev)
{
	return rk801_regulator_get_enable(dev, PM_RUNTIME);
}

static int switch_set_suspend_enable(struct udevice *dev, bool enable)
{
	return buck_ldo_set_suspend_enable(dev, enable);
}

static int switch_get_suspend_enable(struct udevice *dev)
{
	return rk801_regulator_get_enable(dev, PM_SLEEP);
}

static int switch_get_value(struct udevice *dev)
{
	return 0;
}

static int switch_set_value(struct udevice *dev, int uV)
{
	return 0;
}

static int switch_set_suspend_value(struct udevice *dev, int uV)
{
	return 0;
}

static int switch_get_suspend_value(struct udevice *dev)
{
	return 0;
}

static int rk801_buck_probe(struct udevice *dev)
{
	struct rk801_priv *priv = dev_get_priv(dev->parent);
	struct dm_regulator_uclass_platdata *uc_pdata;
	struct udevice *pmic = dev->parent;
	struct runtime_device rdev[] = {
		{ RK801_BUCK1_ON_VSEL_REG, RK801_BUCK1_SLP_VSEL_REG },
		{ RK801_BUCK2_ON_VSEL_REG, RK801_BUCK2_SLP_VSEL_REG },
		{ RK801_BUCK4_ON_VSEL_REG, RK801_BUCK4_SLP_VSEL_REG },
		{ RK801_LDO1_ON_VSEL_REG,  RK801_LDO1_SLP_VSEL_REG },
		{ RK801_LDO2_ON_VSEL_REG,  RK801_LDO2_SLP_VSEL_REG },
	};
	uint val, en0, en1;
	int i, ret;

	uc_pdata = dev_get_uclass_platdata(dev);
	uc_pdata->type = REGULATOR_TYPE_BUCK;
	uc_pdata->mode_count = 0;

	/* probe only once by buck1 */
	if (dev->driver_data != 1)
		return 0;

	/* set pwrctrl active pol and use sleep function */
	val = (priv->pwrctrl_gpio.flags & GPIOD_ACTIVE_LOW) ?
			      RK801_SLEEP_ACT_L : RK801_SLEEP_ACT_H;
	ret = pmic_clrsetbits(pmic, RK801_SYS_CFG2_REG,
			      RK801_SLEEP_POL_MSK, val);
	if (ret < 0)
		return ret;

	ret = pmic_clrsetbits(pmic, RK801_SLEEP_CFG_REG,
			      RK801_SLEEP_FUN_MSK, RK801_SLEEP_FUN);
	if (ret < 0)
		return ret;

	/* disable buck/pldo slp lp */
	ret = pmic_clrsetbits(pmic, RK801_SLP_LP_CONFIG_REG,
			      RK801_SLP_LP_MASK, 0);
	if (ret < 0)
		return ret;

	/* copy on/slp enabel */
	en0 = pmic_reg_read(pmic, RK801_POWER_EN0_REG);
	if (en0 < 0)
		return en0;

	en1 = pmic_reg_read(pmic, RK801_POWER_EN1_REG);
	if (en1 < 0)
		return en1;

	val = (en0 & 0x0f) | ((en1 & 0x0f) << 4);
	ret = pmic_reg_write(pmic, RK801_POWER_SLP_EN_REG, val);
	if (ret < 0)
		return ret;

	/* copy on/slp vsel */
	for (i = 0; i < ARRAY_SIZE(rdev); i++) {
		val = pmic_reg_read(pmic, rdev[i].reg_src);
		if (val < 0)
			return val;

		ret = pmic_reg_write(pmic, rdev[i].reg_dst, val);
		if (ret < 0)
			return ret;
	}

	return 0;
}

static int rk801_ldo_probe(struct udevice *dev)
{
	struct dm_regulator_uclass_platdata *uc_pdata;

	uc_pdata = dev_get_uclass_platdata(dev);
	uc_pdata->type = REGULATOR_TYPE_LDO;
	uc_pdata->mode_count = 0;

	return 0;
}

static int rk801_switch_probe(struct udevice *dev)
{
	struct dm_regulator_uclass_platdata *uc_pdata;

	uc_pdata = dev_get_uclass_platdata(dev);
	uc_pdata->type = REGULATOR_TYPE_FIXED;
	uc_pdata->mode_count = 0;

	return 0;
}

static const struct dm_regulator_ops rk801_buck_ldo_ops = {
	.get_value  = buck_ldo_get_value,
	.set_value  = buck_ldo_set_value,
	.set_enable = buck_ldo_set_enable,
	.get_enable = buck_ldo_get_enable,
	.set_suspend_value = buck_ldo_set_suspend_value,
	.get_suspend_value = buck_ldo_get_suspend_value,
	.set_suspend_enable = buck_ldo_set_suspend_enable,
	.get_suspend_enable = buck_ldo_get_suspend_enable,
	.get_ramp_delay = buck_ldo_get_ramp_delay,
};

static const struct dm_regulator_ops rk801_switch_ops = {
	.get_value  = switch_get_value,
	.set_value  = switch_set_value,
	.set_enable = switch_set_enable,
	.get_enable = switch_get_enable,
	.set_suspend_enable = switch_set_suspend_enable,
	.get_suspend_enable = switch_get_suspend_enable,
	.set_suspend_value = switch_set_suspend_value,
	.get_suspend_value = switch_get_suspend_value,
};

U_BOOT_DRIVER(rk801_buck) = {
	.name = "rk801_buck",
	.id = UCLASS_REGULATOR,
	.ops = &rk801_buck_ldo_ops,
	.probe = rk801_buck_probe,
};

U_BOOT_DRIVER(rk801_ldo) = {
	.name = "rk801_ldo",
	.id = UCLASS_REGULATOR,
	.ops = &rk801_buck_ldo_ops,
	.probe = rk801_ldo_probe,
};

U_BOOT_DRIVER(rk801_switch) = {
	.name = "rk801_switch",
	.id = UCLASS_REGULATOR,
	.ops = &rk801_switch_ops,
	.probe = rk801_switch_probe,
};

