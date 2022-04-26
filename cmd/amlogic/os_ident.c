// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <android_image.h>

//#define OS_IDENT_DEBUG

#ifdef OS_IDENT_DEBUG
#define debug_print(...) printf(__VA_ARGS__)
#else
#define debug_print(...) ((void)0)
#endif

static int do_os_ident(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = -1;

	if (argc < 2) {
		printf("Err! OS hdr addr not specified!\n");
		return ret;
	}

	const void *img_addr = (const void *)simple_strtoul(argv[1], NULL, 16);
	debug_print("os hdr addr: 0x%lx\n", (ulong)img_addr);

	ret = genimg_get_format(img_addr);
	switch (ret) {
		case IMAGE_FORMAT_LEGACY:
			debug_print("IMAGE_FORMAT_LEGACY format\n");
			env_set("os_type", "rtos");
			break;
		case IMAGE_FORMAT_FIT:
			debug_print("IMAGE_FORMAT_FIT format\n");
			/* ignore fdt format, it's not an OS */
			//env_set("os_type", "fdt");
			break;
		case IMAGE_FORMAT_ANDROID:
			debug_print("IMAGE_FORMAT_ANDROID format\n");
			env_set("os_type", "kernel");
			break;
		case IMAGE_FORMAT_INVALID:
			debug_print("IMAGE_FORMAT_INVALID format\n");
			env_set("os_type", "invalid");
			break;
		default:
			debug_print("default format\n");
			break;
	}

	return ret;
}

U_BOOT_CMD(
	os_ident,	CONFIG_SYS_MAXARGS,	1,	do_os_ident,
	"identify OS type",
	"[addr]\n"
);
