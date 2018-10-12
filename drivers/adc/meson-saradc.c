// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2018 Amlogic, Inc. All rights reserved.
 * Author: Xingyu Chen <xingyu.chen@amlogic.com>
 *
 * Meson SARADC driver for U-Boot
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/arch/saradc.h>
#include <asm/arch/secure_apb.h>

#define MESON_SAR_ADC_VDDA_VOLTAGE			(1800000)
#define MESON_SAR_ADC_TIMEOUT				(100 * 1000)
#define SAR_ADC_MAX_FIFO_SIZE				16

#define SAR_CLK_BASE					AO_SAR_CLK
	#define SAR_CLK_DIV_MASK			GENMASK(7, 0)
	#define SAR_CLK_DIV_SHIFT			(0)
	#define SAR_CLK_GATE				BIT(8)
	#define SAR_CLK_MUX_MASK			GENMASK(10, 9)

#define SAR_ADC_REG0					0x00
	#define SAR_ADC_REG0_BUSY_MASK			GENMASK(30, 28)
	#define SAR_ADC_REG0_DELTA_BUSY                 BIT(30)
	#define SAR_ADC_REG0_AVG_BUSY			BIT(29)
	#define SAR_ADC_REG0_SAMPLE_BUSY		BIT(28)
	#define SAR_ADC_REG0_FIFO_FULL			BIT(27)
	#define SAR_ADC_REG0_FIFO_EMPTY			BIT(26)
	#define SAR_ADC_REG0_FIFO_COUNT_SHIFT		(21)
	#define SAR_ADC_REG0_FIFO_COUNT_MASK		GENMASK(25, 21)
	#define SAR_ADC_REG0_CURR_CHAN_ID_MASK		GENMASK(18, 16)
	#define SAR_ADC_REG0_SAMPLING_STOP		BIT(14)
	#define SAR_ADC_REG0_FIFO_CNT_IRQ_MASK          GENMASK(8, 4)
	#define SAR_ADC_REG0_FIFO_CNT_IRQ_SHIFT		(4)
	#define SAR_ADC_REG0_FIFO_IRQ_EN                BIT(3)
	#define SAR_ADC_REG0_SAMPLING_START             BIT(2)
	#define SAR_ADC_REG0_CONTINUOUS_EN              BIT(1)
	#define SAR_ADC_REG0_SAMPLE_ENGINE_ENABLE       BIT(0)

#define SAR_ADC_CHAN_LIST				0x04
	#define SAR_ADC_CHAN_LIST_MAX_INDEX_SHIFT	(24)
	#define SAR_ADC_CHAN_LIST_MAX_INDEX_MASK	GENMASK(26, 24)
	#define SAR_ADC_CHAN_LIST_ENTRY_SHIFT(_chan)	(_chan * 3)
	#define SAR_ADC_CHAN_LIST_ENTRY_MASK(_chan)	\
					(GENMASK(2, 0) << ((_chan) * 3))

#define SAR_ADC_AVG_CNTL				0x08
	#define SAR_ADC_AVG_CNTL_AVG_MODE_SHIFT(_chan)	\
					(16 + ((_chan) * 2))
	#define SAR_ADC_AVG_CNTL_AVG_MODE_MASK(_chan)	\
					(GENMASK(17, 16) << ((_chan) * 2))
	#define SAR_ADC_AVG_CNTL_NUM_SAMPLES_SHIFT(_chan)  \
					(0 + ((_chan) * 2))
	#define SAR_ADC_AVG_CNTL_NUM_SAMPLES_MASK(_chan)  \
					(GENMASK(1, 0) << ((_chan) * 2))

#define SAR_ADC_REG3					0x0c
	#define SAR_ADC_REG3_CTRL_CONT_RING_COUNTER_EN	BIT(27)
	#define SAR_ADC_REG3_CTRL_SAMPLING_CLOCK_PHASE	BIT(26)
	#define SAR_ADC_REG3_CTRL_CHAN7_MUX_SEL_MASK	GENMASK(25, 23)
	#define SAR_ADC_REG3_CTRL_CHAN7_MUX_SEL_SHIFT	(23)
	#define SAR_ADC_REG3_ADC_EN			BIT(21)
	#define SAR_ADC_REG3_BLOCK_DLY_SEL_MASK		GENMASK(9, 8)
	#define SAR_ADC_REG3_BLOCK_DLY_MASK		GENMASK(7, 0)

#define SAR_ADC_DELAY					0x10
	#define SAR_ADC_DELAY_INPUT_DLY_SEL_MASK	GENMASK(25, 24)
	#define SAR_ADC_DELAY_INPUT_DLY_CNT_MASK	GENMASK(23, 16)
	#define SAR_ADC_DELAY_BL30_BUSY			BIT(15)
	#define SAR_ADC_DELAY_KERNEL_BUSY		BIT(14)
	#define SAR_ADC_DELAY_SAMPLE_DLY_SEL_MASK	GENMASK(9, 8)
	#define SAR_ADC_DELAY_SAMPLE_DLY_CNT_MASK	GENMASK(7, 0)

#define SAR_ADC_LAST_RD					0x14

#define SAR_ADC_FIFO_RD					0x18
	#define SAR_ADC_FIFO_RD_CHAN_ID_SHIFT		(12)
	#define SAR_ADC_FIFO_RD_CHAN_ID_MASK		GENMASK(14, 12)
	#define SAR_ADC_FIFO_RD_SAMPLE_VALUE_MASK	GENMASK(11, 0)

#define SAR_ADC_AUX_SW					0x1c
#define SAR_ADC_AUX_SW_MUX_SEL_CHAN_MASK(_chan)		\
					(GENMASK(10, 8) << (((_chan) - 2) * 3))

#define SAR_ADC_CHAN_10_SW				0x20
	#define SAR_ADC_CHAN_10_SW_CHAN1_MUX_SEL_MASK	GENMASK(25, 23)
	#define SAR_ADC_CHAN_10_SW_CHAN0_MUX_SEL_MASK	GENMASK(9, 7)

#define SAR_ADC_DETECT_IDLE_SW				0x24
	#define SAR_ADC_DETECT_IDLE_SW_DETECT_SW_EN	BIT(26)
	#define SAR_ADC_DETECT_IDLE_SW_DETECT_MUX_MASK	GENMASK(25, 23)
	#define SAR_ADC_DETECT_IDLE_SW_IDLE_MUX_SEL_MASK GENMASK(9, 7)

#define SAR_ADC_DELTA_10				0x28

#define SAR_ADC_REG11					0x2c
	#define SAR_ADC_REG11_VREF_SEL			BIT(0)
	#define SAR_ADC_REG11_VREF_EN			BIT(5)
	#define SAR_ADC_REG11_CMV_SEL			BIT(6)
	#define SAR_ADC_REG11_BANDGAP_EN		BIT(13)

#define SAR_ADC_REG13					0x34
	#define SAR_ADC_REG13_CALIB_FACTOR_MASK		GENMASK(13, 8)

enum MESON_SARADC_AVG_MODE {
	NO_AVERAGING = 0x0,
	MEAN_AVERAGING = 0x1,
	MEDIAN_AVERAGING = 0x2,
};

enum MESON_SARADC_NUM_SAMPLES {
	ONE_SAMPLE = 0x0,
	TWO_SAMPLES = 0x1,
	FOUR_SAMPLES = 0x2,
	EIGHT_SAMPLES = 0x3,
};

enum MESON_SARADC_RESOLUTION {
	SARADC_10BIT = 10,
	SARADC_12BIT = 12,
};

enum MESON_SARADC_BIT_STATE {
	BIT_LOW = 0,
	BIT_HIGH = 1,
};

/*
 * struct meson_saradc_reg_diff - various information relative to registers
 *
 * @reg3_ring_counter_disable: to disable continuous ring counter.
 * gxl and later: 1; others(gxtvbb etc): 0
 * @reg11_vref_en: g12a and later: 0; others(axg etc): 1
 * @reg11_cmv_sel: g12a and later: 0; others(axg etc): 1
 */
struct meson_saradc_reg_diff {
	bool    reg3_ring_counter_disable;
	bool	reg11_vref_en;
	bool	reg11_cmv_sel;
};

/*
 * struct meson_saradc_data - describe the differences of different platform
 *
 * @has_bl30_integration:
 * @num_channels: the number of adc channels
 * @self_test_channel: channel of self-test
 * @resolution: gxl and later: 12bit; others(gxtvbb etc): 10bit
 * @regs_diff: to describe the differences of the registers
 */
struct meson_saradc_data {
	bool has_bl30_integration;
	unsigned char self_test_channel;
	unsigned char num_channels;
	unsigned int resolution;
	struct meson_saradc_reg_diff regs_diff;
};

struct meson_saradc {
	phys_addr_t base;
	int active_channel;
	struct meson_saradc_data *data;
};

static void meson_saradc_hw_init(struct meson_saradc *priv)
{
	/* create association between logic and physical channel */
	writel(0x03eb1a0c, priv->base + SAR_ADC_AUX_SW);
	writel(0x008c000c, priv->base + SAR_ADC_CHAN_10_SW);

	/* disable all channels by default */
	writel(0x00000000, priv->base + SAR_ADC_CHAN_LIST);

	/* delay between two input/samples = (10 + 1) * 1us */
	writel(0x010a000a, priv->base + SAR_ADC_DELAY);

	/*
	 * BIT[21]:    disable the ADC by default
	 * BIT[23-25]: vdda*3/4 connect to channel-7 by default
	 * BIT[26]:    select the sampling clock period: 0:3T, 1:5T
	 * BIT[27]:    disable ring counter
	 */
	writel(0x0980000a, priv->base + SAR_ADC_REG3);

	/* disable continuous sampling mode */
	clrsetbits_le32(priv->base + SAR_ADC_REG0,
			SAR_ADC_REG0_CONTINUOUS_EN, 0);

	/* clock initialization: 1.2M=24M/(0x13 + 1) */
	clrsetbits_le32(SAR_CLK_BASE, SAR_CLK_DIV_MASK,
			0x13 << SAR_CLK_DIV_SHIFT);
}

static void meson_saradc_hw_enable(struct meson_saradc *priv)
{
	clrsetbits_le32(priv->base + SAR_ADC_REG11,
			SAR_ADC_REG11_CMV_SEL |
			SAR_ADC_REG11_VREF_EN |
			SAR_ADC_REG11_BANDGAP_EN,
			(priv->data->regs_diff.reg11_cmv_sel ?
				SAR_ADC_REG11_CMV_SEL : 0) |
			(priv->data->regs_diff.reg11_vref_en ?
				SAR_ADC_REG11_VREF_EN : 0) |
			SAR_ADC_REG11_BANDGAP_EN);

	clrsetbits_le32(SAR_CLK_BASE, SAR_CLK_GATE, SAR_CLK_GATE);

	clrsetbits_le32(priv->base + SAR_ADC_REG0,
			SAR_ADC_REG0_SAMPLE_ENGINE_ENABLE,
			SAR_ADC_REG0_SAMPLE_ENGINE_ENABLE);

	clrsetbits_le32(priv->base + SAR_ADC_REG3,
			SAR_ADC_REG3_ADC_EN,
			SAR_ADC_REG3_ADC_EN);
}

static void meson_saradc_hw_disable(struct meson_saradc *priv)
{
	clrsetbits_le32(priv->base + SAR_ADC_REG3,
			SAR_ADC_REG3_ADC_EN, 0);

	clrsetbits_le32(priv->base + SAR_ADC_REG0,
			SAR_ADC_REG0_SAMPLE_ENGINE_ENABLE, 0);

	clrsetbits_le32(SAR_CLK_BASE, SAR_CLK_GATE, 0);
}

static inline int meson_saradc_get_race_flag(struct meson_saradc *priv)
{
	int val;
	int timeout = 10000;

	/* wait until BL30 releases it's lock (so we can use
	 * the SAR ADC)
	 */
	if (priv->data->has_bl30_integration)
	{
		do {
			udelay(1);
			val = readl(priv->base + SAR_ADC_DELAY);
		} while ((val & SAR_ADC_DELAY_BL30_BUSY) && timeout--);

		if (timeout < 0)
			return -ETIMEDOUT;

		/* prevent BL30 from using the SAR ADC while we are using it */
		clrsetbits_le32(priv->base + SAR_ADC_DELAY,
				SAR_ADC_DELAY_KERNEL_BUSY,
				SAR_ADC_DELAY_KERNEL_BUSY);
		udelay(1);

		val = readl(priv->base + SAR_ADC_DELAY);
		if (val & SAR_ADC_DELAY_BL30_BUSY) {
			clrsetbits_le32(priv->base + SAR_ADC_DELAY,
					SAR_ADC_DELAY_KERNEL_BUSY, 0);
			return -ETIMEDOUT;
		}
	}

	return 0;
}

static inline void meson_saradc_put_race_flag(struct meson_saradc *priv)
{
	if (priv->data->has_bl30_integration)
		clrsetbits_le32(priv->base + SAR_ADC_DELAY,
				SAR_ADC_DELAY_KERNEL_BUSY, 0);
}

static inline void meson_saradc_enable_channel(struct meson_saradc *priv,
				int ch, int idx)
{
	clrsetbits_le32(priv->base + SAR_ADC_CHAN_LIST,
			SAR_ADC_CHAN_LIST_MAX_INDEX_MASK,
			idx << SAR_ADC_CHAN_LIST_MAX_INDEX_SHIFT);

	clrsetbits_le32(priv->base + SAR_ADC_CHAN_LIST,
			SAR_ADC_CHAN_LIST_ENTRY_MASK(idx),
			ch << SAR_ADC_CHAN_LIST_ENTRY_SHIFT(idx));

}

static inline void meson_saradc_clear_fifo(struct meson_saradc *priv)
{
	int i;

	for (i = 0; i < 32; i++) {
		if (!((readl(priv->base + SAR_ADC_REG0) >>
				SAR_ADC_REG0_FIFO_COUNT_SHIFT) & 0x1f))
			break;
		readl(priv->base + SAR_ADC_FIFO_RD);
	}
}

static inline void meson_saradc_start_sample(struct meson_saradc *priv)
{
	clrsetbits_le32(priv->base + SAR_ADC_REG0,
			SAR_ADC_REG0_SAMPLING_START |
			SAR_ADC_REG0_SAMPLING_STOP,
			SAR_ADC_REG0_SAMPLING_START);
}

static int meson_saradc_set_mode(struct udevice *dev, int ch, unsigned int mode)
{
	struct meson_saradc *priv = dev_get_priv(dev);

	if (mode & ADC_CAPACITY_AVERAGE) {
		clrsetbits_le32(priv->base + SAR_ADC_AVG_CNTL,
			SAR_ADC_AVG_CNTL_NUM_SAMPLES_MASK(ch),
		EIGHT_SAMPLES << SAR_ADC_AVG_CNTL_NUM_SAMPLES_SHIFT(ch));

		clrsetbits_le32(priv->base + SAR_ADC_AVG_CNTL,
			SAR_ADC_AVG_CNTL_AVG_MODE_MASK(ch),
		MEDIAN_AVERAGING << SAR_ADC_AVG_CNTL_AVG_MODE_SHIFT(ch));
	} else {
		clrsetbits_le32(priv->base + SAR_ADC_AVG_CNTL,
			SAR_ADC_AVG_CNTL_AVG_MODE_MASK(ch),
			NO_AVERAGING << SAR_ADC_AVG_CNTL_AVG_MODE_SHIFT(ch));
	}

	/*
	 * the precision of internal voltage is approximately equal to 10mv,
	 * but the VDDA is approximately equal to 36mv(2% x 1800mv).
	 */
	if (mode & ADC_CAPACITY_HIGH_PRECISION_VREF) {
		if (readl(priv->base + SAR_ADC_REG13) &
				SAR_ADC_REG13_CALIB_FACTOR_MASK) {
			/* select the internal voltage as reference voltage */
			clrsetbits_le32(priv->base + SAR_ADC_REG11,
					SAR_ADC_REG11_VREF_SEL, 0);
		} else {
			/* select the VDDA as reference voltage */
			clrsetbits_le32(priv->base + SAR_ADC_REG11,
					SAR_ADC_REG11_VREF_SEL,
					SAR_ADC_REG11_VREF_SEL);

			pr_notice("%s: calib factor is null, \
					please set it in bl30 first\n",
					dev->name);
		}
	} else {
		/* select the VDDA as reference voltage */
		clrsetbits_le32(priv->base + SAR_ADC_REG11,
				SAR_ADC_REG11_VREF_SEL,
				SAR_ADC_REG11_VREF_SEL);
	}

	return 0;
}

static int meson_saradc_start_channel(struct udevice *dev, int channel)
{
	int ret;

	struct meson_saradc *priv = dev_get_priv(dev);

	ret = meson_saradc_get_race_flag(priv);
	if (ret)
		return ret;

	meson_saradc_enable_channel(priv, channel, 0);

	meson_saradc_clear_fifo(priv);

	meson_saradc_start_sample(priv);

	priv->active_channel = channel;

	return 0;
}

static int meson_saradc_stop(struct udevice *dev)
{
	/* current driver only support single sampling mode, the adc
	 * converter will stop automatically when one sampling is
	 * completed, so it is not necessary to stop sampling manually
	 * in current location. However, if the adc module working in
	 * continues sampling mode, we need to write REG0 bit[14] to
	 * stop sampling.
	 */

	return 0;
}

static int meson_saradc_channel_data(struct udevice *dev, int channel,
		unsigned int *data)
{
	int val;
	int fifo_ch;
	struct meson_saradc *priv = dev_get_priv(dev);
	struct adc_uclass_platdata *uc_pdata = dev_get_uclass_platdata(dev);

	if (priv->active_channel != channel) {
		pr_err("%s: requested channel is not active!\n", dev->name);
		return -EINVAL;
	}

	if (readl(priv->base + SAR_ADC_REG0) & SAR_ADC_REG0_BUSY_MASK)
		return -EBUSY;

	val = readl(priv->base + SAR_ADC_FIFO_RD);
	fifo_ch = (val >> SAR_ADC_FIFO_RD_CHAN_ID_SHIFT) & 0x7;

	if (fifo_ch != channel) {
		pr_err("%s: channel mismatch: exp[%d] - act[%d]\n",
				dev->name, channel, fifo_ch);
		return -EINVAL;
	}

	*data = val & uc_pdata->data_mask;

	priv->active_channel = -1;

	meson_saradc_put_race_flag(priv);

	return 0;
}

static int meson_saradc_select_input_voltage(struct udevice *dev, int channel,
			int mux)
{
	struct meson_saradc *priv = dev_get_priv(dev);

	if (channel != priv->data->self_test_channel) {
		pr_err("%s: channel[%d] does not support self-test\n",
				dev->name, channel);
		return -EINVAL;
	}

	clrsetbits_le32(priv->base + SAR_ADC_REG3,
			SAR_ADC_REG3_CTRL_CHAN7_MUX_SEL_MASK,
			(mux & 0x7) << SAR_ADC_REG3_CTRL_CHAN7_MUX_SEL_SHIFT);

	return 0;
}

static const struct adc_ops meson_saradc_ops = {
	.set_mode		= meson_saradc_set_mode,
	.start_channel		= meson_saradc_start_channel,
	.channel_data		= meson_saradc_channel_data,
	.stop			= meson_saradc_stop,
	.select_input_voltage	= meson_saradc_select_input_voltage,
};

static int meson_saradc_probe(struct udevice *dev)
{
	struct meson_saradc *priv = dev_get_priv(dev);

	priv->base = devfdt_get_addr(dev);
	if (priv->base == FDT_ADDR_T_NONE) {
		pr_err("%s: cannot find saradc base address\n", dev->name);
		return -EINVAL;
	}

	priv->active_channel = -1;

	meson_saradc_hw_init(priv);

	meson_saradc_hw_enable(priv);

	return 0;
}

static int meson_saradc_remove(struct udevice *dev)
{
	struct meson_saradc *priv = dev_get_priv(dev);

	meson_saradc_hw_disable(priv);

	return 0;
}

static int meson_saradc_ofdata_to_platdata(struct udevice *dev)
{
	struct adc_uclass_platdata *uc_pdata = dev_get_uclass_platdata(dev);
	struct meson_saradc *priv = dev_get_priv(dev);

	priv->data = (struct meson_saradc_data *)dev_get_driver_data(dev);

	uc_pdata->data_mask = (1 << priv->data->resolution) - 1;
	uc_pdata->data_format = ADC_DATA_FORMAT_BIN;
	uc_pdata->data_timeout_us = MESON_SAR_ADC_TIMEOUT;
	uc_pdata->channel_mask = (1 << priv->data->num_channels) - 1;
	uc_pdata->vdd_microvolts = MESON_SAR_ADC_VDDA_VOLTAGE;

	return 0;
}

static struct meson_saradc_data meson_saradc_g12a_data = {
	.has_bl30_integration	= true,
	.self_test_channel	= SARADC_CH_SELF_TEST,
	.num_channels		= MESON_SARADC_CH_MAX,
	.resolution		= SARADC_12BIT,
	.regs_diff		= {
		.reg3_ring_counter_disable = BIT_HIGH,
		.reg11_vref_en	= BIT_LOW,
		.reg11_cmv_sel	= BIT_LOW,
	},
};

static const struct udevice_id meson_saradc_ids[] = {
	{
		.compatible = "amlogic,meson-g12a-saradc",
		.data = (ulong)&meson_saradc_g12a_data,
	},
	{ }
};

U_BOOT_DRIVER(meson_saradc) = {
	.name			= "meson_saradc",
	.id			= UCLASS_ADC,
	.of_match		= meson_saradc_ids,
	.ops			= &meson_saradc_ops,
	.probe			= meson_saradc_probe,
	.remove			= meson_saradc_remove,
	.ofdata_to_platdata	= meson_saradc_ofdata_to_platdata,
	.priv_auto_alloc_size	= sizeof(struct meson_saradc),
};
