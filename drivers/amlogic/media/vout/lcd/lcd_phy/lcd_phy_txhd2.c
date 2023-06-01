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

static void lcd_p2p_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	//TODO
}

static void lcd_mlvds_phy_set(struct aml_lcd_drv_s *pdrv, int status)
{
	//TODO
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
	.phy_set_p2p = lcd_p2p_phy_set,
	.phy_set_mipi = lcd_mipi_phy_set,
	.phy_set_edp = NULL,
};

struct lcd_phy_ctrl_s *lcd_phy_config_init_txhd2(struct aml_lcd_data_s *pdata)
{
	phy_ctrl_p = &lcd_phy_ctrl_txhd2;
	return phy_ctrl_p;
}
