// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <dm.h>
#include <misc.h>
#include <errno.h>
#include <dm/pinctrl.h>
#include <asm/arch/bl31_apis.h>
#include <amlogic/jtag.h>

static int meson_jtag_ioctl(struct udevice *dev, unsigned long cmd, void *buf)
{
	int jtag_id = *(int *)buf;
	int jtag_type = JTAG_TYPE_GET(jtag_id);
	int ret;

	switch (cmd) {
		case JTAG_SETPINMUX:
			if (jtag_type == JTAG_A) {
				ret = pinctrl_select_state(dev, "jtag_a_pins");
				if (ret) {
					printf("select state jtag_a_pins failed!\n");
					return -EINVAL;
				}
			} else if (jtag_type == JTAG_B){
				ret = pinctrl_select_state(dev, "jtag_b_pins");
				if (ret) {
					printf("select state jtag_b_pins failed!\n");
					return -EINVAL;
				}
			} else if (jtag_type == SWD_A){
				ret = pinctrl_select_state(dev, "swd_a_pins");
				if (ret) {
					printf("select state swd_a_pins failed!\n");
					return -EINVAL;
				}
			}
			break;
		case JTAG_CLRPINMUX:
			/* nothing to do now */
			break;
		case JTAG_EANBLE:
			aml_set_jtag_state(JTAG_STATE_ON, jtag_id);
			break;
		case JTAG_DISABLE:
			aml_set_jtag_state(JTAG_STATE_OFF, jtag_id);
			break;
	}

	return 0;
}

static const struct misc_ops meson_jtag_ops = {
	.ioctl = meson_jtag_ioctl,
};

static const struct udevice_id meson_jtag_ids[] = {
	{ .compatible = "amlogic, jtag" },
	{}
};

U_BOOT_DRIVER(meson_jtag) = {
	.name = "meson_jtag",
	.id = UCLASS_MISC,
	.of_match = meson_jtag_ids,
	.ops = &meson_jtag_ops,
};
