/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-t5d/regs.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifdef REGISTER_H
#else
#define REGISTER_H

#define ISA_TIMERE                                 ((0x3c62  << 2) + 0xffd00000)
#define WATCHDOG_CNTL                              ((0x3c34  << 2) + 0xffd00000)
#define WATCHDOG_TCNT                              ((0x3c36  << 2) + 0xffd00000)
#define WATCHDOG_RESET                             ((0x3c37  << 2) + 0xffd00000)
#endif