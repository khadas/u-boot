// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <fdtdec.h>
#include <dm.h>
#include <asm/arch/cpu.h>
#include <amlogic/cpu_id.h>
#include <amlogic/media/vout/aml_vout.h>
#include <amlogic/media/vout/aml_cvbs.h>
#include "cvbs_reg.h"
#include "cvbs_config.h"
#include "cvbs.h"
#include "vdac.h"

/*----------------------------------------------------------------------------*/
static struct cvbs_drv_s cvbs_drv = {
	.data = NULL,
};

static struct cvbs_data_s cvbs_data_g12a = {
	.chip_type = CVBS_CHIP_G12A,

	.reg_vid_pll_clk_div = HHI_VID_PLL_CLK_DIV,
	.reg_vid_clk_div = HHI_VID_CLK_DIV,
	.reg_vid_clk_ctrl = HHI_VID_CLK_CNTL,
	.reg_vid2_clk_div = HHI_VIID_CLK_DIV,
	.reg_vid2_clk_ctrl = HHI_VIID_CLK_CNTL,
	.reg_vid_clk_ctrl2 = HHI_VID_CLK_CNTL2,

	.vdac_vref_adj = 0x10,
	.vdac_gsw = 0x0,
};

static struct cvbs_data_s cvbs_data_g12b = {
	.chip_type = CVBS_CHIP_G12B,

	.reg_vid_pll_clk_div = HHI_VID_PLL_CLK_DIV,
	.reg_vid_clk_div = HHI_VID_CLK_DIV,
	.reg_vid_clk_ctrl = HHI_VID_CLK_CNTL,
	.reg_vid2_clk_div = HHI_VIID_CLK_DIV,
	.reg_vid2_clk_ctrl = HHI_VIID_CLK_CNTL,
	.reg_vid_clk_ctrl2 = HHI_VID_CLK_CNTL2,

	.vdac_vref_adj = 0xf,
	.vdac_gsw = 0x0,
};

static struct cvbs_data_s cvbs_data_sc2 = {
	.chip_type = CVBS_CHIP_SC2,

	.reg_vid_pll_clk_div = CLKCTRL_VID_PLL_CLK_DIV,
	.reg_vid_clk_div = CLKCTRL_VID_CLK_DIV,
	.reg_vid_clk_ctrl = CLKCTRL_VID_CLK_CTRL,
	.reg_vid2_clk_div = CLKCTRL_VIID_CLK_DIV,
	.reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK_CTRL,
	.reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK_CTRL2,

	.vdac_vref_adj = 0x10,
	.vdac_gsw = 0x0,
};

static struct cvbs_data_s cvbs_data_s4 = {
	.chip_type = CVBS_CHIP_S4,

	.reg_vid_pll_clk_div = CLKCTRL_VID_PLL_CLK_DIV,
	.reg_vid_clk_div = CLKCTRL_VID_CLK_DIV,
	.reg_vid_clk_ctrl = CLKCTRL_VID_CLK_CTRL,
	.reg_vid2_clk_div = CLKCTRL_VIID_CLK_DIV,
	.reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK_CTRL,
	.reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK_CTRL2,

	.vdac_vref_adj = 0x10,
	.vdac_gsw = 0x0,
};

static struct cvbs_data_s cvbs_data_s4d = {
	.chip_type = CVBS_CHIP_S4D,

	.reg_vid_pll_clk_div = CLKCTRL_VID_PLL_CLK_DIV,
	.reg_vid_clk_div = CLKCTRL_VID_CLK_DIV,
	.reg_vid_clk_ctrl = CLKCTRL_VID_CLK_CTRL,
	.reg_vid2_clk_div = CLKCTRL_VIID_CLK_DIV,
	.reg_vid2_clk_ctrl = CLKCTRL_VIID_CLK_CTRL,
	.reg_vid_clk_ctrl2 = CLKCTRL_VID_CLK_CTRL2,

	.vdac_vref_adj = 0x10,
	.vdac_gsw = 0x0,
};

unsigned int cvbs_mode = VMODE_MAX;
/*bit[0]: 0=vid_pll, 1=gp0_pll*/
/*bit[1]: 0=vid2_clk, 1=vid1_clk*/
/*path 0:vid_pll vid2_clk*/
/*path 1:gp0_pll vid2_clk*/
/*path 2:vid_pll vid1_clk*/
/*path 3:gp0_pll vid1_clk*/
static unsigned int s_enci_clk_path = 0;

/*----------------------------------------------------------------------------*/
// configuration for enci bist
int cvbs_set_bist(char* bist_mode)
{
	if (!strcmp(bist_mode, "off")) {
		cvbs_write_vcbus(ENCI_VIDEO_MODE_ADV, 0x26);
		cvbs_write_vcbus(ENCI_TST_EN, 0x0);
	} else {
		unsigned int mode = 0;

		if (!strcmp(bist_mode, "fixval") || !strcmp(bist_mode, "0"))
			mode = 0;
		else if (!strcmp(bist_mode, "colorbar") || !strcmp(bist_mode, "1"))
			mode = 1;
		else if (!strcmp(bist_mode, "thinline") || !strcmp(bist_mode, "2"))
			mode = 2;
		else if (!strcmp(bist_mode, "dotgrid") || !strcmp(bist_mode, "3"))
			mode = 3;

		cvbs_write_vcbus(ENCI_VIDEO_MODE_ADV, 0x2);
		cvbs_write_vcbus(ENCI_TST_MDSEL, mode);
		cvbs_write_vcbus(ENCI_TST_CLRBAR_STRT, 0x112);
		cvbs_write_vcbus(ENCI_TST_CLRBAR_WIDTH, 0xb4);
		cvbs_write_vcbus(ENCI_TST_EN, 0x1);
	}

	return 0;
}

/*----------------------------------------------------------------------------*/
int cvbs_set_vdac(int status)
{
	switch (status) {
	case 0:// close vdac
		if (cvbs_drv.data)
			vdac_enable(0, VDAC_MODULE_CVBS_OUT);
		else
			printf("cvbs ERROR:need run cvbs init.\n");
		break;
	case 1:// from enci to vdac
		cvbs_set_vcbus_bits(VENC_VDAC_DACSEL0, 0, 5, 1);
		if (cvbs_drv.data) {
			vdac_ctrl_vref_adj(cvbs_drv.data->vdac_vref_adj);
			vdac_enable(1, VDAC_MODULE_CVBS_OUT);
		} else {
			printf("cvbs ERROR:need run cvbs init.\n");
		}
		break;
	default:
		break;
	}

	return 0;
}

/*----------------------------------------------------------------------------*/
// interface for debug
static void cvbs_dump_cvbs_regs(void)
{
	struct reg_s *p = NULL;

	if (VMODE_PAL == cvbs_mode) {
		// 576cvbs
		p = (struct reg_s*)&tvregs_576cvbs_enc[0];

	} else if (VMODE_NTSC == cvbs_mode) {
		// 480cvbs
		p = (struct reg_s*)&tvregs_480cvbs_enc[0];
	}

	if (NULL == p) {
		printf("it's not in cvbs mode!\n");
		return;
	}

	if (MREG_END_MARKER != p->reg)
		printf("cvbs enci registers:\n");
	while (MREG_END_MARKER != p->reg) {
		printf("    vcbus[0x%.2x] = 0x%.4x\n", p->reg, cvbs_read_vcbus(p->reg));
		p ++;
	}

	return;
}

unsigned int cvbs_clk_regs[] = {
	HHI_HDMI_PLL_CNTL0,
	HHI_HDMI_PLL_CNTL1,
	HHI_HDMI_PLL_CNTL2,
	HHI_HDMI_PLL_CNTL3,
	HHI_HDMI_PLL_CNTL4,
	HHI_HDMI_PLL_CNTL5,
	HHI_HDMI_PLL_CNTL6,
	HHI_VID_PLL_CLK_DIV,
	HHI_VIID_CLK_DIV,
	HHI_VIID_CLK_CNTL,
	HHI_VID_CLK_DIV,
	HHI_VID_CLK_CNTL2,
	MREG_END_MARKER
};

static void cvbs_dump_clock_regs(void)
{
	unsigned int *p = &cvbs_clk_regs[0];

	if (MREG_END_MARKER != *p)
		printf("cvbs clock registers:\n");
	while (MREG_END_MARKER != *p) {
		printf("    hiu[0x%.2x] = 0x%.4x\n", *p, cvbs_read_hiu(*p));
		p ++;
	}

	return;
}

int cvbs_reg_debug(int argc, char* const argv[])
{
	unsigned int value;

	if (!cvbs_drv.data) {
		printf("cvbs: error: %s: no cvbs data\n", __func__);
		return -1;
	}

	if (!strcmp(argv[1], "clock")) {
		if (argc != 2)
			goto fail_cmd;

		cvbs_dump_clock_regs();
	} else if (!strcmp(argv[1], "enci")) {
		if (argc != 2)
			goto fail_cmd;

		cvbs_dump_cvbs_regs();
	} else if (!strcmp(argv[1], "clkpath")) {
		if (argc != 3)
			goto fail_cmd;
		value = simple_strtoul(argv[2], NULL, 0);
		if ((cvbs_drv.data->chip_type == CVBS_CHIP_G12A) ||
		    (cvbs_drv.data->chip_type == CVBS_CHIP_G12B)) {
			if (value == 1 || value == 2 ||
				value == 3 || value == 0) {
				s_enci_clk_path = value;
				printf("path 0:vid_pll vid2_clk\n");
				printf("path 1:gp0_pll vid2_clk\n");
				printf("path 2:vid_pll vid1_clk\n");
				printf("path 3:gp0_pll vid1_clk\n");
				printf("you select path %d\n", s_enci_clk_path);
			} else {
				printf("invalid value, only 0/1/2/3\n");
				printf("bit[0]: 0=vid_pll, 1=gp0_pll\n");
				printf("bit[1]: 0=vid2_clk, 1=vid1_clk\n");
			}
		} else {
			printf("don't support for current chip\n");
		}
	}

	return 0;

fail_cmd:
	return 1;
}

/*----------------------------------------------------------------------------*/
// configuration for clock
#define WAIT_FOR_PLL_LOCKED(reg)                \
	do {                                    \
		unsigned int pll_lock;          \
		unsigned int time_out = 0;      \
		do {                            \
			udelay(20);             \
			pll_lock = cvbs_get_hiu_bits(reg, 31, 1);  \
			time_out ++;                               \
		} while ((pll_lock == 0) && (time_out < 10000));   \
		if (pll_lock == 0)                                 \
			printf("[error]: cvbs pll lock failed\n"); \
	} while(0);

static void cvbs_config_hdmipll_g12a(void)
{
	printf("%s\n", __func__);
	cvbs_write_hiu(HHI_HDMI_PLL_CNTL0,	0x1a0504f7);
	cvbs_write_hiu(HHI_HDMI_PLL_CNTL1,	0x00010000);
	cvbs_write_hiu(HHI_HDMI_PLL_CNTL2,	0x00000000);
	cvbs_write_hiu(HHI_HDMI_PLL_CNTL3,	0x6a28dc00);
	cvbs_write_hiu(HHI_HDMI_PLL_CNTL4,	0x65771290);
	cvbs_write_hiu(HHI_HDMI_PLL_CNTL5,	0x39272000);
	cvbs_write_hiu(HHI_HDMI_PLL_CNTL6,	0x56540000);
	cvbs_write_hiu(HHI_HDMI_PLL_CNTL0,	0x3a0504f7);
	udelay(100);
	cvbs_write_hiu(HHI_HDMI_PLL_CNTL0,	0x1a0504f7);
	WAIT_FOR_PLL_LOCKED(HHI_HDMI_PLL_CNTL0);
	return;
}

static void cvbs_config_gp0pll_g12a(void)
{
	printf("%s\n", __func__);
	cvbs_write_hiu(HHI_GP0_PLL_CNTL0,	0x180204f7);
	cvbs_write_hiu(HHI_GP0_PLL_CNTL1,	0x00010000);
	cvbs_write_hiu(HHI_GP0_PLL_CNTL2,	0x00000000);
	cvbs_write_hiu(HHI_GP0_PLL_CNTL3,	0x6a28dc00);
	cvbs_write_hiu(HHI_GP0_PLL_CNTL4,	0x65771290);
	cvbs_write_hiu(HHI_GP0_PLL_CNTL5,	0x39272000);
	cvbs_write_hiu(HHI_GP0_PLL_CNTL6,	0x56540000);
	cvbs_write_hiu(HHI_GP0_PLL_CNTL0,	0x380204f7);
	udelay(100);
	cvbs_write_hiu(HHI_GP0_PLL_CNTL0,	0x180204f7);
	WAIT_FOR_PLL_LOCKED(HHI_GP0_PLL_CNTL0);
	return;
}

static void cvbs_config_hdmipll_sc2(void)
{
	printf("%s\n", __func__);
	cvbs_write_hiu(ANACTRL_HDMIPLL_CTRL0, 0x3b01047b);
	cvbs_write_hiu(ANACTRL_HDMIPLL_CTRL1, 0x00018000);
	cvbs_write_hiu(ANACTRL_HDMIPLL_CTRL2, 0x00000000);
	cvbs_write_hiu(ANACTRL_HDMIPLL_CTRL3, 0x0a691c00);
	cvbs_write_hiu(ANACTRL_HDMIPLL_CTRL4, 0x33771290);
	cvbs_write_hiu(ANACTRL_HDMIPLL_CTRL5, 0x39270000);
	cvbs_write_hiu(ANACTRL_HDMIPLL_CTRL6, 0x50540000);
	udelay(100);
	cvbs_write_hiu(ANACTRL_HDMIPLL_CTRL0, 0x1b01047b);
	WAIT_FOR_PLL_LOCKED(ANACTRL_HDMIPLL_CTRL0);
}

static void cvbs_set_vid1_clk(unsigned int src_pll)
{
	int sel = 0;

	if (!cvbs_drv.data) {
		printf("cvbs: error: %s: no cvbs data\n", __func__);
		return;
	}

	printf("%s\n", __func__);
	if (src_pll == 0) { /* hpll */
		/* divider: 1 */
		/* Disable the div output clock */
		cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_pll_clk_div, 0, 19, 1);
		cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_pll_clk_div, 0, 15, 1);

		cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_pll_clk_div, 1, 18, 1);
		/* Enable the final output clock */
		cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_pll_clk_div, 1, 19, 1);
		sel = 0;
	} else { /* gp0_pll */
		sel = 1;
	}

	/* xd: 55 */
	/* setup the XD divider value */
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_div, (55 - 1), VCLK_XD0, 8);
	//udelay(5);
	/*0x59[16]/0x5f[19]/0x5f[20]*/
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_ctrl, sel, VCLK_CLK_IN_SEL, 3);
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_ctrl, 1, VCLK_EN0, 1);
	//udelay(2);

	/* vclk: 27M */
	/* [31:28]=0 enci_clk_sel, select vclk_div1 */
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_div, 0, 28, 4);
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid2_clk_div, 0, 28, 4);
	/* release vclk_div_reset and enable vclk_div */
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_div, 1, VCLK_XD_EN, 2);
	//udelay(5);

	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_ctrl, 1, VCLK_DIV1_EN, 1);

	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_ctrl, 1, VCLK_SOFT_RST, 1);
	//udelay(10);
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_ctrl, 0, VCLK_SOFT_RST, 1);
	//udelay(5);
}

static void cvbs_set_vid2_clk(unsigned int src_pll)
{
	int sel = 0;

	if (!cvbs_drv.data) {
		printf("cvbs: error: %s: no cvbs data\n", __func__);
		return;
	}

	printf("%s\n", __func__);
	if (src_pll == 0) { /* hpll */
		/* divider: 1 */
		/* Disable the div output clock */
		cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_pll_clk_div, 0, 19, 1);
		cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_pll_clk_div, 0, 15, 1);

		cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_pll_clk_div, 1, 18, 1);
		/* Enable the final output clock */
		cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_pll_clk_div, 1, 19, 1);
		sel = 0;
	} else { /* gp0_pll */
		sel = 1;
	}

	/* xd: 55 */
	/* setup the XD divider value */
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid2_clk_div, (55 - 1), VCLK2_XD, 8);
	//udelay(5);
	/* Bit[18:16] - v2_cntl_clk_in_sel: vid_pll */
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid2_clk_ctrl, sel, VCLK2_CLK_IN_SEL, 3);
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid2_clk_ctrl, 1, VCLK2_EN, 1);
	//udelay(2);

	/* vclk: 27M */
	/* [31:28]=8 enci_clk_sel, select vclk2_div1 */
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_div, 8, 28, 4);
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid2_clk_div, 8, 28, 4);
	/* release vclk2_div_reset and enable vclk2_div */
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid2_clk_div, 1, VCLK2_XD_EN, 2);
	//udelay(5);

	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid2_clk_ctrl, 1, VCLK2_DIV1_EN, 1);
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid2_clk_ctrl, 1, VCLK2_SOFT_RST, 1);
	//udelay(10);
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid2_clk_ctrl, 0, VCLK2_SOFT_RST, 1);
	//udelay(5);
}

static int cvbs_config_clock(void)
{
	if (!cvbs_drv.data) {
		printf("cvbs: error: %s: no cvbs data\n", __func__);
		return -1;
	}

	/* pll output 1485M */
	switch (cvbs_drv.data->chip_type) {
	case CVBS_CHIP_G12A:
	case CVBS_CHIP_G12B:
		if (s_enci_clk_path & 0x1)
			cvbs_config_gp0pll_g12a();
		else
			cvbs_config_hdmipll_g12a();
		if (s_enci_clk_path & 0x2)
			cvbs_set_vid1_clk(s_enci_clk_path & 0x1);
		else
			cvbs_set_vid2_clk(s_enci_clk_path & 0x1);
		break;
	case CVBS_CHIP_SC2:
	case CVBS_CHIP_S4:
	case CVBS_CHIP_S4D:
		cvbs_config_hdmipll_sc2();
		cvbs_set_vid2_clk(0);
		break;
	default:
		printf("cvbs: %s: invalid chip type\n", __func__);
		return -1;
	}

	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_ctrl2, 1, 0, 1);
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_ctrl2, 1, 4, 1);

	return 0;
}

static void cvbs_disable_clock(void)
{
	if (!cvbs_drv.data) {
		printf("cvbs: error: %s: no cvbs data\n", __func__);
		return;
	}

	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_ctrl2, 0, 4, 1);
	cvbs_set_hiu_bits(cvbs_drv.data->reg_vid_clk_ctrl2, 0, 0, 1);
}

/*----------------------------------------------------------------------------*/
// configuration for enci
static void cvbs_performance_enhancement(int mode)
{
	const struct reg_s *s = NULL;
	struct performance_config_s *perfconf = NULL;
	int i = 0;

	switch (mode) {
	case VMODE_PAL:
		if (cvbs_drv.data->sva_val)
			perfconf = &cvbs_drv.perf_conf_pal_sva;
		else
			perfconf = &cvbs_drv.perf_conf_pal;
		break;
	case VMODE_NTSC:
	case VMODE_NTSC_M:
		perfconf = &cvbs_drv.perf_conf_ntsc;
		break;
	default:
		break;
	}
	if (!perfconf)
		return;

	if (!perfconf->reg_table) {
		printf("no performance table\n");
		return;
	}

	i = 0;
	s = perfconf->reg_table;
	while (i < perfconf->reg_cnt) {
		cvbs_write_vcbus(s->reg, s->val);
		//printf("vcbus reg[0x%04x] = 0x%08x\n", s->reg, s->val);

		s++;
		i++;
	}

	printf("%s\n", __func__);
}

static int cvbs_config_enci(int vmode)
{
	const struct reg_s *s = NULL;

	switch (vmode) {
	case VMODE_PAL:
		s = &tvregs_576cvbs_enc[0];
		break;
	case VMODE_NTSC:
	case VMODE_NTSC_M:
		s = &tvregs_480cvbs_enc[0];
		break;
	case VMODE_PAL_M:
		s = &tvregs_pal_m_enc[0];
		break;
	case VMODE_PAL_N:
		s = &tvregs_pal_n_enc[0];
		break;
	default:
		break;
	}
	if (s == NULL)
		return -1;

	while ((s->reg != MREG_END_MARKER)) {
		cvbs_write_vcbus(s->reg, s->val);
		//printf("reg[0x%.2x] = 0x%.4x\n", s->reg, s->val);
		s ++;
	}

	cvbs_performance_enhancement(vmode);

	return 0;
}

/*----------------------------------------------------------------------------*/
// configuration for output
// output vmode: 576cvbs, 480cvbs
int cvbs_set_vmode(char* vmode_name)
{
	if (!strncmp(vmode_name, "576cvbs", strlen("576cvbs"))) {
		cvbs_mode = VMODE_PAL;
		cvbs_config_enci(0);
		cvbs_config_clock();
		cvbs_set_vdac(1);
		return 0;
	} else if (!strncmp(vmode_name, "480cvbs", strlen("480cvbs"))) {
		cvbs_mode = VMODE_NTSC;
		cvbs_config_enci(1);
		cvbs_config_clock();
		cvbs_set_vdac(1);
		return 0;
	} else if (!strncmp(vmode_name, "ntsc_m", strlen("ntsc_m"))) {
		cvbs_mode = VMODE_NTSC_M;
		cvbs_config_enci(VMODE_NTSC_M);
		cvbs_config_clock();
		cvbs_set_vdac(1);
		return 0;
	} else if (!strncmp(vmode_name, "pal_m", strlen("pal_m"))) {
		cvbs_mode = VMODE_PAL_M;
		cvbs_config_enci(VMODE_PAL_M);
		cvbs_config_clock();
		cvbs_set_vdac(1);
		return 0;
	} else if (!strncmp(vmode_name, "pal_n", strlen("pal_n"))) {
		cvbs_mode = VMODE_PAL_N;
		cvbs_config_enci(VMODE_PAL_N);
		cvbs_config_clock();
		cvbs_set_vdac(1);
		return 0;
	} else if (!strncmp(vmode_name, "disable", strlen("disable"))) {
		cvbs_set_vdac(0);
		cvbs_write_vcbus(ENCI_VIDEO_EN, 0);
		cvbs_disable_clock();
		return 0;
	} else {
		printf("[%s] is invalid for cvbs.\n", vmode_name);
		return -1;
	}

	return 0;
}

/*----------------------------------------------------------------------------*/
#define CVBS_MODE_CNT    5
static char *cvbs_mode_str[CVBS_MODE_CNT] = {
	"576cvbs",
	"480cvbs",
	"ntsc_m",
	"pal_m",
	"pal_n",
};

/***********************************************
 * parameters:  vmode_name, such as 576cvbs, 480cvbs...
 *              frac, cvbs alway 0. don't support.
 * return:      viu_mux
 ************************************************/
unsigned int cvbs_outputmode_check(char *vmode_name, unsigned int frac)
{
	unsigned int i;

	if (frac) {
		printf("cvbs: don't support frac\n");
		return VIU_MUX_MAX;
	}

	for (i = 0; i < CVBS_MODE_CNT; i++) {
		if (!strncmp(vmode_name, cvbs_mode_str[i], strlen(cvbs_mode_str[i])))
			return VIU_MUX_ENCI;
	}

	//printf("cvbs: outputmode[%s] is invalid\n", vmode_name);
	return VIU_MUX_MAX;
}

// list for valid video mode
void cvbs_show_valid_vmode(void)
{
	unsigned int i;

	for (i = 0; i < CVBS_MODE_CNT; i++)
		printf("%s\n", cvbs_mode_str[i]);
}

static char *cvbsout_performance_str[] = {
	"performance", /* default for pal */
	"performance_pal",
	"performance_ntsc",
};

static void cvbs_get_config(void)
{
	const void *dt_blob = NULL;
	int node;
	char *propdata;
	const char *str;
	struct reg_s *s;
	unsigned int i, j, temp, cnt;
	int ret;

	dt_blob = gd->fdt_blob;
	if (!dt_blob) {
		printf("cvbs: error: dt_blob is null, load default setting\n");
		return;
	}

	ret = fdt_check_header(dt_blob);
	if (ret < 0) {
		printf("cvbs: error: check dts: %s, load default setting\n",
			fdt_strerror(ret));
		return;
	}

	node = fdt_path_offset(dt_blob, "/cvbsout");
	if (node < 0) {
		printf("not find /cvbsout node: %s\n",
			fdt_strerror(node));
		return;
	}

	/* clk_path */
	propdata = (char *)fdt_getprop(dt_blob, node, "clk_path", NULL);
	if (propdata) {
		s_enci_clk_path = be32_to_cpup((u32*)propdata);
		printf("cvbs: find clk_path: 0x%x\n", s_enci_clk_path);
	}

	propdata = (char *)fdt_getprop(dt_blob, node, "sva_std", NULL);
	if (propdata) {
		cvbs_drv.data->sva_val = be32_to_cpup((u32 *)propdata);
		printf("cvbs: find sva_std: 0x%x\n", cvbs_drv.data->sva_val);
	}

	/* performance: PAL CTCC */
	str = cvbsout_performance_str[1];
	propdata = (char *)fdt_getprop(dt_blob, node, str, NULL);
	if (!propdata) {
		str = cvbsout_performance_str[0];
		propdata = (char *)fdt_getprop(dt_blob, node, str, NULL);
		if (!propdata)
			goto cvbs_performance_config_ntsc;
	}
	cnt = 0;
	while (cnt < CVBS_PERFORMANCE_CNT_MAX) {
		j = 2 * cnt;
		temp = be32_to_cpup((((u32*)propdata)+j));
		if (temp == MREG_END_MARKER) /* ending */
			break;
		cnt++;
	}
	if (cnt >= CVBS_PERFORMANCE_CNT_MAX)
		cnt = 0;
	if (cnt > 0) {
		printf("cvbs: find %s config\n", str);
		cvbs_drv.perf_conf_pal.reg_table = malloc(sizeof(struct reg_s) * cnt);
		if (!cvbs_drv.perf_conf_pal.reg_table) {
			printf("cvbs: error: failed to alloc %s table\n", str);
			cnt = 0;
		}
		memset(cvbs_drv.perf_conf_pal.reg_table, 0, (sizeof(struct reg_s) * cnt));
		cvbs_drv.perf_conf_pal.reg_cnt = cnt;

		i = 0;
		s = cvbs_drv.perf_conf_pal.reg_table;
		while (i < cvbs_drv.perf_conf_pal.reg_cnt) {
			j = 2 * i;
			s->reg = be32_to_cpup((((u32*)propdata)+j));
			s->val = be32_to_cpup((((u32*)propdata)+j+1));
			/* printf("%p: 0x%04x = 0x%x\n", s, s->reg, s->val); */

			s++;
			i++;
		}
	}

	/* performance: PAL SVA */
	str = cvbsout_performance_str[0];
	propdata = (char *)fdt_getprop(dt_blob, node, str, NULL);
	if (!propdata) {
		str = cvbsout_performance_str[1];
		propdata = (char *)fdt_getprop(dt_blob, node, str, NULL);
		if (!propdata)
			goto cvbs_performance_config_ntsc;
	}
	cnt = 0;
	while (cnt < CVBS_PERFORMANCE_CNT_MAX) {
		j = 2 * cnt;
		temp = be32_to_cpup((((u32 *)propdata) + j));
		if (temp == MREG_END_MARKER) /* ending */
			break;
		cnt++;
	}
	if (cnt >= CVBS_PERFORMANCE_CNT_MAX)
		cnt = 0;
	if (cnt > 0) {
		printf("cvbs: find %s config\n", str);
		cvbs_drv.perf_conf_pal_sva.reg_table = malloc(sizeof(struct reg_s) * cnt);
		if (!cvbs_drv.perf_conf_pal_sva.reg_table) {
			printf("cvbs: error: failed to alloc %s table\n", str);
			cnt = 0;
		}
		memset(cvbs_drv.perf_conf_pal_sva.reg_table, 0, (sizeof(struct reg_s) * cnt));
		cvbs_drv.perf_conf_pal_sva.reg_cnt = cnt;

		i = 0;
		s = cvbs_drv.perf_conf_pal_sva.reg_table;
		while (i < cvbs_drv.perf_conf_pal_sva.reg_cnt) {
			j = 2 * i;
			s->reg = be32_to_cpup((((u32 *)propdata) + j));
			s->val = be32_to_cpup((((u32 *)propdata) + j + 1));
			/* printf("%p: 0x%04x = 0x%x\n", s, s->reg, s->val); */

			s++;
			i++;
		}
	}

	/* performance: NTSC */
cvbs_performance_config_ntsc:
	str = cvbsout_performance_str[2];
	propdata = (char *)fdt_getprop(dt_blob, node, str, NULL);
	if (!propdata)
		return;
	cnt = 0;
	while (cnt < CVBS_PERFORMANCE_CNT_MAX) {
		j = 2 * cnt;
		temp = be32_to_cpup((((u32*)propdata)+j));
		if (temp == MREG_END_MARKER) /* ending */
			break;
		cnt++;
	}
	if (cnt >= CVBS_PERFORMANCE_CNT_MAX)
		cnt = 0;
	if (cnt > 0) {
		printf("cvbs: find performance_ntsc config\n");
		cvbs_drv.perf_conf_ntsc.reg_table = malloc(sizeof(struct reg_s) * cnt);
		if (!cvbs_drv.perf_conf_ntsc.reg_table) {
			printf("cvbs: error: failed to alloc %s table\n", str);
			cnt = 0;
		}
		memset(cvbs_drv.perf_conf_ntsc.reg_table, 0, (sizeof(struct reg_s) * cnt));
		cvbs_drv.perf_conf_ntsc.reg_cnt = cnt;

		i = 0;
		s = cvbs_drv.perf_conf_ntsc.reg_table;
		while (i < cvbs_drv.perf_conf_ntsc.reg_cnt) {
			j = 2 * i;
			s->reg = be32_to_cpup((((u32*)propdata)+j));
			s->val = be32_to_cpup((((u32*)propdata)+j+1));
			/* printf("%p: 0x%04x = 0x%x\n", s, s->reg, s->val); */

			s++;
			i++;
		}
	}
}

void vdac_data_config(void)
{
	printf("cvbs: cpuid:0x%x\n", get_cpu_id().family_id);
	switch (get_cpu_id().family_id) {
	case MESON_CPU_MAJOR_ID_G12A:
		cvbs_drv.data = &cvbs_data_g12a;
		break;
	case MESON_CPU_MAJOR_ID_G12B:
		cvbs_drv.data = &cvbs_data_g12b;
		break;
	case MESON_CPU_MAJOR_ID_SC2:
		cvbs_drv.data = &cvbs_data_sc2;
		break;
	case MESON_CPU_MAJOR_ID_S4:
		cvbs_drv.data = &cvbs_data_s4;
		break;
	case MESON_CPU_MAJOR_ID_S4D:
		cvbs_drv.data = &cvbs_data_s4d;
		break;
	default:
		cvbs_drv.data = &cvbs_data_s4d;
		break;
	}
}

void cvbs_init(void)
{
	vdac_data_config();
	vdac_ctrl_config_probe();
	cvbs_get_config();
}

