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
#include <gpio.h>
#include <leds_plat.h>
#include <leds_state.h>

/* TODO: Temporarily use static variables instead of stick mem */
static int32_t LedStickMem0;
//static int32_t LedStickMem1;

static LedCoord_t BreathInflections0[] = {
	{0, 0},
	{2500, 255},
	{5000, 0}
};

static LedCoord_t BreathInflections1[] = {
	{0, 0},
	{5000, 255},
	{10000, 0}
};

static LedCoord_t BreathInflections2[] = {
	{0, 0},
	{10000, 255},
	{20000, 0}
};

static LedCoord_t BreathInflections3[] = {
	{0, 0},
	{20000, 255},
	{40000, 0}
};

LedCoord_t *BreathInflections[LED_BREATH_MAX_COUNT] = {
	BreathInflections0,
	BreathInflections1,
	BreathInflections2,
	BreathInflections3,
};

LedDevice_t MesonLeds[] = {
	{
		.id = LED_ID_0,
		.type = LED_TYPE_PWM,
		.name = "sys_led",
		.hardware_id = LED_PWM_C,
		.polarity = LED_POLARITY_POSITIVE,
		.breathtime = 0,
	},
};

int32_t get_led_breath_len(uint32_t breath_id)
{
	switch (breath_id) {
		case 0:
			return sizeof(BreathInflections0)/sizeof(LedCoord_t);
		case 1:
			return sizeof(BreathInflections1)/sizeof(LedCoord_t);
		case 2:
			return sizeof(BreathInflections2)/sizeof(LedCoord_t);
		case 3:
			return sizeof(BreathInflections3)/sizeof(LedCoord_t);
	} /* end swith */

	iprintf("%s: id: %ld leds state init!\n", DRIVER_NAME, breath_id);

	return -pdFREERTOS_ERRNO_EINVAL;
}

int32_t vLedPlatInit(int32_t ** stickmem)
{
	/* TODO: Here is initialization stickmem, but not doing so now */
	*stickmem = &LedStickMem0;

	/* off by default */
	return xLedsStateSetBrightness(LED_ID_0, LED_OFF);
}

int32_t vLedPinmuxInit(void)
{
	/* set pinmux */
	return xPinmuxSet(GPIOD_7, PIN_FUNC2);
}



