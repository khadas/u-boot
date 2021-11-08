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

static const struct pll_params_table t7_sys_pll_table[] = {
	PLL_PARAMS(67, 1, 1), /*DCO=1608M OD=804MM*/
	PLL_PARAMS(133, 1, 1), /*DCO=3192M OD=1596MM*/
	{ /* sentinel */ }
};

static unsigned int t7_sys_pll_default_rate[] = {804, 1596};

struct meson_clk_pll_data t7_sys_pll = {
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
	.smc_id = SECURE_PLL_CLK,
	.secid_disable = SECID_SYS0_DCO_PLL_DIS,
	.secid = SECID_SYS0_DCO_PLL,
	.clkmsr_id = 172,
	.clkmsr_div16_en = 1
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
	.smc_id = SECURE_PLL_CLK,
	.secid_disable = SECID_SYS1_DCO_PLL_DIS,
	.secid = SECID_SYS1_DCO_PLL,
	.clkmsr_id = 6,
	.clkmsr_div16_en = 1
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
	.clkmsr_id = 20,
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
	.clkmsr_id = 21,
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
	.clkmsr_id = 171,
};

static unsigned int t7_pcie_default_rate[] = {100};

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
	.clkmsr_id = 16,
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
		.width   = 8,
	},
	.n = {
		.reg = ANACTRL_HIFIPLL_CTRL0,
		.shift   = 10,
		.width   = 5,
	},
	.frac = {
		.reg = ANACTRL_HIFIPLL_CTRL0,
		.shift	 = 0,
		.width	 = 19,
	},
	.od = {
		.reg = ANACTRL_HIFIPLL_CTRL0,
		.shift	 = 16,
		.width	 = 5,
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
	.clkmsr_id = 19,
};

static unsigned int t7_mpll_default_rate[] = {166};
static const struct reg_sequence t7_mpll0_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL2,	.def = 0x40000033 },
};

struct meson_clk_mpll_data t7_mpll0 = {
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
	.clkmsr_id = 11
};

static const struct reg_sequence t7_mpll1_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL4,	.def = 0x40000033 },
};

struct meson_clk_mpll_data t7_mpll1 = {
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
	.clkmsr_id = 12
};

static const struct reg_sequence t7_mpll2_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL6,	.def = 0x40000033 },
};

struct meson_clk_mpll_data t7_mpll2 = {
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
	.clkmsr_id = 13
};

static const struct reg_sequence t7_mpll3_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL8,	.def = 0x40000033 },
};

struct meson_clk_mpll_data t7_mpll3 = {
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
	.clkmsr_id = 14
};

void meson_sys_pll_test(struct meson_clk_pll_data *pll, unsigned int cpu_dyn)
{
	struct arm_smccc_res res;

	/* set fixed clk to 1G, Switch to fixed clk first */
	arm_smccc_smc(SECURE_CPU_CLK, cpu_dyn,
	1, 0, 0, 0, 0, 0, &res);

	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_A73_CLK_SEL, 0);

	/* set sys pll rate */
	meson_secure_pll_test(&t7_sys_pll);

	/* Switch back to sys pll */
	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_A73_CLK_SEL, 1);
}

void meson_plls_test(void)
{
	/* test sys0 pll */
	meson_sys_pll_test(&t7_sys_pll, SECID_A73_CLK_DYN);
	//meson_secure_pll_test(&t7_sys1_pll, SECID_CPU_CLK_DYN);

	/* non secure pll */
	meson_pll_test(&t7_gp0_pll);
	meson_pll_test_one(&t7_pcie_pll);
	meson_pll_test(&t7_hifi_pll);
	meson_pll_test(&t7_gp1_pll);
	meson_pll_test_one(&t7_mclk_pll);
	/* mpll */
	meson_mpll_test(&t7_mpll0);
	meson_mpll_test(&t7_mpll1);
	meson_mpll_test(&t7_mpll2);
	meson_mpll_test(&t7_mpll3);

	return;
}

struct meson_clk_pll_data *t7_pll_list[] = {
	&t7_sys_pll,
	&t7_sys1_pll,
	&t7_gp0_pll,
	&t7_hifi_pll,
	&t7_gp1_pll,
};

struct meson_clk_mpll_data *t7_mpll_list[] = {
	&t7_mpll0,
	&t7_mpll1,
	&t7_mpll2,
	&t7_mpll3
};

int pll_test(int argc, char * const argv[])
{
	if (0 == strcmp("all", argv[1]))
		meson_plls_test();
	else if (0 == strcmp("sys0", argv[1])) {
		meson_sys_pll_test(&t7_sys_pll, SECID_A73_CLK_DYN);
	}
	else if (0 == strcmp("sys1", argv[1])) {
		meson_sys_pll_test(&t7_sys1_pll, SECID_CPU_CLK_DYN);
	}
	else if (0 == strcmp("pcie", argv[1])) {
		meson_pll_test_one(&t7_pcie_pll);
	}
	else if (0 == strcmp("mclk", argv[1])) {
		meson_pll_test_one(&t7_mclk_pll);
	}
	else {
		one_pll_test(t7_pll_list, ARRAY_SIZE(t7_pll_list),
		t7_mpll_list, ARRAY_SIZE(t7_mpll_list), argv[1]);
	}

	return 0;
}
