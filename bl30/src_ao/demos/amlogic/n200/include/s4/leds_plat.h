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

/*
 * platform related header file
 */

#ifndef _MESON_LEDS_PLAT_H_
#define _MESON_LEDS_PLAT_H_

#ifdef __cplusplus
extern "C" {
#endif
/*
 * pwm S4 leds config
 */
#include <leds_state.h>

#define LED_BREATH_MAX_COUNT 4

extern LedDevice_t MesonLeds[];

enum led_pwm_id {
	LED_PWM_A = 0,
	LED_PWM_B,
	LED_PWM_C,
	LED_PWM_D,
	LED_PWM_E,
	LED_PWM_F,
	LED_PWM_G,
	LED_PWM_H,
	LED_PWM_I,
	LED_PWM_J,
	LED_PWM_INVALID,
};

enum led_id {
	LED_ID_0 = 0,
	LED_ID_MAX,
};

int32_t get_led_breath_len(uint32_t breath_id);
int32_t vLedPinmuxInit(void);
int32_t vLedPlatInit(int32_t ** stickmem);

#ifdef __cplusplus
}
#endif
#endif	/* _MESON_PWM_PLAT_H_ */
