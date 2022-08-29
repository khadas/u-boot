/*
 * rev history:
 * 2017.04.06 structure done
 *
 *
 *
 *
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
#define STR_PLL_TEST_GP0	"gp0"
#define STR_PLL_TEST_HIFI	"hifi"
#define STR_PLL_TEST_PCIE   "pcie"
#define STR_PLL_TEST_ADC    "adc"
#define STR_PLL_TEST_HDMITX "hdmitx"
#define STR_PLL_TEST_HDMIRX "hdmirx"
#define STR_PLL_TEST_TCON   "tcon"


#define PLL_LOCK_CHECK_MAX		10

#define RET_PLL_LOCK_FAIL		0x1000
#define RET_CLK_NOT_MATCH		0x1
#define SYS_PLL_DIV16_CNTL		(1<<24)
#define SYS_CLK_DIV16_CNTL		(1)
#define SYS_PLL_TEST_DIV		4	/* div16 */
#define HDMI_PLL_DIV_CNTL		(1<<18)
#define HDMI_PLL_DIV_GATE		(1<<19)

#define PLL_DIV16_OFFSET		4	/* div2/2/4 */
#define GP0_PLL_TEST_DIV		0	/* div1 */

#define Wr(addr, data) writel(data, addr)
#define Rd(addr) readl(addr)

static int sys_pll_init(sys_pll_set_t * sys_pll_set);
static int sys_pll_test(sys_pll_set_t1 * sys_pll_set1);
static int sys_pll_init_test(sys_pll_set_t1 * sys_pll_set);
static int sys_pll_test_all(sys_pll_cfg_t1 * sys_pll_cfg1);

//static int ddr_pll_test(void);


static int hdmitx_pll_test(hdmitx_pll_set_t * hdmitx_pll_set);
static int hdmitx_pll_test_all(hdmitx_pll_cfg_t * hdmitx_pll_cfg);

static int gp0_pll_test(gp0_pll_set_t * gp0_pll);
static int gp0_pll_test_all(gp0_pll_cfg_t * gp0_pll_cfg);
static int gp0_pll_init(gp0_pll_set_t * gp0_pll);

static int hifi_pll_test_all(hifi_pll_cfg_t * hifi_pll_cfg);
static int hifi_pll_test(hifi_pll_set_t * hifi_pll);

static int pcie_pll_test_all(pcie_pll_cfg_t * pcie_pll_cfg);
static int pcie_pll_test(pcie_pll_set_t * pcie_pll);

static int adc_pll_test_all(adc_pll_cfg_t * adc_pll_cfg);
static int adc_pll_test(adc_pll_set_t * adc_pll);

//static int fix_pll_test(fix_pll_set_t * fix_pll);
//static int fix_pll_test_all(fix_pll_cfg_t * fix_pll_cfg);


static int tcon1_pll_test(unsigned int pll_clk);
static int tcon2_pll_test(unsigned int pll_clk);
static int tcon_pll_test_all(void);

static int hdmirx1_pll_test(unsigned int pll_clk);
static int hdmirx2_pll_test(unsigned int pll_clk);
static int hdmirx_pll_test_all(void);

static unsigned int clok_div_enter(unsigned int clock , unsigned int clok_check);

sys_pll_cfg_t1 sys_pll_cfg1 = {
	.sys_pll[0] = {
		.cpu_clk   = 6000,
		.pll_cntl0  = 0x080304fa,
		.pll_cntl01  = 0x380304fa,
		.pll_cntl1 = 0x0,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x48681c00,
		.pll_cntl4 = 0x88770290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000,
		.pll_cntl02  = 0x180304fa
	},
	.sys_pll[1] = {
		.cpu_clk   = 3000,
		.pll_cntl0  = 0x0803047d,
		.pll_cntl01  = 0x3803047d,
		.pll_cntl1 = 0x0,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x48681c00,
		.pll_cntl4 = 0x88770290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000,
		.pll_cntl02  = 0x1803047d
	},
};

gp0_pll_cfg_t gp0_pll_cfg = {
	.gp0_pll[0] = {
		.cpu_clk   = 6000,
		.pll_cntl0  = 0x080304fa,
		.pll_cntl01  = 0x380304fa,
		.pll_cntl1 = 0x0,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x48681c00,
		.pll_cntl4 = 0x88770290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000,
		.pll_cntl02  = 0x180304fa
	},
	.gp0_pll[1] = {
		.cpu_clk   = 3000,
		.pll_cntl0  = 0x0803047d,
		.pll_cntl01  = 0x3803047d,
		.pll_cntl1 = 0x00006aab,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x6a295c00,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x54540000,
		.pll_cntl02  = 0X1803047d
	},
};

hifi_pll_cfg_t hifi_pll_cfg = {
	.hifi_pll[0] = {
		.cpu_clk   = 6005,
		.pll_cntl0  = 0x080304fa,
		.pll_cntl01  = 0x380304fa,
		.pll_cntl1 = 0x00006aab,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x6a285c00,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000,
		.pll_cntl02  = 0x180304fa
	},

	.hifi_pll[1] = {
		.cpu_clk   = 3000,
		.pll_cntl0  = 0X0803047d,
		.pll_cntl01  = 0X3803047d,
		.pll_cntl1 = 0X00006aab,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x6a295c00,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x54540000,
		.pll_cntl02  = 0X1803047d
	},
};

pcie_pll_cfg_t pcie_pll_cfg = {
	.pcie_pll[0] = {
		.cpu_clk   = 4800,
		.pll_cntl0 = 0x200c04c8,
		.pll_cntl01 = 0x300c04c8,
		.pll_cntl1 = 0x30000000,
		.pll_cntl2 = 0x00001100,
		.pll_cntl3 = 0x10058e00,
		.pll_cntl4 = 0x000100c0,
		.pll_cntl5 = 0x68000048,
		.pll_cntl51 = 0x68000068,
		.pll_cntl41 = 0x008100c0,
		.pll_cntl02 = 0x340c04c8,
		.pll_cntl03 = 0x140c04c8,
		.pll_cntl21 = 0x00001000
	},

};

adc_pll_cfg_t adc_pll_cfg = {
	.adc_pll[0] = {
		.cpu_clk   = 4800,
		.pll_cntl0 = 0X012004c8,
		.pll_cntl01 = 0X312004c8,
		.pll_cntl1 = 0X03000000,
		.pll_cntl2 = 0xe1800000,
		.pll_cntl3 = 0x48681c00,
		.pll_cntl4 = 0x88770290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000,
		.pll_cntl02 = 0X112004c8
	},
	.adc_pll[1] = {
		.cpu_clk   = 3000,
		.pll_cntl0 = 0X0120047d,
		.pll_cntl01 = 0X3120047d,
		.pll_cntl1 = 0X03000000,
		.pll_cntl2 = 0xe1800000,
		.pll_cntl3 = 0x48681c00,
		.pll_cntl4 = 0x88770290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000,
		.pll_cntl02 = 0X1120047d
	},
};


#if 0
fix_pll_cfg_t fix_pll_cfg = {
	.fix_pll[0] = {
		.cpu_clk   = 400,
		.pll_cntl0 = 0x000104a6 ,
		.pll_cntl01 = 0x300104a6 ,
		.pll_cntl1 = 0x03f15555,
		.pll_cntl2 = 0x00000000,
		.pll_cntl3 = 0x6a285c60,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x54540000,
		.pll_cntl02 = 0x100104a6
	},
};
#endif

hdmitx_pll_cfg_t hdmitx_pll_cfg = {
	.hdmitx_pll[0] = {
		.cpu_clk   = 5940,
		.pll_cntl0 = 0x0b3a04f7 ,
		.pll_cntl01 = 0x3b3a04f7 ,
		.pll_cntl1 = 0x00010000,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x6a28dc00,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x56540000,
		.pll_cntl02 = 0x1b3a04f7
	},
	.hdmitx_pll[1] = {
		.cpu_clk   = 2970,
		.pll_cntl0 = 0x0b3a047b ,
		.pll_cntl01 = 0x3b3a047b ,
		.pll_cntl1 = 0x00018000,
		.pll_cntl2 = 0x0,
		.pll_cntl3 = 0x6a29dc00,
		.pll_cntl4 = 0x65771290,
		.pll_cntl5 = 0x39272000,
		.pll_cntl6 = 0x54540000,
		.pll_cntl02 = 0x1b3a047b
	},
};

static unsigned int clok_div_enter(unsigned int clock , unsigned int div)
{
	unsigned int clk_div = clock/div;
	unsigned int a = clk_div * div;
	unsigned int yushu = 0;
	if (clock != a) {
		yushu = clock - a;
		if (yushu >= (div/2)) {
			return (clk_div + 1);
		}
		if (yushu < (div/2)) {
			return clk_div;
		}
	}
	return clk_div;
}

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

	uint32_t    control1 = Rd(HHI_SYS_CPU_CLK_CNTL1);

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

static int sys_pll_init(sys_pll_set_t * sys_pll_set) {
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	do {
		Wr(HHI_SYS_PLL_CNTL0, sys_pll_set->pll_cntl|(1<<29));
		Wr(HHI_SYS_PLL_CNTL1, sys_pll_set->pll_cntl1);
		Wr(HHI_SYS_PLL_CNTL2, sys_pll_set->pll_cntl2);
		Wr(HHI_SYS_PLL_CNTL3, sys_pll_set->pll_cntl3);
		Wr(HHI_SYS_PLL_CNTL4, sys_pll_set->pll_cntl4);
		Wr(HHI_SYS_PLL_CNTL5, sys_pll_set->pll_cntl5);
		Wr(HHI_SYS_PLL_CNTL0, ((1<<30)|(1<<29)|sys_pll_set->pll_cntl));
		Wr(HHI_SYS_PLL_CNTL0, Rd(HHI_SYS_PLL_CNTL0)&(~(1<<29)));
		_udelay(20);
		//printf("pll lock check %d\n", lock_check);
	} while((!((readl(HHI_SYS_PLL_CNTL0)>>31)&0x1)) && --lock_check);
	if (lock_check != 0)
		return 0;
	else
		return RET_PLL_LOCK_FAIL;
}

static int sys_pll_init_test(sys_pll_set_t1 * sys_pll_set) {
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	do {
		Wr(HHI_SYS_PLL_CNTL0, sys_pll_set->pll_cntl0);
		Wr(HHI_SYS_PLL_CNTL0, sys_pll_set->pll_cntl01);
		Wr(HHI_SYS_PLL_CNTL1, sys_pll_set->pll_cntl1);
		Wr(HHI_SYS_PLL_CNTL2, sys_pll_set->pll_cntl2);
		Wr(HHI_SYS_PLL_CNTL3, sys_pll_set->pll_cntl3);
		Wr(HHI_SYS_PLL_CNTL4, sys_pll_set->pll_cntl4);
		Wr(HHI_SYS_PLL_CNTL5, sys_pll_set->pll_cntl5);
		Wr(HHI_SYS_PLL_CNTL6, sys_pll_set->pll_cntl6);
		Wr(HHI_SYS_PLL_CNTL0, sys_pll_set->pll_cntl02);
		_udelay(20);
		//printf("pll lock check %d\n", lock_check);
	} while((!((readl(HHI_SYS_PLL_CNTL0)>>31)&0x1)) && --lock_check);
	if (lock_check != 0)
		return 0;
	else
		return RET_PLL_LOCK_FAIL;
}


static int sys_pll_test_all(sys_pll_cfg_t1 * sys_pll_cfg1) {
	unsigned int i=0;
	int ret=0;
	for (i=0; i<PLL_TEST_SYS_TOTAL; i++) {
		ret += sys_pll_test(&(sys_pll_cfg1->sys_pll[i]));
	}
	return ret;
}

static int sys_pll_test(sys_pll_set_t1 * sys_pll_set1) {
	unsigned int clk_msr_reg = 0;
	unsigned int clk_msr_val = 0;
	unsigned int sys_clk = 0;
	int ret = 0;
//	unsigned int od = (sys_pll_set1->pll_cntl0>>16)&0x3;
	unsigned int clk_div = 0;

	/* switch sys clk to oscillator */
	clocks_set_sys_cpu_clk( 0, 0, 0, 0);

	/* store current sys pll cntl */
	sys_pll_set_t sys_pll;
	sys_pll.pll_cntl = readl(HHI_SYS_PLL_CNTL0);
	sys_pll.pll_cntl1 = readl(HHI_SYS_PLL_CNTL1);
	sys_pll.pll_cntl2 = readl(HHI_SYS_PLL_CNTL2);
	sys_pll.pll_cntl3 = readl(HHI_SYS_PLL_CNTL3);
	sys_pll.pll_cntl4 = readl(HHI_SYS_PLL_CNTL4);
	sys_pll.pll_cntl5 = readl(HHI_SYS_PLL_CNTL5);


	if (sys_pll_set1->cpu_clk == 0) {
		sys_clk = (24 / ((sys_pll_set1->pll_cntl0>>10)&0x1F) * (sys_pll_set1->pll_cntl0&0x1FF));
	}
	else {
		sys_clk = sys_pll_set1->cpu_clk;
	}

	/* store CPU clk msr reg, restore it when test done */
	clk_msr_reg = readl(HHI_SYS_CPU_CLK_CNTL1);

	/* enable CPU clk msr cntl bit */
	writel(clk_msr_reg | SYS_PLL_DIV16_CNTL | SYS_CLK_DIV16_CNTL, HHI_SYS_CPU_CLK_CNTL1);

	if (0 == sys_pll_set1->pll_cntl0) {
		printf("sys pll cntl equal NULL, skip\n");
		return -1;
	}

	/* test sys pll */
	if (sys_pll_set1->cpu_clk) {
		sys_clk = sys_pll_set1->cpu_clk;
	}

	ret = sys_pll_init_test(sys_pll_set1);
	_udelay(100);
	if (ret) {
		printf("SYS pll lock Failed! - %4d MHz\n", sys_clk);
	}
	else {
		clk_div = clok_div_enter(sys_clk , 128);
		printf("SYS pll lock OK! - %4d MHz. Div128 - %4d MHz. ", sys_clk, clk_div);
		clk_msr_val = clk_util_clk_msr(17);
		printf("CLKMSR(17) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, clk_div)) {
			/* sys clk/pll div16/od */
			printf(": Match\n");
		}
		else {
			ret = RET_CLK_NOT_MATCH;
			printf(": MisMatch\n");
		}
	}
	/* restore sys pll */
	sys_pll_init(&sys_pll);
	clocks_set_sys_cpu_clk( 1, 0, 0, 0);

	/* restore clk msr reg */
	writel(clk_msr_reg, HHI_SYS_CPU_CLK_CNTL1);
	return ret;
}

#if 0
static int ddr_pll_test(void) {
	return 0;
}
#endif

static int gp0_pll_init(gp0_pll_set_t * gp0_pll)
{
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;

	do {
		Wr(HHI_GP0_PLL_CNTL0, gp0_pll->pll_cntl0);
		Wr(HHI_GP0_PLL_CNTL0, gp0_pll->pll_cntl01);
		Wr(HHI_GP0_PLL_CNTL1, gp0_pll->pll_cntl1);
		Wr(HHI_GP0_PLL_CNTL2, gp0_pll->pll_cntl2);
		Wr(HHI_GP0_PLL_CNTL3, gp0_pll->pll_cntl3);
		Wr(HHI_GP0_PLL_CNTL4, gp0_pll->pll_cntl4);
		Wr(HHI_GP0_PLL_CNTL5, gp0_pll->pll_cntl5);
		Wr(HHI_GP0_PLL_CNTL6, gp0_pll->pll_cntl6);
		Wr(HHI_GP0_PLL_CNTL0, gp0_pll->pll_cntl02);
		//printf("lock_check: %d\n", lock_check);
		_udelay(100);
	} while ((!((readl(HHI_GP0_PLL_CNTL0)>>31)&0x1)) && --lock_check);
	if (lock_check != 0)
		return 0;
	else
		return RET_PLL_LOCK_FAIL;
}

static int gp0_pll_test(gp0_pll_set_t * gp0_pll)
{
	unsigned int pll_clk = 0;
	int ret=0;
	unsigned int clk_msr_val = 0,od  = 0;
	od = (gp0_pll->pll_cntl0 >> 16) & 0x7;

	if (gp0_pll->cpu_clk == 0) {
//		pll_clk = (24 / ((gp0_pll->pll_cntl0>>10)&0x1F) * (gp0_pll->pll_cntl0&0x1FF)) >> ((gp0_pll->pll_cntl0>>16)&0x7);
		pll_clk = (24 / ((gp0_pll->pll_cntl0>>10)&0x1F) * (gp0_pll->pll_cntl0&0x1FF));
	}
	else{
		pll_clk = gp0_pll->cpu_clk;
	}

	ret = gp0_pll_init(gp0_pll);
	_udelay(20);

	if (ret) {
		printf("GP0 pll lock Failed! - %4d MHz\n", pll_clk);
	} else {
		printf("GP0 pll lock OK! - %4d MHz.Div8 - %4d MHz. ", pll_clk,pll_clk >> od);
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
	unsigned int i=0;
	int ret=0;
	for (i=0; i<PLL_TEST_HIFI_TOTAL; i++)
		ret += gp0_pll_test(&(gp0_pll_cfg->gp0_pll[i]));
	return ret;
}


static int hifi_pll_test(hifi_pll_set_t * hifi_pll)
{
	unsigned int pll_clk = 0;
	int ret=0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0,od = 0;
	od = (hifi_pll->pll_cntl0 >> 16) & 0x3;

	if (hifi_pll->cpu_clk == 0) {
//		pll_clk = (24 / ((hifi_pll->pll_cntl0>>10)&0x1F) * (hifi_pll->pll_cntl0&0x1FF)) >> ((hifi_pll->pll_cntl0>>16)&0x3);
		pll_clk = (24 / ((hifi_pll->pll_cntl0>>10)&0x1F) * (hifi_pll->pll_cntl0&0x1FF));
	}
	else{
		pll_clk = hifi_pll->cpu_clk;
	}

	do {
		Wr(HHI_HIFI_PLL_CNTL0, hifi_pll->pll_cntl0);
		Wr(HHI_HIFI_PLL_CNTL0, hifi_pll->pll_cntl01);
		Wr(HHI_HIFI_PLL_CNTL1, hifi_pll->pll_cntl1);
		Wr(HHI_HIFI_PLL_CNTL2, hifi_pll->pll_cntl2);
		Wr(HHI_HIFI_PLL_CNTL3, hifi_pll->pll_cntl3);
		Wr(HHI_HIFI_PLL_CNTL4, hifi_pll->pll_cntl4);
		Wr(HHI_HIFI_PLL_CNTL5, hifi_pll->pll_cntl5);
		Wr(HHI_HIFI_PLL_CNTL6, hifi_pll->pll_cntl6);
		Wr(HHI_HIFI_PLL_CNTL0, hifi_pll->pll_cntl02);
		//printf("lock_check: %d\n", lock_check);
		_udelay(100);
		//printf("hifi lock_check: %4d\n", lock_check);
	} while ((!((readl(HHI_HIFI_PLL_CNTL0)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("HIFI pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("HIFI pll lock OK! - %4d MHz.Div8  - %4d MHz.", pll_clk, pll_clk >> od);
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

static int hifi_pll_test_all(hifi_pll_cfg_t * hifi_pll_cfg) {
	unsigned int i = 0;
	int ret=0;

	for (i=0; i<2; i++) {
		ret += hifi_pll_test(&(hifi_pll_cfg->hifi_pll[i]));
	}
	return ret;
}

static int pcie_pll_test(pcie_pll_set_t * pcie_pll) {
	int ret = 0;
	unsigned int pll_clk = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;

	if (pcie_pll->cpu_clk == 0) {
		pll_clk = (24 / ((pcie_pll->pll_cntl0>>10)&0x1F) * (pcie_pll->pll_cntl0&0x1FF)) >> ((pcie_pll->pll_cntl0>>16)&0x3);
	}
	else{
		pll_clk = pcie_pll->cpu_clk;
	}

	do {
		Wr(HHI_PCIE0_PLL_CNTL0, pcie_pll->pll_cntl0);
		Wr(HHI_PCIE0_PLL_CNTL0, pcie_pll->pll_cntl01);
		Wr(HHI_PCIE0_PLL_CNTL1, pcie_pll->pll_cntl1);
		Wr(HHI_PCIE0_PLL_CNTL2, pcie_pll->pll_cntl2);
		Wr(HHI_PCIE0_PLL_CNTL3, pcie_pll->pll_cntl3);
		Wr(HHI_PCIE0_PLL_CNTL4, pcie_pll->pll_cntl4);
		Wr(HHI_PCIE0_PLL_CNTL5, pcie_pll->pll_cntl5);
		Wr(HHI_PCIE0_PLL_CNTL5, pcie_pll->pll_cntl51);
		_udelay(20);
		Wr(HHI_PCIE0_PLL_CNTL4, pcie_pll->pll_cntl41);
		_udelay(10);
		Wr(HHI_PCIE0_PLL_CNTL0, pcie_pll->pll_cntl02);
		Wr(HHI_PCIE0_PLL_CNTL0, pcie_pll->pll_cntl03);
		_udelay(10);
		Wr(HHI_PCIE0_PLL_CNTL2, pcie_pll->pll_cntl21);
		//printf("pcie lock_check: %4d\n", lock_check);
		_udelay(100);

	} while ((!((readl(HHI_PCIE0_PLL_CNTL0)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("PCIE pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("PCIE pll lock OK! - %4d MHz.Div48  - %4d MHz. ", pll_clk, pll_clk / 48);
		/* get pcie_pll_clk */
		clk_msr_val = clk_util_clk_msr(146);
		printf("CLKMSR(146) - %4d MHz ", clk_msr_val);

		if (clk_around(clk_msr_val, pll_clk / 48)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int pcie_pll_test_all(pcie_pll_cfg_t * pcie_pll_cfg) {
	unsigned int i=0;
	int ret=0;

	for (i=0; i<1; i++) {
		ret += pcie_pll_test(&(pcie_pll_cfg->pcie_pll[i]));
	}
	return ret;


}

static int adc_pll_test(adc_pll_set_t * adc_pll) {
	int ret = 0;
	unsigned int pll_clk = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	unsigned int pll_clk_total = 0;
	unsigned int mn = 0;

	if (adc_pll->cpu_clk == 0) {
//		pll_clk = (24 / ((adc_pll->pll_cntl0>>10)&0x1F) * (adc_pll->pll_cntl0&0x1FF)) >> ((adc_pll->pll_cntl0>>16)&0x3);
		mn = adc_pll->pll_cntl0 & 0xFF;
		pll_clk_total = 24 * mn;
		printf("pll_clk_total = %d\n",pll_clk_total);
		if (pll_clk_total == 4800)
		{
			pll_clk = 4800;
		}
		if (pll_clk_total == 3000)
		{
			pll_clk = 3000;
		}
	}
	else{
		pll_clk = adc_pll->cpu_clk;
	}

	do {
		Wr(HHI_ADC_PLL_CNTL, adc_pll->pll_cntl0);
		Wr(HHI_ADC_PLL_CNTL, adc_pll->pll_cntl01);
		Wr(HHI_ADC_PLL_CNTL1, adc_pll->pll_cntl1);
		Wr(HHI_ADC_PLL_CNTL2, adc_pll->pll_cntl2);
		Wr(HHI_ADC_PLL_CNTL3, adc_pll->pll_cntl3);
		Wr(HHI_ADC_PLL_CNTL4, adc_pll->pll_cntl4);
		Wr(HHI_ADC_PLL_CNTL5, adc_pll->pll_cntl5);
		Wr(HHI_ADC_PLL_CNTL6, adc_pll->pll_cntl6);
		Wr(HHI_ADC_PLL_CNTL, adc_pll->pll_cntl02);
		//printf("pcie lock_check: %4d\n", lock_check);
		_udelay(100);
	} while ((!((readl(HHI_ADC_PLL_CNTL)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("ADC pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("ADC pll lock OK! - %4d MHz.Div96  - %4d MHz. ", pll_clk, pll_clk / 96);
		/* get adc_pll_clk */
		clk_msr_val = clk_util_clk_msr(149);
		printf("CLKMSR(149) - %4d MHz ", clk_msr_val);

		if (clk_around(clk_msr_val, pll_clk/96 )) {
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

	for (i=0; i<2; i++) {
		ret += adc_pll_test(&(adc_pll_cfg->adc_pll[i]));
	}
	return ret;
}

#if 0
static int fix_pll_test(fix_pll_set_t * fix_pll) {
	return 0;
}

static int fix_pll_test_all(fix_pll_cfg_t * fix_pll_cfg) {
	return 0;
}
#endif


static int tcon1_pll_test(unsigned int pll_clk)
{
	int ret = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	unsigned int clk_msr_reg = 0;

	/* div_15 */
	clk_msr_reg = readl(HHI_VID_PLL_CLK_DIV);
	clk_msr_reg = clk_msr_reg & (~(0xf<<16));
	clk_msr_reg = clk_msr_reg | (0xa<<16);
	writel(clk_msr_reg,HHI_VID_PLL_CLK_DIV);

	/* tcon_clk enable */
	writel(readl(HHI_LVDS_TX_PHY_CNTL1)&(~(1<<29)),HHI_LVDS_TX_PHY_CNTL1);

	do {
		Wr(0xff63c080, 0x200f04f7);
		_udelay(50);
		Wr(0xff63c080, 0x300e04f7);
		_udelay(50);
		Wr(0xff63c084, 0x10110000);
		_udelay(50);
		Wr(0xff63c088, 0x00001108);
		_udelay(50);
		Wr(0xff63c08c, 0x10058f30);
		_udelay(50);
		Wr(0xff63c37c, 0x010100c0);
		_udelay(50);
		Wr(0xff63c37c, 0x038300c0);
		_udelay(50);
		Wr(0xff63c080, 0x340f04f7);
		_udelay(50);
		Wr(0xff63c080, 0x140f04f7);
		_udelay(50);
		Wr(0xff63c088, 0x00003008);
		_udelay(50);
		//printf("tcon lock_check: %4d\n", lock_check);
	} while ((!((readl(0xff63c080)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("TCON pll model1 lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("TCON pll lock OK! - %4d MHz.Div30  - %4d MHz. ", pll_clk, pll_clk / 30);
		/* get tcon_pll_clk */
		clk_msr_val = clk_util_clk_msr(55);
		printf("CLKMSR(55) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk/30)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}



static int tcon2_pll_test(unsigned int pll_clk)
{
	int ret = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	unsigned int clk_msr_reg = 0;

	/* div_15 */
	clk_msr_reg = readl(HHI_VID_PLL_CLK_DIV);
	clk_msr_reg = clk_msr_reg & (~(0xf<<16));
	clk_msr_reg = clk_msr_reg | (0xa<<16);
	writel(clk_msr_reg,HHI_VID_PLL_CLK_DIV);

	/* tcon enable */
	writel(readl(HHI_LVDS_TX_PHY_CNTL1)&(~(1<<29)),HHI_LVDS_TX_PHY_CNTL1);

	do {
		Wr(0xff63c080, 0x000e047b);
		udelay(50);
		Wr(0xff63c080, 0x200e047b);
		_udelay(50);
		Wr(0xff63c080, 0x300e047b);
		_udelay(50);
		Wr(0xff63c084, 0x10118000);
		_udelay(50);
		Wr(0xff63c088, 0x00003108);
		_udelay(50);
		Wr(0xff63c08c, 0x10051400);
		_udelay(50);
		Wr(0xff63c37c, 0x010100c0);
		_udelay(50);
		Wr(0xff63c37c, 0x038300c0);
		_udelay(50);
		Wr(0xff63c080, 0x340e047b);
		_udelay(50);
		Wr(0xff63c080, 0x140e047b);
		_udelay(50);
		Wr(0xff63c088, 0x00003018);
		_udelay(50);
		Wr(0xff63c088, 0x00003018);
		_udelay(50);
		//printf("tcon lock_check: %4d\n", lock_check);
	} while ((!((readl(0xff63c080)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("TCON pll model3 lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("TCON pll lock OK! - %4d MHz.Div30  - %4d MHz. ", pll_clk, pll_clk / 30);
		/* get tcon_pll_clk */
		clk_msr_val = clk_util_clk_msr(55);
		printf("CLKMSR(55) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, pll_clk/30)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int tcon_pll_test_all(void) {
	unsigned int i=0;
	int ret=0;

	int (*p_con[2])(unsigned int) = {tcon1_pll_test,tcon2_pll_test};
	unsigned int tcon_clk[2] = {5940,2970};
	for (i=0; i<2; i++) {
		_udelay(100);
		ret += p_con[i](tcon_clk[i]);
		_udelay(100);
	}
	return ret;
}

static int hdmitx_pll_test(hdmitx_pll_set_t * hdmitx_pll)
{
	unsigned int pll_clk = 0;
	int ret = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	unsigned int clk_msr_reg = 0;
	unsigned int clk_msr_cntl_reg = 0;

	/* div_1 */
	clk_msr_reg = readl(HHI_VID_PLL_CLK_DIV);
	clk_msr_reg = clk_msr_reg & (~(0xf<<16));
	clk_msr_reg = clk_msr_reg | (0xc<<16);
	writel(clk_msr_reg,HHI_VID_PLL_CLK_DIV);

	/* enable hdmitx clk */
	clk_msr_cntl_reg = readl(HHI_LVDS_TX_PHY_CNTL1);
	clk_msr_cntl_reg = clk_msr_cntl_reg & (~(1<<29));
	clk_msr_cntl_reg = clk_msr_cntl_reg | (1<<29);
	writel(clk_msr_cntl_reg,HHI_LVDS_TX_PHY_CNTL1);

	if (hdmitx_pll->cpu_clk == 0) {
		if (hdmitx_pll->pll_cntl0 == 0x0b3a04f7)
		{
			pll_clk = 5940;
		}
		if (hdmitx_pll->pll_cntl0 == 0x0b3a047b)
		{
			pll_clk = 2970;
		}
	}
	else{
		pll_clk = hdmitx_pll->cpu_clk;
	}

	do {
		Wr(HHI_HDMI_PLL_CNTL0, hdmitx_pll->pll_cntl0);
		Wr(HHI_HDMI_PLL_CNTL0, hdmitx_pll->pll_cntl01);
		Wr(HHI_HDMI_PLL_CNTL1, hdmitx_pll->pll_cntl1);
		Wr(HHI_HDMI_PLL_CNTL2, hdmitx_pll->pll_cntl2);
		Wr(HHI_HDMI_PLL_CNTL3, hdmitx_pll->pll_cntl3);
		Wr(HHI_HDMI_PLL_CNTL4, hdmitx_pll->pll_cntl4);
		Wr(HHI_HDMI_PLL_CNTL5, hdmitx_pll->pll_cntl5);
		Wr(HHI_HDMI_PLL_CNTL6, hdmitx_pll->pll_cntl6);
		Wr(HHI_HDMI_PLL_CNTL0, hdmitx_pll->pll_cntl02);
		_udelay(20);
		//printf("hdmitx lock_check: %4d\n", lock_check);
	} while ((!((readl(HHI_HDMI_PLL_CNTL0)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("HDMITX pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		printf("HDMITX pll lock OK! - %4d MHz.Div128  - %4d MHz. ", pll_clk, pll_clk/128);
		clk_msr_val = clk_util_clk_msr(55);
		printf("CLKMSR(55) - %4d MHz ", clk_msr_val);

		if (clk_around(clk_msr_val, pll_clk/128)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}

	return ret;
}

static int hdmitx_pll_test_all(hdmitx_pll_cfg_t * hdmitx_pll_cfg) {
	unsigned int i = 0;
	int ret=0;

	for (i=0; i<2; i++) {
		ret += hdmitx_pll_test(&(hdmitx_pll_cfg->hdmitx_pll[i]));
	}
	return ret;
}


static int hdmirx1_pll_test(unsigned int pll_clk)
{
	int ret = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	unsigned int clk_div = 0;
	do {
		Wr(0xff63c35c, 0x100347f9);
		_udelay(50);
		Wr(0xff63c348, 0x000904f8);
		_udelay(50);
		Wr(0xff63c348 , 0x200904f8);
		_udelay(50);
		Wr(0xff63c348 , 0x300904f8);
		_udelay(50);
		Wr(0xff63c34c , 0x00000000);
		_udelay(50);
		Wr(0xff63c350, 0x00003118);
		_udelay(50);
		Wr(0xff63c354 , 0x10058f31);
		_udelay(50);
		Wr(0xff63c358, 0x010100c0);
		_udelay(50);
		Wr(0xff63c358, 0x038300c0);
		_udelay(50);
		Wr(0xff63c348, 0x340904f8);
		_udelay(50);
		Wr(0xff63c348, 0x140904f8);
		_udelay(50);
		Wr(0xff63c358, 0x0b8300c0);
		_udelay(50);
		Wr(0xff63c350, 0x00003018);
		_udelay(50);
		//printf("hdmirx lock_check: %4d\n", lock_check);
	}while ((!((readl(0xff63c348)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("HDMIRX pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		clk_div = clok_div_enter(pll_clk , 40);
		printf("HDMIRX pll lock OK! - %4d MHz.Div40  - %4d MHz. ", pll_clk, clk_div);
		/* get hdmirx_apll_clk_out_div */
		clk_msr_val = clk_util_clk_msr(29);
		printf("CLKMSR(29) - %4d MHz ", clk_msr_val);

		if (clk_around(clk_msr_val, clk_div)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int hdmirx2_pll_test(unsigned int pll_clk)
{
	int ret = 0;
	unsigned int lock_check = PLL_LOCK_CHECK_MAX;
	unsigned int clk_msr_val = 0;
	unsigned int clk_div = 0;
	do {
	   Wr(0xff63c35c, 0x100347f9);
		_udelay(20);
		Wr(0xff63c348, 0x0009047b);
		_udelay(20);
		Wr(0xff63c348, 0x2009047b);
		_udelay(20);
		Wr(0xff63c348, 0x3009047b);
		_udelay(20);
		Wr(0xff63c34c, 0x00000000);
		_udelay(20);
		Wr(0xff63c350, 0x00003118);
		_udelay(20);
		Wr(0xff63c354, 0x10058f30);
		_udelay(20);
		Wr(0xff63c358, 0x010100c0);
		_udelay(20);
		Wr(0xff63c358, 0x038300c0);
		_udelay(20);
		Wr(0xff63c348, 0x3409047b);
		_udelay(20);
		Wr(0xff63c348, 0x1409047b);
		_udelay(20);
		Wr(0xff63c350 , 0x00003018);
		_udelay(20);
		Wr(0xff63c358 , 0x0b8300c0);
		_udelay(20);
		Wr(0xff63c350 , 0x00003028);
		_udelay(20);
		//printf("tcon lock_check: %4d\n", lock_check);

	} while ((!((readl(0xff63c348)>>31)&0x1)) && --lock_check);
	if (0 == lock_check) {
		printf("HDMIRX pll lock Failed! - %4d MHz\n", pll_clk);
		ret = RET_PLL_LOCK_FAIL;
	} else {
		clk_div = clok_div_enter(pll_clk , 40);
		printf("HDMIRX pll lock OK! - %4d MHz.Div40  - %4d MHz. ", pll_clk, clk_div);
		/* get hdmirx_pll_clk apll_audio  */
		clk_msr_val = clk_util_clk_msr(29);
		printf("CLKMSR(29) - %4d MHz ", clk_msr_val);
		if (clk_around(clk_msr_val, clk_div)) {
			printf(": Match\n");
		} else {
			printf(": MisMatch\n");
			ret = RET_CLK_NOT_MATCH;
		}
	}
	return ret;
}

static int hdmirx_pll_test_all(void) {
	unsigned int i=0;
	int ret=0;

	int (*p_hdmirx[2])(unsigned int) = {hdmirx1_pll_test,hdmirx2_pll_test};

	unsigned int hdmirx_clk[2] = {5952,2952};
	for (i=0; i<2; i++) {
		_udelay(100);
		ret += p_hdmirx[i](hdmirx_clk[i]);
		_udelay(100);
	}
	return ret;
}

static int pll_test_all(unsigned char * pll_list) {
	int ret = 0;
	unsigned char i=0;
	for (i=0; i<PLL_ENUM; i++) {
		switch (pll_list[i]) {
			case PLL_SYS:
				ret = sys_pll_test_all(&sys_pll_cfg1);
				pll_report(ret, STR_PLL_TEST_SYS);
				break;
#if 0
			case PLL_DDR:
				ret = ddr_pll_test();
				pll_report(ret, STR_PLL_TEST_DDR);
				break;
#endif
			case PLL_GP0:
				ret = gp0_pll_test_all(&gp0_pll_cfg);
				pll_report(ret, STR_PLL_TEST_GP0);
				break;
			case PLL_HIFI:
				ret = hifi_pll_test_all(&hifi_pll_cfg);
				pll_report(ret, STR_PLL_TEST_HIFI);
				break;
			case PLL_PCIE:
				ret = pcie_pll_test_all(&pcie_pll_cfg);
				pll_report(ret, STR_PLL_TEST_PCIE);
				break;
			case PLL_ADC:
				ret = adc_pll_test_all(&adc_pll_cfg);
				pll_report(ret, STR_PLL_TEST_ADC);
				break;
#if 0
			case PLL_FIX:
				ret = fix_pll_test_all(&fix_pll_cfg);
				pll_report(ret, STR_PLL_TEST_FIX);
				break;
#endif
			case PLL_HDMITX:
				ret = hdmitx_pll_test_all(&hdmitx_pll_cfg);
				pll_report(ret, STR_PLL_TEST_HDMITX);
				break;
			case PLL_HDMIRX:
				ret = hdmirx_pll_test_all();
				pll_report(ret, STR_PLL_TEST_HDMIRX);
				break;
			case PLL_TCON:
				ret = tcon_pll_test_all();
				pll_report(ret, STR_PLL_TEST_TCON);
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
	unsigned int tcon_parameter1 = 0;

	sys_pll_set_t1 sys_pll_set = {0};
	gp0_pll_set_t gp0_pll_set = {0};
	hifi_pll_set_t hifi_pll_set = {0};
	pcie_pll_set_t pcie_pll_set = {0};
	adc_pll_set_t adc_pll_set = {0};
//	fix_pll_set_t fix_pll_set = {0};
	hdmitx_pll_set_t hdmitx_pll_set = {0};

	unsigned char plls[PLL_ENUM] = {
		PLL_SYS,
		PLL_GP0,
		PLL_HIFI,
		PLL_PCIE,
		PLL_ADC,
//		PLL_FIX,
		PLL_HDMITX,
		PLL_HDMIRX,
		PLL_TCON,
	};

	if (0 == strcmp(STR_PLL_TEST_ALL, argv[1])) {
		printf("Test all plls\n");
		pll_test_all(plls);
	}
	else if(0 == strcmp(STR_PLL_TEST_SYS, argv[1])) {
		if (argc == 2) {
			ret = sys_pll_test_all(&sys_pll_cfg1);
			pll_report(ret, STR_PLL_TEST_SYS);
		}
		else if (argc != 11){
			printf("%s pll test: args error\n", STR_PLL_TEST_SYS);
			return -1;
		}
		else {
			sys_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			sys_pll_set.pll_cntl01 = simple_strtoul(argv[3], NULL, 16);
			sys_pll_set.pll_cntl1 = simple_strtoul(argv[4], NULL, 16);
			sys_pll_set.pll_cntl2 = simple_strtoul(argv[5], NULL, 16);
			sys_pll_set.pll_cntl3 = simple_strtoul(argv[6], NULL, 16);
			sys_pll_set.pll_cntl4 = simple_strtoul(argv[7], NULL, 16);
			sys_pll_set.pll_cntl5 = simple_strtoul(argv[8], NULL, 16);
			sys_pll_set.pll_cntl6 = simple_strtoul(argv[9], NULL, 16);
			sys_pll_set.pll_cntl02 = simple_strtoul(argv[10], NULL, 16);
			ret = sys_pll_test(&sys_pll_set);
			pll_report(ret, STR_PLL_TEST_SYS);
		}
	}
	else if (0 == strcmp(STR_PLL_TEST_GP0, argv[1])) {
		if (argc == 2) {
			ret = gp0_pll_test_all(&gp0_pll_cfg);
			pll_report(ret, STR_PLL_TEST_GP0);
		}
		else if (argc != 11){
			printf("%s pll test: args error\n", STR_PLL_TEST_GP0);
			return -1;
		}
		else {
			gp0_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			gp0_pll_set.pll_cntl01 = simple_strtoul(argv[3], NULL, 16);
			gp0_pll_set.pll_cntl1 = simple_strtoul(argv[4], NULL, 16);
			gp0_pll_set.pll_cntl2 = simple_strtoul(argv[5], NULL, 16);
			gp0_pll_set.pll_cntl3 = simple_strtoul(argv[6], NULL, 16);
			gp0_pll_set.pll_cntl4 = simple_strtoul(argv[7], NULL, 16);
			gp0_pll_set.pll_cntl5 = simple_strtoul(argv[8], NULL, 16);
			gp0_pll_set.pll_cntl6 = simple_strtoul(argv[9], NULL, 16);
			gp0_pll_set.pll_cntl02 = simple_strtoul(argv[10], NULL, 16);
			ret = gp0_pll_test(&gp0_pll_set);
			pll_report(ret, STR_PLL_TEST_GP0);
		}
	}
	else if (0 == strcmp(STR_PLL_TEST_DDR, argv[1])) {
		printf("%s pll not support now\n", STR_PLL_TEST_DDR);
		return -1;
	}
	else if (0 == strcmp(STR_PLL_TEST_HIFI, argv[1])) {
		if (argc == 2) {
			ret = hifi_pll_test_all(&hifi_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HIFI);
		} else if (argc != 11){
			printf("%s pll test: args error\n", STR_PLL_TEST_HIFI);
			return -1;
		} else {
			hifi_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			hifi_pll_set.pll_cntl01 = simple_strtoul(argv[3], NULL, 16);
			hifi_pll_set.pll_cntl1 = simple_strtoul(argv[4], NULL, 16);
			hifi_pll_set.pll_cntl2 = simple_strtoul(argv[5], NULL, 16);
			hifi_pll_set.pll_cntl3 = simple_strtoul(argv[6], NULL, 16);
			hifi_pll_set.pll_cntl4 = simple_strtoul(argv[7], NULL, 16);
			hifi_pll_set.pll_cntl5 = simple_strtoul(argv[8], NULL, 16);
			hifi_pll_set.pll_cntl6 = simple_strtoul(argv[9], NULL, 16);
			hifi_pll_set.pll_cntl02 = simple_strtoul(argv[10], NULL, 16);
			ret = hifi_pll_test(&hifi_pll_set);
			pll_report(ret, STR_PLL_TEST_HIFI);
		}
	}
	else if (0 == strcmp(STR_PLL_TEST_PCIE, argv[1])) {
		if (argc == 2) {
			ret = pcie_pll_test_all(&pcie_pll_cfg);
			pll_report(ret, STR_PLL_TEST_PCIE);
		} else if (argc != 14){
			printf("%s pll test: args error\n", STR_PLL_TEST_PCIE);
			return -1;
		} else {
			pcie_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			pcie_pll_set.pll_cntl01 = simple_strtoul(argv[3], NULL, 16);
			pcie_pll_set.pll_cntl1 = simple_strtoul(argv[4], NULL, 16);
			pcie_pll_set.pll_cntl2 = simple_strtoul(argv[5], NULL, 16);
			pcie_pll_set.pll_cntl3 = simple_strtoul(argv[6], NULL, 16);
			pcie_pll_set.pll_cntl4 = simple_strtoul(argv[7], NULL, 16);
			pcie_pll_set.pll_cntl5 = simple_strtoul(argv[8], NULL, 16);
			pcie_pll_set.pll_cntl51 = simple_strtoul(argv[9], NULL, 16);
			pcie_pll_set.pll_cntl41 = simple_strtoul(argv[10], NULL, 16);
			pcie_pll_set.pll_cntl02 = simple_strtoul(argv[11], NULL, 16);
			pcie_pll_set.pll_cntl03 = simple_strtoul(argv[12], NULL, 16);
			pcie_pll_set.pll_cntl21 = simple_strtoul(argv[13], NULL, 16);
			ret = pcie_pll_test(&pcie_pll_set);
			pll_report(ret, STR_PLL_TEST_PCIE);
		}
	}
	else if (0 == strcmp(STR_PLL_TEST_ADC, argv[1])) {
		if (argc == 2) {
			ret = adc_pll_test_all(&adc_pll_cfg);
			pll_report(ret, STR_PLL_TEST_ADC);
		} else if (argc != 11){
			printf("%s pll test: args error\n", STR_PLL_TEST_ADC);
			return -1;
		} else {
			adc_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			adc_pll_set.pll_cntl01 = simple_strtoul(argv[3], NULL, 16);
			adc_pll_set.pll_cntl1 = simple_strtoul(argv[4], NULL, 16);
			adc_pll_set.pll_cntl2 = simple_strtoul(argv[5], NULL, 16);
			adc_pll_set.pll_cntl3 = simple_strtoul(argv[6], NULL, 16);
			adc_pll_set.pll_cntl4 = simple_strtoul(argv[7], NULL, 16);
			adc_pll_set.pll_cntl5 = simple_strtoul(argv[8], NULL, 16);
			adc_pll_set.pll_cntl6 = simple_strtoul(argv[9], NULL, 16);
			adc_pll_set.pll_cntl02 = simple_strtoul(argv[10], NULL, 16);
			ret = adc_pll_test(&adc_pll_set);
			pll_report(ret, STR_PLL_TEST_ADC);
		}
	}
#if 0
	else if (0 == strcmp(STR_PLL_TEST_FIX, argv[1])) {
		if (argc == 2) {
			ret = fix_pll_test_all(&fix_pll_cfg);
			pll_report(ret, STR_PLL_TEST_FIX);
		} else if (argc != 11){
			printf("%s pll test: args error\n", STR_PLL_TEST_FIX);
			return -1;
		} else {
			fix_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			fix_pll_set.pll_cntl01 = simple_strtoul(argv[3], NULL, 16);
			fix_pll_set.pll_cntl1 = simple_strtoul(argv[4], NULL, 16);
			fix_pll_set.pll_cntl2 = simple_strtoul(argv[5], NULL, 16);
			fix_pll_set.pll_cntl3 = simple_strtoul(argv[6], NULL, 16);
			fix_pll_set.pll_cntl4 = simple_strtoul(argv[7], NULL, 16);
			fix_pll_set.pll_cntl5 = simple_strtoul(argv[8], NULL, 16);
			fix_pll_set.pll_cntl6 = simple_strtoul(argv[8], NULL, 16);
			fix_pll_set.pll_cntl02 = simple_strtoul(argv[9], NULL, 16);
			ret = fix_pll_test(&fix_pll_set);
			pll_report(ret, STR_PLL_TEST_FIX);
		}
	}
#endif

	else if (0 == strcmp(STR_PLL_TEST_HDMITX, argv[1])) {
		if (argc == 2) {
			ret = hdmitx_pll_test_all(&hdmitx_pll_cfg);
			pll_report(ret, STR_PLL_TEST_HDMITX);
		} else if (argc != 11){
			printf("%s pll test: args error\n", STR_PLL_TEST_HDMITX);
			return -1;
		} else {
			hdmitx_pll_set.pll_cntl0 = simple_strtoul(argv[2], NULL, 16);
			hdmitx_pll_set.pll_cntl01 = simple_strtoul(argv[3], NULL, 16);
			hdmitx_pll_set.pll_cntl1 = simple_strtoul(argv[4], NULL, 16);
			hdmitx_pll_set.pll_cntl2 = simple_strtoul(argv[5], NULL, 16);
			hdmitx_pll_set.pll_cntl3 = simple_strtoul(argv[6], NULL, 16);
			hdmitx_pll_set.pll_cntl4 = simple_strtoul(argv[7], NULL, 16);
			hdmitx_pll_set.pll_cntl5 = simple_strtoul(argv[8], NULL, 16);
			hdmitx_pll_set.pll_cntl6 = simple_strtoul(argv[9], NULL, 16);
			hdmitx_pll_set.pll_cntl02 = simple_strtoul(argv[10], NULL, 16);
			ret = hdmitx_pll_test(&hdmitx_pll_set);
			pll_report(ret, STR_PLL_TEST_HDMITX);
		}
	}

	else if (0 == strcmp(STR_PLL_TEST_HDMIRX, argv[1])) {
		if (argc == 2) {
			ret = hdmirx_pll_test_all();
			pll_report(ret, STR_PLL_TEST_HDMIRX);
		} else if (argc !=16 && argc !=15){
			printf("%s pll test: args error\n", STR_PLL_TEST_HDMIRX);
			return -1;
		} else {
			if (argc == 15)
			{
				ret = hdmirx1_pll_test(5952);
				pll_report(ret, STR_PLL_TEST_HDMIRX);
			}
			if (argc == 16)
			{
				ret = hdmirx2_pll_test(2952);
				pll_report(ret, STR_PLL_TEST_HDMIRX);
			}

		}
	}

	else if (0 == strcmp(STR_PLL_TEST_TCON, argv[1])) {
		if (argc == 2) {
			ret = tcon_pll_test_all();
			pll_report(ret, STR_PLL_TEST_TCON);
		} else if (argc != 14){
			printf("%s pll test: args error\n", STR_PLL_TEST_TCON);
			return -1;
		} else {
			tcon_parameter1 = simple_strtoul(argv[2], NULL, 16);
			if (tcon_parameter1 == 0x000e04f7)
			{
				ret = tcon1_pll_test(5940);
				pll_report(ret, STR_PLL_TEST_TCON);
				return 0;
			}
			if (tcon_parameter1 == 0x000e047b)
			{
				ret = tcon2_pll_test(2970);
				pll_report(ret, STR_PLL_TEST_TCON);
				return 0;
			}
		}
	}
	return 0;
}




/*

1. sys:
	6000MHz:  plltest sys 0x080304fa 0x380304fa 0x0 0x0 0x48681c00 0x88770290 0x39272000 0x56540000 0x180304fa
	3000MHz:  plltest sys 0x0803047d 0x3803047d 0x0 0x0 0x48681c00 0x88770290 0x39272000 0x56540000 0x1803047d
	all:      plltest sys

2. gp0:
	6000MHZ:  plltest gp0 0x080304fa 0x380304fa 0x0 0x0 0x48681c00 0x88770290 0x39272000 0x56540000 0x180304fa
	3000MHZ:  plltest gp0 0x0803047d 0x3803047d 0x00006aab 0x0 0x6a295c00 0x65771290 0x39272000 0x54540000 0x1803047d
	all:	  plltest gp0

3. hifi:
	6005MHZ:  plltest hifi 0x080304fa 0x380304fa 0x00006aab 0x0 0x6a285c00 0x65771290 0x39272000 0x56540000 0x180304fa
	3000MHX:  plltest hifi 0x0803047d 0x3803047d 0x00006aab 0x0 0x6a295c00 0x65771290 0x39272000 0x54540000 0x1803047d
	plltest hifi

4. pcie:
	4800MHz:  plltest pcie 0x200c04c8 0x300c04c8 0x30000000 0x00001100 0x10058e00 0x000100c0 0x68000048 0x68000068 0x008100c0 0x340c04c8 0x140c04c8 0x00001000
	all:      plltest pcie

5. adc :
	4800MHZ:  plltest adc 0x012004c8 0x312004c8 0x03000000 0xe1800000 0x48681c00 0x88770290 0x39272000 0x56540000 0x112004c8
	3000MHZ:  plltest adc 0x0120047d 0x3120047d 0x03000000 0xe1800000 0x48681c00 0x88770290 0x39272000 0x56540000 0x1120047d
	all:      plltest adc

6. hdmirx:
	2952MHZ:   plltest hdmirx 0x100347f9 0x0009047b 0x2009047b 0x3009047b 0x00000000 0x00003118 0x10058f30 0x010100c0 0x038300c0 0x3409047b 0x1409047b 0x00003018 0x0b8300c0 0x00003028
	5952MHZ:   plltest hdmirx 0x100347f9 0x000904f8 0x200904f8 0x300904f8 0x00000000 0x00003118 0x10058f31 0x010100c0 0x038300c0 0x340904f8 0x140904f8 0x0b8300c0 0x00003018

7. all
	plltest all

8. hdmitx
	5940MHX:     plltest hdmitx 0x0b3a04f7 0x3b3a04f7 0x00010000 0x0 0x6a28dc00 0x65771290 0x39272000 0x56540000 0x1b3a04f7
	2970MHZ:	 plltest hdmitx 0x0b3a047b 0x3b3a047b 0x00018000 0x0 0x6a29dc00 0x65771290 0x39272000 0x54540000 0x1b3a047b

9. tcon:
	5940MHZ:  plltest tcon 0x000e04f7 0x200e04f7 0x300e04f7 0x10110000 0x00003118 0x10051400 0x010100c0 0x038300c0 0x340e04f7 0x140e04f7 0x00003018 0x0b8300c0
	2970MHZ:  plltest tcon 0x000e047b 0x200e047b 0x300e047b 0x10118000 0x00003118 0x10051400 0x010100c0 0x038300c0 0x340e047b 0x140e047b 0x00003018 0x00003018
	all:      plltest tcon

*/








































































