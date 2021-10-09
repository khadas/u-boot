
/*
 *  Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 *  All information contained herein is Amlogic confidential.
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <util.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <stdio.h>
#include "myprintf.h"

#include <unistd.h>
#include "n200_func.h"
#include "uart.h"
#include "common.h"
#include "riscv_encoding.h"

#include "mailbox-api.h"

#define MBTAG "AOCPU"
#define PRINT_DBG(...)  printf(__VA_ARGS__)
#define PRINT_ERR(...)  printf(__VA_ARGS__)
#define PRINT(...)	printf(__VA_ARGS__)

/*ARM 2 AOCPU mailbox*/
void vRpcUserCmdInit(void);

struct Uintcase {
	char data[20];
	uint32_t ulTaskDelay;
};

static inline void *mbmemset(void *dst, int val, size_t count)
{
	char *ptr = dst;

	while (count--)
		*ptr++ = val;

	return dst;
}

static inline void *mbmemcpy(void *dst, const void *src, size_t len)
{
	const char *s = src;
	char *d = dst;

	while (len--)
		*d++ = *s++;

	return dst;
}

static void xMboxUintReeTestCase(void *msg)
{
	struct Uintcase *pdata = msg;
	char back[20] = "Response AOCPU\n";

	PRINT("[%s]: scpi %s\n", MBTAG, pdata->data);
	mbmemset(msg, 0, MBOX_BUF_LEN);
	mbmemcpy(msg, back, sizeof(back));

	PRINT("[%s]: delay after %ld\n", MBTAG, pdata->ulTaskDelay);

}

static void xMboxUintTeeTestCase(void *msg)
{
	char *s = msg;

	PRINT("[%s]: from tee: %s\n", MBTAG, s);

}

static void vRegisterRpcCallBack(void)
{
	int ret;

	ret = xInstallRemoteMessageCallbackFeedBack(AOREE_CHANNEL, MBX_CMD_RPCUINTREE_TEST,
						    (void *)xMboxUintReeTestCase, 1);
	if (ret == MBOX_CALL_MAX)
		PRINT("[%s]: mbox cmd 0x%x register fail\n",MBTAG, MBX_CMD_RPCUINTREE_TEST);

	ret = xInstallRemoteMessageCallbackFeedBack(AOTEE_CHANNEL, MBX_CMD_RPCUINTTEE_TEST,
						    (void *)xMboxUintTeeTestCase, 0);
	if (ret == MBOX_CALL_MAX)
		PRINT("[%s]: mbox cmd 0x%x register fail\n", MBTAG, MBX_CMD_RPCUINTTEE_TEST);
}

void vRpcUserCmdInit(void)
{
        vRegisterRpcCallBack();
}
