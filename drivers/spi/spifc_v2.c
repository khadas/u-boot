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

struct spifc_regs {
	u32 cmd;
		#define SPI_FLASH_READ    31
		#define SPI_FLASH_WREN    30
		#define SPI_FLASH_WRDI    29
		#define SPI_FLASH_RDID    28
		#define SPI_FLASH_RDSR    27
		#define SPI_FLASH_WRSR    26
		#define SPI_FLASH_PP      25
		#define SPI_FLASH_SE      24
		#define SPI_FLASH_BE      23
		#define SPI_FLASH_CE      22
		#define SPI_FLASH_DP      21
		#define SPI_FLASH_RES     20
		#define SPI_HPM           19
		#define SPI_FLASH_USR     18
		#define SPI_FLASH_USR_ADDR 15
		#define SPI_FLASH_USR_DUMMY 14
		#define SPI_FLASH_USR_DIN   13
		#define SPI_FLASH_USR_DOUT   12
		#define SPI_FLASH_USR_DUMMY_BLEN   10
		#define SPI_FLASH_USR_CMD     0
	u32 addr;
		#define SPI_FLASH_BYTES_LEN 24
		#define SPI_FLASH_ADDR_START 0
	u32 ctrl;
		#define FAST_READ_QUAD_IO 24
		#define FAST_READ_DUAL_IO 23
		#define FAST_READ_QUAD_OUT 20
		#define SPI_ENABLE_AHB    17
		#define SPI_SST_AAI       16
		#define SPI_RES_RID       15
		#define FAST_READ_DUAL_OUT 14
		#define SPI_READ_READ_EN  13
		#define SPI_CLK_DIV0      12
		#define SPI_CLKCNT_N      8
		#define SPI_CLKCNT_H      4
		#define SPI_CLKCNT_L      0
	u32 ctrl1;
	u32 status;
	u32 ctrl2;
	u32 clock;
		#define SPI_CLK_DIV0_NEW  31
		#define SPI_PRE_SCALE_DIV 18
		#define SPI_CLKCNT_N_NEW  12
		#define SPI_CLKCNT_H_NEW  6
		#define SPI_CLKCNT_L_NEW  0
	u32 user;
		#define USER_CMD_INCLUDE_CMD        31
		#define USER_CMD_INCLUDE_ADDR       30
		#define USER_CMD_INCLUDE_DUMMY      29
		#define USER_CMD_INCLUDE_DIN        28
		#define USER_CMD_INCLUDE_DOUT       27
		#define USER_CMD_DUMMY_IDLE         26
		#define USER_CMD_HIGHPART_DURING_SPI_DOUT_STAGE      25
		#define USER_CMD_HIGHPART_DURING_SPI_DIN_STAGE       24
		#define USER_CMD_EXT_HOLD_IN_STA_PREP  23
		#define USER_CMD_EXT_HOLD_IN_STA_CMD   22
		#define USER_CMD_EXT_HOLD_IN_STA_ADDR  21
		#define USER_CMD_EXT_HOLD_IN_STA_DUMMY 20
		#define USER_CMD_EXT_HOLD_IN_STA_DIN   19
		#define USER_CMD_EXT_HOLD_IN_STA_DOUT  18
		#define USER_CMD_EXT_HOLD_POLARITY  17
		#define SINGLE_DIO_MODE             16
		#define FAST_WRITE_QUAD_IO          15
		#define FAST_WRITE_DUAL_IO          14
		#define FAST_WRITE_QUAD_OUT         13
		#define FAST_WRITE_DUAL_OUT         12
		#define WRITE_BYTE_ORDER            11
		#define READ_BYTE_ORDER             10
		#define AHB_ENDIAN_MODE             8
		#define MASTER_CLK_EDGE             7
		#define SLAVE_CLK_EDGE              6
		#define CS_VALID_IN_STA_PREP        5
		#define CS_VALID_IN_STA_DONE        4
		#define AHB_READ_APPLY_CONFIG       3
		#define COMPATIBLE_TO_APPOLO        2
		#define AHP_READ_SUPPORT_4BYTE_ADDR 1
		#define EN_DIN_DURING_SPI_DOUT_STAGE    0
	u32 user1;
		#define USER_CMD_ADDR_BITS         26
		#define USER_CMD_DOUT_BITS         17
		#define USER_CMD_DIN_BITS          8
		#define USER_CMD_DUMMY_BITS        0
	u32 user2;
		#define USER_CMD_CMD_BITS          28
		#define USER_CMD_CMD_VALUE         0
	u32 user3;
	u32 user4; //pin register
		#define CS_KEEP_ACTIVE_AFTER_TRANS 30
	u32 slave;
	u32 slave1;
	u32 slave2;
	u32 slave3;
	u32 cache[8];
	u32 buffer[8];
		#define SPIFC_CACHE_SIZE_IN_WORD 128
		#define SPIFC_CACHE_SIZE_IN_BYTE SPIFC_CACHE_SIZE_IN_WORD << 2
};

struct spifc_priv {
	struct spifc_regs *regs;
	void __iomem *mem_map;
#if defined(CONFIG_CLK) && (CONFIG_CLK)
	struct clk core;
#endif/* CONFIG_CLK */
	unsigned int wordlen;
	unsigned char cmd;
	struct gpio_desc cs_gpios;
};

/* flash dual/quad read command */
#define FCMD_READ				0x03
#define FCMD_READ_FAST			0x0b
#define FCMD_READ_DUAL_OUT		0x3b
#define FCMD_READ_QUAD_OUT		0x6b
#define FCMD_READ_DUAL_IO		0xbb
#define FCMD_READ_QUAD_IO		0xeb
/* flash quad write command */
#define FCMD_WRITE				0x02
#define FCMD_WRITE_QUAD_OUT		0x32

#define SPIFC_MAX_CLK_RATE		166666666
#define SPIFC_DEFAULT_SPEED		40000000
extern uint32_t get_time(void);

static u8 temp_cmd =0;
static u32 temp_addr =0;
static u32 temp_addr_len =0;

static void spifc_set_rx_op_mode(struct spifc_priv *priv,
				 unsigned int slave_mode, unsigned char cmd)
{
	unsigned int val;

	return;

#if 0
	val = readl(&priv->regs->ctrl);
	val &= ~((1 << FAST_READ_DUAL_OUT) |
			(1 << FAST_READ_QUAD_OUT) |
			(1 << FAST_READ_DUAL_IO) |
			(1 << FAST_READ_QUAD_IO));

	if (slave_mode & SPI_RX_DUAL) {
		if (cmd == FCMD_READ_DUAL_OUT)
			val |= 1 << FAST_READ_DUAL_OUT;
	}
	if (slave_mode & SPI_RX_QUAD) {
		if (cmd == FCMD_READ_QUAD_OUT)
			val |= 1 << FAST_READ_QUAD_OUT;
	}
	writel(val, &priv->regs->ctrl);
#endif
	val = readl(SPIFC_USER_CTRL3);
	val &= ~(3 << 27);   // set rx x1 mode default

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

	return;
	val = readl(SPIFC_USER_CTRL1);
	val &= ~(3 << 10);   // set tx x1 mode default

	if (slave_mode & SPI_TX_QUAD) {
		if (cmd == FCMD_WRITE_QUAD_OUT)
			val |= 1 << 11;
	}
	writel(val, SPIFC_USER_CTRL1);
}

static void spifc_nand_init(void)
{
	writel((1 << 15) | (1 << 8), CLKTREE_SPIFC_CLK_CTRL);/* force to 24M */
	/* disable ahb */
	writel((readl(SPIFC_AHB_REQ_CTRL) & ~(1 << 31)), SPIFC_AHB_REQ_CTRL);
	//writel(0, SPIFC_AHB_REQ_CTRL1);
	//writel(0, SPIFC_AHB_REQ_CTRL2);
	writel((readl(SPIFC_AHB_CTRL) & ~(1 << 31)), SPIFC_AHB_CTRL);
	/* disable ahb clk */
	//writel((readl(SPIFC_CLK_CTRL) | (7 << 12)), SPIFC_CLK_CTRL);

	/* set ahb watchdog mux */
	// writel(0xffff, SPIFC_AHB_WTCH_CTRL);

	/* enable apb clk */
	//writel(readl(SPIFC_CTRL) & ~(3 << 12), SPIFC_CTRL);
	//writel(SPIFC_CTRL, 0);
}

static int spifc_user_cmd(struct spifc_priv *priv,
			  u8 cmd, u8 *buf, u8 len)
{
	struct spifc_regs *regs = priv->regs;
	u16 bits = len ? (len - 1) : 0;
	u32 addr = 0;
	int i = 0;

	if (len) {
		for (i = 0; i < len; i++) {
			addr = addr << 8;
			addr |= buf[i];
			}
		}

	writel((1 << 30) | ((!!len) << 19) | (cmd << 20) | (bits << 15), SPIFC_USER_CTRL1);
	writel(0, SPIFC_USER_CTRL2);
	writel(0, SPIFC_USER_CTRL3);  // no data
	writel(addr, SPIFC_USER_ADDR);
	writel(1 << 31, SPIFC_USER_CTRL0); // start
	while (!(readl(SPIFC_USER_CTRL0) & 0x40000000));

	return 0;
}

static int spifc_user_cmd_dout(struct spifc_priv *priv,
			       u8 *buf, int len, unsigned long flags)
{
	struct spifc_regs *regs = priv->regs;
	unsigned int *cache;
	u32 *p;
	unsigned int val;
	int len32, i;
	u16 bits = temp_addr_len ? (temp_addr_len - 1) : 0;

	writel((0x3 << 30), SPIFC_DBUF_CTRL); // set write DBUF. auto update address

	p = (u32 *)buf;
	len32 = (len / 4) + !!(len % 4);
	for (i = 0; i < len32; i++)
		writel(*p++, SPIFC_DBUF_DATA);

	val = readl(SPIFC_USER_CTRL1);
	val |= (1 << 30);  // enable command
	val &= ~(3 << 28); // clean user command mode
	val &= ~(0xff << 20); // clear cmd
	val |= (temp_cmd << 20); // clear cmd
	val &= ~(1 << 19);
	val |= ((!!temp_addr_len) << 19);
	val &= ~(3 << 17); // clear user address mode
	val &= ~(3 << 15);
	val |= (bits << 15);
	val |= (1 << 14);	// user datout enable.
	val &= ~(1 << 13);  // disable AES
	val &= ~(1 << 12);  // output source from data buffer
	val &= ~(0x3FF << 0); // clear
	val |= (len << 0); // set data in
	writel(val, SPIFC_USER_CTRL1);

	writel(0, SPIFC_USER_CTRL2); // disable dummy
	writel(0, SPIFC_USER_CTRL3); // disable date in
	writel(0, SPIFC_USER_DBUF_ADDR); //clear buffer start address
	writel(temp_addr, SPIFC_USER_ADDR);
	writel(1 << 31, SPIFC_USER_CTRL0); // start

	while (!(readl(SPIFC_USER_CTRL0) & 0x40000000));

	return 0;
}

static int spifc_user_cmd_din(struct spifc_priv *priv,
			      u8 *buf, int len, unsigned long flags)
{
	struct spifc_regs *regs = priv->regs;
	unsigned int *cache;
	u32 *p;
	int len32, i;
	unsigned int val;
	u8 temp_buf[SPIFC_CACHE_SIZE_IN_BYTE];
	uint32_t start;
	u16 bits = temp_addr_len ? (temp_addr_len - 1) : 0;
	u32 addr = 0;

	writel((1 << 30) | ((!!temp_addr_len) << 19) | (temp_cmd << 20) | (bits << 15), SPIFC_USER_CTRL1); // enable and set cmd addr
	writel(temp_addr, SPIFC_USER_ADDR);
	writel(0, SPIFC_USER_CTRL2); // disable dummy

	val = readl(SPIFC_USER_CTRL3);
	val |= (1 << 31);  // enable data in
	val &= ~(1 << 30); // set intput data to data buffer
	val &= ~(1 << 29); // disable AES
	val &= ~(0x3FF << 16); // clear
	val |= (len << 16); // set data in led
	writel(val, SPIFC_USER_CTRL3);
	writel(0, SPIFC_USER_DBUF_ADDR); //clear buffer start address

	writel(1 << 31, SPIFC_USER_CTRL0); // start

	while (!(readl(SPIFC_USER_CTRL0) & 0x40000000));

	while (!(*(volatile uint32_t *)SPIFC_USER_CTRL0 & 0x40000000))
	;
	while (!((*(volatile uint32_t *)SPIFC_USER_CTRL0) & 1))
	;

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
	struct udevice *bus = dev->parent;
	struct spifc_priv *priv = dev_get_priv(bus);
	int ret;
	unsigned int val;
#if 0
	ret = pinctrl_select_state(bus, "default");
	if (ret) {
		pr_err("%s %d ret %d\n", __func__, __LINE__, ret);
		return ret;
	}

	dm_gpio_free(bus, &priv->cs_gpios);
	ret = gpio_request_by_name(bus, "cs-gpios",
				   0, &priv->cs_gpios, 0);
	if (ret) {
		pr_err("%s %d request gpio error!\n", __func__, __LINE__);
		return ret;
	}
	if (!dm_gpio_is_valid(&priv->cs_gpios)) {
		pr_err("%s %d cs pin gpio invalid!\n", __func__, __LINE__);
		return 1;
	}
	ret = dm_gpio_set_dir_flags(&priv->cs_gpios, GPIOD_IS_OUT);
	if (ret)
		pr_err("%s %d set dir error!\n", __func__, __LINE__);

	return ret;
#else
	val = readl(PADCTRL_PIN_MUX_REG2);
	val &= ~(0xffffff);
	val |= ((0x1 << 0) | (0x1 << 4) | (0x1 << 8) | (0x1 << 12) | (0x1 << 16) | (0x1 << 20)); // set all pinmux
	writel(val,PADCTRL_PIN_MUX_REG2);

	return 0;
#endif
}

static int spifc_release_bus(struct udevice *bus)
{
	return 0;
}

static int spifc_set_speed(struct udevice *bus, uint hz)
{
	struct spifc_priv *priv = dev_get_priv(bus);
	struct spifc_platdata *plat = dev_get_platdata(bus);
	static u32 div;
	u32 value = 0;

	if (!hz)
		return 0;

	/* Temporary default 24  */
	writel((1 << 15) | (1 << 8), CLKTREE_SPIFC_CLK_CTRL);/* force to 24M */
	writel((1 << 30) | (1 << 28) | (3 << 16) | (2 << 12) | (2 << 8) | (1), SPIFC_ACTIMING0);
#if 0
	value = SPIFC_MAX_CLK_RATE;
	if (div == value / hz)
		return 0;
	div = value / hz;
	if (div < 2) {
		pr_err("%s %d can not support %d speed!\n",
			__func__, __LINE__, hz);
		if (!plat->speed)
			div = 2;
		hz = SPIFC_MAX_CLK_RATE / div;
	}
#ifdef CONFIG_SPIFC_COMPATIBLE_TO_APPOLO
	if (div > 0x10)
		div = 0x10;
	value = readl(&regs->ctrl);
	value &= ~(0x1fff << SPI_CLKCNT_L);
	value |= ((div >> 1) - 1) << SPI_CLKCNT_H;
	value |= (div - 1) << SPI_CLKCNT_N;
	value |= (div - 1) << SPI_CLKCNT_L;
	writel(value, &regs->ctrl);
#else
	if (div > 0x40)
		div = 0x40;
	value = ((div >> 1) - 1) << SPI_CLKCNT_H_NEW;
	value |= (div - 1) << SPI_CLKCNT_N_NEW;
	value |= (div - 1) << SPI_CLKCNT_L_NEW;
	writel(value, &regs->clock);
#endif

	plat->speed = hz;
#endif
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
	int lening;
	int ret = 0;
	int i = 0;
	if (bitlen % 8) {
		printf("%s: error bitlen\n", __func__);
		return -EINVAL;
	}

	spifc_claim_bus(dev);
	spifc_set_speed(bus, slave->max_hz);
	spifc_set_mode(bus, slave->mode);
	if (flags & SPI_XFER_BEGIN) {
		if (flags & SPI_XFER_END) {
			buf = (u8 *)dout;
			if (!buf || len > 5) {
				printf("%s: error command\n", __func__);
				ret = -EINVAL;
			} else {
				spifc_user_cmd(priv, buf[0], &buf[1], len - 1);
				/* save the command for next xfer dual/quad setting */
				priv->cmd = buf[0];
			}
		} else {
			buf = (u8 *)dout;
			temp_cmd = buf[0];
			temp_addr_len = len - 1;
			temp_addr = 0;
			if (temp_addr_len) {
				for (i = 0; i < temp_addr_len; i++) {
					temp_addr = temp_addr << 8;
					temp_addr |= buf[i + 1];
				}
			}
			else
				temp_addr = 0;
			priv->cmd = buf[0];
		}
	} else if (dout && priv->cmd) { // write
		buf = (u8 *)dout;
		spifc_set_tx_op_mode(priv, slave->mode, priv->cmd);
		while (len > 0) {
			lening = min_t(size_t, 512, len);
			ret = spifc_user_cmd_dout(priv, buf, lening, flags);
			if (ret)
				break;
			buf += lening;
			len -= lening;
			//temp_addr += lening;
		}
	} else if (din && priv->cmd) { // read
		buf = (u8 *)din;
		spifc_set_rx_op_mode(priv, slave->mode, priv->cmd);
		while (len > 0) {
			lening = min_t(size_t, 512, len);
			ret = spifc_user_cmd_din(priv, buf, lening, flags);
			if (ret)
				break;
			buf += lening;
			len -= lening;
			temp_addr = temp_addr >> 8;
			temp_addr += lening;
			temp_addr = temp_addr << 8;
		}
	}
	if (ret || flags & SPI_XFER_END) {
		//temp_cmd = 0;
		//temp_addr = 0;
		//temp_addr_len = 0;
		priv->cmd = 0;
	}
	return ret;
}

static int spifc_probe(struct udevice *bus)
{
	struct spifc_platdata *plat = dev_get_platdata(bus);
	struct spifc_priv *priv = dev_get_priv(bus);
	int ret = 0;
	unsigned int val;
#if 0
#if defined(CONFIG_CLK) && (CONFIG_CLK)
	ret = clk_get_by_name(bus, "core", &priv->core);
	if (ret) {
		printf("can't get clk source!\n");
		return ret;
	}
	ret = clk_enable(&priv->core);
	if (ret) {
		printf("enable clk source fail\n");
		return ret;
	}
#endif/* CONFIG_CLK */
#endif
	spifc_nand_init();
#if 0
	ret = gpio_request_by_name(bus, "cs-gpios",
					   0, &priv->cs_gpios, 0);
	if (ret) {
		pr_err("%s %d can't get cs pin!\n", __func__, __LINE__);
		return ret;
	}
	if (!dm_gpio_is_valid(&priv->cs_gpios)) {
		pr_err("%s %d cs pin gpio invalid!\n", __func__, __LINE__);
		return 1;
	}
	/* gpio act like cs pin, default dir-out and pull up */
	ret = dm_gpio_set_dir_flags(&priv->cs_gpios, GPIOD_IS_OUT);
	if (ret)
		pr_err("%s %d set dir error!\n", __func__, __LINE__);
#else
	val = readl(PADCTRL_PIN_MUX_REG2);
	val &= ~(0xffffff); // set cs to gpio
	val |= ((0x1 << 0) | (0x1 << 4) | (0x1 << 8) | (0x1 << 12) | (0x1 << 16) | (0x1 << 20)); // set all pinmux
	writel(val,PADCTRL_PIN_MUX_REG2);
#endif
	return ret;
}

static int spifc_ofdata_to_platdata(struct udevice *bus)
{
	struct spifc_platdata *plat = dev_get_platdata(bus);
	const void *blob = gd->fdt_blob;
	int node = dev_of_offset(bus);

	plat->reg = (ulong)dev_read_addr_ptr(bus);
	/* plat->mem_map = fdtdec_get_addr(blob, node, "ahb"); */

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
