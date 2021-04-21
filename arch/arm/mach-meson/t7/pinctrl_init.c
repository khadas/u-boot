// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm/uclass.h>
#include <linux/err.h>
#include <asm/arch/pinctrl_init.h>

int pinctrl_devices_active(int pinctrl_num)
{
	int ret;
	int idx;
	struct udevice *pinctrl;

	for (idx = 0; idx < pinctrl_num; idx++) {
		ret = uclass_get_device(UCLASS_PINCTRL, idx, &pinctrl);
		if (ret) {
			debug("%s: failed to active pinctrl device [%d]",
					__func__, idx);
			return -EINVAL;
		}
	}

	return 0;
}
