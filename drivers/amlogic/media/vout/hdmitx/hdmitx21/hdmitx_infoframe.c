// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <amlogic/media/vout/hdmitx21/hdmitx.h>
#include "hdmitx_drv.h"

/* now this interface should be not used, otherwise need
 * adjust as hdmi_vend_infoframe_rawset fistly
 */
void hdmi_vend_infoframe_set(struct hdmi_vendor_infoframe *info)
{
	u8 body[31] = {0};

	if (!info) {
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR, NULL);
		return;
	}

	hdmi_vendor_infoframe_pack(info, body, sizeof(body));
	hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR, body);
}

/* refer to DV Consumer Decoder for Source Devices
 * System Development Guide Kit version chapter 4.4.8 Game
 * content signaling:
 * 1.if DV sink device that supports ALLM with
 * InfoFrame Data Block (IFDB), HF-VSIF with ALLM_Mode = 1
 * should comes after Dolby VSIF with L11_MD_Present = 1 and
 * Content_Type[3:0] = 0x2(case B1)
 * 2. DV sink device that supports ALLM without
 * InfoFrame Data Block (IFDB), Dolby VSIF with L11_MD_Present
 * = 1 and Content_Type[3:0] = 0x2 should comes after HF-VSIF
 * with  ALLM_Mode = 1(case B2), or should only send Dolby VSIF,
 * not send HF-VSIF(case A)
 */
/* only used for DV_VSIF / HDMI1.4b_VSIF / HDR10+ VSIF */
void hdmi_vend_infoframe_rawset(u8 *hb, u8 *pb)
{
	u8 body[31] = {0};
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	if (!hb || !pb) {
		if (!hdev->RXCap.ifdb_present) {
			hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR2, NULL);
			printf("%s clear vendor2\n", __func__);
		} else {
			hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR, NULL);
			printf("%s clear vendor\n", __func__);
		}
		return;
	}

	memcpy(body, hb, 3);
	memcpy(&body[3], pb, 28);
	if (hdev->RXCap.ifdb_present && hdev->RXCap.additional_vsif_num >= 1) {
		/* dolby cts case93 B1 */
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR, body);
		printf("%s set vendor\n", __func__);
	} else if (!hdev->RXCap.ifdb_present) {
		/* dolby cts case92 B2 */
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR2, body);
		printf("%s set vendor2\n", __func__);
	} else {
		/* case89 ifdb_present but no additional_vsif, should not send HF-VSIF */
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR2, NULL);
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR, body);
		printf("%s clear vendor2, set vendor\n", __func__);
	}
}

/* only used for HF-VSIF */
void hdmi_vend_infoframe2_rawset(u8 *hb, u8 *pb)
{
	u8 body[31] = {0};
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	if (!hb || !pb) {
		if (!hdev->RXCap.ifdb_present) {
			hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR, NULL);
			printf("%s clear vendor\n", __func__);
		} else {
			hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR2, NULL);
			printf("%s clear vendor2\n", __func__);
		}
		return;
	}

	memcpy(body, hb, 3);
	memcpy(&body[3], pb, 28);
	if (hdev->RXCap.ifdb_present && hdev->RXCap.additional_vsif_num >= 1) {
		/* dolby cts case93 B1 */
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR2, body);
		printf("%s set vendor2, vsif_num >= 1\n", __func__);
	} else if (!hdev->RXCap.ifdb_present) {
		/* dolby cts case92 B2 */
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR, body);
		printf("%s set vendor\n", __func__);
	} else {
		/* case89 ifdb_present but no additional_vsif, currently
		 * no DV-VSIF enabled, then send HF-VSIF
		 */
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_VENDOR2, body);
		printf("%s set vendor2\n", __func__);
	}
}

void hdmi_avi_infoframe_set(struct hdmi_avi_infoframe *info)
{
	u8 body[31] = {0};

	if (!info) {
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_AVI, NULL);
		return;
	}

	hdmi_avi_infoframe_pack(info, body, sizeof(body));
	hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_AVI, body);
}

void hdmi_avi_infoframe_rawset(u8 *hb, u8 *pb)
{
	u8 body[31] = {0};

	if (!hb || !pb) {
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_AVI, NULL);
		return;
	}

	memcpy(body, hb, 3);
	memcpy(&body[3], pb, 28);
	hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_AVI, body);
}

void hdmi_avi_infoframe_config(enum avi_component_conf conf, u8 val)
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();
	struct hdmi_avi_infoframe *info = &hdev->infoframes.avi.avi;
	struct hdmi_format_para *para = hdev->para;

	switch (conf) {
	case CONF_AVI_CS:
		info->colorspace = val;
		break;
	case CONF_AVI_BT2020:
		if (val == SET_AVI_BT2020) {
			info->colorimetry = HDMI_COLORIMETRY_EXTENDED;
			info->extended_colorimetry = HDMI_EXTENDED_COLORIMETRY_BT2020;
		}
		if (val == CLR_AVI_BT2020) {
			if (para->timing.v_total <= 576) {/* SD formats */
				info->colorimetry = HDMI_COLORIMETRY_ITU_601;
				info->extended_colorimetry = 0;
			} else {
				if (hdev->colormetry) {
					info->colorimetry = HDMI_COLORIMETRY_EXTENDED;
					info->extended_colorimetry =
						HDMI_EXTENDED_COLORIMETRY_BT2020;
				} else {
					info->colorimetry = HDMI_COLORIMETRY_ITU_709;
					info->extended_colorimetry = 0;
				}
			}
		}
		break;
	case CONF_AVI_Q01:
		info->quantization_range = val;
		break;
	case CONF_AVI_YQ01:
		info->ycc_quantization_range = val;
		break;
	case CONF_AVI_VIC:
		info->video_code = val & 0xff;
		break;
	default:
		break;
	}
	hdmi_avi_infoframe_set(info);
}

void hdmi_spd_infoframe_set(struct hdmi_spd_infoframe *info)
{
	u8 body[31] = {0};

	if (!info) {
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_SPD, NULL);
		return;
	}

	hdmi_spd_infoframe_pack(info, body, sizeof(body));
	hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_SPD, body);
}

void hdmi_audio_infoframe_set(struct hdmi_audio_infoframe *info)
{
	u8 body[31] = {0};

	if (!info) {
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_AUDIO, NULL);
		return;
	}

	hdmi_audio_infoframe_pack(info, body, sizeof(body));
	hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_AUDIO, body);
}

void hdmi_audio_infoframe_rawset(u8 *hb, u8 *pb)
{
	u8 body[31] = {0};

	if (!hb || !pb) {
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_AUDIO, NULL);
		return;
	}

	memcpy(body, hb, 3);
	memcpy(&body[3], pb, 28);
	hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_AUDIO, body);
}

void hdmi_drm_infoframe_set(struct hdmi_drm_infoframe *info)
{
	u8 body[31] = {0};

	if (!info) {
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_DRM, NULL);
		return;
	}

	hdmi_drm_infoframe_pack(info, body, sizeof(body));
	hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_DRM, body);
}

void hdmi_drm_infoframe_rawset(u8 *hb, u8 *pb)
{
	u8 body[31] = {0};

	if (!hb || !pb) {
		hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_DRM, NULL);
		return;
	}

	memcpy(body, hb, 3);
	memcpy(&body[3], pb, 28);
	hdmitx21_infoframe_send(HDMI_INFOFRAME_TYPE_DRM, body);
}

/* for only 8bit */
void hdmi_gcppkt_manual_set(bool en)
{
	u8 body[31] = {0};

	body[0] = HDMI_PACKET_TYPE_GCP;
	if (en)
		hdmitx21_infoframe_send(HDMI_PACKET_TYPE_GCP, body);
	else
		hdmitx21_infoframe_send(HDMI_PACKET_TYPE_GCP, NULL);
}

