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

#ifndef _MESON_T3_GPIO_H_
#define _MESON_T3_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif
#define	GPIO_NUM_MAX	136 /*actual numbers of pins*/
#define	BANK_NUM_MAX	12   /*numbers of gpio bank*/
#define	IRQ_REG_NUM	8  /* numbers of irq relative reg*/

#define	GPIO_INVALID	0xffff

/*0-31*/
#define GPIOD_0 	0
#define GPIOD_1 	1
#define GPIOD_2 	2
#define GPIOD_3 	3
#define GPIOD_4 	4
#define GPIOD_5 	5
#define GPIOD_6 	6
#define GPIOD_7 	7
#define GPIOD_8 	8
#define GPIOD_9 	9
#define GPIOD_10	10

/*32-63*/
#define	GPIOE_0		32
#define	GPIOE_1		33

/*64-95*/
#define	GPIOZ_0 	64
#define	GPIOZ_1 	65
#define	GPIOZ_2 	66
#define	GPIOZ_3 	67
#define	GPIOZ_4 	68
#define	GPIOZ_5 	69
#define	GPIOZ_6 	70
#define	GPIOZ_7 	71
#define	GPIOZ_8 	72
#define	GPIOZ_9 	73
#define	GPIOZ_10	74
#define	GPIOZ_11	75
#define	GPIOZ_12	76
#define	GPIOZ_13	77
#define	GPIOZ_14	78
#define	GPIOZ_15	79

/*96-127 */
#define	GPIOZ_16	96
#define	GPIOZ_17	97
#define	GPIOZ_18	98
#define	GPIOZ_19	99

/*128-159 */
#define	GPIOH_0 	128
#define	GPIOH_1 	129
#define	GPIOH_2 	130
#define	GPIOH_3 	131
#define	GPIOH_4 	132
#define	GPIOH_5 	133
#define	GPIOH_6 	134
#define	GPIOH_7 	135
#define	GPIOH_8 	136
#define	GPIOH_9 	137
#define	GPIOH_10	138
#define	GPIOH_11	139
#define	GPIOH_12	140
#define	GPIOH_13	141
#define	GPIOH_14	142
#define	GPIOH_15	143

/* 160-191 */
#define	GPIOH_16	160
#define	GPIOH_17	161
#define	GPIOH_18	162
#define	GPIOH_19	163
#define	GPIOH_20	164
#define	GPIOH_21	165
#define	GPIOH_22	166
#define	GPIOH_23	167
#define	GPIOH_24	168
#define	GPIOH_25	169
#define	GPIOH_26	170
#define	GPIOH_27	171

/* 192-223 */
#define	GPIOB_0		192
#define	GPIOB_1		193
#define	GPIOB_2		194
#define	GPIOB_3		195
#define	GPIOB_4		196
#define	GPIOB_5		197
#define	GPIOB_6 	198
#define	GPIOB_7 	199
#define	GPIOB_8 	200
#define	GPIOB_9 	201
#define	GPIOB_10	202
#define	GPIOB_11	203
#define	GPIOB_12	204
#define	GPIOB_13	205

/* 224-255 */
#define	GPIOC_0		224
#define	GPIOC_1		225
#define	GPIOC_2		226
#define	GPIOC_3		227
#define	GPIOC_4		228
#define	GPIOC_5		229
#define	GPIOC_6		230
#define	GPIOC_7  	231
#define	GPIOC_8  	232
#define	GPIOC_9  	233
#define	GPIOC_10 	234
#define	GPIOC_11 	235
#define	GPIOC_12 	236
#define	GPIOC_13 	237
#define	GPIOC_14 	238
#define	GPIOC_15 	239

/* 256-287 */
#define	GPIOW_0  	256
#define	GPIOW_1  	257
#define	GPIOW_2  	258
#define	GPIOW_3  	259
#define	GPIOW_4  	260
#define	GPIOW_5  	261
#define	GPIOW_6  	262
#define	GPIOW_7  	263
#define	GPIOW_8  	264
#define	GPIOW_9  	265
#define	GPIOW_10 	266
#define	GPIOW_11 	267
#define	GPIOW_12 	268

/* 288-319 */
#define	GPIOM_0 	288
#define	GPIOM_1 	289
#define	GPIOM_2 	290
#define	GPIOM_3 	291
#define	GPIOM_4 	292
#define	GPIOM_5 	293
#define	GPIOM_6 	294
#define	GPIOM_7  	295
#define	GPIOM_8  	296
#define	GPIOM_9  	297
#define	GPIOM_10 	298
#define	GPIOM_11 	299
#define	GPIOM_12 	300
#define	GPIOM_13 	301
#define	GPIOM_14	302
#define	GPIOM_15	303

/* 320-351 */
#define	GPIOM_16	320
#define	GPIOM_17	321
#define	GPIOM_18	322
#define	GPIOM_19	323
#define	GPIOM_20	324
#define	GPIOM_21	325
#define	GPIOM_22	326
#define	GPIOM_23	327
#define	GPIOM_24	328
#define	GPIOM_25	329
#define	GPIOM_26	330
#define	GPIOM_27	331
#define	GPIOM_28	332
#define	GPIOM_29	333
#define	GPIOM_30	334

/* 352-383 */
#define	GPIO_TEST_N	352

#ifdef __cplusplus
}
#endif
#endif	/* _MESON_T3_GPIO_H_ */
