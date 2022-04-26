/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __CVBS_DUMMY_REG_H__
#define __CVBS_DUMMY_REG_H__

#ifndef REG_BASE_VCBUS
#define REG_BASE_VCBUS                             (0xff900000L)
#endif

#ifndef HHI_VDAC_CNTL0
#define HHI_VDAC_CNTL0                             (0xff63c000 + (0x0bb << 2))
#endif
#ifndef HHI_VDAC_CNTL1
#define HHI_VDAC_CNTL1                             (0xff63c000 + (0x0bc << 2))
#endif

#ifndef ANACTRL_VDAC_CTRL0
#define ANACTRL_VDAC_CTRL0                         ((0x00b0  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_VDAC_CTRL1
#define ANACTRL_VDAC_CTRL1                         ((0x00b1  << 2) + 0xfe008000)
#endif

#ifndef HHI_GP0_PLL_CNTL0
#define HHI_GP0_PLL_CNTL0                          (0xff63c000 + (0x010 << 2))
#endif
#ifndef HHI_GP0_PLL_CNTL1
#define HHI_GP0_PLL_CNTL1                          (0xff63c000 + (0x11 << 2))
#endif
#ifndef HHI_GP0_PLL_CNTL2
#define HHI_GP0_PLL_CNTL2                          (0xff63c000 + (0x12 << 2))
#endif
#ifndef HHI_GP0_PLL_CNTL3
#define HHI_GP0_PLL_CNTL3                          (0xff63c000 + (0x13 << 2))
#endif
#ifndef HHI_GP0_PLL_CNTL4
#define HHI_GP0_PLL_CNTL4                          (0xff63c000 + (0x14 << 2))
#endif
#ifndef HHI_GP0_PLL_CNTL5
#define HHI_GP0_PLL_CNTL5                          (0xff63c000 + (0x15 << 2))
#endif
#ifndef HHI_GP0_PLL_CNTL6
#define HHI_GP0_PLL_CNTL6                          (0xff63c000 + (0x016 << 2))
#endif

#ifndef HHI_HDMI_PLL_CNTL
#define HHI_HDMI_PLL_CNTL                          (0xff63c000 + (0xc8 << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL0
#define HHI_HDMI_PLL_CNTL0                         (0xff63c000 + (0xc8 << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL1
#define HHI_HDMI_PLL_CNTL1                         (0xff63c000 + (0xc9 << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL2
#define HHI_HDMI_PLL_CNTL2                         (0xff63c000 + (0xca << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL3
#define HHI_HDMI_PLL_CNTL3                         (0xff63c000 + (0xcb << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL4
#define HHI_HDMI_PLL_CNTL4                         (0xff63c000 + (0xcd << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL5
#define HHI_HDMI_PLL_CNTL5                         (0xff63c000 + (0xcd << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL6
#define HHI_HDMI_PLL_CNTL6                         (0xff63c000 + (0xce << 2))
#endif

#ifndef HHI_VID_PLL_CLK_DIV
#define HHI_VID_PLL_CLK_DIV                        (0xff63c000 + (0x068 << 2))
#endif

#ifndef HHI_VIID_CLK_DIV
#define HHI_VIID_CLK_DIV                           (0xff63c000 + (0x04a << 2))
#endif

#ifndef HHI_VIID_CLK_CNTL
#define HHI_VIID_CLK_CNTL                          (0xff63c000 + (0x04b << 2))
#endif

#ifndef HHI_VID_CLK_DIV
#define HHI_VID_CLK_DIV                            (0xff63c000 + (0x059 << 2))
#endif

#ifndef HHI_VID_CLK_CNTL2
#define HHI_VID_CLK_CNTL2                          (0xff63c000 + (0x065 << 2))
#endif

#ifndef HHI_VID_CLK_CNTL
#define HHI_VID_CLK_CNTL                           (0xff63c000 + (0x05f << 2))
#endif

#ifndef ANACTRL_HDMIPLL_CTRL0
#define ANACTRL_HDMIPLL_CTRL0                      ((0x0070  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_HDMIPLL_CTRL1
#define ANACTRL_HDMIPLL_CTRL1                      ((0x0071  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_HDMIPLL_CTRL2
#define ANACTRL_HDMIPLL_CTRL2                      ((0x0072  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_HDMIPLL_CTRL3
#define ANACTRL_HDMIPLL_CTRL3                      ((0x0073  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_HDMIPLL_CTRL4
#define ANACTRL_HDMIPLL_CTRL4                      ((0x0074  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_HDMIPLL_CTRL5
#define ANACTRL_HDMIPLL_CTRL5                      ((0x0075  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_HDMIPLL_CTRL6
#define ANACTRL_HDMIPLL_CTRL6                      ((0x0076  << 2) + 0xfe008000)
#endif

#ifndef CLKCTRL_VID_CLK_CTRL
#define CLKCTRL_VID_CLK_CTRL                       ((0x0030  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VID_CLK_CTRL2
#define CLKCTRL_VID_CLK_CTRL2                      ((0x0031  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VID_CLK_DIV
#define CLKCTRL_VID_CLK_DIV                        ((0x0032  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VIID_CLK_DIV
#define CLKCTRL_VIID_CLK_DIV                       ((0x0033  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VIID_CLK_CTRL
#define CLKCTRL_VIID_CLK_CTRL                      ((0x0034  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VID_PLL_CLK_DIV
#define CLKCTRL_VID_PLL_CLK_DIV                    ((0x0039  << 2) + 0xfe000000)
#endif

#endif