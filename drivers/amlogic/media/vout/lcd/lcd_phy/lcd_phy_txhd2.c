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

static unsigned int p2p_phy_ch_reg_mipi_dsi = 0x00020002;
static unsigned int p2p_phy_ch_dig_mipi_dsi = 0x01740174;
static unsigned int p2p_phy_ch_reg_lvds = 0x002a002a;
static unsigned int p2p_phy_ch_dig_lvds = 0x00140014;
static unsigned int p2p_phy_ch_dig_mlvds = 0x00100010;

static unsigned int chreg_reg[5] = {
	HHI_DIF_CSI_PHY_CNTL1,
	HHI_DIF_CSI_PHY_CNTL2,
	HHI_DIF_CSI_PHY_CNTL3,
	HHI_DIF_CSI_PHY_CNTL4,
	HHI_DIF_CSI_PHY_CNTL6
};

static unsigned int chdig_reg[5] = {
	HHI_DIF_CSI_PHY_CNTL8,
	HHI_DIF_CSI_PHY_CNTL9,
	HHI_DIF_CSI_PHY_CNTL10,
	HHI_DIF_CSI_PHY_CNTL11,
	HHI_DIF_CSI_PHY_CNTL12
};

static void lcd_phy_cntl14_update(struct phy_config_s *phy, unsigned int cntl14)
{
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("vswing=0x%x\n", phy->vswing);

	/* vswing */
	cntl14 &= ~(0xf << 12);
	cntl14 |= (phy->vswing << 12);

	/* vcm */
	if ((phy->flag & (1 << 1))) {
		cntl14 &= ~(0xff << 4);
		cntl14 |= (phy->vcm & 0xff) << 4;
	}
	/* odt */
	if ((phy->flag & (1 << 3))) {
		cntl14 &= ~(0xff << 23);
		cntl14 |= (phy->odt & 0xff) << 23;
	}
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL14, cntl14);
}

static void lcd_phy_cntl_lvds_set(struct aml_lcd_drv_s *pdrv, struct phy_config_s *phy,
		unsigned int status)
{
	unsigned int chreg, chdig, data;
	unsigned int i, bitl, bith;

	if (!phy_ctrl_p)
		return;

	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL13, 0);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL15, 0);

	if (status) {
		/*amp*/
		if ((phy->flag & (1 << 13)) == 0) {
			for (i = 0; i < 10; i++)
				phy->lane[i].amp = 0x5;
		}
		data = (p2p_phy_ch_reg_lvds | (1 << 16) | (1 << 0)) & 0xe1c5e1c5;
		chdig = p2p_phy_ch_dig_lvds;
		for (i = 0; i < 5; i++) {
			bitl = i * 2;
			bith = i * 2 + 1;

			chreg = data;
			chreg |= (((phy->lane[bitl].preem & 0xf) << 9) |
				 ((phy->lane[bith].preem & 0xf) << 25));
			chreg |= (((phy->lane[bitl].amp & 0x7) << 3) |
				 ((phy->lane[bith].amp & 0x7) << 19));

			if (lcd_debug_print_flag & LCD_DBG_PR_ADV) {
				LCDPR("%s: lane%d[preem:%d amp:%d] lane%d[preem:%d amp:%d]\n",
					__func__,
					bitl, phy->lane[bitl].preem, phy->lane[bitl].amp,
					bith, phy->lane[bith].preem, phy->lane[bith].amp);
			}

			lcd_ana_write(chreg_reg[i], chreg);
			lcd_ana_write(chdig_reg[i], chdig);
		}
	} else {
		chreg = ((3 << 17) | (3 << 1));
		chdig = 0;
		for (i = 0; i < 5; i++) {
			lcd_ana_write(chreg_reg[i], chreg);
			lcd_ana_write(chdig_reg[i], chdig);
		}
	}
}

static void lcd_lvds_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	struct phy_config_s *phy = &pdrv->config.phy_cfg;
	unsigned int cntl14 = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		cntl14 = 0x106f1;
		lcd_phy_cntl14_update(phy, cntl14);
		lcd_phy_cntl_lvds_set(pdrv, phy, 1);
	} else {
		lcd_ana_write(HHI_DIF_CSI_PHY_CNTL14, 0x0);
		lcd_phy_cntl_lvds_set(pdrv, phy, 0);
	}
}

static void lcd_phy_cntl_mlvds_set(struct aml_lcd_drv_s *pdrv, struct phy_config_s *phy,
		unsigned int status, unsigned int ckdi)
{
	unsigned int cntl13 = 0, data = 0, chreg = 0, chdig = 0;
	unsigned int i, bitl, bith;

	if (!phy_ctrl_p)
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("%s: %d, ckdi:0x%x\n", __func__, status, ckdi);

	if (status) {
		cntl13 = ckdi & 0x3ff;  //ckd_sel
		lcd_ana_setb(HHI_DIF_CSI_PHY_CNTL13, cntl13, 16, 10);
		lcd_ana_setb(HHI_DIF_CSI_PHY_CNTL15, 1, 31, 1);

		/*amp*/
		if ((phy->flag & (1 << 13)) == 0) {
			for (i = 0; i < 10; i++)
				phy->lane[i].amp = 0x5;
		}
		data = p2p_phy_ch_reg_lvds & 0xe1c7e1c7;
		for (i = 0; i < 5; i++) {
			bitl = i * 2;
			bith = i * 2 + 1;
			chreg = data;
			chdig = p2p_phy_ch_dig_mlvds;

			chreg |= (((phy->lane[bitl].preem & 0xf) << 9) |
				 ((phy->lane[bith].preem & 0xf) << 25));
			chreg |= (((phy->lane[bitl].amp & 0x7) << 3) |
				 ((phy->lane[bith].amp & 0x7) << 19));

			if ((ckdi & (1 << bitl)) == 0) { //data lane
				chdig |= (1 << 2);
				chreg = (chreg | (1 << 0)) & ~(1 << 1);
			}
			if ((ckdi & (1 << bith)) == 0) { //data lane
				chdig |= (1 << 18);
				chreg = (chreg | (1 << 16)) & ~(1 << 17);
			}
			if (lcd_debug_print_flag & LCD_DBG_PR_ADV) {
				LCDPR("%s: chreg[%d]=0x%08x, chdig[%d]=0x%08x\n",
					__func__, i, chreg, i, chdig);
			}
			lcd_ana_write(chreg_reg[i], chreg);
			lcd_ana_write(chdig_reg[i], chdig);
		}
	} else {
		chreg = ((3 << 17) | (3 << 1));
		chdig = 0;

		lcd_ana_write(HHI_DIF_CSI_PHY_CNTL13, 0);
		lcd_ana_write(HHI_DIF_CSI_PHY_CNTL15, 0);
		for (i = 0; i < 5; i++) {
			lcd_ana_write(chreg_reg[i], chreg);
			lcd_ana_write(chdig_reg[i], chdig);
		}
	}
}

static void lcd_mlvds_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	struct mlvds_config_s *mlvds_conf;
	struct phy_config_s *phy = &pdrv->config.phy_cfg;
	unsigned int cntl14 = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("%s: %d\n", __func__, status);

	mlvds_conf = &pdrv->config.control.mlvds_cfg;
	if (status) {
		cntl14 = 0x106f1;
		lcd_phy_cntl14_update(phy, cntl14);
		lcd_phy_cntl_mlvds_set(pdrv, phy, status, mlvds_conf->pi_clk_sel);

		lcd_combo_dphy_write(COMBO_DPHY_CNTL0, 0x55555);
	} else {
		lcd_ana_write(HHI_DIF_CSI_PHY_CNTL14, 0x0);
		lcd_phy_cntl_mlvds_set(pdrv, phy, status, 0);
	}
}

static void lcd_phy_cntl_mipi_set(struct aml_lcd_drv_s *pdrv, struct phy_config_s *phy,
		unsigned int status)
{
	unsigned int chreg, chdig, cntl13;
	unsigned int i;

	if (!phy_ctrl_p)
		return;

	if (status) {
		chreg = p2p_phy_ch_reg_mipi_dsi;
		chdig = p2p_phy_ch_dig_mipi_dsi;
		cntl13 = 0x00000099;
	} else {
		chreg = ((3 << 17) | (3 << 1));
		chdig = 0;
		cntl13 = 0;
	}

	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL13, cntl13);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL15, 0);
	for (i = 0; i < 5; i++) {
		lcd_ana_write(chreg_reg[i], chreg);
		lcd_ana_write(chdig_reg[i], chdig);
	}
}

static void lcd_mipi_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	struct phy_config_s *phy = &pdrv->config.phy_cfg;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		lcd_ana_write(HHI_DIF_CSI_PHY_CNTL14, 0x7f820613);
		lcd_phy_cntl_mipi_set(pdrv, phy, 1);
	} else {
		lcd_ana_write(HHI_DIF_CSI_PHY_CNTL14, 0x0);
		lcd_phy_cntl_mipi_set(pdrv, phy, 0);
	}
}

static unsigned int lcd_phy_preem_level_to_val_txhd2(struct aml_lcd_drv_s *pdrv, unsigned int level)
{
	unsigned int preem_value = 0;

	switch (pdrv->config.basic.lcd_type) {
	case LCD_LVDS:
	case LCD_MLVDS:
		preem_value = (level >= 0xf) ? 0xf : level;
		break;
	default:
		break;
	}

	return preem_value;
}

static struct lcd_phy_ctrl_s lcd_phy_ctrl_txhd2 = {
	.lane_lock = 0,
	.ctrl_bit_on = 0,
	.phy_vswing_level_to_val = lcd_phy_vswing_level_to_value_dft,
	.phy_preem_level_to_val = lcd_phy_preem_level_to_val_txhd2,
	.phy_set_lvds = lcd_lvds_phy_set,
	.phy_set_vx1 = NULL,
	.phy_set_mlvds = lcd_mlvds_phy_set,
	.phy_set_p2p = NULL,
	.phy_set_mipi = lcd_mipi_phy_set,
	.phy_set_edp = NULL,
};

struct lcd_phy_ctrl_s *lcd_phy_config_init_txhd2(struct aml_lcd_data_s *pdata)
{
	phy_ctrl_p = &lcd_phy_ctrl_txhd2;
	return phy_ctrl_p;
}
