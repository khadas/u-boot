// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <amlogic/media/vout/hdmitx21/hdmitx.h>
#include "hdmitx_drv.h"

static void tpi_info_send(u8 sel, u8 *data)
{
	u8 checksum = 0;
	int i;

	hdmitx21_wr_reg(TPI_INFO_FSEL_IVCTX, sel); // buf selection
	if (!data) {
		hdmitx21_wr_reg(TPI_INFO_EN_IVCTX, 0);
		return;
	}

	/* do checksum */
	data[3] = 0;
	for (i = 0; i < 31; i++)
		checksum += data[i];
	checksum = 0 - checksum;
	data[3] = checksum;

	for (i = 0; i < 31; i++)
		hdmitx21_wr_reg(TPI_INFO_B0_IVCTX + i, data[i]);
	hdmitx21_wr_reg(TPI_INFO_EN_IVCTX, 0xe0); //TPI Info enable
}

static int tpi_info_get(u8 sel, u8 *data)
{
	int i;

	hdmitx21_wr_reg(TPI_INFO_FSEL_IVCTX, sel); // buf selection
	if (!data)
		return -1;
	if (hdmitx21_rd_reg(TPI_INFO_EN_IVCTX) == 0)
		return 0;
	for (i = 0; i < 31; i++)
		data[i] = hdmitx21_rd_reg(TPI_INFO_B0_IVCTX + i);
	return 31; /* fixed value */
}

static int _tpi_infoframe_wrrd(u8 wr, u8 info_type, u8 *body)
{
	u8 sel;

	switch (info_type) {
	case HDMI_INFOFRAME_TYPE_AVI:
		sel = 0;
		break;
	case HDMI_INFOFRAME_TYPE_VENDOR:
		sel = 1;
		break;
	case HDMI_INFOFRAME_TYPE_DRM:
		sel = 2;
		break;
	case HDMI_INFOFRAME_TYPE_AUDIO:
		sel = 3;
		break;
	case HDMI_INFOFRAME_TYPE_SPD:
		sel = 16;
		break;
	default:
		pr_info("%s[%d] wrong info_type %d\n", __func__, __LINE__, info_type);
		return -1;
	}

	if (wr) {
		if (!body) {
			tpi_info_send(sel, NULL);
			return 0;
		}
		/* do checksum */
		tpi_info_send(sel, body);
		return 0;
	} else {
		return tpi_info_get(sel, body);
	}
}

void hdmitx21_infoframe_send(u8 info_type, u8 *body)
{
	_tpi_infoframe_wrrd(1, info_type, body);
}

int hdmitx21_infoframe_rawget(u8 info_type, u8 *body)
{
	return _tpi_infoframe_wrrd(0, info_type, body);
}
