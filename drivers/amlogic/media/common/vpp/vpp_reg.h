/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __VPP_REG_H__
#define __VPP_REG_H__
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>

/* ********************************
 * register define
 * ********************************* */
#ifndef REG_BASE_VCBUS
#define REG_BASE_VCBUS                  (0xFF900000L)
#endif
/* base & offset */
//#define REG_BASE_VCBUS                  (0xd0100000L)
#define REG_OFFSET_VCBUS(reg)           ((reg) << 2)
/* memory mapping */
#define REG_ADDR_VCBUS(reg)             (REG_BASE_VCBUS + REG_OFFSET_VCBUS(reg) + 0L)

#define REG_VPP_ADDR(reg)               (reg + 0L)

#ifndef VIU_EOTF_CTL
#ifdef VPP_EOTF_CTL
#define VIU_EOTF_CTL VPP_EOTF_CTL
#endif
#endif

#ifndef VIU_EOTF_LUT_ADDR_PORT
#ifdef VPP_EOTF_LUT_ADDR_PORT
#define VIU_EOTF_LUT_ADDR_PORT VPP_EOTF_LUT_ADDR_PORT
#endif
#endif

#ifndef VIU_EOTF_LUT_DATA_PORT
#ifdef VPP_EOTF_LUT_DATA_PORT
#define VIU_EOTF_LUT_DATA_PORT VPP_EOTF_LUT_DATA_PORT
#endif
#endif

#ifndef VPP_OSD1_MATRIX_COEF00_01
#define VPP_OSD1_MATRIX_COEF00_01           0x3910
#endif

#ifndef VPP_OSD1_MATRIX_COEF02_10
#define VPP_OSD1_MATRIX_COEF02_10           0x3911
#endif

#ifndef VPP_OSD1_MATRIX_COEF11_12
#define VPP_OSD1_MATRIX_COEF11_12           0x3912
#endif

#ifndef VPP_OSD1_MATRIX_COEF20_21
#define VPP_OSD1_MATRIX_COEF20_21           0x3913
#endif

#ifndef VPP_OSD1_MATRIX_COEF22
#define VPP_OSD1_MATRIX_COEF22              0x3914
#endif

#ifndef VPP_OSD1_MATRIX_COEF13_14
#define VPP_OSD1_MATRIX_COEF13_14           0x3915
#endif

#ifndef VPP_OSD1_MATRIX_COEF23_24
#define VPP_OSD1_MATRIX_COEF23_24           0x3916
#endif

#ifndef VPP_OSD1_MATRIX_COEF15_25
#define VPP_OSD1_MATRIX_COEF15_25           0x3917
#endif

#ifndef VPP_OSD1_MATRIX_CLIP
#define VPP_OSD1_MATRIX_CLIP                0x3918
#endif

#ifndef VPP_OSD1_MATRIX_OFFSET0_1
#define VPP_OSD1_MATRIX_OFFSET0_1           0x3919
#endif

#ifndef VPP_OSD1_MATRIX_OFFSET2
#define VPP_OSD1_MATRIX_OFFSET2             0x391a
#endif

#ifndef VPP_OSD1_MATRIX_PRE_OFFSET0_1
#define VPP_OSD1_MATRIX_PRE_OFFSET0_1       0x391b
#endif

#ifndef VPP_OSD1_MATRIX_PRE_OFFSET2
#define VPP_OSD1_MATRIX_PRE_OFFSET2         0x391c
#endif

#ifndef VPP_OSD1_MATRIX_EN_CTRL
#define VPP_OSD1_MATRIX_EN_CTRL             0x391d
#endif

/* ********************************
 * dummy registers *
 * ********************************* */
#ifndef S0_VPP_POST2_MATRIX_COEF00_01
#define S0_VPP_POST2_MATRIX_COEF00_01              0x2560
#endif

#ifndef S0_VPP_POST2_MATRIX_COEF02_10
#define S0_VPP_POST2_MATRIX_COEF02_10              0x2561
#endif

#ifndef S0_VPP_POST2_MATRIX_COEF11_12
#define S0_VPP_POST2_MATRIX_COEF11_12              0x2562
#endif

#ifndef S0_VPP_POST2_MATRIX_COEF20_21
#define S0_VPP_POST2_MATRIX_COEF20_21              0x2563
#endif

#ifndef S0_VPP_POST2_MATRIX_COEF22
#define S0_VPP_POST2_MATRIX_COEF22                 0x2564
#endif

#ifndef S0_VPP_POST2_MATRIX_OFFSET0_1
#define S0_VPP_POST2_MATRIX_OFFSET0_1              0x2569
#endif

#ifndef S0_VPP_POST2_MATRIX_OFFSET2
#define S0_VPP_POST2_MATRIX_OFFSET2                0x256a
#endif

#ifndef S0_VPP_POST2_MATRIX_PRE_OFFSET0_1
#define S0_VPP_POST2_MATRIX_PRE_OFFSET0_1          0x256b
#endif

#ifndef S0_VPP_POST2_MATRIX_PRE_OFFSET2
#define S0_VPP_POST2_MATRIX_PRE_OFFSET2            0x256c
#endif

#ifndef S0_VPP_POST2_MATRIX_EN_CTRL
#define S0_VPP_POST2_MATRIX_EN_CTRL                0x256d
#endif

#ifndef VPP_POST2_MATRIX_PRE_OFFSET0_1
#define VPP_POST2_MATRIX_PRE_OFFSET0_1             0x39ab
#endif

#ifndef VPP_POST2_MATRIX_PRE_OFFSET2
#define VPP_POST2_MATRIX_PRE_OFFSET2               0x39ac
#endif

#ifndef VPP_POST2_MATRIX_COEF00_01
#define VPP_POST2_MATRIX_COEF00_01                 0x39a0
#endif

#ifndef VPP_POST2_MATRIX_COEF02_10
#define VPP_POST2_MATRIX_COEF02_10                 0x39a1
#endif

#ifndef VPP_POST2_MATRIX_COEF11_12
#define VPP_POST2_MATRIX_COEF11_12                 0x39a2
#endif

#ifndef VPP_POST2_MATRIX_COEF20_21
#define VPP_POST2_MATRIX_COEF20_21                 0x39a3
#endif

#ifndef VPP_POST2_MATRIX_COEF22
#define VPP_POST2_MATRIX_COEF22                    0x39a4
#endif

#ifndef VPP_POST2_MATRIX_OFFSET0_1
#define VPP_POST2_MATRIX_OFFSET0_1                 0x39a9
#endif

#ifndef VPP_POST2_MATRIX_OFFSET2
#define VPP_POST2_MATRIX_OFFSET2                   0x39aa
#endif

#ifndef VPP_POST2_MATRIX_EN_CTRL
#define VPP_POST2_MATRIX_EN_CTRL                   0x39ad
#endif

#ifndef VPP_WRAP_OSD1_MATRIX_PRE_OFFSET0_1
#define VPP_WRAP_OSD1_MATRIX_PRE_OFFSET0_1         0x3d6b
#endif

#ifndef VPP_WRAP_OSD1_MATRIX_PRE_OFFSET2
#define VPP_WRAP_OSD1_MATRIX_PRE_OFFSET2           0x3d6c
#endif

#ifndef VPP_WRAP_OSD1_MATRIX_COEF00_01
#define VPP_WRAP_OSD1_MATRIX_COEF00_01             0x3d60
#endif

#ifndef VPP_WRAP_OSD1_MATRIX_COEF02_10
#define VPP_WRAP_OSD1_MATRIX_COEF02_10             0x3d61
#endif

#ifndef VPP_WRAP_OSD1_MATRIX_COEF11_12
#define VPP_WRAP_OSD1_MATRIX_COEF11_12             0x3d62
#endif

#ifndef VPP_WRAP_OSD1_MATRIX_COEF20_21
#define VPP_WRAP_OSD1_MATRIX_COEF20_21             0x3d63
#endif

#ifndef VPP_WRAP_OSD1_MATRIX_COEF22
#define VPP_WRAP_OSD1_MATRIX_COEF22                0x3d64
#endif

#ifndef VPP_WRAP_OSD1_MATRIX_OFFSET0_1
#define VPP_WRAP_OSD1_MATRIX_OFFSET0_1             0x3d69
#endif

#ifndef VPP_WRAP_OSD1_MATRIX_OFFSET2
#define VPP_WRAP_OSD1_MATRIX_OFFSET2               0x3d6a
#endif

#ifndef VPP_WRAP_OSD1_MATRIX_EN_CTRL
#define VPP_WRAP_OSD1_MATRIX_EN_CTRL               0x3d6d
#endif

#ifndef VPP_WRAP_OSD2_MATRIX_PRE_OFFSET0_1
#define VPP_WRAP_OSD2_MATRIX_PRE_OFFSET0_1         0x3d7b
#endif

#ifndef VPP_WRAP_OSD2_MATRIX_PRE_OFFSET2
#define VPP_WRAP_OSD2_MATRIX_PRE_OFFSET2           0x3d7c
#endif

#ifndef VPP_WRAP_OSD2_MATRIX_COEF00_01
#define VPP_WRAP_OSD2_MATRIX_COEF00_01             0x3d70
#endif

#ifndef VPP_WRAP_OSD2_MATRIX_COEF02_10
#define VPP_WRAP_OSD2_MATRIX_COEF02_10             0x3d71
#endif

#ifndef VPP_WRAP_OSD2_MATRIX_COEF11_12
#define VPP_WRAP_OSD2_MATRIX_COEF11_12             0x3d72
#endif

#ifndef VPP_WRAP_OSD2_MATRIX_COEF20_21
#define VPP_WRAP_OSD2_MATRIX_COEF20_21             0x3d73
#endif

#ifndef VPP_WRAP_OSD2_MATRIX_COEF22
#define VPP_WRAP_OSD2_MATRIX_COEF22                0x3d74
#endif

#ifndef VPP_WRAP_OSD2_MATRIX_OFFSET0_1
#define VPP_WRAP_OSD2_MATRIX_OFFSET0_1             0x3d79
#endif

#ifndef VPP_WRAP_OSD2_MATRIX_OFFSET2
#define VPP_WRAP_OSD2_MATRIX_OFFSET2               0x3d7a
#endif

#ifndef VPP_WRAP_OSD2_MATRIX_EN_CTRL
#define VPP_WRAP_OSD2_MATRIX_EN_CTRL               0x3d7d
#endif

#ifndef VPP_WRAP_OSD3_MATRIX_PRE_OFFSET0_1
#define VPP_WRAP_OSD3_MATRIX_PRE_OFFSET0_1         0x3dbb
#endif

#ifndef VPP_WRAP_OSD3_MATRIX_PRE_OFFSET2
#define VPP_WRAP_OSD3_MATRIX_PRE_OFFSET2           0x3dbc
#endif

#ifndef VPP_WRAP_OSD3_MATRIX_COEF00_01
#define VPP_WRAP_OSD3_MATRIX_COEF00_01             0x3db0
#endif

#ifndef VPP_WRAP_OSD3_MATRIX_COEF02_10
#define VPP_WRAP_OSD3_MATRIX_COEF02_10             0x3db1
#endif

#ifndef VPP_WRAP_OSD3_MATRIX_COEF11_12
#define VPP_WRAP_OSD3_MATRIX_COEF11_12             0x3db2
#endif

#ifndef VPP_WRAP_OSD3_MATRIX_COEF20_21
#define VPP_WRAP_OSD3_MATRIX_COEF20_21             0x3db3
#endif

#ifndef VPP_WRAP_OSD3_MATRIX_COEF22
#define VPP_WRAP_OSD3_MATRIX_COEF22                0x3db4
#endif

#ifndef VPP_WRAP_OSD3_MATRIX_OFFSET0_1
#define VPP_WRAP_OSD3_MATRIX_OFFSET0_1             0x3db9
#endif

#ifndef VPP_WRAP_OSD3_MATRIX_OFFSET2
#define VPP_WRAP_OSD3_MATRIX_OFFSET2               0x3dba
#endif

#ifndef VPP_WRAP_OSD3_MATRIX_EN_CTRL
#define VPP_WRAP_OSD3_MATRIX_EN_CTRL               0x3dbd
#endif

#ifndef VIU2_OSD1_MATRIX_COEF00_01
#define VIU2_OSD1_MATRIX_COEF00_01                 0x1e70
#endif
#ifndef VIU2_OSD1_MATRIX_COEF02_10
#define VIU2_OSD1_MATRIX_COEF02_10                 0x1e71
#endif
#ifndef VIU2_OSD1_MATRIX_COEF11_12
#define VIU2_OSD1_MATRIX_COEF11_12                 0x1e72
#endif
#ifndef VIU2_OSD1_MATRIX_COEF20_21
#define VIU2_OSD1_MATRIX_COEF20_21                 0x1e73
#endif
#ifndef VIU2_OSD1_MATRIX_COEF22
#define VIU2_OSD1_MATRIX_COEF22                    0x1e74
#endif
#ifndef VIU2_OSD1_MATRIX_COEF13_14
#define VIU2_OSD1_MATRIX_COEF13_14                 0x1e75
#endif
#ifndef VIU2_OSD1_MATRIX_COEF23_24
#define VIU2_OSD1_MATRIX_COEF23_24                 0x1e76
#endif
#ifndef VIU2_OSD1_MATRIX_COEF15_25
#define VIU2_OSD1_MATRIX_COEF15_25                 0x1e77
#endif
#ifndef VIU2_OSD1_MATRIX_CLIP
#define VIU2_OSD1_MATRIX_CLIP                      0x1e78
#endif
#ifndef VIU2_OSD1_MATRIX_OFFSET0_1
#define VIU2_OSD1_MATRIX_OFFSET0_1                 0x1e79
#endif
#ifndef VIU2_OSD1_MATRIX_OFFSET2
#define VIU2_OSD1_MATRIX_OFFSET2                   0x1e7a
#endif
#ifndef VIU2_OSD1_MATRIX_PRE_OFFSET0_1
#define VIU2_OSD1_MATRIX_PRE_OFFSET0_1             0x1e7b
#endif
#ifndef VIU2_OSD1_MATRIX_PRE_OFFSET2
#define VIU2_OSD1_MATRIX_PRE_OFFSET2               0x1e7c
#endif
#ifndef VIU2_OSD1_MATRIX_EN_CTRL
#define VIU2_OSD1_MATRIX_EN_CTRL                   0x1e7d
#endif

#ifndef VPP_OSD2_MATRIX_COEF00_01
#define VPP_OSD2_MATRIX_COEF00_01           0x3920
#endif

#ifndef VPP_OSD2_MATRIX_COEF02_10
#define VPP_OSD2_MATRIX_COEF02_10           0x3921
#endif

#ifndef VPP_OSD2_MATRIX_COEF11_12
#define VPP_OSD2_MATRIX_COEF11_12           0x3922
#endif

#ifndef VPP_OSD2_MATRIX_COEF20_21
#define VPP_OSD2_MATRIX_COEF20_21           0x3923
#endif

#ifndef VPP_OSD2_MATRIX_COEF22
#define VPP_OSD2_MATRIX_COEF22              0x3924
#endif

#ifndef VPP_OSD2_MATRIX_COEF13_14
#define VPP_OSD2_MATRIX_COEF13_14           0x3925
#endif

#ifndef VPP_OSD2_MATRIX_COEF23_24
#define VPP_OSD2_MATRIX_COEF23_24           0x3926
#endif

#ifndef VPP_OSD2_MATRIX_COEF15_25
#define VPP_OSD2_MATRIX_COEF15_25           0x3927
#endif

#ifndef VPP_OSD2_MATRIX_CLIP
#define VPP_OSD2_MATRIX_CLIP                0x3928
#endif

#ifndef VPP_OSD2_MATRIX_OFFSET0_1
#define VPP_OSD2_MATRIX_OFFSET0_1           0x3929
#endif

#ifndef VPP_OSD2_MATRIX_OFFSET2
#define VPP_OSD2_MATRIX_OFFSET2             0x392a
#endif

#ifndef VPP_OSD2_MATRIX_PRE_OFFSET0_1
#define VPP_OSD2_MATRIX_PRE_OFFSET0_1       0x392b
#endif

#ifndef VPP_OSD2_MATRIX_PRE_OFFSET2
#define VPP_OSD2_MATRIX_PRE_OFFSET2         0x392c
#endif

#ifndef VPP_OSD2_MATRIX_EN_CTRL
#define VPP_OSD2_MATRIX_EN_CTRL             0x392d
#endif

#ifndef VIU_OSD4_MATRIX_COEF00_01
#define VIU_OSD4_MATRIX_COEF00_01                  0x3df0
#endif

#ifndef VIU_OSD4_MATRIX_COEF02_10
#define VIU_OSD4_MATRIX_COEF02_10                  0x3df1
#endif

#ifndef VIU_OSD4_MATRIX_COEF11_12
#define VIU_OSD4_MATRIX_COEF11_12                  0x3df2
#endif

#ifndef VIU_OSD4_MATRIX_COEF20_21
#define VIU_OSD4_MATRIX_COEF20_21                  0x3df3
#endif

#ifndef VIU_OSD4_MATRIX_COEF22
#define VIU_OSD4_MATRIX_COEF22                     0x3df4
#endif

#ifndef VIU_OSD4_MATRIX_COEF13_14
#define VIU_OSD4_MATRIX_COEF13_14                  0x3df5
#endif

#ifndef VIU_OSD4_MATRIX_COEF23_24
#define VIU_OSD4_MATRIX_COEF23_24                  0x3df6
#endif

#ifndef VIU_OSD4_MATRIX_COEF15_25
#define VIU_OSD4_MATRIX_COEF15_25                  0x3df7
#endif

#ifndef VIU_OSD4_MATRIX_CLIP
#define VIU_OSD4_MATRIX_CLIP                       0x3df8
#endif

#ifndef VIU_OSD4_MATRIX_OFFSET0_1
#define VIU_OSD4_MATRIX_OFFSET0_1                  0x3df9
#endif

#ifndef VIU_OSD4_MATRIX_OFFSET2
#define VIU_OSD4_MATRIX_OFFSET2                    0x3dfa
#endif

#ifndef VIU_OSD4_MATRIX_PRE_OFFSET0_1
#define VIU_OSD4_MATRIX_PRE_OFFSET0_1              0x3dfb
#endif

#ifndef VIU_OSD4_MATRIX_PRE_OFFSET2
#define VIU_OSD4_MATRIX_PRE_OFFSET2                0x3dfc
#endif

#ifndef VIU_OSD4_MATRIX_EN_CTRL
#define VIU_OSD4_MATRIX_EN_CTRL                    0x3dfd
#endif

#ifndef MALI_AFBCD_TOP_CTRL
#define MALI_AFBCD_TOP_CTRL                        0x1a0f
#endif

#ifndef MALI_AFBCD1_TOP_CTRL
#define MALI_AFBCD1_TOP_CTRL                       0x1a55
#endif

#ifndef MALI_AFBCD2_TOP_CTRL
#define MALI_AFBCD2_TOP_CTRL                       0x1a56
#endif

#ifndef DOLBY_PATH_CTRL
#define DOLBY_PATH_CTRL                            0x1a0c
#endif

#ifndef LCD_GAMMA_CNTL_PORT0
#define LCD_GAMMA_CNTL_PORT0                       0x14b4
#endif
#ifndef LCD_GAMMA_DATA_PORT0
#define LCD_GAMMA_DATA_PORT0                       0x14b5
#endif
#ifndef LCD_GAMMA_ADDR_PORT0
#define LCD_GAMMA_ADDR_PORT0                       0x14b6
#endif

//s5 matrix
#define S5_VPP_VD1_MATRIX_COEF00_01                0x1d90
#define S5_VPP_VD1_MATRIX_COEF02_10                0x1d91
#define S5_VPP_VD1_MATRIX_COEF11_12                0x1d92
#define S5_VPP_VD1_MATRIX_COEF20_21                0x1d93
#define S5_VPP_VD1_MATRIX_COEF22                   0x1d94
#define S5_VPP_VD1_MATRIX_COEF13_14                0x1d95
#define S5_VPP_VD1_MATRIX_COEF23_24                0x1d96
#define S5_VPP_VD1_MATRIX_COEF15_25                0x1d97
#define S5_VPP_VD1_MATRIX_CLIP                     0x1d98
#define S5_VPP_VD1_MATRIX_OFFSET0_1                0x1d99
#define S5_VPP_VD1_MATRIX_OFFSET2                  0x1d9a
#define S5_VPP_VD1_MATRIX_PRE_OFFSET0_1            0x1d9b
#define S5_VPP_VD1_MATRIX_PRE_OFFSET2              0x1d9c
#define S5_VPP_VD1_MATRIX_EN_CTRL                  0x1d9d
#define S5_VPP_VD1_MATRIX_SAT                      0x1d9e

#define S5_VPP_POST2_MATRIX_COEF00_01              0x2560
#define S5_VPP_POST2_MATRIX_COEF02_10              0x2561
#define S5_VPP_POST2_MATRIX_COEF11_12              0x2562
#define S5_VPP_POST2_MATRIX_COEF20_21              0x2563
#define S5_VPP_POST2_MATRIX_COEF22                 0x2564
#define S5_VPP_POST2_MATRIX_COEF13_14              0x2565
#define S5_VPP_POST2_MATRIX_COEF23_24              0x2566
#define S5_VPP_POST2_MATRIX_COEF15_25              0x2567
#define S5_VPP_POST2_MATRIX_CLIP                   0x2568
#define S5_VPP_POST2_MATRIX_OFFSET0_1              0x2569
#define S5_VPP_POST2_MATRIX_OFFSET2                0x256a
#define S5_VPP_POST2_MATRIX_PRE_OFFSET0_1          0x256b
#define S5_VPP_POST2_MATRIX_PRE_OFFSET2            0x256c
#define S5_VPP_POST2_MATRIX_EN_CTRL                0x256d

#define S5_VPP_POST_MATRIX_COEF00_01               0x2580
#define S5_VPP_POST_MATRIX_COEF02_10               0x2581
#define S5_VPP_POST_MATRIX_COEF11_12               0x2582
#define S5_VPP_POST_MATRIX_COEF20_21               0x2583
#define S5_VPP_POST_MATRIX_COEF22                  0x2584
#define S5_VPP_POST_MATRIX_COEF13_14               0x2585
#define S5_VPP_POST_MATRIX_COEF23_24               0x2586
#define S5_VPP_POST_MATRIX_COEF15_25               0x2587
#define S5_VPP_POST_MATRIX_CLIP                    0x2588
#define S5_VPP_POST_MATRIX_OFFSET0_1               0x2589
#define S5_VPP_POST_MATRIX_OFFSET2                 0x258a
#define S5_VPP_POST_MATRIX_PRE_OFFSET0_1           0x258b
#define S5_VPP_POST_MATRIX_PRE_OFFSET2             0x258c
#define S5_VPP_POST_MATRIX_EN_CTRL                 0x258d
#define S5_VPP_POST_MATRIX_SAT                     0x258e

#define S5_VPP_SLICE1_VD1_MATRIX_COEF00_01          0x2890
#define S5_VPP_SLICE1_VD1_MATRIX_COEF02_10          0x2891
#define S5_VPP_SLICE1_VD1_MATRIX_COEF11_12          0x2892
#define S5_VPP_SLICE1_VD1_MATRIX_COEF20_21          0x2893
#define S5_VPP_SLICE1_VD1_MATRIX_COEF22             0x2894
#define S5_VPP_SLICE1_VD1_MATRIX_COEF13_14          0x2895
#define S5_VPP_SLICE1_VD1_MATRIX_COEF23_24          0x2896
#define S5_VPP_SLICE1_VD1_MATRIX_COEF15_25          0x2897
#define S5_VPP_SLICE1_VD1_MATRIX_CLIP               0x2898
#define S5_VPP_SLICE1_VD1_MATRIX_OFFSET0_1          0x2899
#define S5_VPP_SLICE1_VD1_MATRIX_OFFSET2            0x289a
#define S5_VPP_SLICE1_VD1_MATRIX_PRE_OFFSET0_1      0x289b
#define S5_VPP_SLICE1_VD1_MATRIX_PRE_OFFSET2        0x289c
#define S5_VPP_SLICE1_VD1_MATRIX_EN_CTRL            0x289d
#define S5_VPP_SLICE1_VD1_MATRIX_SAT                0x289e

//#define GAMMA_CNTL_PORT                            0x1400
#define  GAMMA_VCOM_POL    7     /* RW */
#define  GAMMA_RVS_OUT     6     /* RW */
#define  ADR_RDY           5     /* Read Only */
#define  WR_RDY            4     /* Read Only */
#define  RD_RDY            3     /* Read Only */
#define  GAMMA_TR          2     /* RW */
#define  GAMMA_SET         1     /* RW */
#define  GAMMA_EN          0     /* RW */

//#define GAMMA_DATA_PORT                            0x1401
//#define GAMMA_ADDR_PORT                            0x1402
#define  H_RD              12
#define  H_AUTO_INC        11
#define  H_SEL_R           10
#define  H_SEL_G           9
#define  H_SEL_B           8
#define  HADR_MSB          7            /* 7:0 */
#define  HADR              0            /* 7:0 */

/* ********************************
 * register access api
 * ********************************* */

static inline unsigned int vpp_reg_read(unsigned int _reg)
{
	unsigned int val;

	if (_reg > 0x10000)
		val = *(volatile unsigned int *)REG_VPP_ADDR(_reg);
	else
		val = *(volatile unsigned int *)REG_ADDR_VCBUS(_reg);
	return val;
};

static inline void vpp_reg_write(unsigned int _reg, unsigned int _value)
{
	if (_reg > 0x10000)
		*(volatile unsigned int *)REG_VPP_ADDR(_reg) = (_value);
	else
		*(volatile unsigned int *)REG_ADDR_VCBUS(_reg) = (_value);
};

static inline void vpp_reg_setb(unsigned int _reg, unsigned int _value,
		unsigned int _start, unsigned int _len)
{
	vpp_reg_write(_reg, ((vpp_reg_read(_reg) &
			~(((1L << (_len))-1) << (_start))) |
			(((_value)&((1L<<(_len))-1)) << (_start))));
}

static inline unsigned int vpp_reg_getb(unsigned int _reg,
		unsigned int _start, unsigned int _len)
{
	return (vpp_reg_read(_reg) >> (_start)) & ((1L << (_len)) - 1);
}

#endif
