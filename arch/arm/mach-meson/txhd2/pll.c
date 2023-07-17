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

void clocks_set_sys_cpu_clk(uint32_t freq, uint32_t pclk_ratio,
					uint32_t aclkm_ratio, uint32_t atclk_ratio)
{
	uint32_t    control;
	uint32_t    dyn_pre_mux;
	uint32_t    dyn_post_mux;
	uint32_t    dyn_div;

	// Make sure not busy from last setting and we currently match the last setting
	do {
		control = readl(HHI_SYS_CPU_CLK_CNTL);
	} while ((control & (1 << 28)));

	control = control | (1 << 26);              // Enable

	// Switching to System PLL...just change the final mux
	if (freq == 1) {
		// wire            cntl_final_mux_sel      = control[11];
		control = control | (1 << 11);
	} else {
		switch (freq) {
		case   0:       // If Crystal
			dyn_pre_mux     = 0;
			dyn_post_mux    = 0;
			dyn_div         = 0;    // divide by 1
			break;
		case 1000:      // fclk_div2
			dyn_pre_mux     = 1;
			dyn_post_mux    = 0;
			dyn_div         = 0;    // divide by 1
			break;
		case  667:      // fclk_div3
			dyn_pre_mux     = 2;
			dyn_post_mux    = 0;
			dyn_div         = 0;    // divide by 1
			break;
		case  500:      // fclk_div2/2
			dyn_pre_mux     = 1;
			dyn_post_mux    = 1;
			dyn_div         = 1;    // Divide by 2
			break;
		case  333:      // fclk_div3/2
			dyn_pre_mux     = 2;
			dyn_post_mux    = 1;
			dyn_div         = 1;    // divide by 2
			break;
		case  250:      // fclk_div2/4
			dyn_pre_mux     = 1;
			dyn_post_mux    = 1;
			dyn_div         = 3;    // divide by 4
			break;
		default:
			dyn_pre_mux     = 0;
			dyn_post_mux    = 0;
			dyn_div         = 0;    // divide by 1
			break;
		}
		if (control & (1 << 10)) {     // if using Dyn mux1, set dyn mux 0
			// Toggle bit[10] indicating a dynamic mux change
			control = (control & ~((1 << 10) | (0x3f << 4)  | (1 << 2)  | (0x3 << 0)))
			| ((0 << 10)
			| (dyn_div << 4)
			| (dyn_post_mux << 2)
			| (dyn_pre_mux << 0));
		} else {
			// Toggle bit[10] indicating a dynamic mux change
			control = (control & ~((1 << 10) | (0x3f << 20) | (1 << 18) | (0x3 << 16)))
			| ((1 << 10)
			| (dyn_div << 20)
			| (dyn_post_mux << 18)
			| (dyn_pre_mux << 16));
		}
		// Select Dynamic mux
		control = control & ~(1 << 11);
	}
	writel(control, HHI_SYS_CPU_CLK_CNTL);
}

int txhd2_sys0_pll_prepare(struct meson_clk_pll_data *pll)
{
	/* store rate */
	meson_pll_store_rate(pll);

	/* Set fixed clk to 1G, Switch to fixed clk first */
	clocks_set_sys_cpu_clk(1000, 0, 0, 0);

	return 0;
}

void txhd2_sys0_pll_unprepare(struct meson_clk_pll_data *pll)
{
	/* restore rate */
	meson_pll_restore_rate(pll);

	/* Switch back cpu to sys pll */
	clocks_set_sys_cpu_clk(1, 0, 0, 0);
}

static const struct pll_params_table txhd2_sys_pll_table[] = {
	PLL_PARAMS(42, 0, 0), /*DCO=1008M OD=1008M*/
	PLL_PARAMS(50, 0, 0), /*DCO=1200M OD=1200M*/
	PLL_PARAMS(54, 0, 0), /*DCO=1296M OD=1296M*/
	PLL_PARAMS(58, 0, 0), /*DCO=1392M OD=1392M*/
	PLL_PARAMS(59, 0, 0), /*DCO=1416M OD=1416M*/
	PLL_PARAMS(62, 0, 0), /*DCO=1488M OD=1488M*/
	PLL_PARAMS(63, 0, 0), /*DCO=1512M OD=1512M*/
	PLL_PARAMS(67, 0, 0), /*DCO=1608M OD=1608M*/
	{ /* sentinel */ },
};

static unsigned int txhd2_sys_pll_default_rate[] = {1008, 1608};

static const struct reg_sequence txhd2_sys0_pll_init_regs[] = {
	{ .reg = HHI_SYS_PLL_CNTL0,	.def = 0x61000043 },
	{ .reg = HHI_SYS_PLL_CNTL0,	.def = 0x71000043 },
	{ .reg = HHI_SYS_PLL_CNTL1,	.def = 0x39002000 },
	{ .reg = HHI_SYS_PLL_CNTL2,	.def = 0x00001140 },
	{ .reg = HHI_SYS_PLL_CNTL3,	.def = 0x00000000, .delay_us = 20 },
	{ .reg = HHI_SYS_PLL_CNTL0,	.def = 0x51000043, .delay_us = 20 },
	{ .reg = HHI_SYS_PLL_CNTL2,	.def = 0x00001100 },
};

struct meson_clk_pll_data txhd2_sys0_pll = {
	.name = "sys0",
	.en = {
		.reg = HHI_SYS_PLL_CNTL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = HHI_SYS_PLL_CNTL0,
		.shift   = 0,
		.width   = 9,
	},
	.n = {
		.reg = HHI_SYS_PLL_CNTL0,
		.shift   = 16,
		.width   = 2,
	},
	.od = {
		.reg = HHI_SYS_PLL_CNTL0,
		.shift	 = 12,
		.width	 = 2,
	},
	.def_rate = txhd2_sys_pll_default_rate,
	.def_cnt = ARRAY_SIZE(txhd2_sys_pll_default_rate),
	.table = txhd2_sys_pll_table,
	.init_regs = txhd2_sys0_pll_init_regs,
	.init_count = ARRAY_SIZE(txhd2_sys0_pll_init_regs),
	.l = {
		.reg = HHI_SYS_PLL_CNTL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = HHI_SYS_PLL_CNTL0,
		.shift   = 29,
		.width   = 1,
	},
	.th = {
		.reg = HHI_SYS_PLL_CNTL0,
		.shift   = 24,
		.width   = 1,
	},
	.flags = CLK_MESON_PLL_POWER_OF_TWO,
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = txhd2_sys0_pll_prepare,
		.pll_unprepare = txhd2_sys0_pll_unprepare,
		.pll_disable = meson_clk_pll_disable,
		.pll_set_rate = meson_pll_set_rate,
		.pll_set_parm_rate = meson_pll_set_parm_rate,
		.pll_rate_to_msr = meson_div16_to_msr,

	},
	.clkmsr_id = 17,
	.clkmsr_margin = 10,
};

static unsigned int txhd2_gp0_pll_default_rate[] = {288, 1488};

/* set the od = 2, clkmsr can not measure 6000M */
static const struct pll_params_table txhd2_gp0_pll_table[] = {
	PLL_PARAMS(48, 0, 2), /* DCO = 1152M OD = 2 PLL = 288M */
	PLL_PARAMS(62, 0, 1), /* DCO = 1488M OD = 1 PLL = 744M */
	PLL_PARAMS(48, 0, 0), /* DCO = 1152M OD = 0 PLL = 1152M */
	PLL_PARAMS(62, 0, 0), /* DCO = 1488M OD = 0 PLL = 1488M */
	{ /* sentinel */  }
};

static const struct reg_sequence txhd2_gp0_init_regs[] = {
	{ .reg = HHI_GP0_PLL_CNTL0,	.def = 0x60000030 },
	{ .reg = HHI_GP0_PLL_CNTL0,	.def = 0x70000030 },
	{ .reg = HHI_GP0_PLL_CNTL1,	.def = 0x39002000 },
	{ .reg = HHI_GP0_PLL_CNTL2,	.def = 0x00001140 },
	{ .reg = HHI_GP0_PLL_CNTL3,	.def = 0x00000000, .delay_us = 20 },
	{ .reg = HHI_GP0_PLL_CNTL0,	.def = 0x50000030, .delay_us = 20 },
	{ .reg = HHI_GP0_PLL_CNTL2,	.def = 0x00001100 }
};

struct meson_clk_pll_data txhd2_gp0_pll = {
	.name = "gp0",
	.en = {
		.reg = HHI_GP0_PLL_CNTL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = HHI_GP0_PLL_CNTL0,
		.shift   = 0,
		.width   = 9,
	},
	.n = {
		.reg = HHI_GP0_PLL_CNTL0,
		.shift   = 16,
		.width   = 2,
	},
	.od = {
		.reg = HHI_GP0_PLL_CNTL0,
		.shift	 = 12,
		.width	 = 2,
	},
	.def_rate = txhd2_gp0_pll_default_rate,
	.def_cnt = ARRAY_SIZE(txhd2_gp0_pll_default_rate),
	.table = txhd2_gp0_pll_table,
	.init_regs = txhd2_gp0_init_regs,
	.init_count = ARRAY_SIZE(txhd2_gp0_init_regs),
	.l = {
		.reg = HHI_GP0_PLL_CNTL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = HHI_GP0_PLL_CNTL0,
		.shift   = 29,
		.width   = 1,
	},
	.th = {
		.reg = HHI_GP0_PLL_CNTL0,
		.shift   = 24,
		.width   = 1,
	},
	.flags = CLK_MESON_PLL_POWER_OF_TWO,
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
	},
	.clkmsr_id = 4,
	.clkmsr_margin = 2
};

static const struct pll_params_table txhd2_hifi_pll_table[] = {
	PLL_PARAMS(30, 3, 0),  /* DCO = 1500M OD=0  PLL 1500M*/
	PLL_PARAMS(32, 3, 0),   /* DCO = 1600M  OD=0  PLL 1600M*/
	{ /* sentinel */  }
};

static unsigned int txhd2_hifi_pll_default_rate[] = {1500, 1600};

static const struct reg_sequence txhd2_hifi_init_regs[] = {
	{ .reg = HHI_HIFI_PLL_CNTL0,	.def = 0x61330024 },
	{ .reg = HHI_HIFI_PLL_CNTL0,	.def = 0x71330024 },
	{ .reg = HHI_HIFI_PLL_CNTL1,	.def = 0x25002000 },
	{ .reg = HHI_HIFI_PLL_CNTL2,	.def = 0x09001140 },
	{ .reg = HHI_HIFI_PLL_CNTL3,	.def = 0x00083180, .delay_us = 20 },
	{ .reg = HHI_HIFI_PLL_CNTL0,	.def = 0x51330024, .delay_us = 20 },
	{ .reg = HHI_HIFI_PLL_CNTL2,	.def = 0x09001100 },

};

struct meson_clk_pll_data txhd2_hifi_pll = {
	.name = "hifi",
	.en = {
		.reg = HHI_HIFI_PLL_CNTL0,
		.shift   = 28,
		.width   = 1,
	},
	.m = {
		.reg = HHI_HIFI_PLL_CNTL0,
		.shift   = 0,
		.width   = 9,
	},
	.n = {
		.reg = HHI_HIFI_PLL_CNTL0,
		.shift   = 16,
		.width   = 2,
	},
	.od = {
		.reg = HHI_HIFI_PLL_CNTL0,
		.shift	 = 12,
		.width	 = 2,
	},
	.frac = {
		.reg = HHI_HIFI_PLL_CNTL3,
		.shift   = 0,
		.width   = 19,
	},
	.def_rate = txhd2_hifi_pll_default_rate,
	.def_cnt = ARRAY_SIZE(txhd2_hifi_pll_default_rate),
	.table = txhd2_hifi_pll_table,
	.init_regs = txhd2_hifi_init_regs,
	.init_count = ARRAY_SIZE(txhd2_hifi_init_regs),
	.l = {
		.reg = HHI_HIFI_PLL_CNTL0,
		.shift   = 31,
		.width   = 1,
	},
	.rst = {
		.reg = HHI_HIFI_PLL_CNTL0,
		.shift   = 29,
		.width   = 1,
	},
	.th = {
		.reg = HHI_HIFI_PLL_CNTL0,
		.shift   = 24,
		.width   = 1,
	},
	.flags = CLK_MESON_PLL_POWER_OF_TWO,
	.parent_rate = 400000000,
	.ops = &(const struct meson_pll_test_ops) {
		.pll_prepare = meson_pll_store_rate,
		.pll_unprepare = meson_pll_restore_rate,
	},
	.clkmsr_id = 12,
	.clkmsr_margin = 2
};

struct meson_clk_pll_data *txhd2_pll_list[] = {
	&txhd2_sys0_pll,
	&txhd2_gp0_pll,
	&txhd2_hifi_pll,
};

void txhd2_meson_plls_all_test(void)
{
	for (int i = 0; i < ARRAY_SIZE(txhd2_pll_list); i++)
		meson_pll_test(txhd2_pll_list[i]);
}

void txhd2_meson_hdmirx_plls_test(int argc, char * const argv[])
{
	/* todo: need add hdmirx plls test */
}

void txhd2_meson_tcon_plls_test(int argc, char * const argv[])
{
	/* todo: need add tcon plls test */
}

void txhd2_meson_plls_test(int argc, char * const argv[])
{
	struct meson_clk_pll_data *pll;

	pll = meson_pll_find_by_name(txhd2_pll_list, ARRAY_SIZE(txhd2_pll_list), argv[1]);
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
}

int pll_test(int argc, char * const argv[])
{
	int val;

	val = readl(HHI_SYS_CPU_CLK_CNTL1);
	val |= BIT(1) + BIT(24);
	writel(val, HHI_SYS_CPU_CLK_CNTL1);
	if (!strcmp("all", argv[1]))
		txhd2_meson_plls_all_test();
	else if (!strcmp("tcon", argv[1]))
		txhd2_meson_tcon_plls_test(argc, argv);
	else if (!strcmp("hdmirx", argv[1]))
		txhd2_meson_hdmirx_plls_test(argc, argv);
	else
		txhd2_meson_plls_test(argc, argv);

	return 0;
}
