// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2020 Amlogic, Inc. All rights reserved.
 * Author: Xingyu Chen <xingyu.chen@amlogic.com>
 *
 * Meson SARADC driver for U-Boot
 */

#include <dm.h>
#include <amlogic/saradc.h>
#include <asm/io.h>

#define SARADC_C2_DISCARD_DATA_CNT				30
#define SARADC_C2_SAVE_DATA_CNT				1

#define SARADC_C2_FIFO_RD					0x18
	#define SARADC_C2_FIFO_RD_CHAN_ID_SHIFT		(22)
	#define SARADC_C2_FIFO_RD_CHAN_ID_MASK		GENMASK(24, 22)
	#define SARADC_C2_FIFO_RD_SAMPLE_VALUE_MASK	GENMASK(21, 0)

#define SARADC_C2_REG11					0x2c
	#define SARADC_C2_REG11_CALIB_FACTOR_MASK		GENMASK(18, 12)

#define SARADC_C2_REG13					0x34
	#define SARADC_C2_REG13_VREF_SEL			BIT(19)

#define SARADC_C2_CH0_CTRL1				0x4c
	#define SARADC_C2_CH0_CTRL1_CHAN_MUX_SEL_MASK	GENMASK(23, 21)
	#define SARADC_C2_CH0_CTRL1_CHAN_MUX_SEL_SHIFT	(21)
	#define SARADC_C2_CH0_CTRL1_AUX_DIFF_EN		BIT(17)
	#define SARADC_C2_CH0_CTRL1_AUX_MODE_SEL		BIT(0)

#define SARADC_C2_CH0_CTRL2				0x50
#define SARADC_C2_CH0_CTRL3				0x54

/*
 * bit[19] = 0 default; 0=vref_buf, 1=avdd
 * bit[18] = 0 default; 0=vref_buf, 1=vcm_0p9
 * bit[17] = 1 default; 1=differential; 0=single-ended
 * bit[16] = 0 default; 0=vbg, 1=vcm_0p9
 * bit[0]  = 0 default; 0=new continuous mode; 1=normal mode
 */
static void meson_c2_enable_decim_filter(struct meson_saradc *priv, int ch,
					 unsigned int mode)
{
	int reg_off;

	reg_off = ch * 12;
	clrsetbits_le32(priv->base + SARADC_C2_CH0_CTRL1 + reg_off,
			SARADC_C2_CH0_CTRL1_AUX_DIFF_EN, 0);

	if (mode & ADC_CAPACITY_DECIM_FILTER) {
		clrsetbits_le32(priv->base + SARADC_C2_CH0_CTRL1 + reg_off,
				SARADC_C2_CH0_CTRL1_AUX_MODE_SEL,
				0);
	} else {
		clrsetbits_le32(priv->base + SARADC_C2_CH0_CTRL1 + reg_off,
				SARADC_C2_CH0_CTRL1_AUX_MODE_SEL,
				SARADC_C2_CH0_CTRL1_AUX_MODE_SEL);
	}

	writel(SARADC_C2_DISCARD_DATA_CNT, priv->base + SARADC_C2_CH0_CTRL2 + reg_off);
	writel(SARADC_C2_SAVE_DATA_CNT, priv->base + SARADC_C2_CH0_CTRL3 + reg_off);
}

static void meson_c2_set_ref_voltage(struct meson_saradc *priv,
					unsigned int mode)
{
	if (mode & ADC_CAPACITY_HIGH_PRECISION_VREF) {
		if (readl(priv->base + SARADC_C2_REG11) &
					SARADC_C2_REG11_CALIB_FACTOR_MASK) {
			clrsetbits_le32(priv->base + SARADC_C2_REG13,
					SARADC_C2_REG13_VREF_SEL,
					SARADC_C2_REG13_VREF_SEL);
		} else {
			clrsetbits_le32(priv->base + SARADC_C2_REG13,
					SARADC_C2_REG13_VREF_SEL,
					0);
			pr_notice("calib factor is null,\
					select the vdda as vref\n");
		}
	} else {
		clrsetbits_le32(priv->base + SARADC_C2_REG13,
				SARADC_C2_REG13_VREF_SEL,
				SARADC_C2_REG13_VREF_SEL);
	}
}

static void meson_c2_set_ch7_mux(struct meson_saradc *priv, int ch, int mux)
{
	clrsetbits_le32(priv->base + SARADC_C2_CH0_CTRL1 + ch * 12,
			SARADC_C2_CH0_CTRL1_CHAN_MUX_SEL_MASK,
			(mux & 0x7) << SARADC_C2_CH0_CTRL1_CHAN_MUX_SEL_SHIFT);
}

static int meson_c2_get_fifo_channel(int val)
{
	return (val >> SARADC_C2_FIFO_RD_CHAN_ID_SHIFT) & 0x7;
}

static int meson_c2_get_fifo_data(struct meson_saradc *priv,
				struct adc_uclass_platdata *uc_pdata, int val)
{
	unsigned int data;

	if (priv->current_mode & ADC_CAPACITY_DECIM_FILTER) {
		/* return the 22-bit sampling value */
		data = val & ((1 << SARADC_22BIT) - 1);

		//check whether data is negative
		if ((data >> (SARADC_22BIT - 1)) == 1)
			data = data - (1 << SARADC_22BIT);
		//convert to unsigned int
		data = (data + (1 << (SARADC_22BIT - 1)));
	} else {
		/* return the 10-bit sampling value */
		data = val & uc_pdata->data_mask;
		if (priv->data->resolution == SARADC_12BIT)
			data = data >> 2;
	}

	return data;
}

static struct meson_saradc_diff_ops meson_c2_diff_ops = {
	.enable_decim_filter	= meson_c2_enable_decim_filter,
	.set_ref_voltage	= meson_c2_set_ref_voltage,
	.get_fifo_channel	= meson_c2_get_fifo_channel,
	.set_ch7_mux		= meson_c2_set_ch7_mux,
	.get_fifo_data		= meson_c2_get_fifo_data,
};

struct meson_saradc_data meson_saradc_c2_data = {
	.has_bl30_integration	   = true,
	.self_test_channel	   = SARADC_CH_SELF_TEST,
	.num_channels		   = MESON_SARADC_CH_MAX,
	.resolution		   = SARADC_12BIT,
	.dops			   = &meson_c2_diff_ops,
	.capacity		   = ADC_CAPACITY_AVERAGE |
				     ADC_CAPACITY_HIGH_PRECISION_VREF |
				     ADC_CAPACITY_DECIM_FILTER,
	/* There are 2 reasons for showing down saradc clock in C2:
	 * 1. To save cost, there is no input buffer before saradc.
	 * The value of inside resister of input source can not be too high.
	 * And resister value which can be tolerant is inversely of
	 * frequency of saradc clock.
	 * 2. The drive capability of channel 7 internal input is too weak.
	 */
	.clock_rate		   = 600000,
};

static const struct udevice_id meson_c2_saradc_ids[] = {
	{
		.compatible = "amlogic,meson-c2-saradc",
		.data = (ulong)&meson_saradc_c2_data,
	},
	{ }
};

U_BOOT_DRIVER(meson_c2_saradc) = {
	.name			= "meson_c2_saradc",
	.id			= UCLASS_ADC,
	.of_match		= meson_c2_saradc_ids,
	.ops			= &meson_saradc_ops,
	.probe			= meson_saradc_probe,
	.remove			= meson_saradc_remove,
	.ofdata_to_platdata	= meson_saradc_ofdata_to_platdata,
	.priv_auto_alloc_size	= sizeof(struct meson_saradc),
};
