/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

/* This board does not support stick memory */
#ifndef __STICK_MEM_H__
#define __STICK_MEM_H__

extern uint32_t stick_reboot_flag;

void get_stick_reboot_flag(void);
#endif
