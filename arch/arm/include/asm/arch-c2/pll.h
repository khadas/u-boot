/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __PLL_H
#define __PLL_H

#define PLL_TEST_SYS_TOTAL		2
#define PLL_TEST_HDMI_TOTAL		3
#define PLL_TEST_HIFI_TOTAL		2
#define PLL_TEST_GP0_TOTAL		2

typedef struct sys_pll_set_s {
	unsigned int cpu_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
}sys_pll_set_t;

typedef struct sys_pll_cfg_s {
	sys_pll_set_t sys_pll[PLL_TEST_SYS_TOTAL];
}sys_pll_cfg_t;

typedef struct hifi_pll_set_s {
	unsigned int pll_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
	unsigned int pll_cntl4;
}hifi_pll_set_t;

typedef struct hifi_pll_cfg_s {
	hifi_pll_set_t hifi_pll[PLL_TEST_HIFI_TOTAL];
}hifi_pll_cfg_t;

typedef struct hifipll_rate_table_s {
	unsigned int rate;
	unsigned int m;
	unsigned int n;
	unsigned int od;
}hifipll_rate_table_t;

typedef struct gpll_rate_table_s {
	unsigned int rate;
	unsigned int m;
	unsigned int n;
	unsigned int od;
}gpll_rate_table_t;

typedef struct ddspll_rate_table_t {
	unsigned int RATE;
	unsigned int CTS;
	unsigned int N;
}ddspll_rate_table_t;

typedef struct gp_pll_set_s {
	unsigned int pll_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
	unsigned int pll_cntl4;
	unsigned int pll_cntl5;
	unsigned int pll_cntl6;
}gp_pll_set_t;

typedef struct gp0_pll_cfg_s {
	gp_pll_set_t gp0_pll[PLL_TEST_GP0_TOTAL];
}gp0_pll_cfg_t;

typedef struct dds_pll_set_s {
	unsigned int pll_clk;
	unsigned int misctop_cntl0;
	unsigned int pll_cntl0;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
	unsigned int pll_cntl4;
}dds_pll_set_t;

typedef struct usbphy_pll_set_s {
	unsigned int pll_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
}usbphy_pll_set_t;

typedef struct ethphy_pll_set_s {
	unsigned int pll_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
}ethphy_pll_set_t;

enum pll_enum {
	PLL_SYS = 0,
	PLL_FIX,
	PLL_DDR,
	PLL_HIFI,
	PLL_GP,
	PLL_DDS,
	PLL_USBPHY,
	PLL_ETHPHY,
	PLL_ENUM,
};
int pll_test(int argc, char * const argv[]);

#endif /* __PLL_H */
