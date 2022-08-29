// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <clk.h>
#include <linux/bitfield.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/arch/secure_apb.h>
#include <spi.h>
#include <dm/device-internal.h>
#include <dm/uclass-internal.h>
#include <dm/root.h>
#include <dm/lists.h>
#include <dm/util.h>
#ifdef CONFIG_SECURE_POWER_CONTROL
#include <asm/arch/pwr_ctrl.h>
#endif

#ifdef CONFIG_CLK_MESON_A1
#define CONFIG_CLK_IF
#endif

/* use DDR generally, mustn't enable it unless SRAM test */
//#define CONFIG_SPICC_DMA_SRAM

#define CONFIG_DMA_SMC_MODE

/* Register Map */
#define SPICC_RXDATA	0x00

#define SPICC_TXDATA	0x04

#define SPICC_CONREG	0x08
#define SPICC_ENABLE		BIT(0)
#define SPICC_MODE_MASTER	BIT(1)
#define SPICC_XCH		BIT(2)
#define SPICC_SMC		BIT(3)
#define SPICC_POL		BIT(4)
#define SPICC_PHA		BIT(5)
#define SPICC_SSCTL		BIT(6)
#define SPICC_SSPOL		BIT(7)
#define SPICC_DRCTL_MASK	GENMASK(9, 8)
#define SPICC_DRCTL_IGNORE	0
#define SPICC_DRCTL_FALLING	1
#define SPICC_DRCTL_LOWLEVEL	2
#define SPICC_CS_MASK		GENMASK(13, 12)
#define SPICC_DATARATE_MASK	GENMASK(18, 16)
#define SPICC_FIX_FACTOR_MULT	1
#define SPICC_FIX_FACTOR_DIV	4
#define SPICC_BITLENGTH_MASK	GENMASK(24, 19)
#define SPICC_BURSTLENGTH_MASK	GENMASK(31, 25)
#define XCH_BURST_MAX		128

#define SPICC_INTREG	0x0c
#define SPICC_TE_EN	BIT(0) /* TX FIFO Empty Interrupt */
#define SPICC_TH_EN	BIT(1) /* TX FIFO Half-Full Interrupt */
#define SPICC_TF_EN	BIT(2) /* TX FIFO Full Interrupt */
#define SPICC_RR_EN	BIT(3) /* RX FIFO Ready Interrupt */
#define SPICC_RH_EN	BIT(4) /* RX FIFO Half-Full Interrupt */
#define SPICC_RF_EN	BIT(5) /* RX FIFO Full Interrupt */
#define SPICC_RO_EN	BIT(6) /* RX FIFO Overflow Interrupt */
#define SPICC_TC_EN	BIT(7) /* Transfert Complete Interrupt */

#define SPICC_DMAREG	0x10
#define SPICC_DMA_ENABLE		BIT(0)
/* When txfifo_count<threshold, request a read(dma->txfifo) burst */
#define SPICC_TXFIFO_THRESHOLD_MASK	GENMASK(5, 1)
/* When rxfifo count>threshold, request a write(rxfifo->dma) burst */
#define SPICC_RXFIFO_THRESHOLD_MASK	GENMASK(10, 6)
#define SPICC_READ_BURST_MASK		GENMASK(14, 11)
#define SPICC_WRITE_BURST_MASK		GENMASK(18, 15)
#define DMA_REQ_DEFAULT			8
#define SPICC_DMA_URGENT		BIT(19)
#define SPICC_DMA_THREADID_MASK		GENMASK(25, 20)
#define SPICC_DMA_BURSTNUM_MASK		GENMASK(31, 26)

#define SPICC_STATREG	0x14
#define SPICC_TE	BIT(0) /* TX FIFO Empty Interrupt */
#define SPICC_TH	BIT(1) /* TX FIFO Half-Full Interrupt */
#define SPICC_TF	BIT(2) /* TX FIFO Full Interrupt */
#define SPICC_RR	BIT(3) /* RX FIFO Ready Interrupt */
#define SPICC_RH	BIT(4) /* RX FIFO Half-Full Interrupt */
#define SPICC_RF	BIT(5) /* RX FIFO Full Interrupt */
#define SPICC_RO	BIT(6) /* RX FIFO Overflow Interrupt */
#define SPICC_TC	BIT(7) /* Transfert Complete Interrupt */

#define SPICC_PERIODREG	0x18
#define SPICC_PERIOD	GENMASK(14, 0)	/* Wait cycles */

#define SPICC_TESTREG	0x1c
#define SPICC_TXCNT_MASK	GENMASK(4, 0)	/* TX FIFO Counter */
#define SPICC_RXCNT_MASK	GENMASK(9, 5)	/* RX FIFO Counter */
#define SPICC_SMSTATUS_MASK	GENMASK(12, 10)	/* State Machine Status */
#define SPICC_LBC		BIT(14) /* Loop Back Control */
#define SPICC_SWAP		BIT(15) /* RX FIFO Data Swap */
#define SPICC_MO_DELAY_MASK	GENMASK(17, 16) /* Master Output Delay */
#define SPICC_MO_NO_DELAY	0
#define SPICC_MO_DELAY_1_CYCLE	1
#define SPICC_MO_DELAY_2_CYCLE	2
#define SPICC_MO_DELAY_3_CYCLE	3
#define SPICC_MI_DELAY_MASK	GENMASK(19, 18) /* Master Input Delay */
#define SPICC_MI_NO_DELAY	0
#define SPICC_MI_DELAY_1_CYCLE	1
#define SPICC_MI_DELAY_2_CYCLE	2
#define SPICC_MI_DELAY_3_CYCLE	3
#define SPICC_MI_CAP_DELAY_MASK	GENMASK(21, 20) /* Master Capture Delay */
#define SPICC_CAP_AHEAD_2_CYCLE	0
#define SPICC_CAP_AHEAD_1_CYCLE	1
#define SPICC_CAP_NO_DELAY	2
#define SPICC_CAP_DELAY_1_CYCLE	3
#define SPICC_FIFORST_MASK	GENMASK(23, 22) /* FIFO Softreset */

#define SPICC_DRADDR	0x20	/* Read Address of DMA */

#define SPICC_DWADDR	0x24	/* Write Address of DMA */

#define SPICC_LD_CNTL0	0x28
#define SPICC_LD_CNTL1	0x2c
#define SMC_REQ_CNT_MAX		0xffff
#ifdef CONFIG_DMA_SMC_MODE
#define DMA_BURST_MAX		(DMA_REQ_DEFAULT * SMC_REQ_CNT_MAX)
#else
#define DMA_BURST_MAX		XCH_BURST_MAX
#endif

#define SPICC_ENH_CTL0	0x38	/* Enhanced Feature 0 */
#define SPICC_ENH_CS_PRE_DELAY_MASK	GENMASK(15, 0)
#define SPICC_ENH_DATARATE_MASK		GENMASK(23, 16)
#define SPICC_ENH_FIX_FACTOR_MULT	1
#define SPICC_ENH_FIX_FACTOR_DIV	2
#define SPICC_ENH_DATARATE_EN		BIT(24)
#define SPICC_ENH_MOSI_OEN		BIT(25)
#define SPICC_ENH_CLK_OEN		BIT(26)
#define SPICC_ENH_CS_OEN		BIT(27)
#define SPICC_ENH_CS_PRE_DELAY_EN	BIT(28)
#define SPICC_ENH_MAIN_CLK_AO		BIT(29)

#define SPICC_ENH_CTL1	0x3c	/* Enhanced Feature 1 */
#define SPICC_ENH_MI_CAP_DELAY_EN	BIT(0)
#define SPICC_ENH_MI_CAP_DELAY_MASK	GENMASK(9, 1)
#define SPICC_ENH_SI_CAP_DELAY_EN	BIT(14)		/* slave mode */
#define SPICC_ENH_DELAY_EN		BIT(15)
#define SPICC_ENH_SI_DELAY_EN		BIT(16)		/* slave mode */
#define SPICC_ENH_SI_DELAY_MASK		GENMASK(19, 17)	/* slave mode */
#define SPICC_ENH_MI_DELAY_EN		BIT(20)
#define SPICC_ENH_MI_DELAY_MASK		GENMASK(23, 21)
#define SPICC_ENH_MO_DELAY_EN		BIT(24)
#define SPICC_ENH_MO_DELAY_MASK		GENMASK(27, 25)
#define SPICC_ENH_MO_OEN_DELAY_EN	BIT(28)
#define SPICC_ENH_MO_OEN_DELAY_MASK	GENMASK(31, 29)

#define SPICC_ENH_CTL2	0x40	/* Enhanced Feature */
#define SPICC_ENH_TI_DELAY_MASK		GENMASK(14, 0)
#define SPICC_ENH_TI_DELAY_EN		BIT(15)
#define SPICC_ENH_TT_DELAY_MASK		GENMASK(30, 16)
#define SPICC_ENH_TT_DELAY_EN		BIT(31)

#define CS_GPIO_MAX			4
#define SPICC_DEFAULT_WORDLEN		8

#define spicc_info(fmt, args...) \
	printf("[info]%s: " fmt, __func__, ## args)

#define spicc_err(fmt, args...) \
	printf("[error]%s: " fmt, __func__, ## args)

#define SPICC_DEBUG_EN
#ifdef SPICC_DEBUG_EN
#define spicc_dbg(fmt, args...) \
	printf("[debug]%s: " fmt, __func__, ## args)
#else
#define spicc_dbg(fmt, args...)
#endif

#define writel_relaxed(val, addr) 	writel(val, addr)
#define readl_relaxed(addr) 		readl(addr)
#define writel_bits_relaxed(mask, val, addr) \
	writel_relaxed((readl_relaxed(addr) & ~(mask)) | (val), addr)
#define readl_bits_relaxed(mask, addr) \
	((readl_relaxed(addr) & (mask)) >> (ffs(mask) - 1))
#define mask_width(mask) (fls(mask) + 1 - ffs(mask))

struct meson_spicc_data {
	unsigned int			min_speed_hz;
	unsigned int			max_speed_hz;
	unsigned int			fifo_size;
	bool				dma_burst_triggered_by_ssctl;
	bool				has_oen;
	bool				has_enhance_clk_div;
	bool				has_cs_pre_delay;
	bool				has_enhance_io_delay;
	bool				has_async_clk;
	bool				is_div_parent_async_clk;
	bool				has_enhance_tt_ti_delay;
};

struct meson_spicc_device {
	void __iomem			*base;
	u32				pm_id;
	u32				parent_clk_rate;
	u32				speed_hz;
	u32				real_speed_hz;
	int				num_chipselect;
	struct gpio_desc 		cs_gpios[CS_GPIO_MAX];
	const struct meson_spicc_data	*data;
	u8				*tx_buf;
	u8				*rx_buf;
	u16				mode;
	u16				wordlen;
	unsigned int			bytes_per_word;
	unsigned long			tx_remain;
	unsigned long			rx_remain;
	unsigned long			xfer_remain;
	unsigned int			burst_len;
	unsigned int			using_dma;
};

static void meson_spicc_oen_enable(struct meson_spicc_device *spicc)
{
	u32 conf;

	if (!spicc->data->has_oen)
		return;

	conf = readl_relaxed(spicc->base + SPICC_ENH_CTL0) |
		SPICC_ENH_MOSI_OEN | SPICC_ENH_CLK_OEN | SPICC_ENH_CS_OEN;

	writel_relaxed(conf, spicc->base + SPICC_ENH_CTL0);
}

static void meson_spicc_auto_io_delay(struct meson_spicc_device *spicc)
{
	u32 div, hz;
	u32 mi_delay, cap_delay;
	u32 conf;

	if (spicc->data->has_enhance_clk_div) {
		div = readl_bits_relaxed(SPICC_ENH_DATARATE_MASK,
				spicc->base + SPICC_ENH_CTL0);
		div++;
		div <<= 1;
	} else {
		div = readl_bits_relaxed(SPICC_DATARATE_MASK,
				spicc->base + SPICC_CONREG);
		div += 2;
		div = 1 << div;
	}

	mi_delay = SPICC_MI_NO_DELAY;
	cap_delay = SPICC_CAP_AHEAD_2_CYCLE;

	hz = spicc->real_speed_hz;
	if (spicc->mode & SPI_LOOP)
		cap_delay = SPICC_CAP_AHEAD_1_CYCLE;
	else if (hz >= 100000000)
		cap_delay = SPICC_CAP_DELAY_1_CYCLE;
	else if (hz >= 80000000)
		cap_delay = SPICC_CAP_NO_DELAY;
	else if (hz >= 40000000)
		cap_delay = SPICC_CAP_AHEAD_1_CYCLE;
	else if (div >= 16)
		mi_delay = SPICC_MI_DELAY_3_CYCLE;
	else if (div >= 8)
		mi_delay = SPICC_MI_DELAY_2_CYCLE;
	else if (div >= 6)
		mi_delay = SPICC_MI_DELAY_1_CYCLE;

	conf = readl_relaxed(spicc->base + SPICC_TESTREG);
	conf &= ~(SPICC_MO_DELAY_MASK | SPICC_MI_DELAY_MASK
		  | SPICC_MI_CAP_DELAY_MASK);
	conf |= FIELD_PREP(SPICC_MI_DELAY_MASK, mi_delay);
	conf |= FIELD_PREP(SPICC_MI_CAP_DELAY_MASK, cap_delay);
	writel_relaxed(conf, spicc->base + SPICC_TESTREG);
}

static inline bool meson_spicc_txfull(struct meson_spicc_device *spicc)
{
	return !!FIELD_GET(SPICC_TF,
			   readl_relaxed(spicc->base + SPICC_STATREG));
}

static inline bool meson_spicc_rxready(struct meson_spicc_device *spicc)
{
	return FIELD_GET(SPICC_RH | SPICC_RR | SPICC_RF,
			 readl_relaxed(spicc->base + SPICC_STATREG));
}

static inline void meson_spicc_pre_burst(struct meson_spicc_device *spicc)
{
	writel_bits_relaxed(SPICC_ENABLE, SPICC_ENABLE,
			    spicc->base + SPICC_CONREG);

	if (spicc->data->has_oen)
		writel_bits_relaxed(SPICC_ENH_MAIN_CLK_AO,
				    SPICC_ENH_MAIN_CLK_AO,
				    spicc->base + SPICC_ENH_CTL0);

	writel_bits_relaxed(SPICC_FIFORST_MASK,
			FIELD_PREP(SPICC_FIFORST_MASK, 3),
			spicc->base + SPICC_TESTREG);

	while (meson_spicc_rxready(spicc))
		readl_relaxed(spicc->base + SPICC_RXDATA);

	if (spicc->data->has_oen)
		writel_bits_relaxed(SPICC_ENH_MAIN_CLK_AO, 0,
				    spicc->base + SPICC_ENH_CTL0);
}

static inline void meson_spicc_post_burst(struct meson_spicc_device *spicc)
{
	writel_bits_relaxed(SPICC_ENABLE | SPICC_XCH | SPICC_SMC,
			    0, spicc->base + SPICC_CONREG);
	writel_relaxed(0, spicc->base + SPICC_DMAREG);
}

static inline u32 meson_spicc_pull_data(struct meson_spicc_device *spicc)
{
	unsigned int bytes = spicc->bytes_per_word;
	unsigned int byte_shift = 0;
	u32 data = 0;
	u8 byte;

	while (bytes--) {
		byte = *spicc->tx_buf++;
		data |= (byte & 0xff) << byte_shift;
		byte_shift += 8;
	}

	spicc->tx_remain--;
	return data;
}

static inline void meson_spicc_push_data(struct meson_spicc_device *spicc,
					 u32 data)
{
	unsigned int bytes = spicc->bytes_per_word;
	unsigned int byte_shift = 0;
	u8 byte;

	while (bytes--) {
		byte = (data >> byte_shift) & 0xff;
		*spicc->rx_buf++ = byte;
		byte_shift += 8;
	}

	spicc->rx_remain--;
}

static inline void meson_spicc_rx(struct meson_spicc_device *spicc)
{
	/* Empty RX FIFO */
	while (spicc->rx_remain &&
	       meson_spicc_rxready(spicc))
		meson_spicc_push_data(spicc,
				readl_relaxed(spicc->base + SPICC_RXDATA));
}

static inline void meson_spicc_tx(struct meson_spicc_device *spicc)
{
	/* Fill Up TX FIFO */
	while (spicc->tx_remain &&
	       !meson_spicc_txfull(spicc))
		writel_relaxed(meson_spicc_pull_data(spicc),
			       spicc->base + SPICC_TXDATA);
}

static u32 meson_spicc_calc_dma_len(struct meson_spicc_device *spicc, u32 *req)
{
	u32 len = spicc->xfer_remain / spicc->bytes_per_word;
	u32 i;

	if (len <= spicc->data->fifo_size) {
		*req = len;
		return len;
	}

	*req = DMA_REQ_DEFAULT;
	if (len == (DMA_BURST_MAX + 1)) {
		len = DMA_BURST_MAX - DMA_REQ_DEFAULT;
	} else if (len >= DMA_BURST_MAX) {
		len = DMA_BURST_MAX;
	} else {
		/* 1 < len < DMA_BURST_MAX */
		for (i = DMA_REQ_DEFAULT; i > 1; i--) {
			if ((len % i) == 0) {
				*req = i;
				return len;
			}
		}

		if ((len % DMA_REQ_DEFAULT) == 1)
			len -= DMA_REQ_DEFAULT;
		len -= len % DMA_REQ_DEFAULT;
	}

	return len;
}

static void meson_spicc_setup_dma_burst(struct meson_spicc_device *spicc)
{
	unsigned int words, req;

	words = meson_spicc_calc_dma_len(spicc, &req);

	/* Setup Xfer variables */
	spicc->burst_len = words * spicc->bytes_per_word;
	spicc->xfer_remain -= spicc->burst_len;

	writel_relaxed((u32)(unsigned long)spicc->tx_buf, spicc->base + SPICC_DRADDR);
	writel_relaxed((u32)(unsigned long)spicc->rx_buf, spicc->base + SPICC_DWADDR);
	spicc->tx_buf += spicc->burst_len;
	spicc->rx_buf += spicc->burst_len;

#ifdef CONFIG_DMA_SMC_MODE
	words /= req;
	/* Enable DMA write/read counter */
	writel_relaxed(0x3 << 4, spicc->base + SPICC_LD_CNTL0);
	/* Setup burst length */
	writel_relaxed((words << 16) | words, spicc->base + SPICC_LD_CNTL1);
#else
	/* Setup burst length */
	writel_bits_relaxed(SPICC_BURSTLENGTH_MASK,
			FIELD_PREP(SPICC_BURSTLENGTH_MASK, words - 1),
			spicc->base + SPICC_CONREG);
#endif

	writel_relaxed(SPICC_DMA_ENABLE
		    | SPICC_DMA_URGENT
		    | FIELD_PREP(SPICC_TXFIFO_THRESHOLD_MASK,
				 spicc->data->fifo_size + 1 - req)
		    | FIELD_PREP(SPICC_READ_BURST_MASK, req - 1)
		    | FIELD_PREP(SPICC_RXFIFO_THRESHOLD_MASK, req - 1)
		    | FIELD_PREP(SPICC_WRITE_BURST_MASK, req - 1),
		    spicc->base + SPICC_DMAREG);

#ifdef CONFIG_DMA_SMC_MODE
	writel_bits_relaxed(SPICC_SMC, SPICC_SMC, spicc->base + SPICC_CONREG);
#else
	writel_bits_relaxed(SPICC_XCH, SPICC_XCH, spicc->base + SPICC_CONREG);
#endif
}

static void meson_spicc_setup_pio_burst(struct meson_spicc_device *spicc)
{
	unsigned int burst_len;

	burst_len = min_t(unsigned int,
			  spicc->xfer_remain / spicc->bytes_per_word,
			  spicc->data->fifo_size);

	/* Setup Xfer variables */
	spicc->tx_remain = burst_len;
	spicc->rx_remain = burst_len;
	spicc->burst_len = burst_len * spicc->bytes_per_word;
	spicc->xfer_remain -= spicc->burst_len;

	/* Setup burst length */
	writel_bits_relaxed(SPICC_BURSTLENGTH_MASK,
			FIELD_PREP(SPICC_BURSTLENGTH_MASK,
				burst_len - 1),
			spicc->base + SPICC_CONREG);

	/* Fill TX FIFO */
	meson_spicc_tx(spicc);

	/* Start burst */
	writel_bits_relaxed(SPICC_XCH, SPICC_XCH, spicc->base + SPICC_CONREG);
}

/* os interface */
static int spicc_wait_complete(struct meson_spicc_device *spicc)
{
	u32 comp;
	unsigned long x10us;

	x10us = 8LL * 100000LL * spicc->burst_len * 2 / spicc->speed_hz;
	/* some tolerance */
	x10us += 20000;

	while (x10us--) {
		udelay(10);
		comp = readl_relaxed(spicc->base + SPICC_STATREG) & SPICC_TC;
#ifdef CONFIG_DMA_SMC_MODE
		if (spicc->using_dma)
			comp = !(readl(spicc->base + SPICC_DMAREG)
				 & SPICC_DMA_ENABLE);
#endif
		if (comp)
			break;
	}

	/* set 1 to clear */
	writel_bits_relaxed(SPICC_TC, SPICC_TC, spicc->base + SPICC_STATREG);

	return x10us ? 0 : -ETIME;
}

static void spicc_chipselect(struct udevice *dev, bool select)
{
	struct meson_spicc_device *spicc = dev_get_priv(dev->parent);
	struct spi_slave *slave = dev_get_parent_priv(dev);
	bool level = slave->mode & SPI_CS_HIGH;
	int cs = spi_chip_select(dev);

	spicc = dev_get_priv(slave->dev->parent);
	if ((cs >= spicc->num_chipselect) || (cs < 0)) {
		spicc_err("cs %d over\n", cs);
		return;
	}
	if (!select)
		level = !level;

	dm_gpio_set_value(&spicc->cs_gpios[cs], level);
}

static int spicc_claim_bus(struct udevice *dev)
{
	spicc_chipselect(dev, 1);
	return 0;
}

static int spicc_release_bus(struct udevice *dev)
{
	spicc_chipselect(dev, 0);
	return 0;
}

static int spicc_set_speed(struct udevice *bus, uint hz)
{
	struct meson_spicc_device *spicc = dev_get_priv(bus);
	u32 sys_clk_rate, div, mid, real_hz;
	u32 regv;

	if (!hz || (spicc->speed_hz == hz))
		return 0;

	sys_clk_rate = spicc->parent_clk_rate;
	if (spicc->data->has_enhance_clk_div) {
		/* speed = sys_clk_rate / 2 / (div+1) */
		div = DIV_ROUND_UP(sys_clk_rate, hz << 1);
		if (div)
			div--;
		if (div > 0xff)
			div = 0xff;
		regv = readl_relaxed(spicc->base + SPICC_ENH_CTL0);
		regv &= ~SPICC_ENH_DATARATE_MASK;
		regv |= FIELD_PREP(SPICC_ENH_DATARATE_MASK, div);
		regv |= SPICC_ENH_DATARATE_EN;
		writel_relaxed(regv, spicc->base + SPICC_ENH_CTL0);
		real_hz = sys_clk_rate / ((div + 1) << 1);
	} else {
		/* speed = sys_clk_rate / 2^(div+2) */
		mid = (sys_clk_rate * 3) >> 4;
		for (div = 0; div < 7; div++) {
			if (hz >= mid)
				break;
			mid >>= 1;
		}
		regv = readl_relaxed(spicc->base + SPICC_CONREG);
		regv &= ~SPICC_DATARATE_MASK;
		regv |= FIELD_PREP(SPICC_DATARATE_MASK, div);
		writel_relaxed(regv, spicc->base + SPICC_CONREG);
		real_hz = sys_clk_rate / (1 << (div + 2));
	}

	spicc->speed_hz = hz;
	spicc->real_speed_hz = real_hz;
	spicc_dbg("set speed %dHz (real %dHz)\n", hz, real_hz);

	meson_spicc_auto_io_delay(spicc);

	return 0;
}

static int spicc_set_mode(struct udevice *bus, uint mode)
{
	struct meson_spicc_device *spicc = dev_get_priv(bus);
	u32 conf = 0;

	if (spicc->mode == mode)
		return 0;

	conf = readl_relaxed(spicc->base + SPICC_CONREG);

	/* Setup transfer mode */
	if (mode & SPI_CPOL)
		conf |= SPICC_POL;
	else
		conf &= ~SPICC_POL;

	if (mode & SPI_CPHA)
		conf |= SPICC_PHA;
	else
		conf &= ~SPICC_PHA;

	if (mode & SPI_CS_HIGH)
		conf |= SPICC_SSPOL;
	else
		conf &= ~SPICC_SSPOL;

	writel_relaxed(conf, spicc->base + SPICC_CONREG);

	conf = readl_relaxed(spicc->base + SPICC_TESTREG);
	if (mode & SPI_LOOP)
		conf |= SPICC_LBC;
	else
		conf &= ~SPICC_LBC;
	writel_relaxed(conf, spicc->base + SPICC_TESTREG);

	spicc->mode = mode;
	spicc_dbg("set mode 0x%x\n", mode);

	return 0;
}

static int spicc_set_wordlen(struct udevice *bus, uint wordlen)
{
	struct meson_spicc_device *spicc = dev_get_priv(bus);
	u32 regv;

	if (!wordlen || (spicc->wordlen == wordlen))
		return 0;

	spicc->bytes_per_word = DIV_ROUND_UP(wordlen, 8);

	regv = readl_relaxed(spicc->base + SPICC_CONREG);
	regv &= ~SPICC_BITLENGTH_MASK;
	regv |= FIELD_PREP(SPICC_BITLENGTH_MASK,
			  (spicc->bytes_per_word << 3) - 1);
	writel_relaxed(regv, spicc->base + SPICC_CONREG);

	spicc->using_dma = (wordlen == 64) ? 1 : 0;
	spicc->wordlen = wordlen;
	spicc_dbg("set wordlen %d\n", wordlen);

	return 0;
}

static int spicc_xfer(
		struct udevice *dev,
		unsigned int bitlen,
		const void *dout,
		void *din,
		unsigned long flags)
{
	struct udevice *bus = dev->parent;
	struct meson_spicc_device *spicc = dev_get_priv(bus);
	struct spi_slave *slave = dev_get_parent_priv(dev);
	int ret = 0;

	if ((bitlen % slave->wordlen) || (slave->wordlen == 64 && bitlen <= 64))
		return -EINVAL;

	spicc_set_wordlen(bus, slave->wordlen);

	/* Setup transfer parameters */
	spicc->tx_buf = (u8 *)dout;
	spicc->rx_buf = (u8 *)din;
	spicc->xfer_remain = bitlen >> 3;

#ifndef CONFIG_SPICC_DMA_SRAM
	if (spicc->using_dma) {
		flush_dcache_range((unsigned long)dout,
				   (unsigned long)dout + (bitlen >> 3));
		flush_dcache_range((unsigned long)din,
				   (unsigned long)din + (bitlen >> 3));
	}
#endif

	if (flags & SPI_XFER_BEGIN)
		spicc_chipselect(dev, 1);

	while (spicc->xfer_remain) {
		meson_spicc_pre_burst(spicc);
		if (spicc->using_dma)
			meson_spicc_setup_dma_burst(spicc);
		else
			meson_spicc_setup_pio_burst(spicc);

		ret = spicc_wait_complete(spicc);
		if (ret) {
			spicc_err("time expire\n");
			break;
		}

		if (!spicc->using_dma)
			meson_spicc_rx(spicc);

		meson_spicc_post_burst(spicc);
	}

	if (ret || flags & SPI_XFER_END)
		spicc_chipselect(dev, 0);

#ifndef CONFIG_SPICC_DMA_SRAM
	if (spicc->using_dma) {
		invalidate_dcache_range((unsigned long)dout,
					(unsigned long)dout + (bitlen >> 3));
		invalidate_dcache_range((unsigned long)din,
					(unsigned long)din + (bitlen >> 3));
	}
#endif

	return ret;
}

static int spicc_clk_init(struct udevice *bus)
{
	struct meson_spicc_device *spicc = dev_get_priv(bus);
	struct clk clk;

	spicc->parent_clk_rate = 0;
	if (!clk_get_by_name(bus, "core", &clk)) {
		if (!spicc->data->is_div_parent_async_clk)
			spicc->parent_clk_rate = clk_get_rate(&clk);
	}

	if (!clk_get_by_name(bus, "core-gate", &clk))
		clk_enable(&clk);

	if (spicc->data->has_async_clk) {
		if (!clk_get_by_name(bus, "async", &clk)) {
			if (spicc->data->is_div_parent_async_clk)
				spicc->parent_clk_rate = clk_get_rate(&clk);
		}

		if (!clk_get_by_name(bus, "async-gate", &clk))
			clk_enable(&clk);
	}

	spicc_info("parent clk rate %d\n", spicc->parent_clk_rate);
	return spicc->parent_clk_rate ? 0 : -ENODEV;
}

static int spicc_cs_init(struct udevice *bus)
{
	struct meson_spicc_device *spicc = dev_get_priv(bus);
	int i;
	struct gpio_desc cs;

	spicc->num_chipselect = 0;
	for (i = 0; i < ARRAY_SIZE(spicc->cs_gpios); i++) {
		if (gpio_request_by_name(bus, "cs-gpios", i, &cs, 0))
			break;
		if (!dm_gpio_is_valid(&cs)) {
			spicc_err("cs-gpio invalid (%dth)\n", i);
			return -ENODEV;
		}
		dm_gpio_set_dir_flags(&cs, GPIOD_IS_OUT);
		spicc->cs_gpios[i] = cs;
		spicc->num_chipselect++;
	}

	spicc_info("total cs %d\n", spicc->num_chipselect);
	return 0;
}


static int spicc_probe(struct udevice *bus)
{
	struct meson_spicc_device *spicc = dev_get_priv(bus);

	spicc->data = (struct meson_spicc_data *)dev_get_driver_data(bus);
	spicc->base = (void __iomem *)dev_read_addr(bus);

#ifdef CONFIG_SECURE_POWER_CONTROL
	if (!dev_read_u32(bus, "pm-id", &spicc->pm_id)) {
		pwr_ctrl_psci_smc(spicc->pm_id, true);
		spicc_info("power on %d\n", spicc->pm_id);
	}
#endif
	spicc_info("0x%p\n", spicc->base);
	if (spicc_clk_init(bus))
		return -ENODEV;

	if (spicc_cs_init(bus))
		return -ENODEV;

	/* Set master mode and enable controller */
	writel_relaxed(SPICC_ENABLE | SPICC_MODE_MASTER
		       | (spicc->data->dma_burst_triggered_by_ssctl
					? SPICC_SSCTL : 0),
		       spicc->base + SPICC_CONREG);

	/* Disable all IRQs */
	writel_relaxed(0, spicc->base + SPICC_INTREG);

	/* Setup no wait cycles by default */
	writel_relaxed(0, spicc->base + SPICC_PERIODREG);

	meson_spicc_oen_enable(spicc);

	spicc_set_wordlen(bus, SPICC_DEFAULT_WORDLEN);

	return 0;
}

/* g9tv/gxbb */

/* gxtvbb/gxl/gxm/ (LD_CNTL) */
static const struct meson_spicc_data meson_spicc_gx_data = {
	.min_speed_hz		= 325000,
	.max_speed_hz		= 4166667,
	.dma_burst_triggered_by_ssctl = true,
	.fifo_size		= 16,
};

/* txl/gxlx/txlx (ENHANCE_CNTL) */
static const struct meson_spicc_data meson_spicc_txlx_data = {
	.min_speed_hz		= 325000,
	.max_speed_hz		= 83333333,
	.dma_burst_triggered_by_ssctl = true,
	.fifo_size		= 16,
	.has_oen		= true,
	.has_enhance_clk_div	= true,
	.has_cs_pre_delay	= true,
};

/* axg (ENHANCE_CNTL1, async-clk is only used for delay ctrl) */
static const struct meson_spicc_data meson_spicc_axg_data = {
	.min_speed_hz		= 325000,
	.max_speed_hz		= 83333333,
	.fifo_size		= 16,
	.has_oen		= true,
	.has_enhance_clk_div	= true,
	.has_cs_pre_delay	= true,
	.has_enhance_io_delay	= true,
	.has_async_clk		= true,
};

/* g12a (ENHANCE_CNTL2, async-clk can be used for spi-io-clk)
   txhd hasn't ENHANCE_CNTL2 */
static const struct meson_spicc_data meson_spicc_g12a_data = {
	.min_speed_hz		= 50000,
	.max_speed_hz		= 166666667,
	.fifo_size		= 15,
	.has_oen		= true,
	.has_enhance_clk_div	= true,
	.has_cs_pre_delay	= true,
	.has_enhance_io_delay	= true,
	.has_async_clk		= true,
	.is_div_parent_async_clk = true,
	.has_enhance_tt_ti_delay = true,
};

static const struct udevice_id meson_spicc_of_match[] = {
	{
		.compatible	= "amlogic,meson-gx-spicc",
		.data		= (unsigned long)&meson_spicc_gx_data,
	},
	{
		.compatible	= "amlogic,meson-txlx-spicc",
		.data		= (unsigned long)&meson_spicc_txlx_data,
	},
	{
		.compatible	= "amlogic,meson-axg-spicc",
		.data		= (unsigned long)&meson_spicc_axg_data,
	},
	{
		.compatible	= "amlogic,meson-g12a-spicc",
		.data		= (unsigned long)&meson_spicc_g12a_data,
	},
	{ /* sentinel */ }
};

static const struct dm_spi_ops spicc_ops = {
	.claim_bus = spicc_claim_bus,
	.release_bus = spicc_release_bus,
	.xfer = spicc_xfer,
	.set_speed = spicc_set_speed,
	.set_mode = spicc_set_mode,
	.set_wordlen = spicc_set_wordlen,
};

U_BOOT_DRIVER(spicc) = {
	.name = "spicc",
	.id = UCLASS_SPI,
	.of_match = meson_spicc_of_match,
	.priv_auto_alloc_size = sizeof(struct meson_spicc_device),
	.per_child_auto_alloc_size = sizeof(struct spi_slave),
	.ops= &spicc_ops,
	.probe = spicc_probe,
};
