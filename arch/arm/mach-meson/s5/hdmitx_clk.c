// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <amlogic/media/vout/hdmitx21/hdmitx.h>
#include "hdmitx_clk.h"
#include "hdmitx_misc.h"

#define MIN_HTXPLL_VCO 3000000 /* Min 3GHz */
#define MAX_HTXPLL_VCO 6000000 /* Max 6GHz */
#define MIN_FPLL_VCO 1600000 /* Min 1.6GHz */
#define MAX_FPLL_VCO 3200000 /* Max 3.2GHz */
#define MIN_GP2PLL_VCO 1600000 /* Min 1.6GHz */
#define MAX_GP2PLL_VCO 3200000 /* Max 3.2GHz */

#define usleep_range(a, b) udelay(a)

static int likely_frac_rate_mode(char *m);

/* local frac_rate flag */
static u32 frac_rate;

const static char od_map[9] = {
	0, 0, 1, 0, 2, 0, 0, 0, 3,
};

void disable_hdmitx_s5_plls(struct hdmitx_dev *hdev)
{
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL0, 0);
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL3, 0);
	hd21_write_reg(CLKCTRL_FPLL_CTRL0, 0);
	hd21_write_reg(CLKCTRL_GP2PLL_CTRL0, 0);
}

/* htx pll VCO output: (3G, 6G), for tmds */
static void set_s5_htxpll_clk_other(const u32 clk, const bool frl_en)
{
	u32 quotient;
	u32 remainder;
	u32 div0p5_en;
	u32 rem_1;
	u32 rem_2;

	if (clk < 3000000 || clk >= 6000000) {
		pr_err("%s[%d] clock should be 4~6G\n", __func__, __LINE__);
		return;
	}

	// set sub-pll as 24M output
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL0, (1 << 16) | (100 << 8) | (1 << 1));
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL0, 1, 0, 1);
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL1, 0xf00022a5);
	/* 0x8: lp_pll_clk selects pll_clk  0xb: selects lp_pll_clk24m */
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL2, 0x55813001 | (0xb << 4));
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL0, 0, 1, 1);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL0, 1, 2, 1);

	div0p5_en = 1;
	if (clk % 12000 == 0) {
		quotient = clk / 12000;
		remainder = 0;
	} else {
		quotient = clk / 12000;
		remainder = clk - quotient * 12000;
		/* remainder range: 0 ~ 99999, 0x1869f, 17bits */
		/* convert remainder to 0 ~ 2^17 */
		if (remainder) {
			rem_1 = remainder / 16;
			rem_2 = remainder - rem_1 * 16;
			rem_1 *= 1 << 17;
			rem_1 /= 750;
			rem_2 *= 1 << 13;
			rem_2 /= 750;
			remainder = rem_1 + rem_2;
		}
	}

	hd21_write_reg(ANACTRL_HDMIPLL_CTRL3, 0x000c0000 | (quotient << 8));
	/* HDMIPLL_CTRL4[25] enable tx_phy_clk1618 */
	/* bit16: spll_div_0p5_en */
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL4, 0x414412f2 | (frl_en << 25) | (div0p5_en << 16));
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL5, 0x00000203);
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL6, (!!remainder << 31) | remainder);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 1, 0, 1);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 1, 1, 1);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 1, 2, 1);
}

/* htx pll VCO output: 4/5/6G, for FRL */
static void set_s5_htxpll_clk_4_5_6g(const u32 clk, const bool frl_en)
{
	u32 htxpll_m = 0;
	u32 htxpll_ref_clk_od = 0;

	if (clk != 6000000 && clk != 5000000 && clk != 4000000) {
		pr_err("%s[%d] clock should be 4, 5, or 6G\n", __func__, __LINE__);
		return;
	}

	/* For 6G clock, here use the 24M as source */
	if (clk == 6000000) {
		/* 250 * 24M = 6G */
		hd21_write_reg(ANACTRL_HDMIPLL_CTRL3, 0x000c0000 | (250 << 8));
		hd21_write_reg(ANACTRL_HDMIPLL_CTRL0, 0x00006101 | (1 << 16));
		hd21_write_reg(ANACTRL_HDMIPLL_CTRL1, 0xf0002211);
		/* use the 24m instead of sub-pll */
		/* 0x8: lp_pll_clk selects pll_clk  0xb: selects lp_pll_clk24m */
		hd21_write_reg(ANACTRL_HDMIPLL_CTRL2, 0x55813000 | (0xb << 4));
		hd21_write_reg(ANACTRL_HDMIPLL_CTRL4, 0x0144f2f2 | (frl_en << 25));
		hd21_write_reg(ANACTRL_HDMIPLL_CTRL5, 0x00000203);
		hd21_write_reg(ANACTRL_HDMIPLL_CTRL6, 0x0);
		hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 1, 0, 1);
		hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 1, 1, 1);
		hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 1, 2, 1);
		return;
	}

	/* For 4, or 5G clock, here use the sub-pll generate 200M as source */
	/* 24MHz * 100 / 12 = 200MHz */
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL0, (1 << 1) | (100 << 8) | (12 << 16));
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL0, 1, 0, 1);
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL1, 0xf00022a5);
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL2, 0x55813081);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL0, 0, 1, 1);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL0, 1, 2, 1);
	if (clk == 5000000) {
		htxpll_m = 50;
		htxpll_ref_clk_od = 1;
	}
	if (clk == 4000000) {
		htxpll_m = 20;
		htxpll_ref_clk_od = 0;
	}
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL3,
		0x000c0000 | (htxpll_m << 8) | (htxpll_ref_clk_od << 4));
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL4, 0x03400293 | (frl_en << 25));
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL5, 0x00000203);
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL6, 0x00000000);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 1, 0, 1);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 1, 1, 1);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 1, 2, 1);
}

void set21_s5_htxpll_clk_out(const u32 clk, const u32 div)
{
	u32 div1;
	u32 div2;
	struct hdmitx_dev *hdev = get_hdmitx21_device();
	enum hdmi_colorspace cs = HDMI_COLORSPACE_YUV444;
	enum hdmi_color_depth cd = COLORDEPTH_24B;

	if (!hdev || !hdev->para)
		return;

	cs = hdev->para->cs;
	cd = hdev->para->cd;

	pr_info("%s[%d] htxpll vco %d div %d\n", __func__, __LINE__, clk, div);

	if (clk <= 3000000 || clk > 6000000) {
		pr_info("%s[%d] %d out of htxpll range(3~6G]\n", __func__, __LINE__, clk);
		return;
	}

	/* due to the VCO work performance, here needs to consider 3 cases
	 * 1. 3G to 4G * 2. 4G to 6G
	 * 3. 6G
	 */
	if (clk == 6000000 || clk == 5000000 || clk == 4000000)
		set_s5_htxpll_clk_4_5_6g(clk, hdev->frl_rate ? 1 : 0);
	else
		set_s5_htxpll_clk_other(clk, hdev->frl_rate ? 1 : 0);

	/* setting htxpll div */
	if (div > 8) {
		div1 = 8;
		div2 = div / 8;
	} else {
		div1 = div;
		div2 = 1;
	}
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, od_map[div1], 20, 2);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, od_map[div2], 22, 2);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 0, 24, 2);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL4, 0, 30, 2);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL0, 0, 2, 1);
	if (cs == HDMI_COLORSPACE_YUV420) {
		if (cd == COLORDEPTH_24B)
			hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 1, 20, 2);
		else
			hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 0, 20, 2);
		hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL4, 1, 30, 2);
	}
}

void set_frl_hpll_od(enum frl_rate_enum rate)
{
	if (rate == FRL_NONE || rate > FRL_12G4L) {
		pr_info("hdmitx: frl: wrong rate %d\n", rate);
		return;
	}

	/* fixed OD */
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 0, 22, 2);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 0, 24, 2);
	hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL4, 0, 30, 2);
	switch (rate) {
	case FRL_3G3L:
		hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 2, 20, 2);
		break;
	case FRL_6G3L:
	case FRL_6G4L:
		hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 1, 20, 2);
		break;
	case FRL_8G4L:
	case FRL_10G4L:
	case FRL_12G4L:
		hd21_set_reg_bits(ANACTRL_HDMIPLL_CTRL3, 0, 20, 2);
		break;
	default:
		break;
	};
}

// fpll: 2376M  2376/24=99=0x63
//       2376/16=148.5M
/* div: 1, 2, 4, 8, ... 64 */
/* pixel_od: 1:1, 1:1.25, 1:1.5, 1:2 */
void hdmitx_set_s5_fpll(u32 clk, u32 div, u32 pixel_od)
{
	u32 div1;
	u32 div2;
	u32 quotient;
	u32 remainder;

	pr_info("%s[%d] clk %d div %d pixel_od %d\n", __func__, __LINE__, clk, div, pixel_od);
	/* setting fpll vco */
	quotient = clk / 24000;
	remainder = clk - quotient * 24000;
	/* remainder range: 0 ~ 23999, 0x5dbf, 15bits */
	remainder *= 1 << 17;
	remainder /= 24000;
	hd21_write_reg(CLKCTRL_FPLL_CTRL0, 0x21210000 | quotient);
	hd21_set_reg_bits(CLKCTRL_FPLL_CTRL0, 1, 28, 1);
	hd21_write_reg(CLKCTRL_FPLL_CTRL1, 0x03a00000 | remainder);
	hd21_write_reg(CLKCTRL_FPLL_CTRL2, 0x00040000);
	hd21_write_reg(CLKCTRL_FPLL_CTRL3, 0x0b0da000);
	if (remainder)
		hd21_set_reg_bits(CLKCTRL_FPLL_CTRL3, 1, 27, 1); /* enable frac */
	usleep_range(20, 30);
	hd21_set_reg_bits(CLKCTRL_FPLL_CTRL0, 0, 29, 1);
	usleep_range(20, 30);
	hd21_set_reg_bits(CLKCTRL_FPLL_CTRL3, 1, 9, 1); /* enable pll_lock_rst */

	/* setting fpll div */
	if (div > 8) {
		div1 = 8;
		div2 = div / 8;
	} else {
		div1 = div;
		div2 = 1;
	}
	hd21_set_reg_bits(CLKCTRL_FPLL_CTRL0, od_map[div1], 23, 2); // fpll_tmds_od<3:2> div8
	hd21_set_reg_bits(CLKCTRL_FPLL_CTRL0, od_map[div2], 21, 2); // fpll_tmds_od<1:0> div2

	/* setting pixel_od */
	hd21_set_reg_bits(CLKCTRL_FPLL_CTRL0, pixel_od, 13, 3); // pixel_od
}

// gp2pll: 2376M  2376/24=99=0x63
//       2376/16=148.5M
/* div: 1, 2, 4, 8, ... 64 */
void hdmitx_set_s5_gp2pll(u32 clk, u32 div)
{
	u32 quotient;
	u32 remainder;

	pr_info("%s[%d] clk %d div %d\n", __func__, __LINE__, clk, div);
	/* setting fpll vco */
	quotient = clk / 24000;
	remainder = clk - quotient * 24000;
	/* remainder range: 0 ~ 23999, 0x5dbf, 15bits */
	remainder *= 1 << 17;
	remainder /= 24000;
	hd21_write_reg(CLKCTRL_GP2PLL_CTRL0, 0x20010800 | quotient);
	hd21_set_reg_bits(CLKCTRL_GP2PLL_CTRL0, 1, 28, 1);
	hd21_write_reg(CLKCTRL_GP2PLL_CTRL1, 0x03a00000 | remainder);
	hd21_write_reg(CLKCTRL_GP2PLL_CTRL2, 0x00040000);
	hd21_write_reg(CLKCTRL_GP2PLL_CTRL3, 0x010da000);
	if (remainder)
		hd21_set_reg_bits(CLKCTRL_GP2PLL_CTRL3, 1, 27, 1); /* enable frac */
	usleep_range(20, 30);
	hd21_set_reg_bits(CLKCTRL_GP2PLL_CTRL0, 0, 29, 1);
	usleep_range(20, 30);
	hd21_set_reg_bits(CLKCTRL_GP2PLL_CTRL3, 1, 9, 1); /* enable pll_lock_rst */

	hd21_set_reg_bits(CLKCTRL_FPLL_CTRL0, od_map[div], 23, 2); // gp2pll_tmds_od<2:0>
}

static void hdmitx_set_s5_clkdiv(struct hdmitx_dev *hdev)
{
	if (!hdev && !hdev->para)
		return;

	/* cts_htx_tmds_clk selects the htx_tmds20_clk or fll_tmds_clk */
	hd21_set_reg_bits(CLKCTRL_HTX_CLK_CTRL1, hdev->frl_rate ? 1 : 0, 25, 2);
	if (!hdev->frl_rate && hdev->para->cs == HDMI_COLORSPACE_YUV420)
		hd21_set_reg_bits(CLKCTRL_HTX_CLK_CTRL1, 1, 16, 7);
	else
		hd21_set_reg_bits(CLKCTRL_HTX_CLK_CTRL1, 0, 16, 7);
	/* master_clk selects the vid_pll_clk or fpll_pixel_clk */
	hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL, hdev->frl_rate ? 4 : 0, 16, 3);
}

void hdmitx21_set_audioclk(bool en)
{
	u32 data32;

	// Enable hdmitx_aud_clk
	// [10: 9] clk_sel for cts_hdmitx_aud_clk: 2=fclk_div3
	// [    8] clk_en for cts_hdmitx_aud_clk
	// [ 6: 0] clk_div for cts_hdmitx_aud_clk: fclk_div3/aud_clk_div
	data32 = 0;
	data32 |= (2 << 9);
	data32 |= (0 << 8);
	data32 |= ((18 - 1) << 0);
	hd21_set_reg_bits(CLKCTRL_HTX_CLK_CTRL1, 2, 9, 2);
	hd21_set_reg_bits(CLKCTRL_HTX_CLK_CTRL1, 18 - 1, 0, 8);
	// [    8] clk_en for cts_hdmitx_aud_clk
	hd21_set_reg_bits(CLKCTRL_HTX_CLK_CTRL1, en, 8, 1);
}

void hdmitx21_set_default_clk(void)
{
	u32 data32;

	// Enable clk81_hdmitx_pclk
	hd21_set_reg_bits(CLKCTRL_SYS_CLK_EN0_REG2, 1, 4, 1);

	// Enable fixed hdmitx_sys_clk
	data32 = 0;
	data32 |= (3 << 9); // [10: 9] clk_sel for cts_hdmitx_sys_clk: 3=fclk_div5
	data32 |= (0 << 8); // [    8] clk_en for cts_hdmitx_sys_clk
	data32 |= (1 << 0); // [ 6: 0] clk_div for cts_hdmitx_sys_clk: fclk_dvi5/2=400/2=200M
	hd21_write_reg(CLKCTRL_HDMI_CLK_CTRL, data32);
	data32 |= (1 << 8); // [    8] clk_en for cts_hdmitx_sys_clk
	hd21_write_reg(CLKCTRL_HDMI_CLK_CTRL, data32);

	// Enable fixed hdmitx_prif_clk, hdmitx_200m_clk
	data32 = 0;
	data32 |= (3 << 25); // [26:25] clk_sel for cts_hdmitx_200m_clk: 3=fclk_div5
	data32 |= (0 << 24); // [   24] clk_en for cts_hdmitx_200m_clk
	data32 |= (1 << 16); // [22:16] clk_div for cts_hdmitx_200m_clk: fclk_dvi5/16=400/16=25M
	data32 |= (3 << 9); // [10: 9] clk_sel for cts_hdmitx_prif_clk: 3=fclk_div5
	data32 |= (0 << 8); // [    8] clk_en for cts_hdmitx_prif_clk
	data32 |= (1 << 0); // [ 6: 0] clk_div for cts_hdmitx_prif_clk: fclk_dvi5/2=400/2=200M
	hd21_write_reg(CLKCTRL_HTX_CLK_CTRL0, data32);
	data32 |= (1 << 24); // [   24] clk_en for cts_hdmitx_200m_clk
	data32 |= (1 << 8); // [    8] clk_en for cts_hdmitx_prif_clk
	hd21_write_reg(CLKCTRL_HTX_CLK_CTRL0, data32);

	//hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL, 0, 0, 5);

	// Bring HDMITX MEM output of power down
	hd21_set_reg_bits(PWRCTRL_MEM_PD11, 0, 8, 8);
	// Bring out of reset
	hdmitx21_wr_reg(HDMITX_TOP_SW_RESET, 0);
	// Test after initial out of reset, cannot write to IP register, unless enable access
	hdmitx21_wr_reg(INTR3_MASK_IVCTX, 0xff);
	hdmitx21_wr_reg(HDMITX_TOP_SEC_SCRATCH, 1);
}

void hdmitx21_set_cts_hdcp22_clk(struct hdmitx_dev *hdev)
{
	//hd21_write_reg(CLKCTRL_HDCP22_CLK_CTRL, 0x01000100);
}

void hdmitx21_set_hdcp_pclk(struct hdmitx_dev *hdev)
{
	/* top hdcp pixel clock */
	hd21_set_reg_bits(CLKCTRL_SYS_CLK_EN0_REG2, 1, 3, 1);
}

/* --------------------------------------------------
 *             set_tmds_vid_clk_div
 * --------------------------------------------------
 * wire            clk_final_en    = control[19];
 * wire            clk_div1        = control[18];
 * wire    [1:0]   clk_sel         = control[17:16];
 * wire            set_preset      = control[15];
 * wire    [14:0]  shift_preset    = control[14:0];
 * div_src: 0 means divide the hdmi_tmds_out2 to tmds_clk
 *          1 means divide the hdmi_tmds_out2 to vid_pll0_clk
 */
static void set_tmds_vid_clk_div(u8 div_src, u32 div_val)
{
	u32 div_reg;
	u32 shift_val = 0;
	u32 shift_sel = 0;

	div_reg = (div_src == 1) ? CLKCTRL_HDMI_VID_PLL_CLK_DIV : CLKCTRL_HDMI_PLL_TMDS_CLK_DIV;

	// Disable the output clock
	hd21_set_reg_bits(div_reg, 0, 15, 1);
	hd21_set_reg_bits(div_reg, 0, 19, 1);

	switch (div_val) {
	case VID_PLL_DIV_1:
		shift_val = 0xFFFF;
		shift_sel = 0;
		break;
	case VID_PLL_DIV_2:
		shift_val = 0x0aaa;
		shift_sel = 0;
		break;
	case VID_PLL_DIV_3:
		shift_val = 0x0db6;
		shift_sel = 0;
		break;
	case VID_PLL_DIV_3p5:
		shift_val = 0x36cc;
		shift_sel = 1;
		break;
	case VID_PLL_DIV_3p75:
		shift_val = 0x6666;
		shift_sel = 2;
		break;
	case VID_PLL_DIV_4:
		shift_val = 0x0ccc;
		shift_sel = 0;
		break;
	case VID_PLL_DIV_5:
		shift_val = 0x739c;
		shift_sel = 2;
		break;
	case VID_PLL_DIV_6:
		shift_val = 0x0e38;
		shift_sel = 0;
		break;
	case VID_PLL_DIV_6p25:
		shift_val = 0x0000;
		shift_sel = 3;
		break;
	case VID_PLL_DIV_7:
		shift_val = 0x3c78;
		shift_sel = 1;
		break;
	case VID_PLL_DIV_7p5:
		shift_val = 0x78f0;
		shift_sel = 2;
		break;
	case VID_PLL_DIV_12:
		shift_val = 0x0fc0;
		shift_sel = 0;
		break;
	case VID_PLL_DIV_14:
		shift_val = 0x3f80;
		shift_sel = 1;
		break;
	case VID_PLL_DIV_15:
		shift_val = 0x7f80;
		shift_sel = 2;
		break;
	default:
		pr_err("%s[%d] invalid div %d\n", __func__, __LINE__, div_val);
	}

	if (shift_val == 0xffff) { // if divide by 1
		hd21_set_reg_bits(div_reg, 1, 18, 1);
	} else {
		hd21_set_reg_bits(div_reg, shift_val, 0, 15);
		hd21_set_reg_bits(div_reg, 1, 15, 1);
		hd21_set_reg_bits(div_reg, 0, 20, 1);
		hd21_set_reg_bits(div_reg, shift_sel, 16, 3);
		// Set the selector low
		hd21_set_reg_bits(div_reg, 0, 15, 1);
	}
	// Enable the final output clock
	hd21_set_reg_bits(div_reg, 1, 19, 1);
}

/* if vsync likes 24000, 30000, ... etc, return 1 */
static bool is_vsync_int(u32 clk)
{
	if (clk % 3000 == 0)
		return 1;
	return 0;
}

/* if vsync likes 59940, ... etc, return 1 */
static bool is_vsync_frac(u32 clk)
{
	clk += clk / 1000;
	if (is_vsync_int(clk) || is_vsync_int(clk + 1))
		return 1;
	return 0;
}

/* for varied hdmi basic modes, such as
 * vic/16, the vsync is 60, and may shift to 59.94
 * but vic/2, the vsync is 59.94, and may shift to 60
 * return values:
 *    0: no any shift
 *    1: shift down 0.1%
 *    2: shift up 0.1%
 */
static u32 check_clock_shift(enum hdmi_vic vic, u32 frac_policy)
{
	const struct hdmi_timing *timing = NULL;

	timing = hdmitx21_gettiming_from_vic(vic);
	if (!timing) {
		pr_err("%s[%d] not valid vic %d\n", __func__, __LINE__, vic);
		return 0;
	}

	/* only check such as 24hz, 30hz, 60hz, ... */
	if (!likely_frac_rate_mode(timing->name))
		return 0;

	if (is_vsync_int(timing->v_freq)) {
		if (frac_policy)
			return 1;
		else
			return 0;
	}
	if (is_vsync_frac(timing->v_freq)) {
		if (frac_policy)
			return 0;
		else
			return 2;
	}
	return 0;
}

static void set_hdmitx_s5_htx_pll(struct hdmitx_dev *hdev)
{
	enum hdmi_vic vic = HDMI_0_UNKNOWN;
	enum hdmi_colorspace cs = HDMI_COLORSPACE_YUV444;
	enum hdmi_color_depth cd = COLORDEPTH_24B;
	u32 base_pixel_clk = 25200;
	u32 htx_vco = 5940000;
	u32 div = 1;

	if (!hdev || !hdev->para)
		return;

	vic = hdev->para->timing.vic;
	cs = hdev->para->cs;
	cd = hdev->para->cd;
	if (vic == HDMI_0_UNKNOWN) {
		pr_err("%s[%d] not valid vic %d\n", __func__, __LINE__, vic);
		return;
	}

	base_pixel_clk = hdev->para->timing.pixel_freq;
	if (base_pixel_clk < 25175 || base_pixel_clk > 5940000) {
		pr_err("%s[%d] not valid pixel clock %d\n", __func__, __LINE__, base_pixel_clk);
		return;
	}

	/* For FRL modes */
	if (hdev->frl_rate != FRL_NONE) {
		pr_info("set hpll for frl_rate %d\n", hdev->frl_rate);
		switch (hdev->frl_rate) {
		case FRL_3G3L:
			set21_s5_htxpll_clk_out(6000000, 8);
			break;
		case FRL_6G3L:
			set21_s5_htxpll_clk_out(6000000, 4);
			break;
		case FRL_6G4L:
			set21_s5_htxpll_clk_out(6000000, 2);
			break;
		case FRL_12G4L:
			set21_s5_htxpll_clk_out(6000000, 1);
			break;
		case FRL_8G4L:
			set21_s5_htxpll_clk_out(4000000, 1);
			break;
		case FRL_10G4L:
			set21_s5_htxpll_clk_out(5000000, 1);
			break;
		default:
			pr_err("not support frl_rate: %d\n", hdev->frl_rate);
			break;
		}
		return;
	}

	pr_info("%s[%d] base_pixel_clk %d  cs %d  cd %d  frac_rate %d\n",
		__func__, __LINE__, base_pixel_clk, cs, cd, frac_rate);
	/* for legacy TMDS modes */
	if (cs != HDMI_COLORSPACE_YUV422) {
		switch (cd) {
		case COLORDEPTH_48B:
			base_pixel_clk = base_pixel_clk * 2;
			break;
		case COLORDEPTH_36B:
			base_pixel_clk = base_pixel_clk * 3 / 2;
			break;
		case COLORDEPTH_30B:
			base_pixel_clk = base_pixel_clk * 5 / 4;
			break;
		case COLORDEPTH_24B:
		default:
			base_pixel_clk = base_pixel_clk * 1;
			break;
		}
	}
	if (check_clock_shift(vic, frac_rate) == 1)
		base_pixel_clk = base_pixel_clk - base_pixel_clk / 1001;
	if (check_clock_shift(vic, frac_rate) == 2)
		base_pixel_clk = base_pixel_clk + base_pixel_clk / 1000;
	base_pixel_clk = base_pixel_clk * 10; /* for tmds modes, here should multi 10 */
	if (cs == HDMI_COLORSPACE_YUV420)
		base_pixel_clk /= 2;
	pr_info("%s[%d] calculate pixel_clk to %d\n", __func__, __LINE__, base_pixel_clk);
	if (base_pixel_clk > MAX_HTXPLL_VCO) {
		pr_err("%s[%d] base_pixel_clk %d over MAX_HTXPLL_VCO %d\n",
			__func__, __LINE__, base_pixel_clk, MAX_HTXPLL_VCO);
	}

	div = 1;
	/* the base pixel_clk range should be 250M ~ 5940M? */
	htx_vco = base_pixel_clk;
	do {
		if (htx_vco >= MIN_HTXPLL_VCO && htx_vco < MAX_HTXPLL_VCO)
			break;
		div *= 2;
		htx_vco *= 2;
	} while (div <= 32);

	/* the hdmi phy works under DUAL mode, and the div should be multiply 2 */
	div *= 2;

	set21_s5_htxpll_clk_out(htx_vco, div);
}

static void set_hdmitx_htx_pll(struct hdmitx_dev *hdev)
{
	enum hdmi_colorspace cs = hdev->para->cs;
	enum hdmi_color_depth cd = hdev->para->cd;
	u8 clk_div_val = VID_PLL_DIV_5;

	//if (hdev->pxp_mode) /* skip VCO setting */
	//	return;

	if (1) {
		set_hdmitx_s5_htx_pll(hdev);
		if (hdev->frl_rate)
			set_frl_hpll_od(hdev->frl_rate);
		if (cs != HDMI_COLORSPACE_YUV422) {
			if (cd == COLORDEPTH_36B)
				clk_div_val = VID_PLL_DIV_7p5;
			else if (cd == COLORDEPTH_30B)
				clk_div_val = VID_PLL_DIV_6p25;
			else
				clk_div_val = VID_PLL_DIV_5;
		}
		set_tmds_vid_clk_div(0, VID_PLL_DIV_5);
		set_tmds_vid_clk_div(1, clk_div_val);
		// set crt_vid_mux_div
		//[19] disable clk_div0
		hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL, 0, 19, 1);
		// bit[18:16] crt_vid_mux_div source select
		hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL, 0, 16, 3);
		hd21_set_reg_bits(CLKCTRL_VID_CLK0_DIV, 0, 0, 8);
		// bit[2:0] crt_vid_mux_div div1/2/4 enable
		hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL, 7, 0, 3);
		// cts_enc_clk div and enable
		hd21_set_reg_bits(CLKCTRL_VIID_CLK0_DIV, 0, 12, 4);
		hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL2, 1, 3, 1);
		// enc0_hdmi_tx_fe_clk div and enable
		hd21_set_reg_bits(CLKCTRL_ENC0_HDMI_CLK_CTRL, 0, 20, 4);
		hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL2, 1, 9, 1);
		// enc0_hdmi_tx_pnx_clk div and enable
		hd21_set_reg_bits(CLKCTRL_ENC0_HDMI_CLK_CTRL, 0, 24, 4);
		hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL2, 1, 10, 1);
		// enc0_hdmi_tx_pixel_clk div and enable
		hd21_set_reg_bits(CLKCTRL_ENC0_HDMI_CLK_CTRL, 0, 16, 4);
		hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL2, 1, 5, 1);
		//[19] enable clk_div0
		hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL, 1, 19, 1);
		return;
	}
}

static int likely_frac_rate_mode(char *m)
{
	if (strstr(m, "24hz") || strstr(m, "30hz") || strstr(m, "60hz") ||
	    strstr(m, "120hz") || strstr(m, "240hz"))
		return 1;
	else
		return 0;
}

static void hdmitx_set_fpll_without_dsc(struct hdmitx_dev *hdev)
{
	u32 fpll_vco = 2376000;
	u32 div = 1;
	u32 tmp_clk = 0;
	u32 pixel_od = 0;
	enum hdmi_vic vic = HDMI_0_UNKNOWN;

	if (!hdev && !hdev->para)
		return;

	vic = hdev->para->timing.vic;
	tmp_clk = hdev->para->timing.pixel_freq;
	if (hdev->frl_rate)
		tmp_clk /= 2;
	switch (hdev->para->cs) {
	case HDMI_COLORSPACE_RGB:
	case HDMI_COLORSPACE_YUV444:
		if (hdev->para->cd == COLORDEPTH_30B) {
			tmp_clk = tmp_clk * 5 / 4;
			pixel_od = 1;
		}
		if (hdev->para->cd == COLORDEPTH_36B) {
			tmp_clk = tmp_clk * 3 / 2;
			pixel_od = 2;
		}
		if (hdev->para->cd == COLORDEPTH_48B) {
			tmp_clk = tmp_clk * 2;
			pixel_od = 4;
		}
		break;
	case HDMI_COLORSPACE_YUV420:
		tmp_clk /= 2;
		if (hdev->para->cd == COLORDEPTH_30B) {
			tmp_clk = tmp_clk * 5 / 4;
			pixel_od = 1;
		}
		if (hdev->para->cd == COLORDEPTH_36B) {
			tmp_clk = tmp_clk * 3 / 2;
			pixel_od = 2;
		}
		if (hdev->para->cd == COLORDEPTH_48B) {
			tmp_clk *= 1;
			pixel_od = 4;
		}
		break;
	case HDMI_COLORSPACE_YUV422:
	default:
		tmp_clk *= 1;
		pixel_od = 0;
		break;
	}
	tmp_clk *= 2; /* here is a fixed DIV2 to tmds_clk */

	fpll_vco = tmp_clk;
	if (fpll_vco > MAX_FPLL_VCO) {
		pr_info("hdmitx21: FPLL VCO over clock %d\n", fpll_vco);
		return;
	}
	if (check_clock_shift(vic, frac_rate) == 1) {
		fpll_vco = fpll_vco - fpll_vco / 1001;
		pr_info("fpll_vco %d down shift to %d\n", tmp_clk, fpll_vco);
	}
	if (check_clock_shift(vic, frac_rate) == 2) {
		fpll_vco = fpll_vco + fpll_vco / 1000;
		pr_info("fpll_vco %d up shift to %d\n", tmp_clk, fpll_vco);
	}
	div = 1;
	do {
		if (fpll_vco >= MIN_FPLL_VCO && fpll_vco < MAX_FPLL_VCO)
			break;
		div *= 2;
		fpll_vco *= 2;
	} while (div <= 64);

	hdmitx_set_s5_fpll(fpll_vco, div, pixel_od);
}

static void hdmitx_set_fpll_with_dsc(struct hdmitx_dev *hdev)
{
	u32 fpll_vco = 2376000;
	u32 div = 1;
	u32 tmp_clk = 0;
	u32 pixel_od = 0;

	if (!hdev && !hdev->para)
		return;

	/* HARD CODE, FRL8G4L 4320p60 y420 8bit, HDMI 2.1 Spec, Page 281 */
	/* 594 / 4500 * (2380 + 116) */
	tmp_clk = 329472 * 2;
	/* TODO */
	fpll_vco = tmp_clk;
	if (fpll_vco > MAX_FPLL_VCO) {
		pr_info("hdmitx21: FPLL VCO over clock %d\n", fpll_vco);
		return;
	}
	if (0) { /* TODO */
		fpll_vco = fpll_vco - fpll_vco / 1001;
		pr_info("fpll_vco %d shift to %d\n", tmp_clk, fpll_vco);
	}
	div = 1;
	do {
		if (fpll_vco >= MIN_FPLL_VCO && fpll_vco < MAX_FPLL_VCO)
			break;
		div *= 2;
		fpll_vco *= 2;
	} while (div <= 64);

	hdmitx_set_s5_fpll(fpll_vco, div, pixel_od);
}

void hdmitx_set_fpll(struct hdmitx_dev *hdev)
{
	if (hdev->dsc_en)
		hdmitx_set_fpll_with_dsc(hdev);
	else
		hdmitx_set_fpll_without_dsc(hdev);
}

void hdmitx_set_gp2pll(struct hdmitx_dev *hdev)
{
	u32 gp2pll_vco = 2376000;
	u32 div = 1;
	u32 tmp_clk = 0;

	if (!hdev && !hdev->para)
		return;

	tmp_clk = hdev->para->timing.pixel_freq;
	if (hdev->frl_rate)
		tmp_clk /= 2;
	switch (hdev->para->cs) {
	case HDMI_COLORSPACE_RGB:
	case HDMI_COLORSPACE_YUV444:
		if (hdev->para->cd == COLORDEPTH_30B)
			tmp_clk = tmp_clk * 5 / 4;
		if (hdev->para->cd == COLORDEPTH_36B)
			tmp_clk = tmp_clk * 3 / 2;
		if (hdev->para->cd == COLORDEPTH_48B)
			tmp_clk = tmp_clk * 2;
		break;
	case HDMI_COLORSPACE_YUV420:
		tmp_clk /= 2;
		if (hdev->para->cd == COLORDEPTH_30B)
			tmp_clk = tmp_clk * 5 / 4;
		if (hdev->para->cd == COLORDEPTH_36B)
			tmp_clk = tmp_clk * 3 / 2;
		if (hdev->para->cd == COLORDEPTH_48B)
			tmp_clk *= 1;
		break;
	case HDMI_COLORSPACE_YUV422:
	default:
		tmp_clk *= 1;
		break;
	}
	tmp_clk *= 2; /* here is a fixed DIV2 to tmds_clk */

	gp2pll_vco = tmp_clk;
	if (gp2pll_vco > MAX_FPLL_VCO) {
		pr_info("hdmitx21: GP2PLL VCO over clock %d\n", gp2pll_vco);
		return;
	}
	if (0) { /* TODO */
		gp2pll_vco = gp2pll_vco * 1000 / 1001;
		pr_info("gp2pll_vco %d shift to %d\n", tmp_clk, gp2pll_vco);
	}
	div = 1;
	do {
		if (gp2pll_vco >= MIN_GP2PLL_VCO && gp2pll_vco < MAX_GP2PLL_VCO)
			break;
		div *= 2;
		gp2pll_vco *= 2;
	} while (div <= 16);

	hdmitx_set_s5_gp2pll(gp2pll_vco, div);
}

void hdmitx_set_clkdiv(struct hdmitx_dev *hdev)
{
	hdmitx_set_s5_clkdiv(hdev);
}

static void hdmitx_check_frac_rate(struct hdmitx_dev *hdev)
{
	struct hdmi_format_para *para = hdev->para;
	char *frac_rate_str = NULL;

	frac_rate = hdev->frac_rate_policy;
	frac_rate_str = env_get("frac_rate_policy");
	if (frac_rate_str && (frac_rate_str[0] == '0'))
		frac_rate = 0;
	else if (para && para->timing.name && likely_frac_rate_mode(para->timing.name))
		frac_rate = 1;

	hdev->frac_rate_policy = frac_rate;
	pr_info("%s: frac_rate:%d\n", __func__, frac_rate);
}

void hdmitx21_set_clk(struct hdmitx_dev *hdev)
{
	hdmitx_check_frac_rate(hdev);

	disable_hdmitx_s5_plls(hdev);
	/* typical 3 modes: legacy tmds, FRL w/o DSC, FRL w/ DSC */
	set_hdmitx_htx_pll(hdev);
	if (hdev->frl_rate) {
		hdmitx_set_fpll(hdev);
		if (hdev->dsc_en)
			hdmitx_set_gp2pll(hdev);
	}
}
