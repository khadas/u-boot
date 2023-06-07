/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#define PWR_ON    1
#define PWR_OFF   0

#define PM_DSPA   0
#define	PM_MALI_0 1
#define	PM_MALI_1 2
#define	PM_MALI_2 3
#define	PM_DOS_VDEC   4
#define	PM_DOS_HCODEC 5
#define	PM_DOS_HEVC_TOP   6
#define	PM_USB2HX2    7
#define	PM_GE2D   8
#define	PM_VICP   9
#define	PM_VI_CLK1    10
#define	PM_VI_CLK2    11
#define	PM_VPU_DOLBY  12
#define	PM_VPU_HDMI   13
#define	PM_ETH    14
#define	PM_PCIE   15
#define	PM_USB30  16
#define	PM_USB31  17
#define	PM_SDCARD 18
#define	PM_NAND_EMMC  19
#define	PM_NNA_4T 20
#define	PM_FRC_TOP    21
#define	PM_HDMIRX_WRAP    22
#define	PM_DEMOD  23
#define	PM_AUDIO  24
#define	PM_DMC00  25
#define	PM_NOC_DMC_TOP    26
#define	PM_DMC10  27
#define	PM_MAX    28

unsigned long viu_init_psci_smc(void);
unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain);
