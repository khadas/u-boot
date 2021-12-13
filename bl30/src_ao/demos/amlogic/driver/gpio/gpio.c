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
 * gpio driver
 */
#include "FreeRTOS.h"
#include <register.h>
#include <common.h>
#include <task.h>
#include <gpio.h>
#include "util.h"
#include "projdefs.h"
#include "gpio_drv.h"
#include "portmacro.h"

#define DRIVER_NAME "gpio"

extern unsigned int xPortIsIsrContext(void);
static void prvEnterCritical(UBaseType_t *uxIsr)
{
	if (xPortIsIsrContext())
		*uxIsr = taskENTER_CRITICAL_FROM_ISR();
	else {
		taskENTER_CRITICAL();
		*uxIsr = 0;
	}
};

static void prvExitCritical(UBaseType_t uxSaveIsr)
{
	if (xPortIsIsrContext())
		taskEXIT_CRITICAL_FROM_ISR(uxSaveIsr);
	else {
		taskEXIT_CRITICAL();
		(void)uxSaveIsr;
	}
};

static inline void prvGpioRegWrite(uint32_t addr,
				   uint32_t mask, uint32_t val)
{
	UBaseType_t uxSavedIsr;

	prvEnterCritical(&uxSavedIsr);
	REG32_UPDATE_BITS(addr, mask, val);
	prvExitCritical(uxSavedIsr);
}

static inline void prvGpioCalcRegAndBit(const GpioBank_t *bk,
					uint8_t offset,
					enum GpioRegType regType,
					uint16_t *reg, uint8_t *bit)
{
	*reg = bk->regs[regType].reg << 2;
	*bit = bk->regs[regType].bit + offset;
}

static inline void prvGpioGetBankAndOffset(uint16_t gpio,
					   const GpioBank_t **bk,
					   uint8_t *off)
{
	const GpioBank_t *gpk;

	gpk = pGetGpioBank();
	*bk = &gpk[gpio >> 5];
	*off = gpio % 32;
}

int xGpioSetDir(uint16_t gpio, enum GpioDirType dir)
{
	uint16_t reg;
	uint8_t bit;
	uint8_t offset;
	const GpioBank_t *bk;

	if (dir >= GPIO_DIR_INVALID) {
		iprintf("%s: invalid DIR [OUT=0, IN=1]: %d\n", DRIVER_NAME,
			dir);
		return -pdFREERTOS_ERRNO_EINVAL;
	}

	prvGpioGetBankAndOffset(gpio, &bk, &offset);

	prvGpioCalcRegAndBit(bk, offset, REG_DIR, &reg, &bit);

	prvGpioRegWrite(bk->domain->rGpio + reg, BIT(bit), dir ? BIT(bit) : 0);

	return 0;
}

int xGpioSetValue(uint16_t gpio, enum GpioOutLevelType level)
{
	uint16_t reg;
	uint8_t bit;
	uint8_t offset;
	const GpioBank_t *bk;

	if (level >= GPIO_LEVEL_INVALID) {
		iprintf("%s: invalid output level [LOW=1, HIGH=0]: %d\n",
			DRIVER_NAME, level);
		return -pdFREERTOS_ERRNO_EINVAL;
	}

	prvGpioGetBankAndOffset(gpio, &bk, &offset);

	prvGpioCalcRegAndBit(bk, offset, REG_OUT, &reg, &bit);

	prvGpioRegWrite(bk->domain->rGpio + reg, BIT(bit),
			level ? BIT(bit) : 0);

	return 0;
}

int xGpioGetValue(uint16_t gpio)
{
	uint16_t reg;
	uint8_t bit;
	uint8_t offset;
	const GpioBank_t *bk;

	prvGpioGetBankAndOffset(gpio, &bk, &offset);

	prvGpioCalcRegAndBit(bk, offset, REG_IN, &reg, &bit);

	return !!(REG32((unsigned long)bk->domain->rGpio + reg) & BIT(bit));
}

int xPinconfSet(uint16_t gpio, uint32_t flags)
{
	uint16_t reg;
	uint8_t bit;
	uint8_t offset;
	uint8_t ds_value = 0;
	const GpioBank_t *bk;
	const GpioRegDesc_t *desc;

	prvGpioGetBankAndOffset(gpio, &bk, &offset);

	prvGpioCalcRegAndBit(bk, offset, REG_PULLEN, &reg, &bit);

	if (flags & PINF_CONFIG_BIAS_DISABLE) {
		prvGpioRegWrite(bk->domain->rPullen + reg, BIT(bit), 0);
	} else if (flags &
		   (PINF_CONFIG_BIAS_PULL_UP | PINF_CONFIG_BIAS_PULL_DOWN)) {
		prvGpioRegWrite(bk->domain->rPullen + reg, BIT(bit), BIT(bit));

		prvGpioCalcRegAndBit(bk, offset, REG_PULL, &reg, &bit);
		if (flags & PINF_CONFIG_BIAS_PULL_UP)
			prvGpioRegWrite(bk->domain->rPull + reg, BIT(bit),
					BIT(bit));
		else
			prvGpioRegWrite(bk->domain->rPull + reg, BIT(bit), 0);
	}

	if ((flags & PINF_CONFIG_DRV_MASK) && &bk->regs[REG_DRV] != 0) {

		prvGpioCalcRegAndBit(bk, offset, REG_DRV, &reg, &bit);
		desc = &bk->regs[REG_DRV];

		reg = desc->reg + ((desc->bit + (offset << 1)) >> 5);
		bit = (desc->bit + (offset << 1)) % 32;

		if (flags & PINF_CONFIG_DRV_STRENGTH_0)
			ds_value = 0;
		else if (flags & PINF_CONFIG_DRV_STRENGTH_1)
			ds_value = 1;
		else if (flags & PINF_CONFIG_DRV_STRENGTH_2)
			ds_value = 2;
		else if (flags & PINF_CONFIG_DRV_STRENGTH_3)
			ds_value = 3;

		prvGpioRegWrite(bk->domain->rDrv + (reg << 2), 0x3 << bit,
				ds_value << bit);
	}

	return 0;
}

int xPinmuxSet(uint16_t gpio, enum PinMuxType func)
{
	uint16_t reg;
	uint8_t bit;
	uint8_t offset;
	const GpioBank_t *bk;
	const GpioRegDesc_t *desc;

	if (func >= PIN_FUNC_INVALID) {
		iprintf("%s: invalid pin Function [0 - %d]: %d\n",
			DRIVER_NAME, PIN_FUNC_INVALID - 1, func);
		return -pdFREERTOS_ERRNO_EINVAL;
	}

	prvGpioGetBankAndOffset(gpio, &bk, &offset);
	desc = &bk->regs[REG_MUX];

	reg = desc->reg + ((desc->bit + (offset << 2)) >> 5);
	bit = (desc->bit + (offset << 2)) % 32;

	prvGpioRegWrite(bk->domain->rMux + (reg << 2), 0xf << bit, func << bit);

	return 0;
}
