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

static unsigned int lcd_dth_lut_c3[16] = {
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x82412814, 0x48122481, 0x18242184, 0x18242841,
	0x9653ca56, 0x6a3ca635, 0x6ca9c35a, 0xca935635,
	0x7dbedeb7, 0xde7dbed7, 0xeb7dbed7, 0xdbe77edb
};

static void lcd_venc_set(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int reoder, timgen_mode, serial_rate, field_mode, dith_mode;
	unsigned int bot_bgn_lne, top_bgn_lne;
	unsigned int vs_lne_bgn_o, vs_lne_end_o, vs_pix_bgn_o, vs_pix_end_o;
	unsigned int de_lne_bgn_o, de_lne_end_o;
	int i;
	unsigned int rb_swap;

	rb_swap = pdrv->config.control.rgb_cfg.rb_swap & 1;
	reoder = 0x24;
	if (rb_swap)
		reoder = 0x06;

	//timgen_mode:
	//1<<0:MIPI_TX, 1<<1:LCDs8,  1<<2:BT1120,  1<<3:BT656, 1<<4:lcds6,
	//1<<5:lcdp6, 1<<6:lcdp8, 1<<7:lcd565, 1<<8:lcds9(6+3,3+6), 1<<9:lcds8(5+3,3+5)

	//serial_rate:
	//0:pix/1cycle    1:pix/2cycle  2:pix/3cycle

	//field_mode:
	//0:progress 1:interlace

	switch (pconf->basic.lcd_type) {
	case LCD_MIPI:
		timgen_mode = (1 << 0);
		serial_rate = 0;
		field_mode = 0;
		dith_mode = 0; //10bit to 8bit
		break;
	case LCD_BT656:
		timgen_mode = (1 << 3);
		serial_rate = 1;
		field_mode = 1;
		dith_mode = 0; //10bit to 8bit
		break;
	case LCD_BT1120:
		timgen_mode = (1 << 2);
		serial_rate = 0;
		field_mode = 1;
		dith_mode = 0; //10bit to 8bit
		break;
	case LCD_RGB:
	default:
		timgen_mode = (1 << 5);
		serial_rate = 0;
		field_mode = 0;
		dith_mode = 1; /*10bit to 6bit*/
		break;
	}

	bot_bgn_lne  =    0;
	top_bgn_lne  =    0;
	vs_lne_bgn_o =    0;
	vs_lne_end_o =    0;
	vs_pix_bgn_o =    0;
	vs_pix_end_o =    0;
	de_lne_bgn_o =    0;
	de_lne_end_o =    0;

	lcd_vcbus_setb(VPU_VOUT_CORE_CTRL, 0, 0, 1); //disable venc_en
	lcd_vcbus_setb(VPU_VOUT_DETH_CTRL, dith_mode, 5, 1);
	lcd_vcbus_setb(VPU_VOUT_DETH_CTRL, pconf->basic.h_active, 6, 13);
	lcd_vcbus_setb(VPU_VOUT_DETH_CTRL, pconf->basic.v_active, 19, 13);
	lcd_vcbus_setb(VPU_VOUT_INT_CTRL, 1, 14, 1); //dth_en

	lcd_vcbus_setb(VPU_VOUT_CORE_CTRL, reoder, 4, 6);
	lcd_vcbus_setb(VPU_VOUT_CORE_CTRL, timgen_mode, 16, 10);
	lcd_vcbus_setb(VPU_VOUT_CORE_CTRL, serial_rate, 2, 2);

	lcd_vcbus_write(VPU_VOUT_DTH_ADDR, 0);
	for (i = 0; i < 32; i++)
		lcd_vcbus_write(VPU_VOUT_DTH_DATA, lcd_dth_lut_c3[i % 16]);

	lcd_vcbus_setb(VPU_VOUT_CORE_CTRL,    field_mode,  1, 1);
	lcd_vcbus_setb(VPU_VOUT_MAX_SIZE,     pconf->basic.h_period, 16, 13);
	lcd_vcbus_setb(VPU_VOUT_MAX_SIZE,     pconf->basic.v_period,  0, 13);
	lcd_vcbus_setb(VPU_VOUT_FLD_BGN_LINE, bot_bgn_lne, 16, 13);
	lcd_vcbus_setb(VPU_VOUT_FLD_BGN_LINE, top_bgn_lne,  0, 13);

	lcd_vcbus_setb(VPU_VOUT_HS_POS,      pconf->timing.hs_hs_addr, 16, 13);
	lcd_vcbus_setb(VPU_VOUT_HS_POS,      pconf->timing.hs_he_addr,  0, 13);
	lcd_vcbus_setb(VPU_VOUT_VSLN_E_POS,  pconf->timing.vs_vs_addr, 16, 13);
	lcd_vcbus_setb(VPU_VOUT_VSLN_E_POS,  pconf->timing.vs_ve_addr,  0, 13);
	lcd_vcbus_setb(VPU_VOUT_VSPX_E_POS,  pconf->timing.vs_hs_addr, 16, 13);
	lcd_vcbus_setb(VPU_VOUT_VSPX_E_POS,  pconf->timing.vs_he_addr,  0, 13);
	lcd_vcbus_setb(VPU_VOUT_VSLN_O_POS,  vs_lne_bgn_o, 16, 13);
	lcd_vcbus_setb(VPU_VOUT_VSLN_O_POS,  vs_lne_end_o,  0, 13);
	lcd_vcbus_setb(VPU_VOUT_VSPX_O_POS,  vs_pix_bgn_o, 16, 13);
	lcd_vcbus_setb(VPU_VOUT_VSPX_O_POS,  vs_pix_end_o,  0, 13);

	lcd_vcbus_setb(VPU_VOUT_DELN_E_POS,  pconf->timing.vstart, 16, 13);
	lcd_vcbus_setb(VPU_VOUT_DELN_E_POS,  pconf->timing.vend,  0, 13);
	lcd_vcbus_setb(VPU_VOUT_DELN_O_POS,  de_lne_bgn_o, 16, 13);
	lcd_vcbus_setb(VPU_VOUT_DELN_O_POS,  de_lne_end_o,  0, 13);
	lcd_vcbus_setb(VPU_VOUT_DE_PX_EN,    pconf->timing.hstart, 16, 13);
	lcd_vcbus_setb(VPU_VOUT_DE_PX_EN,    pconf->timing.hend,  0, 13);

	//OSD reg required to be set by hai.cao
	lcd_vcbus_setb(VPU_VOUT_BLEND_SIZE, pconf->basic.h_active, 0, 12);
	lcd_vcbus_setb(VPU_VOUT_BLEND_SIZE, pconf->basic.v_active, 16, 12);

	lcd_vcbus_setb(VPU_VOUT_CORE_CTRL, 1, 0, 1); //venc_en
}

static void lcd_venc_enable_ctrl(struct aml_lcd_drv_s *pdrv, int flag)
{
	lcd_vcbus_setb(VPU_VOUT_CORE_CTRL, flag & 0x1, 0, 1);
}

#define LCD_ENC_TST_MAX_C3    6
#define LCD_ENC_TST_MAX_A4    10

static char *lcd_enc_tst_str[] = {
	"0-None",        /* 0 */
	"1-Color Bar",   /* 1 */
	"2-Thin Line",   /* 2 */
	"3-Dot Grid",    /* 3 */
	"4-Gray",        /* 4 */
	"5-Red",         /* 5 */
	"6-Green",       /* 6 */
	"7-Blue",        /* 7 */
	"8-Black",       /* 8 */
	"9-White",       /* 9 */
	"10-X Patten",   /* 10 */
};

static unsigned int lcd_enc_tst[][4] = {
/*   Y,     Cb,    Cr,    mode_sel */
	{0x200, 0x200, 0x200, 0},  /* 0 */
	{0x0,     0x0,   0x0, 1},  /* 1 */
	{0x0,     0x0,   0x0, 2},  /* 2 */
	{0x0,     0x0,   0x0, 3},  /* 3 */
	{0x1ff, 0x1ff, 0x1ff, 0},  /* 4 */
	{0x3ff,   0x0,   0x0, 0},  /* 5 */
	{  0x0, 0x3ff,   0x0, 0},  /* 6 */
	{  0x0,   0x0, 0x3ff, 0},  /* 7 */
	{  0x0,   0x0,   0x0, 0},  /* 8 */
	{0x3ff, 0x3ff, 0x3ff, 0},  /* 9 */
	{0x3ff,   0x0,   0x0, 4},  /* 10 */
};

static int lcd_venc_debug_test(struct aml_lcd_drv_s *pdrv, unsigned int num)
{
	unsigned int h_start, clb_width, ha, va;
	unsigned int edge_rect_line_width, center_X_line_width, K_frac, K_int;

	ha = pdrv->config.basic.h_active;
	va = pdrv->config.basic.v_active;

	if (pdrv->data->chip_type == LCD_CHIP_A4 && num <= LCD_ENC_TST_MAX_A4) {
		h_start = pdrv->config.timing.hstart;

		lcd_vcbus_setb(VPU_VOUT_BIST_SEL, lcd_enc_tst[num][3], 0, 8);
		switch (lcd_enc_tst[num][3]) {
		case 0: /* Pure Color */
		case 2: /* Thin Line  */
		case 3: /* Dot Grid   */
			lcd_vcbus_setb(VPU_VOUT_BIST_CFG_YUV, lcd_enc_tst[num][0], 20, 10);
			lcd_vcbus_setb(VPU_VOUT_BIST_CFG_YUV, lcd_enc_tst[num][1], 10, 10);
			lcd_vcbus_setb(VPU_VOUT_BIST_CFG_YUV, lcd_enc_tst[num][2], 0, 10);
			break;
		case 1:  /* Color Bar */
			clb_width = ha / 8;
			lcd_vcbus_setb(VPU_VOUT_CLRBAR_CFG, h_start, 16, 13);
			lcd_vcbus_setb(VPU_VOUT_CLRBAR_CFG, clb_width - 1, 0, 13);
			break;
		case 4: /* X patten */
			edge_rect_line_width = 6;
			center_X_line_width = 4;
				/* clrbar_width: total width */
			lcd_vcbus_setb(VPU_VOUT_CLRBAR_CFG, ha, 0, 13);
				/* clrbar_high: total high  */
			lcd_vcbus_setb(VPU_VOUT_BIST_HIGH, va, 0, 13);
				/* Cb: K - 1 */
			K_int = ha / va;
			K_frac = ((ha - va) * (1 << 8)) / va;
			lcd_vcbus_setb(VPU_VOUT_BIST_CFG_YUV, K_frac, 10, 8);
			lcd_vcbus_setb(VPU_VOUT_BIST_CFG_YUV, K_int, 18, 2);
				/* Cr: X-line h width */
			lcd_vcbus_setb(VPU_VOUT_BIST_CFG_YUV, center_X_line_width, 0, 5); //L
			lcd_vcbus_setb(VPU_VOUT_BIST_CFG_YUV, center_X_line_width, 5, 5); //R
				/* vdcnt_strtset: rect_line_w + 1 */
			lcd_vcbus_setb(VPU_VOUT_BIST_CFG_YUV, edge_rect_line_width, 30, 2);
				/* clrbar_strt: start H pos (de-1) */
			lcd_vcbus_setb(VPU_VOUT_CLRBAR_CFG, pdrv->config.timing.hstart, 16, 13);
				/* clrbar_vstrt: start V pos */
			lcd_vcbus_setb(VPU_VOUT_BIST_HIGH, pdrv->config.timing.vstart, 16, 13);
		}
		lcd_vcbus_setb(VPU_VOUT_INT_CTRL, num && 1, 15, 1);   /* bist_en */

		LCDPR("[%d]: show test pattern: %s\n", pdrv->index, lcd_enc_tst_str[num]);
		return 0;

	} else if (pdrv->data->chip_type == LCD_CHIP_C3 && num <= LCD_ENC_TST_MAX_C3) {
		lcd_vcbus_write(VPU_VOUT_BLEND_CTRL, num ? 0xc : 0x0);
		lcd_vcbus_setb(VPU_VOUT_BLEND_DUMDATA, lcd_enc_tst[num + 3][0], 20, 10);
		lcd_vcbus_setb(VPU_VOUT_BLEND_DUMDATA, lcd_enc_tst[num + 3][1], 10, 10);
		lcd_vcbus_setb(VPU_VOUT_BLEND_DUMDATA, lcd_enc_tst[num + 3][2], 0, 10);

		LCDPR("[%d]: show test pattern: %s\n", pdrv->index, lcd_enc_tst_str[num + 3]);
		return 0;
	}

	LCDERR("Test %d invalid\n", num);
	return -1;
}

static void lcd_venc_mute_set(struct aml_lcd_drv_s *pdrv, unsigned char flag)
{
	LCDPR("%s: todo\n", __func__);
}

int lcd_venc_op_init_c3(struct lcd_venc_op_s *venc_op)
{
	if (!venc_op)
		return -1;

	venc_op->wait_vsync = NULL;
	venc_op->venc_debug_test = lcd_venc_debug_test;
	venc_op->venc_set_timing = NULL;
	venc_op->venc_set = lcd_venc_set;
	venc_op->venc_enable = lcd_venc_enable_ctrl;
	venc_op->mute_set = lcd_venc_mute_set;

	return 0;
};
