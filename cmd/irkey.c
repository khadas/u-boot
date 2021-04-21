// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <dm.h>
#include <asm/io.h>
#include <common.h>
#include <command.h>
#include <errno.h>
#include <misc.h>

#define KEY_PARAMS_NUM  10
#define OTHER_PARAMS_NUM 1

extern uint32_t get_time(void);

static int do_irkey(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t key_buf[KEY_PARAMS_NUM];
	uint32_t time_out = 0;
	uint32_t time_base = 0;
	struct udevice *ir_devp;
	uint32_t key;
	char *endp;
	char str[16] = {0};
	u8 i;

	/*at least set a key*/
	if (argc < 3)
		return -EINVAL;

	/*obtain timeout time*/
	time_out = simple_strtoul(argv[1], &endp, 0);
	printf("time_out = %d\n",time_out);
	if (*argv[1] == 0 || *endp != 0)
		return -EINVAL;

	/*obtain IR keys value which need to detect*/
	for (i=2; i<argc; i++) {
		key_buf[i-2] = simple_strtoul(argv[i], &endp, 0);
		printf("key[%d] = %x\n",i-2, key_buf[i-2]);
		if (*argv[i] == 0 || *endp != 0)
			return -EINVAL;
	}

	if (uclass_get_device_by_name(UCLASS_MISC, "meson-ir", &ir_devp)) {
		printf("failed to get ir udevice\n");
		return -EINVAL;
	}

	time_base = get_time();

	while ((get_time() - time_base) < time_out)
	{
		if (misc_read(ir_devp, 0, &key, 1))
			continue;

		printf("keycode = %x\n",key);
		for (i=2; i<argc; i++) {
			if (key == key_buf[i-2]) {
				snprintf(str, sizeof(str), "0x%x", key);
				env_set("irkey_value", str);
				return 0;
			}
		}
	}

	return -EINVAL;
}
/*Maxium key arguments: 10*/
U_BOOT_CMD(
	irkey, (KEY_PARAMS_NUM + OTHER_PARAMS_NUM + 1), 0, do_irkey,
	"irkey <timeout> <key1> ...<keyN> - maximum value of N: 10",
	NULL
);
