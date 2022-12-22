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
	SECID_FIX_PLL_OD,
	SECID_FIX_DCO_PLL,
	SECID_FIX_DCO_PLL_DIS,
	SECID_SYS_CLK_RD,
	SECID_SYS_CLK_DYN,
	SECID_AXI_CLK_RD,
	SECID_AXI_CLK_DYN,
	SECID_PLL_TEST,
};


int t5m_sys0_pll_prepare(struct meson_clk_pll_data *pll)
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
	(0x1 << 31), (0 << 31), 0, 0, 0, 0, &res);

	return 0;
}

void t5m_sys0_pll_unprepare(struct meson_clk_pll_data *pll)
{
	struct arm_smccc_res res;

	/* restore rate */
	meson_pll_restore_rate(pll);

	/* Switch back cpu to sys pll */
	meson_switch_cpu_clk(SECURE_CPU_CLK, SECID_CPU_CLK_SEL, 1);

	/* Switch back dsu to sys1 pll */
	arm_smccc_smc(SECURE_CPU_CLK, SECID_DSU_CLK_SEL,
	(0x1 << 31), (0x1 << 31), 0, 0, 0, 0, &res);
}

static const struct pll_params_table t5m_sys_pll_table[] = {
	PLL_PARAMS(80, 1, 0), /*DCO=1920M OD=1920M restore*/
	PLL_PARAMS(126, 1, 1), /*DCO=3024M OD=1512M*/
	{ /* sentinel */ }
};

static unsigned int t5m_sys_pll_default_rate[] = {1512, 1920};

static const struct reg_sequence t5m_sys0_pll_init_regs[] = {
	{ .reg = ANACTRL_SYS0PLL_CTRL0, .def = 0x20011086 },
	{ .reg = ANACTRL_SYS0PLL_CTRL0, .def = 0x30011086 },
	{ .reg = ANACTRL_SYS0PLL_CTRL1, .def = 0x1420500f },
	{ .reg = ANACTRL_SYS0PLL_CTRL2, .def = 0x00023041 },
	{ .reg = ANACTRL_SYS0PLL_CTRL3, .def = 0x0 },
	{ .reg = ANACTRL_SYS0PLL_CTRL0, .def = 0x10011086 },
	{ .reg = ANACTRL_SYS0PLL_CTRL2, .def = 0x00023001 }
};

struct meson_clk_pll_data t5m_sys0_pll = {
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
	.def_rate = t5m_sys_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t5m_sys_pll_default_rate),
	.table = t5m_sys_pll_table,
	.init_regs = t5m_sys0_pll_init_regs,
	.init_count = ARRAY_SIZE(t5m_sys0_pll_init_regs),
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
		.pll_prepare = t5m_sys0_pll_prepare,
		.pll_unprepare = t5m_sys0_pll_unprepare,
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

static const struct reg_sequence t5m_sys1_pll_init_regs[] = {
	{ .reg = ANACTRL_SYS1PLL_CTRL0, .def = 0x20011086 },
	{ .reg = ANACTRL_SYS1PLL_CTRL0, .def = 0x30011086 },
	{ .reg = ANACTRL_SYS1PLL_CTRL1, .def = 0x1420500f },
	{ .reg = ANACTRL_SYS1PLL_CTRL2, .def = 0x00023041 },
	{ .reg = ANACTRL_SYS1PLL_CTRL3, .def = 0x0 },
	{ .reg = ANACTRL_SYS1PLL_CTRL0, .def = 0x10011086 },
	{ .reg = ANACTRL_SYS1PLL_CTRL2, .def = 0x00023001 }
};

struct meson_clk_pll_data t5m_sys1_pll = {
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
	.def_rate = t5m_sys_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t5m_sys_pll_default_rate),
	.table = t5m_sys_pll_table,
	.init_regs = t5m_sys1_pll_init_regs,
	.init_count = ARRAY_SIZE(t5m_sys1_pll_init_regs),
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
		.pll_prepare = t5m_sys0_pll_prepare,
		.pll_unprepare = t5m_sys0_pll_unprepare,
		.pll_disable = meson_secure_pll_disable,
		.pll_set_rate = meson_secure_pll_set_rate,
		.pll_set_parm_rate = meson_secure_pll_set_parm_rate,
		.pll_rate_to_msr = meson_div16_to_msr,
	},
	.smc_id = SECURE_PLL_CLK,
	.secid_disable = SECID_SYS1_DCO_PLL_DIS,
	.secid_test = SECID_PLL_TEST,
	.secid = SECID_SYS1_DCO_PLL,
	.clkmsr_id = 23,
	.clkmsr_margin = 2
};

static unsigned int t5m_gp0_pll_default_rate[] = {375, 750};

/* set the od = 2, clkmsr can not measure 6000M */
static const struct pll_params_table t5m_gp0_pll_table[] = {
	PLL_PARAMS(126, 1, 1), /* DCO = 3024M  OD = 1152M for restore */
	PLL_PARAMS(125, 1, 2), /* DCO = 3000M  OD = 750M */
	PLL_PARAMS(125, 1, 3), /* DCO = 3000M  OD = 375M */
	{ /* sentinel */  }
};

static const struct reg_sequence t5m_gp0_init_regs[] = {
	{ .reg = ANACTRL_GP0PLL_CTRL0,  .def = 0x20010460 },
	{ .reg = ANACTRL_GP0PLL_CTRL0,  .def = 0x30010460 },
	{ .reg = ANACTRL_GP0PLL_CTRL1,  .def = 0x03a00000 },
	{ .reg = ANACTRL_GP0PLL_CTRL2,  .def = 0x00040000 },
	{ .reg = ANACTRL_GP0PLL_CTRL3,  .def = 0x090da000 },
	{ .reg = ANACTRL_GP0PLL_CTRL0,  .def = 0x10010460 },
	{ .reg = ANACTRL_GP0PLL_CTRL3,  .def = 0x090da200 },
};

struct meson_clk_pll_data t5m_gp0_pll = {
	.name = "gp0",
	.en = {
		.reg = ANACTRL_GP0PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = ANACTRL_GP0PLL_CTRL0,
		.shift   = 0,
		.width   = 9,
	},
	.n = {
		.reg = ANACTRL_GP0PLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.od = {
		.reg = ANACTRL_GP0PLL_CTRL0,
		.shift	 = 10,
		.width	 = 3,
	},
	.def_rate = t5m_gp0_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t5m_gp0_pll_default_rate),
	.table = t5m_gp0_pll_table,
	.init_regs = t5m_gp0_init_regs,
	.init_count = ARRAY_SIZE(t5m_gp0_init_regs),
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

static const struct pll_params_table t5m_hifi_pll_table[] = {
	PLL_PARAMS(126, 1, 1), /* DCO = 3000M  PLL = 1512M */
	PLL_PARAMS(67, 1, 0), /* DCO = 16080M  PLL = 1608M */
	{ /* sentinel */  }
};

static unsigned int t5m_hifi_pll_default_rate[] = {1512, 1608};

static const struct reg_sequence t5m_hifi_init_regs[] = {
	{ .reg = ANACTRL_HIFIPLL_CTRL0, .def = 0x20010460 },
	{ .reg = ANACTRL_HIFIPLL_CTRL0, .def = 0x30010460 },
	{ .reg = ANACTRL_HIFIPLL_CTRL1,	.def = 0x03a00000 },
	{ .reg = ANACTRL_HIFIPLL_CTRL2,	.def = 0x00040000 },
	{ .reg = ANACTRL_HIFIPLL_CTRL3,	.def = 0x090da000 },
	{ .reg = ANACTRL_HIFIPLL_CTRL0, .def = 0x10010460 },
	{ .reg = ANACTRL_HIFIPLL_CTRL3, .def = 0x090da200 },
};

struct meson_clk_pll_data t5m_hifi_pll = {
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
		.shift   = 16,
		.width   = 5,
	},
	.frac = {
		.reg = ANACTRL_HIFIPLL_CTRL1,
		.shift	 = 0,
		.width	 = 19,
	},
	.od = {
		.reg = ANACTRL_HIFIPLL_CTRL0,
		.shift	 = 10,
		.width	 = 3,
	},
	.def_rate = t5m_hifi_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t5m_hifi_pll_default_rate),
	.table = t5m_hifi_pll_table,
	.init_regs = t5m_hifi_init_regs,
	.init_count = ARRAY_SIZE(t5m_hifi_init_regs),
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

static const struct reg_sequence t5m_hifi1_init_regs[] = {
	{ .reg = ANACTRL_HIFI1PLL_CTRL0, .def = 0x20010460 },
	{ .reg = ANACTRL_HIFI1PLL_CTRL0, .def = 0x30010460 },
	{ .reg = ANACTRL_HIFI1PLL_CTRL1, .def = 0x03a00000 },
	{ .reg = ANACTRL_HIFI1PLL_CTRL2, .def = 0x00040000 },
	{ .reg = ANACTRL_HIFI1PLL_CTRL3, .def = 0x090da000 },
	{ .reg = ANACTRL_HIFI1PLL_CTRL0, .def = 0x10010460 },
	{ .reg = ANACTRL_HIFI1PLL_CTRL3, .def = 0x090da200 },
};

struct meson_clk_pll_data t5m_hifi1_pll = {
	.name = "hifi1",
	.en = {
		.reg = ANACTRL_HIFI1PLL_CTRL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = ANACTRL_HIFI1PLL_CTRL0,
		.shift   = 0,
		.width   = 8,
	},
	.n = {
		.reg = ANACTRL_HIFI1PLL_CTRL0,
		.shift   = 16,
		.width   = 5,
	},
	.frac = {
		.reg = ANACTRL_HIFI1PLL_CTRL1,
		.shift	 = 0,
		.width	 = 19,
	},
	.od = {
		.reg = ANACTRL_HIFI1PLL_CTRL0,
		.shift	 = 10,
		.width	 = 3,
	},
	.def_rate = t5m_hifi_pll_default_rate,
	.def_cnt = ARRAY_SIZE(t5m_hifi_pll_default_rate),
	.table = t5m_hifi_pll_table,
	.init_regs = t5m_hifi1_init_regs,
	.init_count = ARRAY_SIZE(t5m_hifi_init_regs),
	.l = {
		.reg = ANACTRL_HIFI1PLL_CTRL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = ANACTRL_HIFI1PLL_CTRL0,
		.shift   = 29,
		.width   = 1,
	},
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
	},
	.clkmsr_id = 18,
	.clkmsr_margin = 2
};

struct meson_clk_pll_data *t5m_pll_list[] = {
	&t5m_sys0_pll,
	&t5m_sys1_pll,
	&t5m_gp0_pll,
	&t5m_hifi_pll,
	&t5m_hifi1_pll
};

void t5m_meson_plls_all_test(void)
{
	for (int i = 0; i < ARRAY_SIZE(t5m_pll_list); i++)
		meson_pll_test(t5m_pll_list[i]);
}

void t5m_meson_hdmirx_plls_test(int argc, char * const argv[])
{
	/* todo: need add hdmirx plls test */
}

void t5m_meson_tcon_plls_test(int argc, char * const argv[])
{
	/* todo: need add tcon plls test */
}

void t5m_meson_plls_test(int argc, char * const argv[])
{
	struct meson_clk_pll_data *pll;

	pll = meson_pll_find_by_name(t5m_pll_list, ARRAY_SIZE(t5m_pll_list), argv[1]);
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
		t5m_meson_plls_all_test();
	else if (!strcmp("tcon", argv[1]))
		t5m_meson_tcon_plls_test(argc, argv);
	else if (!strcmp("hdmirx", argv[1]))
		t5m_meson_hdmirx_plls_test(argc, argv);
	else
		t5m_meson_plls_test(argc, argv);

	return 0;
}
