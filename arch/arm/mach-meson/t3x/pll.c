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
	SECID_DSU_PRE_CLK_SEL,
	SECID_DSU_PRE_CLK_RD,
	SECID_DSU_PRE_CLK_DYN,
	SECID_DSU_CLK_SEL,
	SECID_DSU_CLK_RD,
	SECID_PLL_TEST,
};


int t3_sys0_pll_prepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	/* store rate */
	meson_pll_store_rate(pll);

	/* Set fixed clk to 1G, Switch to fixed clk first */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_CPU_CLK_DYN,
	1, 0, 0, 0, 0, 0, &res);

	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_CPU_CLK_SEL, 0);

	/* Switch dsu to cpu0 */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_DSU_CLK_SEL,
	(0x1 << 27), (0 << 27), 0, 0, 0, 0, &res);

	return 0;
}

void t3_sys0_pll_unprepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	/* restore rate */
	meson_pll_restore_rate(pll);

	/* Switch back cpu to sys pll */
	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_CPU_CLK_SEL, 1);

	/* Switch back dsu to gp1 pll */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_DSU_CLK_SEL,
	(0x1 << 27), (0x1 << 27), 0, 0, 0, 0, &res);
}

static const struct pll_params_table t3_sys_pll_table[] = {
	PLL_PARAMS(63, 1, 0), /*DCO=1512M OD=1512M restore*/
	PLL_PARAMS(66, 1, 1), /*DCO=1584M OD=792M*/
	PLL_PARAMS(134, 1, 1), /*DCO=3216M OD=1608M*/
	{ /* sentinel */ }
};

static unsigned int t3_sys_pll_default_rate[] = {792, 1608};
static const struct reg_sequence t3_sys0_pll_init_regs[] = {
	{ .reg = ANACTRL_SYS0PLL_CTRL0, .def = 0x20011086 },
	{ .reg = ANACTRL_SYS0PLL_CTRL0, .def = 0x30011086 },
	{ .reg = ANACTRL_SYS0PLL_CTRL1, .def = 0x1420500f },
	{ .reg = ANACTRL_SYS0PLL_CTRL2, .def = 0x00023041 },
	{ .reg = ANACTRL_SYS0PLL_CTRL3, .def = 0x0 },
	{ .reg = ANACTRL_SYS0PLL_CTRL0, .def = 0x10011086 },
	{ .reg = ANACTRL_SYS0PLL_CTRL2, .def = 0x00023001 }
};

struct meson_clk_pll_data t3_sys0_pll = {
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
	.def_rate = t3_sys_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t3_sys_pll_default_rate),
	.table = t3_sys_pll_table,
	.init_regs = t3_sys0_pll_init_regs,
	.init_count = ARRAY_SIZE(t3_sys0_pll_init_regs),
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
		.pll_prepare = t3_sys0_pll_prepare,
		.pll_unprepare = t3_sys0_pll_unprepare,
		.pll_disable = meson_secure_pll_disable,
		.pll_set_rate = meson_secure_pll_set_rate,
		.pll_set_parm_rate = meson_secure_pll_set_parm_rate,
		.pll_rate_to_msr = meson_div16_to_msr,

	},
	.smc_id = SECURE_PLL_CLK,
	.secid_disable = SECID_SYS0_DCO_PLL_DIS,
	.secid_test = SECID_PLL_TEST,
	.secid = SECID_SYS0_DCO_PLL,
	.clkmsr_id = 23,
	.clkmsr_margin = 10,
};

static const struct reg_sequence t3_sys1_pll_init_regs[] = {
	{ .reg = ANACTRL_SYS1PLL_CTRL0, .def = 0x20011086 },
	{ .reg = ANACTRL_SYS1PLL_CTRL0, .def = 0x30011086 },
	{ .reg = ANACTRL_SYS1PLL_CTRL1, .def = 0x1420500f },
	{ .reg = ANACTRL_SYS1PLL_CTRL2, .def = 0x00023041 },
	{ .reg = ANACTRL_SYS1PLL_CTRL3, .def = 0x0 },
	{ .reg = ANACTRL_SYS1PLL_CTRL0, .def = 0x10011086 },
	{ .reg = ANACTRL_SYS1PLL_CTRL2, .def = 0x00023001 }
};

struct meson_clk_pll_data t3_sys1_pll = {
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
	.def_rate = t3_sys_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t3_sys_pll_default_rate),
	.table = t3_sys_pll_table,
	.init_regs = t3_sys1_pll_init_regs,
	.init_count = ARRAY_SIZE(t3_sys1_pll_init_regs),
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
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
		.pll_disable = meson_secure_pll_disable,
		.pll_set_rate = meson_secure_pll_set_rate,
		.pll_set_parm_rate = meson_secure_pll_set_parm_rate,
	},
	.smc_id = SECURE_PLL_CLK,
	.secid_disable = SECID_SYS1_DCO_PLL_DIS,
	.secid_test = SECID_PLL_TEST,
	.secid = SECID_SYS1_DCO_PLL,
	.clkmsr_id = 76,
	.clkmsr_margin = 2
};

static unsigned int t3_gp0_pll_default_rate[] = {375, 750};

/* set the od = 2, clkmsr can not measure 6000M */
static const struct pll_params_table t3_gp0_pll_table[] = {
	PLL_PARAMS(192, 1, 2), /* DCO = 4608M  OD = 1152M for restore */
	PLL_PARAMS(125, 1, 2), /* DCO = 3000M  OD = 750M */
	PLL_PARAMS(250, 1, 4), /* DCO = 6000M  OD = 375M */
	{ /* sentinel */  }
};

static const struct reg_sequence t3_gp0_init_regs[] = {
	{ .reg = ANACTRL_GP0PLL_CTRL0,  .def = 0x080304fa },
	{ .reg = ANACTRL_GP0PLL_CTRL0,  .def = 0x380304fa },
	{ .reg = ANACTRL_GP0PLL_CTRL1,  .def = 0x00000000 },
	{ .reg = ANACTRL_GP0PLL_CTRL2,  .def = 0x00000000 },
	{ .reg = ANACTRL_GP0PLL_CTRL3,  .def = 0x48681c00 },
	{ .reg = ANACTRL_GP0PLL_CTRL4,  .def = 0x88770290 },
	{ .reg = ANACTRL_GP0PLL_CTRL5,  .def = 0x39272000 },
	{ .reg = ANACTRL_GP0PLL_CTRL6,  .def = 0x56540000, .delay_us = 50 },
	{ .reg = ANACTRL_GP0PLL_CTRL0,  .def = 0x180304fa, .delay_us = 50 }
};

struct meson_clk_pll_data t3_gp0_pll = {
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
	.def_rate = t3_gp0_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t3_gp0_pll_default_rate),
	.table = t3_gp0_pll_table,
	.init_regs = t3_gp0_init_regs,
	.init_count = ARRAY_SIZE(t3_gp0_init_regs),
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
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
	},
	.clkmsr_id = 20,
	.clkmsr_margin = 2
};

int t3_gp1_pll_prepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	meson_pll_store_rate(pll);

	/* Set fixed clk to 1G, Switch to fixed clk first */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_CPU_CLK_DYN,
	1, 0, 0, 0, 0, 0, &res);

	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_CPU_CLK_SEL, 0);

	/* Switch dsu to cpu0 */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_DSU_CLK_SEL,
	(0x1 << 27), (0 << 27), 0, 0, 0, 0, &res);

	return 0;
}

void t3_gp1_pll_unprepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	meson_pll_restore_rate(pll);

	/* Switch back cpu to sys pll */
	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_CPU_CLK_SEL, 1);

	/* Switch back dsu to gp1 pll */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_DSU_CLK_SEL,
	(0x1 << 27), (0x1 << 27), 0, 0, 0, 0, &res);
}

static unsigned int t3_gp1_pll_default_rate[] = {792, 1608};

static const struct pll_params_table t3_gp1_pll_table[] = {
	PLL_PARAMS(100, 1, 1), /* DCO = 2400M  PLL = 1200M for restore */
	PLL_PARAMS(66, 1, 1), /* DCO = 1584M  PLL = 792M */
	PLL_PARAMS(134, 1, 1), /* DCO = 3216M  PLL = 1608M */
	{ /* sentinel */  }
};

static const struct reg_sequence t3_gp1_init_regs[] = {
	{ .reg = ANACTRL_GP1PLL_CTRL0,  .def = 0x20011086 },
	{ .reg = ANACTRL_GP1PLL_CTRL0,  .def = 0x30011086 },
	{ .reg = ANACTRL_GP1PLL_CTRL1,  .def = 0x1420500f },
	{ .reg = ANACTRL_GP1PLL_CTRL2,  .def = 0x00023041 },
	{ .reg = ANACTRL_GP1PLL_CTRL3,  .def = 0x0, .delay_us = 20 },
	{ .reg = ANACTRL_GP1PLL_CTRL0,  .def = 0x10011086, .delay_us = 20 },
	{ .reg = ANACTRL_GP1PLL_CTRL2,  .def = 0x00023001, .delay_us = 50 },
};

struct meson_clk_pll_data t3_gp1_pll = {
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
	.def_rate = t3_gp1_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t3_gp1_pll_default_rate),
	.table = t3_gp1_pll_table,
	.init_regs = t3_gp1_init_regs,
	.init_count = ARRAY_SIZE(t3_gp1_init_regs),
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
		.pll_prepare = t3_gp1_pll_prepare,
		.pll_unprepare = t3_gp1_pll_unprepare,
	},
	.clkmsr_id = 21,
	.clkmsr_margin = 2
};

unsigned long t3_pcie_pll_rate_to_msr(struct meson_clk_pll_data *pll, unsigned long rate)
{
	u8 od = (readl(ANACTRL_PCIEPLL_CTRL0) >> 16) & 0x1f;

	//printf("rate = %ld od= %d\n", rate, od);
	return (rate >> 2) / od;
}

static unsigned int t3_pcie_default_rate[] = {4800};

static const struct reg_sequence t3_pcie_init_regs[] = {
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

struct meson_clk_pll_data t3_pcie_pll = {
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
	.def_rate = t3_pcie_default_rate,
	.def_cnt = ARRAY_SIZE(t3_pcie_default_rate),
	.init_regs = t3_pcie_init_regs,
	.init_count = ARRAY_SIZE(t3_pcie_init_regs),
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
		.pll_rate_to_msr = t3_pcie_pll_rate_to_msr,
	},
	.clkmsr_id = 16,
	.clkmsr_margin = 2
};

unsigned long t3_adc_pll_rate_to_msr(struct meson_clk_pll_data *pll, unsigned long rate)
{
	u8 pre_div, xd;

	pre_div = (readl(ANACTRL_ADC_PLL_CNTL) & (0x1 << 24)) ? 4 : 2;
	xd = (readl(ANACTRL_ADC_PLL_CNTL1) >> 22) & 0x1f;

	//printf("rate = %ld pre_div = %d xd = %d\n", rate, pre_div, xd);
	return rate / pre_div / xd;
}

static unsigned int t3_adc_default_rate[] = {6000};

static const struct reg_sequence t3_adc_init_regs[] = {
	{ .reg = ANACTRL_VDAC_CTRL0,	.def = 0x00000800 },
	{ .reg = ANACTRL_S2_DADC_CNTL2,	.def = 0x00000582 },
	{ .reg = ANACTRL_DADC_CNTL2,	.def = 0x00000008 },
	{ .reg = ANACTRL_ADC_PLL_CNTL,	.def = 0x210504fa },
	{ .reg = ANACTRL_ADC_PLL_CNTL,	.def = 0x310504fa },
	{ .reg = ANACTRL_ADC_PLL_CNTL1,	.def = 0x01800000 },
	{ .reg = ANACTRL_ADC_PLL_CNTL2,	.def = 0x00000000 },
	{ .reg = ANACTRL_ADC_PLL_CNTL3,	.def = 0x48681f00, .delay_us = 20 },
	{ .reg = ANACTRL_ADC_PLL_CNTL4,	.def = 0x88770290, .delay_us = 10 },
	{ .reg = ANACTRL_ADC_PLL_CNTL5,	.def = 0x39272000 },
	{ .reg = ANACTRL_ADC_PLL_CNTL6,	.def = 0x56540000, .delay_us = 10 },
	{ .reg = ANACTRL_ADC_PLL_CNTL,	.def = 0x110504fa }
};

struct meson_clk_pll_data t3_adc_pll = {
	.name = "adc",
	.en = {
		.reg = ANACTRL_ADC_PLL_CNTL,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = ANACTRL_ADC_PLL_CNTL,
		.shift   = 0,
		.width   = 9,
	},
	.n = {
		.reg = ANACTRL_ADC_PLL_CNTL,
		.shift   = 10,
		.width   = 5,
	},
	.def_rate = t3_adc_default_rate,
	.def_cnt = ARRAY_SIZE(t3_adc_default_rate),
	.init_regs = t3_adc_init_regs,
	.init_count = ARRAY_SIZE(t3_adc_init_regs),
	.rst = {
		.reg = ANACTRL_ADC_PLL_CNTL,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
		.pll_set_rate = meson_pll_set_one_rate,
		.pll_rate_to_msr = t3_adc_pll_rate_to_msr,
	},
	.clkmsr_id = 52,
	.clkmsr_margin = 2
};

static unsigned int t3_hifi_pll_default_rate[] = {375, 750};

/* set the od = 2, clkmsr can not measure 6000M */
static const struct pll_params_table t3_hifi_pll_table[] = {
	PLL_PARAMS(250, 1, 3), /* DCO = 6000M  PLL = 750M */
	PLL_PARAMS(125, 1, 3), /* DCO = 3000M  PLL = 375M */
	{ /* sentinel */  }
};

static const struct reg_sequence t3_hifi_init_regs[] = {
	{ .reg = ANACTRL_HIFIPLL_CTRL0,	.def = 0x080304fa },
	{ .reg = ANACTRL_HIFIPLL_CTRL0,	.def = 0x380304fa },
	{ .reg = ANACTRL_HIFIPLL_CTRL1,	.def = 0x000061bd },
	{ .reg = ANACTRL_HIFIPLL_CTRL2,	.def = 0x00000000 },
	{ .reg = ANACTRL_HIFIPLL_CTRL3,	.def = 0x6a285c00 },
	{ .reg = ANACTRL_HIFIPLL_CTRL4,	.def = 0x65771290 },
	{ .reg = ANACTRL_HIFIPLL_CTRL5,	.def = 0x39272000 },
	{ .reg = ANACTRL_HIFIPLL_CTRL6,	.def = 0x56540000, .delay_us = 50 },
	{ .reg = ANACTRL_HIFIPLL_CTRL0,	.def = 0X180304fa, .delay_us = 50 },
};

struct meson_clk_pll_data t3_hifi_pll = {
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
	.def_rate = t3_hifi_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t3_hifi_pll_default_rate),
	.table = t3_hifi_pll_table,
	.init_regs = t3_hifi_init_regs,
	.init_count = ARRAY_SIZE(t3_hifi_init_regs),
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
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
	},
	.clkmsr_id = 19,
	.clkmsr_margin = 2
};

static unsigned int t3_mpll_default_rate[] = {100};
static struct meson_pll_test_ops mpll_test_ops = {
	.pll_prepare = meson_pll_store_rate,
	.pll_unprepare = meson_pll_restore_rate,
	.pll_set_parm_rate = meson_mpll_set_parm_rate,
	.pll_set_rate = meson_mpll_set_rate,
	.pll_recalc = meson_mpll_recalc_rate
};

static const struct reg_sequence t3_mpll0_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL1,	.def = 0xc14003e7 },
	{ .reg = ANACTRL_MPLL_CTRL2,	.def = 0x60000033,	.delay_us = 50},
};

struct meson_clk_pll_data t3_mpll0 = {
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
	.init_regs = t3_mpll0_init_regs,
	.init_count = ARRAY_SIZE(t3_mpll0_init_regs),
	.def_rate = t3_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(t3_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 11,
	.clkmsr_margin = 2
};

static const struct reg_sequence t3_mpll1_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL3,	.def = 0xc14003e7 },
	{ .reg = ANACTRL_MPLL_CTRL4,	.def = 0x60000033,	.delay_us = 50},
};

struct meson_clk_pll_data t3_mpll1 = {
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
	.init_regs = t3_mpll1_init_regs,
	.init_count = ARRAY_SIZE(t3_mpll1_init_regs),
	.def_rate = t3_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(t3_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 12,
	.clkmsr_margin = 2
};

static const struct reg_sequence t3_mpll2_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL5,	.def = 0xc14003e7 },
	{ .reg = ANACTRL_MPLL_CTRL6,	.def = 0x60000033,	.delay_us = 50},
};

struct meson_clk_pll_data t3_mpll2 = {
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
	.init_regs = t3_mpll2_init_regs,
	.init_count = ARRAY_SIZE(t3_mpll2_init_regs),
	.def_rate = t3_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(t3_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 13,
	.clkmsr_margin = 2
};

static const struct reg_sequence t3_mpll3_init_regs[] = {
	{ .reg = ANACTRL_MPLL_CTRL0,	.def = 0x00000543 },
	{ .reg = ANACTRL_MPLL_CTRL7,	.def = 0xc14003e7 },
	{ .reg = ANACTRL_MPLL_CTRL8,	.def = 0x60000033,	.delay_us = 50},
};

struct meson_clk_pll_data t3_mpll3 = {
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
	.init_regs = t3_mpll3_init_regs,
	.init_count = ARRAY_SIZE(t3_mpll3_init_regs),
	.def_rate = t3_mpll_default_rate,
	.def_cnt = ARRAY_SIZE(t3_mpll_default_rate),
	.ops = &mpll_test_ops,
	.clkmsr_id = 14,
	.clkmsr_margin = 2
};

struct meson_clk_pll_data *t3_pll_list[] = {
	&t3_sys0_pll,
	&t3_sys1_pll,
	&t3_gp0_pll,
	&t3_gp1_pll,
	&t3_pcie_pll,
	&t3_hifi_pll,
	&t3_adc_pll,
	&t3_mpll0,
	&t3_mpll1,
	&t3_mpll2,
	&t3_mpll3
};

void t3_meson_plls_all_test(void)
{
	for (int i = 0; i < ARRAY_SIZE(t3_pll_list); i++)
		meson_pll_test(t3_pll_list[i]);
}

void t3_meson_hdmirx_plls_test(int argc, char * const argv[])
{
	/* todo: need add hdmirx plls test */
}

void t3_meson_tcon_plls_test(int argc, char * const argv[])
{
	/* todo: need add tcon plls test */
}

void t3_meson_plls_test(int argc, char * const argv[])
{
	struct meson_clk_pll_data *pll;

	pll = meson_pll_find_by_name(t3_pll_list, ARRAY_SIZE(t3_pll_list), argv[1]);
	if (!pll) {
		printf("The pll is not supported Or wrong pll name\n");
		return;
	}

	if ((argc - 2) == pll->init_count)
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
		t3_meson_plls_all_test();
	else if (!strcmp("tcon", argv[1]))
		t3_meson_tcon_plls_test(argc, argv);
	else if (!strcmp("hdmirx", argv[1]))
		t3_meson_hdmirx_plls_test(argc, argv);
	else
		t3_meson_plls_test(argc, argv);

	return 0;
}

