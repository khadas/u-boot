// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <i2c.h>
#include <asm/arch/pmic_bd71837.h>

struct udevice *dev;

void bd71837_set_muxsw_switch(int enable)
{
	uint8_t val = 0;
	int ret = 0;

	if (enable)
		val = 1;
	else
		val = 0;

	ret = dm_i2c_write(dev, BD71837_REG_MUXSW_EN, &val, 1);
	if (ret)
		printf("%s i2c write failed\n", __func__);
}


void pmic_bd71837_init(void)
{
	int rc;

	rc = i2c_get_chip_for_busnum(I2C_BUS_ID, BD71837_CHIP_DEV, 1, &dev);
	if (rc)
		printf("%s i2c write failed\n", __func__);
}
