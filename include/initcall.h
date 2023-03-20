/*
 * Copyright (c) 2011 The Chromium OS Authors.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __INIT_CALL_H__
#define __INIT_CALL_H__

typedef int (*init_fnc_t)(void);

int initcall_run_list(const init_fnc_t init_sequence[]);

#ifdef CONFIG_AMLOGIC_TIME_PROFILE
void dump_initcall_time(void);
uint32_t get_time(void);

struct init_call_time {
	void *func;
	unsigned int time;
};

#define INITCALL_CNT		128
#endif

#endif
