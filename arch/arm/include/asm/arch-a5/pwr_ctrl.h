/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _PWRC_H_
#define _PWRC_H_

#define PWR_ON    1
#define PWR_OFF   0
#define PM_NNA    0
#define PM_AUDIO  1
#define PM_SDIOA  2
#define PM_EMMC   3
#define PM_USB_COMB     4
#define PM_ETH          5
#define PM_RSA          6
#define PM_AUDIO_PDM    7
#define PM_DMC          8
#define PM_SYS_WRAP     9
#define PM_DSPA         10

#define PM_MAX		11

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
unsigned long viu_init_psci_smc(void);
unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
#endif
