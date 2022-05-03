/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __VPU_CTRL_H__
#define __VPU_CTRL_H__
#include "vpu.h"

/* #define LIMIT_VPU_CLK_LOW */

/* ************************************************ */
/* VPU frequency table, important. DO NOT modify!! */
/* ************************************************ */
/* fixed pll frequency */
#define FCLK_2000M    2000 /* unit: MHz */

/* G12A */
/* freq max=666M, default=666M */
#define CLK_LEVEL_DFT_G12A     7
#define CLK_LEVEL_MAX_G12A     8

/* T5D */
/* freq max=400M, default=250M */
#define CLK_LEVEL_DFT_T5D     3
#define CLK_LEVEL_MAX_T5D     6

/* C3 */
/* freq max=333M, default=333M */
#define CLK_LEVEL_DFT_C3      4
#define CLK_LEVEL_MAX_C3      5

/* vpu clk setting */
#if 0
static struct fclk_div_s fclk_div_table_gxb[] = {
	/* id,         mux,  div */
	{FCLK_DIV4,    0,    4},
	{FCLK_DIV3,    1,    3},
	{FCLK_DIV5,    2,    5},
	{FCLK_DIV7,    3,    7},
	{FCLK_DIV_MAX, 8,    1},
};
#endif

static struct fclk_div_s fclk_div_table_g12a[] = {
	/* id,         mux,  div */
	{FCLK_DIV3,    0,    3},
	{FCLK_DIV4,    1,    4},
	{FCLK_DIV5,    2,    5},
	{FCLK_DIV7,    3,    7},
	{FCLK_DIV_MAX, 8,    1},
};

static struct fclk_div_s fclk_div_table_c3[] = {
	/* id,         mux,  div */
	{FCLK_DIV3,    1,    3},
	{FCLK_DIV4,    2,    4},
	{FCLK_DIV5,    3,    5},
	{FCLK_DIV7,    7,    7},
	{FCLK_DIV_MAX, 8,    1},
};

static struct vpu_clk_s vpu_clk_table[] = {
	/* frequency   clk_mux       div */
	{100000000,    FCLK_DIV5,    3}, /* 0 */
	{166667000,    FCLK_DIV3,    3}, /* 1 */
	{200000000,    FCLK_DIV5,    1}, /* 2 */
	{250000000,    FCLK_DIV4,    1}, /* 3 */
	{333333000,    FCLK_DIV3,    1}, /* 4 */
	{400000000,    FCLK_DIV5,    0}, /* 5 */
	{500000000,    FCLK_DIV4,    0}, /* 6 */
	{666667000,    FCLK_DIV3,    0}, /* 7 */
	{696000000,    GPLL_CLK,     0}, /* 8 */ /* invalid */
	{850000000,    GPLL_CLK,     0}, /* 9 */ /* invalid */
};

/* ******************************************************* */
/*                VPU reg access test                      */
/* ******************************************************* */
#define VCBUS_REG_CNT_MAX    3
static unsigned int vcbus_test_reg[VCBUS_REG_CNT_MAX] = {
	VENC_VDAC_TST_VAL,
	VPP_DUMMY_DATA,
	VPU_VPU_PWM_V0,
};

static unsigned int vcbus_test_reg_c3[VCBUS_REG_CNT_MAX] = {
	VPU_VOUT_BLEND_DUMDATA,
	VPP_VD1_MATRIX_OFFSET0_1,
	VPU_VOUT_DTH_DATA,
};

/* ******************************************************* */
/*              VPU memory power down table                */
/* ******************************************************* */
static struct vpu_ctrl_s vpu_mem_pd_g12a[] = {
	/* reg,               val, bit, len */
	{HHI_VPU_MEM_PD_REG0, 1,   0,   32},
	{HHI_VPU_MEM_PD_REG1, 1,   0,   32},
	{HHI_VPU_MEM_PD_REG2, 1,   0,    2},
	{HHI_VPU_MEM_PD_REG2, 1,   4,   14},
	{HHI_VPU_MEM_PD_REG2, 1,  30,    2},
	{VPU_REG_END,         0,   0,    0},
};

static struct vpu_ctrl_s vpu_mem_pd_tl1[] = {
	/* reg,               val, bit, len */
	{HHI_VPU_MEM_PD_REG0, 1,   0,   32},
	{HHI_VPU_MEM_PD_REG1, 1,   0,   32},
	{HHI_VPU_MEM_PD_REG2, 1,   0,   32},
	{HHI_VPU_MEM_PD_REG3, 1,   0,   32},
	{HHI_VPU_MEM_PD_REG4, 1,   0,    4},
	{VPU_REG_END,         0,   0,    0},
};

static struct vpu_ctrl_s vpu_mem_pd_tm2[] = {
	/* reg,               val, bit, len */
	{HHI_VPU_MEM_PD_REG0, 1,   0,   32},
	{HHI_VPU_MEM_PD_REG1, 1,   0,   32},
	{HHI_VPU_MEM_PD_REG2, 1,   0,   32},
	{HHI_VPU_MEM_PD_REG3, 1,   0,   32},
	{HHI_VPU_MEM_PD_REG4, 1,   0,    8},
	{VPU_REG_END,         0,   0,    0},
};

/* ******************************************************* */
/*              VPU_HDMI POWER               */
/* ******************************************************* */
static struct vpu_ctrl_s vpu_pwr_gxb[] = {
	/* reg,                val, bit, len */
	{AO_RTI_GEN_PWR_SLEEP0,  0,   8,   1},
	{VPU_REG_END,            0,   0,   0},
};

/* ******************************************************* */
/*              VPU_HDMI ISO                */
/* ******************************************************* */
static struct vpu_ctrl_s vpu_iso_gxb[] = {
	/* reg,                val, bit, len */
	{AO_RTI_GEN_PWR_SLEEP0,  0,   9,   1},
	{VPU_REG_END,            0,   0,   0},
};

static struct vpu_ctrl_s vpu_iso_sm1[] = {
	/* reg,                val, bit, len */
	{AO_RTI_GEN_PWR_ISO0,    0,   8,   1},
	{VPU_REG_END,            0,   0,   0},
};

/* ******************************************************* */
/*                 VPU reset table                    */
/* ******************************************************* */
static struct vpu_reset_s vpu_reset_g12a[] = {
	/* reg,        mask */
	{RESET0_LEVEL, ((1<<5) | (1<<10) | (1<<19) | (1<<13))},
	{RESET1_LEVEL, (1<<5)},
	{RESET2_LEVEL, (1<<15)},
	{RESET4_LEVEL, ((1<<6) | (1<<7) | (1<<13) | (1<<5) | (1<<9) | (1<<4) | (1<<12))},
	{RESET7_LEVEL, (1<<7)},
	{VPU_REG_END,  0},
};

static struct vpu_reset_s vpu_reset_tl1[] = {
	/* reg,        mask */
	{RESET0_LEVEL, ((1<<5) | (1<<10) | (1<<19) | (1<<13))},
	{RESET1_LEVEL, ((1<<5) | (1<<4))},
	{RESET2_LEVEL, (1<<15)},
	{RESET4_LEVEL, ((1<<6) | (1<<7) | (1<<13) | (1<<5) | (1<<9) | (1<<4) | (1<<12))},
	{RESET7_LEVEL, (1<<7)},
	{VPU_REG_END,  0},
};

/* ******************************************************* */
/*                 VPU pwrctrl id table                 */
/* ******************************************************* */
static unsigned int vpu_pwrctrl_id_table[] = {
	PM_VPU_HDMI,
	VPU_PWR_ID_END
};

static unsigned int vpu_pwrctrl_id_table_t7[] = {
	PM_VPU_HDMI,
	PM_VI_CLK1,
	PM_VI_CLK2,
	VPU_PWR_ID_END
};

static unsigned int vpu_pwrctrl_id_table_t3[] = {
	PM_VPU_HDMI,
	PM_VI_CLK1,
	PM_VI_CLK2,
	PM_NOC_VPU,
	VPU_PWR_ID_END
};

/* ******************************************************* */
/*                 VPU module init table                 */
/* ******************************************************* */
#if 0
static struct vpu_ctrl_s vpu_module_init_txlx[] = {
	/* reg,                     val, bit, len */
	{DOLBY_TV_CLKGATE_CTRL,     1,   10,  2},
	{DOLBY_TV_CLKGATE_CTRL,     1,   2,   2},
	{DOLBY_TV_CLKGATE_CTRL,     1,   4,   2},
	{DOLBY_CORE2A_CLKGATE_CTRL, 1,   10,  2},
	{DOLBY_CORE2A_CLKGATE_CTRL, 1,   2,   2},
	{DOLBY_CORE2A_CLKGATE_CTRL, 1,   4,   2},
	{DOLBY_CORE3_CLKGATE_CTRL,  0,   1,   1},
	{DOLBY_CORE3_CLKGATE_CTRL,  1,   2,   2},
	{VPU_REG_END,               0,   0,   0},
};
#endif
#endif
