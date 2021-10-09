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
#define IRQ_GPIO0_NUM 10
#define IRQ_GPIO1_NUM 11
#define IRQ_GPIO2_NUM 12
#define IRQ_GPIO3_NUM 13
#define IRQ_GPIO4_NUM 14
#define IRQ_GPIO5_NUM 15
#define IRQ_GPIO6_NUM 16
#define IRQ_GPIO7_NUM 17

#define REG_PIN_SC2_SEL			0x04
#define REG_EDGE_POL_EXTR		0x1c
#define REG_EDGE_POL_MASK_SC2(x)			\
	({typeof(x) _x = (x); BIT(_x) | BIT(12 + (_x)); })
#define GPIO_IRQ_FILTER_SHIFT(x)	(((x) % 2 == 0) ? 8 : 24)
#define GPIO_IRQ_POL_SHIFT(x)		(BIT(0 + (x)))
#define GPIO_IRQ_EDGE_SHIFT(x)		(BIT(12 + (x)))
#define GPIO_IRQ_BOTH_SHIFT(x)		(BIT(0 + (x)))

static const GpioDomain_t eeDomain = {
	.name = "EE",
	.rPullen = PADCTRL_GPIOD_I,
	.rPull = PADCTRL_GPIOD_I,
	.rGpio = PADCTRL_GPIOD_I,
	.rMux = PADCTRL_PIN_MUX_REG0,
	.rDrv = PADCTRL_GPIOD_I,
};

static const GpioBank_t gpioBanks[BANK_NUM_MAX] = {
			  /* pullen   pull    dir       out      in      mux */
	BANK("D", &eeDomain, 0x03, 0, 0x04, 0, 0x02, 0, 0x01, 0, 0x00, 0, 0x0b,
		/* drv */
	     0, 0x07, 0),
	BANK("E", &eeDomain, 0x0b, 0, 0x0c, 0, 0x0a, 0, 0x09, 0, 0x08, 0, 0x0d,
	     0, 0x0f, 0),
	BANK("Z", &eeDomain, 0x13, 0, 0x14, 0, 0x12, 0, 0x11, 0, 0x10, 0, 0x04,
	     0, 0x17, 0),
	BANK("Z1", &eeDomain, 0x13, 16, 0x14, 16, 0x12, 16, 0x11, 16, 0x10, 16,
	     0x06, 0, 0xc0, 0),
	BANK("H", &eeDomain, 0x1b, 0, 0x1c, 0, 0x1a, 0, 0x19, 0, 0x18, 0, 0x07,
	     0, 0x1f, 0),
	BANK("H1", &eeDomain, 0x1b, 16, 0x1c, 16, 0x1a, 16, 0x19, 16, 0x18, 16,
	     0x09, 0, 0xc1, 0),
	BANK("B", &eeDomain, 0x2b, 0, 0x2c, 0, 0x2a, 0, 0x29, 0, 0x28, 0, 0x00,
	     0, 0x2f, 0),
	BANK("C", &eeDomain, 0x33, 0, 0x34, 0, 0x32, 0, 0x31, 0, 0x30, 0, 0x02,
	     0, 0x37, 0),
	BANK("W", &eeDomain, 0x63, 0, 0x64, 0, 0x62, 0, 0x61, 0, 0x60, 0, 0x12,
	     0, 0x67, 0),
	BANK("M", &eeDomain, 0x73, 0, 0x74, 0, 0x72, 0, 0x71, 0, 0x70, 0, 0x0e,
	     0, 0x77, 0),
	BANK("M1", &eeDomain, 0x73, 16, 0x74, 16, 0x72, 16, 0x71, 16, 0x70, 16,
	     0x10, 0, 0xc2, 0),
	BANK("TEST_N", &eeDomain, 0x83, 0, 0x84, 0, 0x82, 0, 0x81, 0, 0x80, 0,
	     0x14, 0, 0x87, 0),
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

/* need real gpio irq number */
static const GpioIRQBank_t irqBanks[BANK_NUM_MAX] = {
	GPIO_IRQ_BK("D", 43,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("E", 54,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("Z", 56,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("Z1", 72,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("H", 107, eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("H1", 123, eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("B", 0,   eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("C", 14,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("W", 30,  eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("M", 76, eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("M1", 92, eeIRQs, ARRAY_SIZE(eeIRQs)),
	GPIO_IRQ_BK("TEST_N", 135, eeIRQs, ARRAY_SIZE(eeIRQs)),
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
	reg_offset = REG_PIN_SC2_SEL + ((line / 2) << 2);

	/* clear both edge */
	REG32(GPIO_EE_IRQ_BASE + REG_EDGE_POL_EXTR) =
			REG32(GPIO_EE_IRQ_BASE + REG_EDGE_POL_EXTR) &
			~GPIO_IRQ_BOTH_SHIFT(line);

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
		REG32_UPDATE_BITS(GPIO_EE_IRQ_BASE + REG_EDGE_POL_EXTR,
				  GPIO_IRQ_BOTH_SHIFT(line), val);
		return;
	}

	/* set trigger single edge or level  type */
	if (flags & (IRQF_TRIGGER_LOW | IRQF_TRIGGER_FALLING))
		val |= GPIO_IRQ_POL_SHIFT(line);

	if (flags & (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING))
		val |= GPIO_IRQ_EDGE_SHIFT(line);

	REG32_UPDATE_BITS(GPIO_EE_IRQ_BASE, REG_EDGE_POL_MASK_SC2(line), val);

}

