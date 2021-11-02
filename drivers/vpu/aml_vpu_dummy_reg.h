/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/vpu/aml_vpu_dummy_reg.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __VPU_DUMMY_REG_H__
#define __VPU_DUMMY_REG_H__
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>

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

#ifndef DOLBY_TV_CLKGATE_CTRL
#define DOLBY_TV_CLKGATE_CTRL                      (0x33f1)
#endif
#ifndef DOLBY_CORE1_CLKGATE_CTRL
#define DOLBY_CORE1_CLKGATE_CTRL                   (0x33f2)
#endif
#ifndef DOLBY_CORE2A_CLKGATE_CTRL
#define DOLBY_CORE2A_CLKGATE_CTRL                  (0x3432)
#endif
#ifndef DOLBY_CORE3_CLKGATE_CTRL
#define DOLBY_CORE3_CLKGATE_CTRL                   (0x36f0)
#endif
#ifndef DOLBY_TM2_TV_CLKGATE_CTRL
#define DOLBY_TM2_TV_CLKGATE_CTRL                  (0x43f1)
#endif
#ifndef DOLBY_CORE1_1_CLKGATE_CTRL
#define DOLBY_CORE1_1_CLKGATE_CTRL                 (0x44f2)
#endif

#ifndef VENC_VDAC_TST_VAL
#define VENC_VDAC_TST_VAL                          (0x1b7f)
#endif
#ifndef ENCP_DVI_HSO_BEGIN
#define ENCP_DVI_HSO_BEGIN                         (0x1c30)
#endif
#ifndef VPP_DUMMY_DATA
#define VPP_DUMMY_DATA                             (0x1d00)
#endif
#ifndef VPU_VPU_PWM_V0
#define VPU_VPU_PWM_V0                             (0x2730)
#endif

#ifndef VPU_RDARB_MODE_L1C1
#define VPU_RDARB_MODE_L1C1                        (0x2790)
#endif
#ifndef VPU_RDARB_MODE_L1C2
#define VPU_RDARB_MODE_L1C2                        (0x2799)
#endif

#ifndef HHI_VPU_CLK_CNTL
#define HHI_VPU_CLK_CNTL                           (0xc883c000 + (0x6f << 2))
#endif

#ifndef HHI_VPU_CLK_CTRL
#define HHI_VPU_CLK_CTRL                           (0xc883c000 + (0x6f << 2))
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

#ifndef HHI_VPU_CLKB_CNTL
#define HHI_VPU_CLKB_CNTL			   (0xff63c000 + (0x083 << 2))
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

#endif
