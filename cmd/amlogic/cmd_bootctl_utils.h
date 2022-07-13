/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __CMD_BOOTCTL_UTILS_H
#define __CMD_BOOTCTL_UTILS_H

enum BOOT_MODE {
	BOOT_MODE_NORMAL = 1,
	BOOT_MODE_AVB = 2,
	BOOT_MODE_VAB = 3,
};

int aml_get_boot_mode(void);
int boot_info_open_partition(char *miscbuf);

#endif
