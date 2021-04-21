// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <common.h>
#include <linux/bitops.h>
#include <dm.h>
#include <phy.h>

/* This function is provided to cope with the possible failures of this phy
 * during aneg process. When aneg fails, the PHY reports that aneg is done
 * but the value found in MII_LPA is wrong:
 *  - Early failures: MII_LPA is just 0x0001. if MII_EXPANSION reports that
 *    the link partner (LP) supports aneg but the LP never acked our base
 *    code word, it is likely that we never sent it to begin with.
 *  - Late failures: MII_LPA is filled with a value which seems to make sense
 *    but it actually is not what the LP is advertising. It seems that we
 *    can detect this using a magic bit in the WOL bank (reg 12 - bit 12).
 *    If this particular bit is not set when aneg is reported being done,
 *    it means MII_LPA is likely to be wrong.
 *
 * In both case, forcing a restart of the aneg process solve the problem.
 * When this failure happens, the first retry is usually successful but,
 * in some cases, it may take up to 6 retries to get a decent result
 */
extern unsigned int setup_amp;

int meson_aml_startup(struct phy_device *phydev)
{
	int ret;

	ret = genphy_update_link(phydev);
	if (ret)
		return ret;

	return genphy_parse_link(phydev);
}
static int meson_phy_config(struct phy_device *phydev)
{
//	unsigned int setup_amp = readl(SYSCTRL_SEC_STATUS_REG12);
	unsigned int efuse_valid = 0;
	efuse_valid = setup_amp >> 4;
	if (efuse_valid) {
		/*Enable Analog and DSP register Bank access by*/
		phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x0000);
		phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x0400);
		phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x0000);
		phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x0400);
		phy_write(phydev, MDIO_DEVAD_NONE, 0x17, setup_amp & 0xf);
		phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x4418);
		printf("setup_amp = %x\n", (setup_amp & 0xf));
	}
	return genphy_config(phydev);
}

static struct phy_driver amlogic_internal_driver = {
	.name = "Meson GXL Internal PHY",
	.uid = 0x01803300,
	.mask = 0xfffffff0,
	.features = PHY_BASIC_FEATURES,
	.config = &meson_phy_config,
	.startup = &meson_aml_startup,
	.shutdown = &genphy_shutdown,
};

int phy_amlogic_init(void)
{
	phy_register(&amlogic_internal_driver);

	return 0;
}
