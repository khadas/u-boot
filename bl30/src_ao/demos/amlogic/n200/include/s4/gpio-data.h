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
 * platform related header file
 */

#ifndef _MESON_S4_GPIO_H_
#define _MESON_S4_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif
#define	GPIO_NUM_MAX	82 /*actual numbers of pins*/
#define	BANK_NUM_MAX	8   /*numbers of gpio bank*/
#define	IRQ_REG_NUM	8  /* numbers of irq relative reg*/

#define	GPIO_INVALID	0xffff

/*0-31*/
#define	GPIOE_0		0
#define	GPIOE_1		1

/*32-63*/
#define	GPIOB_0		32
#define	GPIOB_1		33
#define	GPIOB_2		34
#define	GPIOB_3		35
#define	GPIOB_4		36
#define	GPIOB_5		37
#define	GPIOB_6		38
#define	GPIOB_7		39
#define	GPIOB_8		40
#define	GPIOB_9		41
#define	GPIOB_10	42
#define	GPIOB_11	43
#define	GPIOB_12	44
#define	GPIOB_13	45

/*64-95*/
#define	GPIOC_0		64
#define	GPIOC_1		65
#define	GPIOC_2		66
#define	GPIOC_3		67
#define	GPIOC_4		68
#define	GPIOC_5		69
#define	GPIOC_6		70
#define	GPIOC_7		71

/*96-127 */
#define	GPIOD_0		96
#define	GPIOD_1		97
#define	GPIOD_2		98
#define	GPIOD_3		99
#define	GPIOD_4		100
#define	GPIOD_5		101
#define	GPIOD_6		102
#define	GPIOD_7		103
#define	GPIOD_8		104
#define	GPIOD_9		105
#define	GPIOD_10	106
#define	GPIOD_11	107

/*128-159 */
#define	GPIOH_0		128
#define	GPIOH_1		129
#define	GPIOH_2		130
#define	GPIOH_3		131
#define	GPIOH_4		132
#define	GPIOH_5		133
#define	GPIOH_6		134
#define	GPIOH_7		135
#define	GPIOH_8		136
#define	GPIOH_9		137
#define	GPIOH_10	138
#define	GPIOH_11	139

/* 160-191 */
#define	GPIOX_0		160
#define	GPIOX_1		161
#define	GPIOX_2		162
#define	GPIOX_3		163
#define	GPIOX_4		164
#define	GPIOX_5		165
#define	GPIOX_6		166
#define	GPIOX_7		167
#define	GPIOX_8		168
#define	GPIOX_9		169
#define	GPIOX_10	170
#define	GPIOX_11	171
#define	GPIOX_12	172
#define	GPIOX_13	173
#define	GPIOX_14	174
#define	GPIOX_15	175
#define	GPIOX_16	176
#define	GPIOX_17	177
#define	GPIOX_18	178
#define	GPIOX_19	179

/* 192- 223 */
#define	GPIOZ_0		192
#define	GPIOZ_1		193
#define	GPIOZ_2		194
#define	GPIOZ_3		195
#define	GPIOZ_4		196
#define	GPIOZ_5		197
#define	GPIOZ_6		198
#define	GPIOZ_7		199
#define	GPIOZ_8		200
#define	GPIOZ_9		201
#define	GPIOZ_10	202
#define	GPIOZ_11	203
#define	GPIOZ_12	204

/* 224- 255*/
#define	GPIO_TEST_N	224

#ifdef __cplusplus
}
#endif
#endif	/* _MESON_S4_GPIO_H_ */
