/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __TIMER_H
#define __TIMER_H

#include <asm/arch/romboot.h>
#include <asm/arch/timer.h>
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

#endif /* __TIMER_H */
