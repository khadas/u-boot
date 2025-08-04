/*
 * (C) Copyright 2024 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <irq-generic.h>
#include <power/rk801_pmic.h>
#include <power/pmic.h>

DECLARE_GLOBAL_DATA_PTR;

#if CONFIG_IS_ENABLED(IRQ)
static const struct virq_reg rk801_irqs[] = {
	[RK801_IRQ_PWRON_FALL] = {
		.mask = RK801_IRQ_PWRON_FALL_MSK,
		.reg_offset = 0,
	},
	[RK801_IRQ_PWRON_RISE] = {
		.mask = RK801_IRQ_PWRON_RISE_MSK,
		.reg_offset = 0,
	},
};

static struct virq_chip rk801_irq_chip = {
	.status_base    = RK801_INT_STS0_REG,
	.mask_base	= RK801_INT_STS0_REG,
	.num_regs	= 1,
	.read		= pmic_reg_read,
	.write		= pmic_reg_write,
	.irqs		= rk801_irqs,
	.num_irqs	= ARRAY_SIZE(rk801_irqs),
};
#endif

static struct reg_data rk801_init_reg[] = {
	{ RK801_SLEEP_CFG_REG, RK801_NONE_FUN, RK801_SLEEP_FUN_MSK },
	{ RK801_SYS_CFG2_REG, RK801_RST_RESTART_REG, RK801_RST_MSK },
	{ RK801_INT_CONFIG_REG, RK801_INT_ACT_L, RK801_INT_POL_MSK },
	{ RK801_POWER_FPWM_EN_REG, RK801_PLDO_HRDEC_EN, RK801_PLDO_HRDEC_EN },
	{ RK801_BUCK_DEBUG5_REG, 0x54, 0xff },
	{ RK801_CON_BACK1_REG, 0x18, 0xff },
};

static const struct pmic_child_info pmic_children_info[] = {
	{ .prefix = "DCDC", .driver = "rk801_buck"},
	{ .prefix = "LDO", .driver = "rk801_ldo"},
	{ .prefix = "SWITCH", .driver = "rk801_switch"},
	{ },
};

static const struct pmic_child_info power_key_info[] = {
	{ .prefix = "pwrkey", .driver = "rk8xx_pwrkey"},
	{ },
};

static int rk801_reg_count(struct udevice *dev)
{
	return RK801_SYS_CFG3_OTP_REG + 1;
}

static int rk801_write(struct udevice *dev, uint reg, const uint8_t *buff, int len)
{
	int ret;

	ret = dm_i2c_write(dev, reg, buff, len);
	if (ret) {
		printf("rk801: write reg 0x%02x failed, ret=%d\n", reg, ret);
		return ret;
	}

	return 0;
}

static int rk801_read(struct udevice *dev, uint reg, uint8_t *buff, int len)
{
	int ret;

	ret = dm_i2c_read(dev, reg, buff, len);
	if (ret) {
		printf("rk801: read reg 0x%02x failed, ret=%d\n", reg, ret);
		return ret;
	}

	return 0;
}

static int rk801_shutdown(struct udevice *dev)
{
	int ret;
	u8 val;

	ret = rk801_read(dev, RK801_SYS_CFG2_REG, &val, 1);
	if (ret)
		return ret;

	val |= DEV_OFF;

	return rk801_write(dev, RK801_SYS_CFG2_REG, &val, 1);
}

#if CONFIG_IS_ENABLED(PMIC_CHILDREN)
static int rk801_bind(struct udevice *dev)
{
	ofnode regulators_node;
	int children;

	regulators_node = dev_read_subnode(dev, "regulators");
	if (!ofnode_valid(regulators_node))
		return -ENXIO;

	children = pmic_bind_children(dev, regulators_node, pmic_children_info);
	if (!children)
		debug("%s: %s - no child found\n", __func__, dev->name);

	children = pmic_bind_children(dev, dev->node, power_key_info);
	if (!children)
		debug("%s: %s - no child found\n", __func__, dev->name);

	return 0;
}
#endif

static int rk801_ofdata_to_platdata(struct udevice *dev)
{
#if CONFIG_IS_ENABLED(IRQ)
	struct rk801_priv *priv = dev_get_priv(dev);
	u32 interrupt, phandle;
	int ret;

	phandle = dev_read_u32_default(dev, "interrupt-parent", -ENODATA);
	if (phandle == -ENODATA) {
		printf("Read 'interrupt-parent' failed, ret=%d\n", phandle);
		return phandle;
	}

	ret = dev_read_u32_array(dev, "interrupts", &interrupt, 1);
	if (ret) {
		printf("Read 'interrupts' failed, ret=%d\n", ret);
		return ret;
	}

	priv->irq = phandle_gpio_to_irq(phandle, interrupt);
	if (priv->irq < 0) {
		printf("priv to request rk801 irq, ret=%d\n", priv->irq);
		return priv->irq;
	}
#endif

	return 0;
}

static int rk801_probe(struct udevice *dev)
{
	struct rk801_priv *priv = dev_get_priv(dev);
	uint8_t msb, lsb, on, off;
	u32 pmic_id;
	int i, ret;

	ret = rk801_read(dev, RK801_ID_MSB, &msb, 1);
	if (ret)
		return ret;

	ret = rk801_read(dev, RK801_ID_LSB, &lsb, 1);
	if (ret)
		return ret;

	pmic_id = (msb << 8) | lsb;
	priv->variant = pmic_id & RK8XX_ID_MSK;
	priv->req_pwrctrl_dvs = (lsb & 0x0f) < 3;
	if (priv->req_pwrctrl_dvs) {
		/* GPIOD_IS_OUT: output inactive */
		ret = gpio_request_by_name(dev, "pwrctrl-gpios", 0,
					   &priv->pwrctrl_gpio, GPIOD_IS_OUT);
		if (ret) {
			printf("rk801: failed to get pwrctrl-gpio, ret=%d\n", ret);
			return ret;
		}
	}

	rk801_read(dev, RK801_ON_SOURCE_REG, &on, 1);
	rk801_read(dev, RK801_OFF_SOURCE_REG, &off, 1);

	printf("PMIC:  RK%x (on=0x%02x, off=0x%02x, req_dvs: %d, act: %s)\n",
	       pmic_id, on, off, priv->req_pwrctrl_dvs,
	       priv->pwrctrl_gpio.flags & GPIOD_ACTIVE_LOW ? "low" : "high");

	for (i = 0; i < ARRAY_SIZE(rk801_init_reg); i++) {
		ret = pmic_clrsetbits(dev,
				      rk801_init_reg[i].reg,
				      rk801_init_reg[i].mask,
				      rk801_init_reg[i].val);
		if (ret < 0) {
			printf("rk801: set reg 0x%x failed, ret=%d\n",
			       rk801_init_reg[i].reg, ret);
		}
	}

#if CONFIG_IS_ENABLED(IRQ)
	priv->irq_chip = &rk801_irq_chip;
	ret = virq_add_chip(dev, priv->irq_chip, priv->irq);
	if (ret) {
		printf("rk801: failed to add irqchip(irq=%d), ret=%d\n",
		       priv->irq, ret);
		return ret;
	}
#endif
	return 0;
}

static struct dm_pmic_ops rk801_ops = {
	.reg_count = rk801_reg_count,
	.read = rk801_read,
	.write = rk801_write,
	.shutdown = rk801_shutdown,
};

static const struct udevice_id rk801_ids[] = {
	{ .compatible = "rockchip,rk801" },
	{ }
};

U_BOOT_DRIVER(pmic_rk801) = {
	.name = "rk801 pmic",
	.id = UCLASS_PMIC,
	.of_match = rk801_ids,
#if CONFIG_IS_ENABLED(PMIC_CHILDREN)
	.bind = rk801_bind,
#endif
	.ofdata_to_platdata = rk801_ofdata_to_platdata,
	.priv_auto_alloc_size = sizeof(struct rk801_priv),
	.probe = rk801_probe,
	.ops = &rk801_ops,
};

