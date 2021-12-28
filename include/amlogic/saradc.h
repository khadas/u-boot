/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __MESON_SARADC_H__
#define __MESON_SARADC_H__

#include <common.h>
#include <adc.h>
#include <clk.h>

enum ADC_CHANNEL_TYPE {
	MESON_SARADC_CH0 = 0,
	MESON_SARADC_CH1,
	MESON_SARADC_CH2,
	MESON_SARADC_CH3,
	MESON_SARADC_CH4,
	MESON_SARADC_CH5,
	MESON_SARADC_CH6,
	MESON_SARADC_CH7,
	MESON_SARADC_CH_MAX,
};

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
	SARADC_22BIT = 22,
};

enum MESON_SARADC_BIT_STATE {
	BIT_LOW = 0,
	BIT_HIGH = 1,
};

struct meson_saradc;

struct meson_saradc_diff_ops {
	void (*extra_init)(struct meson_saradc *priv);
	void (*set_ch7_mux)(struct meson_saradc *priv, int ch, int mux);
	void (*enable_decim_filter)(struct meson_saradc *priv,
				    int ch, unsigned int mode);
	void (*set_ref_voltage)(struct meson_saradc *priv, unsigned int mode,
				int ch);
	int (*get_fifo_channel)(int val);
	int (*get_fifo_data)(struct meson_saradc *priv,
			     struct adc_uclass_platdata *uc_pdata, int val);
};

/*
 * struct meson_saradc_data - describe the differences of different platform
 *
 * @reg3_ring_counter_disable: to disable continuous ring counter.
 * gxl and later: 1; others(gxtvbb etc): 0
 * @reg11_vref_en: g12a and later: 0; others(axg etc): 1
 * @reg11_cmv_sel: g12a and later: 0; others(axg etc): 1
 * @reg11_eoc:     g12a and later: 1; others(axg etc): 0
 * @has_bl30_integration:
 * @update_vref_conf: only for C2 & A5; C2: 0; A5: 1
 * @num_channels: the number of adc channels
 * @self_test_channel: channel of self-test
 * @resolution: gxl and later: 12bit; others(gxtvbb etc): 10bit
 * @clock_rate: saradc clock rate
 */
struct meson_saradc_data {
	bool reg3_ring_counter_disable;
	bool reg11_vref_en;
	bool reg11_cmv_sel;
	bool reg11_eoc;
	bool has_bl30_integration;
	bool update_vref_conf;
	unsigned char self_test_channel;
	unsigned char num_channels;
	unsigned int resolution;
	const struct meson_saradc_diff_ops *dops;
	unsigned int capacity;
	unsigned long clock_rate;
};

struct meson_saradc {
	phys_addr_t base;
	int active_channel;
	unsigned int current_mode;
	struct clk xtal;
	struct clk adc_mux;
	struct clk adc_div;
	struct clk adc_gate;
	struct meson_saradc_data *data;
};

extern const struct adc_ops meson_saradc_ops;
int meson_saradc_probe(struct udevice *dev);
int meson_saradc_remove(struct udevice *dev);
int meson_saradc_ofdata_to_platdata(struct udevice *dev);

#define SARADC_CH_SELF_TEST	MESON_SARADC_CH7

#endif /*_MESON_SARADC_H_*/
