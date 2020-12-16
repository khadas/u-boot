/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/gxl/firmware/scp_task/lib/delay.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#define P_EE_TIMER_E		(*((volatile unsigned *)(0xc1100000 + (0x2662 << 2))))

unsigned int get_time(void)
{
	return P_EE_TIMER_E;
}

void _udelay(unsigned int us)
{
	unsigned int t0 = get_time();

	while (get_time() - t0 <= us)
		;
}

