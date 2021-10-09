/*
 * Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.  *
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
 * gpio driver header file
 */
#ifndef _GPIO_DRIVER_H_
#define _GPIO_DRIVER_H_

/**
 * enum GpioRegType - type of registers encoded in @meson_reg_desc
 */
enum GpioRegType {
	REG_PULLEN = 0x0,
	REG_PULL,
	REG_DIR,
	REG_OUT,
	REG_IN,
	REG_MUX,
	REG_DRV,
	NUM_REG,
};

typedef struct GpioRegDesc {
	uint8_t reg;
	uint8_t bit;
} GpioRegDesc_t;

typedef struct GpioDomain {
	const char *name;

	volatile uint32_t rPullen;
	volatile uint32_t rPull;
	volatile uint32_t rGpio;
	volatile uint32_t rMux;
	volatile uint32_t rDrv;
} GpioDomain_t;

typedef struct GpioBank {
	const char *name;
	const GpioDomain_t *domain;
	GpioRegDesc_t regs[NUM_REG];
} GpioBank_t;

const GpioBank_t *pGetGpioBank(void);

#define BANK(n, d, per, peb, pr, pb, dr, db, or, ob, ir, ib, mr, mb, sr, sb)	\
{										\
	.name   = n,								\
	.domain = d,								\
	.regs	= {								\
		[REG_PULLEN]	= {per, peb},					\
		[REG_PULL]	= {pr, pb},					\
		[REG_DIR]	= {dr, db},					\
		[REG_OUT]	= {or, ob},					\
		[REG_IN]	= {ir, ib},					\
		[REG_MUX]	= {mr, mb},					\
		[REG_DRV]	= {sr, sb},					\
	},									\
}

#endif
