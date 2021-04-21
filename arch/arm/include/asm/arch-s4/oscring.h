/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __OSCRING_H__
#define __OSCRING_H__

#define	OSCRING_CTL_REG0		SYSCTRL_OSC_RING_CTRL0
#define OSCRING_CTL_DATA0	0x55555555	/*0:25*/
#define OSCRING_CTL_REG1		SYSCTRL_OSC_RING_CTRL1
#define OSCRING_CTL_DATA1	0x85555555	/*0:7*/

#define RING_PWM_VCCK		PWMIJ_PWM_B
#define RING_PWM_EE		PWMGH_PWM_B

int ring_msr(int index);
#endif
