/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/bitops.h>
#include <linux/io.h>

enum MESON_IR_REG {
	REG_LDR_ACTIVE		= 0x00 << 2,
	REG_LDR_IDLE		= 0x01 << 2,
	REG_LDR_REPEAT		= 0x02 << 2,
	REG_BIT_0		= 0x03 << 2,
	REG_REG0		= 0x04 << 2,
	REG_FRAME		= 0x05 << 2,
	REG_STATUS		= 0x06 << 2,
	REG_REG1		= 0x07 << 2,
	REG_REG2		= 0x08 << 2,
	REG_DURATN2		= 0x09 << 2,
	REG_DURATN3		= 0x0a << 2,
	REG_FRAME1		= 0x0b << 2,
	REG_STATUS1		= 0x0c << 2,
	REG_STATUS2		= 0x0d << 2,
	REG_REG3		= 0x0e << 2,
	REG_FRAME_RSV0		= 0x0f << 2,
	REG_FRAME_RSV1		= 0x10 << 2,
	REG_FILTE		= 0x11 << 2,
	REG_IRQ_CTL		= 0x12 << 2,
	REG_FIFO_CTL		= 0x13 << 2,
	REG_WIDTH_NEW		= 0x14 << 2,
	REG_REPEAT_DET		= 0x15 << 2,
};

enum IR_CONTR_NUMBER {
	MULTI_IR_ID = 0,
	LEGACY_IR_ID,
	IR_ID_MAX
};

struct meson_ir_chip;
struct meson_ir_contr {
	void __iomem *base;
	int (*get_framecode)(struct meson_ir_chip *chip);
};

struct meson_ir_chip {
	unsigned char nr_contr;
	int protocol;
	struct meson_ir_contr ir_contr[IR_ID_MAX];
};

#define MULTI_IR_TYPE_MASK(type) (type & 0xff)  /*8bit*/
#define LEGACY_IR_TYPE_MASK(type) ((type >> 8) & 0xff) /*8bit*/
#define ENABLE_LEGACY_IR(type) (LEGACY_IR_TYPE_MASK(type) == 0xff)

#define FRAME_STATUS_VALID	BIT(3)
#define FRAME_STATUS_REPEAT	BIT(0)

int meson_ir_register_default_config(struct meson_ir_chip *chip, int type);
