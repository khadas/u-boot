/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#define PWR_ON    1
#define PWR_OFF   0

#define PM_DSP              0
#define PM_DOS_HCODEC       1
#define PM_DOS_HEVC         2
#define PM_DOS_VDEC         3
#define PM_DOS_WAVE         4
#define PM_VPU_HDMI         5
#define PM_USB_COMB         6
#define PM_PCIE             7
#define PM_GE2D             8
#define PM_ETH              9
#define PM_AUDIO            10
#define PM_MAX              (PM_AUDIO + 1)
unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain);
