/*
 * (C) Copyright 2025 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _SMP_H_
#define _SMP_H_

enum {
	SEVT_0 = 0,
	SEVT_1,
	SEVT_2,
	SEVT_3,
};

enum {
	STID_16 = 16,
	STID_17,
	STID_18,
};

struct taskdata {
	ulong arg0;
	ulong arg1;
	ulong arg2;
	ulong arg3;
};

#if CONFIG_IS_ENABLED(SMP)
int smp_event1(int evt, ulong arg0);
int smp_event2(int evt, ulong arg0, ulong arg1);
#else
static inline int smp_event1(int evt, ulong arg0) { return 0; }
static inline int smp_event2(int evt, ulong arg0, ulong arg1) { return 0; }
#endif

#endif

