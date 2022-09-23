/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef MIPI_DSI_REGISTER
#define REG_BASE_DSI_HOST                          (0xFFD00000L)

/* ********  mipi_dsi_host = 0xffD06000(0xffD06000 - 0xffD06fff) *********/
#define MIPI_DSI_DWC_VERSION_OS                (0x1800)
#define MIPI_DSI_DWC_PWR_UP_OS                 (0x1801)
#define MIPI_DSI_DWC_CLKMGR_CFG_OS             (0x1802)
#define MIPI_DSI_DWC_DPI_VCID_OS               (0x1803)
#define MIPI_DSI_DWC_DPI_COLOR_CODING_OS       (0x1804)
#define MIPI_DSI_DWC_DPI_CFG_POL_OS            (0x1805)
#define MIPI_DSI_DWC_DPI_LP_CMD_TIM_OS         (0x1806)
#define MIPI_DSI_DWC_PCKHDL_CFG_OS             (0x180b)
#define MIPI_DSI_DWC_GEN_VCID_OS               (0x180c)
#define MIPI_DSI_DWC_MODE_CFG_OS               (0x180d)
#define MIPI_DSI_DWC_VID_MODE_CFG_OS           (0x180e)
#define MIPI_DSI_DWC_VID_PKT_SIZE_OS           (0x180f)
#define MIPI_DSI_DWC_VID_NUM_CHUNKS_OS         (0x1810)
#define MIPI_DSI_DWC_VID_NULL_SIZE_OS          (0x1811)
#define MIPI_DSI_DWC_VID_HSA_TIME_OS           (0x1812)
#define MIPI_DSI_DWC_VID_HBP_TIME_OS           (0x1813)
#define MIPI_DSI_DWC_VID_HLINE_TIME_OS         (0x1814)
#define MIPI_DSI_DWC_VID_VSA_LINES_OS          (0x1815)
#define MIPI_DSI_DWC_VID_VBP_LINES_OS          (0x1816)
#define MIPI_DSI_DWC_VID_VFP_LINES_OS          (0x1817)
#define MIPI_DSI_DWC_VID_VACTIVE_LINES_OS      (0x1818)
#define MIPI_DSI_DWC_EDPI_CMD_SIZE_OS          (0x1819)
#define MIPI_DSI_DWC_CMD_MODE_CFG_OS           (0x181a)
#define MIPI_DSI_DWC_GEN_HDR_OS                (0x181b)
#define MIPI_DSI_DWC_GEN_PLD_DATA_OS           (0x181c)
#define MIPI_DSI_DWC_CMD_PKT_STATUS_OS         (0x181d)
#define MIPI_DSI_DWC_TO_CNT_CFG_OS             (0x181e)
#define MIPI_DSI_DWC_HS_RD_TO_CNT_OS           (0x181f)
#define MIPI_DSI_DWC_LP_RD_TO_CNT_OS           (0x1820)
#define MIPI_DSI_DWC_HS_WR_TO_CNT_OS           (0x1821)
#define MIPI_DSI_DWC_LP_WR_TO_CNT_OS           (0x1822)
#define MIPI_DSI_DWC_BTA_TO_CNT_OS             (0x1823)
#define MIPI_DSI_DWC_SDF_3D_OS                 (0x1824)
#define MIPI_DSI_DWC_LPCLK_CTRL_OS             (0x1825)
#define MIPI_DSI_DWC_PHY_TMR_LPCLK_CFG_OS      (0x1826)
#define MIPI_DSI_DWC_PHY_TMR_CFG_OS            (0x1827)
#define MIPI_DSI_DWC_PHY_RSTZ_OS               (0x1828)
#define MIPI_DSI_DWC_PHY_IF_CFG_OS             (0x1829)
#define MIPI_DSI_DWC_PHY_ULPS_CTRL_OS          (0x182a)
#define MIPI_DSI_DWC_PHY_TX_TRIGGERS_OS        (0x182b)
#define MIPI_DSI_DWC_PHY_STATUS_OS             (0x182c)
#define MIPI_DSI_DWC_PHY_TST_CTRL0_OS          (0x182d)
#define MIPI_DSI_DWC_PHY_TST_CTRL1_OS          (0x182e)
#define MIPI_DSI_DWC_INT_ST0_OS                (0x182f)
#define MIPI_DSI_DWC_INT_ST1_OS                (0x1830)
#define MIPI_DSI_DWC_INT_MSK0_OS               (0x1831)
#define MIPI_DSI_DWC_INT_MSK1_OS               (0x1832)

#define MIPI_DSI_TOP_SW_RESET                  (0x18f0)
#define MIPI_DSI_TOP_CLK_CNTL                  (0x18f1)
#define MIPI_DSI_TOP_CNTL                      (0x18f2)
#define MIPI_DSI_TOP_SUSPEND_CNTL              (0x18f3)
#define MIPI_DSI_TOP_SUSPEND_LINE              (0x18f4)
#define MIPI_DSI_TOP_SUSPEND_PIX               (0x18f5)
#define MIPI_DSI_TOP_MEAS_CNTL                 (0x18f6)
#define MIPI_DSI_TOP_STAT                      (0x18f7)
#define MIPI_DSI_TOP_MEAS_STAT_TE0             (0x18f8)
#define MIPI_DSI_TOP_MEAS_STAT_TE1             (0x18f9)
#define MIPI_DSI_TOP_MEAS_STAT_VS0             (0x18fa)
#define MIPI_DSI_TOP_MEAS_STAT_VS1             (0x18fb)
#define MIPI_DSI_TOP_INTR_CNTL_STAT            (0x18fc)
#define MIPI_DSI_TOP_MEM_PD                    (0x18fd)

/*=============    MIPI_DSI_PHY   ********* */
#define MIPI_DSI_PHY_CTRL                                  (0xff640000 + (0x000 << 2))
#define MIPI_DSI_CHAN_CTRL                                 (0xff640000 + (0x001 << 2))
#define MIPI_DSI_CHAN_STS                                  (0xff640000 + (0x002 << 2))
#define MIPI_DSI_CLK_TIM                                   (0xff640000 + (0x003 << 2))
#define MIPI_DSI_HS_TIM                                    (0xff640000 + (0x004 << 2))
#define MIPI_DSI_LP_TIM                                    (0xff640000 + (0x005 << 2))
#define MIPI_DSI_ANA_UP_TIM                                (0xff640000 + (0x006 << 2))
#define MIPI_DSI_INIT_TIM                                  (0xff640000 + (0x007 << 2))
#define MIPI_DSI_WAKEUP_TIM                                (0xff640000 + (0x008 << 2))
#define MIPI_DSI_LPOK_TIM                                  (0xff640000 + (0x009 << 2))
#define MIPI_DSI_LP_WCHDOG                                 (0xff640000 + (0x00a << 2))
#define MIPI_DSI_ANA_CTRL                                  (0xff640000 + (0x00b << 2))
#define MIPI_DSI_CLK_TIM1                                  (0xff640000 + (0x00c << 2))
#define MIPI_DSI_TURN_WCHDOG                               (0xff640000 + (0x00d << 2))
#define MIPI_DSI_ULPS_CHECK                                (0xff640000 + (0x00e << 2))
#define MIPI_DSI_TEST_CTRL0                                (0xff640000 + (0x00f << 2))
#define MIPI_DSI_TEST_CTRL1                                (0xff640000 + (0x010 << 2))

#define HHI_MIPI_CNTL0                                     (0xff63c000 + (0x00 << 2))
#define HHI_MIPI_CNTL1                                     (0xff63c000 + (0x01 << 2))
#define HHI_MIPI_CNTL2                                     (0xff63c000 + (0x02 << 2))

#endif /* #ifndef MIPI_DSI_REGISTER */

#ifndef HHI_GP0_PLL_CNTL
#define HHI_GP0_PLL_CNTL                                   (0xff63c000 + (0x010 << 2))
#endif
#ifndef HHI_GP0_PLL_CNTL0
#define HHI_GP0_PLL_CNTL0                                  (0xff63c000 + (0x010 << 2))
#endif
#ifndef HHI_GP0_PLL_CNTL1
#define HHI_GP0_PLL_CNTL1                                  (0xff63c000 + (0x11 << 2))
#endif
#ifndef HHI_GP0_PLL_CNTL6
#define HHI_GP0_PLL_CNTL6                                  (0xff63c000 + (0x016 << 2))
#endif

#ifndef HHI_HDMI_PLL_CNTL
#define HHI_HDMI_PLL_CNTL                                  (0xff63c000 + (0xc8 << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL0
#define HHI_HDMI_PLL_CNTL0                                 (0xff63c000 + (0xc8 << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL1
#define HHI_HDMI_PLL_CNTL1                                 (0xff63c000 + (0xc9 << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL2
#define HHI_HDMI_PLL_CNTL2                                 (0xff63c000 + (0xca << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL3
#define HHI_HDMI_PLL_CNTL3                                 (0xff63c000 + (0xcb << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL4
#define HHI_HDMI_PLL_CNTL4                                 (0xff63c000 + (0xcd << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL5
#define HHI_HDMI_PLL_CNTL5                                 (0xff63c000 + (0xcd << 2))
#endif
#ifndef HHI_HDMI_PLL_CNTL6
#define HHI_HDMI_PLL_CNTL6                                 (0xff63c000 + (0xce << 2))
#endif

#ifndef HHI_MIPIDSI_PHY_CLK_CNTL
#define HHI_MIPIDSI_PHY_CLK_CNTL                           (0xff63c000 + (0x95 << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL0
#define HHI_DIF_CSI_PHY_CNTL0                              (0xff63c000 + (0xd8 << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL1
#define HHI_DIF_CSI_PHY_CNTL1                              (0xff63c000 + (0xd9 << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL2
#define HHI_DIF_CSI_PHY_CNTL2                              (0xff63c000 + (0xda << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL3
#define HHI_DIF_CSI_PHY_CNTL3                              (0xff63c000 + (0xdb << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL4
#define HHI_DIF_CSI_PHY_CNTL4                              (0xff63c000 + (0xdc << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL5
#define HHI_DIF_CSI_PHY_CNTL5                              (0xff63c000 + (0xdd << 2))
#endif
#ifndef HHI_DSI_LVDS_EDP_CNTL0
#define HHI_DSI_LVDS_EDP_CNTL0                             (0xff63c000 + (0xd1 << 2))
#endif
#ifndef HHI_DSI_LVDS_EDP_CNTL1
#define HHI_DSI_LVDS_EDP_CNTL1                             (0xff63c000 + (0xd2 << 2))
#endif
#ifndef HHI_CSI_PHY_CNTL1
#define HHI_CSI_PHY_CNTL1                                  (0xff63c000 + (0xd4 << 2))
#endif
#ifndef HHI_CSI_PHY_CNTL2
#define HHI_CSI_PHY_CNTL2                                  (0xff63c000 + (0xd5 << 2))
#endif
#ifndef HHI_CSI_PHY_CNTL3
#define HHI_CSI_PHY_CNTL3                                  (0xff63c000 + (0xd6 << 2))
#endif
#ifndef HHI_LVDS_TX_PHY_CNTL0
#define HHI_LVDS_TX_PHY_CNTL0                              (0xff63c000 + (0xde << 2))
#endif
#ifndef HHI_LVDS_TX_PHY_CNTL1
#define HHI_LVDS_TX_PHY_CNTL1                              (0xff63c000 + (0xdf << 2))
#endif

#ifndef HHI_DIF_CSI_PHY_CNTL10
#define HHI_DIF_CSI_PHY_CNTL10			(0xff63c000 + (0x8e << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL11
#define HHI_DIF_CSI_PHY_CNTL11			(0xff63c000 + (0x8f << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL12
#define HHI_DIF_CSI_PHY_CNTL12			(0xff63c000 + (0x90 << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL13
#define HHI_DIF_CSI_PHY_CNTL13			(0xff63c000 + (0x91 << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL14
#define HHI_DIF_CSI_PHY_CNTL14			(0xff63c000 + (0x92 << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL15
#define HHI_DIF_CSI_PHY_CNTL15			(0xff63c000 + (0x93 << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL16
#define HHI_DIF_CSI_PHY_CNTL16			(0xff63c000 + (0xde << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL6
#define HHI_DIF_CSI_PHY_CNTL6			(0xff63c000 + (0xea << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL7
#define HHI_DIF_CSI_PHY_CNTL7			(0xff63c000 + (0xeb << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL8
#define HHI_DIF_CSI_PHY_CNTL8			(0xff63c000 + (0xec << 2))
#endif
#ifndef HHI_DIF_CSI_PHY_CNTL9
#define HHI_DIF_CSI_PHY_CNTL9			(0xff63c000 + (0xed << 2))
#endif

#ifndef HHI_TCON_PLL_CNTL0
#define HHI_TCON_PLL_CNTL0			(0xff63c000 + (0x020 << 2))
#endif
#ifndef HHI_TCON_PLL_CNTL1
#define HHI_TCON_PLL_CNTL1			(0xff63c000 + (0x021 << 2))
#endif
#ifndef HHI_TCON_PLL_CNTL2
#define HHI_TCON_PLL_CNTL2			(0xff63c000 + (0x022 << 2))
#endif
#ifndef HHI_TCON_PLL_CNTL3
#define HHI_TCON_PLL_CNTL3			(0xff63c000 + (0x023 << 2))
#endif
#ifndef HHI_TCON_PLL_CNTL4
#define HHI_TCON_PLL_CNTL4			(0xff63c000 + (0x0df << 2))
#endif

#ifndef HHI_DIF_TCON_CNTL0
#define HHI_DIF_TCON_CNTL0                         (0xff63c000 + (0x3c << 2))
#endif
#ifndef HHI_DIF_TCON_CNTL1
#define HHI_DIF_TCON_CNTL1                         (0xff63c000 + (0x3d << 2))
#endif
#ifndef HHI_DIF_TCON_CNTL2
#define HHI_DIF_TCON_CNTL2                         (0xff63c000 + (0x3e << 2))
#endif
#ifndef HHI_TCON_CLK_CNTL
#define HHI_TCON_CLK_CNTL                          (0xff63c000 + (0xf0 << 2))
#endif

#ifndef ANACTRL_TCON_PLL0_CNTL0
#define ANACTRL_TCON_PLL0_CNTL0			((0x00e0  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL0_CNTL1
#define ANACTRL_TCON_PLL0_CNTL1			((0x00e1  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL0_CNTL2
#define ANACTRL_TCON_PLL0_CNTL2			((0x00e2  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL0_CNTL3
#define ANACTRL_TCON_PLL0_CNTL3			((0x00e3  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL0_CNTL4
#define ANACTRL_TCON_PLL0_CNTL4			((0x00e4  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL1_CNTL0
#define ANACTRL_TCON_PLL1_CNTL0			((0x00e5  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL1_CNTL1
#define ANACTRL_TCON_PLL1_CNTL1			((0x00e6  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL1_CNTL2
#define ANACTRL_TCON_PLL1_CNTL2			((0x00e7  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL1_CNTL3
#define ANACTRL_TCON_PLL1_CNTL3			((0x00e8  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL1_CNTL4
#define ANACTRL_TCON_PLL1_CNTL4			((0x00e9  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL2_CNTL0
#define ANACTRL_TCON_PLL2_CNTL0			((0x00ea  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL2_CNTL1
#define ANACTRL_TCON_PLL2_CNTL1			((0x00eb  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL2_CNTL2
#define ANACTRL_TCON_PLL2_CNTL2			((0x00ec  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL2_CNTL3
#define ANACTRL_TCON_PLL2_CNTL3			((0x00ed  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL2_CNTL4
#define ANACTRL_TCON_PLL2_CNTL4			((0x00ee  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL0_STS
#define ANACTRL_TCON_PLL0_STS			((0x00ef  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL1_STS
#define ANACTRL_TCON_PLL1_STS			((0x00f0  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_TCON_PLL2_STS
#define ANACTRL_TCON_PLL2_STS			((0x00f1  << 2) + 0xfe008000)
#endif

#ifndef ANACTRL_LVDS_TX_PHY_CNTL0
#define ANACTRL_LVDS_TX_PHY_CNTL0                  ((0x00f4  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_LVDS_TX_PHY_CNTL1
#define ANACTRL_LVDS_TX_PHY_CNTL1                  ((0x00f5  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_LVDS_TX_PHY_CNTL2
#define ANACTRL_LVDS_TX_PHY_CNTL2                  ((0x00f6  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_LVDS_TX_PHY_CNTL3
#define ANACTRL_LVDS_TX_PHY_CNTL3                  ((0x00f7  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_VID_PLL_CLK_DIV
#define ANACTRL_VID_PLL_CLK_DIV                    ((0x00f8  << 2) + 0xfe008000)
#endif

#ifndef COMBO_DPHY_CNTL0
#define COMBO_DPHY_CNTL0			((0x0000  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_CNTL1
#define COMBO_DPHY_CNTL1			((0x0001  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_VID_PLL0_DIV
#define COMBO_DPHY_VID_PLL0_DIV			((0x0002  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_VID_PLL1_DIV
#define COMBO_DPHY_VID_PLL1_DIV			((0x0003  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_VID_PLL2_DIV
#define COMBO_DPHY_VID_PLL2_DIV			((0x0004  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_EDP_PIXEL_CLK_DIV
#define COMBO_DPHY_EDP_PIXEL_CLK_DIV		((0x0005  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0
#define COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0	((0x0006  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL0
#define COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL0	((0x0007  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0
#define COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0	((0x0008  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1
#define COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1	((0x0009  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL1
#define COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL1	((0x000a  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1
#define COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1	((0x000b  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_RO_EDP_LVDS_TX_PHY0_CNTL1
#define COMBO_DPHY_RO_EDP_LVDS_TX_PHY0_CNTL1	((0x0000  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_RO_EDP_LVDS_TX_PHY1_CNTL1
#define COMBO_DPHY_RO_EDP_LVDS_TX_PHY1_CNTL1	((0x0011  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_RO_EDP_LVDS_TX_PHY2_CNTL1
#define COMBO_DPHY_RO_EDP_LVDS_TX_PHY2_CNTL1	((0x0012  << 2) + 0xfe018000)
#endif

#ifndef COMBO_DPHY_RO_EDP_LVDS_TX_PHY0_CNTL1
#define COMBO_DPHY_RO_EDP_LVDS_TX_PHY0_CNTL1	((0x0000  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_RO_EDP_LVDS_TX_PHY1_CNTL1
#define COMBO_DPHY_RO_EDP_LVDS_TX_PHY1_CNTL1	((0x0011  << 2) + 0xfe018000)
#endif
#ifndef COMBO_DPHY_RO_EDP_LVDS_TX_PHY2_CNTL1
#define COMBO_DPHY_RO_EDP_LVDS_TX_PHY2_CNTL1	((0x0012  << 2) + 0xfe018000)
#endif

#ifndef ANACTRL_DIF_PHY_CNTL1
#define ANACTRL_DIF_PHY_CNTL1			((0x00c8  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL2
#define ANACTRL_DIF_PHY_CNTL2			((0x00c9  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL3
#define ANACTRL_DIF_PHY_CNTL3			((0x00ca  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL4
#define ANACTRL_DIF_PHY_CNTL4			((0x00cb  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL5
#define ANACTRL_DIF_PHY_CNTL5			((0x00cc  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL6
#define ANACTRL_DIF_PHY_CNTL6			((0x00cd  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL7
#define ANACTRL_DIF_PHY_CNTL7			((0x00ce  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL8
#define ANACTRL_DIF_PHY_CNTL8			((0x00cf  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL9
#define ANACTRL_DIF_PHY_CNTL9			((0x00d0  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL10
#define ANACTRL_DIF_PHY_CNTL10			((0x00d1  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL11
#define ANACTRL_DIF_PHY_CNTL11			((0x00d2  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL12
#define ANACTRL_DIF_PHY_CNTL12			((0x00d3  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL13
#define ANACTRL_DIF_PHY_CNTL13			((0x00d4  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL14
#define ANACTRL_DIF_PHY_CNTL14			((0x00d5  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL15
#define ANACTRL_DIF_PHY_CNTL15			((0x00d6  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL16
#define ANACTRL_DIF_PHY_CNTL16			((0x00d7  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL17
#define ANACTRL_DIF_PHY_CNTL17			((0x00d8  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL18
#define ANACTRL_DIF_PHY_CNTL18			((0x00d9  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL19
#define ANACTRL_DIF_PHY_CNTL19			((0x00da  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL20
#define ANACTRL_DIF_PHY_CNTL20			((0x00db  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_DIF_PHY_CNTL21
#define ANACTRL_DIF_PHY_CNTL21			((0x00dc  << 2) + 0xfe008000)
#endif

#ifndef CLKCTRL_VID_CLK0_DIV
#define CLKCTRL_VID_CLK0_DIV                       ((0x0032  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VID_PLL_CLK2_DIV
#define CLKCTRL_VID_PLL_CLK2_DIV                   ((0x007e  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VIID_CLK0_DIV
#define CLKCTRL_VIID_CLK0_DIV			((0x0033  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VIID_CLK0_CTRL
#define CLKCTRL_VIID_CLK0_CTRL			((0x0034  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VID_CLK0_CTRL2
#define CLKCTRL_VID_CLK0_CTRL2			((0x0031  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VIID_CLK1_DIV
#define CLKCTRL_VIID_CLK1_DIV			((0x0076  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VIID_CLK1_CTRL
#define CLKCTRL_VIID_CLK1_CTRL			((0x0077  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VID_CLK1_CTRL2
#define CLKCTRL_VID_CLK1_CTRL2			((0x0074  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VIID_CLK2_DIV
#define CLKCTRL_VIID_CLK2_DIV			((0x007b  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VIID_CLK2_CTRL
#define CLKCTRL_VIID_CLK2_CTRL			((0x007c  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VID_CLK2_CTRL2
#define CLKCTRL_VID_CLK2_CTRL2			((0x0079  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_VID_PLL_CLK0_DIV
#define CLKCTRL_VID_PLL_CLK0_DIV		((0x0039  << 2) + 0xfe000000)
#endif

#ifndef CLKCTRL_MIPIDSI_PHY_CLK_CTRL
#define CLKCTRL_MIPIDSI_PHY_CLK_CTRL		((0x0041  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL
#define CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL		((0x0080  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_HDMI_VID_PLL_CLK_DIV
#define CLKCTRL_HDMI_VID_PLL_CLK_DIV            ((0x0081  << 2) + 0xfe000000)
#endif

#ifndef CLKCTRL_TCON_CLK_CNTL
#define CLKCTRL_TCON_CLK_CNTL                      ((0x0087  << 2) + 0xfe000000)
#endif

#ifndef ANACTRL_GP0PLL_CTRL0
#define ANACTRL_GP0PLL_CTRL0                       ((0x0020  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_GP0PLL_CTRL1
#define ANACTRL_GP0PLL_CTRL1                       ((0x0021  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_GP0PLL_CTRL2
#define ANACTRL_GP0PLL_CTRL2                       ((0x0022  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_GP0PLL_CTRL3
#define ANACTRL_GP0PLL_CTRL3                       ((0x0023  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_GP0PLL_CTRL4
#define ANACTRL_GP0PLL_CTRL4                       ((0x0024  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_GP0PLL_CTRL5
#define ANACTRL_GP0PLL_CTRL5                       ((0x0025  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_GP0PLL_CTRL6
#define ANACTRL_GP0PLL_CTRL6                       ((0x0026  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_GP0PLL_STS
#define ANACTRL_GP0PLL_STS                         ((0x0027  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_MIPIDSI_CTRL0
#define ANACTRL_MIPIDSI_CTRL0                      ((0x00a0  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_MIPIDSI_CTRL1
#define ANACTRL_MIPIDSI_CTRL1                      ((0x00a1  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_MIPIDSI_CTRL2
#define ANACTRL_MIPIDSI_CTRL2                      ((0x00a2  << 2) + 0xfe008000)
#endif
#ifndef ANACTRL_MIPIDSI_STS
#define ANACTRL_MIPIDSI_STS                        ((0x00a3  << 2) + 0xfe008000)
#endif

#ifndef CLKCTRL_VOUTENC_CLK_CTRL
#define CLKCTRL_VOUTENC_CLK_CTRL                   ((0x0046  << 2) + 0xfe000000)
#endif

#ifndef ENCL_VIDEO_H_PRE_DE_END
#define ENCL_VIDEO_H_PRE_DE_END                    0x1ccf
#endif
#ifndef ENCL_VIDEO_H_PRE_DE_BEGIN
#define ENCL_VIDEO_H_PRE_DE_BEGIN                  0x1cd0
#endif
#ifndef ENCL_VIDEO_V_PRE_DE_ELINE
#define ENCL_VIDEO_V_PRE_DE_ELINE                  0x1cd1
#endif
#ifndef ENCL_VIDEO_V_PRE_DE_BLINE
#define ENCL_VIDEO_V_PRE_DE_BLINE                  0x1cd2
#endif
#ifndef ENCL_INBUF_CNTL0
#define ENCL_INBUF_CNTL0                           0x1cd3
#endif
#ifndef ENCL_INBUF_CNTL1
#define ENCL_INBUF_CNTL1                           0x1cd4
#endif
#ifndef ENCL_INBUF_CNT
#define ENCL_INBUF_CNT                             0x1cd5
#endif

#ifndef VPU_VENC_CTRL
#define VPU_VENC_CTRL                              ((0x1cef  << 2) + 0xff000000)
#endif
#ifndef VPU_DISP_VIU0_CTRL
#define VPU_DISP_VIU0_CTRL                         ((0x2786  << 2) + 0xff000000)
#endif
#ifndef VPU_DISP_VIU1_CTRL
#define VPU_DISP_VIU1_CTRL                         ((0x2787  << 2) + 0xff000000)
#endif
#ifndef VPU_DISP_VIU2_CTRL
#define VPU_DISP_VIU2_CTRL                         ((0x2788  << 2) + 0xff000000)
#endif
#ifndef VPU_VIU_VENC_MUX_CTRL
#define VPU_VIU_VENC_MUX_CTRL                      ((0x271a  << 2) + 0xff000000)
#endif
#ifndef ENCL_INFO_READ
#define ENCL_INFO_READ                             ((0x271f  << 2) + 0xff000000)
#endif

#ifndef VPU_VENCI_STAT
#define VPU_VENCI_STAT                             ((0x1ceb  << 2) + 0xff000000)
#endif
#ifndef VPU_VENCP_STAT
#define VPU_VENCP_STAT                             ((0x1cec  << 2) + 0xff000000)
#endif
#ifndef VPU_VENCL_STAT
#define VPU_VENCL_STAT                             ((0x1ced  << 2) + 0xff000000)
#endif

#ifndef LCD_RGB_BASE_ADDR
#define LCD_RGB_BASE_ADDR                          ((0x14a5  << 2) + 0xff000000)
#endif
#ifndef LCD_RGB_COEFF_ADDR
#define LCD_RGB_COEFF_ADDR                         ((0x14a6  << 2) + 0xff000000)
#endif
#ifndef LCD_POL_CNTL_ADDR
#define LCD_POL_CNTL_ADDR                          ((0x14a7  << 2) + 0xff000000)
#endif
#ifndef LCD_DITH_CNTL_ADDR
#define LCD_DITH_CNTL_ADDR                         ((0x14a8  << 2) + 0xff000000)
#endif

#ifndef VBO_INFILTER_CTRL
#define VBO_INFILTER_CTRL                          ((0x19f9  << 2) + 0xff000000)
#endif
#ifndef VBO_INFILTER_CTRL_H
#define VBO_INFILTER_CTRL_H                        ((0x1977  << 2) + 0xff000000)
#endif
#ifndef VBO_INFILTER_TICK_PERIOD_L
#define VBO_INFILTER_TICK_PERIOD_L                 0x14f9
#endif
#ifndef VBO_INFILTER_TICK_PERIOD_H
#define VBO_INFILTER_TICK_PERIOD_H                 0x1477
#endif

#ifndef LCD_GAMMA_CNTL_PORT0
#define LCD_GAMMA_CNTL_PORT0                       ((0x14b4  << 2) + 0xff000000)
#endif
#ifndef LCD_GAMMA_DATA_PORT0
#define LCD_GAMMA_DATA_PORT0                       ((0x14b5  << 2) + 0xff000000)
#endif
#ifndef LCD_GAMMA_ADDR_PORT0
#define LCD_GAMMA_ADDR_PORT0                       ((0x14b6  << 2) + 0xff000000)
#endif

#ifndef ENCL_TST_EN
#define ENCL_TST_EN                                ((0x1c98  << 2) + 0xff000000)
#endif
#ifndef ENCL_TST_MDSEL
#define ENCL_TST_MDSEL                             ((0x1c99  << 2) + 0xff000000)
#endif
#ifndef ENCL_TST_Y
#define ENCL_TST_Y                                 ((0x1c9a  << 2) + 0xff000000)
#endif
#ifndef ENCL_TST_CB
#define ENCL_TST_CB                                ((0x1c9b  << 2) + 0xff000000)
#endif
#ifndef ENCL_TST_CR
#define ENCL_TST_CR                                ((0x1c9c  << 2) + 0xff000000)
#endif
#ifndef ENCL_TST_CLRBAR_STRT
#define ENCL_TST_CLRBAR_STRT                       ((0x1c9d  << 2) + 0xff000000)
#endif
#ifndef ENCL_TST_CLRBAR_WIDTH
#define ENCL_TST_CLRBAR_WIDTH                      ((0x1c9e  << 2) + 0xff000000)
#endif
#ifndef ENCL_TST_VDCNT_STSET
#define ENCL_TST_VDCNT_STSET                       ((0x1c9f  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_EN
#define ENCL_VIDEO_EN                              ((0x1ca0  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_Y_SCL
#define ENCL_VIDEO_Y_SCL                           ((0x1ca1  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_PB_SCL
#define ENCL_VIDEO_PB_SCL                          ((0x1ca2  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_PR_SCL
#define ENCL_VIDEO_PR_SCL                          ((0x1ca3  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_Y_OFFST
#define ENCL_VIDEO_Y_OFFST                         ((0x1ca4  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_PB_OFFST
#define ENCL_VIDEO_PB_OFFST                        ((0x1ca5  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_PR_OFFST
#define ENCL_VIDEO_PR_OFFST                        ((0x1ca6  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_MODE
#define ENCL_VIDEO_MODE                            ((0x1ca7  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_MODE_ADV
#define ENCL_VIDEO_MODE_ADV                        ((0x1ca8  << 2) + 0xff000000)
#endif
#ifndef ENCL_DBG_PX_RST
#define ENCL_DBG_PX_RST                            ((0x1ca9  << 2) + 0xff000000)
#endif
#ifndef ENCL_DBG_LN_RST
#define ENCL_DBG_LN_RST                            ((0x1caa  << 2) + 0xff000000)
#endif
#ifndef ENCL_DBG_PX_INT
#define ENCL_DBG_PX_INT                            ((0x1cab  << 2) + 0xff000000)
#endif
#ifndef ENCL_DBG_LN_INT
#define ENCL_DBG_LN_INT                            ((0x1cac  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_YFP1_HTIME
#define ENCL_VIDEO_YFP1_HTIME                      ((0x1cad  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_YFP2_HTIME
#define ENCL_VIDEO_YFP2_HTIME                      ((0x1cae  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_YC_DLY
#define ENCL_VIDEO_YC_DLY                          ((0x1caf  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_MAX_PXCNT
#define ENCL_VIDEO_MAX_PXCNT                       ((0x1cb0  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_HAVON_END
#define ENCL_VIDEO_HAVON_END                       ((0x1cb1  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_HAVON_BEGIN
#define ENCL_VIDEO_HAVON_BEGIN                     ((0x1cb2  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_VAVON_ELINE
#define ENCL_VIDEO_VAVON_ELINE                     ((0x1cb3  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_VAVON_BLINE
#define ENCL_VIDEO_VAVON_BLINE                     ((0x1cb4  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_HSO_BEGIN
#define ENCL_VIDEO_HSO_BEGIN                       ((0x1cb5  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_HSO_END
#define ENCL_VIDEO_HSO_END                         ((0x1cb6  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_VSO_BEGIN
#define ENCL_VIDEO_VSO_BEGIN                       ((0x1cb7  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_VSO_END
#define ENCL_VIDEO_VSO_END                         ((0x1cb8  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_VSO_BLINE
#define ENCL_VIDEO_VSO_BLINE                       ((0x1cb9  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_VSO_ELINE
#define ENCL_VIDEO_VSO_ELINE                       ((0x1cba  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_MAX_LNCNT
#define ENCL_VIDEO_MAX_LNCNT                       ((0x1cbb  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_BLANKY_VAL
#define ENCL_VIDEO_BLANKY_VAL                      ((0x1cbc  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_BLANKPB_VAL
#define ENCL_VIDEO_BLANKPB_VAL                     ((0x1cbd  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_BLANKPR_VAL
#define ENCL_VIDEO_BLANKPR_VAL                     ((0x1cbe  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_HOFFST
#define ENCL_VIDEO_HOFFST                          ((0x1cbf  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_VOFFST
#define ENCL_VIDEO_VOFFST                          ((0x1cc0  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_RGB_CTRL
#define ENCL_VIDEO_RGB_CTRL                        ((0x1cc1  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_FILT_CTRL
#define ENCL_VIDEO_FILT_CTRL                       ((0x1cc2  << 2) + 0xff000000)
#endif
#ifndef ENCL_VIDEO_RGBIN_CTRL
#define ENCL_VIDEO_RGBIN_CTRL                      ((0x1cc7  << 2) + 0xff000000)
#endif
#ifndef ENCL_MAX_LINE_SWITCH_POINT
#define ENCL_MAX_LINE_SWITCH_POINT                 ((0x1cc8  << 2) + 0xff000000)
#endif
#ifndef ENCT_VIDEO_H_PRE_DE_END
#define ENCT_VIDEO_H_PRE_DE_END                    ((0x1ccb  << 2) + 0xff000000)
#endif
#ifndef ENCT_VIDEO_H_PRE_DE_BEGIN
#define ENCT_VIDEO_H_PRE_DE_BEGIN                  ((0x1ccc  << 2) + 0xff000000)
#endif
#ifndef ENCT_VIDEO_V_PRE_DE_ELINE
#define ENCT_VIDEO_V_PRE_DE_ELINE                  ((0x1ccd  << 2) + 0xff000000)
#endif
#ifndef ENCT_VIDEO_V_PRE_DE_BLINE
#define ENCT_VIDEO_V_PRE_DE_BLINE                  ((0x1cce  << 2) + 0xff000000)
#endif
#ifndef VPU_VPU_PWM_V0
#define VPU_VPU_PWM_V0                             ((0x1ce0  << 2) + 0xff000000)
#endif
#ifndef VPU_VPU_PWM_V1
#define VPU_VPU_PWM_V1                             ((0x1ce1  << 2) + 0xff000000)
#endif
#ifndef VPU_VPU_PWM_V2
#define VPU_VPU_PWM_V2                             ((0x1ce2  << 2) + 0xff000000)
#endif
#ifndef VPU_VPU_PWM_V3
#define VPU_VPU_PWM_V3                             ((0x1ce3  << 2) + 0xff000000)
#endif
#ifndef VPU_VPU_PWM_H0
#define VPU_VPU_PWM_H0                             ((0x1ce4  << 2) + 0xff000000)
#endif
#ifndef VPU_VPU_PWM_H1
#define VPU_VPU_PWM_H1                             ((0x1ce5  << 2) + 0xff000000)
#endif
#ifndef VPU_VPU_PWM_H2
#define VPU_VPU_PWM_H2                             ((0x1ce6  << 2) + 0xff000000)
#endif
#ifndef VPU_VPU_PWM_H3
#define VPU_VPU_PWM_H3                             ((0x1ce7  << 2) + 0xff000000)
#endif

#ifndef L_GAMMA_CNTL_PORT
#define L_GAMMA_CNTL_PORT                          ((0x1900  << 2) + 0xff000000)
#endif
#ifndef L_GAMMA_DATA_PORT
#define L_GAMMA_DATA_PORT                          ((0x1901  << 2) + 0xff000000)
#endif
#ifndef L_GAMMA_ADDR_PORT
#define L_GAMMA_ADDR_PORT                          ((0x1902  << 2) + 0xff000000)
#endif
#ifndef L_GAMMA_VCOM_HSWITCH_ADDR
#define L_GAMMA_VCOM_HSWITCH_ADDR                  ((0x1903  << 2) + 0xff000000)
#endif
#ifndef L_RGB_BASE_ADDR
#define L_RGB_BASE_ADDR                            ((0x1905  << 2) + 0xff000000)
#endif
#ifndef L_RGB_COEFF_ADDR
#define L_RGB_COEFF_ADDR                           ((0x1906  << 2) + 0xff000000)
#endif
#ifndef L_POL_CNTL_ADDR
#define L_POL_CNTL_ADDR                            ((0x1907  << 2) + 0xff000000)
#endif
#ifndef L_DITH_CNTL_ADDR
#define L_DITH_CNTL_ADDR                           ((0x1908  << 2) + 0xff000000)
#endif
#ifndef L_STH1_HS_ADDR
#define L_STH1_HS_ADDR                             ((0x1910  << 2) + 0xff000000)
#endif
#ifndef L_STH1_HE_ADDR
#define L_STH1_HE_ADDR                             ((0x1911  << 2) + 0xff000000)
#endif
#ifndef L_STH1_VS_ADDR
#define L_STH1_VS_ADDR                             ((0x1912  << 2) + 0xff000000)
#endif
#ifndef L_STH1_VE_ADDR
#define L_STH1_VE_ADDR                             ((0x1913  << 2) + 0xff000000)
#endif
#ifndef L_STH2_HS_ADDR
#define L_STH2_HS_ADDR                             ((0x1914  << 2) + 0xff000000)
#endif
#ifndef L_STH2_HE_ADDR
#define L_STH2_HE_ADDR                             ((0x1915  << 2) + 0xff000000)
#endif
#ifndef L_STH2_VS_ADDR
#define L_STH2_VS_ADDR                             ((0x1916  << 2) + 0xff000000)
#endif
#ifndef L_STH2_VE_ADDR
#define L_STH2_VE_ADDR                             ((0x1917  << 2) + 0xff000000)
#endif
#ifndef L_OEH_HS_ADDR
#define L_OEH_HS_ADDR                              ((0x1918  << 2) + 0xff000000)
#endif
#ifndef L_OEH_HE_ADDR
#define L_OEH_HE_ADDR                              ((0x1919  << 2) + 0xff000000)
#endif
#ifndef L_OEH_VS_ADDR
#define L_OEH_VS_ADDR                              ((0x191a  << 2) + 0xff000000)
#endif
#ifndef L_OEH_VE_ADDR
#define L_OEH_VE_ADDR                              ((0x191b  << 2) + 0xff000000)
#endif
#ifndef L_VCOM_HSWITCH_ADDR
#define L_VCOM_HSWITCH_ADDR                        ((0x191c  << 2) + 0xff000000)
#endif
#ifndef L_VCOM_VS_ADDR
#define L_VCOM_VS_ADDR                             ((0x191d  << 2) + 0xff000000)
#endif
#ifndef L_VCOM_VE_ADDR
#define L_VCOM_VE_ADDR                             ((0x191e  << 2) + 0xff000000)
#endif
#ifndef L_CPV1_HS_ADDR
#define L_CPV1_HS_ADDR                             ((0x191f  << 2) + 0xff000000)
#endif
#ifndef L_CPV1_HE_ADDR
#define L_CPV1_HE_ADDR                             ((0x1920  << 2) + 0xff000000)
#endif
#ifndef L_CPV1_VS_ADDR
#define L_CPV1_VS_ADDR                             ((0x1921  << 2) + 0xff000000)
#endif
#ifndef L_CPV1_VE_ADDR
#define L_CPV1_VE_ADDR                             ((0x1922  << 2) + 0xff000000)
#endif
#ifndef L_CPV2_HS_ADDR
#define L_CPV2_HS_ADDR                             ((0x1923  << 2) + 0xff000000)
#endif
#ifndef L_CPV2_HE_ADDR
#define L_CPV2_HE_ADDR                             ((0x1924  << 2) + 0xff000000)
#endif
#ifndef L_CPV2_VS_ADDR
#define L_CPV2_VS_ADDR                             ((0x1925  << 2) + 0xff000000)
#endif
#ifndef L_CPV2_VE_ADDR
#define L_CPV2_VE_ADDR                             ((0x1926  << 2) + 0xff000000)
#endif
#ifndef L_STV1_HS_ADDR
#define L_STV1_HS_ADDR                             ((0x1927  << 2) + 0xff000000)
#endif
#ifndef L_STV1_HE_ADDR
#define L_STV1_HE_ADDR                             ((0x1928  << 2) + 0xff000000)
#endif
#ifndef L_STV1_VS_ADDR
#define L_STV1_VS_ADDR                             ((0x1929  << 2) + 0xff000000)
#endif
#ifndef L_STV1_VE_ADDR
#define L_STV1_VE_ADDR                             ((0x192a  << 2) + 0xff000000)
#endif
#ifndef L_STV2_HS_ADDR
#define L_STV2_HS_ADDR                             ((0x192b  << 2) + 0xff000000)
#endif
#ifndef L_STV2_HE_ADDR
#define L_STV2_HE_ADDR                             ((0x192c  << 2) + 0xff000000)
#endif
#ifndef L_STV2_VS_ADDR
#define L_STV2_VS_ADDR                             ((0x192d  << 2) + 0xff000000)
#endif
#ifndef L_STV2_VE_ADDR
#define L_STV2_VE_ADDR                             ((0x192e  << 2) + 0xff000000)
#endif
#ifndef L_OEV1_HS_ADDR
#define L_OEV1_HS_ADDR                             ((0x192f  << 2) + 0xff000000)
#endif
#ifndef L_OEV1_HE_ADDR
#define L_OEV1_HE_ADDR                             ((0x1930  << 2) + 0xff000000)
#endif
#ifndef L_OEV1_VS_ADDR
#define L_OEV1_VS_ADDR                             ((0x1931  << 2) + 0xff000000)
#endif
#ifndef L_OEV1_VE_ADDR
#define L_OEV1_VE_ADDR                             ((0x1932  << 2) + 0xff000000)
#endif
#ifndef L_OEV2_HS_ADDR
#define L_OEV2_HS_ADDR                             ((0x1933  << 2) + 0xff000000)
#endif
#ifndef L_OEV2_HE_ADDR
#define L_OEV2_HE_ADDR                             ((0x1934  << 2) + 0xff000000)
#endif
#ifndef L_OEV2_VS_ADDR
#define L_OEV2_VS_ADDR                             ((0x1935  << 2) + 0xff000000)
#endif
#ifndef L_OEV2_VE_ADDR
#define L_OEV2_VE_ADDR                             ((0x1936  << 2) + 0xff000000)
#endif
#ifndef L_OEV3_HS_ADDR
#define L_OEV3_HS_ADDR                             ((0x1937  << 2) + 0xff000000)
#endif
#ifndef L_OEV3_HE_ADDR
#define L_OEV3_HE_ADDR                             ((0x1938  << 2) + 0xff000000)
#endif
#ifndef L_OEV3_VS_ADDR
#define L_OEV3_VS_ADDR                             ((0x1939  << 2) + 0xff000000)
#endif
#ifndef L_OEV3_VE_ADDR
#define L_OEV3_VE_ADDR                             ((0x193a  << 2) + 0xff000000)
#endif
#ifndef L_LCD_PWR_ADDR
#define L_LCD_PWR_ADDR                             ((0x193b  << 2) + 0xff000000)
#endif
#ifndef L_LCD_PWM0_LO_ADDR
#define L_LCD_PWM0_LO_ADDR                         ((0x193c  << 2) + 0xff000000)
#endif
#ifndef L_LCD_PWM0_HI_ADDR
#define L_LCD_PWM0_HI_ADDR                         ((0x193d  << 2) + 0xff000000)
#endif
#ifndef L_LCD_PWM1_LO_ADDR
#define L_LCD_PWM1_LO_ADDR                         ((0x193e  << 2) + 0xff000000)
#endif
#ifndef L_LCD_PWM1_HI_ADDR
#define L_LCD_PWM1_HI_ADDR                         ((0x193f  << 2) + 0xff000000)
#endif
#ifndef L_INV_CNT_ADDR
#define L_INV_CNT_ADDR                             ((0x1940  << 2) + 0xff000000)
#endif
#ifndef L_TCON_MISC_SEL_ADDR
#define L_TCON_MISC_SEL_ADDR                       ((0x1941  << 2) + 0xff000000)
#endif
#ifndef L_DUAL_PORT_CNTL_ADDR
#define L_DUAL_PORT_CNTL_ADDR                      ((0x1942  << 2) + 0xff000000)
#endif
#ifndef MLVDS_CLK_CTL1_HI
#define MLVDS_CLK_CTL1_HI                          ((0x1943  << 2) + 0xff000000)
#endif
#ifndef MLVDS_CLK_CTL1_LO
#define MLVDS_CLK_CTL1_LO                          ((0x1944  << 2) + 0xff000000)
#endif
#ifndef L_TCON_DOUBLE_CTL
#define L_TCON_DOUBLE_CTL                          ((0x1949  << 2) + 0xff000000)
#endif
#ifndef L_TCON_PATTERN_HI
#define L_TCON_PATTERN_HI                          ((0x194a  << 2) + 0xff000000)
#endif
#ifndef L_TCON_PATTERN_LO
#define L_TCON_PATTERN_LO                          ((0x194b  << 2) + 0xff000000)
#endif
#ifndef LDIM_BL_ADDR_PORT
#define LDIM_BL_ADDR_PORT                          ((0x194e  << 2) + 0xff000000)
#endif
#ifndef LDIM_BL_DATA_PORT
#define LDIM_BL_DATA_PORT                          ((0x194f  << 2) + 0xff000000)
#endif
#ifndef L_DE_HS_ADDR
#define L_DE_HS_ADDR                               ((0x1951  << 2) + 0xff000000)
#endif
#ifndef L_DE_HE_ADDR
#define L_DE_HE_ADDR                               ((0x1952  << 2) + 0xff000000)
#endif
#ifndef L_DE_VS_ADDR
#define L_DE_VS_ADDR                               ((0x1953  << 2) + 0xff000000)
#endif
#ifndef L_DE_VE_ADDR
#define L_DE_VE_ADDR                               ((0x1954  << 2) + 0xff000000)
#endif
#ifndef L_HSYNC_HS_ADDR
#define L_HSYNC_HS_ADDR                            ((0x1955  << 2) + 0xff000000)
#endif
#ifndef L_HSYNC_HE_ADDR
#define L_HSYNC_HE_ADDR                            ((0x1956  << 2) + 0xff000000)
#endif
#ifndef L_HSYNC_VS_ADDR
#define L_HSYNC_VS_ADDR                            ((0x1957  << 2) + 0xff000000)
#endif
#ifndef L_HSYNC_VE_ADDR
#define L_HSYNC_VE_ADDR                            ((0x1958  << 2) + 0xff000000)
#endif
#ifndef L_VSYNC_HS_ADDR
#define L_VSYNC_HS_ADDR                            ((0x1959  << 2) + 0xff000000)
#endif
#ifndef L_VSYNC_HE_ADDR
#define L_VSYNC_HE_ADDR                            ((0x195a  << 2) + 0xff000000)
#endif
#ifndef L_VSYNC_VS_ADDR
#define L_VSYNC_VS_ADDR                            ((0x195b  << 2) + 0xff000000)
#endif
#ifndef L_VSYNC_VE_ADDR
#define L_VSYNC_VE_ADDR                            ((0x195c  << 2) + 0xff000000)
#endif
#ifndef L_LCD_MCU_CTL
#define L_LCD_MCU_CTL                              ((0x195d  << 2) + 0xff000000)
#endif
#ifndef LVDS_PACK_CNTL_ADDR
#define LVDS_PACK_CNTL_ADDR                        ((0x19d0  << 2) + 0xff000000)
#endif
#ifndef DE_HS_ADDR
#define DE_HS_ADDR                                 ((0x19d1  << 2) + 0xff000000)
#endif
#ifndef DE_HE_ADDR
#define DE_HE_ADDR                                 ((0x19d2  << 2) + 0xff000000)
#endif
#ifndef DE_VS_ADDR
#define DE_VS_ADDR                                 ((0x19d3  << 2) + 0xff000000)
#endif
#ifndef DE_VE_ADDR
#define DE_VE_ADDR                                 ((0x19d4  << 2) + 0xff000000)
#endif
#ifndef HSYNC_HS_ADDR
#define HSYNC_HS_ADDR                              ((0x19d5  << 2) + 0xff000000)
#endif
#ifndef HSYNC_HE_ADDR
#define HSYNC_HE_ADDR                              ((0x19d6  << 2) + 0xff000000)
#endif
#ifndef HSYNC_VS_ADDR
#define HSYNC_VS_ADDR                              ((0x19d7  << 2) + 0xff000000)
#endif
#ifndef HSYNC_VE_ADDR
#define HSYNC_VE_ADDR                              ((0x19d8  << 2) + 0xff000000)
#endif
#ifndef VSYNC_HS_ADDR
#define VSYNC_HS_ADDR                              ((0x19d9  << 2) + 0xff000000)
#endif
#ifndef VSYNC_HE_ADDR
#define VSYNC_HE_ADDR                              ((0x19da  << 2) + 0xff000000)
#endif
#ifndef VSYNC_VS_ADDR
#define VSYNC_VS_ADDR                              ((0x19db  << 2) + 0xff000000)
#endif
#ifndef VSYNC_VE_ADDR
#define VSYNC_VE_ADDR                              ((0x19dc  << 2) + 0xff000000)
#endif

#ifndef LVDS_GEN_CNTL
#define LVDS_GEN_CNTL                              ((0x19e0  << 2) + 0xff000000)
#endif
#ifndef LVDS_SRG_TEST
#define LVDS_SRG_TEST                              ((0x19e8  << 2) + 0xff000000)
#endif
#ifndef LVDS_BIST_MUX0
#define LVDS_BIST_MUX0                             ((0x19e9  << 2) + 0xff000000)
#endif
#ifndef LVDS_BIST_MUX1
#define LVDS_BIST_MUX1                             ((0x19ea  << 2) + 0xff000000)
#endif
#ifndef LVDS_BIST_FIXED0
#define LVDS_BIST_FIXED0                           ((0x19eb  << 2) + 0xff000000)
#endif
#ifndef LVDS_BIST_FIXED1
#define LVDS_BIST_FIXED1                           ((0x19ec  << 2) + 0xff000000)
#endif
#ifndef LVDS_BIST_CNTL0
#define LVDS_BIST_CNTL0                            ((0x19ed  << 2) + 0xff000000)
#endif
#ifndef LVDS_CLKB_CLKA
#define LVDS_CLKB_CLKA                             ((0x19ee  << 2) + 0xff000000)
#endif
#ifndef LVDS_PHY_CLK_CNTL
#define LVDS_PHY_CLK_CNTL                          ((0x19ef  << 2) + 0xff000000)
#endif
#ifndef LVDS_SER_EN
#define LVDS_SER_EN                                ((0x19f0  << 2) + 0xff000000)
#endif

#ifndef VBO_CTRL_L
#define VBO_CTRL_L                                 ((0x1960  << 2) + 0xff000000)
#endif
#ifndef VBO_CTRL_H
#define VBO_CTRL_H                                 ((0x1961  << 2) + 0xff000000)
#endif
#ifndef VBO_SOFT_RST
#define VBO_SOFT_RST                               ((0x1962  << 2) + 0xff000000)
#endif
#ifndef VBO_LANES
#define VBO_LANES                                  ((0x1963  << 2) + 0xff000000)
#endif
#ifndef VBO_VIN_CTRL
#define VBO_VIN_CTRL                               ((0x1964  << 2) + 0xff000000)
#endif
#ifndef VBO_ACT_VSIZE
#define VBO_ACT_VSIZE                              ((0x1965  << 2) + 0xff000000)
#endif
#ifndef VBO_REGION_00
#define VBO_REGION_00                              ((0x1966  << 2) + 0xff000000)
#endif
#ifndef VBO_REGION_01
#define VBO_REGION_01                              ((0x1967  << 2) + 0xff000000)
#endif
#ifndef VBO_REGION_02
#define VBO_REGION_02                              ((0x1968  << 2) + 0xff000000)
#endif
#ifndef VBO_REGION_03
#define VBO_REGION_03                              ((0x1969  << 2) + 0xff000000)
#endif
#ifndef VBO_VBK_CTRL_0
#define VBO_VBK_CTRL_0                             ((0x196a  << 2) + 0xff000000)
#endif
#ifndef VBO_VBK_CTRL_1
#define VBO_VBK_CTRL_1                             ((0x196b  << 2) + 0xff000000)
#endif
#ifndef VBO_HBK_CTRL
#define VBO_HBK_CTRL                               ((0x196c  << 2) + 0xff000000)
#endif
#ifndef VBO_PXL_CTRL
#define VBO_PXL_CTRL                               ((0x196d  << 2) + 0xff000000)
#endif
#ifndef VBO_LANE_SKEW_L
#define VBO_LANE_SKEW_L                            ((0x196e  << 2) + 0xff000000)
#endif
#ifndef VBO_LANE_SKEW_H
#define VBO_LANE_SKEW_H                            ((0x196f  << 2) + 0xff000000)
#endif
#ifndef VBO_GCLK_LANE_L
#define VBO_GCLK_LANE_L                            ((0x1970  << 2) + 0xff000000)
#endif
#ifndef VBO_GCLK_LANE_H
#define VBO_GCLK_LANE_H                            ((0x1971  << 2) + 0xff000000)
#endif
#ifndef VBO_GCLK_MAIN
#define VBO_GCLK_MAIN                              ((0x1972  << 2) + 0xff000000)
#endif
#ifndef VBO_STATUS_L
#define VBO_STATUS_L                               ((0x1973  << 2) + 0xff000000)
#endif
#ifndef VBO_STATUS_H
#define VBO_STATUS_H                               ((0x1974  << 2) + 0xff000000)
#endif
#ifndef VBO_LANE_OUTPUT
#define VBO_LANE_OUTPUT                            ((0x1975  << 2) + 0xff000000)
#endif
#ifndef LCD_PORT_SWAP
#define LCD_PORT_SWAP                              ((0x1976  << 2) + 0xff000000)
#endif
#ifndef VBO_TMCHK_THRD_L
#define VBO_TMCHK_THRD_L                           ((0x1978  << 2) + 0xff000000)
#endif
#ifndef VBO_TMCHK_THRD_H
#define VBO_TMCHK_THRD_H                           ((0x1979  << 2) + 0xff000000)
#endif
#ifndef VBO_FSM_HOLDER_L
#define VBO_FSM_HOLDER_L                           ((0x197a  << 2) + 0xff000000)
#endif
#ifndef VBO_FSM_HOLDER_H
#define VBO_FSM_HOLDER_H                           ((0x197b  << 2) + 0xff000000)
#endif
#ifndef VBO_INTR_STATE_CTRL
#define VBO_INTR_STATE_CTRL                        ((0x197c  << 2) + 0xff000000)
#endif
#ifndef VBO_INTR_UNMASK
#define VBO_INTR_UNMASK                            ((0x197d  << 2) + 0xff000000)
#endif
#ifndef VBO_TMCHK_HSYNC_STATE_L
#define VBO_TMCHK_HSYNC_STATE_L                    ((0x197e  << 2) + 0xff000000)
#endif
#ifndef VBO_TMCHK_HSYNC_STATE_H
#define VBO_TMCHK_HSYNC_STATE_H                    ((0x197f  << 2) + 0xff000000)
#endif
#ifndef VBO_TMCHK_VSYNC_STATE_L
#define VBO_TMCHK_VSYNC_STATE_L                    ((0x19f4  << 2) + 0xff000000)
#endif
#ifndef VBO_TMCHK_VSYNC_STATE_H
#define VBO_TMCHK_VSYNC_STATE_H                    ((0x19f5  << 2) + 0xff000000)
#endif
#ifndef VBO_TMCHK_VDE_STATE_L
#define VBO_TMCHK_VDE_STATE_L                      ((0x19f6  << 2) + 0xff000000)
#endif
#ifndef VBO_TMCHK_VDE_STATE_H
#define VBO_TMCHK_VDE_STATE_H                      ((0x19f7  << 2) + 0xff000000)
#endif
#ifndef VBO_INTR_STATE
#define VBO_INTR_STATE                             ((0x19f8  << 2) + 0xff000000)
#endif
#ifndef VBO_INSGN_CTRL
#define VBO_INSGN_CTRL                             ((0x19fa  << 2) + 0xff000000)
#endif

#define TCON_CORE_FLAG_LIC2			(0xff630000L + (0x00e << 2))

#ifndef REG_TCON_APB_BASE
#define REG_TCON_APB_BASE                          (0xFF600000L)
#define TCON_CORE_REG_START                        0x0000
#define TCON_CTRL_TIMING_BASE                      0x01b0
#define TCON_TOP_CTRL                              0x1000
#define TCON_RGB_IN_MUX                            0x1001
#define TCON_OUT_CH_SEL0                           0x1002
#define TCON_OUT_CH_SEL1                           0x1003
#define TCON_I2C_DEGLITCH_CNTL                     0x1004
#define TCON_STATUS0                               0x1008 /* read only */
#define TCON_PLLLOCK_CNTL                          0x1009
#define TCON_PLLLCK_RST_CNT                        0x100a
#define TCON_RST_CTRL                              0x100b
#define TCON_AXI_OFST                              0x100c
#define TCON_DDRIF_CTRL0                           0x100d
#define TCON_CLK_CTRL                              0x100e
#define TCON_DDRIF_CTRL1                           0x100f
#define TCON_STATUS1                               0x1010 /* read only */
#define TCON_DDRIF_CTRL2                           0x1011
#define TCON_INTR_MASKN                            0x1022
#define TCON_INTR                                  0x1023 /* read only */
#endif

#ifndef TCON_AXI_OFST
#define TCON_AXI_OFST                              0x100c
#endif
#ifndef TCON_AXI_OFST0
#define TCON_AXI_OFST0                             0x200c
#endif
#ifndef TCON_STATUS2
#define TCON_STATUS2                               0x2012
#endif
#ifndef TCON_AXI_OFST1
#define TCON_AXI_OFST1                             0x2013
#endif
#ifndef TCON_AXI_OFST2
#define TCON_AXI_OFST2                             0x2014
#endif
#ifndef TCON_GPO_CTRL0
#define TCON_GPO_CTRL0                             0x2015
#endif
#ifndef TCON_GPO_CTRL1
#define TCON_GPO_CTRL1                             0x2016
#endif
#ifndef TCON_GPO_CTRL2
#define TCON_GPO_CTRL2                             0x2017
#endif
#ifndef TCON_INTR_WR
#define TCON_INTR_WR                               0x2020
#endif
#ifndef TCON_INTR_CLR
#define TCON_INTR_CLR                              0x2021
#endif
#ifndef TCON_INTR_MASKN
#define TCON_INTR_MASKN                            0x2022
#endif
#ifndef TCON_INTR_RO
#define TCON_INTR_RO                               0x2023
#endif

#ifndef P2P_CH_SWAP0
#define P2P_CH_SWAP0                               0x4200
#endif
#ifndef P2P_CH_SWAP1
#define P2P_CH_SWAP1                               0x4201
#endif
#ifndef P2P_BIT_REV
#define P2P_BIT_REV                               ((0x1950  << 2) + 0xff000000)
#endif

#ifndef LVDS_CH_SWAP0
#define LVDS_CH_SWAP0                              0x14e1
#endif
#ifndef LVDS_CH_SWAP1
#define LVDS_CH_SWAP1                              0x14e2
#endif
#ifndef LVDS_CH_SWAP2
#define LVDS_CH_SWAP2                              0x14e3
#endif

#ifndef VPU_VOUT_CORE_CTRL
#define VPU_VOUT_CORE_CTRL                         ((0x0100  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_INT_CTRL
#define VPU_VOUT_INT_CTRL                          ((0x0101  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_DETH_CTRL
#define VPU_VOUT_DETH_CTRL                         ((0x0102  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_DTH_DATA
#define VPU_VOUT_DTH_DATA                          ((0x0103  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_DTH_ADDR
#define VPU_VOUT_DTH_ADDR                          ((0x0104  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_HS_POS
#define VPU_VOUT_HS_POS                            ((0x0112  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_VSLN_E_POS
#define VPU_VOUT_VSLN_E_POS                        ((0x0113  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_VSPX_E_POS
#define VPU_VOUT_VSPX_E_POS                        ((0x0114  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_VSLN_O_POS
#define VPU_VOUT_VSLN_O_POS                        ((0x0115  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_VSPX_O_POS
#define VPU_VOUT_VSPX_O_POS                        ((0x0116  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_DE_PX_EN
#define VPU_VOUT_DE_PX_EN                          ((0x0117  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_DELN_E_POS
#define VPU_VOUT_DELN_E_POS                        ((0x0118  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_DELN_O_POS
#define VPU_VOUT_DELN_O_POS                        ((0x0119  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_MAX_SIZE
#define VPU_VOUT_MAX_SIZE                          ((0x011a  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_FLD_BGN_LINE
#define VPU_VOUT_FLD_BGN_LINE                      ((0x011b  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUTO_HS_POS
#define VPU_VOUTO_HS_POS                           ((0x0120  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUTO_VSLN_E_POS
#define VPU_VOUTO_VSLN_E_POS                       ((0x0121  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUTO_VSPX_E_POS
#define VPU_VOUTO_VSPX_E_POS                       ((0x0122  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUTO_VSLN_O_POS
#define VPU_VOUTO_VSLN_O_POS                       ((0x0123  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUTO_VSPX_O_POS
#define VPU_VOUTO_VSPX_O_POS                       ((0x0124  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUTO_DE_PX_EN
#define VPU_VOUTO_DE_PX_EN                         ((0x0125  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUTO_DELN_E_POS
#define VPU_VOUTO_DELN_E_POS                       ((0x0126  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUTO_DELN_O_POS
#define VPU_VOUTO_DELN_O_POS                       ((0x0127  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUTO_MAX_SIZE
#define VPU_VOUTO_MAX_SIZE                         ((0x0128  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUTO_FLD_BGN_LINE
#define VPU_VOUTO_FLD_BGN_LINE                     ((0x0129  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_BT_CTRL
#define VPU_VOUT_BT_CTRL                           ((0x0130  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_BT_PLD_LINE
#define VPU_VOUT_BT_PLD_LINE                       ((0x0131  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_BT_PLDIDT0
#define VPU_VOUT_BT_PLDIDT0                        ((0x0132  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_BT_PLDIDT1
#define VPU_VOUT_BT_PLDIDT1                        ((0x0133  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_BT_BLK_DATA
#define VPU_VOUT_BT_BLK_DATA                       ((0x0134  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_BT_DAT_CLPY
#define VPU_VOUT_BT_DAT_CLPY                       ((0x0135  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_BT_DAT_CLPC
#define VPU_VOUT_BT_DAT_CLPC                       ((0x0136  << 2) + 0xfe350000)
#endif
#ifndef VPU_VOUT_RO_INT
#define VPU_VOUT_RO_INT                            ((0x0140  << 2) + 0xfe350000)
#endif

#ifndef PWM_PWM_E
#define PWM_PWM_E                                  ((0x6400 << 2) + 0xffd00000)
#endif
#ifndef PWM_PWM_F
#define PWM_PWM_F                                  ((0x6401 << 2) + 0xffd00000)
#endif
#ifndef PWM_MISC_REG_EF
#define PWM_MISC_REG_EF                            ((0x6402 << 2) + 0xffd00000)
#endif

#ifndef REG_BASE_CBUS
#define REG_BASE_CBUS 0x1
#endif

#ifndef REG_BASE_VCBUS
#define REG_BASE_VCBUS 0x2
#endif

#ifndef REG_BASE_DSI_HOST
#define REG_BASE_DSI_HOST 0x3
#endif

#ifndef MIPI_DSI_BASE
#define MIPI_DSI_BASE        0xfe074000L
#endif
#ifndef MIPI_DSI_B_BASE
#define MIPI_DSI_B_BASE      0xfe072000L
#endif
#ifndef MIPI_DSI_PHY_BASE
#define MIPI_DSI_PHY_BASE    0xfe014000L
#endif
#ifndef MIPI_DSI_B_PHY_BASE
#define MIPI_DSI_B_PHY_BASE  0xfe016000L
#endif
#ifndef EDPTX0_BASE
#define EDPTX0_BASE    0xfe0b0000L
#endif
#ifndef EDPTX1_BASE
#define EDPTX1_BASE    0xfe0b2000L
#endif

#ifndef HHI_GP0_PLL_CNTL2
#define HHI_GP0_PLL_CNTL2 0x4
#endif

#ifndef HHI_GP0_PLL_CNTL3
#define HHI_GP0_PLL_CNTL3 0x4
#endif

#ifndef HHI_GP0_PLL_CNTL4
#define HHI_GP0_PLL_CNTL4 0x4
#endif

#ifndef HHI_GP0_PLL_CNTL5
#define HHI_GP0_PLL_CNTL5 0x4
#endif

#ifndef HHI_VIID_CLK_CNTL
#define HHI_VIID_CLK_CNTL 0x4
#endif

#ifndef HHI_VIID_CLK_DIV
#define HHI_VIID_CLK_DIV 0x4
#endif

#ifndef HHI_VID_PLL_CLK_DIV
#define HHI_VID_PLL_CLK_DIV 0x4
#endif

#ifndef HHI_VID_CLK_CNTL2
#define HHI_VID_CLK_CNTL2 0x4
#endif

#ifndef HHI_MIPI_CNTL0
#define HHI_MIPI_CNTL0 0x4
#endif

#ifndef HHI_MIPI_CNTL1
#define HHI_MIPI_CNTL1 0x4
#endif

#ifndef HHI_MIPI_CNTL2
#define HHI_MIPI_CNTL2 0x4
#endif

#ifndef HHI_VDIN_MEAS_CLK_CNTL
#define HHI_VDIN_MEAS_CLK_CNTL 0x4
#endif

#ifndef PWM_MISC_REG_AB
#define PWM_MISC_REG_AB                            0x6c02
#endif

#ifndef PWM_MISC_REG_CD
#define PWM_MISC_REG_CD                            0x6802
#endif

#ifndef PWM_MISC_REG_EF
#define PWM_MISC_REG_EF                            0x6402
#endif

#ifndef PWM_PWM_A
#define PWM_PWM_A                                  0x6c00
#endif

#ifndef PWM_PWM_B
#define PWM_PWM_B                                  0x6c01
#endif

#ifndef PWM_PWM_C
#define PWM_PWM_C                                  0x6800
#endif

#ifndef PWM_PWM_D
#define PWM_PWM_D                                  0x6801
#endif

#ifndef PWM_PWM_E
#define PWM_PWM_E                                  0x6400
#endif

#ifndef PWM_PWM_F
#define PWM_PWM_F                                  0x6401
#endif

#ifndef AO_PWM_PWM_A
#define AO_PWM_PWM_A                               (0xff807000 + (0x000 << 2))
#endif
#ifndef AO_PWM_PWM_B
#define AO_PWM_PWM_B                               (0xff807000 + (0x001 << 2))
#endif
#ifndef AO_PWM_MISC_REG_AB
#define AO_PWM_MISC_REG_AB                         (0xff807000 + (0x002 << 2))
#endif

#ifndef PWMAB_PWM_A
#define PWMAB_PWM_A                                ((0x0000  << 2) + 0xfe058000)
#endif
#ifndef PWMAB_PWM_B
#define PWMAB_PWM_B                                ((0x0001  << 2) + 0xfe058000)
#endif
#ifndef PWMAB_MISC_REG_AB
#define PWMAB_MISC_REG_AB                          ((0x0002  << 2) + 0xfe058000)
#endif
#ifndef PWMCD_PWM_A
#define PWMCD_PWM_A                                ((0x0000  << 2) + 0xfe05a000)
#endif
#ifndef PWMCD_PWM_B
#define PWMCD_PWM_B                                ((0x0001  << 2) + 0xfe05a000)
#endif
#ifndef PWMCD_MISC_REG_AB
#define PWMCD_MISC_REG_AB                          ((0x0002  << 2) + 0xfe05a000)
#endif
#ifndef PWMEF_PWM_A
#define PWMEF_PWM_A                                ((0x0000  << 2) + 0xfe05c000)
#endif
#ifndef PWMEF_PWM_B
#define PWMEF_PWM_B                                ((0x0001  << 2) + 0xfe05c000)
#endif
#ifndef PWMEF_MISC_REG_AB
#define PWMEF_MISC_REG_AB                          ((0x0002  << 2) + 0xfe05c000)
#endif
#ifndef PWMGH_PWM_A
#define PWMGH_PWM_A                                ((0x0000  << 2) + 0xfe05e000)
#endif
#ifndef PWMGH_PWM_B
#define PWMGH_PWM_B                                ((0x0001  << 2) + 0xfe05e000)
#endif
#ifndef PWMGH_MISC_REG_AB
#define PWMGH_MISC_REG_AB                          ((0x0002  << 2) + 0xfe05e000)
#endif
#ifndef PWMIJ_PWM_A
#define PWMIJ_PWM_A                                ((0x0000  << 2) + 0xfe060000)
#endif
#ifndef PWMIJ_PWM_B
#define PWMIJ_PWM_B                                ((0x0001  << 2) + 0xfe060000)
#endif
#ifndef PWMIJ_MISC_REG_AB
#define PWMIJ_MISC_REG_AB                          ((0x0002  << 2) + 0xfe060000)
#endif

#ifndef PWM_AO_AB_PWM_A
#define PWM_AO_AB_PWM_A                            ((0x0000  << 2) + 0xfe05e000)
#endif
#ifndef PWM_AO_AB_PWM_B
#define PWM_AO_AB_PWM_B                            ((0x0001  << 2) + 0xfe05e000)
#endif
#ifndef PWM_AO_AB_MISC_REG_AB
#define PWM_AO_AB_MISC_REG_AB                      ((0x0002  << 2) + 0xfe05e000)
#endif
#ifndef PWM_AO_CD_PWM_A
#define PWM_AO_CD_PWM_A                            ((0x0000  << 2) + 0xfe060000)
#endif
#ifndef PWM_AO_CD_PWM_B
#define PWM_AO_CD_PWM_B                            ((0x0001  << 2) + 0xfe060000)
#endif
#ifndef PWM_AO_CD_MISC_REG_AB
#define PWM_AO_CD_MISC_REG_AB                      ((0x0002  << 2) + 0xfe060000)
#endif
#ifndef PWM_AO_EF_PWM_A
#define PWM_AO_EF_PWM_A                            ((0x0000  << 2) + 0xfe030000)
#endif
#ifndef PWM_AO_EF_PWM_B
#define PWM_AO_EF_PWM_B                            ((0x0001  << 2) + 0xfe030000)
#endif
#ifndef PWM_AO_EF_MISC_REG_AB
#define PWM_AO_EF_MISC_REG_AB                      ((0x0002  << 2) + 0xfe030000)
#endif
#ifndef PWM_AO_GH_PWM_A
#define PWM_AO_GH_PWM_A                            ((0x0000  << 2) + 0xfe032000)
#endif
#ifndef PWM_AO_GH_PWM_B
#define PWM_AO_GH_PWM_B                            ((0x0001  << 2) + 0xfe032000)
#endif
#ifndef PWM_AO_GH_MISC_REG_AB
#define PWM_AO_GH_MISC_REG_AB                      ((0x0002  << 2) + 0xfe032000)
#endif
#ifndef CLKCTRL_PWM_CLK_AB_CTRL
#define CLKCTRL_PWM_CLK_AB_CTRL                    ((0x0060  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_PWM_CLK_CD_CTRL
#define CLKCTRL_PWM_CLK_CD_CTRL                    ((0x0061  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_PWM_CLK_EF_CTRL
#define CLKCTRL_PWM_CLK_EF_CTRL                    ((0x0062  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_PWM_CLK_GH_CTRL
#define CLKCTRL_PWM_CLK_GH_CTRL                    ((0x0063  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_PWM_CLK_IJ_CTRL
#define CLKCTRL_PWM_CLK_IJ_CTRL                    ((0x0064  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_PWM_CLK_AO_AB_CTRL
#define CLKCTRL_PWM_CLK_AO_AB_CTRL                 ((0x0068  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_PWM_CLK_AO_CD_CTRL
#define CLKCTRL_PWM_CLK_AO_CD_CTRL                 ((0x0069  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_PWM_CLK_AO_EF_CTRL
#define CLKCTRL_PWM_CLK_AO_EF_CTRL                 ((0x006a  << 2) + 0xfe000000)
#endif
#ifndef CLKCTRL_PWM_CLK_AO_GH_CTRL
#define CLKCTRL_PWM_CLK_AO_GH_CTRL                 ((0x006b  << 2) + 0xfe000000)
#endif

#ifndef PERIPHS_PIN_MUX_0
#define PERIPHS_PIN_MUX_0 PADCTRL_PIN_MUX_REG0
#endif
#ifndef PERIPHS_PIN_MUX_7
#define PERIPHS_PIN_MUX_7 0x5
#endif
#ifndef PERIPHS_PIN_MUX_8
#define PERIPHS_PIN_MUX_8 0x5
#endif
#ifndef PERIPHS_PIN_MUX_9
#define PERIPHS_PIN_MUX_9 0x5
#endif

#ifndef PADCTRL_PIN_MUX_REG0
#define PADCTRL_PIN_MUX_REG0                       ((0x0000  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REG1
#define PADCTRL_PIN_MUX_REG1                       ((0x0001  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REG2
#define PADCTRL_PIN_MUX_REG2                       ((0x0002  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REG3
#define PADCTRL_PIN_MUX_REG3                       ((0x0003  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REG4
#define PADCTRL_PIN_MUX_REG4                       ((0x0004  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REG5
#define PADCTRL_PIN_MUX_REG5                       ((0x0005  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REG6
#define PADCTRL_PIN_MUX_REG6                       ((0x0006  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REG7
#define PADCTRL_PIN_MUX_REG7                       ((0x0007  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REG8
#define PADCTRL_PIN_MUX_REG8                       ((0x0008  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REG9
#define PADCTRL_PIN_MUX_REG9                       ((0x0009  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGA
#define PADCTRL_PIN_MUX_REGA                       ((0x000a  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGB
#define PADCTRL_PIN_MUX_REGB                       ((0x000b  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGC
#define PADCTRL_PIN_MUX_REGC                       ((0x000c  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGD
#define PADCTRL_PIN_MUX_REGD                       ((0x000d  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGE
#define PADCTRL_PIN_MUX_REGE                       ((0x000e  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGF
#define PADCTRL_PIN_MUX_REGF                       ((0x000f  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGG
#define PADCTRL_PIN_MUX_REGG                       ((0x0010  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGH
#define PADCTRL_PIN_MUX_REGH                       ((0x0011  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGI
#define PADCTRL_PIN_MUX_REGI                       ((0x0012  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGJ
#define PADCTRL_PIN_MUX_REGJ                       ((0x0013  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGK
#define PADCTRL_PIN_MUX_REGK                       ((0x0014  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGL
#define PADCTRL_PIN_MUX_REGL                       ((0x0015  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGM
#define PADCTRL_PIN_MUX_REGM                       ((0x0016  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGN
#define PADCTRL_PIN_MUX_REGN                       ((0x0017  << 2) + 0xfe004000)
#endif
#ifndef PADCTRL_PIN_MUX_REGO
#define PADCTRL_PIN_MUX_REGO                       ((0x0018  << 2) + 0xfe004000)
#endif

#ifndef RESETCTRL_RESET0_LEVEL
#define RESETCTRL_RESET0_LEVEL                     ((0x0010  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET1_LEVEL
#define RESETCTRL_RESET1_LEVEL                     ((0x0011  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET2_LEVEL
#define RESETCTRL_RESET2_LEVEL                     ((0x0012  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET3_LEVEL
#define RESETCTRL_RESET3_LEVEL                     ((0x0013  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET4_LEVEL
#define RESETCTRL_RESET4_LEVEL                     ((0x0014  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET5_LEVEL
#define RESETCTRL_RESET5_LEVEL                     ((0x0015  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET6_LEVEL
#define RESETCTRL_RESET6_LEVEL                     ((0x0016  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET0_MASK
#define RESETCTRL_RESET0_MASK                      ((0x0020  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET1_MASK
#define RESETCTRL_RESET1_MASK                      ((0x0021  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET2_MASK
#define RESETCTRL_RESET2_MASK                      ((0x0022  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET3_MASK
#define RESETCTRL_RESET3_MASK                      ((0x0023  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET4_MASK
#define RESETCTRL_RESET4_MASK                      ((0x0024  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET5_MASK
#define RESETCTRL_RESET5_MASK                      ((0x0025  << 2) + 0xfe002000)
#endif
#ifndef RESETCTRL_RESET6_MASK
#define RESETCTRL_RESET6_MASK                      ((0x0026  << 2) + 0xfe002000)
#endif

#ifndef RESET0_MASK
#define RESET0_MASK                                ((0x0410  << 2) + 0xffd00000)
#endif
#ifndef RESET1_MASK
#define RESET1_MASK                                ((0x0411  << 2) + 0xffd00000)
#endif
#ifndef RESET2_MASK
#define RESET2_MASK                                ((0x0412  << 2) + 0xffd00000)
#endif
#ifndef RESET3_MASK
#define RESET3_MASK                                ((0x0413  << 2) + 0xffd00000)
#endif
#ifndef RESET4_MASK
#define RESET4_MASK                                ((0x0414  << 2) + 0xffd00000)
#endif
#ifndef RESET5_MASK
#define RESET5_MASK                                ((0x0415  << 2) + 0xffd00000)
#endif
#ifndef RESET6_MASK
#define RESET6_MASK                                ((0x0416  << 2) + 0xffd00000)
#endif
#ifndef RESET7_MASK
#define RESET7_MASK                                ((0x0417  << 2) + 0xffd00000)
#endif
#ifndef RESET0_LEVEL
#define RESET0_LEVEL                               ((0x0420  << 2) + 0xffd00000)
#endif
#ifndef RESET1_LEVEL
#define RESET1_LEVEL                               ((0x0421  << 2) + 0xffd00000)
#endif
#ifndef RESET2_LEVEL
#define RESET2_LEVEL                               ((0x0422  << 2) + 0xffd00000)
#endif
#ifndef RESET3_LEVEL
#define RESET3_LEVEL                               ((0x0423  << 2) + 0xffd00000)
#endif
#ifndef RESET4_LEVEL
#define RESET4_LEVEL                               ((0x0424  << 2) + 0xffd00000)
#endif
#ifndef RESET5_LEVEL
#define RESET5_LEVEL                               ((0x0425  << 2) + 0xffd00000)
#endif
#ifndef RESET6_LEVEL
#define RESET6_LEVEL                               ((0x0426  << 2) + 0xffd00000)
#endif
#ifndef RESET7_LEVEL
#define RESET7_LEVEL                               ((0x0427  << 2) + 0xffd00000)
#endif

#ifndef PM_MIPI_DSI0
#define PM_MIPI_DSI0 0xff
#endif
#ifndef PM_MIPI_DSI1
#define PM_MIPI_DSI1 0xff
#endif
#ifndef PM_EDP0
#define PM_EDP0 0xff
#endif
#ifndef PM_EDP1
#define PM_EDP1 0xff
#endif
