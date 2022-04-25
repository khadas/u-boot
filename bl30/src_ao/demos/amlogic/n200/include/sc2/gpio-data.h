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

#ifndef _MESON_SC2_GPIO_H_
#define _MESON_SC2_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif
#define	GPIO_NUM_MAX	87 /*actual numbers of pins*/
#define	BANK_NUM_MAX	9   /*numbers of gpio bank*/
#define	IRQ_REG_NUM	8  /* numbers of irq relative reg*/

#define	GPIO_INVALID	0xffff

/*0-31*/
#define	GPIOA_14	0
#define	GPIOA_15	1


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
#define	GPIOB_14	46
#define	GPIOB_15	47

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
#define	GPIOE_0		96
#define	GPIOE_1		97
#define	GPIOE_2		98

/*128-159 */
#define	GPIOD_0		128
#define	GPIOD_1		129
#define	GPIOD_2		130
#define	GPIOD_3		131
#define	GPIOD_4		132
#define	GPIOD_5		133
#define	GPIOD_6		134
#define	GPIOD_7		135
#define	GPIOD_8		136
#define	GPIOD_9		137
#define	GPIOD_10	138
#define	GPIOD_11	139

/* 160-191 */
#define	GPIOH_0		160
#define	GPIOH_1		161
#define	GPIOH_2		162
#define	GPIOH_3		163
#define	GPIOH_4		164
#define	GPIOH_5		165
#define	GPIOH_6		166
#define	GPIOH_7		167
#define	GPIOH_8		168

/* 192- 223 */
#define	GPIOX_0		192
#define	GPIOX_1		193
#define	GPIOX_2		194
#define	GPIOX_3		195
#define	GPIOX_4		196
#define	GPIOX_5		197
#define	GPIOX_6		198
#define	GPIOX_7		199
#define	GPIOX_8		200
#define	GPIOX_9		201
#define	GPIOX_10	202
#define	GPIOX_11	203
#define	GPIOX_12	204
#define	GPIOX_13	205
#define	GPIOX_14	206
#define	GPIOX_15	207
#define	GPIOX_16	208
#define	GPIOX_17	209
#define	GPIOX_18	210
#define	GPIOX_19	211

/* 224- 255*/
#define	GPIOZ_0		224
#define	GPIOZ_1		225
#define	GPIOZ_2		226
#define	GPIOZ_3		227
#define	GPIOZ_4		228
#define	GPIOZ_5		229
#define	GPIOZ_6		230
#define	GPIOZ_7		231
#define	GPIOZ_8		232
#define	GPIOZ_9		233
#define	GPIOZ_10	234
#define	GPIOZ_11	235
#define	GPIOZ_12	236
#define	GPIOZ_13	237
#define	GPIOZ_14	238
#define	GPIOZ_15	239

#define	GPIO_TEST_N	256

#ifdef __cplusplus
}
#endif
#endif	/* _MESON_SC2_GPIO_H_ */
