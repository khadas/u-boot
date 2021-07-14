/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_keypad.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <command.h>
#include <asm/arch/mailbox.h>

/*
 * add a gpio/adc key for keypad
 *
 * key_id: unique key id
 *		GPIO KEY: ID(0 <= id < 512) from BL30 gpio-date.h
 *		ADC KEY : ID(id >= 512)
 *		Remove Keypad: ID = 0xFFFFFFFF
 *
 * status: response status
 *		GPIO KEY: 0(Low level), 1(High level)
 *		ADC KEY : voltage value(12 bits)
 *
 * channel: saradc channel, only for adc key
 *
 * event  : key event, 1 - short press, 2 - long press
 */

static int do_keypad_set(cmd_tbl_t *cmdtp, int flag, int argc,
			 char * const argv[])
{
	uint32_t key_info[4];
	int i;

	if (argc != 5) {
		printf("illegal argc nums\n");
		return -1;
	}

	for (i = 0; i < 4; i++)
		key_info[i] =  simple_strtoul(argv[i + 1], NULL, 10);

	scpi_send_data(AOCPU_REE_CHANNEL, CMD_SET_KEYPAD,
		       key_info, sizeof(key_info), NULL, 0);

	return 0;
}

static cmd_tbl_t cmd_keypad_sub[] = {
	U_BOOT_CMD_MKENT(set, 6, 0, do_keypad_set, "", ""),
};

static int do_keypad(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *c;
	/* Strip off leading 'bmp' command argument */
	argc--;
	argv++;
	c = find_cmd_tbl(argv[0], &cmd_keypad_sub[0],
			 ARRAY_SIZE(cmd_keypad_sub));
	if (c) {
		return	c->cmd(cmdtp, flag, argc, argv);
	} else {
		cmd_usage(cmdtp);
		return 1;
	}
}

U_BOOT_CMD(
	keypad, 6, 0, do_keypad,
	"set aocpu keypad",
	"set <key_id> <status> <channel> <event>"
);
