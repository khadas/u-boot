// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <dm.h>
#include <amlogic/saradc.h>
#include <asm/io.h>

#define SARADC_REG3					0x0c
	#define SARADC_REG3_CTRL_CHAN7_MUX_SEL_MASK	GENMASK(25, 23)
	#define SARADC_REG3_CTRL_CHAN7_MUX_SEL_SHIFT	(23)

#define SARADC_FIFO_RD					0x18
	#define SARADC_FIFO_RD_CHAN_ID_SHIFT		(12)
	#define SARADC_FIFO_RD_CHAN_ID_MASK		GENMASK(14, 12)
	#define SARADC_FIFO_RD_SAMPLE_VALUE_MASK	GENMASK(11, 0)

#define SARADC_REG11					0x2c
	#define SARADC_REG11_VREF_SEL			BIT(0)
	#define SARADC_REG11_EOC			BIT(1)
	#define SARADC_REG11_VREF_EN			BIT(5)
	#define SARADC_REG11_CMV_SEL			BIT(6)
	#define SARADC_REG11_BANDGAP_EN			BIT(13)
	#define SARADC_REG11_TEMP_SEL			BIT(21)

#define SARADC_REG13					0x34
	#define SARADC_REG13_CALIB_FACTOR_MASK		GENMASK(13, 8)

static void meson_g12a_extra_init(struct meson_saradc *priv)
{
	clrsetbits_le32(priv->base + SARADC_REG11,
			SARADC_REG11_CMV_SEL |
			SARADC_REG11_VREF_EN |
			SARADC_REG11_EOC |
			SARADC_REG11_BANDGAP_EN,
			(priv->data->reg11_cmv_sel ?
				SARADC_REG11_CMV_SEL : 0) |
			(priv->data->reg11_vref_en ?
				SARADC_REG11_VREF_EN : 0) |
			(priv->data->reg11_eoc ?
				SARADC_REG11_EOC : 0) |
			SARADC_REG11_BANDGAP_EN);

	/* select channel 6 input from temp sensor to external input */
	clrsetbits_le32(priv->base + SARADC_REG11, SARADC_REG11_TEMP_SEL, 0);
}

static void meson_g12a_set_ref_voltage(struct meson_saradc *priv,
					unsigned int mode, int ch)
{
	if (mode & ADC_CAPACITY_HIGH_PRECISION_VREF) {
		if (readl(priv->base + SARADC_REG13) &
				SARADC_REG13_CALIB_FACTOR_MASK) {
			/* select the internal voltage as reference voltage */
			clrsetbits_le32(priv->base + SARADC_REG11,
					SARADC_REG11_VREF_SEL, 0);
		} else {
			/* select the VDDA as reference voltage */
			clrsetbits_le32(priv->base + SARADC_REG11,
					SARADC_REG11_VREF_SEL,
					SARADC_REG11_VREF_SEL);

			pr_notice("calib factor is null,\
					select the vdda as vref\n");
		}
	} else {
		/* select the VDDA as reference voltage */
		clrsetbits_le32(priv->base + SARADC_REG11,
				SARADC_REG11_VREF_SEL,
				SARADC_REG11_VREF_SEL);
	}


}

static int meson_g12a_get_fifo_channel(int val)
{
	return (val >> SARADC_FIFO_RD_CHAN_ID_SHIFT) & 0x7;
}

static void meson_g12a_set_ch7_mux(struct meson_saradc *priv, int ch, int mux)
{
	clrsetbits_le32(priv->base + SARADC_REG3,
			SARADC_REG3_CTRL_CHAN7_MUX_SEL_MASK,
			(mux & 0x7) << SARADC_REG3_CTRL_CHAN7_MUX_SEL_SHIFT);
}

static int meson_g12a_get_fifo_data(struct meson_saradc *priv,
				struct adc_uclass_platdata *uc_pdata, int val)
{
	unsigned int data;

	data = val & uc_pdata->data_mask;

	/* return the 10-bit sampling value */
	if (priv->data->resolution == SARADC_12BIT)
		data = data >> 2;

	return data;
}

static struct meson_saradc_diff_ops meson_g12a_diff_ops = {
	.extra_init		= meson_g12a_extra_init,
	.set_ref_voltage	= meson_g12a_set_ref_voltage,
	.get_fifo_channel	= meson_g12a_get_fifo_channel,
	.set_ch7_mux		= meson_g12a_set_ch7_mux,
	.get_fifo_data		= meson_g12a_get_fifo_data,
};

struct meson_saradc_data meson_saradc_g12a_data = {
	.reg3_ring_counter_disable = BIT_HIGH,
	.reg11_vref_en		   = BIT_LOW,
	.reg11_cmv_sel		   = BIT_LOW,
	.reg11_eoc		   = BIT_HIGH,
	.has_bl30_integration	   = true,
	.self_test_channel	   = SARADC_CH_SELF_TEST,
	.num_channels		   = MESON_SARADC_CH_MAX,
	.resolution		   = SARADC_12BIT,
	.dops			   = &meson_g12a_diff_ops,
	.capacity		   = ADC_CAPACITY_AVERAGE |
				     ADC_CAPACITY_HIGH_PRECISION_VREF,
	.clock_rate		   = 1200000,
};

static const struct udevice_id meson_g12a_saradc_ids[] = {
	{
		.compatible = "amlogic,meson-g12a-saradc",
		.data = (ulong)&meson_saradc_g12a_data,
	},
	{ }
};

U_BOOT_DRIVER(meson_g12a_saradc) = {
	.name			= "meson_g12a_saradc",
	.id			= UCLASS_ADC,
	.of_match		= meson_g12a_saradc_ids,
	.ops			= &meson_saradc_ops,
	.probe			= meson_saradc_probe,
	.remove			= meson_saradc_remove,
	.ofdata_to_platdata	= meson_saradc_ofdata_to_platdata,
	.priv_auto_alloc_size	= sizeof(struct meson_saradc),
};
