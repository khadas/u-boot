// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <amlogic/cpu_id.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/timer.h>
#include <asm/arch/pll.h>

#define STR_PLL_TEST_ALL	"all"
#define STR_PLL_TEST_SYS	"sys"
#define STR_PLL_TEST_FIX	"fix"
#define STR_PLL_TEST_DDR	"ddr"
#define STR_PLL_TEST_HIFI	"hifi"
#define STR_PLL_TEST_GP	    	"gp0"
#define STR_PLL_TEST_DDS	"dds"
#define STR_PLL_TEST_USBPHY	"usbphy"
#define STR_PLL_TEST_ETHPHY	"ethphy"

#define PLL_LOCK_CHECK_MAX		10

#define RET_PLL_LOCK_FAIL		0x1000
#define RET_CLK_NOT_MATCH		0x1
#define SYS_PLL_DIV16_CNTL		(1<<24)
#define SYS_CLK_DIV16_CNTL		(1)
#define SYS_PLL_TEST_DIV		4	/* div16 */
#define HDMI_PLL_DIV_CNTL		(1<<18)
#define HDMI_PLL_DIV_GATE		(1<<19)

#define PLL_DIV16_OFFSET		4	/* div2/2/4 */
#define gp_PLL_TEST_DIV		0	/* div1 */
#define PLL_DIV8_OFFSET		    3

#define Wr(addr, data) writel(data, addr)
#define Rd(addr) readl(addr)

static int sys_pll_init(sys_pll_set_t * sys_pll_set);
static int sys_pll_test(sys_pll_set_t * sys_pll_set);
static int sys_pll_test_all(sys_pll_cfg_t * sys_pll_cfg);
static int fix_pll_test(void);
static int ddr_pll_test(void);
static int gp_pll_test(gp_pll_set_t * gp_pll);
static int gp_pll_test_all(gp0_pll_cfg_t * gp0_pll_cfg);
extern unsigned long clk_util_clk_msr(unsigned long clk_mux);

static void update_bits(size_t reg, size_t mask, unsigned int val)
{
	unsigned int tmp, orig;
	orig = readl(reg);
	tmp = orig & ~mask;
	tmp |= val & mask;
	writel(tmp, reg);
}

uint32_t sys_pll_clk[PLL_TEST_SYS_TOTAL] = {3216, 1584};
sys_pll_cfg_t sys_pll_cfg = {
	.sys_pll[0] = {
		.cpu_clk   = 3216,
		.pll_cntl0 = 0x00300486,
		.pll_cntl1 = 0x00021092,
		.pll_cntl2 = 0x09023302,
		.pll_cntl3 = 0x02008000,
	},
	.sys_pll[1] = {
		.cpu_clk   = 1584,
		.pll_cntl0 = 0x00000442,
		.pll_cntl1 = 0x00021092,
		.pll_cntl2 = 0x09023302,
		.pll_cntl3 = 0x02008000,
	},
};

uint32_t hifi_pll_clk[] = {1560, 744};
hifi_pll_cfg_t hifi_pll_cfg = {
	.hifi_pll[0] = {
		.pll_clk   = 1560,
		.pll_cntl0 = 0x00030441,
		.pll_cntl1 = 0x802000b1,
		.pll_cntl2 = 0x15002320,
		.pll_cntl3 = 0xf0005555,
		.pll_cntl4 = 0x45004000,
	},

	.hifi_pll[1] = {
		.pll_clk   = 744,
		.pll_cntl0 = 0x0003041f,
		.pll_cntl1 = 0x802000b1,
		.pll_cntl2 = 0x12002320,
		.pll_cntl3 = 0xf0005555,
		.pll_cntl4 = 0x45004000,
	},
};

gp0_pll_cfg_t gp0_pll_cfg = {
	.gp0_pll[0] = {
		.pll_clk   = 1560, /* MHz */
		.pll_cntl0 = 0x00030441,
		.pll_cntl1 = 0x802000b1,
		.pll_cntl2 = 0x11002320,
		.pll_cntl3 = 0xd0010000,
		.pll_cntl4 = 0x45004000,
		.pll_cntl5 = 0x51a051a,
		.pll_cntl6 = 0x01888585
	},
	.gp0_pll[1] = {
		.pll_clk   = 744, /* MHz */
		.pll_cntl0 = 0x0003041f,
		.pll_cntl1 = 0x802000b1,
		.pll_cntl2 = 0x11002320,
		.pll_cntl3 = 0xd0010000,
		.pll_cntl4 = 0x45004000,
		.pll_cntl5 = 0x51a051a,
		.pll_cntl6 = 0x01888585
	},
};

#define DDSPLL_RATE(_RATE, _CTS, _N) 			\
{							\
	    .RATE	=	(_RATE),	    	\
		.CTS=	(_CTS),				\
		.N =	(_N),				\
}

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
	if (((cmp - 2) <= clk) && (clk <= (cmp + 2)))
		return 1;
	else
		return 0;
}

void clocks_set_sys_cpu_clk(uint32_t src)
{
	/*
	 * src
	 * 0: use 24M, 1: use SYS PLL
	 */
	uint32_t control;
	uint32_t dyn_pre_mux;
	uint32_t dyn_post_mux;
	uint32_t dyn_div;
	//runtime_info_t info;

	// Make sure not busy from last setting and we currently match the last setting
	do {
		control = readl(CPUCTRL_CLK_CTRL0);
	} while( (control & (1 << 28)) );

	control = control | (1 << 26);// Enable

	dyn_pre_mux = 0; //use oscin
	dyn_post_mux = 0;
	dyn_div = 0;/* divide by 1 */

	// Select Dynamic mux
	if (src == 1) {
		control = control | (1 << 11);
	}
	else {
		control = control & ~(1 << 11);
		if (control & (1 << 10)) {// if using Dyn mux1, set dyn mux 0
			// Toggle bit[10] indicating a dynamic mux change
			control = (control & ~((1 << 10) | (0x3f << 4) | (1 << 2) | (0x3 << 0)))
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
	}

	writel(control, CPUCTRL_CLK_CTRL0);

	return;
}

static int sys_pll_init(sys_pll_set_t * sys_pll_set)
{
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int lock_timeout = 0;

	do {
		_udelay(5);
		writel(sys_pll_set->pll_cntl0 | (1 << 29),
			ANACTRL_SYSPLL_CTRL0);
		writel(sys_pll_set->pll_cntl1, ANACTRL_SYSPLL_CTRL1);
		writel(sys_pll_set->pll_cntl2, ANACTRL_SYSPLL_CTRL2);
		writel(sys_pll_set->pll_cntl3, ANACTRL_SYSPLL_CTRL3);
		writel(sys_pll_set->pll_cntl0 | (1 << 28) | (1 << 29),
			ANACTRL_SYSPLL_CTRL0);
		_udelay(5);
		writel(sys_pll_set->pll_cntl0 | (1 << 28),
			ANACTRL_SYSPLL_CTRL0);
		_udelay(10);
		writel(sys_pll_set->pll_cntl0 | (1 << 28) | (1 << 27),
			ANACTRL_SYSPLL_CTRL0);
		lock_timeout = 80;
		do {
			_udelay(5);
		} while ((0 == ((Rd(ANACTRL_SYSPLL_STS) >> 31) & 0x1)) &&
				(--lock_timeout));
	} while((0 == ((Rd(ANACTRL_SYSPLL_STS) >> 31) & 0x1)) &&
			(--lock_check));
	if (lock_check != 0)
		return 0;
	else
		return RET_PLL_LOCK_FAIL;
}


static int sys_pll_test_all(sys_pll_cfg_t * sys_pll_cfg)
{
	unsigned int i = 0;
	int ret = 0;

	for (i = 0; i < (sizeof(sys_pll_clk) / sizeof(uint32_t)); i++) {
		sys_pll_cfg->sys_pll[0].cpu_clk = sys_pll_clk[i];
		ret += sys_pll_test(&(sys_pll_cfg->sys_pll[i]));
	}
	return ret;
}


static int sys_pll_test(sys_pll_set_t * sys_pll_set)
{
	unsigned int clk_msr_val = 0;
	unsigned int sys_clk = 0, od = 0;
	int ret0, ret = 0;

	/* switch sys clk to oscillator */
	clocks_set_sys_cpu_clk(0);

	/* store current sys pll cntl */
	sys_pll_set_t sys_pll;
	sys_pll.pll_cntl0 = readl(ANACTRL_SYSPLL_CTRL0) & (~(0x7 << 27));
	sys_pll.pll_cntl1 = readl(ANACTRL_SYSPLL_CTRL1);
	sys_pll.pll_cntl2 = readl(ANACTRL_SYSPLL_CTRL2);
	sys_pll.pll_cntl3 = readl(ANACTRL_SYSPLL_CTRL3);

	if (sys_pll_set->cpu_clk == 0) {
		sys_clk = (24 * (sys_pll_set->pll_cntl0 & 0xFF)) /
				((sys_pll_set->pll_cntl0 >> 10) & 0x1F);
	}

	if (0 == sys_pll_set->pll_cntl0) {
		printf("sys pll cntl equal NULL, skip\n");
		return -1;
	}

	od = (sys_pll_set->pll_cntl0 >> 20) & 0x3;
	/* test sys pll */
	if (sys_pll_set->cpu_clk)
		sys_clk = sys_pll_set->cpu_clk;

	ret0 = sys_pll_init(sys_pll_set);
	if (ret0) {
		printf("SYS pll lock Failed! - %4d MHz\n", sys_clk);
	} else {
		printf("SYS pll lock OK! - %4d MHz >> %d/Div16 - %4d MHz.",
			sys_clk, od, (sys_clk >> od) >> PLL_DIV16_OFFSET);
		clocks_set_sys_cpu_clk(1);
		/*open div16*/
		writel(readl(CPUCTRL_CLK_CTRL1) | (1 << 24) | (1 << 1),
			CPUCTRL_CLK_CTRL1);
		clk_msr_val = clk_util_clk_msr(45);
		printf("CLKMSR(45) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, (sys_clk >> od) >>
					SYS_PLL_TEST_DIV)) {
			/* sys clk/pll div16 */
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
		}
	}

	/* restore sys pll */
	clocks_set_sys_cpu_clk(0);
	//printf("cpu = 0x%x\n", readl(CPUCTRL_CLK_CTRL0));
	ret = sys_pll_init(&sys_pll);
	if (ret)
		printf("restore sys pll lock Failed!\n");
	clocks_set_sys_cpu_clk(1);

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

static int hifi_pll_test(hifi_pll_set_t * hifi_pll)
{
	int ret = 0;
	unsigned int pll_clk = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;

	if (hifi_pll->pll_clk == 0)
		pll_clk = (24 / ((hifi_pll->pll_cntl0 >> 10) & 0x1F) *
				(hifi_pll->pll_cntl0 & 0x1FF));
	else
		pll_clk = hifi_pll->pll_clk;

	do {
		writel(hifi_pll->pll_cntl0 | (1 << 29),
			ANACTRL_HIFIPLL_CTRL0);
		writel(hifi_pll->pll_cntl1, ANACTRL_HIFIPLL_CTRL1);
		writel(hifi_pll->pll_cntl2, ANACTRL_HIFIPLL_CTRL2);
		writel(hifi_pll->pll_cntl3, ANACTRL_HIFIPLL_CTRL3);
		writel(hifi_pll->pll_cntl4, ANACTRL_HIFIPLL_CTRL4);
		_udelay(5);
		writel(hifi_pll->pll_cntl0 | (1 << 28) | (1 << 29),
			ANACTRL_HIFIPLL_CTRL0);
		_udelay(5);
		writel(hifi_pll->pll_cntl0 | (1 << 28),
			ANACTRL_HIFIPLL_CTRL0);
		_udelay(10);
		writel(hifi_pll->pll_cntl4 | (1 << 0), ANACTRL_HIFIPLL_CTRL4);
		_udelay(100);
	} while((0 == ((Rd(ANACTRL_HIFIPLL_STS) >> 31) & 0x1)) &&
		(--lock_check));

	if (0 == lock_check) {
		printf("HIFI pll lock Failed! - %4d MHz\n", pll_clk);
		ret += RET_PLL_LOCK_FAIL;
	} else {
		printf("HIFI pll lock OK! - %4d MHz. Div8 - %4d MHz. ",
			pll_clk, pll_clk >> PLL_DIV8_OFFSET);

		Wr(CLKTREE_SPICC_CLK_CTRL, ((3 << 9) | (1 << 8) | (0 << 0)));
		clk_msr_val = clk_util_clk_msr(16);
		printf("CLKMSR(16) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk >> PLL_DIV8_OFFSET)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret += RET_CLK_NOT_MATCH;
		}
	}

	return ret;
}

static int hifi_pll_test_all(hifi_pll_cfg_t * hifi_pll_cfg)
{
	unsigned int i = 0;
	int ret = 0;

	for (i = 0; i < (sizeof(hifi_pll_clk) / sizeof(uint32_t)); i++)
		ret += hifi_pll_test(&(hifi_pll_cfg->hifi_pll[i]));
	return ret;
}

static int gp_pll_test(gp_pll_set_t * gp_pll)
{
	int ret = 0;
	unsigned int pll_clk = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;

	if (gp_pll->pll_clk == 0)
		pll_clk = (24 / ((gp_pll->pll_cntl0 >> 10) & 0x1F) *
				(gp_pll->pll_cntl0 & 0xFF));
	else
		pll_clk = gp_pll->pll_clk;

	lock_check = PLL_LOCK_CHECK_MAX;
	do {
		writel(gp_pll->pll_cntl0 | (1 << 29), ANACTRL_GPPLL_CTRL0);
		writel(gp_pll->pll_cntl1, ANACTRL_GPPLL_CTRL1);
		writel(gp_pll->pll_cntl2, ANACTRL_GPPLL_CTRL2);
		writel(gp_pll->pll_cntl3, ANACTRL_GPPLL_CTRL3);
		writel(gp_pll->pll_cntl4, ANACTRL_GPPLL_CTRL4);
		writel(gp_pll->pll_cntl5, ANACTRL_GPPLL_CTRL5);
		writel(gp_pll->pll_cntl6, ANACTRL_GPPLL_CTRL6);
		_udelay(5);
		writel(gp_pll->pll_cntl0 | (1 << 28) | (1 << 29),
			ANACTRL_GPPLL_CTRL0);
		_udelay(5);
		writel(gp_pll->pll_cntl0 | (1 << 28),
			ANACTRL_GPPLL_CTRL0);
		_udelay(10);
		writel(gp_pll->pll_cntl4 | (1 << 0), ANACTRL_GPPLL_CTRL4);
		_udelay(100);
	} while ((!((readl(ANACTRL_GPPLL_STS) >> 31) & 0x1)) && --lock_check);

	if (0 == lock_check) {
		printf("GP pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("GP pll lock OK! - %4d MHz. Div8 - %4d MHz. ",
				pll_clk, pll_clk >> PLL_DIV8_OFFSET);
		Wr(CLKTREE_SPICC_CLK_CTRL, ((4 << 9) | (1 << 8) | (0 << 0)));
		/* get gp_pll_clk */
		clk_msr_val = clk_util_clk_msr(16);
		printf("CLKMSR(16) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk >> PLL_DIV8_OFFSET)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret += RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int gp_pll_test_all(gp0_pll_cfg_t * gp0_pll_cfg)
{
	unsigned int i = 0;
	int ret = 0;
	for (i = 0; i < (sizeof(gp0_pll_cfg_t) / sizeof(gp_pll_set_t)); i++)
		ret += gp_pll_test(&(gp0_pll_cfg->gp0_pll[i]));
	return ret;
}

static int dds_pll_test(dds_pll_set_t * dds_pll)
{
	int ret=0;
	#if 0 //2020.02.20
	unsigned int i=0, pll_clk=0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;

	for (i=0; i<(sizeof(ddspll_tbl)/sizeof(ddspll_tbl[0])); i++) {
		if (((dds_pll->pll_cntl3)/(dds_pll->pll_cntl4)) == ((ddspll_tbl[i].CTS)/(ddspll_tbl[i].N))) {
			pll_clk = ddspll_tbl[i].RATE;
		}
	}
	do {
		writel(dds_pll->misctop_cntl0, ANACTRL_MISCTOP_CTRL0);
		writel(dds_pll->pll_cntl0, ANACTRL_AUDDDS_CTRL0);
		writel(dds_pll->pll_cntl1, ANACTRL_AUDDDS_CTRL1);
		writel(dds_pll->pll_cntl2, ANACTRL_AUDDDS_CTRL2);
		writel(dds_pll->pll_cntl3, ANACTRL_AUDDDS_CTRL3);
		writel(dds_pll->pll_cntl4, ANACTRL_AUDDDS_CTRL4);
		writel((dds_pll->pll_cntl0 | (1<<29) | (1<<10) | (1<<11)) & ~(1<<28), ANACTRL_AUDDDS_CTRL0);

		writel(dds_pll->pll_cntl1 | (1<<31), ANACTRL_AUDDDS_CTRL1);

		//printf("gp lock_check: %4d\n", lock_check);
	} while ((!((readl(ANACTRL_AUDDDS_STS)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("dds pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	}
	else {
		printf("dds pll lock OK! - %4d MHz.", pll_clk);
		clk_msr_val = clk_util_clk_msr(44);
		printf("CLKMSR(44) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk)) {
			printf(": Match\n");
		}
		else {
			printf(": MisMatch\n");
			ret += RET_CLK_NOT_MATCH;
		}
	}
	#endif
	return ret;
}

static int dds_pll_test_all(void)
{
	#if 0 //2020.02.20
	unsigned int i=0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	int ret=0;
	//unsigned int ctrl0 = 0;

	for (i=0; i<(sizeof(ddspll_tbl)/sizeof(ddspll_tbl[0])); i++) {
		//ctrl0 = 0;
		//ctrl0 = ctrl0 | ((gpll0_tbl[i].m)<<0) | ((gpll0_tbl[i].n)<<10);
		do {
			writel(0x4, ANACTRL_MISCTOP_CTRL0);
			writel(0x50041340, ANACTRL_AUDDDS_CTRL0);
			writel(0x0, ANACTRL_AUDDDS_CTRL1);
			writel(0x0, ANACTRL_AUDDDS_CTRL2);
			writel(ddspll_tbl[i].CTS, ANACTRL_AUDDDS_CTRL3);
			writel(ddspll_tbl[i].N, ANACTRL_AUDDDS_CTRL4);
			writel(0x60041f40, ANACTRL_AUDDDS_CTRL0);
			writel(0x80000000, ANACTRL_AUDDDS_CTRL1);
		} while((0 == ((Rd(ANACTRL_AUDDDS_STS) >> 31) & 0x1)) && (--lock_check));

		if (0 == lock_check) {
			printf("dds pll lock Failed! - %4d MHz\n", ddspll_tbl[i].RATE);
			ret += RET_PLL_LOCK_FAIL;
		}
		else {
			printf("dds pll lock OK! - %4d MHz.", ddspll_tbl[i].RATE);
			//Wr(CLKTREE_SPICC_CLK_CTRL, ((4<<9) | (1<<8) | (7<<0)));
			//printf("CLKTREE_SPICC_CLK_CTRL: 0x%8x\n", readl(CLKTREE_SPICC_CLK_CTRL));
			clk_msr_val = clk_util_clk_msr(44);
			printf("CLKMSR(44) - %4d MHz ", clk_msr_val);
			if (clk_around(clk_msr_val, ddspll_tbl[i].RATE)) {
				printf(": Match\n");
			}
			else {
				printf(": MisMatch\n");
				ret += RET_CLK_NOT_MATCH;
			}
		}
	}
	return ret;
	#endif

	return 0;
}

static int usbphy_pll_test(usbphy_pll_set_t * usbphy_pll)
{
	int ret = 0;
	unsigned int pll_clk = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned long usbphy_base_cfg = 0xFE004000;

	if ((usbphy_pll->pll_cntl0 & 0x1FF) == 0x14) {
		pll_clk = 480;
	} else {
		printf("input frequency point is not support\n");
		return -1;
	}
	writel(usbphy_pll->pll_cntl0, (usbphy_base_cfg + 0x40));
	writel(usbphy_pll->pll_cntl1, (usbphy_base_cfg + 0x44));
	writel(usbphy_pll->pll_cntl2, (usbphy_base_cfg + 0x48));

	lock_check = PLL_LOCK_CHECK_MAX;
	do {
		update_bits((usbphy_base_cfg + 0x40), 1<<29, 1 << 29);
		_udelay(10);
		update_bits((usbphy_base_cfg + 0x40), 1<<29, 0);
		_udelay(100);
		//printf("ethphy lock_check: %4d\n", lock_check);
	} while ((!((readl(usbphy_base_cfg + 0x40)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("usbphy pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("usbphy pll lock OK! - %4d MHz.\n", pll_clk);
	}

	return ret;
}

static int usbphy_pll_test_all(void)
{
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	int ret=0;
	unsigned long usbphy_base_cfg = 0xFE004000;

	writel(0x09400414, (usbphy_base_cfg + 0x40));
	writel(0x927e0000, (usbphy_base_cfg + 0x44));
	writel(0xac5f69e5, (usbphy_base_cfg + 0x48));
	writel(0x29400414, (usbphy_base_cfg + 0x40));
	writel(0x09400414, (usbphy_base_cfg + 0x40));

	do {
		update_bits((usbphy_base_cfg + 0x40), 1<<29, 1 << 29);
		_udelay(10);
		update_bits((usbphy_base_cfg + 0x40), 1<<29, 0);
		_udelay(100);
		//printf("usbphy lock_check: %4d\n", lock_check);
	} while ((!((readl(usbphy_base_cfg + 0x40)>>31)&0x1)) && --lock_check);

	if (0 == lock_check) {
		printf("usbphy pll lock Failed! - 480MHz\n");
		ret += RET_PLL_LOCK_FAIL;
	} else {
		printf("usbphy pll lock OK! - 480MHz.\n");
	}

	return ret;
}

static int ethphy_pll_test(ethphy_pll_set_t * ethphy_pll)
{
	int ret = 0;
	unsigned int pll_clk=0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;

	if ((ethphy_pll->pll_cntl0 & 0x1FF) == 0xA) {
		pll_clk = 500;
	} else {
		printf("input frequency point is not support\n");
		return -1;
	}

	do {
		writel(ethphy_pll->pll_cntl0 | 0x30000000, ETH_PLL_CTL0);
		writel(ethphy_pll->pll_cntl1, ETH_PLL_CTL1);
		writel(ethphy_pll->pll_cntl2, ETH_PLL_CTL2);
		_udelay(150);
		writel(ethphy_pll->pll_cntl0 | 0x10000000, ETH_PLL_CTL0);
		_udelay(150);
	} while ((!((readl(ETH_PLL_CTL0)>>30)&0x1))&& --lock_check);

	if (0 == lock_check) {
		printf("ETHPHY pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("ETHPHY pll lock OK! - %4d MHz. ", pll_clk);
		/* get ethphy_pll_clk */
		clk_msr_val = clk_util_clk_msr(64)<<2;
		printf("CLKMSR(64) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int ethphy_pll_test_all(void)
{
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	int ret = 0;

	do {
		writel(0x9c0040a | 0x30000000, ETH_PLL_CTL0);
		writel(0x927e0000, ETH_PLL_CTL1);
		writel(0xac5f49e5, ETH_PLL_CTL2);
		writel(0x00000000, ETH_PLL_CTL3);
		_udelay(150);
		writel(0x9c0040a | 0x10000000, ETH_PLL_CTL0);
	    _udelay(150);
	} while ((!((readl(ETH_PLL_CTL0)>>30)&0x1))&& --lock_check);

	if (0 == lock_check) {
		printf("ethphy pll lock Failed! - 500MHz\n");
		ret += RET_PLL_LOCK_FAIL;
	} else {
		printf("ethphy pll lock OK! - 500MHz. ");
		/* get ethphy_pll_clk */
		clk_msr_val = clk_util_clk_msr(64)<<2;
		printf("CLKMSR(64) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, 500)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret += RET_CLK_NOT_MATCH;
		}
	}

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
		case PLL_HIFI:
			ret = hifi_pll_test_all(&hifi_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HIFI);
			break;
		case PLL_GP:
			ret = gp_pll_test_all(&gp0_pll_cfg);
			pll_report(ret, STR_PLL_TEST_GP);
			break;
		case PLL_USBPHY:
			ret = usbphy_pll_test_all();
			pll_report(ret, STR_PLL_TEST_USBPHY);
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
	hifi_pll_set_t hifi_pll_set = {0};
	gp_pll_set_t gp_pll_set = {0};
	dds_pll_set_t dds_pll_set = {0};
	usbphy_pll_set_t usbphy_pll_set = {0};
	ethphy_pll_set_t ethphy_pll_set = {0};

	unsigned char plls[PLL_ENUM] = {
		PLL_SYS,
		0xff,//	PLL_FIX, //0xff will skip this pll
		0xff,//	PLL_DDR,
		PLL_HIFI,
		PLL_GP,
		0xff,//PLL_DDS
		0xff,//PLL_USBPHY
		0xff,//PLL_ETHPHY
	};

	if (0 == strcmp(STR_PLL_TEST_ALL, argv[1])) {
		printf("Test all plls\n");
		pll_test_all(plls);
	} else if(0 == strcmp(STR_PLL_TEST_SYS, argv[1])) {
		if (argc == 2) {
			ret = sys_pll_test_all(&sys_pll_cfg);
			pll_report(ret, STR_PLL_TEST_SYS);
		} else if (argc != 6) {
			printf("%s pll test: args error\n", STR_PLL_TEST_SYS);
			return -1;
		} else {
			sys_pll_set.pll_cntl0 = simple_strtoul(argv[2],
							NULL, 16);
			sys_pll_set.pll_cntl1 = simple_strtoul(argv[3],
							NULL, 16);
			sys_pll_set.pll_cntl2 = simple_strtoul(argv[4],
							NULL, 16);
			sys_pll_set.pll_cntl3 = simple_strtoul(argv[5],
							NULL, 16);
			ret = sys_pll_test(&sys_pll_set);
			pll_report(ret, STR_PLL_TEST_SYS);
		}
	} else if (0 == strcmp(STR_PLL_TEST_HIFI, argv[1])) {
		if (argc == 2) {
			ret = hifi_pll_test_all(&hifi_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HIFI);
		} else if (argc != 7){
			printf("%s pll test: args error\n", STR_PLL_TEST_HIFI);
			return -1;
		} else {
			hifi_pll_set.pll_cntl0 = simple_strtoul(argv[2],
							NULL, 16);
			hifi_pll_set.pll_cntl1 = simple_strtoul(argv[3],
							NULL, 16);
			hifi_pll_set.pll_cntl2 = simple_strtoul(argv[4],
							NULL, 16);
			hifi_pll_set.pll_cntl3 = simple_strtoul(argv[5],
							NULL, 16);
			hifi_pll_set.pll_cntl4 = simple_strtoul(argv[6],
							NULL, 16);
			ret = hifi_pll_test(&hifi_pll_set);
			pll_report(ret, STR_PLL_TEST_HIFI);
		}
	} else if (0 == strcmp(STR_PLL_TEST_GP, argv[1])) {
		if (argc == 2) {
			ret = gp_pll_test_all(&gp0_pll_cfg);
			pll_report(ret, STR_PLL_TEST_GP);
		} else if (argc != 9) {
			printf("%s pll test: args error\n", STR_PLL_TEST_GP);
			return -1;
		} else {
			gp_pll_set.pll_cntl0 = simple_strtoul(argv[2],
							NULL, 16);
			gp_pll_set.pll_cntl1 = simple_strtoul(argv[3],
							NULL, 16);
			gp_pll_set.pll_cntl2 = simple_strtoul(argv[4],
							NULL, 16);
			gp_pll_set.pll_cntl3 = simple_strtoul(argv[5],
							NULL, 16);
			gp_pll_set.pll_cntl4 = simple_strtoul(argv[6],
							NULL, 16);
			gp_pll_set.pll_cntl5 = simple_strtoul(argv[7],
							NULL, 16);
			gp_pll_set.pll_cntl6 = simple_strtoul(argv[8],
							NULL, 16);
			ret = gp_pll_test(&gp_pll_set);
			pll_report(ret, STR_PLL_TEST_GP);
		}
	} else if (0 == strcmp(STR_PLL_TEST_DDR, argv[1])) {
		printf("%s pll not support now\n", STR_PLL_TEST_DDR);
		return -1;
	} else if (0 == strcmp(STR_PLL_TEST_FIX, argv[1])) {
		printf("%s pll not support now\n", STR_PLL_TEST_FIX);
		return -1;
	} else if (0 == strcmp(STR_PLL_TEST_DDS, argv[1])) {
		if (argc == 2) {
			ret = dds_pll_test_all();
			pll_report(ret, STR_PLL_TEST_DDS);
		} else if (argc != 8) {
			printf("%s pll test: args error\n", STR_PLL_TEST_DDS);
			return -1;
		} else {
			dds_pll_set.misctop_cntl0 = simple_strtoul(argv[2],
							NULL, 16);
			dds_pll_set.pll_cntl0 = simple_strtoul(argv[3],
							NULL, 16);
			dds_pll_set.pll_cntl1 = simple_strtoul(argv[4],
							NULL, 16);
			dds_pll_set.pll_cntl2 = simple_strtoul(argv[5],
							NULL, 16);
			dds_pll_set.pll_cntl3 = simple_strtoul(argv[6],
							NULL, 16);
			dds_pll_set.pll_cntl4 = simple_strtoul(argv[7],
							NULL, 16);
			ret = dds_pll_test(&dds_pll_set);
			pll_report(ret, STR_PLL_TEST_DDS);
		}
	} else if (0 == strcmp(STR_PLL_TEST_USBPHY, argv[1])) {
		if (argc == 2) {
			ret = usbphy_pll_test_all();
			pll_report(ret, STR_PLL_TEST_USBPHY);
		} else if (argc != 5) {
			printf("%s pll test: args error\n",
					STR_PLL_TEST_USBPHY);
			return -1;
		} else {
			usbphy_pll_set.pll_cntl0 = simple_strtoul(argv[2],
							NULL, 16);
			usbphy_pll_set.pll_cntl1 = simple_strtoul(argv[3],
							NULL, 16);
			usbphy_pll_set.pll_cntl2 = simple_strtoul(argv[4],
							NULL, 16);
			ret = usbphy_pll_test(&usbphy_pll_set);
			pll_report(ret, STR_PLL_TEST_USBPHY);
		}
	} else if (0 == strcmp(STR_PLL_TEST_ETHPHY, argv[1])) {
		if (argc == 2) {
			ret = ethphy_pll_test_all();
			pll_report(ret, STR_PLL_TEST_ETHPHY);
		} else if (argc != 5) {
			printf("%s pll test: args error\n",
					STR_PLL_TEST_ETHPHY);
			return -1;
		} else {
			ethphy_pll_set.pll_cntl0 = simple_strtoul(argv[2],
							NULL, 16);
			ethphy_pll_set.pll_cntl1 = simple_strtoul(argv[3],
							NULL, 16);
			ethphy_pll_set.pll_cntl2 = simple_strtoul(argv[4],
							NULL, 16);
			ret = ethphy_pll_test(&ethphy_pll_set);
			pll_report(ret, STR_PLL_TEST_ETHPHY);
		}
	}

	return 0;
}
