// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <dm.h>
#include <misc.h>
#include <errno.h>
#include <dm/read.h>
#include "ir.h"

static int meson_ir_read(struct udevice *dev, int offset, void *buf, int size)
{
	int code;
	int idx;
	struct meson_ir_chip *chip = dev_get_priv(dev);

	if (!buf)
		return -EINVAL;

	for (idx = 0; idx < chip->nr_contr; idx++) {
		if (chip->ir_contr[idx].get_framecode) {
			code = chip->ir_contr[idx].get_framecode(chip);
			if (code) {
				*(int *)buf = code;
				return 0;
			}
		}
	}

	return -EINVAL;
}

static int meson_ir_probe(struct udevice *dev)
{
	int idx;
	fdt_addr_t addr;
	fdt_addr_t size;
	struct meson_ir_chip *chip;

	chip = dev_get_priv(dev);

	chip->protocol = dev_read_u32_default(dev, "protocol", -EINVAL);
	if (!chip->protocol) {
		printf("failed to get protocol: %d\n", chip->protocol);
		return -EINVAL;
	}

	chip->nr_contr = ENABLE_LEGACY_IR(chip->protocol) ? IR_ID_MAX : LEGACY_IR_ID;

	for (idx = 0; idx < chip->nr_contr; idx++) {
		addr = devfdt_get_addr_size_index(dev, idx, &size);
		if (addr == FDT_ADDR_T_NONE) {
			printf("failed to get address: %d\n", idx);
			return -EINVAL;
		}
		chip->ir_contr[idx].base = (void __iomem *)addr;
	}

	return meson_ir_register_default_config(chip, chip->protocol);
}

static const struct misc_ops meson_ir_ops = {
	.read = meson_ir_read,
};

static const struct udevice_id meson_ir_ids[] = {
	{ .compatible = "amlogic, aml_remote" },
	{ .compatible = "amlogic, meson-ir"   },
	{}
};

U_BOOT_DRIVER(meson_ir) = {
	.name     = "meson-ir",
	.id       = UCLASS_MISC,
	.of_match = meson_ir_ids,
	.ops      = &meson_ir_ops,
	.probe    = meson_ir_probe,
	.priv_auto_alloc_size = sizeof(struct meson_ir_chip),
};
