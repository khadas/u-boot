/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/t5/gpio.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <dm.h>
#include <linux/compiler.h>
#include <aml_gpio.h>
#include <asm/arch/gpio.h>

struct pin_mux_desc {
	unsigned char domain;
	unsigned char reg;
	unsigned char bit;
};

#define MUX_AO_DOMAIN 0
#define MUX_EE_DOMAIN 1

#define PIN_MUX(d, r, b)     \
{                            \
	.domain = d,         \
	.reg    = r,         \
	.bit    = b,         \
}

#define BANK(n, f, l, per, peb, pr, pb, dr, db, or, ob, ir, ib)		\
{									\
	.name	= n,							\
	.first	= f,							\
	.last	= l,							\
	.regs	= {							\
		[REG_PULLEN]	= { (0xff634520 + (per<<2)), peb },	\
		[REG_PULL]	= { (0xff6344e8 + (pr<<2)), pb },	\
		[REG_DIR]	= { (0xff634440 + (dr<<2)), db },	\
		[REG_OUT]	= { (0xff634440 + (or<<2)), ob },	\
		[REG_IN]	= { (0xff634440 + (ir<<2)), ib },	\
	},								\
 }
#define AOBANK(n, f, l, per, peb, pr, pb, dr, db, or, ob, ir, ib)	\
{									\
	.name	= n,							\
	.first	= f,							\
	.last	= l,							\
	.regs	= {							\
		[REG_PULLEN]	= { (0xff800024 + (per<<2)), peb },	\
		[REG_PULL]	= { (0xff800024 + (pr<<2)), pb },	\
		[REG_DIR]	= { (0xff800024 + (dr<<2)), db },	\
		[REG_OUT]	= { (0xff800024 + (or<<2)), ob },	\
		[REG_IN]	= { (0xff800024 + (ir<<2)), ib },	\
	},								\
 }

static struct pin_mux_desc pin_to_gpio[] = {
	[GPIOAO(GPIOD_0)] = PIN_MUX(MUX_AO_DOMAIN, 0x0, 0),
	[GPIOAO(GPIOD_1)] = PIN_MUX(MUX_AO_DOMAIN, 0x0, 4),
	[GPIOAO(GPIOD_2)] = PIN_MUX(MUX_AO_DOMAIN, 0x0, 8),
	[GPIOAO(GPIOD_3)] = PIN_MUX(MUX_AO_DOMAIN, 0x0, 12),
	[GPIOAO(GPIOD_4)] = PIN_MUX(MUX_AO_DOMAIN, 0x0, 16),
	[GPIOAO(GPIOD_5)] = PIN_MUX(MUX_AO_DOMAIN, 0x0, 20),
	[GPIOAO(GPIOD_6)] = PIN_MUX(MUX_AO_DOMAIN, 0x0, 24),
	[GPIOAO(GPIOD_7)] = PIN_MUX(MUX_AO_DOMAIN, 0x0, 28),
	[GPIOAO(GPIOD_8)] = PIN_MUX(MUX_AO_DOMAIN, 0x1, 0),
	[GPIOAO(GPIOD_9)] = PIN_MUX(MUX_AO_DOMAIN, 0x1, 4),
	[GPIOAO(GPIOD_10)] = PIN_MUX(MUX_AO_DOMAIN, 0x1, 8),
	[GPIOAO(GPIOE_0)] = PIN_MUX(MUX_AO_DOMAIN, 0x1, 16),
	[GPIOAO(GPIOE_1)] = PIN_MUX(MUX_AO_DOMAIN, 0x1, 20),
	[GPIOAO(GPIOE_2)] = PIN_MUX(MUX_AO_DOMAIN, 0x1, 24),
	[GPIOEE(GPIOB_0)] = PIN_MUX(MUX_EE_DOMAIN, 0x0, 0),
	[GPIOEE(GPIOB_1)] = PIN_MUX(MUX_EE_DOMAIN, 0x0, 4),
	[GPIOEE(GPIOB_2)] = PIN_MUX(MUX_EE_DOMAIN, 0x0, 8),
	[GPIOEE(GPIOB_3)] = PIN_MUX(MUX_EE_DOMAIN, 0x0, 12),
	[GPIOEE(GPIOB_4)] = PIN_MUX(MUX_EE_DOMAIN, 0x0, 16),
	[GPIOEE(GPIOB_5)] = PIN_MUX(MUX_EE_DOMAIN, 0x0, 20),
	[GPIOEE(GPIOB_6)] = PIN_MUX(MUX_EE_DOMAIN, 0x0, 24),
	[GPIOEE(GPIOB_7)] = PIN_MUX(MUX_EE_DOMAIN, 0x0, 28),
	[GPIOEE(GPIOB_8)] = PIN_MUX(MUX_EE_DOMAIN, 0x1, 0),
	[GPIOEE(GPIOB_9)] = PIN_MUX(MUX_EE_DOMAIN, 0x1, 4),
	[GPIOEE(GPIOB_10)] = PIN_MUX(MUX_EE_DOMAIN, 0x1, 8),
	[GPIOEE(GPIOB_11)] = PIN_MUX(MUX_EE_DOMAIN, 0x1, 12),
	[GPIOEE(GPIOB_12)] = PIN_MUX(MUX_EE_DOMAIN, 0x1, 16),
	[GPIOEE(GPIOB_13)] = PIN_MUX(MUX_EE_DOMAIN, 0x1, 20),
	[GPIOEE(GPIOW_0)] = PIN_MUX(MUX_EE_DOMAIN, 0x2, 0),
	[GPIOEE(GPIOW_1)] = PIN_MUX(MUX_EE_DOMAIN, 0x2, 4),
	[GPIOEE(GPIOW_2)] = PIN_MUX(MUX_EE_DOMAIN, 0x2, 8),
	[GPIOEE(GPIOW_3)] = PIN_MUX(MUX_EE_DOMAIN, 0x2, 12),
	[GPIOEE(GPIOW_4)] = PIN_MUX(MUX_EE_DOMAIN, 0x2, 16),
	[GPIOEE(GPIOW_5)] = PIN_MUX(MUX_EE_DOMAIN, 0x2, 20),
	[GPIOEE(GPIOW_6)] = PIN_MUX(MUX_EE_DOMAIN, 0x2, 24),
	[GPIOEE(GPIOW_7)] = PIN_MUX(MUX_EE_DOMAIN, 0x2, 28),
	[GPIOEE(GPIOW_8)] = PIN_MUX(MUX_EE_DOMAIN, 0x3, 0),
	[GPIOEE(GPIOW_9)] = PIN_MUX(MUX_EE_DOMAIN, 0x3, 4),
	[GPIOEE(GPIOW_10)] = PIN_MUX(MUX_EE_DOMAIN, 0x3, 8),
	[GPIOEE(GPIOW_11)] = PIN_MUX(MUX_EE_DOMAIN, 0x3, 12),
	[GPIOEE(GPIOW_12)] = PIN_MUX(MUX_EE_DOMAIN, 0x3, 16),
	[GPIOEE(GPIOZ_0)] = PIN_MUX(MUX_EE_DOMAIN, 0x4, 0),
	[GPIOEE(GPIOZ_1)] = PIN_MUX(MUX_EE_DOMAIN, 0x4, 4),
	[GPIOEE(GPIOZ_2)] = PIN_MUX(MUX_EE_DOMAIN, 0x4, 8),
	[GPIOEE(GPIOZ_3)] = PIN_MUX(MUX_EE_DOMAIN, 0x4, 12),
	[GPIOEE(GPIOZ_4)] = PIN_MUX(MUX_EE_DOMAIN, 0x4, 16),
	[GPIOEE(GPIOZ_5)] = PIN_MUX(MUX_EE_DOMAIN, 0x4, 20),
	[GPIOEE(GPIOZ_6)] = PIN_MUX(MUX_EE_DOMAIN, 0x4, 24),
	[GPIOEE(GPIOH_0)] = PIN_MUX(MUX_EE_DOMAIN, 0x5, 0),
	[GPIOEE(GPIOH_1)] = PIN_MUX(MUX_EE_DOMAIN, 0x5, 4),
	[GPIOEE(GPIOH_2)] = PIN_MUX(MUX_EE_DOMAIN, 0x5, 8),
	[GPIOEE(GPIOH_3)] = PIN_MUX(MUX_EE_DOMAIN, 0x5, 12),
	[GPIOEE(GPIOH_4)] = PIN_MUX(MUX_EE_DOMAIN, 0x5, 16),
	[GPIOEE(GPIOH_5)] = PIN_MUX(MUX_EE_DOMAIN, 0x5, 20),
	[GPIOEE(GPIOH_6)] = PIN_MUX(MUX_EE_DOMAIN, 0x5, 24),
	[GPIOEE(GPIOH_7)] = PIN_MUX(MUX_EE_DOMAIN, 0x5, 28),
	[GPIOEE(GPIOH_8)] = PIN_MUX(MUX_EE_DOMAIN, 0x6, 0),
	[GPIOEE(GPIOH_9)] = PIN_MUX(MUX_EE_DOMAIN, 0x6, 4),
	[GPIOEE(GPIOH_10)] = PIN_MUX(MUX_EE_DOMAIN, 0x6, 8),
	[GPIOEE(GPIOH_11)] = PIN_MUX(MUX_EE_DOMAIN, 0x6, 12),
	[GPIOEE(GPIOH_12)] = PIN_MUX(MUX_EE_DOMAIN, 0x6, 16),
	[GPIOEE(GPIOH_13)] = PIN_MUX(MUX_EE_DOMAIN, 0x6, 20),
	[GPIOEE(GPIOH_14)] = PIN_MUX(MUX_EE_DOMAIN, 0x6, 24),
	[GPIOEE(GPIOH_15)] = PIN_MUX(MUX_EE_DOMAIN, 0x6, 28),
	[GPIOEE(GPIOH_16)] = PIN_MUX(MUX_EE_DOMAIN, 0x7, 0),
	[GPIOEE(GPIOH_17)] = PIN_MUX(MUX_EE_DOMAIN, 0x7, 4),
	[GPIOEE(GPIOH_18)] = PIN_MUX(MUX_EE_DOMAIN, 0x7, 8),
	[GPIOEE(GPIOH_19)] = PIN_MUX(MUX_EE_DOMAIN, 0x7, 12),
	[GPIOEE(GPIOH_20)] = PIN_MUX(MUX_EE_DOMAIN, 0x7, 16),
	[GPIOEE(GPIOH_21)] = PIN_MUX(MUX_EE_DOMAIN, 0x7, 20),
};

/* sequence of banks keep same as arch-t5/gpio.h */

static struct meson_bank mesont5_banks[] = {
	/*name   first  last   pullen   pull   dir   out  in*/
	AOBANK("GPIOD_", GPIOAO(GPIOD_0), GPIOAO(GPIOD_10),
	3,  0, 2,  0,  0,  0,  4,  0, 1,  0),
	AOBANK("GPIOE_", GPIOAO(GPIOE_0), GPIOAO(GPIOE_2),
	3,  16, 2,  16,  0,  16,  4,  16, 1,  16),
	BANK("GPIOH_", GPIOEE(GPIOH_0), GPIOEE(GPIOH_21),
	2, 0,  2,  0,  6,  0, 7, 0, 8, 0),
	BANK("GPIOB_", GPIOEE(GPIOB_0), GPIOEE(GPIOB_13),
	0, 0,  0,  0,  0,  0,  1, 0,  2, 0),
	BANK("GPIOZ_", GPIOEE(GPIOZ_0), GPIOEE(GPIOZ_6),
	1, 0,  1,  0,  3,  0,  4, 0,  5, 0),
	BANK("GPIOW_", GPIOEE(GPIOW_0), GPIOEE(GPIOW_12),
	3, 0,  3,  0,  9,  0,  10, 0,  11, 0),
};

U_BOOT_DEVICES(t5_gpios) = {
	{ "gpio_aml", &mesont5_banks[0] },
	{ "gpio_aml", &mesont5_banks[1] },
	{ "gpio_aml", &mesont5_banks[2] },
	{ "gpio_aml", &mesont5_banks[3] },
	{ "gpio_aml", &mesont5_banks[4] },
	{ "gpio_aml", &mesont5_banks[5] },
};

static unsigned long domain[]={
	[MUX_AO_DOMAIN] = 0xff800014,
	[MUX_EE_DOMAIN] = 0xff6346c0,
};

int clear_pinmux(unsigned int pin)
{
	struct pin_mux_desc *pmux_desc = &pin_to_gpio[pin];

	regmap_update_bits(domain[pmux_desc->domain] + (pmux_desc->reg << 2),
			0xf << (pmux_desc->bit), 0 << (pmux_desc->bit));

	return 0;
}

#ifdef CONFIG_AML_SPICC
#include <asm/arch/secure_apb.h>
/* generic pins control for spicc0.
 * if deleted, you have to add it into all t5 board files as necessary.
 * GPIOH_9:  MISO:reg6[7:4]  =2
 * GPIOH_10: MOSI:reg6[11:8] =2
 * GPIOH_11: CLK: reg6[15:12]=2
 */
int spicc0_pinctrl_enable(bool enable)
{
	unsigned int val;

	val = readl(P_PERIPHS_PIN_MUX_6);
	val &= ~(0xfff << 4);
	if (enable)
		val |= 0x222 << 4;
	writel(val, P_PERIPHS_PIN_MUX_6);
	return 0;
}
#endif /* CONFIG_AML_SPICC */
