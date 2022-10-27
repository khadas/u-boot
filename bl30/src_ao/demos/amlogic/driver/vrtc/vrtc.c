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
#include "vrtc.h"
#include "timer_source.h"
#include "register.h"
#include "FreeRTOS.h"
#include "mailbox-api.h"
#include "timers.h"
#include "suspend.h"
#include "soc.h"
#include "util.h"

#undef TAG
#define TAG "VRTC"
/* Timer handle */
//TimerHandle_t xRTCTimer = NULL;
static uint32_t last_time;

void set_rtc(uint32_t val)
{
	REG32(VRTC_STICKY_REG) = val;
	/*The last time update RTC*/
	last_time = timere_read();
}

int get_rtc(uint32_t *val)
{
	if (!REG32(VRTC_STICKY_REG))
		return -1;
	else
		*(val) = REG32(VRTC_STICKY_REG);

	return 0;
}

void vRTC_update(void)
{
	uint32_t val;

	if (!get_rtc(&val)) {
		val += timere_read() - last_time;
		set_rtc(val);
	}
}

void *xMboxSetRTC(void *msg)
{
	unsigned int val = *(uint32_t *)msg;
	set_rtc(val);

	return NULL;
}

void *xMboxGetRTC(void *msg)
{
	uint32_t val = 0;

	get_rtc(&val);
	memset(msg, 0, MBOX_BUF_LEN);
	*(uint32_t *)msg = val;

	return NULL;
}

void vRtcInit(void)
{
	int ret;

	ret = xInstallRemoteMessageCallbackFeedBack(AOREE_CHANNEL, MBX_CMD_SET_RTC,
						xMboxSetRTC, 0);
	if (ret == MBOX_CALL_MAX)
		printf("[%s]: mbox cmd 0x%x register fail\n", TAG, MBX_CMD_SET_RTC);

	ret = xInstallRemoteMessageCallbackFeedBack(AOREE_CHANNEL, MBX_CMD_GET_RTC,
						xMboxGetRTC, 1);
	if (ret == MBOX_CALL_MAX)
		printf("[%s]: mbox cmd 0x%x register fail\n", TAG, MBX_CMD_GET_RTC);
}

static TimerHandle_t xRTCTimer = NULL;
static uint32_t time_start;

void alarm_set(void)
{
	uint32_t val;

	val = REG32(VRTC_PARA_REG);

	if (val) {
		printf("[%s]: alarm val=%d S\n", TAG, val);
		time_start = timere_read();
		if (xRTCTimer)
			xTimerStart(xRTCTimer, 0);
	}
}

void alarm_clr(void)
{
	time_start = 0;
	xTimerStop(xRTCTimer, 0);
}


static void valarm_update(TimerHandle_t xTimer) {
	uint32_t val;

	val = REG32(VRTC_PARA_REG);
	xTimer = xTimer;

	if (time_start && (timere_read() - time_start > val)) {
		uint32_t buf[4] = {0};
		buf[0] = RTC_WAKEUP;

		printf("[%s]: vrtc alarm fired\n", TAG);

		REG32(VRTC_PARA_REG) = 0;
		STR_Wakeup_src_Queue_Send(buf);
	}
}

void vCreat_alarm_timer(void)
{
	xRTCTimer = xTimerCreate("Timer", pdMS_TO_TICKS(1000), pdTRUE, NULL, valarm_update);
}

