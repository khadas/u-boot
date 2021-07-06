// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

/*
 * MultiPhase Locked Loops are outputs from a PLL with additional frequency
 * scaling capabilities. MPLL rates are calculated as:
 *
 * f(N2_integer, SDM_IN ) = 2.0G/(N2_integer + SDM_IN/16384)
 */

#include "clk-pll.h"
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/math64.h>
#include <amlogic/clk_measure.h>
#include <div64.h>

#define SDM_DEN 16384
#define N2_MIN	4
#define N2_MAX	511
#define MPLL_PARENT_RATE 2000*1000*1000

static void params_from_rate(unsigned long requested_rate,
			     unsigned int *sdm,
			     unsigned int *n2,
			     u8 flags)
{
	uint64_t div = MPLL_PARENT_RATE;
	uint64_t frac = do_div(div, requested_rate);

	frac *= SDM_DEN;

	if (flags & CLK_MESON_MPLL_ROUND_CLOSEST)
		*sdm = DIV_ROUND_CLOSEST_ULL(frac, requested_rate);
	else
		*sdm = DIV_ROUND_UP_ULL(frac, requested_rate);

	if (*sdm == SDM_DEN) {
		*sdm = 0;
		div += 1;
	}

	if (div < N2_MIN) {
		*n2 = N2_MIN;
		*sdm = 0;
	} else if (div > N2_MAX) {
		*n2 = N2_MAX;
		*sdm = SDM_DEN - 1;
	} else {
		*n2 = div;
	}
}

static long rate_from_params(unsigned int sdm, unsigned int n2)
{
	unsigned long divisor = (SDM_DEN * n2) + sdm;

	if (n2 < N2_MIN)
		return -EINVAL;

	return DIV_ROUND_UP_ULL((u64)2000000000 * SDM_DEN, divisor);
}

static void mpll_init(struct meson_clk_pll_data *mpll)
{
	const struct reg_sequence *init_regs = mpll->init_regs;
	unsigned int i, val;

	for (i = 0; i < mpll->init_count; i++) {
		val = init_regs[i].def;
		writel(val, init_regs[i].reg);
		if (init_regs[i].delay_us)
			_udelay(init_regs[i].delay_us);
	}

	/* Enable the fractional part */
	meson_parm_write(&mpll->sdm_en, 1);

	/* Set spread spectrum if possible */
	if (MESON_PARM_APPLICABLE(&mpll->ssen)) {
		unsigned int ss =
			mpll->flags & CLK_MESON_MPLL_SPREAD_SPECTRUM ? 1 : 0;
		meson_parm_write(&mpll->ssen, ss);
	}

	/* Set the magic misc bit if required */
	if (MESON_PARM_APPLICABLE(&mpll->misc))
		meson_parm_write(&mpll->misc, 1);
}

unsigned long meson_mpll_recalc_rate(struct meson_clk_pll_data *mpll)
{
	unsigned int sdm, n2;
	long rate;

	sdm = meson_parm_read(&mpll->sdm);
	n2 = meson_parm_read(&mpll->n2);

	rate = rate_from_params(sdm, n2);
	return rate < 0 ? 0 : rate;
}

int meson_mpll_set_parm_rate(struct meson_clk_pll_data *mpll, char * const argv[])
{
	const struct reg_sequence *init_regs = mpll->init_regs;
	unsigned int i, val;

	for (i = 0; i < mpll->init_count; i++) {
		val = simple_strtoul(argv[i + 2], NULL, 16);
		writel(val, init_regs[i].reg);
		if (init_regs[i].delay_us)
			_udelay(init_regs[i].delay_us);
	}

	return 0;
}

int meson_mpll_set_rate(struct meson_clk_pll_data *mpll,
			 unsigned long rate)
{
	unsigned int sdm, n2;

	mpll_init(mpll);

	params_from_rate(rate, &sdm, &n2, mpll->flags);

	/* Set the fractional part */
	meson_parm_write(&mpll->sdm, sdm);

	/* Set the integer divider part */
	meson_parm_write(&mpll->n2, n2);

	/* Enable */
	meson_parm_write(&mpll->en, 1);

	_udelay(50);

	return 0;
}

