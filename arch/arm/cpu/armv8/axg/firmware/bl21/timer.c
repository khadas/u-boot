/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/axg/firmware/bl21/timer.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "io.h"
#include "timer.h"
#include <asm/arch/secure_apb.h>

#define P_EE_TIMER_E		P_ISA_TIMERE

unsigned int get_time(void)
{
	return readl(P_EE_TIMER_E);
}

void _udelay(unsigned int us)
{
#ifndef CONFIG_PXP_EMULATOR
	unsigned int t0 = get_time();

	while (get_time() - t0 <= us)
		;
#endif
}
