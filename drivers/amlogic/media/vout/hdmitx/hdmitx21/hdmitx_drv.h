/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __HDMITX_DRV_H__
#define __HDMITX_DRV_H__

#include <amlogic/media/vout/hdmitx21/hdmitx.h>

const struct hdmi_timing *get_hdmitx_timing_para0(void);
const struct hdmi_timing *hdmitx21_gettiming_from_name(const char *name);
int get_hdmitx_timing_size(void);

void set_vmode_enc_hw(enum hdmi_vic vic);
void dump_infoframe_packets(void);

void hdmi_gcppkt_manual_set(bool en);
void set_tv_enci_new(struct hdmitx_dev *hdev, u32 enc_index,
	enum hdmi_vic vic, u32 enable);
void set_tv_encp_new(struct hdmitx_dev *hdev, u32 enc_index,
	enum hdmi_vic vic, u32 enable);

void hdmitx_dfm_cfg(u8 bw_type, u16 val);
bool frl_check_full_bw(enum hdmi_colorspace cs, enum hdmi_color_depth cd, u32 pixel_clock,
	u32 h_active, enum frl_rate_enum frl_rate, u32 *tri_bytes);
void frl_tx_stop(struct hdmitx_dev *hdev);
bool scdc_tx_frl_cfg1_set(u8 cfg1);
u8 scdc_tx_update_flags_get(void);
bool scdc_tx_update_flags_set(u8 update_flags);

#endif
