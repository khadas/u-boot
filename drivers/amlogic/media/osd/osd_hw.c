// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

/* System Headers */
#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <amlogic/cpu_id.h>
#include <asm/arch/cpu.h>
#include <asm/arch/timer.h>
#include <div64.h>

/* Local Headers */
#include <amlogic/media/vout/aml_vmode.h>
#ifdef CONFIG_AML_CANVAS
#include <amlogic/canvas.h>
#endif
#ifdef CONFIG_AML_VOUT
#include <amlogic/media/vout/aml_vout.h>
#endif
#include <amlogic/fb.h>
#include <video_fb.h>

/* Local Headers */
#include "osd_canvas.h"
#include "osd_log.h"
#include "osd_io.h"
#include "osd_hw.h"
#include "osd_hw_def.h"
#include "osd_fb.h"
#include "vpp.h"
#include <amlogic/media/dv/dolby_vision.h>

static bool vsync_hit;
static bool osd_vf_need_update;

unsigned int osd_log_level;
static unsigned int logo_loaded = 0;

#ifdef CONFIG_AM_VIDEO
static struct vframe_s vf;
static struct vframe_provider_s osd_vf_prov;
static unsigned char osd_vf_prov_init;
#endif
static int g_vf_visual_width;
static int g_vf_width;
static int g_vf_height;

static int g_rotation_width;
static int g_rotation_height;

static int use_h_filter_mode = -1;
static int use_v_filter_mode = -1;

static unsigned int osd_h_filter_mode = 1;
static unsigned int osd_v_filter_mode = 1;
extern GraphicDevice fb_gdev;

static void independ_path_default_regs(void);
static void fix_vpu_clk2_default_regs(void);

struct fb_layout_s fb_layout[OSD_MAX];

#ifdef AML_OSD_HIGH_VERSION
#define VIU2_OSD1_UNSUPPORT               VIU_OSD2_TCOLOR_AG3

#ifdef reg_define
struct hw_encp_reg_s hw_vout_reg = {
	ENCP_VIDEO_MODE,
	ENCP_INFO_READ,
	ENCP_VIDEO_VAVON_BLINE,
	ENCL_INFO_READ,
	ENCL_VIDEO_VAVON_BLINE,
	ENCI_INFO_READ,
	ENCI_VFIFO2VD_LINE_TOP_START,
	ENCT_INFO_READ,
	ENCT_VIDEO_VAVON_BLINE,
};
#endif

#ifndef AML_T7_DISPLAY
struct hw_osd_reg_s hw_osd_reg_array[HW_OSD_COUNT] = {
	{
		VIU_OSD1_CTRL_STAT,
		VIU_OSD1_CTRL_STAT2,
		VIU_OSD1_COLOR_ADDR,
		VIU_OSD1_COLOR,
		VIU_OSD1_TCOLOR_AG0,
		VIU_OSD1_TCOLOR_AG1,
		VIU_OSD1_TCOLOR_AG2,
		VIU_OSD1_TCOLOR_AG3,
		VIU_OSD1_BLK0_CFG_W0,
		VIU_OSD1_BLK0_CFG_W1,
		VIU_OSD1_BLK0_CFG_W2,
		VIU_OSD1_BLK0_CFG_W3,
		VIU_OSD1_BLK0_CFG_W4,
		VIU_OSD1_BLK1_CFG_W4,
		VIU_OSD1_BLK2_CFG_W4,
		VIU_OSD1_FIFO_CTRL_STAT,

		VPP_OSD_SCALE_COEF_IDX,
		VPP_OSD_SCALE_COEF,
		VPP_OSD_VSC_PHASE_STEP,
		VPP_OSD_VSC_INI_PHASE,
		VPP_OSD_VSC_CTRL0,
		VPP_OSD_HSC_PHASE_STEP,
		VPP_OSD_HSC_INI_PHASE,
		VPP_OSD_HSC_CTRL0,
		VPP_OSD_SC_DUMMY_DATA,
		VPP_OSD_SC_CTRL0,
		VPP_OSD_SCI_WH_M1,
		VPP_OSD_SCO_H_START_END,
		VPP_OSD_SCO_V_START_END,
	},
	{
		VIU_OSD2_CTRL_STAT,
		VIU_OSD2_CTRL_STAT2,
		VIU_OSD2_COLOR_ADDR,
		VIU_OSD2_COLOR,
		VIU_OSD2_TCOLOR_AG0,
		VIU_OSD2_TCOLOR_AG1,
		VIU_OSD2_TCOLOR_AG2,
		VIU_OSD2_TCOLOR_AG3,
		VIU_OSD2_BLK0_CFG_W0,
		VIU_OSD2_BLK0_CFG_W1,
		VIU_OSD2_BLK0_CFG_W2,
		VIU_OSD2_BLK0_CFG_W3,
		VIU_OSD2_BLK0_CFG_W4,
		VIU_OSD2_BLK1_CFG_W4,
		VIU_OSD2_BLK2_CFG_W4,
		VIU_OSD2_FIFO_CTRL_STAT,

		VPP_OSD_SCALE_COEF_IDX,
		VPP_OSD_SCALE_COEF,
		VPP_OSD_VSC_PHASE_STEP,
		VPP_OSD_VSC_INI_PHASE,
		VPP_OSD_VSC_CTRL0,
		VPP_OSD_HSC_PHASE_STEP,
		VPP_OSD_HSC_INI_PHASE,
		VPP_OSD_HSC_CTRL0,
		VPP_OSD_SC_DUMMY_DATA,
		VPP_OSD_SC_CTRL0,
		VPP_OSD_SCI_WH_M1,
		VPP_OSD_SCO_H_START_END,
		VPP_OSD_SCO_V_START_END,
	},
	{
		VIU2_OSD1_CTRL_STAT,
		VIU2_OSD1_CTRL_STAT2,
		VIU2_OSD1_COLOR_ADDR,
		VIU2_OSD1_COLOR,
		VIU2_OSD1_TCOLOR_AG0,
		VIU2_OSD1_TCOLOR_AG1,
		VIU2_OSD1_TCOLOR_AG2,
		VIU2_OSD1_TCOLOR_AG3,
		VIU2_OSD1_BLK0_CFG_W0,
		VIU2_OSD1_BLK0_CFG_W1,
		VIU2_OSD1_BLK0_CFG_W2,
		VIU2_OSD1_BLK0_CFG_W3,
		VIU2_OSD1_BLK0_CFG_W4,
		VIU2_OSD1_BLK1_CFG_W4,
		VIU2_OSD1_BLK2_CFG_W4,
		VIU2_OSD1_FIFO_CTRL_STAT,

		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
	},
	{
		VIU2_OSD1_CTRL_STAT,
		VIU2_OSD1_CTRL_STAT2,
		VIU2_OSD1_COLOR_ADDR,
		VIU2_OSD1_COLOR,
		VIU2_OSD1_TCOLOR_AG0,
		VIU2_OSD1_TCOLOR_AG1,
		VIU2_OSD1_TCOLOR_AG2,
		VIU2_OSD1_TCOLOR_AG3,
		VIU2_OSD1_BLK0_CFG_W0,
		VIU2_OSD1_BLK0_CFG_W1,
		VIU2_OSD1_BLK0_CFG_W2,
		VIU2_OSD1_BLK0_CFG_W3,
		VIU2_OSD1_BLK0_CFG_W4,
		VIU2_OSD1_BLK1_CFG_W4,
		VIU2_OSD1_BLK2_CFG_W4,
		VIU2_OSD1_FIFO_CTRL_STAT,

		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
		VIU2_OSD1_UNSUPPORT,
	},
};
#else
struct hw_osd_reg_s hw_osd_reg_array[HW_OSD_COUNT] = {
	{
		VIU_OSD1_CTRL_STAT,
		VIU_OSD1_CTRL_STAT2,
		VIU_OSD1_COLOR_ADDR,
		VIU_OSD1_COLOR,
		VIU_OSD1_TCOLOR_AG0,
		VIU_OSD1_TCOLOR_AG1,
		VIU_OSD1_TCOLOR_AG2,
		VIU_OSD1_TCOLOR_AG3,
		VIU_OSD1_BLK0_CFG_W0,
		VIU_OSD1_BLK0_CFG_W1,
		VIU_OSD1_BLK0_CFG_W2,
		VIU_OSD1_BLK0_CFG_W3,
		VIU_OSD1_BLK0_CFG_W4,
		VIU_OSD1_BLK1_CFG_W4,
		VIU_OSD1_BLK2_CFG_W4,
		VIU_OSD1_FIFO_CTRL_STAT,

		VPP_OSD_SCALE_COEF_IDX,
		VPP_OSD_SCALE_COEF,
		VPP_OSD_VSC_PHASE_STEP,
		VPP_OSD_VSC_INI_PHASE,
		VPP_OSD_VSC_CTRL0,
		VPP_OSD_HSC_PHASE_STEP,
		VPP_OSD_HSC_INI_PHASE,
		VPP_OSD_HSC_CTRL0,
		VPP_OSD_SC_DUMMY_DATA,
		VPP_OSD_SC_CTRL0,
		VPP_OSD_SCI_WH_M1,
		VPP_OSD_SCO_H_START_END,
		VPP_OSD_SCO_V_START_END,
	},
	{
		VIU_OSD2_CTRL_STAT,
		VIU_OSD2_CTRL_STAT2,
		VIU_OSD2_COLOR_ADDR,
		VIU_OSD2_COLOR,
		VIU_OSD2_TCOLOR_AG0,
		VIU_OSD2_TCOLOR_AG1,
		VIU_OSD2_TCOLOR_AG2,
		VIU_OSD2_TCOLOR_AG3,
		VIU_OSD2_BLK0_CFG_W0,
		VIU_OSD2_BLK0_CFG_W1,
		VIU_OSD2_BLK0_CFG_W2,
		VIU_OSD2_BLK0_CFG_W3,
		VIU_OSD2_BLK0_CFG_W4,
		VIU_OSD2_BLK1_CFG_W4,
		VIU_OSD2_BLK2_CFG_W4,
		VIU_OSD2_FIFO_CTRL_STAT,

		OSD2_SCALE_COEF_IDX,
		OSD2_SCALE_COEF,
		OSD2_VSC_PHASE_STEP,
		OSD2_VSC_INI_PHASE,
		OSD2_VSC_CTRL0,
		OSD2_HSC_PHASE_STEP,
		OSD2_HSC_INI_PHASE,
		OSD2_HSC_CTRL0,
		OSD2_SC_DUMMY_DATA,
		OSD2_SC_CTRL0,
		OSD2_SCI_WH_M1,
		OSD2_SCO_H_START_END,
		OSD2_SCO_V_START_END,
	},
	{
		VIU_OSD3_CTRL_STAT,
		VIU_OSD3_CTRL_STAT2,
		VIU_OSD3_COLOR_ADDR,
		VIU_OSD3_COLOR,
		VIU_OSD3_TCOLOR_AG0,
		VIU_OSD3_TCOLOR_AG1,
		VIU_OSD3_TCOLOR_AG2,
		VIU_OSD3_TCOLOR_AG3,
		VIU_OSD3_BLK0_CFG_W0,
		VIU_OSD3_BLK0_CFG_W1,
		VIU_OSD3_BLK0_CFG_W2,
		VIU_OSD3_BLK0_CFG_W3,
		VIU_OSD3_BLK0_CFG_W4,
		VIU_OSD3_BLK1_CFG_W4,
		VIU_OSD3_BLK2_CFG_W4,
		VIU_OSD3_FIFO_CTRL_STAT,

		OSD34_SCALE_COEF_IDX,
		OSD34_SCALE_COEF,
		OSD34_VSC_PHASE_STEP,
		OSD34_VSC_INI_PHASE,
		OSD34_VSC_CTRL0,
		OSD34_HSC_PHASE_STEP,
		OSD34_HSC_INI_PHASE,
		OSD34_HSC_CTRL0,
		OSD34_SC_DUMMY_DATA,
		OSD34_SC_CTRL0,
		OSD34_SCI_WH_M1,
		OSD34_SCO_H_START_END,
		OSD34_SCO_V_START_END,
	},
	{
		VIU_OSD4_CTRL_STAT,
		VIU_OSD4_CTRL_STAT2,
		VIU_OSD4_COLOR_ADDR,
		VIU_OSD4_COLOR,
		VIU_OSD4_TCOLOR_AG0,
		VIU_OSD4_TCOLOR_AG1,
		VIU_OSD4_TCOLOR_AG2,
		VIU_OSD4_TCOLOR_AG3,
		VIU_OSD4_BLK0_CFG_W0,
		VIU_OSD4_BLK0_CFG_W1,
		VIU_OSD4_BLK0_CFG_W2,
		VIU_OSD4_BLK0_CFG_W3,
		VIU_OSD4_BLK0_CFG_W4,
		VIU_OSD4_BLK1_CFG_W4,
		VIU_OSD4_BLK2_CFG_W4,
		VIU_OSD4_FIFO_CTRL_STAT,

		OSD4_SCALE_COEF_IDX,
		OSD4_SCALE_COEF,
		OSD4_VSC_PHASE_STEP,
		OSD4_VSC_INI_PHASE,
		OSD4_VSC_CTRL0,
		OSD4_HSC_PHASE_STEP,
		OSD4_HSC_INI_PHASE,
		OSD4_HSC_CTRL0,
		OSD4_SC_DUMMY_DATA,
		OSD4_SC_CTRL0,
		OSD4_SCI_WH_M1,
		OSD4_SCO_H_START_END,
		OSD4_SCO_V_START_END,
	},
};
#endif
#endif

static unsigned int osd_filter_coefs_bicubic_sharp[] = {
	0x01fa008c, 0x01fa0100, 0xff7f0200, 0xfe7f0300,
	0xfd7e0500, 0xfc7e0600, 0xfb7d0800, 0xfb7c0900,
	0xfa7b0b00, 0xfa7a0dff, 0xf9790fff, 0xf97711ff,
	0xf87613ff, 0xf87416fe, 0xf87218fe, 0xf8701afe,
	0xf76f1dfd, 0xf76d1ffd, 0xf76b21fd, 0xf76824fd,
	0xf76627fc, 0xf76429fc, 0xf7612cfc, 0xf75f2ffb,
	0xf75d31fb, 0xf75a34fb, 0xf75837fa, 0xf7553afa,
	0xf8523cfa, 0xf8503ff9, 0xf84d42f9, 0xf84a45f9,
	0xf84848f8
};

static unsigned int osd_filter_coefs_bicubic[] = { /* bicubic	coef0 */
	0x00800000, 0x007f0100, 0xff7f0200, 0xfe7f0300, 0xfd7e0500, 0xfc7e0600,
	0xfb7d0800, 0xfb7c0900, 0xfa7b0b00, 0xfa7a0dff, 0xf9790fff, 0xf97711ff,
	0xf87613ff, 0xf87416fe, 0xf87218fe, 0xf8701afe, 0xf76f1dfd, 0xf76d1ffd,
	0xf76b21fd, 0xf76824fd, 0xf76627fc, 0xf76429fc, 0xf7612cfc, 0xf75f2ffb,
	0xf75d31fb, 0xf75a34fb, 0xf75837fa, 0xf7553afa, 0xf8523cfa, 0xf8503ff9,
	0xf84d42f9, 0xf84a45f9, 0xf84848f8
};

static unsigned int osd_filter_coefs_bilinear[] = { /* 2 point bilinear	coef1 */
	0x00800000, 0x007e0200, 0x007c0400, 0x007a0600, 0x00780800, 0x00760a00,
	0x00740c00, 0x00720e00, 0x00701000, 0x006e1200, 0x006c1400, 0x006a1600,
	0x00681800, 0x00661a00, 0x00641c00, 0x00621e00, 0x00602000, 0x005e2200,
	0x005c2400, 0x005a2600, 0x00582800, 0x00562a00, 0x00542c00, 0x00522e00,
	0x00503000, 0x004e3200, 0x004c3400, 0x004a3600, 0x00483800, 0x00463a00,
	0x00443c00, 0x00423e00, 0x00404000
};

static unsigned int osd_filter_coefs_2point_binilear[] = {
	/* 2 point bilinear, bank_length == 2	coef2 */
	0x80000000, 0x7e020000, 0x7c040000, 0x7a060000, 0x78080000, 0x760a0000,
	0x740c0000, 0x720e0000, 0x70100000, 0x6e120000, 0x6c140000, 0x6a160000,
	0x68180000, 0x661a0000, 0x641c0000, 0x621e0000, 0x60200000, 0x5e220000,
	0x5c240000, 0x5a260000, 0x58280000, 0x562a0000, 0x542c0000, 0x522e0000,
	0x50300000, 0x4e320000, 0x4c340000, 0x4a360000, 0x48380000, 0x463a0000,
	0x443c0000, 0x423e0000, 0x40400000
};

/* filt_triangle, point_num =3, filt_len =2.6, group_num = 64 */
static unsigned int osd_filter_coefs_3point_triangle_sharp[] = {
	0x40400000, 0x3e420000, 0x3d430000, 0x3b450000,
	0x3a460000, 0x38480000, 0x37490000, 0x354b0000,
	0x344c0000, 0x324e0000, 0x314f0000, 0x2f510000,
	0x2e520000, 0x2c540000, 0x2b550000, 0x29570000,
	0x28580000, 0x265a0000, 0x245c0000, 0x235d0000,
	0x215f0000, 0x20600000, 0x1e620000, 0x1d620100,
	0x1b620300, 0x19630400, 0x17630600, 0x15640700,
	0x14640800, 0x12640a00, 0x11640b00, 0x0f650c00,
	0x0d660d00
};

static unsigned int osd_filter_coefs_3point_triangle[] = {
	0x40400000, 0x3f400100, 0x3d410200, 0x3c410300,
	0x3a420400, 0x39420500, 0x37430600, 0x36430700,
	0x35430800, 0x33450800, 0x32450900, 0x31450a00,
	0x30450b00, 0x2e460c00, 0x2d460d00, 0x2c470d00,
	0x2b470e00, 0x29480f00, 0x28481000, 0x27481100,
	0x26491100, 0x25491200, 0x24491300, 0x234a1300,
	0x224a1400, 0x214a1500, 0x204a1600, 0x1f4b1600,
	0x1e4b1700, 0x1d4b1800, 0x1c4c1800, 0x1b4c1900,
	0x1a4c1a00
};

static unsigned int osd_filter_coefs_4point_triangle[] = {
	0x20402000, 0x20402000, 0x1f3f2101, 0x1f3f2101,
	0x1e3e2202, 0x1e3e2202, 0x1d3d2303, 0x1d3d2303,
	0x1c3c2404, 0x1c3c2404, 0x1b3b2505, 0x1b3b2505,
	0x1a3a2606, 0x1a3a2606, 0x19392707, 0x19392707,
	0x18382808, 0x18382808, 0x17372909, 0x17372909,
	0x16362a0a, 0x16362a0a, 0x15352b0b, 0x15352b0b,
	0x14342c0c, 0x14342c0c, 0x13332d0d, 0x13332d0d,
	0x12322e0e, 0x12322e0e, 0x11312f0f, 0x11312f0f,
	0x10303010
};

/* 4th order (cubic) b-spline */
/* filt_cubic point_num =4, filt_len =4, group_num = 64 */
static unsigned int vpp_filter_coefs_4point_bspline[] = {
	0x15561500, 0x14561600, 0x13561700, 0x12561800,
	0x11551a00, 0x11541b00, 0x10541c00, 0x0f541d00,
	0x0f531e00, 0x0e531f00, 0x0d522100, 0x0c522200,
	0x0b522300, 0x0b512400, 0x0a502600, 0x0a4f2700,
	0x094e2900, 0x084e2a00, 0x084d2b00, 0x074c2c01,
	0x074b2d01, 0x064a2f01, 0x06493001, 0x05483201,
	0x05473301, 0x05463401, 0x04453601, 0x04433702,
	0x04423802, 0x03413a02, 0x03403b02, 0x033f3c02,
	0x033d3d03
};

/* filt_quadratic, point_num =3, filt_len =3, group_num = 64 */
static unsigned int osd_filter_coefs_3point_bspline[] = {
	0x40400000, 0x3e420000, 0x3c440000, 0x3a460000,
	0x38480000, 0x364a0000, 0x344b0100, 0x334c0100,
	0x314e0100, 0x304f0100, 0x2e500200, 0x2c520200,
	0x2a540200, 0x29540300, 0x27560300, 0x26570300,
	0x24580400, 0x23590400, 0x215a0500, 0x205b0500,
	0x1e5c0600, 0x1d5c0700, 0x1c5d0700, 0x1a5e0800,
	0x195e0900, 0x185e0a00, 0x175f0a00, 0x15600b00,
	0x14600c00, 0x13600d00, 0x12600e00, 0x11600f00,
	0x10601000
};

static unsigned int *filter_table[] = {
	osd_filter_coefs_bicubic_sharp,
	osd_filter_coefs_bicubic,
	osd_filter_coefs_bilinear,
	osd_filter_coefs_2point_binilear,
	osd_filter_coefs_3point_triangle_sharp,
	osd_filter_coefs_3point_triangle,
	osd_filter_coefs_4point_triangle,
	vpp_filter_coefs_4point_bspline,
	osd_filter_coefs_3point_bspline
};

#define OSD_TYPE_TOP_FIELD 0
#define OSD_TYPE_BOT_FIELD 1

int osd_get_chip_type(void)
{
	unsigned int cpu_type;

	cpu_type = get_cpu_id().family_id;
	return cpu_type;
}

static void osd_vpu_power_on(void)
{
}
#ifndef AML_T7_DISPLAY
#ifdef AML_OSD_HIGH_VERSION
static void osd_vpu_power_on_viu2(void)
{
}
#endif
#endif

void osd_set_log_level(int level)
{
	osd_log_level = level;
}

void osd_get_hw_para(struct hw_para_s **para)
{
	*para = &osd_hw;
	return;
}

#ifdef CONFIG_AM_VIDEO
static struct vframe_s *osd_vf_peek(void *arg)
{
	if (osd_vf_need_update && (vf.width > 0) && (vf.height > 0))
		return &vf;
	else
		return NULL;
}

static struct vframe_s *osd_vf_get(void *arg)
{
	if (osd_vf_need_update) {
		vf_ext_light_unreg_provider(&osd_vf_prov);
		osd_vf_need_update = false;
		return &vf;
	}
	return NULL;
}

#define PROVIDER_NAME   "osd"
static const struct vframe_operations_s osd_vf_provider = {
	.peek = osd_vf_peek,
	.get  = osd_vf_get,
	.put  = NULL,
};

#endif

static inline void  osd_update_3d_mode(int enable_osd1, int enable_osd2,
				       int enable_osd3, int enable_viu2_osd1)
{
	if (enable_osd1)
		osd1_update_disp_3d_mode();
	if (enable_osd2)
		osd2_update_disp_3d_mode();
	if (enable_osd3)
		osd3_update_disp_3d_mode();
	if (enable_viu2_osd1)
		viu2_osd1_update_disp_3d_mode();
}

#if 1
static void get_encp_line(int *enc_line, int *active_line_begin)
{
	unsigned int reg = 0;
	unsigned int viu_sel;
	int osd_index;

	osd_index = get_osd_layer();
	if (osd_index < VIU2_OSD1)
		viu_sel = osd_reg_read(VPU_VIU_VENC_MUX_CTRL) & 0x3;
	else
		viu_sel = (osd_reg_read(VPU_VIU_VENC_MUX_CTRL) >> 2) & 0x3;

	switch (viu_sel) {
	case 0:
		reg = osd_reg_read(ENCL_INFO_READ);
		*active_line_begin =
			osd_reg_read(ENCL_VIDEO_VAVON_BLINE);

		break;
	case 1:
		reg = osd_reg_read(ENCI_INFO_READ);
		*active_line_begin =
			osd_reg_read(ENCI_VFIFO2VD_LINE_TOP_START);
		break;
	case 2:
		reg = osd_reg_read(ENCP_INFO_READ);
		*active_line_begin =
			osd_reg_read(ENCP_VIDEO_VAVON_BLINE);
		break;
	case 3:
		reg = osd_reg_read(ENCT_INFO_READ);
		*active_line_begin =
			osd_reg_read(ENCT_VIDEO_VAVON_BLINE);
		break;
	}
	*enc_line = (reg >> 16) & 0x1fff;
	*enc_line -= (*active_line_begin);
}

static inline void wait_vsync_wakeup(void)
{
	int i = 0;
	int enc_line = 0;
	int active_line_begin;
	int vsync_line = 0;
	struct vinfo_s *info = NULL;
	int line_after = 0;

#ifdef CONFIG_AML_VOUT
	info = vout_get_current_vinfo();
	vsync_line = info->field_height;
#endif

	get_encp_line(&enc_line, &active_line_begin);

	/* if current line is always zero */
	if (enc_line == (0 - active_line_begin)) {
		_udelay(50);
		get_encp_line(&line_after, &active_line_begin);
		if (enc_line == (0 - active_line_begin))
			goto vsync_hit_flag;
	}

	while (enc_line < vsync_line) {
		if (i > 100000) {
			osd_logi("wait_vsync_wakeup exit\n");
			break;
		}
		i++;
		get_encp_line(&enc_line, &active_line_begin);
	}

vsync_hit_flag:
	vsync_hit = true;
}
#else

static int get_encp_line(int *enc_line)
{
	int vsync_get = 0;
	unsigned int reg = 0;
	int field_status = 0;
	static int pre_field_status = 0;

	switch (osd_reg_read(VPU_VIU_VENC_MUX_CTRL) & 0x3) {
	case 0:
		reg = osd_reg_read(ENCL_INFO_READ);
		break;
	case 1:
		reg = osd_reg_read(ENCI_INFO_READ);
		break;
	case 2:
		reg = osd_reg_read(ENCP_INFO_READ);
		break;
	case 3:
		reg = osd_reg_read(ENCT_INFO_READ);
		break;
	}
	*enc_line = (reg >> 16) & 0x1fff;
	field_status = (reg >> 29) & 0x7;
	if (field_status != pre_field_status)
		vsync_get = 1;
	pre_field_status = field_status;
	return vsync_get;
}

static inline void wait_vsync_wakeup(void)
{
	int i = 0;
	int line = 0;
	int vsync_get = 0;

	vsync_get = get_encp_line(&line);
	if ((line > 0) || vsync_get) {
		while (!vsync_get) {
			if (i>100000) {
				osd_logi("wait_vsync_wakeup exit, i=%d\n", i);
				break;
			}
			i++;
			vsync_get = get_encp_line(&line);
		}
	}
	vsync_hit = true;
}
#endif

static inline void walk_through_update_list(void)
{
	u32  i, j;

	for (i = 0; i < HW_OSD_COUNT; i++) {
		j = 0;
		while (osd_hw.updated[i] && j < HW_REG_INDEX_MAX) {
			if (osd_hw.updated[i] & (1 << j)) {
				osd_hw.reg[i][j].update_func();
				remove_from_update_list(i, j);
			}
			j++;
		}
	}
}

static void osd_check_scan_mode(void)
{
#define	VOUT_ENCI	1
#define	VOUT_ENCP	2
#define	VOUT_ENCT	3
	int vmode = -1;

	osd_hw.scan_mode = SCAN_MODE_PROGRESSIVE;
#ifdef CONFIG_AML_VOUT
	vmode = vout_get_current_vmode();
#endif
	switch (vmode) {
	/* case VMODE_LCD:*/
	case VMODE_480I:
	case VMODE_480CVBS:
	case VMODE_576I:
	case VMODE_576CVBS:
	case VMODE_1080I:
	case VMODE_1080I_50HZ:
		osd_hw.scan_mode = SCAN_MODE_INTERLACE;
		break;
	default:
		break;
	}
}

static void vsync_isr(void)
{
	unsigned int odd_even;
	unsigned int scan_line_number = 0;
	unsigned int fb0_cfg_w0, fb1_cfg_w0;

	osd_check_scan_mode();
	if (osd_hw.scan_mode == SCAN_MODE_INTERLACE) {
		fb0_cfg_w0 = osd_reg_read(hw_osd_reg_array[0].osd_blk0_cfg_w0);
		fb1_cfg_w0 = osd_reg_read(hw_osd_reg_array[1].osd_blk0_cfg_w0);
		if (osd_reg_read(ENCP_VIDEO_MODE) & (1 << 12)) {
			/* 1080I */
			scan_line_number = ((osd_reg_read(ENCP_INFO_READ))
					    & 0x1fff0000) >> 16;
			if ((osd_hw.pandata[OSD1].y_start % 2) == 0) {
				if (scan_line_number >= 562) {
					/* bottom field, odd lines*/
					odd_even = 1;
				} else {
					/* top field, even lines*/
					odd_even = 0;
				}
			} else {
				if (scan_line_number >= 562) {
					/* top field, even lines*/
					odd_even = 0;
				} else {
					/* bottom field, odd lines*/
					odd_even = 1;
				}
			}
		} else {
			if ((osd_hw.pandata[OSD1].y_start % 2) == 0)
				odd_even = osd_reg_read(ENCI_INFO_READ) & 1;
			else
				odd_even = !(osd_reg_read(ENCI_INFO_READ) & 1);
		}
		fb0_cfg_w0 &= ~1;
		fb1_cfg_w0 &= ~1;
		fb0_cfg_w0 |= odd_even;
		fb1_cfg_w0 |= odd_even;
		osd_reg_write(hw_osd_reg_array[0].osd_blk0_cfg_w0, fb0_cfg_w0);
		osd_reg_write(hw_osd_reg_array[1].osd_blk0_cfg_w0, fb1_cfg_w0);
	}
	/* go through update list */
	if (!vsync_hit)
		wait_vsync_wakeup();

	walk_through_update_list();
	osd_update_3d_mode(osd_hw.mode_3d[OSD1].enable,
			   osd_hw.mode_3d[OSD2].enable,
			   osd_hw.mode_3d[OSD3].enable,
			   osd_hw.mode_3d[VIU2_OSD1].enable);
}

void osd_wait_vsync_hw(void)
{
	//mdelay(16);
	vsync_isr();
	vsync_hit = false;
}

int osd_set_scan_mode(u32 index)
{
	u32 data32 = 0x0;
	int vmode = -1;
	int real_scan_mode = SCAN_MODE_INTERLACE;

#ifdef CONFIG_AML_VOUT
	vmode = vout_get_current_vmode();
#endif
	osd_hw.scan_mode = SCAN_MODE_PROGRESSIVE;
	if (osd_hw.fb_for_4k2k) {
		if (osd_hw.free_scale_enable[index])
			osd_hw.scale_workaround = 1;
	}
	switch (vmode) {
	/* case VMODE_LCD: */
	case VMODE_480I:
	case VMODE_480CVBS:
	case VMODE_576I:
	case VMODE_576CVBS:
		if (osd_hw.free_scale_mode[index]) {
			osd_hw.field_out_en = 1;
			switch (osd_hw.free_scale_data[index].y_end) {
			case 719:
				osd_hw.bot_type = 2;
				break;
			case 1079:
				osd_hw.bot_type = 3;
				break;
			default:
				osd_hw.bot_type = 2;
				break;
			}
		}
		osd_hw.scan_mode = real_scan_mode = SCAN_MODE_INTERLACE;
		break;
	case VMODE_1080I:
	case VMODE_1080I_50HZ:
#ifdef CONFIG_AML_VOUT_FRAMERATE_AUTOMATION
	case VMODE_1080I_59HZ:
#endif
		if (osd_hw.free_scale_mode[index]) {
			osd_hw.field_out_en = 1;
			switch (osd_hw.free_scale_data[index].y_end) {
			case 719:
				osd_hw.bot_type = 1;
				break;
			case 1079:
				osd_hw.bot_type = 2;
				break;
			default:
				osd_hw.bot_type = 1;
				break;
			}
		}
		osd_hw.scan_mode = real_scan_mode = SCAN_MODE_INTERLACE;
		break;
	case VMODE_4K2K_24HZ:
	case VMODE_4K2K_25HZ:
	case VMODE_4K2K_30HZ:
	case VMODE_4K2K_SMPTE:
	case VMODE_4K2K_SMPTE_25HZ:
	case VMODE_4K2K_SMPTE_30HZ:
	case VMODE_4K2K_SMPTE_50HZ:
	case VMODE_4K2K_SMPTE_60HZ:
	case VMODE_4K2K_SMPTE_50HZ_Y420:
	case VMODE_4K2K_SMPTE_60HZ_Y420:
		if (osd_hw.fb_for_4k2k) {
			if (osd_hw.free_scale_enable[index])
				osd_hw.scale_workaround = 1;
		}
		osd_hw.field_out_en = 0;
		break;
	default:
		if (osd_hw.free_scale_mode[index])
			osd_hw.field_out_en = 0;
		break;
	}
	if (osd_hw.free_scale_enable[index])
		osd_hw.scan_mode = SCAN_MODE_PROGRESSIVE;

	if (index == OSD1) {
		data32 = (osd_reg_read(hw_osd_reg_array[0].osd_blk0_cfg_w0) & 3) >> 1;
	} else if (index == OSD2) {
		if (real_scan_mode == SCAN_MODE_INTERLACE)
			return 1;
		/* data32 = (osd_reg_read(hw_osd_reg_array[1].osd_blk0_cfg_w0) & 3) >> 1; */
	} else {
		data32 = (osd_reg_read(VIU_OSD3_BLK0_CFG_W0) & 3) >> 1;
	}

	if (data32 == osd_hw.scan_mode)
		return 1;
	else
		return 0;
}

void  osd_set_gbl_alpha_hw(u32 index, u32 gbl_alpha)
{
	if (osd_hw.gbl_alpha[index] != gbl_alpha) {
		osd_hw.gbl_alpha[index] = gbl_alpha;
		add_to_update_list(index, OSD_GBL_ALPHA);
		osd_wait_vsync_hw();
	}
}

u32 osd_get_gbl_alpha_hw(u32  index)
{
	return osd_hw.gbl_alpha[index];
}

void osd_set_color_key_hw(u32 index, u32 color_index, u32 colorkey)
{
	u8 r = 0, g = 0, b = 0, a = (colorkey & 0xff000000) >> 24;
	u32 data32;

	colorkey &= 0x00ffffff;
	switch (color_index) {
	case COLOR_INDEX_16_655:
		r = (colorkey >> 10 & 0x3f) << 2;
		g = (colorkey >> 5 & 0x1f) << 3;
		b = (colorkey & 0x1f) << 3;
		break;
	case COLOR_INDEX_16_844:
		r = colorkey >> 8 & 0xff;
		g = (colorkey >> 4 & 0xf) << 4;
		b = (colorkey & 0xf) << 4;
		break;
	case COLOR_INDEX_16_565:
		r = (colorkey >> 11 & 0x1f) << 3;
		g = (colorkey >> 5 & 0x3f) << 2;
		b = (colorkey & 0x1f) << 3;
		break;
	case COLOR_INDEX_24_888_B:
		b = colorkey >> 16 & 0xff;
		g = colorkey >> 8 & 0xff;
		r = colorkey & 0xff;
		break;
	case COLOR_INDEX_24_RGB:
	case COLOR_INDEX_YUV_422:
		r = colorkey >> 16 & 0xff;
		g = colorkey >> 8 & 0xff;
		b = colorkey & 0xff;
		break;
	}
	data32 = r << 24 | g << 16 | b << 8 | a;
	if (osd_hw.color_key[index] != data32) {
		osd_hw.color_key[index] = data32;
		osd_logd2("bpp:%d--r:0x%x g:0x%x b:0x%x ,a:0x%x\n",
			  color_index, r, g, b, a);
		add_to_update_list(index, OSD_COLOR_KEY);
		osd_wait_vsync_hw();
	}
	return;
}

void  osd_srckey_enable_hw(u32  index, u8 enable)
{
	if (enable != osd_hw.color_key_enable[index]) {
		osd_hw.color_key_enable[index] = enable;
		add_to_update_list(index, OSD_COLOR_KEY_ENABLE);
		osd_wait_vsync_hw();
	}
}

void osd_set_color_mode(u32 index, const struct color_bit_define_s *color)
{
	if (color != osd_hw.color_info[index]) {
		osd_hw.color_info[index] = color;
		add_to_update_list(index, OSD_COLOR_MODE);
	}
}

void osd_update_disp_axis_hw(
	u32 index,
	u32 display_h_start,
	u32 display_h_end,
	u32 display_v_start,
	u32 display_v_end,
	u32 xoffset,
	u32 yoffset,
	u32 mode_change)
{
	struct pandata_s disp_data;
	struct pandata_s pan_data;

	if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_M8) {
		if (index == OSD2)
			return;
	}
	if (NULL == osd_hw.color_info[index])
		return;
	disp_data.x_start = display_h_start;
	disp_data.y_start = display_v_start;
	disp_data.x_end = display_h_end;
	disp_data.y_end = display_v_end;
	pan_data.x_start = xoffset;
	pan_data.x_end = xoffset + (display_h_end - display_h_start);
	pan_data.y_start = yoffset;
	pan_data.y_end = yoffset + (display_v_end - display_v_start);
	/* if output mode change then reset pan ofFfset. */
	memcpy(&osd_hw.pandata[index], &pan_data, sizeof(struct pandata_s));
	memcpy(&osd_hw.dispdata[index], &disp_data, sizeof(struct pandata_s));
	if (mode_change) /* modify pandata . */
		add_to_update_list(index, OSD_COLOR_MODE);
	osd_hw.reg[index][DISP_GEOMETRY].update_func();
	osd_wait_vsync_hw();
}

/* the return stride unit is 128bit(16bytes) */
static u32 line_stride_calc(u32 fmt_mode,
			    u32 hsize,
			    u32 stride_align_32bytes)
{
	u32 line_stride = 0;
	u32 line_stride_32bytes;
	u32 line_stride_64bytes;

	switch (fmt_mode) {
	/* 2-bit LUT */
	case COLOR_INDEX_02_PAL4:
		line_stride = ((hsize << 1) + 127) >> 7;
		break;
	/* 4-bit LUT */
	case COLOR_INDEX_04_PAL16:
		line_stride = ((hsize << 2) + 127) >> 7;
		break;
	/* 8-bit LUT */
	case COLOR_INDEX_08_PAL256:
		line_stride = ((hsize << 3) + 127) >> 7;
		break;
	/* 4:2:2, 32-bit per 2 pixels */
	case COLOR_INDEX_YUV_422:
		line_stride = ((((hsize + 1) >> 1) << 5) + 127) >> 7;
		break;
	/* 16-bit LUT */
	case COLOR_INDEX_16_655:
	case COLOR_INDEX_16_844:
	case COLOR_INDEX_16_6442:
	case COLOR_INDEX_16_4444_R:
	case COLOR_INDEX_16_4642_R:
	case COLOR_INDEX_16_1555_A:
	case COLOR_INDEX_16_4444_A:
	case COLOR_INDEX_16_565:
		line_stride = ((hsize << 4) + 127) >> 7;
		break;
	/* 32-bit LUT */
	case COLOR_INDEX_32_BGRA:
	case COLOR_INDEX_32_ABGR:
	case COLOR_INDEX_32_RGBA:
	case COLOR_INDEX_32_ARGB:
		line_stride = ((hsize << 5) + 127) >> 7;
		break;
	/* 24-bit LUT */
	case COLOR_INDEX_24_6666_A:
	case COLOR_INDEX_24_6666_R:
	case COLOR_INDEX_24_8565:
	case COLOR_INDEX_24_5658:
	case COLOR_INDEX_24_888_B:
	case COLOR_INDEX_24_RGB:
		line_stride = ((hsize << 4) + (hsize << 3) + 127) >> 7;
		break;
	}
	line_stride_32bytes = ((line_stride + 1) >> 1) << 1;
	line_stride_64bytes = ((line_stride + 3) >> 2) << 2;
	/* need wr ddr is 32bytes aligned */
	if (stride_align_32bytes)
		line_stride = line_stride_32bytes;
	else
		line_stride = line_stride_64bytes;
	return line_stride;
}

#ifdef AML_OSD_HIGH_VERSION
/* only one layer */
void osd_setting_default_hwc(u32 index, struct pandata_s *disp_data)
{
	u32 width, height;
	u32 blend2_premult_en = 3, din_premult_en = 0;
	u32 blend_din_en = 0x5;
	/* blend_din0 input to blend0 */
	u32 din0_byp_blend = 0;
	/* blend1_dout to blend2 */
	u32 din2_osd_sel = 0;
	/* blend1_din3 input to blend1 */
	u32 din3_osd_sel = 0;
	u32 din_reoder_sel = 0x1;
	u32 postbld_src3_sel = 3, postbld_src4_sel = 0;
	u32 postbld_osd1_premult = 0, postbld_osd2_premult = 0;
	u32 reg_offset = 2;
	u32 shift_line = osd_hw.shift_line;

	if (osd_get_chip_type() > MESON_CPU_MAJOR_ID_TM2)
		reg_offset = 8;
	if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_T7 ||
	    osd_get_chip_type() == MESON_CPU_MAJOR_ID_T3)
		postbld_src3_sel = 4;
	if (index == OSD1)
		din_reoder_sel = 0x4441;
	else if (index == OSD2)
		din_reoder_sel = 0x4442;
	else if (index == OSD3)
		din_reoder_sel = 0x4443;
	/* depend on din0_premult_en */
	postbld_osd1_premult = 0;
	/* depend on din_premult_en bit 4 */
	postbld_osd2_premult = 0;
	/* osd blend ctrl */
	osd_reg_write(VIU_OSD_BLEND_CTRL,
		4				  << 29|
		blend2_premult_en << 27|
		din0_byp_blend	  << 26|
		din2_osd_sel	  << 25|
		din3_osd_sel	  << 24|
		blend_din_en	  << 20|
		din_premult_en	  << 16|
		din_reoder_sel);
	/* vpp osd1 blend ctrl */
	osd_reg_write(OSD1_BLEND_SRC_CTRL,
		(0 & 0xf) << 0 |
		(0 & 0x1) << 4 |
		(postbld_src3_sel & 0xf) << 8 |
		(postbld_osd1_premult & 0x1) << 16|
		(1 & 0x1) << 20);
	/* vpp osd2 blend ctrl */
	osd_reg_write(OSD2_BLEND_SRC_CTRL,
		(0 & 0xf) << 0 |
		(0 & 0x1) << 4 |
		(postbld_src4_sel & 0xf) << 8 |
		(postbld_osd2_premult & 0x1) << 16 |
		(1 & 0x1) << 20);

	/* Do later: different format select different dummy_data */
	/* used default dummy data */
	osd_reg_write(VIU_OSD_BLEND_DUMMY_DATA0,
		0x0 << 16 |
		0x0 << 8 |
		0x0);
	/* used default dummy alpha data */
	osd_reg_write(VIU_OSD_BLEND_DUMMY_ALPHA,
		0x0  << 20 |
		0x0  << 11 |
		0x0);

	width = disp_data->x_end - disp_data->x_start + 1;
	height = disp_data->y_end - disp_data->y_start + 1 + shift_line;
	/* it is setting for osdx */
	osd_reg_write(
		VIU_OSD_BLEND_DIN0_SCOPE_H + reg_offset * index,
		disp_data->x_end << 16 |
		disp_data->x_start);
	osd_reg_write(
		VIU_OSD_BLEND_DIN0_SCOPE_V + reg_offset * index,
		(disp_data->y_end + shift_line) << 16 |
		(disp_data->y_start + shift_line));
	if (index == OSD1) {
		int i;

		for (i = 1; i < 4; i++)
			osd_reg_write(
				VIU_OSD_BLEND_DIN0_SCOPE_V + reg_offset * i,
				0xffffffff);
	} else if (index == OSD2) {
		int i = 0;

		osd_reg_write(
			VIU_OSD_BLEND_DIN0_SCOPE_V + reg_offset * i,
			0xffffffff);
		for (i = 2; i < 4; i++)
			osd_reg_write(
				VIU_OSD_BLEND_DIN0_SCOPE_V + reg_offset * i,
				0xffffffff);
	} else if (index == OSD3) {
		int i = 0;

		for (i = 0; i < 2; i++)
			osd_reg_write(VIU_OSD_BLEND_DIN0_SCOPE_V +
				      reg_offset * i,
				      0xffffffff);

		i = 3;
		osd_reg_write(VIU_OSD_BLEND_DIN0_SCOPE_V + reg_offset * i,
			      0xffffffff);
	}

	osd_reg_write(VIU_OSD_BLEND_BLEND0_SIZE,
		height << 16 |
		width);
	osd_reg_write(VIU_OSD_BLEND_BLEND1_SIZE,
		height  << 16 |
		width);
	osd_reg_write(VPP_OSD1_IN_SIZE,
		height << 16 | width);
	if (!is_dolby_enable())
		osd_reg_set_bits(DOLBY_PATH_CTRL,
			0x3, 2, 2);

	/* setting blend scope */
	osd_reg_write(VPP_OSD1_BLD_H_SCOPE,
		disp_data->x_start << 16 | disp_data->x_end);
	osd_reg_write(VPP_OSD1_BLD_V_SCOPE,
		disp_data->y_start << 16 | disp_data->y_end);
}

void osd_update_blend(struct pandata_s *disp_data)
{
	u32 width, height;
	int vmode = -1;

#ifdef CONFIG_AML_VOUT
	vmode = vout_get_current_vmode();
#endif
	switch (vmode) {
	/* case VMODE_LCD: */
	case VMODE_480I:
	case VMODE_480CVBS:
	case VMODE_576I:
	case VMODE_576CVBS:
	case VMODE_1080I:
	case VMODE_1080I_50HZ:
#ifdef CONFIG_AML_VOUT_FRAMERATE_AUTOMATION
	case VMODE_1080I_59HZ:
#endif
		disp_data->y_start /=2;
		disp_data->y_end /= 2;
		break;
	default:
		break;
	}
	width = disp_data->x_end - disp_data->x_start + 1;
	height = disp_data->y_end - disp_data->y_start + 1;

	/* setting blend scope */
	osd_reg_write(VPP_OSD1_BLD_H_SCOPE,
		disp_data->x_start << 16 | disp_data->x_end);
	osd_reg_write(VPP_OSD1_BLD_V_SCOPE,
		disp_data->y_start << 16 | disp_data->y_end);
	osd_reg_write(VPP_OUT_H_V_SIZE,
			width << 16 | height);

}
#endif

static void osd_update_mif_linear_addr(u32 index)
{
	u32 line_stride, bpp;
	u32 fmt_mode = 0;
	u32 osd_blk1_cfg_w4, osd_blk2_cfg_w4, osd_ctrl_stat;

	switch (index) {
	case 0:
		osd_blk1_cfg_w4 = hw_osd_reg_array[0].osd_blk1_cfg_w4;
		osd_blk2_cfg_w4 = hw_osd_reg_array[0].osd_blk2_cfg_w4;
		osd_ctrl_stat = hw_osd_reg_array[0].osd_ctrl_stat;
		break;
	case 1:
		osd_blk1_cfg_w4 = hw_osd_reg_array[1].osd_blk1_cfg_w4;
		osd_blk2_cfg_w4 = hw_osd_reg_array[1].osd_blk2_cfg_w4;
		osd_ctrl_stat = hw_osd_reg_array[1].osd_ctrl_stat;
		break;
	case 2:
		osd_blk1_cfg_w4 = hw_osd_reg_array[2].osd_blk1_cfg_w4;
		osd_blk2_cfg_w4 = hw_osd_reg_array[2].osd_blk2_cfg_w4;
		osd_ctrl_stat = hw_osd_reg_array[2].osd_ctrl_stat;
		break;
	case 3:
		osd_blk1_cfg_w4 = hw_osd_reg_array[3].osd_blk1_cfg_w4;
		osd_blk2_cfg_w4 = hw_osd_reg_array[3].osd_blk2_cfg_w4;
		osd_ctrl_stat = hw_osd_reg_array[3].osd_ctrl_stat;
		break;
	default:
		osd_blk1_cfg_w4 = hw_osd_reg_array[0].osd_blk1_cfg_w4;
		osd_blk2_cfg_w4 = hw_osd_reg_array[0].osd_blk2_cfg_w4;
		osd_ctrl_stat = hw_osd_reg_array[0].osd_ctrl_stat;
		break;
	}
	if (osd_hw.color_info[index])
		fmt_mode =
			osd_hw.color_info[index]->color_index;
	bpp = osd_hw.color_info[index]->bpp / 8;
	/* 64 bytes align */
	line_stride = line_stride_calc
		(fmt_mode,
		osd_hw.fb_gem[index].width / bpp, 0);
	/* set frame addr, 8G addr, need >> 4*/
	VSYNCOSD_WR_MPEG_REG(osd_blk1_cfg_w4,
			     osd_hw.fb_gem[index].addr >> 4);
	/* set line stride */
	VSYNCOSD_WR_MPEG_REG_BITS(osd_blk2_cfg_w4,
		line_stride,
		0, 12);
	/* used phyic addr */
	VSYNCOSD_WR_MPEG_REG_BITS(osd_ctrl_stat,
		1, 2, 1);
}

static void set_fb_layout_info(u32 index, u32 fb_len)
{
	int i, offset = 0;

	if (fb_layout[index].used) {
		osd_logi("%s, osd%d is already set\n", __func__, index);
		return;
	}
	for (i = 0; i < OSD_MAX; i++) {
		if (fb_layout[i].used)
			offset += fb_layout[i].fb_len;
	}
	fb_layout[index].fb_offset = offset;
	fb_layout[index].fb_len = fb_len;
	fb_layout[index].used = 1;
}

void clear_fb_layout_info(u32 index)
{
	fb_layout[index].fb_offset = 0;
	fb_layout[index].fb_len = 0;
	fb_layout[index].used = 0;
}

unsigned int get_fb_offset(u32 index)
{
	if (fb_layout[index].used)
		return fb_layout[index].fb_offset;

	osd_loge("%s, osd%d fb_offset is not set\n", __func__, index);
	return 0;
}

unsigned int get_fb_len(u32 index)
{
	if (fb_layout[index].used)
		return fb_layout[index].fb_len;

	osd_loge("%s, osd%d fb_len is not set\n", __func__, index);
	return 0;
}

void osd_setup_hw(u32 index,
		  u32 xoffset,
		  u32 yoffset,
		  u32 xres,
		  u32 yres,
		  u32 xres_virtual,
		  u32 yres_virtual,
		  u32 disp_start_x,
		  u32 disp_start_y,
		  u32 disp_end_x,
		  u32 disp_end_y,
		  u32 fbmem,
		  const struct color_bit_define_s *color)
{
	struct pandata_s disp_data;
	struct pandata_s pan_data;
	int update_color_mode = 0;
	int update_geometry = 0;
	u32 w = (color->bpp * xres_virtual + 7) >> 3;
	static int is_blend_set;
	u32 fb_len;

	if (osd_hw.osd_ver == OSD_SIMPLE) {
		if (index >= OSD2) {
			osd_loge("AXG not support osd2\n");
			return ;
		}
	}
	pan_data.x_start = xoffset;
	pan_data.y_start = yoffset;
	disp_data.x_start = disp_start_x;
	disp_data.y_start = disp_start_y;
	if (osd_hw.free_scale_enable[OSD1] && index == OSD1) {
		if (!(osd_hw.free_scale_mode[OSD1])) {
			pan_data.x_end = xoffset + g_vf_visual_width;
			pan_data.y_end = yoffset + g_vf_height;
			disp_data.x_end = disp_start_x + g_vf_width;
			disp_data.y_end = disp_start_y + g_vf_height;
		} else {
			pan_data.x_end = xoffset + (disp_end_x - disp_start_x);
			pan_data.y_end = yoffset + (disp_end_y - disp_start_y);
			disp_data.x_end = disp_end_x;
			disp_data.y_end = disp_end_y;
		}
	} else {
		pan_data.x_end = xoffset + (disp_end_x - disp_start_x);
		pan_data.y_end = yoffset + (disp_end_y - disp_start_y);
		if (likely(osd_hw.rotate[index].on_off
			   && osd_hw.rotate[index].on_off > 0)) {
			disp_data.x_end = disp_start_x + g_rotation_height;
			disp_data.y_end = disp_start_y + g_rotation_width;
		} else {
			disp_data.x_end = disp_end_x;
			disp_data.y_end = disp_end_y;
		}
	}
	if (color != osd_hw.color_info[index]) {
		update_color_mode = 1;
		osd_hw.color_info[index] = color;
	}
	if (osd_hw.fb_gem[index].addr != fbmem
	    || osd_hw.fb_gem[index].width != w
	    ||  osd_hw.fb_gem[index].height != yres_virtual) {
		osd_hw.fb_gem[index].addr = fbmem;
		osd_hw.fb_gem[index].width = w;
		osd_hw.fb_gem[index].height = yres_virtual;

		if (osd_hw.osd_ver == OSD_SIMPLE) {
			u32 line_stride, fmt_mode, bpp;

			bpp = color->bpp/8;
			fmt_mode = color->color_index;
			line_stride = line_stride_calc(fmt_mode,
				osd_hw.fb_gem[index].width / bpp, 1);
			VSYNCOSD_WR_MPEG_REG(
				hw_osd_reg_array[0].osd_blk1_cfg_w4,
				osd_hw.fb_gem[index].addr);
			VSYNCOSD_WR_MPEG_REG_BITS(
				hw_osd_reg_array[0].osd_blk2_cfg_w4,
				line_stride,
				0, 12);
		} else {
			fb_len = fb_gdev.fb_height *
				CANVAS_ALIGNED(fb_gdev.fb_width * color->bpp >> 3);
			set_fb_layout_info(index, fb_len);
			osd_hw.fb_gem[index].addr += get_fb_offset(index);

			osd_logd("%s, set osd%d fb_addr:0x%x fb_len:%d\n",
				 __func__, index,
				 osd_hw.fb_gem[index].addr, fb_len);
			if (osd_hw.mif_linear)
				osd_update_mif_linear_addr(index);
#ifdef CONFIG_AML_CANVAS
			else
				canvas_config(osd_hw.fb_gem[index].canvas_idx,
					      osd_hw.fb_gem[index].addr,
					      CANVAS_ALIGNED(osd_hw.fb_gem[index].width),
					      osd_hw.fb_gem[index].height,
					      CANVAS_ADDR_NOWRAP, CANVAS_BLKMODE_LINEAR);
#endif
		}
		osd_logd("osd[%d] canvas.idx =0x%x\n",
			 index, osd_hw.fb_gem[index].canvas_idx);
		osd_logd("osd[%d] canvas.addr=0x%x\n",
			 index, osd_hw.fb_gem[index].addr);
		osd_logd("osd[%d] canvas.width=%d\n",
			 index, CANVAS_ALIGNED(osd_hw.fb_gem[index].width));
		osd_logd("osd[%d] canvas.height=%d\n",
			 index, osd_hw.fb_gem[index].height);
	}
	/* osd blank only control by /sys/class/graphcis/fbx/blank */
#if 0
	if (osd_hw.enable[index] == DISABLE) {
		osd_hw.enable[index] = ENABLE;
		add_to_update_list(index, OSD_ENABLE);
	}
#endif
	if (memcmp(&pan_data, &osd_hw.pandata[index],
		   sizeof(struct pandata_s)) != 0 ||
	    memcmp(&disp_data, &osd_hw.dispdata[index],
		   sizeof(struct pandata_s)) != 0) {
		update_geometry = 1;
		memcpy(&osd_hw.pandata[index], &pan_data,
		       sizeof(struct pandata_s));
		memcpy(&osd_hw.dispdata[index], &disp_data,
		       sizeof(struct pandata_s));
	}
	if (update_color_mode)
		add_to_update_list(index, OSD_COLOR_MODE);
	if (update_geometry)
		add_to_update_list(index, DISP_GEOMETRY);
	add_to_update_list(index, DISP_OSD_REVERSE);
#ifdef AML_OSD_HIGH_VERSION
	if (osd_hw.osd_ver == OSD_HIGH_ONE && index < VIU2_OSD1 && !is_blend_set) {
		is_blend_set = 1;
		osd_setting_default_hwc(index, &disp_data);
	}
#endif
	osd_wait_vsync_hw();
}

void osd2_setup_hw(u32 index,
		  u32 xoffset,
		  u32 yoffset,
		  u32 xres,
		  u32 yres,
		  u32 xres_virtual,
		  u32 yres_virtual,
		  u32 disp_start_x,
		  u32 disp_start_y,
		  u32 disp_end_x,
		  u32 disp_end_y,
		  u32 fbmem,
		  const struct color_bit_define_s *color)
{
	struct pandata_s disp_data;
	struct pandata_s pan_data;
	int update_color_mode = 0;
	int update_geometry = 0;
	u32 w = (color->bpp * xres_virtual + 7) >> 3;

	if (osd_hw.osd_ver == OSD_SIMPLE) {
		if (index >= OSD2) {
			osd_loge("AXG not support osd2\n");
			return;
		}
	}
	pan_data.x_start = xoffset;
	pan_data.y_start = yoffset;
	disp_data.x_start = disp_start_x;
	disp_data.y_start = disp_start_y;
	if (osd_hw.free_scale_enable[OSD1] && index == OSD1) {
		if (!(osd_hw.free_scale_mode[OSD1])) {
			pan_data.x_end = xoffset + g_vf_visual_width;
			pan_data.y_end = yoffset + g_vf_height;
			disp_data.x_end = disp_start_x + g_vf_width;
			disp_data.y_end = disp_start_y + g_vf_height;
		} else {
			pan_data.x_end = xoffset + (disp_end_x - disp_start_x);
			pan_data.y_end = yoffset + (disp_end_y - disp_start_y);
			disp_data.x_end = disp_end_x;
			disp_data.y_end = disp_end_y;
		}
	} else {
		pan_data.x_end = xoffset + (disp_end_x - disp_start_x);
		pan_data.y_end = yoffset + (disp_end_y - disp_start_y);
		if (likely(osd_hw.rotate[index].on_off &&
			osd_hw.rotate[index].on_off > 0)) {
			disp_data.x_end = disp_start_x + g_rotation_height;
			disp_data.y_end = disp_start_y + g_rotation_width;
		} else {
			disp_data.x_end = disp_end_x;
			disp_data.y_end = disp_end_y;
		}
	}
	if (color != osd_hw.color_info[index]) {
		update_color_mode = 1;
		osd_hw.color_info[index] = color;
	}
	if (osd_hw.fb_gem[index].addr != fbmem ||
		osd_hw.fb_gem[index].width != w ||
		osd_hw.fb_gem[index].height != yres_virtual) {
		osd_hw.fb_gem[index].addr = fbmem;
		osd_hw.fb_gem[index].width = w;
		osd_hw.fb_gem[index].height = yres_virtual;

		if (osd_hw.osd_ver == OSD_SIMPLE) {
			u32 line_stride, fmt_mode, bpp;

			bpp = color->bpp / 8;
			fmt_mode = color->color_index;
			line_stride = line_stride_calc(fmt_mode,
				osd_hw.fb_gem[index].width / bpp, 1);
			VSYNCOSD_WR_MPEG_REG
				(hw_osd_reg_array[0].osd_blk1_cfg_w4,
				osd_hw.fb_gem[index].addr);
			VSYNCOSD_WR_MPEG_REG_BITS
				(hw_osd_reg_array[0].osd_blk2_cfg_w4,
				line_stride,
				0, 12);
		} else {
			if (osd_hw.mif_linear) {
				if (index >= VIU2_OSD1)
					/* for dual logo display */
					osd_hw.fb_gem[index].addr +=
						fb_gdev.fb_height *
						CANVAS_ALIGNED(fb_gdev.fb_width *
						color->bpp >> 3);

				osd_update_mif_linear_addr(index);
			}
		}
		osd_logd("osd[%d] canvas.idx =0x%x\n",
			 index, osd_hw.fb_gem[index].canvas_idx);
		osd_logd("osd[%d] canvas.addr=0x%x\n",
			 index, osd_hw.fb_gem[index].addr);
		osd_logd("osd[%d] canvas.width=%d\n",
			 index, CANVAS_ALIGNED(osd_hw.fb_gem[index].width));
		osd_logd("osd[%d] canvas.height=%d\n",
			 index, osd_hw.fb_gem[index].height);
	}
	/* osd blank only control by /sys/class/graphcis/fbx/blank */

	if (memcmp(&pan_data, &osd_hw.pandata[index],
		   sizeof(struct pandata_s)) != 0 ||
	    memcmp(&disp_data, &osd_hw.dispdata[index],
		   sizeof(struct pandata_s)) != 0) {
		update_geometry = 1;
		memcpy(&osd_hw.pandata[index], &pan_data,
		       sizeof(struct pandata_s));
		memcpy(&osd_hw.dispdata[index], &disp_data,
		       sizeof(struct pandata_s));
	}
	if (update_color_mode)
		add_to_update_list(index, OSD_COLOR_MODE);
	if (update_geometry)
		add_to_update_list(index, DISP_GEOMETRY);
	add_to_update_list(index, DISP_OSD_REVERSE);

	osd_wait_vsync_hw();
}

void osd_setpal_hw(u32 index,
		   unsigned regno,
		   unsigned red,
		   unsigned green,
		   unsigned blue,
		   unsigned transp
		  )
{
	if (regno < 256) {
		u32 pal;

		pal = ((red   & 0xff) << 24) |
		      ((green & 0xff) << 16) |
		      ((blue  & 0xff) <<  8) |
		      (transp & 0xff);
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_color_addr +
			REG_OFFSET * index,
				     regno);
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_color +
			REG_OFFSET * index, pal);
	}
}

void osd_get_order_hw(u32 index, u32 *order)
{
	*order = osd_hw.order & 0x3;
}

void osd_set_order_hw(u32 index, u32 order)
{
	if ((order != OSD_ORDER_01) && (order != OSD_ORDER_10))
		return;
	osd_hw.order = order;
	add_to_update_list(index, OSD_CHANGE_ORDER);
	osd_wait_vsync_hw();
}

/* vpu free scale mode */
static void osd_set_free_scale_enable_mode0(u32 index, u32 enable)
{
	static struct pandata_s save_disp_data = {0, 0, 0, 0};
#ifdef CONFIG_AM_VIDEO
#ifdef CONFIG_POST_PROCESS_MANAGER
	int mode_changed = 0;

	if ((index == OSD1) && (osd_hw.free_scale_enable[index] != enable))
		mode_changed = 1;
#endif
#endif

	osd_logi("osd%d free scale %s\n",
		 index, enable ? "ENABLE" : "DISABLE");
	enable = (enable & 0xffff ? 1 : 0);
	osd_hw.free_scale_enable[index] = enable;
	if (index == OSD1) {
		if (enable) {
			osd_vf_need_update = true;
#ifdef CONFIG_AM_VIDEO
			if ((osd_hw.free_scale_data[OSD1].x_end > 0)
			    && (osd_hw.free_scale_data[OSD1].x_end > 0)) {
				vf.width = osd_hw.free_scale_data[index].x_end -
					   osd_hw.free_scale_data[index].x_start + 1;
				vf.height =
					osd_hw.free_scale_data[index].y_end -
					osd_hw.free_scale_data[index].y_start + 1;
			} else {
				vf.width = osd_hw.free_scale_width[OSD1];
				vf.height = osd_hw.free_scale_height[OSD1];
			}
			vf.type = (VIDTYPE_NO_VIDEO_ENABLE | VIDTYPE_PROGRESSIVE
				   | VIDTYPE_VIU_FIELD | VIDTYPE_VSCALE_DISABLE);
			vf.ratio_control = DISP_RATIO_FORCECONFIG
					   | DISP_RATIO_NO_KEEPRATIO;
			if (osd_vf_prov_init == 0) {
				vf_provider_init(&osd_vf_prov,
						 PROVIDER_NAME, &osd_vf_provider, NULL);
				osd_vf_prov_init = 1;
			}
			vf_reg_provider(&osd_vf_prov);
			memcpy(&save_disp_data, &osd_hw.dispdata[OSD1],
			       sizeof(struct pandata_s));
			g_vf_visual_width =
				vf.width - 1 - osd_hw.dispdata[OSD1].x_start;
			g_vf_width = vf.width - 1;
			g_vf_height = vf.height - 1;
			osd_hw.dispdata[OSD1].x_end =
				osd_hw.dispdata[OSD1].x_start + vf.width - 1;
			osd_hw.dispdata[OSD1].y_end =
				osd_hw.dispdata[OSD1].y_start + vf.height - 1;
#endif
			osd_set_scan_mode(index);
			osd_hw.reg[index][DISP_GEOMETRY].update_func();
			osd_hw.reg[index][OSD_COLOR_MODE].update_func();
			osd_hw.reg[index][OSD_ENABLE].update_func();
		} else {
			osd_vf_need_update = false;
			osd_set_scan_mode(index);
			if (save_disp_data.x_end <= save_disp_data.x_start ||
			    save_disp_data.y_end <= save_disp_data.y_start)
				return;
			memcpy(&osd_hw.dispdata[OSD1], &save_disp_data,
			       sizeof(struct pandata_s));
			osd_hw.reg[index][DISP_GEOMETRY].update_func();
			osd_hw.reg[index][OSD_COLOR_MODE].update_func();
			osd_hw.reg[index][OSD_ENABLE].update_func();
#ifdef CONFIG_AM_VIDEO
			vf_unreg_provider(&osd_vf_prov);
#endif
		}
	} else {
		osd_hw.reg[index][DISP_GEOMETRY].update_func();
		osd_hw.reg[index][OSD_COLOR_MODE].update_func();
		osd_hw.reg[index][OSD_ENABLE].update_func();
	}
	osd_wait_vsync_hw();
#ifdef CONFIG_AM_VIDEO
#ifdef CONFIG_POST_PROCESS_MANAGER
	if (mode_changed) {
		/* extern void vf_ppmgr_reset(int type); */
		vf_ppmgr_reset(1);
	}
#endif
#endif
}

/* osd free scale mode */
static void osd_set_free_scale_enable_mode1(u32 index, u32 enable)
{
	unsigned int h_enable = 0;
	unsigned int v_enable = 0;
	int ret = 0;

	h_enable = (enable & 0xffff0000 ? 1 : 0);
	v_enable = (enable & 0xffff ? 1 : 0);
	osd_hw.free_scale[index].h_enable = h_enable;
	osd_hw.free_scale[index].v_enable = v_enable;
	osd_hw.free_scale_enable[index] = enable;
	if (osd_hw.free_scale_enable[index]) {
		if ((osd_hw.free_scale_data[index].x_end > 0) && h_enable) {
			osd_hw.free_scale_width[index] =
				osd_hw.free_scale_data[index].x_end -
				osd_hw.free_scale_data[index].x_start + 1;
		}
		if ((osd_hw.free_scale_data[index].y_end > 0) && v_enable) {
			osd_hw.free_scale_height[index] =
				osd_hw.free_scale_data[index].y_end -
				osd_hw.free_scale_data[index].y_start + 1;
		}
		ret = osd_set_scan_mode(index);
		if (ret)
			osd_hw.reg[index][OSD_COLOR_MODE].update_func();
		osd_hw.reg[index][OSD_FREESCALE_COEF].update_func();
		osd_hw.reg[index][DISP_GEOMETRY].update_func();
		osd_hw.reg[index][DISP_FREESCALE_ENABLE].update_func();
		osd_hw.reg[index][OSD_ENABLE].update_func();
	} else {
		ret = osd_set_scan_mode(index);
		if (ret)
			osd_hw.reg[index][OSD_COLOR_MODE].update_func();
		osd_hw.reg[index][DISP_GEOMETRY].update_func();
		osd_hw.reg[index][DISP_FREESCALE_ENABLE].update_func();
		osd_hw.reg[index][OSD_ENABLE].update_func();
	}
	osd_wait_vsync_hw();
}

void osd_set_free_scale_enable_hw(u32 index, u32 enable)
{
	if (osd_hw.free_scale_mode[index])
		osd_set_free_scale_enable_mode1(index, enable);
	else
		osd_set_free_scale_enable_mode0(index, enable);
}

void osd_get_free_scale_enable_hw(u32 index, u32 *free_scale_enable)
{
	*free_scale_enable = osd_hw.free_scale_enable[index];
}

void osd_set_free_scale_mode_hw(u32 index, u32 freescale_mode)
{
	osd_hw.free_scale_mode[index] = freescale_mode;
}

void osd_get_free_scale_mode_hw(u32 index, u32 *freescale_mode)
{
	*freescale_mode = osd_hw.free_scale_mode[index];
}

void osd_set_4k2k_fb_mode_hw(u32 fb_for_4k2k)
{
	osd_hw.fb_for_4k2k = fb_for_4k2k;
}

void osd_set_free_scale_width_hw(u32 index, u32 width)
{
	osd_hw.free_scale_width[index] = width;
	if (osd_hw.free_scale_enable[index] &&
	    (!osd_hw.free_scale_mode[index])) {
		osd_vf_need_update = true;
#ifdef CONFIG_AM_VIDEO
		vf.width = osd_hw.free_scale_width[index];
#endif
	}
}

void osd_get_free_scale_width_hw(u32 index, u32 *free_scale_width)
{
	*free_scale_width = osd_hw.free_scale_width[index];
}

void osd_set_free_scale_height_hw(u32 index, u32 height)
{
	osd_hw.free_scale_height[index] = height;
	if (osd_hw.free_scale_enable[index] &&
	    (!osd_hw.free_scale_mode[index])) {
		osd_vf_need_update = true;
#ifdef CONFIG_AM_VIDEO
		vf.height = osd_hw.free_scale_height[index];
#endif
	}
}

void osd_get_free_scale_height_hw(u32 index, u32 *free_scale_height)
{
	*free_scale_height = osd_hw.free_scale_height[index];
}

void osd_get_free_scale_axis_hw(u32 index, s32 *x0, s32 *y0, s32 *x1, s32 *y1)
{
	*x0 = osd_hw.free_scale_data[index].x_start;
	*y0 = osd_hw.free_scale_data[index].y_start;
	*x1 = osd_hw.free_scale_data[index].x_end;
	*y1 = osd_hw.free_scale_data[index].y_end;
}

void osd_set_free_scale_axis_hw(u32 index, s32 x0, s32 y0, s32 x1, s32 y1)
{
	osd_hw.free_scale_data[index].x_start = x0;
	osd_hw.free_scale_data[index].y_start = y0;
	osd_hw.free_scale_data[index].x_end = x1;
	osd_hw.free_scale_data[index].y_end = y1;
}

void osd_get_scale_axis_hw(u32 index, s32 *x0, s32 *y0, s32 *x1, s32 *y1)
{
	*x0 = osd_hw.scaledata[index].x_start;
	*x1 = osd_hw.scaledata[index].x_end;
	*y0 = osd_hw.scaledata[index].y_start;
	*y1 = osd_hw.scaledata[index].y_end;
}

void osd_set_scale_axis_hw(u32 index, s32 x0, s32 y0, s32 x1, s32 y1)
{
	osd_hw.scaledata[index].x_start = x0;
	osd_hw.scaledata[index].x_end = x1;
	osd_hw.scaledata[index].y_start = y0;
	osd_hw.scaledata[index].y_end = y1;
}

void osd_get_window_axis_hw(u32 index, s32 *x0, s32 *y0, s32 *x1, s32 *y1)
{
	int vmode = -1;

#ifdef CONFIG_AML_VOUT
	vmode = vout_get_current_vmode();
#endif
	switch (vmode) {
	/*case VMODE_LCD:*/
	case VMODE_480I:
	case VMODE_480CVBS:
	case VMODE_576I:
	case VMODE_576CVBS:
	case VMODE_1080I:
	case VMODE_1080I_50HZ:
#ifdef CONFIG_AML_VOUT_FRAMERATE_AUTOMATION
	case VMODE_1080I_59HZ:
#endif
		*y0 = osd_hw.free_dst_data[index].y_start * 2;
		*y1 = osd_hw.free_dst_data[index].y_end * 2;
		break;
	default:
		*y0 = osd_hw.free_dst_data[index].y_start;
		*y1 = osd_hw.free_dst_data[index].y_end;
		break;
	}
	*x0 = osd_hw.free_dst_data[index].x_start;
	*x1 = osd_hw.free_dst_data[index].x_end;
}

void osd_set_window_axis_hw(u32 index, s32 x0, s32 y0, s32 x1, s32 y1)
{
	int vmode = -1;

#ifdef CONFIG_AML_VOUT
	vmode = vout_get_current_vmode();
#endif
	switch (vmode) {
	/* case VMODE_LCD: */
	case VMODE_480I:
	case VMODE_480CVBS:
	case VMODE_576I:
	case VMODE_576CVBS:
	case VMODE_1080I:
	case VMODE_1080I_50HZ:
#ifdef CONFIG_AML_VOUT_FRAMERATE_AUTOMATION
	case VMODE_1080I_59HZ:
#endif
		osd_hw.free_dst_data[index].y_start = y0 / 2;
		osd_hw.free_dst_data[index].y_end = y1 / 2;
		break;
	default:
		osd_hw.free_dst_data[index].y_start = y0;
		osd_hw.free_dst_data[index].y_end = y1;
		break;
	}
	osd_hw.free_dst_data[index].x_start = x0;
	osd_hw.free_dst_data[index].x_end = x1;
	osd_logi("osd_hw.free_dst_data: %d,%d,%d,%d\n",x0,x1,y0,y1);
#if defined(CONFIG_FB_OSD2_CURSOR)
	osd_hw.cursor_dispdata[index].x_start = x0;
	osd_hw.cursor_dispdata[index].x_end = x1;
	osd_hw.cursor_dispdata[index].y_start = y0;
	osd_hw.cursor_dispdata[index].y_end = y1;
#endif
}

void osd_get_block_windows_hw(u32 index, u32 *windows)
{
	memcpy(windows, osd_hw.block_windows[index],
	       sizeof(osd_hw.block_windows[index]));
}

void osd_set_block_windows_hw(u32 index, u32 *windows)
{
	memcpy(osd_hw.block_windows[index], windows,
	       sizeof(osd_hw.block_windows[index]));
	add_to_update_list(index, DISP_GEOMETRY);
	osd_wait_vsync_hw();
}

void osd_get_block_mode_hw(u32 index, u32 *mode)
{
	*mode = osd_hw.block_mode[index];
}

void osd_set_block_mode_hw(u32 index, u32 mode)
{
	osd_hw.block_mode[index] = mode;
	add_to_update_list(index, DISP_GEOMETRY);
	osd_wait_vsync_hw();
}

void osd_enable_3d_mode_hw(u32 index, u32 enable)
{
	osd_hw.mode_3d[index].enable = enable;
	if (enable) {
		/* when disable 3d mode ,we should return to stardard state. */
		osd_hw.mode_3d[index].left_right = OSD_LEFT;
		osd_hw.mode_3d[index].l_start = osd_hw.pandata[index].x_start;
		osd_hw.mode_3d[index].l_end = (osd_hw.pandata[index].x_end +
					       osd_hw.pandata[index].x_start) >> 1;
		osd_hw.mode_3d[index].r_start = osd_hw.mode_3d[index].l_end + 1;
		osd_hw.mode_3d[index].r_end = osd_hw.pandata[index].x_end;
		osd_hw.mode_3d[index].origin_scale.h_enable =
			osd_hw.scale[index].h_enable;
		osd_hw.mode_3d[index].origin_scale.v_enable =
			osd_hw.scale[index].v_enable;
		osd_set_2x_scale_hw(index, 1, 0);
	} else {
		osd_set_2x_scale_hw(index,
				    osd_hw.mode_3d[index].origin_scale.h_enable,
				    osd_hw.mode_3d[index].origin_scale.v_enable);
	}
}

void osd_enable_hw(u32 index, u32 enable)
{
	osd_logd("osd[%d] enable: %d\n", index, enable);

	osd_hw.enable[index] = enable;
	add_to_update_list(index, OSD_ENABLE);
	osd_wait_vsync_hw();
}

void osd_set_2x_scale_hw(u32 index, u16 h_scale_enable, u16 v_scale_enable)
{
	osd_logi("osd[%d] set scale, h_scale: %s, v_scale: %s\n",
		 index, h_scale_enable ? "ENABLE" : "DISABLE",
		 v_scale_enable ? "ENABLE" : "DISABLE");
	osd_logi("osd[%d].scaledata: %d %d %d %d\n",
		 index,
		 osd_hw.scaledata[index].x_start,
		 osd_hw.scaledata[index].x_end,
		 osd_hw.scaledata[index].y_start,
		 osd_hw.scaledata[index].y_end);
	osd_logi("osd[%d].pandata: %d %d %d %d\n",
		 index,
		 osd_hw.pandata[index].x_start,
		 osd_hw.pandata[index].x_end,
		 osd_hw.pandata[index].y_start,
		 osd_hw.pandata[index].y_end);
	osd_hw.scale[index].h_enable = h_scale_enable;
	osd_hw.scale[index].v_enable = v_scale_enable;
	osd_hw.reg[index][DISP_SCALE_ENABLE].update_func();
	osd_hw.reg[index][DISP_GEOMETRY].update_func();
	osd_wait_vsync_hw();
}

void osd_set_rotate_angle_hw(u32 index, u32 angle)
{
	osd_hw.rotate[index].angle = angle;
	add_to_update_list(index, DISP_OSD_ROTATE);

	osd_wait_vsync_hw();
}

void osd_get_rotate_angle_hw(u32 index, u32 *angle)
{
	*angle = osd_hw.rotate[index].angle;
}

void osd_set_rotate_on_hw(u32 index, u32 on_off)
{
	osd_hw.rotate[index].on_off = on_off;
	if (on_off) {
		g_rotation_width = osd_hw.rotation_pandata[index].x_end -
				   osd_hw.rotation_pandata[index].x_start;
		g_rotation_height = osd_hw.rotation_pandata[index].y_end -
				    osd_hw.rotation_pandata[index].y_start;
		osd_hw.dispdata[index].x_end = osd_hw.dispdata[OSD1].x_start +
					       g_rotation_height;
		osd_hw.dispdata[index].y_end = osd_hw.dispdata[OSD1].y_start +
					       g_rotation_width;
	} else {
		if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_M8) {
			osd_reg_set_mask(VPU_SW_RESET, 1 << 8);
			osd_reg_clr_mask(VPU_SW_RESET, 1 << 8);
		}
		if (index == OSD1) {
			if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_M8) {
				osd_reg_set_mask(VIU_SW_RESET, 1 << 0);
				osd_reg_clr_mask(VIU_SW_RESET, 1 << 0);
			}
			VSYNCOSD_SET_MPEG_REG_MASK(hw_osd_reg_array[0].osd_fifo_ctrl_stat,
						   1 << 0);
		} else {
			osd_reg_set_mask(VIU_SW_RESET, 1 << 1);
			osd_reg_clr_mask(VIU_SW_RESET, 1 << 1);
			VSYNCOSD_SET_MPEG_REG_MASK(hw_osd_reg_array[1].osd_fifo_ctrl_stat,
						   1 << 0);
		}
	}
	osd_hw.reg[index][OSD_COLOR_MODE].update_func();
	osd_hw.reg[index][DISP_GEOMETRY].update_func();
	osd_hw.reg[index][DISP_OSD_ROTATE].update_func();
	osd_wait_vsync_hw();
}

void osd_get_rotate_on_hw(u32 index, u32 *on_off)
{
	*on_off = osd_hw.rotate[index].on_off;
}

void osd_get_update_state_hw(u32 index, u32 *up_free)
{
	if (osd_vf_need_update)
		*up_free = 1;
	else
		*up_free = 0;
}

void osd_set_update_state_hw(u32 index, u32 up_free)
{
	if (up_free > 0)
		osd_vf_need_update = true;
	else
		osd_vf_need_update = false;
}

void osd_set_reverse_hw(u32 index, u32 reverse)
{
	osd_hw.osd_reverse[index] = reverse;
	add_to_update_list(index, DISP_OSD_REVERSE);
	osd_wait_vsync_hw();
}

void osd_get_reverse_hw(u32 index, u32 *reverse)
{
	*reverse = osd_hw.osd_reverse[index];
}

void osd_set_prot_canvas_hw(u32 index, s32 x_start, s32 y_start, s32 x_end,
			    s32 y_end)
{
	osd_hw.rotation_pandata[index].x_start = x_start;
	osd_hw.rotation_pandata[index].y_start = y_start;
	osd_hw.rotation_pandata[index].x_end = x_end;
	osd_hw.rotation_pandata[index].y_end = y_end;
	if (osd_hw.rotate[index].on_off && osd_hw.rotate[index].angle > 0) {
		g_rotation_width = osd_hw.rotation_pandata[index].x_end -
				   osd_hw.rotation_pandata[index].x_start;
		g_rotation_height = osd_hw.rotation_pandata[index].y_end -
				    osd_hw.rotation_pandata[index].y_start;
		osd_hw.dispdata[index].x_end = osd_hw.dispdata[OSD1].x_start +
					       g_rotation_height;
		osd_hw.dispdata[index].y_end = osd_hw.dispdata[OSD1].y_start +
					       g_rotation_width;
		osd_hw.reg[index][DISP_GEOMETRY].update_func();
		osd_hw.reg[index][OSD_COLOR_MODE].update_func();
	}
}

void osd_get_prot_canvas_hw(u32 index, s32 *x_start, s32 *y_start, s32 *x_end,
			    s32 *y_end)
{
	*x_start = osd_hw.rotation_pandata[index].x_start;
	*y_start = osd_hw.rotation_pandata[index].y_start;
	*x_end = osd_hw.rotation_pandata[index].x_end;
	*y_end = osd_hw.rotation_pandata[index].y_end;
}

void osd_pan_display_hw(u32 index, unsigned int xoffset, unsigned int yoffset)
{
	long diff_x, diff_y;

#if defined(CONFIG_FB_OSD2_CURSOR)
	if (index >= 1)
#else
	if (index >= 2)
#endif
		return;
	if (xoffset != osd_hw.pandata[index].x_start
	    || yoffset != osd_hw.pandata[index].y_start) {
		diff_x = xoffset - osd_hw.pandata[index].x_start;
		diff_y = yoffset - osd_hw.pandata[index].y_start;
		osd_hw.pandata[index].x_start += diff_x;
		osd_hw.pandata[index].x_end   += diff_x;
		osd_hw.pandata[index].y_start += diff_y;
		osd_hw.pandata[index].y_end   += diff_y;
		add_to_update_list(index, DISP_GEOMETRY);
		osd_wait_vsync_hw();
	}
#ifdef CONFIG_AM_FB_EXT
	osd_ext_clone_pan(index);
#endif
	osd_logd2("offset[%d-%d]x[%d-%d]y[%d-%d]\n",
		  xoffset, yoffset,
		  osd_hw.pandata[index].x_start,
		  osd_hw.pandata[index].x_end,
		  osd_hw.pandata[index].y_start,
		  osd_hw.pandata[index].y_end);
}

static  void  osd_update_disp_scale_enable(u32 index)
{
	if (osd_hw.scale[index].h_enable)
		VSYNCOSD_SET_MPEG_REG_MASK(hw_osd_reg_array[index].osd_blk0_cfg_w0,
			3 << 12);
	else
		VSYNCOSD_CLR_MPEG_REG_MASK(hw_osd_reg_array[index].osd_blk0_cfg_w0,
			3 << 12);
	if (osd_hw.scan_mode != SCAN_MODE_INTERLACE) {
		if (osd_hw.scale[index].v_enable)
			VSYNCOSD_SET_MPEG_REG_MASK(hw_osd_reg_array[index].osd_blk0_cfg_w0,
						   1 << 14);
		else
			VSYNCOSD_CLR_MPEG_REG_MASK(hw_osd_reg_array[index].osd_blk0_cfg_w0,
						   1 << 14);
	}
}

static  void  osd1_update_disp_scale_enable(void)
{
	osd_update_disp_scale_enable(OSD1);
}

static  void  osd2_update_disp_scale_enable(void)
{
	osd_update_disp_scale_enable(OSD2);
}

static void viu2_osd1_update_disp_scale_enable(void)
{
	osd_update_disp_scale_enable(VIU2_OSD1);
}

static void viu3_osd1_update_disp_scale_enable(void)
{
	osd_update_disp_scale_enable(VIU3_OSD1);
}

static void osd_super_scale_enable(u32 index)
{
#ifndef AML_OSD_HIGH_VERSION
	u32 data32 = 0x0;

	osd_super_scale_mem_power_on();
	/* enable osd scaler path */
	if (index == OSD1)
		data32 = 8;
	else {
		data32 = 1;       /* select osd2 input */
		data32 |= 1 << 3; /* enable osd scaler path */
	}
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_sc_ctrl0, data32);
	/* enable osd super scaler */
	data32 = (1 << 0)
		 | (1 << 1)
		 | (1 << 2);
	VSYNCOSD_WR_MPEG_REG(OSDSR_CTRL_MODE, data32);
	/* config osd super scaler setting */
	VSYNCOSD_WR_MPEG_REG(OSDSR_UK_GRAD2DDIAG_LIMIT, 0xffffff);
	VSYNCOSD_WR_MPEG_REG(OSDSR_UK_GRAD2DADJA_LIMIT, 0xffffff);
	VSYNCOSD_WR_MPEG_REG(OSDSR_UK_BST_GAIN, 0x7a7a3a50);
	/* config osd super scaler input size */
	data32 = (osd_hw.free_scale_height[index] & 0x1fff)
		 | (osd_hw.free_scale_width[index] & 0x1fff) << 16;
	VSYNCOSD_WR_MPEG_REG(OSDSR_HV_SIZEIN, data32);
	/* config osd super scaler output size */
	data32 = ((osd_hw.free_dst_data[index].x_end & 0xfff) |
		  (osd_hw.free_dst_data[index].x_start & 0xfff) << 16);
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_sco_h_start_end, data32);
	data32 = ((osd_hw.free_dst_data[index].y_end & 0xfff) |
		  (osd_hw.free_dst_data[index].y_start & 0xfff) << 16);
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_sco_v_start_end, data32);
#endif
}

static void osd_super_scale_disable(void)
{
#ifndef AML_OSD_HIGH_VERSION
	/* disable osd scaler path */
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_sc_ctrl0, 0);
	/* disable osd super scaler */
	VSYNCOSD_WR_MPEG_REG(OSDSR_HV_SIZEIN, 0);
	VSYNCOSD_WR_MPEG_REG(OSDSR_CTRL_MODE, 0);
	osd_super_scale_mem_power_off();
#endif
}

#define NEW_PPS_PHASE
#ifdef NEW_PPS_PHASE
#define OSD_ZOOM_BITS 20
#define OSD_PHASE_BITS 16

enum osd_f2v_vphase_type_e {
	OSD_F2V_IT2IT = 0,
	OSD_F2V_IB2IB,
	OSD_F2V_IT2IB,
	OSD_F2V_IB2IT,
	OSD_F2V_P2IT,
	OSD_F2V_P2IB,
	OSD_F2V_IT2P,
	OSD_F2V_IB2P,
	OSD_F2V_P2P,
	OSD_F2V_TYPE_MAX
};

struct osd_f2v_vphase_s {
	u8 rcv_num;
	u8 rpt_num;
	u16 phase;
};

static void f2v_get_vertical_phase(u32 zoom_ratio,
	enum osd_f2v_vphase_type_e type,
	u8 bank_length,
	struct osd_f2v_vphase_s *vphase)
{
	u8 f2v_420_in_pos_luma[OSD_F2V_TYPE_MAX] = {
		0, 2, 0, 2, 0, 0, 0, 2, 0};
	u8 f2v_420_out_pos[OSD_F2V_TYPE_MAX] = {
		0, 2, 2, 0, 0, 2, 0, 0, 0};
	s32 offset_in, offset_out;

	/* luma */
	offset_in = f2v_420_in_pos_luma[type]
		<< OSD_PHASE_BITS;
	offset_out = (f2v_420_out_pos[type] * zoom_ratio)
		>> (OSD_ZOOM_BITS - OSD_PHASE_BITS);

	vphase->rcv_num = bank_length;
	if (bank_length == 4 || bank_length == 3)
		vphase->rpt_num = 1;
	else
		vphase->rpt_num = 0;

	if (offset_in > offset_out) {
		vphase->rpt_num = vphase->rpt_num + 1;
		vphase->phase =
			((4 << OSD_PHASE_BITS) + offset_out - offset_in)
			>> 2;
	} else {
		while ((offset_in + (4 << OSD_PHASE_BITS))
			<= offset_out) {
			if (vphase->rpt_num == 1)
				vphase->rpt_num = 0;
			else
				vphase->rcv_num++;
			offset_in += 4 << OSD_PHASE_BITS;
		}
		vphase->phase = (offset_out - offset_in) >> 2;
	}
}
#endif

static void osd_update_disp_freescale_enable(u32 index, u32 reg_index)
{
	u64 hf_phase_step, vf_phase_step;
	int src_w, src_h, dst_w, dst_h;
	int bot_ini_phase, top_ini_phase;
	int vsc_ini_rcv_num, vsc_ini_rpt_p0_num;
	int vsc_bot_rcv_num = 0, vsc_bot_rpt_p0_num = 0;
	int hsc_ini_rcv_num, hsc_ini_rpt_p0_num;
	int hf_bank_len = 4;
	int vf_bank_len = 4;
	u32 data32 = 0x0;
	u32 shift_line = osd_hw.shift_line;

	osd_logi("osd%d_update_disp_freescale_enable\n", index);
	if (osd_hw.scale_workaround)
		vf_bank_len = 2;
	else
		vf_bank_len = 4;

#ifndef NEW_PPS_PHASE
	if (osd_hw.bot_type == 1) {
		vsc_bot_rcv_num = 4;
		vsc_bot_rpt_p0_num = 1;
	} else if (osd_hw.bot_type == 2) {
		vsc_bot_rcv_num = 6;
		vsc_bot_rpt_p0_num = 2;
	} else if (osd_hw.bot_type == 3) {
		vsc_bot_rcv_num = 8;
		vsc_bot_rpt_p0_num = 3;
	}
	vsc_ini_rcv_num = vf_bank_len;
	vsc_ini_rpt_p0_num =
		(vf_bank_len / 2 - 1) > 0 ? (vf_bank_len / 2 - 1) : 0;
#endif
	hsc_ini_rcv_num = hf_bank_len;
	hsc_ini_rpt_p0_num = hf_bank_len / 2 - 1;

	src_w = osd_hw.free_scale_width[index];
	src_h = osd_hw.free_scale_height[index];
	dst_w = osd_hw.free_dst_data[index].x_end -
		osd_hw.free_dst_data[index].x_start + 1;
	dst_h = osd_hw.free_dst_data[index].y_end -
		osd_hw.free_dst_data[index].y_start + 1;
	if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_MG9TV) {
		/* super scaler mode */
		if (osd_hw.free_scale_mode[index] & 0x2) {
			if (osd_hw.free_scale_enable[index])
				osd_super_scale_enable(index);
			else
				osd_super_scale_disable();
			remove_from_update_list(index, DISP_FREESCALE_ENABLE);
			return;
		}
	}
	/* config osd sc control reg */
	data32 = 0x0;
	if (osd_hw.free_scale_enable[index]) {
		/* enable osd scaler */
		if (osd_hw.free_scale_mode[index] & 0x1) {
			if (osd_hw.osd_ver == OSD_NORMAL)
				data32 |= index;  /* select osd1/2 input */
			data32 |= 1 << 2; /* enable osd scaler */
			data32 |= 1 << 3; /* enable osd scaler path */
			VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[reg_index].osd_sc_ctrl0, data32);
		}
	} else {
		/* disable osd scaler path */
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[reg_index].osd_sc_ctrl0, 0);
	}

	hf_phase_step = (u64)src_w << 24;
	if (dst_w == 0)
		dst_w = 1;
	do_div(hf_phase_step, dst_w);
	if (shift_line) {
		vf_phase_step = (u64)(src_h - 1) << 20;
		if (dst_h == 0)
			dst_h = 1;
		do_div(vf_phase_step, dst_h);
	} else {
		vf_phase_step = (u64)src_h << 20;
		if (dst_h == 0)
			dst_h = 1;
		do_div(vf_phase_step, dst_h);
	}

#ifdef NEW_PPS_PHASE
	if (osd_hw.field_out_en) {
		struct osd_f2v_vphase_s vphase;

		f2v_get_vertical_phase(vf_phase_step,
			OSD_F2V_P2IT,
			vf_bank_len,
			&vphase);
		vsc_ini_rcv_num = vphase.rcv_num;
		vsc_ini_rpt_p0_num = vphase.rpt_num;
		top_ini_phase = vphase.phase;

		f2v_get_vertical_phase(vf_phase_step,
			OSD_F2V_P2IB,
			vf_bank_len,
			&vphase);
		vsc_bot_rcv_num = vphase.rcv_num;
		vsc_bot_rpt_p0_num = vphase.rpt_num;
		bot_ini_phase = vphase.phase;
	} else {
		struct osd_f2v_vphase_s vphase;

		f2v_get_vertical_phase(vf_phase_step,
			OSD_F2V_P2P,
			vf_bank_len,
			&vphase);
		vsc_ini_rcv_num = vphase.rcv_num;
		vsc_ini_rpt_p0_num = vphase.rpt_num;
		top_ini_phase = vphase.phase;

		vsc_bot_rcv_num = 0;
		vsc_bot_rpt_p0_num = 0;
		bot_ini_phase = 0;
	}
#else
	if (osd_hw.field_out_en) /* interface output */
		bot_ini_phase = ((vf_phase_step / 2) >> 4);
	else
		bot_ini_phase = 0;
	top_ini_phase = 0;
#endif

	vf_phase_step = (vf_phase_step << 4);
	/* config osd scaler in/out hv size */
	if (shift_line) {
		vsc_ini_rcv_num++;
		if (osd_hw.field_out_en)
			vsc_bot_rcv_num++;
	}

	data32 = 0x0;
	if (osd_hw.free_scale_enable[index]) {
		data32 = (((src_h - 1 + shift_line) & 0x1fff)
			  | ((src_w - 1) & 0x1fff) << 16);
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[reg_index].osd_sci_wh_m1, data32);
		data32 = ((osd_hw.free_dst_data[index].x_end & 0xfff) |
			  (osd_hw.free_dst_data[index].x_start & 0xfff) << 16);
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[reg_index].osd_sco_h_start_end, data32);
		data32 = ((osd_hw.free_dst_data[index].y_end & 0xfff) |
			  (osd_hw.free_dst_data[index].y_start & 0xfff) << 16);
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[reg_index].osd_sco_v_start_end, data32);
	}
	data32 = 0x0;
	if (osd_hw.free_scale[index].v_enable) {
		data32 |= (vf_bank_len & 0x7)
			  | ((vsc_ini_rcv_num & 0xf) << 3)
			  | ((vsc_ini_rpt_p0_num & 0x3) << 8);
		if (osd_hw.field_out_en)
			data32 |= ((vsc_bot_rcv_num & 0xf) << 11)
				  | ((vsc_bot_rpt_p0_num & 0x3) << 16)
				  | (1 << 23);
		if (osd_hw.scale_workaround)
			data32 |= 1 << 21;
		data32 |= 1 << 24;
		if (osd_get_chip_type() >= MESON_CPU_MAJOR_ID_G12B)
			data32 |= 1 << 25;
	}
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[reg_index].osd_vsc_ctrl0, data32);
	data32 = 0x0;
	if (osd_hw.free_scale[index].h_enable) {
		data32 |= (hf_bank_len & 0x7)
			  | ((hsc_ini_rcv_num & 0xf) << 3)
			  | ((hsc_ini_rpt_p0_num & 0x3) << 8);
		data32 |= 1 << 22;
	}
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[reg_index].osd_hsc_ctrl0, data32);

	data32 = top_ini_phase;
	if (osd_hw.free_scale_enable[index]) {
		data32 |= (bot_ini_phase & 0xffff) << 16;
		if (osd_hw.field_out_en) {
			if (shift_line)
				src_h--;
			if (src_h == dst_h * 2)
				data32 |= 0x80008000;
		}
		VSYNCOSD_WR_MPEG_REG_BITS(hw_osd_reg_array[reg_index].osd_hsc_phase_step,
					  hf_phase_step, 0, 28);
		VSYNCOSD_WR_MPEG_REG_BITS(hw_osd_reg_array[reg_index].osd_hsc_init_phase,
			0, 0, 16);
		VSYNCOSD_WR_MPEG_REG_BITS(hw_osd_reg_array[reg_index].osd_vsc_phase_step,
					  vf_phase_step, 0, 28);
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[reg_index].osd_vsc_init_phase, data32);
	}
	remove_from_update_list(index, DISP_FREESCALE_ENABLE);
}

static void osd1_update_disp_freescale_enable(void)
{
	osd_update_disp_freescale_enable(OSD1, OSD1);
}

static void osd2_update_disp_freescale_enable(void)
{
	osd_update_disp_freescale_enable(OSD2, OSD1);
}

#ifdef AML_T7_DISPLAY
static void osdx_update_disp_freescale_enable(u32 index)
{
	u64 hf_phase_step, vf_phase_step;
	int src_w, src_h, dst_w, dst_h;
	int bot_ini_phase, top_ini_phase;
	int vsc_ini_rcv_num, vsc_ini_rpt_p0_num;
	int vsc_bot_rcv_num = 0, vsc_bot_rpt_p0_num = 0;
	int hsc_ini_rcv_num, hsc_ini_rpt_p0_num;
	int hf_bank_len = 4;
	int vf_bank_len = 4;
	u32 data32 = 0x0;
	u32 shift_line = osd_hw.shift_line;
	int osd_index = -1;
	u32 osd_sc_ctrl0 = 0, osd_sci_wh_m1 = 0;
	u32 osd_sco_h_start_end = 0, osd_sco_v_start_end = 0;
	u32 osd_sco_hsc_ctrl0 = 0, osd_sco_vsc_ctrl0 = 0;
	u32 osd_sco_hsc_phase_step = 0, osd_sco_vsc_phase_step = 0;
	u32 osd_sco_hsc_ini_phase = 0, osd_sco_vsc_ini_phase = 0;

	osd_index = get_osd_layer();
	if (osd_index < 0) {
		osd_loge("%s: invalid osd_index\n", __func__);
		return;
	}

	if (osd_index < VIU2_OSD1) {
		osd_sc_ctrl0 = hw_osd_reg_array[0].osd_sc_ctrl0;
		osd_sci_wh_m1 = hw_osd_reg_array[0].osd_sci_wh_m1;
		osd_sco_h_start_end = hw_osd_reg_array[0].osd_sco_h_start_end;
		osd_sco_v_start_end = hw_osd_reg_array[0].osd_sco_v_start_end;
		osd_sco_hsc_ctrl0 = hw_osd_reg_array[0].osd_hsc_ctrl0;
		osd_sco_vsc_ctrl0 = hw_osd_reg_array[0].osd_vsc_ctrl0;
		osd_sco_hsc_phase_step = hw_osd_reg_array[0].osd_hsc_phase_step;
		osd_sco_hsc_ini_phase = hw_osd_reg_array[0].osd_hsc_init_phase;
		osd_sco_vsc_phase_step = hw_osd_reg_array[0].osd_vsc_phase_step;
		osd_sco_vsc_ini_phase = hw_osd_reg_array[0].osd_vsc_init_phase;
	} else if (osd_index == VIU2_OSD1) {
		osd_sc_ctrl0 = hw_osd_reg_array[2].osd_sc_ctrl0;
		osd_sci_wh_m1 = hw_osd_reg_array[2].osd_sci_wh_m1;
		osd_sco_h_start_end = hw_osd_reg_array[2].osd_sco_h_start_end;
		osd_sco_v_start_end = hw_osd_reg_array[2].osd_sco_v_start_end;
		osd_sco_hsc_ctrl0 = hw_osd_reg_array[2].osd_hsc_ctrl0;
		osd_sco_vsc_ctrl0 = hw_osd_reg_array[2].osd_vsc_ctrl0;
		osd_sco_hsc_phase_step = hw_osd_reg_array[2].osd_hsc_phase_step;
		osd_sco_hsc_ini_phase = hw_osd_reg_array[2].osd_hsc_init_phase;
		osd_sco_vsc_phase_step = hw_osd_reg_array[2].osd_vsc_phase_step;
		osd_sco_vsc_ini_phase = hw_osd_reg_array[2].osd_vsc_init_phase;
	} else if (osd_index == VIU3_OSD1) {
		osd_sc_ctrl0 = hw_osd_reg_array[3].osd_sc_ctrl0;
		osd_sci_wh_m1 = hw_osd_reg_array[3].osd_sci_wh_m1;
		osd_sco_h_start_end = hw_osd_reg_array[3].osd_sco_h_start_end;
		osd_sco_v_start_end = hw_osd_reg_array[3].osd_sco_v_start_end;
		osd_sco_hsc_ctrl0 = hw_osd_reg_array[3].osd_hsc_ctrl0;
		osd_sco_vsc_ctrl0 = hw_osd_reg_array[3].osd_vsc_ctrl0;
		osd_sco_hsc_phase_step = hw_osd_reg_array[3].osd_hsc_phase_step;
		osd_sco_hsc_ini_phase = hw_osd_reg_array[3].osd_hsc_init_phase;
		osd_sco_vsc_phase_step = hw_osd_reg_array[3].osd_vsc_phase_step;
		osd_sco_vsc_ini_phase = hw_osd_reg_array[3].osd_vsc_init_phase;
	} else {
		osd_loge("%s: invalid osd_index\n", __func__);
		return;
	}

	if (osd_hw.scale_workaround)
		vf_bank_len = 2;
	else
		vf_bank_len = 4;

#ifndef NEW_PPS_PHASE
	if (osd_hw.bot_type == 1) {
		vsc_bot_rcv_num = 4;
		vsc_bot_rpt_p0_num = 1;
	} else if (osd_hw.bot_type == 2) {
		vsc_bot_rcv_num = 6;
		vsc_bot_rpt_p0_num = 2;
	} else if (osd_hw.bot_type == 3) {
		vsc_bot_rcv_num = 8;
		vsc_bot_rpt_p0_num = 3;
	}
	vsc_ini_rcv_num = vf_bank_len;
	vsc_ini_rpt_p0_num =
		(vf_bank_len / 2 - 1) > 0 ? (vf_bank_len / 2 - 1) : 0;
#endif
	hsc_ini_rcv_num = hf_bank_len;
	hsc_ini_rpt_p0_num = hf_bank_len / 2 - 1;

	src_w = osd_hw.free_scale_width[index];
	src_h = osd_hw.free_scale_height[index];
	dst_w = osd_hw.free_dst_data[index].x_end -
		osd_hw.free_dst_data[index].x_start + 1;
	dst_h = osd_hw.free_dst_data[index].y_end -
		osd_hw.free_dst_data[index].y_start + 1;

	if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_MG9TV) {
		/* super scaler mode */
		if (osd_hw.free_scale_mode[index] & 0x2) {
			if (osd_hw.free_scale_enable[index])
				osd_super_scale_enable(index);
			else
				osd_super_scale_disable();
			remove_from_update_list(index, DISP_FREESCALE_ENABLE);
			return;
		}
	}
	/* config osd sc control reg */
	data32 = 0x0;
	if (osd_hw.free_scale_enable[index]) {
		/* enable osd scaler */
		if (osd_hw.free_scale_mode[index] & 0x1) {
			data32 |= 1;      /* select osd2 input */
			data32 |= 1 << 2; /* enable osd scaler */
			data32 |= 1 << 3; /* enable osd scaler path */
			VSYNCOSD_WR_MPEG_REG(osd_sc_ctrl0, data32);
		}
	} else {
		/* disable osd scaler path */
		VSYNCOSD_WR_MPEG_REG(osd_sc_ctrl0, 0);
	}

	hf_phase_step = (u64)src_w << 24;
	if (dst_w == 0)
		dst_w = 1;
	do_div(hf_phase_step, dst_w);
	if (shift_line) {
		vf_phase_step = (u64)(src_h - 1) << 20;
		if (dst_h == 0)
			dst_h = 1;
		do_div(vf_phase_step, dst_h);
	} else {
		vf_phase_step = (u64)src_h << 20;
		if (dst_h == 0)
			dst_h = 1;
		do_div(vf_phase_step, dst_h);
	}

#ifdef NEW_PPS_PHASE
	if (osd_hw.field_out_en) {
		struct osd_f2v_vphase_s vphase;

		f2v_get_vertical_phase(vf_phase_step,
			OSD_F2V_P2IT,
			vf_bank_len,
			&vphase);
		vsc_ini_rcv_num = vphase.rcv_num;
		vsc_ini_rpt_p0_num = vphase.rpt_num;
		top_ini_phase = vphase.phase;

		f2v_get_vertical_phase(vf_phase_step,
			OSD_F2V_P2IB,
			vf_bank_len,
			&vphase);
		vsc_bot_rcv_num = vphase.rcv_num;
		vsc_bot_rpt_p0_num = vphase.rpt_num;
		bot_ini_phase = vphase.phase;
	} else {
		struct osd_f2v_vphase_s vphase;

		f2v_get_vertical_phase(vf_phase_step,
			OSD_F2V_P2P,
			vf_bank_len,
			&vphase);
		vsc_ini_rcv_num = vphase.rcv_num;
		vsc_ini_rpt_p0_num = vphase.rpt_num;
		top_ini_phase = vphase.phase;

		vsc_bot_rcv_num = 0;
		vsc_bot_rpt_p0_num = 0;
		bot_ini_phase = 0;
	}
#else
	if (osd_hw.field_out_en)	 /* interface output */
		bot_ini_phase = ((vf_phase_step / 2) >> 4);
	else
		bot_ini_phase = 0;
	top_ini_phase = 0;
#endif

	vf_phase_step = (vf_phase_step << 4);
	/* config osd scaler in/out hv size */
	if (shift_line) {
		vsc_ini_rcv_num++;
		if (osd_hw.field_out_en)
			vsc_bot_rcv_num++;
	}

	data32 = 0x0;
	if (osd_hw.free_scale_enable[index]) {
		data32 = (((src_h - 1 + shift_line) & 0x1fff)
			  | ((src_w - 1) & 0x1fff) << 16);
		VSYNCOSD_WR_MPEG_REG(osd_sci_wh_m1, data32);
		data32 = ((osd_hw.free_dst_data[index].x_end & 0xfff) |
			  (osd_hw.free_dst_data[index].x_start & 0xfff) << 16);
		VSYNCOSD_WR_MPEG_REG(osd_sco_h_start_end, data32);
		data32 = ((osd_hw.free_dst_data[index].y_end & 0xfff) |
			  (osd_hw.free_dst_data[index].y_start & 0xfff) << 16);
		VSYNCOSD_WR_MPEG_REG(osd_sco_v_start_end, data32);
	}
	data32 = 0x0;
	if (osd_hw.free_scale[index].h_enable) {
		data32 |= (hf_bank_len & 0x7)
			  | ((hsc_ini_rcv_num & 0xf) << 3)
			  | ((hsc_ini_rpt_p0_num & 0x3) << 8);
		data32 |= 1 << 22;
	}
	VSYNCOSD_WR_MPEG_REG(osd_sco_hsc_ctrl0, data32);
	data32 = 0x0;
	if (osd_hw.free_scale[index].v_enable) {
		data32 |= (vf_bank_len & 0x7)
			  | ((vsc_ini_rcv_num & 0xf) << 3)
			  | ((vsc_ini_rpt_p0_num & 0x3) << 8);
		if (osd_hw.field_out_en)   /* interface output */
			data32 |= ((vsc_bot_rcv_num & 0xf) << 11)
				  | ((vsc_bot_rpt_p0_num & 0x3) << 16)
				  | (1 << 23);
		if (osd_hw.scale_workaround)
			data32 |= 1 << 21;
		data32 |= 1 << 24;
		if (osd_get_chip_type() >= MESON_CPU_MAJOR_ID_G12B)
			data32 |= 1 << 25;
	}
	VSYNCOSD_WR_MPEG_REG(osd_sco_vsc_ctrl0, data32);
	data32 = top_ini_phase;
	if (osd_hw.free_scale_enable[index]) {
		data32 |= (bot_ini_phase & 0xffff) << 16;
		if (osd_hw.field_out_en) {
			if (shift_line)
				src_h--;
			if (src_h == dst_h * 2)
				data32 |= 0x80008000;
		}
		VSYNCOSD_WR_MPEG_REG_BITS(osd_sco_hsc_phase_step,
					  hf_phase_step, 0, 28);
		VSYNCOSD_WR_MPEG_REG_BITS(osd_sco_hsc_ini_phase, 0, 0, 16);
		VSYNCOSD_WR_MPEG_REG_BITS(osd_sco_vsc_phase_step,
					  vf_phase_step, 0, 28);
		VSYNCOSD_WR_MPEG_REG(osd_sco_vsc_ini_phase, data32);
	}
	if (osd_index == VIU2_OSD1) {
		/* hdr in size */
		VSYNCOSD_WR_MPEG_REG(OSD3_HDR_IN_SIZE, dst_h << 16 | dst_w);
		/* vpp_top1 scope */
		VSYNCOSD_WR_MPEG_REG
			(VPP1_BLD_DIN1_HSCOPE,
			 (osd_hw.free_dst_data[index].x_start << 16) |
			 osd_hw.free_dst_data[index].x_end);

		VSYNCOSD_WR_MPEG_REG
			(VPP1_BLD_DIN1_VSCOPE,
			 (osd_hw.free_dst_data[index].y_start << 16) |
			 osd_hw.free_dst_data[index].y_end);

		VSYNCOSD_WR_MPEG_REG(VPP1_BLD_OUT_SIZE, dst_h << 16 | dst_w);

		osd_logd2("vpp1 bld scop(%d %d %d %d)\n",
			osd_hw.free_dst_data[index].x_start,
			osd_hw.free_dst_data[index].x_end,
			osd_hw.free_dst_data[index].y_start,
			osd_hw.free_dst_data[index].y_end);
	} else if (osd_index == VIU3_OSD1) {
		/* hdr in size */
		VSYNCOSD_WR_MPEG_REG(OSD4_HDR_IN_SIZE, dst_h << 16 | dst_w);
		/* vpp_top1 scope */
		VSYNCOSD_WR_MPEG_REG
			(VPP2_BLD_DIN1_HSCOPE,
			 (osd_hw.free_dst_data[index].x_start << 16) |
			 osd_hw.free_dst_data[index].x_end);

		VSYNCOSD_WR_MPEG_REG
			(VPP2_BLD_DIN1_VSCOPE,
			 (osd_hw.free_dst_data[index].y_start << 16) |
			 osd_hw.free_dst_data[index].y_end);

		VSYNCOSD_WR_MPEG_REG(VPP2_BLD_OUT_SIZE, dst_h << 16 | dst_w);

		osd_logd2("vpp2 bld scop(%d %d %d %d)\n",
			osd_hw.free_dst_data[index].x_start,
			osd_hw.free_dst_data[index].x_end,
			osd_hw.free_dst_data[index].y_start,
			osd_hw.free_dst_data[index].y_end);
	}
	remove_from_update_list(index, DISP_FREESCALE_ENABLE);
}
#endif

static void osd_update_coef(u32 index)
{
	int i;
	bool need_update_coef = false;
	int hf_coef_wren = 1;
	int vf_coef_wren = 1;
	unsigned int *hf_coef, *vf_coef;

	if (osd_hw.scale_workaround) {
		if (use_v_filter_mode != 3) {
			use_v_filter_mode = 3;
			need_update_coef = true;
		} else
			need_update_coef = false;
	} else {
		if (use_v_filter_mode != osd_v_filter_mode) {
			use_v_filter_mode = osd_v_filter_mode;
			need_update_coef = true;
		} else
			need_update_coef = false;
	}
	if (need_update_coef) {
		vf_coef = filter_table[use_v_filter_mode];
		if (vf_coef_wren) {
			osd_reg_set_bits(hw_osd_reg_array[index].osd_scale_coef_idx,
				0x0000, 0, 9);
			for (i = 0; i < 33; i++)
				osd_reg_write(hw_osd_reg_array[index].osd_scale_coef, vf_coef[i]);
		}
	}
	need_update_coef = false;
	if (use_h_filter_mode != osd_h_filter_mode) {
		use_h_filter_mode = osd_h_filter_mode;
		need_update_coef = true;
	}
	hf_coef = filter_table[use_h_filter_mode];
	if (need_update_coef) {
		if (hf_coef_wren) {
			osd_reg_set_bits(hw_osd_reg_array[index].osd_scale_coef_idx,
				0x0100, 0, 9);
			for (i = 0; i < 33; i++)
				osd_reg_write(hw_osd_reg_array[index].osd_scale_coef, hf_coef[i]);
		}
	}
}

static void osd1_update_coef(void)
{
	osd_update_coef(OSD1);
	remove_from_update_list(OSD1, OSD_FREESCALE_COEF);
}

static void osd2_update_coef(void)
{
	osd_update_coef(OSD1);
	remove_from_update_list(OSD2, OSD_FREESCALE_COEF);
}

#ifdef AML_T7_DISPLAY
static void osdx_update_coef(u32 index)
{
	int i;
	bool need_update_coef = false;
	int hf_coef_wren = 1;
	int vf_coef_wren = 1;
	unsigned int *hf_coef, *vf_coef;
	int osd_index = -1;
	u32 osd_scale_coef_idx = 0;
	u32 osd_scale_coef = 0;
	static int osd34_use_v_filter_mode[2] = {-1, -1};
	static int osd34_use_h_filter_mode[2] = {-1, -1};
	int *use_v_filter_mode;
	int *use_h_filter_mode;

	osd_index = get_osd_layer();
	if (osd_index < 0) {
		osd_loge("%s: invalid osd_index\n", __func__);
		return;
	}

	osd_logd2("%s, index:%d\n", __func__, osd_index);

	if (osd_index < VIU2_OSD1) {
		use_v_filter_mode = &osd34_use_v_filter_mode[0];
		use_h_filter_mode = &osd34_use_h_filter_mode[0];
		osd_scale_coef_idx = hw_osd_reg_array[0].osd_scale_coef_idx;
		osd_scale_coef = hw_osd_reg_array[0].osd_scale_coef;
	} else {
		if (osd_index == VIU2_OSD1) { /* VIU2_OSD1 */
			use_v_filter_mode = &osd34_use_v_filter_mode[0];
			use_h_filter_mode = &osd34_use_h_filter_mode[0];
		} else {                      /* VIU3_OSD1 */
			use_v_filter_mode = &osd34_use_v_filter_mode[1];
			use_h_filter_mode = &osd34_use_h_filter_mode[1];
		}
		osd_scale_coef_idx = hw_osd_reg_array[index].osd_scale_coef_idx;
		osd_scale_coef = hw_osd_reg_array[index].osd_scale_coef;
	}

	if (osd_hw.scale_workaround) {
		if (*use_v_filter_mode != 3) {
			*use_v_filter_mode = 3;
			need_update_coef = true;
		} else {
			need_update_coef = false;
		}
	} else {
		if (*use_v_filter_mode != osd_v_filter_mode) {
			*use_v_filter_mode = osd_v_filter_mode;
			need_update_coef = true;
		} else {
			need_update_coef = false;
		}
	}
	vf_coef = filter_table[*use_v_filter_mode];
	if (need_update_coef) {
		if (vf_coef_wren) {
			osd_reg_set_bits(osd_scale_coef_idx, 0x0000, 0, 9);
			for (i = 0; i < 33; i++)
				osd_reg_write(osd_scale_coef, vf_coef[i]);
		}
	}
	need_update_coef = false;
	if (*use_h_filter_mode != osd_h_filter_mode) {
		*use_h_filter_mode = osd_h_filter_mode;
		need_update_coef = true;
	}
	hf_coef = filter_table[*use_h_filter_mode];
	if (need_update_coef) {
		if (hf_coef_wren) {
			osd_reg_set_bits(osd_scale_coef_idx, 0x0100, 0, 9);
			for (i = 0; i < 33; i++)
				osd_reg_write(osd_scale_coef, hf_coef[i]);
		}
	}
	remove_from_update_list(index, OSD_FREESCALE_COEF);
}

static void viu2_osd1_update_disp_freescale_enable(void)
{
	osdx_update_disp_freescale_enable(VIU2_OSD1);
}

static void viu3_osd1_update_disp_freescale_enable(void)
{
	osdx_update_disp_freescale_enable(VIU3_OSD1);
}

static void viu2_osd1_update_coef(void)
{
	osdx_update_coef(VIU2_OSD1);
}

static void viu3_osd1_update_coef(void)
{
	osdx_update_coef(VIU3_OSD1);
}
#else
static void viu2_osd1_update_disp_freescale_enable(void)
{
	/* do nothing */
	remove_from_update_list(VIU2_OSD1, DISP_FREESCALE_ENABLE);
}

static void viu3_osd1_update_disp_freescale_enable(void)
{
	/* do nothing */
	remove_from_update_list(VIU3_OSD1, DISP_FREESCALE_ENABLE);
}

static void viu2_osd1_update_coef(void)
{
	osd_update_coef(OSD1);
	remove_from_update_list(VIU2_OSD1, OSD_FREESCALE_COEF);
}

static void viu3_osd1_update_coef(void)
{
	/* do nothing */
	remove_from_update_list(VIU3_OSD1, OSD_FREESCALE_COEF);
}
#endif

static void osd_update_color_mode(u32 index)
{
	u32 data32 = 0;

	if (osd_hw.color_info[index]) {
		data32 = (osd_hw.scan_mode == SCAN_MODE_INTERLACE) ? 2 : 0;
		data32 |= VSYNCOSD_RD_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w0)
			  & 0x30007040;
		data32 |= osd_hw.fb_gem[index].canvas_idx << 16;
		if (!osd_hw.rotate[index].on_off)
			data32 |= OSD_DATA_LITTLE_ENDIAN << 15;
		data32 |= osd_hw.color_info[index]->hw_colormat << 2;
		if (osd_get_chip_type() < MESON_CPU_MAJOR_ID_GXTVBB) {
			if (osd_hw.color_info[index]->color_index
				< COLOR_INDEX_YUV_422)
				data32 |= 1 << 7; /* yuv enable */
		}
		/* osd_blk_mode */
		data32 |=  osd_hw.color_info[index]->hw_blkmode << 8;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w0, data32);
	}
	remove_from_update_list(index, OSD_COLOR_MODE);
}

static void osd1_update_color_mode(void)
{
	osd_update_color_mode(OSD1);
}

static void osd2_update_color_mode(void)
{
	osd_update_color_mode(OSD2);
}

static void viu2_osd1_update_color_mode(void)
{
	osd_update_color_mode(VIU2_OSD1);
}

static void viu3_osd1_update_color_mode(void)
{
	osd_update_color_mode(VIU3_OSD1);
}

static void osd1_update_enable(void)
{
	u32 video_enable = 0;

	if (osd_hw.free_scale_mode[OSD1]) {
		if (osd_hw.enable[OSD1] == ENABLE) {
			if (osd_hw.osd_ver <= OSD_NORMAL)
			VSYNCOSD_SET_MPEG_REG_MASK(VPP_MISC,
						   VPP_OSD1_POSTBLEND | VPP_POSTBLEND_EN);
			VSYNCOSD_SET_MPEG_REG_MASK
				(hw_osd_reg_array[0].osd_ctrl_stat, 1 << 0);
		} else {
			VSYNCOSD_CLR_MPEG_REG_MASK
				(hw_osd_reg_array[0].osd_ctrl_stat, 1 << 0);
			if (osd_hw.osd_ver <= OSD_NORMAL)
			VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
						   VPP_OSD1_POSTBLEND);
		}
	} else if (osd_hw.osd_ver <= OSD_NORMAL) {
		video_enable |= VSYNCOSD_RD_MPEG_REG(VPP_MISC)&VPP_VD1_PREBLEND;
		if (osd_hw.enable[OSD1] == ENABLE) {
			if (osd_hw.free_scale_enable[OSD1]) {
				VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD1_POSTBLEND);
				VSYNCOSD_SET_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD1_PREBLEND
							   | VPP_VD1_POSTBLEND
							   | VPP_PREBLEND_EN);
			} else {
				VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD1_PREBLEND);
				if (!video_enable)
					VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
								   VPP_VD1_POSTBLEND);
				VSYNCOSD_SET_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD1_POSTBLEND);
			}
		} else {
			if (osd_hw.free_scale_enable[OSD1])
				VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD1_PREBLEND);
			else
				VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD1_POSTBLEND);
		}
	}
	remove_from_update_list(OSD1, OSD_ENABLE);
}

static void osd2_update_enable(void)
{
	u32 video_enable = 0;

	if (osd_hw.free_scale_mode[OSD2]) {
		if (osd_hw.enable[OSD2] == ENABLE)
			VSYNCOSD_SET_MPEG_REG_MASK(hw_osd_reg_array[1].osd_ctrl_stat,
						   1 << 0);
		else
			VSYNCOSD_CLR_MPEG_REG_MASK(hw_osd_reg_array[1].osd_ctrl_stat,
						   1 << 0);

		/* for older chips than g12a:
		 * freescale output always on VPP_OSD1_POSTBLEND
		 * if freescale is enable, VPP_OSD1_POSTBLEND to control OSD1&OSD2
		 * if freescale is disable, VPP_OSD2_POSTBLEND to control OSD2
		 */
		if (osd_hw.osd_ver <= OSD_NORMAL) {
			if (osd_hw.free_scale_enable[OSD2]) {
				if (osd_hw.enable[OSD2] == ENABLE)
						VSYNCOSD_SET_MPEG_REG_MASK(VPP_MISC,
									   VPP_OSD1_POSTBLEND
									   | VPP_POSTBLEND_EN);
				else
					if (!osd_hw.enable[OSD1])
						VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
									   VPP_OSD1_POSTBLEND);
			} else {
				if (osd_hw.enable[OSD2] == ENABLE)
						VSYNCOSD_SET_MPEG_REG_MASK(VPP_MISC,
									   VPP_OSD2_POSTBLEND
									   | VPP_POSTBLEND_EN);
				else
						VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
									   VPP_OSD2_POSTBLEND);
			}
		}
	} else if (osd_hw.osd_ver <= OSD_NORMAL){
		video_enable |= VSYNCOSD_RD_MPEG_REG(VPP_MISC)&VPP_VD1_PREBLEND;
		if (osd_hw.enable[OSD2] == ENABLE) {
			if (osd_hw.free_scale_enable[OSD2]) {
				VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD2_POSTBLEND);
				VSYNCOSD_SET_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD2_PREBLEND
							   | VPP_VD1_POSTBLEND);
			} else {
				VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD2_PREBLEND);
				if (!video_enable)
					VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
								   VPP_VD1_POSTBLEND);
				VSYNCOSD_SET_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD2_POSTBLEND);
			}
		} else {
			if (osd_hw.free_scale_enable[OSD2])
				VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD2_PREBLEND);
			else
				VSYNCOSD_CLR_MPEG_REG_MASK(VPP_MISC,
							   VPP_OSD2_POSTBLEND);
		}
	}
	remove_from_update_list(OSD2, OSD_ENABLE);
}

static void osd_update_enable(u32 index)
{
	if (osd_hw.free_scale_mode[index]) {
		if (osd_hw.enable[index] == ENABLE)
			VSYNCOSD_SET_MPEG_REG_MASK
				(hw_osd_reg_array[index].osd_ctrl_stat,
				1 << 0);
		else
			VSYNCOSD_CLR_MPEG_REG_MASK
				(hw_osd_reg_array[index].osd_ctrl_stat,
				1 << 0);
	}
	remove_from_update_list(index, OSD_ENABLE);
}

static void viu2_osd1_update_enable(void)
{
	osd_update_enable(VIU2_OSD1);
}

static void viu3_osd1_update_enable(void)
{
	osd_update_enable(VIU3_OSD1);
}

static void osd_update_disp_osd_reverse(u32 index)
{
	u32 val = 0;

	switch (osd_hw.osd_reverse[index]) {
	case REVERSE_XY:
		val = 3;
		break;
	case REVERSE_X:
		val = 1;
		break;
	case REVERSE_Y:
		val = 2;
		break;
	default:
		val = REVERSE_NONE;
		break;
	}

	VSYNCOSD_WR_MPEG_REG_BITS(hw_osd_reg_array[index].osd_blk0_cfg_w0,
		val, 28, 2);
	remove_from_update_list(index, DISP_OSD_REVERSE);
}

static void osd1_update_disp_osd_reverse(void)
{
	osd_update_disp_osd_reverse(OSD1);
}

static void osd2_update_disp_osd_reverse(void)
{
	osd_update_disp_osd_reverse(OSD2);
}

static void viu2_osd1_update_disp_osd_reverse(void)
{
	osd_update_disp_osd_reverse(VIU2_OSD1);
}

static void viu3_osd1_update_disp_osd_reverse(void)
{
	osd_update_disp_osd_reverse(VIU3_OSD1);
}

static void osd1_update_disp_osd_rotate(void)
{
	remove_from_update_list(OSD1, DISP_OSD_ROTATE);
}

static void osd2_update_disp_osd_rotate(void)
{
	remove_from_update_list(OSD2, DISP_OSD_ROTATE);
}

static void viu2_osd1_update_disp_osd_rotate(void)
{
	/* do nothing*/

	remove_from_update_list(VIU2_OSD1, DISP_OSD_ROTATE);
}

static void viu3_osd1_update_disp_osd_rotate(void)
{
	/* do nothing*/

	remove_from_update_list(VIU3_OSD1, DISP_OSD_ROTATE);
}

static void osd_update_color_key(u32 index)
{
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_tcolor_ag0,
		osd_hw.color_key[index]);
	remove_from_update_list(index, OSD_COLOR_KEY);
}

static void osd1_update_color_key(void)
{
	osd_update_color_key(OSD1);
}

static void osd2_update_color_key(void)
{
	osd_update_color_key(OSD2);
}

static void viu2_osd1_update_color_key(void)
{
	osd_update_color_key(VIU2_OSD1);
}

static void viu3_osd1_update_color_key(void)
{
	osd_update_color_key(VIU3_OSD1);
}

static void osd_update_color_key_enable(u32 index)
{
	u32  data32;

	data32 = VSYNCOSD_RD_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w0);
	data32 &= ~(1 << 6);
	data32 |= (osd_hw.color_key_enable[index] << 6);
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w0, data32);
	remove_from_update_list(index, OSD_COLOR_KEY_ENABLE);
}

static void osd1_update_color_key_enable(void)
{
	osd_update_color_key_enable(OSD1);
}

static void osd2_update_color_key_enable(void)
{
	osd_update_color_key_enable(OSD2);
}

static void viu2_osd1_update_color_key_enable(void)
{
	osd_update_color_key_enable(VIU2_OSD1);
}

static void viu3_osd1_update_color_key_enable(void)
{
	osd_update_color_key_enable(VIU3_OSD1);
}

static void osd_update_gbl_alpha(u32 index)
{
	u32  data32;

	data32 = VSYNCOSD_RD_MPEG_REG(hw_osd_reg_array[index].osd_ctrl_stat);
	data32 &= ~(0x1ff << 12);
	data32 |= osd_hw.gbl_alpha[index] << 12;
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_ctrl_stat, data32);
	remove_from_update_list(index, OSD_GBL_ALPHA);
}

static void osd1_update_gbl_alpha(void)
{
	osd_update_gbl_alpha(OSD1);
}

static void osd2_update_gbl_alpha(void)
{
	osd_update_gbl_alpha(OSD2);
}

static void viu2_osd1_update_gbl_alpha(void)
{
	osd_update_gbl_alpha(VIU2_OSD1);
}

static void viu3_osd1_update_gbl_alpha(void)
{
	osd_update_gbl_alpha(VIU3_OSD1);
}

static void osd2_update_order(void)
{
	switch (osd_hw.order) {
	case  OSD_ORDER_01:
		osd_reg_clr_mask(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);
		break;
	case  OSD_ORDER_10:
		osd_reg_set_mask(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);
		break;
	default:
		break;
	}
	remove_from_update_list(OSD2, OSD_CHANGE_ORDER);
}

static void osd1_update_order(void)
{
	switch (osd_hw.order) {
	case  OSD_ORDER_01:
		osd_reg_clr_mask(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);
		break;
	case  OSD_ORDER_10:
		osd_reg_set_mask(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);
		break;
	default:
		break;
	}
	remove_from_update_list(OSD1, OSD_CHANGE_ORDER);
}

static void viu2_osd1_update_order(void)
{
	/* do nothing */

	remove_from_update_list(VIU2_OSD1, OSD_CHANGE_ORDER);
}

static void viu3_osd1_update_order(void)
{
	/* do nothing */

	remove_from_update_list(VIU3_OSD1, OSD_CHANGE_ORDER);
}

static void osd1_2x_scale_update_geometry(void)
{
	u32 data32;

	data32 = (osd_hw.scaledata[OSD1].x_start & 0x1fff) |
		 (osd_hw.scaledata[OSD1].x_end & 0x1fff) << 16;
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w1, data32);
	data32 = ((osd_hw.scaledata[OSD1].y_start
		   + osd_hw.pandata[OSD1].y_start) & 0x1fff)
		 | ((osd_hw.scaledata[OSD1].y_end
		     + osd_hw.pandata[OSD1].y_start) & 0x1fff) << 16;
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w2, data32);
	/* adjust display x-axis */
	if (osd_hw.scale[OSD1].h_enable) {
		data32 = (osd_hw.dispdata[OSD1].x_start & 0xfff)
			 | ((osd_hw.dispdata[OSD1].x_start
			     + (osd_hw.scaledata[OSD1].x_end
				- osd_hw.scaledata[OSD1].x_start) * 2 + 1)
			    & 0xfff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w3, data32);
		if (osd_hw.scan_mode == SCAN_MODE_INTERLACE) {
			data32 = ((osd_hw.dispdata[OSD1].y_start >> 1) & 0xfff)
				 | (((((osd_hw.dispdata[OSD1].y_start
					+ (osd_hw.scaledata[OSD1].y_end
					   - osd_hw.scaledata[OSD1].y_start) * 2)
				       + 1) >> 1) - 1) & 0xfff) << 16;
		} else {
			data32 = (osd_hw.dispdata[OSD1].y_start & 0xfff)
				 | (((osd_hw.dispdata[OSD1].y_start
				      + (osd_hw.scaledata[OSD1].y_end
					 - osd_hw.scaledata[OSD1].y_start) * 2))
				    & 0xfff) << 16;
		}
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w4, data32);
	}
	/* adjust display y-axis */
	if (osd_hw.scale[OSD1].v_enable) {
		data32 = (osd_hw.dispdata[OSD1].x_start & 0xfff)
			 | ((osd_hw.dispdata[OSD1].x_start
			     + (osd_hw.scaledata[OSD1].x_end
				- osd_hw.scaledata[OSD1].x_start) * 2 + 1)
			    & 0xfff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w3, data32);
		if (osd_hw.scan_mode == SCAN_MODE_INTERLACE) {
			data32 = ((osd_hw.dispdata[OSD1].y_start >> 1) & 0xfff)
				 | (((((osd_hw.dispdata[OSD1].y_start
					+ (osd_hw.scaledata[OSD1].y_end
					   - osd_hw.scaledata[OSD1].y_start) * 2)
				       + 1) >> 1) - 1) & 0xfff) << 16;
		} else {
			data32 = (osd_hw.dispdata[OSD1].y_start & 0xfff)
				 | (((osd_hw.dispdata[OSD1].y_start
				      + (osd_hw.scaledata[OSD1].y_end
					 - osd_hw.scaledata[OSD1].y_start) * 2))
				    & 0xfff) << 16;
		}
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w4, data32);
	}
}

static void osd1_basic_update_disp_geometry(void)
{
	u32 data32;

	if (osd_get_chip_type() >= MESON_CPU_MAJOR_ID_M8) {
		data32 = (osd_hw.dispdata[OSD1].x_start & 0xfff)
			 | (osd_hw.dispdata[OSD1].x_end & 0xfff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w3, data32);
		if (osd_hw.scan_mode == SCAN_MODE_INTERLACE)
			data32 = ((osd_hw.dispdata[OSD1].y_start >> 1) & 0xfff)
				 | ((((osd_hw.dispdata[OSD1].y_end + 1)
				      >> 1) - 1) & 0xfff) << 16;
		else
			data32 = (osd_hw.dispdata[OSD1].y_start & 0xfff)
				 | (osd_hw.dispdata[OSD1].y_end
				    & 0xfff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w4, data32);
	} else {
		if (osd_hw.free_scale_mode[OSD1] == 0) {
			if (osd_hw.free_scale_enable[OSD1] == 1) {
				data32 = (osd_hw.free_scale_data[OSD1].x_start
					  & 0xfff)
					 | (osd_hw.free_scale_data[OSD1].x_end
					    & 0xfff) << 16;
				VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w3,
						     data32);
				data32 = (osd_hw.free_scale_data[OSD1].y_start
					  & 0xfff)
					 | (osd_hw.free_scale_data[OSD1].y_end
					    & 0xfff) << 16;
				VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w4,
						     data32);
			} else {
				data32 = (osd_hw.dispdata[OSD1].x_start
					  & 0xfff)
					 | (osd_hw.dispdata[OSD1].x_end
					    & 0xfff) << 16;
				VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w3,
						     data32);
				if (osd_hw.scan_mode == SCAN_MODE_INTERLACE)
					data32 = ((osd_hw.dispdata[OSD1].y_start
						   >> 1) & 0xfff)
						 | ((((osd_hw.dispdata[OSD1].y_end
						       + 1) >> 1) - 1)
						    & 0xfff) << 16;
				else
					data32 = (osd_hw.dispdata[OSD1].y_start
						  & 0xfff)
						 | (osd_hw.dispdata[OSD1].y_end
						    & 0xfff) << 16;
				VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w4,
						     data32);
			}
		} else {
			data32 = (osd_hw.dispdata[OSD1].x_start & 0xfff)
				 | (osd_hw.dispdata[OSD1].x_end & 0xfff) << 16;
			VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w3, data32);
			if (osd_hw.scan_mode == SCAN_MODE_INTERLACE)
				data32 = ((osd_hw.dispdata[OSD1].y_start >> 1)
					  & 0xfff)
					 | ((((osd_hw.dispdata[OSD1].y_end + 1)
					      >> 1) - 1) & 0xfff) << 16;
			else
				data32 = (osd_hw.dispdata[OSD1].y_start & 0xfff)
					 | (osd_hw.dispdata[OSD1].y_end
					    & 0xfff) << 16;
			VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w4, data32);
		}
	}
	/* enable osd 2x scale */
	if (osd_hw.scale[OSD1].h_enable || osd_hw.scale[OSD1].v_enable)
		osd1_2x_scale_update_geometry();
	else if (osd_hw.free_scale_enable[OSD1]
		 && (osd_hw.free_scale_data[OSD1].x_end > 0)
		 && (osd_hw.free_scale_data[OSD1].y_end > 0)
		 && (!osd_hw.rotate[OSD1].on_off)) {
		/* enable osd free scale */
		data32 = (osd_hw.free_scale_data[OSD1].x_start & 0x1fff) |
			 (osd_hw.free_scale_data[OSD1].x_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w1, data32);
		data32 = ((osd_hw.free_scale_data[OSD1].y_start
			   + osd_hw.pandata[OSD1].y_start) & 0x1fff)
			 | ((osd_hw.free_scale_data[OSD1].y_end
			     + osd_hw.pandata[OSD1].y_start) & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w2, data32);
	} else if (osd_hw.free_scale_enable[OSD1]
		   && (osd_hw.free_scale_data[OSD1].x_end > 0)
		   && (osd_hw.free_scale_data[OSD1].y_end > 0)
		   && (osd_hw.rotate[OSD1].on_off
		       && osd_hw.rotate[OSD1].angle > 0)) {
		data32 = (osd_hw.rotation_pandata[OSD1].x_start & 0x1fff) |
			 (osd_hw.rotation_pandata[OSD1].x_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w1, data32);
		data32 = ((osd_hw.rotation_pandata[OSD1].y_start
			   + osd_hw.pandata[OSD1].y_start) & 0x1fff)
			 | ((osd_hw.rotation_pandata[OSD1].y_end
			     + osd_hw.pandata[OSD1].y_start) & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w2, data32);
#ifndef AML_OSD_HIGH_VERSION
		if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_M8)
			VSYNCOSD_WR_MPEG_REG(VPU_PROT1_Y_START_END, data32);
#endif
	} else if (osd_hw.rotate[OSD1].on_off
		   && osd_hw.rotate[OSD1].angle > 0) {
		/* enable osd rotation */
		data32 = (osd_hw.rotation_pandata[OSD1].x_start & 0x1fff) |
			 (osd_hw.rotation_pandata[OSD1].x_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w1, data32);
		data32 = ((osd_hw.rotation_pandata[OSD1].y_start
			   + osd_hw.pandata[OSD1].y_start) & 0x1fff)
			 | ((osd_hw.rotation_pandata[OSD1].y_end
			     + osd_hw.pandata[OSD1].y_start) & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w2, data32);
#ifndef AML_OSD_HIGH_VERSION
		if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_M8)
			VSYNCOSD_WR_MPEG_REG(VPU_PROT1_Y_START_END, data32);
#endif
	} else {
		/* normal mode */
		data32 = (osd_hw.pandata[OSD1].x_start & 0x1fff)
			 | (osd_hw.pandata[OSD1].x_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w1, data32);
		data32 = (osd_hw.pandata[OSD1].y_start & 0x1fff)
			 | (osd_hw.pandata[OSD1].y_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[0].osd_blk0_cfg_w2, data32);
	}
}

static void osd1_update_disp_geometry(void)
{
	osd1_basic_update_disp_geometry();
	remove_from_update_list(OSD1, DISP_GEOMETRY);
}

static void osd_update_disp_geometry(u32 index)
{
	u32 data32;

	data32 = (osd_hw.dispdata[index].x_start & 0xfff)
		 | (osd_hw.dispdata[index].x_end & 0xfff) << 16;
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w3, data32);
	if (osd_hw.scan_mode == SCAN_MODE_INTERLACE)
		data32 = ((osd_hw.dispdata[index].y_start >> 1) & 0xfff)
			 | ((((osd_hw.dispdata[index].y_end + 1) >> 1) - 1)
			    & 0xfff) << 16;
	else
		data32 = (osd_hw.dispdata[index].y_start & 0xfff)
			 | (osd_hw.dispdata[index].y_end & 0xfff) << 16;
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w4, data32);
	if (osd_hw.scale[index].h_enable || osd_hw.scale[index].v_enable) {
#if defined(CONFIG_FB_OSD2_CURSOR)
		data32 = (osd_hw.pandata[index].x_start & 0x1fff)
			 | (osd_hw.pandata[index].x_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w1, data32);
		data32 = (osd_hw.pandata[index].y_start & 0x1fff)
			 | (osd_hw.pandata[index].y_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w2, data32);
#else
		data32 = (osd_hw.scaledata[index].x_start & 0x1fff) |
			 (osd_hw.scaledata[index].x_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w1, data32);
		data32 = ((osd_hw.scaledata[index].y_start
			   + osd_hw.pandata[index].y_start) & 0x1fff)
			 | ((osd_hw.scaledata[index].y_end
			     + osd_hw.pandata[index].y_start) & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w2, data32);
#endif
	} else if (osd_hw.free_scale_enable[index] &&
			(osd_hw.free_scale_data[index].x_end > 0) &&
			(osd_hw.free_scale_data[index].y_end > 0)) {
		/* enable osd free scale */
		data32 = (osd_hw.free_scale_data[index].x_start & 0x1fff)
			 | (osd_hw.free_scale_data[index].x_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w1, data32);
		data32 = ((osd_hw.free_scale_data[index].y_start
			   + osd_hw.pandata[index].y_start) & 0x1fff)
			 | ((osd_hw.free_scale_data[index].y_end
			     + osd_hw.pandata[index].y_start) & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w2, data32);
	} else {
		data32 = (osd_hw.pandata[index].x_start & 0x1fff)
			 | (osd_hw.pandata[index].x_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w1, data32);
		data32 = (osd_hw.pandata[index].y_start & 0x1fff)
			 | (osd_hw.pandata[index].y_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w2, data32);
	}
	remove_from_update_list(index, DISP_GEOMETRY);
}

static void osd2_update_disp_geometry(void)
{
	osd_update_disp_geometry(OSD2);
}

#ifdef AML_T7_DISPLAY
static void viu2_osd1_update_disp_geometry(void)
{
	osd_update_disp_geometry(VIU2_OSD1);
}

static void viu3_osd1_update_disp_geometry(void)
{
	osd_update_disp_geometry(VIU3_OSD1);
}
#else
static void viu2_osd1_update_disp_geometry(void)
{
	u32 data32;

	data32 = (osd_hw.dispdata[VIU2_OSD1].x_start & 0xfff)
		 | (osd_hw.dispdata[VIU2_OSD1].x_end & 0xfff) << 16;
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[2].osd_blk0_cfg_w3, data32);
	if (osd_hw.scan_mode == SCAN_MODE_INTERLACE)
		data32 = ((osd_hw.dispdata[VIU2_OSD1].y_start >> 1) & 0xfff)
			 | ((((osd_hw.dispdata[VIU2_OSD1].y_end + 1) >> 1) - 1)
			    & 0xfff) << 16;
	else
		data32 = (osd_hw.dispdata[VIU2_OSD1].y_start & 0xfff)
			 | (osd_hw.dispdata[VIU2_OSD1].y_end & 0xfff) << 16;
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[2].osd_blk0_cfg_w4, data32);

	data32 = (osd_hw.pandata[VIU2_OSD1].x_start & 0x1fff)
		 | (osd_hw.pandata[VIU2_OSD1].x_end & 0x1fff) << 16;
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[2].osd_blk0_cfg_w1, data32);
	data32 = (osd_hw.pandata[VIU2_OSD1].y_start & 0x1fff)
		 | (osd_hw.pandata[VIU2_OSD1].y_end & 0x1fff) << 16;
	VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[2].osd_blk0_cfg_w2, data32);

	remove_from_update_list(VIU2_OSD1, DISP_GEOMETRY);
}

static void viu3_osd1_update_disp_geometry(void)
{
	remove_from_update_list(VIU3_OSD1, DISP_GEOMETRY);
}
#endif

static void osd_update_disp_3d_mode(u32 index)
{
	/*step 1 . set pan data */
	u32  data32;

	if (osd_hw.mode_3d[index].left_right == OSD_LEFT) {
		data32 = (osd_hw.mode_3d[index].l_start & 0x1fff)
			 | (osd_hw.mode_3d[index].l_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w1, data32);
	} else {
		data32 = (osd_hw.mode_3d[index].r_start & 0x1fff)
			 | (osd_hw.mode_3d[index].r_end & 0x1fff) << 16;
		VSYNCOSD_WR_MPEG_REG(hw_osd_reg_array[index].osd_blk0_cfg_w1, data32);
	}
	osd_hw.mode_3d[index].left_right ^= 1;
}

static void osd1_update_disp_3d_mode(void)
{
	osd_update_disp_3d_mode(OSD1);
}

static void osd2_update_disp_3d_mode(void)
{
	osd_update_disp_3d_mode(OSD2);
}

static void osd3_update_disp_3d_mode(void)
{
	osd_update_disp_3d_mode(OSD3);
}

static void viu2_osd1_update_disp_3d_mode(void)
{
	osd_update_disp_3d_mode(VIU2_OSD1);
}

void osd_hist_enable(u32 osd_index)
{
	if (osd_hw.osd_ver == OSD_HIGH_ONE) {
		VSYNCOSD_WR_MPEG_REG(VI_HIST_CTRL, 0x1801);
	} else {
		if (OSD1 == osd_index) {
			VSYNCOSD_WR_MPEG_REG(VI_HIST_CTRL, 0x1801);
		} else if (OSD2 == osd_index) {
			VSYNCOSD_WR_MPEG_REG(VI_HIST_CTRL, 0x2001);
		} else {
			osd_loge("osd%d is not supported.\n", osd_index);
		}
	}
	osd_logd("VI_HIST_CTRL = %x\n", VSYNCOSD_RD_MPEG_REG(VI_HIST_CTRL));
}

int osd_get_hist_stat(u32 *hist_result)
{
	hist_result[0] = VSYNCOSD_RD_MPEG_REG(VI_HIST_MAX_MIN);
	hist_result[1] = VSYNCOSD_RD_MPEG_REG(VI_HIST_SPL_VAL);
	hist_result[2] = VSYNCOSD_RD_MPEG_REG(VI_HIST_SPL_PIX_CNT);
	hist_result[3] = VSYNCOSD_RD_MPEG_REG(VI_HIST_CHROMA_SUM);

	osd_logd("osd hist stat result:0x%x, 0x%x, 0x%x, 0x%x\n",
		hist_result[0], hist_result[1],
		hist_result[2], hist_result[3]);
	return 0;
}

#ifdef VEHICLE_CONFIG
/*  UBOOT_INFO_FLAG bit definition
 *  |          31         |         30       |  29 ~ 16 |    15    |    14    |  13 ~ 0  |
 *  | uboot config finish | screen interlace | screen H | reserved | reserved | screen W |
 *
 * bit31: 0 - uboot config is finished
 *        1 - uboot config is not finished
 */
void transfer_info_to_rtos(void)
{
	struct vinfo_s *info = NULL;
	int screen_w, screen_h, interlace;
	u32 val;

#if defined CONFIG_AML_VOUT
	info = vout_get_current_vinfo();
#endif
	screen_w = info->width;
	screen_h = info->height;
	interlace = info->field_height == screen_h ? 0 : 1;

	val = (0 << 31) | (interlace << 30) | (screen_h << 16) | screen_w;
	VSYNCOSD_WR_MPEG_REG(UBOOT_INFO_FLAG, val);
}

bool is_osd2_configed(void)
{
	bool ret = false;

	if (VSYNCOSD_RD_MPEG_REG(hw_osd_reg_array[1].osd_ctrl_stat) & 0x1)
		ret = true;
	printf("%s ret=%d\n", __func__, ret);
	return ret;
}

static void osd_set_dimm(u32 index, u32 dim_color)
{
	u32 dimm_rgb = 0;

	dimm_rgb =
		((dim_color & 0xff000000)
		>> 24) << 22;
	dimm_rgb |=
		((dim_color & 0xff0000)
		>> 16) << 12;
	dimm_rgb |=
		((dim_color & 0xff00)
		>> 8) << 2;
	VSYNCOSD_WR_MPEG_REG(VIU_OSD2_DIMM_CTRL,
				 0x40000000 | dimm_rgb);
	VSYNCOSD_WR_MPEG_REG_BITS
		(hw_osd_reg_array[index].osd_ctrl_stat2, 0x1, 14, 1);
	VSYNCOSD_WR_MPEG_REG_BITS
		(hw_osd_reg_array[index].osd_ctrl_stat2,
		dim_color & 0xff, 6, 8);
}

static void osd2_config_blend(void)
{
	u32 data;
	u32 din_reoder_sel = 0, blend_din_en = 0;

	data = osd_reg_read(VIU_OSD_BLEND_CTRL);
	din_reoder_sel = data & 0xffff;
	blend_din_en = data & 0xf00000;
	din_reoder_sel &= ~0xf000;
	din_reoder_sel |= 2 << 12;
	blend_din_en &= ~0x400000;
	blend_din_en |= 1 << 23;
	data &= ~0xf0ffff;
	data |= din_reoder_sel | blend_din_en;

	osd_reg_write(VIU_OSD_BLEND_CTRL, data);

	osd_reg_write(VIU_OSD_BLEND_DIN1_SCOPE_H,
		osd_hw.free_dst_data[OSD1].x_end << 16 |
		osd_hw.free_dst_data[OSD1].x_start);
	osd_reg_write(VIU_OSD_BLEND_DIN1_SCOPE_V,
		(osd_hw.free_dst_data[OSD1].y_end) << 16 |
		(osd_hw.free_dst_data[OSD1].y_start));
}

void osd2_config_with_dimm(int *axis)
{
	osd2_layer_init();
	osd_set_free_scale_mode_hw(OSD2, 1);
	osd_set_free_scale_axis_hw(OSD2, 0, 0, fb_gdev.fb_width - 1,
				   fb_gdev.fb_height - 1);
	osd_set_window_axis_hw(OSD2, axis[0], axis[1], axis[0] + axis[2] - 1,
			       axis[1] + axis[3] - 1);
	osd_set_free_scale_enable_hw(OSD2, 0x10001);
	osd_enable_hw(OSD2, 1);
	osd_set_dimm(OSD2, 0x0000);
	osd2_config_blend();
}
#endif

#ifdef AML_T7_DISPLAY
void osd_init_hw_viux(u32 index)
{
	u32 group, idx, reverse_val = 0;
	char *osd_reverse;
	char *s;
	/* 1:vd1  2:osd1 else :close */
	u32 bld_src2_sel = 2;
	u32 osd_premult = 0;
	u32 blend_en = 1;
	u32 data32;
	u32 holdline = 8;

	/* init osd fifo control register
	 * set DDR request priority to be urgent
	 */
	if (index > VIU3_OSD1 || index < VIU2_OSD1)
		return;
	data32 = 1;

	/* hold_fifo_lines */
	if (index == VIU2_OSD1)
		s = env_get("viu2_hold_line");
	else if (index == VIU3_OSD1)
		s = env_get("viu3_hold_line");
	if (s) {
		holdline = simple_strtoul(s, NULL, 10);
		data32 |= holdline << 5;  /* hold_fifo_lines */
	} else {
		if (osd_hw.osd_ver == OSD_HIGH_ONE)
			data32 |= 8 << 5;  /* hold_fifo_lines */
		else
			data32 |= 4 << 5;
	}
	/* burst_len_sel: 3=64 */
	if (osd_hw.osd_ver == OSD_HIGH_ONE) {
		data32 |= 1 << 10;
		data32 |= 1 << 31;
	} else {
		data32 |= 3  << 10;
	}

	/*
	 * bit 23:22, fifo_ctrl
	 * 00 : for 1 word in 1 burst
	 * 01 : for 2 words in 1 burst
	 * 10 : for 4 words in 1 burst
	 * 11 : reserved
	 */
	data32 |= 2 << 22;
	/* bit 28:24, fifo_lim */
	data32 |= 2 << 24;

	/* fifo_depth_val: 32*8=256 */
	data32 |= 64 << 12;
	osd_reg_write(hw_osd_reg_array[index].osd_fifo_ctrl_stat, data32);

	/* just disable osd to avoid booting hang up */
	data32 = 0x0 << 0;
	data32 |= OSD_GLOBAL_ALPHA_DEF << 12;
	osd_reg_write(hw_osd_reg_array[index].osd_ctrl_stat, data32);

	/* set replaced_alpha */
	data32 = 0x1 << 14;
	data32 |= 0xff << 6;
	osd_reg_write(hw_osd_reg_array[index].osd_ctrl_stat2, data32);

	if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_T3)
		osd_hw.path_ctrl_independ = 1;

	if (index == VIU2_OSD1) {
		/* OSD3 -> VPP1 */
		if (osd_hw.path_ctrl_independ) {
			independ_path_default_regs();
			/* osd byp osd_blend */
			osd_reg_set_bits(VIU_OSD3_PATH_CTRL, 0x1, 4, 1);
		} else {
			fix_vpu_clk2_default_regs();
			/* osd byp osd_blend */
			osd_reg_set_bits(VPP_OSD3_SCALE_CTRL, 0x7, 0, 3);
		}

		/* vpp1 osd order, premult, blend_en */
		osd_reg_set_bits(VPP1_BLD_CTRL, bld_src2_sel, 4, 4);
		osd_reg_set_bits(VPP1_BLD_CTRL, osd_premult, 17, 1);
		osd_reg_set_bits(VPP1_BLD_CTRL, blend_en, 31, 1);

		/* vpp_top input mux */
		osd_reg_set_bits(OSD_PATH_MISC_CTRL, OSD3 + VPP_OSD1,
				 OSD3 * 4 + 16, 4);

		/* to vpp_top1 */
		osd_reg_set_bits(PATH_START_SEL, VPU_VPP1, 24, 2);
	}

	if (index == VIU3_OSD1) {
		/* OSD4 -> VPP2 */
		if (osd_hw.path_ctrl_independ) {
			independ_path_default_regs();
			/* osd byp osd_blend */
			osd_reg_set_bits(VIU_OSD4_PATH_CTRL, 0x1, 4, 1);
		} else {
			fix_vpu_clk2_default_regs();
			/* osd byp osd_blend */
			osd_reg_set_bits(VPP_OSD4_SCALE_CTRL, 0x7, 0, 3);
		}

		/* vpp1 osd order, premult, blend_en */
		osd_reg_set_bits(VPP2_BLD_CTRL, bld_src2_sel, 4, 4);
		osd_reg_set_bits(VPP2_BLD_CTRL, osd_premult, 17, 1);
		osd_reg_set_bits(VPP2_BLD_CTRL, blend_en, 31, 1);

		/* vpp_top input mux */
		osd_reg_set_bits(OSD_PATH_MISC_CTRL, OSD4 + VPP_OSD1,
				 OSD4 * 4 + 16, 4);

		/* to vpp_top2 */
		osd_reg_set_bits(PATH_START_SEL, VPU_VPP2, 28, 2);
	}

	/* init param */
	osd_reverse = env_get("osd_reverse");
	for (group = 0; group < HW_OSD_COUNT; group++)
		for (idx = 0; idx < HW_REG_INDEX_MAX; idx++)
			osd_hw.reg[group][idx].update_func =
				hw_func_array[group][idx];

	osd_hw.updated[index] = 0;
	osd_hw.enable[index] = DISABLE;

	osd_hw.fb_gem[index].canvas_idx = OSD4_CANVAS_INDEX;
	osd_hw.gbl_alpha[index] = OSD_GLOBAL_ALPHA_DEF;
	osd_hw.color_info[index] = NULL;
	osd_hw.color_key[index] = 0xffffffff;
	osd_hw.scale[index].h_enable = 0;
	osd_hw.scale[index].v_enable = 0;
	osd_hw.mode_3d[index].enable = 0;

	if (osd_reverse) {
		if (!strcmp(osd_reverse, "all,true"))
			reverse_val = REVERSE_XY;
		else if (!strcmp(osd_reverse, "all,x_rev"))
			reverse_val = REVERSE_X;
		else if (!strcmp(osd_reverse, "all,y_rev"))
			reverse_val = REVERSE_Y;
		else
			reverse_val = REVERSE_NONE;
	}
	osd_hw.osd_reverse[index] = reverse_val;

	osd_hw.rotation_pandata[index].x_start = 0;
	osd_hw.rotation_pandata[index].y_start = 0;

	if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_T7 ||
	    osd_get_chip_type() == MESON_CPU_MAJOR_ID_T3)
		osd_hw.mif_linear = 1;
}
#else
void osd_init_hw_viux(u32 index)
{
	u32 group, idx, data32, holdline = 4, reverse_val = 0;
	char *osd_reverse;
	char *s;

	if (index != VIU2_OSD1)
		return;

	osd_reverse = env_get("osd_reverse");
	s = env_get("viu2_hold_line");
	if (s)
		holdline = simple_strtoul(s, NULL, 10);
	for (group = 0; group < HW_OSD_COUNT; group++)
		for (idx = 0; idx < HW_REG_INDEX_MAX; idx++)
			osd_hw.reg[group][idx].update_func =
				hw_func_array[group][idx];

	osd_hw.updated[index] = 0;

	osd_vpu_power_on_viu2();

	osd_reg_write(VPP2_OFIFO_SIZE, 0x7ff00800);

	/* init osd fifo control register */
	/* set DDR request priority to be urgent */
	data32 = 1;
	/* hold_fifo_lines */
	data32 |= holdline << 5;  /* hold_fifo_lines */
	/* burst_len_sel: 3=64 */
	data32 |= 1 << 10;
	data32 |= 1 << 31;

	/*
	 * bit 23:22, fifo_ctrl
	 * 00 : for 1 word in 1 burst
	 * 01 : for 2 words in 1 burst
	 * 10 : for 4 words in 1 burst
	 * 11 : reserved
	 */
	data32 |= 2 << 22;
	/* bit 28:24, fifo_lim */
	data32 |= 2 << 24;
	/* fifo_depth_val: 32 or 64 *8 = 256 or 512 */
	data32 |= 64 << 12;

	osd_reg_write(hw_osd_reg_array[index].osd_fifo_ctrl_stat, data32);

	/* disable  osd */
	data32 = 0x0 << 0;
	data32 |= OSD_GLOBAL_ALPHA_DEF << 12;
	data32 |= 0x80000000;
	osd_reg_write(hw_osd_reg_array[index].osd_ctrl_stat, data32);

	/* set replaced_alpha */
	data32 = 0x1 << 14;
	data32 |= 0xff << 6;
	osd_reg_write(hw_osd_reg_array[index].osd_ctrl_stat2, data32);
	osd_hw.enable[index] = DISABLE;

	osd_hw.fb_gem[index].canvas_idx = OSD4_CANVAS_INDEX;
	osd_hw.gbl_alpha[index] = OSD_GLOBAL_ALPHA_DEF;
	osd_hw.color_info[index] = NULL;
	osd_hw.color_key[index] = 0xffffffff;
	osd_hw.scale[index].h_enable = 0;
	osd_hw.scale[index].v_enable = 0;
	osd_hw.mode_3d[index].enable = 0;

	if (osd_reverse) {
		if (!strcmp(osd_reverse, "all,true"))
			reverse_val = REVERSE_XY;
		else if (!strcmp(osd_reverse, "all,x_rev"))
			reverse_val = REVERSE_X;
		else if (!strcmp(osd_reverse, "all,y_rev"))
			reverse_val = REVERSE_Y;
		else
			reverse_val = REVERSE_NONE;
	}
	osd_hw.osd_reverse[index] = reverse_val;

	osd_hw.rotation_pandata[index].x_start = 0;
	osd_hw.rotation_pandata[index].y_start = 0;
}
#endif

static void set_vpp_super_position(void)
{
#define PREBLD_SR0_VD1_SCALER		(1 << 1)
#define DNLP_SR1_CM			        (1 << 3)

	if ((osd_get_chip_type() == MESON_CPU_MAJOR_ID_G12A) ||
		(osd_get_chip_type() == MESON_CPU_MAJOR_ID_G12B) ||
		 (osd_get_chip_type() == MESON_CPU_MAJOR_ID_SM1))
		osd_reg_set_mask(VPP_MISC, PREBLD_SR0_VD1_SCALER);
	else if ((osd_get_chip_type() == MESON_CPU_MAJOR_ID_TL1) ||
		(osd_get_chip_type() >= MESON_CPU_MAJOR_ID_TM2))
		osd_reg_set_mask(VPP_MISC, DNLP_SR1_CM);
}

static void fix_vpu_clk2_default_regs(void)
{
	static int init_done;

	if (init_done) {
		osd_logd("%s, has been initialized, skip.\n", __func__);
		return;
	}
	init_done = 1;
#ifdef AML_T7_DISPLAY
	if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_T7) {
		 /* default: osd byp osd_blend */
		osd_reg_set_bits(VPP_OSD1_SCALE_CTRL, 0x2, 0, 3);
		osd_reg_set_bits(VPP_OSD2_SCALE_CTRL, 0x3, 0, 3);
		osd_reg_set_bits(VPP_OSD3_SCALE_CTRL, 0x3, 0, 3);
		osd_reg_set_bits(VPP_OSD4_SCALE_CTRL, 0x3, 0, 3);

		/* default: osd byp dolby */
		osd_reg_set_bits(VPP_VD1_DSC_CTRL, 0x1, 4, 1);
		osd_reg_set_bits(VPP_VD2_DSC_CTRL, 0x1, 4, 1);
		osd_reg_set_bits(VPP_VD3_DSC_CTRL, 0x1, 4, 1);
#ifdef CONFIG_AML_DOLBY
		if (dolby_vision_on)
			osd_reg_set_bits(MALI_AFBCD_TOP_CTRL, 0x0, 14, 1);
		else
#endif
			osd_reg_set_bits(MALI_AFBCD_TOP_CTRL, 0x1, 14, 1);
		osd_reg_set_bits(MALI_AFBCD_TOP_CTRL, 0x1, 19, 1);
		osd_reg_set_bits(MALI_AFBCD1_TOP_CTRL, 0x1, 19, 1);
		osd_reg_set_bits(MALI_AFBCD1_TOP_CTRL, 0x1, 19, 1);

		/* default: osd 12bit path */
		osd_reg_set_bits(VPP_VD1_DSC_CTRL, 0x0, 5, 1);
		osd_reg_set_bits(VPP_VD2_DSC_CTRL, 0x0, 5, 1);
		osd_reg_set_bits(VPP_VD3_DSC_CTRL, 0x0, 5, 1);
		osd_reg_set_bits(MALI_AFBCD_TOP_CTRL, 0x0, 15, 1);
		osd_reg_set_bits(MALI_AFBCD_TOP_CTRL, 0x0, 20, 1);
		osd_reg_set_bits(MALI_AFBCD1_TOP_CTRL, 0x0, 20, 1);
		osd_reg_set_bits(MALI_AFBCD1_TOP_CTRL, 0x0, 20, 1);

		/* clean vpp_top 1/2 blend default order */
		osd_reg_set_bits(VPP1_BLD_CTRL, 0x0, 0, 8);
		osd_reg_set_bits(VPP2_BLD_CTRL, 0x0, 0, 8);

		/* set vpp_top 1/2 default blend dummy */
		osd_reg_write(VPP1_BLEND_BLEND_DUMMY_DATA, 0x008080);
		osd_reg_write(VPP1_BLEND_DUMMY_ALPHA, 0xffffffff);
		osd_reg_write(VPP2_BLEND_BLEND_DUMMY_DATA, 0x008080);
		osd_reg_write(VPP2_BLEND_DUMMY_ALPHA, 0xffffffff);

	}
#endif
}

static void independ_path_default_regs(void)
{
	static int init_done;

	if (init_done) {
		osd_logd("%s, has been initialized, skip.\n", __func__);
		return;
	}
	init_done = 1;
	/* default: osd1_bld_din_sel -- do not osd_data_byp osd_blend */
	osd_reg_set_bits(VIU_OSD1_PATH_CTRL, 0x0, 4, 1);
	osd_reg_set_bits(VIU_OSD2_PATH_CTRL, 0x0, 4, 1);
	osd_reg_set_bits(VIU_OSD3_PATH_CTRL, 0x0, 4, 1);

	/* default: osd1_sc_path_sel -- before osd_blend or after hdr */
	osd_reg_set_bits(VIU_OSD1_PATH_CTRL, 0x0, 0, 1);
	osd_reg_set_bits(VIU_OSD2_PATH_CTRL, 0x1, 0, 1);
	osd_reg_set_bits(VIU_OSD3_PATH_CTRL, 0x1, 0, 1);

	/* default: osd byp dolby */
	osd_reg_set_bits(VIU_VD1_PATH_CTRL, 0x1, 16, 1);
	osd_reg_set_bits(VIU_VD2_PATH_CTRL, 0x1, 16, 1);
	osd_reg_set_bits(VIU_OSD1_PATH_CTRL, 0x1, 16, 1);
	osd_reg_set_bits(VIU_OSD2_PATH_CTRL, 0x1, 16, 1);
	osd_reg_set_bits(VIU_OSD3_PATH_CTRL, 0x1, 16, 1);

	/* default: osd 12bit path */
	osd_reg_set_bits(VIU_VD1_PATH_CTRL, 0x0, 17, 1);
	osd_reg_set_bits(VIU_VD2_PATH_CTRL, 0x0, 17, 1);
	osd_reg_set_bits(VIU_OSD1_PATH_CTRL, 0x0, 17, 1);
	osd_reg_set_bits(VIU_OSD2_PATH_CTRL, 0x0, 17, 1);
	osd_reg_set_bits(VIU_OSD3_PATH_CTRL, 0x0, 17, 1);

	/* clean vpp_top 1/2 blend default order */
	osd_reg_set_bits(VPP1_BLD_CTRL, 0x0, 0, 8);
	osd_reg_set_bits(VPP2_BLD_CTRL, 0x0, 0, 8);

	/* set vpp_top 1/2 default blend dummy */
	osd_reg_write(VPP1_BLEND_BLEND_DUMMY_DATA, 0x008080);
	osd_reg_write(VPP1_BLEND_DUMMY_ALPHA, 0xffffffff);
	osd_reg_write(VPP2_BLEND_BLEND_DUMMY_DATA, 0x008080);
	osd_reg_write(VPP2_BLEND_DUMMY_ALPHA, 0xffffffff);
}

void osd_init_hw(void)
{
	u32 group, idx, data32, data2, holdline = 8, reverse_val = 0;
	char *osd_reverse;
	struct vinfo_s *info = NULL;
	char *s;

	osd_reverse = env_get("osd_reverse");
	for (group = 0; group < HW_OSD_COUNT; group++)
		for (idx = 0; idx < HW_REG_INDEX_MAX; idx++)
			osd_hw.reg[group][idx].update_func =
				hw_func_array[group][idx];
	osd_hw.updated[OSD1] = 0;
	osd_hw.updated[OSD2] = 0;
	osd_hw.updated[OSD3] = 0;
	osd_hw.updated[OSD4] = 0;

	/* set display output resolution */
	info = vout_get_current_vinfo();
	if (info != NULL)
		osd_reg_write(VPP_POSTBLEND_H_SIZE, info->width);
	osd_vpu_power_on();

	if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_T3)
		osd_hw.path_ctrl_independ = 1;

	/* here we will init default value ,these value only set once . */
	if (!logo_loaded) {
		if (osd_hw.path_ctrl_independ)
			independ_path_default_regs();
		else
			fix_vpu_clk2_default_regs();

		/* init vpu fifo control register */
		data32 = osd_reg_read(VPP_OFIFO_SIZE);
		osd_logi("VPP_OFIFO_SIZE:0x%x\n", data32);
		if (osd_hw.osd_ver == OSD_HIGH_ONE) {
			data32 &= ~((0xfff << 20) | 0x3fff);
			data32 |= (0xfff << 20);
			data32 |= (0xfff + 1);
			osd_reg_write(VPP_OFIFO_SIZE, data32);
		}

		/* init osd fifo control register */
		/* set DDR request priority to be urgent */
		data32 = 1;
		if ((osd_get_chip_type() == MESON_CPU_MAJOR_ID_M6TV)
		    || (osd_get_chip_type() == MESON_CPU_MAJOR_ID_MTVD)) {
			data32 |= 18 << 5;  /* hold_fifo_lines */
		} else if (osd_hw.osd_ver == OSD_SIMPLE) {
			data32 &= ~(0x1f << 5); /* bit[9:5] HOLD_FIFO_LINES */
			data32 |= 0x18 << 5;
		} else {
			s = env_get("viu1_hold_line");
			if (s) {
				holdline = simple_strtoul(s, NULL, 10);
				data32 |= holdline << 5;  /* hold_fifo_lines */
			} else {
				if (osd_hw.osd_ver == OSD_HIGH_ONE)
					data32 |= 8 << 5;  /* hold_fifo_lines */
				else
					data32 |= 4 << 5;  /* hold_fifo_lines */
			}
		}
		/* burst_len_sel: 3=64 */
		if (osd_hw.osd_ver == OSD_HIGH_ONE) {
			data32 |= 1 << 10;
			data32 |= 1 << 31;
		} else
			data32 |= 3  << 10;

		if (osd_get_chip_type() >= MESON_CPU_MAJOR_ID_GXBB) {
			/*
			 * bit 23:22, fifo_ctrl
			 * 00 : for 1 word in 1 burst
			 * 01 : for 2 words in 1 burst
			 * 10 : for 4 words in 1 burst
			 * 11 : reserved
			 */
			data32 |= 2 << 22;
			/* bit 28:24, fifo_lim */
			data32 |= 2 << 24;
		}
		data2 = data32;
		/* fifo_depth_val: 32*8=256 */
		if ((osd_get_chip_type() == MESON_CPU_MAJOR_ID_TXL)
			|| (osd_get_chip_type() == MESON_CPU_MAJOR_ID_TXLX))
			data32 |= 64 << 12;
		else
			data32 |= 32 << 12;
		data2 |= 32 << 12;
		osd_reg_write(hw_osd_reg_array[0].osd_fifo_ctrl_stat, data32);
		osd_reg_write(hw_osd_reg_array[1].osd_fifo_ctrl_stat, data2);
		osd_reg_set_mask(VPP_MISC, VPP_POSTBLEND_EN);
		osd_reg_clr_mask(VPP_MISC, VPP_PREBLEND_EN);
		if (osd_hw.osd_ver <= OSD_NORMAL)
			osd_reg_clr_mask(VPP_MISC,
				VPP_OSD1_POSTBLEND | VPP_OSD2_POSTBLEND | VPP_VD1_POSTBLEND);
		/* just disable osd to avoid booting hang up */
		data32 = 0x0 << 0;
		data32 |= OSD_GLOBAL_ALPHA_DEF << 12;
		osd_reg_write(hw_osd_reg_array[0].osd_ctrl_stat, data32);
		osd_reg_write(hw_osd_reg_array[1].osd_ctrl_stat, data32);

		/* set replaced_alpha */
		data32 = 0x1 << 14;
		data32 |= 0xff << 6;
		osd_reg_write(hw_osd_reg_array[0].osd_ctrl_stat2, data32);
		osd_reg_write(hw_osd_reg_array[1].osd_ctrl_stat2, data32);
		logo_loaded = 1;
	}
	if (osd_hw.osd_ver <= OSD_NORMAL)
		osd_reg_clr_mask(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);
	else if (osd_hw.osd_ver > OSD_NORMAL)
		set_vpp_super_position();

	if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_S4)
		osd_reg_clr_mask(VPP_MISC, VPP_WATER_MARK_10BIT);

	osd_hw.order = OSD_ORDER_01;
	osd_hw.enable[OSD1] = DISABLE;
	osd_hw.enable[OSD2] = DISABLE;
	osd_hw.enable[OSD3] = DISABLE;
	osd_hw.enable[OSD4] = DISABLE;

	osd_hw.fb_gem[OSD1].canvas_idx = OSD1_CANVAS_INDEX;
	osd_hw.fb_gem[OSD2].canvas_idx = OSD2_CANVAS_INDEX;
	osd_hw.fb_gem[OSD3].canvas_idx = OSD3_CANVAS_INDEX;
	osd_hw.fb_gem[OSD4].canvas_idx = OSD4_CANVAS_INDEX;

	osd_hw.gbl_alpha[OSD1] = OSD_GLOBAL_ALPHA_DEF;
	osd_hw.gbl_alpha[OSD2] = OSD_GLOBAL_ALPHA_DEF;
	osd_hw.gbl_alpha[OSD3] = OSD_GLOBAL_ALPHA_DEF;
	osd_hw.gbl_alpha[OSD4] = OSD_GLOBAL_ALPHA_DEF;

	osd_hw.color_info[OSD1] = NULL;
	osd_hw.color_info[OSD2] = NULL;
	osd_hw.color_info[OSD3] = NULL;
	osd_hw.color_info[OSD4] = NULL;

	osd_hw.color_key[OSD1] = 0xffffffff;
	osd_hw.color_key[OSD2] = 0xffffffff;
	osd_hw.color_key[OSD3] = 0xffffffff;
	osd_hw.color_key[OSD4] = 0xffffffff;

	osd_hw.free_scale_enable[OSD1] = 0;
	osd_hw.free_scale_enable[OSD2] = 0;
	osd_hw.free_scale_enable[OSD3] = 0;
	osd_hw.free_scale_enable[OSD4] = 0;

	osd_hw.scale[OSD1].h_enable = 0;
	osd_hw.scale[OSD1].v_enable = 0;
	osd_hw.scale[OSD2].h_enable = 0;
	osd_hw.scale[OSD2].v_enable = 0;
	osd_hw.scale[OSD3].h_enable = 0;
	osd_hw.scale[OSD3].v_enable = 0;
	osd_hw.scale[OSD4].h_enable = 0;
	osd_hw.scale[OSD4].v_enable = 0;

	osd_hw.mode_3d[OSD1].enable = 0;
	osd_hw.mode_3d[OSD2].enable = 0;
	osd_hw.mode_3d[OSD3].enable = 0;
	osd_hw.mode_3d[OSD4].enable = 0;

	osd_hw.block_mode[OSD1] = 0;
	osd_hw.block_mode[OSD2] = 0;
	osd_hw.block_mode[OSD3] = 0;
	osd_hw.block_mode[OSD4] = 0;

	osd_hw.free_scale[OSD1].h_enable = 0;
	osd_hw.free_scale[OSD1].v_enable = 0;
	osd_hw.free_scale[OSD2].h_enable = 0;
	osd_hw.free_scale[OSD2].v_enable = 0;
	osd_hw.free_scale[OSD3].h_enable = 0;
	osd_hw.free_scale[OSD3].v_enable = 0;
	osd_hw.free_scale[OSD4].h_enable = 0;
	osd_hw.free_scale[OSD4].v_enable = 0;

	if (osd_reverse) {
		if (!strcmp(osd_reverse, "all,true"))
			reverse_val = REVERSE_XY;
		else if (!strcmp(osd_reverse, "all,x_rev"))
			reverse_val = REVERSE_X;
		else if (!strcmp(osd_reverse, "all,y_rev"))
			reverse_val = REVERSE_Y;
		else
			reverse_val = REVERSE_NONE;
	}
	osd_hw.osd_reverse[OSD1] = reverse_val;
	osd_hw.osd_reverse[OSD2] = reverse_val;
	osd_hw.osd_reverse[OSD3] = reverse_val;
	osd_hw.osd_reverse[OSD4] = reverse_val;

	osd_hw.rotation_pandata[OSD1].x_start = 0;
	osd_hw.rotation_pandata[OSD1].y_start = 0;
	osd_hw.rotation_pandata[OSD2].x_start = 0;
	osd_hw.rotation_pandata[OSD2].y_start = 0;
	osd_hw.rotation_pandata[OSD3].x_start = 0;
	osd_hw.rotation_pandata[OSD3].y_start = 0;
	osd_hw.rotation_pandata[OSD4].x_start = 0;
	osd_hw.rotation_pandata[OSD4].y_start = 0;

	osd_hw.antiflicker_mode = 0;
	if (osd_get_chip_type() >= MESON_CPU_MAJOR_ID_M8) {
		osd_hw.free_scale_data[OSD1].x_start = 0;
		osd_hw.free_scale_data[OSD1].x_end = 0;
		osd_hw.free_scale_data[OSD1].y_start = 0;
		osd_hw.free_scale_data[OSD1].y_end = 0;
		osd_hw.free_scale_data[OSD2].x_start = 0;
		osd_hw.free_scale_data[OSD2].x_end = 0;
		osd_hw.free_scale_data[OSD2].y_start = 0;
		osd_hw.free_scale_data[OSD2].y_end = 0;
		osd_hw.free_scale_data[OSD3].x_start = 0;
		osd_hw.free_scale_data[OSD3].x_end = 0;
		osd_hw.free_scale_data[OSD3].y_start = 0;
		osd_hw.free_scale_data[OSD3].y_end = 0;
		osd_hw.free_scale_data[OSD4].x_start = 0;
		osd_hw.free_scale_data[OSD4].x_end = 0;
		osd_hw.free_scale_data[OSD4].y_start = 0;
		osd_hw.free_scale_data[OSD4].y_end = 0;

		osd_hw.free_scale_mode[OSD1] = 1;
		osd_hw.free_scale_mode[OSD2] = 1;
		osd_hw.free_scale_mode[OSD3] = 1;
		osd_hw.free_scale_mode[OSD4] = 1;

		if ((osd_get_chip_type() == MESON_CPU_MAJOR_ID_GXM)
			||(osd_get_chip_type() == MESON_CPU_MAJOR_ID_TXLX))
			osd_reg_write(hw_osd_reg_array[0].osd_sc_dummy_data,
				0x00202000);
		else if (osd_get_chip_type() ==
			MESON_CPU_MAJOR_ID_GXTVBB)
			osd_reg_write(hw_osd_reg_array[0].osd_sc_dummy_data, 0xff);
		else
			osd_reg_write(hw_osd_reg_array[0].osd_sc_dummy_data,
				0x00808000);
	} else {
		osd_hw.free_scale_mode[OSD1] = 0;
		osd_hw.free_scale_mode[OSD2] = 0;
		osd_hw.free_scale_mode[OSD3] = 0;
		osd_hw.free_scale_mode[OSD4] = 0;
	}
	memset(osd_hw.rotate, 0, sizeof(struct osd_rotate_s) * HW_OSD_COUNT);
	if ((osd_get_chip_type() == MESON_CPU_MAJOR_ID_G12A) ||
		((osd_get_chip_type() == MESON_CPU_MAJOR_ID_G12B) &&
		(get_cpu_id().chip_rev == MESON_CPU_CHIP_REVISION_A)))
		osd_hw.shift_line = 1;
	else
		osd_hw.shift_line = 0;
	if (osd_get_chip_type() == MESON_CPU_MAJOR_ID_T7 ||
	    osd_get_chip_type() == MESON_CPU_MAJOR_ID_T3)
		osd_hw.mif_linear = 1;

	return;
}
