/*
 * driver/display/lcd/aml_lcd_phy_config.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the named License,
 * or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _AML_LCD_PHY_CONFIG_H
#define _AML_LCD_PHY_CONFIG_H

struct lcd_phy_ctrl_s {
	void (*phy_set_lvds)(struct lcd_config_s *pconf, int status);
	void (*phy_set_vx1)(struct lcd_config_s *pconf, int status);
	void (*phy_set_mlvds)(struct lcd_config_s *pconf, int status);
	void (*phy_set_p2p)(struct lcd_config_s *pconf, int status);
	void (*phy_set_mipi)(struct lcd_config_s *pconf, int status);
};

/* -------------------------- */
/* lvsd phy parameters define */
/* -------------------------- */
#define LVDS_PHY_CNTL1_G9TV    0x606cca80
#define LVDS_PHY_CNTL2_G9TV    0x0000006c
#define LVDS_PHY_CNTL3_G9TV    0x00000800

#define LVDS_PHY_CNTL1_TXHD    0x6c60ca80
#define LVDS_PHY_CNTL2_TXHD    0x00000070
#define LVDS_PHY_CNTL3_TXHD    0x03ff0c00
/* -------------------------- */

/* -------------------------- */
/* vbyone phy parameters define */
/* -------------------------- */
#define VX1_PHY_CNTL1_G9TV            0x6e0ec900
#define VX1_PHY_CNTL1_G9TV_PULLUP     0x6e0f4d00
#define VX1_PHY_CNTL2_G9TV            0x0000007c
#define VX1_PHY_CNTL3_G9TV            0x00ff0800
/* -------------------------- */

/* -------------------------- */
/* minilvds phy parameters define */
/* -------------------------- */
#define MLVDS_PHY_CNTL1_TXHD   0x6c60ca80
#define MLVDS_PHY_CNTL2_TXHD   0x00000070
#define MLVDS_PHY_CNTL3_TXHD   0x03ff0c00
/* -------------------------- */

/* ******** MIPI_DSI_PHY ******** */
/* bit[15:11] */
#define MIPI_PHY_LANE_BIT        11
#define MIPI_PHY_LANE_WIDTH      5

/* MIPI-DSI */
#define DSI_LANE_0              (1 << 4)
#define DSI_LANE_1              (1 << 3)
#define DSI_LANE_CLK            (1 << 2)
#define DSI_LANE_2              (1 << 1)
#define DSI_LANE_3              (1 << 0)
#define DSI_LANE_COUNT_1        (DSI_LANE_CLK | DSI_LANE_0)
#define DSI_LANE_COUNT_2        (DSI_LANE_CLK | DSI_LANE_0 | DSI_LANE_1)
#define DSI_LANE_COUNT_3        (DSI_LANE_CLK | DSI_LANE_0 |\
					DSI_LANE_1 | DSI_LANE_2)
#define DSI_LANE_COUNT_4        (DSI_LANE_CLK | DSI_LANE_0 |\
					DSI_LANE_1 | DSI_LANE_2 | DSI_LANE_3)

#define DSI_LANE_0_G12A         0x10 //(1 << 4)
#define DSI_LANE_CLK_G12A       0x08 //(1 << 3)
#define DSI_LANE_1_G12A         0x04 //(1 << 2)
#define DSI_LANE_2_G12A         0x02 //(1 << 1)
#define DSI_LANE_3_G12A         0x01 //(1 << 0)
#define DSI_LANE_COUNT_1_G12A   (DSI_LANE_CLK_G12A | DSI_LANE_0_G12A)
#define DSI_LANE_COUNT_2_G12A   (DSI_LANE_CLK_G12A | DSI_LANE_0_G12A | DSI_LANE_1_G12A)
#define DSI_LANE_COUNT_3_G12A   (DSI_LANE_CLK_G12A | DSI_LANE_0_G12A |\
					DSI_LANE_1_G12A | DSI_LANE_2_G12A)
#define DSI_LANE_COUNT_4_G12A   (DSI_LANE_CLK_G12A | DSI_LANE_0_G12A |\
					DSI_LANE_1_G12A | DSI_LANE_2_G12A | DSI_LANE_3_G12A)
static unsigned int lvds_vx1_p2p_phy_ch_tl1 = 0x00020002;
static unsigned int lvds_vx1_p2p_phy_preem_tl1[] = {
	0x06,
	0x26,
	0x46,
	0x66,
	0x86,
	0xa6,
	0xf6,
};

static unsigned int p2p_low_common_phy_ch_tl1 = 0x000b000b;
static unsigned int p2p_low_common_phy_preem_tl1[] = {
	0x07,
	0x17,
	0x37,
	0x77,
	0xf7,
	0xff,
};
#endif

