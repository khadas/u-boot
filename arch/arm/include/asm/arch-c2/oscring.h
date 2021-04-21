/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __OSCRING_H__
#define __OSCRING_H__

#define	OSCRING_CTL_REG0		SYSCTRL_OSC_RING_CTRL0
#define OSCRING_CTL_DATA0	0x00000555	/*0:25*/
#define OSCRING_CTL_REG1		SYSCTRL_OSC_RING_CTRL1
#define OSCRING_CTL_DATA1	0x15555555	/*0:7*/
#define OSCRING_CTL_REG2		SYSCTRL_OSC_RING_CTRL2
#define OSCRING_CTL_DATA2	0x00000555

#define RING_PWM_VCCK		PWMAB_PWM_B
#define RING_PWM_EE		PWMAB_PWM_A

int ring_msr(int index);
#endif
