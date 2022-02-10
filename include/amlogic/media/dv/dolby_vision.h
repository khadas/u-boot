/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _DOBLY_VISION_H_
#define _DOBLY_VISION_H_

#define DOLBY_VISION_OUTPUT_MODE_IPT			0
#define DOLBY_VISION_OUTPUT_MODE_IPT_TUNNEL		1
#define DOLBY_VISION_OUTPUT_MODE_HDR10			2
#define DOLBY_VISION_OUTPUT_MODE_SDR10			3
#define DOLBY_VISION_OUTPUT_MODE_SDR8			4
#define DOLBY_VISION_OUTPUT_MODE_BYPASS			5

#define DOLBY_VISION_LL_DISABLE				0
#define DOLBY_VISION_LL_YUV422				1
#define DOLBY_VISION_LL_RGB444				2

#define DOLBY_VISION_SET_DISABLE    "0"
#define DOLBY_VISION_SET_STD        "1"
#define DOLBY_VISION_SET_LL_YUV     "2"
#define DOLBY_VISION_SET_LL_RGB     "3"

enum vpp_matrix_sel_e {
	VPP_MATRIX_0 = 0,	/* OSD convert matrix - new from GXL */
	VPP_MATRIX_1,		/* vd1 matrix before post-blend */
	VPP_MATRIX_2,		/* post matrix */
	VPP_MATRIX_3,		/* xvycc matrix */
	VPP_MATRIX_4,		/* in video eotf - new from GXL */
	VPP_MATRIX_5,		/* in osd eotf - new from GXL */
	VPP_MATRIX_6		/* vd2 matrix before pre-blend */
};
#define NUM_MATRIX 6

/* matrix names */
#define VPP_MATRIX_OSD		VPP_MATRIX_0
#define VPP_MATRIX_VD1		VPP_MATRIX_1
#define VPP_MATRIX_POST		VPP_MATRIX_2
#define VPP_MATRIX_XVYCC	VPP_MATRIX_3
#define VPP_MATRIX_EOTF		VPP_MATRIX_4
#define VPP_MATRIX_OSD_EOTF	VPP_MATRIX_5
#define VPP_MATRIX_VD2		VPP_MATRIX_6

#define CSC_ON              1
#define CSC_OFF             0

enum mtx_en_e {
	POST_MTX_EN = 0,
	VD2_MTX_EN = 4,
	VD1_MTX_EN,
	XVY_MTX_EN,
	OSD1_MTX_EN
};

struct dovi_mode_s {
	u8 dv_rgb_444_8bit;
	u8 ll_ycbcr_422_12bit;
	u8 ll_rgb_444_10bit;
	u8 ll_rgb_444_12bit;
	u8 sup_2160p60hz;
};

#define POST_MTX_EN_MASK (1 << POST_MTX_EN)
#define VD2_MTX_EN_MASK  (1 << VD2_MTX_EN)
#define VD1_MTX_EN_MASK  (1 << VD1_MTX_EN)
#define XVY_MTX_EN_MASK  (1 << XVY_MTX_EN)
#define OSD1_MTX_EN_MASK (1 << OSD1_MTX_EN)

extern bool dolby_vision_on;
void dolby_vision_process(void);
void send_hdmi_pkt(void);
int apply_stb_core_settings(void);
unsigned int  dv_read_reg(unsigned int addr);
void dv_write_reg(unsigned int addr, unsigned int value);
void dolbyvision_dump_setting(void);
void dolbyvision_debug(int enable_debug);

#ifdef CONFIG_AML_DOLBY
extern int is_dolby_enable(void);
extern bool request_ll_mode(void);
extern bool check_dolby_vision_on(void);
#else
int __attribute__((weak))is_dolby_enable(void)
{
	return 0;
}
bool __attribute__((weak))request_ll_mode(void)
{
	return false;
}
bool __attribute__((weak))check_dolby_vision_on(void)
{
	return false;
}
#endif


#endif
