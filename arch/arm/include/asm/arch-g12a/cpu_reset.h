/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _BOOT_ROM_WATCHDOG_H
#define _BOOT_ROM_WATCHDOG_H
#include <asm/arch/secure_apb.h>

void reset_system(void);

/* uboot reset interface */
void reset_cpu(unsigned long flag);

#endif /* _BOOT_ROM_WATCHDOG_H */
