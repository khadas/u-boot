/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _PWRC_H_
#define _PWRC_H_

#define PWR_ON    1
#define PWR_OFF   0

#define PM_AUDIO	0
#define PM_SDIOA	1
#define PM_EMMC  2
#define PM_USB_COMB   3
#define PM_ETH     4
#define PM_VOUT          5
#define PM_AUDIO_PDM          6
#define PM_DMC    7
#define PM_SYS_WRAP          8
#define PM_AO_I2C_S     9
#define PM_AO_UART         10
#define PM_AO_IR     11

#define PM_MAX		12

unsigned long viu_init_psci_smc(void);
unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain);
#endif
