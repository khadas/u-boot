// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <asm/arch/secure_apb.h>
#include <../clk-pll.h>
#include <linux/kernel.h>

#define SECURE_PLL_CLK		0x82000098
#define SECURE_CPU_CLK		0x82000099

/* PLL secure clock index */
enum sec_pll {
	SECID_SYS0_DCO_PLL = 0,
	SECID_SYS0_DCO_PLL_DIS,
	SECID_SYS0_PLL_OD,
	SECID_SYS1_DCO_PLL,
	SECID_SYS1_DCO_PLL_DIS,
	SECID_SYS1_PLL_OD,
	SECID_CPU_CLK_SEL,
	SECID_CPU_CLK_RD,
	SECID_CPU_CLK_DYN,
	SECID_A73_CLK_SEL,
	SECID_A73_CLK_RD,
	SECID_A73_CLK_DYN,
};

int t7_sys0_pll_prepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	/* store rate */
	meson_pll_store_rate(pll);

	/* Set fixed clk to 1G, Switch to fixed clk first */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_A73_CLK_DYN,
	1, 0, 0, 0, 0, 0, &res);

	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_A73_CLK_SEL, 0);

	return 0;
}

void t7_sys0_pll_unprepare(struct meson_clk_pll_data *pll)
{
	/* restore rate */
	meson_pll_restore_rate(pll);

	/* Switch back cpu to sys pll */
	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_A73_CLK_SEL, 1);
}

static const struct pll_params_table t7_sys_pll_table[] = {
	PLL_PARAMS(67, 1, 1), /*DCO=1608M OD=804MM*/
	PLL_PARAMS(133, 1, 1), /*DCO=3192M OD=1596MM*/
	{ /* sentinel */ }
};

static unsigned int t7_sys_pll_default_rate[] = {804, 1596};

static const struct reg_sequence t7_sys0_pll_init_regs[] = {
	{ .reg = ANACTRL_SYS0PLL_CTRL0, .def = 0x20011086 },
	{ .reg = ANACTRL_SYS0PLL_CTRL1, .def = 0x1420500f },
	{ .reg = ANACTRL_SYS0PLL_CTRL2, .def = 0x00023041 },
	{ .reg = ANACTRL_SYS0PLL_CTRL3, .def = 0x0, .delay_us = 50},
	{ .reg = ANACTRL_SYS0PLL_CTRL0, .def = 0x10011086 },
	{ .reg = ANACTRL_SYS0PLL_CTRL2, .def = 0x00023001i, .delay_us = 20 }
};

struct meson_clk_pll_data t7_sys0_pll = {
	.name = "sys0",
	.en = {
		.reg = ANACTRL_SYS0PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = ANACTRL_SYS0PLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = ANACTRL_SYS0PLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = ANACTRL_SYS0PLL_CTRL0,
		.shift	 = 12,
		.width	 = 3,
	},
	.def_rate = t7_sys_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t7_sys_pll_default_rate),
	.table = t7_sys_pll_table,
	.init_regs = t7_sys0_pll_init_regs,
	.init_count = ARRAY_SIZE(t7_sys0_pll_init_regs),
	.l = {
		.reg = ANACTRL_SYS0PLL_CTRL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = ANACTRL_SYS0PLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = t7_sys0_pll_prepare,
		.pll_unprepare = t7_sys0_pll_unprepare,
		.pll_disable = meson_secure_pll_disable,
		.pll_set_rate = meson_secure_pll_set_rate,
		.pll_set_parm_rate = meson_secure_pll_set_parm_rate,
		.pll_rate_to_msr = meson_div16_to_msr,

	},
	.smc_id = SECURE_PLL_CLK,
	.secid_disable = SECID_SYS0_DCO_PLL_DIS,
	.secid = SECID_SYS0_DCO_PLL,
	.clkmsr_id = 172,
	.clkmsr_margin = 10,
};

int t7_sys1_pll_prepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	/* store rate */
	meson_pll_store_rate(pll);

	/* Set fixed clk to 1G, Switch to fixed clk first */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_CPU_CLK_DYN,
	1, 0, 0, 0, 0, 0, &res);

	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_CPU_CLK_SEL, 0);

	return 0;
}

void t7_sys1_pll_unprepare(struct meson_clk_pll_data *pll)
{
	/* restore rate */
	meson_pll_restore_rate(pll);

	/* Switch back cpu to sys pll */
	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_CPU_CLK_SEL, 1);
}

static const struct reg_sequence t7_sys1_pll_init_regs[] = {
	{ .reg = ANACTRL_SYS1PLL_CTRL0, .def = 0x20011086 },
	{ .reg = ANACTRL_SYS1PLL_CTRL0, .def = 0x30011086 },
	{ .reg = ANACTRL_SYS1PLL_CTRL1, .def = 0x1420500f },
	{ .reg = ANACTRL_SYS1PLL_CTRL2, .def = 0x00023041 },
	{ .reg = ANACTRL_SYS1PLL_CTRL3, .def = 0x0, .delay_us = 50},
	{ .reg = ANACTRL_SYS1PLL_CTRL0, .def = 0x10011086 },
	{ .reg = ANACTRL_SYS1PLL_CTRL2, .def = 0x00023001i, .delay_us = 20 }
};

struct meson_clk_pll_data t7_sys1_pll = {
	.name = "sys1",
	.en = {
		.reg = ANACTRL_SYS1PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = ANACTRL_SYS1PLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = ANACTRL_SYS1PLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = ANACTRL_SYS1PLL_CTRL0,
		.shift	 = 12,
		.width	 = 3,
	},
	.def_rate = t7_sys_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t7_sys_pll_default_rate),
	.table = t7_sys_pll_table,
	.init_regs = t7_sys1_pll_init_regs,
	.init_count = ARRAY_SIZE(t7_sys1_pll_init_regs),
	.l = {
		.reg = ANACTRL_SYS1PLL_CTRL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = ANACTRL_SYS1PLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = t7_sys1_pll_prepare,
		.pll_unprepare = t7_sys1_pll_unprepare,
		.pll_disable = meson_secure_pll_disable,
		.pll_set_rate = meson_secure_pll_set_rate,
		.pll_set_parm_rate = meson_secure_pll_set_parm_rate,
		.pll_rate_to_msr = meson_div16_to_msr,
	},
	.smc_id = SECURE_PLL_CLK,
	.secid_disable = SECID_SYS1_DCO_PLL_DIS,
	.secid = SECID_SYS1_DCO_PLL,
	.clkmsr_id = 23,
	.clkmsr_margin = 2
};

static unsigned int t7_gp0_pll_default_rate[] = {1500, 750, 1152};

/* set the od = 2, clkmsr can not measure 6000M */
static const struct pll_params_table t7_gp0_pll_table[] = {
	PLL_PARAMS(250, 1, 2), /* DCO = 6000M  PLL = 1500M */
	PLL_PARAMS(125, 1, 2), /* DCO = 3000M  PLL = 750M */
	PLL_PARAMS(192, 1, 2), /* DCO = 4608M  PLL = 1152M */
	{ /* sentinel */  }
};

static const struct reg_sequence t7_gp0_init_regs[] = {
	{ .reg = ANACTRL_GP0PLL_CTRL0,  .def = 0x080304fa },
	{ .reg = ANACTRL_GP0PLL_CTRL0,  .def = 0x380304fa },
	{ .reg = ANACTRL_GP0PLL_CTRL1,  .def = 0x00000000 },
	{ .reg = ANACTRL_GP0PLL_CTRL2,  .def = 0x00000000 },
	{ .reg = ANACTRL_GP0PLL_CTRL3,  .def = 0x48681c00 },
	{ .reg = ANACTRL_GP0PLL_CTRL4,  .def = 0x88770290 },
	{ .reg = ANACTRL_GP0PLL_CTRL5,  .def = 0x3927200a },
	{ .reg = ANACTRL_GP0PLL_CTRL6,  .def = 0x56540000, .delay_us = 50 },
	{ .reg = ANACTRL_GP0PLL_CTRL0,  .def = 0x180304fa, .delay_us = 50 }
};

struct meson_clk_pll_data t7_gp0_pll = {
	.name = "gp0",
	.en = {
		.reg = ANACTRL_GP0PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = ANACTRL_GP0PLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = ANACTRL_GP0PLL_CTRL0,
		.shift   = 10,
		.width   = 5,
	},
	.od = {
		.reg = ANACTRL_GP0PLL_CTRL0,
		.shift	 = 16,
		.width	 = 3,
	},
	.def_rate = t7_gp0_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t7_gp0_pll_default_rate),
	.table = t7_gp0_pll_table,
	.init_regs = t7_gp0_init_regs,
	.init_count = ARRAY_SIZE(t7_gp0_init_regs),
	.l = {
		.reg = ANACTRL_GP0PLL_CTRL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = ANACTRL_GP0PLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		//.pll_prepare = meson_pll_store_rate,
		//.pll_unprepare = meson_pll_restore_rate,
	},
	.clkmsr_id = 20,
	.clkmsr_margin = 2
};

static unsigned int t7_gp1_pll_default_rate[] = {1596, 804};

static const struct pll_params_table t7_gp1_pll_table[] = {
	PLL_PARAMS(133, 1, 1), /* DCO = 3192M  PLL = 1596M */
	PLL_PARAMS(67, 1, 1), /* DCO = 1608M  PLL = 804M */
	{ /* sentinel */  }
};

static const struct reg_sequence t7_gp1_init_regs[] = {
	{ .reg = ANACTRL_GP1PLL_CTRL0,  .def = 0x20011086 },
	{ .reg = ANACTRL_GP1PLL_CTRL0,  .def = 0x30011086 },
	{ .reg = ANACTRL_GP1PLL_CTRL1,  .def = 0x1420500f },
	{ .reg = ANACTRL_GP1PLL_CTRL2,  .def = 0x00023041 },
	{ .reg = ANACTRL_GP1PLL_CTRL3,  .def = 0x0, .delay_us = 20 },
	{ .reg = ANACTRL_GP1PLL_CTRL0,  .def = 0x10011086, .delay_us = 20 },
	{ .reg = ANACTRL_GP1PLL_CTRL2,  .def = 0x00023001, .delay_us = 50 },
};

struct meson_clk_pll_data t7_gp1_pll = {
	.name = "gp1",
	.en = {
		.reg = ANACTRL_GP1PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = ANACTRL_GP1PLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = ANACTRL_GP1PLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = ANACTRL_GP1PLL_CTRL0,
		.shift	 = 12,
		.width	 = 3,
	},
	.def_rate = t7_gp1_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t7_gp1_pll_default_rate),
	.table = t7_gp1_pll_table,
	.init_regs = t7_gp1_init_regs,
	.init_count = ARRAY_SIZE(t7_gp1_init_regs),
	.l = {
		.reg = ANACTRL_GP1PLL_CTRL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = ANACTRL_GP1PLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
	//	.pll_prepare = t7_gp1_pll_prepare,
	//	.pll_unprepare = t7_gp1_pll_unprepare,
	},
	.clkmsr_id = 21,
	.clkmsr_margin = 2
};

unsigned long t7_pcie_pll_rate_to_msr(struct meson_clk_pll_data *pll, unsigned long rate)
{
	u8 od = (readl(ANACTRL_PCIEPLL_CTRL0) >> 16) & 0x1f;

	//printf("rate = %ld od= %d\n", rate, od);
	return (rate >> 2) / od;
}

static unsigned int t7_pcie_default_rate[] = {4800};

static const struct reg_sequence t7_pcie_init_regs[] = {
	{ .reg = ANACTRL_PCIEPLL_CTRL0,	.def = 0x200c04c8 },
	{ .reg = ANACTRL_PCIEPLL_CTRL0,	.def = 0x300c04c8 },
	{ .reg = ANACTRL_PCIEPLL_CTRL1,	.def = 0x30000000 },
	{ .reg = ANACTRL_PCIEPLL_CTRL2,	.def = 0x00001100 },
	{ .reg = ANACTRL_PCIEPLL_CTRL3,	.def = 0x10058e00 },
	{ .reg = ANACTRL_PCIEPLL_CTRL4,	.def = 0x000100c0 },
	{ .reg = ANACTRL_PCIEPLL_CTRL5,	.def = 0x68000040 },
	{ .reg = ANACTRL_PCIEPLL_CTRL5,	.def = 0x68000060, .delay_us = 20 },
	{ .reg = ANACTRL_PCIEPLL_CTRL4,	.def = 0x008100c0, .delay_us = 10 },
	{ .reg = ANACTRL_PCIEPLL_CTRL0,	.def = 0x340c04c8 },
	{ .reg = ANACTRL_PCIEPLL_CTRL0,	.def = 0x140c04c8, .delay_us = 10 },
	{ .reg = ANACTRL_PCIEPLL_CTRL2,	.def = 0x00001000 }
};

struct meson_clk_pll_data t7_pcie_pll = {
	.name = "pcie",
	.en = {
		.reg = ANACTRL_PCIEPLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = ANACTRL_PCIEPLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = ANACTRL_PCIEPLL_CTRL0,
		.shift   = 10,
		.width   = 5,
	},
	.def_rate = t7_pcie_default_rate,
	.def_cnt = ARRAY_SIZE(t7_pcie_default_rate),
	.init_regs = t7_pcie_init_regs,
	.init_count = ARRAY_SIZE(t7_pcie_init_regs),
	.l = {
		.reg = ANACTRL_PCIEPLL_CTRL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = ANACTRL_PCIEPLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
		.pll_set_rate = meson_pll_set_one_rate,
		.pll_rate_to_msr = t7_pcie_pll_rate_to_msr,
	},
	.clkmsr_id = 16,
	.clkmsr_margin = 2
};

static unsigned int t7_hifi_pll_default_rate[] = {1500, 750};

/* set the od = 2, clkmsr can not measure 6000M */
static const struct pll_params_table t7_hifi_pll_table[] = {
	PLL_PARAMS(250, 1, 2), /* DCO = 6000M  PLL = 1500M */
	PLL_PARAMS(125, 1, 2), /* DCO = 3000M  PLL = 750M */
	{ /* sentinel */  }
};

static const struct reg_sequence t7_hifi_init_regs[] = {
	{ .reg = ANACTRL_HIFIPLL_CTRL0,	.def = 0x080304fa },
	{ .reg = ANACTRL_HIFIPLL_CTRL0,	.def = 0x380304fa },
	//{ .reg = ANACTRL_HIFIPLL_CTRL1,	.def = 0x00010e56 },
	{ .reg = ANACTRL_HIFIPLL_CTRL2,	.def = 0x00000000 },
	{ .reg = ANACTRL_HIFIPLL_CTRL3,	.def = 0x6a285c00 },
	{ .reg = ANACTRL_HIFIPLL_CTRL4,	.def = 0x65771290 },
	{ .reg = ANACTRL_HIFIPLL_CTRL5,	.def = 0x3927200a },
	{ .reg = ANACTRL_HIFIPLL_CTRL6,	.def = 0x56540000, .delay_us = 50 },
	{ .reg = ANACTRL_HIFIPLL_CTRL0,	.def = 0X180304fa, .delay_us = 50 },
};

struct meson_clk_pll_data t7_hifi_pll = {
	.name = "hifi",
	.en = {
		.reg = ANACTRL_HIFIPLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = ANACTRL_HIFIPLL_CTRL0,
		.shift   = 0,
		.width   = 9,
	},
	.n = {
		.reg = ANACTRL_HIFIPLL_CTRL0,
		.shift   = 10,
		.width   = 5,
	},
	.frac = {
		.reg = ANACTRL_HIFIPLL_CTRL1,
		.shift	 = 0,
		.width	 = 19,
	},
	.od = {
		.reg = ANACTRL_HIFIPLL_CTRL0,
		.shift	 = 16,
		.width	 = 2,
	},
	.def_rate = t7_hifi_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t7_hifi_pll_default_rate),
	.table = t7_hifi_pll_table,
	.init_regs = t7_hifi_init_regs,
	.init_count = ARRAY_SIZE(t7_hifi_init_regs),
	.l = {
		.reg = ANACTRL_HIFIPLL_CTRL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = ANACTRL_HIFIPLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_set_rate  = meson_pll_set_rate,
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
	},
	.clkmsr_id = 19,
	.clkmsr_margin = 2
};

static unsigned int t7_mclk_pll_default_rate[] = {50};

static const struct reg_sequence t7_mclk_init_regs[] = {
	{ .reg = ANACTRL_MCLK_PLL_CNTL0,  .def = 0x20011064 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL0,  .def = 0x30011064 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL1,  .def = 0x0480500f },
	{ .reg = ANACTRL_MCLK_PLL_CNTL2,  .def = 0x00023041 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL3,  .def = 0x18180000 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL4,  .def = 0x00180303, .delay_us = 20 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL0,  .def = 0x10011064, .delay_us = 20 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL2,  .def = 0x00023001, .delay_us = 50 },
};

struct meson_clk_pll_data t7_mclk_pll = {
	.name = "mclk",
	.en = {
		.reg = ANACTRL_MCLK_PLL_CNTL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = ANACTRL_MCLK_PLL_CNTL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = ANACTRL_MCLK_PLL_CNTL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = ANACTRL_MCLK_PLL_CNTL0,
		.shift	 = 12,
		.width	 = 3,
	},
	.def_rate = t7_mclk_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t7_mclk_pll_default_rate),
	.init_regs = t7_mclk_init_regs,
	.init_count = ARRAY_SIZE(t7_mclk_init_regs),
	.l = {
		.reg = ANACTRL_MCLK_PLL_CNTL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = ANACTRL_MCLK_PLL_CNTL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_set_rate = meson_pll_set_one_rate,
		.pll_set_parm_rate = meson_pll_set_parm_rate,
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
	},
	.clkmsr_id = 171,
};

static unsigned int t7_mpll_default_rate[] = {100};
static struct meson_pll_test_ops mpll_test_ops = {
	.pll_prepare = meson_pll_store_rate,
	.pll_unprepare = meson_pll_restore_rate,
	.pll_set_parm_rate = meson_mpll_set_parm_rate,
	.pll_set_rate = meson_mpll_set_rate,
	.pll_recalc = meson_mpll_recalc_rate
};

static const struct reg_sequence t7_mpll0_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL2,	.def = 0x40000033 },
};

struct meson_clk_pll_data t7_mpll0 = {
	.name = "mpll0",
	.sdm = {
		.reg = ANACTRL_MPLL_CTRL1,
		.shift   = 0,
		.width   = 14,
	},
	.sdm_en = {
		.reg = ANACTRL_MPLL_CTRL1,
		.shift   = 30,
		.width   = 1,
	},
	.n2 = {
		.reg = ANACTRL_MPLL_CTRL1,
		.shift   = 20,
		.width   = 9,
	},
	.ssen = {
		.reg = ANACTRL_MPLL_CTRL1,
		.shift   = 29,
		.width   = 1,
	},
	.en = {
		.reg = ANACTRL_MPLL_CTRL1,
		.shift   = 31,
		.width   = 1,
	},
	.init_regs = t7_mpll0_init_regs,
	.init_count = ARRAY_SIZE(t7_mpll0_init_regs),
	.def_rate = t7_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(t7_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 11,
	.clkmsr_margin = 2
};

static const struct reg_sequence t7_mpll1_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL4,	.def = 0x40000033 },
};

struct meson_clk_pll_data t7_mpll1 = {
	.name = "mpll1",
	.sdm = {
		.reg = ANACTRL_MPLL_CTRL3,
		.shift   = 0,
		.width   = 14,
	},
	.sdm_en = {
		.reg = ANACTRL_MPLL_CTRL3,
		.shift   = 30,
		.width   = 1,
	},
	.n2 = {
		.reg = ANACTRL_MPLL_CTRL3,
		.shift   = 20,
		.width   = 9,
	},
	.ssen = {
		.reg = ANACTRL_MPLL_CTRL3,
		.shift   = 29,
		.width   = 1,
	},
	.en = {
		.reg = ANACTRL_MPLL_CTRL3,
		.shift   = 31,
		.width   = 1,
	},
	.init_regs = t7_mpll1_init_regs,
	.init_count = ARRAY_SIZE(t7_mpll1_init_regs),
	.def_rate = t7_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(t7_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 12,
	.clkmsr_margin = 2
};

static const struct reg_sequence t7_mpll2_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL6,	.def = 0x40000033 },
};

struct meson_clk_pll_data t7_mpll2 = {
	.name = "mpll2",
	.sdm = {
		.reg = ANACTRL_MPLL_CTRL5,
		.shift   = 0,
		.width   = 14,
	},
	.sdm_en = {
		.reg = ANACTRL_MPLL_CTRL5,
		.shift   = 30,
		.width   = 1,
	},
	.n2 = {
		.reg = ANACTRL_MPLL_CTRL5,
		.shift   = 20,
		.width   = 9,
	},
	.ssen = {
		.reg = ANACTRL_MPLL_CTRL5,
		.shift   = 29,
		.width   = 1,
	},
	.en = {
		.reg = ANACTRL_MPLL_CTRL5,
		.shift   = 31,
		.width   = 1,
	},
	.init_regs = t7_mpll2_init_regs,
	.init_count = ARRAY_SIZE(t7_mpll2_init_regs),
	.def_rate = t7_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(t7_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 13,
	.clkmsr_margin = 2
};

static const struct reg_sequence t7_mpll3_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL8,	.def = 0x40000033 },
};

struct meson_clk_pll_data t7_mpll3 = {
	.name = "mpll3",
	.sdm = {
		.reg = ANACTRL_MPLL_CTRL7,
		.shift   = 0,
		.width   = 14,
	},
	.sdm_en = {
		.reg = ANACTRL_MPLL_CTRL7,
		.shift   = 30,
		.width   = 1,
	},
	.n2 = {
		.reg = ANACTRL_MPLL_CTRL7,
		.shift   = 20,
		.width   = 9,
	},
	.ssen = {
		.reg = ANACTRL_MPLL_CTRL7,
		.shift   = 29,
		.width   = 1,
	},
	.en = {
		.reg = ANACTRL_MPLL_CTRL7,
		.shift   = 31,
		.width   = 1,
	},
	.init_regs = t7_mpll3_init_regs,
	.init_count = ARRAY_SIZE(t7_mpll3_init_regs),
	.def_rate = t7_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(t7_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 14,
	.clkmsr_margin = 2
};

struct meson_clk_pll_data *t7_pll_list[] = {
	&t7_sys0_pll,
	&t7_sys1_pll,
	&t7_gp0_pll,
	&t7_gp1_pll,
	&t7_pcie_pll,
	&t7_hifi_pll,
	&t7_mclk_pll,
	&t7_mpll0,
	&t7_mpll1,
	&t7_mpll2,
	&t7_mpll3
};

void t7_meson_plls_all_test(void)
{
	for (int i = 0; i < ARRAY_SIZE(t7_pll_list); i++)
		meson_pll_test(t7_pll_list[i]);
}

void t7_meson_hdmirx_plls_test(int argc, char * const argv[])
{
	/* todo: need add hdmirx plls test */
}

void t7_meson_tcon_plls_test(int argc, char * const argv[])
{
	/* todo: need add tcon plls test */
}

void t7_meson_plls_test(int argc, char * const argv[])
{
	struct meson_clk_pll_data *pll;

	pll = meson_pll_find_by_name(t7_pll_list, ARRAY_SIZE(t7_pll_list), argv[1]);
	if (!pll) {
		printf("The pll is not supported Or wrong pll name\n");
		return;
	}

	if (pll->init_count && ((argc - 2) == pll->init_count))
		meson_pll_parm_test(pll, argv);
	else if (argc == 2)
		meson_pll_test(pll);
	else
		printf("The pll cmd argc is error!\n");
}

int pll_test(int argc, char * const argv[])
{
	if (!strcmp("all", argv[1]))
		t7_meson_plls_all_test();
	else if (!strcmp("tcon", argv[1]))
		t7_meson_tcon_plls_test(argc, argv);
	else if (!strcmp("hdmirx", argv[1]))
		t7_meson_hdmirx_plls_test(argc, argv);
	else
		t7_meson_plls_test(argc, argv);

	return 0;
}
