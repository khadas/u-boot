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
#define STR_PLL_TEST_GP	    "gp"
#define STR_PLL_TEST_DDS	"dds"
#define STR_PLL_TEST_USBPHY	"usbphy"
#define STR_PLL_TEST_ETHPHY	"ethphy"


#define PLL_LOCK_CHECK_MAX		20

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
static int gp_pll_test_all(void);
extern unsigned long clk_util_clk_msr(unsigned long clk_mux);

#if 0
static unsigned int pll_range[PLL_ENUM][2] = {
	{101, 202}, //sys pll range
	{303, 404}, //fix pll range
	{505, 606}, //ddr pll range
	{707, 808}, //hdmi pll range
	{909, 999}, // pll range
};

static char pll_range_ind[PLL_ENUM][10] = {
	"sys",
	"fix",
	"ddr",
	"hdmi",
	"gp",
};
#endif

static void update_bits(size_t reg, size_t mask, unsigned int val)
{
	unsigned int tmp, orig;
	orig = readl(reg);
	tmp = orig & ~mask;
	tmp |= val & mask;
	writel(tmp, reg);
}

uint32_t sys_pll_clk[PLL_TEST_SYS_TOTAL] = {768, 888, 960, 1008, 1152, 1248, 1392, 1536};
sys_pll_cfg_t sys_pll_cfg = {
	.sys_pll[0] = {
		.cpu_clk   = 768,
		.pll_cntl0 = 0x01f18420,
		.pll_cntl1 = 0x01800000,
		.pll_cntl2 = 0x00001100,
		.pll_cntl3 = 0x10022300,
		.pll_cntl4 = 0x00300000
	},
	.sys_pll[1] = {
		.cpu_clk   = 888,
		.pll_cntl0 = 0x01f18425,
		.pll_cntl1 = 0x01800000,
		.pll_cntl2 = 0x00001100,
		.pll_cntl3 = 0x10022300,
		.pll_cntl4 = 0x00300000
	},
	.sys_pll[2] = {
		.cpu_clk   = 960,
		.pll_cntl0 = 0x01f18428,
		.pll_cntl1 = 0x01800000,
		.pll_cntl2 = 0x00001100,
		.pll_cntl3 = 0x10022300,
		.pll_cntl4 = 0x00300000
	},
	.sys_pll[3] = {
		.cpu_clk   = 1008,
		.pll_cntl0 = 0x01f1842A,
		.pll_cntl1 = 0x01800000,
		.pll_cntl2 = 0x00001100,
		.pll_cntl3 = 0x10022300,
		.pll_cntl4 = 0x00300000
	},
	.sys_pll[4] = {
		.cpu_clk   = 1152,
		.pll_cntl0 = 0x01f18430,
		.pll_cntl1 = 0x01800000,
		.pll_cntl2 = 0x00001100,
		.pll_cntl3 = 0x10022300,
		.pll_cntl4 = 0x00300000
	},
	.sys_pll[5] = {
		.cpu_clk   = 1248,
		.pll_cntl0 = 0x01f18434,
		.pll_cntl1 = 0x01800000,
		.pll_cntl2 = 0x00001100,
		.pll_cntl3 = 0x10022300,
		.pll_cntl4 = 0x00300000
	},
	.sys_pll[6] = {
		.cpu_clk   = 1392,
		.pll_cntl0 = 0x01f1843A,
		.pll_cntl1 = 0x01800000,
		.pll_cntl2 = 0x00001100,
		.pll_cntl3 = 0x10022300,
		.pll_cntl4 = 0x00300000
	},
	.sys_pll[7] = {
		.cpu_clk   = 1536,
		.pll_cntl0 = 0x01f18440,
		.pll_cntl1 = 0x01800000,
		.pll_cntl2 = 0x00001100,
		.pll_cntl3 = 0x10022300,
		.pll_cntl4 = 0x00300000
	},
};


#define PLL_RATE(_rate, _m, _n, _od) \
{							\
		.rate	=	(_rate),				\
		.m		=	(_m),					\
		.n		=	(_n),					\
		.od		=	(_od),					\
}

#define DDSPLL_RATE(_RATE, _CTS, _N) \
{							\
	    .RATE	=	(_RATE),	    \
		.CTS=	(_CTS),				\
		.N =	(_N),				\
}

static gpll_rate_table_t gpll_tbl[] = {
	PLL_RATE(864, 36, 1, 0),   /*DCO=864M*/
	PLL_RATE(960, 40, 1, 0),   /*DCO=960M*/
	PLL_RATE(1056, 44, 1, 0),  /*DCO=1056M*/
	PLL_RATE(1200, 50, 1, 0),  /*DCO=1200M*/
	PLL_RATE(1320, 55, 1, 0),  /*DCO=1320M*/
	PLL_RATE(1536, 64, 1, 0),  /*DCO=1536M*/

};

static hifipll_rate_table_t hifipll_tbl[] = {
	PLL_RATE(864, 36, 1, 0),   /*DCO=864M*/
	PLL_RATE(960, 40, 1, 0),   /*DCO=960M*/
	PLL_RATE(1056, 44, 1, 0),  /*DCO=1056M*/
	PLL_RATE(1200, 50, 1, 0),  /*DCO=1200M*/
	PLL_RATE(1320, 55, 1, 0),  /*DCO=1320M*/
	PLL_RATE(1536, 64, 1, 0),  /*DCO=1536M*/
};


static ddspll_rate_table_t ddspll_tbl[] = {
	DDSPLL_RATE(32, 1000, 64),
	DDSPLL_RATE(50, 1000, 100),
	DDSPLL_RATE(60, 82, 10),
	DDSPLL_RATE(80, 625, 100),
	DDSPLL_RATE(100, 5, 1),
};



static void pll_report(unsigned int flag, char * name) {
	if (flag)
		printf("%s pll test failed!\n", name);
	else
		printf("%s pll test pass!\n", name);
	return;
}

static int clk_around(unsigned int clk, unsigned int cmp) {
	if (((cmp-2) <= clk) && (clk <= (cmp+2)))
		return 1;
	else
		return 0;
}

void clocks_set_sys_cpu_clk(uint32_t src) {
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

static int sys_pll_init(sys_pll_set_t * sys_pll_set) {
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	do {
		writel(sys_pll_set->pll_cntl1, ANACTRL_SYSPLL_CTRL1);
		writel(sys_pll_set->pll_cntl2, ANACTRL_SYSPLL_CTRL2);
		writel(sys_pll_set->pll_cntl3, ANACTRL_SYSPLL_CTRL3);
		writel(sys_pll_set->pll_cntl4, ANACTRL_SYSPLL_CTRL4);
		writel(sys_pll_set->pll_cntl0, ANACTRL_SYSPLL_CTRL0);
		writel(sys_pll_set->pll_cntl0 | (1<<28) , ANACTRL_SYSPLL_CTRL0);
		_udelay(10);
		writel(sys_pll_set->pll_cntl0 | (1<<28) | (1<<26), ANACTRL_SYSPLL_CTRL0);
		_udelay(40);
		writel(sys_pll_set->pll_cntl2 | (1<<5), ANACTRL_SYSPLL_CTRL2);
		_udelay(20);
		} while((0 == ((Rd(ANACTRL_SYSPLL_STS) >> 31) & 0x1)) && (--lock_check));
	if (lock_check != 0)
		return 0;
	else
		return RET_PLL_LOCK_FAIL;
}


static int sys_pll_test_all(sys_pll_cfg_t * sys_pll_cfg) {
	unsigned int i=0;
	int ret=0;
/*
	for (i=0; i<(sizeof(sys_pll_cfg_t)/sizeof(sys_pll_set_t)); i++) {
		ret += sys_pll_test(&(sys_pll_cfg->sys_pll[i]));
	}
*/
	for (i=0; i<(sizeof(sys_pll_clk)/sizeof(uint32_t)); i++) {
		sys_pll_cfg->sys_pll[0].cpu_clk = sys_pll_clk[i];
		ret += sys_pll_test(&(sys_pll_cfg->sys_pll[i]));
	}
	return ret;
}


static int sys_pll_test(sys_pll_set_t * sys_pll_set) {
	//unsigned int clk_msr_reg = 0;
	unsigned int clk_msr_val = 0;
	unsigned int sys_clk = 0;
	//unsigned int sys_pll_cntl = 0;
	int ret=0;

	/* switch sys clk to oscillator */
	clocks_set_sys_cpu_clk(0);

	/* store current sys pll cntl */
	sys_pll_set_t sys_pll;
	sys_pll.pll_cntl0 = readl(ANACTRL_SYSPLL_CTRL0);
	sys_pll.pll_cntl1 = readl(ANACTRL_SYSPLL_CTRL1);
	sys_pll.pll_cntl2 = readl(ANACTRL_SYSPLL_CTRL2);
	sys_pll.pll_cntl3 = readl(ANACTRL_SYSPLL_CTRL3);
	sys_pll.pll_cntl4 = readl(ANACTRL_SYSPLL_CTRL4);

	if (sys_pll_set->cpu_clk == 0) {
		sys_clk = (24 * (sys_pll_set->pll_cntl0&0xFF)) / ((sys_pll_set->pll_cntl0>>10)&0x1F);
	}

	/* store CPU clk msr reg, restore it when test done */
	//clk_msr_reg = readl(HHI_SYS_CPU_CLK_CNTL1);

	/* enable CPU clk msr cntl bit */
	//writel(clk_msr_reg | SYS_PLL_DIV16_CNTL | SYS_CLK_DIV16_CNTL, HHI_SYS_CPU_CLK_CNTL1);

	//printf("HHI_SYS_CPU_CLK_CNTL: 0x%x\n", readl(HHI_SYS_CPU_CLK_CNTL));
	//printf("HHI_SYS_CPU_CLK_CNTL1: 0x%x\n", readl(HHI_SYS_CPU_CLK_CNTL1));

	if (0 == sys_pll_set->pll_cntl0) {
		printf("sys pll cntl equal NULL, skip\n");
		return -1;
	}

	/* test sys pll */
	if (sys_pll_set->cpu_clk)
		sys_clk = sys_pll_set->cpu_clk;

	ret = sys_pll_init(sys_pll_set);
	if (ret) {
		printf("SYS pll lock Failed! - %4d MHz\n", sys_clk);
	}
	else {
		printf("SYS pll lock OK! - %4d MHz. Div16 - %4d MHz. ", sys_clk, sys_clk>>PLL_DIV16_OFFSET);
		clocks_set_sys_cpu_clk(1);
		//open div16
		writel(0x005012A2, CPUCTRL_CLK_CTRL1);
		clk_msr_val = clk_util_clk_msr(45);
		printf("CLKMSR(45) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, sys_clk>>SYS_PLL_TEST_DIV)) {
			/* sys clk/pll div16 */
			printf(": Match\n");
		}
		else {
			ret = RET_CLK_NOT_MATCH;
			printf(": MisMatch\n");
		}
	}

	/* restore sys pll */
	sys_pll_init(&sys_pll);
	//clocks_set_sys_cpu_clk( 1, 0, 0, 0);
	clocks_set_sys_cpu_clk(1);

	/* restore clk msr reg */
	//writel(clk_msr_reg, HHI_SYS_CPU_CLK_CNTL1);
	return ret;
}


static int fix_pll_test(void) {
	return 0;
}

static int ddr_pll_test(void) {
	return 0;
}

static int hifi_pll_test(hifi_pll_set_t * hifi_pll) {
	int ret=0;
	unsigned int i=0, pll_clk=0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;

	for (i=0; i<(sizeof(hifipll_tbl)/sizeof(hifipll_tbl[0])); i++) {
		if ((hifi_pll->pll_cntl0 & 0xFF) == hifipll_tbl[i].m) {
			pll_clk = hifipll_tbl[i].rate;
		}
	}

	writel(hifi_pll->pll_cntl1, ANACTRL_HIFIPLL_CTRL1);
	writel(hifi_pll->pll_cntl2, ANACTRL_HIFIPLL_CTRL2);
	writel(hifi_pll->pll_cntl3, ANACTRL_HIFIPLL_CTRL3);
	writel(hifi_pll->pll_cntl4, ANACTRL_HIFIPLL_CTRL4);
	writel(hifi_pll->pll_cntl0, ANACTRL_HIFIPLL_CTRL0);
	writel(hifi_pll->pll_cntl0 | (1<<28), ANACTRL_HIFIPLL_CTRL0);
	_udelay(10);
	writel(hifi_pll->pll_cntl0 | (1<<28) | (1<<26), ANACTRL_HIFIPLL_CTRL0);
	_udelay(40);
	writel(hifi_pll->pll_cntl2 | (1<<5), ANACTRL_HIFIPLL_CTRL2);
	_udelay(20);

	lock_check = PLL_LOCK_CHECK_MAX;
	do {
		update_bits(ANACTRL_HIFIPLL_CTRL0, 1<<26, 1 << 26);
		_udelay(10);
		update_bits(ANACTRL_HIFIPLL_CTRL0, 1<<26, 0);
		_udelay(100);
		//printf("hifi lock_check: %4d\n", lock_check);
	} while((0 == ((Rd(ANACTRL_HIFIPLL_STS) >> 31) & 0x1)) && (--lock_check));
	if (0 == lock_check) {
		printf("HIFI pll lock Failed! - %4d MHz\n", pll_clk);
		ret += RET_PLL_LOCK_FAIL;
	}
	else {
		printf("HIFI pll lock OK! - %4d MHz. Div8 - %4d MHz. ", pll_clk, pll_clk>>PLL_DIV8_OFFSET);
		Wr(CLKTREE_SPICC_CLK_CTRL, ((3<<9) | (1<<8) | (7<<0)));
		//printf("CLKTREE_SPICC_CLK_CTRL: 0x%8x\n", readl(CLKTREE_SPICC_CLK_CTRL));
		clk_msr_val = clk_util_clk_msr(16);
		printf("CLKMSR(16) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk>>PLL_DIV8_OFFSET)) {
			printf(": Match\n");
		}
		else {
			printf(": MisMatch\n");
			ret += RET_CLK_NOT_MATCH;
		}
	}

	return ret;
}

static int hifi_pll_test_all(void){
	unsigned int i=0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	int ret=0;
	unsigned int ctrl0 = 0;
	for (i=0; i<(sizeof(hifipll_tbl)/sizeof(hifipll_tbl[0])); i++) {
		ctrl0 = 0;
		ctrl0 = ctrl0 | ((hifipll_tbl[i].m)<<0) | ((hifipll_tbl[i].n)<<10);
		writel(0x01800000, ANACTRL_HIFIPLL_CTRL1);
		writel(0x00001100, ANACTRL_HIFIPLL_CTRL2);
		writel(0x10022300, ANACTRL_HIFIPLL_CTRL3);
		writel(0x00300000, ANACTRL_HIFIPLL_CTRL4);
		writel(0x01f18400 | ctrl0, ANACTRL_HIFIPLL_CTRL0);
		writel(0x11f18400 | ctrl0, ANACTRL_HIFIPLL_CTRL0);
		_udelay(10);
		writel(0x15f18400 | ctrl0, ANACTRL_HIFIPLL_CTRL0);
		_udelay(40);
		//writel(0x00001140, ANACTRL_HIFIPLL_CTRL2);
		writel(0x00001120, ANACTRL_HIFIPLL_CTRL2);
		_udelay(20);
/* dump paras */
#if 0
		printf("hifi %d:\n", hifipll_tbl[i].rate);
		printf("HHI_HIFI_PLL_CNTL0: 0x%8x\n", readl(HHI_HIFI_PLL_CNTL0));
		//printf("HHI_HIFI_PLL_CNTL2: 0x%8x\n", readl(HHI_HIFI_PLL_CNTL2));
		//printf("HHI_HIFI_PLL_CNTL3: 0x%8x\n", readl(HHI_HIFI_PLL_CNTL3));
		//printf("HHI_HIFI_PLL_CNTL4: 0x%8x\n", readl(HHI_HIFI_PLL_CNTL4));
		//printf("HHI_HIFI_PLL_CNTL5: 0x%8x\n", readl(HHI_HIFI_PLL_CNTL5));
#endif

		lock_check = PLL_LOCK_CHECK_MAX;
		do {
			update_bits(ANACTRL_HIFIPLL_CTRL0, 1<<26, 1 << 26);
			_udelay(10);
			update_bits(ANACTRL_HIFIPLL_CTRL0, 1<<26, 0);
			_udelay(100);
			//printf("hifi lock_check: %4d\n", lock_check);
		} while((0 == ((Rd(ANACTRL_HIFIPLL_STS) >> 31) & 0x1)) && (--lock_check));

		if (0 == lock_check) {
			printf("HIFI pll lock Failed! - %4d MHz\n", hifipll_tbl[i].rate);
			ret += RET_PLL_LOCK_FAIL;
		}
		else {
			printf("HIFI pll lock OK! - %4d MHz. Div8 - %4d MHz. ", hifipll_tbl[i].rate, hifipll_tbl[i].rate>>PLL_DIV8_OFFSET);
			Wr(CLKTREE_SPICC_CLK_CTRL, ((3<<9) | (1<<8) | (7<<0)));
			//printf("CLKTREE_SPICC_CLK_CTRL: 0x%8x\n", readl(CLKTREE_SPICC_CLK_CTRL));
			clk_msr_val = clk_util_clk_msr(16);
			printf("CLKMSR(16) - %4d MHz ", clk_msr_val);
			if (clk_around(clk_msr_val, hifipll_tbl[i].rate>>PLL_DIV8_OFFSET)) {
				printf(": Match\n");
			}
			else {
				printf(": MisMatch\n");
				ret += RET_CLK_NOT_MATCH;
			}
		}
	}

	return ret;

}

static int gp_pll_test(gp_pll_set_t * gp_pll) {
	int ret = 0;
	unsigned int i = 0, pll_clk = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;

	for (i=0; i<(sizeof(gpll_tbl)/sizeof(gpll_tbl[0])); i++) {
		if ((gp_pll->pll_cntl0 & 0xFF) == gpll_tbl[i].m) {
			pll_clk = gpll_tbl[i].rate;
		}
	}

	writel(gp_pll->pll_cntl1, ANACTRL_GPPLL_CTRL1);
	writel(gp_pll->pll_cntl2, ANACTRL_GPPLL_CTRL2);
	writel(gp_pll->pll_cntl3, ANACTRL_GPPLL_CTRL3);
	writel(gp_pll->pll_cntl4, ANACTRL_GPPLL_CTRL4);
	writel(gp_pll->pll_cntl5, ANACTRL_GPPLL_CTRL5);
	writel(gp_pll->pll_cntl0, ANACTRL_GPPLL_CTRL0);
	writel(gp_pll->pll_cntl0 | (1<<28), ANACTRL_GPPLL_CTRL0);

	_udelay(10);
	writel(gp_pll->pll_cntl0 | (1<<28) | (1<<26), ANACTRL_GPPLL_CTRL0);
	_udelay(40);
	writel(gp_pll->pll_cntl2 | (1<<5), ANACTRL_GPPLL_CTRL2);
	_udelay(20);

	lock_check = PLL_LOCK_CHECK_MAX;
	do {
		update_bits(ANACTRL_GPPLL_CTRL0, 1<<26, 1 << 26);
		_udelay(10);
		update_bits(ANACTRL_GPPLL_CTRL0, 1<<26, 0);
		_udelay(100);
		//printf("gp lock_check: %4d\n", lock_check);
	} while ((!((readl(ANACTRL_GPPLL_STS)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("GP pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	}
	else {
		printf("GP pll lock OK! - %4d MHz. Div8 - %4d MHz. ", pll_clk, pll_clk>>PLL_DIV8_OFFSET);
		Wr(CLKTREE_SPICC_CLK_CTRL, ((4<<9) | (1<<8) | (7<<0)));
		/* get gp_pll_clk */
		clk_msr_val = clk_util_clk_msr(16);
		printf("CLKMSR(16) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk>>PLL_DIV8_OFFSET)) {
			printf(": Match\n");
		}
		else {
			printf(": MisMatch\n");
			ret += RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int gp_pll_test_all(void) {
	unsigned int i=0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	int ret=0;
	unsigned int ctrl0 = 0;

	for (i=0; i<(sizeof(gpll_tbl)/sizeof(gpll_tbl[0])); i++) {
		ctrl0 = 0;
		ctrl0 = ctrl0 | ((gpll_tbl[i].m)<<0) | ((gpll_tbl[i].n)<<10);
		writel(0x01800000, ANACTRL_GPPLL_CTRL1);
		writel(0x00001100, ANACTRL_GPPLL_CTRL2);
		writel(0x10022300, ANACTRL_GPPLL_CTRL3);
		writel(0x00300000, ANACTRL_GPPLL_CTRL4);
		writel(0x00088888, ANACTRL_GPPLL_CTRL5);
		writel(0x01f18400 | ctrl0, ANACTRL_GPPLL_CTRL0);
		writel(0x11f18400 | ctrl0, ANACTRL_GPPLL_CTRL0);
		_udelay(10);
		writel(0x15f18400 | ctrl0, ANACTRL_GPPLL_CTRL0);
		_udelay(40);
		writel(0x00001120, ANACTRL_GPPLL_CTRL2);
		_udelay(20);

/* dump paras */
#if 0
		printf("gp %d:\n", gpll0_tbl[i].rate);
		printf("HHI_gp_PLL_CNTL0: 0x%8x\n", readl(HHI_gp_PLL_CNTL0));
		//printf("HHI_gp_PLL_CNTL2: 0x%8x\n", readl(HHI_gp_PLL_CNTL2));
		//printf("HHI_gp_PLL_CNTL3: 0x%8x\n", readl(HHI_gp_PLL_CNTL3));
		//printf("HHI_gp_PLL_CNTL4: 0x%8x\n", readl(HHI_gp_PLL_CNTL4));
		//printf("HHI_gp_PLL_CNTL5: 0x%8x\n", readl(HHI_gp_PLL_CNTL5));
#endif

	lock_check = PLL_LOCK_CHECK_MAX;
		do {
			update_bits(ANACTRL_GPPLL_CTRL0, 1<<26, 1 << 26);
			_udelay(10);
			update_bits(ANACTRL_GPPLL_CTRL0, 1<<26, 0);
			_udelay(100);
			//printf("gp lock_check: %4d\n", lock_check);
		} while((0 == ((Rd(ANACTRL_GPPLL_STS) >> 31) & 0x1)) && (--lock_check));

		if (0 == lock_check) {
			printf("gp pll lock Failed! - %4d MHz\n", gpll_tbl[i].rate);
			ret += RET_PLL_LOCK_FAIL;
		}
		else {
			printf("gp pll lock OK! - %4d MHz. Div8 - %4d MHz. ", gpll_tbl[i].rate, gpll_tbl[i].rate>>PLL_DIV8_OFFSET);
			Wr(CLKTREE_SPICC_CLK_CTRL, ((4<<9) | (1<<8) | (7<<0)));
			//printf("CLKTREE_SPICC_CLK_CTRL: 0x%8x\n", readl(CLKTREE_SPICC_CLK_CTRL));
			clk_msr_val = clk_util_clk_msr(16);
			printf("CLKMSR(16) - %4d MHz ", clk_msr_val);
			if (clk_around(clk_msr_val, gpll_tbl[i].rate>>PLL_DIV8_OFFSET)) {
				printf(": Match\n");
			}
			else {
				printf(": MisMatch\n");
				ret += RET_CLK_NOT_MATCH;
			}
		}
	}

	return ret;
}

static int dds_pll_test(dds_pll_set_t * dds_pll) {
	int ret=0;
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
	return ret;
}

static int dds_pll_test_all(void) {
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

}

static int usbphy_pll_test(usbphy_pll_set_t * usbphy_pll) {
	int ret = 0;
	unsigned int pll_clk = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned long usbphy_base_cfg = 0xFE004000;

	if ((usbphy_pll->pll_cntl0 & 0x1FF) == 0x14) {
		pll_clk = 480;
	}
	else
	{
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
	}
	else{
		printf("usbphy pll lock OK! - %4d MHz.\n", pll_clk);
	}

	return ret;
}

static int usbphy_pll_test_all(void) {
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
	}
	else {
		printf("usbphy pll lock OK! - 480MHz.\n");
	}

	return ret;
}


static int ethphy_pll_test(ethphy_pll_set_t * ethphy_pll) {
	int ret=0;
	unsigned int pll_clk=0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;

    if ((ethphy_pll->pll_cntl0 & 0x1FF) == 0xA) {
		pll_clk = 500;
	}
	else
	{
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
	}
	else {
		printf("ETHPHY pll lock OK! - %4d MHz. ", pll_clk);
		/* get ethphy_pll_clk */
		clk_msr_val = clk_util_clk_msr(64)<<2;
		printf("CLKMSR(64) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk)) {
			printf(": Match\n");
		}
		else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int ethphy_pll_test_all(void) {
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	int ret=0;

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
	}
	else {
		printf("ethphy pll lock OK! - 500MHz. ");
		/* get ethphy_pll_clk */
		clk_msr_val = clk_util_clk_msr(64)<<2;
		printf("CLKMSR(64) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, 500)) {
			printf(": Match\n");
		}
		else {
			printf(": MisMatch\n");
			ret += RET_CLK_NOT_MATCH;
		}
	}

	return ret;
}


static int pll_test_all(unsigned char * pll_list) {
	int ret = 0;
	unsigned char i=0;
	for (i=0; i<PLL_ENUM; i++) {
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
				ret = hifi_pll_test_all();
				pll_report(ret, STR_PLL_TEST_HIFI);
				break;
			case PLL_GP:
				ret = gp_pll_test_all();
				pll_report(ret, STR_PLL_TEST_GP);
				break;
			case PLL_DDS:
				ret = dds_pll_test_all();
				pll_report(ret, STR_PLL_TEST_DDS);
				break;
			case PLL_USBPHY:
				ret = usbphy_pll_test_all();
				pll_report(ret, STR_PLL_TEST_USBPHY);
				break;
			case PLL_ETHPHY:
				ret = ethphy_pll_test_all();
				pll_report(ret, STR_PLL_TEST_ETHPHY);
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
		PLL_DDS,
		PLL_USBPHY,
		PLL_ETHPHY,
	};

	if (0 == strcmp(STR_PLL_TEST_ALL, argv[1])) {
		printf("Test all plls\n");
		pll_test_all(plls);
	}
	else if(0 == strcmp(STR_PLL_TEST_SYS, argv[1])) {
		if (argc == 2) {
			ret = sys_pll_test_all(&sys_pll_cfg);
			pll_report(ret, STR_PLL_TEST_SYS);
		}
		else if (argc != 7){
			printf("%s pll test: args error\n", STR_PLL_TEST_SYS);
			return -1;
		}
		else {
			sys_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			sys_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL, 16);
			sys_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL, 16);
			sys_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL, 16);
			sys_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL, 16);
			ret = sys_pll_test(&sys_pll_set);
			pll_report(ret, STR_PLL_TEST_SYS);
		}
	}
	else if (0 == strcmp(STR_PLL_TEST_HIFI, argv[1])) {
		if (argc == 2) {
			ret = hifi_pll_test_all();
			pll_report(ret, STR_PLL_TEST_HIFI);
		}
		else if (argc != 7){
			printf("%s pll test: args error\n", STR_PLL_TEST_HIFI);
			return -1;
		}
		else {
			hifi_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			hifi_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL, 16);
			hifi_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL, 16);
			hifi_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL, 16);
			hifi_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL, 16);
			ret = hifi_pll_test(&hifi_pll_set);
			pll_report(ret, STR_PLL_TEST_HIFI);
		}
	}
	else if (0 == strcmp(STR_PLL_TEST_GP, argv[1])) {
		if (argc == 2) {
			ret = gp_pll_test_all();
			pll_report(ret, STR_PLL_TEST_GP);
		}
		else if (argc != 8){
			printf("%s pll test: args error\n", STR_PLL_TEST_GP);
			return -1;
		}
		else {
			gp_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			gp_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL, 16);
			gp_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL, 16);
			gp_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL, 16);
			gp_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL, 16);
			gp_pll_set.pll_cntl5 = simple_strtoul(argv[7], NULL, 16);
			ret = gp_pll_test(&gp_pll_set);
			pll_report(ret, STR_PLL_TEST_GP);
		}
	}
	else if (0 == strcmp(STR_PLL_TEST_DDR, argv[1])) {
		printf("%s pll not support now\n", STR_PLL_TEST_DDR);
		return -1;
	}
	else if (0 == strcmp(STR_PLL_TEST_FIX, argv[1])) {
		printf("%s pll not support now\n", STR_PLL_TEST_FIX);
		return -1;
	}
	else if (0 == strcmp(STR_PLL_TEST_DDS, argv[1])) {
		if (argc == 2) {
			ret = gp_pll_test_all();
			pll_report(ret, STR_PLL_TEST_DDS);
		}
		else if (argc != 8){
			printf("%s pll test: args error\n", STR_PLL_TEST_DDS);
			return -1;
		}
		else {
			dds_pll_set.misctop_cntl0 = simple_strtoul(argv[2], NULL, 16);
			dds_pll_set.pll_cntl0 = simple_strtoul(argv[3], NULL, 16);
			dds_pll_set.pll_cntl1 = simple_strtoul(argv[4], NULL, 16);
			dds_pll_set.pll_cntl2 = simple_strtoul(argv[5], NULL, 16);
			dds_pll_set.pll_cntl3 = simple_strtoul(argv[6], NULL, 16);
			dds_pll_set.pll_cntl4 = simple_strtoul(argv[7], NULL, 16);
			ret = dds_pll_test(&dds_pll_set);
			pll_report(ret, STR_PLL_TEST_DDS);
		}
	}
	else if (0 == strcmp(STR_PLL_TEST_USBPHY, argv[1])) {
		if (argc == 2) {
			ret = usbphy_pll_test_all();
			pll_report(ret, STR_PLL_TEST_USBPHY);
		}
		else if (argc != 5){
			printf("%s pll test: args error\n", STR_PLL_TEST_USBPHY);
			return -1;
		}
		else {
			usbphy_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			usbphy_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL, 16);
			usbphy_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL, 16);
			ret = usbphy_pll_test(&usbphy_pll_set);
			pll_report(ret, STR_PLL_TEST_USBPHY);
		}
	}
	else if (0 == strcmp(STR_PLL_TEST_ETHPHY, argv[1])) {
		if (argc == 2) {
			ret = ethphy_pll_test_all();
			pll_report(ret, STR_PLL_TEST_ETHPHY);
		}
		else if (argc != 5){
			printf("%s pll test: args error\n", STR_PLL_TEST_ETHPHY);
			return -1;
		}
		else {
			ethphy_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			ethphy_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL, 16);
			ethphy_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL, 16);
			ret = ethphy_pll_test(&ethphy_pll_set);
			pll_report(ret, STR_PLL_TEST_ETHPHY);
		}
	}

#if 0
	unsigned char * pll_list = NULL;
	switch (get_cpu_id().family_id) {
		case MESON_CPU_MAJOR_ID_GXTVBB:
			pll_list = gxtvbb_plls;
			break;
		case MESON_CPU_MAJOR_ID_GXL:
			pll_list = gxl_plls;
			break;
		default:
			printf("un-support chip\n");
			break;
	}
	if (pll_list) {
		return plltest(pll_list);
	}
#endif

	return 0;
}
