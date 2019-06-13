/*
 * Copyright (C) 2019 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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
