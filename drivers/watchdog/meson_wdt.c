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
#include <asm/arch/bl31_apis.h>

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

#define WDT_DISABLE	1
#define WDT_ENABLE	2
#define WDT_PING	3
#define WDT_INIT	4
#define WDT_RESETNOW	5
#define WDT_SETTIMEOUT	6
#define WDT_OPS		0x82000086

#define DEFAULT_TIMEOUT 1

struct meson_wdt_priv {
	void __iomem *regs;
};

struct meson_wdt_data {
	struct wdt_ops *ops;
};

/***************** gxbb ***********************/
static int meson_gxbb_wdt_reset(struct udevice *dev)
{
	struct meson_wdt_priv *priv;

	assert(dev);
	priv = dev_get_priv(dev);
	writel(0, priv->regs + MESON_WDT_RSET_REG);

	return 0;
}

static int meson_gxbb_wdt_set_timeout(struct udevice *dev, unsigned int timeout)
{
	struct meson_wdt_priv *priv;
	unsigned long tcnt = timeout * 1000;

	assert(dev);
	priv = dev_get_priv(dev);
	if (tcnt > MESON_WDT_TCNT_SETUP_MASK)
		tcnt = MESON_WDT_TCNT_SETUP_MASK;
	meson_gxbb_wdt_reset(dev);
	writel(tcnt, priv->regs + MESON_WDT_TCNT_REG);

	return 0;
}

static int meson_gxbb_wdt_start(struct udevice *dev, u64 timeout, ulong flags)
{
	struct meson_wdt_priv *priv;

	assert(dev);
	priv = dev_get_priv(dev);
	meson_gxbb_wdt_set_timeout(dev, timeout);
	writel(readl(priv->regs + MESON_WDT_CTRL_REG) | MESON_WDT_CTRL_EN,
	       priv->regs + MESON_WDT_CTRL_REG);

	return 0;
}

static int meson_gxbb_wdt_stop(struct udevice *dev)
{
	struct meson_wdt_priv *priv;

	assert(dev);
	priv = dev_get_priv(dev);
	writel(readl(priv->regs + MESON_WDT_CTRL_REG) & ~MESON_WDT_CTRL_EN,
	       priv->regs + MESON_WDT_CTRL_REG);

	return 0;
}

static int meson_gxbb_wdt_expire_now(struct udevice *dev, ulong flags)
{
	meson_gxbb_wdt_start(dev, 1, flags);

	return 0;
}

static int meson_gxbb_wdt_probe(struct udevice *dev)
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
	meson_gxbb_wdt_set_timeout(dev, DEFAULT_TIMEOUT);
	meson_gxbb_wdt_stop(dev);

	return 0;
}

static struct wdt_ops meson_gxbb_wdt_ops = {
	.start = meson_gxbb_wdt_start,
	.stop = meson_gxbb_wdt_stop,
	.expire_now = meson_gxbb_wdt_expire_now,
	.reset = meson_gxbb_wdt_reset,
};

static struct meson_wdt_data meson_gxbb_data = {
	.ops = &meson_gxbb_wdt_ops,
};

/**************** a1 **********************/
void __attribute__((weak)) wdt_send_cmd_to_bl31(uint64_t cmd, uint64_t value)
{

}
static int meson_a1_wdt_start(struct udevice *dev, u64 timeout, ulong flags)
{
	wdt_send_cmd_to_bl31(WDT_INIT, timeout);
	wdt_send_cmd_to_bl31(WDT_ENABLE, 0);
	wdt_send_cmd_to_bl31(WDT_PING, 0);

	return 0;
}

static int meson_a1_wdt_stop(struct udevice *dev)
{
	wdt_send_cmd_to_bl31(WDT_DISABLE, 0);

	return 0;
}

static int meson_a1_wdt_expire_now(struct udevice *dev, ulong flags)
{
	meson_a1_wdt_start(dev, 1, flags);

	return 0;
}

static int meson_a1_wdt_reset(struct udevice *dev)
{
	wdt_send_cmd_to_bl31(WDT_PING, 0);

	return 0;
}

static struct wdt_ops meson_a1_ops = {
	.start = meson_a1_wdt_start,
	.stop = meson_a1_wdt_stop,
	.expire_now = meson_a1_wdt_expire_now,
	.reset = meson_a1_wdt_reset,
};

static struct meson_wdt_data meson_a1_data = {
	.ops = &meson_a1_ops,
};

/************ common code *******************/
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
	struct meson_wdt_data* priv;
	struct driver* dri;

	priv =(struct meson_wdt_data *)dev_get_driver_data(dev);
	dri = (struct driver*)dev->driver;
	dri->ops = priv->ops;
	if (device_is_compatible(dev,"amlogic,meson-gxbb-wdt"))
		meson_gxbb_wdt_probe(dev);

	return 0;
}

static const struct udevice_id meson_wdt_ids[] =
{
	{
		.compatible = "amlogic,meson-gxbb-wdt",
		.data = (ulong)&meson_gxbb_data,
	},
	{
		.compatible = "amlogic,meson-a1-wdt",
		.data = (ulong)&meson_a1_data,
	},
	{}
};

U_BOOT_DRIVER(meson_wdt) = {
	.name = "meson_wdt",
	.id = UCLASS_WDT,
	.of_match = meson_wdt_ids,
	.probe = meson_wdt_probe,
	.priv_auto_alloc_size = sizeof(struct meson_wdt_priv),
	.ofdata_to_platdata = meson_wdt_ofdata_to_platdata,
};
