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
 * eth wol driver
 */
#include "FreeRTOS.h"
#include "suspend.h"
#include "interrupt.h"
#include "mailbox-api.h"
#include "eth.h"

uint32_t eth_wol_flag = 0;
uint32_t ethIrq;
void eth_handler(void)
{
	uint32_t buf[4] = {0};

	buf[0] = ETH_PMT_WAKEUP;
	STR_Wakeup_src_Queue_Send_FromISR(buf);
	DisableIrq(ethIrq);
}

void vETHInit(uint32_t ulIrq,function_ptr_t handler)
{
	ethIrq = ulIrq;
	RegisterIrq(ulIrq, 2, handler);
}

void vETHDeint(void)
{
	DisableIrq(ethIrq);
	UnRegisterIrq(ethIrq);
}

int eth_deinit = 0;
void eth_handler_t5(void)
{
	uint32_t buf[4] = {0};
	if (eth_deinit == 0) {
		buf[0] = ETH_PMT_WAKEUP;
		STR_Wakeup_src_Queue_Send_FromISR(buf);
		DisableIrq(ethIrq);
	} else {
		eth_deinit = 0;
	}
}

void vETHDeint_t5(void)
{
	eth_deinit = 1;
	DisableIrq(ethIrq);
	UnRegisterIrq(ethIrq);
}

int get_ETHWol_flag(void)
{
	return eth_wol_flag;
}

static void *prvETHSetWol(void *msg)
{
	eth_wol_flag = *(u32 *)msg;
	return NULL;
}

void vETHMailboxCallback(void)
{
	int32_t ret;
	ret = xInstallRemoteMessageCallbackFeedBack(AOREE_CHANNEL, MBX_CMD_SET_ETHERNET_WOL,
		prvETHSetWol, 1);
	if (ret == MBOX_CALL_MAX) {
		printf("mailbox cmd 0x%x register fail\n");
		return;
	}
}