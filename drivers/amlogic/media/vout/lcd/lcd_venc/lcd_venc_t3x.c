// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/io.h>
#ifdef CONFIG_AML_VPP
#include <amlogic/media/vpp/vpp.h>
#endif
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "../lcd_reg.h"
#include "../lcd_common.h"
#include "lcd_venc.h"

static inline
unsigned int __cal_h_timing_to_reg(unsigned int origin, unsigned int ppc)
{
	unsigned int val = 0;

	val = origin / ppc;
	return  (val > 0 ? val - 1 : 0);
}

static inline
unsigned int __lcd_round_inc(unsigned int base, unsigned int val, unsigned int round_const)
{
	unsigned int res;

	res = base + val;
	return res < round_const ? res : res % round_const;
}

static void lcd_venc_wait_vsync(struct aml_lcd_drv_s *pdrv)
{
	unsigned int offset, reg;
	int line_cnt, line_cnt_previous;
	int i = 0;

	offset = pdrv->index == 1 ? 0x600 : 0x0;
	reg = VPU_VENCP_STAT + (offset << 2);

	line_cnt = 0x1fff;
	line_cnt_previous = lcd_vcbus_getb(reg, 16, 13);
	while (i++ < LCD_WAIT_VSYNC_TIMEOUT) {
		line_cnt = lcd_vcbus_getb(reg, 16, 13);
		if (line_cnt < line_cnt_previous)
			break;
		line_cnt_previous = line_cnt;
		udelay(2);
	}
}

static unsigned int lcd_venc_get_max_lint_cnt(struct aml_lcd_drv_s *pdrv)
{
	unsigned int offset, reg, line_cnt;

	offset = pdrv->data->offset_venc[pdrv->index];
	reg = ENCL_VIDEO_MAX_CNT + offset;

	line_cnt = lcd_vcbus_getb(reg, 0, 16) + 1;
	/*LCDPR("[%d]: %s: line_cnt=%d", pdrv->index, __func__, line_cnt); */

	return line_cnt;
}

#define LCD_ENC_TST_NUM_MAX    10
struct lcd_enc_test_t {
	char *name;
	unsigned int mode;
	unsigned int y;
	unsigned int cb;
	unsigned int cr;
	unsigned int en;
	unsigned int vfifo_en;
	unsigned int rgb_in;
};

struct lcd_enc_test_t lcd_enc_tst[] = {
	/*       tst_mode,   Y/R, Cb/G,   Cr/B,  tst_en,  vfifo_en  rgbin*/
	{"0-none",      0, 0x200, 0x200,  0x200,   0,      1,        1},
	{"1-Color Bar", 1, 0x200, 0x200,  0x200,   1,      0,        0},  /* 1 */
	{"2-Thin Line", 2, 0x200, 0x200,  0x200,   1,      0,        0},  /* 2 */
	{"3-Dot Grid",  3, 0x200, 0x200,  0x200,   1,      0,        0},  /* 3 */
	{"4-Gray",      0, 0x1ff, 0x1ff,  0x1ff,   1,      0,        1},  /* 4 */
	{"5-Red",       0, 0x3ff, 0x000,  0x000,   1,      0,        1},  /* 5 */
	{"6-Green",     0, 0x000, 0x3ff,  0x000,   1,      0,        1},  /* 6 */
	{"7-Blue",      0, 0x000, 0x000,  0x3ff,   1,      0,        1},  /* 7 */
	{"8-Black",     0, 0x000, 0x000,  0x000,   1,      0,        1},  /* 8 */
	{"9-X icon",    4, 0x200, 0x200,  0x200,   1,      0,        0},  /* 9 */
};

static int lcd_venc_pattern(struct aml_lcd_drv_s *pdrv, unsigned int num)
{
	unsigned int hstart, vstart, width, offset, height, ppc, step;

	if (num >= LCD_ENC_TST_NUM_MAX)
		return -1;

	offset = pdrv->data->offset_venc[pdrv->index];
	ppc = pdrv->config.timing.ppc;

	hstart = pdrv->config.timing.hstart / ppc - 1;
	vstart = pdrv->config.timing.vstart;
	height = pdrv->config.basic.v_active;

	if (num == 9) {
		width = pdrv->config.basic.h_active / ppc;
		step = width * 256 / height;
		lcd_vcbus_setb(ENCL_TST_DATA + offset, step, 10, 10);//px step
		lcd_vcbus_setb(ENCL_TST_DATA + offset, 0, 0, 10);//X line width
		lcd_vcbus_setb(ENCL_TST_DATA + offset, 0x200, 20, 10);//color Y/R
		lcd_vcbus_setb(ENCL_TST_EN + offset, 1, 12, 2);//rectangle width
		hstart -= 1;
	} else {
		width = pdrv->config.basic.h_active / ppc / 8 - 1;
		lcd_vcbus_setb(ENCL_TST_DATA + offset, lcd_enc_tst[num].y, 20, 10);//color Y/R
		lcd_vcbus_setb(ENCL_TST_DATA + offset, lcd_enc_tst[num].cb, 10, 10);//color cb/G
		lcd_vcbus_setb(ENCL_TST_DATA + offset, lcd_enc_tst[num].cr, 0, 10);//color cr/B
		hstart -= 2;
	}

	lcd_vcbus_setb(ENCL_TST_CLRBAR_CNTL + offset, hstart, 0, 13);//color bar x start
	lcd_vcbus_setb(ENCL_TST_CLRBAR_CNTL + offset, width, 16, 13);//color bar  width
	lcd_vcbus_setb(ENCL_TST_CRSBAR_CNTL + offset, vstart, 0, 13);//color bar y start
	lcd_vcbus_setb(ENCL_TST_CRSBAR_CNTL + offset, height, 16, 13);//color bar  height
	lcd_vcbus_setb(ENCL_TST_EN + offset, lcd_enc_tst[num].mode, 0, 8);//encl_test_model_sel
	lcd_vcbus_setb(ENCL_TST_EN + offset, lcd_enc_tst[num].en, 8, 1);//enable

	lcd_vcbus_setb(ENCL_VIDEO_VSRC_CTRL + offset, lcd_enc_tst[num].vfifo_en, 4, 1);
	/* 1: rgb to rgb   0: yuv to rgb) */
	lcd_vcbus_setb(ENCL_VIDEO_MODE + offset, lcd_enc_tst[num].rgb_in, 16,  1);

	if (num > 0) {
		LCDPR("[%d]: show test pattern: %s\n",
		      pdrv->index, lcd_enc_tst[num].name);
	}

	return 0;
}

static int lcd_venc_debug_test(struct aml_lcd_drv_s *pdrv, unsigned int num)
{
	lcd_venc_wait_vsync(pdrv);
	return lcd_venc_pattern(pdrv, num);
}

static void lcd_venc_set_timing(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int h_period, v_period, hstart, hend, vstart, vend;
	unsigned int hs_hs_addr, hs_he_addr, vs_hs_addr, vs_he_addr, vs_vs_addr, vs_ve_addr;
	unsigned int offset;
	unsigned int pre_de_vs, pre_de_ve, pre_de_hs, pre_de_he;
	unsigned int ppc, slice, p2s_px_dly;
	unsigned int hde_px_bgn, hde_px_end;
	unsigned int vde_ln_bgn, vde_ln_end;
	unsigned int hso_px_bgn, hso_px_end;
	unsigned int vso_px_bgn, vso_px_end;
	unsigned int vso_ln_bgn, vso_ln_end;
	unsigned int hact, ht, vt;

	ppc = pconf->timing.ppc;//must check zero in init place
	offset = pdrv->data->offset_venc[pdrv->index];
	LCDPR("%s, ppc=%d\n", __func__, ppc);

	hstart = __cal_h_timing_to_reg(pconf->timing.hstart, ppc);
	hend = __cal_h_timing_to_reg(pconf->timing.hend + 1, ppc) - 1;
	hde_px_bgn = hstart;
	hde_px_end = hend;

	vstart = pconf->timing.vstart;
	vend = pconf->timing.vend;
	vde_ln_bgn = vstart;
	vde_ln_end = vend;

	h_period = pconf->basic.h_period;
	v_period = pconf->basic.v_period;
	ht = h_period / ppc;
	vt = v_period;
	hact = hde_px_end - hde_px_bgn + 1;

	hs_hs_addr = __cal_h_timing_to_reg(pconf->timing.hs_hs_addr, ppc);
	hso_px_bgn = hs_hs_addr;
	hs_he_addr = __cal_h_timing_to_reg(pconf->timing.hs_he_addr, ppc);
	hso_px_end = hs_he_addr;

	vs_hs_addr = __cal_h_timing_to_reg(pconf->timing.vs_hs_addr, ppc);
	vs_he_addr = __cal_h_timing_to_reg(pconf->timing.vs_he_addr, ppc);
	vso_px_bgn = vs_hs_addr;
	vso_px_end = vs_he_addr;

	vs_vs_addr = pconf->timing.vs_vs_addr;
	vs_ve_addr = pconf->timing.vs_ve_addr;
	vso_ln_bgn = vs_vs_addr;
	vso_ln_end = vs_ve_addr;

	lcd_vcbus_write(ENCL_VIDEO_MODE + offset, 0x10040);
	lcd_vcbus_setb(ENCL_VIDEO_VSRC_CTRL + offset, 1, 4, 1);//reg_vfifo_en
	/*vfifo_upmode, 0:1ppc,1:2ppc,2, 4ppc */
	lcd_vcbus_setb(ENCL_VIDEO_VSRC_CTRL + offset, 0, 0, 3);

	lcd_vcbus_setb(ENCL_VIDEO_MAX_CNT + offset, h_period / ppc - 1, 16, 16);
	lcd_vcbus_setb(ENCL_VIDEO_MAX_CNT + offset, v_period - 1, 0, 16);

	lcd_vcbus_setb(ENCL_VIDEO_HAVON_PX_RNG + offset, hstart, 16, 16);//HAVON_BEGIN
	lcd_vcbus_setb(ENCL_VIDEO_HAVON_PX_RNG + offset, hend, 0, 16);//HAVON_END

	lcd_vcbus_setb(ENCL_VIDEO_VAVON_LN_RNG + offset, vstart, 16, 16);//BLINE
	lcd_vcbus_setb(ENCL_VIDEO_VAVON_LN_RNG + offset, vend, 0, 16);//ELINE

	lcd_vcbus_setb(ENCL_VIDEO_HSO_PX_RNG + offset, hs_hs_addr, 16, 16);//HSO_BEGIN
	lcd_vcbus_setb(ENCL_VIDEO_HSO_PX_RNG + offset, hs_he_addr, 0, 16);//HSO_END

	lcd_vcbus_setb(ENCL_VIDEO_VSO_PX_RNG + offset, vs_hs_addr, 16, 16);//HSO_BEGIN
	lcd_vcbus_setb(ENCL_VIDEO_VSO_PX_RNG + offset, vs_he_addr, 0, 16);//HSO_END

	lcd_vcbus_setb(ENCL_VIDEO_VSO_LN_RNG + offset, vs_vs_addr, 16, 16);//VSO BLINE
	lcd_vcbus_setb(ENCL_VIDEO_VSO_LN_RNG + offset, vs_ve_addr, 0, 16);//VSO ELINE

	if (pconf->basic.lcd_type == LCD_P2P ||
	    pconf->basic.lcd_type == LCD_MLVDS) {
		pre_de_vs = vstart - 16;
		pre_de_ve = pconf->basic.v_active + pre_de_vs;
		pre_de_hs = hstart + 6;
		pre_de_he = pconf->basic.h_active + pre_de_hs;

		lcd_vcbus_setb(ENCL_VIDEO_V_PRE_DE_LN_RNG + offset, pre_de_vs, 16, 16);
		lcd_vcbus_setb(ENCL_VIDEO_V_PRE_DE_LN_RNG + offset, pre_de_ve, 0, 16);
		lcd_vcbus_setb(ENCL_VIDEO_H_PRE_DE_PX_RNG + offset, pre_de_hs / ppc, 16, 16);
		lcd_vcbus_setb(ENCL_VIDEO_H_PRE_DE_PX_RNG + offset,   (pre_de_he / ppc) - 1, 0, 16);
	} else if (pconf->basic.lcd_type == LCD_VBYONE) {
		slice = ppc;
		p2s_px_dly = hact  + ((slice == 2) ? hact / 2 : 0);// p2s_hm_dly + hact;

		vde_ln_bgn = __lcd_round_inc(vde_ln_bgn, (hde_px_bgn + p2s_px_dly) / ht, vt);
		vde_ln_end = __lcd_round_inc(vde_ln_end, (hde_px_end + p2s_px_dly) / ht, vt);
		hde_px_bgn = __lcd_round_inc(hde_px_bgn, p2s_px_dly, ht);
		hde_px_end = __lcd_round_inc(hde_px_end + 1, p2s_px_dly, ht);
		hso_px_bgn = __lcd_round_inc(hso_px_bgn, p2s_px_dly, ht);
		hso_px_end = __lcd_round_inc(hso_px_end, p2s_px_dly, ht);
		vso_ln_bgn = __lcd_round_inc(vso_ln_bgn, (vso_px_bgn + p2s_px_dly) / ht, vt);
		vso_ln_end = __lcd_round_inc(vso_ln_end, (vso_px_end + p2s_px_dly) / ht, vt);
		vso_px_bgn = __lcd_round_inc(vso_px_bgn, p2s_px_dly, ht);
		vso_px_end = __lcd_round_inc(vso_px_end, p2s_px_dly, ht);

		lcd_vcbus_setb(ENCL_VIDEO_H_PRE_DE_PX_RNG + offset, hde_px_bgn, 16, 16);
		lcd_vcbus_setb(ENCL_VIDEO_H_PRE_DE_PX_RNG + offset, hde_px_end, 0, 16);

		lcd_vcbus_setb(ENCL_VIDEO_V_PRE_DE_LN_RNG + offset, vde_ln_bgn, 16, 16);
		lcd_vcbus_setb(ENCL_VIDEO_V_PRE_DE_LN_RNG + offset, vde_ln_end, 0, 16);

		lcd_vcbus_setb(ENCL_VIDEO_HSO_PRE_PX_RNG + offset, hso_px_bgn, 16, 16);
		lcd_vcbus_setb(ENCL_VIDEO_HSO_PRE_PX_RNG + offset, hso_px_end, 0, 16);

		lcd_vcbus_setb(ENCL_VIDEO_VSO_PRE_PX_RNG + offset, vso_px_bgn, 16, 16);
		lcd_vcbus_setb(ENCL_VIDEO_VSO_PRE_PX_RNG + offset, vso_px_end, 0, 16);

		lcd_vcbus_setb(ENCL_VIDEO_VSO_PRE_LN_RNG + offset, vso_ln_bgn, 16, 16);
		lcd_vcbus_setb(ENCL_VIDEO_VSO_PRE_LN_RNG + offset, vso_ln_end, 0, 16);
	} else if (pconf->basic.lcd_type == LCD_LVDS) {
		lcd_vcbus_setb(LCD_LCD_IF_CTRL + offset, 1, 28, 3);//rgb pol
	}

	switch (pdrv->data->chip_type) {
	case LCD_CHIP_T3X:
		lcd_vcbus_write(ENCL_INBUF_CNTL1 + offset, (4 << 13) | (pconf->basic.h_active - 1));
		lcd_vcbus_write(ENCL_INBUF_CNTL0 + offset, 0x200);
		break;
	default:
		break;
	}
}

static void lcd_venc_set(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg_disp_viu_ctrl, offset, offset1;
	unsigned int frm_hsize, reg_data;//, vpp0_ppc_num;
	//unsigned int ppc;
	struct lcd_config_s *pconf = &pdrv->config;

	if (!pconf) {
		LCDERR("null pconf\n");
		return;
	}
	if (!pdrv || !pdrv->data) {
		LCDERR("null pdrv\n");
		return;
	}
	//ppc = pconf->timing.ppc;
	offset = pdrv->data->offset_venc[pdrv->index];

	lcd_vcbus_write(ENCL_VIDEO_EN + offset, 0);
	lcd_venc_set_timing(pdrv);
	lcd_venc_pattern(pdrv, 0);

	if (pdrv->index == 0) {
		reg_data  = lcd_vcbus_read(ENCL_VIDEO_HAVON_PX_RNG);
		frm_hsize = (reg_data & 0xFFFF) - ((reg_data >> 16) & 0xFFFF) + 1;
		//vpp0_ppc_num  = lcd_vcbus_read(VPP_4S4P_CTRL) & 3;	//2: 1ppc, 1: 2ppc, 0: 4ppc
		//s2p enable, only for venc0
		//lcd_vcbus_setb(VPU_VENC_DITH, vpp0_ppc_num == 1 && ppc == 2, 31, 1);
		lcd_vcbus_setb(VPU_VENC_DITH, frm_hsize, 0,  13); //hsize
	}

	offset1 = pdrv->index == 1 ? 0x600 : 0x0;
	offset1 <<= 2;
	lcd_vcbus_setb(VPU_VENC_CTRL + offset1, 2, 0, 2);// enable timming
	lcd_vcbus_setb(ENCL_VIDEO_EN + offset, 1, 0, 1);//enable

	lcd_vcbus_setb(LCD_LCD_IF_CTRL + offset1, (0x400 << 12), 0, 23);
	lcd_vcbus_write(LCD_DITH_CTRL + offset1,   0);

	switch (pdrv->index) {
	case 0:
		reg_disp_viu_ctrl = VPU_DISP_VIU0_CTRL;
		break;
	case 1:
		reg_disp_viu_ctrl = VPU_DISP_VIU1_CTRL;
		break;
	case 2:
		reg_disp_viu_ctrl = VPU_DISP_VIU2_CTRL;
		break;
	default:
		LCDERR("[%d]: %s: invalid drv_index\n",
			pdrv->index, __func__);
		return;
	}

	/*
	 * bit31: lvds enable
	 * bit30: vx1 enable
	 * bit29: hdmitx enable
	 * bit28: dsi_edp enable
	 */
	switch (pdrv->config.basic.lcd_type) {
	case LCD_LVDS:
		lcd_vcbus_write(reg_disp_viu_ctrl, (1 << 31) |
						(0 << 30) |
						(0 << 29) |
						(0 << 28));
		break;
	case LCD_VBYONE:
		lcd_vcbus_write(reg_disp_viu_ctrl, (0 << 31) |
						(1 << 30) |
						(0 << 29) |
						(0 << 28));
		break;
	case LCD_MIPI:
	case LCD_EDP:
		lcd_vcbus_write(reg_disp_viu_ctrl, (0 << 31) |
						(0 << 30) |
						(0 << 29) |
						(1 << 28));
		break;
	default:
		break;
	}
}

static void lcd_venc_enable_ctrl(struct aml_lcd_drv_s *pdrv, int flag)
{
	unsigned int offset;

	offset = pdrv->data->offset_venc[pdrv->index];
	if (flag)
		lcd_vcbus_setb(ENCL_VIDEO_EN + offset, 1, 0, 1);
	else
		lcd_vcbus_setb(ENCL_VIDEO_EN + offset, 0, 0, 1);
}

static void lcd_venc_mute_set(struct aml_lcd_drv_s *pdrv, unsigned char flag)
{
	lcd_venc_wait_vsync(pdrv);
	if (flag)
		lcd_venc_debug_test(pdrv, 9);
	else
		lcd_venc_debug_test(pdrv, 0);
}

int lcd_venc_op_init_t3x(struct lcd_venc_op_s *venc_op)
{
	if (!venc_op)
		return -1;

	LCDPR("%s\n", __func__);
	venc_op->wait_vsync = lcd_venc_wait_vsync;
	venc_op->get_max_lcnt = lcd_venc_get_max_lint_cnt;
	venc_op->venc_debug_test = lcd_venc_debug_test;
	venc_op->venc_set_timing = lcd_venc_set_timing;
	venc_op->venc_set = lcd_venc_set;
	venc_op->venc_enable = lcd_venc_enable_ctrl;
	venc_op->mute_set = lcd_venc_mute_set;

	return 0;
};
