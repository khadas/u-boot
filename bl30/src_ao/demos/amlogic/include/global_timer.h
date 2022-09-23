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
#ifndef __AML_GLB_TIMER_H__
#define __AML_GLB_TIMER_H__

/* used for mipi0~4 */
#define GLBT_MIPI0_SOF          0
#define GLBT_MIPI0_EOF          1
#define GLBT_MIPI1_SOF          2
#define GLBT_MIPI1_EOF          3
#define GLBT_MIPI2_SOF          4
#define GLBT_MIPI2_EOF          5
#define GLBT_MIPI3_SOF          6
#define GLBT_MIPI3_EOF          7
#define GLBT_MIPI4_SOF          8
#define GLBT_MIPI4_EOF          9

/* used for 32 gpio irq lines and  MIPI*/
#define GLBT_GPIO0_IRQ          10
#define GLBT_GPIO1_IRQ          11
#define GLBT_GPIO2_IRQ          12
#define GLBT_GPIO3_IRQ          13
#define GLBT_GPIO4_IRQ          14
#define GLBT_GPIO5_IRQ          15
#define GLBT_GPIO6_IRQ          16
#define GLBT_GPIO7_IRQ          17
#define GLBT_GPIO8_IRQ          18
#define GLBT_GPIO9_IRQ          19
#define GLBT_GPIO10_IRQ         20
#define GLBT_GPIO11_IRQ         21
#define GLBT_GPIO12_IRQ         22
#define GLBT_GPIO13_IRQ         23
#define GLBT_GPIO14_IRQ         24
#define GLBT_GPIO15_IRQ         25
#define GLBT_GPIO16_IRQ         26
#define GLBT_GPIO17_IRQ         27
#define GLBT_GPIO18_IRQ         28
#define GLBT_GPIO19_IRQ         29
#define GLBT_GPIO20_IRQ         30
#define GLBT_GPIO21_IRQ         31
#define GLBT_GPIO22_IRQ         32
#define GLBT_GPIO23_IRQ         33
#define GLBT_GPIO24_IRQ         34
#define GLBT_GPIO25_IRQ         35
#define GLBT_GPIO26_IRQ         36
#define GLBT_GPIO27_IRQ         37
#define GLBT_GPIO28_IRQ         38
#define GLBT_GPIO29_IRQ         39
#define GLBT_GPIO30_IRQ         40
#define GLBT_GPIO31_IRQ         41

#define GLBT_OUTPUT_GEN0        0
#define GLBT_OUTPUT_GEN1        1
#define GLBT_OUTPUT_GEN2        2
#define GLBT_OUTPUT_GEN3        3
#define GLBT_OUTPUT_GEN4        4
#define GLBT_OUTPUT_GEN5        5
#define GLBT_OUTPUT_GEN6        6
#define GLBT_OUTPUT_GEN7        7
#define GLBT_OUTPUT_GEN8        8
#define GLBT_OUTPUT_GEN9        9
#define GLBT_OUTPUT_GEN10       10
#define GLBT_OUTPUT_GEN11       11
#define GLBT_OUTPUT_GEN12       12
#define GLBT_OUTPUT_GEN13       13
#define GLBT_OUTPUT_GEN14       14
#define GLBT_OUTPUT_GEN15       15

#define GLBT_OUTPUT_GPIO0       0
#define GLBT_OUTPUT_GPIO1       1
#define GLBT_OUTPUT_GPIO2       2
#define GLBT_OUTPUT_GPIO3       3
#define GLBT_OUTPUT_GPIO4       4
#define GLBT_OUTPUT_GPIO5       5
#define GLBT_OUTPUT_GPIO6       6
#define GLBT_OUTPUT_GPIO7       7
#define GLBT_OUTPUT_GPIO8       8
#define GLBT_OUTPUT_GPIO9       9
#define GLBT_OUTPUT_GPIO10      10
#define GLBT_OUTPUT_GPIO11      11
#define GLBT_OUTPUT_GPIO12      12
#define GLBT_OUTPUT_GPIO13      13
#define GLBT_OUTPUT_GPIO14      14
#define GLBT_OUTPUT_GPIO15      15

/* TRIG_TYPE_** | TRIG_ONE_SHOT */
enum MesonGlbSrcSelFlag {
	TRIG_TYPE_SW_SET                = 0,
	TRIG_TYPE_RISING                = 1,
	TRIG_TYPE_FALLING               = 2,
	TRIG_TYPE_BOTH                  = 3,
	TRIG_ONE_SHOT                   = 4,
};

enum OutputMode {
	CONTINUE_MODE			= 0,
	ONESHOT_MODE                    = 1,
};
uint64_t ullGlobalTimerGlobalSnapshot(void);
void vGlobalTimerReset(void);
int uGlobalTimerOutputGPIOSetup(uint8_t ucSrc, uint8_t ucOneshot,
		uint32_t uPulseWith, uint32_t uInterval, uint8_t ucInitVal);
int uGlobalTimerOutputStart(uint8_t ucSrcn, uint64_t ullexpires);
int uGlobalTimerInputGPIOConfigure(uint8_t ucSrc,
				    enum MesonGlbSrcSelFlag type);
uint64_t ullGlobalTimerInputGPIOGetSnapshot(uint8_t ucSrc);
void vGlobalTimerHardwareLockInit(void);
#endif
