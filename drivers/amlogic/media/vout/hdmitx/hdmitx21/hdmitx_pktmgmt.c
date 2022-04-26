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

/* packet no need checksum */
static void tpi_packet_send(u8 sel, u8 *data)
{
	int i;

	hdmitx21_wr_reg(TPI_INFO_FSEL_IVCTX, sel); // buf selection
	if (!data) {
		hdmitx21_wr_reg(TPI_INFO_EN_IVCTX, 0);
		return;
	}

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

void dump_infoframe_packets(void)
{
	int i, j;
	u8 body[32] = {0};

	for (i = 0; i < 17; i++) {
		tpi_info_get(i, body);
		body[31] = hdmitx21_rd_reg(TPI_INFO_EN_IVCTX);
		printf("dump hdmi infoframe[%d]\n", i);
		for (j = 0; j < 32; j += 8)
			printf("%02x%02x%02x%02x%02x%02x%02x%02x\n",
				body[j + 0], body[j + 1],
				body[j + 2], body[j + 3],
				body[j + 4], body[j + 5],
				body[j + 6], body[j + 7]);
		memset(body, 0, sizeof(body));
	}
}

static int _tpi_infoframe_wrrd(u8 wr, u8 info_type, u8 *body)
{
	u8 sel;

	switch (info_type) {
	case HDMI_INFOFRAME_TYPE_AVI:
		sel = 0;
		break;
	case HDMI_INFOFRAME_TYPE_AUDIO:
		sel = 2;
		break;
	case HDMI_INFOFRAME_TYPE_SPD:
		sel = 3;
		break;
	case HDMI_INFOFRAME_TYPE_VENDOR:
		sel = 5;
		break;
	case HDMI_INFOFRAME_TYPE_DRM:
		sel = 6;
		break;
	case HDMI_PACKET_TYPE_GCP:
		sel = 7;
		tpi_packet_send(sel, body);
		return 0;
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

