// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timer.h>
#include <asm/types.h>

#define P_EE_TIMER_E		P_ISA_TIMERE

uint32_t get_time(void)
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
