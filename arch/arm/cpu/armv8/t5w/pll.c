/*
* Copyright (C) 2017 Amlogic, Inc. All rights reserved.
* *
This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* *
This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
* *
You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
* *
Description:
*/
#include <common.h>
#include <command.h>
#include <asm/cpu_id.h>
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
#define STR_PLL_TEST_HDMIRX	"hdmirx"
#define STR_PLL_TEST_GP0	"gp0"
#define STR_PLL_TEST_GP1	"gp1"
#define STR_PLL_TEST_HIFI	"hifi"
#define STR_PLL_TEST_ADC	"adc"
#define STR_PLL_TEST_TCON	"tcon"
#define STR_PLL_TEST_MPLL	"mpll"
#define STR_PLL_TEST_PCIE	"pcie"
#define STR_PLL_TEST_ETHPHY	"ethphy"
#define STR_PLL_TEST_USBPHY	"usbphy"


#define PLL_LOCK_CHECK_MAX		3

#define RET_PLL_LOCK_FAIL		0x1000
#define RET_CLK_NOT_MATCH		0x1
#define SYS_PLL_DIV16_CNTL		(1 << 24)
#define SYS_CLK_DIV16_CNTL		(1 << 1)
#define SYS_PLL_TEST_DIV		4	/* div16 */

#define PLL_DIV16_OFFSET		4	/* div2/2/4 */

#define RW_CPUCTRL_SYS_CPU_CLK	0
#define SYS_PLL	1
#define RW_CPUCTRL_SYS_CPU_CLK5	2
#define GP1_PLL	3

#define Wr(addr, data) writel(data, addr)
#define Rd(addr) readl(addr)

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
		.pll_clk   = 1608, /* MHz */
		.pll_cntl0 = 0x20011086,
		.pll_cntl1 = 0x30011086,
		.pll_cntl2 = 0x1420500f,
		.pll_cntl3 = 0x00023041,
		.pll_cntl4 = 0x0,
		.pll_cntl5 = 0x10011086,
		.pll_cntl6 = 0x00023001
	},
	.gp1_pll[1] = {
		.pll_clk   = 1500, /* MHz */
		.pll_cntl0 = 0x2001107d,
		.pll_cntl1 = 0x3001107d,
		.pll_cntl2 = 0x1420500f,
		.pll_cntl3 = 0x00023041,
		.pll_cntl4 = 0x0,
		.pll_cntl5 = 0x1001107d,
		.pll_cntl6 = 0x00023001
	},
};

hdmirx_pll_cfg_t hdmirx_pll_cfg = {
	.hdmirx_pll[0] = {
		.pll_clk   = 5952, /* MHz */
		.pll_misc_cntl0 = 0x100347f9,
		.pll_cntl0 = 0x000904f8,
		.pll_cntl1 = 0x00000000,
		.pll_cntl2 = 0x00003118,
		.pll_cntl3 = 0x10058f31,
		.pll_cntl4 = 0x010100c0,
		.pll_cntl41 = 0x038300c0,
		.pll_cntl03 = 0x340904f8,
		.pll_cntl04 = 0x140904f8,
		.pll_cntl42 = 0x0b8300c0,
		.pll_cntl21 = 0x00003018,
	},
	.hdmirx_pll[1] = {
		.pll_clk   = 2952, /* MHz */
		.pll_misc_cntl0 = 0x100347f9,
		.pll_cntl0 = 0x0009047b,
		.pll_cntl1 = 0x00000000,
		.pll_cntl2 = 0x00003118,
		.pll_cntl3 = 0x10058f30,
		.pll_cntl4 = 0x010100c0,
		.pll_cntl41 = 0x038300c0,
		.pll_cntl03 = 0x3409047b,
		.pll_cntl04 = 0x1409047b,
		.pll_cntl42 = 0x0b8300c0,
		.pll_cntl21 = 0x00003018,
	},
	.hdmirx_pll[2] = {
		.pll_clk   = 4008, /* MHz */
		.pll_misc_cntl0 = 0x100347f9,
		.pll_cntl0 = 0x000904a7,
		.pll_cntl1 = 0x00000000,
		.pll_cntl2 = 0x00003118,
		.pll_cntl3 = 0x10058f31,
		.pll_cntl4 = 0x010100c0,
		.pll_cntl41 = 0x038300c0,
		.pll_cntl03 = 0x340904a7,
		.pll_cntl04 = 0x140904a7,
		.pll_cntl42 = 0x0b8300c0,
		.pll_cntl21 = 0x00003228,
	},
};

sys_pll_cfg_t sys_pll_cfg = {
	.sys_pll[0] = {
		.cpu_clk   = 750,
		.pll_cntl  = 0x2001207D,
		.pll_cntl1 = 0x3001207D,
		.pll_cntl2 = 0x1420500f,
		.pll_cntl3 = 0x00023041,
		.pll_cntl4 = 0x0,
		.pll_cntl5 = 0x1001207D,
		.pll_cntl6 = 0x00023001,
	},
	.sys_pll[1] = {
		.cpu_clk   = 375,
		.pll_cntl  = 0x2001307D,
		.pll_cntl1 = 0x3001307D,
		.pll_cntl2 = 0x1420500f,
		.pll_cntl3 = 0x00023041,
		.pll_cntl4 = 0x0,
		.pll_cntl5 = 0x1001307D,
		.pll_cntl6 = 0x00023001,
	},
};

hifi_pll_cfg_t hifi_pll_cfg = {
	.hifi_pll[0] = {
		.pll_clk   = 6005,
		.pll_cntl0 = 0x080304fa,
		.pll_cntl1 = 0X000061bd,
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

tcon_pll_cfg_t tcon_pll_cfg = {
	.tcon_pll[0] = {
		.cpu_clk   = 5940,
		.pll_cntl0 = 0x000e04f7,
		.pll_cntl1 = 0x10110000,
		.pll_cntl2 = 0x00003118,
		.pll_cntl3 = 0x10051400,
		.pll_cntl4 = 0x010100c0,
		.pll_cntl41 = 0x038300c0,
		.pll_cntl01 = 0x340e04f7,
		.pll_cntl02 = 0x140e04f7,
		.pll_cnt21 = 0x00003018,
		.pll_cntl42 = 0x0b8300c0,
	},
	.tcon_pll[1] = {
		.cpu_clk   = 2970,
		.pll_cntl0 = 0x000e047b,
		.pll_cntl1 = 0x10118000,
		.pll_cntl2 = 0x00003118,
		.pll_cntl3 = 0x10051400,
		.pll_cntl4 = 0x010100c0,
		.pll_cntl41 = 0x038300c0,
		.pll_cntl01 = 0x340e047b,
		.pll_cntl02 = 0x140e047b,
		.pll_cnt21 = 0x00003018,
		.pll_cntl42 = 0x0,
	},
};

adc_pll_cfg_t adc_pll_cfg = {
	.adc_pll[0] = {
		/* adc_dpll_clk_b2 */
		.cpu_clk   = 125,
		.pll_cntl0 = 0x010504fa,
		.pll_cntl1 = 0x01800000,
		.pll_cntl2 = 0x00000000,
		.pll_cntl3 = 0x0a691f00,
		.pll_cntl4 = 0x33771291,
		.pll_cntl5 = 0x39270000,
		.pll_cntl6 = 0x50540000,
	},
	.adc_pll[1] = {
		/* adc_dpll_clk_b2 */
		.cpu_clk   = 64,
		.pll_cntl0 = 0x000704d0,
		.pll_cntl1 = 0x03400000,
		.pll_cntl2 = 0x00000000,
		.pll_cntl3 = 0x0a691d00,
		.pll_cntl4 = 0x33771291,
		.pll_cntl5 = 0x39270000,
		.pll_cntl6 = 0x50540000,
	},
};

m_pll_cfg_t m_pll_cfg = {
	.m_pll[0] = {
		.pll_clk   = 100,
		.pll_cntl0 = 0x00000543,
		.pll_cntl1 = 0xc14003e7,
		.pll_cntl2 = 0x60000033,
		.pll_cntl3 = 0xc14003e7,
		.pll_cntl4 = 0x60000033,
		.pll_cntl5 = 0xc14003e7,
		.pll_cntl6 = 0x60000033,
		.pll_cntl7 = 0xc14003e7,
		.pll_cntl8 = 0x60000033,
	},
};

/*PCIE clk_out = 24M*m/2^(n+1)/OD*/
/*
 *static const struct pciepll_rate_table pcie_pll_rate_table[] = {
 *	PLL_RATE(4800, 200, 1, 12),
 *};
 */

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

static void clocks_set_sys_cpu_clk(uint32_t freq, uint32_t pclk_ratio, uint32_t aclkm_ratio, uint32_t atclk_ratio )
{
	uint32_t    control;
	uint32_t    dyn_pre_mux;
	uint32_t    dyn_post_mux;
	uint32_t    dyn_div;

	// Make sure not busy from last setting and we currently match the last setting
	do {
		control = Rd(HHI_SYS_CPU_CLK_CNTL);
	} while( (control & (1 << 28)) );

	control = control | (1 << 26);              // Enable

	// Switching to System PLL...just change the final mux
	if ( freq == 1 ) {
		// wire            cntl_final_mux_sel      = control[11];
		control = control | (1 << 11);
	} else {
		switch ( freq ) {
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
		if ( control & (1 << 10) ) {     // if using Dyn mux1, set dyn mux 0
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
	Wr(HHI_SYS_CPU_CLK_CNTL,control);
	//
	// Now set the divided clocks related to the System CPU
	//
	// This function changes the clock ratios for the
	// PCLK, ACLKM (AXI) and ATCLK
	//       .clk_clken0_i   ( {clk_div2_en,clk_div2}    ),
	//       .clk_clken1_i   ( {clk_div3_en,clk_div3}    ),
	//       .clk_clken2_i   ( {clk_div4_en,clk_div4}    ),
	//       .clk_clken3_i   ( {clk_div5_en,clk_div5}    ),
	//       .clk_clken4_i   ( {clk_div6_en,clk_div6}    ),
	//       .clk_clken5_i   ( {clk_div7_en,clk_div7}    ),
	//       .clk_clken6_i   ( {clk_div8_en,clk_div8}    ),

	uint32_t    control1 = Rd(HHI_SYS_CPU_CLK_CNTL1);

	//       .cntl_PCLK_mux              ( hi_sys_cpu_clk_cntl1[5:3]     ),
	if ( (pclk_ratio >= 2) && (pclk_ratio <= 8) ) {
		control1 = (control1 & ~(0x7 << 3)) | ((pclk_ratio-2) << 3);
	}
	//       .cntl_ACLKM_clk_mux         ( hi_sys_cpu_clk_cntl1[11:9]    ),  // AXI matrix
	if ( (aclkm_ratio >= 2) && (aclkm_ratio <= 8) ) {
		control1 = (control1 & ~(0x7 << 9)) | ((aclkm_ratio-2) << 9);
	}
	//       .cntl_ATCLK_clk_mux         ( hi_sys_cpu_clk_cntl1[8:6]     ),
	if ( (atclk_ratio >= 2) && (atclk_ratio <= 8) ) {
		control1 = (control1 & ~(0x7 << 6)) | ((atclk_ratio-2) << 6);
	}
	Wr( HHI_SYS_CPU_CLK_CNTL1, control1 );
}

static int sys_pll_init(sys_pll_set_t * sys_pll_set)
{
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;

	do {
		Wr(HHI_SYS_PLL_CNTL0, sys_pll_set->pll_cntl);
		Wr(HHI_SYS_PLL_CNTL0, sys_pll_set->pll_cntl1);
		Wr(HHI_SYS_PLL_CNTL1, sys_pll_set->pll_cntl2);
		Wr(HHI_SYS_PLL_CNTL2, sys_pll_set->pll_cntl3);
		Wr(HHI_SYS_PLL_CNTL3, sys_pll_set->pll_cntl4);
		_udelay(20);
		Wr(HHI_SYS_PLL_CNTL0, sys_pll_set->pll_cntl5);
		_udelay(20);
		Wr(HHI_SYS_PLL_CNTL2, sys_pll_set->pll_cntl6);
		_udelay(20);
	} while((!((readl(HHI_SYS_PLL_CNTL0)>>31)&0x1)) && --lock_check);
	if (lock_check != 0)
		return 0;
	else
		return RET_PLL_LOCK_FAIL;
}

static int sys_pll_test(sys_pll_set_t * sys_pll_set)
{
	unsigned int clk_msr_reg = 0;
	unsigned int clk_msr_val = 0;
	unsigned int sys_clk = 0;
	int ret=0;

	/* enable CPU clk msr cntl bit */
	writel(clk_msr_reg | SYS_PLL_DIV16_CNTL | SYS_CLK_DIV16_CNTL, HHI_SYS_CPU_CLK_CNTL1);
	/* switch sys clk to oscillator */
	clocks_set_sys_cpu_clk(1000, 0, 0, 0);

	if (sys_pll_set->cpu_clk == 0) {
		sys_clk = (24 / ((sys_pll_set->pll_cntl >> 16) & 0x1F) *
			(sys_pll_set->pll_cntl & 0x1FF))
			>> ((sys_pll_set->pll_cntl >> 12) & 0x7);
	}
	else {
		sys_clk = sys_pll_set->cpu_clk;
	}

	/* reset */
	sys_pll_set->pll_cntl |= 0x30000000;

	/* store CPU clk msr reg, restore it when test done */
	clk_msr_reg = readl(HHI_SYS_CPU_CLK_CNTL1);


	if (0 == sys_pll_set->pll_cntl) {
		printf("sys pll cntl equal NULL, skip\n");
		return -1;
	}

	/* test sys pll */
	if (sys_pll_set->cpu_clk)
		sys_clk = sys_pll_set->cpu_clk;

	ret = sys_pll_init(sys_pll_set);
	_udelay(100);
	if (ret) {
		printf("SYS pll lock Failed! - %4d MHz\n", sys_clk);
		return ret;
	} else {
		printf("SYS pll lock OK! - %4d MHz. Div16 - %4d MHz. ", sys_clk, sys_clk >> PLL_DIV16_OFFSET);
		clk_msr_val = clk_util_clk_msr(17);
		printf("CLKMSR(17) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, sys_clk >> SYS_PLL_TEST_DIV)) {
			/* sys clk/pll div16 */
			printf(": Match\n");
		} else {
			ret = RET_CLK_NOT_MATCH;
			printf(": MisMatch\n");
		}
	}

	clocks_set_sys_cpu_clk( 1, 0, 0, 0);

	/* restore clk msr reg */
	writel(clk_msr_reg, HHI_SYS_CPU_CLK_CNTL1);

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

static int fix_pll_test(void)
{
	return 0;
}

static int ddr_pll_test(void)
{
	return 0;
}

static int hdmirx_pll_init(hdmirx_pll_set_t * hdmirx_pll)
{
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;

	do {
		Wr(P_HDMIRX_PHY_MISC0, hdmirx_pll->pll_misc_cntl0);
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL0, hdmirx_pll->pll_cntl0);
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL0, hdmirx_pll->pll_cntl0 | (1 << 29));
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL0, hdmirx_pll->pll_cntl0 | (3 << 28));
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL1, hdmirx_pll->pll_cntl1);
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL2, hdmirx_pll->pll_cntl2);
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL3, hdmirx_pll->pll_cntl3);
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL4, hdmirx_pll->pll_cntl4);
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL4, hdmirx_pll->pll_cntl41);
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL0, hdmirx_pll->pll_cntl03);
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL0, hdmirx_pll->pll_cntl04);
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL4, hdmirx_pll->pll_cntl42);
		_udelay(20);
		Wr(P_HDMIRX_APLL_CNTL2, hdmirx_pll->pll_cntl21);
		_udelay(20);
	} while ((!(0x3==((readl(P_HDMIRX_APLL_CNTL0)>>30)&0x3))) && --lock_check);
	if (lock_check != 0)
		return 0;
	else
		return RET_PLL_LOCK_FAIL;
}

static int hdmirx_pll_test(hdmirx_pll_set_t * hdmirx_pll_set)
{
	unsigned int pll_clk = 0;
	unsigned int pll_clk_div = 0;
	unsigned int clk_msr_val = 0;
	unsigned int clk_msr_reg = 0;
	int ret = 0;

	/* store pll div setting */
	clk_msr_reg = readl(HHI_VID_PLL_CLK_DIV);
	/* connect vid_pll_div to HDMIRXPLL directly */
	clrbits_le32(HHI_VID_PLL_CLK_DIV, 1 << 19);
	clrbits_le32(HHI_VID_PLL_CLK_DIV, 1 << 15);

	/* div14 */
	clrbits_le32(HHI_VID_PLL_CLK_DIV, 1 << 18);
	clrbits_le32(HHI_VID_PLL_CLK_DIV, 0x3 << 16);
	clrbits_le32(HHI_VID_PLL_CLK_DIV, 1 << 15);
	clrbits_le32(HHI_VID_PLL_CLK_DIV, 0x7fff);
	setbits_le32(HHI_VID_PLL_CLK_DIV, 1 << 16);
	setbits_le32(HHI_VID_PLL_CLK_DIV, 1 << 15);
	setbits_le32(HHI_VID_PLL_CLK_DIV, 0x3f80);
	clrbits_le32(HHI_VID_PLL_CLK_DIV, 1 << 15);

	setbits_le32(HHI_VID_PLL_CLK_DIV, 1 << 19);

	/* test pll */
	if (hdmirx_pll_set->pll_clk == 0)
		pll_clk = ((24 * (hdmirx_pll_set->pll_cntl0 & 0xff)) /
			  ((hdmirx_pll_set->pll_cntl0 >> 10) & 0x1f));
	else
		pll_clk = hdmirx_pll_set->pll_clk;

	_udelay(100);
	ret = hdmirx_pll_init(hdmirx_pll_set);
	if (ret) {
		printf("HDMIRX pll lock Failed! - %4d MHz\n", pll_clk);
	} else {
		//od = (hdmirx_pll_set->pll_cntl0 >> 20) & 0x3;
		//pll_clk_div = (pll_clk >>od) / 14;
		/* hdmirx_apll_clk_out_div div = 5*8 */
		pll_clk_div = pll_clk / 40;
		printf("HDMIRX pll lock OK! - %4d MHz and Div40 - %4d MHz. ",
			pll_clk, pll_clk_div);
		/* get [  55][1485 MHz] vid_pll_div_clk_out */
		clk_msr_val = clk_util_clk_msr(29);
		printf("CLKMSR(29) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk_div)) {
			printf(": Match\n");
		} else {
			ret = RET_CLK_NOT_MATCH;
			printf(": MisMatch\n");
		}
	}

	/* restore pll */
	/* restore div cntl bit */
	writel(clk_msr_reg, HHI_VID_PLL_CLK_DIV);

	return ret;
}

static int hdmirx_pll_test_all(hdmirx_pll_cfg_t * hdmirx_pll_cfg)
{
	unsigned int i = 0;
	int ret = 0;

	for (i = 0; i < (sizeof(hdmirx_pll_cfg_t) / sizeof(hdmirx_pll_set_t)); i++)
		ret += hdmirx_pll_test(&(hdmirx_pll_cfg->hdmirx_pll[i]));
	return ret;
}

static int gp0_pll_test(gp0_pll_set_t * gp0_pll)
{
	unsigned int pll_clk = 0;
	int ret = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0, od = 0, m = 0, n = 0, frac = 0;

	od = (gp0_pll->pll_cntl0 >> 16) & 0x3;

	if (gp0_pll->pll_clk == 0) {
		m = gp0_pll->pll_cntl0 & 0x1FF;
		n = (gp0_pll->pll_cntl0 >> 10) & 0x1F;
		frac = gp0_pll->pll_cntl1 & 0x3FFFF;
		if ((gp0_pll->pll_cntl1 >> 18) & 0x1)
			pll_clk = 24 * (131072 * m - frac) / 131072 / n;
		else
			pll_clk = 24 * (131072 * m + frac) / 131072 / n;
	} else {
		pll_clk = gp0_pll->pll_clk;
	}

	writel(gp0_pll->pll_cntl0, HHI_GP0_PLL_CNTL0);
	writel(gp0_pll->pll_cntl0 | (3 << 28), HHI_GP0_PLL_CNTL0);
	writel(gp0_pll->pll_cntl1, HHI_GP0_PLL_CNTL1);
	writel(gp0_pll->pll_cntl2, HHI_GP0_PLL_CNTL2);
	writel(gp0_pll->pll_cntl3, HHI_GP0_PLL_CNTL3);
	writel(gp0_pll->pll_cntl4, HHI_GP0_PLL_CNTL4);
	writel(gp0_pll->pll_cntl5, HHI_GP0_PLL_CNTL5);
	writel(gp0_pll->pll_cntl6, HHI_GP0_PLL_CNTL6);
	do {
		writel(readl(HHI_GP0_PLL_CNTL0) | (1 << 29),
				HHI_GP0_PLL_CNTL0);
		_udelay(10);
		writel(readl(HHI_GP0_PLL_CNTL0) & (~(1 << 29)),
				HHI_GP0_PLL_CNTL0);
		_udelay(100);
	} while ((!((readl(HHI_GP0_PLL_CNTL0) >> 31) & 0x1)) && --lock_check);

	if (0 == lock_check) {
		printf("GP0 pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("GP0 pll lock OK! - %4d MHz >>%d - %4d MHz. ",
			pll_clk, od, pll_clk >> od);
		/* get gp0_pll_clk */
		clk_msr_val = clk_util_clk_msr(4);
		printf("CLKMSR(4) - %4d MHz ", clk_msr_val);
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

static int gp1_pll_test(gp1_pll_set_t *gp1_pll)
{
	unsigned int pll_clk = 0;
	int ret = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0, od = 0, m = 0, n = 0;

	od = (gp1_pll->pll_cntl0 >> 12) & 0x7;

	if (gp1_pll->pll_clk == 0) {
		m = gp1_pll->pll_cntl0 & 0x1FF;
		n = (gp1_pll->pll_cntl0 >> 16) & 0x1F;
		pll_clk = (24 *  m / n) >> od;
	} else {
		pll_clk = gp1_pll->pll_clk;
	}

	writel(gp1_pll->pll_cntl0, HHI_GP1_PLL_CNTL0);
	writel(gp1_pll->pll_cntl1, HHI_GP1_PLL_CNTL0);
	writel(gp1_pll->pll_cntl2, HHI_GP1_PLL_CNTL1);
	writel(gp1_pll->pll_cntl3, HHI_GP1_PLL_CNTL2);
	writel(gp1_pll->pll_cntl4, HHI_GP1_PLL_CNTL3);
	_udelay(20);
	writel(gp1_pll->pll_cntl5, HHI_GP1_PLL_CNTL0);
	_udelay(20);
	writel(gp1_pll->pll_cntl6, HHI_GP1_PLL_CNTL2);
	_udelay(10);
	do {
		writel(readl(HHI_GP1_PLL_CNTL0) | (1 << 29),
				HHI_GP1_PLL_CNTL0);
		_udelay(10);
		writel(readl(HHI_GP1_PLL_CNTL0) & (~(1 << 29)),
				HHI_GP1_PLL_CNTL0);
		_udelay(100);
	} while ((!((readl(HHI_GP1_PLL_CNTL0) >> 31) & 0x1)) && --lock_check);

	if (lock_check == 0) {
		printf("GP1 pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("GP1 pll lock OK! - %4d MHz >>%d - %4d MHz. ",
			pll_clk, od, pll_clk >> od);
		/* get gp1_pll_clk */
		clk_msr_val = clk_util_clk_msr(86);
		printf("CLKMSR(86) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int gp1_pll_test_all(gp1_pll_cfg_t *gp1_pll_cfg)
{
	unsigned int i = 0;
	int ret = 0;

	for (i = 0; i < (sizeof(gp1_pll_cfg_t) / sizeof(gp1_pll_set_t)); i++)
		ret += gp1_pll_test(&gp1_pll_cfg->gp1_pll[i]);
	return ret;
}

static int hifi_pll_test(hifi_pll_set_t * hifi_pll)
{
	unsigned int pll_clk = 0;
	int ret = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0, od = 0, m = 0, n = 0, frac = 0;

	od = (hifi_pll->pll_cntl0 >> 16) & 0x3;

	if (hifi_pll->pll_clk == 0) {
		m = hifi_pll->pll_cntl0 & 0x1FF;
		n = (hifi_pll->pll_cntl0 >> 10) & 0x1F;
		frac = hifi_pll->pll_cntl1 & 0x3FFFF;
		if ((hifi_pll->pll_cntl1 >> 18) & 0x1)
			pll_clk = 24 * (131072 * m - frac) / 131072 / n;
		else
			pll_clk = 24 * (131072 * m + frac) / 131072 / n;
	} else {
		pll_clk = hifi_pll->pll_clk;
	}

	do {
		writel(hifi_pll->pll_cntl0, HHI_HIFI_PLL_CNTL0);
		writel(hifi_pll->pll_cntl0 | (3 << 28), HHI_HIFI_PLL_CNTL0);
		writel(hifi_pll->pll_cntl1, HHI_HIFI_PLL_CNTL1);
		writel(hifi_pll->pll_cntl2, HHI_HIFI_PLL_CNTL2);
		writel(hifi_pll->pll_cntl3, HHI_HIFI_PLL_CNTL3);
		writel(hifi_pll->pll_cntl4, HHI_HIFI_PLL_CNTL4);
		writel(hifi_pll->pll_cntl5, HHI_HIFI_PLL_CNTL5);
		writel(hifi_pll->pll_cntl6, HHI_HIFI_PLL_CNTL6);
		writel(readl(HHI_HIFI_PLL_CNTL0) | (1 << 29),
			HHI_HIFI_PLL_CNTL0);
		_udelay(10);
		writel(readl(HHI_HIFI_PLL_CNTL0) & (~(1 << 29)),
			HHI_HIFI_PLL_CNTL0);
		_udelay(100);
	} while ((!((readl(HHI_HIFI_PLL_CNTL0) >> 31) & 0x1)) &&
			--lock_check);
	if (0 == lock_check) {
		printf("HIFI pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("HIFI pll lock OK! - %4d MHz.Div8  - %4d MHz.",
			pll_clk, pll_clk >> od);
		/* get hifi_pll_clk */
		clk_msr_val = clk_util_clk_msr(12);
		printf("CLKMSR(12) - %4d MHz ", clk_msr_val);
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

static int adc_pll_test(adc_pll_set_t * adc_pll) {
	int ret = 0;
	unsigned int pll_clk = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	unsigned int pre_div= 0, od = 0, xd = 0, m = 0, n = 0;
	unsigned int od_div[4] = {1, 2, 4, 3};

	if (adc_pll->cpu_clk == 0) {
		m = adc_pll->pll_cntl0 & 0x1FF;
		n = (adc_pll->pll_cntl0 >> 10) & 0x1F;
		/* 24bit if set 1 Pre_div = 4   if set 0 Pre_div = 2  */
		if ((adc_pll->pll_cntl0 >> 24) & 0x1)
			pre_div = 4;
		else
			pre_div = 2;

		od = od_div[(adc_pll->pll_cntl0 >> 16) & 0x3];
		xd = (adc_pll->pll_cntl1 >> 22) & 0x1F;
		pll_clk = 24 * m / n / pre_div / od / xd;
	} else {
		pll_clk = adc_pll->cpu_clk;
	}

	do {
		writel(adc_pll->pll_cntl0, HHI_ADC_PLL_CNTL);
		writel(adc_pll->pll_cntl0 | (3 << 28), HHI_ADC_PLL_CNTL);
		writel(adc_pll->pll_cntl1, HHI_ADC_PLL_CNTL1);
		writel(adc_pll->pll_cntl2, HHI_ADC_PLL_CNTL2);
		writel(adc_pll->pll_cntl3, HHI_ADC_PLL_CNTL3);
		writel(adc_pll->pll_cntl4, HHI_ADC_PLL_CNTL4);
		writel(adc_pll->pll_cntl5, HHI_ADC_PLL_CNTL5);
		writel(adc_pll->pll_cntl6, HHI_ADC_PLL_CNTL6);
		/* different with T5, T5D should config the two reg */
		writel(readl(0xff63c2ec) | (1 << 11), 0xff63c2ec);
		writel(readl(0xff63c108) | 0x582, 0xff63c108);

		writel(readl(HHI_ADC_PLL_CNTL) | (1 << 29),
			HHI_ADC_PLL_CNTL);
		_udelay(10);
		writel(readl(HHI_ADC_PLL_CNTL)&(~(1 << 29)),
			HHI_ADC_PLL_CNTL);
		_udelay(100);
	} while ((!((readl(HHI_ADC_PLL_CNTL)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("ADC pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("ADC pll lock OK! - %4d MHz ", pll_clk);
		/* get adc_pll_clk */
		clk_msr_val = clk_util_clk_msr(149);
		printf("CLKMSR(149) - %4d MHz ", clk_msr_val);

		if (clk_around(clk_msr_val, pll_clk )) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int adc_pll_test_all(adc_pll_cfg_t * adc_pll_cfg) {
	unsigned int i=0;
	int ret=0;

	for (i = 0; i < (sizeof(adc_pll_cfg_t) / sizeof(adc_pll_set_t)); i++)
		ret += adc_pll_test(&(adc_pll_cfg->adc_pll[i]));

	return ret;
}

static int tcon_pll_test(tcon_pll_set_t * tcon_pll) {
	int ret = 0;
	unsigned int pll_clk = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	unsigned int clk_msr_reg = 0;
	unsigned int od = 0, od1 = 0, od2 = 0, od3 = 0, m = 0, n = 0, frac = 0;
	unsigned int od_div[4] = {1, 2, 3, 4};

	/* div_15 */
	clk_msr_reg = readl(HHI_VID_PLL_CLK_DIV);
	clk_msr_reg = clk_msr_reg & (~(0xf<<16));
	clk_msr_reg = clk_msr_reg | (0xa<<16);
	writel(clk_msr_reg,HHI_VID_PLL_CLK_DIV);

	/* tcon enable */
	writel(readl(HHI_LVDS_TX_PHY_CNTL1)&(~(1<<29)),HHI_LVDS_TX_PHY_CNTL1);

	/* od */
	od1 = (tcon_pll->pll_cntl0 >> 19) & 0x3;
	od2 = (tcon_pll->pll_cntl0 >> 21) & 0x3;
	od3 = (tcon_pll->pll_cntl0 >> 23) & 0x3;
	od = od_div[od1] * od_div[od2] * od_div[od3];

	if (tcon_pll->cpu_clk == 0) {
		m = tcon_pll->pll_cntl0 & 0xFF;
		n = (tcon_pll->pll_cntl0 >> 10) & 0x1F;
		frac = tcon_pll->pll_cntl1 & 0x3FFFF;
		if ((tcon_pll->pll_cntl1 >> 18) & 0x1)
			pll_clk = 24 * (131072 * m - frac) / 131072 / n;
		else
			pll_clk = 24 * (131072 * m + frac) / 131072 / n;
	} else {
		pll_clk = tcon_pll->cpu_clk;
	}

	do {
		writel(tcon_pll->pll_cntl0, HHI_TCON_PLL_CNTL0);
		_udelay(50);
		writel(tcon_pll->pll_cntl0 | (1 << 28), HHI_TCON_PLL_CNTL0);
		_udelay(50);
		writel(tcon_pll->pll_cntl0 | (3 << 28), HHI_TCON_PLL_CNTL0);
		_udelay(50);
		writel(tcon_pll->pll_cntl1, HHI_TCON_PLL_CNTL1);
		_udelay(50);
		writel(tcon_pll->pll_cntl2, HHI_TCON_PLL_CNTL2);
		_udelay(50);
		writel(tcon_pll->pll_cntl3, HHI_TCON_PLL_CNTL3);
		_udelay(50);
		writel(tcon_pll->pll_cntl4, HHI_TCON_PLL_CNTL4);
		_udelay(50);
		writel(tcon_pll->pll_cntl41, HHI_TCON_PLL_CNTL4);
		_udelay(50);
		writel(tcon_pll->pll_cntl01, HHI_TCON_PLL_CNTL0);
		_udelay(50);
		writel(tcon_pll->pll_cntl02, HHI_TCON_PLL_CNTL0);
		_udelay(50);
		writel(tcon_pll->pll_cnt21, HHI_TCON_PLL_CNTL2);
		if (tcon_pll->pll_cntl42) {
			_udelay(50);
			writel(tcon_pll->pll_cntl42, HHI_TCON_PLL_CNTL4);
		}
		_udelay(100);
	} while ((!((readl(HHI_TCON_PLL_CNTL0)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("TCON pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("TCON pll lock OK! - %4d MHz.Div%4d  - %4d MHz. ", pll_clk, od * 15, pll_clk / od / 15);
		/* get tcon_pll_clk */
		clk_msr_val = clk_util_clk_msr(55);
		printf("CLKMSR(55) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk / od / 15)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int tcon_pll_test_all(tcon_pll_cfg_t * tcon_pll_cfg) {
	unsigned int i=0;
	int ret=0;

	for (i = 0; i < (sizeof(tcon_pll_cfg_t) / sizeof(tcon_pll_set_t)); i++)
		ret += tcon_pll_test(&(tcon_pll_cfg->tcon_pll[i]));

	return ret;
}

static int m_pll_test(m_pll_set_t * m_pll) {
	unsigned int pll_clk = 0;
	unsigned int clk_msr_val = 0, n2_integer = 0, sdm_in = 0;
	int ret=0;


	if (m_pll->pll_clk == 0) {
		sdm_in = m_pll->pll_cntl1 & 0x3FFF;
		n2_integer = (m_pll->pll_cntl1 >> 20) & 0x1FF;
		pll_clk = 2000 * 16384 / (n2_integer * 16384 + sdm_in);
	} else {
		pll_clk = m_pll->pll_clk;
	}

	Wr(HHI_MPLL_CNTL0, m_pll->pll_cntl0);
	_udelay(20);
	Wr(HHI_MPLL_CNTL1, m_pll->pll_cntl1);
	_udelay(20);
	Wr(HHI_MPLL_CNTL2, m_pll->pll_cntl2);
	_udelay(20);
	Wr(HHI_MPLL_CNTL3, m_pll->pll_cntl3);
	_udelay(20);
	Wr(HHI_MPLL_CNTL4, m_pll->pll_cntl4);
	_udelay(20);
	Wr(HHI_MPLL_CNTL5, m_pll->pll_cntl5);
	_udelay(20);
	Wr(HHI_MPLL_CNTL6, m_pll->pll_cntl6);
	_udelay(20);
	Wr(HHI_MPLL_CNTL7, m_pll->pll_cntl7);
	_udelay(20);
	Wr(HHI_MPLL_CNTL8, m_pll->pll_cntl8);
	_udelay(20);

	printf("M pll lock OK! - %4d MHz.", pll_clk);
	/* get mp0_clk_out */
	clk_msr_val = clk_util_clk_msr(42);
	printf("CLKMSR(42) - %4d MHz ", clk_msr_val);
	if (clk_around(clk_msr_val, pll_clk)) {
		printf(": Match\n");
	} else {
		printf(": MisMatch\n");
		ret = RET_CLK_NOT_MATCH;
	}
	return ret;
}

static int m_pll_test_all(m_pll_cfg_t * m_pll_cfg) {
	return m_pll_test(&(m_pll_cfg->m_pll[0]));
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
		case PLL_HDMIRX:
			ret = hdmirx_pll_test_all(&hdmirx_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HDMIRX);
			break;
		case PLL_GP0:
			ret = gp0_pll_test_all(&gp0_pll_cfg);
			pll_report(ret, STR_PLL_TEST_GP0);
			break;
		case PLL_HIFI:
			ret = hifi_pll_test_all(&hifi_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HIFI);
			break;
		case PLL_ADC:
			ret = adc_pll_test_all(&adc_pll_cfg);
			pll_report(ret, STR_PLL_TEST_ADC);
			break;
		case PLL_TCON:
			ret = tcon_pll_test_all(&tcon_pll_cfg);
			pll_report(ret, STR_PLL_TEST_TCON);
			break;
		case PLL_M:
			ret = m_pll_test_all(&m_pll_cfg);
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
	hdmirx_pll_set_t hdmirx_pll_set = {0};
	gp0_pll_set_t gp0_pll_set = {0};
	gp1_pll_set_t gp1_pll_set = {0};
	hifi_pll_set_t hifi_pll_set = {0};
	adc_pll_set_t adc_pll_set = {0};
	m_pll_set_t m_pll_set = {0};
	tcon_pll_set_t tcon_pll_set = {0};
	//unsigned int tcon_parameter1 = 0;

	unsigned char plls[PLL_ENUM] = {
		PLL_SYS,
		0xff,//	PLL_FIX, //0xff will skip this pll
		0xff,//	PLL_DDR,
		PLL_HDMIRX,
		PLL_GP0,
		PLL_HIFI,
		PLL_ADC,
		PLL_TCON,
		PLL_M
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
	} else if (0 == strcmp(STR_PLL_TEST_HDMIRX, argv[1])) {
		if (argc == 2) {
			ret = hdmirx_pll_test_all(&hdmirx_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HDMIRX);
		} else if (argc != 13) {
			printf("%s pll test: args error\n", STR_PLL_TEST_HDMIRX);
			return -1;
		} else {
			hdmirx_pll_set.pll_misc_cntl0 = simple_strtoul(argv[2], NULL, 16);
			hdmirx_pll_set.pll_cntl0 = simple_strtoul(argv[3], NULL, 16);
			hdmirx_pll_set.pll_cntl1 = simple_strtoul(argv[4], NULL, 16);
			hdmirx_pll_set.pll_cntl2 = simple_strtoul(argv[5], NULL, 16);
			hdmirx_pll_set.pll_cntl3 = simple_strtoul(argv[6], NULL, 16);
			hdmirx_pll_set.pll_cntl4 = simple_strtoul(argv[7], NULL, 16);
			hdmirx_pll_set.pll_cntl41 = simple_strtoul(argv[8], NULL, 16);
			hdmirx_pll_set.pll_cntl03 = simple_strtoul(argv[9], NULL, 16);
			hdmirx_pll_set.pll_cntl04 = simple_strtoul(argv[10], NULL, 16);
			hdmirx_pll_set.pll_cntl42 = simple_strtoul(argv[11], NULL, 16);
			hdmirx_pll_set.pll_cntl21 = simple_strtoul(argv[12], NULL, 16);
			ret = hdmirx_pll_test(&hdmirx_pll_set);
			pll_report(ret, STR_PLL_TEST_HDMIRX);
		}
	} else if (0 == strcmp(STR_PLL_TEST_GP0, argv[1])) {
		if (argc == 2) {
			ret = gp0_pll_test_all(&gp0_pll_cfg);
			pll_report(ret, STR_PLL_TEST_GP0);
		} else if (argc != 11) {
			printf("%s pll test: args error\n", STR_PLL_TEST_GP0);
			return -1;
		} else {
			gp0_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL,
							16);
			gp0_pll_set.pll_cntl1 = simple_strtoul(argv[4], NULL,
							16);
			gp0_pll_set.pll_cntl2 = simple_strtoul(argv[5], NULL,
							16);
			gp0_pll_set.pll_cntl3 = simple_strtoul(argv[6], NULL,
							16);
			gp0_pll_set.pll_cntl4 = simple_strtoul(argv[7], NULL,
							16);
			gp0_pll_set.pll_cntl5 = simple_strtoul(argv[8], NULL,
							16);
			gp0_pll_set.pll_cntl6 = simple_strtoul(argv[9], NULL,
							16);
			ret = gp0_pll_test(&gp0_pll_set);
			pll_report(ret, STR_PLL_TEST_GP0);
		}
	} else if (strcmp(STR_PLL_TEST_GP1, argv[1]) == 0) {
		if (argc == 2) {
			ret = gp1_pll_test_all(&gp1_pll_cfg);
			pll_report(ret, STR_PLL_TEST_GP1);
		} else if (argc != 9) {
			printf("%s pll test: args error\n", STR_PLL_TEST_GP1);
			ret = -1;
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
	} else if (0 == strcmp(STR_PLL_TEST_HIFI, argv[1])) {
		if (argc == 2) {
			ret = hifi_pll_test_all(&hifi_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HIFI);
		} else if (argc != 11) {
			printf("%s pll test:args error\n", STR_PLL_TEST_HIFI);
			return -1;
		} else {
			hifi_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL,
							16);
			hifi_pll_set.pll_cntl1 = simple_strtoul(argv[4], NULL,
							16);
			hifi_pll_set.pll_cntl2 = simple_strtoul(argv[5], NULL,
							16);
			hifi_pll_set.pll_cntl3 = simple_strtoul(argv[6], NULL,
							16);
			hifi_pll_set.pll_cntl4 = simple_strtoul(argv[7], NULL,
							16);
			hifi_pll_set.pll_cntl5 = simple_strtoul(argv[8], NULL,
							16);
			hifi_pll_set.pll_cntl6 = simple_strtoul(argv[9], NULL,
							16);
			ret = hifi_pll_test(&hifi_pll_set);
			pll_report(ret, STR_PLL_TEST_HIFI);
		}
	} else if (0 == strcmp(STR_PLL_TEST_ADC, argv[1])) {
		if (argc == 2) {
			ret = adc_pll_test_all(&adc_pll_cfg);
			pll_report(ret, STR_PLL_TEST_ADC);
		} else if (argc != 9){
			printf("%s pll test: args error\n", STR_PLL_TEST_ADC);
			return -1;
		} else {
			adc_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			adc_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL, 16);
			adc_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL, 16);
			adc_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL, 16);
			adc_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL, 16);
			adc_pll_set.pll_cntl5 = simple_strtoul(argv[7], NULL, 16);
			adc_pll_set.pll_cntl6 = simple_strtoul(argv[8], NULL, 16);
			ret = adc_pll_test(&adc_pll_set);
			pll_report(ret, STR_PLL_TEST_ADC);
		}
	} else if (0 == strcmp(STR_PLL_TEST_TCON, argv[1])) {
		if (argc == 2) {
			ret = tcon_pll_test_all(&tcon_pll_cfg);
			pll_report(ret, STR_PLL_TEST_TCON);
		} else if (argc == 12) {
			tcon_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			tcon_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL, 16);
			tcon_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL, 16);
			tcon_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL, 16);
			tcon_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL, 16);
			tcon_pll_set.pll_cntl41 = simple_strtoul(argv[7], NULL, 16);
			tcon_pll_set.pll_cntl01 = simple_strtoul(argv[8], NULL, 16);
			tcon_pll_set.pll_cntl02 = simple_strtoul(argv[9], NULL, 16);
			tcon_pll_set.pll_cnt21 = simple_strtoul(argv[10], NULL, 16);
			tcon_pll_set.pll_cntl42 = simple_strtoul(argv[11], NULL, 16);
			ret = tcon_pll_test(&tcon_pll_set);
			pll_report(ret, STR_PLL_TEST_TCON);
		} else if (argc == 11) {
			tcon_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			tcon_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL, 16);
			tcon_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL, 16);
			tcon_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL, 16);
			tcon_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL, 16);
			tcon_pll_set.pll_cntl41 = simple_strtoul(argv[7], NULL, 16);
			tcon_pll_set.pll_cntl01 = simple_strtoul(argv[8], NULL, 16);
			tcon_pll_set.pll_cntl02 = simple_strtoul(argv[9], NULL, 16);
			tcon_pll_set.pll_cnt21 = simple_strtoul(argv[10], NULL, 16);
			ret = tcon_pll_test(&tcon_pll_set);
			pll_report(ret, STR_PLL_TEST_TCON);
		} else {
			printf("%s pll test: args error\n", STR_PLL_TEST_TCON);
			return -1;
		}
	} else if (0 == strcmp(STR_PLL_TEST_MPLL, argv[1])) {
		if (argc == 2) {
			ret = m_pll_test_all(&m_pll_cfg);
			pll_report(ret, STR_PLL_TEST_MPLL);
		} else if (argc != 11){
			printf("%s pll test: args error\n", STR_PLL_TEST_MPLL);
			return -1;
		} else {
			m_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			m_pll_set.pll_cntl1 = simple_strtoul(argv[3], NULL, 16);
			m_pll_set.pll_cntl2 = simple_strtoul(argv[4], NULL, 16);
			m_pll_set.pll_cntl3 = simple_strtoul(argv[5], NULL, 16);
			m_pll_set.pll_cntl4 = simple_strtoul(argv[6], NULL, 16);
			m_pll_set.pll_cntl5 = simple_strtoul(argv[7], NULL, 16);
			m_pll_set.pll_cntl6 = simple_strtoul(argv[8], NULL, 16);
			m_pll_set.pll_cntl7 = simple_strtoul(argv[9], NULL, 16);
			m_pll_set.pll_cntl8 = simple_strtoul(argv[10], NULL, 16);
			ret = m_pll_test(&m_pll_set);
			pll_report(ret, STR_PLL_TEST_MPLL);
		}
	}

	return ret;
}
