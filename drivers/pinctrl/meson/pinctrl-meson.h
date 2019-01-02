/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2016 - Beniamino Galvani <b.galvani@gmail.com>
 */

#ifndef __PINCTRL_MESON_H__
#define __PINCTRL_MESON_H__

#include <common.h>
#include <dm.h>
#include <linux/types.h>
#ifdef CONFIG_PINCONF_MESON_G12A
#include "pinconf-meson-g12a.h"
#endif

struct meson_pmx_group {
	const char *name;
	const unsigned int *pins;
	unsigned int num_pins;
	const void *data;
};

struct meson_pmx_func {
	const char *name;
	const char * const *groups;
	unsigned int num_groups;
};

struct meson_pinctrl_data {
	const char *name;
	struct meson_pmx_group *groups;
	struct meson_pmx_func *funcs;
	struct meson_bank *banks;
	unsigned int num_pins;
	unsigned int num_groups;
	unsigned int num_funcs;
	unsigned int num_banks;
	void *pmx_data;
	void *drv_data;
};

struct meson_pinctrl {
	struct meson_pinctrl_data *data;
	void __iomem *reg_mux;
	void __iomem *reg_gpio;
	void __iomem *reg_pull;
	void __iomem *reg_pullen;
	void __iomem *reg_drive;
};

/**
 * struct meson_reg_desc - a register descriptor
 *
 * @reg:	register offset in the regmap
 * @bit:	bit index in register
 *
 * The structure describes the information needed to control pull,
 * pull-enable, direction, etc. for a single pin
 */
struct meson_reg_desc {
	unsigned int reg;
	unsigned int bit;
};

/**
 * enum meson_reg_type - type of registers encoded in @meson_reg_desc
 */
enum meson_reg_type {
	REG_PULLEN,
	REG_PULL,
	REG_DIR,
	REG_OUT,
	REG_IN,
	NUM_REG,
};

/**
 * struct meson bank
 *
 * @name:	bank name
 * @first:	first pin of the bank
 * @last:	last pin of the bank
 * @regs:	array of register descriptors
 *
 * A bank represents a set of pins controlled by a contiguous set of
 * bits in the domain registers. The structure specifies which bits in
 * the regmap control the different functionalities. Each member of
 * the @regs array refers to the first pin of the bank.
 */
struct meson_bank {
	const char *name;
	unsigned int first;
	unsigned int last;
	struct meson_reg_desc regs[NUM_REG];
};

#define FUNCTION(fn)							\
	{								\
		.name = #fn,						\
		.groups = fn ## _groups,				\
		.num_groups = ARRAY_SIZE(fn ## _groups),		\
	}

#define BANK(n, f, l, per, peb, pr, pb, dr, db, or, ob, ir, ib)		\
	{								\
		.name	= n,						\
		.first	= f,						\
		.last	= l,						\
		.regs	= {						\
			[REG_PULLEN]	= { per, peb },			\
			[REG_PULL]	= { pr, pb },			\
			[REG_DIR]	= { dr, db },			\
			[REG_OUT]	= { or, ob },			\
			[REG_IN]	= { ir, ib },			\
		},							\
	 }

extern int meson_pinctrl_get_groups_count(struct udevice *dev);
extern const char *meson_pinctrl_get_group_name(struct udevice *dev,
						unsigned selector);
extern int meson_pinmux_get_functions_count(struct udevice *dev);
extern const char *meson_pinmux_get_function_name(struct udevice *dev,
						unsigned selector);
int meson_pinctrl_probe(struct udevice *dev);


int meson_pinconf_group_set(struct udevice *dev, unsigned int group_selector,
				unsigned int param, unsigned int arg);

#endif /* __PINCTRL_MESON_H__ */
