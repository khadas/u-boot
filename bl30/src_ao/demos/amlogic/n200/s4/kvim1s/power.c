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
#include "btwake.h"
#include "meson_i2c.h"

/* #define CONFIG_ETH_WAKEUP */

#ifdef CONFIG_ETH_WAKEUP
#include "interrupt_control.h"
#include "eth.h"
#endif

#define CHIP_ADDR              0x18
#define REG_LED_SYSTEM_ON_MODE  0x28



static TaskHandle_t cecTask = NULL;
static int vdd_ee;

static IRPowerKey_t prvPowerKeyList[] = {
	{ 0xeb14ff00, IR_NORMAL}, /* ref tv pwr */
	{ 0xba45bd02, IR_NORMAL}, /* small ir pwr */
	{ 0xef10fb04, IR_NORMAL}, /* old ref tv pwr */
	{ 0xf20dfe01, IR_NORMAL},
	{ 0xe51afb04, IR_NORMAL},
	{ 0xff00fe06, IR_NORMAL},
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
	vETHInit(IRQ_ETH_PMT_NUM,eth_handler);
#endif
	xTaskCreate(vCEC_task, "CECtask", configMINIMAL_STACK_SIZE,
		    NULL, CEC_TASK_PRI, &cecTask);

	vBackupAndClearGpioIrqReg();
	vKeyPadInit();
	vGpioIRQInit();
	bt_task_init();
}


void str_hw_disable(void)
{
	/*disable wakeup source interrupt*/
	vIRDeint();
#ifdef CONFIG_ETH_WAKEUP
	vETHDeint();
#endif
	if (cecTask) {
		vTaskDelete(cecTask);
		cec_req_irq(0);
	}
	bt_task_deinit();
	vKeyPadDeinit();
	vRestoreGpioIrqReg();
}

void mcu_i2c_init(void);
void mcu_i2c_init(void)
{
        // set pinmux
        xPinmuxSet(GPIOD_6, PIN_FUNC5);
        xPinmuxSet(GPIOD_7, PIN_FUNC5);
        //set ds and pull up
        xPinconfSet(GPIOD_6, PINF_CONFIG_BIAS_PULL_UP | PINF_CONFIG_DRV_STRENGTH_3);
        xPinconfSet(GPIOD_7, PINF_CONFIG_BIAS_PULL_UP | PINF_CONFIG_DRV_STRENGTH_3);

        xI2cMesonPortInit(I2C_M1);
}


void str_power_on(int shutdown_flag)
{
	int ret;
	/***Power 5v***/
	ret = xGpioSetDir(GPIOZ_7,GPIO_DIR_OUT);
	if (ret < 0) {
		printf("vdd_cpu set gpio dir fail\n");
		return;
	}

	ret= xGpioSetValue(GPIOZ_7,GPIO_LEVEL_HIGH);
	if (ret < 0) {
		printf("vcc5v set gpio val fail\n");
		return;
	}

	shutdown_flag = shutdown_flag;
	/***set vdd_ee val***/
	ret = vPwmMesonsetvoltage(VDDEE_VOLT,vdd_ee);
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
	/*Wait 200ms for VDDCPU statble*/
	vTaskDelay(pdMS_TO_TICKS(200));

	uint8_t val = 0;
	mcu_i2c_init();
	// pwr_led set off
	ret = xI2cMesonWrite(CHIP_ADDR, REG_LED_SYSTEM_ON_MODE, &val, 1);
	if (ret < 0) {
			printf("pwr_led set off fail\n");
			return;
	}
	printf("vdd_cpu on\n");
}

void str_power_off(int shutdown_flag)
{
	int ret;
	shutdown_flag = shutdown_flag;

	/***set vdd_ee val***/
	vdd_ee = vPwmMesongetvoltage(VDDEE_VOLT);
	if (vdd_ee < 0) {
		printf("vdd_EE pwm get fail\n");
		return;
	}

#ifndef CONFIG_ETH_WAKEUP
	ret = vPwmMesonsetvoltage(VDDEE_VOLT,770);
	if (ret < 0) {
		printf("vdd_EE pwm set fail\n");
		return;
	}
#endif

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
	printf("vdd_cpu off\n");

	/***Power 5v***/
	ret = xGpioSetDir(GPIOZ_7,GPIO_DIR_OUT);
	if (ret < 0) {
		printf("vdd_cpu set gpio dir fail\n");
		return;
	}

	ret= xGpioSetValue(GPIOZ_7,GPIO_LEVEL_LOW);
	if (ret < 0) {
		printf("vcc5v set gpio val fail\n");
		return;
	}

    uint8_t val = 1;
	mcu_i2c_init();
	// pwr_led set on
	ret = xI2cMesonWrite(CHIP_ADDR, REG_LED_SYSTEM_ON_MODE, &val, 1);
	if (ret < 0) {
			printf("pwr_led set on fail\n");
			return;
	}

}
