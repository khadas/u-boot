/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __VPU_DUMMY_REG_H__
#define __VPU_DUMMY_REG_H__
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>

#ifndef REG_BASE_CBUS
#define REG_BASE_CBUS                              (0xffd00000L)
#endif

#ifndef REG_BASE_VCBUS
#define REG_BASE_VCBUS                             (0xff900000L)
#endif

#ifndef HHI_GP_PLL_CNTL
#define HHI_GP_PLL_CNTL                            (0xc1100000 + (0x1010 << 2))
#endif

#ifndef HHI_GP1_PLL_CNTL
#define HHI_GP1_PLL_CNTL                           (0xc883c000 + (0x16 << 2))
#endif
#ifndef HHI_GP1_PLL_CNTL2
#define HHI_GP1_PLL_CNTL2                          (0xc883c000 + (0x17 << 2))
#endif
#ifndef HHI_GP1_PLL_CNTL3
#define HHI_GP1_PLL_CNTL3                          (0xc883c000 + (0x18 << 2))
#endif
#ifndef HHI_GP1_PLL_CNTL4
#define HHI_GP1_PLL_CNTL4                          (0xc883c000 + (0x19 << 2))
#endif

#ifndef HHI_VPU_MEM_PD_REG2
#define HHI_VPU_MEM_PD_REG2                        (0xff63c000 + (0x4d << 2))
#endif
#ifndef HHI_VPU_MEM_PD_REG3
#define HHI_VPU_MEM_PD_REG3                        (0xff63c000 + (0x4e << 2))
#endif
#ifndef HHI_VPU_MEM_PD_REG4
#define HHI_VPU_MEM_PD_REG4                        (0xff63c000 + (0x4c << 2))
#endif

#ifndef VENC_VDAC_TST_VAL
#define VENC_VDAC_TST_VAL                          (0x1b7f)
#endif
#ifndef VPP_DUMMY_DATA
#define VPP_DUMMY_DATA                             (0x1d00)
#endif
#ifndef VPU_VPU_PWM_V0
#define VPU_VPU_PWM_V0                             (0x2730)
#endif

#ifndef VPU_VOUT_BLEND_DUMDATA
#define VPU_VOUT_BLEND_DUMDATA                     ((0x0011  << 2) + 0xfe350000)
#endif
#ifndef VPP_VD1_MATRIX_OFFSET0_1
#define VPP_VD1_MATRIX_OFFSET0_1                   ((0x0289  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_DTH_DATA
#define VPU_VOUT_DTH_DATA                          ((0x0103  << 2) + 0xfe350000)
#endif

#ifndef VPU_RDARB_MODE_L2C1
#define VPU_RDARB_MODE_L2C1                        ((0x279d  << 2) + 0xff000000)
#endif
#ifndef VPU_WRARB_MODE_L2C1
#define VPU_WRARB_MODE_L2C1                        ((0x27a2  << 2) + 0xff000000)
#endif

#ifndef HHI_VPU_CLK_CNTL
#define HHI_VPU_CLK_CNTL                           (0xc883c000 + (0x6f << 2))
#endif

#ifndef HHI_VAPBCLK_CNTL
#define HHI_VAPBCLK_CNTL                           (0xc883c000 + (0x7d << 2))
#endif

#ifndef HHI_VID_CLK_CNTL2
#define HHI_VID_CLK_CNTL2                          (0xff63c000 + (0x065 << 2))
#endif

#ifndef AO_RTI_GEN_PWR_SLEEP0
#define AO_RTI_GEN_PWR_SLEEP0                      (0xff800000 + (0x03a << 2))
#endif

#ifndef AO_RTI_GEN_PWR_ISO0
#define AO_RTI_GEN_PWR_ISO0                        (0xff800000 + (0x03b << 2))
#endif

#ifndef HHI_MEM_PD_REG0
#define HHI_MEM_PD_REG0                            (0xff63c000 + (0x040 << 2))
#endif

#ifndef HHI_VPU_MEM_PD_REG0
#define HHI_VPU_MEM_PD_REG0                        (0xff63c000 + (0x041 << 2))
#endif

#ifndef HHI_VPU_MEM_PD_REG1
#define HHI_VPU_MEM_PD_REG1                        (0xff63c000 + (0x042 << 2))
#endif

#ifndef HHI_VPU_MEM_PD_REG2
#define HHI_VPU_MEM_PD_REG2                        (0xff63c000 + (0x04d << 2))
#endif

#ifndef HHI_VPU_MEM_PD_REG3
#define HHI_VPU_MEM_PD_REG3                        (0xff63c000 + (0x043 << 2))
#endif

#ifndef HHI_VPU_MEM_PD_REG4
#define HHI_VPU_MEM_PD_REG4                        (0xff63c000 + (0x044 << 2))
#endif

#ifndef HHI_MEM_PD_REG0
#define HHI_MEM_PD_REG0                            (0xff63c000 + (0x040 << 2))
#endif

#ifndef RESET0_LEVEL
#define RESET0_LEVEL                               (0x0420)
#endif
#ifndef RESET1_LEVEL
#define RESET1_LEVEL                               (0x0421)
#endif
#ifndef RESET2_LEVEL
#define RESET2_LEVEL                               (0x0422)
#endif
#ifndef RESET4_LEVEL
#define RESET4_LEVEL                               (0x0424)
#endif
#ifndef RESET7_LEVEL
#define RESET7_LEVEL                               (0x0427)
#endif

/* new mmap */
#ifndef CLKCTRL_VPU_CLK_CTRL
#define CLKCTRL_VPU_CLK_CTRL                       ((0x003a  << 2) + 0xfe000000)
#endif

#ifndef CLKCTRL_VAPBCLK_CTRL
#define CLKCTRL_VAPBCLK_CTRL                       ((0x003f  << 2) + 0xfe000000)
#endif

#ifndef CLKCTRL_VID_CLK_CTRL2
#define CLKCTRL_VID_CLK_CTRL2                      ((0x0031  << 2) + 0xfe000000)
#endif

#ifndef CLKCTRL_VID_CLK0_CTRL2
#define CLKCTRL_VID_CLK0_CTRL2                     ((0x0031  << 2) + 0xfe000000)
#endif

#ifndef CLKCTRL_VOUTENC_CLK_CTRL
#define CLKCTRL_VOUTENC_CLK_CTRL                   ((0x0046  << 2) + 0xfe000000)
#endif

#ifndef PWRCTRL_PWR_ACK0
#define PWRCTRL_PWR_ACK0                           ((0x0000  << 2) + 0xfe00c000)
#endif

#ifndef PWRCTRL_PWR_OFF0
#define PWRCTRL_PWR_OFF0                           ((0x0004  << 2) + 0xfe00c000)
#endif

#ifndef PWRCTRL_ISO_EN0
#define PWRCTRL_ISO_EN0                            ((0x0008  << 2) + 0xfe00c000)
#endif

#ifndef PWRCTRL_FOCRST0
#define PWRCTRL_FOCRST0                            ((0x000c  << 2) + 0xfe00c000)
#endif

#ifndef PWRCTRL_MEM_PD5
#define PWRCTRL_MEM_PD5                            ((0x0015  << 2) + 0xfe00c000)
#endif

#ifndef PWRCTRL_MEM_PD6
#define PWRCTRL_MEM_PD6                            ((0x0016  << 2) + 0xfe00c000)
#endif

#ifndef PWRCTRL_MEM_PD7
#define PWRCTRL_MEM_PD7                            ((0x0017  << 2) + 0xfe00c000)
#endif

#ifndef PWRCTRL_MEM_PD8
#define PWRCTRL_MEM_PD8                            ((0x0018  << 2) + 0xfe00c000)
#endif

#ifndef PWRCTRL_MEM_PD9
#define PWRCTRL_MEM_PD9                            ((0x0019  << 2) + 0xfe00c000)
#endif

#ifndef PWRCTRL_MEM_PD11
#define PWRCTRL_MEM_PD11                           ((0x001b  << 2) + 0xfe00c000)
#endif

#ifndef VPU_RDARB_MODE_L1C1
#define VPU_RDARB_MODE_L1C1                        ((0x2790  << 2) + 0xff000000)
#endif

#ifndef VPU_RDARB_MODE_L1C2
#define VPU_RDARB_MODE_L1C2                        ((0x2799  << 2) + 0xff000000)
#endif

#ifndef CLKCTRL_VPU_CLKB_CTRL
#define CLKCTRL_VPU_CLKB_CTRL 0
#endif

#endif
