/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/txlx/firmware/bl21/timer.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __TIMER_H
#define __TIMER_H

/**
 * Get the current timestamp from the system timer.
 */
unsigned int get_time(void);

/**
 * Busy-wait.
 *
 * @param us            Number of microseconds to delay.
 */
void _udelay(unsigned int us);

/**
 * time counter
 * usage:
 *     timer_start();
 *     func(); //func that you want measure time consumption
 *     timer_end("func"); //will print "func Time: xxxx us"
 */
//void timer_start(void);
//void timer_end(const char * name);

#endif /* __TIMER_H */
