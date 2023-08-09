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

__maybe_unused static unsigned int p2p_phy_ch_reg_mipi = 0x00060006;
__maybe_unused static unsigned int p2p_phy_ch_reg_lvds = 0x002a002a;
__maybe_unused static unsigned int p2p_phy_ch_dig_mipi = 0x01700170;
__maybe_unused static unsigned int p2p_phy_ch_dig_lvds = 0x00140014;
__maybe_unused static unsigned int p2p_phy_ch_dig_mlvds = 0x00100010;

static void lcd_phy_cntl_set(struct aml_lcd_drv_s *pdrv, struct phy_config_s *phy,
	unsigned int chreg, unsigned int chdig)
{
	unsigned int data = 0;

	if (!phy_ctrl_p)
		return;

	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL15, 0);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL16, 1 << 31);

	data = ((phy->lane[0].preem & 0xff) << 8 |
	       (phy->lane[1].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL1, chreg | data);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL8, chdig);

	data = ((phy->lane[2].preem & 0xff) << 8 |
	       (phy->lane[3].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL2, chreg | data);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL9, chdig);

	data = ((phy->lane[4].preem & 0xff) << 8 |
	       (phy->lane[5].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL3, chreg | data);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL10, chdig);

	data = ((phy->lane[6].preem & 0xff) << 8 |
	       (phy->lane[7].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL4, chreg | data);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL11, chdig);

	data = ((phy->lane[8].preem & 0xff) << 8 |
	       (phy->lane[9].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL6, chreg | data);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL12, chdig);

	data = ((phy->lane[10].preem & 0xff) << 8 |
	       (phy->lane[11].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL7, chreg | data);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL13, chdig);
}

static void lcd_lvds_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	struct phy_config_s *phy = &pdrv->config.phy_cfg;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		lcd_ana_write(HHI_DIF_CSI_PHY_CNTL14, 0x156f1);
		lcd_phy_cntl_set(pdrv, phy,
			p2p_phy_ch_reg_lvds, p2p_phy_ch_dig_lvds);
	} else {
		lcd_ana_write(HHI_DIF_CSI_PHY_CNTL14, 0x0);
	}
}

static void lcd_phy_cntl14_update(struct phy_config_s *phy, unsigned int cntl14)
{
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

/*
 *    chreg: channel ctrl
 *    bypass: 1=bypass
 *    ckdi: clk phase for minilvds
 */
static void lcd_phy_cntl_mlvds_set(struct aml_lcd_drv_s *pdrv, struct phy_config_s *phy, int status,
				int bypass, unsigned int pi_clk_sel)
{
	unsigned int cntl13 = 0;
	unsigned int data = 0, chreg = 0;
	unsigned int tmp = p2p_phy_ch_dig_mlvds;

	if (!phy_ctrl_p)
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("%s: %d\n", __func__, status);

	if (status) {
		chreg |= ((phy_ctrl_p->ctrl_bit_on << 16) |
			     (phy_ctrl_p->ctrl_bit_on << 0)) |
			     p2p_phy_ch_reg_lvds;
		cntl13 = (pi_clk_sel) & 0x3ff;  //ckd_sel
	} else {
		if (phy_ctrl_p->ctrl_bit_on)
			data = 0;
		else
			data = 1;
		chreg |= ((data << 16) | (data << 0));
		cntl13 = 0;
		lcd_ana_write(HHI_DIF_CSI_PHY_CNTL14, 0);
	}

	lcd_ana_setb(HHI_DIF_CSI_PHY_CNTL13, cntl13, 16, 10);
	lcd_ana_setb(HHI_DIF_CSI_PHY_CNTL15, 1, 31, 1);

	data = ((phy->lane[0].preem & 0xff) << 8 |
	       (phy->lane[1].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL1, chreg | data);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL8, tmp);

	data = ((phy->lane[2].preem & 0xff) << 8 |
	       (phy->lane[3].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL2, chreg | data);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL9, tmp);

	data = ((phy->lane[4].preem & 0xff) << 8 |
	       (phy->lane[5].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL3, chreg | data);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL10, tmp);

	data = ((phy->lane[6].preem & 0xff) << 8 |
	       (phy->lane[7].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL4, chreg | data);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL11, tmp);

	data = ((phy->lane[8].preem & 0xff) << 8 |
	       (phy->lane[9].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL6, chreg | data);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL12, tmp);

	data = ((phy->lane[10].preem & 0xff) << 8 |
	       (phy->lane[11].preem & 0xff) << 24);
	lcd_ana_write(HHI_DIF_CSI_PHY_CNTL7, chreg | data);
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
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
			LCDPR("vswing_level=0x%x\n", phy->vswing_level);

		cntl14 = 0x156f1 | (phy->vswing << 12);
		lcd_phy_cntl14_update(phy, cntl14);
		lcd_phy_cntl_mlvds_set(pdrv, phy, status, 0, mlvds_conf->pi_clk_sel);
	} else {
		lcd_phy_cntl_mlvds_set(pdrv, phy, status, 0, 0);
	}
}

static void lcd_mipi_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	//TODO
}

static struct lcd_phy_ctrl_s lcd_phy_ctrl_txhd2 = {
	.lane_lock = 0,
	.ctrl_bit_on = 0,
	.phy_vswing_level_to_val = NULL,
	.phy_preem_level_to_val = NULL,
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
