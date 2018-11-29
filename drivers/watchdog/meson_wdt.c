/*
 * Copyright 2018 Amlogic, Inc
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <wdt.h>
#include <asm/io.h>
#include <linux/io.h>
#include <clk.h>

#define MESON_WDT_CTRL_REG			0x0
#define MESON_WDT_TCNT_REG			0x8
#define MESON_WDT_RSET_REG			0xc

#define MESON_WDT_CTRL_CLKDIV_EN		BIT(25)
#define MESON_WDT_CTRL_CLK_EN			BIT(24)
#define MESON_WDT_CTRL_EE_RESET			BIT(21)
#define MESON_WDT_CTRL_EN				BIT(18)
#define MESON_WDT_CTRL_DIV_MASK			(BIT(18) - 1)
#define MESON_WDT_TCNT_SETUP_MASK		(BIT(16) - 1)
#define MESON_WDT_TCNT_CNT_SHIFT		(16)

#define DEFAULT_TIMEOUT 1

struct meson_wdt_priv {
	void __iomem *regs;
};

static int meson_wdt_reset(struct udevice *dev)
{
	struct meson_wdt_priv *priv;

	assert(dev);
	priv = dev_get_priv(dev);
	writel(0, priv->regs + MESON_WDT_RSET_REG);

	return 0;
}

static int meson_wdt_set_timeout(struct udevice *dev, unsigned int timeout)
{
	struct meson_wdt_priv *priv;
	unsigned long tcnt = timeout * 1000;

	assert(dev);
	priv = dev_get_priv(dev);
	if (tcnt > MESON_WDT_TCNT_SETUP_MASK)
		tcnt = MESON_WDT_TCNT_SETUP_MASK;
	meson_wdt_reset(dev);
	writel(tcnt, priv->regs + MESON_WDT_TCNT_REG);

	return 0;
}

static int meson_wdt_start(struct udevice *dev, u64 timeout, ulong flags)
{
	struct meson_wdt_priv *priv;

	assert(dev);
	priv = dev_get_priv(dev);
	meson_wdt_set_timeout(dev, timeout);
	writel(readl(priv->regs + MESON_WDT_CTRL_REG) | MESON_WDT_CTRL_EN,
	       priv->regs + MESON_WDT_CTRL_REG);

	return 0;
}

static int meson_wdt_stop(struct udevice *dev)
{
	struct meson_wdt_priv *priv;

	assert(dev);
	priv = dev_get_priv(dev);
	writel(readl(priv->regs + MESON_WDT_CTRL_REG) & ~MESON_WDT_CTRL_EN,
	       priv->regs + MESON_WDT_CTRL_REG);

	return 0;
}

static int meson_wdt_expire_now(struct udevice *dev, ulong flags)
{
	meson_wdt_start(dev, 1, flags);

	return 0;
}

static int meson_wdt_ofdata_to_platdata(struct udevice *dev)
{
	struct meson_wdt_priv *priv;

	assert(dev);
	priv = dev_get_priv(dev);
	priv->regs = devfdt_get_addr_ptr(dev);
	if (IS_ERR(priv->regs))
		return PTR_ERR(priv->regs);

	return 0;
}

static int meson_wdt_probe(struct udevice *dev)
{
	struct meson_wdt_priv *priv;
	fdt_addr_t addr;
	fdt_size_t size;
	struct clk w_clk;
	unsigned int rate = 0;
	int ret;

	assert(dev);
	priv = dev_get_priv(dev);
	addr = devfdt_get_addr_size_index(dev, 0, &size);
	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;
	priv->regs = (void __iomem *)addr;
	ret = clk_get_by_name(dev, "wdt-clk", &w_clk);
	if (ret < 0) {
		printf("Failed to get wdt-clk.\n");
		return ret;
	}
	rate = clk_get_rate(&w_clk);
	if (rate < 0) {
		printf("Failed to get wdt-clk rate.\n");
		return ret;
	}
	writel(((rate / 1000) & MESON_WDT_CTRL_DIV_MASK) |
		MESON_WDT_CTRL_EE_RESET |
		MESON_WDT_CTRL_CLK_EN |
		MESON_WDT_CTRL_CLKDIV_EN, priv->regs + MESON_WDT_CTRL_REG);
	meson_wdt_set_timeout(dev, DEFAULT_TIMEOUT);
	meson_wdt_stop(dev);

	return 0;
}

static const struct wdt_ops meson_wdt_ops = {
	.start = meson_wdt_start,
	.reset = meson_wdt_reset,
	.stop = meson_wdt_stop,
	.expire_now = meson_wdt_expire_now,
};

static const struct udevice_id meson_wdt_ids[] = {
	{ .compatible = "amlogic,meson-gxbb-wdt" },
	{}
};

U_BOOT_DRIVER(meson_wdt) = {
	.name = "meson_wdt",
	.id = UCLASS_WDT,
	.of_match = meson_wdt_ids,
	.ops = &meson_wdt_ops,
	.probe = meson_wdt_probe,
	.priv_auto_alloc_size = sizeof(struct meson_wdt_priv),
	.ofdata_to_platdata = meson_wdt_ofdata_to_platdata,
};

