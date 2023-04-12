/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-t5d/pwr_ctrl.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#define PWR_ON    1
#define PWR_OFF   0
#define PM_ETH		5

enum pm_e {
	PM_VPU,
	PM_DOS_HEVC,
	PM_DEMOD,
};

#define PM_MAX (PM_DEMOD + 1)

unsigned long pwr_ctrl_psci_smc(enum pm_e power_domain, bool power_control);
unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain);
