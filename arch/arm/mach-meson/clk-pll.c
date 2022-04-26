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

#if 0
static long meson_clk_pll_round_rate(struct meson_clk_pll_data *pll, unsigned long rate)
{
	unsigned int m, n, frac, od;
	unsigned long round;
	int ret;

	ret = meson_clk_get_pll_settings(rate, &m, &n, pll, &od);
	if (ret)
		return meson_clk_pll_recalc_rate(pll);

	round = __pll_params_to_rate(m, n, 0, pll, od);

	if (!MESON_PARM_APPLICABLE(&pll->frac) || rate == round)
		return round;

	/*
	 * The rate provided by the setting is not an exact match, let's
	 * try to improve the result using the fractional parameter
	 */
	frac = __pll_params_with_frac(rate, m, n, pll);

	return __pll_params_to_rate(m, n, frac, pll, od);
}
#endif

static int meson_clk_pll_wait_lock(struct meson_clk_pll_data *pll)
{
	int delay = 1000;

	do {
		/* Is the clock locked now ? */
		if (meson_parm_read(&pll->l))
			return 0;
		_udelay(1);
	} while (delay--);

	return -ETIMEDOUT;
}

void meson_clk_pll_disable(struct meson_clk_pll_data *pll)
{
	/* Put the pll is in reset */
	setbits_le32(pll->rst.reg, 1 << pll->rst.shift);

	/* Disable the pll */
	setbits_le32(pll->en.reg, 0 << pll->en.shift);
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
	printf("meson_pll_set_rate: %s trate = %lu, m = %d, n = %d, od = %d\n",
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

int meson_pll_set_one_rate(struct meson_clk_pll_data *pll, unsigned long rate)
{
	unsigned int enabled, i, val, j = 10;
	const struct reg_sequence *init_regs = pll->init_regs;

	if (PARENT_RATE == 0 || rate == 0)
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
			printf("pcie pll did not lock, retrying %d\n", 10 - j);
		else
			break;
		j--;
	} while (j);

	return 0;
}

void meson_secure_pll_disable(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	arm_smccc_smc(pll->smc_id, pll->secid_disable,
			      0, 0, 0, 0, 0, 0, &res);
}

int meson_secure_pll_set_rate(struct meson_clk_pll_data *pll, unsigned long rate)
{
	struct arm_smccc_res res;
	unsigned int enabled, m, n, ret = 0;
	unsigned int od;

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

	return 0;
}

void meson_pll_report(struct meson_clk_pll_data *pll, unsigned int target, unsigned  long res)
{
	if (((target-2) <= res) && (res <= (target+2)))
		printf("%s PLL lock ok, target rate = %uM, clkmsr rate = %luM : Match\n", pll->name, target, res);
	else
		printf("%s PLL lock failed, target rate = %uM, clkmsr rate = %luM: Not Match\n", pll->name, target, res);
}

void meson_pll_test(struct meson_clk_pll_data *pll)
{
	int i;
	unsigned long result;

	for (i = 0; i < pll->def_cnt;i++) {
		meson_pll_set_rate(pll, (unsigned long)pll->def_rate[i] * (unsigned long)1000000);

		result = clk_util_clk_msr(pll->clkmsr_id);

		meson_pll_report(pll, pll->def_rate[i], result);
	}
}

void meson_secure_pll_test(struct meson_clk_pll_data *pll)
{
	int i;
	unsigned long result;

	for (i = 0; i < pll->def_cnt;i++) {
		meson_secure_pll_set_rate(pll, (unsigned long)pll->def_rate[i] * (unsigned long)1000000);

		result = clk_util_clk_msr(pll->clkmsr_id);
		/* for sys_pll, clkmsr result is actual value/16 */
		if (pll->clkmsr_div16_en)
			result *= 16;

		if (((pll->def_rate[i]-2*5) <= result) && (result <= (pll->def_rate[i]+2*5)))
			printf("%s PLL lock ok, target rate = %u M, clkmsr rate = %lu: Match\n", pll->name, pll->def_rate[i], result);
		else
			printf("%s PLL lock failed, target rate = %u M, clkmsr rate = %lu: Not Match\n", pll->name, pll->def_rate[i], result);
	}
}

void meson_pll_test_one(struct meson_clk_pll_data *pll)
{
	int i;
	unsigned long result;

	for (i = 0; i < pll->def_cnt;i++) {
		meson_pll_set_one_rate(pll, (unsigned long)pll->def_rate[i] * (unsigned long)1000000);

		result = clk_util_clk_msr(pll->clkmsr_id);

		meson_pll_report(pll, pll->def_rate[i], result);
	}
}

void meson_switch_cpu_clk(unsigned int smc_id, unsigned int secid, unsigned int flag)
{
	struct arm_smccc_res res;

	arm_smccc_smc(smc_id, secid,
	0x1 << 11, flag << 11,
	0, 0, 0, 0, &res);
}

void one_pll_test(struct meson_clk_pll_data **pll_list, int pll_cnt,
		struct meson_clk_mpll_data **mpll_list, int mpll_cnt,
		char *arg)
{
	struct meson_clk_pll_data *pll;
	struct meson_clk_mpll_data *mpll;
	unsigned int i, j;

	for (i = 0; i < pll_cnt;i++) {
		pll = pll_list[i];
		if (0 == strcmp(pll->name, arg)) {
			meson_pll_test(pll);
			return;
		}
	}
	for (j = 0; j < mpll_cnt;j++) {
		mpll = mpll_list[j];
		if (0 == strcmp(mpll->name, arg)) {
			meson_mpll_test(mpll);
			return;
		}
	}
	if (i == pll_cnt && (j == mpll_cnt))
		printf("The pll is not supported Or wrong pll name\n");
}
