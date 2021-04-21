/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __OSCRING_H__
#define __OSCRING_H__

#define	OSCRING_CTL_REG		0xff6345fc
#define OSCRING_CTL_DATA	0x555555

#define RING_PWM_VCCK		(0xff802000 + (0x01 << 2))
#define RING_PWM_EE		(0xff807000 + (0x01 << 2))

int ring_msr_g12a(int index);
int ring_msr_sm1(int index);
int ring_msr(int index);
#endif
