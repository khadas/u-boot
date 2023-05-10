// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <stdint.h>
#include <watchdog.h>
#include <io.h>
#include <timer.h>

void watchdog_init(uint32_t msec)
{
	// src: 24MHz
	// div: 24000 for 1ms
	// reset ao-22 and ee-21
	*REG_MDUMP_WATCHDOG_CNTL = BIT(24) | BIT(25) | BIT(22) | BIT(21) | (24000 - 1);

	// set timeout
	*REG_MDUMP_WATCHDOG_TCNT = msec;
	*REG_MDUMP_WATCHDOG_RST = 0;

	// enable
	*REG_MDUMP_WATCHDOG_CNTL |= BIT(18);
}

void watchdog_reset(void)
{
	*REG_MDUMP_WATCHDOG_RST = 0;
}

void watchdog_disable(void)
{
	// turn off internal counter and disable
	*REG_MDUMP_WATCHDOG_CNTL &= ~(BIT(18) | BIT(25));
}

void reset_system(void)
{
	int i;
	_udelay(10000); //wait print
	while (1) {
		writel(0x3 | BIT(21) // sys reset en
					| BIT(23) // interrupt en
					| BIT(24) // clk en
					| BIT(25) // clk div en
					| BIT(26) // sys reset now
			, REG_MDUMP_WATCHDOG_CNTL);
		writel(0, REG_MDUMP_WATCHDOG_RST);

		writel(readl(REG_MDUMP_WATCHDOG_CNTL) | BIT(18), // watchdog en
			REG_MDUMP_WATCHDOG_CNTL);
		for (i = 0; i < 100; i++)
			readl(REG_MDUMP_WATCHDOG_CNTL);/*Deceive gcc for waiting some cycles */
	}
}
