// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <linux/kernel.h>
#ifdef CONFIG_SECURE_POWER_CONTROL
#include <asm/arch/pwr_ctrl.h>
#endif
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "lcd_reg.h"
#include "lcd_common.h"

/*******************************************************************************
 * vpu/display init
 ******************************************************************************/
static unsigned int pll_table[] = {0x0, 0x5, 0xa};
static unsigned int venc_table[] = {0x0, 0x600, 0x800};
static unsigned int venc_if_table[] = {0x0, 0x500, 0x600};
static unsigned int venc_data_table[] = {0x0, 0x100, 0x200};
#define EDP_AUX_RETRY_CNT    5
#define EDP_AUX_TIMEOUT      1000
#define EDP_AUX_INTERVAL     200
static int dptx_aux_write(int index, unsigned int addr, unsigned int len, unsigned char *buf)
{
	unsigned int data, i, state;
	int retry_cnt = 0, timeout = 0;

aux_write_retry:
	do {
		state = dptx_reg_read(index, EDP_TX_AUX_STATE);
	} while (state & (1 << 1));

	dptx_reg_write(index, EDP_TX_AUX_ADDRESS, addr);
	for (i = 0; i < len; i++)
		dptx_reg_write(index, EDP_TX_AUX_WRITE_FIFO, buf[i]);

	dptx_reg_write(index, EDP_TX_AUX_COMMAND, (0x800 | ((len - 1) & 0xf)));

	while (timeout++ < EDP_AUX_TIMEOUT) {
		udelay(EDP_AUX_INTERVAL);
		data = dptx_reg_read(index, EDP_TX_AUX_TRANSFER_STATUS);
		if (data & (1 << 0)) {
			state = dptx_reg_read(index, EDP_TX_AUX_REPLY_CODE);
			if (state == 0)
				return 0;
			if (state == 1) {
				LCDPR("edp aux write addr 0x%x NACK!\n", addr);
				return -1;
			}
			if (state == 2)
				LCDPR("  edp aux write addr 0x%x Defer!\n", addr);
			break;
		}

		if (data & (1 << 3)) {
			LCDPR("  edp aux write addr 0x%x Error!\n", addr);
			break;
		}
	}

	if (retry_cnt++ < EDP_AUX_RETRY_CNT) {
		udelay(EDP_AUX_INTERVAL);
		LCDPR("edp aux write addr 0x%x timeout, retry %d\n", addr, retry_cnt);
		goto aux_write_retry;
	}

	LCDPR("edp aux write addr 0x%x failed\n", addr);
	return -1;
}
#if 0
static int dptx_aux_read(int index, unsigned int addr, unsigned int len, unsigned char *buf)
{
	unsigned int data, i, state;
	int retry_cnt = 0, timeout = 0;

aux_read_retry:
	do {
		state = dptx_reg_read(index, EDP_TX_AUX_STATE);
	} while (state & (1 << 1));

	dptx_reg_write(index, EDP_TX_AUX_ADDRESS, addr);
	dptx_reg_write(index, EDP_TX_AUX_COMMAND, (0x900 | ((len - 1) & 0xf)));

	while (timeout++ < EDP_AUX_TIMEOUT) {
		udelay(EDP_AUX_INTERVAL);
		data = dptx_reg_read(index, EDP_TX_AUX_TRANSFER_STATUS);
		if (data & (1 << 0)) {
			state = dptx_reg_read(index, EDP_TX_AUX_REPLY_CODE);
			if (state == 0)
				goto aux_read_succeed;
			if (state == 1) {
				LCDPR("edp aux read addr 0x%x NACK!\n", addr);
				return -1;
			}
			if (state == 2)
				LCDPR("  edp aux read addr 0x%x Defer!\n", addr);
			break;
		}

		if (data & (1 << 3)) {
			LCDPR("  edp aux read addr 0x%x Error!\n", addr);
			break;
		}
	}

	if (retry_cnt++ < EDP_AUX_RETRY_CNT) {
		udelay(EDP_AUX_INTERVAL);
		LCDPR("  edp aux read addr 0x%x timeout, retry %d\n", addr, retry_cnt);
		goto aux_read_retry;
	}

	LCDPR("edp aux read addr 0x%x failed\n", addr);
	return -1;

aux_read_succeed:
	for (i = 0; i < len; i++)
		buf[i] = (unsigned char)(dptx_reg_read(index, EDP_TX_AUX_REPLY_DATA));

	return 0;
}
#endif

static void dptx_link_fast_training(int index)
{
	unsigned char p_data = 0;
	int ret;

	LCDPR("..... Sending edp training pattern 1 .....\n");
	dptx_reg_write(index, EDP_TX_SCRAMBLING_DISABLE ,0x1);   // disable scrambling
	dptx_reg_write(index, EDP_TX_TRAINING_PATTERN_SET, 0x1); // set training pattern 1

	p_data = 0x21;
	ret = dptx_aux_write(index, 0x102, 1, &p_data);    // set pattern 1 in the RX
	if (ret) {
		LCDPR("..... edp training pattern 1 failed.....\n");
		//return;
	}

	udelay(10);

	LCDPR("..... Sending training pattern 2 .....\n");
	dptx_reg_write(index, EDP_TX_TRAINING_PATTERN_SET, 0x2);
	p_data = 0x22;
	ret = dptx_aux_write(index, 0x102, 1, &p_data);      // set pattern 2 in the RX
	if (ret) {
		LCDPR("..... edp training pattern 2 failed.....\n");
		//return;
	}

	udelay(10);

	LCDPR("..... Sending training pattern 3 .....\n");
	dptx_reg_write(index, EDP_TX_TRAINING_PATTERN_SET, 0x3);
	p_data = 0x23;
	ret = dptx_aux_write(index, 0x102, 1, &p_data);      // set pattern 3 in the RX
	if (ret) {
		LCDPR("..... edp training pattern 3 failed.....\n");
		//return;
	}

	udelay(10);

	p_data = 0x20;
	ret = dptx_aux_write(index, 0x102, 1, &p_data);         // pattern off
	if (ret) {
		LCDPR("..... edp training pattern off failed.....\n");
		//return;
	}

	// disable the training pattern
	dptx_reg_write(index, EDP_TX_TRAINING_PATTERN_SET, 0x0);
}

static void dptx_set_msa(int index)
{
	unsigned int hactive = 1920;
	unsigned int vactive = 1080;
	unsigned int htotal = 2200;
	unsigned int vtotal = 1120;
	unsigned int hsw = 44;
	unsigned int hbp = 148;
	unsigned int vsw = 5;
	unsigned int vbp = 30;
	//unsigned int pclk = 147840000;

	//unsigned int lane_count = 2;
	unsigned int data_per_lane;
	unsigned int misc0_data;
	unsigned int m_vid = 147840; //pclk/1000
	unsigned int n_vid = 270000; //162000, 270000, 540000

	unsigned int ppc = 1;// 1 pix per clock pix0 only
	unsigned int cfmt = 0;// RGB
	unsigned int bpc = 8;// bits per color

	data_per_lane = ((hactive * bpc * 3) + 15) / 16 - 1;

	//bit[0] sync mode (1=sync 0=async)
	misc0_data = (cfmt << 1) | (1 << 0);
	misc0_data |= (0x1 << 5); //6bit:0x0, 8bit:0x1, 10bit:0x2, 12bit:0x3

	dptx_reg_write(index, EDP_TX_MAIN_STREAM_HTOTAL, htotal);
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_VTOTAL, vtotal);
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_POLARITY, (0 << 1) | (0 << 0));
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_HSWIDTH, hsw);
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_VSWIDTH, vsw);
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_HRES, hactive);
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_VRES, vactive);
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_HSTART, (hsw + hbp));
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_VSTART, (vsw + vbp));
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_MISC0, misc0_data);
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_MISC1, 0x00000000);
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_M_VID, m_vid); //unit: 1kHz
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_N_VID, n_vid); //unit: 10kHz
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_TRANSFER_UNIT_SIZE, 32);
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_DATA_COUNT_PER_LANE, data_per_lane);
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_USER_PIXEL_WIDTH, ppc);

	LCDPR(" edp%d MSA done\n", index);
}

static void dptx_reset(int index)
{
	unsigned int data32, bit;

	if (index == 0)
		bit = 17;
	else
		bit = 18;

	// Level reset mail
	data32 = readl(RESETCTRL_RESET1_MASK);
	data32 &= (~(0x1 << bit));
	writel(data32, RESETCTRL_RESET1_MASK);
	data32 = readl(RESETCTRL_RESET1_LEVEL);
	data32 &= (~(0x1 << bit));
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(1);
	data32 |= (0x1 << bit);
	writel(data32, RESETCTRL_RESET1_LEVEL);

	// Set clock-divider for EDP-APB
	dptx_reg_write(index, EDP_TX_AUX_CLOCK_DIVIDER, 24); // Set Aux channel clk-div: 24MHz
}

static void dptx_phy_reset(int index)
{
	unsigned int data32, bit;

	if (index == 0)
		bit = 19;
	else
		bit = 20;

	// Level reset mail
	data32 = readl(RESETCTRL_RESET1_MASK);
	data32 &= (~(0x1 << bit));
	writel(data32, RESETCTRL_RESET1_MASK);
	data32 = readl(RESETCTRL_RESET1_LEVEL);
	data32 &= (~(0x1 << bit));
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(1);
	data32 |= (0x1 << bit);
	writel(data32, RESETCTRL_RESET1_LEVEL);
}

static void dptx_init(int index)
{
	unsigned int data32, pinmux_bit;
	unsigned int reg_dphy_tx_ctrl0, reg_dphy_tx_ctrl1;

	switch (index) {
	case 0:
		reg_dphy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0;
		reg_dphy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1;
		break;
	case 1:
		reg_dphy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL0;
		reg_dphy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL1;
		break;
	default:
		return;
	}

	// Select Data-in
	//data32 = readl(COMBO_DPHY_CNTL0);
	//data32 &= (~((0x1 << 0) | (0x1 << 1)));
	//data32 |= ((0x0 << 0) | (0x1 << 1)); //reg_phy0_din_sel_edp
	//writel(data32, COMBO_DPHY_CNTL0);

	data32 = (0x1 | (0x1 << 4) | (0x0 << 5) | (0x1 << 6) | (0x1 << 7));
	writel(data32, reg_dphy_tx_ctrl1);
	data32 = ((0x4 << 5) | (0x1f << 16));
	writel(data32, reg_dphy_tx_ctrl0); //select clkdiv20 , datachain-sel
	data32 = readl(reg_dphy_tx_ctrl0);
	data32 &= ~(0x1 << 2);  //no pn swap
	writel(data32, reg_dphy_tx_ctrl0);
	// Mux pads in combo-phy -- Lanes 0 through 4
	//data32 = readl(COMBO_DPHY_CNTL1);
	//data32 &= (~(0x3 | (0x3 << 2) | (0x3 << 4) | (0x3 << 6) | (0x3 << 8)));
	//data32 |= (0x1 | (0x1 << 2) | (0x1 << 4) | (0x1 << 6) | (0x1 << 8));
	//writel(data32, COMBO_DPHY_CNTL1); // Select EDP in combo-phy

	//Hot-plug GPIOY_10
	if (index == 0)
		pinmux_bit = 8;
	else
		pinmux_bit = 12;
	data32 = readl(PADCTRL_PIN_MUX_REGK);
	data32 &= (~(0xf << pinmux_bit));
	data32 |= (0x4 << pinmux_bit);
	writel(data32, PADCTRL_PIN_MUX_REGK);

	// Set clock-divider for EDP-APB
	dptx_reg_write(index, EDP_TX_AUX_CLOCK_DIVIDER, 24); // Set Aux channel clk-div: 24MHz
}

static void dptx_set_phy_clk(int index, uint16_t linkclk_x100)
{
	dptx_phy_reset(index);
	dptx_reset(index);
}

static void dptx_set_pixclk_divN(int index, uint32_t N)
{
	unsigned int data32;
	unsigned int sel0; // 1,2,3,4,5,7,8,9,11,13,17,19,23,29,31
	unsigned int sel1; // 1,2,3,4,5,6,7,8,9,13
	unsigned int bit_clk_en, bit_div_sel0, bit_div_sel1, reg_vid_pll_div;

	if (index == 0) {
		bit_clk_en = 24;
		bit_div_sel0 = 0;
		bit_div_sel1 = 4;
		reg_vid_pll_div = COMBO_DPHY_VID_PLL0_DIV;
	} else {
		bit_clk_en = 25;
		bit_div_sel0 = 8;
		bit_div_sel1 = 12;
		reg_vid_pll_div = COMBO_DPHY_VID_PLL1_DIV;
	}

	switch (N) {
	case 10:
		sel0 = 4;
		sel1 = 1;
		break;
	case 13:
		sel0 = 9;
		sel1 = 0;
		break;
	case 18:
		sel0 = 2;
		sel1 = 5;
		break;
	case 64:
		sel0 = 6;
		sel1 = 7;
		break;
	default:
		sel0 = 2;
		sel1 = 1;
		break;
	}

	// Disable edp_div clock
	//Wr(COMBO_DPHY_EDP_PIXEL_CLK_DIV, 0x0);  // Note this register is shared with edp1 so need to set specific bits
	data32 = readl(COMBO_DPHY_EDP_PIXEL_CLK_DIV);
	data32 &= (~(0x1 << bit_clk_en));
	writel(data32, COMBO_DPHY_EDP_PIXEL_CLK_DIV);

	// Disable hdmi clk_div clock output + bypass
	writel((0x0 << 19) | (0x1 << 18), reg_vid_pll_div);

	// Set EDP clock divider
	data32 = readl(COMBO_DPHY_EDP_PIXEL_CLK_DIV);
	data32 &= (~((0xf << bit_div_sel0) | (0xf << bit_div_sel1)));
	data32 |= ((sel0 << bit_div_sel0) | (sel1 << bit_div_sel1));
	writel(data32, COMBO_DPHY_EDP_PIXEL_CLK_DIV);

	// Enable hdmi clk_div clock output + bypass
	writel((0x1 << 19) | (0x1 << 18), reg_vid_pll_div);

	// Enable edp_div clock
	data32 = readl(COMBO_DPHY_EDP_PIXEL_CLK_DIV);
	data32 |= (0x1 << bit_clk_en);
	writel(data32, COMBO_DPHY_EDP_PIXEL_CLK_DIV);

	dptx_reset(index);  // Reset controller : IP is sync reset, may not have been reset if
			// first time video clock
}

static int dptx_wait_phy_ready(int index)
{
	unsigned int data = 0;
	unsigned int done = 100;

	do {
		data = dptx_reg_read(index, EDP_TX_PHY_STATUS);
		if (done < 20) {
			LCDPR("dptx%d wait phy ready: reg_val=0x%x, wait_count=%u\n",
				index, data, (100 - done));
		}
		done--;
		udelay(100);
	}while(((data & 0x7f) != 0x7f) && (done > 0));

	if ((data & 0x7f) == 0x7f)
		return 0;

	LCDPR(" edp%d tx phy error!\n", index);
	return -1;
}

static void edp_power_init(int index)
{
#ifdef CONFIG_SECURE_POWER_CONTROL
//#define PM_EDP0          48
//#define PM_EDP1          49
//#define PM_MIPI_DSI1     50
//#define PM_MIPI_DSI0     41
	if (index == 0)
		pwr_ctrl_psci_smc(PM_EDP0, 1);
	else
		pwr_ctrl_psci_smc(PM_EDP1, 1);
#endif
	LCDPR(" edp power domain on\n");
}

#define EDP_HPD_TIMEOUT    1000
static void edp_tx_init(int index)
{
	unsigned int hpd_state = 0;
	unsigned char edptx_auxdata[2];
	unsigned int offset;
	int ret, i;

	offset = (venc_table[index] << 2);

	edp_power_init(index);

	dptx_reset(index);
	dptx_init(index);  // initialize aux-channel clk_div

	dptx_set_phy_clk(index, 270);  // 2.7GHz link
	dptx_set_pixclk_divN(index, 18);

	dptx_reset(index);
	mdelay(10);

	LCDPR(" edp tx reset done, start host\n");
	writel(0, ENCL_VIDEO_EN + offset);

	// Enable the transmitter
	//dptx_reg_write(index, EDP_TX_TRANSMITTER_OUTPUT_ENABLE, 0x1);
	dptx_reg_write(index, EDP_TX_PHY_RESET, 0);  // remove the reset on the PHY
	dptx_wait_phy_ready(index);
	mdelay(10);
	dptx_reg_write(index, EDP_TX_TRANSMITTER_OUTPUT_ENABLE, 0x1);

	i = 0;
	while (i++ < EDP_HPD_TIMEOUT) {
		hpd_state = dptx_reg_read(index, EDP_TX_AUX_STATE) & 0x1;
		if (hpd_state)
			break;
		mdelay(2);
	}
	LCDPR("HPD state: %d, i=%d\n", hpd_state, i);

	// Program Link-rate and Lane_count
	dptx_reg_write(index, EDP_TX_LINK_BW_SET, 0x0a); // Link-rate
	dptx_reg_write(index, EDP_TX_LINK_COUNT_SET, 0x02); // Number of Lanes

	// Program Lane count
	LCDPR("edp set lane bw & count\n");
	edptx_auxdata[0] = 0x0a; // 2.7GHz
	edptx_auxdata[1] = 2;
	ret = dptx_aux_write(index, 0x100, 2, edptx_auxdata);
	if (ret) {
		LCDPR("..... edp set lane bw & count failed.....\n");
		//return;
	}

	// Power up link
	LCDPR("edp power up link\n");
	edptx_auxdata[0] = 0x1;
	ret = dptx_aux_write(index, 0x600, 1, edptx_auxdata);
	if (ret) {
		LCDPR("..... edp power up link failed.....\n");
		//return;
	}

	// Fast Link train
	dptx_link_fast_training(index);
	//dptx_dpcd_dump();

	dptx_set_msa(index);
	writel(1, ENCL_VIDEO_EN + offset);

	LCDPR("edp enabling the main stream video\n");
	dptx_reg_write(index, EDP_TX_MAIN_STREAM_ENABLE, 0x1); // Enable main-link

	LCDPR(" edp%d tx finish\n", index);
}

void lcd_init_pre_edp(int index)
{
	unsigned int data32, offset = (pll_table[index] << 2);
	unsigned int reg_stts, vid2_clk_ctrl, vid2_clk_div, vid2_clk_ctrl2;
	unsigned int vid_pll_div;
	unsigned int bit_dphy_rst, bit_dphy_din_sel, bit_dphy_lane_sel, lane_sel_val;
	int cnt = 0, i;

	switch (index) {
	case 0:
		reg_stts = ANACTRL_TCON_PLL0_STS;
		vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;
		vid2_clk_div = CLKCTRL_VIID_CLK0_DIV;
		vid2_clk_ctrl2 = CLKCTRL_VID_CLK0_CTRL2;
		vid_pll_div = COMBO_DPHY_VID_PLL0_DIV;
		bit_dphy_rst = 19;
		bit_dphy_din_sel = 0;
		bit_dphy_lane_sel = 0;
		lane_sel_val = 0x155;
		break;
	case 1:
		reg_stts = ANACTRL_TCON_PLL1_STS;
		vid2_clk_ctrl = CLKCTRL_VIID_CLK1_CTRL;
		vid2_clk_div = CLKCTRL_VIID_CLK1_DIV;
		vid2_clk_ctrl2 = CLKCTRL_VID_CLK1_CTRL2;
		vid_pll_div = COMBO_DPHY_VID_PLL1_DIV;
		bit_dphy_rst = 20;
		bit_dphy_din_sel = 2;
		bit_dphy_lane_sel = 16;
		lane_sel_val = 0x155;
		break;
	default:
		LCDERR("%s: invalid index %d\n", __func__, index);
		return;
	}

	//1.config pll
set_pll_retry_edp:
	writel(0x000f04e1, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x200f04e1, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x300f04e1, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x10000000, ANACTRL_TCON_PLL0_CNTL1 + offset);
	udelay(10);
	writel(0x0000110c, ANACTRL_TCON_PLL0_CNTL2 + offset);
	udelay(10);
	writel(0x10051400, ANACTRL_TCON_PLL0_CNTL3 + offset);
	udelay(10);
	writel(0x000100c0, ANACTRL_TCON_PLL0_CNTL4 + offset);
	udelay(10);
	writel(0x008300c0, ANACTRL_TCON_PLL0_CNTL4 + offset);
	udelay(10);
	writel(0x340f04e1, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x140f04e1, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x0000300c, ANACTRL_TCON_PLL0_CNTL2 + offset);
	udelay(100);
	i = 0;
	while (i++ < 200) {
		udelay(50);
		if (readl(reg_stts) & 0x80000000)
			break;
	}
	if (!(readl(reg_stts) & 0x80000000)) {
		if (cnt++ < 20)
			goto set_pll_retry_edp;
		else
			LCDPR(" pll lock failed!!!\n");
	}

	//2.config divider
	data32 = readl(vid2_clk_ctrl);
	//cntrl_clk_en0 disable
	data32 &= ~(1 << 19);
	writel(data32, vid2_clk_ctrl);
	/* Disable the div output clock */
	data32 = readl(vid_pll_div);
	//clk_final_en disable ?
	data32 &= ~(1 << 19);
	writel(data32, vid_pll_div);
	//set_preset disable ?
	data32 &= ~(1 << 15);
	writel(data32, vid_pll_div);

	data32 = readl(vid_pll_div);  //used COMBO not VID
	data32 |= (1 << 18);
	writel(data32, vid_pll_div);
	data32 |= (1 << 19);
	writel(data32, vid_pll_div);

	//3.config vclk
	writel(0x00000000, vid2_clk_div);
	udelay(5);
	writel(0x00080000, vid2_clk_ctrl);
	udelay(5);
	writel(0x00008000, vid2_clk_div);
	writel(0x00018000, vid2_clk_div);
	udelay(5);
	writel(0x00080001, vid2_clk_ctrl);
	writel(0x00088001, vid2_clk_ctrl);
	udelay(10);
	writel(0x00080001, vid2_clk_ctrl);
	udelay(5);
	writel(0x00000008, vid2_clk_ctrl2);

	//4. config phy clk
	//done by edp init

	//5. reset phy
	data32 = readl(RESETCTRL_RESET1_MASK);
	data32 &= ~(0x1 << bit_dphy_rst);
	writel(data32, RESETCTRL_RESET1_MASK);
	data32 = readl(RESETCTRL_RESET1_LEVEL);
	data32 &= ~(0x1 << bit_dphy_rst);
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(1);
	data32 |= (0x1 << bit_dphy_rst);
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(10);

	data32 = readl(COMBO_DPHY_CNTL0);
	data32 &= ~(0x3 << bit_dphy_din_sel);
	data32 |= (0x2 << bit_dphy_din_sel);
	writel(data32, COMBO_DPHY_CNTL0);
	data32 = readl(COMBO_DPHY_CNTL1);
	data32 &= ~(0x3ff << bit_dphy_lane_sel);
	data32 |= (lane_sel_val << bit_dphy_lane_sel);
	writel(data32, COMBO_DPHY_CNTL1);

	//6. config phy
	switch (index) {
	case 1:
		writel(0x46770038, ANACTRL_DIF_PHY_CNTL10);
		writel(0x0000ffff, ANACTRL_DIF_PHY_CNTL11);
		writel(0x16530028, ANACTRL_DIF_PHY_CNTL12);
		writel(0x16530028, ANACTRL_DIF_PHY_CNTL13);
		writel(0x16530028, ANACTRL_DIF_PHY_CNTL14);
		writel(0x16530028, ANACTRL_DIF_PHY_CNTL15);
		break;
	case 0:
	default:
		writel(0x46770038, ANACTRL_DIF_PHY_CNTL1);
		writel(0x0000ffff, ANACTRL_DIF_PHY_CNTL2);
		writel(0x16530028, ANACTRL_DIF_PHY_CNTL3);
		writel(0x16530028, ANACTRL_DIF_PHY_CNTL4);
		writel(0x16530028, ANACTRL_DIF_PHY_CNTL5);
		writel(0x16530028, ANACTRL_DIF_PHY_CNTL6);
		break;
	}
	writel(0x00406253, ANACTRL_DIF_PHY_CNTL19);
	writel(0x0, ANACTRL_DIF_PHY_CNTL20);
	writel(0x0, ANACTRL_DIF_PHY_CNTL21);

	LCDPR(" lcd init pre\n");
}

static void lcd_venc_set_edp(int index)
{
	unsigned int hactive = 1920;
	unsigned int vactive = 1080;
	unsigned int htotal = 2200;
	unsigned int vtotal = 1120;
	unsigned int hsw = 44;
	unsigned int hbp = 148;
	unsigned int vsw = 5;
	unsigned int vbp = 30;
	//unsigned int pclk = 147840000;
	unsigned int data32, offset;

	offset = (venc_table[index] << 2);
	lcd_vcbus_write(ENCL_VIDEO_EN + offset, 0);

	lcd_vcbus_write(ENCL_VIDEO_MODE + offset, 0x8000);/*bit[15] shadown en*/
	lcd_vcbus_write(ENCL_VIDEO_MODE_ADV + offset, 0x0418); /* Sampling rate: 1 */

	lcd_vcbus_write(ENCL_VIDEO_FILT_CTRL + offset, 0x1000); /* bypass filter */
	lcd_vcbus_write(ENCL_VIDEO_MAX_PXCNT + offset, htotal - 1);
	lcd_vcbus_write(ENCL_VIDEO_MAX_LNCNT + offset, vtotal - 1);
	lcd_vcbus_write(ENCL_VIDEO_HAVON_BEGIN + offset, hsw + hbp);
	lcd_vcbus_write(ENCL_VIDEO_HAVON_END + offset, hactive - 1 + hsw + hbp);
	lcd_vcbus_write(ENCL_VIDEO_VAVON_BLINE + offset, vsw + vbp);
	lcd_vcbus_write(ENCL_VIDEO_VAVON_ELINE + offset, vactive - 1 + vsw + vbp);

	lcd_vcbus_write(ENCL_VIDEO_HSO_BEGIN + offset, 0);
	lcd_vcbus_write(ENCL_VIDEO_HSO_END + offset, hsw);
	lcd_vcbus_write(ENCL_VIDEO_VSO_BEGIN + offset, 0);
	lcd_vcbus_write(ENCL_VIDEO_VSO_END + offset, 0);
	lcd_vcbus_write(ENCL_VIDEO_VSO_BLINE + offset, 0);
	lcd_vcbus_write(ENCL_VIDEO_VSO_ELINE + offset, vsw);
	lcd_vcbus_write(ENCL_VIDEO_RGBIN_CTRL + offset, 3); //yuv: 1, rgb: 3

	lcd_vcbus_write(ENCL_INBUF_CNTL1 + offset, (5 << 13) | (hactive - 1));
	lcd_vcbus_write(ENCL_INBUF_CNTL0 + offset, 0x200);
#if 0
	/* default colorbar pattern */
	lcd_vcbus_write(1, ENCL_TST_MDSEL + offset);
	lcd_vcbus_write(0x200, ENCL_TST_Y + offset);
	lcd_vcbus_write(0x200, ENCL_TST_CB + offset);
	lcd_vcbus_write(0x200, ENCL_TST_CR + offset);
	lcd_vcbus_write(hsw + hbp, ENCL_TST_CLRBAR_STRT + offset);
	lcd_vcbus_write(240, ENCL_TST_CLRBAR_WIDTH + offset);
	lcd_vcbus_write(1, ENCL_TST_EN + offset);
	//lcd_vcbus_write(0x0410, ENCL_VIDEO_MODE_ADV + offset);
#endif
	lcd_vcbus_write(ENCL_VIDEO_EN + offset, 1);

	//select venc to edp
	data32 = (0 << 31) | (0 << 30) | (0 << 29) | (1 << 28);
	switch (index) {
	case 0:
		lcd_vcbus_write(VPU_DISP_VIU0_CTRL, data32);
		break;
	case 1:
		lcd_vcbus_write(VPU_DISP_VIU1_CTRL, data32);
		break;
	default:
		break;
	}

	//config venc_tcon
	offset = (venc_data_table[index] << 2);
	lcd_vcbus_write(LCD_RGB_BASE_ADDR + offset, 0x0);
	lcd_vcbus_write(LCD_RGB_COEFF_ADDR + offset, 0x400);
	lcd_vcbus_write(LCD_POL_CNTL_ADDR + offset, (1 << 0));

	/* DE signal */
	offset = (venc_if_table[index] << 2);
	lcd_vcbus_write(DE_HS_ADDR + offset, hsw + hbp);
	lcd_vcbus_write(DE_HE_ADDR + offset, hsw + hbp + hactive);
	lcd_vcbus_write(DE_VS_ADDR + offset, vsw + vbp);
	lcd_vcbus_write(DE_VE_ADDR + offset, vsw + vbp + vactive - 1);

	/* Hsync signal */
	lcd_vcbus_write(HSYNC_HS_ADDR + offset, 0);
	lcd_vcbus_write(HSYNC_HE_ADDR + offset, hsw);
	lcd_vcbus_write(HSYNC_VS_ADDR + offset, 0);
	lcd_vcbus_write(HSYNC_VE_ADDR + offset, vtotal - 1);

	/* Vsync signal */
	lcd_vcbus_write(VSYNC_HS_ADDR + offset, 0);
	lcd_vcbus_write(VSYNC_HE_ADDR + offset, 0);
	lcd_vcbus_write(VSYNC_VS_ADDR + offset, 0);
	lcd_vcbus_write(VSYNC_VE_ADDR + offset, vsw);

	//select encl
	offset = (venc_table[index] << 2);
	lcd_vcbus_write(VPU_VENC_CTRL + offset, 2);

	LCDPR(" lcd venc init\n");
}

static void mipi_dsi_power_init(int index)
{
#ifdef CONFIG_SECURE_POWER_CONTROL
//#define PM_EDP0          48
//#define PM_EDP1          49
//#define PM_MIPI_DSI1     50
//#define PM_MIPI_DSI0     41
	if (index == 0)
		pwr_ctrl_psci_smc(PM_MIPI_DSI0, 1);
	else
		pwr_ctrl_psci_smc(PM_MIPI_DSI1, 1);
#endif
	LCDPR(" mipi-dsi power domain on\n");
}

static void mipi_dsi_reset(int index)
{
	unsigned int data32, bit_host_reset, bit_phy_reset;

	if (index == 0) {
		bit_host_reset = 29;
		bit_phy_reset = 30;
	} else {
		bit_host_reset = 31;
		bit_phy_reset = 28;
	}

	data32 = readl(RESETCTRL_RESET1_MASK);
	data32 &= ~((0x1 << bit_host_reset) | (1 << bit_phy_reset));
	writel(data32, RESETCTRL_RESET1_MASK);
	data32 = readl(RESETCTRL_RESET1_LEVEL);
	data32 &= ~((0x1 << bit_host_reset) | (1 << bit_phy_reset));
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(1);
	data32 |= ((0x1 << bit_host_reset) | (1 << bit_phy_reset));
	writel(data32, RESETCTRL_RESET1_LEVEL);
}

static void mipi_dsi_tx_init(struct aml_lcd_drv_s *pdrv)
{
	mipi_dsi_reset(pdrv->index);

	pdrv->config.timing.bit_rate = 283500000; //378000000;
	pdrv->config.control.mipi_cfg.clk_factor = 6;//8;
	mipi_dsi_tx_ctrl(pdrv, 1);
}

#define MIPI_DSI_1G_TEST    0
static void lcd_init_pre_mipi_dsi(int index)
{
	unsigned int data32, offset = (pll_table[index] << 2);
	unsigned int reg_stts, vid2_clk_ctrl, vid2_clk_div, vid2_clk_ctrl2;
	unsigned int vid_pll_div;
	unsigned int bit_dphy_rst, bit_dphy_lane_sel, lane_sel_val;
	unsigned int bit_dsi_phy_clk_div, bit_dsi_phy_clk_en, bit_dsi_phy_clk_sel;
	unsigned int bit_dsi_meas_clk_div, bit_dsi_meas_clk_en, bit_dsi_meas_clk_sel;
	int cnt = 0, i;

	switch (index) {
	case 0:
		reg_stts = ANACTRL_TCON_PLL0_STS;
		vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;
		vid2_clk_div = CLKCTRL_VIID_CLK0_DIV;
		vid2_clk_ctrl2 = CLKCTRL_VID_CLK0_CTRL2;
		vid_pll_div = COMBO_DPHY_VID_PLL0_DIV;
		bit_dphy_rst = 19;
		bit_dphy_lane_sel = 0;
		lane_sel_val = 0x0;
		bit_dsi_phy_clk_div = 0;
		bit_dsi_phy_clk_en = 8;
		bit_dsi_phy_clk_sel = 12;
		bit_dsi_meas_clk_div = 0;
		bit_dsi_meas_clk_en = 8;
		bit_dsi_meas_clk_sel = 9;
		break;
	case 1:
		reg_stts = ANACTRL_TCON_PLL1_STS;
		vid2_clk_ctrl = CLKCTRL_VIID_CLK1_CTRL;
		vid2_clk_div = CLKCTRL_VIID_CLK1_DIV;
		vid2_clk_ctrl2 = CLKCTRL_VID_CLK1_CTRL2;
		vid_pll_div = COMBO_DPHY_VID_PLL1_DIV;
		bit_dphy_rst = 20;
		bit_dphy_lane_sel = 16;
		lane_sel_val = 0x0;
		bit_dsi_phy_clk_div = 16;
		bit_dsi_phy_clk_en = 24;
		bit_dsi_phy_clk_sel = 25;
		bit_dsi_meas_clk_div = 12;
		bit_dsi_meas_clk_en = 20;
		bit_dsi_meas_clk_sel = 21;
		break;
	default:
		LCDERR("%s: invalid index %d\n", __func__, index);
		return;
	}

#if (MIPI_DSI_1G_TEST == 1)
	/*1.config pll: 3984M*/
set_pll_retry_mipi:
	writel(0x002f04a6, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x202f04a6, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x302f04a6, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x10000000, ANACTRL_TCON_PLL0_CNTL1 + offset);
	udelay(10);
	writel(0x0000110c, ANACTRL_TCON_PLL0_CNTL2 + offset);
	udelay(10);
	writel(0x10051400, ANACTRL_TCON_PLL0_CNTL3 + offset);
	udelay(10);
	writel(0x000100c0, ANACTRL_TCON_PLL0_CNTL4 + offset);
	udelay(10);
	writel(0x008300c0, ANACTRL_TCON_PLL0_CNTL4 + offset);
	udelay(10);
	writel(0x342f04a6, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x142f04a6, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x0000300c, ANACTRL_TCON_PLL0_CNTL2 + offset);
	udelay(100);
	i = 0;
	while (i++ < 200) {
		udelay(50);
		if (readl(reg_stts) & 0x80000000)
			break;
	}
	if (!(readl(reg_stts) & 0x80000000)) {
		if (cnt++ < 20)
			goto set_pll_retry_mipi;
		else
			LCDPR(" pll lock failed!!!\n");
	}

#else
	//1.config pll
set_pll_retry_mipi:
	writel(0x00b704bd, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x20b704bd, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x30b704bd, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x10000000, ANACTRL_TCON_PLL0_CNTL1 + offset);
	udelay(10);
	writel(0x0000110c, ANACTRL_TCON_PLL0_CNTL2 + offset);
	udelay(10);
	writel(0x10051400, ANACTRL_TCON_PLL0_CNTL3 + offset);
	udelay(10);
	writel(0x000100c0, ANACTRL_TCON_PLL0_CNTL4 + offset);
	udelay(10);
	writel(0x008300c0, ANACTRL_TCON_PLL0_CNTL4 + offset);
	udelay(10);
	writel(0x34b704bd, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x14b704bd, ANACTRL_TCON_PLL0_CNTL0 + offset);
	udelay(10);
	writel(0x0000300c, ANACTRL_TCON_PLL0_CNTL2 + offset);
	udelay(100);
	i = 0;
	while (i++ < 200) {
		udelay(50);
		if (readl(reg_stts) & 0x80000000)
			break;
	}
	if (!(readl(reg_stts) & 0x80000000)) {
		if (cnt++ < 20)
			goto set_pll_retry_mipi;
		else
			LCDPR(" pll lock failed!!!\n");
	}
#endif
	//2.config divider
	data32 = readl(vid2_clk_ctrl);
	//cntrl_clk_en0 disable
	data32 &= ~(1 << 19);
	writel(data32, vid2_clk_ctrl);
	/* Disable the div output clock */
	data32 = readl(vid_pll_div);
	//clk_final_en disable ?
	data32 &= ~(1 << 19);
	writel(data32, vid_pll_div);
	//set_preset disable ?
	data32 &= ~(1 << 15);
	writel(data32, vid_pll_div);

	data32 = readl(vid_pll_div);  //used COMBO not VID
	data32 |= (1 << 18);
	writel(data32, vid_pll_div);
	data32 |= (1 << 19);
	writel(data32, vid_pll_div);

	//3.config vclk
	writel(0x00000000, vid2_clk_div);
	writel(0x00000005, vid2_clk_div);
	udelay(5);
	writel(0x00080000, vid2_clk_ctrl);
	udelay(5);
	writel(0x00008005, vid2_clk_div);
	writel(0x00018005, vid2_clk_div);
	udelay(5);
	writel(0x00080001, vid2_clk_ctrl);
	writel(0x00088001, vid2_clk_ctrl);
	udelay(10);
	writel(0x00080001, vid2_clk_ctrl);
	udelay(5);
	writel(0x00000008, vid2_clk_ctrl2);

	//mipi-dsi phy clk
	data32 = readl(CLKCTRL_MIPIDSI_PHY_CLK_CTRL);
	data32 &= ~((0x7 << bit_dsi_phy_clk_sel) |
		    (0x1 << bit_dsi_phy_clk_en) |
		    (0x7f << bit_dsi_phy_clk_div));
	data32 |= ((0x0 << bit_dsi_phy_clk_sel) |
		   (0x1 << bit_dsi_phy_clk_en) |
		   (0x0 << bit_dsi_phy_clk_div));
	writel(data32, CLKCTRL_MIPIDSI_PHY_CLK_CTRL);
	//mipi-dsi meas clk
	data32 = readl(CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL);
	data32 &= ~((0x7 << bit_dsi_meas_clk_sel) |
		    (0x1 << bit_dsi_meas_clk_en) |
		    (0x7f << bit_dsi_meas_clk_div));
	data32 |= ((0x0 << bit_dsi_meas_clk_sel) |
		   (0x1 << bit_dsi_meas_clk_en) |
		   (0x7 << bit_dsi_meas_clk_div));
	writel(data32, CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL);
	writel(data32, CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL);

	//4. reset phy
	data32 = readl(RESETCTRL_RESET1_MASK);
	data32 &= ~(0x1 << bit_dphy_rst);
	writel(data32, RESETCTRL_RESET1_MASK);
	data32 = readl(RESETCTRL_RESET1_LEVEL);
	data32 &= ~(0x1 << bit_dphy_rst);
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(1);
	data32 |= (0x1 << bit_dphy_rst);
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(10);

	//5.select mipi-dsi
	data32 = readl(COMBO_DPHY_CNTL1);
	data32 &= ~(0x3ff << bit_dphy_lane_sel);
	data32 |= (lane_sel_val << bit_dphy_lane_sel);
	writel(data32, COMBO_DPHY_CNTL1);
	//writel(0x0, COMBO_DPHY_CNTL1);

	//6. config phy
	switch (index) {
	case 1:
		writel(0x022a0028, ANACTRL_DIF_PHY_CNTL10);
		writel(0x0000ffcf, ANACTRL_DIF_PHY_CNTL11);
		writel(0x022a0028, ANACTRL_DIF_PHY_CNTL12);
		writel(0x822a0028, ANACTRL_DIF_PHY_CNTL13);
		writel(0x022a0028, ANACTRL_DIF_PHY_CNTL14);
		writel(0x022a0028, ANACTRL_DIF_PHY_CNTL15);
		break;
	case 0:
	default:
		writel(0x022a0028, ANACTRL_DIF_PHY_CNTL1);
		writel(0x0000ffcf, ANACTRL_DIF_PHY_CNTL2);
		writel(0x022a0028, ANACTRL_DIF_PHY_CNTL3);
		writel(0x822a0028, ANACTRL_DIF_PHY_CNTL4);
		writel(0x022a0028, ANACTRL_DIF_PHY_CNTL5);
		writel(0x022a0028, ANACTRL_DIF_PHY_CNTL6);
		break;
	}
	writel(0x1e406253, ANACTRL_DIF_PHY_CNTL19);
	writel(0xffff0000, ANACTRL_DIF_PHY_CNTL20);
	writel(0x0, ANACTRL_DIF_PHY_CNTL21);

	LCDPR(" lcd init pre\n");
}

static void lcd_venc_set_mipi_dsi(int index)
{
	unsigned int hactive = 1024;
	unsigned int vactive = 600;
	unsigned int htotal = 1250;
	unsigned int vtotal = 630;
	unsigned int hsw = 80;
	unsigned int hbp = 100;
	unsigned int vsw = 5;
	unsigned int vbp = 20;
	//unsigned int pclk = 147840000;
	unsigned int data32, offset;

	offset = (venc_table[index] << 2);
	writel(0, ENCL_VIDEO_EN + offset);

	writel(0x8000, ENCL_VIDEO_MODE + offset);/*bit[15] shadown en*/
	writel(0x0418, ENCL_VIDEO_MODE_ADV + offset); /* Sampling rate: 1 */

	writel(0x1000, ENCL_VIDEO_FILT_CTRL + offset); /* bypass filter */
	writel(htotal - 1, ENCL_VIDEO_MAX_PXCNT + offset);
	writel(vtotal - 1, ENCL_VIDEO_MAX_LNCNT + offset);
	writel(hsw + hbp, ENCL_VIDEO_HAVON_BEGIN + offset);
	writel(hactive - 1 + hsw + hbp, ENCL_VIDEO_HAVON_END + offset);
	writel(vsw + vbp, ENCL_VIDEO_VAVON_BLINE + offset);
	writel(vactive - 1 + vsw + vbp, ENCL_VIDEO_VAVON_ELINE + offset);

	writel(0, ENCL_VIDEO_HSO_BEGIN + offset);
	writel(hsw, ENCL_VIDEO_HSO_END + offset);
	writel(0, ENCL_VIDEO_VSO_BEGIN + offset);
	writel(0, ENCL_VIDEO_VSO_END + offset);
	writel(0, ENCL_VIDEO_VSO_BLINE + offset);
	writel(vsw, ENCL_VIDEO_VSO_ELINE + offset);
	writel(3, ENCL_VIDEO_RGBIN_CTRL + offset); //yuv: 1, rgb: 3

	writel((5 << 13) | (hactive - 1), ENCL_INBUF_CNTL1 + offset);
	writel(0x200, ENCL_INBUF_CNTL0 + offset);

	/* default colorbar pattern */
	writel(1, ENCL_TST_MDSEL + offset);
	writel(0x200, ENCL_TST_Y + offset);
	writel(0x200, ENCL_TST_CB + offset);
	writel(0x200, ENCL_TST_CR + offset);
	writel(hsw + hbp, ENCL_TST_CLRBAR_STRT + offset);
	writel(240, ENCL_TST_CLRBAR_WIDTH + offset);
	writel(0, ENCL_TST_EN + offset);
	//writel(0x0410, ENCL_VIDEO_MODE_ADV);

	writel(1, ENCL_VIDEO_EN + offset);

	//select venc to mipi-dsi
	data32 = (0 << 31) | (0 << 30) | (0 << 29) | (1 << 28);
	switch (index) {
	case 0:
		writel(data32, VPU_DISP_VIU0_CTRL);
		break;
	case 1:
		writel(data32, VPU_DISP_VIU1_CTRL);
		break;
	default:
		break;
	}

	//config venc_tcon
	offset = (venc_data_table[index] << 2);
	writel(0x0, LCD_RGB_BASE_ADDR + offset);
	writel(0x400, LCD_RGB_COEFF_ADDR + offset);
	//writel((1 << 0), LCD_POL_CNTL_ADDR);

	/* DE signal */
	offset = (venc_if_table[index] << 2);
	writel(hsw + hbp, DE_HS_ADDR + offset);
	writel(hsw + hbp + hactive, DE_HE_ADDR + offset);
	writel(vsw + vbp, DE_VS_ADDR + offset);
	writel(vsw + vbp + vactive - 1, DE_VE_ADDR + offset);

	/* Hsync signal */
	writel(0, HSYNC_HS_ADDR + offset);
	writel(hsw, HSYNC_HE_ADDR + offset);
	writel(0, HSYNC_VS_ADDR + offset);
	writel(vtotal - 1, HSYNC_VE_ADDR + offset);

	/* Vsync signal */
	writel(0, VSYNC_HS_ADDR + offset);
	writel(0, VSYNC_HE_ADDR + offset);
	writel(0, VSYNC_VS_ADDR + offset);
	writel(vsw, VSYNC_VE_ADDR + offset);

	//select encl
	offset = (venc_table[index] << 2);
	writel(2, VPU_VENC_CTRL + offset);

	LCDPR(" lcd venc init\n");
}

static void vx1_sw_reset(void)
{
	/* force PHY to 0 */
	lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0, 3, 8, 2);
	lcd_vcbus_write(VBO_SOFT_RST, 0x1ff);
	udelay(5);
	/* realease PHY */
	lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0, 0, 8, 2);
	lcd_vcbus_write(VBO_SOFT_RST, 0);
}

#define VX1_HPD_WAIT_TIMEOUT    5000 /* 500ms */
static void vx1_wait_hpd(void)
{
	int i = 0;
	int ret = 1;

	LCDPR("%s:", __func__);
	while (i++ < VX1_HPD_WAIT_TIMEOUT) {
		ret = (lcd_vcbus_read(VBO_STATUS_L) >> 6) & 1;
		if (ret == 0)
			break;
		udelay(100);
	}

	if (ret)
		printf("hpd=%d\n", ((lcd_vcbus_read(VBO_STATUS_L) >> 6) & 1));
	else
		printf("hpd=%d, i=%d\n", ((lcd_vcbus_read(VBO_STATUS_L) >> 6) & 1), i);

	mdelay(10); /* add 10ms delay for compatibility */
}

#define VX1_LOCKN_WAIT_TIMEOUT    500 /* 500ms */
static void vx1_wait_stable(void)
{
	int i = 0;
	int ret = 1;

	while (i++ < VX1_LOCKN_WAIT_TIMEOUT) {
		ret = lcd_vcbus_read(VBO_STATUS_L) & 0x3f;
		if (ret == 0x20)
			break;
		mdelay(1);
	}
	LCDPR("%s status: 0x%x, i=%d\n", __func__,
	      lcd_vcbus_read(VBO_STATUS_L), i);
}

static void vx1_clk_util_set(void)
{
	/* set fifo_clk_sel*/
	lcd_combo_dphy_write(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0, (3 << 5));
	/* set cntl_ser_en:  8-channel to 1 */
	lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0, 0xfff, 16, 8);

	/* decoupling fifo enable, gated clock enable */
	lcd_combo_dphy_write(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1,
			(1 << 6) | (1 << 0));
	/* decoupling fifo write enable after fifo enable */
	lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1, 1, 7, 1);
}

static int vx1_lanes_set(unsigned int lane_num, unsigned int region_num)
{
	unsigned int sublane_num, tmp;
	unsigned int region_size[4];

	sublane_num = lane_num / region_num; /* lane num in each region */
	lcd_vcbus_setb(VBO_LANES, (8 - 1), 0, 3);
	lcd_vcbus_setb(VBO_LANES, (2 - 1), 4, 2);
	lcd_vcbus_setb(VBO_LANES, (sublane_num - 1), 8, 3);
	lcd_vcbus_setb(VBO_LANES, (4 - 1), 11, 2);

	region_size[3] = (3840 / 8) * sublane_num;
	tmp = (3840 % lane_num);
	region_size[0] = region_size[3] + (((tmp / sublane_num) > 0) ?
		sublane_num : (tmp % sublane_num));
	region_size[1] = region_size[3] + (((tmp / sublane_num) > 1) ?
		sublane_num : (tmp % sublane_num));
	region_size[2] = region_size[3] + (((tmp / sublane_num) > 2) ?
		sublane_num : (tmp % sublane_num));
	lcd_vcbus_write(VBO_REGION_00, region_size[0]);
	lcd_vcbus_write(VBO_REGION_01, region_size[1]);
	lcd_vcbus_write(VBO_REGION_02, region_size[2]);
	lcd_vcbus_write(VBO_REGION_03, region_size[3]);

	lcd_vcbus_write(VBO_ACT_VSIZE, 2160);
	/* different from FBC code!!! */
	/* lcd_vcbus_setb(VBO_CTRL_H,0x80,11,5); */
	/* different from simulation code!!! */
	lcd_vcbus_setb(VBO_CTRL_H, 0x0, 0, 4);
	lcd_vcbus_setb(VBO_CTRL_H, 0x1, 9, 1);
	/* lcd_vcbus_setb(VBO_CTRL_L,enable,0,1); */

	return 0;
}

static void vx1_sync_pol(int hsync_pol, int vsync_pol)
{
	lcd_vcbus_setb(VBO_VIN_CTRL, hsync_pol, 4, 1);
	lcd_vcbus_setb(VBO_VIN_CTRL, vsync_pol, 5, 1);

	lcd_vcbus_setb(VBO_VIN_CTRL, hsync_pol, 6, 1);
	lcd_vcbus_setb(VBO_VIN_CTRL, vsync_pol, 7, 1);
}

static void vx1_wait_timing_stable(void)
{
	unsigned int timing_state;
	int i = 200;

	timing_state = lcd_vcbus_read(VBO_INTR_STATE) & 0x1ff;
	while ((timing_state) && (i > 0)) {
		/* clear video timing error intr */
		lcd_vcbus_setb(VBO_INTR_STATE_CTRL, 0x7, 0, 3);
		lcd_vcbus_setb(VBO_INTR_STATE_CTRL, 0, 0, 3);
		mdelay(2);
		timing_state = lcd_vcbus_read(VBO_INTR_STATE) & 0x1ff;
		i--;
	};

	LCDPR("vbyone timing state: 0x%03x, i=%d\n",
		timing_state, (200 - i));

	mdelay(2);
}

static void vx1_control_set(unsigned int lane_count, unsigned int region_num)
{
	/* int color_fmt; */
	int vin_color, vin_bpp;

	LCDPR("%s\n", __func__);

	vx1_clk_util_set();

	vin_color = 4; /* fixed RGB */
	vin_bpp   = 0; /* fixed 30bbp 4:4:4 */


	/* set Vbyone vin color format */
	lcd_vcbus_setb(VBO_VIN_CTRL, vin_color, 8, 3);
	lcd_vcbus_setb(VBO_VIN_CTRL, vin_bpp, 11, 2);

	vx1_lanes_set(lane_count, region_num);
	/*set hsync/vsync polarity to let the polarity is low active
	inside the VbyOne */
	vx1_sync_pol(0, 0);

	/* below line copy from simulation */
	/* gate the input when vsync asserted */
	lcd_vcbus_setb(VBO_VIN_CTRL, 1, 0, 2);
	/* lcd_vcbus_write(VBO_VBK_CTRL_0,0x13);
	//lcd_vcbus_write(VBO_VBK_CTRL_1,0x56);
	//lcd_vcbus_write(VBO_HBK_CTRL,0x3478);
	//lcd_vcbus_setb(VBO_PXL_CTRL,0x2,0,4);
	//lcd_vcbus_setb(VBO_PXL_CTRL,0x3,VBO_PXL_CTR1_BIT,VBO_PXL_CTR1_WID);
	//set_vbyone_ctlbits(1,0,0); */
	/* VBO_RGN_GEN clk always on */
	lcd_vcbus_setb(VBO_GCLK_MAIN, 2, 2, 2);

	/* PAD select: */
	if ((lane_count == 1) || (lane_count == 2))
		lcd_vcbus_setb(LCD_PORT_SWAP, 1, 9, 2);
	else if (lane_count == 4)
		lcd_vcbus_setb(LCD_PORT_SWAP, 2, 9, 2);
	else
		lcd_vcbus_setb(LCD_PORT_SWAP, 0, 9, 2);
	/* lcd_vcbus_setb(LCD_PORT_SWAP, 1, 8, 1);//reverse lane output order */

	/* Mux pads in combo-phy: 0 for dsi; 1 for lvds or vbyone; 2 for edp */
	//lcd_hiu_write(HHI_DSI_LVDS_EDP_CNTL0, 0x1);

	lcd_vcbus_setb(VBO_INSGN_CTRL, 0, 2, 2);

	lcd_vcbus_setb(VBO_CTRL_L, 1, 0, 1);

	/*force vencl clk enable, otherwise, it might auto turn off by mipi DSI
	//lcd_vcbus_setb(VPU_MISC_CTRL, 1, 0, 1); */

	vx1_wait_timing_stable();
	vx1_sw_reset();
}

static void vx1_tx_init(void)
{
	unsigned int data32;

	//Hot-plug GPIO
	data32 = readl(PADCTRL_PIN_MUX_REGK);
	data32 &= (~((0xf << 8) | (0xf << 16)));
	data32 |= ((0x3 << 8) | (0x3 << 16));
	writel(data32, PADCTRL_PIN_MUX_REGK);

	vx1_control_set(8, 2); /* lane_num, region_num */
	vx1_wait_hpd();
	vx1_wait_stable();

	LCDPR(" vx1 tx finish\n");
}

static void lcd_init_pre_vx1(void)
{
	unsigned int data32;
	int cnt = 0, i;

	//1.config pll
set_pll_retry_vx1:
	writel(0x000f04f7, ANACTRL_TCON_PLL0_CNTL0);//ok
	udelay(10);
	writel(0x200f04f7, ANACTRL_TCON_PLL0_CNTL0);//ok
	udelay(10);
	writel(0x300f04f7, ANACTRL_TCON_PLL0_CNTL0);//ok
	udelay(10);
	writel(0x10110000, ANACTRL_TCON_PLL0_CNTL1);//ok
	udelay(10);
	writel(0x00001108, ANACTRL_TCON_PLL0_CNTL2);//ok
	udelay(10);
	writel(0x10051400, ANACTRL_TCON_PLL0_CNTL3);//ok
	udelay(10);
	writel(0x010100c0, ANACTRL_TCON_PLL0_CNTL4);//ok
	udelay(10);
	writel(0x038300c0, ANACTRL_TCON_PLL0_CNTL4);//ok
	udelay(10);
	writel(0x340f04f7, ANACTRL_TCON_PLL0_CNTL0);//ok
	udelay(10);
	writel(0x140f04f7, ANACTRL_TCON_PLL0_CNTL0);//ok
	udelay(10);
	writel(0x0000300c, ANACTRL_TCON_PLL0_CNTL2);//ok
	udelay(100);
	i = 0;
	while (i++ < 200) {
		udelay(50);
		if (readl(ANACTRL_TCON_PLL0_STS) & 0x80000000)
			break;
	}
	if (!(readl(ANACTRL_TCON_PLL0_STS) & 0x80000000)) {
		if (cnt++ < 20)
			goto set_pll_retry_vx1;
		else
			LCDPR(" pll lock failed!!!\n");
	}

	//2.config divider
	data32 = readl(CLKCTRL_VIID_CLK0_CTRL);
	//cntrl_clk_en0 disable
	data32 &= ~(1 << 19);
	writel(data32, CLKCTRL_VIID_CLK0_CTRL);
	/* Disable the div output clock */
	data32 = readl(COMBO_DPHY_VID_PLL0_DIV);
	//clk_final_en disable ?
	data32 &= ~(1 << 19);
	writel(data32, COMBO_DPHY_VID_PLL0_DIV);
	//set_preset disable ?
	data32 &= ~(1 << 15);
	writel(data32, COMBO_DPHY_VID_PLL0_DIV);

	lcd_clk_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 18, 1);
	lcd_clk_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 16, 2);
	lcd_clk_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 15, 1);
	lcd_clk_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 0, 14);

	lcd_clk_setb(COMBO_DPHY_VID_PLL0_DIV, 2, 16, 2);
	lcd_clk_setb(COMBO_DPHY_VID_PLL0_DIV, 1, 15, 1);
	lcd_clk_setb(COMBO_DPHY_VID_PLL0_DIV, 0x739c, 0, 15);
	lcd_clk_setb(COMBO_DPHY_VID_PLL0_DIV, 0, 15, 1);

	data32 = readl(COMBO_DPHY_VID_PLL0_DIV);  //used COMBO not VID
	data32 |= (1 << 19);
	writel(data32, COMBO_DPHY_VID_PLL0_DIV);

	//3.config vclk
	writel(0x00000000, CLKCTRL_VIID_CLK0_DIV);
	udelay(5);
	writel(0x00080000, CLKCTRL_VIID_CLK0_CTRL);
	udelay(5);
	writel(0x00008000, CLKCTRL_VIID_CLK0_DIV);
	writel(0x00018000, CLKCTRL_VIID_CLK0_DIV);
	udelay(5);
	writel(0x00080001, CLKCTRL_VIID_CLK0_CTRL);
	writel(0x00088001, CLKCTRL_VIID_CLK0_CTRL);
	udelay(10);
	writel(0x00080001, CLKCTRL_VIID_CLK0_CTRL);
	udelay(5);
	writel(0x00000008, CLKCTRL_VID_CLK0_CTRL2);

	//4. reset phy
	data32 = readl(RESETCTRL_RESET1_MASK);
	data32 &= ~((0x1 << 20) | (0x1 << 19) | (0x1 << 7));
	writel(data32, RESETCTRL_RESET1_MASK);
	data32 = readl(RESETCTRL_RESET1_LEVEL);
	data32 &= ~((0x1 << 20) | (0x1 << 19) | (0x1 << 7));
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(1);
	data32 |= (0x1 << 20) | (0x1 << 19) | (0x1 << 7);
	writel(data32, RESETCTRL_RESET1_LEVEL);
	udelay(10);

	//5.select vx1
	//to analog 8 lane  0=mipi(venc0/1);1=vx1/lvds/edp(venc0/1);2=lvds(only venc2)
	writel(0x15, COMBO_DPHY_CNTL0);
	writel(0x55555555, COMBO_DPHY_CNTL1);

	//6. config phy
	writel(0x26430028, ANACTRL_DIF_PHY_CNTL1);
	writel(0x0000ffff, ANACTRL_DIF_PHY_CNTL2);
	writel(0x26530028, ANACTRL_DIF_PHY_CNTL3);
	writel(0x26530028, ANACTRL_DIF_PHY_CNTL4);
	writel(0x26530028, ANACTRL_DIF_PHY_CNTL5);
	writel(0x26530028, ANACTRL_DIF_PHY_CNTL6);
	writel(0x26530028, ANACTRL_DIF_PHY_CNTL7);
	writel(0x26530028, ANACTRL_DIF_PHY_CNTL8);
	writel(0x26530028, ANACTRL_DIF_PHY_CNTL9);
	writel(0x0, ANACTRL_DIF_PHY_CNTL10);
	writel(0x0, ANACTRL_DIF_PHY_CNTL11);
	writel(0x0, ANACTRL_DIF_PHY_CNTL12);
	writel(0x0, ANACTRL_DIF_PHY_CNTL13);
	writel(0x0, ANACTRL_DIF_PHY_CNTL14);
	writel(0x0, ANACTRL_DIF_PHY_CNTL15);
	writel(0x00401648, ANACTRL_DIF_PHY_CNTL19);
	writel(0x0, ANACTRL_DIF_PHY_CNTL20);
	writel(0x0, ANACTRL_DIF_PHY_CNTL21);

	LCDPR(" vx1 init pre\n");
}

static void lcd_venc_set_vx1(void)
{
	unsigned int hactive = 3840;
	unsigned int vactive = 2160;
	unsigned int htotal = 4400;
	unsigned int vtotal = 2250;
	unsigned int hsw = 33;
	unsigned int hbp = 477;
	unsigned int vsw = 6;
	unsigned int vbp = 65;
	unsigned int data32;

	writel(0, ENCL_VIDEO_EN);

	writel(0x8000, ENCL_VIDEO_MODE);/*bit[15] shadown en*/
	writel(0x0418, ENCL_VIDEO_MODE_ADV); /* Sampling rate: 1 */

	writel(0x1000, ENCL_VIDEO_FILT_CTRL); /* bypass filter */
	writel(htotal - 1, ENCL_VIDEO_MAX_PXCNT);
	writel(vtotal - 1, ENCL_VIDEO_MAX_LNCNT);
	writel(hsw + hbp, ENCL_VIDEO_HAVON_BEGIN);
	writel(hactive - 1 + hsw + hbp, ENCL_VIDEO_HAVON_END);
	writel(vsw + vbp, ENCL_VIDEO_VAVON_BLINE);
	writel(vactive - 1 + vsw + vbp, ENCL_VIDEO_VAVON_ELINE);

	writel(0, ENCL_VIDEO_HSO_BEGIN);
	writel(hsw, ENCL_VIDEO_HSO_END);
	writel(0, ENCL_VIDEO_VSO_BEGIN);
	writel(0, ENCL_VIDEO_VSO_END);
	writel(0, ENCL_VIDEO_VSO_BLINE);
	writel(vsw, ENCL_VIDEO_VSO_ELINE);
	writel(3, ENCL_VIDEO_RGBIN_CTRL); //yuv: 1, rgb: 3

	writel((5 << 13) | (hactive - 1), ENCL_INBUF_CNTL1);
	writel(0x200, ENCL_INBUF_CNTL0);

	/* default colorbar pattern */
	writel(1, ENCL_TST_MDSEL);
	writel(0x200, ENCL_TST_Y);
	writel(0x200, ENCL_TST_CB);
	writel(0x200, ENCL_TST_CR);
	writel(hsw + hbp, ENCL_TST_CLRBAR_STRT);
	writel(240, ENCL_TST_CLRBAR_WIDTH);
	writel(1, ENCL_TST_EN);
	//writel(0x0410, ENCL_VIDEO_MODE_ADV);

	writel(1, ENCL_VIDEO_EN);

	//select venc to vx1
	data32 = (0 << 31) | (1 << 30) | (0 << 29) | (0 << 28);
	writel(data32, VPU_DISP_VIU0_CTRL);

	//config venc_tcon
	writel(0x0, LCD_RGB_BASE_ADDR);
	writel(0x400, LCD_RGB_COEFF_ADDR);
	//writel((1 << 0), LCD_POL_CNTL_ADDR);

	/* DE signal */
	writel(hsw + hbp, DE_HS_ADDR);
	writel(hsw + hbp + hactive, DE_HE_ADDR);
	writel(vsw + vbp, DE_VS_ADDR);
	writel(vsw + vbp + vactive - 1, DE_VE_ADDR);

	/* Hsync signal */
	writel(0, HSYNC_HS_ADDR);
	writel(hsw, HSYNC_HE_ADDR);
	writel(0, HSYNC_VS_ADDR);
	writel(vtotal - 1, HSYNC_VE_ADDR);

	/* Vsync signal */
	writel(0, VSYNC_HS_ADDR);
	writel(0, VSYNC_HE_ADDR);
	writel(0, VSYNC_VS_ADDR);
	writel(vsw, VSYNC_VE_ADDR);

	//select encl
	writel(2, VPU_VENC_CTRL);

	LCDPR(" lcd venc init\n");
}

static void lcd_init_pre_lvds(void)
{
	unsigned int data32;
	int cnt = 0, i;

	//1.config pll
set_pll_retry_lvds:
	lcd_ana_write(ANACTRL_TCON_PLL2_CNTL0, 0x008e04ad);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL2_CNTL0, 0x208e04ad);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL2_CNTL0, 0x308e04ad);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL2_CNTL1, 0x10108000);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL2_CNTL2, 0x0000110c);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL2_CNTL3, 0x10051400);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL2_CNTL4, 0x000100c0);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL2_CNTL4, 0x008300c0);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL2_CNTL0, 0x348e04ad);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL2_CNTL0, 0x148e04ad);
	udelay(10);
	lcd_ana_write(ANACTRL_TCON_PLL2_CNTL2, 0x0000300c);
	//udelay(100);
	//lcd_ana_write(ANACTRL_TCON_PLL2_CNTL4, 0x0b8300c0);
	udelay(10);
	i = 0;
	while (i++ < 200) {
		udelay(50);
		if (lcd_ana_read(ANACTRL_TCON_PLL2_STS) & 0x80000000)
			break;
	}
	if (!(lcd_ana_read(ANACTRL_TCON_PLL2_STS) & 0x80000000)) {
		if (cnt++ < 20)
			goto set_pll_retry_lvds;
		else
			LCDPR(" pll lock failed!!!\n");
	}

	//2.config divider
	data32 = lcd_clk_read(CLKCTRL_VIID_CLK2_CTRL);
	data32 &= ~(1 << 19);
	lcd_clk_write(CLKCTRL_VIID_CLK2_CTRL, data32);
	/* Disable the div output clock */
	data32 = lcd_combo_dphy_read(COMBO_DPHY_VID_PLL2_DIV);
	data32 &= ~(1 << 19);
	lcd_combo_dphy_write(COMBO_DPHY_VID_PLL2_DIV, data32);
	data32 &= ~(1 << 15);
	lcd_combo_dphy_write(COMBO_DPHY_VID_PLL2_DIV, data32);

	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL2_DIV, 0, 18, 1);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL2_DIV, 0, 16, 2);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL2_DIV, 0, 15, 1);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL2_DIV, 0, 0, 14);

	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL2_DIV, 1, 16, 2);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL2_DIV, 1, 15, 1);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL2_DIV, 0x3c78, 0, 15);
	lcd_combo_dphy_setb(COMBO_DPHY_VID_PLL2_DIV, 0, 15, 1);
	data32 = lcd_combo_dphy_read(COMBO_DPHY_VID_PLL2_DIV);

	data32 |= (1 << 19);
	lcd_combo_dphy_write(COMBO_DPHY_VID_PLL2_DIV, data32);

	//3.config vclk
	lcd_clk_write(CLKCTRL_VIID_CLK2_DIV, 0x00000000);
	udelay(5);
	lcd_clk_write(CLKCTRL_VIID_CLK2_CTRL, 0x00080000);
	udelay(5);
	lcd_clk_write(CLKCTRL_VIID_CLK2_DIV, 0x00008000);
	lcd_clk_write(CLKCTRL_VIID_CLK2_DIV, 0x00018000);
	udelay(5);
	lcd_clk_write(CLKCTRL_VIID_CLK2_CTRL, 0x00080001);
	lcd_clk_write(CLKCTRL_VIID_CLK2_CTRL, 0x00088001);
	udelay(10);
	lcd_clk_write(CLKCTRL_VIID_CLK2_CTRL, 0x00080001);
	udelay(5);
	lcd_clk_write(CLKCTRL_VID_CLK2_CTRL2, 0x00000008);

	//4. config phy clk
	//done by edp init

	//5. reset phy
	data32 = lcd_reset_read(RESETCTRL_RESET1_MASK);
	data32 &= ~((0x1 << 20) | (0x1 << 19) | (0x1 << 7));
	lcd_reset_write(RESETCTRL_RESET1_MASK, data32);
	data32 = lcd_reset_read(RESETCTRL_RESET1_LEVEL);
	data32 &= ~((0x1 << 20) | (0x1 << 19) | (0x1 << 7));
	lcd_reset_write(RESETCTRL_RESET1_LEVEL, data32);
	udelay(1);
	data32 |= (0x1 << 20) | (0x1 << 19) | (0x1 << 7);
	lcd_reset_write(RESETCTRL_RESET1_LEVEL, data32);
	udelay(10);
	lcd_combo_dphy_write(COMBO_DPHY_CNTL0, 0x10);
	lcd_combo_dphy_write(COMBO_DPHY_CNTL1, 0xaaaaaaaa);

	//6. config phy
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL1, 0xc6770038);//ch0
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL2, 0x0100ffff);//ch0_aux
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL3, 0x16530028);//ch1
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL4, 0x16530028);//ch2
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL5, 0x16530028);//ch3
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL6, 0x16530028);//ch4
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL7, 0x16530028);//ch5
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL8, 0x16530028);//ch6
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL9, 0x16530028);//ch7
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL10, 0x16430028);//ch8
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL11, 0x0100ffff);//ch8 AUX
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL12, 0x16530028);//ch9
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL13, 0x16530028);//ch10
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL14, 0x16530028);//ch11
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL15, 0x16530028);//ch12
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL16, 0x16530028);//ch13
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL17, 0x16530028);//ch14
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL19, 0x00406253);//COMMON
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL20, 0x0);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL21, 0x0);

	LCDPR(" lcd init pre\n");
}

static void lvds_init(void)
{
	/* set fifo_clk_sel: div 7 */
	lcd_combo_dphy_write(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0, (1 << 5));
	/* set cntl_ser_en:  8-channel to 1 */
	lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0, 0x3ff, 16, 10);

	/* decoupling fifo enable, gated clock enable */
	lcd_combo_dphy_write(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1,
			(1 << 6) | (1 << 0));
	/* decoupling fifo write enable after fifo enable */
	lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1, 1, 7, 1);

	lcd_vcbus_write(LVDS_SER_EN + (0x600 << 2), 0xfff );
	lcd_vcbus_write(LVDS_PACK_CNTL_ADDR + (0x600 << 2),
			(1 << 0) | // repack //[1:0]
			(0 << 3) |		// reserve
			(0 << 4) |		// lsb first
			(0 << 5) |	// pn swap
			(1 << 6) |	// dual port
			(0 << 7) |		// use tcon control
			(1 << 8) |	// 0:10bits, 1:8bits, 2:6bits, 3:4bits.
			(0 << 10) |		//r_select  //0:R, 1:G, 2:B, 3:0
			(1 << 12) |		//g_select  //0:R, 1:G, 2:B, 3:0
			(2 << 14));		//b_select  //0:R, 1:G, 2:B, 3:0;

	lcd_vcbus_write(LVDS_GEN_CNTL + (0x600 << 2),
		(lcd_vcbus_read(LVDS_GEN_CNTL + (0x600 << 2)) | (1 << 4) | (0x3 << 0)));
	lcd_vcbus_setb(LVDS_GEN_CNTL + (0x600 << 2), 1, 3, 1);

	lcd_vcbus_write(P2P_CH_SWAP0+(0x600<<2) , 0x87643210);//ch5 data4 none for LVDS
	lcd_vcbus_write(P2P_CH_SWAP1+(0x600<<2) , 0x000000a9);//chb data4 none for LVDS
	LCDPR("CH swap: 0x%x=0x%x, 0x%x=0x%x\n",
		P2P_CH_SWAP0+(0x600<<2), lcd_vcbus_read(P2P_CH_SWAP0+(0x600<<2)),
		P2P_CH_SWAP1+(0x600<<2), lcd_vcbus_read(P2P_CH_SWAP1+(0x600<<2)));
	lcd_vcbus_write(P2P_BIT_REV+(0x600<<2) , 2);
	LCDPR("bit rev: 0x%x=0x%x\n",
		P2P_BIT_REV+(0x600<<2), lcd_vcbus_read(P2P_BIT_REV+(0x600<<2)));

	LCDPR(" lvds init finish\n");
}

static void lcd_venc_set_lvds(void)
{
	unsigned int hactive = 1920;
	unsigned int vactive = 1080;
	unsigned int htotal = 2200;
	unsigned int vtotal = 1120;
	unsigned int hsw = 44;
	unsigned int hbp = 148;
	unsigned int vsw = 5;
	unsigned int vbp = 30;
	//unsigned int pclk = 147840000;
	unsigned int data32;

	lcd_vcbus_write(ENCL_VIDEO_EN + (0x800 << 2), 0);

	lcd_vcbus_write(ENCL_VIDEO_MODE + (0x800 << 2), 0x8000);/*bit[15] shadown en*/
	lcd_vcbus_write(ENCL_VIDEO_MODE_ADV + (0x800 << 2), 0x0418); /* Sampling rate: 1 */

	lcd_vcbus_write(ENCL_VIDEO_FILT_CTRL + (0x800 << 2), 0x1000); /* bypass filter */
	lcd_vcbus_write(ENCL_VIDEO_MAX_PXCNT + (0x800 << 2), htotal - 1);
	lcd_vcbus_write(ENCL_VIDEO_MAX_LNCNT + (0x800 << 2), vtotal - 1);
	lcd_vcbus_write(ENCL_VIDEO_HAVON_BEGIN + (0x800 << 2), hsw + hbp);
	lcd_vcbus_write(ENCL_VIDEO_HAVON_END + (0x800 << 2), hactive - 1 + hsw + hbp);
	lcd_vcbus_write(ENCL_VIDEO_VAVON_BLINE + (0x800 << 2), vsw + vbp);
	lcd_vcbus_write(ENCL_VIDEO_VAVON_ELINE + (0x800 << 2), vactive - 1 + vsw + vbp);

	lcd_vcbus_write(ENCL_VIDEO_HSO_BEGIN + (0x800 << 2), 0);
	lcd_vcbus_write(ENCL_VIDEO_HSO_END + (0x800 << 2), hsw);
	lcd_vcbus_write(ENCL_VIDEO_VSO_BEGIN + (0x800 << 2), 0);
	lcd_vcbus_write(ENCL_VIDEO_VSO_END + (0x800 << 2), 0);
	lcd_vcbus_write(ENCL_VIDEO_VSO_BLINE + (0x800 << 2), 0);
	lcd_vcbus_write(ENCL_VIDEO_VSO_ELINE + (0x800 << 2), vsw);
	lcd_vcbus_write(ENCL_VIDEO_RGBIN_CTRL + (0x800 << 2), 3); //yuv: 1, rgb: 3

	lcd_vcbus_write(ENCL_INBUF_CNTL1 + (0x800 << 2), (5 << 13) | (hactive - 1));
	lcd_vcbus_write(ENCL_INBUF_CNTL0 + (0x800 << 2), 0x200);

	/* default colorbar pattern */
	lcd_vcbus_write(ENCL_TST_MDSEL + (0x800 << 2), 1);
	lcd_vcbus_write(ENCL_TST_Y + (0x800 << 2), 0x200);
	lcd_vcbus_write(ENCL_TST_CB + (0x800 << 2), 0x200);
	lcd_vcbus_write(ENCL_TST_CR + (0x800 << 2), 0x200);
	lcd_vcbus_write(ENCL_TST_CLRBAR_STRT + (0x800 << 2), hsw + hbp);
	lcd_vcbus_write(ENCL_TST_CLRBAR_WIDTH + (0x800 << 2), 240);
	lcd_vcbus_write(ENCL_TST_EN + (0x800 << 2), 0);

	lcd_vcbus_write(ENCL_VIDEO_EN + (0x800 << 2), 1);

	//select venc to lvds
	data32 = (1 << 31) | (0 << 30) | (0 << 29) | (0 << 28);
	lcd_vcbus_write(VPU_DISP_VIU2_CTRL, data32);

	//config venc_tcon
	lcd_vcbus_write(LCD_RGB_BASE_ADDR + (0x200 << 2), 0x0);
	lcd_vcbus_write(LCD_RGB_COEFF_ADDR + (0x200 << 2), 0x400);
	lcd_vcbus_write(LCD_POL_CNTL_ADDR + (0x200 << 2), (1 << 0));

	/* DE signal */
	lcd_vcbus_write(DE_HS_ADDR + (0x600 << 2), hsw + hbp);
	lcd_vcbus_write(DE_HE_ADDR + (0x600 << 2), hsw + hbp + hactive);
	lcd_vcbus_write(DE_VS_ADDR + (0x600 << 2), vsw + vbp);
	lcd_vcbus_write(DE_VE_ADDR + (0x600 << 2), vsw + vbp + vactive - 1);

	/* Hsync signal */
	lcd_vcbus_write(HSYNC_HS_ADDR + (0x600 << 2), 0);
	lcd_vcbus_write(HSYNC_HE_ADDR + (0x600 << 2), hsw);
	lcd_vcbus_write(HSYNC_VS_ADDR + (0x600 << 2), 0);
	lcd_vcbus_write(HSYNC_VE_ADDR + (0x600 << 2), vtotal - 1);

	/* Vsync signal */
	lcd_vcbus_write(VSYNC_HS_ADDR + (0x600 << 2), 0);
	lcd_vcbus_write(VSYNC_HE_ADDR + (0x600 << 2), 0);
	lcd_vcbus_write(VSYNC_VS_ADDR + (0x600 << 2), 0);
	lcd_vcbus_write(VSYNC_VE_ADDR + (0x600 << 2), vsw);

	//select encl
	lcd_vcbus_write(VPU_VENC_CTRL + (0x800 << 2), 2);

	LCDPR(" lcd venc init\n");
}

void lcd_display_init_test(struct aml_lcd_drv_s *pdrv)
{
	int index = pdrv->index;

	if (pdrv->config.basic.lcd_type == LCD_LVDS) {
		if (index == 2) {
			run_command("gpio set GPIOY_14", 0);
			mdelay(20);

			lcd_vcbus_write(VPU_VIU_VENC_MUX_CTRL, 0x6);
			lcd_init_pre_lvds();
			lcd_venc_set_lvds();
			mdelay(100);
			lvds_init();
			LCDPR("lvds%d init test done\n", index);
		}
	} else if (pdrv->config.basic.lcd_type == LCD_EDP) {
		if (index == 0) {
			run_command("gpio set GPIOY_0", 0);
			run_command("gpio set GPIOY_1", 0);
			run_command("gpio set GPIOY_12", 0);
		} else {
			run_command("gpio set GPIOY_14", 0);
			run_command("gpio set GPIOY_8", 0);
			run_command("gpio set GPIOY_13", 0);
		}
		LCDPR("%s: edp index=%d\n", __func__, index);
		mdelay(50);

		lcd_vcbus_write(VPU_VIU_VENC_MUX_CTRL, 0x24);
		lcd_init_pre_edp(index);
		lcd_venc_set_edp(index);
		mdelay(100);
		edp_tx_init(index);
		LCDPR("edp%d init test done\n", index);
	} else if (pdrv->config.basic.lcd_type == LCD_MIPI) {
		if (index == 0) {
			run_command("gpio clear GPIOY_1", 0); //bl_pwm
			run_command("gpio set GPIOY_12", 0); //bl_en

			run_command("gpio clear GPIOY_4", 0);
			run_command("gpio clear GPIOY_0", 0);
			mdelay(500);
			run_command("gpio set GPIOY_0", 0);
			mdelay(100);
			run_command("gpio set GPIOY_4", 0);
		} else {
			run_command("gpio clear GPIOY_8", 0); //bl_pwm
			run_command("gpio set GPIOY_13", 0); //bl_en

			run_command("gpio clear GPIOY_6", 0);
			run_command("gpio clear GPIOY_14", 0);
			mdelay(500);
			run_command("gpio set GPIOY_14", 0);
			mdelay(100);
			run_command("gpio set GPIOY_6", 0);
		}
		mdelay(20);

		lcd_vcbus_write(VPU_VIU_VENC_MUX_CTRL, 0x24);
		lcd_init_pre_mipi_dsi(index);
		lcd_venc_set_mipi_dsi(index);
		mdelay(20);
		mipi_dsi_power_init(index);
		mipi_dsi_tx_init(pdrv);
		LCDPR("mipi_dsi init test done\n");
	} else if (pdrv->config.basic.lcd_type == LCD_VBYONE) {
		if (index == 0) {
			run_command("gpio set GPIOY_0", 0);
			mdelay(50);

			lcd_vcbus_write(VPU_VIU_VENC_MUX_CTRL, 0x24);
			lcd_init_pre_vx1();
			lcd_venc_set_vx1();
			mdelay(20);
			vx1_tx_init();
			LCDPR("vbyone%d init test done\n", index);
		}
	}
	pdrv->status |= (LCD_STATUS_IF_ON | LCD_STATUS_ENCL_ON);
}

void lcd_display_init_reg_dump(struct aml_lcd_drv_s *pdrv)
{
	int index = pdrv->index;
	unsigned int offset;
	unsigned int reg_stts, vid2_clk_ctrl, vid2_clk_div, vid2_clk_ctrl2;
	unsigned int vid_pll_div, vpu_disp_ctrl;
	unsigned int reg_dphy_tx_ctrl0, reg_dphy_tx_ctrl1;

	if (pdrv->config.basic.lcd_type == LCD_LVDS) {
		printf("pll regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ANACTRL_TCON_PLL2_CNTL0,
			readl(ANACTRL_TCON_PLL2_CNTL0),
			ANACTRL_TCON_PLL2_CNTL1,
			readl(ANACTRL_TCON_PLL2_CNTL1),
			ANACTRL_TCON_PLL2_CNTL2,
			readl(ANACTRL_TCON_PLL2_CNTL2),
			ANACTRL_TCON_PLL2_CNTL3,
			readl(ANACTRL_TCON_PLL2_CNTL3),
			ANACTRL_TCON_PLL2_CNTL4,
			readl(ANACTRL_TCON_PLL2_CNTL4),
			ANACTRL_TCON_PLL2_STS,
			readl(ANACTRL_TCON_PLL2_STS));

		printf("clk regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			COMBO_DPHY_VID_PLL2_DIV,
			readl(COMBO_DPHY_VID_PLL2_DIV),
			CLKCTRL_VIID_CLK2_DIV,
			readl(CLKCTRL_VIID_CLK2_DIV),
			CLKCTRL_VIID_CLK2_CTRL,
			readl(CLKCTRL_VIID_CLK2_CTRL),
			CLKCTRL_VID_CLK2_CTRL2,
			readl(CLKCTRL_VID_CLK2_CTRL2));

		printf("lvds regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0,
			readl(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0),
			COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1,
			readl(COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1),
			LVDS_SER_EN + (0x600 << 2),
			readl(LVDS_SER_EN + (0x600 << 2)),
			LVDS_PACK_CNTL_ADDR + (0x600 << 2),
			readl(LVDS_PACK_CNTL_ADDR + (0x600 << 2)),
			LVDS_GEN_CNTL + (0x600 << 2),
			readl(LVDS_GEN_CNTL + (0x600 << 2)),
			P2P_CH_SWAP0 + (0x600 << 2),
			readl(P2P_CH_SWAP0 + (0x600 << 2)),
			P2P_CH_SWAP1 + (0x600 << 2),
			readl(P2P_CH_SWAP1 + (0x600 << 2)),
			P2P_BIT_REV + (0x600 << 2),
			readl(P2P_BIT_REV + (0x600 << 2)));

		printf("venc regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ENCL_VIDEO_EN + (0x800 << 2),
			readl(ENCL_VIDEO_EN + (0x800 << 2)),
			ENCL_VIDEO_MODE + (0x800 << 2),
			readl(ENCL_VIDEO_MODE + (0x800 << 2)),
			ENCL_VIDEO_MODE_ADV + (0x800 << 2),
			readl(ENCL_VIDEO_MODE_ADV + (0x800 << 2)),
			ENCL_VIDEO_FILT_CTRL + (0x800 << 2),
			readl(ENCL_VIDEO_FILT_CTRL + (0x800 << 2)),
			ENCL_VIDEO_MAX_PXCNT + (0x800 << 2),
			readl(ENCL_VIDEO_MAX_PXCNT + (0x800 << 2)),
			ENCL_VIDEO_MAX_LNCNT + (0x800 << 2),
			readl(ENCL_VIDEO_MAX_LNCNT + (0x800 << 2)),
			ENCL_VIDEO_HAVON_BEGIN + (0x800 << 2),
			readl(ENCL_VIDEO_HAVON_BEGIN + (0x800 << 2)),
			ENCL_VIDEO_HAVON_END + (0x800 << 2),
			readl(ENCL_VIDEO_HAVON_END + (0x800 << 2)),
			ENCL_VIDEO_VAVON_BLINE + (0x800 << 2),
			readl(ENCL_VIDEO_VAVON_BLINE + (0x800 << 2)),
			ENCL_VIDEO_VAVON_ELINE + (0x800 << 2),
			readl(ENCL_VIDEO_VAVON_ELINE + (0x800 << 2)),
			ENCL_VIDEO_HSO_BEGIN + (0x800 << 2),
			readl(ENCL_VIDEO_HSO_BEGIN + (0x800 << 2)),
			ENCL_VIDEO_HSO_END + (0x800 << 2),
			readl(ENCL_VIDEO_HSO_END + (0x800 << 2)),
			ENCL_VIDEO_VSO_BEGIN + (0x800 << 2),
			readl(ENCL_VIDEO_VSO_BEGIN + (0x800 << 2)),
			ENCL_VIDEO_VSO_END + (0x800 << 2),
			readl(ENCL_VIDEO_VSO_END + (0x800 << 2)),
			ENCL_VIDEO_VSO_BLINE + (0x800 << 2),
			readl(ENCL_VIDEO_VSO_BLINE + (0x800 << 2)),
			ENCL_VIDEO_VSO_ELINE + (0x800 << 2),
			readl(ENCL_VIDEO_VSO_ELINE + (0x800 << 2)),
			ENCL_VIDEO_RGBIN_CTRL + (0x800 << 2),
			readl(ENCL_VIDEO_RGBIN_CTRL + (0x800 << 2)),
			ENCL_INBUF_CNTL1 + (0x800 << 2),
			readl(ENCL_INBUF_CNTL1 + (0x800 << 2)),
			ENCL_INBUF_CNTL0 + (0x800 << 2),
			readl(ENCL_INBUF_CNTL0 + (0x800 << 2)),
			VPU_DISP_VIU2_CTRL,
			readl(VPU_DISP_VIU2_CTRL),
			VPU_VENC_CTRL + (0x800 << 2),
			readl(VPU_VENC_CTRL + (0x800 << 2)));
	} else if (pdrv->config.basic.lcd_type == LCD_EDP) {
		switch (index) {
		case 0:
			reg_stts = ANACTRL_TCON_PLL0_STS;
			vid2_clk_ctrl = CLKCTRL_VIID_CLK0_CTRL;
			vid2_clk_div = CLKCTRL_VIID_CLK0_DIV;
			vid2_clk_ctrl2 = CLKCTRL_VID_CLK0_CTRL2;
			vid_pll_div = COMBO_DPHY_VID_PLL0_DIV;
			reg_dphy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0;
			reg_dphy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1;
			vpu_disp_ctrl = VPU_DISP_VIU0_CTRL;
			break;
		case 1:
			reg_stts = ANACTRL_TCON_PLL1_STS;
			vid2_clk_ctrl = CLKCTRL_VIID_CLK1_CTRL;
			vid2_clk_div = CLKCTRL_VIID_CLK1_DIV;
			vid2_clk_ctrl2 = CLKCTRL_VID_CLK1_CTRL2;
			vid_pll_div = COMBO_DPHY_VID_PLL1_DIV;
			reg_dphy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL0;
			reg_dphy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL1;
			vpu_disp_ctrl = VPU_DISP_VIU1_CTRL;
			break;
		default:
			return;
		}
		printf("\nedp%d regs:\n", index);
		offset = (pll_table[index] << 2);
		printf("pll regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ANACTRL_TCON_PLL0_CNTL0 + offset,
			readl(ANACTRL_TCON_PLL0_CNTL0 + offset),
			ANACTRL_TCON_PLL0_CNTL1 + offset,
			readl(ANACTRL_TCON_PLL0_CNTL1 + offset),
			ANACTRL_TCON_PLL0_CNTL2 + offset,
			readl(ANACTRL_TCON_PLL0_CNTL2 + offset),
			ANACTRL_TCON_PLL0_CNTL3 + offset,
			readl(ANACTRL_TCON_PLL0_CNTL3 + offset),
			ANACTRL_TCON_PLL0_CNTL4 + offset,
			readl(ANACTRL_TCON_PLL0_CNTL4 + offset),
			reg_stts,
			readl(reg_stts));

		printf("clk regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			COMBO_DPHY_EDP_PIXEL_CLK_DIV,
			readl(COMBO_DPHY_EDP_PIXEL_CLK_DIV),
			vid_pll_div,
			readl(vid_pll_div),
			vid2_clk_div,
			readl(vid2_clk_div),
			vid2_clk_ctrl,
			readl(vid2_clk_ctrl),
			vid2_clk_ctrl2,
			readl(vid2_clk_ctrl2));

		printf("edp regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n",
			reg_dphy_tx_ctrl0,
			readl(reg_dphy_tx_ctrl0),
			reg_dphy_tx_ctrl1,
			readl(reg_dphy_tx_ctrl1),
			EDP_TX_LINK_BW_SET,
			dptx_reg_read(index, EDP_TX_LINK_BW_SET),
			EDP_TX_LINK_COUNT_SET,
			dptx_reg_read(index, EDP_TX_LINK_COUNT_SET),
			EDP_TX_TRAINING_PATTERN_SET,
			dptx_reg_read(index, EDP_TX_TRAINING_PATTERN_SET),
			EDP_TX_SCRAMBLING_DISABLE,
			dptx_reg_read(index, EDP_TX_SCRAMBLING_DISABLE),
			EDP_TX_TRANSMITTER_OUTPUT_ENABLE,
			dptx_reg_read(index, EDP_TX_TRANSMITTER_OUTPUT_ENABLE),
			EDP_TX_MAIN_STREAM_ENABLE,
			dptx_reg_read(index, EDP_TX_MAIN_STREAM_ENABLE),
			EDP_TX_PHY_RESET,
			dptx_reg_read(index, EDP_TX_PHY_RESET),
			EDP_TX_PHY_STATUS,
			dptx_reg_read(index, EDP_TX_PHY_STATUS),
			EDP_TX_AUX_COMMAND,
			dptx_reg_read(index, EDP_TX_AUX_COMMAND),
			EDP_TX_AUX_ADDRESS,
			dptx_reg_read(index, EDP_TX_AUX_ADDRESS),
			EDP_TX_AUX_STATE,
			dptx_reg_read(index, EDP_TX_AUX_STATE),
			EDP_TX_AUX_REPLY_CODE,
			dptx_reg_read(index, EDP_TX_AUX_REPLY_CODE),
			EDP_TX_AUX_REPLY_COUNT,
			dptx_reg_read(index, EDP_TX_AUX_REPLY_COUNT),
			EDP_TX_AUX_REPLY_DATA_COUNT,
			dptx_reg_read(index, EDP_TX_AUX_REPLY_DATA_COUNT),
			EDP_TX_AUX_TRANSFER_STATUS,
			dptx_reg_read(index, EDP_TX_AUX_TRANSFER_STATUS));

		offset = (venc_table[index] << 2);
		printf("venc regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ENCL_VIDEO_EN + offset,
			readl(ENCL_VIDEO_EN + offset),
			ENCL_VIDEO_MODE + offset,
			readl(ENCL_VIDEO_MODE + offset),
			ENCL_VIDEO_MODE_ADV + offset,
			readl(ENCL_VIDEO_MODE_ADV + offset),
			ENCL_VIDEO_FILT_CTRL + offset,
			readl(ENCL_VIDEO_FILT_CTRL + offset),
			ENCL_VIDEO_MAX_PXCNT + offset,
			readl(ENCL_VIDEO_MAX_PXCNT + offset),
			ENCL_VIDEO_MAX_LNCNT + offset,
			readl(ENCL_VIDEO_MAX_LNCNT + offset),
			ENCL_VIDEO_HAVON_BEGIN + offset,
			readl(ENCL_VIDEO_HAVON_BEGIN + offset),
			ENCL_VIDEO_HAVON_END + offset,
			readl(ENCL_VIDEO_HAVON_END + offset),
			ENCL_VIDEO_VAVON_BLINE + offset,
			readl(ENCL_VIDEO_VAVON_BLINE + offset),
			ENCL_VIDEO_VAVON_ELINE + offset,
			readl(ENCL_VIDEO_VAVON_ELINE + offset),
			ENCL_VIDEO_HSO_BEGIN + offset,
			readl(ENCL_VIDEO_HSO_BEGIN + offset),
			ENCL_VIDEO_HSO_END + offset,
			readl(ENCL_VIDEO_HSO_END + offset),
			ENCL_VIDEO_VSO_BEGIN + offset,
			readl(ENCL_VIDEO_VSO_BEGIN + offset),
			ENCL_VIDEO_VSO_END + offset,
			readl(ENCL_VIDEO_VSO_END + offset),
			ENCL_VIDEO_VSO_BLINE + offset,
			readl(ENCL_VIDEO_VSO_BLINE + offset),
			ENCL_VIDEO_VSO_ELINE + offset,
			readl(ENCL_VIDEO_VSO_ELINE + offset),
			ENCL_VIDEO_RGBIN_CTRL + offset,
			readl(ENCL_VIDEO_RGBIN_CTRL + offset),
			ENCL_INBUF_CNTL1 + offset,
			readl(ENCL_INBUF_CNTL1 + offset),
			ENCL_INBUF_CNTL0 + offset,
			readl(ENCL_INBUF_CNTL0 + offset),
			vpu_disp_ctrl,
			readl(vpu_disp_ctrl),
			VPU_VENC_CTRL + offset,
			readl(VPU_VENC_CTRL + offset));
	} else if (pdrv->config.basic.lcd_type == LCD_MIPI) {
		printf("pll regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ANACTRL_TCON_PLL0_CNTL0,
			readl(ANACTRL_TCON_PLL0_CNTL0),
			ANACTRL_TCON_PLL0_CNTL1,
			readl(ANACTRL_TCON_PLL0_CNTL1),
			ANACTRL_TCON_PLL0_CNTL2,
			readl(ANACTRL_TCON_PLL0_CNTL2),
			ANACTRL_TCON_PLL0_CNTL3,
			readl(ANACTRL_TCON_PLL0_CNTL3),
			ANACTRL_TCON_PLL0_CNTL4,
			readl(ANACTRL_TCON_PLL0_CNTL4),
			ANACTRL_TCON_PLL0_STS,
			readl(ANACTRL_TCON_PLL0_STS));

		printf("clk regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			CLKCTRL_MIPIDSI_PHY_CLK_CTRL,
			readl(CLKCTRL_MIPIDSI_PHY_CLK_CTRL),
			CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL,
			readl(CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL),
			COMBO_DPHY_VID_PLL0_DIV,
			readl(COMBO_DPHY_VID_PLL0_DIV),
			CLKCTRL_VIID_CLK0_DIV,
			readl(CLKCTRL_VIID_CLK0_DIV),
			CLKCTRL_VIID_CLK0_CTRL,
			readl(CLKCTRL_VIID_CLK0_CTRL),
			CLKCTRL_VID_CLK0_CTRL2,
			readl(CLKCTRL_VID_CLK0_CTRL2));

		printf("mipi-dsi host regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			MIPI_DSI_TOP_CNTL,
			readl(MIPI_DSI_TOP_CNTL),
			MIPI_DSI_TOP_SW_RESET,
			readl(MIPI_DSI_TOP_SW_RESET),
			MIPI_DSI_TOP_CLK_CNTL,
			readl(MIPI_DSI_TOP_CLK_CNTL),
			MIPI_DSI_TOP_MEM_PD,
			readl(MIPI_DSI_TOP_MEM_PD),
			MIPI_DSI_TOP_INTR_CNTL_STAT,
			readl(MIPI_DSI_TOP_INTR_CNTL_STAT),
			MIPI_DSI_TOP_MEAS_CNTL,
			readl(MIPI_DSI_TOP_MEAS_CNTL));
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			MIPI_DSI_DWC_PHY_TST_CTRL0_OS,
			readl(MIPI_DSI_DWC_PHY_TST_CTRL0_OS),
			MIPI_DSI_DWC_PHY_TST_CTRL1_OS,
			readl(MIPI_DSI_DWC_PHY_TST_CTRL1_OS),
			MIPI_DSI_DWC_PHY_RSTZ_OS,
			readl(MIPI_DSI_DWC_PHY_RSTZ_OS),
			MIPI_DSI_DWC_PHY_IF_CFG_OS,
			readl(MIPI_DSI_DWC_PHY_IF_CFG_OS),
			MIPI_DSI_DWC_PHY_TMR_CFG_OS,
			readl(MIPI_DSI_DWC_PHY_TMR_CFG_OS),
			MIPI_DSI_DWC_PHY_TMR_LPCLK_CFG_OS,
			readl(MIPI_DSI_DWC_PHY_TMR_LPCLK_CFG_OS),
			MIPI_DSI_DWC_CMD_MODE_CFG_OS,
			readl(MIPI_DSI_DWC_CMD_MODE_CFG_OS),
			MIPI_DSI_DWC_PCKHDL_CFG_OS,
			readl(MIPI_DSI_DWC_PCKHDL_CFG_OS),
			MIPI_DSI_DWC_DPI_COLOR_CODING_OS,
			readl(MIPI_DSI_DWC_DPI_COLOR_CODING_OS),
			MIPI_DSI_DWC_DPI_CFG_POL_OS,
			readl(MIPI_DSI_DWC_DPI_CFG_POL_OS),
			MIPI_DSI_DWC_VID_MODE_CFG_OS,
			readl(MIPI_DSI_DWC_VID_MODE_CFG_OS),
			MIPI_DSI_DWC_DPI_LP_CMD_TIM_OS,
			readl(MIPI_DSI_DWC_DPI_LP_CMD_TIM_OS),
			MIPI_DSI_DWC_CLKMGR_CFG_OS,
			readl(MIPI_DSI_DWC_CLKMGR_CFG_OS),
			MIPI_DSI_DWC_MODE_CFG_OS,
			readl(MIPI_DSI_DWC_MODE_CFG_OS));
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			MIPI_DSI_DWC_VID_PKT_SIZE_OS,
			readl(MIPI_DSI_DWC_VID_PKT_SIZE_OS),
			MIPI_DSI_DWC_VID_NUM_CHUNKS_OS,
			readl(MIPI_DSI_DWC_VID_NUM_CHUNKS_OS),
			MIPI_DSI_DWC_VID_NULL_SIZE_OS,
			readl(MIPI_DSI_DWC_VID_NULL_SIZE_OS),
			MIPI_DSI_DWC_VID_HLINE_TIME_OS,
			readl(MIPI_DSI_DWC_VID_HLINE_TIME_OS),
			MIPI_DSI_DWC_VID_HSA_TIME_OS,
			readl(MIPI_DSI_DWC_VID_HSA_TIME_OS),
			MIPI_DSI_DWC_VID_HBP_TIME_OS,
			readl(MIPI_DSI_DWC_VID_HBP_TIME_OS),
			MIPI_DSI_DWC_VID_VSA_LINES_OS,
			readl(MIPI_DSI_DWC_VID_VSA_LINES_OS),
			MIPI_DSI_DWC_VID_VBP_LINES_OS,
			readl(MIPI_DSI_DWC_VID_VBP_LINES_OS),
			MIPI_DSI_DWC_VID_VFP_LINES_OS,
			readl(MIPI_DSI_DWC_VID_VFP_LINES_OS),
			MIPI_DSI_DWC_VID_VACTIVE_LINES_OS,
			readl(MIPI_DSI_DWC_VID_VACTIVE_LINES_OS));

		printf("mipi-dsi phy regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			MIPI_DSI_PHY_CTRL,
			readl(MIPI_DSI_PHY_CTRL),
			MIPI_DSI_CLK_TIM,
			readl(MIPI_DSI_CLK_TIM),
			MIPI_DSI_CLK_TIM1,
			readl(MIPI_DSI_CLK_TIM1),
			MIPI_DSI_HS_TIM,
			readl(MIPI_DSI_HS_TIM),
			MIPI_DSI_LP_TIM,
			readl(MIPI_DSI_LP_TIM),
			MIPI_DSI_ANA_UP_TIM,
			readl(MIPI_DSI_ANA_UP_TIM),
			MIPI_DSI_INIT_TIM,
			readl(MIPI_DSI_INIT_TIM),
			MIPI_DSI_WAKEUP_TIM,
			readl(MIPI_DSI_WAKEUP_TIM),
			MIPI_DSI_LPOK_TIM,
			readl(MIPI_DSI_LPOK_TIM),
			MIPI_DSI_ULPS_CHECK,
			readl(MIPI_DSI_ULPS_CHECK),
			MIPI_DSI_LP_WCHDOG,
			readl(MIPI_DSI_LP_WCHDOG),
			MIPI_DSI_TURN_WCHDOG,
			readl(MIPI_DSI_TURN_WCHDOG),
			MIPI_DSI_CHAN_CTRL,
			readl(MIPI_DSI_CHAN_CTRL),
			MIPI_DSI_PHY_CTRL,
			readl(MIPI_DSI_PHY_CTRL),
			MIPI_DSI_LP_WCHDOG,
			readl(MIPI_DSI_LP_WCHDOG));

		printf("venc regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ENCL_VIDEO_EN,
			readl(ENCL_VIDEO_EN),
			ENCL_VIDEO_MODE,
			readl(ENCL_VIDEO_MODE),
			ENCL_VIDEO_MODE_ADV,
			readl(ENCL_VIDEO_MODE_ADV),
			ENCL_VIDEO_FILT_CTRL,
			readl(ENCL_VIDEO_FILT_CTRL),
			ENCL_VIDEO_MAX_PXCNT,
			readl(ENCL_VIDEO_MAX_PXCNT),
			ENCL_VIDEO_MAX_LNCNT,
			readl(ENCL_VIDEO_MAX_LNCNT),
			ENCL_VIDEO_HAVON_BEGIN,
			readl(ENCL_VIDEO_HAVON_BEGIN),
			ENCL_VIDEO_HAVON_END,
			readl(ENCL_VIDEO_HAVON_END),
			ENCL_VIDEO_VAVON_BLINE,
			readl(ENCL_VIDEO_VAVON_BLINE),
			ENCL_VIDEO_VAVON_ELINE,
			readl(ENCL_VIDEO_VAVON_ELINE),
			ENCL_VIDEO_HSO_BEGIN,
			readl(ENCL_VIDEO_HSO_BEGIN),
			ENCL_VIDEO_HSO_END,
			readl(ENCL_VIDEO_HSO_END),
			ENCL_VIDEO_VSO_BEGIN,
			readl(ENCL_VIDEO_VSO_BEGIN),
			ENCL_VIDEO_VSO_END,
			readl(ENCL_VIDEO_VSO_END),
			ENCL_VIDEO_VSO_BLINE,
			readl(ENCL_VIDEO_VSO_BLINE),
			ENCL_VIDEO_VSO_ELINE,
			readl(ENCL_VIDEO_VSO_ELINE),
			ENCL_VIDEO_RGBIN_CTRL,
			readl(ENCL_VIDEO_RGBIN_CTRL),
			ENCL_INBUF_CNTL1,
			readl(ENCL_INBUF_CNTL1),
			ENCL_INBUF_CNTL0,
			readl(ENCL_INBUF_CNTL0),
			VPU_DISP_VIU0_CTRL,
			readl(VPU_DISP_VIU0_CTRL),
			VPU_VENC_CTRL,
			readl(VPU_VENC_CTRL));
	} else if (pdrv->config.basic.lcd_type == LCD_VBYONE) {
		printf("pll regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ANACTRL_TCON_PLL0_CNTL0,
			readl(ANACTRL_TCON_PLL0_CNTL0),
			ANACTRL_TCON_PLL0_CNTL1,
			readl(ANACTRL_TCON_PLL0_CNTL1),
			ANACTRL_TCON_PLL0_CNTL2,
			readl(ANACTRL_TCON_PLL0_CNTL2),
			ANACTRL_TCON_PLL0_CNTL3,
			readl(ANACTRL_TCON_PLL0_CNTL3),
			ANACTRL_TCON_PLL0_CNTL4,
			readl(ANACTRL_TCON_PLL0_CNTL4),
			ANACTRL_TCON_PLL0_STS,
			readl(ANACTRL_TCON_PLL0_STS));

		printf("clk regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			COMBO_DPHY_VID_PLL0_DIV,
			readl(COMBO_DPHY_VID_PLL0_DIV),
			CLKCTRL_VIID_CLK0_DIV,
			readl(CLKCTRL_VIID_CLK0_DIV),
			CLKCTRL_VIID_CLK0_CTRL,
			readl(CLKCTRL_VIID_CLK0_CTRL),
			CLKCTRL_VID_CLK0_CTRL2,
			readl(CLKCTRL_VID_CLK0_CTRL2));

		printf("vx1 regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n"
			"0x%04x: 0x%08x\n",
			VBO_VIN_CTRL,
			readl(VBO_VIN_CTRL),
			VBO_STATUS_L,
			readl(VBO_STATUS_L),
			VBO_LANES,
			readl(VBO_LANES),
			VBO_REGION_00,
			readl(VBO_REGION_00),
			VBO_REGION_01,
			readl(VBO_REGION_01),
			VBO_REGION_02,
			readl(VBO_REGION_02),
			VBO_REGION_03,
			readl(VBO_REGION_03),
			VBO_ACT_VSIZE,
			readl(VBO_ACT_VSIZE),
			VBO_CTRL_H,
			readl(VBO_CTRL_H),
			VBO_INTR_STATE_CTRL,
			readl(VBO_INTR_STATE_CTRL),
			VBO_INTR_STATE,
			readl(VBO_INTR_STATE),
			VBO_GCLK_MAIN,
			readl(VBO_GCLK_MAIN),
			VBO_INSGN_CTRL,
			readl(VBO_INSGN_CTRL),
			VBO_CTRL_L,
			readl(VBO_CTRL_L));

		printf("venc regs:\n");
		printf("0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n"
			"0x%08x: 0x%08x\n",
			ENCL_VIDEO_EN,
			readl(ENCL_VIDEO_EN),
			ENCL_VIDEO_MODE,
			readl(ENCL_VIDEO_MODE),
			ENCL_VIDEO_MODE_ADV,
			readl(ENCL_VIDEO_MODE_ADV),
			ENCL_VIDEO_FILT_CTRL,
			readl(ENCL_VIDEO_FILT_CTRL),
			ENCL_VIDEO_MAX_PXCNT,
			readl(ENCL_VIDEO_MAX_PXCNT),
			ENCL_VIDEO_MAX_LNCNT,
			readl(ENCL_VIDEO_MAX_LNCNT),
			ENCL_VIDEO_HAVON_BEGIN,
			readl(ENCL_VIDEO_HAVON_BEGIN),
			ENCL_VIDEO_HAVON_END,
			readl(ENCL_VIDEO_HAVON_END),
			ENCL_VIDEO_VAVON_BLINE,
			readl(ENCL_VIDEO_VAVON_BLINE),
			ENCL_VIDEO_VAVON_ELINE,
			readl(ENCL_VIDEO_VAVON_ELINE),
			ENCL_VIDEO_HSO_BEGIN,
			readl(ENCL_VIDEO_HSO_BEGIN),
			ENCL_VIDEO_HSO_END,
			readl(ENCL_VIDEO_HSO_END),
			ENCL_VIDEO_VSO_BEGIN,
			readl(ENCL_VIDEO_VSO_BEGIN),
			ENCL_VIDEO_VSO_END,
			readl(ENCL_VIDEO_VSO_END),
			ENCL_VIDEO_VSO_BLINE,
			readl(ENCL_VIDEO_VSO_BLINE),
			ENCL_VIDEO_VSO_ELINE,
			readl(ENCL_VIDEO_VSO_ELINE),
			ENCL_VIDEO_RGBIN_CTRL,
			readl(ENCL_VIDEO_RGBIN_CTRL),
			ENCL_INBUF_CNTL1,
			readl(ENCL_INBUF_CNTL1),
			ENCL_INBUF_CNTL0,
			readl(ENCL_INBUF_CNTL0),
			VPU_DISP_VIU0_CTRL,
			readl(VPU_DISP_VIU0_CTRL),
			VPU_VENC_CTRL,
			readl(VPU_VENC_CTRL));
	}
	printf("0x%08x: 0x%08x\n",
		VPU_VIU_VENC_MUX_CTRL,
		readl(VPU_VIU_VENC_MUX_CTRL));

	printf("phy regs:\n");
	printf("0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n"
		"0x%08x: 0x%08x\n",
		ANACTRL_DIF_PHY_CNTL1,
		readl(ANACTRL_DIF_PHY_CNTL1),
		ANACTRL_DIF_PHY_CNTL2,
		readl(ANACTRL_DIF_PHY_CNTL2),
		ANACTRL_DIF_PHY_CNTL3,
		readl(ANACTRL_DIF_PHY_CNTL3),
		ANACTRL_DIF_PHY_CNTL4,
		readl(ANACTRL_DIF_PHY_CNTL4),
		ANACTRL_DIF_PHY_CNTL5,
		readl(ANACTRL_DIF_PHY_CNTL5),
		ANACTRL_DIF_PHY_CNTL6,
		readl(ANACTRL_DIF_PHY_CNTL6),
		ANACTRL_DIF_PHY_CNTL7,
		readl(ANACTRL_DIF_PHY_CNTL7),
		ANACTRL_DIF_PHY_CNTL8,
		readl(ANACTRL_DIF_PHY_CNTL8),
		ANACTRL_DIF_PHY_CNTL9,
		readl(ANACTRL_DIF_PHY_CNTL9),
		ANACTRL_DIF_PHY_CNTL10,
		readl(ANACTRL_DIF_PHY_CNTL10),
		ANACTRL_DIF_PHY_CNTL11,
		readl(ANACTRL_DIF_PHY_CNTL11),
		ANACTRL_DIF_PHY_CNTL12,
		readl(ANACTRL_DIF_PHY_CNTL12),
		ANACTRL_DIF_PHY_CNTL13,
		readl(ANACTRL_DIF_PHY_CNTL13),
		ANACTRL_DIF_PHY_CNTL14,
		readl(ANACTRL_DIF_PHY_CNTL14),
		ANACTRL_DIF_PHY_CNTL15,
		readl(ANACTRL_DIF_PHY_CNTL15),
		ANACTRL_DIF_PHY_CNTL16,
		readl(ANACTRL_DIF_PHY_CNTL16),
		ANACTRL_DIF_PHY_CNTL17,
		readl(ANACTRL_DIF_PHY_CNTL17),
		ANACTRL_DIF_PHY_CNTL18,
		readl(ANACTRL_DIF_PHY_CNTL18),
		ANACTRL_DIF_PHY_CNTL19,
		readl(ANACTRL_DIF_PHY_CNTL19),
		ANACTRL_DIF_PHY_CNTL20,
		readl(ANACTRL_DIF_PHY_CNTL20),
		ANACTRL_DIF_PHY_CNTL21,
		readl(ANACTRL_DIF_PHY_CNTL21));
}
