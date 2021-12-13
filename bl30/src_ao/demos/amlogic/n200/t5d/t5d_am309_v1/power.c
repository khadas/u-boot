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

#include "hdmi_cec.h"

/*#define CONFIG_ETH_WAKEUP*/

#ifdef CONFIG_ETH_WAKEUP
#include "interrupt_control.h"
#include "eth.h"
#include "irq.h"
#endif

static TaskHandle_t cecTask = NULL;
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

void str_hw_init(void)
{
	/*enable device & wakeup source interrupt*/
	vIRInit(MODE_HARD_NEC, GPIOD_5, PIN_FUNC1, prvPowerKeyList, ARRAY_SIZE(prvPowerKeyList), vIRHandler);
#ifdef CONFIG_ETH_WAKEUP
	vETHInit(IRQ_ETH_PMT_NUM,eth_handler_t5);
#endif
	xTaskCreate(vCEC_task, "CECtask", configMINIMAL_STACK_SIZE,
		    NULL, CEC_TASK_PRI, &cecTask);
	return;
	vBackupAndClearGpioIrqReg();
	vGpioKeyEnable();
	vGpioIRQInit();
}


void str_hw_disable(void)
{
	/*disable wakeup source interrupt*/
	vIRDeint();
#ifdef CONFIG_ETH_WAKEUP
	vETHDeint_t5();
#endif
	if (cecTask) {
		vTaskDelete(cecTask);
		cec_req_irq(0);
	}
	return;
	vGpioKeyDisable();
	vRestoreGpioIrqReg();
}

void str_power_on(int shutdown_flag)
{
	int ret;

	/***set vdd_ee val***/
	ret = vPwmMesonsetvoltage(VDDEE_VOLT,vdd_ee);
	if (ret < 0) {
		printf("vdd_EE pwm set fail\n");
		return;
	}

	/***set vdd_ee val***/
	ret = vPwmMesonsetvoltage(VDDCPU_VOLT,vdd_cpu);
	if (ret < 0) {
		printf("vdd_CPU pwm set fail\n");
		return;
	}

	/***power on vcc3.3***/
	ret = xGpioSetDir(GPIOD_10,GPIO_DIR_OUT);
	if (ret < 0) {
		printf("vcc3.3 set gpio dir fail\n");
		return;
	}

	ret = xGpioSetValue(GPIOD_10,GPIO_LEVEL_HIGH);
	if (ret < 0) {
		printf("vcc3.3 set gpio val fail\n");
		return;
	}

	if (shutdown_flag) {
		/***power on VDDQ/VDDCPU***/
		ret = xGpioSetDir(GPIOD_4,GPIO_DIR_OUT);
		if (ret < 0) {
			printf("VDDCPU/VDDQ set gpio dir fail\n");
			return;
		}

		ret = xGpioSetValue(GPIOD_4,GPIO_LEVEL_HIGH);
		if (ret < 0) {
			printf("VDDCPU/VDDQ set gpio val fail\n");
			return;
		}
		/*Wait 200ms for VDDCPU statble*/
		vTaskDelay(pdMS_TO_TICKS(200));
	}

	/***power on 5v***/
	REG32(AO_GPIO_TEST_N) = REG32(AO_GPIO_TEST_N) | (1 << 31);
}

void str_power_off(int shutdown_flag)
{
	int ret;

	printf("poweroff 5v\n");
	printf("0x%x\n", REG32(AO_GPIO_TEST_N));

	REG32(AO_GPIO_TEST_N) = (REG32(AO_GPIO_TEST_N) << 1) >> 1;

	if (shutdown_flag) {
		/***power off VDDQ/VDDCPU***/
		ret = xGpioSetDir(GPIOD_4,GPIO_DIR_OUT);
		if (ret < 0) {
			printf("VDDCPU/VDDQ set gpio dir fail\n");
			return;
		}

		ret = xGpioSetValue(GPIOD_4,GPIO_LEVEL_LOW);
		if (ret < 0) {
			printf("VDDCPU/VDDQ set gpio val fail\n");
			return;
		}
	}

	/***power off vcc3.3***/
	ret = xGpioSetDir(GPIOD_10,GPIO_DIR_OUT);
	if (ret < 0) {
		printf("vcc3.3 set gpio dir fail\n");
		return;
	}

#ifndef CONFIG_ETH_WAKEUP
	ret= xGpioSetValue(GPIOD_10,GPIO_LEVEL_LOW);
	if (ret < 0) {
		printf("vcc3.3 set gpio val fail\n");
		return;
	}
#endif

	/***set vdd_cpu val***/
	vdd_cpu = vPwmMesongetvoltage(VDDCPU_VOLT);
	if (vdd_cpu < 0) {
		printf("vdd_CPU pwm get fail\n");
		return;
	}

	ret = vPwmMesonsetvoltage(VDDCPU_VOLT,700);
	if (ret < 0) {
		printf("vdd_CPU pwm set fail\n");
		return;
	}

	/***set vdd_ee val***/
	vdd_ee = vPwmMesongetvoltage(VDDEE_VOLT);
	if (vdd_ee < 0) {
		printf("vdd_EE pwm get fail\n");
		return;
	}

	ret = vPwmMesonsetvoltage(VDDEE_VOLT,770);
	if (ret < 0) {
		printf("vdd_EE pwm set fail\n");
		return;
	}

	//REG32( ((0x0000 << 2) + 0xff638c00)) = 0;
}

