/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __OSCRING_H__
#define __OSCRING_H__

#define	OSCRING_CTL_REG		SYSCTRL_OSC_RING_CTRL0
#define OSCRING_CTL_DATA	0x55555555	/*0:23*/

#define RING_PWM_VCCK		PWMAB_PWM_A

int ring_msr(int index);
#endif
