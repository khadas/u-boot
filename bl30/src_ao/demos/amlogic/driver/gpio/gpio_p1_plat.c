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
 * gpio driver platform data
 */
#include "FreeRTOS.h"
#include <register.h>
#include <common.h>
#include <gpio.h>
#include "util.h"
#include "projdefs.h"
#include "gpio_drv.h"
#include "gpio_irq.h"
#include "portmacro.h"

/* gpio irq controller */
#define IRQ_GPIO0_NUM 228
#define IRQ_GPIO1_NUM 229
#define IRQ_GPIO2_NUM 230
#define IRQ_GPIO3_NUM 231
#define IRQ_GPIO4_NUM 232
#define IRQ_GPIO5_NUM 233
#define IRQ_GPIO6_NUM 234
#define IRQ_GPIO7_NUM 235

#define REG_PIN_P1_SEL			0x10
#define REG_EDGE_SINGLE			0x04
#define REG_POL_LOW			0x08
#define REG_EDGE_BOTH			0x0c
#define GPIO_IRQ_FILTER_SHIFT(x)	(((x) % 2 == 0) ? 8 : 24)
#define GPIO_IRQ_POL_SHIFT(x)		(BIT(0 + (x)))
#define GPIO_IRQ_EDGE_SHIFT(x)		(BIT(0 + (x)))
#define GPIO_IRQ_BOTH_SHIFT(x)		(BIT(0 + (x)))

static const GpioDomain_t stDomain = {
	.name = "ST",
	.rPullen = PADCTRL_GPIOB_I,
	.rPull = PADCTRL_GPIOB_I,
	.rGpio = PADCTRL_GPIOB_I,
	.rMux = PADCTRL_PIN_MUX_REG10,
	.rDrv = PADCTRL_GPIOB_I,
};

static const GpioDomain_t eeDomain = {
	.name = "EE",
	.rPullen = PADCTRL_TESTN_I,
	.rPull = PADCTRL_TESTN_I,
	.rGpio = PADCTRL_TESTN_I,
	.rMux = PADCTRL_PIN_MUX_REG2,
	.rDrv = PADCTRL_TESTN_I,
};

static const GpioBank_t gpioBanks[BANK_NUM_MAX] = {
			/* pullen   pull    dir       out      in      mux */
	BANK("D", &eeDomain, 0x13, 0, 0x14, 0, 0x12, 0, 0x11, 0, 0x10, 0, 0x00,
	        /* drv */
	     0, 0x17, 0),
	BANK("E", &eeDomain, 0x23, 0, 0x24, 0, 0x22, 0, 0x21, 0, 0x20, 0, 0x02,
	     0, 0x27, 0),
	BANK("Z", &eeDomain, 0x33, 0, 0x34, 0, 0x32, 0, 0x31, 0, 0x30, 0, 0x1d,
	     0, 0x37, 0),
	BANK("H", &eeDomain, 0x43, 0, 0x44, 0, 0x42, 0, 0x41, 0, 0x40, 0, 0x0d,
	     0, 0x47, 0),
	BANK("C", &eeDomain, 0x53, 0, 0x54, 0, 0x52, 0, 0x51, 0, 0x50, 0, 0x03,
	     0, 0x57, 0),
	BANK("B", &stDomain, 0x03, 0, 0x04, 0, 0x02, 0, 0x01, 0, 0x00, 0, 0x00,
	     0, 0x07, 0),
	BANK("X", &eeDomain, 0x73, 0, 0x74, 0, 0x72, 0, 0x71, 0, 0x70, 0, 0x17,
	     0, 0x77, 0),
	BANK("T", &eeDomain, 0x83, 0, 0x84, 0, 0x82, 0, 0x81, 0, 0x80, 0, 0x06,
	     0, 0x87, 0),
	BANK("K", &eeDomain, 0x93, 0, 0x94, 0, 0x92, 0, 0x91, 0, 0x90, 0, 0x10,
	     0, 0x97, 0),
	BANK("W", &eeDomain, 0xa3, 0, 0xa4, 0, 0xa2, 0, 0xa1, 0, 0xa0, 0, 0x14,
	     0, 0xa7, 0),
	BANK("M", &eeDomain, 0xb3, 0, 0xb4, 0, 0xb2, 0, 0xb1, 0, 0xb0, 0, 0x0a,
	     0, 0xb7, 0),
	BANK("Y", &eeDomain, 0xc3, 0, 0xc4, 0, 0xc2, 0, 0xc1, 0, 0xc0, 0, 0x1b,
	     0, 0xc7, 0),
	BANK("N", &eeDomain, 0xd3, 0, 0xd4, 0, 0xd2, 0, 0xd1, 0, 0xd0, 0, 0x0b,
	     0, 0xd7, 0),
	BANK("TEST_N", &eeDomain, 0x03, 0, 0x04, 0, 0x02, 0, 0x01, 0, 0x00, 0, 0x0a,
	0, 0x07, 0),
};

static ParentIRQDesc_t eeIRQs[] = {
	[GPIO_EE_IRQ_L0] = PARENT_IRQ_BK(NULL, 0, GPIO_INVALID, IRQ_GPIO0_NUM),
	[GPIO_EE_IRQ_L1] = PARENT_IRQ_BK(NULL, 0, GPIO_INVALID, IRQ_GPIO1_NUM),
	[GPIO_EE_IRQ_L2] = PARENT_IRQ_BK(NULL, 0, GPIO_INVALID, IRQ_GPIO2_NUM),
	[GPIO_EE_IRQ_L3] = PARENT_IRQ_BK(NULL, 0, GPIO_INVALID, IRQ_GPIO3_NUM),
	[GPIO_EE_IRQ_L4] = PARENT_IRQ_BK(NULL, 0, GPIO_INVALID, IRQ_GPIO4_NUM),
	[GPIO_EE_IRQ_L5] = PARENT_IRQ_BK(NULL, 0, GPIO_INVALID, IRQ_GPIO5_NUM),
	[GPIO_EE_IRQ_L6] = PARENT_IRQ_BK(NULL, 0, GPIO_INVALID, IRQ_GPIO6_NUM),
	[GPIO_EE_IRQ_L7] = PARENT_IRQ_BK(NULL, 0, GPIO_INVALID, IRQ_GPIO7_NUM),
};

static const GpioIRQBank_t irqBanks[BANK_NUM_MAX] = {
	GPIO_IRQ_BK("D", 13,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("E", 26,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("Z", 222,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("H", 102, eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("C", 32,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("B", 0,   eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("X", 175,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("T", 50,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("K", 126,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("W", 157,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("M", 82, eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("Y", 207, eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("N", 88, eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("TEST_N", 230, eeIRQs, ARRAY_SIZE(eeIRQs)),
};

const GpioBank_t *pGetGpioBank(void)
{
	return gpioBanks;
}

const GpioIRQBank_t *pGetGpioIrqBank(void)
{
	return irqBanks;
}

void prvGpioPlatIrqSetup(uint16_t irqNum, uint8_t line, uint32_t flags)
{
	uint32_t val = 0;
	uint32_t reg_offset = 0;
	uint16_t bit_offset = 0;

	bit_offset = ((line % 2) == 0) ? 0 : 16;
	reg_offset = REG_PIN_P1_SEL + ((line / 2) << 2);

	/* clear both edge */
	REG32_UPDATE_BITS(GPIO_EE_IRQ_BASE + REG_EDGE_BOTH,
			  GPIO_IRQ_BOTH_SHIFT(line), 0);
	/* set filter */
	REG32_UPDATE_BITS(GPIO_EE_IRQ_BASE + reg_offset,
			  0x7 << GPIO_IRQ_FILTER_SHIFT(line),
			  0x7 << GPIO_IRQ_FILTER_SHIFT(line));

	/* select trigger pin */
	REG32_UPDATE_BITS(GPIO_EE_IRQ_BASE + reg_offset,
			  0xff << bit_offset, irqNum << bit_offset);

	/* set trigger both type */
	if (flags & IRQF_TRIGGER_BOTH) {
		val |= GPIO_IRQ_BOTH_SHIFT(line);
		REG32_UPDATE_BITS(GPIO_EE_IRQ_BASE + REG_EDGE_BOTH,
				  GPIO_IRQ_BOTH_SHIFT(line), val);
		return;
	}

	/* set trigger single edge or level  type */
	if (flags & (IRQF_TRIGGER_LOW | IRQF_TRIGGER_FALLING)) {
		val |= GPIO_IRQ_POL_SHIFT(line);
		REG32_UPDATE_BITS(GPIO_EE_IRQ_BASE + REG_POL_LOW, GPIO_IRQ_POL_SHIFT(line), val);
	}

	if (flags & (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)) {
		val = 0;
		val |= GPIO_IRQ_EDGE_SHIFT(line);
		REG32_UPDATE_BITS(GPIO_EE_IRQ_BASE + REG_EDGE_SINGLE, GPIO_IRQ_EDGE_SHIFT(line), val);
	}
}

