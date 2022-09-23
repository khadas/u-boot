/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _PWRC_H_
#define _PWRC_H_

#define PM_NNA		0
#define PM_AUDIO	1
#define PM_RESV_SEC	2
#define PM_SDIOA	3
#define PM_EMMC		4
#define PM_USB_COMB	5
#define PM_SDCARD	6
#define PM_ETH		7
#define PM_RESV0	8
#define PM_GE2D		9
#define PM_CVE		10
#define PM_CDG_WRAP	11
#define PM_ISP_TOP	12
#define PM_MIPI_ISP_WRAP	13
#define PM_VCODEC	14

#define PM_MAX		15

//static int pm_max = 8;
/*
char* domain_name[] =
{
	"PM_DOS_HEVC",
	"PM_DOS_VDEC",
	"PM_VPU_HDMI",
	"PM_USB_COMB",
	"PM_GE2D",
	"PM_ETH",
	"PM_DEMOD",
	"PM_AUDIO",
};
*/
unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
#endif
