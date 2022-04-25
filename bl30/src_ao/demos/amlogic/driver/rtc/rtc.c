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
 *//*
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
#include "common.h"
#include "rtc.h"
#include "register.h"
#include "FreeRTOS.h"
#include "mailbox-api.h"
#include "soc.h"
#include "util.h"
#include "string.h"
#include "interrupt.h"
#include "suspend.h"
#include <unistd.h>

#undef TAG
#define TAG "AOCPU RTC"

static void vRTCInterruptHandler(void)
{
	uint32_t buf[4] = {0};
	uint32_t alarm0_int_status;
	uint32_t reg_val;

	/* Mask alarm0 irq */
	reg_val = REG32(RTC_INT_MASK);
	reg_val |= 0x1;
	REG32(RTC_INT_MASK) = reg_val;

	/* Clear alarm0 */
	REG32(RTC_ALARM0_REG) = 0;

	alarm0_int_status = REG32(RTC_INT_STATUS) & (1 << RTC_INT_ALM0_IRQ);
	/* Clear alarm0 int status */
	if (alarm0_int_status)
		REG32(RTC_INT_CLR) |= (1 << RTC_INT_CLR_ALM0_IRQ);

	printf("[%s]: rtc alarm fired\n", TAG);

	buf[0] = RTC_WAKEUP;
	STR_Wakeup_src_Queue_Send_FromISR(buf);
}

static uint32_t get_reboot_mode(void)
{
	uint32_t reg_val;
	uint32_t reboot_mode;

	reg_val = REG32(SYSCTRL_SEC_STATUS_REG31);
	reboot_mode = ((reg_val >> 12) & 0xf);

	return reboot_mode;
}

static void reset_rtc(void)
{
	uint32_t reg_val;

	printf("[%s]: reset rtc\n", TAG);
	/* Reset RTC */
	reg_val = (1 << 0);
	REG32(RESETCTRL_RESET4) = reg_val;
	/* Mask RTC reset to prevent RTC being reset in the next reboot */
	reg_val = REG32(RESETCTRL_RESET4_MASK);
	reg_val |= (1 << 0);
	REG32(RESETCTRL_RESET4_MASK) = reg_val;
}

static int get_rtc(uint32_t *val)
{
	if (!REG32(VRTC_STICKY_REG))
		return -1;
	else
		*(val) = REG32(VRTC_STICKY_REG);

	return 0;
}

static void set_rtc(uint32_t val)
{
	REG32(VRTC_STICKY_REG) = val;
}

void store_rtc(void)
{
	uint32_t reg_val;

	reg_val = REG32(RTC_REAL_TIME);
	REG32(VRTC_STICKY_REG) = reg_val;
}

void *MboxSetRTC(void *msg)
{
	unsigned int val = *(uint32_t *)msg;
	printf("[%s]: MboxSetRTC val=0x%x \n", TAG, val);
	set_rtc(val);

	return NULL;
}

void *MboxGetRTC(void *msg)
{
	uint32_t val = 0;

	get_rtc(&val);
	memset(msg, 0, MBOX_BUF_LEN);
	*(uint32_t *)msg = val;

	printf("[%s]: MboxGetRTC val=0x%x\n", TAG, val);

	return NULL;
}

void rtc_init(void)
{
	int ret;
	uint32_t reboot_mode;

	printf("[%s]: init rtc\n", TAG);
	ret = RegisterIrq(RTC_IRQ, 6, vRTCInterruptHandler);
	if (ret)
		printf("[%s]: RegisterIrq error, ret = %d\n", TAG, ret);
	EnableIrq(RTC_IRQ);

	ret = xInstallRemoteMessageCallbackFeedBack(AOREE_CHANNEL, MBX_CMD_SET_RTC,
						MboxSetRTC, 0);
	if (ret == MBOX_CALL_MAX)
		printf("[%s]: mbox cmd 0x%x register fail\n", TAG, MBX_CMD_SET_RTC);

	ret = xInstallRemoteMessageCallbackFeedBack(AOREE_CHANNEL, MBX_CMD_GET_RTC,
						MboxGetRTC, 1);
	if (ret == MBOX_CALL_MAX)
		printf("[%s]: mbox cmd 0x%x register fail\n", TAG, MBX_CMD_GET_RTC);

	reboot_mode = get_reboot_mode();
	if (reboot_mode == COLD_REBOOT)
		reset_rtc();
}

