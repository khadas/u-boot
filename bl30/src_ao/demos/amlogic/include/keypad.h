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
#ifndef __MESON_Button_H
#define __MESON_Button_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include <timers.h>
#include <task.h>
#include <gpio.h>
#include <saradc.h>

/*common macro*/
#define TIMER_CYCLE_TIME		100
#define KEY_JITTER_COUNT		1

/* adc key param */
#define SAMPLE_DEVIATION		40
#define ADCKEY_ID_BASE			512

/* key event */
#define EVENT_SHORT			(1 << 0)
#define EVENT_LONG			(1 << 1)

/* key threshold */
#define THRESHOLD_LONG			2000

enum KeyType{
	GPIO_KEY = 0,
	ADC_KEY,
	KEY_TYPE_NUM
};

enum GpioLevel {
	LOW = 0,
	HIGH
};

enum KeyState{
	UP = 0,
	DOWN,
};

struct xReportEvent {
	uint32_t ulCode;
	uint32_t event;
	TickType_t responseTime;
	void *data;
};

struct xKeyInitInfo {
	uint32_t ulKeyId;
	uint32_t eventMask;
	uint32_t repeatTimeMs;
	uint32_t repeatDelayTimeMs;
	uint32_t longDTTMs;
	uint32_t doubleDTTMs;
	uint32_t combLongDTTMs;
	uint32_t combDTTMs;
	void (*CallBack)(struct xReportEvent);
	void *data;
};

struct xGpioKeyInfo {
	int ulInitLevel;
	struct xKeyInitInfo keyInitInfo;
};

struct xAdcKeyInfo {
	int32_t ulValue;
	AdcInstanceConfig_t xAdcDecp;
	struct xKeyInitInfo keyInitInfo;
};

#define KEY_INIT_INFO(_ulKeyId, _eventMask,	_repeatTimeMs,\
		      _repeatDelayTimeMs, _longDTTMs, _doubleDTTMs,\
		      _combLongDTTMs, _combDTTMs, _CallBack, _data) {	\
	.ulKeyId = _ulKeyId,		\
	.eventMask = _eventMask,		\
	.repeatTimeMs = _repeatTimeMs,		\
	.repeatDelayTimeMs = _repeatDelayTimeMs,	\
	.longDTTMs = _longDTTMs,		\
	.doubleDTTMs = _doubleDTTMs,	\
	.combLongDTTMs = _combLongDTTMs,		\
	.combDTTMs = _combDTTMs,		\
	.CallBack = _CallBack,		\
	.data = _data,			\
}

#define GPIO_KEY_INFO(_ulKeyId, _ulInitLevel, _eventMask,\
		      _CallBack, _data) {	\
	.ulInitLevel = _ulInitLevel,		\
	.keyInitInfo = KEY_INIT_INFO(_ulKeyId, _eventMask, 0, 0, 0,\
				     0, 0, 0, _CallBack, _data),\
}

#define ADC_KEY_INFO(_ulKeyId, _ulValue, _adcChan, _eventMask,\
		     _CallBack, _data) {	\
	.ulValue = _ulValue,			\
	.xAdcDecp = {_adcChan, NO_AVERAGING, 1},			\
	.keyInitInfo = KEY_INIT_INFO(_ulKeyId, _eventMask, 0, 0, 0,\
				     0, 0, 0, _CallBack, _data),\
}

void vCreateGpioKey(struct xGpioKeyInfo *keyArr, uint16_t keyNum);
void vDestoryGpioKey(void);
void vGpioKeyEnable(void);
void vGpioKeyDisable(void);
int  vGpioKeyIsEmpty(void);

void vCreateAdcKey(struct xAdcKeyInfo *keyArr, uint16_t keyNum);
void vDestoryAdcKey(void);
void vAdcKeyEnable(void);
void vAdcKeyDisable(void);
int  vAdcKeyIsEmpty(void);

void vKeyPadCreate(void);
void vKeyPadInit(void);
void vKeyPadDeinit(void);

void vDynamicKeypadInit(void);
#ifdef __cplusplus
}
#endif
#endif
