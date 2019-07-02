// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Amlogic Meson SPI flash controller(SPIFC)
 *
 * Copyright (C) 2018 Amlogic Corporation
 *
 * Licensed under the GPL-2 or later.
 *
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <clk.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/arch/secure_apb.h>
#include <spi.h>
#include <amlogic/spifc.h>
#include <dm/device-internal.h>
#include <dm/uclass-internal.h>
#include <dm/root.h>
#include <dm/lists.h>
#include <dm/util.h>
#include <dm/pinctrl.h>

DECLARE_GLOBAL_DATA_PTR;

struct spifc_priv {
	struct spifc_regs *regs;
	void __iomem *mem_map;
#if defined(CONFIG_CLK) && (CONFIG_CLK)
	struct clk spifc_source;
	struct clk spifc_mux;
	struct clk spifc_div;
	struct clk spifc_gate;
#endif/* CONFIG_CLK */
	unsigned int wordlen;
	unsigned char cmd;
	/* If datain or dataout is included, save the current command address */
	unsigned char save_cmd;
	u32 save_addr;
	u32 save_addr_len;
	u32 loop;
};

/* flash dual/quad read command */
#define FCMD_READ						0x03
#define FCMD_READ_FAST					0x0b
#define FCMD_READ_DUAL_OUT				0x3b
#define FCMD_READ_QUAD_OUT				0x6b
#define FCMD_READ_DUAL_IO				0xbb
#define FCMD_READ_QUAD_IO				0xeb
/* flash quad write command */
#define FCMD_WRITE						0x02
#define FCMD_WRITE_QUAD_OUT				0x32
#define FCMD_RANDOM_DATA_PROGRAM		0x84
#define FCMD_RANDOM_DATA_PROGRAM_QUAD	0x34

#define SPIFC_DEFAULT_SPEED		24000000
#define SPIFC_CACHE_SIZE_IN_WORD 128
#define SPIFC_CACHE_SIZE_IN_BYTE SPIFC_CACHE_SIZE_IN_WORD << 2

static void spifc_set_rx_op_mode(struct spifc_priv *priv,
				 unsigned int slave_mode, unsigned char cmd)
{
	unsigned int val;

	val = readl(SPIFC_USER_CTRL3);
	val &= ~(3 << 27);

	if (slave_mode & SPI_RX_DUAL) {
		if (cmd == FCMD_READ_DUAL_OUT)
			val |= 1 << 27;
	}
	if (slave_mode & SPI_RX_QUAD) {
		if (cmd == FCMD_READ_QUAD_OUT)
			val |= 1 << 28;
	}
	writel(val, SPIFC_USER_CTRL3);
}

static void spifc_set_tx_op_mode(struct spifc_priv *priv,
				 unsigned int slave_mode, unsigned char cmd)
{
	unsigned int val = 0;

	val = readl(SPIFC_USER_CTRL1);
	val &= ~(3 << 10);

	if (slave_mode & SPI_TX_QUAD) {
		if (cmd == FCMD_WRITE_QUAD_OUT)
			val |= 1 << 11;
	}
	writel(val, SPIFC_USER_CTRL1);
}

static void spifc_init(void)
{
	/* disable ahb */
	writel((readl(SPIFC_AHB_REQ_CTRL) & ~(1 << 31)), SPIFC_AHB_REQ_CTRL);
	writel((readl(SPIFC_AHB_CTRL) & ~(1 << 31)), SPIFC_AHB_CTRL);
}

static int spifc_user_cmd(struct spifc_priv *priv,
			  u8 cmd, u8 *buf, u8 len)
{
	u16 bits = len ? (len - 1) : 0;
	u32 addr = 0;
	int i = 0;

	if (len)
		for (i = 0; i < len; i++) {
			addr = addr << 8;
			addr |= buf[i];
			}
	writel((1 << 30) | ((!!len) << 19) | (cmd << 20) | (bits << 15), SPIFC_USER_CTRL1);
	writel(0, SPIFC_USER_CTRL2);
	writel(0, SPIFC_USER_CTRL3);
	writel(addr, SPIFC_USER_ADDR);
	writel(1 << 31, SPIFC_USER_CTRL0);
	while (!(readl(SPIFC_USER_CTRL0) & 0x40000000));

	return 0;
}

static int spifc_user_cmd_dout(struct spifc_priv *priv,
			       u8 *buf, int len, unsigned long flags)
{
	u32 *p;
	unsigned int val;
	int len32, i;
	u16 bits = priv->save_addr_len ? (priv->save_addr_len - 1) : 0;

	/* The commands FCMD_WRITE and FCMD_WRITE_QUAD_OUT
	 * do not support continuous writes. You need to change
	 * the commands to FCMD_RANDOM_DATA_PROGRAM and
	 * FCMD_RANDOM_DATA_PROGRAM_QUAD.
	 */
	if (priv->loop) {
		if (priv->save_cmd == FCMD_WRITE)
			priv->save_cmd = FCMD_RANDOM_DATA_PROGRAM;
		else if (priv->save_cmd == FCMD_WRITE_QUAD_OUT)
			priv->save_cmd = FCMD_RANDOM_DATA_PROGRAM_QUAD;
	}
	/* set write DBUF. auto update address */
	writel((0x3 << 30), SPIFC_DBUF_CTRL);

	p = (u32 *)buf;
	len32 = (len / 4) + !!(len % 4);
	for (i = 0; i < len32; i++)
		writel(*p++, SPIFC_DBUF_DATA);

	val = readl(SPIFC_USER_CTRL1);
	val &= ~((3 << 28) | (0xff << 20) | (1 << 19) | (3 << 17) | (3 << 15) | (1 << 13) | (1 << 12) | (0x3FF << 0));
	val |= ((1 << 30) | ((priv->save_cmd << 20)) | ((!!priv->save_addr_len) << 19) | (bits << 15) | (1 << 14) | (len << 0));
	writel(val, SPIFC_USER_CTRL1);
	/* disable dummy */
	writel(0, SPIFC_USER_CTRL2);
	/* disable date in */
	writel(0, SPIFC_USER_CTRL3);
	/* clear buffer start address */
	writel(0, SPIFC_USER_DBUF_ADDR);
	writel(priv->save_addr, SPIFC_USER_ADDR);
	writel(1 << 31, SPIFC_USER_CTRL0);
	while (!(readl(SPIFC_USER_CTRL0) & 0x40000000));

	return 0;
}

static int spifc_user_cmd_din(struct spifc_priv *priv,
			      u8 *buf, int len, unsigned long flags)
{
	u32 *p;
	int len32, i;
	unsigned int val;
	u8 temp_buf[SPIFC_CACHE_SIZE_IN_BYTE];
	u16 bits = priv->save_addr_len ? (priv->save_addr_len - 1) : 0;

	/* enable and set cmd addr */
	writel((1 << 30) | ((!!priv->save_addr_len) << 19) | (priv->save_cmd << 20) | (bits << 15), SPIFC_USER_CTRL1);
	writel(priv->save_addr, SPIFC_USER_ADDR);
	/* disable dummy */
	writel(0, SPIFC_USER_CTRL2);
	val = readl(SPIFC_USER_CTRL3);
	val &= ~((1 << 30) | (1 << 29) | (0x3FF << 16));
	val |= ((1 << 31) | (len << 16));
	writel(val, SPIFC_USER_CTRL3);
	/* clear buffer start address */
	writel(0, SPIFC_USER_DBUF_ADDR);
	writel(1 << 31, SPIFC_USER_CTRL0);
	while (!(readl(SPIFC_USER_CTRL0) & 0x40000000));
	while (!(readl(SPIFC_USER_CTRL0) & 1));

	writel((0x1 << 30) ,SPIFC_DBUF_CTRL);
	p = (u32 *)temp_buf;
	len32 = (len / 4) + !!(len % 4);
	for (i = 0; i < len32; i++) {
		*p++ = readl(SPIFC_DBUF_DATA);
	}
	memcpy(buf, temp_buf, len);

	return 0;
}

static int spifc_claim_bus(struct udevice *dev)
{
	return 0;
}

static int spifc_release_bus(struct udevice *bus)
{
	return 0;
}

static int spifc_set_speed(struct udevice *bus, uint hz)
{
	struct spifc_priv *priv = dev_get_priv(bus);
	struct spifc_platdata *plat = dev_get_platdata(bus);
	int ret;

	if (!hz)
		return 0;

	ret = clk_set_rate(&priv->spifc_div, plat->speed);
	if (ret) {
		pr_err("can't set clk rate 0x%x\n !",plat->speed);
		return ret;
	}
	writel((1 << 30) | (1 << 28) | (7 << 16) | (4 << 12) | (4 << 8) | (2), SPIFC_ACTIMING0);

	return 0;
}

static int spifc_set_mode(struct udevice *bus, uint mode)
{
	struct spifc_platdata *plat= dev_get_platdata(bus);

	if (mode == plat->mode)
		return 0;
	plat->mode = mode;

	return 0;
}

static int spifc_set_wordlen(struct udevice *bus, unsigned int wordlen)
{
	if (wordlen != 8)
		return -1;

	return 0;
}

static int spifc_xfer(struct udevice *dev,
		      unsigned int bitlen,
				const void *dout,
				void *din,
				unsigned long flags)
{
	struct udevice *bus = dev->parent;
	struct spi_slave *slave = dev_get_parent_priv(dev);
	struct spifc_priv *priv = dev_get_priv(bus);
	u8 *buf;
	int len = bitlen >> 3;
	int lening, ret = 0, i = 0;

	if (bitlen % 8) {
		pr_err("%s: error bitlen\n", __func__);
		return -EINVAL;
	}

	spifc_claim_bus(dev);
	spifc_set_speed(bus, slave->max_hz);
	spifc_set_mode(bus, slave->mode);
	if (flags & SPI_XFER_BEGIN) {
		if (flags & SPI_XFER_END) {
			buf = (u8 *)dout;
			if (!buf || len > 5) {
				pr_err("%s: error command\n", __func__);
				ret = -EINVAL;
			} else {
				spifc_user_cmd(priv, buf[0], &buf[1], len - 1);
				/* save the command for next xfer dual/quad setting */
				priv->cmd = buf[0];
			}
		} else {
			buf = (u8 *)dout;
			priv->save_cmd = buf[0];
			priv->save_addr_len = len - 1;
			priv->save_addr = 0;
			if (priv->save_addr_len) {
				for (i = 0; i < priv->save_addr_len; i++) {
					priv->save_addr = priv->save_addr << 8;
					priv->save_addr |= buf[i + 1];
				}
			}
			else
				priv->save_addr = 0;
			priv->cmd = buf[0];
		}
	} else if (dout && priv->cmd) {
		buf = (u8 *)dout;
		priv->loop = 0;
		spifc_set_tx_op_mode(priv, slave->mode, priv->cmd);
		while (len > 0) {
			lening = min_t(size_t, 512, len);
			ret = spifc_user_cmd_dout(priv, buf, lening, flags);
			if (ret)
				break;
			buf += lening;
			len -= lening;
			priv->save_addr += lening;
			priv->loop++;
		}
	} else if (din && priv->cmd) {
		buf = (u8 *)din;
		spifc_set_rx_op_mode(priv, slave->mode, priv->cmd);
		while (len > 0) {
			lening = min_t(size_t, 512, len);
			ret = spifc_user_cmd_din(priv, buf, lening, flags);
			if (ret)
				break;
			buf += lening;
			len -= lening;
			priv->save_addr = priv->save_addr >> 8;
			priv->save_addr += lening;
			priv->save_addr = priv->save_addr << 8;
		}
	}
	if (ret || flags & SPI_XFER_END) {
		priv->cmd = 0;
	}
	return ret;
}

static int spifc_probe(struct udevice *bus)
{
	struct spifc_priv *priv = dev_get_priv(bus);
	int ret = 0;

#if defined(CONFIG_CLK) && (CONFIG_CLK)
	ret = clk_get_by_name(bus, "fclk_source", &priv->spifc_source);
	if (ret) {
		pr_err("can't get clk fclk_source!\n");
		return ret;
	}

	ret = clk_get_by_name(bus, "spifc_mux", &priv->spifc_mux);
	if (ret) {
		pr_err("can't get clk spifc_mux!\n");
		return ret;
	}

	ret = clk_get_by_name(bus, "spifc_div", &priv->spifc_div);
	if (ret) {
		pr_err("can't get clk spifc_div!\n");
		return ret;
	}

	ret = clk_get_by_name(bus, "spifc_gate", &priv->spifc_gate);
	if (ret) {
		pr_err("can't get clk spifc_gate!\n");
		return ret;
	}

	ret = clk_set_parent(&priv->spifc_mux, &priv->spifc_source);
	if (ret) {
		pr_err("can't set clk parent!\n");
		return ret;
	}

	ret = clk_set_rate(&priv->spifc_div, SPIFC_DEFAULT_SPEED);
	if (ret) {
		pr_err("failed to set rate to 24M\n");
		return ret;
	}

	ret = clk_enable(&priv->spifc_gate);
	if (ret) {
		pr_err("enable clk fail\n");
		return ret;
	}
#endif
	spifc_init();

	return ret;
}

static int spifc_ofdata_to_platdata(struct udevice *bus)
{
	struct spifc_platdata *plat = dev_get_platdata(bus);
	const void *blob = gd->fdt_blob;
	int node = dev_of_offset(bus);

	plat->speed = fdtdec_get_uint(blob, node,
				      "max-frequency",
				      40000000);
	plat->io_num = fdtdec_get_uint(blob, node,
				       "max-io",
				       2);/* default 2 because some board only have 2 spifc io */
	plat->max_cs = fdtdec_get_uint(blob, node,
					       "max-cs",
					       2);
	plat->mode = 0;
	printf("spifc freq %d, max io %d, reg %p\n",
	       plat->speed, plat->io_num, (void *)plat->reg);
	return 0;
}

static const struct udevice_id spifc_ids[] = {
	{ .compatible = "amlogic,spifc" },
	{ }
};

static struct dm_spi_ops spifc_ops = {
	.claim_bus = spifc_claim_bus,
	.release_bus = spifc_release_bus,
	.xfer = spifc_xfer,
	.set_speed = spifc_set_speed,
	.set_mode = spifc_set_mode,
	.set_wordlen = spifc_set_wordlen,
};

U_BOOT_DRIVER(spifc) = {
	.name = "spifc",
	.id = UCLASS_SPI,
	.of_match = spifc_ids,
	.ofdata_to_platdata = spifc_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct spifc_platdata),
	.priv_auto_alloc_size = sizeof(struct spifc_priv),
	.per_child_auto_alloc_size = sizeof(struct spi_slave),
	.ops = &spifc_ops,
	.probe = spifc_probe,
};
