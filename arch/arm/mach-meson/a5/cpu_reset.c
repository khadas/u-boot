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
#include <dm.h>
#include <wdt.h>

/*
 *GPIOE_0   VDDEE_PWM
 *GPIOE_1   VDDCPU_PWM
 * */
void set_pwm_to_input(void)
{
	//need fine tune
#if 0
	unsigned int val;

	val = readl(AO_RTI_PINMUX_REG1);
	val &= ~(0xff << 16);
	writel(val, AO_RTI_PINMUX_REG1);	/* clear pinmux */
	val = readl(AO_GPIO_O_EN_N);
	val &= ~(0x3 << 16);
	val |= 0x3 << 16;
	writel(val, AO_GPIO_O_EN_N);	/* set input mode */
	val = readl(AO_RTI_PULL_UP_EN_REG);
	val &= ~(0x3 << 16);
	writel(val, AO_RTI_PULL_UP_EN_REG);	/* disable pull up/down */
#endif
}

void reset_system(void)
{
	struct udevice *watchdog_devp;
	int ret;

	set_pwm_to_input();

	_udelay(10000);		//wait print

	ret = uclass_get_device_by_name(UCLASS_WDT, "watchdog", &watchdog_devp);
	if (ret < 0) {
		printf("failed to reset system because can't get wdt device\n");
		return;
	}
	wdt_start(watchdog_devp, 0, 0);
	while (1);
}

/* uboot reset interface */
void reset_cpu(unsigned long flag)
{
	reset_system();
}
