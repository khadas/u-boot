/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _MESON_PMIC_BD71837_H_
#define _MESON_PMIC_BD71837_H_

#define I2C_BUS_ID		6
#define BD71837_CHIP_DEV	0x4b
#define BD71837_REG_MUXSW_EN    0x30

void bd71837_set_muxsw_switch(int enable);
void pmic_bd71837_init(void);
#endif /*_MESON_PMIC_BD71837_H_*/
