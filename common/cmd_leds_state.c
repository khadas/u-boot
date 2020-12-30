/*
 * Copyright (c) 2020 Amlogic, Inc
 * Written by jiayi.zhou <jiayi.zhou@amlogic.com>
 */

#include <common.h>
#include <config.h>
#include <command.h>
#include <asm/arch/mailbox.h>
#include "amlogic/leds_state.h"

int32_t do_leds_state(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t data[12];
	uint32_t rev_data, i;

	if (argc < 2)
		return CMD_RET_USAGE;
	for (i = 1; i < argc; i++)
		data[i-1] = (u32)simple_strtoul(argv[i], NULL, 10);
	scpi_send_data(AOCPU_REE_CHANNEL, 0xF7, data, 4*(argc-1), &rev_data, 0);

	return 0;
}

#define BLINK ""

U_BOOT_CMD(
	leds_state, 12, 1, do_leds_state,
	"manage amlogic LEDs",
	"<led_label> on|off|toggle" BLINK "\tChange LED state\n"
	"led [<led_label>\tGet LED state\n"
	"led list\t\tshow a list of LEDs\n"
);
