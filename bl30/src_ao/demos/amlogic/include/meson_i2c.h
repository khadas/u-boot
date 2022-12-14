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

#ifndef __MESON_I2C_H
#define __MESON_I2C_H

#ifdef __cplusplus
extern "C" {
#endif
#include "common.h"

	enum {
		I2C_M0 = 0,
		I2C_M1,
		I2C_M2,
		I2C_M3,
		I2C_M4,
		I2C_M5,
		I2C_AO_A,
		I2C_AO_B
	};

	enum {
		I2C0 = 0,
		I2C1,
		I2C2,
		I2C3,
		I2C4,
		I2C5,
		I2C6,
		I2C7,
		I2C8,
		I2C9,
		I2C10,
	};

	struct xI2cMsg {
		uint32_t addr;
		uint32_t flags;
		uint32_t len;
		uint8_t *buf;
	};

	struct xMesonI2cPlatdata {
		uint32_t bus_num;
		unsigned long reg;
		uint32_t div_factor;
		uint32_t delay_adjust;
		uint32_t clock_frequency;	/* i2c rate */
		uint32_t clkin_rate;
		uint32_t clk_base;
		uint32_t clk_offset;
	};

#define I2C_M_RD		0x0001
#define MESON_I2C_CLK_RATE      166666667

	void meson_i2c_plat_init(void);
	int32_t xI2cMesonPortInit(uint32_t id);
	int32_t xI2cMesonSetBusSpeed(uint32_t speed);
	int32_t xI2cMesonXfer(struct xI2cMsg *msg, uint32_t nmsgs);
	int32_t xI2cMesonRead(uint32_t addr, uint8_t offset,
			      uint8_t *buffer, uint32_t len);
	int32_t xI2cMesonWrite(uint32_t addr, uint8_t offset,
			       uint8_t *buffer, uint32_t len);
	void xI2cDumpRegs(void);
	void xI2cSetCurrentId(uint32_t id);

	extern struct xMesonI2cPlatdata *plat;
#ifdef __cplusplus
}
#endif
#endif
