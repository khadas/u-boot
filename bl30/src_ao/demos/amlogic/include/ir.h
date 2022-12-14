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
#ifndef _IR_COMMON_H
#define _IR_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"

typedef struct IRPowerKey {
	uint32_t code;
	uint8_t  type;
} IRPowerKey_t;

enum PowerKeyType {
	IR_NORMAL,
	IR_CUSTOM
};

/*supported protocol*/
#define MODE_SOFT		0x0
#define MODE_HARD_NEC		0x1
#define MODE_HARD_LEGACY_NEC	0xff
#define MAX_KEY_NUM  		80

/**
 * Other protocol used by customer, due to size of firmware will not enabled
 * by default
 */

/*
#define MODE_HARD_DUOKAN	0x02
#define MODE_HARD_XMP_1		0x03
#define MODE_HARD_RC5		0x04
#define MODE_HARD_RC6		0x05
#define MODE_HARD_TOSHIBA	0x06
#define MODE_HARD_RCA		0x08
#define MODE_HARD_RCMM		0x09
*/


/* sample for multi-protocol */
/* #define MODE_HARD_RCA_NEC	(MODE_HARD_RCA | MODE_HARD_LEAGCY_NEC << 8) */

/**
 *  xIRInit() - IR hardware initialize.
 *  @usWorkMode: supported protocol.
 *  @usGpio: which gpio is used as input.
 *  @func: function number of gpio use as ir input.
 */
	extern void vIRInit(uint16_t usWorkMode, uint16_t usGpio,
			    enum PinMuxType func, IRPowerKey_t *ulPowerKeyList,
			    uint8_t ucPowerKeyNum, void (*vIRHandler)(IRPowerKey_t *pkey));

/**
 *  vInitIRWorkMode() - change ir protocol.
 *  @usWorkMode - protocol(0:soft, 1: hard nec)
 */
	extern void vInitIRWorkMode(uint16_t usWorkMode);

/**
 *   vIsIRDebugEnable() - open/close ir driver debug
 *   @ucEnable: 1 enable 0 disable debug message.
 */
	extern void vIsIRDebugEnable(uint8_t ucEnable);

/**
 *  ucIsIRInit() - use to see if ir driver is init.
 *  return 1 :already init  0: not ready.
 */
	extern int8_t ucIsIRInit(void);

/**
 *  vIRDeint() - deinit ir
 */
	extern void vIRDeint(void);
	extern void vIRMailboxEnable(void);
	extern void vIRGetKeyCode(IRPowerKey_t *ulPowerKeyList);
#ifdef __cplusplus
}
#endif
#endif
