/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-t5d/pwr_ctrl.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#define PWR_ON    1
#define PWR_OFF   0

enum pm_e {
	PM_DSP = 0,
	PM_DOS_HCODEC,
	PM_DOS_HEVC,
	PM_DOS_VDEC,
	PM_DOS_WAVE,
	PM_VPU_HDMI,
	PM_USB_COMB,
	PM_PCIE,
	PM_GE2D,
	PM_ETH,
	PM_AUDIO
};

unsigned long pwr_ctrl_psci_smc(enum pm_e power_domain, bool power_control);