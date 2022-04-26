// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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
#define MESON_WDT_CTRL1_REG			0x4
#define MESON_WDT_TCNT_REG			0x8
#define MESON_WDT_RSET_REG			0xc

#define MESON_WDT_CTRL_CLKDIV_EN		BIT(25)
#define MESON_WDT_CTRL_CLK_EN			BIT(24)
#define MESON_WDT_CTRL_EN				BIT(18)
#define MESON_WDT_CTRL_DIV_MASK			(BIT(18) - 1)
#define MESON_WDT_TCNT_SETUP_MASK		(BIT(16) - 1)
#define MESON_WDT_TCNT_CNT_SHIFT		(16)
#define MESON_WDT_RST_SIG_EN			BIT(17)

#define WDT_DISABLE	1
#define WDT_ENABLE	2
#define WDT_PING	3
#define WDT_INIT	4
#define WDT_RESETNOW	5
#define WDT_SETTIMEOUT	6
#define WDT_OPS		0x82000086

#define DEFAULT_TIMEOUT 1			/* second */

struct meson_wdt_priv {
	void __iomem *regs;
};

struct meson_wdt_data {
	unsigned char rst_shift;
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

static int meson_gxbb_wdt_set_timeout(struct udevice *dev, u64 timeout_ms)
{
	struct meson_wdt_priv *priv;
	unsigned long tcnt = timeout_ms;

	assert(dev);
	priv = dev_get_priv(dev);
	if (tcnt > MESON_WDT_TCNT_SETUP_MASK)
		tcnt = MESON_WDT_TCNT_SETUP_MASK;
	meson_gxbb_wdt_reset(dev);
	writel(tcnt, priv->regs + MESON_WDT_TCNT_REG);

	return 0;
}

static int meson_gxbb_wdt_start(struct udevice *dev, u64 timeout_ms, ulong flags)
{
	struct meson_wdt_priv *priv;

	assert(dev);
	priv = dev_get_priv(dev);
	meson_gxbb_wdt_set_timeout(dev, timeout_ms);
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
	struct meson_wdt_data *data;

	fdt_addr_t addr;
	fdt_size_t size;
	struct clk w_clk;
	ulong rate = 0;
	unsigned int reset_by_soc = 0;
	int ret;

	assert(dev);
	priv = dev_get_priv(dev);
	data = (struct meson_wdt_data *)dev_get_driver_data(dev);
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
	if (IS_ERR_VALUE(rate)) {
		printf("Failed to get wdt-clk rate.\n");
		return ret;
	}

	reset_by_soc  = !(readl(priv->regs + MESON_WDT_CTRL1_REG) &
			  MESON_WDT_RST_SIG_EN);

	writel(((rate / 1000) & MESON_WDT_CTRL_DIV_MASK) |
		(reset_by_soc << data->rst_shift) |
		MESON_WDT_CTRL_CLK_EN |
		MESON_WDT_CTRL_CLKDIV_EN, priv->regs + MESON_WDT_CTRL_REG);
	meson_gxbb_wdt_set_timeout(dev, DEFAULT_TIMEOUT * 1000);
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
	.rst_shift =  21,
	.ops = &meson_gxbb_wdt_ops,
};

static struct meson_wdt_data meson_sc2_data = {
	.rst_shift = 22,
	.ops = &meson_gxbb_wdt_ops,
};
/**************** a1 **********************/
void __attribute__((weak)) wdt_send_cmd_to_bl31(uint64_t cmd, uint64_t value)
{

}
static int meson_a1_wdt_start(struct udevice *dev, u64 timeout_ms, ulong flags)
{
	wdt_send_cmd_to_bl31(WDT_INIT, timeout_ms);
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
	if (device_is_compatible(dev,"amlogic,meson-gxbb-wdt") ||
	    device_is_compatible(dev,"amlogic,meson-sc2-wdt"))
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
	{
		.compatible = "amlogic,meson-sc2-wdt",
		.data = (ulong)&meson_sc2_data,
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
