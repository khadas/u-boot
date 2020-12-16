/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * include/amlogic/power_domain.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#define PWR_ON	0
#define PWR_OFF	1

#define	PM_DOS_HCODEC		0
#define	PM_DOS_VDEC		1
#define	PM_DOS_HEVC		2
#define	PM_WAVE420L		3
#define	PM_CSI			6
#define	PM_VPU			8
#define	PM_NN			16
#define	PM_USB			17
#define	PM_PCIE0		18
#define	PM_GE2D			19
#define	PM_PCIE1		20
#define	PM_DSPA			21
#define	PM_DSPB			22
#define	PM_DEMOD		23

#ifdef CONFIG_AML_POWER_DOMAIN
void power_domain_switch(int pwr_domain, bool pwr_switch);
#else
#if (defined CONFIG_USB_DEVICE_V2)
static void power_domain_switch(int pwr_domain, bool pwr_switch)
{
}
#endif
#endif
