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

/*
 *    chreg: channel ctrl
 *    bypass: 1=bypass
 *    mode: 1=normal mode, 0=low common mode
 *    ckdi: clk phase for minilvds
 */
static void lcd_phy_cntl_set(struct aml_lcd_drv_s *pdrv,
				struct phy_config_s *phy,
				int status, uint32_t flag,
				int bypass, unsigned int mode, unsigned int ckdi)
{
	unsigned int cntl_vinlp_pi = 0, cntl_ckdi = 0;
	unsigned int data = 0, chreg, chctl = 0;
	unsigned int amp = 0;
	int i = 0;
	uint8_t bit;

	uint32_t preem_reg[8] = {
		ANACTRL_DIF_PHY_CNTL1, ANACTRL_DIF_PHY_CNTL2,
		ANACTRL_DIF_PHY_CNTL3, ANACTRL_DIF_PHY_CNTL4,
		ANACTRL_DIF_PHY_CNTL6, ANACTRL_DIF_PHY_CNTL7,
		ANACTRL_DIF_PHY_CNTL8, ANACTRL_DIF_PHY_CNTL9,
	};
	uint32_t amp_reg[8] = {
		ANACTRL_DIF_PHY_CNTL10, ANACTRL_DIF_PHY_CNTL11,
		ANACTRL_DIF_PHY_CNTL12, ANACTRL_DIF_PHY_CNTL13,
		ANACTRL_DIF_PHY_CNTL14, ANACTRL_DIF_PHY_CNTL15,
		ANACTRL_DIF_PHY_CNTL16, ANACTRL_DIF_PHY_CNTL17,
	};

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV)
		LCDPR("%s: %d\n", __func__, status);

	chreg = phy_ctrl_p->ctrl_bit_on ? 0x0 : 0x1;

	if (status) {
		if ((phy->flag & (1 << 13)) == 0) {
			for (i = 0; i < 16; i++)
				phy->lane[i].amp = 0x7;
		}

		if (bypass)
			chctl = 1 << 2;

		if (mode) {
			chreg |= 0x0002;
			cntl_vinlp_pi = 0x00070000;
		} else {
			chreg |= 0x000b;
			if (phy->weakly_pull_down)
				chreg &= ~(1 << 3);
			cntl_vinlp_pi = 0x000e0000;
		}
		cntl_ckdi = ckdi | 0x80000000;
		phy_ctrl_p->lane_lock |= flag;
		lcd_ana_write(ANACTRL_DIF_PHY_CNTL19, cntl_vinlp_pi);
		lcd_ana_write(ANACTRL_DIF_PHY_CNTL20, cntl_ckdi);
	} else {
		phy_ctrl_p->lane_lock &= ~flag;
		if (!phy_ctrl_p->lane_lock) {
			lcd_ana_write(ANACTRL_DIF_PHY_CNTL19, 0);
			lcd_ana_write(ANACTRL_DIF_PHY_CNTL20, 0);
			lcd_ana_write(ANACTRL_DIF_PHY_CNTL18, 0);
		}
	}

	i = 0;
	while (i < 16) {
		if (flag & (1 << i)) {
			bit = i % 2 ? 16 : 0;
			data = (phy->lane[i].preem & 0xff) << 8 | chreg;
			amp = (phy->lane[i].amp & 0x7) << 3 | chctl;
			lcd_ana_setb(preem_reg[i / 2], data, bit, 16);
			lcd_ana_setb(amp_reg[i / 2], amp, bit, 16);
		}
		i++;
	}
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
	unsigned int com_data = 0, flag;

	if (pdrv->index) {
		LCDERR("invalid drv_index %d for lvds\n", pdrv->index);
		return;
	}

	flag = pdrv->config.control.lvds_cfg.dual_port ? 0x3ff : 0x1f;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		if ((phy_ctrl_p->lane_lock & flag) &&
			((phy_ctrl_p->lane_lock & flag) != flag)) {
			LCDERR("phy lane already locked: 0x%x, invalid 0x%x\n",
				phy_ctrl_p->lane_lock, flag);
			return;
		}
		phy_ctrl_p->lane_lock |= flag;
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
			LCDPR("vswing_level=0x%x\n", phy->vswing_level);

		com_data = 0xff2027e0 | phy->vswing;
		lcd_phy_common_update(phy, com_data);
		lcd_phy_cntl_set(pdrv, phy, status, flag, 1, 1, 0);
	} else {
		lcd_phy_cntl_set(pdrv, phy, status, flag, 1, 0, 0);
	}

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("phy lane_lock: 0x%x\n", phy_ctrl_p->lane_lock);
}

static void lcd_vbyone_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	struct phy_config_s *phy = &pdrv->config.phy_cfg;
	unsigned int com_data = 0, flag;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV)
		LCDPR("%s: %d\n", __func__, status);

	if (pdrv->index)
		flag = 0xff << 8;
	else
		flag = pdrv->config.control.vbyone_cfg.lane_count == 16 ? 0xffff : 0xff;

	if (status) {
		if ((phy_ctrl_p->lane_lock & flag) &&
			((phy_ctrl_p->lane_lock & flag) != flag)) {
			LCDERR("phy lane already locked: 0x%x, invalid 0x%x\n",
				phy_ctrl_p->lane_lock, flag);
			return;
		}
		phy_ctrl_p->lane_lock |= flag;

		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			LCDPR("vswing_level=0x%x, ext_pullup=%d\n",
			      phy->vswing_level, phy->ext_pullup);
		}

		if (phy->ext_pullup)
			com_data = 0xff2027e0 | phy->vswing;
		else
			com_data = 0xf02027a0 | phy->vswing;
		lcd_phy_common_update(phy, com_data);
		lcd_phy_cntl_set(pdrv, phy, status, flag, 1, 1, 0);
	} else {
		lcd_phy_cntl_set(pdrv, phy, status, flag, 1, 0, 0);
	}
}

static void lcd_p2p_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	unsigned int p2p_type, vcm_flag;
	struct p2p_config_s *p2p_conf = &pdrv->config.control.p2p_cfg;
	struct phy_config_s *phy = &pdrv->config.phy_cfg;
	unsigned int com_data = 0;
	unsigned int mode = 1;  //1-normal mode, 0-low common mode
	uint32_t flag = 0xffff;

	if (lcd_debug_print_flag & LCD_DBG_PR_ADV)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		if ((phy_ctrl_p->lane_lock & flag) &&
			((phy_ctrl_p->lane_lock & flag) != flag)) {
			LCDERR("phy lane already locked: 0x%x, invalid 0x%x\n",
				phy_ctrl_p->lane_lock, flag);
			return;
		}
		phy_ctrl_p->lane_lock |= flag;

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
		lcd_phy_cntl_set(pdrv, phy, status, flag, 1, mode, 0);
	} else {
		lcd_phy_cntl_set(pdrv, phy, status, flag, 1, 0, 0);
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
