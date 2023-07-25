// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "../lcd_reg.h"
#include "lcd_phy_config.h"
#include "../lcd_common.h"

static struct lcd_phy_ctrl_s *phy_ctrl_p;

static unsigned int p2p_low_common_phy_ch_tl1 = 0x000b000b;
static unsigned int lvds_vx1_p2p_phy_ch_tl1 = 0x00020002;

/*
 *    chreg: channel ctrl
 *    bypass: 1=bypass
 *    mode: 1=normal mode, 0=low common mode
 *    ckdi: clk phase for minilvds
 */
static void lcd_phy_cntl_set(struct aml_lcd_drv_s *pdrv, struct phy_config_s *phy, int status,
				int bypass, unsigned int mode, unsigned int ckdi)
{
	unsigned int cntl_vinlp_pi = 0, cntl_ckdi = 0;
	unsigned int data = 0, chreg = 0, chctl = 0;
	unsigned int amp = 0;
	int i = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		chreg |= ((phy_ctrl_p->ctrl_bit_on << 16) |
			  (phy_ctrl_p->ctrl_bit_on << 0));
		if ((phy->flag & (1 << 13)) == 0) {
			for (i = 0; i < 16; i++)
				phy->lane[i].amp = 0x7;
		}

		if (bypass)
			chctl |= ((1 << 18) | (1 << 2));

		if (mode) {
			chreg |= lvds_vx1_p2p_phy_ch_tl1;
			cntl_vinlp_pi = 0x00070000;
		} else {
			chreg |= p2p_low_common_phy_ch_tl1;
			if (phy->weakly_pull_down)
				chreg &= ~((1 << 19) | (1 << 3));
			cntl_vinlp_pi = 0x000e0000;
		}
		cntl_ckdi = ckdi | 0x80000000;
	} else {
		if (phy_ctrl_p->ctrl_bit_on)
			data = 0;
		else
			data = 1;
		chreg |= ((data << 16) | (data << 0));
		cntl_vinlp_pi = 0;
		cntl_ckdi = 0;
		lcd_ana_write(ANACTRL_DIF_PHY_CNTL18, 0);
	}

	lcd_ana_write(ANACTRL_DIF_PHY_CNTL19, cntl_vinlp_pi);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL20, cntl_ckdi);

	data = ((phy->lane[0].preem & 0xff) << 8 | (phy->lane[1].preem & 0xff) << 24);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL1, chreg | data);
	amp = ((phy->lane[0].amp & 0x7) << 3 | (phy->lane[1].amp & 0x7) << 19);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL10, chctl | amp);

	data = ((phy->lane[2].preem & 0xff) << 8 | (phy->lane[3].preem & 0xff) << 24);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL2, chreg | data);
	amp = ((phy->lane[2].amp & 0x7) << 3 | (phy->lane[3].amp & 0x7) << 19);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL11, chctl | amp);

	data = ((phy->lane[4].preem & 0xff) << 8 | (phy->lane[5].preem & 0xff) << 24);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL3, chreg | data);
	amp = ((phy->lane[4].amp & 0x7) << 3 | (phy->lane[5].amp & 0x7) << 19);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL12, chctl | amp);

	data = ((phy->lane[6].preem & 0xff) << 8 | (phy->lane[7].preem & 0xff) << 24);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL4, chreg | data);
	amp = ((phy->lane[6].amp & 0x7) << 3 | (phy->lane[7].amp & 0x7) << 19);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL13, chctl | amp);

	data = ((phy->lane[8].preem & 0xff) << 8 | (phy->lane[9].preem & 0xff) << 24);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL6, chreg | data);
	amp = ((phy->lane[8].amp & 0x7) << 3 | (phy->lane[9].amp & 0x7) << 19);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL14, chctl | amp);

	data = ((phy->lane[10].preem & 0xff) << 8 | (phy->lane[11].preem & 0xff) << 24);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL7, chreg | data);
	amp = ((phy->lane[10].amp & 0x7) << 3 | (phy->lane[11].amp & 0x7) << 19);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL15, chctl | amp);

	data = ((phy->lane[12].preem & 0xff) << 8 | (phy->lane[13].preem & 0xff) << 24);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL8, chreg | data);
	amp = ((phy->lane[12].amp & 0x7) << 3 | (phy->lane[13].amp & 0x7) << 19);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL16, chctl | amp);

	data = ((phy->lane[14].preem & 0xff) << 8 | (phy->lane[15].preem & 0xff) << 24);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL9, chreg | data);
	amp = ((phy->lane[14].amp & 0x7) << 3 | (phy->lane[15].amp & 0x7) << 19);
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL17, chctl | amp);
}

static void lcd_phy_common_update(struct phy_config_s *phy, unsigned int com_data)
{
	if (!phy)
		return;
	/* vcm */
	if ((phy->flag & (1 << 1))) {
		com_data &= ~(0x7ff << 4);
		com_data |= (phy->vcm & 0x7ff) << 4;
	}
	/* ref bias switch */
	if ((phy->flag & (1 << 2))) {
		com_data &= ~(1 << 15);
		com_data |= (phy->ref_bias & 0x1) << 15;
	}
	/* odt */
	if ((phy->flag & (1 << 3))) {
		com_data &= ~(0xff << 24);
		com_data |= (phy->odt & 0xff) << 24;
	}
	lcd_ana_write(ANACTRL_DIF_PHY_CNTL18, com_data);
}

static void lcd_lvds_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	struct phy_config_s *phy = &pdrv->config.phy_cfg;
	unsigned int com_data = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
			LCDPR("vswing_level=0x%x\n", phy->vswing_level);

		com_data = 0xff2027e0 | phy->vswing;
		lcd_phy_common_update(phy, com_data);
		lcd_phy_cntl_set(pdrv, phy, status, 1, 1, 0);
	} else {
		lcd_phy_cntl_set(pdrv, phy, status, 1, 0, 0);
	}
}

static void lcd_vbyone_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	struct phy_config_s *phy = &pdrv->config.phy_cfg;
	unsigned int com_data = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			LCDPR("vswing_level=0x%x, ext_pullup=%d\n",
			      phy->vswing_level, phy->ext_pullup);
		}

		if (phy->ext_pullup)
			com_data = 0xff2027e0 | phy->vswing;
		else
			com_data = 0xf02027a0 | phy->vswing;
		lcd_phy_common_update(phy, com_data);
		lcd_phy_cntl_set(pdrv, phy, status, 1, 1, 0);
	} else {
		lcd_phy_cntl_set(pdrv, phy, status, 1, 0, 0);
	}
}

static void lcd_p2p_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	unsigned int p2p_type, vcm_flag;
	struct p2p_config_s *p2p_conf;
	struct phy_config_s *phy = &pdrv->config.phy_cfg;
	unsigned int com_data = 0;
	unsigned int mode = 1;  //1-normal mode, 0-low common mode

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV)
		LCDPR("%s: %d\n", __func__, status);

	p2p_conf = &pdrv->config.control.p2p_cfg;

	if (status) {
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			LCDPR("vswing_level=0x%x, ext_pullup=%d\n",
			      phy->vswing_level, phy->ext_pullup);
		}
		p2p_type = p2p_conf->p2p_type & 0x1f;
		vcm_flag = (p2p_conf->p2p_type >> 5) & 0x1;

		switch (p2p_type) {
		case P2P_CEDS:
		case P2P_CMPI:
		case P2P_ISP:
		case P2P_EPI:
			com_data = 0xff2027a0 | phy->vswing;
			mode = 1;
			break;
		case P2P_CHPI: /* low common mode */
		case P2P_CSPI:
		case P2P_USIT:
			if (p2p_type == P2P_CHPI)
				phy->weakly_pull_down = 1;

			if (vcm_flag) /* 580mV */
				com_data = 0xe0600272;
			else /* default 385mV */
				com_data = 0xfe60027f;

			/* vswing */
			com_data &= ~(0xf);
			com_data |= phy->vswing;
			mode = 0;
			break;
		default:
			LCDERR("%s: invalid p2p_type %d\n", __func__, p2p_type);
			return;
		}

		lcd_phy_common_update(phy, com_data);
		lcd_phy_cntl_set(pdrv, phy, status, 1, mode, 0);
	} else {
		lcd_phy_cntl_set(pdrv, phy, status, 1, 0, 0);
	}
}

static struct lcd_phy_ctrl_s lcd_phy_ctrl_t3x = {
	.lane_lock = 0,
	.ctrl_bit_on = 1,
	.phy_vswing_level_to_val = lcd_phy_vswing_level_to_value_dft,
	.phy_preem_level_to_val = lcd_phy_preem_level_to_value_dft,
	.phy_set_lvds = lcd_lvds_phy_set,
	.phy_set_vx1 = lcd_vbyone_phy_set,
	.phy_set_mlvds = NULL,
	.phy_set_p2p = lcd_p2p_phy_set,
	.phy_set_mipi = NULL,
	.phy_set_edp = NULL,
};

struct lcd_phy_ctrl_s *lcd_phy_config_init_t3x(struct aml_lcd_data_s *pdata)
{
	phy_ctrl_p = &lcd_phy_ctrl_t3x;
	return phy_ctrl_p;
}
