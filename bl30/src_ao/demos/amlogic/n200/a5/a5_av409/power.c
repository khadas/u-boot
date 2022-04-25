/*
 * Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License Agreement
 * (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 * only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission from
 * Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "FreeRTOS.h"
#include "common.h"
#include "gpio.h"
#include "ir.h"
#include "suspend.h"
#include "task.h"
#include "gpio.h"
#include "pwm.h"
#include "pwm_plat.h"
#include "keypad.h"
#include <util.h>
#include "timer_source.h"
#include "../include/vad_suspend.h"

/* #define CONFIG_ETH_WAKEUP */

#ifdef CONFIG_ETH_WAKEUP
#include "interrupt_control.h"
#include "eth.h"
#endif
static int vdd_ee;
static int vdd_cpu;

static IRPowerKey_t prvPowerKeyList[] = {
	{ 0xef10fe01, IR_NORMAL}, /* ref tv pwr */
	{ 0xba45bd02, IR_NORMAL}, /* small ir pwr */
	{ 0xef10fb04, IR_NORMAL}, /* old ref tv pwr */
	{ 0xf20dfe01, IR_NORMAL},
	{ 0xe51afb04, IR_NORMAL},
	{ 0x3ac5bd02, IR_CUSTOM},
	{}
        /* add more */
};

static void vIRHandler(IRPowerKey_t *pkey)
{
	uint32_t buf[4] = {0};
	if (pkey->type == IR_NORMAL)
		buf[0] = REMOTE_WAKEUP;
	else if (pkey->type == IR_CUSTOM)
		buf[0] = REMOTE_CUS_WAKEUP;

        /* do sth below  to wakeup*/
	STR_Wakeup_src_Queue_Send_FromISR(buf);
};

void str_hw_init(void);
void str_hw_disable(void);
void str_power_on(int shutdown_flag);
void str_power_off(int shutdown_flag);
static TaskHandle_t vadTask = NULL;
void vVAD_task(void __unused *pvParameters);
extern void watchdog_reset_system(void);
void vVAD_task(void __unused *pvParameters)
{
	uint32_t buf[4] = {0};

	while (1) {
		if (REG32(SYSCTRL_DEBUG_REG6) == DSP_VAD_WAKUP_ARM) {
			buf[0] = VAD_WAKEUP;
			printf("enter  vVAD_task\n");
			STR_Wakeup_src_Queue_Send(buf);
			vadTask = NULL;
			vTaskDelete(NULL);
			break;
		} else
			vTaskDelay(pdMS_TO_TICKS(30));
	}

}
void str_hw_init(void)
{
	/*enable device & wakeup source interrupt*/
	vIRInit(MODE_HARD_NEC, GPIOD_5, PIN_FUNC1, prvPowerKeyList, ARRAY_SIZE(prvPowerKeyList), vIRHandler);
#ifdef CONFIG_ETH_WAKEUP
	vETHInit(IRQ_ETH_PMT_NUM,eth_handler);
#endif
	xTaskCreate(vVAD_task, "VADtask", configMINIMAL_STACK_SIZE,
		    NULL, VAD_TASK_PRI, &vadTask);

	vBackupAndClearGpioIrqReg();
	vKeyPadInit();
	vGpioIRQInit();
}


void str_hw_disable(void)
{
	/*disable wakeup source interrupt*/
	vIRDeint();
#ifdef CONFIG_ETH_WAKEUP
	vETHDeint();
#endif
	if (vadTask) {
		vTaskDelete(vadTask);
		vadTask = NULL;
	}
	vKeyPadDeinit();
	vRestoreGpioIrqReg();
}

void str_power_on(int shutdown_flag)
{
	int ret;

	shutdown_flag = shutdown_flag;

	/* open PWM clk */
	REG32(CLKCTRL_PWM_CLK_EF_CTRL) |= (1 << 24) | (1 << 8);

	/* set GPIOE_0 & GPIOE_1 pinmux to pwm */
	xPinmuxSet(GPIOE_1, PIN_FUNC1);

	/* enable PWM channel */
	REG32(PWMEF_MISC_REG_AB) |= (1 << 0);
	REG32(PWMEF_MISC_REG_AB) |= (1 << 1);

	/***set vdd_cpu val***/
	ret = vPwmMesonsetvoltage(VDDCPU_VOLT,vdd_cpu);
	if (ret < 0) {
		printf("VDD_CPU pwm set fail\n");
		return;
	}

	/***power on vdd_cpu***/
	ret = xGpioSetDir(GPIO_TEST_N,GPIO_DIR_OUT);
	if (ret < 0) {
		printf("vdd_cpu set gpio dir fail\n");
		return;
	}

	ret = xGpioSetValue(GPIO_TEST_N,GPIO_LEVEL_HIGH);
	if (ret < 0) {
		printf("vdd_cpu set gpio val fail\n");
		return;
	}

	/***set vdd_ee val***/
	ret = vPwmMesonsetvoltage(VDDEE_VOLT,vdd_ee);
	if (ret < 0) {
		printf("VDD_EE pwm set fail\n");
		return;
	}

	if (shutdown_flag) {
		/***power on vcc_3.3v***/
		ret = xGpioSetDir(GPIOD_2,GPIO_DIR_OUT);
		if (ret < 0) {
			printf("vcc_3.3v set gpio dir fail\n");
			return;
		}

		ret= xGpioSetValue(GPIOD_2,GPIO_LEVEL_HIGH);
		if (ret < 0) {
			printf("vcc_3.3v gpio val fail\n");
			return;
		}
	}

	/***power on vcc_5v***/
	ret = xGpioSetDir(GPIOD_6,GPIO_DIR_OUT);
	if (ret < 0) {
		printf("vcc_5v set gpio dir fail\n");
		return;
	}

	ret= xGpioSetValue(GPIOD_6,GPIO_LEVEL_HIGH);
	if (ret < 0) {
		printf("vcc_5v gpio val fail\n");
		return;
	}

	/*Wait 200ms for VDDCPU statble*/
	vTaskDelay(pdMS_TO_TICKS(200));

	if (shutdown_flag) {
		/* disable sar adc */
		watchdog_reset_system();
	}

	printf("vdd_cpu on\n");
}

void str_power_off(int shutdown_flag)
{
	int ret;

	shutdown_flag = shutdown_flag;

	/***power off vcc_5v***/
	ret = xGpioSetDir(GPIOD_6,GPIO_DIR_OUT);
	if (ret < 0) {
		printf("vcc_5v set gpio dir fail\n");
		return;
	}

	ret= xGpioSetValue(GPIOD_6,GPIO_LEVEL_LOW);
	if (ret < 0) {
		printf("vcc_5v gpio val fail\n");
		return;
	}

	if (shutdown_flag) {
		/***power off vcc_3.3v***/
		ret = xGpioSetDir(GPIOD_2,GPIO_DIR_OUT);
		if (ret < 0) {
			printf("vcc_3.3v set gpio dir fail\n");
			return;
		}

		ret= xGpioSetValue(GPIOD_2,GPIO_LEVEL_LOW);
		if (ret < 0) {
			printf("vcc_3.3v gpio val fail\n");
			return;
		}
	}

	/***set vdd_ee val***/
	vdd_ee = vPwmMesongetvoltage(VDDEE_VOLT);
	if (vdd_ee < 0) {
		printf("vdd_EE pwm get fail\n");
		return;
	}

	ret = vPwmMesonsetvoltage(VDDEE_VOLT,720);
	if (ret < 0) {
		printf("vdd_EE pwm set fail\n");
		return;
	}

	/***set vdd_cpu val***/
	vdd_cpu = vPwmMesongetvoltage(VDDCPU_VOLT);
	if (vdd_ee < 0) {
		printf("VDD_CPU pwm get fail\n");
		return;
	}

	/***power off vdd_cpu***/
	ret = xGpioSetDir(GPIO_TEST_N,GPIO_DIR_OUT);
	if (ret < 0) {
		printf("vdd_cpu set gpio dir fail\n");
		return;
	}

	ret= xGpioSetValue(GPIO_TEST_N,GPIO_LEVEL_LOW);
	if (ret < 0) {
		printf("vdd_cpu set gpio val fail\n");
		return;
	}

	/* set GPIOE_0 & GPIOE_1 pinmux to gpio */
	xPinmuxSet(GPIOE_1, PIN_FUNC0);

	/***set vddcpu pwm to input***/
	ret = xGpioSetDir(GPIOE_1,GPIO_DIR_IN);
	if (ret < 0) {
		printf("GPIOE_1 set gpio dir fail\n");
		return;
	}

	/*disable PWM CLK*/
	REG32(CLKCTRL_PWM_CLK_EF_CTRL) &= ~(1 << 24);

	/* disable PWM channel */
	REG32(PWMEF_MISC_REG_AB) &= ~(1 << 1);

	if (shutdown_flag) {
		/* disable sar adc */
		vKeyPadDeinit();
	}

	printf("vdd_cpu off\n");
}
