// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <command.h>
#include <asm/arch/mailbox.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/stick_mem.h>

//stick reboot flag saved in aocpu local sram, and can
//be fetched from mailbox
uint32_t stick_reboot_flag;

uint32_t wdt_status_check(uint32_t *p_stick_mem)
{
	uint32_t wdt_flag, stick_mem_flag1, stick_mem_flag2;

	stick_mem_flag1 = *(p_stick_mem + STICK_FLAG_1);
	stick_mem_flag2 = *(p_stick_mem + STICK_FLAG_2);
	if (stick_mem_flag1 != STICK_MEM_FLAG_1 ||
	    stick_mem_flag2 != STICK_MEM_FLAG_2)
		return 0;

	//The STICK_REBOOT_FLAG stored in AOCPU local sram
	wdt_flag = *(p_stick_mem + STICK_REBOOT_FLAG);
	return wdt_flag;
}

void get_stick_reboot_flag(void)
{
	uint32_t ret;

	ret = scpi_send_data(AOCPU_REE_CHANNEL, CMD_GET_STICK_REBOOT_FLAG, NULL, 0,
			     &stick_reboot_flag, 4);
	if (ret != 0)
		printf("\r\nstick_reboot_flag communication failed\n");
	else
		printf("\r\nstick_reboot_flag = 0x%x\n", stick_reboot_flag);
}
