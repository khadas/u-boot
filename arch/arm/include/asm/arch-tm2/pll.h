
#ifndef __PLL_H
#define __PLL_H

#define PLL_TEST_SYS_TOTAL		2
#define PLL_TEST_GP0_TOTAL      2
#define PLL_TEST_HIFI_TOTAL     2
#define PLL_TEST_PCIE_TOTAL     1
#define PLL_TEST_ADC_TOTAL      2
#define PLL_TEST_FIX_TOTAL      1
#define PLL_TEST_HDMITX_TOTAL   2
#define PLL_TEST_HDMITX_TOTAL   2

typedef struct sys_pll_set_s {
	unsigned int cpu_clk;
	unsigned int pll_cntl;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
	unsigned int pll_cntl4;
	unsigned int pll_cntl5;
	unsigned int pll_cntl6;
}sys_pll_set_t;

typedef struct sys_pll_cfg_s {
	sys_pll_set_t sys_pll[PLL_TEST_SYS_TOTAL];
}sys_pll_cfg_t;

/* sys_pll test */
typedef struct sys_pll_set_s1 {
	unsigned int cpu_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl01;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
	unsigned int pll_cntl4;
	unsigned int pll_cntl5;
	unsigned int pll_cntl6;
	unsigned int pll_cntl02;
}sys_pll_set_t1;

typedef struct sys_pll_cfg_s1 {
	sys_pll_set_t1 sys_pll[PLL_TEST_SYS_TOTAL];
}sys_pll_cfg_t1;


typedef struct gp0_pll_set_s {
	unsigned int cpu_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl01;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
	unsigned int pll_cntl4;
	unsigned int pll_cntl5;
	unsigned int pll_cntl6;
	unsigned int pll_cntl02;
}gp0_pll_set_t;

typedef struct gp0_pll_cfg_s {
	gp0_pll_set_t gp0_pll[PLL_TEST_GP0_TOTAL];
}gp0_pll_cfg_t;

typedef struct hifi_pll_set_s {
	unsigned int cpu_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl01;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
	unsigned int pll_cntl4;
	unsigned int pll_cntl5;
	unsigned int pll_cntl6;
	unsigned int pll_cntl02;
}hifi_pll_set_t;

typedef struct hifi_pll_cfg_s {
	hifi_pll_set_t hifi_pll[PLL_TEST_HIFI_TOTAL];
}hifi_pll_cfg_t;

typedef struct pcie_pll_set_s {
	unsigned int cpu_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl01;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
	unsigned int pll_cntl4;
	unsigned int pll_cntl5;
	unsigned int pll_cntl51;
	unsigned int pll_cntl41;
	unsigned int pll_cntl02;
	unsigned int pll_cntl03;
	unsigned int pll_cntl21;
}pcie_pll_set_t;

typedef struct pcie_pll_cfg_s {
	pcie_pll_set_t pcie_pll[PLL_TEST_PCIE_TOTAL];
}pcie_pll_cfg_t;

typedef struct adc_pll_set_s {
	unsigned int cpu_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl01;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
	unsigned int pll_cntl4;
	unsigned int pll_cntl5;
	unsigned int pll_cntl6;
	unsigned int pll_cntl02;
}adc_pll_set_t;

typedef struct adc_pll_cfg_s {
	adc_pll_set_t adc_pll[PLL_TEST_ADC_TOTAL];
}adc_pll_cfg_t;

typedef struct fix_pll_set_s {
	unsigned int cpu_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl01;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
	unsigned int pll_cntl4;
	unsigned int pll_cntl5;
	unsigned int pll_cntl6;
	unsigned int pll_cntl02;
}fix_pll_set_t;

typedef struct fix_pll_cfg_s {
	fix_pll_set_t fix_pll[PLL_TEST_FIX_TOTAL];
}fix_pll_cfg_t;


typedef struct hdmitx_pll_set_s {
	unsigned int cpu_clk;
	unsigned int pll_cntl0;
	unsigned int pll_cntl01;
	unsigned int pll_cntl1;
	unsigned int pll_cntl2;
	unsigned int pll_cntl3;
	unsigned int pll_cntl4;
	unsigned int pll_cntl5;
	unsigned int pll_cntl6;
	unsigned int pll_cntl02;
}hdmitx_pll_set_t;

typedef struct hdmitx_pll_cfg_s {
	hdmitx_pll_set_t hdmitx_pll[PLL_TEST_HDMITX_TOTAL];
}hdmitx_pll_cfg_t;





enum pll_enum {
	PLL_SYS = 0,
	PLL_GP0,
	PLL_HIFI,
	PLL_PCIE,
	PLL_ADC,
//	PLL_FIX,
	PLL_HDMITX,
	PLL_TCON,
	PLL_HDMIRX,
	PLL_ENUM,
};

int pll_test(int argc, char * const argv[]);

#endif /* __PLL_H */