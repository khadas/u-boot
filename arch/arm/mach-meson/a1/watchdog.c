
/*
 * arch/arm/cpu/armv8/txl/watchdog.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
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
#include <asm/arch/watchdog.h>
#include <asm/arch/io.h>
#include <asm/arch/timer.h>
#include <dm.h>

#define WDT_DISABLE	1
#define WDT_ENABLE	2
#define WDT_PING	3
#define WDT_INIT	4
#define WDT_RESETNOW	5
#define WDT_SETTIMEOUT	6

void watchdog_init(uint32_t msec)
{
	writel((1 << 25) |  //clk_div_en
		 (1 << 24) |  //clk_en
		 (1 << 22) |  //reset_n_en
		 (0 << 19) |  //src: 24MHz
		 (24000-1),   //24000 for 1ms
		 RESETCTRL_WATCHDOG_CTRL0);

	// set timeout
	writel(msec, RESETCTRL_WATCHDOG_CNT);
	writel(0, RESETCTRL_WATCHDOG_CLR);

	// enable
	setbits_le32(RESETCTRL_WATCHDOG_CTRL0, (1 << 18));
}

void watchdog_reset(void)
{
	writel(0, RESETCTRL_WATCHDOG_CLR);
}

void watchdog_disable(void)
{
	// turn off internal counter and disable
	clrbits_le32(RESETCTRL_WATCHDOG_CTRL0, (1 << 18) | (1 << 25));
}


/*
 *GPIOE_0   VDDEE_PWM
 *GPIOE_1   VDDCPU_PWM
 * */
void set_pwm_to_input(void)
{
	//hxbao, need fine tune
	#if 0
	unsigned int val;

	val = readl(AO_RTI_PINMUX_REG1);
	val &= ~(0xff << 16);
	writel(val, AO_RTI_PINMUX_REG1);/* clear pinmux */
	val = readl(AO_GPIO_O_EN_N);
	val &= ~(0x3 << 16);
	val |= 0x3 << 16;
	writel(val, AO_GPIO_O_EN_N);/* set input mode */
	val = readl(AO_RTI_PULL_UP_EN_REG);
	val &= ~(0x3 << 16);
	writel(val, AO_RTI_PULL_UP_EN_REG);/* disable pull up/down */
	#endif
}

void reset_system(void)
{
	//int i;
	struct udevice *watchdog_devp;
	int ret;

	set_pwm_to_input();

	//_udelay(10000); //wait prin

	set_pwm_to_input();
	ret = uclass_get_device_by_name(UCLASS_WDT, "watchdog", &watchdog_devp);
	if (ret < 0) {
		printf("Failed to find watchdog node, check device tree.\n");
		return ret;
	}
	wdt_start(watchdog_devp, 0, 0);
	while (1);
}

/* uboot reset interface */
void reset_cpu(unsigned long flag){
	reset_system();
}
