/*
 * drivers/display/dolby_vision.h
 *
 * Copyright (C) 2018 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
*/


#ifndef _DOBLY_VISION_H_
#define _DOBLY_VISION_H_



#define EXT_MD_AVAIL_LEVEL_1    (1 << 0)
#define EXT_MD_AVAIL_LEVEL_2    (1 << 1)
#define EXT_MD_AVAIL_LEVEL_4    (1 << 2)
#define EXT_MD_AVAIL_LEVEL_5    (1 << 3)
#define EXT_MD_AVAIL_LEVEL_6    (1 << 4)
#define EXT_MD_AVAIL_LEVEL_255  (1 << 31)

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

#define DEF_G2L_LUT_SIZE_2P        8
#define DEF_G2L_LUT_SIZE           (1 << DEF_G2L_LUT_SIZE_2P)

/*
#define DOLBY_CORE2A_REG_START			0x3400
*/
#ifndef DOLBY_CORE2A_CTRL
#define DOLBY_CORE2A_CTRL			0x3401
#endif
/*

#define DOLBY_CORE2A_CLKGATE_CTRL		0x3432
#define DOLBY_CORE2A_SWAP_CTRL0			0x3433
#define DOLBY_CORE2A_SWAP_CTRL1			0x3434
#define DOLBY_CORE2A_SWAP_CTRL2			0x3435
#define DOLBY_CORE2A_SWAP_CTRL3			0x3436
#define DOLBY_CORE2A_SWAP_CTRL4			0x3437
#define DOLBY_CORE2A_SWAP_CTRL5			0x3438
#define DOLBY_CORE2A_DMA_CTRL			0x3439
#define DOLBY_CORE2A_DMA_STATUS			0x343a
#define DOLBY_CORE2A_STATUS0			0x343b
#define DOLBY_CORE2A_STATUS1			0x343c
#define DOLBY_CORE2A_STATUS2			0x343d
#define DOLBY_CORE2A_STATUS3			0x343e
#define DOLBY_CORE2A_DMA_PORT			0x343f

#define DOLBY_CORE3_REG_START			0x3600
#define DOLBY_CORE3_CLKGATE_CTRL		0x36f0
#define DOLBY_CORE3_SWAP_CTRL0			0x36f1
#define DOLBY_CORE3_SWAP_CTRL1			0x36f2
#define DOLBY_CORE3_SWAP_CTRL2			0x36f3
#define DOLBY_CORE3_SWAP_CTRL3			0x36f4
#define DOLBY_CORE3_SWAP_CTRL4			0x36f5
#define DOLBY_CORE3_SWAP_CTRL5			0x36f6
#define DOLBY_CORE3_SWAP_CTRL6			0x36f7
*/

#ifndef DOLBY_CORE3_DIAG_CTRL
#define DOLBY_CORE3_DIAG_CTRL			0x36f8
#endif
/*
#define DOLBY_CORE3_CRC_CTRL			0x36fb
#define DOLBY_CORE3_INPUT_CSC_CRC		0x36fc
#define DOLBY_CORE3_OUTPUT_CSC_CRC		0x36fd
*/

#ifndef VPP_WRAP_OSD1_MATRIX_EN_CTRL
#define VPP_WRAP_OSD1_MATRIX_EN_CTRL            0x3d6d
#endif

#ifndef VPP_WRAP_OSD2_MATRIX_EN_CTRL
#define VPP_WRAP_OSD2_MATRIX_EN_CTRL            0x3d7d
#endif

#ifndef VPP_WRAP_OSD3_MATRIX_EN_CTRL
#define VPP_WRAP_OSD3_MATRIX_EN_CTRL            0x3dbd
#endif

#ifndef DOLBY_PATH_CTRL
#define DOLBY_PATH_CTRL                         0x1a0c
#endif

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
