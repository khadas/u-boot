/*
 * Copyright 2018 Amlogic, Inc
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <dm.h>
#include <misc.h>
#include <errno.h>
#include <dm/pinctrl.h>
#include <asm/arch/bl31_apis.h>

#define SWD_AP_AO               10

static int meson_jtag_ioctl(struct udevice *dev, unsigned long request, void *buf)
{
	int sel = *(int *)buf;
	int ret;

	if (request) { /* set pinmux */
		if (sel == JTAG_A53_AO || sel == JTAG_M3_AO) {
			ret = pinctrl_select_state(dev, "jtag_a_pins");
			if (ret) {
				printf("select state jtag_a_pins failed!\n");
				return -EINVAL;
			}
		} else if (sel == JTAG_A53_EE || sel == JTAG_M3_EE){ /* JTAG_A53_EE/JTAG_M3_EE */
			ret = pinctrl_select_state(dev, "jtag_b_pins");
			if (ret) {
				printf("select state jtag_b_pins failed!\n");
				return -EINVAL;
			}
		} else if (sel == SWD_AP_AO){ /* SWD */
			ret = pinctrl_select_state(dev, "swd_a_pins");
			if (ret) {
				printf("select state swd_a_pins failed!\n");
				return -EINVAL;
			}
		}
	} else { /* clean pinmux */
		if (sel == JTAG_A53_AO || sel == JTAG_M3_AO || sel == SWD_AP_AO) {

		} else if (sel == JTAG_A53_EE || sel == JTAG_M3_EE){ /* JTAG_A53_EE/JTAG_M3_EE */

		} else if (sel == SWD_AP_AO) {

		}
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
