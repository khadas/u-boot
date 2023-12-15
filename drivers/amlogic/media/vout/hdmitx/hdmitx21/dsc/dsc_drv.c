// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2021 Amlogic, Inc. All rights reserved.
 */

#include <linux/types.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <malloc.h>
#include <amlogic/media/vout/dsc.h>
#include "dsc_drv.h"
#include "dsc_reg.h"
#include "dsc_config.h"
#include "dsc_hw.h"
#include "dsc_debug.h"

static struct aml_dsc_drv_s dsc_drv_local;

struct aml_dsc_drv_s *dsc_drv_get(void)
{
	return &dsc_drv_local;
}

/* TODO, add protection only for S5 */
void hdmitx_get_dsc_data(struct dsc_offer_tx_data *dsc_data)
{
	struct aml_dsc_drv_s *dsc_drv = dsc_drv_get();

	if (!dsc_drv || !dsc_data) {
		DSC_ERR("%s not get dsc data\n", __func__);
		return;
	}
	memcpy(&dsc_data->pps_data, &dsc_drv->pps_data, sizeof(dsc_drv->pps_data));
	dsc_data->dsc_mode = dsc_drv->dsc_mode;
	dsc_data->enc0_clk = dsc_drv->enc0_clk;
	dsc_data->cts_hdmi_tx_pixel_clk = dsc_drv->cts_hdmi_tx_pixel_clk;
}

static void dsc_get_notifier_data(struct aml_dsc_drv_s *dsc_drv,
				 struct dsc_notifier_data_s *notifier_data)
{
	if (!(dsc_drv->dsc_debug.manual_set_select & MANUAL_PIC_W_H)) {
		dsc_drv->pps_data.pic_width = notifier_data->pic_width;
		dsc_drv->pps_data.pic_height = notifier_data->pic_height;
	}

	if (!(dsc_drv->dsc_debug.manual_set_select & MANUAL_VIDEO_FPS))
		dsc_drv->fps = notifier_data->fps;

	if (!(dsc_drv->dsc_debug.manual_set_select & MANUAL_BITS_PER_COMPONENT))
		dsc_drv->pps_data.bits_per_component = notifier_data->bits_per_component;

	if (!(dsc_drv->dsc_debug.manual_set_select & MANUAL_COLOR_FORMAT))
		dsc_drv->color_format = notifier_data->color_format;
	DSC_PR("hdmitx notify input: %dx%dp%dhz cs:%d,cd:%d\n",
		dsc_drv->pps_data.pic_width,
		dsc_drv->pps_data.pic_height,
		dsc_drv->fps / 1000,
		dsc_drv->color_format,
		dsc_drv->pps_data.bits_per_component);
}

/* TODO, add protection only for S5 */
int aml_set_dsc_input_param(struct dsc_notifier_data_s *notifier_data)
{
	struct aml_dsc_drv_s *dsc_drv = dsc_drv_get();

	if (!dsc_drv)
		return -1;

	if (!notifier_data) {
		DSC_ERR("%s: notifier_data is null\n", __func__);
		return -1;
	}

	dsc_get_notifier_data(dsc_drv, notifier_data);
	calculate_dsc_enc_data(dsc_drv);

	return 0;
}

/* TODO, add protection only for S5 */
void aml_dsc_enable(bool dsc_en)
{
	struct aml_dsc_drv_s *dsc_drv = dsc_drv_get();

	if (!dsc_drv)
		return;

	if (!dsc_en) {
		set_dsc_en(0);
	} else {
		/* if (dsc_dbg) */
			/* return; */
		dsc_config_register(dsc_drv);
		set_dsc_en(1);
	}
}

