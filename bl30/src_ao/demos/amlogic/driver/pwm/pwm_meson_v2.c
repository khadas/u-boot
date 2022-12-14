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
 * pwm driver
 */
#include "FreeRTOS.h"
#include <common.h>
#include <pwm.h>
#include <util.h>
#include <register.h>
#include <task.h>

#define pwm_readl(reg)				(*((volatile uint32_t *)(reg)))
#define CLK_24M					24000000UL
#define USEC_PER_SEC				1000000000ULL

/*pwm register att*/
typedef struct xPwmMesonRegs {
	uint32_t  dar; /* A Duty Register */
	uint32_t  dbr; /* B Duty Register */
	uint32_t  miscr; /* misc Register */
	uint32_t  dsr;/*DS Register*/
	uint32_t  tr;/*times Register*/
	uint32_t  da2r; /* A2 Duty Register */
	uint32_t  db2r; /* B2 Duty Register */
	uint32_t  br; /*Blink Register*/
} xPwmMesonRegs_t;

extern unsigned int xPortIsIsrContext(void);
static void prvPwmEnterCritical(UBaseType_t *uxIsr)
{
	if (xPortIsIsrContext())
		*uxIsr = taskENTER_CRITICAL_FROM_ISR();
	else {
		taskENTER_CRITICAL();
		*uxIsr = 0;
	}
};

static void prvPwmExitCritical(UBaseType_t uxSaveIsr)
{
	if (xPortIsIsrContext())
		taskEXIT_CRITICAL_FROM_ISR(uxSaveIsr);
	else {
		taskEXIT_CRITICAL();
		uxSaveIsr = 0;
	}
};

static void prvPwmRegWrite(uint32_t addr, uint32_t mask, uint32_t val)
{
	UBaseType_t uxSavedIsr;

	prvPwmEnterCritical(&uxSavedIsr);
	REG32_UPDATE_BITS(addr, mask, val);
	prvPwmExitCritical(uxSavedIsr);
}

static xPwmMesonRegs_t *prvDeviceToRegs(xPwmMesondevice_t *pwm)
{
	return (xPwmMesonRegs_t *) pwm->chip->addr;
}

static uint32_t vPwmMesonVolttoDuty(xPwmMesonVoltage_t *vtable,
				 uint32_t vtable_size,
				 uint32_t voltage_mv)
{
	uint32_t i;

	if ((voltage_mv < vtable[0].Voltage_mv) ||
	    (voltage_mv > vtable[vtable_size - 1].Voltage_mv)) {
		iprintf("volt: %dmv out of set range [%dmv - %dmv]\n",
			voltage_mv, vtable[0].Voltage_mv, vtable[vtable_size - 1].Voltage_mv);
		return 0;
	}

	/* Get voltage up */
	for (i = 0; i < vtable_size; i++)
		if (voltage_mv <= vtable[i].Voltage_mv)
			return vtable[i].Duty_reg;

	return 0;
}

static int32_t vPwmMesonDutytoVolt(xPwmMesonVoltage_t *vtable,
				 uint32_t vtable_size,
				 uint32_t duty)
{
	uint32_t i;

	for (i = 0; i < vtable_size; i++)
		if (duty == vtable[i].Duty_reg)
			return vtable[i].Voltage_mv;

	return -1;
}

void vPwmMesonPwmDebug(xPwmMesondevice_t *pwm)
{
	uint32_t tmp = 0;
	uint32_t i;
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	iprintf("pwm debug info chip_id = %d channel_id = %d addr = %p\n",
		pwm->chip->chip_id, pwm->hwpwm, reg);

	for (i = 0; i <= 7; i++) {
		tmp = pwm_readl(&reg->dar + i);
		iprintf("reg-> = 0x%x\n", tmp);
	}
}

void vPwmConstantDisable(xPwmMesondevice_t *pwm)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
	case MESON_PWM_2:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 28), 0);
		break;

	case MESON_PWM_1:
	case MESON_PWM_3:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 29), 0);
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

void vPwmConstantEnable(xPwmMesondevice_t *pwm)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
	case MESON_PWM_2:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 28), (1 << 28));
		break;

	case MESON_PWM_1:
	case MESON_PWM_3:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 29), (1 << 29));
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

static uint32_t prvPwmGetPolarity(xPwmMesondevice_t *pwm)
{
	uint32_t tmp, val;
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
	case MESON_PWM_2:
		val = 0x1 << 26;
		break;

	case MESON_PWM_1:
	case MESON_PWM_3:
		val = 0x1 << 27;
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		return 0;
	}

	tmp = pwm_readl(&reg->miscr);
	tmp = tmp & val;
	if (tmp == 0)
		return 0;
	else
		return 1;
}

static void prvPwmMesonClockSet(xPwmMesondevice_t *pwm)
{
	switch (pwm->hwpwm) {
	case MESON_PWM_0:
	case MESON_PWM_2:
		prvPwmRegWrite(pwm->chip->clk_addr, ((0x3 << 9) | (0x1 << 8)), (0x1 << 8));
		break;

	case MESON_PWM_1:
	case MESON_PWM_3:
		prvPwmRegWrite(pwm->chip->clk_addr, ((0x3 << 25) | (0x1 << 24)), (0x1 << 24));
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

static int32_t prvPwmCalc(xPwmMesondevice_t *pwm, uint32_t duty, uint32_t period)
{
	uint32_t pre_div, cnt, duty_cnt;
	uint32_t fin_freq, fin_ns;
	int32_t inv;

	inv = prvPwmGetPolarity(pwm);
	if (inv)
		duty = period - duty;

	/*fin_freq = CLK_24M; */
	fin_freq = 24000000;
	fin_ns = USEC_PER_SEC / fin_freq;

	for (pre_div = 0; pre_div < 0x7f; pre_div++) {
		cnt = DIV_ROUND_CLOSEST(period, fin_ns * (pre_div + 1));
		if (cnt <= 0xffff)
			break;
	}

	if (pre_div == 0x7f) {
		iprintf("unable to get period pre_div\n");
		return -1;
	}

	if (duty == period) {
		pwm->pwm_pre_div = pre_div;
		pwm->pwm_hi = cnt - 2;
		pwm->pwm_lo = 0;
		vPwmConstantEnable(pwm);
	} else if (duty == 0) {
		pwm->pwm_pre_div = pre_div;
		pwm->pwm_hi = 0;
		pwm->pwm_lo = cnt - 2;
		vPwmConstantEnable(pwm);
	} else {
		/* Then check is we can have the duty with the same pre_div */
		duty_cnt = DIV_ROUND_CLOSEST(duty, fin_ns * (pre_div + 1));
		if (duty_cnt > 0xffff) {
			iprintf("unable to get duty cycle\n");
			return -1;
		}

		pwm->pwm_pre_div = pre_div;

		if (duty_cnt == 0) {
			cnt = (cnt < 2 ? 2 : cnt);
			pwm->pwm_hi = 0;
			pwm->pwm_lo = cnt - 2;
		} else if (cnt == duty_cnt) {
			duty_cnt = (duty_cnt < 2 ? 2 : duty_cnt);
			pwm->pwm_hi = duty_cnt - 2;
			pwm->pwm_lo = 0;
		} else {
			pwm->pwm_hi = duty_cnt - 1;
			pwm->pwm_lo = cnt - duty_cnt - 1;
		}
		vPwmConstantDisable(pwm);
	}

	return 0;
}

static void prvMesonConfig(xPwmMesondevice_t *pwm)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
		/*set div and clock enable */
		if (pwm->chip->clk_addr) {
			/* If using clktree */
			prvPwmRegWrite(pwm->chip->clk_addr, ((0xff << 0) | (1 << 8)), ((pwm->pwm_pre_div << 0) | (1 << 8)));
		} else {
			prvPwmRegWrite((uint32_t)&reg->miscr, ((0x3 << 4) | (0x7f << 8) | (1 << 15)), (((pwm->pwm_pre_div << 8)) | (1 << 15)));
		}

		/*set duty */
		prvPwmRegWrite((uint32_t)&reg->dar, 0xffffffff, ((pwm->pwm_hi << 16) | (pwm->pwm_lo)));
		break;

	case MESON_PWM_1:
		/*set div and clock enable */
		if (pwm->chip->clk_addr) {
			prvPwmRegWrite(pwm->chip->clk_addr, ((0xff << 16) | (1 << 24)), ((pwm->pwm_pre_div << 16) | (1 << 24)));
		} else {
			prvPwmRegWrite((uint32_t)&reg->miscr, ((0x3 << 6) | (0x7f << 16) | (1 << 23)),  ((pwm->pwm_pre_div << 16) | (1 << 23)));
		}

		/*set duty */
		prvPwmRegWrite((uint32_t)&reg->dbr, 0xffffffff, ((pwm->pwm_hi << 16) | (pwm->pwm_lo)));
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

static void prvMesonConfigExt(xPwmMesondevice_t *pwm)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	switch (pwm->hwpwm) {
	case MESON_PWM_2:
		/*set div and clock enable */
		if (pwm->chip->clk_addr)
			prvPwmRegWrite(pwm->chip->clk_addr, ((0xff << 0) | (1 << 8)), ((pwm->pwm_pre_div) | (1 << 8)));
		else
			prvPwmRegWrite((uint32_t)&reg->miscr, ((0x3 << 4) | (0x7f << 8) | (1 << 15)), ((pwm->pwm_pre_div << 8) | (1 << 15)));
		/*set duty */
		prvPwmRegWrite((uint32_t)&reg->da2r, 0xffffffff, ((pwm->pwm_hi << 16) | (pwm->pwm_lo)));
		break;

	case MESON_PWM_3:
		/*set div and clock enable */
		if (pwm->chip->clk_addr)
			prvPwmRegWrite(pwm->chip->clk_addr, ((0xff << 16) | (1 << 24)), ((pwm->pwm_pre_div << 16) | (1 << 24)));
		else
			prvPwmRegWrite((uint32_t)&reg->miscr, ((0x3 << 6) | (0x7f << 16) | (1 << 23)), ((pwm->pwm_pre_div << 16) | (1 << 23)));
		/*set duty */
		prvPwmRegWrite((uint32_t)&reg->db2r, 0xffffffff, ((pwm->pwm_hi << 16) | (pwm->pwm_lo)));
		break;
	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

int32_t xPwmMesonConfig(xPwmMesondevice_t *pwm, uint32_t duty_ns,
			uint32_t period_ns)
{
	int32_t tmp;

	if ((duty_ns > period_ns) || (!period_ns)) {
		iprintf("Not available duty_ns period_ns !\n");
		return -1;
	}

	/* If using clktree */
	if (pwm->chip->clk_addr)
		prvPwmMesonClockSet(pwm);
	tmp = prvPwmCalc(pwm, duty_ns, period_ns);
	if (tmp != 0) {
		iprintf("calc pwm freq err error");
	}

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
	case MESON_PWM_1:
		prvMesonConfig(pwm);
		break;

	case MESON_PWM_2:
	case MESON_PWM_3:
		prvMesonConfigExt(pwm);
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}

	return 0;
}

void vPwmMesonDisable(xPwmMesondevice_t *pwm)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 0), 0);
		break;

	case MESON_PWM_1:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 1), 0);
		break;

	case MESON_PWM_2:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 25), 0);
		break;

	case MESON_PWM_3:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 24), 0);
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
	return;
}

/**
 * pwm_meson_enable() - enable pwm output
 * @pwm: pwm channel to choose
 */
void vPwmMesonEnable(xPwmMesondevice_t *pwm)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 0), (1 << 0));
		break;

	case MESON_PWM_1:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 1), (1 << 1));
		break;

	case MESON_PWM_2:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 25), (1 << 25));
		break;

	case MESON_PWM_3:
		prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 24), (1 << 24));
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

void vPwmMesonSetTimes(xPwmMesondevice_t *pwm, uint32_t times)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	times--;
	switch (pwm->hwpwm) {
	case MESON_PWM_0:
		prvPwmRegWrite((uint32_t)&reg->tr, (0xff << 24), (times << 24));
		break;

	case MESON_PWM_1:
		prvPwmRegWrite((uint32_t)&reg->tr, (0xff << 8), (times << 8));
		break;

	case MESON_PWM_2:
		prvPwmRegWrite((uint32_t)&reg->tr, (0xff << 16), (times << 16));
		break;

	case MESON_PWM_3:
		prvPwmRegWrite((uint32_t)&reg->tr, (0xff << 0), (times << 0));
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

void vPwmMesonSetBlinkTimes(xPwmMesondevice_t *pwm, uint32_t times)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	times--;
	switch (pwm->hwpwm) {
	case MESON_PWM_0:
		prvPwmRegWrite((uint32_t)&reg->br, 0xf, times);
		break;

	case MESON_PWM_1:
		prvPwmRegWrite((uint32_t)&reg->br, (0xf << 4), (times << 4));
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

void vPwmMesonBlinkEnable(xPwmMesondevice_t *pwm)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
		prvPwmRegWrite((uint32_t)&reg->br, (1 << 8), (1 << 8));
		break;

	case MESON_PWM_1:
		prvPwmRegWrite((uint32_t)&reg->br, (1 << 9), (1 << 9));
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

void vPwmMesonBlinkDisable(xPwmMesondevice_t *pwm)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
		prvPwmRegWrite((uint32_t)&reg->br, (1 << 8), (0 << 8));
		break;

	case MESON_PWM_1:
		prvPwmRegWrite((uint32_t)&reg->br, (1 << 9), (0 << 9));
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

int32_t xPwmMesonIsBlinkComplete(xPwmMesondevice_t *pwm)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);
	uint32_t a1, val;

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
		val = 0x1 << 0;
		break;

	case MESON_PWM_1:
		val = 0x1 << 1;
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		return 0;
	}

	a1 = pwm_readl(&reg->miscr);
	a1 = a1 & (val);
	if (a1 == 0)
		return 1;
	else
		return 0;
}

void vPwmMesonSetPolarity(xPwmMesondevice_t *pwm, uint32_t val)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
		if (val)
			prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 26), (1 << 26));
		else
			prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 26), 0);
		break;

	case MESON_PWM_1:
		if (val)
			prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 27), (1 << 27));
		else
			prvPwmRegWrite((uint32_t)&reg->miscr, (1 << 27), 0);
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

void vPwmMesonClear(xPwmMesondevice_t *pwm)
{
	xPwmMesonRegs_t *reg = prvDeviceToRegs(pwm);

	switch (pwm->hwpwm) {
	case MESON_PWM_0:
		prvPwmRegWrite((uint32_t)&reg->miscr, ((0xff << 8) | (1 << 25) | (3 << 4)), 0);
		prvPwmRegWrite((uint32_t)&reg->br, ((0xf) | (1 << 8)), 0);
		break;

	case MESON_PWM_1:
		prvPwmRegWrite((uint32_t)&reg->miscr, ((0xff << 16) | (1 << 24) | (0x3 << 6)), 0);
		prvPwmRegWrite((uint32_t)&reg->br, ((0xf << 4) | (1 << 9)), 0);
		break;

	default:
		iprintf("%s Id:%d is invalid!\n", __func__, pwm->hwpwm);
		break;
	}
}

/**
 * xPwmMesonChannelApply() - apply pwm channel
 * @chip_id: pwm controller to choose,like PWM_AB ,PWM_CD, PWM_EF
 * PWM_GH,PWM_IJ
 * @channel_id: pwm channel to choose,like MESON_PWM_0,
 * MESON_PWM_1 MESON_PWM_2 MESON_PWM_3
 */
xPwmMesondevice_t *xPwmMesonChannelApply(uint32_t chip_id, uint32_t channel_id)
{
	xPwmMesondevice_t *pwm;
	xPwmMesonChip_t *chip;

	if (chip_id >= PWM_MUX) {
		iprintf("pwm chip id is invalid!\n");
		return NULL;
	}

	if (channel_id > MESON_PWM_3) {
		iprintf("pwm channel id is invalid!\n");
		return NULL;
	}

	chip = prvIdToPwmChip(chip_id);
	if (!chip) {
		iprintf("can not get pwm Controller!\n");
		return NULL;
	}

	if (chip->mask & (1 << channel_id)) {
		iprintf("pwm channel is applied already!\n");
		return NULL;
	}

	pwm = (xPwmMesondevice_t *)pvPortMalloc(sizeof(*pwm));
	if (!pwm) {
		iprintf("pwm channel malloc fail!\n");
		return NULL;
	}

	pwm->chip = chip;
	pwm->hwpwm = channel_id;
	chip->mask |= (1 << channel_id);

	return pwm;
}

/**
 * vPwmMesonChannelFree() - free pwm channel
 * @pwm: pwm channel to choose,return by pwm_channel_apply
 */
void vPwmMesonChannelFree(xPwmMesondevice_t *pwm)
{
	xPwmMesonChip_t *chip = pwm->chip;

	if (!(chip->mask & (1 << pwm->hwpwm))) {
		iprintf("pwm channel is free already\n");
		return;
	}

	chip->mask &= ~(1 << pwm->hwpwm);
	vPortFree(pwm);
}

int32_t vPwmMesonSetVoltage(uint32_t voltage_id, uint32_t voltage_mv)
{
	xPwmMesondevice_t *pwm;
	xPwmMesonVoltage_t *vtable;
#if PwmMesonVolt_Duty
	xPwmMesonRegs_t *reg;
#endif
	uint32_t chip_id, channel_id, duty, vtable_size, max_value, min_value;

	chip_id = prvMesonVoltToPwmChip(voltage_id);
	if (chip_id >= PWM_MUX) {
		iprintf("volt id:%d get chip id fail!\n", voltage_id);
		return -1;
	}

	channel_id = prvMesonVoltToPwmChannel(voltage_id);
	if (channel_id >= MESON_PWM_2) {
		iprintf("volt id:%d get channel id fail!\n", voltage_id);
		return -1;
	}

	vtable = vPwmMesonGetVoltTable(voltage_id);
	if (!vtable) {
		iprintf("volt id:%d pwm get volt table fail!\n", voltage_id);
		return -1;
	}

	vtable_size = vPwmMesonGetVoltTableSize(voltage_id);
	if (!vtable_size) {
		iprintf("volt id:%d pwm get volt table size fail!\n", voltage_id);
		return -1;
	}

	duty = vPwmMesonVolttoDuty(vtable, vtable_size, voltage_mv);
	if (!duty) {
		iprintf("volt id: %d, volt: %d get duty fail!\n", voltage_id, voltage_mv);
		return -1;
	}

	min_value = vtable[0].Voltage_mv;
	max_value = vtable[vtable_size-1].Voltage_mv;

	pwm = xPwmMesonChannelApply(chip_id, channel_id);
	if (!pwm) {
		iprintf("volt id:%d pwm device apply fail!\n", voltage_id);
		return -1;
	}

#if PwmMesonVolt_Duty
	/* only update duty reg */
	reg = prvDeviceToRegs(pwm);
	if (channel_id == MESON_PWM_0) {
		prvPwmRegWrite((uint32_t)&reg->dar, 0xffffffff, duty);
		/*Vddee outputs the maximum or minimum voltage, pwm outputs all high or all low, the
		 corresponding register should be cleared*/
		if ((voltage_mv == max_value) || (voltage_mv == min_value))
			prvPwmRegWrite((uint32_t)&reg->miscr, (1<<28), (1<<28));
		else
			prvPwmRegWrite((uint32_t)&reg->miscr, (1<<28), (0<<28));
	} else {
		prvPwmRegWrite((uint32_t)&reg->dbr, 0xffffffff, duty);
		if ((voltage_mv == max_value) || (voltage_mv == min_value))
			prvPwmRegWrite((uint32_t)&reg->miscr, (1<<29), (1<<29));
		else
			prvPwmRegWrite((uint32_t)&reg->miscr, (1<<29), (0<<29));
	}
#else
	pwm->pwm_hi = duty >> 16;
	pwm->pwm_lo = duty & 0xFFFF;
	prvMesonConfig(pwm);
	vPwmMesonEnable(pwm);
#endif
	vPwmMesonChannelFree(pwm);

	return 0;
}

int32_t vPwmMesonGetVoltage(uint32_t voltage_id)
{
	xPwmMesondevice_t *pwm;
	xPwmMesonVoltage_t *vtable;
	xPwmMesonRegs_t *reg;
	uint32_t chip_id, channel_id, duty, vtable_size;
	int32_t voltage_mv;

	chip_id = prvMesonVoltToPwmChip(voltage_id);
	if (chip_id >= PWM_MUX) {
		iprintf("volt id:%d get chip id fail!\n", voltage_id);
		return -1;
	}

	channel_id = prvMesonVoltToPwmChannel(voltage_id);
	if (channel_id >= MESON_PWM_2) {
		iprintf("volt id:%d get channel id fail!\n", voltage_id);
		return -1;
	}

	vtable = vPwmMesonGetVoltTable(voltage_id);
	if (!vtable) {
		iprintf("volt id:%d pwm get volt table fail!\n", voltage_id);
		return -1;
	}

	vtable_size = vPwmMesonGetVoltTableSize(voltage_id);
	if (!vtable_size) {
		iprintf("volt id:%d pwm get volt table size fail!\n", voltage_id);
		return -1;
	}

	pwm = xPwmMesonChannelApply(chip_id, channel_id);
	if (!pwm) {
		iprintf("volt id:%d pwm device apply fail!\n", voltage_id);
		return -1;
	}

	reg = prvDeviceToRegs(pwm);
	if (channel_id == MESON_PWM_0)
		duty = pwm_readl(&reg->dar);
	else
		duty = pwm_readl(&reg->dbr);

	vPwmMesonChannelFree(pwm);

	voltage_mv = vPwmMesonDutytoVolt(vtable, vtable_size, duty);
	if (voltage_mv < 0) {
		iprintf("volt id: %d, duty: 0x%x get voltage fail!\n", voltage_id, duty);
		return -1;
	}

	return voltage_mv;
}

