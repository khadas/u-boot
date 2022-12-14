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
#include "meson_i2c.h"

#if DEF_P1_AWAKE_SOURCE  //TODO
static int vdd_ee;
#endif

void str_hw_init(void);
void str_hw_disable(void);
void str_power_on(int shutdown_flag);
void str_power_off(int shutdown_flag);

void str_hw_init(void)
{
	/*enable device & wakeup source interrupt*/
#if DEF_P1_AWAKE_SOURCE  //TODO
	vBackupAndClearGpioIrqReg();
	vKeyPadInit();
	vGpioIRQInit();
#endif
}


void str_hw_disable(void)
{
#if DEF_P1_AWAKE_SOURCE //TODO
	/*disable wakeup source interrupt*/
	vKeyPadDeinit();
	vRestoreGpioIrqReg();
#endif
}

void str_power_on(int shutdown_flag)
{
	shutdown_flag = shutdown_flag;
#if DEF_PWM_PWR    //todo for pmic
	int ret;

	shutdown_flag = shutdown_flag;
	/***set vdd_ee val***/
	ret = vPwmMesonSetVoltage(VDDEE_VOLT,vdd_ee);
	if (ret < 0) {
		printf("vdd_EE pwm set fail\n");
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
	/*Wait 200ms for VDDCPU stable*/
	vTaskDelay(pdMS_TO_TICKS(200));
#endif
	printf("vdd_cpu on\n");
}

void str_power_off(int shutdown_flag)
{
	shutdown_flag = shutdown_flag;
#if DEF_PWM_PWR    //todo for pmic
	int ret;

	shutdown_flag = shutdown_flag;
	/***set vdd_ee val***/
	vdd_ee = vPwmMesonGetVoltage(VDDEE_VOLT);
	if (vdd_ee < 0) {
		printf("vdd_EE pwm get fail\n");
		return;
	}

	ret = vPwmMesonSetVoltage(VDDEE_VOLT,770);
	if (ret < 0) {
		printf("vdd_EE pwm set fail\n");
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
#endif
	printf("vdd_cpu off\n");
}
