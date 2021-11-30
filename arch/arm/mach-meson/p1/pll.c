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
	SECID_GP1_DCO_PLL,
	SECID_GP1_DCO_PLL_DIS,
	SECID_GP1_PLL_OD,
	SECID_CPU_CLK_SEL,
	SECID_CPU_CLK_RD,
	SECID_CPU_CLK_DYN,
	SECID_A76_CLK_SEL,
	SECID_A76_CLK_RD,
	SECID_A76_CLK_DYN,
	SECID_DSU_CLK_SEL,
	SECID_DSU_CLK_RD,
	SECID_DSU_CLK_DYN,
	SECID_DSU_FINAL_CLK_SEL,
	SECID_DSU_FINAL_CLK_RD,
	SECID_CPU6_CLK_SEL,
	SECID_CPU6_CLK_RD,
	SECID_CPU7_CLK_SEL,
	SECID_CPU7_CLK_RD,
	SECID_SLT_PLL,
};


int p1_sys0_pll_prepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	/* store rate */
	meson_pll_store_rate(pll);

	/* Enable sys0 pll div16 */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_CPU6_CLK_SEL,
	(0x1 << 13), (1 << 13), 0, 0, 0, 0, &res);
	arm_smccc_smc(SECURE_CPU_CLK, SECID_CPU6_CLK_SEL,
	(0x1 << 12), (0 << 12), 0, 0, 0, 0, &res);
	/* Set fixed clk to 1G, Switch to fixed clk first */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_CPU_CLK_DYN,
	1, 0, 0, 0, 0, 0, &res);

	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_CPU_CLK_SEL, 0);

	/* Switch dsu to cpu0 */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_DSU_CLK_SEL,
	(0x1 << 11), (0 << 11), 0, 0, 0, 0, &res);

	return 0;
}

void p1_sys0_pll_unprepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	/* restore rate */
	meson_pll_restore_rate(pll);

	/* Switch back cpu to sys pll */
	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_CPU_CLK_SEL, 1);

	/* Switch back dsu to gp1 pll */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_DSU_CLK_SEL,
	(0x1 << 11), (0x1 << 11), 0, 0, 0, 0, &res);
}

static const struct pll_params_table p1_sys_pll_table[] = {
	PLL_PARAMS(100, 1, 1), /*DCO=2400M OD=1200M */
	PLL_PARAMS(126, 1, 1), /*DCO=3024M OD=1512M */
	PLL_PARAMS(83, 1, 0), /*DCO=1992M OD=1992M*/
	{ /* sentinel */ }
};

static unsigned int p1_sys_pll_default_rate[] = {1200, 1992};

static const struct reg_sequence p1_sys0_init_regs[] = {
	{ .reg = CLKCTRL_SYS0PLL_CTRL0, .def = 0x20011086 },
	{ .reg = CLKCTRL_SYS0PLL_CTRL0, .def = 0x30011086 },
	{ .reg = CLKCTRL_SYS0PLL_CTRL1, .def = 0x1420500f },
	{ .reg = CLKCTRL_SYS0PLL_CTRL2, .def = 0x00023041 },
	{ .reg = CLKCTRL_SYS0PLL_CTRL3, .def = 0x0, .delay_us = 50 },
	{ .reg = CLKCTRL_SYS0PLL_CTRL0, .def = 0x10011086, .delay_us = 20 },
	{ .reg = CLKCTRL_SYS0PLL_CTRL2, .def = 0x00023001, .delay_us = 50 }
};

struct meson_clk_pll_data p1_sys0_pll = {
	.name = "sys0",
	.en = {
		.reg = CLKCTRL_SYS0PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = CLKCTRL_SYS0PLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = CLKCTRL_SYS0PLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = CLKCTRL_SYS0PLL_CTRL0,
		.shift	 = 12,
		.width	 = 3,
	},
	.def_rate = p1_sys_pll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_sys_pll_default_rate),
	.table = p1_sys_pll_table,
	.init_regs = p1_sys0_init_regs,
	.init_count = ARRAY_SIZE(p1_sys0_init_regs),
	.l = {
		.reg = CLKCTRL_SYS0PLL_CTRL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = CLKCTRL_SYS0PLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = p1_sys0_pll_prepare,
		.pll_unprepare = p1_sys0_pll_unprepare,
		.pll_disable = meson_secure_pll_disable,
		.pll_set_rate = meson_secure_pll_set_rate,
		.pll_set_parm_rate = meson_secure_pll_set_parm_rate,
		.pll_rate_to_msr = meson_div16_to_msr,

	},
	.smc_id = SECURE_PLL_CLK,
	.secid_disable = SECID_SYS0_DCO_PLL_DIS,
	.secid_test = SECID_SLT_PLL,
	.secid = SECID_SYS0_DCO_PLL,
	.clkmsr_id = 23,
	.clkmsr_margin = 10,
};

int p1_sys1_pll_prepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	/* store rate */
	meson_pll_store_rate(pll);

	/* Enable sys0 pll div16 */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_CPU6_CLK_SEL,
	(0x1 << 13), (1 << 13), 0, 0, 0, 0, &res);
	arm_smccc_smc(SECURE_CPU_CLK, SECID_CPU6_CLK_SEL,
	(0x1 << 12), (1 << 12), 0, 0, 0, 0, &res);
	/* Set fixed clk to 1G, Switch to fixed clk first */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_A76_CLK_DYN,
	1, 0, 0, 0, 0, 0, &res);

	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_A76_CLK_SEL, 0);

	/* Switch dsu to cpu0 */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_DSU_CLK_SEL,
	(0x1 << 27), (0 << 27), 0, 0, 0, 0, &res);

	return 0;
}

void p1_sys1_pll_unprepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	/* restore rate */
	meson_pll_restore_rate(pll);

	/* Switch back cpu to sys pll */
	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_A76_CLK_SEL, 1);

	/* Switch back dsu to gp1 pll */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_DSU_CLK_SEL,
	(0x1 << 27), (0x1 << 27), 0, 0, 0, 0, &res);
}

static unsigned int p1_sys1_pll_default_rate[] = {1200, 1944};
static const struct pll_params_table p1_sys1_pll_table[] = {
	PLL_PARAMS(100, 1, 1), /*DCO=2400M OD=1200M */
	PLL_PARAMS(126, 1, 1), /*DCO=3024M OD=1512M */
	PLL_PARAMS(81, 1, 0), /*DCO=1944M OD=1944M*/
	{ /* sentinel */ }
};

static const struct reg_sequence p1_sys1_init_regs[] = {
	{ .reg = CLKCTRL_SYS1PLL_CTRL0, .def = 0x20011086 },
	{ .reg = CLKCTRL_SYS1PLL_CTRL0, .def = 0x30011086 },
	{ .reg = CLKCTRL_SYS1PLL_CTRL1, .def = 0x1420500f },
	{ .reg = CLKCTRL_SYS1PLL_CTRL2, .def = 0x00023041 },
	{ .reg = CLKCTRL_SYS1PLL_CTRL3, .def = 0x0, .delay_us = 20 },
	{ .reg = CLKCTRL_SYS1PLL_CTRL0, .def = 0x10011086, .delay_us = 20 },
	{ .reg = CLKCTRL_SYS1PLL_CTRL2, .def = 0x00023001, .delay_us = 50 }
};

struct meson_clk_pll_data p1_sys1_pll = {
	.name = "sys1",
	.en = {
		.reg = CLKCTRL_SYS1PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = CLKCTRL_SYS1PLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = CLKCTRL_SYS1PLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = CLKCTRL_SYS1PLL_CTRL0,
		.shift	 = 12,
		.width	 = 3,
	},
	.def_rate = p1_sys1_pll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_sys1_pll_default_rate),
	.table = p1_sys1_pll_table,
	.init_regs = p1_sys1_init_regs,
	.init_count = ARRAY_SIZE(p1_sys1_init_regs),
	.l = {
		.reg = CLKCTRL_SYS1PLL_CTRL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = CLKCTRL_SYS1PLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = p1_sys1_pll_prepare,
		.pll_unprepare = p1_sys1_pll_unprepare,
		.pll_disable = meson_secure_pll_disable,
		.pll_set_rate = meson_secure_pll_set_rate,
		.pll_set_parm_rate = meson_secure_pll_set_parm_rate,
		.pll_rate_to_msr = meson_div16_to_msr,
	},
	.smc_id = SECURE_PLL_CLK,
	.secid_disable = SECID_SYS1_DCO_PLL_DIS,
	.secid_test = SECID_SLT_PLL,
	.secid = SECID_SYS1_DCO_PLL,
	.clkmsr_id = 23,
	.clkmsr_margin = 10
};

static unsigned int p1_gp0_pll_default_rate[] = {750, 1152};

/* set the od = 2, clkmsr can not measure 6000M */
static const struct pll_params_table p1_gp0_pll_table[] = {
	PLL_PARAMS(125, 1, 2), /* DCO = 3000M  OD = 750M */
	PLL_PARAMS(192, 1, 2), /* DCO = 4608M  OD = 1152M for restore */
	{ /* sentinel */  }
};

static const struct reg_sequence p1_gp0_init_regs[] = {
	{ .reg = CLKCTRL_GP0PLL_CTRL0,  .def = 0x080304fa },
	{ .reg = CLKCTRL_GP0PLL_CTRL0,  .def = 0x380304fa },
	{ .reg = CLKCTRL_GP0PLL_CTRL1,  .def = 0x00000000 },
	{ .reg = CLKCTRL_GP0PLL_CTRL2,  .def = 0x00000000 },
	{ .reg = CLKCTRL_GP0PLL_CTRL3,  .def = 0x48681c00 },
	{ .reg = CLKCTRL_GP0PLL_CTRL4,  .def = 0x88770290 },
	{ .reg = CLKCTRL_GP0PLL_CTRL5,  .def = 0x39272000 },
	{ .reg = CLKCTRL_GP0PLL_CTRL6,  .def = 0x56540000, .delay_us = 50 },
	{ .reg = CLKCTRL_GP0PLL_CTRL0,  .def = 0x180304fa, .delay_us = 50 }
};

struct meson_clk_pll_data p1_gp0_pll = {
	.name = "gp0",
	.en = {
		.reg = CLKCTRL_GP0PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = CLKCTRL_GP0PLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = CLKCTRL_GP0PLL_CTRL0,
		.shift   = 10,
		.width   = 5,
	},
	.od = {
		.reg = CLKCTRL_GP0PLL_CTRL0,
		.shift	 = 16,
		.width	 = 3,
	},
	.def_rate = p1_gp0_pll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_gp0_pll_default_rate),
	.table = p1_gp0_pll_table,
	.init_regs = p1_gp0_init_regs,
	.init_count = ARRAY_SIZE(p1_gp0_init_regs),
	.l = {
		.reg = CLKCTRL_GP0PLL_CTRL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = CLKCTRL_GP0PLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
	},
	.clkmsr_id = 20,
	.clkmsr_margin = 2
};

int p1_gp1_pll_prepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	meson_pll_store_rate(pll);

	/* Set fixed clk to 1G, Switch to fixed clk first */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_CPU_CLK_DYN,
	1, 0, 0, 0, 0, 0, &res);

	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_CPU_CLK_SEL, 0);

	/* Switch dsu to cpu0 */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_DSU_CLK_SEL,
	(0x1 << 11), (0 << 11), 0, 0, 0, 0, &res);

	return 0;
}

void p1_gp1_pll_unprepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	meson_pll_restore_rate(pll);

	/* Switch back cpu to sys pll */
	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_CPU_CLK_SEL, 1);

	/* Switch back dsu to gp1 pll */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_DSU_CLK_SEL,
	(0x1 << 11), (0x1 << 11), 0, 0, 0, 0, &res);
}

static unsigned int p1_gp1_pll_default_rate[] = {1200, 1500};

static const struct pll_params_table p1_gp1_pll_table[] = {
	PLL_PARAMS(100, 1, 1), /* DCO = 2400M  PLL = 1200M for restore */
	PLL_PARAMS(125, 1, 1), /* DCO = 3000M  PLL = 1500M */
	{ /* sentinel */  }
};

static const struct reg_sequence p1_gp1_init_regs[] = {
	{ .reg = CLKCTRL_GP1PLL_CTRL0, .def = 0x20011086 },
	{ .reg = CLKCTRL_GP1PLL_CTRL0, .def = 0x30011086 },
	{ .reg = CLKCTRL_GP1PLL_CTRL1, .def = 0x1420500f },
	{ .reg = CLKCTRL_GP1PLL_CTRL2, .def = 0x00023041 },
	{ .reg = CLKCTRL_GP1PLL_CTRL3, .def = 0x0, .delay_us = 20 },
	{ .reg = CLKCTRL_GP1PLL_CTRL0, .def = 0x10011086, .delay_us = 20 },
	{ .reg = CLKCTRL_GP1PLL_CTRL2, .def = 0x00023001, .delay_us = 20 }
};

struct meson_clk_pll_data p1_gp1_pll = {
	.name = "gp1",
	.en = {
		.reg = CLKCTRL_GP1PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = CLKCTRL_GP1PLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = CLKCTRL_GP1PLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = CLKCTRL_GP1PLL_CTRL0,
		.shift	 = 12,
		.width	 = 3,
	},
	.def_rate = p1_gp1_pll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_gp1_pll_default_rate),
	.table = p1_gp1_pll_table,
	.init_regs = p1_gp1_init_regs,
	.init_count = ARRAY_SIZE(p1_gp1_init_regs),
	.l = {
		.reg = CLKCTRL_GP1PLL_STS,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = CLKCTRL_GP1PLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = p1_gp1_pll_prepare,
		.pll_unprepare = p1_gp1_pll_unprepare,
		.pll_disable = meson_secure_pll_disable,
		.pll_set_rate = meson_secure_pll_set_rate,
		.pll_set_parm_rate = meson_secure_pll_set_parm_rate,
	},
	.smc_id = SECURE_PLL_CLK,
	.secid_disable = SECID_GP1_DCO_PLL_DIS,
	.secid_test = SECID_SLT_PLL,
	.secid = SECID_GP1_DCO_PLL,
	.clkmsr_id = 21,
	.clkmsr_margin = 2
};

static unsigned int p1_gp2_pll_default_rate[] = {1200, 1500};

static const struct pll_params_table p1_gp2_pll_table[] = {
	PLL_PARAMS(100, 1, 1), /* DCO = 2400M  OD = 1200M */
	PLL_PARAMS(125, 1, 1), /* DCO = 3000M  OD = 1500M */
	{ /* sentinel */  }
};

static const struct reg_sequence p1_gp2_init_regs[] = {
	{ .reg = CLKCTRL_GP2PLL_CTRL0, .def = 0x20011086 },
	{ .reg = CLKCTRL_GP2PLL_CTRL0, .def = 0x30011086 },
	{ .reg = CLKCTRL_GP2PLL_CTRL1, .def = 0x1420500f },
	{ .reg = CLKCTRL_GP2PLL_CTRL2, .def = 0x00023041 },
	{ .reg = CLKCTRL_GP2PLL_CTRL3, .def = 0x0, .delay_us = 20 },
	{ .reg = CLKCTRL_GP2PLL_CTRL0, .def = 0x10011086, .delay_us = 20 },
	{ .reg = CLKCTRL_GP2PLL_CTRL2, .def = 0x00023001, .delay_us = 20 }
};

struct meson_clk_pll_data p1_gp2_pll = {
	.name = "gp2",
	.en = {
		.reg = CLKCTRL_GP2PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = CLKCTRL_GP2PLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = CLKCTRL_GP2PLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = CLKCTRL_GP2PLL_CTRL0,
		.shift	 = 12,
		.width	 = 3,
	},
	.def_rate = p1_gp2_pll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_gp2_pll_default_rate),
	.table = p1_gp2_pll_table,
	.init_regs = p1_gp2_init_regs,
	.init_count = ARRAY_SIZE(p1_gp2_init_regs),
	.ops = &(const struct meson_pll_test_ops) {
		.pll_set_parm_rate = meson_pll_set_parm_rate,
	},
	.l = {
		.reg = CLKCTRL_GP2PLL_STS,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = CLKCTRL_GP2PLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.clkmsr_id = 22,
	.clkmsr_margin = 2
};

static const struct reg_sequence p1_fdle_init_regs[] = {
	{ .reg = CLKCTRL_FDLEPLL_CTRL0, .def = 0x20011086 },
	{ .reg = CLKCTRL_FDLEPLL_CTRL0, .def = 0x30011086 },
	{ .reg = CLKCTRL_FDLEPLL_CTRL1, .def = 0x1420500f },
	{ .reg = CLKCTRL_FDLEPLL_CTRL2, .def = 0x00023041 },
	{ .reg = CLKCTRL_FDLEPLL_CTRL3, .def = 0x0, .delay_us = 50 },
	{ .reg = CLKCTRL_FDLEPLL_CTRL0, .def = 0x10011086, .delay_us = 20 },
	{ .reg = CLKCTRL_FDLEPLL_CTRL2, .def = 0x00023001, .delay_us = 50 }
};

struct meson_clk_pll_data p1_fdle_pll = {
	.name = "fdle",
	.en = {
		.reg = CLKCTRL_FDLEPLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = CLKCTRL_FDLEPLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = CLKCTRL_FDLEPLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = CLKCTRL_FDLEPLL_CTRL0,
		.shift	 = 12,
		.width	 = 3,
	},
	.def_rate = p1_gp2_pll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_gp2_pll_default_rate),
	.table = p1_gp2_pll_table,
	.init_regs = p1_fdle_init_regs,
	.init_count = ARRAY_SIZE(p1_fdle_init_regs),
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
		.pll_set_parm_rate = meson_pll_set_parm_rate,
	},
	.l = {
		.reg = CLKCTRL_FDLEPLL_STS,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = CLKCTRL_FDLEPLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.clkmsr_id = 46,
	.clkmsr_margin = 2
};

static const struct reg_sequence p1_nna_init_regs[] = {
	{ .reg = CLKCTRL_NNAPLL_CTRL0, .def = 0x20011086 },
	{ .reg = CLKCTRL_NNAPLL_CTRL0, .def = 0x30011086 },
	{ .reg = CLKCTRL_NNAPLL_CTRL1, .def = 0x1420500f },
	{ .reg = CLKCTRL_NNAPLL_CTRL2, .def = 0x00023041 },
	{ .reg = CLKCTRL_NNAPLL_CTRL3, .def = 0x0, .delay_us = 50 },
	{ .reg = CLKCTRL_NNAPLL_CTRL0, .def = 0x10011086, .delay_us = 20 },
	{ .reg = CLKCTRL_NNAPLL_CTRL2, .def = 0x00023001, .delay_us = 50 }
};

struct meson_clk_pll_data p1_nna_pll = {
	.name = "nna",
	.en = {
		.reg = CLKCTRL_NNAPLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = CLKCTRL_NNAPLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = CLKCTRL_NNAPLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = CLKCTRL_NNAPLL_CTRL0,
		.shift	 = 12,
		.width	 = 3,
	},
	.def_rate = p1_gp2_pll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_gp2_pll_default_rate),
	.table = p1_gp2_pll_table,
	.init_regs = p1_nna_init_regs,
	.init_count = ARRAY_SIZE(p1_nna_init_regs),
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
		.pll_set_parm_rate = meson_pll_set_parm_rate,
	},
	.l = {
		.reg = CLKCTRL_NNAPLL_STS,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = CLKCTRL_NNAPLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.clkmsr_id = 44,
	.clkmsr_margin = 2
};

static const struct reg_sequence p1_m4_init_regs[] = {
	{ .reg = CLKCTRL_M4PLL_CTRL0, .def = 0x20011086 },
	{ .reg = CLKCTRL_M4PLL_CTRL0, .def = 0x30011086 },
	{ .reg = CLKCTRL_M4PLL_CTRL1, .def = 0x1420500f },
	{ .reg = CLKCTRL_M4PLL_CTRL2, .def = 0x00023041 },
	{ .reg = CLKCTRL_M4PLL_CTRL3, .def = 0x0, .delay_us = 50 },
	{ .reg = CLKCTRL_M4PLL_CTRL0, .def = 0x10011086, .delay_us = 20 },
	{ .reg = CLKCTRL_M4PLL_CTRL2, .def = 0x00023001, .delay_us = 50 }
};

struct meson_clk_pll_data p1_m4_pll = {
	.name = "m4",
	.en = {
		.reg = CLKCTRL_M4PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = CLKCTRL_M4PLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = CLKCTRL_M4PLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = CLKCTRL_M4PLL_CTRL0,
		.shift	 = 12,
		.width	 = 3,
	},
	.def_rate = p1_gp2_pll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_gp2_pll_default_rate),
	.table = p1_gp2_pll_table,
	.init_regs = p1_m4_init_regs,
	.init_count = ARRAY_SIZE(p1_m4_init_regs),
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
		.pll_set_parm_rate = meson_pll_set_parm_rate,
	},
	.l = {
		.reg = CLKCTRL_M4PLL_STS,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = CLKCTRL_M4PLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.clkmsr_id = 45,
	.clkmsr_margin = 2
};

unsigned long p1_pcie_pll_rate_to_msr(struct meson_clk_pll_data *pll, unsigned long rate)
{
	u8 od = (readl(ANACTRL_PCIEPLL_CTRL0) >> 16) & 0x1f;

	//printf("rate = %ld od= %d\n", rate, od);
	return (rate >> 2) / od;
}

static unsigned int p1_pcie_default_rate[] = {4800};

static const struct reg_sequence p1_pcie_init_regs[] = {
	{ .reg = ANACTRL_PCIEPLL_CTRL0,	.def = 0x200c04c8 },
	{ .reg = ANACTRL_PCIEPLL_CTRL0,	.def = 0x300c04c8 },
	{ .reg = ANACTRL_PCIEPLL_CTRL1,	.def = 0x30000000 },
	{ .reg = ANACTRL_PCIEPLL_CTRL2,	.def = 0x00001100 },
	{ .reg = ANACTRL_PCIEPLL_CTRL3,	.def = 0x10058e00 },
	{ .reg = ANACTRL_PCIEPLL_CTRL4,	.def = 0x000100c0 },
	{ .reg = ANACTRL_PCIEPLL_CTRL5,	.def = 0x68000048 },
	{ .reg = ANACTRL_PCIEPLL_CTRL5,	.def = 0x68000068, .delay_us = 20 },
	{ .reg = ANACTRL_PCIEPLL_CTRL4,	.def = 0x008100c0, .delay_us = 10 },
	{ .reg = ANACTRL_PCIEPLL_CTRL0,	.def = 0x340c04c8 },
	{ .reg = ANACTRL_PCIEPLL_CTRL0,	.def = 0x140c04c8, .delay_us = 10 },
	{ .reg = ANACTRL_PCIEPLL_CTRL2,	.def = 0x00001000 }
};

struct meson_clk_pll_data p1_pcie_pll = {
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
	.def_rate = p1_pcie_default_rate,
	.def_cnt = ARRAY_SIZE(p1_pcie_default_rate),
	.init_regs = p1_pcie_init_regs,
	.init_count = ARRAY_SIZE(p1_pcie_init_regs),
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
		.pll_set_parm_rate = meson_pll_set_parm_rate,
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
		.pll_set_rate = meson_pll_set_one_rate,
		.pll_rate_to_msr = p1_pcie_pll_rate_to_msr,
	},
	.clkmsr_id = 26,
	.clkmsr_margin = 2
};

static unsigned int p1_hifi_pll_default_rate[] = {375, 750};

/* set the od = 2, clkmsr can not measure 6000M */
static const struct pll_params_table p1_hifi_pll_table[] = {
	PLL_PARAMS(250, 1, 3), /* DCO = 6000M  PLL = 750M */
	PLL_PARAMS(125, 1, 3), /* DCO = 3000M  PLL = 375M */
	{ /* sentinel */  }
};

static const struct reg_sequence p1_hifi_init_regs[] = {
	{ .reg = CLKCTRL_HIFIPLL_CTRL0,	.def = 0x080304fa },
	{ .reg = CLKCTRL_HIFIPLL_CTRL0,	.def = 0x380304fa },
	{ .reg = CLKCTRL_HIFIPLL_CTRL1,	.def = 0x000061bd },
	{ .reg = CLKCTRL_HIFIPLL_CTRL2,	.def = 0x00000000 },
	{ .reg = CLKCTRL_HIFIPLL_CTRL3,	.def = 0x6a285c00 },
	{ .reg = CLKCTRL_HIFIPLL_CTRL4,	.def = 0x65771290 },
	{ .reg = CLKCTRL_HIFIPLL_CTRL5,	.def = 0x39272000 },
	{ .reg = CLKCTRL_HIFIPLL_CTRL6,	.def = 0x56540000, .delay_us = 50 },
	{ .reg = CLKCTRL_HIFIPLL_CTRL0,	.def = 0X180304fa, .delay_us = 50 },
};

struct meson_clk_pll_data p1_hifi_pll = {
	.name = "hifi",
	.en = {
		.reg = CLKCTRL_HIFIPLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = CLKCTRL_HIFIPLL_CTRL0,
		.shift   = 0,
		.width   = 9,
	},
	.n = {
		.reg = CLKCTRL_HIFIPLL_CTRL0,
		.shift   = 10,
		.width   = 5,
	},
	.frac = {
		.reg = CLKCTRL_HIFIPLL_CTRL1,
		.shift	 = 0,
		.width	 = 19,
	},
	.od = {
		.reg = CLKCTRL_HIFIPLL_CTRL0,
		.shift	 = 16,
		.width	 = 2,
	},
	.def_rate = p1_hifi_pll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_hifi_pll_default_rate),
	.table = p1_hifi_pll_table,
	.init_regs = p1_hifi_init_regs,
	.init_count = ARRAY_SIZE(p1_hifi_init_regs),
	.ops = &(const struct meson_pll_test_ops) {
		.pll_set_parm_rate = meson_pll_set_parm_rate,
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
		.pll_set_rate = meson_pll_set_one_rate,
	},
	.l = {
		.reg = CLKCTRL_HIFIPLL_CTRL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = CLKCTRL_HIFIPLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_set_parm_rate = meson_pll_set_parm_rate,
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
	},
	.clkmsr_id = 19,
	.clkmsr_margin = 2
};

static const struct reg_sequence p1_mclk_init_regs[] = {
	{ .reg = ANACTRL_MCLK_PLL_CNTL0, .def = 0x20010063 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL0, .def = 0x30010063 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL1, .def = 0x0480500f },
	{ .reg = ANACTRL_MCLK_PLL_CNTL2, .def = 0x00023041 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL3, .def = 0x18000000 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL4, .def = 0x01303003 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL5, .def = 0x00000008 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL6, .def = 0x01303003 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL7, .def = 0x00000008 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL8, .def = 0x01303003 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL9, .def = 0x00000008 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL10, .def = 0x01303003 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL11, .def = 0x00000008 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL12, .def = 0x01303003 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL13, .def = 0x00000008, .delay_us = 20 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL0, .def = 0x10010063, .delay_us = 20 },
	{ .reg = ANACTRL_MCLK_PLL_CNTL2, .def = 0x00023001, .delay_us = 20 }
};

static unsigned int p1_mclk_pll_default_rate[] = {148};

unsigned long meson_mclk_to_msr(struct meson_clk_pll_data *pll, unsigned long rate)
{
	return (unsigned long)p1_mclk_pll_default_rate[0] * 1000000;
}

struct meson_clk_pll_data p1_mclk_pll = {
	.name = "mclk",
	.en = {
		.reg = ANACTRL_MCLK_PLL_CNTL0,
		.shift = 28,
		.width = 1,
	},
	.m = {
		.reg = ANACTRL_MCLK_PLL_CNTL0,
		.shift = 0,
		.width = 8,
	},
	.n = {
		.reg = ANACTRL_MCLK_PLL_CNTL0,
		.shift = 16,
		.width = 5,
	},
	.od = {
		.reg = ANACTRL_MCLK_PLL_CNTL0,
		.shift = 12,
		.width = 3,
	},
	.def_rate = p1_mclk_pll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_mclk_pll_default_rate),
	.init_regs = p1_mclk_init_regs,
	.init_count = ARRAY_SIZE(p1_mclk_init_regs),
	.ops = &(const struct meson_pll_test_ops) {
		.pll_set_rate = meson_pll_set_one_rate,
		.pll_set_parm_rate = meson_pll_set_parm_rate,
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
		.pll_rate_to_msr = meson_mclk_to_msr,
	},
	.l = {
		.reg = ANACTRL_MCLK_PLL_STS,
		.shift = 31,
		.width = 1,
	},
	.rst = {
		.reg = ANACTRL_MCLK_PLL_CNTL0,
		.shift = 29,
		.width  = 1,
	},
	.clkmsr_id = 171,
	.clkmsr_margin = 2
};

static unsigned int p1_mpll_default_rate[] = {100};
static struct meson_pll_test_ops mpll_test_ops = {
	.pll_prepare = meson_pll_store_rate,
	.pll_unprepare = meson_pll_restore_rate,
	.pll_set_parm_rate = meson_mpll_set_parm_rate,
	.pll_set_rate = meson_mpll_set_rate,
	.pll_recalc = meson_mpll_recalc_rate
};

static const struct reg_sequence p1_mpll0_init_regs[] = {
	{ .reg = CLKCTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = CLKCTRL_MPLL_CTRL1,	.def = 0xc14003e7 },
	{ .reg = CLKCTRL_MPLL_CTRL2,	.def = 0x60000033,	.delay_us = 50},
};

struct meson_clk_pll_data p1_mpll0 = {
	.name = "mpll0",
	.sdm = {
		.reg = CLKCTRL_MPLL_CTRL1,
		.shift   = 0,
		.width   = 14,
	},
	.sdm_en = {
		.reg = CLKCTRL_MPLL_CTRL1,
		.shift   = 30,
		.width   = 1,
	},
	.n2 = {
		.reg = CLKCTRL_MPLL_CTRL1,
		.shift   = 20,
		.width   = 9,
	},
	.ssen = {
		.reg = CLKCTRL_MPLL_CTRL1,
		.shift   = 29,
		.width   = 1,
	},
	.en = {
		.reg = CLKCTRL_MPLL_CTRL1,
		.shift   = 31,
		.width   = 1,
	},
	.init_regs = p1_mpll0_init_regs,
	.init_count = ARRAY_SIZE(p1_mpll0_init_regs),
	.def_rate = p1_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 11,
	.clkmsr_margin = 2
};

static const struct reg_sequence p1_mpll1_init_regs[] = {
	{ .reg = CLKCTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = CLKCTRL_MPLL_CTRL3,	.def = 0xc14003e7 },
	{ .reg = CLKCTRL_MPLL_CTRL4,	.def = 0x60000033,	.delay_us = 50},
};

struct meson_clk_pll_data p1_mpll1 = {
	.name = "mpll1",
	.sdm = {
		.reg = CLKCTRL_MPLL_CTRL3,
		.shift   = 0,
		.width   = 14,
	},
	.sdm_en = {
		.reg = CLKCTRL_MPLL_CTRL3,
		.shift   = 30,
		.width   = 1,
	},
	.n2 = {
		.reg = CLKCTRL_MPLL_CTRL3,
		.shift   = 20,
		.width   = 9,
	},
	.ssen = {
		.reg = CLKCTRL_MPLL_CTRL3,
		.shift   = 29,
		.width   = 1,
	},
	.en = {
		.reg = CLKCTRL_MPLL_CTRL3,
		.shift   = 31,
		.width   = 1,
	},
	.init_regs = p1_mpll1_init_regs,
	.init_count = ARRAY_SIZE(p1_mpll1_init_regs),
	.def_rate = p1_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 12,
	.clkmsr_margin = 2
};

static const struct reg_sequence p1_mpll2_init_regs[] = {
	{ .reg = CLKCTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = CLKCTRL_MPLL_CTRL5,	.def = 0xc14003e7 },
	{ .reg = CLKCTRL_MPLL_CTRL6,	.def = 0x60000033,	.delay_us = 50},
};

struct meson_clk_pll_data p1_mpll2 = {
	.name = "mpll2",
	.sdm = {
		.reg = CLKCTRL_MPLL_CTRL5,
		.shift   = 0,
		.width   = 14,
	},
	.sdm_en = {
		.reg = CLKCTRL_MPLL_CTRL5,
		.shift   = 30,
		.width   = 1,
	},
	.n2 = {
		.reg = CLKCTRL_MPLL_CTRL5,
		.shift   = 20,
		.width   = 9,
	},
	.ssen = {
		.reg = CLKCTRL_MPLL_CTRL5,
		.shift   = 29,
		.width   = 1,
	},
	.en = {
		.reg = CLKCTRL_MPLL_CTRL5,
		.shift   = 31,
		.width   = 1,
	},
	.init_regs = p1_mpll2_init_regs,
	.init_count = ARRAY_SIZE(p1_mpll2_init_regs),
	.def_rate = p1_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 13,
	.clkmsr_margin = 2
};

static const struct reg_sequence p1_mpll3_init_regs[] = {
	{ .reg = CLKCTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = CLKCTRL_MPLL_CTRL7,	.def = 0xc14003e7 },
	{ .reg = CLKCTRL_MPLL_CTRL8,	.def = 0x60000033,	.delay_us = 50},
};

struct meson_clk_pll_data p1_mpll3 = {
	.name = "mpll3",
	.sdm = {
		.reg = CLKCTRL_MPLL_CTRL7,
		.shift   = 0,
		.width   = 14,
	},
	.sdm_en = {
		.reg = CLKCTRL_MPLL_CTRL7,
		.shift   = 30,
		.width   = 1,
	},
	.n2 = {
		.reg = CLKCTRL_MPLL_CTRL7,
		.shift   = 20,
		.width   = 9,
	},
	.ssen = {
		.reg = CLKCTRL_MPLL_CTRL7,
		.shift   = 29,
		.width   = 1,
	},
	.en = {
		.reg = CLKCTRL_MPLL_CTRL7,
		.shift   = 31,
		.width   = 1,
	},
	.init_regs = p1_mpll3_init_regs,
	.init_count = ARRAY_SIZE(p1_mpll3_init_regs),
	.def_rate = p1_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(p1_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 14,
	.clkmsr_margin = 2
};

struct meson_clk_pll_data *p1_pll_list[] = {
	&p1_sys0_pll,
	&p1_sys1_pll,
	&p1_gp0_pll,
	&p1_gp1_pll,
	&p1_gp2_pll,
	&p1_pcie_pll,
	&p1_hifi_pll,
	&p1_fdle_pll,
	&p1_nna_pll,
	&p1_m4_pll,
	&p1_mpll0,
	&p1_mpll1,
	&p1_mpll2,
	&p1_mpll3,
	&p1_mclk_pll
};

void p1_meson_plls_all_test(void)
{
	for (int i = 0; i < ARRAY_SIZE(p1_pll_list); i++)
		meson_pll_test(p1_pll_list[i]);
}

void p1_meson_hdmirx_plls_test(int argc, char * const argv[])
{
	/* todo: need add hdmirx plls test */
}

void p1_meson_tcon_plls_test(int argc, char * const argv[])
{
	/* todo: need add tcon plls test */
}

void p1_meson_plls_test(int argc, char * const argv[])
{
	struct meson_clk_pll_data *pll;

	pll = meson_pll_find_by_name(p1_pll_list, ARRAY_SIZE(p1_pll_list), argv[1]);
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

	return;
}

int pll_test(int argc, char * const argv[])
{
	if (!strcmp("all", argv[1]))
		p1_meson_plls_all_test();
	else if (!strcmp("tcon", argv[1]))
		p1_meson_tcon_plls_test(argc, argv);
	else if (!strcmp("hdmirx", argv[1]))
		p1_meson_hdmirx_plls_test(argc, argv);
	else
		p1_meson_plls_test(argc, argv);

	return 0;
}
