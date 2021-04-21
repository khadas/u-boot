// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/timer.h>
#include <asm/arch/pll.h>
#include <linux/arm-smccc.h>

#define STR_PLL_TEST_ALL	"all"
#define STR_PLL_TEST_SYS	"sys"
#define STR_PLL_TEST_FIX	"fix"
#define STR_PLL_TEST_DDR	"ddr"
#define STR_PLL_TEST_HDMI	"hdmi"
#define STR_PLL_TEST_GP0	"gp0"
#define STR_PLL_TEST_GP1	"gp1"
#define STR_PLL_TEST_HIFI	"hifi"
#define STR_PLL_TEST_PCIE	"pcie"
#define STR_PLL_TEST_ETHPHY	"ethphy"
#define STR_PLL_TEST_USBPHY	"usbphy"
#define STR_PLL_TEST_MPLL	"mpll"


#define PLL_LOCK_CHECK_MAX		3

#define RET_PLL_LOCK_FAIL		0x1000
#define RET_CLK_NOT_MATCH		0x1
#define SYS_PLL_DIV16_CNTL		(1 << 24)
#define SYS_CLK_DIV16_CNTL		(1 << 1)
#define SYS_PLL_TEST_DIV		4	/* div16 */
#define HDMI_PLL_DIV_CNTL		(1 << 18)
#define HDMI_PLL_DIV_GATE		(1 << 19)

#define PLL_DIV16_OFFSET		4	/* div2/2/4 */

#define RW_CPUCTRL_SYS_CPU_CLK	0
#define SYS_PLL	1
#define RW_CPUCTRL_SYS_CPU_CLK5	2
#define GP1_PLL	3

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
};

extern unsigned long clk_util_clk_msr(unsigned long clk_mux);

gp0_pll_cfg_t gp0_pll_cfg = {
	.gp0_pll[0] = {
		.pll_clk   = 6000, /* MHz */
		.pll_cntl0 = 0x080304fa,
		.pll_cntl1 = 0x00000000,
		.pll_cntl2 = 0x00000000,
		.pll_cntl3 = 0x48681c00,
		.pll_cntl4 = 0x88770290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000
	},
	.gp0_pll[1] = {
		.pll_clk   = 3000, /* MHz */
		.pll_cntl0 = 0X0803047d,
		.pll_cntl1 = 0x00006aab,
		.pll_cntl2 = 0x00000000,
		.pll_cntl3 = 0x6a295c00,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x54540000
	},
};

gp1_pll_cfg_t gp1_pll_cfg = {
	.gp1_pll[0] = {
		.pll_clk   = 6000, /* MHz */
		.pll_cntl0 = 0x080304fa,
		.pll_cntl1 = 0x00000000,
		.pll_cntl2 = 0x00000000,
		.pll_cntl3 = 0x48681c00,
		.pll_cntl4 = 0x88770290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000
	},
	.gp1_pll[1] = {
		.pll_clk   = 3000, /* MHz */
		.pll_cntl0 = 0X0803047d,
		.pll_cntl1 = 0x00006aab,
		.pll_cntl2 = 0x00000000,
		.pll_cntl3 = 0x6a295c00,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x54540000
	},
};

hdmi_pll_cfg_t hdmi_pll_cfg = {
	.hdmi_pll[0] = {
		.pll_clk   = 5940, /* MHz */
		.pll_cntl0 = 0x0b3004f7,
		.pll_cntl1 = 0x00010000,
		.pll_cntl2 = 0x00000000,
		.pll_cntl3 = 0x6a28dc00,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000
	},
	.hdmi_pll[1] = {
		.pll_clk   = 2970,
		.pll_cntl0 = 0x0b00047b,
		.pll_cntl1 = 0x00018000,
		.pll_cntl2 = 0x00000000,
		.pll_cntl3 = 0x6a29dc00,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x54540000
	},
};

uint32_t sys_pll_clk[] = {6000, 3000};
sys_pll_cfg_t sys_pll_cfg = {
	.sys_pll[0] = {
		.cpu_clk   = 6000,
		.pll_cntl  = 0X080304fa,
		.pll_cntl1 = 0x0,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x48681c00,
		.pll_cntl4 = 0x88770290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000
	},

	.sys_pll[1] = {
		.cpu_clk   = 3000,
		.pll_cntl  = 0X0803047d,
		.pll_cntl1 = 0x0,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x48681c00,
		.pll_cntl4 = 0x88770290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000
	},
};

hifi_pll_cfg_t hifi_pll_cfg = {
	.hifi_pll[0] = {
		.pll_clk   = 6005,
		.pll_cntl0 = 0X080304fa,
		.pll_cntl1 = 0X00006aab,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x6a285c00,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000
	},

	.hifi_pll[1] = {
		.pll_clk   = 3000,
		.pll_cntl0 = 0X0803047d,
		.pll_cntl1 = 0X00006aab,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x68295c00,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x54540000
	},
};

mpll_pll_set_t mpll = {
	.pll_clk   = 100,
	.pll_cntl0 = 0x00000543,
	.pll_cntl1 = 0xc14003e7,
	.pll_cntl2 = 0x60000033,
	.pll_cntl3 = 0xc14003e7,
	.pll_cntl4 = 0x60000033,
	.pll_cntl5 = 0xc14003e7,
	.pll_cntl6 = 0x60000033,
	.pll_cntl7 = 0xc14003e7,
	.pll_cntl8 = 0x60000033

};

/*PCIE clk_out = 24M*m/2^(n+1)/OD*/
static const struct pciepll_rate_table pcie_pll_rate_table[] = {
	PLL_RATE(4800, 200, 1, 12),
};

static void pll_report(unsigned int flag, char * name)
{
	if (flag)
		printf("%s pll test failed!\n", name);
	else
		printf("%s pll test pass!\n", name);
	return;
}

static int clk_around(unsigned int clk, unsigned int cmp)
{
	if (cmp == 1)
		cmp += 1;
	if (((cmp-2) <= clk) && (clk <= (cmp+2)))
		return 1;
	else
		return 0;
}

static void clocks_set_sys_cpu_clk(uint32_t freq)
{
	struct arm_smccc_res res;

	arm_smccc_smc(0x82000099, SECID_CPU_CLK_SEL, (1 << 11), freq? 1 : 0,
		      0, 0, 0, 0, &res);
	arm_smccc_smc(0x82000099, SECID_CPU_CLK_DYN, freq, 0, 0, 0,
		      0, 0, &res);
}

static int sys_pll_init(sys_pll_set_t * sys_pll_set)
{
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int lock_timeout = 0;
	struct arm_smccc_res res;

	do {
		arm_smccc_smc(0x82000098, SECID_SYS0_DCO_PLL,
			      (sys_pll_set->pll_cntl & 0x1FF),
			      ((sys_pll_set->pll_cntl >> 10) & 0x1F),
			      ((sys_pll_set->pll_cntl >> 16) & 0x3),
			      0, 0, 0, &res);
		//Wr(ANACTRL_SYSPLL_CTRL0, sys_pll_set->pll_cntl);
		//Wr(ANACTRL_SYSPLL_CTRL0, sys_pll_set->pll_cntl | (3 << 28));
		//Wr(ANACTRL_SYSPLL_CTRL1, sys_pll_set->pll_cntl1);
		//Wr(ANACTRL_SYSPLL_CTRL2, sys_pll_set->pll_cntl2);
		//Wr(ANACTRL_SYSPLL_CTRL3, sys_pll_set->pll_cntl3);
		//Wr(ANACTRL_SYSPLL_CTRL4, sys_pll_set->pll_cntl4);
		//Wr(ANACTRL_SYSPLL_CTRL5, sys_pll_set->pll_cntl5);
		//Wr(ANACTRL_SYSPLL_CTRL6, sys_pll_set->pll_cntl6);
		//Wr(ANACTRL_SYSPLL_CTRL0, ((1<<29) | Rd(HHI_SYS_PLL_CNTL0)));
		//Wr(ANACTRL_SYSPLL_CTRL0, Rd(HHI_SYS_PLL_CNTL0)&(~(1<<29)));
		lock_timeout = 200;
		do {
			_udelay(2);
		} while ((!((readl(ANACTRL_SYSPLL_STS) >> 31) & 0x1)) &&
			 --lock_timeout);
	} while((!((readl(ANACTRL_SYSPLL_STS) >> 31) & 0x1)) && --lock_check);

	if (lock_check != 0)
		return 0;
	else
		return RET_PLL_LOCK_FAIL;
}

static int sys_pll_test(sys_pll_set_t * sys_pll_set)
{
	unsigned int clk_msr_val = 0;
	unsigned int sys_clk = 0;
	sys_pll_set_t sys_pll;
	int ret = 0, od = 0;

	od = ((sys_pll_set->pll_cntl >> 16) & 0x3);
	/* switch sys clk to oscillator */
	clocks_set_sys_cpu_clk(0);

	/* store current sys pll cntl */
	sys_pll.pll_cntl = readl(ANACTRL_SYSPLL_CTRL0);
	sys_pll.pll_cntl1 = readl(ANACTRL_SYSPLL_CTRL1);
	sys_pll.pll_cntl2 = readl(ANACTRL_SYSPLL_CTRL2);
	sys_pll.pll_cntl3 = readl(ANACTRL_SYSPLL_CTRL3);
	sys_pll.pll_cntl4 = readl(ANACTRL_SYSPLL_CTRL4);
	sys_pll.pll_cntl5 = readl(ANACTRL_SYSPLL_CTRL5);
	sys_pll.pll_cntl6 = readl(ANACTRL_SYSPLL_CTRL6);

	if (sys_pll_set->cpu_clk == 0) {
		sys_clk = (24 / ((sys_pll_set->pll_cntl >> 10) & 0x1F) *
			  (sys_pll_set->pll_cntl & 0x1FF)) >>
			  ((sys_pll_set->pll_cntl >> 16) & 0x3);
	} else {
		sys_clk = sys_pll_set->cpu_clk;
	}

	if (0 == sys_pll_set->pll_cntl) {
		printf("sys pll cntl equal NULL, skip\n");
		return -1;
	}

	/* test sys pll */
	ret = sys_pll_init(sys_pll_set);
	_udelay(5);
	if (ret) {
		printf("SYS pll lock Failed! - %4d MHz\n", sys_clk);
	} else {
		printf("SYS pll lock OK! - %4d MHz. Div16 >>%d - %4d MHz. ",
			sys_clk, od, (sys_clk >> SYS_PLL_TEST_DIV) >> od);
		clk_msr_val = clk_util_clk_msr(23);
		printf("CLKMSR(23) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, (sys_clk >> SYS_PLL_TEST_DIV) >> od)) {
			/* sys clk/pll div16 */
			printf(": Match\n");
		} else {
			ret = RET_CLK_NOT_MATCH;
			printf(": MisMatch\n");
		}
	}

	/* restore sys pll */
	sys_pll_init(&sys_pll);
	clocks_set_sys_cpu_clk(2);

	return ret;
}

static int sys_pll_test_all(sys_pll_cfg_t * sys_pll_cfg)
{
	unsigned int i = 0;
	int ret = 0;

	for (i = 0; i < (sizeof(sys_pll_cfg_t) / sizeof(sys_pll_set_t)); i++)
		ret += sys_pll_test(&(sys_pll_cfg->sys_pll[i]));
	return ret;
}

static void set_dsu_clk(uint32_t flag)
{
	struct arm_smccc_res res;

	arm_smccc_smc(0x8200009C, RW_CPUCTRL_SYS_CPU_CLK5, flag, 0, 0, 0,
		      0, 0, &res);
}

static int gp1_pll_init(gp1_pll_set_t * gp1_pll_set)
{
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int lock_timeout = 0;
	struct arm_smccc_res res;

	do {
		arm_smccc_smc(0x8200009C, GP1_PLL, gp1_pll_set->pll_cntl0,
			      gp1_pll_set->pll_cntl3, gp1_pll_set->pll_cntl4,
			      0, 0, 0, &res);
		lock_timeout = 200;
		do {
			_udelay(2);
		} while ((!((readl(ANACTRL_GP1PLL_STS) >> 31) & 0x1)) &&
			 --lock_timeout);
	} while((!((readl(ANACTRL_GP1PLL_STS) >> 31) & 0x1)) && --lock_check);

	if (lock_check != 0)
		return 0;
	else
		return RET_PLL_LOCK_FAIL;
}

static int gp1_pll_test(gp1_pll_set_t * gp1_pll_set)
{
	unsigned int clk_msr_val = 0, od = 0;
	unsigned int pll_clk = 0;
	gp1_pll_set_t gp1_pll;
	int ret = 0;

	od = (gp1_pll_set->pll_cntl0 >> 16) & 0x7;
	/* switch dsu clk to oscillator */
	set_dsu_clk(0);

	/* store current gp1 pll cntl */
	gp1_pll.pll_cntl0 = readl(ANACTRL_GP1PLL_CTRL0);
	gp1_pll.pll_cntl1 = readl(ANACTRL_GP1PLL_CTRL1);
	gp1_pll.pll_cntl2 = readl(ANACTRL_GP1PLL_CTRL2);
	gp1_pll.pll_cntl3 = readl(ANACTRL_GP1PLL_CTRL3);
	gp1_pll.pll_cntl4 = readl(ANACTRL_GP1PLL_CTRL4);
	gp1_pll.pll_cntl5 = readl(ANACTRL_GP1PLL_CTRL5);
	gp1_pll.pll_cntl6 = readl(ANACTRL_GP1PLL_CTRL6);

	if (gp1_pll_set->pll_clk == 0) {
		pll_clk = (24 / ((gp1_pll_set->pll_cntl0 >> 10) & 0x1F) *
			  (gp1_pll_set->pll_cntl0 & 0x1FF));
	} else {
		pll_clk = gp1_pll_set->pll_clk;
	}

	if (0 == gp1_pll_set->pll_cntl0) {
		printf("gp1 pll cntl equal NULL, skip\n");
		return -1;
	}

	/* test gp1 pll */
	ret = gp1_pll_init(gp1_pll_set);
	_udelay(5);
	if (ret) {
		printf("GP1 pll lock Failed! - %4d MHz\n", pll_clk);
	} else {
		printf("GP1 pll lock OK! - %4d MHz>>%d - %4d MHz. ",
			pll_clk, od,  pll_clk >> od);
		clk_msr_val = clk_util_clk_msr(21);
		printf("CLKMSR(21) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk >> od)) {
			printf(": Match\n");
		} else {
			ret = RET_CLK_NOT_MATCH;
			printf(": MisMatch\n");
		}
	}

	/* restore gp1 pll */
	gp1_pll_init(&gp1_pll);
	set_dsu_clk(1);

	return ret;
}

static int gp1_pll_test_all(gp1_pll_cfg_t * gp1_pll_cfg)
{
	unsigned int i = 0;
	int ret = 0;

	for (i = 0; i < (sizeof(gp1_pll_cfg_t) / sizeof(gp1_pll_set_t)); i++)
		ret += gp1_pll_test(&(gp1_pll_cfg->gp1_pll[i]));
	return ret;
}

static int fix_pll_test(void)
{
	return 0;
}

static int ddr_pll_test(void)
{
	return 0;
}

static int hdmi_pll_init(hdmi_pll_set_t * hdmi_pll)
{
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;

	do {
		writel(hdmi_pll->pll_cntl0, ANACTRL_HDMIPLL_CTRL0);
		writel(hdmi_pll->pll_cntl0 | (3 << 28), ANACTRL_HDMIPLL_CTRL0);
		writel(hdmi_pll->pll_cntl1, ANACTRL_HDMIPLL_CTRL1);
		writel(hdmi_pll->pll_cntl2, ANACTRL_HDMIPLL_CTRL2);
		writel(hdmi_pll->pll_cntl3, ANACTRL_HDMIPLL_CTRL3);
		writel(hdmi_pll->pll_cntl4, ANACTRL_HDMIPLL_CTRL4);
		writel(hdmi_pll->pll_cntl5, ANACTRL_HDMIPLL_CTRL5);
		writel(hdmi_pll->pll_cntl6, ANACTRL_HDMIPLL_CTRL6);

		writel(readl(ANACTRL_HDMIPLL_CTRL0) | (1 << 29),
				ANACTRL_HDMIPLL_CTRL0);
		_udelay(10);
		writel(readl(ANACTRL_HDMIPLL_CTRL0) & (~(1 << 29)),
				ANACTRL_HDMIPLL_CTRL0);
		_udelay(100);
	} while ((!(0x3==((readl(ANACTRL_HDMIPLL_STS) >> 30) & 0x3))) &&
			--lock_check);
	if (lock_check != 0)
		return 0;
	else
		return RET_PLL_LOCK_FAIL;
}

static int hdmi_pll_test(hdmi_pll_set_t * hdmi_pll_set)
{
	unsigned int pll_clk = 0 , od = 0;
	unsigned int pll_clk_div = 0;
	unsigned int clk_msr_val = 0;
	unsigned int clk_msr_reg = 0;
	int ret = 0;

	/* store pll div setting */
	clk_msr_reg = readl(CLKCTRL_VID_PLL_CLK_DIV);
	/* connect vid_pll_div to HDMIPLL directly */
	clrbits_le32(CLKCTRL_VID_PLL_CLK_DIV, 1 << 19);
	clrbits_le32(CLKCTRL_VID_PLL_CLK_DIV, 1 << 15);

	/* div14 */
	clrbits_le32(CLKCTRL_VID_PLL_CLK_DIV, 1 << 18);
	clrbits_le32(CLKCTRL_VID_PLL_CLK_DIV, 0x3 << 16);
	clrbits_le32(CLKCTRL_VID_PLL_CLK_DIV, 1 << 15);
	clrbits_le32(CLKCTRL_VID_PLL_CLK_DIV, 0x7fff);
	setbits_le32(CLKCTRL_VID_PLL_CLK_DIV, 1 << 16);
	setbits_le32(CLKCTRL_VID_PLL_CLK_DIV, 1 << 15);
	setbits_le32(CLKCTRL_VID_PLL_CLK_DIV, 0x3f80);
	clrbits_le32(CLKCTRL_VID_PLL_CLK_DIV, 1 << 15);

	setbits_le32(CLKCTRL_VID_PLL_CLK_DIV, 1 << 19);

	/* test pll */
	if (hdmi_pll_set->pll_clk == 0)
		pll_clk = ((24 * (hdmi_pll_set->pll_cntl0 & 0xff)) /
			  ((hdmi_pll_set->pll_cntl0 >> 10) & 0x1f));
	else
		pll_clk = hdmi_pll_set->pll_clk;

	_udelay(10);
	ret = hdmi_pll_init(hdmi_pll_set);
	if (ret) {
		printf("HDMI pll lock Failed! - %4d MHz\n", pll_clk);
	} else {
		od = (hdmi_pll_set->pll_cntl0 >> 20) & 0x3;
		pll_clk_div = (pll_clk >>od) / 14;
		printf("HDMI pll lock OK! - %4d MHz>>%d and Div14 - %4d MHz. ",
			pll_clk, od, pll_clk_div);
		/* get [  55][1485 MHz] vid_pll_div_clk_out */
		clk_msr_val = clk_util_clk_msr(50);
		printf("CLKMSR(50) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk_div)) {
			printf(": Match\n");
		} else {
			ret = RET_CLK_NOT_MATCH;
			printf(": MisMatch\n");
		}
	}

	/* restore pll */
	/* restore div cntl bit */
	writel(clk_msr_reg, CLKCTRL_VID_PLL_CLK_DIV);

	return ret;
}

static int hdmi_pll_test_all(hdmi_pll_cfg_t * hdmi_pll_cfg)
{
	unsigned int i = 0;
	int ret = 0;

	for (i = 0; i < (sizeof(hdmi_pll_cfg_t) / sizeof(hdmi_pll_set_t)); i++)
		ret += hdmi_pll_test(&(hdmi_pll_cfg->hdmi_pll[i]));
	return ret;
}

static int gp0_pll_test(gp0_pll_set_t * gp0_pll)
{
	unsigned int pll_clk = 0;
	int ret = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0, od = 0;

	od = (gp0_pll->pll_cntl0 >> 16) & 0x7;

	if (gp0_pll->pll_clk == 0)
		pll_clk = (24 / ((gp0_pll->pll_cntl0 >> 10) & 0x1F) *
				(gp0_pll->pll_cntl0 & 0x1FF));
	else
		pll_clk = gp0_pll->pll_clk;

	do {
		writel(gp0_pll->pll_cntl0, ANACTRL_GP0PLL_CTRL0);
		writel(gp0_pll->pll_cntl0 | (3 << 28), ANACTRL_GP0PLL_CTRL0);
		writel(gp0_pll->pll_cntl1, ANACTRL_GP0PLL_CTRL1);
		writel(gp0_pll->pll_cntl2, ANACTRL_GP0PLL_CTRL2);
		writel(gp0_pll->pll_cntl3, ANACTRL_GP0PLL_CTRL3);
		writel(gp0_pll->pll_cntl4, ANACTRL_GP0PLL_CTRL4);
		writel(gp0_pll->pll_cntl5, ANACTRL_GP0PLL_CTRL5);
		writel(gp0_pll->pll_cntl6, ANACTRL_GP0PLL_CTRL6);

		writel(readl(ANACTRL_GP0PLL_CTRL0) | (1 << 29),
				ANACTRL_GP0PLL_CTRL0);
		_udelay(10);
		writel(readl(ANACTRL_GP0PLL_CTRL0) & (~(1 << 29)),
				ANACTRL_GP0PLL_CTRL0);
		_udelay(100);
	} while ((!((readl(ANACTRL_GP0PLL_STS) >> 31) & 0x1)) && --lock_check);

	if (0 == lock_check) {
		printf("GP0 pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("GP0 pll lock OK! - %4d MHz >>%d - %4d MHz. ",
			pll_clk, od, pll_clk >> od);
		/* get gp0_pll_clk */
		clk_msr_val = clk_util_clk_msr(20);
		printf("CLKMSR(20) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk >> od)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int gp0_pll_test_all(gp0_pll_cfg_t * gp0_pll_cfg)
{
	unsigned int i = 0;
	int ret = 0;
	for (i = 0; i < (sizeof(gp0_pll_cfg_t) / sizeof(gp0_pll_set_t)); i++)
		ret += gp0_pll_test(&(gp0_pll_cfg->gp0_pll[i]));
	return ret;
}

static int hifi_pll_test(hifi_pll_set_t * hifi_pll)
{
	unsigned int pll_clk = 0;
	int ret = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0, od = 0;

	od = (hifi_pll->pll_cntl0 >> 16) & 0x3;

	if (hifi_pll->pll_clk == 0)
		pll_clk = (24 / ((hifi_pll->pll_cntl0 >> 10) & 0x1F) *
				(hifi_pll->pll_cntl0 & 0x1FF));
	else
		pll_clk = hifi_pll->pll_clk;

	do {
		writel(hifi_pll->pll_cntl0, ANACTRL_HIFIPLL_CTRL0);
		writel(hifi_pll->pll_cntl0 | (3 << 28), ANACTRL_HIFIPLL_CTRL0);
		writel(hifi_pll->pll_cntl1, ANACTRL_HIFIPLL_CTRL1);
		writel(hifi_pll->pll_cntl2, ANACTRL_HIFIPLL_CTRL2);
		writel(hifi_pll->pll_cntl3, ANACTRL_HIFIPLL_CTRL3);
		writel(hifi_pll->pll_cntl4, ANACTRL_HIFIPLL_CTRL4);
		writel(hifi_pll->pll_cntl5, ANACTRL_HIFIPLL_CTRL5);
		writel(hifi_pll->pll_cntl6, ANACTRL_HIFIPLL_CTRL6);
		writel(readl(ANACTRL_HIFIPLL_CTRL0) | (1 << 29),
			ANACTRL_HIFIPLL_CTRL0);
		_udelay(10);
		writel(readl(ANACTRL_HIFIPLL_CTRL0)&(~(1 << 29)),
			ANACTRL_HIFIPLL_CTRL0);
		_udelay(100);
	} while ((!((readl(ANACTRL_HIFIPLL_STS) >> 31) & 0x1)) &&
			--lock_check);
	if (0 == lock_check) {
		printf("HIFI pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("HIFI pll lock OK! - %4d MHz.Div8  - %4d MHz.",
			pll_clk, pll_clk >> od);
		/* get hifi_pll_clk */
		clk_msr_val = clk_util_clk_msr(19);
		printf("CLKMSR(19) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk >> od)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int hifi_pll_test_all(hifi_pll_cfg_t * hifi_pll_cfg)
{
	unsigned int i = 0;
	int ret = 0;

	for (i = 0; i < (sizeof(hifi_pll_cfg_t) / sizeof(hifi_pll_set_t)); i++)
		ret += hifi_pll_test(&(hifi_pll_cfg->hifi_pll[i]));
	return ret;
}

static int pcie_pll_test(pcie_pll_set_t * pcie_pll)
{
	int ret = 0;
	unsigned int i = 0, pll_clk = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;

	for (i = 0; i < (sizeof(pcie_pll_rate_table) /
			sizeof(pcie_pll_rate_table[0])); i++) {
		if ((pcie_pll->pll_cntl0 & 0xFF) ==
				pcie_pll_rate_table[i].m)
			pll_clk = pcie_pll_rate_table[i].rate;
	}

	do {
		writel(pcie_pll->pll_cntl0, ANACTRL_PCIEPLL_CTRL0);
		writel(pcie_pll->pll_cntl0 | 0x30000000,
			ANACTRL_PCIEPLL_CTRL0);
		writel(pcie_pll->pll_cntl1, ANACTRL_PCIEPLL_CTRL1);
		writel(pcie_pll->pll_cntl2, ANACTRL_PCIEPLL_CTRL2);
		writel(pcie_pll->pll_cntl3, ANACTRL_PCIEPLL_CTRL3);
		writel(pcie_pll->pll_cntl4, ANACTRL_PCIEPLL_CTRL4);
		writel(pcie_pll->pll_cntl5, ANACTRL_PCIEPLL_CTRL5);
		writel(pcie_pll->pll_cntl5 | 0x68, ANACTRL_PCIEPLL_CTRL5);
		_udelay(20);
		writel(pcie_pll->pll_cntl4 | 0x00800000,
			ANACTRL_PCIEPLL_CTRL4);
		_udelay(10);
		writel(pcie_pll->pll_cntl0 | 0x34000000,
			ANACTRL_PCIEPLL_CTRL0);
		writel(((pcie_pll->pll_cntl0 & (~(1 << 29))) | (1 << 26)) |
			(1 << 28), ANACTRL_PCIEPLL_CTRL0);
		_udelay(10);
		writel(pcie_pll->pll_cntl2 & (~(1 << 8)),
			ANACTRL_PCIEPLL_CTRL2);
		_udelay(10);
	} while ((!((readl(ANACTRL_PCIEPLL_STS) >> 31) & 0x1)) &&
			--lock_check);
	if (0 == lock_check) {
		printf("PCIE pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("PCIE pll lock OK! - %4d MHz.Div48  - %4d MHz. ",
			pll_clk, pll_clk / 48);
		/* get pcie_pll_clk */
		clk_msr_val = clk_util_clk_msr(16);
		printf("CLKMSR(16) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk / 48)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}

	return ret;
}

static int pcie_pll_test_all(void)
{
	unsigned int i = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	int ret = 0;

	do {
		writel(0x200c04c8, ANACTRL_PCIEPLL_CTRL0);
		writel(0x300c04c8, ANACTRL_PCIEPLL_CTRL0);
		writel(0x30000000, ANACTRL_PCIEPLL_CTRL1);
		writel(0x00001100, ANACTRL_PCIEPLL_CTRL2);
		writel(0x10058e00, ANACTRL_PCIEPLL_CTRL3);
		writel(0x000100c0, ANACTRL_PCIEPLL_CTRL4);
		writel(0x68000048, ANACTRL_PCIEPLL_CTRL5);
		writel(0x68000068, ANACTRL_PCIEPLL_CTRL5);
		_udelay(20);
		writel(0x008100c0, ANACTRL_PCIEPLL_CTRL4);
		_udelay(10);
		writel(0x340c04c8, ANACTRL_PCIEPLL_CTRL0);
		writel(0x140c04c8, ANACTRL_PCIEPLL_CTRL0);
		_udelay(10);
		writel(0x00001000, ANACTRL_PCIEPLL_CTRL2);
		_udelay(10);
	} while ((!((readl(ANACTRL_PCIEPLL_STS) >> 31) & 0x1)) &&
			--lock_check);

	if (0 == lock_check) {
		printf("pcie pll lock Failed! - %4d MHz\n",
			pcie_pll_rate_table[i].rate);
		ret += RET_PLL_LOCK_FAIL;
	} else {
		printf("pcie pll lock OK! - %4d MHz.Div48  - %4d MHz.",
			pcie_pll_rate_table[i].rate,
			pcie_pll_rate_table[i].rate / 48);
		/* get pcie_pll_clk */
		clk_msr_val = clk_util_clk_msr(16);
		printf("CLKMSR(16) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val,
			       pcie_pll_rate_table[i].rate / 48)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret += RET_CLK_NOT_MATCH;
		}
	}

	return ret;
}

static void set_mpll(mpll_pll_set_t *mpll)
{
	writel(mpll->pll_cntl0, ANACTRL_MPLL_CTRL0);
	writel(mpll->pll_cntl1, ANACTRL_MPLL_CTRL1);
	writel(mpll->pll_cntl2, ANACTRL_MPLL_CTRL2);
	writel(mpll->pll_cntl3, ANACTRL_MPLL_CTRL3);
	writel(mpll->pll_cntl4, ANACTRL_MPLL_CTRL4);
	writel(mpll->pll_cntl5, ANACTRL_MPLL_CTRL5);
	writel(mpll->pll_cntl6, ANACTRL_MPLL_CTRL6);
	writel(mpll->pll_cntl7, ANACTRL_MPLL_CTRL7);
	writel(mpll->pll_cntl8, ANACTRL_MPLL_CTRL8);
}

static int mpll_pll_test(mpll_pll_set_t *mpll)
{
	unsigned int i, ret = 0, clk_msr_val = 0;
	mpll_pll_set_t old_mpll;
	u64 clk_rate[4] = {0};

	old_mpll.pll_cntl0 = readl(ANACTRL_MPLL_CTRL0);
	old_mpll.pll_cntl1 = readl(ANACTRL_MPLL_CTRL1);
	old_mpll.pll_cntl2 = readl(ANACTRL_MPLL_CTRL2);
	old_mpll.pll_cntl3 = readl(ANACTRL_MPLL_CTRL3);
	old_mpll.pll_cntl4 = readl(ANACTRL_MPLL_CTRL4);
	old_mpll.pll_cntl5 = readl(ANACTRL_MPLL_CTRL5);
	old_mpll.pll_cntl6 = readl(ANACTRL_MPLL_CTRL6);
	old_mpll.pll_cntl7 = readl(ANACTRL_MPLL_CTRL7);
	old_mpll.pll_cntl8 = readl(ANACTRL_MPLL_CTRL8);

	if (!mpll->pll_clk) {
		clk_rate[0] = (2000 * 16384)/
			((16384 * ((mpll->pll_cntl1 >> 20) & 0x1ff)) +
			(mpll->pll_cntl1 & 0x7fff));
		clk_rate[1] = (2000 * 16384)/
			((16384 * ((mpll->pll_cntl3 >> 20) & 0x1ff)) +
			(mpll->pll_cntl3 & 0x7fff));
		clk_rate[2] = (2000 * 16384)/
			((16384 * ((mpll->pll_cntl5 >> 20) & 0x1ff)) +
			(mpll->pll_cntl5 & 0x7fff));
		clk_rate[3] = (2000 * 16384)/
			((16384 * ((mpll->pll_cntl7 >> 20) & 0x1ff)) +
			(mpll->pll_cntl7 & 0x7fff));
	}

	set_mpll(mpll);
	for (i = 0; i < 4; i++) {
		if (mpll->pll_clk) {
			clk_rate[i] = mpll->pll_clk;
		}
		printf("mpll%d - %4lld MHz-", i, clk_rate[i]);
		clk_msr_val = clk_util_clk_msr(11 + i);
		printf("CLKMSR(16) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, clk_rate[i])) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret += RET_CLK_NOT_MATCH;
		}
	}

	set_mpll(&old_mpll);
	return ret;
}

static int pll_test_all(unsigned char * pll_list)
{
	int ret = 0;
	unsigned char i = 0;

	for (i = 0; i < PLL_ENUM; i++) {
		switch (pll_list[i]) {
		case PLL_SYS:
			ret = sys_pll_test_all(&sys_pll_cfg);
			pll_report(ret, STR_PLL_TEST_SYS);
			break;
		case PLL_FIX:
			ret = fix_pll_test();
			pll_report(ret, STR_PLL_TEST_FIX);
			break;
		case PLL_DDR:
			ret = ddr_pll_test();
			pll_report(ret, STR_PLL_TEST_DDR);
			break;
		case PLL_HDMI:
			ret = hdmi_pll_test_all(&hdmi_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HDMI);
			break;
		case PLL_GP0:
			ret = gp0_pll_test_all(&gp0_pll_cfg);
			pll_report(ret, STR_PLL_TEST_GP0);
			break;
		case PLL_HIFI:
			ret = hifi_pll_test_all(&hifi_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HIFI);
			break;
		case PLL_PCIE:
			ret = pcie_pll_test_all();
			pll_report(ret, STR_PLL_TEST_PCIE);
			break;
		case PLL_GP1:
			ret = gp1_pll_test_all(&gp1_pll_cfg);
			pll_report(ret, STR_PLL_TEST_GP1);
			break;
		case PLL_MPLL:
			ret = mpll_pll_test(&mpll);
			pll_report(ret, STR_PLL_TEST_MPLL);
			break;
		default:
			break;
		}
	}
	return ret;
}

int pll_test(int argc, char * const argv[])
{
	int ret = 0;
	sys_pll_set_t sys_pll_set = {0};
	hdmi_pll_set_t hdmi_pll_set = {0};
	gp0_pll_set_t gp0_pll_set = {0};
	gp1_pll_set_t gp1_pll_set = {0};
	hifi_pll_set_t hifi_pll_set = {0};
	pcie_pll_set_t pcie_pll_set = {0};
	mpll_pll_set_t mpll_pll_set = {0};

	unsigned char plls[PLL_ENUM] = {
		PLL_SYS,
		0xff,//	PLL_FIX, //0xff will skip this pll
		0xff,//	PLL_DDR,
		0xff,// PLL_HDMI
		PLL_GP0,
		PLL_HIFI,
		0xff, //PLL_PCIE
		0xff, //PLL_GP1
		0xff,//PLL_USBPHY
		PLL_MPLL,
	};

	if (0 == strcmp(STR_PLL_TEST_ALL, argv[1])) {
		printf("Test all plls\n");
		pll_test_all(plls);
	} else if(0 == strcmp(STR_PLL_TEST_SYS, argv[1])) {
		if (argc == 2) {
			ret = sys_pll_test_all(&sys_pll_cfg);
			pll_report(ret, STR_PLL_TEST_SYS);
		} else if (argc != 9) {
			printf("%s pll test: args error\n", STR_PLL_TEST_SYS);
			return -1;
		} else {
			sys_pll_set.pll_cntl = simple_strtoul(argv[2], NULL,
							16);
			sys_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL,
							16);
			sys_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL,
							16);
			sys_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL,
							16);
			sys_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL,
							16);
			sys_pll_set.pll_cntl5 = simple_strtoul(argv[7], NULL,
							16);
			sys_pll_set.pll_cntl6 = simple_strtoul(argv[8], NULL,
							16);
			ret = sys_pll_test(&sys_pll_set);
			pll_report(ret, STR_PLL_TEST_SYS);
		}
	} else if (0 == strcmp(STR_PLL_TEST_HDMI, argv[1])) {
		if (argc == 2) {
			ret = hdmi_pll_test_all(&hdmi_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HDMI);
		} else if (argc != 9) {
			printf("%s pll test: args error\n", STR_PLL_TEST_HDMI);
			return -1;
		} else {
			hdmi_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL,
							16);
			hdmi_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL,
							16);
			hdmi_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL,
							16);
			hdmi_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL,
							16);
			hdmi_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL,
							16);
			hdmi_pll_set.pll_cntl5 = simple_strtoul(argv[7], NULL,
							16);
			hdmi_pll_set.pll_cntl6 = simple_strtoul(argv[8], NULL,
							16);
			ret = hdmi_pll_test(&hdmi_pll_set);
			pll_report(ret, STR_PLL_TEST_HDMI);
		}
	} else if (0 == strcmp(STR_PLL_TEST_GP0, argv[1])) {
		if (argc == 2) {
			ret = gp0_pll_test_all(&gp0_pll_cfg);
			pll_report(ret, STR_PLL_TEST_GP0);
		} else if (argc != 9) {
			printf("%s pll test: args error\n", STR_PLL_TEST_GP0);
			return -1;
		} else {
			gp0_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL,
							16);
			gp0_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL,
							16);
			gp0_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL,
							16);
			gp0_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL,
							16);
			gp0_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL,
							16);
			gp0_pll_set.pll_cntl5 = simple_strtoul(argv[7], NULL,
							16);
			gp0_pll_set.pll_cntl6 = simple_strtoul(argv[8], NULL,
							16);
			ret = gp0_pll_test(&gp0_pll_set);
			pll_report(ret, STR_PLL_TEST_GP0);
		}
	} else if (0 == strcmp(STR_PLL_TEST_HIFI, argv[1])) {
		if (argc == 2) {
			ret = hifi_pll_test_all(&hifi_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HIFI);
		} else if (argc != 9) {
			printf("%s pll test:args error\n", STR_PLL_TEST_HIFI);
			return -1;
		} else {
			hifi_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL,
							16);
			hifi_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL,
							16);
			hifi_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL,
							16);
			hifi_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL,
							16);
			hifi_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL,
							16);
			hifi_pll_set.pll_cntl5 = simple_strtoul(argv[7], NULL,
							16);
			hifi_pll_set.pll_cntl6 = simple_strtoul(argv[8], NULL,
							16);
			ret = hifi_pll_test(&hifi_pll_set);
			pll_report(ret, STR_PLL_TEST_HIFI);
		}
	} else if (0 == strcmp(STR_PLL_TEST_PCIE, argv[1])) {
		if (argc == 2) {
			ret = pcie_pll_test_all();
			pll_report(ret, STR_PLL_TEST_PCIE);
		} else if (argc != 8) {
			printf("%s pll test: args error\n", STR_PLL_TEST_PCIE);
			return -1;
		} else {
			pcie_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL,
							16);
			pcie_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL,
							16);
			pcie_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL,
							16);
			pcie_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL,
							16);
			pcie_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL,
							16);
			pcie_pll_set.pll_cntl5 = simple_strtoul(argv[7], NULL,
							16);
			ret = pcie_pll_test(&pcie_pll_set);
			pll_report(ret, STR_PLL_TEST_PCIE);
		}
	} else if (0 == strcmp(STR_PLL_TEST_MPLL, argv[1])) {
		if (argc == 2) {
			ret = mpll_pll_test(&mpll);
			pll_report(ret, STR_PLL_TEST_MPLL);
		} else if (argc != 11) {
			printf("%s pll test: args error\n", STR_PLL_TEST_MPLL);
			return -1;
		} else {
			mpll_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL,
							16);
			mpll_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL,
							16);
			mpll_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL,
							16);
			mpll_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL,
							16);
			mpll_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL,
							16);
			mpll_pll_set.pll_cntl5 = simple_strtoul(argv[7], NULL,
							16);
			mpll_pll_set.pll_cntl6 = simple_strtoul(argv[8], NULL,
							16);
			mpll_pll_set.pll_cntl7 = simple_strtoul(argv[9], NULL,
							16);
			mpll_pll_set.pll_cntl8 = simple_strtoul(argv[10], NULL,
							16);
			ret = mpll_pll_test(&mpll_pll_set);
			pll_report(ret, STR_PLL_TEST_MPLL);
		}
	} else if (0 == strcmp(STR_PLL_TEST_GP1, argv[1])) {
		if (argc == 2) {
			ret = gp1_pll_test_all(&gp1_pll_cfg);
			pll_report(ret, STR_PLL_TEST_GP1);
		} else if (argc != 9) {
			printf("%s pll test: args error\n", STR_PLL_TEST_GP1);
			return -1;
		} else {
			gp1_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL,
							16);
			gp1_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL,
							16);
			gp1_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL,
							16);
			gp1_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL,
							16);
			gp1_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL,
							16);
			gp1_pll_set.pll_cntl5 = simple_strtoul(argv[7], NULL,
							16);
			gp1_pll_set.pll_cntl6 = simple_strtoul(argv[8], NULL,
							16);
			ret = gp1_pll_test(&gp1_pll_set);
			pll_report(ret, STR_PLL_TEST_GP1);
		}
	} else if (0 == strcmp(STR_PLL_TEST_DDR, argv[1])) {
		printf("%s pll not support now\n", STR_PLL_TEST_DDR);
		return -1;
	} else if (0 == strcmp(STR_PLL_TEST_FIX, argv[1])) {
		printf("%s pll not support now\n", STR_PLL_TEST_FIX);
		return -1;
	}

	return 0;
}
