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
#include <amlogic/saradc.h>
#include <clk.h>
#include <asm/arch/secure_apb.h>

#define MESON_SARADC_VDDA_VOLTAGE			(1800000)
/* C2/A5 need more time to sample while decim filter is enable */
#define MESON_SARADC_TIMEOUT				(120 * 1000)
#define SARADC_MAX_FIFO_SIZE				16

#define SARADC_REG0					0x00
	#define SARADC_REG0_BUSY_MASK			GENMASK(30, 28)
	#define SARADC_REG0_DELTA_BUSY                 BIT(30)
	#define SARADC_REG0_AVG_BUSY			BIT(29)
	#define SARADC_REG0_SAMPLE_BUSY		BIT(28)
	#define SARADC_REG0_FIFO_FULL			BIT(27)
	#define SARADC_REG0_FIFO_EMPTY			BIT(26)
	#define SARADC_REG0_FIFO_COUNT_SHIFT		(21)
	#define SARADC_REG0_FIFO_COUNT_MASK		GENMASK(25, 21)
	#define SARADC_REG0_CURR_CHAN_ID_MASK		GENMASK(18, 16)
	#define SARADC_REG0_SAMPLING_STOP		BIT(14)
	#define SARADC_REG0_FIFO_CNT_IRQ_MASK          GENMASK(8, 4)
	#define SARADC_REG0_FIFO_CNT_IRQ_SHIFT		(4)
	#define SARADC_REG0_FIFO_IRQ_EN                BIT(3)
	#define SARADC_REG0_SAMPLING_START             BIT(2)
	#define SARADC_REG0_CONTINUOUS_EN              BIT(1)
	#define SARADC_REG0_SAMPLE_ENGINE_ENABLE       BIT(0)

#define SARADC_CHAN_LIST				0x04
	#define SARADC_CHAN_LIST_MAX_INDEX_SHIFT	(24)
	#define SARADC_CHAN_LIST_MAX_INDEX_MASK	GENMASK(26, 24)
	#define SARADC_CHAN_LIST_ENTRY_SHIFT(_chan)	(_chan * 3)
	#define SARADC_CHAN_LIST_ENTRY_MASK(_chan)	\
					(GENMASK(2, 0) << ((_chan) * 3))

#define SARADC_AVG_CNTL				0x08
	#define SARADC_AVG_CNTL_AVG_MODE_SHIFT(_chan)	\
					(16 + ((_chan) * 2))
	#define SARADC_AVG_CNTL_AVG_MODE_MASK(_chan)	\
					(GENMASK(17, 16) << ((_chan) * 2))
	#define SARADC_AVG_CNTL_NUM_SAMPLES_SHIFT(_chan)  \
					(0 + ((_chan) * 2))
	#define SARADC_AVG_CNTL_NUM_SAMPLES_MASK(_chan)  \
					(GENMASK(1, 0) << ((_chan) * 2))

#define SARADC_REG3					0x0c
	#define SARADC_REG3_CTRL_CONT_RING_COUNTER_EN	BIT(27)
	#define SARADC_REG3_CTRL_SAMPLING_CLOCK_PHASE	BIT(26)
	#define SARADC_REG3_ADC_EN			BIT(21)
	#define SARADC_REG3_BLOCK_DLY_SEL_MASK		GENMASK(9, 8)
	#define SARADC_REG3_BLOCK_DLY_MASK		GENMASK(7, 0)

#define SARADC_DELAY					0x10
	#define SARADC_DELAY_INPUT_DLY_SEL_MASK	GENMASK(25, 24)
	#define SARADC_DELAY_INPUT_DLY_CNT_MASK	GENMASK(23, 16)
	#define SARADC_DELAY_BL30_BUSY			BIT(15)
	#define SARADC_DELAY_KERNEL_BUSY		BIT(14)
	#define SARADC_DELAY_SAMPLE_DLY_SEL_MASK	GENMASK(9, 8)
	#define SARADC_DELAY_SAMPLE_DLY_CNT_MASK	GENMASK(7, 0)

#define SARADC_LAST_RD					0x14

#define SARADC_FIFO_RD					0x18

#define SARADC_AUX_SW					0x1c
#define SARADC_AUX_SW_MUX_SEL_CHAN_MASK(_chan)		\
					(GENMASK(10, 8) << (((_chan) - 2) * 3))

#define SARADC_CHAN_10_SW				0x20
	#define SARADC_CHAN_10_SW_CHAN1_MUX_SEL_MASK	GENMASK(25, 23)
	#define SARADC_CHAN_10_SW_CHAN0_MUX_SEL_MASK	GENMASK(9, 7)

#define SARADC_DETECT_IDLE_SW				0x24
	#define SARADC_DETECT_IDLE_SW_DETECT_SW_EN	BIT(26)
	#define SARADC_DETECT_IDLE_SW_DETECT_MUX_MASK	GENMASK(25, 23)
	#define SARADC_DETECT_IDLE_SW_IDLE_MUX_SEL_MASK GENMASK(9, 7)

#define SARADC_DELTA_10				0x28

static int meson_saradc_clk_init(struct udevice *dev)
{
	struct meson_saradc *priv = dev_get_priv(dev);
	int ret;

	ret = clk_get_by_name(dev, "xtal", &priv->xtal);
	if (ret) {
		pr_err("%s: failed to get xtal clk\n", dev->name);
		return ret;
	}

	ret = clk_get_by_name(dev, "adc_mux", &priv->adc_mux);
	if (ret) {
		pr_err("%s: failed to get adc_mux clk\n", dev->name);
		return ret;
	}

	ret = clk_get_by_name(dev, "adc_div", &priv->adc_div);
	if (ret) {
		pr_err("%s: failed to get adc_div clk\n", dev->name);
		return ret;
	}

	ret = clk_get_by_name(dev, "adc_gate", &priv->adc_gate);
	if (ret) {
		pr_err("%s: failed to get adc_gate\n", dev->name);
		return ret;
	}

	ret = clk_set_parent(&priv->adc_mux, &priv->xtal);
	if (ret) {
		pr_err("%s: failed to reparent adc clk\n", dev->name);
		return ret;
	}

	ret = clk_set_rate(&priv->adc_div, priv->data->clock_rate);
	if (ret) {
		pr_err("%s: failed to set rate\n", dev->name);
		return ret;
	}

	return 0;
}

static void meson_saradc_hw_init(struct meson_saradc *priv)
{
	/* create association between logic and physical channel */
	writel(0x03eb1a0c, priv->base + SARADC_AUX_SW);
	writel(0x008c000c, priv->base + SARADC_CHAN_10_SW);

	/* disable all channels by default */
	writel(0x00000000, priv->base + SARADC_CHAN_LIST);

	/* delay between two input/samples = (10 + 1) * 1us */
	writel(0x010a000a, priv->base + SARADC_DELAY);

	/*
	 * BIT[21]:    disable the ADC by default
	 * BIT[23-25]: vdda*3/4 connect to channel-7 by default
	 * BIT[26]:    select the sampling clock period: 0:3T, 1:5T
	 * BIT[27]:    disable ring counter
	 * BIT[31]:    use the clk domain to add delay to
	 *             the start convert signal for hold times
	 */
	writel(0x8980000a, priv->base + SARADC_REG3);

	/* disable continuous sampling mode */
	clrsetbits_le32(priv->base + SARADC_REG0,
			SARADC_REG0_CONTINUOUS_EN, 0);
}

static void meson_saradc_hw_enable(struct meson_saradc *priv)
{
	if (priv->data->dops->extra_init)
		priv->data->dops->extra_init(priv);

	clk_enable(&priv->adc_gate);

	clrsetbits_le32(priv->base + SARADC_REG0,
			SARADC_REG0_SAMPLE_ENGINE_ENABLE,
			SARADC_REG0_SAMPLE_ENGINE_ENABLE);

	clrsetbits_le32(priv->base + SARADC_REG3,
			SARADC_REG3_ADC_EN,
			SARADC_REG3_ADC_EN);
}

static void meson_saradc_hw_disable(struct meson_saradc *priv)
{
	clrsetbits_le32(priv->base + SARADC_REG3,
			SARADC_REG3_ADC_EN, 0);

	clrsetbits_le32(priv->base + SARADC_REG0,
			SARADC_REG0_SAMPLE_ENGINE_ENABLE, 0);

	clk_disable(&priv->adc_gate);
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
			val = readl(priv->base + SARADC_DELAY);
		} while ((val & SARADC_DELAY_BL30_BUSY) && timeout--);

		if (timeout < 0)
			return -ETIMEDOUT;

		/* prevent BL30 from using the SAR ADC while we are using it */
		clrsetbits_le32(priv->base + SARADC_DELAY,
				SARADC_DELAY_KERNEL_BUSY,
				SARADC_DELAY_KERNEL_BUSY);
		udelay(1);

		val = readl(priv->base + SARADC_DELAY);
		if (val & SARADC_DELAY_BL30_BUSY) {
			clrsetbits_le32(priv->base + SARADC_DELAY,
					SARADC_DELAY_KERNEL_BUSY, 0);
			return -ETIMEDOUT;
		}
	}

	return 0;
}

static inline void meson_saradc_put_race_flag(struct meson_saradc *priv)
{
	if (priv->data->has_bl30_integration)
		clrsetbits_le32(priv->base + SARADC_DELAY,
				SARADC_DELAY_KERNEL_BUSY, 0);
}

static inline void meson_saradc_enable_channel(struct meson_saradc *priv,
				int ch, int idx)
{
	clrsetbits_le32(priv->base + SARADC_CHAN_LIST,
			SARADC_CHAN_LIST_MAX_INDEX_MASK,
			idx << SARADC_CHAN_LIST_MAX_INDEX_SHIFT);

	clrsetbits_le32(priv->base + SARADC_CHAN_LIST,
			SARADC_CHAN_LIST_ENTRY_MASK(idx),
			ch << SARADC_CHAN_LIST_ENTRY_SHIFT(idx));

}

static inline void meson_saradc_clear_fifo(struct meson_saradc *priv)
{
	int i;

	for (i = 0; i < 32; i++) {
		if (!((readl(priv->base + SARADC_REG0) >>
				SARADC_REG0_FIFO_COUNT_SHIFT) & 0x1f))
			break;
		readl(priv->base + SARADC_FIFO_RD);
	}
}

static inline void meson_saradc_start_sample(struct meson_saradc *priv)
{
	clrsetbits_le32(priv->base + SARADC_REG0,
			SARADC_REG0_SAMPLING_START |
			SARADC_REG0_SAMPLING_STOP,
			SARADC_REG0_SAMPLING_START);
}

static int meson_saradc_check_mode(struct meson_saradc *priv, unsigned int mode)
{
	if (mode & ~priv->data->capacity)
		return -EINVAL;

	return 0;
}

static int meson_saradc_set_mode(struct udevice *dev, int ch, unsigned int mode)
{
	int ret;
	struct meson_saradc *priv = dev_get_priv(dev);

	ret = meson_saradc_check_mode(priv, mode);
	if (ret < 0)
		return ret;

	if (mode & ADC_CAPACITY_AVERAGE) {
		clrsetbits_le32(priv->base + SARADC_AVG_CNTL,
			SARADC_AVG_CNTL_NUM_SAMPLES_MASK(ch),
		EIGHT_SAMPLES << SARADC_AVG_CNTL_NUM_SAMPLES_SHIFT(ch));

		clrsetbits_le32(priv->base + SARADC_AVG_CNTL,
			SARADC_AVG_CNTL_AVG_MODE_MASK(ch),
		MEDIAN_AVERAGING << SARADC_AVG_CNTL_AVG_MODE_SHIFT(ch));
	} else {
		clrsetbits_le32(priv->base + SARADC_AVG_CNTL,
			SARADC_AVG_CNTL_AVG_MODE_MASK(ch),
			NO_AVERAGING << SARADC_AVG_CNTL_AVG_MODE_SHIFT(ch));
	}

	priv->data->dops->set_ref_voltage(priv, mode, ch);

	priv->current_mode = mode;

	if (priv->data->dops->enable_decim_filter)
		priv->data->dops->enable_decim_filter(priv, ch,
							priv->current_mode);

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

	if (readl(priv->base + SARADC_REG0) & SARADC_REG0_BUSY_MASK)
		return -EBUSY;

	val = readl(priv->base + SARADC_FIFO_RD);

	fifo_ch = priv->data->dops->get_fifo_channel(val);

	if (fifo_ch != channel) {
		pr_err("%s: channel mismatch: exp[%d] - act[%d]\n",
				dev->name, channel, fifo_ch);
		return -EINVAL;
	}

	*data = priv->data->dops->get_fifo_data(priv, uc_pdata, val);

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

	priv->data->dops->set_ch7_mux(priv, channel, mux);

	return 0;
}

const struct adc_ops meson_saradc_ops = {
	.set_mode		= meson_saradc_set_mode,
	.start_channel		= meson_saradc_start_channel,
	.channel_data		= meson_saradc_channel_data,
	.stop			= meson_saradc_stop,
	.select_input_voltage	= meson_saradc_select_input_voltage,
};

int meson_saradc_probe(struct udevice *dev)
{
	struct meson_saradc *priv = dev_get_priv(dev);
	int ret;

	priv->base = devfdt_get_addr(dev);
	if (priv->base == FDT_ADDR_T_NONE) {
		pr_err("%s: cannot find saradc base address\n", dev->name);
		return -EINVAL;
	}

	ret = meson_saradc_clk_init(dev);
	if (ret)
		return ret;

	priv->active_channel = -1;

	meson_saradc_hw_init(priv);

	meson_saradc_hw_enable(priv);

	return 0;
}

int meson_saradc_remove(struct udevice *dev)
{
	struct meson_saradc *priv = dev_get_priv(dev);

	meson_saradc_hw_disable(priv);

	return 0;
}

int meson_saradc_ofdata_to_platdata(struct udevice *dev)
{
	struct adc_uclass_platdata *uc_pdata = dev_get_uclass_platdata(dev);
	struct meson_saradc *priv = dev_get_priv(dev);

	priv->data = (struct meson_saradc_data *)dev_get_driver_data(dev);

	uc_pdata->data_mask = (1 << priv->data->resolution) - 1;
	uc_pdata->data_format = ADC_DATA_FORMAT_BIN;
	uc_pdata->data_timeout_us = MESON_SARADC_TIMEOUT;
	uc_pdata->channel_mask = (1 << priv->data->num_channels) - 1;
	uc_pdata->vdd_microvolts = MESON_SARADC_VDDA_VOLTAGE;

	return 0;
}
