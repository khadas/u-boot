/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __MESON_CLK_PLL_H
#define __MESON_CLK_PLL_H

#include <asm/io.h>
#include <linux/bitops.h>
#include <common.h>
#include <linux/arm-smccc.h>
//#include <linux/delay.h>
#include <asm/arch/timer.h>

#define PLL_ENABLE              BIT(28)
#define PLL_RESET               BIT(29)
#define PLL_LOCK                BIT(31)
#define SETPMASK(width, shift)          GENMASK(shift + width - 1, shift)
#define CLRPMASK(width, shift)          (~SETPMASK(width, shift))

#define PARM_GET(width, shift, reg)                                     \
        (((reg) & SETPMASK(width, shift)) >> (shift))
#define PARM_SET(width, shift, reg, val)                                \
        (((reg) & CLRPMASK(width, shift)) | (val << (shift)))
#define MESON_PARM_APPLICABLE(p)                (!!((p)->width))

struct meson_clk_pll_data;

struct pll_params_table {
	u16		m;
	u16		n;
	u16		od;
};

#define PLL_PARAMS(_m, _n, _od)						\
	{								\
		.m		= (_m),					\
		.n		= (_n),					\
		.od		= (_od),				\
	}

#define CLK_MESON_PLL_ROUND_CLOSEST	BIT(0)

struct parm {
	uint32_t reg;
	uint8_t  shift;
	uint8_t  width;
};

struct reg_sequence {
	unsigned int reg;
	unsigned int def;
	unsigned int mask;
	unsigned int delay_us;
};

/**
 * struct meson_pll_test_ops -  Callback operations for hardware clocks; these are to
 * be provided by the pll test, and will be called by pll test drivers.
 *
 * @pll_prepare: Some PLLs need to do some special preparations before testing.
 *		For example, record the current PLL frequency and restore it after the test.
 *		Not necessary.
 *
 * @pll_unprepare:	The reverse operation of pll_prepare.
 *		Not necessary.
 *
 * @pll_disable: disabled the current PLL
 *		If not implemented, the default meson_clk_pll_disable.
 *
 * @pll_set_rate: Set the frequency of pll according to def_rate.
 *		If not implemented, the default meson_pll_set_rate.
 *
 * @pll_set_parm_rate: Set the frequency of pll according to argv.
 *		If not implemented, the default meson_pll_set_parm_rate.
 *
 * @pll_recalc:	Recalculate the frequency according to the setting of the register
 *		If not implemented, the default meson_clk_pll_recalc_rate.
 *
 * @pll_rate_to_msr: There are some clk device (divider, mux, od) between measuring and pll
 *		Can be described in this api.
 *		Not necessary.
 */
struct meson_pll_test_ops {
	int (*pll_prepare)(struct meson_clk_pll_data *pll);
	void (*pll_unprepare)(struct meson_clk_pll_data *pll);
	void (*pll_disable)(struct meson_clk_pll_data *pll);
	int (*pll_set_rate)(struct meson_clk_pll_data *pll, unsigned long rate);
	int (*pll_set_parm_rate)(struct meson_clk_pll_data *pll, char * const argv[]);
	unsigned long (*pll_recalc)(struct meson_clk_pll_data *pll);
	unsigned long (*pll_rate_to_msr)(struct meson_clk_pll_data *pll, unsigned long rate);
};

struct meson_clk_pll_data {
	char name[16];
	struct parm m;
	struct parm n;
	struct parm rst;
	struct parm l;
	struct parm frac;
	struct parm od;
	struct parm en;
	/* mpll parm */
	struct parm sdm;
	struct parm sdm_en;
	struct parm n2;
	struct parm ssen;
	struct parm misc;
	const struct reg_sequence *init_regs;
	unsigned int init_count;
	unsigned int *def_rate;
	unsigned int def_cnt;
	/* restore pll rate */
	unsigned long store_rate;
	const struct pll_params_table *table;
	const struct meson_pll_test_ops	*ops;
	unsigned int smc_id;
	u8 flags;
	u8 secid_disable;
	u8 secid;
	u8 secid_test;
	u8 clkmsr_id;
	u8 clkmsr_margin;
};

static inline unsigned int meson_parm_read(struct parm *p)
{
	unsigned int val;

	val = readl(p->reg);

	return PARM_GET(p->width, p->shift, val);
}

static inline void meson_parm_write(struct parm *p,
                                    unsigned int val)
{
	unsigned int tmp;

	tmp = readl(p->reg);
	tmp &= CLRPMASK(p->width, p->shift);
	tmp |= val << p->shift;
	writel(tmp, p->reg);
}

#define CLK_MESON_MPLL_ROUND_CLOSEST	BIT(0)
#define CLK_MESON_MPLL_SPREAD_SPECTRUM	BIT(1)

void meson_pll_test(struct meson_clk_pll_data *pll);
void meson_pll_parm_test(struct meson_clk_pll_data *pll, char * const argv[]);
int meson_pll_set_rate(struct meson_clk_pll_data *pll, unsigned long rate);
int meson_pll_set_one_rate(struct meson_clk_pll_data *pll, unsigned long rate);
int meson_pll_set_parm_rate(struct meson_clk_pll_data *pll, char * const argv[]);
int meson_secure_pll_set_rate(struct meson_clk_pll_data *pll, unsigned long rate);
int meson_secure_pll_set_parm_rate(struct meson_clk_pll_data *pll, char * const argv[]);
int meson_mpll_set_rate(struct meson_clk_pll_data *mpll, unsigned long rate);
int meson_mpll_set_parm_rate(struct meson_clk_pll_data *mpll, char * const argv[]);

void meson_clk_pll_disable(struct meson_clk_pll_data *pll);
void meson_secure_pll_disable(struct meson_clk_pll_data *pll);
int meson_pll_store_rate(struct meson_clk_pll_data *pll);
void meson_pll_restore_rate(struct meson_clk_pll_data *pll);
unsigned long meson_mpll_recalc_rate(struct meson_clk_pll_data *mpll);

unsigned long meson_div16_to_msr(struct meson_clk_pll_data *pll, unsigned long rate);
void meson_switch_cpu_clk(unsigned int smc_id, unsigned int secid, unsigned int flag);
struct meson_clk_pll_data *meson_pll_find_by_name(struct meson_clk_pll_data **pll_list,
						  int pll_cnt, char *name);

#endif /* __MESON_CLK_PLL_H */
