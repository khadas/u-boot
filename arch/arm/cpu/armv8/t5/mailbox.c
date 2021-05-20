/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/t5/mailbox.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <asm/arch/io.h>
#include <asm/arch/regs.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/mailbox.h>

//DEFINE_BAKERY_LOCK(mhu_lock);

void open_scp_log(unsigned int channel)
{
	printf("[BL33]: mbox no open_scp_log\n");
	return;
}
int thermal_calibration(unsigned int type, unsigned int data)
{
	printf("[BL33]: mbox no thermal_calibration\n");
	return 0;
}
int thermal_get_value(unsigned int sensor_id, unsigned int *value)
{
	printf("[BL33]: mbox no aml_get_value\n");
	return 0;
};
void send_pwm_delt(int32_t vcck_delt, int32_t ee_delt)
{
	printf("[BL33]: mbox no end_pwm_delt\n");
	return;
}

void init_dsp(unsigned int id,unsigned int addr,unsigned int cfg0,
	      unsigned int jtag_ctrl,unsigned int cfg1)
{
	printf("[BL33]: mbox no init_dsp\n");
	return;
}

void init_dsp_jtag(unsigned int id)
{
	printf("[BL33]: mbox no init_dsp_jtag\n");
	return;
}

int mhu_get_addr(uint32_t chan, uintptr_t *mboxset_addr,
                        uintptr_t *mboxstat_addr, uintptr_t *mboxpl_addr)
{
        switch (chan) {
        case HIFIA_REE_CHANNEL:
        case HIFIB_REE_CHANNEL:
        case SECPU_REE_CHANNEL:
                printf("[BL33]: no support hifi or secpu channnel\n");
                break;
        case AOCPU_REE_CHANNEL:
                *mboxset_addr = REE2AO_SET_ADDR;
                *mboxstat_addr = REE2AO_STS_ADDR;
                *mboxpl_addr = REE2AO_PL_ADDR;
                break;
        default:
                printf("[BL33]: no support channnel 0x%x\n", chan);
                break;
        };
        return 0;
}

void mhu_message_start(uintptr_t mboxstat_addr)
{
        //bakery_lock_get(&mhu_secure_lock);

        /* Make sure any previous command has finished */
        while (readl(mboxstat_addr) != 0);
}

void mhu_message_send(uintptr_t mboxset_addr, uint32_t command, uint32_t size)
{
        uint32_t mbox_cmd;

        //if (size < 0 || size > (MHU_PAYLOAD_SIZE - MHU_DATA_OFFSET)) {  //coverity error
        if (size > (MHU_PAYLOAD_SIZE - MHU_DATA_OFFSET)) {
                printf("[BL33]: scpi send input size error\n");
                return;
        }
        mbox_cmd = MHU_CMD_BUILD(command, size + MHU_DATA_OFFSET);
        /* Send command to HIFI and wait for it to pick it up */
        writel(mbox_cmd, mboxset_addr);
}

void mhu_build_payload(uintptr_t mboxpl_addr, void *message, uint32_t size)
{
        //if (size < 0 || size > (MHU_PAYLOAD_SIZE - MHU_DATA_OFFSET)) {  //coverity error
        if (size > (MHU_PAYLOAD_SIZE - MHU_DATA_OFFSET)) {
                printf("[BL33]: scpi send input size error\n");
                return;
        }
        memset((void *)mboxpl_addr, 0, MHU_PAYLOAD_SIZE);
        memcpy((char *)mboxpl_addr + MHU_DATA_OFFSET, message, size);
}

void mhu_get_payload(uintptr_t mboxpl_addr, void *message, uint32_t size)
{
        //if (size < 0 || size > (MHU_PAYLOAD_SIZE - MHU_DATA_OFFSET)) { //coverity error
        if (size > (MHU_PAYLOAD_SIZE - MHU_DATA_OFFSET)) {
                printf("[BL33]: scpi revsize input size error\n");
                return;
        }
        memcpy(message, (char *)mboxpl_addr + MHU_DATA_OFFSET, size);
        memset((void *)mboxpl_addr, 0, MHU_PAYLOAD_SIZE);
}

uint32_t mhu_message_wait(uintptr_t mboxstat_addr)
{
        /* Wait for response from HIFI */
        uint32_t response;
        while ((response = readl(mboxstat_addr)));

        return response;
}

void mhu_message_end(uintptr_t mboxpl_addr)
{
        memset((void *)mboxpl_addr, 0x0, MHU_PAYLOAD_SIZE);

        //bakery_lock_release(&mhu_lock);
}

void mhu_init(void)
{
        //bakery_lock_init(&mhu_lock);

        /*
         * Clear the CPU's INTR register to make sure we don't see a stale
         * or garbage value and think it's a message we've already sent.
         */
        writel(REE2AO_CLR_ADDR, 0xffffffffu);
}

void scpi_send_data(uint32_t chan, uint32_t command, void *sendmessage,
		    uint32_t sendsize, void *revmessage, uint32_t revsize)
{
        uintptr_t mboxset_addr = 0;
        uintptr_t mboxstat_addr = 0;
        uintptr_t mboxpl_addr = 0;
        int ret;

        ret = mhu_get_addr(chan, &mboxset_addr, &mboxstat_addr, &mboxpl_addr);
        if (ret) {
                printf("[BL33] mhu pl get addr fail\n");
                return;
        }
        mhu_message_start(mboxstat_addr);
	if (sendmessage != NULL && sendsize != 0)
		mhu_build_payload(mboxpl_addr, sendmessage, sendsize);
        mhu_message_send(mboxset_addr, command, sendsize);
        mhu_message_wait(mboxstat_addr);
	if (revmessage != NULL && revsize != 0)
		mhu_get_payload(mboxpl_addr, revmessage, revsize);
        mhu_message_end(mboxpl_addr);
}
