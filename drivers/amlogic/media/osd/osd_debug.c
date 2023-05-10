// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

/* System Headers */
#include <config.h>
#include <common.h>
#include <malloc.h>
#include <asm/arch/io.h>
#include <amlogic/cpu_id.h>
#include <asm/arch/cpu.h>

/* Local Headers */
#ifdef CONFIG_AML_CANVAS
#include <amlogic/canvas.h>
#endif

/* Local Headers */
#include "osd_log.h"
#include "osd_io.h"
#include "osd_hw.h"

#define OSD_TEST_DURATION 500

#define msleep(a) mdelay(a)
#define REG_INDEX_VCBUS(reg)  (u32)((reg) & REG_BASE_VCBUS ? ((reg - REG_BASE_VCBUS) >> 2) : reg)

extern struct hw_para_s osd_hw;

#ifdef AML_S5_DISPLAY
static u32 get_reg_slice_vpost(int reg_addr, int slice_idx)
{
	u32 reg_offset;
	u32 reg_addr_tmp;

	reg_offset = slice_idx == 0 ? 0 :
		slice_idx == 1 ? 0x100 :
		slice_idx == 2 ? 0x700 : 0x1900;
	reg_addr_tmp = reg_addr + reg_offset;
	return reg_addr_tmp;
}

static void dump_vpp_blend_reg(void)
{
	u32 reg_addr, reg_val = 0;

	osd_logi("vpp blend regs:\n");
	reg_addr = REG_INDEX_VCBUS(VPP_OSD1_BLD_H_SCOPE);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_OSD1_BLD_V_SCOPE);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_OSD2_BLD_H_SCOPE);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_OSD2_BLD_V_SCOPE);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);

	reg_addr = REG_INDEX_VCBUS(VPP_POSTBLEND_VD1_H_START_END);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POSTBLEND_VD1_V_START_END);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POSTBLEND_VD2_H_START_END);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POSTBLEND_VD2_V_START_END);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POSTBLEND_VD3_H_START_END);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POSTBLEND_VD3_V_START_END);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POSTBLND_H_V_SIZE);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POST_BLEND_BLEND_DUMMY_DATA);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POST_BLEND_DUMMY_ALPHA);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POST_BLEND_DUMMY_ALPHA1);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);

	reg_addr = REG_INDEX_VCBUS(VD1_BLEND_SRC_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VD2_BLEND_SRC_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VD3_BLEND_SRC_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(OSD1_BLEND_SRC_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(OSD2_BLEND_SRC_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VD1_POSTBLEND_ALPHA);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VD2_POSTBLEND_ALPHA);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VD3_POSTBLEND_ALPHA);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POSTBLND_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
}

static void dump_vpp_post_misc_reg(void)
{
	int i;
	u32 reg_addr, reg_val = 0;

	osd_logi("vpp post misc regs:\n");
	reg_addr = REG_INDEX_VCBUS(VPP_POSTBLND_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_OBUF_RAM_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_4P4S_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_4S4P_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POST_VD1_WIN_CUT_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POST_WIN_CUT_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POST_PAD_HSIZE);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	reg_addr = REG_INDEX_VCBUS(VPP_POST_PAD_CTRL);
	reg_val = osd_reg_read(reg_addr);
	osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
	for (i = 0; i < SLICE_NUM; i++) {
		reg_addr = REG_INDEX_VCBUS(VPP_OUT_H_V_SIZE);
		reg_addr = get_reg_slice_vpost(reg_addr, i);
		reg_val = osd_reg_read(reg_addr);
		osd_logi("reg[0x%x]: 0x%08x\n",
		   reg_addr, reg_val);
		reg_addr = REG_INDEX_VCBUS(VPP_OFIFO_SIZE);
		reg_addr = get_reg_slice_vpost(reg_addr, i);
		reg_val = osd_reg_read(reg_addr);
		osd_logi("reg[0x%x]: 0x%08x\n",
			   reg_addr, reg_val);
		reg_addr = REG_INDEX_VCBUS(VPP_SLC_DEAL_CTRL);
		reg_addr = get_reg_slice_vpost(reg_addr, i);
		reg_val = osd_reg_read(reg_addr);
		osd_logi("reg[0x%x]: 0x%08x\n",
			   reg_addr, reg_val);
		reg_addr = REG_INDEX_VCBUS(VPP_HWIN_SIZE);
		reg_addr = get_reg_slice_vpost(reg_addr, i);
		reg_val = osd_reg_read(reg_addr);
		osd_logi("reg[0x%x]: 0x%08x\n",
			   reg_addr, reg_val);
		reg_addr = REG_INDEX_VCBUS(VPP_ALIGN_FIFO_SIZE);
		reg_addr = get_reg_slice_vpost(reg_addr, i);
		reg_val = osd_reg_read(reg_addr);
		osd_logi("reg[0x%x]: 0x%08x\n",
			   reg_addr, reg_val);
	}
}

static void dump_vpp_post_reg(void)
{
	dump_vpp_blend_reg();
	dump_vpp_post_misc_reg();
}
#endif

static void osd_debug_dump_value(void)
{
	u32 index = 0;
	struct hw_para_s *hwpara = NULL;
	struct pandata_s *pdata = NULL;

	osd_get_hw_para(&hwpara);
	if (hwpara == NULL)
		return;

	osd_logi("--- OSD ---\n");
	osd_logi("scan_mode: %s\n", (hwpara->scan_mode == SCAN_MODE_INTERLACE) ? "interlace" : "progressive");
	osd_logi("order: %d\n", hwpara->order);
	osd_logi("bot_type: %d\n", hwpara->bot_type);
	osd_logi("field_out_en: %d\n", hwpara->field_out_en);

	for (index = 0; index < HW_OSD_COUNT; index++) {
		osd_logi("\n--- OSD%d ---\n", index);
		osd_logi("enable: %d\n", hwpara->enable[index]);
		osd_logi("2x-scale enable.h:%d .v: %d\n",
			 hwpara->scale[index].h_enable,
			 hwpara->scale[index].v_enable);
		osd_logi("free-scale-mode: %d\n",
			 hwpara->free_scale_mode[index]);
		osd_logi("free-scale enable.h:%d .v: %d\n",
			 hwpara->free_scale[index].h_enable,
			 hwpara->free_scale[index].v_enable);
		pdata = &hwpara->pandata[index];
		osd_logi("pan data:\n");
		osd_logi("\tx_start: %10d, x_end: %10d\n",
			 pdata->x_start, pdata->x_end);
		osd_logi("\ty_start: %10d, y_end: %10d\n",
			 pdata->y_start, pdata->y_end);

		pdata = &hwpara->dispdata[index];
		osd_logi("disp data:\n");
		osd_logi("\tx_start: 0x%08x, x_end: 0x%08x\n",
			 pdata->x_start, pdata->x_end);
		osd_logi("\ty_start: 0x%08x, y_end: 0x%08x\n",
			 pdata->y_start, pdata->y_end);

		pdata = &hwpara->scaledata[index];
		osd_logi("2x-scale data:\n");
		osd_logi("\tx_start: 0x%08x, x_end: 0x%08x\n",
			 pdata->x_start, pdata->x_end);
		osd_logi("\ty_start: 0x%08x, y_end: 0x%08x\n",
			 pdata->y_start, pdata->y_end);

		pdata = &hwpara->free_scale_data[index];
		osd_logi("free-scale src data:\n");
		osd_logi("\tx_start: 0x%08x, x_end: 0x%08x\n",
			 pdata->x_start, pdata->x_end);
		osd_logi("\ty_start: 0x%08x, y_end: 0x%08x\n",
			 pdata->y_start, pdata->y_end);

		pdata = &hwpara->free_dst_data[index];
		osd_logi("free-scale dst data:\n");
		osd_logi("\tx_start: 0x%08x, x_end: 0x%08x\n",
			 pdata->x_start, pdata->x_end);
		osd_logi("\ty_start: 0x%08x, y_end: 0x%08x\n",
			 pdata->y_start, pdata->y_end);
	}
}

static void osd_debug_dump_register_all(void)
{
	u32 reg = 0;
	u32 index = 0;

	reg = VPU_VIU_VENC_MUX_CTRL;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = VPP_MISC;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = VPP_OFIFO_SIZE;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));

#ifndef AML_S5_DISPLAY
	reg = VPP_HOLD_LINES;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
#endif
#ifdef AML_S5_DISPLAY
	reg = VPP_INTF_OSD3_CTRL;
	osd_logi("reg[0x%x]: 0x%08x(2mux1)\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_PROC_1MUX3_SEL;
	osd_logi("reg[0x%x]: 0x%08x(1mux3)\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_PI_BYPASS_EN;
	osd_logi("reg[0x%x]: 0x%08x(PI)\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_SYS_5MUX4_SEL;
	osd_logi("reg[0x%x]: 0x%08x(5mux4)\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
#endif
	if (osd_hw.osd_ver == OSD_HIGH_ONE) {
#ifdef AML_OSD_HIGH_VERSION
	#ifndef AML_S5_DISPLAY
		reg = OSD_PATH_MISC_CTRL;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	#endif
		reg = VIU_OSD_BLEND_CTRL;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_DIN0_SCOPE_H;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_DIN0_SCOPE_V;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_DIN1_SCOPE_H;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_DIN1_SCOPE_V;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_DIN2_SCOPE_H;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_DIN2_SCOPE_V;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_DIN3_SCOPE_H;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_DIN3_SCOPE_V;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_DUMMY_DATA0;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_DUMMY_ALPHA;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_BLEND0_SIZE;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VIU_OSD_BLEND_BLEND1_SIZE;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));

#ifndef AML_S5_DISPLAY
		reg = VPP_OSD1_IN_SIZE;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
#else
		reg = OSD_BLEND_DOUT0_SIZE;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = OSD_BLEND_DOUT1_SIZE;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
#endif
		reg = VPP_OSD1_BLD_H_SCOPE;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VPP_OSD1_BLD_V_SCOPE;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VPP_OSD2_BLD_H_SCOPE;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VPP_OSD2_BLD_V_SCOPE;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = OSD1_BLEND_SRC_CTRL;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = OSD2_BLEND_SRC_CTRL;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VPP_POSTBLEND_H_SIZE;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = VPP_OUT_H_V_SIZE;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
#endif
	}
	reg = hw_osd_reg_array[0].osd_sc_ctrl0;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = hw_osd_reg_array[0].osd_sci_wh_m1;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = hw_osd_reg_array[0].osd_sco_h_start_end;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = hw_osd_reg_array[0].osd_sco_v_start_end;
	osd_logi("reg[0x%x]: 0x%08x\n\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = VPP_POSTBLEND_H_SIZE;
	osd_logi("reg[0x%x]: 0x%08x\n\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	for (index = 0; index < 2; index++) {
		reg = hw_osd_reg_array[index].osd_fifo_ctrl_stat;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[index].osd_ctrl_stat;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[index].osd_blk0_cfg_w0;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[index].osd_blk0_cfg_w1;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[index].osd_blk0_cfg_w2;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[index].osd_blk0_cfg_w3;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[index].osd_blk0_cfg_w4;
		osd_logi("reg[0x%x]: 0x%08x\n\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	}

#ifdef AML_T7_DISPLAY
	if (!strcmp(env_get("display_layer"), "osd2") ||
	    !strcmp(env_get("display_layer"), "viu2_osd0")) {
		reg = hw_osd_reg_array[2].osd_sc_ctrl0;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_sci_wh_m1;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_sco_h_start_end;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_sco_v_start_end;
		osd_logi("reg[0x%x]: 0x%08x\n\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));

		reg = hw_osd_reg_array[2].osd_fifo_ctrl_stat;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_ctrl_stat;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_blk0_cfg_w0;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_blk0_cfg_w1;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_blk0_cfg_w2;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_blk0_cfg_w3;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_blk0_cfg_w4;
		osd_logi("reg[0x%x]: 0x%08x\n\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	}
	if (!strcmp(env_get("display_layer"), "osd3") ||
	    !strcmp(env_get("display_layer"), "viu3_osd0")) {
		reg = hw_osd_reg_array[3].osd_sc_ctrl0;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[3].osd_sci_wh_m1;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[3].osd_sco_h_start_end;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[3].osd_sco_v_start_end;
		osd_logi("reg[0x%x]: 0x%08x\n\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));

		reg = hw_osd_reg_array[3].osd_fifo_ctrl_stat;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[3].osd_ctrl_stat;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[3].osd_blk0_cfg_w0;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[3].osd_blk0_cfg_w1;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[3].osd_blk0_cfg_w2;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[3].osd_blk0_cfg_w3;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[3].osd_blk0_cfg_w4;
		osd_logi("reg[0x%x]: 0x%08x\n\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	}

#endif

#ifdef CONFIG_AML_MESON_G12A
	/* for viu2_osd */
	if (!strcmp(env_get("display_layer"), "viu2_osd0")) {
		reg = hw_osd_reg_array[2].osd_fifo_ctrl_stat;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_ctrl_stat;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_blk0_cfg_w0;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_blk0_cfg_w1;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_blk0_cfg_w2;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_blk0_cfg_w3;
		osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
		reg = hw_osd_reg_array[2].osd_blk0_cfg_w4;
		osd_logi("reg[0x%x]: 0x%08x\n\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	}
#endif

#ifdef AML_S5_DISPLAY
	osd_logi("--- slice2ppc ---\n");
	reg = OSD_2SLICE2PPC_IN_SIZE;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_2SLICE2PPC_MODE;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_SYS_HWIN0_CUT;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_SYS_HWIN1_CUT;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_SYS_PAD_CTRL;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_SYS_PAD_DUMMY_DATA0;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_SYS_PAD_DUMMY_DATA1;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_SYS_PAD_H_SIZE;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_SYS_PAD_V_SIZE;
	osd_logi("reg[0x%x]: 0x%08x\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));
	reg = OSD_SYS_2SLICE_HWIN_CUT;
	osd_logi("reg[0x%x]: 0x%08x\n\n", REG_INDEX_VCBUS(reg), osd_reg_read(reg));

	dump_vpp_post_reg();
#endif
}

static void osd_test_colorbar(void)
{
	u32 encp_video_adv = 0;

	encp_video_adv = osd_reg_read(ENCP_VIDEO_MODE_ADV);

	/* start test mode */
	osd_logi("--- OSD TEST COLORBAR ---\n");
	osd_reg_write(ENCP_VIDEO_MODE_ADV, 0);
	osd_reg_write(VENC_VIDEO_TST_EN, 1);
	/* TST_MODE COLORBAR */
	osd_logi("- COLORBAR -\n");
	osd_reg_write(VENC_VIDEO_TST_MDSEL, 1);
	msleep(OSD_TEST_DURATION);

	/* TST_MODE THINLINE */
	osd_logi("- THINLINE -\n");
	osd_reg_write(VENC_VIDEO_TST_MDSEL, 2);
	msleep(OSD_TEST_DURATION);
	/* TST_MODE DOTGRID */
	osd_logi("- DOTGRID -\n");
	osd_reg_write(VENC_VIDEO_TST_MDSEL, 3);
	msleep(OSD_TEST_DURATION);

	/* stop test mode */
	osd_reg_write(ENCP_VIDEO_MODE_ADV, encp_video_adv);
	osd_reg_write(VENC_VIDEO_TST_EN, 0);
	osd_reg_write(VENC_VIDEO_TST_MDSEL, 0);
}

static void osd_reset(void)
{
	osd_set_free_scale_enable_hw(0, 0);
	osd_enable_hw(0, 1);
}

static void osd_test_dummydata(void)
{
	u32 dummy_data = 0;

	dummy_data = osd_reg_read(VPP_DUMMY_DATA1);
	osd_reset();
	osd_logi("--- OSD TEST DUMMYDATA ---\n");
	if (osd_hw.osd_ver == OSD_HIGH_ONE) {
#ifdef AML_OSD_HIGH_VERSION
		osd_reg_write(VPP_POST_BLEND_BLEND_DUMMY_DATA, 0xff);
		msleep(OSD_TEST_DURATION);
		osd_reg_write(VPP_POST_BLEND_BLEND_DUMMY_DATA, 0xff00);
		msleep(OSD_TEST_DURATION);
		osd_reg_write(VPP_POST_BLEND_BLEND_DUMMY_DATA, 0xff0000);
		msleep(OSD_TEST_DURATION);
		osd_reg_write(VPP_POST_BLEND_BLEND_DUMMY_DATA, dummy_data);
		msleep(OSD_TEST_DURATION);
#endif
	} else if (get_cpu_id().family_id ==
		MESON_CPU_MAJOR_ID_GXTVBB) {
		osd_reg_write(VPP_DUMMY_DATA1, 0xFF0000); /* R */
		msleep(OSD_TEST_DURATION);
		osd_reg_write(VPP_DUMMY_DATA1, 0xFF00); /* G */
		msleep(OSD_TEST_DURATION);
		osd_reg_write(VPP_DUMMY_DATA1, 0xFF); /* B */
		msleep(OSD_TEST_DURATION);
		osd_reg_write(VPP_DUMMY_DATA1, dummy_data);
		msleep(OSD_TEST_DURATION);

	} else {
		osd_reg_write(VPP_DUMMY_DATA1, 0xFF);
		msleep(OSD_TEST_DURATION);
		osd_reg_write(VPP_DUMMY_DATA1, 0);
		msleep(OSD_TEST_DURATION);
		osd_reg_write(VPP_DUMMY_DATA1, 0xFF00);
		msleep(OSD_TEST_DURATION);
		osd_reg_write(VPP_DUMMY_DATA1, dummy_data);
		msleep(OSD_TEST_DURATION);
	}
}

static void osd_debug_auto_test(void)
{
	osd_test_colorbar();

	osd_test_dummydata();
}

void osd_debug(void)
{
	osd_debug_dump_value();
	osd_logi("\n");
	osd_debug_dump_register_all();
}

void osd_test(void)
{
	osd_debug_auto_test();
}
