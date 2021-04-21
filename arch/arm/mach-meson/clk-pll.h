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

struct meson_clk_pll_data {
	char name[16];
	struct parm m;
	struct parm n;
	struct parm rst;
	struct parm l;
	struct parm frac;
	struct parm od;
	struct parm en;
	const struct reg_sequence *init_regs;
	unsigned int init_count;
	unsigned int *def_rate;
	unsigned int def_cnt;
	const struct pll_params_table *table;
	unsigned int smc_id;
	u8 flags;
	u8 secid_disable;
	u8 secid;
	u8 clkmsr_id;
	u8 clkmsr_div16_en;
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

/* mpll data structure */
struct meson_clk_mpll_data {
	char name[8];
	struct parm sdm;
	struct parm sdm_en;
	struct parm n2;
	struct parm ssen;
	struct parm misc;
	struct parm en;
	unsigned int *def_rate;
	unsigned int def_cnt;
	const struct reg_sequence *init_regs;
	unsigned int init_count;
	u8 flags;
	u8 clkmsr_id;
};

#define CLK_MESON_MPLL_ROUND_CLOSEST	BIT(0)
#define CLK_MESON_MPLL_SPREAD_SPECTRUM	BIT(1)

int meson_pll_set_rate(struct meson_clk_pll_data *pll, unsigned long rate);
int meson_pll_set_one_rate(struct meson_clk_pll_data *pll, unsigned long rate);
void meson_pll_test(struct meson_clk_pll_data *pll);
void meson_pll_test_one(struct meson_clk_pll_data *pll);

int meson_secure_pll_set_rate(struct meson_clk_pll_data *pll, unsigned long rate);
void meson_secure_pll_test(struct meson_clk_pll_data *pll);
void meson_secure_pll_disable(struct meson_clk_pll_data *pll);
void meson_clk_pll_disable(struct meson_clk_pll_data *pll);

void meson_switch_cpu_clk(unsigned int smc_id, unsigned int secid, unsigned int flag);

int meson_mpll_set_rate(struct meson_clk_mpll_data *mpll,
			unsigned long rate);
void meson_mpll_test(struct meson_clk_mpll_data *pll);

void one_pll_test(struct meson_clk_pll_data **pll, int pll_cnt,
		struct meson_clk_mpll_data **mpll, int mpll_cnt, char *arg);

#endif /* __MESON_CLK_PLL_H */
