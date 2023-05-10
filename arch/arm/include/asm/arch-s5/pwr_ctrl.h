/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#define PWR_ON    1
#define PWR_OFF   0

#define PM_MALI_0			0
#define PM_MALI_1			1
#define PM_MALI_2			2
#define PM_MALI_TOP			3
#define PM_DOS_TOP_WRAP		4
#define PM_DOS_VDEC			5
#define PM_DOS_HCODEC		6
#define PM_DOS_HEVC_TOP		7
#define PM_DOS_HEVC_CORE1	8
#define PM_USB2				9
#define PM_PCIE0			10
#define PM_GE2D				11
#define PM_VC9000E			12
#define PM_VICP				13
#define PM_VGE_TOP			14
#define PM_VI_CLK1			15
#define PM_VI_CLK2			16
#define PM_VPU_HDMI			17
#define PM_ETH				18
#define PM_PCIE1			19
#define PM_VPU_DOLBY		20
#define PM_USB30			21
#define PM_USB31			22
#define PM_SDCARD			23
#define PM_SDIO				24
#define PM_NAND_EMMC		25
#define PM_NNA_4T			26
#define PM_DMC00			27
#define PM_DMC01			28
#define PM_NOC_DMC_TOP		29
#define PM_DMC10			30
#define PM_DMC11			31
#define PM_DDRPHY0			32
#define PM_DDRPHY1			33

#define PM_MAX (PM_DDRPHY1 + 1)

unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain);
