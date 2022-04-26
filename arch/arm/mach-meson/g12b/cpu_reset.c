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

/*
 *GPIOE_0   VDDEE_PWM
 *GPIOE_1   VDDCPU_PWM_B
 *GPIOE_2   VDDCPU_PWM_A
 * */
void set_pwm_to_input(void)
{
	unsigned int val;

	val = readl(AO_RTI_PINMUX_REG1);
	val &= ~(0xfff << 16);
	writel(val, AO_RTI_PINMUX_REG1);/* clear pinmux */
	val = readl(AO_GPIO_O_EN_N);
	val &= ~(0x7 << 16);
	val |= 0x7 << 16;
	writel(val, AO_GPIO_O_EN_N);/* set input mode */
	val = readl(AO_RTI_PULL_UP_EN_REG);
	val &= ~(0x7 << 16);
	writel(val, AO_RTI_PULL_UP_EN_REG);/* disable pull up/down */
}

void reset_system(void)
{
	struct udevice *wdt;
	int ret;

#ifdef CONFIG_USB_DEVICE_V2
	*P_RESET1_REGISTER |= (1<<17);
	mdelay(200);
#endif

	set_pwm_to_input();
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
