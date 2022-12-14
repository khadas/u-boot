// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <dm.h>
#include <errno.h>
#include <dt-bindings/input/meson_rc.h>
#include "ir.h"

struct meson_ir_reg_map {
	unsigned int reg;
	unsigned int val;
};

struct meson_ir_reg_proto {
	int protocol;
	int reg_map_size;
	const char *name;
	const struct meson_ir_reg_map *reg_map;
	int (*get_framecode)(struct meson_ir_chip *chip);
};

static const struct meson_ir_reg_map regs_legacy_nec[] = {
	{REG_LDR_ACTIVE,    (500 << 16) | (400 << 0)},
	{REG_LDR_IDLE,      (300 << 16) | (200 << 0)},
	{REG_LDR_REPEAT,    (150 << 16) | (80 << 0)},
	{REG_BIT_0,         (72 << 16) | (40 << 0) },
	{REG_REG0,          (7 << 28) | (0xFA0 << 12) | 0x13},
	{REG_STATUS,        (134 << 20) | (90 << 10)},
	{REG_REG1,          0xbe00}
};

static const struct meson_ir_reg_map regs_default_nec[] = {
	{REG_LDR_ACTIVE,   (500 << 16) | (400 << 0)},
	{REG_LDR_IDLE,     (300 << 16) | (200 << 0)},
	{REG_LDR_REPEAT,   (150 << 16) | (80 << 0)},
	{REG_BIT_0,        (72 << 16) | (40 << 0)},
	{REG_REG0,         (7 << 28) | (0xFA0 << 12) | 0x13},
	{REG_STATUS,       (134 << 20) | (90 << 10) | (1 << 30)},
	{REG_REG1,         0x9f00},
	{REG_REG2,         0x00},
	{REG_DURATN2,      0x00},
	{REG_DURATN3,      0x00},
	{REG_IRQ_CTL,      0xCFA10BB8}
};

static const struct meson_ir_reg_map regs_default_duokan[] = {
	{REG_LDR_ACTIVE,   ((70 << 16) | (30 << 0))},
	{REG_LDR_IDLE,     ((50 << 16) | (15 << 0))},
	{REG_LDR_REPEAT,   ((30 << 16) | (26 << 0))},
	{REG_BIT_0,        ((66 << 16) | (40 << 0))},
	{REG_REG0,         ((3 << 28) | (0x4e2 << 12) | (0x13))},
	{REG_STATUS,       ((80 << 20) | (66 << 10))},
	{REG_REG1,         0x9300},
	{REG_REG2,         0xb90b},
	{REG_DURATN2,      ((97 << 16) | (80 << 0))},
	{REG_DURATN3,      ((120 << 16) | (97 << 0))},
	{REG_REG3,         5000 << 0}
};

static const struct meson_ir_reg_map regs_default_xmp_1[] = {
	{REG_LDR_ACTIVE,   0},
	{REG_LDR_IDLE,     0},
	{REG_LDR_REPEAT,   0},
	{REG_BIT_0,        (52 << 16) | (45 << 0)},
	{REG_REG0,         ((7 << 28) | (0x5DC << 12) | (0x13))},
	{REG_STATUS,       (87 << 20) | (80 << 10)},
	{REG_REG1,         0x9f00},
	{REG_REG2,         0xa90e},
	/*n=10,758+137*10=2128us,2128/20= 106*/
	{REG_DURATN2,      (121 << 16) | (114 << 0)},
	{REG_DURATN3,      (7 << 16) | (7 << 0)},
	{REG_REG3,         0}
};

static const struct meson_ir_reg_map regs_default_rc5[] = {
	{REG_LDR_ACTIVE,   0},
	{REG_LDR_IDLE,     0},
	{REG_LDR_REPEAT,   0},
	{REG_BIT_0,        0},
	{REG_REG0,         ((3 << 28) | (0x1644 << 12) | 0x13)},
	{REG_STATUS,       (1 << 30)},
	{REG_REG1,         ((1 << 15) | (13 << 8))},
	/*bit[0-3]: RC5; bit[8]: MSB first mode; bit[11]: compare frame method*/
	{REG_REG2,         ((1 << 13) | (1 << 11) | (1 << 8) | 0x7)},
	/*Half bit for RC5 format: 888.89us*/
	{REG_DURATN2,      ((49 << 16) | (40 << 0))},
	/*RC5 typically 1777.78us for whole bit*/
	{REG_DURATN3,      ((94 << 16) | (83 << 0))},
	{REG_REG3,         0}
};

static const struct meson_ir_reg_map regs_default_rc6[] = {
	{REG_LDR_ACTIVE,    (210 << 16) | (125 << 0)},
	/*rc6 leader 4000us,200* timebase*/
	{REG_LDR_IDLE,      (50 << 16) | (38 << 0)}, /* leader idle 400*/
	{REG_LDR_REPEAT,    (145 << 16) | (125 << 0)}, /* leader repeat*/
	/* logic '0' or '00' 1500us*/
	{REG_BIT_0,         (51 << 16) | (38 << 0) },
	{REG_REG0,          (7 << 28) | (0xFA0 << 12) | 0x13},
	/* sys clock body time.base time = 20 body frame*/
	{REG_STATUS,        (94 << 20) | (82 << 10)},
	/*20bit:9440 32bit:9f40 36bit:a340 37bit:a440*/
	{REG_REG1,          0xa440},
	/*it may get the wrong customer value and key value from register if
	 *the value is set to 0x4,so the register value must set to 0x104
	 */
	{REG_REG2,          0x2909},
	{REG_DURATN2,       ((28 << 16) | (16 << 0))},
	{REG_DURATN3,       ((51 << 16) | (38 << 0))}
};

static const struct meson_ir_reg_map regs_default_toshiba[] = {
	{REG_LDR_ACTIVE,   (280 << 16) | (180 << 0)},
	{REG_LDR_IDLE,     (280 << 16) | (180 << 0)},
	{REG_LDR_REPEAT,   (150 << 16) | (60 << 0)},
	{REG_BIT_0,        (72 << 16) | (40 << 0)},
	{REG_REG0,         (7 << 28) | (0xFA0 << 12) | 0x13},
	{REG_STATUS,       (134 << 20) | (90 << 10)},
	{REG_REG1,         0x9f00},
	{REG_REG2,         0x05 | (1 << 24) | (23 << 11)},
	{REG_DURATN2,      0x00},
	{REG_DURATN3,      0x00,},
	{REG_REPEAT_DET,   (1 << 31) | (0xFA0 << 16) | (10 << 0)},
	{REG_REG3,         0x2AF8}
};

static struct meson_ir_reg_map regs_default_rca[] = {
	{REG_LDR_ACTIVE,   (250 << 16) | (160 << 0)},
	{REG_LDR_IDLE,     (250 << 16) | (160 << 0)},
	{REG_LDR_REPEAT,   (250 << 16) | (160 << 0)},
	{REG_BIT_0,        (100 << 16) | (48 << 0)},
	{REG_REG0,         (7 << 28) | (0xFA0 << 12) | 0x13},
	{REG_STATUS,       (150 << 20) | (110 << 10)},
	{REG_REG1,         0x9700},
	{REG_REG2,         0x104 | (1 << 24) | (23 << 11)},
	{REG_DURATN2,      0x00},
	{REG_REPEAT_DET,   (1 << 31) | (0xFA0 << 16) | (10 << 0)},
	{REG_REG3,         0x1A00},
	{REG_DURATN3,      0x00}
};

static int meson_ir_get_framecode(struct meson_ir_chip *chip, unsigned char id)
{
	int status;

	status = readl(chip->ir_contr[id].base + REG_STATUS);
	if (status & FRAME_STATUS_VALID) {
		if (status & FRAME_STATUS_REPEAT) {
			readl(chip->ir_contr[id].base + REG_FRAME);
			return 0;
		}

		return readl(chip->ir_contr[id].base + REG_FRAME);
	}

	return 0;
}

static int meson_legacy_ir_get_framecode_default(struct meson_ir_chip *chip)
{
	return meson_ir_get_framecode(chip, LEGACY_IR_ID);
}

static int meson_multi_ir_get_framecode_default(struct meson_ir_chip *chip)
{
	return meson_ir_get_framecode(chip, MULTI_IR_ID);
}

static const struct meson_ir_reg_proto reg_legacy_nec ={
	.protocol	= REMOTE_TYPE_LEGACY_NEC,
	.name		= "LEGACY_NEC",
	.reg_map	= regs_legacy_nec,
	.reg_map_size	= ARRAY_SIZE(regs_legacy_nec),
	.get_framecode	= meson_legacy_ir_get_framecode_default,
};

static const struct meson_ir_reg_proto reg_nec ={
	.protocol	= REMOTE_TYPE_NEC,
	.name		= "NEC",
	.reg_map	= regs_default_nec,
	.reg_map_size	= ARRAY_SIZE(regs_default_nec),
	.get_framecode	= meson_multi_ir_get_framecode_default,
};

static const struct meson_ir_reg_proto reg_duokan ={
	.protocol	= REMOTE_TYPE_DUOKAN,
	.name		= "DUOKAN",
	.reg_map	= regs_default_duokan,
	.reg_map_size	= ARRAY_SIZE(regs_default_duokan),
	.get_framecode	= meson_multi_ir_get_framecode_default,
};

static const struct meson_ir_reg_proto reg_xmp_1 ={
	.protocol	= REMOTE_TYPE_XMP_1,
	.name		= "XMP_1",
	.reg_map	= regs_default_xmp_1,
	.reg_map_size	= ARRAY_SIZE(regs_default_xmp_1),
	.get_framecode	= meson_multi_ir_get_framecode_default,
};

static const struct meson_ir_reg_proto reg_rc5 ={
	.protocol	= REMOTE_TYPE_RC5,
	.name		= "RC5",
	.reg_map	= regs_default_rc5,
	.reg_map_size	= ARRAY_SIZE(regs_default_rc5),
	.get_framecode	= meson_multi_ir_get_framecode_default,
};

static const struct meson_ir_reg_proto reg_rc6 ={
	.protocol	= REMOTE_TYPE_RC6,
	.name		= "RC6",
	.reg_map	= regs_default_rc6,
	.reg_map_size	= ARRAY_SIZE(regs_default_rc6),
};

static const struct meson_ir_reg_proto reg_toshiba ={
	.protocol	= REMOTE_TYPE_TOSHIBA,
	.name		= "toshiba",
	.reg_map	= regs_default_toshiba,
	.reg_map_size	= ARRAY_SIZE(regs_default_toshiba),
	.get_framecode	= meson_multi_ir_get_framecode_default,
};

static const struct meson_ir_reg_proto reg_rca ={
	.protocol	= REMOTE_TYPE_RCA,
	.name		= "RCA",
	.reg_map	= regs_default_rca,
	.reg_map_size	= ARRAY_SIZE(regs_default_rca),
	.get_framecode	= meson_multi_ir_get_framecode_default,
};

static const struct meson_ir_reg_proto *meson_ir_regs_proto[] = {
	&reg_nec,
	&reg_duokan,
	&reg_rc5,
	&reg_rc6,
	&reg_toshiba,
	&reg_rca,
	&reg_xmp_1,
	&reg_legacy_nec,
	NULL
};

static int meson_ir_contr_init(struct meson_ir_chip *chip, int type, unsigned char id)
{
	const struct meson_ir_reg_proto **reg_proto = meson_ir_regs_proto;
	const struct meson_ir_reg_map *regmap;
	int size;

	for (; (*reg_proto) != NULL ; ) {
		if ((*reg_proto)->protocol == type)
			break;
		reg_proto++;
	}

	if (!(*reg_proto)) {
		printf("%s: invalid protocol type: %d\n", __func__, type);
		return -EINVAL;
	}

	readl(chip->ir_contr[id].base + REG_STATUS);
	readl(chip->ir_contr[id].base + REG_FRAME);

	/*
	 * reset ir decoder and disable the state machine
	 * of IR decoder.
	 * [15] = 0 ,disable the machine of IR decoder
	 * [0] = 0x01,set to 1 to reset the IR decoder
	 */
	regmap = (*reg_proto)->reg_map;
	size   = (*reg_proto)->reg_map_size;

	for ( ; size > 0; ) {
		writel(regmap->val, chip->ir_contr[id].base + regmap->reg);
		size--;
		regmap++;
	}

	/*
	 * when we reinstall remote controller register,
	 * we need reset IR decoder, set 1 to REG_REG1 bit0,
	 * after IR decoder reset, we need to clear the bit0
	 */
	writel(readl(chip->ir_contr[id].base + REG_REG1) | BIT(0),
	       chip->ir_contr[id].base + REG_REG1);
	writel(readl(chip->ir_contr[id].base + REG_REG1) & (~BIT(0)),
	       chip->ir_contr[id].base + REG_REG1);

	chip->ir_contr[id].get_framecode  = (*reg_proto)->get_framecode;

	return 0;
}

int meson_ir_register_default_config(struct meson_ir_chip *chip, int type)
{
	if (ENABLE_LEGACY_IR(type)) {
		/*initialize registers for legacy IR controller*/
		meson_ir_contr_init(chip, LEGACY_IR_TYPE_MASK(type),
				    LEGACY_IR_ID);
	} else {
		/*disable legacy IR controller: REG_REG1[15]*/
		writel(0x0, chip->ir_contr[LEGACY_IR_ID].base + REG_REG1);
	}
	/*initialize registers for Multi-format IR controller*/
	meson_ir_contr_init(chip, MULTI_IR_TYPE_MASK(type), MULTI_IR_ID);

	return 0;
}
