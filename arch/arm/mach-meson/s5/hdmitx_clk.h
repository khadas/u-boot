/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __HDMITX_COMMON_H__
#define __HDMITX_COMMON_H__

#include <common.h>
#include <amlogic/media/vout/hdmitx/hdmitx.h>

#define VID_PLL_DIV_1      0
#define VID_PLL_DIV_2      1
#define VID_PLL_DIV_3      2
#define VID_PLL_DIV_3p5    3
#define VID_PLL_DIV_3p75   4
#define VID_PLL_DIV_4      5
#define VID_PLL_DIV_5      6
#define VID_PLL_DIV_6      7
#define VID_PLL_DIV_6p25   8
#define VID_PLL_DIV_7      9
#define VID_PLL_DIV_7p5    10
#define VID_PLL_DIV_12     11
#define VID_PLL_DIV_14     12
#define VID_PLL_DIV_15     13
#define VID_PLL_DIV_2p5    14
#define VID_PLL_DIV_3p25   15

#define GROUP_MAX       10
#define GROUP_END       -1
struct hw_enc_clk_val_group {
	enum hdmi_vic group[GROUP_MAX];
	u32 hpll_clk_out; /* Unit: kHz */
	u32 od1;
	u32 od2; /* HDMI_CLK_TODIG */
	u32 od3;
	u32 vid_pll_div;
	u32 vid_clk_div;
	u32 enc_div;
	u32 fe_div;
	u32 pnx_div;
	u32 pixel_div;
};
#endif
