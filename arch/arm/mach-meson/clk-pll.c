// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

/*
 * In the most basic form, a Meson PLL is composed as follows:
 *
 *                     PLL
 *        +--------------------------------+
 *        |                                |
 *        |             +--+               |
 *  in >>-----[ /N ]--->|  |      +-----+  |
 *        |             |  |------| DCO |---->> out
 *        |  +--------->|  |      +--v--+  |
 *        |  |          +--+         |     |
 *        |  |                       |     |
 *        |  +--[ *(M + (F/Fmax) ]<--+     |
 *        |                                |
 *        +--------------------------------+
 *
 * out = in * (m + frac / frac_max) / n
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/math64.h>
#include <div64.h>
#include "clk-pll.h"
#include <amlogic/clk_measure.h>
#include <asm/arch/timer.h>

#define PARENT_RATE 		24000000
//#define MESON_PLL_DEBUG	/* pll debug macro */

static int __pll_round_closest_mult(struct meson_clk_pll_data *pll)
{
	if ((pll->flags & CLK_MESON_PLL_ROUND_CLOSEST) &&
	    !MESON_PARM_APPLICABLE(&pll->frac))
		return 1;

	return 0;
}

static unsigned long __pll_params_to_rate(unsigned int m, unsigned int n,
					  unsigned int frac,
					  struct meson_clk_pll_data *pll,
					  unsigned int od)
{
	u64 rate = (u64)PARENT_RATE * m;
	u64 frac_rate;

	if (frac && MESON_PARM_APPLICABLE(&pll->frac)) {
		frac_rate = (u64)PARENT_RATE * frac;
		if ((pll->frac.width - 2) < 0)
			return -EINVAL;
		if (frac & (1 << (pll->frac.width - 1)))
			rate -= DIV_ROUND_UP_ULL(frac_rate,
						 (1 << (pll->frac.width - 2)));
		else
			rate += DIV_ROUND_UP_ULL(frac_rate,
						 (1 << (pll->frac.width - 2)));
	}

	if (n == 0)
		return 0;

	return DIV_ROUND_UP_ULL(rate, n) >> od;
}

static unsigned int __pll_params_with_frac(unsigned long rate,
					   unsigned int m,
					   unsigned int n,
					   struct meson_clk_pll_data *pll)
{
	unsigned int frac_max = (1 << pll->frac.width);
	u64 val = (u64)rate * n;

	/* Bail out if we are already over the requested rate */
	if (rate < PARENT_RATE * m / n)
		return 0;

	if (pll->flags & CLK_MESON_PLL_ROUND_CLOSEST)
		val = DIV_ROUND_CLOSEST_ULL(val * frac_max, PARENT_RATE);
	else
		val = div_u64(val * frac_max, PARENT_RATE);

	val -= (u64)m * frac_max;

	return min((unsigned int)val, (frac_max - 1));
}

static bool meson_clk_pll_is_better(unsigned long rate,
				    unsigned long best,
				    unsigned long now,
				    struct meson_clk_pll_data *pll)
{
	if (__pll_round_closest_mult(pll)) {
		/* Round Closest */
		if (abs(now - rate) < abs(best - rate))
			return true;
	} else {
		/* Round down */
		if (now <= rate && best < now)
			return true;
	}

	return false;
}

static int meson_clk_get_pll_table_index(unsigned int index,
					 unsigned int *m,
					 unsigned int *n,
					 struct meson_clk_pll_data *pll,
					 unsigned int *od)
{
	if (!pll->table[index].n)
		return -EINVAL;

	*m = pll->table[index].m;
	*n = pll->table[index].n;
	*od = pll->table[index].od;

	return 0;
}

static int meson_clk_get_pll_get_index(unsigned long rate,
				       unsigned int index,
				       unsigned int *m,
				       unsigned int *n,
				       struct meson_clk_pll_data *pll,
				       unsigned int *od)
{
	/* only support table in arm32 */
	if (pll->table)
		return meson_clk_get_pll_table_index(index, m, n, pll, od);

	return -EINVAL;
}

static int meson_clk_get_pll_settings(unsigned long rate,
				      unsigned int *best_m,
				      unsigned int *best_n,
				      struct meson_clk_pll_data *pll,
				      unsigned int *best_od)
{
	unsigned long best = 0, now = 0;
	unsigned int i, m, n, od;
	int ret;

	for (i = 0, ret = 0; !ret; i++) {
		ret = meson_clk_get_pll_get_index(rate,
						  i, &m, &n, pll, &od);
		if (ret == -EINVAL)
			break;

		now = __pll_params_to_rate(m, n, 0, pll, od);
		if (meson_clk_pll_is_better(rate, best, now, pll)) {
			best = now;
			*best_m = m;
			*best_n = n;
			*best_od = od;

			if (now == rate)
				break;
		}
	}

	return best ? 0 : -EINVAL;
}

static unsigned long meson_clk_pll_recalc_rate(struct meson_clk_pll_data *pll)
{
	unsigned int m, n, frac, od;

	n = meson_parm_read(&pll->n);
	m = meson_parm_read(&pll->m);
	od = MESON_PARM_APPLICABLE(&pll->od) ?
		meson_parm_read(&pll->od) :
		0;

	frac = MESON_PARM_APPLICABLE(&pll->frac) ?
		meson_parm_read(&pll->frac) :
		0;

	return __pll_params_to_rate(m, n, frac, pll, od);
}

static int meson_clk_pll_wait_lock(struct meson_clk_pll_data *pll)
{
	int delay = 1000;

	do {
		/* There may be no lock status check for some plls */
		if (!pll->l.reg) {
			_udelay(1000);
			return 0;
		}

		/* Is the clock locked now ? */
		if (meson_parm_read(&pll->l))
			return 0;
		_udelay(1);
	} while (delay--);

	return -ETIMEDOUT;
}

static void meson_pll_report(struct meson_clk_pll_data *pll, unsigned long target,
			     unsigned long msr_rate, unsigned long res,
			     unsigned int margin)
{
	if (!target) {
		printf("invalid target\n");
		return;
	}

	if (!msr_rate) {
		printf("invalid msr rate\n");
		return;
	}

	if (((msr_rate - margin) <= res) && (res <= (msr_rate + margin))) {
		printf("%s PLL lock ok, target rate=%luM div=%ld clkmsr rate=%luM: Match\n",
		       pll->name, target, target / msr_rate, res);
		printf("%s pll test pass\n", pll->name);
	} else {
		printf("%s PLL lock failed, target rate=%luM div=%ld clkmsr rate=%luM: Not Match\n",
		       pll->name, target, target / msr_rate, res);
		printf("%s pll test failed\n", pll->name);
	}
}

void meson_switch_cpu_clk(unsigned int smc_id, unsigned int secid, unsigned int flag)
{
	struct arm_smccc_res res;

	arm_smccc_smc(smc_id, secid,
	0x1 << 11, flag << 11,
	0, 0, 0, 0, &res);
}

unsigned long meson_div16_to_msr(struct meson_clk_pll_data *pll, unsigned long rate)
{
	/* msr is div16 */
	return rate >> 4;
}

int meson_pll_store_rate(struct meson_clk_pll_data *pll)
{
	if (meson_parm_read(&pll->en)) {
		/* Record the current pll frequency when pll is enabled */
		if (pll->ops->pll_recalc)
			pll->store_rate = pll->ops->pll_recalc(pll);
		else /* default */
			pll->store_rate = meson_clk_pll_recalc_rate(pll);
	} else {
		pll->store_rate = 0;
	}

	return 0;
}

void meson_pll_restore_rate(struct meson_clk_pll_data *pll)
{
	if (!pll->store_rate) {
		/* disabled pll */
		if (pll->ops->pll_disable)
			pll->ops->pll_disable(pll);
		else
			meson_clk_pll_disable(pll);
		return;
	}

	if (pll->ops->pll_set_rate)
		pll->ops->pll_set_rate(pll, pll->store_rate);
	else /* default */
		meson_pll_set_rate(pll, pll->store_rate);
}

void meson_clk_pll_disable(struct meson_clk_pll_data *pll)
{
	/* Put the pll is in reset */
	setbits_le32(pll->rst.reg, 1 << pll->rst.shift);
	/* Disable the pll */
	clrbits_le32(pll->en.reg, 1 << pll->en.shift);
}

void meson_secure_pll_disable(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	arm_smccc_smc(pll->smc_id, pll->secid_disable,
			      0, 0, 0, 0, 0, 0, &res);
}

int meson_secure_pll_set_parm_rate(struct meson_clk_pll_data *pll, char * const argv[])
{
	struct arm_smccc_res res;
	unsigned int enabled, i, val, j = 10;
	const struct reg_sequence *init_regs = pll->init_regs;

	if (PARENT_RATE == 0)
		return -EINVAL;

	enabled = meson_parm_read(&pll->en);
	if (enabled)
		meson_secure_pll_disable(pll);

	do {
		/* run the same sequence provided by vlsi */
		for (i = 0; i < pll->init_count; i++) {
			val = simple_strtoul(argv[i + 2], NULL, 16);
			arm_smccc_smc(pll->smc_id, pll->secid_test, init_regs[i].reg, val,
				      init_regs[i].delay_us, 0, 0, 0, &res);
		}
		/* start test */
		arm_smccc_smc(pll->smc_id, pll->secid_test, 0xff, 0xff, 0xff, 0, 0, 0, &res);
		if (meson_clk_pll_wait_lock(pll))
			printf("%s pll did not lock, retrying %d\n", pll->name, 10 - j);
		else
			break;
		j--;
	} while (j);

	return 0;
}

int meson_secure_pll_set_rate(struct meson_clk_pll_data *pll, unsigned long rate)
{
	struct arm_smccc_res res;
	unsigned int enabled, m, n, od, ret = 0;

	if (PARENT_RATE == 0 || rate == 0)
		return -EINVAL;

	ret = meson_clk_get_pll_settings(rate, &m, &n, pll, &od);
	if (ret)
		return ret;

	enabled = meson_parm_read(&pll->en);
	if (enabled)
		meson_secure_pll_disable(pll);
	/*Send m,n for arm64 */
	arm_smccc_smc(pll->smc_id, pll->secid,
			      m, n, od, 0, 0, 0, &res);

	if (meson_clk_pll_wait_lock(pll))
		printf("%s pll did not lock\n", pll->name);

	return 0;
}

int meson_pll_set_parm_rate(struct meson_clk_pll_data *pll, char * const argv[])
{
	unsigned int enabled, i, val, j = 10;
	const struct reg_sequence *init_regs;

	if (PARENT_RATE == 0 || !pll)
		return -EINVAL;

	init_regs = pll->init_regs;
	enabled = meson_parm_read(&pll->en);
	if (enabled)
		meson_clk_pll_disable(pll);

	do {
		/* run the same sequence provided by vlsi */
		for (i = 0; i < pll->init_count; i++) {
			//val = init_regs[i].def;
			val = simple_strtoul(argv[i + 2], NULL, 16);
			writel(val, init_regs[i].reg);
			if (init_regs[i].delay_us)
				_udelay(init_regs[i].delay_us);
		}

		if (meson_clk_pll_wait_lock(pll))
			printf("%s pll did not lock, retrying %d\n", pll->name, 10 - j);
		else
			break;
		j--;
	} while (j);

	return 0;
}

int meson_pll_set_one_rate(struct meson_clk_pll_data *pll, unsigned long rate)
{
	unsigned int enabled, i, val, j = 10;
	const struct reg_sequence *init_regs = pll->init_regs;

	if (!PARENT_RATE || !rate)
		return -EINVAL;

	enabled = meson_parm_read(&pll->en);
	if (enabled)
		meson_clk_pll_disable(pll);

	do {
		/* run the same sequence provided by vlsi */
		for (i = 0; i < pll->init_count; i++) {
			val = init_regs[i].def;
			writel(val, init_regs[i].reg);
			if (init_regs[i].delay_us)
				_udelay(init_regs[i].delay_us);
		}

		if (meson_clk_pll_wait_lock(pll))
			printf("%s pll did not lock, retrying %d\n", pll->name, 10 - j);
		else
			break;
		j--;
	} while (j);

	return 0;
}

int meson_pll_set_rate(struct meson_clk_pll_data *pll, unsigned long rate)
{
	unsigned int enabled, m, n, frac = 0, od, i, ret, val;
	struct parm *pm = &pll->m;
	struct parm *pn = &pll->n;
	struct parm *pod = &pll->od;
	struct parm *pfrac = &pll->frac;
	const struct reg_sequence *init_regs = pll->init_regs;

	if (PARENT_RATE == 0 || rate == 0) {
		printf("%s, target rate is invalid\n", __func__);
		return -EINVAL;
	}

	ret = meson_clk_get_pll_settings(rate, &m, &n, pll, &od);
	if (ret)
		return ret;

	if (MESON_PARM_APPLICABLE(&pll->frac))
		frac = __pll_params_with_frac(rate, m, n, pll);

#ifdef MESON_PLL_DEBUG
	printf("pll set rate: %s trate = %lu, m = %d, n = %d, od = %d\n",
	       pll->name, rate, m, n, od);
#endif
	enabled = meson_parm_read(&pll->en);
	if (enabled)
		meson_clk_pll_disable(pll);

	/* run the same sequence provided by vlsi */
	for (i = 0; i < pll->init_count; i++) {
		if (pn->reg == init_regs[i].reg) {
			/* Clear M N Vbits and Update M N value */
			val = init_regs[i].def;
			val &= CLRPMASK(pn->width, pn->shift);
			val &= CLRPMASK(pm->width, pm->shift);
			val &= CLRPMASK(pod->width, pod->shift);
			val |= n << pn->shift;
			val |= m << pm->shift;
			val |= od << pod->shift;
			writel(val, pn->reg);
		} else if (pfrac->reg == init_regs[i].reg &&
				(MESON_PARM_APPLICABLE(&pll->frac))) {
			/* Clear Frac bits and Update Frac value */
			val = init_regs[i].def;
			val &= CLRPMASK(pfrac->width, pfrac->shift);
			val |= frac << pfrac->shift;
			writel(val, pfrac->reg);
		} else {
			val = init_regs[i].def;
			writel(val, init_regs[i].reg);
		}
		if (init_regs[i].delay_us)
			_udelay(init_regs[i].delay_us);
	}

	if (meson_clk_pll_wait_lock(pll))
		printf("%s pll did not lock, retrying?\n", pll->name);

	return 0;
}

void meson_pll_test(struct meson_clk_pll_data *pll)
{
	int i;
	unsigned long result, rate;

	if (!pll || !pll->ops)
		return;

	if (pll->ops && pll->ops->pll_prepare)
		pll->ops->pll_prepare(pll);

	for (i = 0; i < pll->def_cnt; i++) {
		if (pll->ops->pll_set_rate)
			pll->ops->pll_set_rate(pll, (unsigned long)pll->def_rate[i] * 1000000);
		else
			meson_pll_set_rate(pll, (unsigned long)pll->def_rate[i] * 1000000);

		if (pll->ops->pll_rate_to_msr)
			rate = pll->ops->pll_rate_to_msr(pll, (unsigned long)pll->def_rate[i]);
		else
			rate = (unsigned long)pll->def_rate[i];

		result = clk_util_clk_msr(pll->clkmsr_id);
		meson_pll_report(pll, (unsigned long)pll->def_rate[i],
				 rate, result, pll->clkmsr_margin);
	}

	if (pll->ops && pll->ops->pll_unprepare)
		pll->ops->pll_unprepare(pll);
}

void meson_pll_parm_test(struct meson_clk_pll_data *pll, char * const argv[])
{
	unsigned long result, target, rate;

	if (!pll || !pll->ops)
		return;

	/* some pll need prepare before test */
	if (pll->ops && pll->ops->pll_prepare)
		pll->ops->pll_prepare(pll);

	/* set rate according to argv */
	if (pll->ops && pll->ops->pll_set_parm_rate)
		pll->ops->pll_set_parm_rate(pll, argv);
	else /* default */
		meson_pll_set_parm_rate(pll, argv);

	/* recalc rate */
	if (pll->ops->pll_recalc)
		target = pll->ops->pll_recalc(pll);
	else /* default */
		target = meson_clk_pll_recalc_rate(pll);

	/*
	 * There are some clk device (divider, mux, od) between measuring and pll.
	 * We can describe in pll_rate_to_msr.
	 */
	if (pll->ops->pll_rate_to_msr)
		rate = pll->ops->pll_rate_to_msr(pll, target);
	else
		rate = target;

	/* clk msr */
	result = clk_util_clk_msr(pll->clkmsr_id);

	/* report */
	meson_pll_report(pll, target ? (target / 1000000) : 0, rate ? (rate / 1000000) : 0,
			 result, pll->clkmsr_margin);

	/* unprepare */
	if (pll->ops && pll->ops->pll_unprepare)
		pll->ops->pll_unprepare(pll);
}

struct meson_clk_pll_data *meson_pll_find_by_name(struct meson_clk_pll_data **pll_list,
						  int pll_cnt, char *name)
{
	struct meson_clk_pll_data *pll;
	unsigned int i;

	if (!pll_list) {
		printf("The pll list is null!\n");
		return NULL;
	}

	for (i = 0, pll = pll_list[0]; i < pll_cnt; i++, pll = pll_list[i])
		if (!strcmp(pll->name, name))
			return pll;

	return NULL;
}
