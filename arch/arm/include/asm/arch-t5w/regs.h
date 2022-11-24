/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-t5d/regs.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __AML_REG_H__
#define __AML_REG_H__

#define ISA_TIMERE                                 ((0x3c62  << 2) + 0xffd00000)
#define WATCHDOG_CNTL                              ((0x3c34  << 2) + 0xffd00000)
#define WATCHDOG_TCNT                              ((0x3c36  << 2) + 0xffd00000)
#define WATCHDOG_RST                               ((0x3c37  << 2) + 0xffd00000)

/*
 * REG_MDUMP_REBOOT_MODE use SYSCTRL_SEC_STATUS_REG2
 */
#define REG_MDUMP_REBOOT_MODE               (0xff800000 + (0x8f << 2)) /* reboot mode */

/* REG_MDUMP_CPUBOOT_STATUS use SYSCTRL_STICKY_REG6
 * REG_MDUMP_COMPRESS_BASE use SYSCTRL_DEBUG_REG6
 * REG_MDUMP_COMPRESS_SIZE use SYSCTRL_DEBUG_REG7
 */
#define REG_MDUMP_CPUBOOT_STATUS            (0xff634400 + (0x076 << 2)) /* boot status*/
#define REG_MDUMP_COMPRESS_BASE             (0xff800000 + (0x2a << 2)) /* ram compress size */
#define REG_MDUMP_COMPRESS_SIZE             (0xff800000 + (0x2b << 2)) /* mdump start addrs */

#endif
