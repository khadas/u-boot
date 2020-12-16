/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-tm2/timer.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __TIMER_H
#define __TIMER_H

#include <asm/arch/romboot.h>
//#include <asm/arch/timer.h>
#include <asm/arch/io.h>

/**
 * Get the current timestamp from the system timer.
 */
uint32_t get_time(void);

/**
 * Busy-wait.
 *
 * @param us            Number of microseconds to delay.
 */
void _udelay(unsigned int us);

void TE_time(const char *szInfo);

#if defined(BL33_BOOT_TIME_PROBE)
	#define TE TE_time
#else
	#define TE(...)
#endif

#endif /* __TIMER_H */
