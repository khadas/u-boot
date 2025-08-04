/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2025 Rockchip Electronics Co. Ltd.
 * Author: Elaine Zhang <zhangqing@rock-chips.com>
 */

#ifndef _ASM_ARCH_CRU_RV1126B_H
#define _ASM_ARCH_CRU_RV1126B_H

#include <common.h>

#define MHz		1000000
#define KHz		1000
#define OSC_HZ		(24 * MHz)
#define RC_OSC_HZ	(125 * MHz)

#define GPLL_HZ		(1188 * MHz)
#define AUPLL_HZ	(983040000)
#define CPLL_HZ		(1000 * MHz)

/* RV1126B pll id */
enum rv1126b_pll_id {
	GPLL,
	AUPLL,
	CPLL,
	PLL_COUNT,
};

struct rv1126b_clk_info {
	unsigned long id;
	char *name;
	bool is_cru;
};

struct rv1126b_clk_priv {
	struct rv1126b_cru *cru;
	struct rv1126b_grf *grf;
	ulong gpll_hz;
	ulong aupll_hz;
	ulong cpll_hz;
	ulong armclk_hz;
	ulong armclk_enter_hz;
	ulong armclk_init_hz;
	bool sync_kernel;
	bool set_armclk_rate;
};

struct rv1126b_grf_clk_priv {
	struct rv1126b_grf *grf;
};

struct rv1126b_pll {
	unsigned int con0;
	unsigned int con1;
	unsigned int con2;
	unsigned int con3;
	unsigned int con4;
	unsigned int reserved0[3];
};

struct rv1126b_cru {
	struct rv1126b_pll pll[2];
	unsigned int reserved0[176];
	unsigned int clksel_con[71];
	unsigned int reserved1[249];
	unsigned int clkgate_con[16];
	unsigned int reserved2[112];
	unsigned int softrst_con[16];
	unsigned int reserved3[112];
	unsigned int glb_cnt_th;
	unsigned int glb_rst_st;
	unsigned int glb_srst_fst;
	unsigned int glb_srst_snd;
	unsigned int glb_rst_con[3];
	unsigned int reserved4[41];
	unsigned int clk_cm_frac0_div_h;
	unsigned int clk_cm_frac1_div_h;
	unsigned int clk_cm_frac2_div_h;
	unsigned int clk_uart_frac0_div_h;
	unsigned int clk_uart_frac1_div_h;
	unsigned int clk_audio_frac0_div_h;
	unsigned int clk_audio_frac1_div_h;
	unsigned int reserved5[15753];
	unsigned int bus_clksel_con[4];
	unsigned int reserved6[316];
	unsigned int bus_clkgate_con[7];
	unsigned int reserved7[121];
	unsigned int bus_softrst_con[8];
	unsigned int reserved8[15928];
	unsigned int peri_clksel_con[2];
	unsigned int reserved9[318];
	unsigned int peri_clkgate_con[2];
	unsigned int reserved10[126];
	unsigned int peri_softrst_con[2];
	unsigned int reserved11[15934];
	unsigned int core_clksel_con[3];
	unsigned int reserved12[317];
	unsigned int core_clkgate_con[2];
	unsigned int reserved13[126];
	unsigned int core_softrst_con[2];
	unsigned int reserved14[15934];
	unsigned int pmu_clksel_con[9];
	unsigned int reserved15[311];
	unsigned int pmu_clkgate_con[4];
	unsigned int reserved16[124];
	unsigned int pmu_softrst_con[4];
	unsigned int reserved17[15932];
	unsigned int pmu1_clksel_con[2];
	unsigned int reserved18[318];
	unsigned int pmu1_clkgate_con[2];
	unsigned int reserved19[126];
	unsigned int pmu1_softrst_con[2];
	unsigned int reserved20[32318];
	unsigned int vi_clksel_con[1];
	unsigned int reserved21[319];
	unsigned int vi_clkgate_con[5];
	unsigned int reserved22[123];
	unsigned int vi_softrst_con[4];
};

check_member(rv1126b_cru, clksel_con[0], 0x300);
check_member(rv1126b_cru, clkgate_con[0], 0x800);
check_member(rv1126b_cru, softrst_con[0], 0xa00);
check_member(rv1126b_cru, clk_cm_frac0_div_h, 0xcc0);
check_member(rv1126b_cru, bus_clksel_con[0], 0x10300);
check_member(rv1126b_cru, bus_clkgate_con[0], 0x10800);
check_member(rv1126b_cru, bus_softrst_con[0], 0x10a00);
check_member(rv1126b_cru, peri_clksel_con[0], 0x20300);
check_member(rv1126b_cru, peri_clkgate_con[0], 0x20800);
check_member(rv1126b_cru, peri_softrst_con[0], 0x20a00);
check_member(rv1126b_cru, core_clksel_con[0], 0x30300);
check_member(rv1126b_cru, core_clkgate_con[0], 0x30800);
check_member(rv1126b_cru, core_softrst_con[0], 0x30a00);
check_member(rv1126b_cru, pmu_clksel_con[0], 0x40300);
check_member(rv1126b_cru, pmu_clkgate_con[0], 0x40800);
check_member(rv1126b_cru, pmu_softrst_con[0], 0x40a00);
check_member(rv1126b_cru, pmu1_clksel_con[0], 0x50300);
check_member(rv1126b_cru, pmu1_clkgate_con[0], 0x50800);
check_member(rv1126b_cru, pmu1_softrst_con[0], 0x50a00);
check_member(rv1126b_cru, vi_clksel_con[0], 0x70300);
check_member(rv1126b_cru, vi_clkgate_con[0], 0x70800);
check_member(rv1126b_cru, vi_softrst_con[0], 0x70a00);

struct pll_rate_table {
	unsigned long rate;
	unsigned int fbdiv;
	unsigned int postdiv1;
	unsigned int refdiv;
	unsigned int postdiv2;
	unsigned int dsmpd;
	unsigned int frac;
};

#define RV1126B_CRU_BASE		0x20000000
#define RV1126B_TOPCRU_BASE		0x0
#define RV1126B_BUSCRU_BASE		0x10000
#define RV1126B_PERICRU_BASE		0x20000
#define RV1126B_CORECRU_BASE		0x30000
#define RV1126B_PMUCRU_BASE		0x40000
#define RV1126B_PMU1CRU_BASE		0x50000
#define RV1126B_DDRCRU_BASE		0x60000
#define RV1126B_SUBDDRCRU_BASE		0x68000
#define RV1126B_VICRU_BASE		0x70000
#define RV1126B_VEPUCRU_BASE		0x80000
#define RV1126B_NPUCRU_BASE		0x90000
#define RV1126B_VDOCRU_BASE		0xA0000
#define RV1126B_VCPCRU_BASE		0xB0000
#define RV1126B_SBUSCRU_BASE		0x200000

#define RV1126B_PLL_CON(x)		((x) * 0x4 + RV1126B_TOPCRU_BASE)
#define RV1126B_MODE_CON		(0x280 + RV1126B_TOPCRU_BASE)
#define RV1126B_CLKSEL_CON(x)		((x) * 0x4 + 0x300 + RV1126B_TOPCRU_BASE)
#define RV1126B_PERIPLL_CON(x)		((x) * 0x4 + RV1126B_PERICRU_BASE)
#define RV1126B_SUBDDRPLL_CON(x)	((x) * 0x4 + RV1126B_SUBDDRCRU_BASE)
#define RV1126B_SBUSCLKGATE_CON(x)	((x) * 0x4 + 0x800 + RV1126B_SBUSCRU_BASE)

enum {
	/* CRU_CLK_SEL10_CON */
	CLK_AUDIO_FRAC1_SRC_SEL_SHIFT		= 12,
	CLK_AUDIO_FRAC1_SRC_SEL_MASK		= 0x3 << CLK_AUDIO_FRAC1_SRC_SEL_SHIFT,
	CLK_AUDIO_FRAC0_SRC_SEL_SHIFT		= 10,
	CLK_AUDIO_FRAC0_SRC_SEL_MASK		= 0x3 << CLK_AUDIO_FRAC0_SRC_SEL_SHIFT,
	CLK_UART_FRAC1_SRC_SEL_SHIFT		= 8,
	CLK_UART_FRAC1_SRC_SEL_MASK		= 0x3 << CLK_UART_FRAC1_SRC_SEL_SHIFT,
	CLK_UART_FRAC0_SRC_SEL_SHIFT		= 6,
	CLK_UART_FRAC0_SRC_SEL_MASK		= 0x3 << CLK_UART_FRAC0_SRC_SEL_SHIFT,
	CLK_CM_FRAC2_SRC_SEL_SHIFT		= 4,
	CLK_CM_FRAC2_SRC_SEL_MASK		= 0x3 << CLK_CM_FRAC2_SRC_SEL_SHIFT,
	CLK_CM_FRAC1_SRC_SEL_SHIFT		= 2,
	CLK_CM_FRAC1_SRC_SEL_MASK		= 0x3 << CLK_CM_FRAC1_SRC_SEL_SHIFT,
	CLK_CM_FRAC0_SRC_SEL_SHIFT		= 0,
	CLK_CM_FRAC0_SRC_SEL_MASK		= 0x3 << CLK_CM_FRAC0_SRC_SEL_SHIFT,
	CLK_FRAC_SRC_SEL_24M			= 0,
	CLK_FRAC_SRC_SEL_GPLL,
	CLK_FRAC_SRC_SEL_AUPLL,
	CLK_FRAC_SRC_SEL_CPLL,

	/* CRU_CLK_SEL12_CON */
	SCLK_UART1_SEL_SHIFT			= 13,
	SCLK_UART1_SEL_MASK			= 0x7 << SCLK_UART1_SEL_SHIFT,
	SCLK_UART1_DIV_SHIFT			= 8,
	SCLK_UART1_DIV_MASK			= 0x1f << SCLK_UART1_DIV_SHIFT,
	SCLK_UART0_SRC_SEL_SHIFT		= 5,
	SCLK_UART0_SRC_SEL_MASK			= 0x7 << SCLK_UART0_SRC_SEL_SHIFT,
	SCLK_UART_SEL_OSC			= 0,
	SCLK_UART_SEL_CM_FRAC0,
	SCLK_UART_SEL_CM_FRAC1,
	SCLK_UART_SEL_CM_FRAC2,
	SCLK_UART_SEL_UART_FRAC0,
	SCLK_UART_SEL_UART_FRAC1,
	SCLK_UART0_SRC_DIV_SHIFT		= 0,
	SCLK_UART0_SRC_DIV_MASK			= 0x1f << SCLK_UART0_SRC_DIV_SHIFT,

	/* CRU_CLK_SEL13_CON */
	SCLK_UART3_SEL_SHIFT			= 13,
	SCLK_UART3_SEL_MASK			= 0x7 << SCLK_UART3_SEL_SHIFT,
	SCLK_UART3_DIV_SHIFT			= 8,
	SCLK_UART3_DIV_MASK			= 0x1f << SCLK_UART3_DIV_SHIFT,
	SCLK_UART2_SEL_SHIFT			= 5,
	SCLK_UART2_SEL_MASK			= 0x7 << SCLK_UART2_SEL_SHIFT,
	SCLK_UART2_DIV_SHIFT			= 0,
	SCLK_UART2_DIV_MASK			= 0x1f << SCLK_UART2_DIV_SHIFT,

	/* CRU_CLK_SEL14_CON */
	SCLK_UART5_SEL_SHIFT			= 13,
	SCLK_UART5_SEL_MASK			= 0x7 << SCLK_UART5_SEL_SHIFT,
	SCLK_UART5_DIV_SHIFT			= 8,
	SCLK_UART5_DIV_MASK			= 0x1f << SCLK_UART5_DIV_SHIFT,
	SCLK_UART4_SEL_SHIFT			= 5,
	SCLK_UART4_SEL_MASK			= 0x7 << SCLK_UART4_SEL_SHIFT,
	SCLK_UART4_DIV_SHIFT			= 0,
	SCLK_UART4_DIV_MASK			= 0x1f << SCLK_UART4_DIV_SHIFT,

	/* CRU_CLK_SEL15_CON */
	SCLK_UART7_SEL_SHIFT			= 13,
	SCLK_UART7_SEL_MASK			= 0x7 << SCLK_UART7_SEL_SHIFT,
	SCLK_UART7_DIV_SHIFT			= 8,
	SCLK_UART7_DIV_MASK			= 0x1f << SCLK_UART7_DIV_SHIFT,
	SCLK_UART6_SEL_SHIFT			= 5,
	SCLK_UART6_SEL_MASK			= 0x7 << SCLK_UART6_SEL_SHIFT,
	SCLK_UART6_DIV_SHIFT			= 0,
	SCLK_UART6_DIV_MASK			= 0x1f << SCLK_UART6_DIV_SHIFT,

	/* CRU_CLK_SEL25_CON */
	CLK_FRAC_NUMERATOR_SHIFT		= 16,
	CLK_FRAC_NUMERATOR_MASK			= 0xffff << 16,
	CLK_FRAC_DENOMINATOR_SHIFT		= 0,
	CLK_FRAC_DENOMINATOR_MASK		= 0xffff,
	CLK_FRAC_H_NUMERATOR_SHIFT		= 8,
	CLK_FRAC_H_NUMERATOR_MASK		= 0xff << 8,
	CLK_FRAC_H_DENOMINATOR_SHIFT		= 0,
	CLK_FRAC_H_DENOMINATOR_MASK		= 0xff,

	/* CRU_CLK_SEL43_CON */
	DCLK_VOP_SEL_SHIFT			= 8,
	DCLK_VOP_SEL_MASK			= 0x1 << DCLK_VOP_SEL_SHIFT,
	DCLK_VOP_SEL_GPLL			= 0,
	DCLK_VOP_SEL_CPLL,
	DCLK_VOP_DIV_SHIFT			= 0,
	DCLK_VOP_DIV_MASK			= 0xff << DCLK_VOP_DIV_SHIFT,

	/* CRU_CLK_SEL44_CON */
	HCLK_BUS_SEL_SHIFT			= 10,
	HCLK_BUS_SEL_MASK			= 0x1 << HCLK_BUS_SEL_SHIFT,
	HCLK_BUS_SEL_200M			= 0,
	HCLK_BUS_SEL_100M,
	ACLK_BUS_SEL_SHIFT			= 8,
	ACLK_BUS_SEL_MASK			= 0x3 << ACLK_BUS_SEL_SHIFT,
	ACLK_BUS_SEL_400M			= 0,
	ACLK_BUS_SEL_300M,
	ACLK_BUS_SEL_200M,
	ACLK_TOP_SEL_SHIFT			= 6,
	ACLK_TOP_SEL_MASK			= 0x3 << ACLK_TOP_SEL_SHIFT,
	ACLK_TOP_SEL_600M			= 0,
	ACLK_TOP_SEL_400M,
	ACLK_TOP_SEL_200M,

	/* CRU_CLK_SEL45_CON */
	CLK_GMAC_PTP_REF_SRC_DIV_SHIFT		= 11,
	CLK_GMAC_PTP_REF_SRC_DIV_MASK		= 0x1f << CLK_GMAC_PTP_REF_SRC_DIV_SHIFT,
	CLK_GMAC_PTP_REF_SRC_SEL_SHIFT		= 10,
	CLK_GMAC_PTP_REF_SRC_SEL_MASK		= 0x1 << CLK_GMAC_PTP_REF_SRC_SEL_SHIFT,
	CLK_GMAC_PTP_REF_SRC_SEL_CPLL		= 0,
	CLK_GMAC_PTP_REF_SRC_SEL_24M,
	CLK_SDMMC_SEL_SHIFT			= 8,
	CLK_SDMMC_SEL_MASK			= 0x3 << CLK_SDMMC_SEL_SHIFT,
	CLK_SDMMC_SEL_GPLL			= 0,
	CLK_SDMMC_SEL_CPLL,
	CLK_SDMMC_SEL_24M,
	CLK_SDMMC_DIV_SHIFT			= 0,
	CLK_SDMMC_DIV_MASK			= 0xff << CLK_SDMMC_DIV_SHIFT,

	/* CRU_CLK_SEL46_CON */
	TCLK_WDT_HPMCU_SEL_SHIFT		= 14,
	TCLK_WDT_HPMCU_SEL_MASK			= 0x1 << TCLK_WDT_HPMCU_SEL_SHIFT,
	TCLK_WDT_S_SEL_SHIFT			= 13,
	TCLK_WDT_S_SEL_MASK			= 0x1 << TCLK_WDT_S_SEL_SHIFT,
	TCLK_WDT_NS_SEL_SHIFT			= 12,
	TCLK_WDT_NS_SEL_MASK			= 0x1 << TCLK_WDT_NS_SEL_SHIFT,
	TCLK_WDT_SEL_100M			= 0,
	TCLK_WDT_SEL_OSC,

	/* CRU_CLK_SEL47_CON */
	ACLK_PERI_SEL_SHIFT			= 13,
	ACLK_PERI_SEL_MASK			= 0x1 << ACLK_PERI_SEL_SHIFT,
	ACLK_PERI_SEL_200M			= 0,
	ACLK_PERI_SEL_24M,
	PCLK_PERI_SEL_SHIFT			= 12,
	PCLK_PERI_SEL_MASK			= 0x1 << PCLK_PERI_SEL_SHIFT,
	PCLK_PERI_SEL_100M			= 0,
	PCLK_PERI_SEL_24M,

	/* CRU_CLK_SEL50_CON */
	ACLK_RKCE_SEL_SHIFT			= 13,
	ACLK_RKCE_SEL_MASK			= 0x1 << ACLK_RKCE_SEL_SHIFT,
	ACLK_RKCE_SEL_200M			= 0,
	ACLK_RKCE_SEL_24M,
	CLK_PKA_RKCE_SEL_SHIFT			= 12,
	CLK_PKA_RKCE_SEL_MASK			= 0x1 << CLK_PKA_RKCE_SEL_SHIFT,
	CLK_PKA_RKCE_SEL_300M			= 0,
	CLK_PKA_RKCE_SEL_200M,
	CLK_PWM3_SEL_SHIFT			= 11,
	CLK_PWM3_SEL_MASK			= 0x1 << CLK_PWM3_SEL_SHIFT,
	CLK_PWM2_SEL_SHIFT			= 10,
	CLK_PWM2_SEL_MASK			= 0x1 << CLK_PWM2_SEL_SHIFT,
	CLK_PWM0_SEL_SHIFT			= 8,
	CLK_PWM0_SEL_MASK			= 0x1 << CLK_PWM0_SEL_SHIFT,
	CLK_PWM_SEL_100M			= 0,
	CLK_PWM_SEL_24M,
	CLK_SPI1_SEL_SHIFT			= 4,
	CLK_SPI1_SEL_MASK			= 0x3 << CLK_SPI1_SEL_SHIFT,
	CLK_SPI0_SEL_SHIFT			= 2,
	CLK_SPI0_SEL_MASK			= 0x3 << CLK_SPI0_SEL_SHIFT,
	CLK_SPI0_SEL_200M			= 0,
	CLK_SPI0_SEL_100M,
	CLK_SPI0_SEL_50M,
	CLK_SPI0_SEL_24M,
	CLK_I2C_SEL_SHIFT			= 1,
	CLK_I2C_SEL_MASK			= 0x1 << CLK_I2C_SEL_SHIFT,
	CLK_I2C_SEL_200M			= 0,
	CLK_I2C_SEL_24M,

	/* CRU_CLK_SEL63_CON */
	CLK_SARADC2_SEL_SHIFT			= 14,
	CLK_SARADC2_SEL_MASK			= 0x1 << CLK_SARADC2_SEL_SHIFT,
	CLK_SARADC1_SEL_SHIFT			= 13,
	CLK_SARADC1_SEL_MASK			= 0x1 << CLK_SARADC1_SEL_SHIFT,
	CLK_SARADC0_SEL_SHIFT			= 12,
	CLK_SARADC0_SEL_MASK			= 0x1 << CLK_SARADC0_SEL_SHIFT,
	CLK_SARADC_SEL_200M			= 0,
	CLK_SARADC_SEL_24M,
	CLK_SARADC2_DIV_SHIFT			= 8,
	CLK_SARADC2_DIV_MASK			= 0x7 << CLK_SARADC2_DIV_SHIFT,
	CLK_SARADC1_DIV_SHIFT			= 4,
	CLK_SARADC1_DIV_MASK			= 0x7 << CLK_SARADC1_DIV_SHIFT,
	CLK_SARADC0_DIV_SHIFT			= 0,
	CLK_SARADC0_DIV_MASK			= 0x7 << CLK_SARADC0_DIV_SHIFT,

	/* CRU_CLK_SEL69_CON */
	CLK_MAC_OUT2IO_DIV_SHIFT		= 8,
	CLK_MAC_OUT2IO_DIV_MASK			= 0x7f << CLK_MAC_OUT2IO_DIV_SHIFT,
	CLK_MAC_OUT2IO_SEL_SHIFT		= 6,
	CLK_MAC_OUT2IO_SEL_MASK			= 0x3 << CLK_MAC_OUT2IO_SEL_SHIFT,
	CLK_MAC_OUT2IO_SEL_GPLL			= 0,
	CLK_MAC_OUT2IO_SEL_CPLL,
	CLK_MAC_OUT2IO_SEL_24M,

	/* PMUCRU_CLK_SEL2_CON */
	CLK_I2C2_SEL_SHIFT			= 14,
	CLK_I2C2_SEL_MASK			= 0x3 << CLK_I2C2_SEL_SHIFT,
	CLK_I2C2_SEL_24M			= 0,
	CLK_I2C2_SEL_RCOSC,
	CLK_I2C2_SEL_100M,
	CLK_PWM1_SEL_SHIFT			= 8,
	CLK_PWM1_SEL_MASK			= 0x3 << CLK_PWM1_SEL_SHIFT,
	CLK_PWM1_SEL_24M			= 0,
	CLK_PWM1_SEL_RCOSC,
	CLK_PWM1_SEL_100M,
	CLK_PWM1_DIV_SHIFT			= 6,
	CLK_PWM1_DIV_MASK			= 0x3 << CLK_PWM1_DIV_SHIFT,

	/* PMUCRU_CLK_SEL3_CON */
	TCLK_WDT_LPMCU_SEL_SHIFT		= 6,
	TCLK_WDT_LPMCU_SEL_MASK			= 0x3 << TCLK_WDT_LPMCU_SEL_SHIFT,
	TCLK_WDT_LPMCU_SEL_OSC			= 0,
	TCLK_WDT_LPMCU_SEL_RCOSC,
	TCLK_WDT_LPMCU_SEL_100M,
	TCLK_WDT_LPMCU_SEL_32K,
	SCLK_UART0_SEL_SHIFT			= 0,
	SCLK_UART0_SEL_MASK			= 0x3 << SCLK_UART0_SEL_SHIFT,
	SCLK_UART0_SEL_UART0_SRC		= 0,
	SCLK_UART0_SEL_OSC,
	SCLK_UART0_SEL_RCOSC,

	/* PMU1CRU_CLK_SEL0_CON */
	SCLK_1X_FSPI1_DIV_SHIFT			= 2,
	SCLK_1X_FSPI1_DIV_MASK			= 0x7 << SCLK_1X_FSPI1_DIV_SHIFT,
	SCLK_1X_FSPI1_SEL_SHIFT			= 0,
	SCLK_1X_FSPI1_SEL_MASK			= 0x3 << SCLK_1X_FSPI1_SEL_SHIFT,
	SCLK_1X_FSPI1_SEL_24M			= 0,
	SCLK_1X_FSPI1_SEL_RCOSC,
	SCLK_1X_FSPI1_SEL_100M,
};
#endif
