/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/lcd/aml_lcd_phy_config.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <amlogic/aml_lcd.h>
#include "aml_lcd_reg.h"
#include "aml_lcd_phy_config.h"
#include "aml_lcd_common.h"

static int phy_ctrl_bit_on;
static struct lcd_phy_ctrl_s *lcd_phy_ctrl;

static unsigned int lcd_lvds_channel_on_value(struct lcd_config_s *pconf)
{
	unsigned int channel_on = 0;

	if (pconf->lcd_control.lvds_config->dual_port == 0) {
		if (pconf->lcd_control.lvds_config->lane_reverse == 0) {
			switch (pconf->lcd_basic.lcd_bits) {
			case 6:
				channel_on = 0xf;
				break;
			case 8:
				channel_on = 0x1f;
				break;
			case 10:
			default:
				channel_on = 0x3f;
				break;
			}
		} else {
			switch (pconf->lcd_basic.lcd_bits) {
			case 6:
				channel_on = 0x3c;
				break;
			case 8:
				channel_on = 0x3e;
				break;
			case 10:
			default:
				channel_on = 0x3f;
				break;
			}
		}
		if (pconf->lcd_control.lvds_config->port_swap == 1)
			channel_on = (channel_on << 6); /* use channel B */
	} else {
		if (pconf->lcd_control.lvds_config->lane_reverse == 0) {
			switch (pconf->lcd_basic.lcd_bits) {
			case 6:
				channel_on = 0x3cf;
				break;
			case 8:
				channel_on = 0x7df;
				break;
			case 10:
			default:
				channel_on = 0xfff;
				break;
			}
		} else {
			switch (pconf->lcd_basic.lcd_bits) {
			case 6:
				channel_on = 0xf3c;
				break;
			case 8:
				channel_on = 0xfbe;
				break;
			case 10:
			default:
				channel_on = 0xfff;
				break;
			}
		}
	}
	return channel_on;
}

static void lcd_phy_cntl_set_tl1(struct phy_config_s *phy, int status,
				 int bypass, unsigned int mode,
				 unsigned int ckdi)
{
	unsigned int tmp = 0;
	unsigned int chreg = 0, data = 0;
	unsigned int cntl16 = 0;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		chreg |= ((phy_ctrl_bit_on << 16) | (phy_ctrl_bit_on << 0));
		if (bypass)
			tmp |= ((1 << 18) | (1 << 2));
		if (mode) {
			chreg |= lvds_vx1_p2p_phy_ch_tl1;
		} else {
			chreg |= p2p_low_common_phy_ch_tl1;
			if (phy->weakly_pull_down)
				chreg &= ~((1 << 19) | (1 << 3));
		}
		cntl16 = ckdi | 0x80000000;
	} else {
		if (phy_ctrl_bit_on)
			data = 0;
		else
			data = 1;
		chreg |= ((data << 16) | (data << 0));
		cntl16 = 0;
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, 0);
	}

	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL15, tmp);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL16, cntl16);

	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL8, tmp);
	data = ((phy->lane[0].preem & 0xff) << 8) |
		((phy->lane[1].preem & 0xff) << 24);
	if (data)
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL9, tmp);
	data = ((phy->lane[2].preem & 0xff) << 8) |
		((phy->lane[3].preem & 0xff) << 24);
	if (data)
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL10, tmp);
	data = ((phy->lane[4].preem & 0xff) << 8) |
		((phy->lane[5].preem & 0xff) << 24);
	if (data)
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL11, tmp);
	data = ((phy->lane[6].preem & 0xff) << 8) |
		((phy->lane[7].preem & 0xff) << 24);
	if (data)
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL4, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL12, tmp);
	data = ((phy->lane[8].preem & 0xff) << 8) |
		((phy->lane[9].preem & 0xff) << 24);
	if (data)
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL6, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL13, tmp);
	data = ((phy->lane[10].preem & 0xff) << 8) |
		((phy->lane[11].preem & 0xff) << 24);
	if (data)
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL7, chreg | data);
}

/*
 *    chreg: channel ctrl
 *    bypass: 1=bypass
 *    mode: 1=normal mode, 0=low common mode
 *    ckdi: clk phase for minilvds
 */
static void lcd_phy_cntl_set_t5(struct phy_config_s *phy, int status,
				int bypass, unsigned int mode,
				unsigned int ckdi)
{
	unsigned int cntl15 = 0, cntl16 = 0;
	unsigned int chreg = 0, data = 0;
	unsigned int tmp = 0;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		chreg |= ((phy_ctrl_bit_on << 16) | (phy_ctrl_bit_on << 0));
		if (bypass)
			tmp |= ((1 << 18) | (1 << 2));
		if (mode) {
			chreg |= lvds_vx1_p2p_phy_ch_tl1;
			cntl15 = 0x00070000;
		} else {
			chreg |= p2p_low_common_phy_ch_tl1;
			if (phy->weakly_pull_down)
				chreg &= ~((1 << 19) | (1 << 3));
			cntl15 = 0x000e0000;
		}
		cntl16 = ckdi | 0x80000000;
	} else {
		if (phy_ctrl_bit_on)
			data = 0;
		else
			data = 1;
		chreg |= ((data << 16) | (data << 0));
		cntl15 = 0;
		cntl16 = 0;
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, 0);
	}

	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL15, cntl15);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL16, cntl16);

	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL8, tmp);
	data = ((phy->lane[0].preem & 0xff) << 8) |
		((phy->lane[1].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL9, tmp);
	data = ((phy->lane[2].preem & 0xff) << 8) |
		((phy->lane[3].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL10, tmp);
	data = ((phy->lane[4].preem & 0xff) << 8) |
		((phy->lane[5].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL11, tmp);
	data = ((phy->lane[6].preem & 0xff) << 8) |
		((phy->lane[7].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL4, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL12, tmp);
	data = ((phy->lane[8].preem & 0xff) << 8) |
		((phy->lane[9].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL6, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL13, tmp);
	data = ((phy->lane[10].preem & 0xff) << 8) |
		((phy->lane[11].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL7, chreg | data);
}

static void lcd_phy_cntl_set_t5w(struct phy_config_s *phy, int status,
				 int bypass, unsigned int mode,
				 unsigned int ckdi)
{
	unsigned int cntl15 = 0, cntl16 = 0;
	unsigned int chreg = 0, data = 0;
	unsigned int tmp = 0;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		chreg |= ((phy_ctrl_bit_on << 16) | (phy_ctrl_bit_on << 0));
		tmp |= ((0x7 << 19) | (0x7 << 3));
		if (bypass)
			tmp |= ((1 << 18) | (1 << 2));
		if (mode) {
			chreg |= lvds_vx1_p2p_phy_ch_tl1;
			cntl15 = 0x00070000;
		} else {
			chreg |= p2p_low_common_phy_ch_tl1;
			if (phy->weakly_pull_down)
				chreg &= ~((1 << 19) | (1 << 3));
			cntl15 = 0x000e0000;
		}
		cntl16 = ckdi | 0x80000000;
	} else {
		if (phy_ctrl_bit_on)
			data = 0;
		else
			data = 1;
		chreg |= ((data << 16) | (data << 0));
		cntl15 = 0;
		cntl16 = 0;
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, 0);
	}

	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL15, cntl15);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL16, cntl16);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL8, tmp);
	data = ((phy->lane[0].preem & 0xff) << 8) |
		((phy->lane[1].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL9, tmp);
	data = ((phy->lane[2].preem & 0xff) << 8) |
		((phy->lane[3].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL10, tmp);
	data = ((phy->lane[4].preem & 0xff) << 8) |
		((phy->lane[5].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL11, tmp);
	data = ((phy->lane[6].preem & 0xff) << 8) |
		((phy->lane[7].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL4, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL12, tmp);
	data = ((phy->lane[8].preem & 0xff) << 8) |
		((phy->lane[9].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL6, chreg | data);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL13, tmp);
	data = ((phy->lane[10].preem & 0xff) << 8) |
		((phy->lane[11].preem & 0xff) << 24);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL7, chreg | data);
}

void lcd_phy_tcon_chpi_bbc_init_tl1(struct lcd_config_s *pconf)
{
	unsigned int data32 = 0;
	unsigned int chreg = 0, preem;
	unsigned int size;
	unsigned int n = 10;
	struct p2p_config_s *p2p_conf;

	n = getenv_ulong("tcon_delay", 10, 10);
	p2p_conf = pconf->lcd_control.p2p_config;
	size = sizeof(p2p_low_common_phy_preem_tl1) / sizeof(unsigned int);

	/*get tcon tx pre_emphasis*/
	preem = p2p_conf->phy_preem & 0xf;

	/*check tx pre_emphasis ok or no*/
	if (preem >= size) {
		LCDERR("%s: invalid preem=0x%x, use default\n",
		       __func__, preem);
		preem = 0x1;
	}

	udelay(n);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL1, 1, 3, 1);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL1, 1, 19, 1);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL2, 1, 3, 1);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL2, 1, 19, 1);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL3, 1, 3, 1);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL3, 1, 19, 1);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL4, 1, 3, 1);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL4, 1, 19, 1);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL6, 1, 3, 1);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL6, 1, 19, 1);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL7, 1, 3, 1);
	lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL7, 1, 19, 1);
	LCDPR("%s: delay: %dus\n", __func__, n);

	/*follow pre-emphasis*/
	data32 = p2p_low_common_phy_preem_tl1[preem];
	chreg |= ((data32 << 24) | (data32 << 8));

	if (phy_ctrl_bit_on)
		chreg &= ~((1 << 16) | (1 << 0));
	else
		chreg |= ((1 << 16) | (1 << 0));

	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, chreg);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, chreg);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, chreg);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL4, chreg);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL6, chreg);
	lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL7, chreg);
}

static void lcd_lvds_phy_set_txl(struct lcd_config_s *pconf, int status)
{
	unsigned int vswing, preem, clk_vswing, clk_preem, channel_on;
	unsigned int data32 = 0;
	struct lvds_config_s *lvds_conf;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	lvds_conf = pconf->lcd_control.lvds_config;
	if (status) {
		vswing = lvds_conf->phy_vswing & 0xf;
		preem = lvds_conf->phy_preem & 0xf;
		clk_vswing = lvds_conf->phy_clk_vswing & 0xf;
		clk_preem = lvds_conf->phy_clk_preem & 0xf;
		if (lcd_debug_print_flag)
			LCDPR("vswing=0x%x, preem=0x%x\n", vswing, preem);

		if (vswing > 7) {
			LCDERR("%s: invalid vswing=0x%x, use default\n",
				__func__, vswing);
			vswing = LVDS_PHY_VSWING_DFT;
		}
		if (preem > 7) {
			LCDERR("%s: invalid preem=0x%x, use default\n",
				__func__, preem);
			preem = LVDS_PHY_PREEM_DFT;
		}
		if (clk_vswing > 3) {
			LCDERR("%s: invalid clk_vswing=0x%x, use default\n",
				__func__, clk_vswing);
			clk_vswing = LVDS_PHY_CLK_VSWING_DFT;
		}
		if (clk_preem > 7) {
			LCDERR("%s: invalid clk_preem=0x%x, use default\n",
				__func__, clk_preem);
			clk_preem = LVDS_PHY_CLK_PREEM_DFT;
		}
		channel_on = lcd_lvds_channel_on_value(pconf);

		data32 = LVDS_PHY_CNTL1_G9TV |
			(vswing << 26) | (preem << 0);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, data32);
		data32 = LVDS_PHY_CNTL2_G9TV;
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, data32);
		data32 = LVDS_PHY_CNTL3_G9TV |
			(channel_on << 16) |
			(clk_vswing << 8) |
			(clk_preem << 5);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, data32);
	} else {
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, 0);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, 0);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, 0);
	}
}

static void lcd_lvds_phy_set_txhd(struct lcd_config_s *pconf, int status)
{
	unsigned int vswing, preem;
	unsigned int data32 = 0;
	struct lvds_config_s *lvds_conf;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	lvds_conf = pconf->lcd_control.lvds_config;
	if (status) {
		vswing = lvds_conf->phy_vswing & 0xf;
		preem = lvds_conf->phy_preem & 0xf;
		if (lcd_debug_print_flag)
			LCDPR("vswing=0x%x, preem=0x%x\n", vswing, preem);

		if (preem > 3) {
			LCDERR("%s: invalid preem=%d, use default\n",
				__func__, preem);
			preem = LVDS_PHY_PREEM_DFT;
		}

		data32 = LVDS_PHY_CNTL1_TXHD |
			(vswing << 3) | (vswing << 0) | (preem << 23);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, data32);
		data32 = LVDS_PHY_CNTL2_TXHD |
			(preem << 14) | (preem << 12) |
			(preem << 26) | (preem << 24);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, data32);
		data32 = LVDS_PHY_CNTL3_TXHD |
			(preem << 6) | (preem << 4) |
			(preem << 2) | (preem << 0) | (preem << 30);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, data32);
	} else {
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, 0);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, 0);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, 0);
	}
}

static void lcd_lvds_phy_set_tl1(struct lcd_config_s *pconf, int status)
{
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, 0xff2027e0 | phy->vswing);
		lcd_phy_cntl_set_tl1(phy, status, 0, 1, 0);
	} else {
		lcd_phy_cntl_set_tl1(phy, status, 0, 1, 0);
	}
}

static void lcd_lvds_phy_set_t5(struct lcd_config_s *pconf, int status)
{
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;
	unsigned int cntl14 = 0;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		if (lcd_debug_print_flag)
			LCDPR("vswing_level=0x%x\n", phy->vswing_level);

		cntl14 = 0xff2027e0 | phy->vswing;
		/* vcm */
		if ((phy->flag & (1 << 1))) {
			cntl14 &= ~(0x7ff << 4);
			cntl14 |= (phy->vcm & 0x7ff) << 4;
		}
		/* ref bias switch */
		if ((phy->flag & (1 << 2))) {
			cntl14 &= ~(1 << 15);
			cntl14 |= (phy->ref_bias & 0x1) << 15;
		}
		/* odt */
		if ((phy->flag & (1 << 3))) {
			cntl14 &= ~(0xff << 24);
			cntl14 |= (phy->odt & 0xff) << 24;
		}
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, cntl14);
		lcd_phy_cntl_set_t5(phy, status, 0, 1, 0);
	} else {
		lcd_phy_cntl_set_t5(phy, status, 0, 0, 0);
	}
}

static void lcd_lvds_phy_set_t5w(struct lcd_config_s *pconf, int status)
{
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;
	unsigned int cntl14 = 0;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		if (lcd_debug_print_flag)
			LCDPR("vswing_level=0x%x\n", phy->vswing_level);

		cntl14 = 0xff2027e0 | phy->vswing;
		/* vcm */
		if ((phy->flag & (1 << 1))) {
			cntl14 &= ~(0x7ff << 4);
			cntl14 |= (phy->vcm & 0x7ff) << 4;
		}
		/* ref bias switch */
		if ((phy->flag & (1 << 2))) {
			cntl14 &= ~(1 << 15);
			cntl14 |= (phy->ref_bias & 0x1) << 15;
		}
		/* odt */
		if ((phy->flag & (1 << 3))) {
			cntl14 &= ~(0xff << 24);
			cntl14 |= (phy->odt & 0xff) << 24;
		}

		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, cntl14);
		lcd_phy_cntl_set_t5w(phy, status, 0, 1, 0);
	} else {
		lcd_phy_cntl_set_t5w(phy, status, 0, 0, 0);
	}
}

static void lcd_vbyone_phy_set_txl(struct lcd_config_s *pconf, int status)
{
	unsigned int vswing, preem, ext_pullup;
	unsigned int data32 = 0;
	unsigned int rinner_table[] = {0xa, 0xa, 0x6, 0x4};
	struct vbyone_config_s *vbyone_conf;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	vbyone_conf = pconf->lcd_control.vbyone_config;
	if (status) {
		ext_pullup = (vbyone_conf->phy_vswing >> 4) & 0x3;
		vswing = vbyone_conf->phy_vswing & 0xf;
		preem = vbyone_conf->phy_preem & 0xf;
		if (lcd_debug_print_flag) {
			LCDPR("vswing=0x%x, preem=0x%x\n",
				vbyone_conf->phy_vswing, preem);
		}

		if (vswing > 7) {
			LCDERR("%s: invalid vswing=0x%x, use default\n",
				__func__, vswing);
			vswing = VX1_PHY_VSWING_DFT;
		}
		if (preem > 7) {
			LCDERR("%s: invalid preem=0x%x, use default\n",
				__func__, preem);
			preem = VX1_PHY_PREEM_DFT;
		}
		if (ext_pullup) {
			data32 = VX1_PHY_CNTL1_G9TV_PULLUP |
				(vswing << 3);
		} else {
			data32 = VX1_PHY_CNTL1_G9TV | (vswing << 3);
		}
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, data32);
		data32 = VX1_PHY_CNTL2_G9TV | (preem << 20) |
			(rinner_table[ext_pullup] << 8);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, data32);
		data32 = VX1_PHY_CNTL3_G9TV;
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, data32);
	} else {
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, 0);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, 0);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, 0);
	}
}

static void lcd_vbyone_phy_set_tl1(struct lcd_config_s *pconf, int status)
{
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		if (phy->ext_pullup) {
			lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14,
				0xff2027e0 | phy->vswing);
		} else {
			lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14,
				0xf02027a0 | phy->vswing);
		}
		lcd_phy_cntl_set_tl1(phy, status, 1, 1, 0);
	} else {
		lcd_phy_cntl_set_tl1(phy, status, 1, 1, 0);
	}
}

static void lcd_vbyone_phy_set_t5(struct lcd_config_s *pconf, int status)
{
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;
	unsigned int cntl14 = 0;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		if (lcd_debug_print_flag) {
			LCDPR("vswing_level=0x%x, ext_pullup=%d\n",
			      phy->vswing_level, phy->ext_pullup);
		}

		if (phy->ext_pullup)
			cntl14 = 0xff2027e0 | phy->vswing;
		else
			cntl14 = 0xf02027a0 | phy->vswing;
		/* vcm */
		if ((phy->flag & (1 << 1))) {
			cntl14 &= ~(0x7ff << 4);
			cntl14 |= (phy->vcm & 0x7ff) << 4;
		}
		/* ref bias switch */
		if ((phy->flag & (1 << 2))) {
			cntl14 &= ~(1 << 15);
			cntl14 |= (phy->ref_bias & 0x1) << 15;
		}
		/* odt */
		if ((phy->flag & (1 << 3))) {
			cntl14 &= ~(0xff << 24);
			cntl14 |= (phy->odt & 0xff) << 24;
		}

		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, cntl14);
		lcd_phy_cntl_set_t5(phy, status, 1, 1, 0);
	} else {
		lcd_phy_cntl_set_t5(phy, status, 1, 0, 0);
	}
}

static void lcd_vbyone_phy_set_t5w(struct lcd_config_s *pconf, int status)
{
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;
	unsigned int cntl14 = 0;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		if (lcd_debug_print_flag) {
			LCDPR("vswing_level=0x%x, ext_pullup=%d\n",
			      phy->vswing_level, phy->ext_pullup);
		}

		if (phy->ext_pullup)
			cntl14 = 0xff2027e0 | phy->vswing;
		else
			cntl14 = 0xf02027a0 | phy->vswing;
		/* vcm */
		if ((phy->flag & (1 << 1))) {
			cntl14 &= ~(0x7ff << 4);
			cntl14 |= (phy->vcm & 0x7ff) << 4;
		}
		/* ref bias switch */
		if ((phy->flag & (1 << 2))) {
			cntl14 &= ~(1 << 15);
			cntl14 |= (phy->ref_bias & 0x1) << 15;
		}
		/* odt */
		if ((phy->flag & (1 << 3))) {
			cntl14 &= ~(0xff << 24);
			cntl14 |= (phy->odt & 0xff) << 24;
		}

		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, cntl14);
		lcd_phy_cntl_set_t5w(phy, status, 1, 1, 0);
	} else {
		lcd_phy_cntl_set_t5w(phy, status, 1, 0, 0);
	}
}

static void lcd_mlvds_phy_set_txhd(struct lcd_config_s *pconf, int status)
{
	unsigned int vswing, preem;
	unsigned int data32 = 0;
	struct mlvds_config_s *mlvds_conf;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	mlvds_conf = pconf->lcd_control.mlvds_config;
	if (status) {
		vswing = mlvds_conf->phy_vswing & 0xf;
		preem = mlvds_conf->phy_preem & 0xf;
		if (lcd_debug_print_flag)
			LCDPR("vswing=0x%x, preem=0x%x\n", vswing, preem);

		if (vswing > 7) {
			LCDERR("%s: invalid vswing=0x%x, use default\n",
				__func__, vswing);
			vswing = LVDS_PHY_VSWING_DFT;
		}
		if (preem > 3) {
			LCDERR("%s: invalid preem=0x%x, use default\n",
				__func__, preem);
			preem = LVDS_PHY_PREEM_DFT;
		}

		data32 = MLVDS_PHY_CNTL1_TXHD |
			(vswing << 3) | (vswing << 0) | (preem << 23);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, data32);
		data32 = MLVDS_PHY_CNTL2_TXHD |
			(preem << 14) | (preem << 12) |
			(preem << 26) | (preem << 24);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, data32);
		data32 = MLVDS_PHY_CNTL3_TXHD |
			(preem << 6) | (preem << 4) |
			(preem << 2) | (preem << 0) | (preem << 30);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, data32);
	} else {
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL1, 0);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL2, 0);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL3, 0);
	}
}

static void lcd_mlvds_phy_set_tl1(struct lcd_config_s *pconf, int status)
{
	unsigned int ckdi;
	struct mlvds_config_s *mlvds_conf;
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	mlvds_conf = pconf->lcd_control.mlvds_config;
	if (status) {
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, 0xff2027e0 | phy->vswing);
		ckdi = (mlvds_conf->pi_clk_sel << 12);
		lcd_phy_cntl_set_tl1(phy, status, 0, 1, ckdi);
	} else {
		lcd_phy_cntl_set_tl1(phy, status, 0, 1, 0);
	}
}

static void lcd_mlvds_phy_set_t5(struct lcd_config_s *pconf, int status)
{
	struct mlvds_config_s *mlvds_conf;
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;
	unsigned int ckdi, cntl14 = 0;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	mlvds_conf = pconf->lcd_control.mlvds_config;
	if (status) {
		if (lcd_debug_print_flag)
			LCDPR("vswing_level=0x%x\n", phy->vswing_level);

		cntl14 = 0xff2027e0 | phy->vswing;
		/* vcm */
		if ((phy->flag & (1 << 1))) {
			cntl14 &= ~(0x7ff << 4);
			cntl14 |= (phy->vcm & 0x7ff) << 4;
		}
		/* ref bias switch */
		if ((phy->flag & (1 << 2))) {
			cntl14 &= ~(1 << 15);
			cntl14 |= (phy->ref_bias & 0x1) << 15;
		}
		/* odt */
		if ((phy->flag & (1 << 3))) {
			cntl14 &= ~(0xff << 24);
			cntl14 |= (phy->odt & 0xff) << 24;
		}

		ckdi = (mlvds_conf->pi_clk_sel << 12);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, cntl14);
		lcd_phy_cntl_set_t5(phy, status, 0, 1, ckdi);
	} else {
		lcd_phy_cntl_set_t5(phy, status, 0, 0, 0);
	}
}

static void lcd_mlvds_phy_set_t5w(struct lcd_config_s *pconf, int status)
{
	struct mlvds_config_s *mlvds_conf;
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;
	unsigned int ckdi, cntl14 = 0;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	mlvds_conf = pconf->lcd_control.mlvds_config;
	if (status) {
		if (lcd_debug_print_flag)
			LCDPR("vswing_level=0x%x\n", phy->vswing_level);

		cntl14 = 0xff2027e0 | phy->vswing;
		/* vcm */
		if ((phy->flag & (1 << 1))) {
			cntl14 &= ~(0x7ff << 4);
			cntl14 |= (phy->vcm & 0x7ff) << 4;
		}
		/* ref bias switch */
		if ((phy->flag & (1 << 2))) {
			cntl14 &= ~(1 << 15);
			cntl14 |= (phy->ref_bias & 0x1) << 15;
		}
		/* odt */
		if ((phy->flag & (1 << 3))) {
			cntl14 &= ~(0xff << 24);
			cntl14 |= (phy->odt & 0xff) << 24;
		}

		ckdi = (mlvds_conf->pi_clk_sel << 12);
		lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, cntl14);
		lcd_phy_cntl_set_t5w(phy, status, 0, 1, ckdi);
	} else {
		lcd_phy_cntl_set_t5w(phy, status, 0, 0, 0);
	}
}

static void lcd_p2p_phy_set_tl1(struct lcd_config_s *pconf, int status)
{
	unsigned int p2p_type;
	struct p2p_config_s *p2p_conf;
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	p2p_conf = pconf->lcd_control.p2p_config;
	if (status) {
		p2p_type = p2p_conf->p2p_type & 0x1f;
		switch (p2p_type) {
		case P2P_CEDS:
		case P2P_CMPI:
		case P2P_ISP:
		case P2P_EPI:
			lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, 0xff2027a0 |
				      phy->vswing);
			lcd_phy_cntl_set_tl1(phy, status, 1, 1, 0);
			break;
		case P2P_CHPI: /* low common mode */
		case P2P_CSPI:
		case P2P_USIT:
			if (p2p_type == P2P_CHPI)
				phy->weakly_pull_down = 1;
			lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, 0xfe60027f);
			lcd_phy_cntl_set_tl1(phy, status, 1, 0, 0);
			break;
		default:
			LCDERR("%s: invalid p2p_type %d\n", __func__, p2p_type);
			break;
		}
	} else {
		lcd_phy_cntl_set_tl1(phy, status, 1, 1, 0);
	}
}

static void lcd_p2p_phy_set_t5(struct lcd_config_s *pconf, int status)
{
	struct p2p_config_s *p2p_conf = pconf->lcd_control.p2p_config;
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;
	unsigned int p2p_type, vcm_flag;
	unsigned int cntl14 = 0;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		if (lcd_debug_print_flag)
			LCDPR("vswing_level=0x%x\n", phy->vswing_level);
		p2p_type = p2p_conf->p2p_type & 0x1f;
		vcm_flag = (p2p_conf->p2p_type >> 5) & 0x1;
		switch (p2p_type) {
		case P2P_CEDS:
		case P2P_CMPI:
		case P2P_ISP:
		case P2P_EPI:
			cntl14 = 0xff2027a0 | phy->vswing;
			/* vcm */
			if ((phy->flag & (1 << 1))) {
				cntl14 &= ~(0x7ff << 4);
				cntl14 |= (phy->vcm & 0x7ff) << 4;
			}
			/* ref bias switch */
			if ((phy->flag & (1 << 2))) {
				cntl14 &= ~(1 << 15);
				cntl14 |= (phy->ref_bias & 0x1) << 15;
			}
			/* odt */
			if ((phy->flag & (1 << 3))) {
				cntl14 &= ~(0xff << 24);
				cntl14 |= (phy->odt & 0xff) << 24;
			}
			lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, cntl14);
			lcd_phy_cntl_set_t5(phy, status, 1, 1, 0);
			break;
		case P2P_CHPI: /* low common mode */
		case P2P_CSPI:
		case P2P_USIT:
			if (p2p_type == P2P_CHPI)
				phy->weakly_pull_down = 1;
			if (vcm_flag) /* 580mV */
				cntl14 = 0xe0600272;
			else /* default 385mV */
				cntl14 = 0xfe60027f;

			/* vswing */
			cntl14 &= ~(0xf);
			cntl14 |= phy->vswing;

			/* vcm */
			if ((phy->flag & (1 << 1))) {
				cntl14 &= ~(0x7ff << 4);
				cntl14 |= (phy->vcm & 0x7ff) << 4;
			}
			/* ref bias switch */
			if ((phy->flag & (1 << 2))) {
				cntl14 &= ~(1 << 15);
				cntl14 |= (phy->ref_bias & 0x1) << 15;
			}
			/* odt */
			if ((phy->flag & (1 << 3))) {
				cntl14 &= ~(0xff << 24);
				cntl14 |= (phy->odt & 0xff) << 24;
			}

			lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, cntl14);
			lcd_phy_cntl_set_t5(phy, status, 1, 0, 0);
			break;
		default:
			LCDERR("%s: invalid p2p_type %d\n", __func__, p2p_type);
			break;
		}
	} else {
		lcd_phy_cntl_set_t5(phy, status, 1, 0, 0);
	}
}

static void lcd_p2p_phy_set_t5w(struct lcd_config_s *pconf, int status)
{
	struct p2p_config_s *p2p_conf = pconf->lcd_control.p2p_config;
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;
	unsigned int p2p_type, vcm_flag;
	unsigned int cntl14 = 0;

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		if (lcd_debug_print_flag)
			LCDPR("vswing_level=0x%x\n", phy->vswing_level);

		p2p_type = p2p_conf->p2p_type & 0x1f;
		vcm_flag = (p2p_conf->p2p_type >> 5) & 0x1;
		switch (p2p_type) {
		case P2P_CEDS:
		case P2P_CMPI:
		case P2P_ISP:
		case P2P_EPI:
			cntl14 = 0xff2027a0 | phy->vswing;
			/* vcm */
			if ((phy->flag & (1 << 1))) {
				cntl14 &= ~(0x7ff << 4);
				cntl14 |= (phy->vcm & 0x7ff) << 4;
			}
			/* ref bias switch */
			if ((phy->flag & (1 << 2))) {
				cntl14 &= ~(1 << 15);
				cntl14 |= (phy->ref_bias & 0x1) << 15;
			}
			/* odt */
			if ((phy->flag & (1 << 3))) {
				cntl14 &= ~(0xff << 24);
				cntl14 |= (phy->odt & 0xff) << 24;
			}
			lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, cntl14);
			lcd_phy_cntl_set_t5w(phy, status, 1, 1, 0);
			break;
		case P2P_CHPI: /* low common mode */
		case P2P_CSPI:
		case P2P_USIT:
			if (p2p_type == P2P_CHPI)
				phy->weakly_pull_down = 1;
			if (vcm_flag) /* 580mV */
				cntl14 = 0xe0600272;
			else /* default 385mV */
				cntl14 = 0xfe60027f;

			/* vswing */
			cntl14 &= ~(0xf);
			cntl14 |= phy->vswing;

			/* vcm */
			if ((phy->flag & (1 << 1))) {
				cntl14 &= ~(0x7ff << 4);
				cntl14 |= (phy->vcm & 0x7ff) << 4;
			}
			/* ref bias switch */
			if ((phy->flag & (1 << 2))) {
				cntl14 &= ~(1 << 15);
				cntl14 |= (phy->ref_bias & 0x1) << 15;
			}
			/* odt */
			if ((phy->flag & (1 << 3))) {
				cntl14 &= ~(0xff << 24);
				cntl14 |= (phy->odt & 0xff) << 24;
			}

			lcd_hiu_write(HHI_DIF_CSI_PHY_CNTL14, cntl14);
			lcd_phy_cntl_set_t5w(phy, status, 1, 0, 0);
			break;
		default:
			LCDERR("%s: invalid p2p_type %d\n", __func__, p2p_type);
			break;
		}
	} else {
		lcd_phy_cntl_set_t5w(phy, status, 1, 0, 0);
	}
}

static void lcd_mipi_phy_set_axg(struct lcd_config_s *pconf, int status)
{
	unsigned int phy_reg, phy_bit, phy_width;
	unsigned int lane_cnt;

	if (status) {
		/* HHI_MIPI_CNTL0 */
		/* DIF_REF_CTL1:31-16bit, DIF_REF_CTL0:15-0bit */
		lcd_hiu_setb(HHI_MIPI_CNTL0, 0x1b8, 16, 10);
		lcd_hiu_setb(HHI_MIPI_CNTL0, 1, 26, 1); /* bandgap */
		lcd_hiu_setb(HHI_MIPI_CNTL0, 1, 29, 1); /* current */
		lcd_hiu_setb(HHI_MIPI_CNTL0, 1, 31, 1);
		lcd_hiu_setb(HHI_MIPI_CNTL0, 0x8, 0, 16);

		/* HHI_MIPI_CNTL1 */
		/* DIF_REF_CTL2:15-0bit */
		lcd_hiu_write(HHI_MIPI_CNTL1, (0x001e << 0));

		/* HHI_MIPI_CNTL2 */
		/* DIF_TX_CTL1:31-16bit, DIF_TX_CTL0:15-0bit */
		lcd_hiu_write(HHI_MIPI_CNTL2, (0x26e0 << 16) | (0x459 << 0));

		phy_reg = HHI_MIPI_CNTL2;
		phy_bit = MIPI_PHY_LANE_BIT;
		phy_width = MIPI_PHY_LANE_WIDTH;
		switch (pconf->lcd_control.mipi_config->lane_num) {
		case 1:
			lane_cnt = DSI_LANE_COUNT_1;
			break;
		case 2:
			lane_cnt = DSI_LANE_COUNT_2;
			break;
		case 3:
			lane_cnt = DSI_LANE_COUNT_3;
			break;
		case 4:
			lane_cnt = DSI_LANE_COUNT_4;
			break;
		default:
			lane_cnt = 0;
			break;
		}
		lcd_hiu_setb(phy_reg, lane_cnt, phy_bit, phy_width);
	} else {
		lcd_hiu_setb(HHI_MIPI_CNTL0, 0, 16, 10);
		lcd_hiu_setb(HHI_MIPI_CNTL0, 0, 31, 1);
		lcd_hiu_setb(HHI_MIPI_CNTL0, 0, 0, 16);
		lcd_hiu_write(HHI_MIPI_CNTL1, 0x6);
		lcd_hiu_write(HHI_MIPI_CNTL2, 0x00200000);
	}
}

static void lcd_mipi_phy_set_g12a(struct lcd_config_s *pconf, int status)
{
	unsigned int phy_reg, phy_bit, phy_width;
	unsigned int lane_cnt;

	if (status) {
		/* HHI_MIPI_CNTL0 */
		/* DIF_REF_CTL1:31-16bit, DIF_REF_CTL0:15-0bit */
		lcd_hiu_write(HHI_MIPI_CNTL0,
			(0xa487 << 16) | (0x8 << 0));

		/* HHI_MIPI_CNTL1 */
		/* DIF_REF_CTL2:15-0bit; bandgap bit16 */
		lcd_hiu_write(HHI_MIPI_CNTL1,
			(0x1 << 16) | (0x002e << 0));

		/* HHI_MIPI_CNTL2 */
		/* DIF_TX_CTL1:31-16bit, DIF_TX_CTL0:15-0bit */
		lcd_hiu_write(HHI_MIPI_CNTL2,
			(0x2680 << 16) | (0x45a << 0));

		phy_reg = HHI_MIPI_CNTL2;
		phy_bit = MIPI_PHY_LANE_BIT;
		phy_width = MIPI_PHY_LANE_WIDTH;
		switch (pconf->lcd_control.mipi_config->lane_num) {
		case 1:
			lane_cnt = DSI_LANE_COUNT_1_G12A;
			break;
		case 2:
			lane_cnt = DSI_LANE_COUNT_2_G12A;
			break;
		case 3:
			lane_cnt = DSI_LANE_COUNT_3_G12A;
			break;
		case 4:
			lane_cnt = DSI_LANE_COUNT_4_G12A;
			break;
		default:
			lane_cnt = 0;
			break;
		}
		lcd_hiu_setb(phy_reg, lane_cnt, phy_bit, phy_width);
	} else {
		lcd_hiu_write(HHI_MIPI_CNTL0, 0);
		lcd_hiu_write(HHI_MIPI_CNTL1, 0);
		lcd_hiu_write(HHI_MIPI_CNTL2, 0);
	}
}

void lcd_lvds_phy_set(struct lcd_config_s *pconf, int status)
{
	if (!lcd_phy_ctrl)
		return;
	if (!lcd_phy_ctrl->phy_set_lvds)
		return;

	lcd_phy_ctrl->phy_set_lvds(pconf, status);
}

void lcd_vbyone_phy_set(struct lcd_config_s *pconf, int status)
{
	if (!lcd_phy_ctrl)
		return;
	if (!lcd_phy_ctrl->phy_set_vx1)
		return;

	lcd_phy_ctrl->phy_set_vx1(pconf, status);
}

void lcd_mlvds_phy_set(struct lcd_config_s *pconf, int status)
{
	if (!lcd_phy_ctrl)
		return;
	if (!lcd_phy_ctrl->phy_set_mlvds)
		return;

	lcd_phy_ctrl->phy_set_mlvds(pconf, status);
}

void lcd_p2p_phy_set(struct lcd_config_s *pconf, int status)
{
	if (!lcd_phy_ctrl)
		return;
	if (!lcd_phy_ctrl->phy_set_p2p)
		return;

	lcd_phy_ctrl->phy_set_p2p(pconf, status);
}

void lcd_mipi_phy_set(struct lcd_config_s *pconf, int status)
{
	if (!lcd_phy_ctrl)
		return;
	if (!lcd_phy_ctrl->phy_set_mipi)
		return;

	lcd_phy_ctrl->phy_set_mipi(pconf, status);
}

unsigned int lcd_phy_vswing_level_to_value(struct aml_lcd_drv_s *pdrv, unsigned int level)
{
	unsigned int vswing_value = 0;

	vswing_value = level;

	return vswing_value;
}

unsigned int lcd_phy_preem_level_to_value(struct aml_lcd_drv_s *pdrv, unsigned int level)
{
	unsigned int p2p_type, size, preem_value = 0;

	switch (pdrv->lcd_config->lcd_basic.lcd_type) {
	case LCD_LVDS:
	case LCD_VBYONE:
	case LCD_MLVDS:
		size = sizeof(lvds_vx1_p2p_phy_preem_tl1) / sizeof(unsigned int);
		if (level >= size) {
			LCDERR(" %s: level %d invalid\n",
			       __func__, level);
			level = 0;
		}
		preem_value = lvds_vx1_p2p_phy_preem_tl1[level];
		break;
	case LCD_P2P:
		p2p_type = pdrv->lcd_config->lcd_control.p2p_config->p2p_type & 0x1f;
		switch (p2p_type) {
		case P2P_CEDS:
		case P2P_CMPI:
		case P2P_ISP:
		case P2P_EPI:
			size = sizeof(lvds_vx1_p2p_phy_preem_tl1) / sizeof(unsigned int);
			if (level >= size) {
				LCDERR("%s: level %d invalid\n",
				       __func__, level);
				level = 0;
			}
			preem_value = lvds_vx1_p2p_phy_preem_tl1[level];
			break;
		case P2P_CHPI: /* low common mode */
		case P2P_CSPI:
		case P2P_USIT:
			size = sizeof(p2p_low_common_phy_preem_tl1) / sizeof(unsigned int);
			if (level >= size) {
				LCDERR("%s: level %d invalid\n",
				       __func__, level);
				level = 0;
			}
			preem_value = p2p_low_common_phy_preem_tl1[level];
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return preem_value;
}

struct lcd_phy_ctrl_s lcd_phy_ctrl_txl = {
	.phy_set_lvds = lcd_lvds_phy_set_txl,
	.phy_set_vx1 = lcd_vbyone_phy_set_txl,
	.phy_set_mlvds = NULL,
	.phy_set_p2p = NULL,
	.phy_set_mipi = NULL,
};

struct lcd_phy_ctrl_s lcd_phy_ctrl_axg = {
	.phy_set_lvds = NULL,
	.phy_set_vx1 = NULL,
	.phy_set_mlvds = NULL,
	.phy_set_p2p = NULL,
	.phy_set_mipi = lcd_mipi_phy_set_axg,
};

struct lcd_phy_ctrl_s lcd_phy_ctrl_txhd = {
	.phy_set_lvds = lcd_lvds_phy_set_txhd,
	.phy_set_vx1 = NULL,
	.phy_set_mlvds = lcd_mlvds_phy_set_txhd,
	.phy_set_p2p = NULL,
	.phy_set_mipi = NULL,
};

struct lcd_phy_ctrl_s lcd_phy_ctrl_g12a = {
	.phy_set_lvds = NULL,
	.phy_set_vx1 = NULL,
	.phy_set_mlvds = NULL,
	.phy_set_p2p = NULL,
	.phy_set_mipi = lcd_mipi_phy_set_g12a,
};

struct lcd_phy_ctrl_s lcd_phy_ctrl_tl1 = {
	.phy_set_lvds = lcd_lvds_phy_set_tl1,
	.phy_set_vx1 = lcd_vbyone_phy_set_tl1,
	.phy_set_mlvds = lcd_mlvds_phy_set_tl1,
	.phy_set_p2p = lcd_p2p_phy_set_tl1,
	.phy_set_mipi = NULL,
};

struct lcd_phy_ctrl_s lcd_phy_ctrl_t5 = {
	.phy_set_lvds = lcd_lvds_phy_set_t5,
	.phy_set_vx1 = lcd_vbyone_phy_set_t5,
	.phy_set_mlvds = lcd_mlvds_phy_set_t5,
	.phy_set_p2p = lcd_p2p_phy_set_t5,
	.phy_set_mipi = NULL,
};

struct lcd_phy_ctrl_s lcd_phy_ctrl_t5w = {
	.phy_set_lvds = lcd_lvds_phy_set_t5w,
	.phy_set_vx1 = lcd_vbyone_phy_set_t5w,
	.phy_set_mlvds = lcd_mlvds_phy_set_t5w,
	.phy_set_p2p = lcd_p2p_phy_set_t5w,
	.phy_set_mipi = NULL,
};

int lcd_phy_probe(void)
{
	struct aml_lcd_drv_s *lcd_drv = aml_lcd_get_driver();

	lcd_phy_ctrl = NULL;
#ifdef CONFIG_AML_LCD_PXP
	return 0;
#endif

	switch (lcd_drv->chip_type) {
	case LCD_CHIP_TXL:
	case LCD_CHIP_TXLX:
		lcd_phy_ctrl = &lcd_phy_ctrl_txl;
		break;
	case LCD_CHIP_AXG:
		lcd_phy_ctrl = &lcd_phy_ctrl_axg;
		break;
	case LCD_CHIP_TXHD:
		lcd_phy_ctrl = &lcd_phy_ctrl_txhd;
		break;
	case LCD_CHIP_G12A:
	case LCD_CHIP_G12B:
	case LCD_CHIP_SM1:
		lcd_phy_ctrl = &lcd_phy_ctrl_g12a;
		break;
	case LCD_CHIP_TL1:
		lcd_phy_ctrl = &lcd_phy_ctrl_tl1;
		if (lcd_drv->rev_type == 0xA || lcd_drv->rev_type == 0xB)
			phy_ctrl_bit_on = 0;
		else
			phy_ctrl_bit_on = 1;
		break;
	case LCD_CHIP_TM2:
		lcd_phy_ctrl = &lcd_phy_ctrl_tl1;
		if (lcd_drv->rev_type == 0xA)
			phy_ctrl_bit_on = 0;
		else
			phy_ctrl_bit_on = 1;
		break;
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
		lcd_phy_ctrl = &lcd_phy_ctrl_t5;
		phy_ctrl_bit_on = 1;
		break;
	case LCD_CHIP_T5W:
		lcd_phy_ctrl = &lcd_phy_ctrl_t5w;
		phy_ctrl_bit_on = 1;
		break;
	default:
		break;
	}

	printf("%s\n", __func__);

	return 0;
}
