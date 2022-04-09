// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <io.h>
#include <stdio.h>
#include <timer.h>

#define P_EE_TIMER_E		P_ISA_TIMERE

uint32_t time_start;
uint32_t time_end;

uint32_t get_time(void)
{
	return readl(P_EE_TIMER_E);
}

void _udelay(unsigned int us)
{
#ifndef CONFIG_PXP_EMULATOR
	uint32_t t0 = get_time();

	while (get_time() - t0 <= us)
		;
#endif
}

void timer_start(void)
{
	time_start = get_time();
}

void timer_end(const char *name)
{
	time_end = get_time();
	serial_puts(name);
	serial_puts(" Time: ");
	serial_put_dec(time_end - time_start);
	serial_puts(" us\n");
}
