/*
 * Copyright (C)2018 Amlogic, Inc. All rights reserved.
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
#ifndef _IR_DRIVER_H_
#define _IR_DRIVER_H_
#include <ir.h>
#include "register.h"
#include "irq.h"

#define	DRIVE_NAME		"[ir]"

/* only for sc2 now */
#define	IR_INTERRUPT_NUM	22

#ifndef IR_INTERRUPT_NUM
#define IR_INTERRUPT_NUM	0
#endif

#ifndef IRQ_NUM_IRIN
#define IRQ_NUM_IRIN		0
#endif

#ifdef IRCTRL_IR_DEC_LDR_ACTIVE
#define	IR_BASE_ADDR_OLD	IRCTRL_IR_DEC_LDR_ACTIVE
#else
#ifdef AO_IR_DEC_LDR_ACTIVE
#define	IR_BASE_ADDR_OLD	AO_IR_DEC_LDR_ACTIVE
#else
#define	IR_BASE_ADDR_OLD	0
#endif
#endif

#ifdef IRCTRL_MF_IR_DEC_LDR_ACTIVE
#define	IR_BASE_ADDR		IRCTRL_MF_IR_DEC_LDR_ACTIVE
#else
#ifdef AO_MF_IR_DEC_LDR_ACTIVE
#define	IR_BASE_ADDR		AO_MF_IR_DEC_LDR_ACTIVE
#else
#define	IR_BASE_ADDR		0
#endif
#endif

#define	IS_LEGACY_CTRL(x)	((x) ? (IR_BASE_ADDR_OLD) : (IR_BASE_ADDR))
#define	ENABLE_LEGACY_CTL(x)	(((x) >> 8) & 0xff)
#define	LEGACY_IR_CTL_MASK(x)	ENABLE_LEGACY_CTL(x)
#define	MULTI_IR_CTL_MASK(x)	((x) & 0xff)

/*frame status*/
enum {
	STATUS_INVALID = 0,
	STATUS_NORMAL,
	STATUS_REPEAT,
};

enum {
	MULTI_CTL = 0,
	LEGACY_CTL,
	ERROR_CTL,
};

/*register file*/
enum xIRReg {
	REG_LDR_ACTIVE = 0x00 << 2,
	REG_LDR_IDLE = 0x01 << 2,
	REG_LDR_REPEAT = 0x02 << 2,
	REG_BIT_0 = 0x03 << 2,
	REG_REG0 = 0x04 << 2,
	REG_FRAME = 0x05 << 2,
	REG_STATUS = 0x06 << 2,
	REG_REG1 = 0x07 << 2,
	REG_REG2 = 0x08 << 2,
	REG_DURATN2 = 0x09 << 2,
	REG_DURATN3 = 0x0a << 2,
	REG_FRAME1 = 0x0b << 2,
	REG_STATUS1 = 0x0c << 2,
	REG_STATUS2 = 0x0d << 2,
	REG_REG3 = 0x0e << 2,
	REG_FRAME_RSV0 = 0x0f << 2,
	REG_FRAME_RSV1 = 0x10 << 2,
	REG_IRQ_CTL = 0x12 << 2,
	REG_FIFO = 0x13 << 2,
	REG_WITH = 0x14 << 2,
	REG_REPEAT_DET = 0x15 << 2,
	REG_DEMOD_CNTL0 = 0x20 << 2,
	REG_DEMOD_CNTL1 = 0x21 << 2,
	REG_DEMOD_IIR_THD = 0x22 << 2,
	REG_DEMOD_THD0 = 0x23 << 2,
	REG_DEMOD_THD1 = 0x24 << 2,
	REG_DEMOD_SUM_CNT0 = 0x25 << 2,
	REG_DEMOD_SUM_CNT1 = 0x26 << 2,
	REG_DEMOD_CNT0 = 0x27 << 2,
	REG_DEMOD_CNT1 = 0x28 << 2,
};

struct xRegList {
	uint8_t ucReg;
	uint32_t ulVal;
};

/* protocol-->register */
typedef struct xRegProtocolMethod {
	uint8_t ucProtocol;
	uint8_t ucRegNum;
	struct xRegList *RegList;
} xRegProtocolMethod;

/*IR Driver data*/
struct xIRDrvData {
	uint8_t ucWorkCTL:1;
	uint8_t ucIsInit:1;
	uint8_t ucIRStatus:4;
	uint8_t ucPowerKeyNum;
	uint16_t ucCurWorkMode;
	uint32_t ulFrameCode;
	IRPowerKey_t *ulPowerKeyList;
	void (*vIRHandler)(IRPowerKey_t *pkey);
};

const xRegProtocolMethod **pGetSupportProtocol(void);
struct xIRDrvData *pGetIRDrvData(void);

#endif
