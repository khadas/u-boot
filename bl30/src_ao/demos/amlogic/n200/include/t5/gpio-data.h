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

#ifndef _MESON_T5_GPIO_H_
#define _MESON_T5_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "register.h"
#define	GPIO_NUM_MAX	71 /*actual numbers of pins*/
#define	BANK_NUM_MAX	6   /*numbers of gpio bank*/
#define	IRQ_REG_NUM	4  /* numbers of irq relative reg*/
#define	GPIO_INVALID	0xffff
#ifdef AO_IRQ_GPIO_REG
#define GPIO_AO_IRQ_BASE	AO_IRQ_GPIO_REG
#endif

/*0-31*/
#define	GPIOD_0 	0
#define	GPIOD_1 	1
#define	GPIOD_2 	2
#define	GPIOD_3 	3
#define	GPIOD_4 	4
#define	GPIOD_5 	5
#define	GPIOD_6 	6
#define	GPIOD_7 	7
#define	GPIOD_8 	8
#define	GPIOD_9 	9
#define	GPIOD_10	10

/*32-63*/
#define	GPIOH_0		32
#define	GPIOH_1		33
#define	GPIOH_2		34
#define	GPIOH_3		35
#define	GPIOH_4		36
#define	GPIOH_5		37
#define	GPIOH_6		38
#define	GPIOH_7		39
#define	GPIOH_8		40
#define	GPIOH_9		41
#define	GPIOH_10	42
#define	GPIOH_11	43
#define	GPIOH_12	44
#define	GPIOH_13	45
#define	GPIOH_14	46
#define	GPIOH_15	47
#define	GPIOH_16	48
#define	GPIOH_17	49
#define	GPIOH_18	50
#define	GPIOH_19	51
#define	GPIOH_20	52
#define	GPIOH_21	53

/*64-95*/
#define	GPIOB_0		64
#define	GPIOB_1		65
#define	GPIOB_2		66
#define	GPIOB_3		67
#define	GPIOB_4		68
#define	GPIOB_5		69
#define	GPIOB_6		70
#define	GPIOB_7 	71
#define	GPIOB_8 	72
#define	GPIOB_9 	73
#define	GPIOB_10	74
#define	GPIOB_11	75
#define	GPIOB_12	76
#define	GPIOB_13	77

/*96-127 */
#define	GPIOZ_0		96
#define	GPIOZ_1		97
#define	GPIOZ_2		98
#define	GPIOZ_3		99
#define	GPIOZ_4		100
#define	GPIOZ_5		101
#define	GPIOZ_6		102

/*128-159 */
#define	GPIOW_0		128
#define	GPIOW_1		129
#define	GPIOW_2		130
#define	GPIOW_3		131
#define	GPIOW_4		132
#define	GPIOW_5		133
#define	GPIOW_6		134
#define	GPIOW_7		135
#define	GPIOW_8		136
#define	GPIOW_9		137
#define	GPIOW_10	138
#define	GPIOW_11	139
#define	GPIOW_12	140

/* 160-191 */
#define	GPIOE_0		160
#define	GPIOE_1		161
#define	GPIOE_2		162

/* 192 */
#define	GPIO_TEST_N	192

#ifdef __cplusplus
}
#endif
#endif	/* _MESON_T5_GPIO_H_ */
