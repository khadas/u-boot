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

#ifndef _MESON_A5_GPIO_H_
#define _MESON_A5_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif
#define	GPIO_NUM_MAX	99 /*actual numbers of pins*/
#define	BANK_NUM_MAX	9   /*numbers of gpio bank*/
#define	IRQ_REG_NUM	8  /* numbers of irq relative reg*/

#define	GPIO_INVALID	0xffff

/*0-31*/
#define	GPIOE_0		0
#define	GPIOE_1		1

/*32-63*/
#define	GPIOH_0		32
#define	GPIOH_1		33
#define	GPIOH_2		34
#define	GPIOH_3		35
#define	GPIOH_4		36

/*64-95*/
#define	GPIOD_0		64
#define	GPIOD_1		65
#define	GPIOD_2		66
#define	GPIOD_3		67
#define	GPIOD_4		68
#define	GPIOD_5		69
#define	GPIOD_6		70
#define	GPIOD_7		71
#define	GPIOD_8		72
#define	GPIOD_9		73
#define	GPIOD_10	74
#define	GPIOD_11	75
#define	GPIOD_12	76
#define	GPIOD_13	77
#define	GPIOD_14	78
#define	GPIOD_15	79

/*96-127 */
#define	GPIOB_0		96
#define	GPIOB_1		97
#define	GPIOB_2		98
#define	GPIOB_3		99
#define	GPIOB_4		100
#define	GPIOB_5		101
#define	GPIOB_6		102
#define	GPIOB_7		103
#define	GPIOB_8		104
#define	GPIOB_9		105
#define	GPIOB_10	106
#define	GPIOB_11	107
#define	GPIOB_12	108
#define	GPIOB_13	109

/*128-159 */
#define	GPIOX_0		128
#define	GPIOX_1		129
#define	GPIOX_2		130
#define	GPIOX_3		131
#define	GPIOX_4		132
#define	GPIOX_5		133
#define	GPIOX_6		134
#define	GPIOX_7		135
#define	GPIOX_8		136
#define	GPIOX_9		137
#define	GPIOX_10	138
#define	GPIOX_11	139
#define	GPIOX_12	140
#define	GPIOX_13	141
#define	GPIOX_14	142
#define	GPIOX_15	143
#define	GPIOX_16	144
#define	GPIOX_17	145
#define	GPIOX_18	146
#define	GPIOX_19	147

/* 160-191 */
#define	GPIOC_0		160
#define	GPIOC_1		161
#define	GPIOC_2		162
#define	GPIOC_3		163
#define	GPIOC_4		164
#define	GPIOC_5		165
#define	GPIOC_6		166
#define	GPIOC_7		167
#define	GPIOC_8		168
#define	GPIOC_9		169
#define	GPIOC_10	170

/* 192- 223 */
#define	GPIOT_0		192
#define	GPIOT_1		193
#define	GPIOT_2		194
#define	GPIOT_3		195
#define	GPIOT_4		196
#define	GPIOT_5		197
#define	GPIOT_6		198
#define	GPIOT_7		199
#define	GPIOT_8		200
#define	GPIOT_9		201
#define	GPIOT_10	202
#define	GPIOT_11	203
#define	GPIOT_12	204
#define	GPIOT_13	205

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

/* 256- 287*/
#define	GPIO_TEST_N	256

#ifdef __cplusplus
}
#endif
#endif	/* _MESON_A5_GPIO_H_ */
