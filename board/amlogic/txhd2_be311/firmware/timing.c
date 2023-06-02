/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * board/amlogic/t5d_skt_v1/firmware/timing.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */
#include "ddr_timing.c"

pll_set_t __pll_setting = {
	.cpu_clk				= CONFIG_CPU_CLK / 24 * 24,
#ifdef CONFIG_PXP_EMULATOR
	.pxp					= 1,
#else
	.pxp					= 0,
#endif
	.spi_ctrl				= 0,
	.lCustomerID			= CONFIG_AML_CUSTOMER_ID,
#ifdef CONFIG_DEBUG_MODE
	.debug_mode				= CONFIG_DEBUG_MODE,
	.ddr_clk_debug			= CONFIG_DDR_CLK_DEBUG,
	.cpu_clk_debug			= CONFIG_CPU_CLK_DEBUG,
#endif

#ifdef CONFIG_AML_SUPPORT_TLV
	.nCFGTAddr              = AML_BL2_TMASTER_DDR_ADDR,
#endif
	.low_console_baud		= CONFIG_LOW_CONSOLE_BAUD,
};

#define VCCK_VAL				CONFIG_VCCK_INIT_VOLTAGE
#define VDDEE_VAL				CONFIG_VDDEE_INIT_VOLTAGE
/* VCCK PWM table */
#if   (VCCK_VAL == 800)
	#define VCCK_VAL_REG	0x00220000
#elif (VCCK_VAL == 810)
	#define VCCK_VAL_REG	0x00210001
#elif (VCCK_VAL == 820)
	#define VCCK_VAL_REG	0x00200002
#elif (VCCK_VAL == 830)
	#define VCCK_VAL_REG	0x001F0003
#elif (VCCK_VAL == 840)
	#define VCCK_VAL_REG	0x001E0004
#elif (VCCK_VAL == 850)
	#define VCCK_VAL_REG	0x001D0005
#elif (VCCK_VAL == 860)
	#define VCCK_VAL_REG	0x001C0006
#elif (VCCK_VAL == 870)
	#define VCCK_VAL_REG	0x001B0007
#elif (VCCK_VAL == 880)
	#define VCCK_VAL_REG	0x001A0008
#elif (VCCK_VAL == 890)
	#define VCCK_VAL_REG	0x00190009
#elif (VCCK_VAL == 900)
	#define VCCK_VAL_REG	0x0018000A
#elif (VCCK_VAL == 910)
	#define VCCK_VAL_REG	0x0017000B
#elif (VCCK_VAL == 920)
	#define VCCK_VAL_REG	0x0016000C
#elif (VCCK_VAL == 930)
	#define VCCK_VAL_REG	0x0015000D
#elif (VCCK_VAL == 940)
	#define VCCK_VAL_REG	0x0014000E
#elif (VCCK_VAL == 950)
	#define VCCK_VAL_REG	0x0013000F
#elif (VCCK_VAL == 960)
	#define VCCK_VAL_REG	0x00120010
#elif (VCCK_VAL == 970)
	#define VCCK_VAL_REG	0x00110011
#elif (VCCK_VAL == 980)
	#define VCCK_VAL_REG	0x00100012
#elif (VCCK_VAL == 990)
	#define VCCK_VAL_REG	0x000F0013
#elif (VCCK_VAL == 1000)
	#define VCCK_VAL_REG	0x000E0014
#elif (VCCK_VAL == 1010)
	#define VCCK_VAL_REG	0x000D0015
#elif (VCCK_VAL == 1020)
	#define VCCK_VAL_REG	0x000C0016
#elif (VCCK_VAL == 1030)
	#define VCCK_VAL_REG	0x000B0017
#elif (VCCK_VAL == 1040)
	#define VCCK_VAL_REG	0x000A0018
#elif (VCCK_VAL == 1050)
	#define VCCK_VAL_REG	0x00090019
#elif (VCCK_VAL == 1060)
	#define VCCK_VAL_REG	0x0008001A
#elif (VCCK_VAL == 1070)
	#define VCCK_VAL_REG	0x0007001B
#elif (VCCK_VAL == 1080)
	#define VCCK_VAL_REG	0x0006001C
#elif (VCCK_VAL == 1090)
	#define VCCK_VAL_REG	0x0005001D
#elif (VCCK_VAL == 1100)
	#define VCCK_VAL_REG	0x0004001E
#elif (VCCK_VAL == 1110)
	#define VCCK_VAL_REG	0x0003001F
#elif (VCCK_VAL == 1120)
	#define VCCK_VAL_REG	0x00020020
#elif (VCCK_VAL == 1130)
	#define VCCK_VAL_REG	0x00010021
#elif (VCCK_VAL == 1140)
	#define VCCK_VAL_REG	0x00000022
#else
	#error "VCCK val out of range\n"
#endif

/* VDDEE PWM table */
#if   (VDDEE_VAL == 800)
	#define PWM_CONSTANT_OUT
	#define VDDEE_VAL_REG	0x001D0000
#elif (VDDEE_VAL == 810)
	#define VDDEE_VAL_REG   0x001C0000
#elif (VDDEE_VAL == 820)
	#define VDDEE_VAL_REG   0x001B0001
#elif (VDDEE_VAL == 830)
	#define VDDEE_VAL_REG   0x001A0002
#elif (VDDEE_VAL == 840)
	#define VDDEE_VAL_REG   0x00190003
#elif (VDDEE_VAL == 850)
	#define VDDEE_VAL_REG   0x00180004
#elif (VDDEE_VAL == 860)
	#define VDDEE_VAL_REG   0x00170005
#elif (VDDEE_VAL == 870)
	#define VDDEE_VAL_REG   0x00160006
#elif (VDDEE_VAL == 880)
	#define VDDEE_VAL_REG   0x00150007
#elif (VDDEE_VAL == 890)
	#define VDDEE_VAL_REG   0x00140008
#elif (VDDEE_VAL == 900)
	#define VDDEE_VAL_REG   0x00130009
#elif (VDDEE_VAL == 910)
	#define VDDEE_VAL_REG   0x0012000A
#elif (VDDEE_VAL == 920)
	#define VDDEE_VAL_REG   0x0011000B
#elif (VDDEE_VAL == 930)
	#define VDDEE_VAL_REG   0x0010000C
#elif (VDDEE_VAL == 940)
	#define VDDEE_VAL_REG   0x000F000D
#elif (VDDEE_VAL == 950)
	#define VDDEE_VAL_REG   0x000E000E
#elif (VDDEE_VAL == 960)
	#define VDDEE_VAL_REG   0x000D000F
#elif (VDDEE_VAL == 970)
	#define VDDEE_VAL_REG   0x000C0010
#elif (VDDEE_VAL == 980)
	#define VDDEE_VAL_REG   0x000B0011
#elif (VDDEE_VAL == 990)
	#define VDDEE_VAL_REG   0x000A0012
#elif (VDDEE_VAL == 1000)
	#define VDDEE_VAL_REG   0x00090013
#elif (VDDEE_VAL == 1010)
	#define VDDEE_VAL_REG   0x00080014
#elif (VDDEE_VAL == 1020)
	#define VDDEE_VAL_REG   0x00070015
#elif (VDDEE_VAL == 1030)
	#define VDDEE_VAL_REG   0x00060016
#elif (VDDEE_VAL == 1040)
	#define VDDEE_VAL_REG   0x00050017
#elif (VDDEE_VAL == 1050)
	#define VDDEE_VAL_REG   0x00040018
#elif (VDDEE_VAL == 1060)
	#define VDDEE_VAL_REG   0x00030019
#elif (VDDEE_VAL == 1070)
	#define VDDEE_VAL_REG   0x0002001A
#elif (VDDEE_VAL == 1080)
	#define VDDEE_VAL_REG   0x0001001B
#elif (VDDEE_VAL == 1090)
	#define VDDEE_VAL_REG   0x0000001C
#elif (VDDEE_VAL == 1100)
	#define PWM_CONSTANT_OUT
	#define VDDEE_VAL_REG	0x0000001F
#else
	#error "VDDEE val out of range\n"
#endif

/* for PWM use */
/* PWM driver check http://scgit.amlogic.com:8080/#/c/38093/ */
#define PWMAB_PWM_A                     ((0x0000 << 2) + 0xffd1b000)
#define PWMAB_PWM_B                     ((0x0001 << 2) + 0xffd1b000)
#define PWMAB_MISC_REG_AB       ((0x0002 << 2) + 0xffd1b000)

/* board vmin_value defines */
#define VMIN_FT_FF                           770
#define VMIN_FT_TT                           800
#define VMIN_FT_SS                           810
/* board vddee_value defines */
/* SS/TT/FF=1.0/1.0/1.0 V */
#define VMIN_EE_FF                          0x00090013
#define VMIN_EE_TT                          0x00090013
#define VMIN_EE_SS                          0x00090013

bl2_reg_t __bl2_reg[] = {
	/* demo, user defined override register */
	/* vmin value init */
	{0,	VMIN_FT_FF,	0xffffffff,	0,	BL2_VMIN_FT__FLAG_1, 0},
	{0,	VMIN_FT_TT,	0xffffffff,	0,	BL2_VMIN_FT__FLAG_2, 0},
	{0,	VMIN_FT_SS,	0xffffffff,	0,	BL2_VMIN_FT__FLAG_3, 0},
	/* eg: PWM init */
	/* PWM_A VCCK_VAL_REG */
	{ PWMAB_PWM_A,		       VCCK_VAL_REG,			   0xffffffff,	 0, BL2_INIT_STAGE_1, 0 },
	/* PWMAO_B VDDEE_VAL_REG */
#ifdef CONFIG_PDVFS_ENABLE
	{AO_PWM_PWM_B,	VMIN_EE_FF,	0xffffffff,	0,	BL2_VMIN_EE__FLAG_1, 0},
	{AO_PWM_PWM_B,	VMIN_EE_TT,	0xffffffff,	0,	BL2_VMIN_EE__FLAG_2, 0},
	{AO_PWM_PWM_B,	VMIN_EE_SS,	0xffffffff,	0,	BL2_VMIN_EE__FLAG_3, 0},
#else
	{ AO_PWM_PWM_B,	VDDEE_VAL_REG,	0xffffffff,	0,	BL2_INIT_STAGE_1, 0 },
#endif
	/*pwm needs to support full-height or full-low output*/
	{PWMAB_MISC_REG_AB,	((1 << 0) | (1 << 15) | (1 << 28)), ((1 << 0) | (1 << 15) | (1 << 28)), 0, BL2_INIT_STAGE_1, 0},
#ifdef PWM_CONSTANT_OUT
	{AO_PWM_MISC_REG_AB,	((1 << 1) | (1 << 23) | (1 << 29)), ((1 << 1) | (1 << 23) | (1 << 29))), 0, BL2_INIT_STAGE_1, 0},
#else
	{AO_PWM_MISC_REG_AB,	((1 << 1) | (1 << 23)), ((1 << 1) | (1 << 23)), 0, BL2_INIT_STAGE_1, 0},
#endif
	/* MUX GPIOAO_9 to PWMAO_B GPIOZ5 to PWM_A */
	{ AO_RTI_PINMUX_REG1,	       (4 << 4),		   (0xF << 4), 0, BL2_INIT_STAGE_1, 0 },
	{ PERIPHS_PIN_MUX_4,	       (1 << 20),		   (0xF << 20), 0, BL2_INIT_STAGE_1, 0 },
	/* Enable VCCK SET GPIOD_10 HIGH */
	//{ AO_RTI_PINMUX_REG1,	       (0 << 8),			   (0xF << 8),	 0, BL2_INIT_STAGE_1, 0 },
	//{ AO_GPIO_O_EN_N,	       (0 << 10),			   (1 << 10),	 0, BL2_INIT_STAGE_1, 0 },
	//{ AO_GPIO_O,		       (1 << 10),			   (1 << 10),	 0, BL2_INIT_STAGE_1, 0 },
	/* Enable 5V_EN ,set GPIOAO_10 low --> several millisecond--set high in board_init */
	{ AO_RTI_PINMUX_REG1,	       (0 << 8),		(0xF << 8),	 0, BL2_INIT_STAGE_1, 0 },
	{ AO_GPIO_O_EN_N,	       (0 << 10),		(1 << 10),	 0, BL2_INIT_STAGE_1, 0 },
	{ AO_GPIO_O,		       (0 << 10),		(1 << 10),	 0, BL2_INIT_STAGE_1, 0 },
	/* GPIOH_12 pull high */
	//{PREG_PAD_GPIO2_O,     (1 << 12),               (1 << 12),   0, BL2_INIT_STAGE_1, 0},
	//{PREG_PAD_GPIO2_EN_N,  (0 << 12),               (1 << 12),   0, BL2_INIT_STAGE_1, 0},
};
