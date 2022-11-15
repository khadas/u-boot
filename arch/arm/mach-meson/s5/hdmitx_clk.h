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
#define VID_PLL_DIV_10     16

struct hw_clk_conf {
	enum hdmi_vic vic;
	u32 txpll_vco_clk; /* 3G~6G, Unit: kHz */
	u8  txpll_3_od0; /* div 1, 2, 4, 8 */
	u8  txpll_4_div9_en; /* tx_phy_clk1618 */
	u8  txpll_3_od1; /* div 1, 2, 4, 8 */
	u8  txpll_3_od2; /* div 1, 2, 4, 8, hdmi_clk_out2 */
	u8  txpll_4_od; /* div 1, 2, 4, 8 */
	u8  txpll_4_mux; /* a_phy_clock */
	u8  hdmi_ctl2_div5_10; /* hdmi_clk_todig, hdmi_ch_clk */
	u8  fpll_vco_clk; /* 1.6G ~ 3.2G, Unit: kHz */
	u8  fpll_tmds_od1; /* div 1, 2, 4, 8 */
	u8  fpll_tmds_od2; /* div 1, 2, 4, 8, then div2 to tmds_clk */
	u8  fpll_pixel_od; /* div 1, 1.25, 1.5, 2, pixel_clk */
	u8  vid_pll_clk_div; /* div 5, 6.25, 7.5, htx_tmds_clk */
	u8  hdmi_pll_tmds_clk_div; /* div 5, 6.25, 7.5, vid_pll0_clk*/
};
#endif
