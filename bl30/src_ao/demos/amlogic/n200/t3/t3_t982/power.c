#define MTAG "power"
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
#include "mailbox-api.h"
#include "hdmi_cec.h"
#include "btwake.h"
#include "soc.h"
#include "pm.h"

/*#define CONFIG_ETH_WAKEUP*/

#ifdef CONFIG_ETH_WAKEUP
#include "interrupt_control.h"
#include "eth.h"
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

static void *xMboxVadWakeup(void *msg)
{
	uint32_t buf[4] = {0};

	msg = msg;
	buf[0] = VAD_WAKEUP;
	STR_Wakeup_src_Queue_Send(buf);

	return NULL;
}

void str_hw_init(void);
void str_hw_disable(void);
void str_power_on(int shutdown_flag);
void str_power_off(int shutdown_flag);

void str_hw_init(void)
{
	int ret;

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
	Bt_GpioIRQRegister();

	ret = xInstallRemoteMessageCallbackFeedBack(AODSPA_CHANNEL, MBX_CMD_VAD_AWE_WAKEUP, xMboxVadWakeup, 0);
	if (ret == MBOX_CALL_MAX)
		printf("mbox cmd 0x%x register fail\n", MBX_CMD_VAD_AWE_WAKEUP);
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
	Bt_GpioIRQFree();
	vKeyPadDeinit();
	vRestoreGpioIrqReg();
	xUninstallRemoteMessageCallback(AODSPA_CHANNEL, MBX_CMD_VAD_AWE_WAKEUP);
}

static void vcc3v3_ctrl(int is_on)
{
	if (xGpioSetDir(GPIOD_10,GPIO_DIR_OUT) < 0) {
		printf("vcc3v3 set gpio dir fail\n");
		return;
	}

	if (xGpioSetValue(GPIOD_10, is_on ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW) < 0) {
		printf("vcc3v3 set gpio val fail\n");
		return;
	}

	printf("vcc3v3 %s\n", is_on ? "on" : "off");
};

static void vcc5v_ctrl(int is_on)
{
	if (xGpioSetDir(GPIO_TEST_N,GPIO_DIR_OUT) < 0) {
		printf("vcc5v set gpio dir fail\n");
		return;
	}

	if (xGpioSetValue(GPIO_TEST_N, is_on ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW) < 0) {
		printf("vcc5v set gpio val fail\n");
		return;
	}

	printf("vcc5v %s\n", is_on ? "on" : "off");
};

#define power_on_vcc3v3()	vcc3v3_ctrl(1)
#define power_off_vcc3v3()	vcc3v3_ctrl(0)
#define power_on_vcc5v()	vcc5v_ctrl(1)
#define power_off_vcc5v()	vcc5v_ctrl(0)


void str_power_on(int shutdown_flag)
{
	int ret;

	shutdown_flag = shutdown_flag;

	/***power on vcc3v3***/
	power_on_vcc3v3();

	/***restore vddcpu***/
	ret = vPwmMesonsetvoltage(VDDCPU_VOLT,vdd_cpu);
	if (ret < 0) {
		printf("vddcpu pwm set fail\n");
		return;
	}
	printf("vddcpu restored\n");

	/***set vdd_ee val***/
	ret = vPwmMesonsetvoltage(VDDEE_VOLT,vdd_ee);
	if (ret < 0) {
		printf("vdd_EE pwm set fail\n");
		return;
	}

	/***power on vcc5v***/
	power_on_vcc5v();

	/*Wait 200ms for VDDCPU statble*/
	vTaskDelay(pdMS_TO_TICKS(200));
}

void str_power_off(int shutdown_flag)
{
	int ret;

	shutdown_flag = shutdown_flag;
#ifndef CONFIG_ETH_WAKEUP
	/***power off vcc3v3***/
	power_off_vcc3v3();
#endif
	/***set vddcpu val***/
	vdd_cpu = vPwmMesongetvoltage(VDDCPU_VOLT);
	if (vdd_cpu < 0) {
		printf("vddcpu pwm get fail\n");
		return;
	}
	ret = vPwmMesonsetvoltage(VDDCPU_VOLT,720);
	if (ret < 0) {
		printf("vddcpu pwm set fail\n");
		return;
	}
	printf("vddcpu 720mv\n");

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

	/***power off vcc5v***/
	power_off_vcc5v();
}

extern int set_platform_power_ops(struct platform_power_ops *ops);


static int platform_power_begin(void)
{
	logi("%s, power off\n", __func__);
	str_power_off(0);
	return 0;
}

static int platform_power_end(void)
{
	logi("%s, power on\n", __func__);
	str_power_on(0);
	return 0;
}

static struct platform_power_ops ops = {
	.begin = platform_power_begin,
	.end = platform_power_end,
};

void platform_power_interface_register(void);
void platform_power_interface_register(void)
{
	set_platform_power_ops(&ops);
}

