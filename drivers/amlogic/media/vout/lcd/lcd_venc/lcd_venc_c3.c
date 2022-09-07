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

	reoder = 36;

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

	lcd_vcbus_setb(VPU_VOUT_CORE_CTRL, 1, 0, 1); //venc_en
}

static void lcd_venc_enable_ctrl(struct aml_lcd_drv_s *pdrv, int flag)
{
	if (flag)
		lcd_vcbus_setb(VPU_VOUT_CORE_CTRL, 1, 0, 1);
	else
		lcd_vcbus_setb(VPU_VOUT_CORE_CTRL, 0, 0, 1);
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
	venc_op->venc_debug_test = NULL;
	venc_op->venc_set_timing = NULL;
	venc_op->venc_set = lcd_venc_set;
	venc_op->venc_enable = lcd_venc_enable_ctrl;
	venc_op->mute_set = lcd_venc_mute_set;

	return 0;
};
