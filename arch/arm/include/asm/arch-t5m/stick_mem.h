/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __STICK_MEM_H__
#define __STICK_MEM_H__

#define STICK_MEM_FLAG_1	0x01234567
#define STICK_MEM_FLAG_2	0xfedcba98

enum stick_mem_idx {
	STICK_WAKEUP_REASON = 0,
	STICK_VRTC = 1,
	STICK_REBOOT_FLAG = 2,
	STICK_FLAG_1 = 30,
	STICK_FLAG_2 = 31,
	STICK_MAX = 32
};

extern uint32_t stick_reboot_flag;

void get_stick_reboot_flag(void);
uint32_t wdt_status_check(uint32_t *p_stick_mem);
#endif
