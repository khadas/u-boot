/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#define PWR_ON    1
#define PWR_OFF   0
#define PM_ETH    5

#define PM_DOS_HCODEC    0
#define PM_DOS_GE2D_WRAP 1
#define PM_VPU_HDMI      2
#define PM_DEMOD         3
#define PM_FRC_TOP       4

#define PM_MAX           5
#define PM_DSPA		(PM_MAX + 1)
#define PM_HDMIRX	9

unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain);
