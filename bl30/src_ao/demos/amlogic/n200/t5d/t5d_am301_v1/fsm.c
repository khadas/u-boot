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


#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */
#include "myprintf.h"

#include <unistd.h>

#include "n200_func.h"
#include "common.h"
#include "riscv_encoding.h"
#include "mailbox-api.h"
#include "irq.h"

typedef enum a {
    PM_CPU_PWR,
    PM_CPU_CORE0,
    PM_CPU_CORE1,
    PM_CPU_CORE2,
    PM_CPU_CORE3,
} PM_E;

static void *xMboxCoreFsmIdle(void *msg)
{
	PM_E domain = *(uint32_t *)msg;

	switch (domain) {
		case PM_CPU_CORE0:
			REG32_UPDATE_BITS(ISA_SOFT_IRQ, (1 << 0), 0);
			EnableIrq(IRQ_NUM_OUT_0);
			break;
		case PM_CPU_CORE1:
			REG32_UPDATE_BITS(ISA_SOFT_IRQ, (1 << 1), 0);
			EnableIrq(IRQ_NUM_OUT_1);
			break;
		case PM_CPU_CORE2:
			REG32_UPDATE_BITS(ISA_SOFT_IRQ, (1 << 2), 0);
			EnableIrq(IRQ_NUM_OUT_2);
			break;
		case PM_CPU_CORE3:
			REG32_UPDATE_BITS(ISA_SOFT_IRQ, (1 << 3), 0);
			EnableIrq(IRQ_NUM_OUT_3);
			break;
		default:
			break;
	}
	return NULL;
}

static void xSetCoreFsmAwakeIrq(int cpuid)
{
	REG32_UPDATE_BITS(ISA_SOFT_IRQ, (1 << cpuid), (1 << cpuid));
}
static void xCore0FsmIdleHandleIsr(void)
{
	xSetCoreFsmAwakeIrq(0);
	DisableIrq(IRQ_NUM_OUT_0);
}

static void xCore1FsmIdleHandleIsr(void)
{
	xSetCoreFsmAwakeIrq(1);
	DisableIrq(IRQ_NUM_OUT_1);
}

static void xCore2FsmIdleHandleIsr(void)
{
	xSetCoreFsmAwakeIrq(2);
	DisableIrq(IRQ_NUM_OUT_2);
}

static void xCore3FsmIdleHandleIsr(void)
{
	xSetCoreFsmAwakeIrq(3);
	DisableIrq(IRQ_NUM_OUT_3);
}

#if 0
#define     PWRCTRL_CPU1_FSM_STS0   (0xff644000 + (0x097 << 2))
void checkstatus(void)
{
	unsigned value;

	printf("\n\n");
	value = REG32(PWRCTRL_CPU0_FSM_STS0);
	value = ((value >> 12) & 0x1f);
	if (value != 0)
		printf("sts0:%x\n", value);

	value = REG32(PWRCTRL_CPU1_FSM_STS0);
	value = ((value >> 12) & 0x1f);
	if (value != 0)
		printf("sts1:%x\n", value);

	value = REG32(PWRCTRL_CPU2_FSM_STS0);
	value = ((value >> 12) & 0x1f);
	if (value != 0)
		printf("sts2:%x\n", value);

	value = REG32(PWRCTRL_CPU3_FSM_STS0);
	value = ((value >> 12) & 0x1f);
	if (value != 0)
		printf("sts3:%x\n", value);
}
#endif

void vCoreFsmIdleInit(void);
void vCoreFsmIdleInit(void)
{
	int ret;

	ret = xInstallRemoteMessageCallbackFeedBack(AOTEE_CHANNEL, MBX_CMD_CPU_FSM_IDLE,
						xMboxCoreFsmIdle, 0);
	if (ret == MBOX_CALL_MAX)
		printf("mbox cmd 0x%x register fail\n", MBX_CMD_CPU_FSM_IDLE);

	RegisterIrq(IRQ_NUM_OUT_0, 1, xCore0FsmIdleHandleIsr);
	RegisterIrq(IRQ_NUM_OUT_1, 1, xCore1FsmIdleHandleIsr);
	RegisterIrq(IRQ_NUM_OUT_2, 1, xCore2FsmIdleHandleIsr);
	RegisterIrq(IRQ_NUM_OUT_3, 1, xCore3FsmIdleHandleIsr);
	REG32_UPDATE_BITS(SEC_SYS_CPU_CFG10, 0xF << 10, 0xF << 10);
}
