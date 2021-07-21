/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __HDMITX_DRV_H__
#define __HDMITX_DRV_H__

#include <amlogic/media/vout/hdmitx21/hdmitx.h>

const struct hdmi_timing *get_hdmitx_timing_para0(void);
int get_hdmitx_timing_size(void);

void set_vmode_enc_hw(enum hdmi_vic vic);

#endif
