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

#ifndef _MESON_PWM_PLAT_H_
#define _MESON_PWM_PLAT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <register.h>

#define PwmMesonVolt_Duty	1

/* There are 4 pwm controllers in a5 */
enum pwm_chip_id {
	PWM_AB = 0,
	PWM_CD,
	PWM_EF,
	PWM_GH,
	PWM_MUX,
};

/* VDDEE VDDCPU in s4 */
enum pwm_voltage_id {
	VDDEE_VOLT = 0,
	VDDCPU_VOLT,
};

typedef struct xPwmMesonVoltage {
	uint32_t Voltage_mv;
	uint32_t Duty_reg;
} xPwmMesonVoltage_t;

typedef struct xPwmMesonChip {
	uint32_t chip_id;
	unsigned long addr;
	uint32_t mask;
	/*transfers the clk div, clk gate, and clk mux to the clktree.*/
	unsigned long clk_addr;
} xPwmMesonChip_t;

uint32_t prvMesonVoltToPwmChip(enum pwm_voltage_id voltage_id);
uint32_t prvMesonVoltToPwmChannel(enum pwm_voltage_id voltage_id);
xPwmMesonVoltage_t *vPwmMesonGetVoltTable(uint32_t voltage_id);
uint32_t vPwmMesonGetVoltTableSize(uint32_t voltage_id);
xPwmMesonChip_t *prvIdToPwmChip(uint32_t chip_id);

#ifdef __cplusplus
}
#endif
#endif	/* _MESON_PWM_PLAT_H_ */
