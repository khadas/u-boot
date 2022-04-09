/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _BOOT_ROM_WATCHDOG_H
#define _BOOT_ROM_WATCHDOG_H

#include "platform_def.h"

#ifndef BIT
#define BIT(nr)			(1 << nr)
#endif
void watchdog_init(uint32_t msec);
void watchdog_reset(void);
void reset_system(void);
void watchdog_disable(void);

/* uboot reset interface */
void reset_cpu(unsigned long flag);

#endif /* _BOOT_ROM_WATCHDOG_H */
