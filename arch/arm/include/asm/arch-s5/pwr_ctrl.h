/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#define PWR_ON    1
#define PWR_OFF   0

typedef enum pdid {
	PM_MALI_0,
	PM_MALI_1,
	PM_MALI_2,
	PM_MALI_TOP,
	PM_DOS_TOP_WRAP,
	PM_DOS_VDEC,
	PM_DOS_HCODEC,
	PM_DOS_HEVC_TOP,
	PM_DOS_HEVC_CORE1,
	PM_USB2,
	PM_PCIE0,
	PM_GE2D,
	PM_VC9000E,
	PM_VICP,
	PM_VGE_TOP,
	PM_VI_CLK1,
	PM_VI_CLK2,
	PM_VPU_HDMI,
	PM_ETH,
	PM_PCIE1,
	PM_VPU_DOLBY,
	PM_USB30,
	PM_USB31,
	PM_SDCARD,
	PM_SDIO,
	PM_NAND_EMMC,
	PM_NNA_4T,
	PM_DMC00,
	PM_DMC01,
	PM_NOC_DMC_TOP,
	PM_DMC10,
	PM_DMC11,
	PM_DDRPHY0,
	PM_DDRPHY1,
} PM_E;
#define PM_MAX (PM_DDRPHY1 + 1)

unsigned long viu_init_psci_smc(void);
unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain);
