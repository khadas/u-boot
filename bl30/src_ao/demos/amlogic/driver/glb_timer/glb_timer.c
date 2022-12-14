/*
 * Copyright (C) 2014-2021 Amlogic, Inc. All rights reserved.
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
#include <common.h>
//#include <printf.h>
#include <stdint.h>
#include <task.h>
#include <gpio.h>
#include "util.h"
#include "projdefs.h"
#include "portmacro.h"
#include "glb_timer.h"
#include "global_timer.h"
//#include "hwspinlock_new.h"

#ifdef HWSPINLOCK_ID_GLB_TIMER
static  hwspinlock_lock_t *hwlock;
#endif

static inline void prvGlbTimerUpdateBit(uint32_t addr, uint32_t mask,
					uint32_t val)
{
	REG32_UPDATE_BITS(addr, mask, val);
}

static inline int prvGlbTimerRead(uint32_t addr)
{
	return REG32(addr);
}

static inline void prvGlbTimerWrite(uint32_t addr, uint32_t ulvalue)
{
	REG32((unsigned long)(addr)) = ulvalue;
}

static inline void vGlbTimerTopCtlReset(void)
{
	prvGlbTimerUpdateBit(TOP_CTRL_BASE + TOP_CTRL0, BIT(2), BIT(2));
	prvGlbTimerUpdateBit(TOP_CTRL_BASE + TOP_CTRL0, BIT(2), 0);
}

/******************************get counter function***************************/
static inline void vGlbTimerTopCtlClear(void)
{
	prvGlbTimerUpdateBit(TOP_CTRL_BASE + TOP_CTRL0, BIT(3), BIT(3));
}

static inline void vGlbTimerTopCtlHold(void)
{
	prvGlbTimerUpdateBit(TOP_CTRL_BASE + TOP_CTRL0, BIT(1), BIT(1));
}

static inline uint64_t prvGlbTimerGetCounter(void)
{
	uint32_t ulTS_L;
	uint32_t ulTS_H;
	uint64_t ullTS;

	ulTS_L = prvGlbTimerRead(TOP_CTRL_BASE + TOP_TS0);
	ulTS_H = prvGlbTimerRead(TOP_CTRL_BASE + TOP_TS1);

	ullTS = ulTS_H;

	return (ullTS << 32) | ulTS_L;
}

/*
 * ullGlobalTimerGlobalSnapshot
 * return the global timer 64 bit snapshot value
 */
uint64_t ullGlobalTimerGlobalSnapshot(void)
{
	uint64_t ts;
#ifdef  HWSPINLOCK_ID_GLB_TIMER
	if (hwlock) {
		vHwspinLock_get(hwlock, HWSPINLOCK_ID_GLB_TIMER);
		ts = prvGlbTimerGetCounter();
		vHwspinLock_release(hwlock, HWSPINLOCK_ID_GLB_TIMER);
	} else {
		ts = prvGlbTimerGetCounter();
	}
#else
		ts = prvGlbTimerGetCounter();
#endif

	return ts;
}

void vGlobalTimerReset(void)
{
	vGlbTimerTopCtlReset();
}

/******************************input function*********************************/
static inline void prvGlbTimerTrigSrcEnable(uint8_t ucSrc, uint8_t on)
{
	on = on;
	prvGlbTimerUpdateBit(TRIG_SRC_BASE + SRC_OFFSET(TRIG_SRC0_CTRL0, ucSrc),
			     BIT(31), BIT(31));
}

static inline void prvGlbTimerTrigSrcConfig(uint8_t ucSrc,
					enum MesonGlbSrcSelFlag type)
{
	uint32_t ulmask;
	uint32_t ulvalue;

	ulmask = GENMASK(31, 29) | GENMASK(17, 16);
	ulvalue = GENMASK(31, 30) |
		  ((type & GENMASK(1, 0)) << 16) |
		  ((type & BIT(2)) << 29);

	prvGlbTimerUpdateBit(TRIG_SRC_BASE + SRC_OFFSET(TRIG_SRC0_CTRL0, ucSrc),
			     ulmask, ulvalue);
}

static inline void prvGlbTimerTrigSrcOneShot(uint8_t ucSrc)
{
	prvGlbTimerUpdateBit(TRIG_SRC_BASE + SRC_OFFSET(TRIG_SRC0_CTRL0, ucSrc),
			     BIT(1), BIT(1));
}

static inline void prvGlbTimerTrigSrcSWTrig(uint8_t ucSrc)
{
	prvGlbTimerUpdateBit(TRIG_SRC_BASE + SRC_OFFSET(TRIG_SRC0_CTRL0, ucSrc),
			     BIT(0), BIT(0));
}

static inline uint64_t prvGlbTimerTrigSrcGetCount(uint8_t ucSrc)
{
	uint32_t ulTS_L;
	uint32_t ulTS_H;
	uint64_t ullTS;

	ulTS_L = prvGlbTimerRead(TRIG_SRC_BASE + SRC_OFFSET(TRIG_SRC0_TS_L,
				 ucSrc));
	ulTS_H = prvGlbTimerRead(TRIG_SRC_BASE + SRC_OFFSET(TRIG_SRC0_TS_H,
				 ucSrc));

	ullTS = ulTS_H;

	return (ullTS << 32) | ulTS_L;
}

/*
 * ulGlobalTimerInputGPIOConfigure
 * @ucSrc: Triger Source can be found in
 * lib/third_party/amlogic/include/drivers/global_timer.h note that this number
 * and GPIO
 * line number should keep mapping 0-31 <--> GLBT_GPIO0_IRQ -- GLBT_GPIO31_IRQ
 * @MesonGlbSrcSelFlag: Trigger type can be found in
 * lib/third_party/amlogic/include/drivers/global_timer.h
 * @returns: return 0 for success or -EINVAL fail.
 */
int uGlobalTimerInputGPIOConfigure(uint8_t ucSrc, enum MesonGlbSrcSelFlag type)
{
	if (type > TRIG_ONE_SHOT)
		return -EINVAL;

	if (ucSrc < GLBT_GPIO0_IRQ || ucSrc > GLBT_GPIO31_IRQ)
		return -EINVAL;

	prvGlbTimerTrigSrcConfig(ucSrc, type);
	return 0;
}


/*
 * ullGlobalTimerInputGPIOGetSnapshot
 * @ucSrc: Triger Source can be found in
 * lib/third_party/amlogic/include/drivers/global_timer.h note that this
 * number and GPIO
 * line number should keep mapping 0-31 <--> GLBT_GPIO0_IRQ -- GLBT_GPIO31_IRQ
 * @retruns:the global timer 64 bit snapshot value
 */
uint64_t ullGlobalTimerInputGPIOGetSnapshot(uint8_t ucSrc)
{
	return prvGlbTimerTrigSrcGetCount(ucSrc);
}

/******************************output function*********************************/
static inline void prvGlbTimerOutputEnable(uint8_t ucSrc, uint8_t ucOn)
{
	prvGlbTimerUpdateBit(OUTPUT_BASE + SRC_OFFSET(OUTPUT_CTRL0, ucSrc),
			     BIT(31), ucOn << 31);
}

static inline void prvGlbTimerOutputSWStart(uint8_t ucSrc)
{
	prvGlbTimerUpdateBit(OUTPUT_BASE + SRC_OFFSET(OUTPUT_CTRL0, ucSrc),
			     BIT(1), BIT(1));
}

static inline void prvGlbTimerOutputSWStop(uint8_t ucSrc)
{
	prvGlbTimerUpdateBit(OUTPUT_BASE + SRC_OFFSET(OUTPUT_CTRL0, ucSrc),
			     BIT(0), BIT(0));
}

static inline void prvGlbTimerOutputForceOutput(uint8_t ucSrc, uint8_t level)
{
	prvGlbTimerUpdateBit(OUTPUT_BASE + SRC_OFFSET(OUTPUT_CTRL0, ucSrc),
			     GENMASK(17, 16), BIT(17) | (level << 16));
	prvGlbTimerUpdateBit(OUTPUT_BASE + SRC_OFFSET(OUTPUT_CTRL0, ucSrc),
			     GENMASK(19, 18), BIT(19));
}

static inline void prvGlbTimerOutputForceInput(uint8_t ucSrc)
{
	prvGlbTimerUpdateBit(OUTPUT_BASE + SRC_OFFSET(OUTPUT_CTRL0, ucSrc),
			     GENMASK(19, 18), GENMASK(19, 18));
}

static inline void prvGlbTimerOutputPulseConfig(uint8_t ucSrc,
						uint8_t ucPulsePolarity,
						uint32_t ulPulseWith,
						uint32_t ulPulseInterval,
						uint32_t ulPulseNum)
{
	if (ucPulsePolarity)
		prvGlbTimerUpdateBit(OUTPUT_BASE +
				     SRC_OFFSET(OUTPUT_CTRL0, ucSrc),
				     BIT(28), BIT(28));

	prvGlbTimerWrite(OUTPUT_BASE + SRC_OFFSET(OUTPUT_PULSE_WDITH, ucSrc),
			 ulPulseWith);
	prvGlbTimerWrite(OUTPUT_BASE + SRC_OFFSET(OUTPUT_INTERVAL, ucSrc),
			ulPulseInterval);
	prvGlbTimerWrite(OUTPUT_BASE + SRC_OFFSET(OUTPUT_STOP_NUM, ucSrc),
			ulPulseNum);
}

static inline void prvGlbTimerOutputSetExpire(uint8_t ucSrc, uint64_t ts)
{
	uint32_t ts_l;
	uint32_t ts_h;

	ts_l = ts & GENMASK(31, 0);
	ts_h = (ts >> 32) & GENMASK(31, 0);

	prvGlbTimerWrite(OUTPUT_BASE + SRC_OFFSET(OUTPUT_EXPIRES_TS_L, ucSrc),
			 ts_l);
	prvGlbTimerWrite(OUTPUT_BASE + SRC_OFFSET(OUTPUT_EXPIRES_TS_H, ucSrc),
			 ts_h);
}

/* OUTPUT Source Select routing gen_n to rt_gpio_n */
static inline void prvGlbTimerSetOutputSrc(uint8_t ucSrc, uint8_t gen)
{
	prvGlbTimerUpdateBit(OUTPUT_SEL_BASE + (ucSrc << 2),
			     BIT(31) | GENMASK(3, 0),
			     BIT(31) | gen);
}

/*
 * uGlobalTimerOutputGPIOSetup
 * @ucSrc: Select output source, can be found in
 * lib/third_party/amlogic/include/drivers/global_timer.h
 * @ucOneShot: if true then one shot pulse or continuous macro can be found
 * in /lib/third_party/amlogic/include/driver/global_timer.h
 * @uPulseWith: pulse with in global timer clock cycles
 * @uInterval: pulse interval in global timer cycles
 * @ucInitval: decides the initial value of the gpio state and the pulse
 * would be inverse of that init value
 * @retruns: 0 on success and error code on failure
 */
int uGlobalTimerOutputGPIOSetup(uint8_t ucSrc, uint8_t ucOneshot,
				uint32_t uPulseWith, uint32_t uInterval,
				uint8_t ucInitVal)
{
	uint32_t ureg;

	if (ucSrc > GLBT_OUTPUT_GPIO15)
		return -EINVAL;

	/* reset generator */
	for (ureg = 0; ureg < (OUTPUT_ST1 >> 2); ureg++)
		prvGlbTimerUpdateBit(OUTPUT_BASE + SRC_OFFSET(ureg << 2, ucSrc),
				     BIT(0), BIT(0));

	/* init val */
	if (ucInitVal)
		prvGlbTimerUpdateBit(OUTPUT_BASE +
				     SRC_OFFSET(OUTPUT_CTRL0, ucSrc), BIT(28),
				     BIT(28));

	/* config pulse type */
	prvGlbTimerWrite(OUTPUT_BASE + SRC_OFFSET(OUTPUT_PULSE_WDITH, ucSrc),
			 uPulseWith);
	prvGlbTimerWrite(OUTPUT_BASE + SRC_OFFSET(OUTPUT_INTERVAL, ucSrc),
			 uInterval);
	prvGlbTimerWrite(OUTPUT_BASE + SRC_OFFSET(OUTPUT_STOP_NUM, ucSrc), 0);

	if (ucOneshot)
		prvGlbTimerUpdateBit(OUTPUT_BASE +
				     SRC_OFFSET(OUTPUT_CTRL0, ucSrc),
				     BIT(3), BIT(3));
	else
		prvGlbTimerUpdateBit(OUTPUT_BASE +
				     SRC_OFFSET(OUTPUT_CTRL0, ucSrc),
				     BIT(2), BIT(2));
	return 0;
}


/*
 * uGlobalTimerOutputStart
 * @ucSrcn: Select output source, must same as setup api can be found in
 * lib/third_party/amlogic/include/drivers/global_timer.h
 * @ullexpires: the 64 bit future expires value for when the pulse must start
 * @returns: 0 for success or -EINVAL for fail
 */
int uGlobalTimerOutputStart(uint8_t ucSrcn, uint64_t ullexpires)
{
	if (ucSrcn > GLBT_OUTPUT_GPIO15)
		return -EINVAL;

	prvGlbTimerSetOutputSrc(ucSrcn, ucSrcn);
	prvGlbTimerOutputSetExpire(ucSrcn, ullexpires);
	prvGlbTimerOutputEnable(ucSrcn, 1);

	return 0;
}

/*
 * vGlobalTimerHardwareLockInit
 * Init hwspinlock must be used at main.c before read counter
 */
void vGlobalTimerHardwareLockInit(void)
{
#if HWSPINLOCK_ID_GLB_TIMER
	if (!hwlock)
		vHwspinLock_init(&hwlock, HWSPINLOCK_ID_GLB_TIMER, NULL);
#endif
}
