
/*
 *  Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 *  All information contained herein is Amlogic confidential.
 *
 */

/*Mailbox driver*/
#include <stdint.h>
#include <stdlib.h>
#include <util.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <stdio.h>
#include <unistd.h>
#include "n200_func.h"
#include "uart.h"
#include "common.h"
#include "riscv_encoding.h"

#include "mailbox.h"
#include "mailbox-pl-in.h"
#include "mailbox-htbl.h"
#include "mailbox-api.h"

#define MBTAG		"AOCPU"
#define PRINT_DBG(...)	//printf(__VA_ARGS__)
#define PRINT_ERR(...)	printf(__VA_ARGS__)
#define PRINT(...)	printf(__VA_ARGS__)

#define AO_MBOX_ONLY_SYNC	1

void *g_tbl_ao;

TaskHandle_t mbTeeHandler;
TaskHandle_t mbReeHandler;
static uint32_t ulTeeSyncTaskWake;
static uint32_t ulReeSyncTaskWake;
mbPackInfo syncTeeMbInfo;
mbPackInfo syncReeMbInfo;

extern void vRpcUserCmdInit(void);

static void vEnterCritical(void)
{
        taskENTER_CRITICAL();
}

static void vExitCritical(void)
{
        taskEXIT_CRITICAL();
}


/*ARM 2 AOCPU mailbox*/
static void vAoRevTeeMbHandler(uint32_t inmbox)
{
	//BaseType_t xYieldRequired = pdFALSE;
	uint32_t mbox = inmbox;
	mbPackInfo mbInfo;
	MbStat_t st;
	uint32_t *addr = NULL;
	uint32_t ulMbCmd, ulSize, ulSync;

	DisableIrq(MAILBOX_AOCPU_TEE_IRQ);
	PRINT_DBG("[%s]: Teembox 0x%x\n", MBTAG, mbox);
	st = xGetMboxStats(MAILBOX_STAT(mbox));
	addr = xRevAddr(xGetChan(mbox));
	ulMbCmd = st.cmd;
	ulSize = st.size;
	ulSync = st.sync;

	PRINT_DBG("[%s]: %X, %X, %X, %X\n", MBTAG, mbox, ulMbCmd, ulSize, ulSync);

	if (aml_readl32(MAILBOX_STAT(mbox)) == 0) {
		PRINT_DBG("mbox cmd is 0, cannot match\n");
		ClearPendingIrq(MAILBOX_AOCPU_TEE_IRQ);
		EnableIrq(MAILBOX_AOCPU_TEE_IRQ);
		return;
	}

	if (ulSize != 0)
		vGetPayload(addr, &mbInfo.mbdata, ulSize);

	PRINT_DBG("%s taskid: 0x%llx\n", MBTAG, mbInfo.mbdata.taskid);
	PRINT_DBG("%s complete: 0x%llx\n", MBTAG, mbInfo.mbdata.complete);
	PRINT_DBG("%s ullclt: 0x%llx\n", MBTAG, mbInfo.mbdata.ullclt);

	switch (ulSync) {
	case MB_SYNC:
		if (ulTeeSyncTaskWake) {
			PRINT("TeeSyncTaskWake Busy\n");
			break;
		}
		PRINT_DBG("[%s]: SYNC\n", MBTAG);
		ulTeeSyncTaskWake = 1;
		mbInfo.ulCmd = ulMbCmd;
		mbInfo.ulSize = ulSize;
		mbInfo.ulChan = xGetChan(mbox);
		mbmemcpy(&syncTeeMbInfo, &mbInfo, sizeof(syncTeeMbInfo));;
		vTaskNotifyGiveFromISR(mbTeeHandler, NULL);
		//portYIELD_FROM_ISR(xYieldRequired);
		break;
	case MB_ASYNC:
#ifdef AO_MBOX_ONLY_SYNC
		PRINT_DBG("[%s]: ASYNC no support\n", MBTAG);
		vClrMboxStats(MAILBOX_CLR(mbox));
		ClearPendingIrq(MAILBOX_AOCPU_REE_IRQ);
		EnableIrq(MAILBOX_AOCPU_TEE_IRQ);
#endif
		break;
	default:
		PRINT_ERR("[%s]: Not SYNC or ASYNC, Fail\n", MBTAG);
		vClrMboxStats(MAILBOX_CLR(mbox));
		ClearPendingIrq(MAILBOX_AOCPU_TEE_IRQ);
		EnableIrq(MAILBOX_AOCPU_TEE_IRQ);
		break;
	}
}

static void vTeeSyncTask(void *pvParameters)
{
	uint32_t *addr = NULL;
	uint32_t mbox = 0;
	int index = 0;

	pvParameters = pvParameters;
	while (1) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		PRINT_DBG("[%s]:TeeSyncTask\n", MBTAG);

		index = mailbox_htbl_invokeCmd(g_tbl_ao, syncTeeMbInfo.ulCmd,
					       syncTeeMbInfo.mbdata.data);
		mbox = xGetRevMbox(syncTeeMbInfo.ulChan);
		addr = xSendAddrBack(syncTeeMbInfo.ulChan);
		PRINT_DBG("[%s]:MbSyncTask mbox:%d\n", MBTAG, mbox);
		if (index != 0) {
			if (index == MAX_ENTRY_NUM) {
				mbmemset(&syncTeeMbInfo.mbdata.data, 0, sizeof(syncTeeMbInfo.mbdata.data));
				syncTeeMbInfo.mbdata.status = ACK_FAIL;
				vReBuildPayload(addr, &syncTeeMbInfo.mbdata, sizeof(syncTeeMbInfo.mbdata));
				PRINT_DBG("[%s]: undefine cmd or no callback\n", MBTAG);
			} else {
				PRINT_DBG("[%s]:MbSyncTask re len:%d\n", MBTAG, sizeof(syncTeeMbInfo.mbdata));
				syncTeeMbInfo.mbdata.status = ACK_OK;
				vReBuildPayload(addr, &syncTeeMbInfo.mbdata, sizeof(syncTeeMbInfo.mbdata));
			}
		}

		vEnterCritical();
		PRINT_DBG("[%s]:MbSync clear mbox:0x%lx\n", MBTAG, mbox);
		ulTeeSyncTaskWake = 0;
		vClrMboxStats(MAILBOX_CLR(mbox));
		ClearPendingIrq(MAILBOX_AOCPU_TEE_IRQ);
		EnableIrq(MAILBOX_AOCPU_TEE_IRQ);
		vExitCritical();
	}
}



static void vAoRevReeMbHandler(uint32_t inmbox)
{
	//BaseType_t xYieldRequired = pdFALSE;
	uint32_t mbox = inmbox;
	mbPackInfo mbInfo;
	MbStat_t st;
	uint32_t *addr = NULL;
	uint32_t ulMbCmd, ulSize, ulSync;

	DisableIrq(MAILBOX_AOCPU_REE_IRQ);
	PRINT_DBG("[%s]: Reembox 0x%x\n", MBTAG, mbox);
	st = xGetMboxStats(MAILBOX_STAT(mbox));
	addr = xRevAddr(xGetChan(mbox));
	ulMbCmd = st.cmd;
	ulSize = st.size;
	ulSync = st.sync;

	PRINT_DBG("[%s]: %X, %X, %X, %X\n", MBTAG, ulMbCmd, ulSize, ulSync);

	if (aml_readl32(MAILBOX_STAT(mbox)) == 0) {
		PRINT_DBG("mbox cmd is 0, cannot match\n");
		ClearPendingIrq(MAILBOX_AOCPU_REE_IRQ);
		EnableIrq(MAILBOX_AOCPU_REE_IRQ);
		return;
	}

	if (ulSize != 0)
		vGetPayload(addr, &mbInfo.mbdata, ulSize);

	PRINT_DBG("%s taskid: 0x%llx\n", MBTAG, mbInfo.mbdata.taskid);
	PRINT_DBG("%s complete: 0x%llx\n", MBTAG, mbInfo.mbdata.complete);
	PRINT_DBG("%s ullclt: 0x%llx\n", MBTAG, mbInfo.mbdata.ullclt);

	switch (ulSync) {
	case MB_SYNC:
		if (ulReeSyncTaskWake) {
			PRINT("ulReeSyncTaskWake Busy\n");
			break;
		}
		PRINT_DBG("[%s]: SYNC\n", MBTAG);
		ulReeSyncTaskWake = 1;
		mbInfo.ulCmd = ulMbCmd;
		mbInfo.ulSize = ulSize;
		mbInfo.ulChan = xGetChan(mbox);
		mbmemcpy(&syncReeMbInfo, &mbInfo, sizeof(syncReeMbInfo));;
		vTaskNotifyGiveFromISR(mbReeHandler, NULL);
		//portYIELD_FROM_ISR(xYieldRequired);
		break;
	case MB_ASYNC:
#ifdef AO_MBOX_ONLY_SYNC
		PRINT_ERR("[%s]: ASYNC no support\n", MBTAG);
		vClrMboxStats(MAILBOX_CLR(mbox));
		ClearPendingIrq(MAILBOX_AOCPU_REE_IRQ);
		EnableIrq(MAILBOX_AOCPU_REE_IRQ);
#endif
		break;
	default:
		PRINT_ERR("[%s]: Not SYNC or ASYNC, Fail\n", MBTAG);
		vClrMboxStats(MAILBOX_CLR(mbox));
		ClearPendingIrq(MAILBOX_AOCPU_REE_IRQ);
		EnableIrq(MAILBOX_AOCPU_REE_IRQ);
		break;
	}
}

static void vReeSyncTask(void *pvParameters)
{
	uint32_t *addr = NULL;
	uint32_t mbox = 0;
	int index = 0;

	pvParameters = pvParameters;
	while (1) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		PRINT_DBG("[%s]:ReeSyncTask\n", MBTAG);

		index = mailbox_htbl_invokeCmd(g_tbl_ao, syncReeMbInfo.ulCmd,
					       syncReeMbInfo.mbdata.data);
		mbox = xGetRevMbox(syncReeMbInfo.ulChan);
		addr = xSendAddrBack(syncReeMbInfo.ulChan);
		PRINT_DBG("[%s]:MbSyncTask mbox:%d\n", MBTAG, mbox);
		if (index != 0) {
			if (index == MAX_ENTRY_NUM) {
				mbmemset(&syncReeMbInfo.mbdata.data, 0, sizeof(syncReeMbInfo.mbdata.data));
				syncReeMbInfo.mbdata.status = ACK_FAIL;
				vReBuildPayload(addr, &syncReeMbInfo.mbdata, sizeof(syncReeMbInfo.mbdata));
				PRINT_DBG("[%s]: undefine cmd or no callback\n", MBTAG);
			} else {
				PRINT_DBG("[%s]:MbSyncTask re len:%d\n", MBTAG, sizeof(syncReeMbInfo.mbdata));
				syncReeMbInfo.mbdata.status = ACK_OK;
				vReBuildPayload(addr, &syncReeMbInfo.mbdata, sizeof(syncReeMbInfo.mbdata));
			}
		}

		vEnterCritical();
		PRINT_DBG("[%s]:MbSync clear mbox:0x%lx\n", MBTAG, mbox);
		ulReeSyncTaskWake = 0;
		vClrMboxStats(MAILBOX_CLR(mbox));
		ClearPendingIrq(MAILBOX_AOCPU_REE_IRQ);
		EnableIrq(MAILBOX_AOCPU_REE_IRQ);
		vExitCritical();
	}
}


static void vMbHandleReeIsr(void);
static void vMbHandleReeIsr(void)
{
	vAoRevReeMbHandler(MAILBOX_ARMREE2AO);
}

static void vMbHandleTeeIsr(void);
static void vMbHandleTeeIsr(void)
{
	vAoRevTeeMbHandler(MAILBOX_ARMTEE2AO);
}

void vMbInit(void)
{
	PRINT("[%s]: mailbox init start\n", MBTAG);

	mailbox_htbl_init(&g_tbl_ao);

	RegisterIrq(MAILBOX_AOCPU_REE_IRQ, 6, vMbHandleReeIsr);
	RegisterIrq(MAILBOX_AOCPU_TEE_IRQ, 6, vMbHandleTeeIsr);

	EnableIrq(MAILBOX_AOCPU_REE_IRQ);
	EnableIrq(MAILBOX_AOCPU_TEE_IRQ);

	xTaskCreate(vTeeSyncTask,
		    "AoTeeSyncTask",
		    configMINIMAL_STACK_SIZE,
		    0,
		    MHU_MB_TASK_PRIORITIES,
		    (TaskHandle_t *)&mbTeeHandler);

	xTaskCreate(vReeSyncTask,
		    "AoReeSyncTask",
		    configMINIMAL_STACK_SIZE,
		    0,
		    MHU_MB_TASK_PRIORITIES,
		    (TaskHandle_t *)&mbReeHandler);

	vRpcUserCmdInit();
	PRINT("[%s]: mailbox -v1 init end\n", MBTAG);
}

BaseType_t xInstallRemoteMessageCallbackFeedBack(uint32_t ulChan, uint32_t cmd,
						 void *(handler) (void *),
						 uint8_t needFdBak)
{
	VALID_CHANNEL(ulChan);
	UNUSED(ulChan);
	return mailbox_htbl_reg_feedback(g_tbl_ao, cmd, handler, needFdBak);
}

BaseType_t xUninstallRemoteMessageCallback(uint32_t ulChan, int32_t cmd)
{
	UNUSED(ulChan);
	return mailbox_htbl_unreg(g_tbl_ao, cmd);
}
