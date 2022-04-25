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
#include "keypad.h"
#include "gpio.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include <timers.h>
#include <task.h>

struct xOneGpioKeyInfo {
	uint16_t keyJitterCount;
	enum KeyState keyState;
	struct xGpioKeyInfo *gpioKeyInfo;
	struct xOneGpioKeyInfo *xNext;
};

static struct xOneGpioKeyInfo *xHeadKey;
static TimerHandle_t xGpioKeyCycleTimer;

static void prReportEvent(struct xGpioKeyInfo *xKey, uint32_t event)
{
	struct xReportEvent reportEvent;

	reportEvent.ulCode = xKey->keyInitInfo.ulKeyId;
	reportEvent.data = xKey->keyInitInfo.data;
	reportEvent.event = event;
	reportEvent.responseTime = 0;

	if (xKey->keyInitInfo.CallBack)
		xKey->keyInitInfo.CallBack(reportEvent);
}

static void prRunPollFromISR(void)
{
	if (xGpioKeyCycleTimer)
		xTimerChangePeriodFromISR(xGpioKeyCycleTimer,
					  1,
					  NULL);
}

static void prvDetectGpioKey(TimerHandle_t xTimer)
{
	int uiValue;
	uint8_t isPressingKey = 0;
	struct xOneGpioKeyInfo *xPassBtn = xHeadKey;
	struct xGpioKeyInfo *gpioKeyInfo;

	(void)xTimer;
	for (xPassBtn = xHeadKey; xPassBtn != NULL;
						xPassBtn = xPassBtn->xNext) {
		gpioKeyInfo = xPassBtn->gpioKeyInfo;

		uiValue = xGpioGetValue(gpioKeyInfo->keyInitInfo.ulKeyId);
		if (uiValue != gpioKeyInfo->ulInitLevel
					&& xPassBtn->keyState == UP) {
			isPressingKey = 1;

			if (xPassBtn->keyJitterCount < KEY_JITTER_COUNT) {
				xPassBtn->keyJitterCount++;
				continue;
			}
			xPassBtn->keyJitterCount = 0;
			xPassBtn->keyState = DOWN;
			prReportEvent(xPassBtn->gpioKeyInfo, EVENT_SHORT);
		} else if (uiValue == gpioKeyInfo->ulInitLevel
			   && xPassBtn->keyState == DOWN) {

			if (xPassBtn->keyJitterCount < KEY_JITTER_COUNT) {
				xPassBtn->keyJitterCount++;
				isPressingKey = 1;
				continue;
			}
			xPassBtn->keyJitterCount = 0;
			xPassBtn->keyState = UP;
		} else if (uiValue != gpioKeyInfo->ulInitLevel
			   && xPassBtn->keyState == DOWN) {
			isPressingKey = 1;
		}
	}

	if (isPressingKey) {
		xTimerChangePeriod(xGpioKeyCycleTimer,
				   pdMS_TO_TICKS(TIMER_CYCLE_TIME),
				   0);
	}
}

static void prvAddGpioKey(struct xOneGpioKeyInfo *xKey)
{
	if (!xKey)
		return;
	xKey->xNext = xHeadKey;
	xHeadKey = xKey;
}

static void prvGpioKeyHandler(void)
{
	UBaseType_t uxSavedIntStaus;

	uxSavedIntStaus = taskENTER_CRITICAL_FROM_ISR();
	prRunPollFromISR();
	taskEXIT_CRITICAL_FROM_ISR(uxSavedIntStaus);
}

void vCreateGpioKey(struct xGpioKeyInfo *keyArr, uint16_t keyNum)
{
	struct xOneGpioKeyInfo *xOneKey;
	struct xGpioKeyInfo *gpioKeyInfo;
	uint16_t i;

	if (!xGpioKeyCycleTimer) {
		xGpioKeyCycleTimer = xTimerCreate((const char *)"xGpioKeyTimer",
						  1,
						  pdFALSE,
						  (void *)1,
						  (TimerCallbackFunction_t)
						  prvDetectGpioKey);
		if (!xGpioKeyCycleTimer) {
			printf("gpio timer create failed!\n");
			return;
		}
	}

	for (i = 0; i < keyNum; i++) {
		xOneKey = pvPortMalloc(sizeof(struct xOneGpioKeyInfo));
		if (xOneKey == NULL)
			goto fail_alloc2;

		gpioKeyInfo = pvPortMalloc(sizeof(struct xGpioKeyInfo));
		if (gpioKeyInfo == NULL)
			goto fail_alloc1;

		memcpy(gpioKeyInfo, &keyArr[i], sizeof(struct xGpioKeyInfo));
		memset(xOneKey, 0, sizeof(struct xOneGpioKeyInfo));
		xOneKey->keyJitterCount = 0;
		xOneKey->keyState = UP;
		xOneKey->gpioKeyInfo = gpioKeyInfo;
		prvAddGpioKey(xOneKey);

		printf("keypad: add gpio key [%ld]\n",
			xOneKey->gpioKeyInfo->keyInitInfo.ulKeyId);
	}

	return;

fail_alloc1:
	vPortFree(xOneKey);
fail_alloc2:
	printf("gpio key: [%d] malloc failed!\n", i);
}

void vDestoryGpioKey(void)
{
	struct xOneGpioKeyInfo *xPassBtn, *xTmpBtn;
	uint32_t key_id;

	for (xPassBtn = xHeadKey; xPassBtn != NULL;) {
		key_id = xHeadKey->gpioKeyInfo->keyInitInfo.ulKeyId;

		vPortFree(xPassBtn->gpioKeyInfo);

		xTmpBtn = xPassBtn;
		xPassBtn = xPassBtn->xNext;

		vPortFree(xTmpBtn);

		printf("keypad: del gpio key [%ld]\n", key_id);
	}

	xHeadKey = NULL;
}

void vGpioKeyEnable(void)
{
	struct xOneGpioKeyInfo *xPassBtn = xHeadKey;
	struct xKeyInitInfo *keyInitInfo;

	for (xPassBtn = xHeadKey; xPassBtn != NULL;
						xPassBtn = xPassBtn->xNext) {
		keyInitInfo = &(xPassBtn->gpioKeyInfo->keyInitInfo);

		xPinmuxSet(keyInitInfo->ulKeyId, PIN_FUNC0);
		xGpioSetDir(keyInitInfo->ulKeyId, GPIO_DIR_IN);
		xPinconfSet(keyInitInfo->ulKeyId,
				(xPassBtn->gpioKeyInfo->ulInitLevel == HIGH) ?
				PINF_CONFIG_BIAS_PULL_UP :
				PINF_CONFIG_BIAS_PULL_DOWN);
		xRequestGpioIRQ(keyInitInfo->ulKeyId, prvGpioKeyHandler,
				(xPassBtn->gpioKeyInfo->ulInitLevel == HIGH) ?
				IRQF_TRIGGER_FALLING : IRQF_TRIGGER_RISING);
	}
}

void vGpioKeyDisable(void)
{
	struct xOneGpioKeyInfo *xPassBtn = xHeadKey;
	struct xKeyInitInfo *keyInitInfo;

	for (xPassBtn = xHeadKey; xPassBtn != NULL;
						xPassBtn = xPassBtn->xNext) {
		keyInitInfo = &(xPassBtn->gpioKeyInfo->keyInitInfo);

		vFreeGpioIRQ(keyInitInfo->ulKeyId);
	}
}

int vGpioKeyIsEmpty(void)
{
	return (!xHeadKey);
}
