/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __REBOOT_H
#define __REBOOT_H

/*
Reboot reason AND corresponding env setting:
0:  Cold boot                 cold_boot
1:  Normal boot               normal
2:  Factory reset             factory_reset
3:  Upgrade system            update
4:  Fastboot                  fastboot
5:  Suspend                   suspend_off
6:  Hibernate                 hibernate
7:  Fastboot Bootloader       bootloader
8:  Shutdown reboot           shutdown_reboot
9:  RPMBP reboot              rpmbp
10: quiescent reboot          quiescent reboot
11:  Crash dump               crash_dump
12:  Kernel panic             kernel_panic
13:  Watchdog reboot          watchdog_reboot
14: quiescent recovery reboot   quiescent recovery
15: reserved
*/
#define AMLOGIC_COLD_BOOT				0
#define	AMLOGIC_NORMAL_BOOT				1
#define	AMLOGIC_FACTORY_RESET_REBOOT	2
#define	AMLOGIC_UPDATE_REBOOT			3
#define AMLOGIC_FASTBOOT_REBOOT			4
#define AMLOGIC_SUSPEND_REBOOT			5
#define AMLOGIC_HIBERNATE_REBOOT		6
#define AMLOGIC_BOOTLOADER_REBOOT		7 /* fastboot bootloader */
#define AMLOGIC_SHUTDOWN_REBOOT                 8
#define AMLOGIC_RPMBP_REBOOT			9
#define AMLOGIC_QUIESCENT_REBOOT		10
#define	AMLOGIC_CRASH_REBOOT			11
#define AMLOGIC_KERNEL_PANIC			12
#define AMLOGIC_WATCHDOG_REBOOT			13
#define AMLOGIC_RECOVERY_QUIESCENT_REBOOT	14

/*
old version env
0x01010101, normal
0x02020202, factory_reset
0x03030303, update
0x09090909, usb_burning
0x0b0b0b0b, suspend_off
*/

#endif

