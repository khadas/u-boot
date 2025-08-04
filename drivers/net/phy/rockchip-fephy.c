// SPDX-License-Identifier: GPL-2.0+
/**
 *
 * Driver for ROCKCHIP Fast Ethernet PHYs
 *
 * Copyright (c) 2025, Rockchip Electronics Co., Ltd
 *
 * David Wu <david.wu@rock-chips.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <config.h>
#include <common.h>
#include <misc.h>
#include <phy.h>

#define ROCKCHIP_FEPHY_ID			0x06808101

#define MII_INTERNAL_CTRL_STATUS		17
#define SMI_ADDR_CFGCNTL			20
#define SMI_ADDR_TSTREAD1			21
#define SMI_ADDR_TSTREAD2			22
#define SMI_ADDR_TSTWRITE			23
#define MII_LED_CTRL				25
#define MII_INT_STATUS				29
#define MII_INT_MASK				30
#define MII_SPECIAL_CONTROL_STATUS		31

#define MII_AUTO_MDIX_EN			BIT(7)
#define MII_MDIX_EN				BIT(6)

#define MII_SPEED_10				BIT(2)
#define MII_SPEED_100				BIT(3)

#define CFGCNTL_WRITE_ADDR			0
#define CFGCNTL_READ_ADDR			5
#define CFGCNTL_GROUP_SEL			11
#define CFGCNTL_RD				(BIT(15) | BIT(10))
#define CFGCNTL_WR				(BIT(14) | BIT(10))

#define CFGCNTL_WRITE(group, reg)		(CFGCNTL_WR | ((group) << CFGCNTL_GROUP_SEL) \
						| ((reg) << CFGCNTL_WRITE_ADDR))
#define CFGCNTL_READ(group, reg)		(CFGCNTL_RD | ((group) << CFGCNTL_GROUP_SEL) \
						| ((reg) << CFGCNTL_READ_ADDR))

#define GAIN_PRE				GENMASK(5, 2)
#define WR_ADDR_A7CFG				0x18

enum {
	GROUP_CFG0 = 0,
	GROUP_WOL,
	GROUP_CFG0_READ,
	GROUP_BIST,
	GROUP_AFE,
	GROUP_CFG1
};

static int rockchip_fephy_group_read(struct phy_device *phydev, u8 group, u32 reg)
{
	int ret;

	ret = phy_write(phydev, MDIO_DEVAD_NONE, SMI_ADDR_CFGCNTL, CFGCNTL_READ(group, reg));
	if (ret)
		return ret;

	if (group)
		return phy_read(phydev, MDIO_DEVAD_NONE, SMI_ADDR_TSTREAD1);
	else
		return (phy_read(phydev, MDIO_DEVAD_NONE, SMI_ADDR_TSTREAD1) |
			(phy_read(phydev, MDIO_DEVAD_NONE, SMI_ADDR_TSTREAD2) << 16));
}

static int rockchip_fephy_group_write(struct phy_device *phydev, u8 group,
				      u32 reg, u16 val)
{
	int ret;

	ret = phy_write(phydev, MDIO_DEVAD_NONE, SMI_ADDR_TSTWRITE, val);
	if (ret)
		return ret;

	return phy_write(phydev, MDIO_DEVAD_NONE, SMI_ADDR_CFGCNTL, CFGCNTL_WRITE(group, reg));
}

static int rockchip_fephy_startup(struct phy_device *phydev)
{
	int ret;

	/* Read the Status (2x to make sure link is right) */
	ret = genphy_update_link(phydev);
	if (ret)
		return ret;

	/* Read the Status (2x to make sure link is right) */
	phy_read(phydev, MDIO_DEVAD_NONE, MII_BMSR);

	return genphy_parse_link(phydev);
}

static int rockchip_fephy_config_init(struct phy_device *phydev)
{
	int ret;

	/* LED Control, default:0x7f */
	ret = phy_write(phydev, MDIO_DEVAD_NONE, MII_LED_CTRL, 0x7aa);
	if (ret)
		return ret;

	/* off-energy level0 threshold */
	ret = rockchip_fephy_group_write(phydev, GROUP_CFG0, 0xa, 0x6664);
	if (ret)
		return ret;

	/* 100M amplitude control */
	ret = rockchip_fephy_group_write(phydev, GROUP_CFG0, 0x18, 0xc);
	if (ret)
		return ret;

	/* 24M */
	{
		int sel;

		/* pll cp cur sel */
		sel = rockchip_fephy_group_read(phydev, GROUP_AFE, 0x3);
		if (sel < 0)
			return sel;
		ret = rockchip_fephy_group_write(phydev, GROUP_AFE, 0x3, sel | 0x2);
		if (ret)
			return ret;

		/* pll lpf res sel */
		ret = rockchip_fephy_group_write(phydev, GROUP_CFG0, 0x1a, 0x6);
		if (ret)
			return ret;
	}

	return ret;
}

static struct phy_driver rockchip_fephy_driver = {
	.name = "Rockchip FEPHY",
	.uid = ROCKCHIP_FEPHY_ID,
	.mask = 0xfffffff,
	.features = PHY_BASIC_FEATURES,
	.config = &rockchip_fephy_config_init,
	.startup = &rockchip_fephy_startup,
};

int phy_rockchip_fephy_init(void)
{
	phy_register(&rockchip_fephy_driver);
	return 0;
}
