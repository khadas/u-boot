// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <command.h>
#include <malloc.h>
#include <asm/arch/mailbox.h>
#include <asm/arch/secure_apb.h>

#define aml_writel32(value, reg)	writel(value, reg)
#define aml_readl32(reg)		readl(reg)

static inline void mbwrite(uint32_t to, void *from, long count)
{
	int i = 0;
	int len = count / 4 + (count % 4);
	uint32_t *p = from;

	while (len > 0) {
		aml_writel32(p[i], to + (4 * i));
		len--;
		i++;
	}
}

static inline void mbclean(uint32_t to, long count)
{
	int i = 0;
	int len = count / 4 + (count % 4);

	while (len > 0) {
		aml_writel32(0, to + (4 * i));
		len--;
		i++;
	}
}

int mhu_get_addr(uint32_t chan, uint32_t *mboxset, uint32_t *mboxsts,
		 uintptr_t *mboxpl, uint32_t *mboxwr, uint32_t *mboxrd,
		 uint32_t *mboxirqclr, uint32_t *mboxid)
{
	int ret = 0;

	switch (chan) {
	case AOCPU_REE_CHANNEL:
		*mboxset = REE2AO_SET_ADDR;
		*mboxsts = REE2AO_STS_ADDR;
		*mboxwr = REE2AO_WR_ADDR;
		*mboxrd = REE2AO_RD_ADDR;
		*mboxirqclr = REE2AO_IRQCLR_ADDR;
		*mboxid = REE2AO_MBOX_ID;
		break;
	default:
		printf("[BL33]: no support chan 0x%x\n", chan);
		ret = -1;
		break;
	};
	return ret;
}

void mhu_message_start(uint32_t mboxsts)
{
	/* Make sure any previous command has finished */
	while (aml_readl32(mboxsts) != 0)
		;
}

void mhu_build_payload(uintptr_t mboxpl, uint32_t mboxwr, void *message, uint32_t size)
{
	if (size > (MHU_PAYLOAD_SIZE - MHU_DATA_OFFSET)) {
		printf("bl33: scpi send input size error\n");
		return;
	}
	if (size == 0)
		return;
	mbwrite(mboxwr + MHU_DATA_OFFSET, message, size);
}

void mhu_get_payload(uintptr_t mboxpl, uint32_t mboxwr, void *message, uint32_t size)
{
	if (size > (MHU_PAYLOAD_SIZE - MHU_DATA_OFFSET)) {
		printf("bl33: scpi send input size error\n");
		return;
	}
	if (size == 0)
		return;
	printf("bl33: scpi no support get revmessage\n");
}

void mhu_message_send(uint32_t mboxset, uint32_t command, uint32_t size)
{
	uint32_t mbox_cmd;

	mbox_cmd = MHU_CMD_BUILD(command, size + MHU_DATA_OFFSET);
	aml_writel32(mbox_cmd, mboxset);
}

uint32_t mhu_message_wait(uint32_t mboxsts)
{
	/* Wait for response from HIFI */
	uint32_t response;

	while ((response = aml_readl32(mboxsts)))
		;

	return response;
}

void mhu_message_end(uintptr_t mboxpl, uint32_t mboxwr, uint32_t mboxirqclr, uint32_t mboxid)
{
	mbclean(mboxwr, MHU_PAYLOAD_SIZE);
	/* Clear any response we got by writing all ones to the CLEAR register */
	aml_writel32(MHU_ACK_MASK(mboxid), mboxirqclr);
}

void mhu_init(void)
{
	aml_writel32(0xffffffffu, REE2AO_CLR_ADDR);
	printf("[BL33] mhu init done -v2\n");
}

int scpi_send_data(uint32_t chan, uint32_t command,
		   void *sendmessage, uint32_t sendsize, void *revmessage, uint32_t revsize)
{
	uint32_t mboxset = 0;
	uint32_t mboxsts = 0;
	uintptr_t mboxpl = 0;
	uint32_t mboxwr = 0;
	uint32_t mboxrd = 0;
	uint32_t mboxirq = 0;
	uint32_t mboxid = 0;
	int ret = 0;

	ret = mhu_get_addr(chan, &mboxset, &mboxsts, &mboxpl, &mboxwr, &mboxrd, &mboxirq, &mboxid);
	if (ret) {
		printf("bl33: mhu get addr fail\n");
		return ret;
	}
	mhu_message_start(mboxsts);
	if (sendmessage && sendsize != 0)
		mhu_build_payload(mboxpl, mboxwr, sendmessage, sendsize);
	mhu_message_send(mboxset, command, sendsize);
	mhu_message_wait(mboxsts);
	if (revmessage && revsize != 0)
		mhu_get_payload(mboxpl, mboxrd, revmessage, revsize);
	mhu_message_end(mboxpl, mboxwr, mboxirq, mboxid);
	return ret;
}
