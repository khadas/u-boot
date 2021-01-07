/*
 * drivers/amlogic/media/vout/lcd/lcd_tv/lcd_drv.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/io.h>
#include <amlogic/media/vpp/vpp.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "../lcd_reg.h"
#include "../lcd_common.h"
#include "lcd_tv.h"

static int lcd_type_supported(struct lcd_config_s *pconf)
{
	int lcd_type = pconf->lcd_basic.lcd_type;
	int ret = -1;

	switch (lcd_type) {
	case LCD_LVDS:
	case LCD_VBYONE:
	case LCD_MLVDS:
	case LCD_P2P:
		ret = 0;
		break;
	default:
		LCDERR("invalid lcd type: %s(%d)\n",
			lcd_type_type_to_str(lcd_type), lcd_type);
		break;
	}
	return ret;
}

static void lcd_encl_tcon_set(struct lcd_config_s *pconf)
{
	unsigned int lcd_bits;

	lcd_vcbus_write(L_RGB_BASE_ADDR, 0);
	lcd_vcbus_write(L_RGB_COEFF_ADDR, 0x400);

	switch (pconf->lcd_basic.lcd_type) {
	case LCD_MLVDS:
	case LCD_P2P:
		lcd_bits = 10;
		break;
	default:
		lcd_bits = pconf->lcd_basic.lcd_bits;
		break;
	}

	switch (lcd_bits) {
	case 6:
		lcd_vcbus_write(L_DITH_CNTL_ADDR,  0x600);
		break;
	case 8:
		lcd_vcbus_write(L_DITH_CNTL_ADDR,  0x400);
		break;
	case 10:
	default:
		lcd_vcbus_write(L_DITH_CNTL_ADDR,  0x0);
		break;
	}

	switch (pconf->lcd_basic.lcd_type) {
	case LCD_LVDS:
		lcd_vcbus_setb(L_POL_CNTL_ADDR, 1, 0, 1);
		if (pconf->lcd_timing.vsync_pol)
			lcd_vcbus_setb(L_POL_CNTL_ADDR, 1, 1, 1);
		break;
	case LCD_VBYONE:
		if (pconf->lcd_timing.hsync_pol)
			lcd_vcbus_setb(L_POL_CNTL_ADDR, 1, 0, 1);
		if (pconf->lcd_timing.vsync_pol)
			lcd_vcbus_setb(L_POL_CNTL_ADDR, 1, 1, 1);
		break;
	default:
		break;
	}

	lcd_vcbus_write(VPP_MISC, lcd_vcbus_read(VPP_MISC) & ~(VPP_OUT_SATURATE));
}

static void lcd_venc_set(struct lcd_config_s *pconf)
{
	unsigned int h_active, v_active;
	unsigned int video_on_pixel, video_on_line;
	unsigned int pre_de_vs = 0, pre_de_ve = 0, pre_de_hs = 0, pre_de_he = 0;
	struct lcd_drv_s *lcd_drv = lcd_get_driver();

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);

	h_active = pconf->lcd_basic.h_active;
	v_active = pconf->lcd_basic.v_active;
	video_on_pixel = pconf->lcd_timing.video_on_pixel;
	video_on_line = pconf->lcd_timing.video_on_line;

	lcd_vcbus_write(ENCL_VIDEO_EN, 0);

	lcd_vcbus_write(ENCL_VIDEO_MODE, 0x8000); // bit[15] shadown en
	lcd_vcbus_write(ENCL_VIDEO_MODE_ADV, 0x0418); // Sampling rate: 1

	// bypass filter
	lcd_vcbus_write(ENCL_VIDEO_FILT_CTRL, 0x1000);
	lcd_vcbus_write(ENCL_VIDEO_MAX_PXCNT, pconf->lcd_basic.h_period - 1);
	lcd_vcbus_write(ENCL_VIDEO_MAX_LNCNT, pconf->lcd_basic.v_period - 1);
	lcd_vcbus_write(ENCL_VIDEO_HAVON_BEGIN, video_on_pixel);
	lcd_vcbus_write(ENCL_VIDEO_HAVON_END,   h_active - 1 + video_on_pixel);
	lcd_vcbus_write(ENCL_VIDEO_VAVON_BLINE, video_on_line);
	lcd_vcbus_write(ENCL_VIDEO_VAVON_ELINE, v_active - 1  + video_on_line);
	if (pconf->lcd_basic.lcd_type == LCD_P2P ||
	    pconf->lcd_basic.lcd_type == LCD_MLVDS) {
		switch (lcd_drv->chip_type) {
		case LCD_CHIP_TL1:
		case LCD_CHIP_TM2:
			pre_de_vs = video_on_line - 1 - 4;
			pre_de_ve = video_on_line - 1;
			pre_de_hs = video_on_pixel + PRE_DE_DELAY;
			pre_de_he = h_active - 1 + pre_de_hs;
			break;
		default:
			pre_de_vs = video_on_line - 8;
			pre_de_ve = v_active + pre_de_vs;
			pre_de_hs = video_on_pixel + PRE_DE_DELAY;
			pre_de_he = h_active - 1 + pre_de_hs;
			break;
		}
		lcd_vcbus_write(ENCL_VIDEO_V_PRE_DE_BLINE, pre_de_vs);
		lcd_vcbus_write(ENCL_VIDEO_V_PRE_DE_ELINE, pre_de_ve);
		lcd_vcbus_write(ENCL_VIDEO_H_PRE_DE_BEGIN, pre_de_hs);
		lcd_vcbus_write(ENCL_VIDEO_H_PRE_DE_END, pre_de_he);
	}

	lcd_vcbus_write(ENCL_VIDEO_HSO_BEGIN, pconf->lcd_timing.hs_hs_addr);
	lcd_vcbus_write(ENCL_VIDEO_HSO_END,   pconf->lcd_timing.hs_he_addr);
	lcd_vcbus_write(ENCL_VIDEO_VSO_BEGIN, pconf->lcd_timing.vs_hs_addr);
	lcd_vcbus_write(ENCL_VIDEO_VSO_END,   pconf->lcd_timing.vs_he_addr);
	lcd_vcbus_write(ENCL_VIDEO_VSO_BLINE, pconf->lcd_timing.vs_vs_addr);
	lcd_vcbus_write(ENCL_VIDEO_VSO_ELINE, pconf->lcd_timing.vs_ve_addr);
	lcd_vcbus_write(ENCL_VIDEO_RGBIN_CTRL, 3);

	switch (lcd_drv->chip_type) {
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
	case LCD_CHIP_T7:
		/*[15:14]: 2'b10 or 2'b01*/
		lcd_vcbus_write(ENCL_INBUF_CNTL1, (2 << 14) | (h_active - 1));
		lcd_vcbus_write(ENCL_INBUF_CNTL0, 0x200);
		break;
	default:
		break;
	}

	/* default black pattern */
	lcd_vcbus_write(ENCL_TST_MDSEL, 0);
	lcd_vcbus_write(ENCL_TST_Y, 0);
	lcd_vcbus_write(ENCL_TST_CB, 0);
	lcd_vcbus_write(ENCL_TST_CR, 0);
	lcd_vcbus_write(ENCL_TST_EN, 1);
	lcd_vcbus_setb(ENCL_VIDEO_MODE_ADV, 0, 3, 1);

	lcd_vcbus_write(ENCL_VIDEO_EN, 1);
	if (lcd_drv->chip_type == LCD_CHIP_T7) {
		/*
		 * bit31: lvds enable
		 * bit30: vx1 enable
		 * bit29: hdmitx enable
		 * bit28: dsi_edp enable
		 */
		switch (lcd_drv->lcd_config->lcd_basic.lcd_type) {
		case LCD_LVDS:
			lcd_vcbus_write(VPU_DISP_VIU0_CTRL, (1 << 31) |
							    (0 << 30) |
							    (0 << 29) |
							    (0 << 28));
			break;
		case LCD_VBYONE:
			lcd_vcbus_write(VPU_DISP_VIU0_CTRL, (0 << 31) |
							    (1 << 30) |
							    (0 << 29) |
							    (0 << 28));
			break;
		case LCD_MIPI:
			lcd_vcbus_write(VPU_DISP_VIU0_CTRL, (0 << 31) |
							    (0 << 30) |
							    (0 << 29) |
							    (1 << 28));
			break;
		default:
			break;
		}
	}
}

static void lcd_lvds_clk_util_set(struct lcd_config_s *pconf)
{
	unsigned int phy_div;
	struct lcd_drv_s *lcd_drv = lcd_get_driver();

	if (pconf->lcd_control.lvds_config->dual_port)
		phy_div = 2;
	else
		phy_div = 1;

	if (lcd_drv->chip_type == LCD_CHIP_T7) {
		/* set fifo_clk_sel: div 7 */
		lcd_combo_dphy_write(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0, (1 << 5));
		/* set cntl_ser_en:  8-channel to 1 */
		lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0, 0xff, 16, 8);
	} else {
		/* set fifo_clk_sel: div 7 */
		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL0, (1 << 6));
		/* set cntl_ser_en:  8-channel to 1 */
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 0xfff, 16, 12);
	}

	switch (lcd_drv->chip_type) { /* pn swap */
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 1, 2, 1);
		break;
	case LCD_CHIP_T7:
		lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0, 1, 2, 1);
		break;
	default:
		break;
	}

	if (lcd_drv->chip_type == LCD_CHIP_T7) {
		/* decoupling fifo enable, gated clock enable */
		lcd_combo_dphy_write(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1,
				(1 << 6) | (1 << 0));
		/* decoupling fifo write enable after fifo enable */
		lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1, 1, 7, 1);
	} else {
		/* decoupling fifo enable, gated clock enable */
		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL1,
			      (1 << 30) | ((phy_div - 1) << 25) | (1 << 24));
		/* decoupling fifo write enable after fifo enable */
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL1, 1, 31, 1);
	}
}

static void lcd_lvds_control_set(struct lcd_config_s *pconf)
{
	unsigned int bit_num = 1;
	unsigned int pn_swap, port_swap, lane_reverse;
	unsigned int dual_port, fifo_mode;
	unsigned int lvds_repack = 1;
	struct lcd_drv_s *lcd_drv = lcd_get_driver();

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);

	lcd_lvds_clk_util_set(pconf);

	lvds_repack = (pconf->lcd_control.lvds_config->lvds_repack) & 0x3;
	pn_swap   = (pconf->lcd_control.lvds_config->pn_swap) & 0x1;
	dual_port = (pconf->lcd_control.lvds_config->dual_port) & 0x1;
	port_swap = (pconf->lcd_control.lvds_config->port_swap) & 0x1;
	lane_reverse = (pconf->lcd_control.lvds_config->lane_reverse) & 0x1;

	switch (pconf->lcd_basic.lcd_bits) {
	case 10:
		bit_num=0;
		break;
	case 8:
		bit_num=1;
		break;
	case 6:
		bit_num=2;
		break;
	case 4:
		bit_num=3;
		break;
	default:
		bit_num=1;
		break;
	}
	if (dual_port)
		fifo_mode = 0x3;
	else
		fifo_mode = 0x1;

	lcd_vcbus_write(LVDS_PACK_CNTL_ADDR,
			(lvds_repack << 0) | // repack //[1:0]
			(0 << 3) |		// reserve
			(0 << 4) |		// lsb first
			(pn_swap << 5) |	// pn swap
			(dual_port << 6) |	// dual port
			(0 << 7) |		// use tcon control
			(bit_num << 8) |	// 0:10bits, 1:8bits, 2:6bits, 3:4bits.
			(0 << 10) |		//r_select  //0:R, 1:G, 2:B, 3:0
			(1 << 12) |		//g_select  //0:R, 1:G, 2:B, 3:0
			(2 << 14));		//b_select  //0:R, 1:G, 2:B, 3:0;

	/* lvsd swap */
	switch (lcd_drv->chip_type) {
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
		/* lvds channel:    //tx 12 channels
		 *    0: d0_a
		 *    1: d1_a
		 *    2: d2_a
		 *    3: clk_a
		 *    4: d3_a
		 *    5: d4_a
		 *    6: d0_b
		 *    7: d1_b
		 *    8: d2_b
		 *    9: clk_b
		 *    a: d3_b
		 *    b: d4_b
		 */
		if (port_swap) {
			if (lane_reverse) {
				lcd_vcbus_write(P2P_CH_SWAP0, 0x456789ab);
				lcd_vcbus_write(P2P_CH_SWAP1, 0x0123);
			} else {
				lcd_vcbus_write(P2P_CH_SWAP0, 0x10ba9876);
				lcd_vcbus_write(P2P_CH_SWAP1, 0x5432);
			}
		} else {
			if (lane_reverse) {
				lcd_vcbus_write(P2P_CH_SWAP0, 0xab012345);
				lcd_vcbus_write(P2P_CH_SWAP1, 0x6789);
			} else {
				lcd_vcbus_write(P2P_CH_SWAP0, 0x76543210);
				lcd_vcbus_write(P2P_CH_SWAP1, 0xba98);
			}
		}
		break;
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
	case LCD_CHIP_T7:
		/* lvds channel:    //tx 12 channels
		 *    0: d0_a
		 *    1: d1_a
		 *    2: d2_a
		 *    3: clk_a
		 *    4: d3_a
		 *    5: d4_a
		 *    6: d0_b
		 *    7: d1_b
		 *    8: d2_b
		 *    9: clk_b
		 *    a: d3_b
		 *    b: d4_b
		 */
		if (port_swap) {
			if (lane_reverse) {
				lcd_vcbus_write(P2P_CH_SWAP0, 0x345789ab);
				lcd_vcbus_write(P2P_CH_SWAP1, 0x0612);
			} else {
				lcd_vcbus_write(P2P_CH_SWAP0, 0x210a9876);
				lcd_vcbus_write(P2P_CH_SWAP1, 0x5b43);
			}
		} else {
			if (lane_reverse) {
				lcd_vcbus_write(P2P_CH_SWAP0, 0xab12345);
				lcd_vcbus_write(P2P_CH_SWAP1, 0x60789);
			} else {
				lcd_vcbus_write(P2P_CH_SWAP0, 0x87643210);
				lcd_vcbus_write(P2P_CH_SWAP1, 0xb5a9);
			}
		}
		break;
	default:
		lcd_vcbus_setb(LCD_PORT_SWAP, port_swap, 12, 1);
		if (lane_reverse)
			lcd_vcbus_setb(LVDS_GEN_CNTL, 0x03, 13, 2);
		break;
	}

	lcd_vcbus_write(LVDS_GEN_CNTL, (lcd_vcbus_read(LVDS_GEN_CNTL) | (1 << 4) | (fifo_mode << 0)));
	lcd_vcbus_setb(LVDS_GEN_CNTL, 1, 3, 1);
}

static void lcd_lvds_disable(void)
{
	lcd_vcbus_setb(LVDS_GEN_CNTL, 0, 3, 1); /* disable lvds fifo */
}

#ifdef CONFIG_AML_LCD_TCON
static void lcd_mlvds_control_set(struct lcd_config_s *pconf)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();
	unsigned int div_sel;
	unsigned int channel_sel0, channel_sel1;

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);

	/* phy_div: 0=div6, 1=div 7, 2=div8, 3=div10 */
	switch (pconf->lcd_basic.lcd_bits) {
	case 6:
		div_sel = 0;
		break;
	case 8:
		div_sel = 2;
		break;
	default:
		div_sel = 2;
		break;
	}

	/* fifo_clk_sel[7:6]: 0=div6, 1=div 7, 2=div8, 3=div10 */
	lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL0, (div_sel << 6));
	/* serializer_en[27:16] */
	lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 0xfff, 16, 12);

	switch (lcd_drv->chip_type) {
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
		/* pn swap[2] */
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 1, 2, 1);

		/* fifo enable[30], phy_clock gating[24] */
		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL1, (1 << 30) | (1 << 24));
		/* fifo write enable[31] */
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL1, 1, 31, 1);

		channel_sel0 = pconf->lcd_control.mlvds_config->channel_sel0;
		channel_sel1 = pconf->lcd_control.mlvds_config->channel_sel1;
		lcd_vcbus_write(P2P_CH_SWAP0, channel_sel0);
		lcd_vcbus_write(P2P_CH_SWAP1, channel_sel1);
		break;
	default:
		/* decoupling fifo enable, gated clock enable */
		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL1,
			      (1 << 30) | (0 << 25) | (1 << 24));
		/* decoupling fifo write enable after fifo enable */
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL1, 1, 31, 1);
		break;
	}

	lcd_tcon_enable(pconf);
}

static void lcd_mlvds_disable(void)
{
	lcd_tcon_disable();
}
#endif

static void lcd_vbyone_sync_pol(int hsync_pol, int vsync_pol)
{
	lcd_vcbus_setb(VBO_VIN_CTRL, hsync_pol, 4, 1);
	lcd_vcbus_setb(VBO_VIN_CTRL, vsync_pol, 5, 1);

	lcd_vcbus_setb(VBO_VIN_CTRL, hsync_pol, 6, 1);
	lcd_vcbus_setb(VBO_VIN_CTRL, vsync_pol, 7, 1);
}

static void lcd_vbyone_clk_util_set(struct lcd_config_s *pconf)
{
	unsigned int lcd_bits;
	unsigned int div_sel, phy_div;
	struct lcd_drv_s *lcd_drv = lcd_get_driver();

	phy_div = pconf->lcd_control.vbyone_config->phy_div;
	lcd_bits = pconf->lcd_basic.lcd_bits;

	switch (lcd_bits) {
	case 6:
		div_sel = 0;
		break;
	case 8:
		div_sel = 2;
		break;
	case 10:
		div_sel = 3;
		break;
	default:
		div_sel = 3;
		break;
	}

	if (lcd_drv->chip_type == LCD_CHIP_T7) {
		/* set fifo_clk_sel*/
		lcd_combo_dphy_write(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0,
				(div_sel << 5));
		/* set cntl_ser_en:  8-channel to 1 */
		lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0, 0xff, 16, 8);
	} else {
		/* set fifo_clk_sel*/
		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL0, (div_sel << 6));
		/* set cntl_ser_en:  8-channel to 1 */
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 0xfff, 16, 12);
	}

	switch (lcd_drv->chip_type) { /* pn swap */
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 1, 2, 1);
		break;
	case LCD_CHIP_T7:
		lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0, 1, 2, 1);
		break;
	default:
		break;
	}

	if (lcd_drv->chip_type == LCD_CHIP_T7) {
		/* decoupling fifo enable, gated clock enable */
		lcd_combo_dphy_write(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1,
				(1 << 6) | (1 << 0));
		/* decoupling fifo write enable after fifo enable */
		lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1, 1, 7, 1);
	} else {
		/* decoupling fifo enable, gated clock enable */
		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL1,
			      (1 << 30) | ((phy_div - 1) << 25) | (1 << 24));
		/* decoupling fifo write enable after fifo enable */
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL1, 1, 31, 1);
	}
}

static int lcd_vbyone_lanes_set(int lane_num, int byte_mode, int region_num,
		int hsize, int vsize)
{
	int sublane_num;
	int region_size[4];
	int tmp;

	switch (lane_num) {
	case 1:
	case 2:
	case 4:
	case 8:
		break;
	default:
		return -1;
	}
	switch (region_num) {
	case 1:
	case 2:
	case 4:
		break;
	default:
		return -1;
	}
	if (lane_num % region_num)
		return -1;
	switch (byte_mode) {
	case 3:
	case 4:
		break;
	default:
		return -1;
	}
	if (lcd_debug_print_flag) {
		LCDPR("byte_mode=%d, lane_num=%d, region_num=%d\n",
			byte_mode, lane_num, region_num);
	}

	sublane_num = lane_num / region_num; /* lane num in each region */
	lcd_vcbus_setb(VBO_LANES, (lane_num - 1), 0, 3);
	lcd_vcbus_setb(VBO_LANES, (region_num - 1), 4, 2);
	lcd_vcbus_setb(VBO_LANES, (sublane_num - 1), 8, 3);
	lcd_vcbus_setb(VBO_LANES, (byte_mode - 1), 11, 2);

	if (region_num > 1) {
		region_size[3] = (hsize / lane_num) * sublane_num;
		tmp = (hsize % lane_num);
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
	}
	lcd_vcbus_write(VBO_ACT_VSIZE, vsize);
	/* different from FBC code!!! */
	/* lcd_vcbus_setb(VBO_CTRL_H,0x80,11,5); */
	/* different from simulation code!!! */
	lcd_vcbus_setb(VBO_CTRL_H, 0x0, 0, 4);
	lcd_vcbus_setb(VBO_CTRL_H, 0x1, 9, 1);
	/* lcd_vcbus_setb(VBO_CTRL_L,enable,0,1); */

	return 0;
}

static void lcd_vbyone_hw_filter(int flag)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();
	struct vbyone_config_s *vx1_conf;
	unsigned int temp, period;
	unsigned int tick_period[] = {
		0xfff,
		0xff,    /* 1: 0.8us */
		0x1ff,   /* 2: 1.7us */
		0x3ff,   /* 3: 3.4us */
		0x7ff,   /* 4: 6.9us */
		0xfff,   /* 5: 13.8us */
		0x1fff,  /* 6: 27us */
		0x3fff,  /* 7: 55us */
		0x7fff,  /* 8: 110us */
		0xffff,  /* 9: 221us */
		0x1ffff, /* 10: 441us */
		0x3ffff, /* 11: 883us */
		0x7ffff, /* 12: 1.76ms */
		0xfffff, /* 13: 3.53ms */
	};

	vx1_conf = lcd_drv->lcd_config->lcd_control.vbyone_config;
	switch (lcd_drv->chip_type) {
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
	case LCD_CHIP_T7:
		if (flag) {
			period = vx1_conf->hw_filter_time & 0xff;
			if (period >=
				(sizeof(tick_period) / sizeof(unsigned int)))
				period = tick_period[0];
			else
				period = tick_period[period];
			temp = period & 0xffff;
			lcd_vcbus_write(VBO_INFILTER_TICK_PERIOD_L, temp);
			temp = (period >> 16) & 0xf;
			lcd_vcbus_write(VBO_INFILTER_TICK_PERIOD_H, temp);
			/* hpd */
			temp = vx1_conf->hw_filter_cnt & 0xff;
			if (temp == 0xff) {
				lcd_vcbus_setb(VBO_INSGN_CTRL, 0, 8, 4);
			} else {
				temp = (temp == 0) ? 0x7 : temp;
				lcd_vcbus_setb(VBO_INSGN_CTRL, temp, 8, 4);
			}
			/* lockn */
			temp = (vx1_conf->hw_filter_cnt >> 8) & 0xff;
			if (temp == 0xff) {
				lcd_vcbus_setb(VBO_INSGN_CTRL, 0, 12, 4);
			} else {
				temp = (temp == 0) ? 0x7 : temp;
				lcd_vcbus_setb(VBO_INSGN_CTRL, temp, 12, 4);
			}
		} else {
			temp = (vx1_conf->hw_filter_time >> 8) & 0x1;
			if (temp) {
				lcd_vcbus_write(VBO_INFILTER_TICK_PERIOD_L,
						0xff);
				lcd_vcbus_write(VBO_INFILTER_TICK_PERIOD_H,
						0x0);
				lcd_vcbus_setb(VBO_INSGN_CTRL, 0, 8, 4);
				lcd_vcbus_setb(VBO_INSGN_CTRL, 0, 12, 4);
				LCDPR("%s: %d disable for debug\n",
				      __func__, flag);
			}
		}
		break;
	default:
		if (flag) {
			lcd_vcbus_setb(VBO_INFILTER_CTRL, 0xff, 8, 8);
			/* hpd */
			temp = vx1_conf->hw_filter_cnt & 0xff;
			if (temp == 0xff) {
				lcd_vcbus_setb(VBO_INFILTER_CTRL, 0, 0, 3);
			} else {
				temp = (temp == 0) ? 0x7 : temp;
				lcd_vcbus_setb(VBO_INFILTER_CTRL, temp, 0, 3);
			}
			/* lockn */
			temp = (vx1_conf->hw_filter_cnt >> 8) & 0xff;
			if (temp == 0xff) {
				lcd_vcbus_setb(VBO_INFILTER_CTRL, 0, 4, 3);
			} else {
				temp = (temp == 0) ? 0x7 : temp;
				lcd_vcbus_setb(VBO_INFILTER_CTRL, temp, 4, 3);
			}
		} else {
			temp = (vx1_conf->hw_filter_time >> 8) & 0x1;
			if (temp) {
				lcd_vcbus_write(VBO_INFILTER_CTRL, 0xff00);
				LCDPR("%s: %d disable for debug\n",
				      __func__, flag);
			}
		}
		break;
	}
}

static void lcd_vbyone_sw_reset(void)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);

	/* force PHY to 0 */
	if (lcd_drv->chip_type == LCD_CHIP_T7)
		lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0, 3, 8, 2);
	else
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 3, 8, 2);
	lcd_vcbus_write(VBO_SOFT_RST, 0x1ff);
	udelay(5);
	/* realease PHY */
	if (lcd_drv->chip_type == LCD_CHIP_T7)
		lcd_combo_dphy_setb(COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0, 0, 8, 2);
	else
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 0, 8, 2);
	lcd_vcbus_write(VBO_SOFT_RST, 0);
}

static void lcd_vbyone_wait_timing_stable(void)
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
	if (lcd_debug_print_flag) {
		LCDPR("vbyone timing state: 0x%03x, i=%d\n",
			timing_state, (200 - i));
	}
	mdelay(2);
}

static void lcd_vbyone_cdr_training_hold(struct vbyone_config_s *vx1_conf, int flag)
{
	if (flag) {
		LCDPR("ctrl_flag for cdr_training_hold\n");
		lcd_vcbus_setb(VBO_FSM_HOLDER_H, 0xffff, 0, 16);
	} else {
		mdelay(vx1_conf->cdr_training_hold);
		lcd_vcbus_setb(VBO_FSM_HOLDER_H, 0, 0, 16);
	}
}

static void lcd_vbyone_control_set(struct lcd_config_s *pconf)
{
	int lane_count, byte_mode, region_num, hsize, vsize;
	/* int color_fmt; */
	int vin_color, vin_bpp;

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);

	hsize = pconf->lcd_basic.h_active;
	vsize = pconf->lcd_basic.v_active;
	lane_count = pconf->lcd_control.vbyone_config->lane_count; /* 8 */
	region_num = pconf->lcd_control.vbyone_config->region_num; /* 2 */
	byte_mode = pconf->lcd_control.vbyone_config->byte_mode; /* 4 */
	/* color_fmt = pconf->lcd_control.vbyone_config->color_fmt; // 4 */

	lcd_vbyone_clk_util_set(pconf);
#if 0
	switch (color_fmt) {
	case 0:/* SDVT_VBYONE_18BPP_RGB */
		vin_color = 4;
		vin_bpp   = 2;
		break;
	case 1:/* SDVT_VBYONE_18BPP_YCBCR444 */
		vin_color = 0;
		vin_bpp   = 2;
		break;
	case 2:/* SDVT_VBYONE_24BPP_RGB */
		vin_color = 4;
		vin_bpp   = 1;
		break;
	case 3:/* SDVT_VBYONE_24BPP_YCBCR444 */
		vin_color = 0;
		vin_bpp   = 1;
		break;
	case 4:/* SDVT_VBYONE_30BPP_RGB */
		vin_color = 4;
		vin_bpp   = 0;
		break;
	case 5:/* SDVT_VBYONE_30BPP_YCBCR444 */
		vin_color = 0;
		vin_bpp   = 0;
		break;
	default:
		LCDERR("vbyone COLOR_FORMAT unsupport\n");
		return;
	}
#else
	vin_color = 4; /* fixed RGB */
	vin_bpp   = 0; /* fixed 30bbp 4:4:4 */
#endif

	/* set Vbyone vin color format */
	lcd_vcbus_setb(VBO_VIN_CTRL, vin_color, 8, 3);
	lcd_vcbus_setb(VBO_VIN_CTRL, vin_bpp, 11, 2);

	lcd_vbyone_lanes_set(lane_count, byte_mode, region_num, hsize, vsize);
	/*set hsync/vsync polarity to let the polarity is low active
	inside the VbyOne */
	lcd_vbyone_sync_pol(0, 0);

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

	lcd_vbyone_hw_filter(1);
	lcd_vcbus_setb(VBO_INSGN_CTRL, 0, 2, 2);

	lcd_vcbus_setb(VBO_CTRL_L, 1, 0, 1);

	/*force vencl clk enable, otherwise, it might auto turn off by mipi DSI
	//lcd_vcbus_setb(VPU_MISC_CTRL, 1, 0, 1); */

	lcd_vbyone_wait_timing_stable();
	lcd_vbyone_sw_reset();

	if ((pconf->lcd_control.vbyone_config->ctrl_flag) & 0x4) /* training hold */
		lcd_vbyone_cdr_training_hold(pconf->lcd_control.vbyone_config, 1);
}

static void lcd_vbyone_disable(void)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();

	lcd_vcbus_setb(VBO_CTRL_L, 0, 0, 1);
	switch (lcd_drv->chip_type) {
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
	case LCD_CHIP_T7:
		lcd_vcbus_setb(VBO_INSGN_CTRL, 0, 2, 1);
		lcd_vcbus_setb(VBO_INSGN_CTRL, 0, 0, 1);
		break;
	default:
		break;
	}
}

#define VX1_LOCKN_WAIT_TIMEOUT    500 /* 500ms */
static void lcd_vbyone_wait_stable(struct lcd_config_s *pconf)
{
	int i = 0;
	int ret = 1;

	/* training hold */
	if (pconf->lcd_control.vbyone_config->ctrl_flag & 0x4)
		lcd_vbyone_cdr_training_hold
			(pconf->lcd_control.vbyone_config, 0);
	while (i++ < VX1_LOCKN_WAIT_TIMEOUT) {
		ret = lcd_vcbus_read(VBO_STATUS_L) & 0x3f;
		if (ret == 0x20)
			break;
		mdelay(1);
	}
	LCDPR("%s status: 0x%x, i=%d\n", __func__,
	      lcd_vcbus_read(VBO_STATUS_L), i);
	/* power on reset */
	if (pconf->lcd_control.vbyone_config->ctrl_flag & 0x1) {
		LCDPR("ctrl_flag for power on reset\n");
		mdelay
		(pconf->lcd_control.vbyone_config->power_on_reset_delay);
		lcd_vbyone_sw_reset();
	}
}


#define VX1_HPD_WAIT_TIMEOUT    5000 /* 500ms */
static void lcd_vbyone_wait_hpd(struct lcd_config_s *pconf)
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

	/* musk htpd */
	lcd_vcbus_setb(VBO_INSGN_CTRL, 1, 2, 2);

	if ((pconf->lcd_control.vbyone_config->ctrl_flag) & 0x2) {
		LCDPR("ctrl_flag for hpd_data delay\n");
		mdelay(pconf->lcd_control.vbyone_config->hpd_data_delay);
	} else {
		mdelay(10); /* add 10ms delay for compatibility */
	}
}

#ifdef CONFIG_AML_LCD_TCON
static void lcd_p2p_control_set(struct lcd_config_s *pconf)
{
	unsigned int phy_div;
	unsigned int channel_sel0, channel_sel1;

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);

	/* phy_div: 0=div6, 1=div 7, 2=div8, 3=div10 */
	switch (pconf->lcd_control.p2p_config->p2p_type) {
	case P2P_CHPI: /* 8/10 coding */
	case P2P_USIT:
		phy_div = 3;
		break;
	default:
		phy_div = 2;
		break;
	}

	/* fifo_clk_sel[7:6]: 0=div6, 1=div 7, 2=div8, 3=div10 */
	lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL0, (phy_div << 6));
	/* serializer_en[27:16] */
	lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 0xfff, 16, 12);
	/* pn swap[2] */
	lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 1, 2, 1);

	/* fifo enable[30], phy_clock gating[24] */
	lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL1, (1 << 30) | (1 << 24));
	/* fifo write enable[31] */
	lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL1, 1, 31, 1);

	channel_sel0 = pconf->lcd_control.p2p_config->channel_sel0;
	channel_sel1 = pconf->lcd_control.p2p_config->channel_sel1;
	lcd_vcbus_write(P2P_CH_SWAP0, channel_sel0);
	lcd_vcbus_write(P2P_CH_SWAP1, channel_sel1);

	lcd_tcon_enable(pconf);
}

static void lcd_p2p_disable(void)
{
	lcd_tcon_disable();
}
#endif

static unsigned int vbyone_lane_num[] = {
	1,
	2,
	4,
	8,
	8,
};

#define VBYONE_BIT_RATE_MAX		3100 //MHz
#define VBYONE_BIT_RATE_MIN		600
static void lcd_vbyone_config_set(struct lcd_config_s *pconf)
{
	unsigned int band_width, bit_rate, pclk, phy_div;
	unsigned int byte_mode, lane_count, minlane;
	unsigned int temp, i;

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);

	//auto calculate bandwidth, clock
	lane_count = pconf->lcd_control.vbyone_config->lane_count;
	byte_mode = pconf->lcd_control.vbyone_config->byte_mode;
	/* byte_mode * byte2bit * 8/10_encoding * pclk =
	   byte_mode * 8 * 10 / 8 * pclk */
	pclk = pconf->lcd_timing.lcd_clk / 1000; /* kHz */
	band_width = byte_mode * 10 * pclk;

	temp = VBYONE_BIT_RATE_MAX * 1000;
	temp = (band_width + temp - 1) / temp;
	for (i = 0; i < 4; i++) {
		if (temp <= vbyone_lane_num[i])
			break;
	}
	minlane = vbyone_lane_num[i];
	if (lane_count < minlane) {
		LCDERR("vbyone lane_num(%d) is less than min(%d)\n",
			lane_count, minlane);
		lane_count = minlane;
		pconf->lcd_control.vbyone_config->lane_count = lane_count;
		LCDPR("change to min lane_num %d\n", minlane);
	}

	bit_rate = band_width / lane_count;
	phy_div = lane_count / lane_count;
	if (phy_div == 8) {
		phy_div /= 2;
		bit_rate /= 2;
	}
	if (bit_rate > (VBYONE_BIT_RATE_MAX * 1000)) {
		LCDERR("vbyone bit rate(%dKHz) is out of max(%dKHz)\n",
			bit_rate, (VBYONE_BIT_RATE_MAX * 1000));
	}
	if (bit_rate < (VBYONE_BIT_RATE_MIN * 1000)) {
		LCDERR("vbyone bit rate(%dKHz) is out of min(%dKHz)\n",
			bit_rate, (VBYONE_BIT_RATE_MIN * 1000));
	}
	bit_rate = bit_rate * 1000; /* Hz */

	pconf->lcd_control.vbyone_config->phy_div = phy_div;
	pconf->lcd_timing.bit_rate = bit_rate;

	if (lcd_debug_print_flag) {
		LCDPR("lane_count=%u, bit_rate = %uMHz, pclk=%u.%03uMhz\n",
			lane_count, (bit_rate / 1000000),
			(pclk / 1000), (pclk % 1000));
	}
}

#ifdef CONFIG_AML_LCD_TCON
static void lcd_mlvds_config_set(struct lcd_config_s *pconf)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();
	unsigned int bit_rate, pclk;
	unsigned int lcd_bits, channel_num;
	unsigned int channel_sel0, channel_sel1, pi_clk_sel = 0;
	unsigned int i, temp;

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);

	lcd_bits = pconf->lcd_basic.lcd_bits;
	channel_num = pconf->lcd_control.mlvds_config->channel_num;
	pclk = pconf->lcd_timing.lcd_clk / 1000;
	bit_rate = lcd_bits * 3 * pclk / channel_num;

	pconf->lcd_timing.bit_rate = bit_rate * 1000;

	if (lcd_debug_print_flag) {
		LCDPR("channel_num=%u, bit_rate=%u.%03uMHz, pclk=%u.%03uMhz\n",
		      channel_num, (bit_rate / 1000), (bit_rate % 1000),
		      (pclk / 1000), (pclk % 1000));
	}

	/* pi_clk select */
	channel_sel0 = pconf->lcd_control.mlvds_config->channel_sel0;
	channel_sel1 = pconf->lcd_control.mlvds_config->channel_sel1;
	switch (lcd_drv->chip_type) {
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
	case LCD_CHIP_T7:
		/* mlvds channel:    //tx 12 channels
		 *    0: clk_a
		 *    1: d0_a
		 *    2: d1_a
		 *    3: d2_a
		 *    4: d3_a
		 *    5: d4_a
		 *    6: clk_b
		 *    7: d0_b
		 *    8: d1_b
		 *    9: d2_b
		 *   10: d3_b
		 *   11: d4_b
		 */
		for (i = 0; i < 8; i++) {
			temp = (channel_sel0 >> (i * 4)) & 0xf;
			if (temp == 0 || temp == 6)
				pi_clk_sel |= (1 << i);
		}
		for (i = 0; i < 4; i++) {
			temp = (channel_sel1 >> (i * 4)) & 0xf;
			if (temp == 0 || temp == 6)
				pi_clk_sel |= (1 << (i + 8));
		}
		break;
	default:
		break;
	}

	pconf->lcd_control.mlvds_config->pi_clk_sel = pi_clk_sel;
	if (lcd_debug_print_flag) {
		LCDPR
	("channel_sel0=0x%08x, channel_sel1=0x%08x, pi_clk_sel=0x%03x\n",
	 channel_sel0, channel_sel1, pi_clk_sel);
	}
}

static void lcd_p2p_config_set(struct lcd_config_s *pconf)
{
	unsigned int bit_rate, pclk;
	unsigned int lcd_bits, lane_num;

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);

	lcd_bits = pconf->lcd_basic.lcd_bits;
	lane_num = pconf->lcd_control.p2p_config->lane_num;
	pclk = pconf->lcd_timing.lcd_clk / 1000;
	switch (pconf->lcd_control.p2p_config->p2p_type) {
	case P2P_CEDS:
		if (pclk >= 600000)
			bit_rate = pclk * 3 * lcd_bits / lane_num;
		else
			bit_rate = pclk * (3 * lcd_bits + 4) / lane_num;
		break;
	case P2P_CHPI: /* 8/10 coding */
		bit_rate = (pclk * 3 * lcd_bits * 10 / 8) / lane_num;
		break;
	default:
		bit_rate = pclk * 3 * lcd_bits / lane_num;
		break;
	}

	pconf->lcd_timing.bit_rate = bit_rate * 1000;

	if (lcd_debug_print_flag) {
		LCDPR("lane_num=%u, bit_rate=%u.%03uMHz, pclk=%u.%03uMhz\n",
		      lane_num, (bit_rate / 1000), (bit_rate % 1000),
		      (pclk / 1000), (pclk % 1000));
	}
}
#endif

void lcd_tv_config_update(struct lcd_config_s *pconf)
{
	/* update interface timing */
	switch (pconf->lcd_basic.lcd_type) {
	case LCD_VBYONE:
		lcd_vbyone_config_set(pconf);
		break;
#ifdef CONFIG_AML_LCD_TCON
	case LCD_MLVDS:
		lcd_mlvds_config_set(pconf);
		break;
	case LCD_P2P:
		lcd_p2p_config_set(pconf);
		break;
#endif
	default:
		break;
	}
}

void lcd_tv_driver_init_pre(void)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();
	struct lcd_config_s *pconf;
	int ret;

	pconf = lcd_drv->lcd_config;
	LCDPR("tv driver init(ver %s): %s\n", lcd_drv->version,
		lcd_type_type_to_str(pconf->lcd_basic.lcd_type));
	ret = lcd_type_supported(pconf);
	if (ret)
		return;

	lcd_clk_set(pconf);
	lcd_venc_set(pconf);
	lcd_encl_tcon_set(pconf);
}

int lcd_tv_driver_init(void)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();
	struct lcd_config_s *pconf;
	int ret;

	pconf = lcd_drv->lcd_config;
	ret = lcd_type_supported(pconf);
	if (ret)
		return -1;

	/* init driver */
	switch (pconf->lcd_basic.lcd_type) {
	case LCD_LVDS:
		lcd_lvds_control_set(pconf);
		lcd_lvds_phy_set(pconf, 1);
		break;
	case LCD_VBYONE:
		lcd_pinmux_set(1);
		lcd_vbyone_control_set(pconf);
		lcd_vbyone_wait_hpd(pconf);
		lcd_vbyone_phy_set(pconf, 1);
		lcd_vbyone_wait_stable(pconf);
		break;
#ifdef CONFIG_AML_LCD_TCON
	case LCD_MLVDS:
		lcd_mlvds_control_set(pconf);
		lcd_pinmux_set(1);
		lcd_mlvds_phy_set(pconf, 1);
		break;
	case LCD_P2P:
		lcd_pinmux_set(1);
		lcd_p2p_phy_set(pconf, 1);
		lcd_p2p_control_set(pconf);
		break;
#endif
	default:
		break;
	}

	lcd_vcbus_write(VENC_INTCTRL, 0x200);

	if (lcd_debug_print_flag)
		LCDPR("%s finished\n", __func__);
	return 0;
}

void lcd_tv_driver_disable(void)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();
	struct lcd_config_s *pconf;
	int ret;

	LCDPR("disable driver\n");
	pconf = lcd_drv->lcd_config;
	ret = lcd_type_supported(pconf);
	if (ret)
		return;

	switch (pconf->lcd_basic.lcd_type) {
	case LCD_LVDS:
		lcd_lvds_phy_set(pconf, 0);
		lcd_lvds_disable();
		break;
	case LCD_VBYONE:
		lcd_vbyone_phy_set(pconf, 0);
		lcd_pinmux_set(0);
		lcd_vbyone_disable();
		break;
#ifdef CONFIG_AML_LCD_TCON
	case LCD_MLVDS:
		lcd_mlvds_disable();
		lcd_mlvds_phy_set(pconf, 0);
		lcd_pinmux_set(0);
		break;
	case LCD_P2P:
		lcd_p2p_disable();
		lcd_p2p_phy_set(pconf, 0);
		lcd_pinmux_set(0);
		break;
#endif
	default:
		break;
	}

	if (lcd_debug_print_flag)
		LCDPR("%s finished\n", __func__);
}

