// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/types.h>
#include <asm/arch/romboot.h>
#include <asm/arch/cpu_reset.h>
#include <asm/arch/io.h>
#include <asm/arch/timer.h>
#include <wdt.h>
#include <dm.h>

void reset_system(void)
{
	struct udevice *wdt;
	int ret;

#ifdef CONFIG_USB_DEVICE_V2
	*P_RESET1_REGISTER |= (1<<17);
	mdelay(200);
#endif
	_udelay(10000); //wait print

	ret = uclass_get_device_by_name(UCLASS_WDT, "watchdog", &wdt);
	if (ret) {
		printf("failed to reset system because can't get wdt device\n");
		return;
	}

	wdt_start(wdt, 0, 0);
	while (1);
}

/* uboot reset interface */
void reset_cpu(unsigned long flag){
	reset_system();
}
