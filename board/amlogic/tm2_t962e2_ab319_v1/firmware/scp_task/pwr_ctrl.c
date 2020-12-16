/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * board/amlogic/tm2_t962e2_ab319_v1/firmware/scp_task/pwr_ctrl.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <gpio.h>
#include "pwm_ctrl.h"
#ifdef CONFIG_CEC_WAKEUP
#include <cec_tx_reg.h>
#include <hdmi_cec_arc.h>
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static void set_vddee_voltage(unsigned int target_voltage)
{
	unsigned int to;

	for (to = 0; to < ARRAY_SIZE(pwm_voltage_table_ee); to++) {
		if (pwm_voltage_table_ee[to][1] >= target_voltage) {
			break;
		}
	}

	if (to >= ARRAY_SIZE(pwm_voltage_table_ee)) {
		to = ARRAY_SIZE(pwm_voltage_table_ee) - 1;
	}

	writel(pwm_voltage_table_ee[to][0],AO_PWM_PWM_B);
}

static void power_off_ddr(unsigned int flag)
{
	if (flag) {
		/*set test_n high to power on VDDQ1.5*/
		writel(readl(AO_GPIO_TEST_N) | (1 << 31), AO_GPIO_TEST_N);
		writel(readl(AO_GPIO_TEST_N) & (~(0xf << 8)), AO_GPIO_TEST_N);
	} else {
		/*set test_n low to power off VDDQ1.5*/
		writel(readl(AO_GPIO_TEST_N) & (~(1 << 31)), AO_GPIO_TEST_N);
		writel(readl(AO_GPIO_TEST_N) & (~(0xf << 8)), AO_GPIO_TEST_N);

	}
}

static void power_off_at_24M(unsigned int suspend_from)
{
	/*set gpioAO_3 low to power off VDD5V and hdmi_3.3v*/
	writel(readl(AO_GPIO_O) & (~(1 << 3)), AO_GPIO_O);
	writel(readl(AO_GPIO_O_EN_N) & (~(1 << 3)), AO_GPIO_O_EN_N);
	writel(readl(AO_RTI_PINMUX_REG0) & (~(0xf << 12)), AO_RTI_PINMUX_REG0);
	/*set gpiaoAO_2 low to power off VDDIO_3.3V*/
	writel(readl(AO_GPIO_O) & (~(1 << 2)), AO_GPIO_O);
	writel(readl(AO_GPIO_O_EN_N) & (~(1 << 2)), AO_GPIO_O_EN_N);
	writel(readl(AO_RTI_PINMUX_REG0) & (~(0xf << 8)), AO_RTI_PINMUX_REG0);
	/*set gpioE_2 low to power off VDDCPU and VAD3.3V*/
	writel(readl(AO_GPIO_O) & (~(1 << 18)), AO_GPIO_O);
	writel(readl(AO_GPIO_O_EN_N) & (~(1 << 18)), AO_GPIO_O_EN_N);
	writel(readl(AO_RTI_PINMUX_REG1) & (~(0xf << 24)), AO_RTI_PINMUX_REG1);
	_udelay(100);

	/*step down ee voltage*/
	set_vddee_voltage(CONFIG_VDDEE_SLEEP_VOLTAGE);
	if (suspend_from == SYS_POWEROFF) {
		power_off_ddr(0);
	}
}

static void power_on_at_24M(unsigned int suspend_from)
{

	/*step up ee voltage*/
	set_vddee_voltage(CONFIG_VDDEE_INIT_VOLTAGE);
	_udelay(100);
	/*set gpioE_2 low to power on VDDCPU and VAD3.3V*/
	writel(readl(AO_GPIO_O) | (1 << 18), AO_GPIO_O);
	writel(readl(AO_GPIO_O_EN_N) & (~(1 << 18)), AO_GPIO_O_EN_N);
	writel(readl(AO_RTI_PINMUX_REG1) & (~(0xf << 24)), AO_RTI_PINMUX_REG1);
	/*set gpiaoAO_2 high to power on VDDIO_3*/
	writel(readl(AO_GPIO_O) | (1 << 2), AO_GPIO_O);
	writel(readl(AO_GPIO_O_EN_N) & (~(1 << 2)), AO_GPIO_O_EN_N);
	writel(readl(AO_RTI_PINMUX_REG0) & (~(0xf << 8)), AO_RTI_PINMUX_REG0);
	_udelay(100);
	/*set gpioAO_3 low to power on VDD5V and hdmi_3.3v*/
	writel(readl(AO_GPIO_O) | (1 << 3), AO_GPIO_O);
	writel(readl(AO_GPIO_O_EN_N) & (~(1 << 3)), AO_GPIO_O_EN_N);
	writel(readl(AO_RTI_PINMUX_REG0) & (~(0xf << 12)), AO_RTI_PINMUX_REG0);
	_udelay(10000);
	if (suspend_from == SYS_POWEROFF) {
		power_off_ddr(1);
	}

}

void get_wakeup_source(void *response, unsigned int suspend_from)
{
	struct wakeup_info *p = (struct wakeup_info *)response;
	unsigned val;
	unsigned i = 0;

	p->status = RESPONSE_OK;
	val = (POWER_KEY_WAKEUP_SRC | AUTO_WAKEUP_SRC | REMOTE_WAKEUP_SRC |
	       BT_WAKEUP_SRC | CECB_WAKEUP_SRC);

	p->sources = val;
	p->gpio_info_count = i;

}
extern void __switch_idle_task(void);

static unsigned int detect_key(unsigned int suspend_from)
{
	int exit_reason = 0;
	unsigned char adc_key_cnt = 0;
	unsigned *irq = (unsigned *)WAKEUP_SRC_IRQ_ADDR_BASE;
	init_remote();
#ifdef CONFIG_CEC_WAKEUP
		if (hdmi_cec_func_config & 0x1) {
			cec_hw_reset();
			cec_node_init();
		}
#endif

	do {
#ifdef CONFIG_CEC_WAKEUP
		/*if receive wake up message, wait for the port*/
		if (cec_suspend_wakeup_chk())
			exit_reason = CEC_WAKEUP;

		if (irq[IRQ_AO_CECB] == IRQ_AO_CEC2_NUM) {
			irq[IRQ_AO_CECB] = 0xFFFFFFFF;
			if (cec_suspend_handle())
				exit_reason = CEC_WAKEUP;
		}
#endif

		if (irq[IRQ_AO_IR_DEC] == IRQ_AO_IR_DEC_NUM) {
			irq[IRQ_AO_IR_DEC] = 0xFFFFFFFF;
			if (remote_detect_key())
				exit_reason = REMOTE_WAKEUP;
		}
		if (irq[IRQ_VRTC] == IRQ_VRTC_NUM) {
			irq[IRQ_VRTC] = 0xFFFFFFFF;
			exit_reason = RTC_WAKEUP;
		}

		if (irq[IRQ_AO_TIMERA] == IRQ_AO_TIMERA_NUM) {
			irq[IRQ_AO_TIMERA] = 0xFFFFFFFF;
			saradc_enable();
			if (check_adc_key_resume()) {
				adc_key_cnt++;
				/* using variable 'adc_key_cnt' to eliminate
				 * the dithering of the key
				 */
				if (2 == adc_key_cnt)
					exit_reason = POWER_KEY_WAKEUP;
			} else {
				adc_key_cnt = 0;
			}
			saradc_disable();
		}

		if (exit_reason) {
			set_cec_val2(exit_reason);
			break;
		} else {
			__switch_idle_task();
		}
	} while (1);

	return exit_reason;
}

static void pwr_op_init(struct pwr_op *pwr_op)
{
	pwr_op->power_off_at_24M = power_off_at_24M;
	pwr_op->power_on_at_24M = power_on_at_24M;
	pwr_op->detect_key = detect_key;
	pwr_op->get_wakeup_source = get_wakeup_source;
}
