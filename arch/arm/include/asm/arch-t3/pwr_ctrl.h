/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#define PWR_ON    1
#define PWR_OFF   0

#define PM_DSPA		0
#define PM_DOS_HCODEC	1
#define PM_DOS_HEVC	2
#define PM_DOS_VDEC	3
#define PM_VPU_HDMI	4
#define PM_USB_COMB	5
#define PM_PCIE		6
#define PM_GE2D		7
#define PM_SRAMA		8
#define PM_HDMIRX		9
#define PM_VI_CLK1		10
#define PM_VI_CLK2		11
#define PM_ETH		12
#define PM_NNA		13
#define PM_DEMOD		14
#define PM_FRCTOP		15
#define PM_FRCME		16
#define PM_FRCMC		17
#define PM_SDEMMC_B	18
#define PM_SDEMMC_C	19
#define PM_NOC_DEV		20
#define PM_NOC_VPU	21
#define PM_SPICC0		22
#define PM_SPICC1		23
#define PM_SPICC2		24
#define PM_AUDIO		25

#define PM_MAX		26

unsigned long viu_init_psci_smc(void);
unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain);
