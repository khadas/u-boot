/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __VPP_H__
#define __VPP_H__

/* OSD csc defines */
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

enum vpp_lut_sel_e {
	VPP_LUT_OSD_EOTF = 0,
	VPP_LUT_OSD_OETF,
	VPP_LUT_EOTF,
	VPP_LUT_OETF
};
#define NUM_LUT 4

/* matrix registers */
struct matrix_s {
	u16 pre_offset[3];
	u16 matrix[3][3];
	u16 offset[3];
	u16 right_shift;
};

/* vpp1 post2 matrix */
#ifndef VPP1_MATRIX_COEF00_01
#define VPP1_MATRIX_COEF00_01                      0x5990
#endif
#ifndef VPP1_MATRIX_COEF02_10
#define VPP1_MATRIX_COEF02_10                      0x5991
#endif
#ifndef VPP1_MATRIX_COEF11_12
#define VPP1_MATRIX_COEF11_12                      0x5992
#endif
#ifndef VPP1_MATRIX_COEF20_21
#define VPP1_MATRIX_COEF20_21                      0x5993
#endif
#ifndef VPP1_MATRIX_COEF22
#define VPP1_MATRIX_COEF22                         0x5994
#endif
#ifndef VPP1_MATRIX_COEF13_14
#define VPP1_MATRIX_COEF13_14                      0x5995
#endif
#ifndef VPP1_MATRIX_COEF23_24
#define VPP1_MATRIX_COEF23_24                      0x5996
#endif
#ifndef VPP1_MATRIX_COEF15_25
#define VPP1_MATRIX_COEF15_25                      0x5997
#endif
#ifndef VPP1_MATRIX_CLIP
#define VPP1_MATRIX_CLIP                           0x5998
#endif
#ifndef VPP1_MATRIX_OFFSET0_1
#define VPP1_MATRIX_OFFSET0_1                      0x5999
#endif
#ifndef VPP1_MATRIX_OFFSET2
#define VPP1_MATRIX_OFFSET2                        0x599a
#endif
#ifndef VPP1_MATRIX_PRE_OFFSET0_1
#define VPP1_MATRIX_PRE_OFFSET0_1                  0x599b
#endif
#ifndef VPP1_MATRIX_PRE_OFFSET2
#define VPP1_MATRIX_PRE_OFFSET2                    0x599c
#endif
#ifndef VPP1_MATRIX_EN_CTRL
#define VPP1_MATRIX_EN_CTRL                        0x599d
#endif

/* vpp2 post2 matrix */
#ifndef VPP2_MATRIX_COEF00_01
#define VPP2_MATRIX_COEF00_01                      0x59d0
#endif
#ifndef VPP2_MATRIX_COEF02_10
#define VPP2_MATRIX_COEF02_10                      0x59d1
#endif
#ifndef VPP2_MATRIX_COEF11_12
#define VPP2_MATRIX_COEF11_12                      0x59d2
#endif
#ifndef VPP2_MATRIX_COEF20_21
#define VPP2_MATRIX_COEF20_21                      0x59d3
#endif
#ifndef VPP2_MATRIX_COEF22
#define VPP2_MATRIX_COEF22                         0x59d4
#endif
#ifndef VPP2_MATRIX_COEF13_14
#define VPP2_MATRIX_COEF13_14                      0x59d5
#endif
#ifndef VPP2_MATRIX_COEF23_24
#define VPP2_MATRIX_COEF23_24                      0x59d6
#endif
#ifndef VPP2_MATRIX_COEF15_25
#define VPP2_MATRIX_COEF15_25                      0x59d7
#endif
#ifndef VPP2_MATRIX_CLIP
#define VPP2_MATRIX_CLIP                           0x59d8
#endif
#ifndef VPP2_MATRIX_OFFSET0_1
#define VPP2_MATRIX_OFFSET0_1                      0x59d9
#endif
#ifndef VPP2_MATRIX_OFFSET2
#define VPP2_MATRIX_OFFSET2                        0x59da
#endif
#ifndef VPP2_MATRIX_PRE_OFFSET0_1
#define VPP2_MATRIX_PRE_OFFSET0_1                  0x59db
#endif
#ifndef VPP2_MATRIX_PRE_OFFSET2
#define VPP2_MATRIX_PRE_OFFSET2                    0x59dc
#endif
#ifndef VPP2_MATRIX_EN_CTRL
#define VPP2_MATRIX_EN_CTRL                        0x59dd
#endif

#endif
