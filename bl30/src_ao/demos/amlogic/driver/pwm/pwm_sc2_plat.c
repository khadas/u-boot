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
 * pwm sc2 plat driver
 */
#include "FreeRTOS.h"
#include <common.h>
#include <pwm.h>
#include "util.h"

xPwmMesonChip_t meson_pwm_chip[] = {
	{PWM_AB, PWMAB_PWM_A, 0, CLKCTRL_PWM_CLK_AB_CTRL},
	{PWM_CD, PWMCD_PWM_A, 0, CLKCTRL_PWM_CLK_CD_CTRL},
	{PWM_EF, PWMEF_PWM_A, 0, CLKCTRL_PWM_CLK_EF_CTRL},
	{PWM_GH, PWMGH_PWM_A, 0, CLKCTRL_PWM_CLK_GH_CTRL},
	{PWM_IJ, PWMIJ_PWM_A, 0, CLKCTRL_PWM_CLK_IJ_CTRL},
};

/* VDDEE voltage table  volt must ascending */
xPwmMesonVoltage_t vddee_table[] = {
		{690, 0x140000},
		{700, 0x120000},
		{710, 0x110001},
		{720, 0x100002},
		{730, 0x0f0003},
		{740, 0x0e0004},
		{750, 0x0d0005},
		{760, 0x0c0006},
		{770, 0x0b0007},
		{780, 0x0a0008},
		{790, 0x090009},
		{800, 0x08000a},
		{810, 0x07000b},
		{820, 0x06000c},
		{830, 0x05000d},
		{840, 0x04000e},
		{850, 0x03000f},
		{860, 0x020010},
		{870, 0x010011},
		{880, 0x12},
		{890, 0x14},
};

/* VDDCPU voltage table  volt must ascending */
xPwmMesonVoltage_t vddcpu_table[] = {
		{689, 0x00240000},
		{699, 0x00220000},
		{709, 0x00210001},
		{719, 0x00200002},
		{729, 0x001f0003},
		{739, 0x001e0004},
		{749, 0x001d0005},
		{759, 0x001c0006},
		{769, 0x001b0007},
		{779, 0x001a0008},
		{789, 0x00190009},
		{799, 0x0018000a},
		{809, 0x0017000b},
		{819, 0x0016000c},
		{829, 0x0015000d},
		{839, 0x0014000e},
		{849, 0x0013000f},
		{859, 0x00120010},
		{869, 0x00110011},
		{879, 0x00100012},
		{889, 0x000f0013},
		{899, 0x000e0014},
		{909, 0x000d0015},
		{919, 0x000c0016},
		{929, 0x000b0017},
		{939, 0x000a0018},
		{949, 0x00090019},
		{959, 0x0008001a},
		{969, 0x0007001b},
		{979, 0x0006001c},
		{989, 0x0005001d},
		{999, 0x0004001e},
		{1009, 0x0003001f},
		{1019, 0x00020020},
		{1029, 0x00010021},
		{1039, 0x00000022},
		{1049, 0x00000024},
};

/*
 * todo: need processing here vddee pwmh vddcpu pwmj
 * Different boards may use different pwm channels
 */
uint32_t prvMesonVoltToPwmChip(enum pwm_voltage_id voltage_id)
{
	switch (voltage_id) {
	case VDDEE_VOLT:
		return PWM_GH;

	case VDDCPU_VOLT:
		return PWM_IJ;

	default:
		break;
	}
	return PWM_MUX;
}

/*
 * todo: need processing here
 * Different boards may use different pwm channels
 */
uint32_t prvMesonVoltToPwmChannel(enum pwm_voltage_id voltage_id)
{
	switch (voltage_id) {
	case VDDEE_VOLT:
	case VDDCPU_VOLT:
		return MESON_PWM_1;

	default:
		break;
	}
	return MESON_PWM_2;
}

xPwmMesonVoltage_t *vPwmMesonGetVoltTable(uint32_t voltage_id)
{
	switch (voltage_id) {
	case VDDEE_VOLT:
		return vddee_table;

	case VDDCPU_VOLT:
		return vddcpu_table;

	default:
		break;
	}
	return NULL;
}

uint32_t vPwmMesonGetVoltTableSize(uint32_t voltage_id)
{
	switch (voltage_id) {
	case VDDEE_VOLT:
		return sizeof(vddee_table)/sizeof(xPwmMesonVoltage_t);

	case VDDCPU_VOLT:
		return sizeof(vddcpu_table)/sizeof(xPwmMesonVoltage_t);

	default:
		break;
	}
	return 0;
}

xPwmMesonChip_t *prvIdToPwmChip(uint32_t chip_id)
{
	if (chip_id >= PWM_MUX) {
		iprintf("pwm chip id is invalid!\n");
		return NULL;
	}

	return meson_pwm_chip + chip_id;
}

