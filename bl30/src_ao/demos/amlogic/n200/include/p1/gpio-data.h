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

#ifndef _MESON_T7_GPIO_H_
#define _MESON_T7_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif
#define	GPIO_NUM_MAX	231 /*actual numbers of pins*/
#define	BANK_NUM_MAX	14   /*numbers of gpio bank*/
#define	IRQ_REG_NUM	20  /* numbers of irq relative reg*/

#define	GPIO_INVALID	0xffff

/*0-31*/
#define	GPIOD_0		0
#define	GPIOD_1		1
#define	GPIOD_2		2
#define	GPIOD_3		3
#define	GPIOD_4		4
#define	GPIOD_5		5
#define	GPIOD_6		6
#define	GPIOD_7		7
#define	GPIOD_8		8
#define	GPIOD_9		9
#define	GPIOD_10	10
#define	GPIOD_11	11
#define	GPIOD_12	12

/*32-63*/
#define	GPIOE_0		32
#define	GPIOE_1		33
#define	GPIOE_2		34
#define	GPIOE_3		35
#define	GPIOE_4		36
#define	GPIOE_5		37

/*64-95*/
#define	GPIOZ_0		64
#define	GPIOZ_1		65
#define	GPIOZ_2		66
#define	GPIOZ_3		67
#define	GPIOZ_4		68
#define	GPIOZ_5		69
#define	GPIOZ_6		70
#define	GPIOZ_7		71

/*96-127 */
#define	GPIOH_0		96
#define	GPIOH_1		97
#define	GPIOH_2		98
#define	GPIOH_3		99
#define	GPIOH_4		100
#define	GPIOH_5		101
#define	GPIOH_6		102
#define	GPIOH_7 	103
#define	GPIOH_8 	104
#define	GPIOH_9 	105
#define	GPIOH_10	106
#define	GPIOH_11	107
#define	GPIOH_12	108
#define	GPIOH_13	109
#define	GPIOH_14	110
#define	GPIOH_15	111
#define	GPIOH_16	112
#define	GPIOH_17	113
#define	GPIOH_18	114
#define	GPIOH_19	115
#define	GPIOH_20	116
#define	GPIOH_21	117
#define	GPIOH_22	118
#define	GPIOH_23	119

/*128-159 */
#define	GPIOC_0		128
#define	GPIOC_1		129
#define	GPIOC_2		130
#define	GPIOC_3		131
#define	GPIOC_4		132
#define	GPIOC_5		133
#define	GPIOC_6 	134
#define	GPIOC_7 	135
#define	GPIOC_8 	136
#define	GPIOC_9 	137
#define	GPIOC_10	138
#define	GPIOC_11	139
#define	GPIOC_12	140
#define	GPIOC_13	141
#define	GPIOC_14	142
#define	GPIOC_15	143
#define	GPIOC_16	144
#define	GPIOC_17	145

/* 160-191 */
#define	GPIOB_0		160
#define	GPIOB_1		161
#define	GPIOB_2		162
#define	GPIOB_3		163
#define	GPIOB_4		164
#define	GPIOB_5		165
#define	GPIOB_6		166
#define	GPIOB_7		167
#define	GPIOB_8		168
#define	GPIOB_9		169
#define	GPIOB_10	170
#define	GPIOB_11	171
#define	GPIOB_12	172

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
#define	GPIOX_20	212
#define	GPIOX_21	213
#define	GPIOX_22	214
#define	GPIOX_23	215
#define	GPIOX_24	216
#define	GPIOX_25	217
#define	GPIOX_26	218
#define	GPIOX_27	219
#define	GPIOX_28	220
#define	GPIOX_29	221
#define	GPIOX_30	222
#define	GPIOX_31	223

/* 224- 255*/
#define	GPIOT_0		224
#define	GPIOT_1		225
#define	GPIOT_2		226
#define	GPIOT_3		227
#define	GPIOT_4		228
#define	GPIOT_5		229
#define	GPIOT_6		230
#define	GPIOT_7		231
#define	GPIOT_8		232
#define	GPIOT_9		233
#define	GPIOT_10	234
#define	GPIOT_11	235
#define	GPIOT_12	236
#define	GPIOT_13	237
#define	GPIOT_14	238
#define	GPIOT_15	239
#define	GPIOT_16	240
#define	GPIOT_17	241
#define	GPIOT_18	242
#define	GPIOT_19	243
#define	GPIOT_20	244
#define	GPIOT_21	245
#define	GPIOT_22	246
#define	GPIOT_23	247
#define	GPIOT_24	248
#define	GPIOT_25	249
#define	GPIOT_26	250
#define	GPIOT_27	251
#define	GPIOT_28	252
#define	GPIOT_29	253
#define	GPIOT_30	254
#define	GPIOT_31	255

/* 256-287 */
#define	GPIOK_0		256
#define	GPIOK_1		257
#define	GPIOK_2		258
#define	GPIOK_3		259
#define	GPIOK_4		260
#define	GPIOK_5		261
#define	GPIOK_6		262
#define	GPIOK_7		263
#define	GPIOK_8		264
#define	GPIOK_9		265
#define	GPIOK_10	266
#define	GPIOK_11	267
#define	GPIOK_12	268
#define	GPIOK_13	269
#define	GPIOK_14	270
#define	GPIOK_15	271
#define	GPIOK_16	272
#define	GPIOK_17	273
#define	GPIOK_18	274
#define	GPIOK_19	275
#define	GPIOK_20	276
#define	GPIOK_21	277
#define	GPIOK_22	278
#define	GPIOK_23	279
#define	GPIOK_24	280
#define	GPIOK_25	281
#define	GPIOK_26	282
#define	GPIOK_27	283
#define	GPIOK_28	284
#define	GPIOK_29	285
#define	GPIOK_30	286

/* 288-319 */
#define	GPIOW_0		288
#define	GPIOW_1		289
#define	GPIOW_2		290
#define	GPIOW_3		291
#define	GPIOW_4		292
#define	GPIOW_5		293
#define	GPIOW_6		294
#define	GPIOW_7		295
#define	GPIOW_8		296
#define	GPIOW_9		297
#define	GPIOW_10	298
#define	GPIOW_11	299
#define	GPIOW_12	300
#define	GPIOW_13	301
#define	GPIOW_14	302
#define	GPIOW_15	303
#define	GPIOW_16	304
#define	GPIOW_17	305

/* 320-351 */
#define	GPIOM_0		320
#define	GPIOM_1		321
#define	GPIOM_2		322
#define	GPIOM_3		323
#define	GPIOM_4		324
#define	GPIOM_5 	325

/* 352-383 */
#define	GPIOY_0 	352
#define	GPIOY_1 	353
#define	GPIOY_2 	354
#define	GPIOY_3 	355
#define	GPIOY_4 	356
#define	GPIOY_5 	357
#define	GPIOY_6 	358
#define	GPIOY_7 	359
#define	GPIOY_8 	360
#define	GPIOY_9 	361
#define	GPIOY_10	362
#define	GPIOY_11	363
#define	GPIOY_12	364
#define	GPIOY_13	365
#define	GPIOY_14	366

/* 384 - 415*/
#define	GPION_0  	384
#define	GPION_1  	385
#define	GPION_2  	386
#define	GPION_3  	387
#define	GPION_4  	388
#define	GPION_5  	389
#define	GPION_6  	390
#define	GPION_7  	391
#define	GPION_8  	392
#define	GPION_9  	393
#define	GPION_10 	394
#define	GPION_11 	395
#define	GPION_12 	396
#define	GPION_13 	397

/* 416 -*/
#define	GPIO_TEST_N	416

#ifdef __cplusplus
}
#endif
#endif	/* _MESON_T7_GPIO_H_ */
