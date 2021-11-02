#ifndef VPU_REGS_H
#define VPU_REGS_H

//===========================================================================
// For ENCL
//===========================================================================
// bit 15:8 -- vfifo2vd_vd_sel
// bit 7 -- vfifo2vd_drop
// bit 6:1 -- vfifo2vd_delay
// bit 0 -- vfifo2vd_en
#define ENCL_VFIFO2VD_CTL                          0x1c90
// bit 12:0 -- vfifo2vd_pixel_start
#define ENCL_VFIFO2VD_PIXEL_START                  0x1c91
// bit 12:00 -- vfifo2vd_pixel_end
#define ENCL_VFIFO2VD_PIXEL_END                    0x1c92
// bit 10:0 -- vfifo2vd_line_top_start
#define ENCL_VFIFO2VD_LINE_TOP_START               0x1c93
// bit 10:00 -- vfifo2vd_line_top_end
#define ENCL_VFIFO2VD_LINE_TOP_END                 0x1c94
// bit 10:00 -- vfifo2vd_line_bot_start
#define ENCL_VFIFO2VD_LINE_BOT_START               0x1c95
// bit 10:00 -- vfifo2vd_line_bot_end
#define ENCL_VFIFO2VD_LINE_BOT_END                 0x1c96
#define ENCL_VFIFO2VD_CTL2                         0x1c97
#define ENCL_TST_EN                                0x1c98
#define ENCL_TST_MDSEL                             0x1c99
#define ENCL_TST_Y                                 0x1c9a
#define ENCL_TST_CB                                0x1c9b
#define ENCL_TST_CR                                0x1c9c
#define ENCL_TST_CLRBAR_STRT                       0x1c9d
#define ENCL_TST_CLRBAR_WIDTH                      0x1c9e
#define ENCL_TST_VDCNT_STSET                       0x1c9f
//===========================================================================
// ENCL registers
#define ENCL_VIDEO_EN                              0x1ca0
#define ENCL_VIDEO_Y_SCL                           0x1ca1
#define ENCL_VIDEO_PB_SCL                          0x1ca2
#define ENCL_VIDEO_PR_SCL                          0x1ca3
#define ENCL_VIDEO_Y_OFFST                         0x1ca4
#define ENCL_VIDEO_PB_OFFST                        0x1ca5
#define ENCL_VIDEO_PR_OFFST                        0x1ca6
//----- Video mode
#define ENCL_VIDEO_MODE                            0x1ca7
#define ENCL_VIDEO_MODE_ADV                        0x1ca8
//--------------- Debug pins
#define ENCL_DBG_PX_RST                            0x1ca9
#define ENCL_DBG_LN_RST                            0x1caa
#define ENCL_DBG_PX_INT                            0x1cab
#define ENCL_DBG_LN_INT                            0x1cac
//----------- Video Advanced setting
#define ENCL_VIDEO_YFP1_HTIME                      0x1cad
#define ENCL_VIDEO_YFP2_HTIME                      0x1cae
#define ENCL_VIDEO_YC_DLY                          0x1caf
#define ENCL_VIDEO_MAX_PXCNT                       0x1cb0
#define ENCL_VIDEO_HAVON_END                       0x1cb1
#define ENCL_VIDEO_HAVON_BEGIN                     0x1cb2
#define ENCL_VIDEO_VAVON_ELINE                     0x1cb3
#define ENCL_VIDEO_VAVON_BLINE                     0x1cb4
#define ENCL_VIDEO_HSO_BEGIN                       0x1cb5
#define ENCL_VIDEO_HSO_END                         0x1cb6
#define ENCL_VIDEO_VSO_BEGIN                       0x1cb7
#define ENCL_VIDEO_VSO_END                         0x1cb8
#define ENCL_VIDEO_VSO_BLINE                       0x1cb9
#define ENCL_VIDEO_VSO_ELINE                       0x1cba
#define ENCL_VIDEO_MAX_LNCNT                       0x1cbb
#define ENCL_VIDEO_BLANKY_VAL                      0x1cbc
#define ENCL_VIDEO_BLANKPB_VAL                     0x1cbd
#define ENCL_VIDEO_BLANKPR_VAL                     0x1cbe
#define ENCL_VIDEO_HOFFST                          0x1cbf
#define ENCL_VIDEO_VOFFST                          0x1cc0
#define ENCL_VIDEO_RGB_CTRL                        0x1cc1
#define ENCL_VIDEO_FILT_CTRL                       0x1cc2
#define ENCL_VIDEO_OFLD_VPEQ_OFST                  0x1cc3
#define ENCL_VIDEO_OFLD_VOAV_OFST                  0x1cc4
#define ENCL_VIDEO_MATRIX_CB                       0x1cc5
#define ENCL_VIDEO_MATRIX_CR                       0x1cc6
#define ENCL_VIDEO_RGBIN_CTRL                      0x1cc7
#define ENCL_MAX_LINE_SWITCH_POINT                 0x1cc8
#define ENCL_DACSEL_0                              0x1cc9
#define ENCL_DACSEL_1                              0x1cca
#define ENCT_VIDEO_H_PRE_DE_END                    0x1ccb
#define ENCT_VIDEO_H_PRE_DE_BEGIN                  0x1ccc
#define ENCT_VIDEO_V_PRE_DE_ELINE                  0x1ccd
#define ENCT_VIDEO_V_PRE_DE_BLINE                  0x1cce
#define ENCL_VIDEO_H_PRE_DE_END                    0x1ccf
#define ENCL_VIDEO_H_PRE_DE_BEGIN                  0x1cd0
#define ENCL_VIDEO_V_PRE_DE_ELINE                  0x1cd1
#define ENCL_VIDEO_V_PRE_DE_BLINE                  0x1cd2
#define ENCL_INBUF_CNTL0                           0x1cd3
#define ENCL_INBUF_CNTL1                           0x1cd4
#define ENCL_INBUF_CNT                             0x1cd5
#define ENCL_INBUF_HOLD_CNT                        0x1cd6
#define ENCL_INBUF_FIX_PIX_NUM                     0x1cd7
//

//========================================================================
//LCD DRV     12'h480~12'h4ef
//=======================================================================
#define LCD_RGB_BASE_ADDR                          0x14a5
#define LCD_RGB_COEFF_ADDR                         0x14a6
#define LCD_POL_CNTL_ADDR                          0x14a7
#define LCD_DITH_CNTL_ADDR                         0x14a8
#define LCD_GAMMA_CNTL_PORT0                       0x14b4
#define LCD_GAMMA_DATA_PORT0                       0x14b5
#define LCD_GAMMA_ADDR_PORT0                       0x14b6

#define L_GAMMA_CNTL_PORT                          0x1900
#define L_GAMMA_DATA_PORT                          0x1901
#define L_GAMMA_ADDR_PORT                          0x1902
#define L_GAMMA_VCOM_HSWITCH_ADDR                  0x1903
#define L_RGB_BASE_ADDR                            0x1905
#define L_RGB_COEFF_ADDR                           0x1906
#define L_POL_CNTL_ADDR                            0x1907
#define L_DITH_CNTL_ADDR                           0x1908
#define L_GAMMA_PROBE_CTRL                         0x1909
//read only
#define L_GAMMA_PROBE_COLOR_L                      0x190a
#define L_GAMMA_PROBE_COLOR_H                      0x190b
#define L_GAMMA_PROBE_HL_COLOR                     0x190c
#define L_GAMMA_PROBE_POS_X                        0x190d
#define L_GAMMA_PROBE_POS_Y                        0x190e
#define L_STH1_HS_ADDR                             0x1910
#define L_STH1_HE_ADDR                             0x1911
#define L_STH1_VS_ADDR                             0x1912
#define L_STH1_VE_ADDR                             0x1913
#define L_STH2_HS_ADDR                             0x1914
#define L_STH2_HE_ADDR                             0x1915
#define L_STH2_VS_ADDR                             0x1916
#define L_STH2_VE_ADDR                             0x1917
#define L_OEH_HS_ADDR                              0x1918
#define L_OEH_HE_ADDR                              0x1919
#define L_OEH_VS_ADDR                              0x191a
#define L_OEH_VE_ADDR                              0x191b
#define L_VCOM_HSWITCH_ADDR                        0x191c
#define L_VCOM_VS_ADDR                             0x191d
#define L_VCOM_VE_ADDR                             0x191e
#define L_CPV1_HS_ADDR                             0x191f
#define L_CPV1_HE_ADDR                             0x1920
#define L_CPV1_VS_ADDR                             0x1921
#define L_CPV1_VE_ADDR                             0x1922
#define L_CPV2_HS_ADDR                             0x1923
#define L_CPV2_HE_ADDR                             0x1924
#define L_CPV2_VS_ADDR                             0x1925
#define L_CPV2_VE_ADDR                             0x1926
#define L_STV1_HS_ADDR                             0x1927
#define L_STV1_HE_ADDR                             0x1928
#define L_STV1_VS_ADDR                             0x1929
#define L_STV1_VE_ADDR                             0x192a
#define L_STV2_HS_ADDR                             0x192b
#define L_STV2_HE_ADDR                             0x192c
#define L_STV2_VS_ADDR                             0x192d
#define L_STV2_VE_ADDR                             0x192e
#define L_OEV1_HS_ADDR                             0x192f
#define L_OEV1_HE_ADDR                             0x1930
#define L_OEV1_VS_ADDR                             0x1931
#define L_OEV1_VE_ADDR                             0x1932
#define L_OEV2_HS_ADDR                             0x1933
#define L_OEV2_HE_ADDR                             0x1934
#define L_OEV2_VS_ADDR                             0x1935
#define L_OEV2_VE_ADDR                             0x1936
#define L_OEV3_HS_ADDR                             0x1937
#define L_OEV3_HE_ADDR                             0x1938
#define L_OEV3_VS_ADDR                             0x1939
#define L_OEV3_VE_ADDR                             0x193a
#define L_LCD_PWR_ADDR                             0x193b
#define L_LCD_PWM0_LO_ADDR                         0x193c
#define L_LCD_PWM0_HI_ADDR                         0x193d
#define L_LCD_PWM1_LO_ADDR                         0x193e
#define L_LCD_PWM1_HI_ADDR                         0x193f
#define L_INV_CNT_ADDR                             0x1940
#define L_TCON_MISC_SEL_ADDR                       0x1941
#define L_DUAL_PORT_CNTL_ADDR                      0x1942
#define MLVDS_CLK_CTL1_HI                          0x1943
#define MLVDS_CLK_CTL1_LO                          0x1944
//  [31:30] enable mlvds clocks
//  [24]    mlvds_clk_half_delay       24 // Bit 0
//  [23:0]  mlvds_clk_pattern           0 // Bit 23:0
#define L_TCON_DOUBLE_CTL                          0x1949
#define L_TCON_PATTERN_HI                          0x194a
#define L_TCON_PATTERN_LO                          0x194b
#define LDIM_BL_ADDR_PORT                          0x194e
#define LDIM_BL_DATA_PORT                          0x194f
#define L_DE_HS_ADDR                               0x1951
#define L_DE_HE_ADDR                               0x1952
#define L_DE_VS_ADDR                               0x1953
#define L_DE_VE_ADDR                               0x1954
#define L_HSYNC_HS_ADDR                            0x1955
#define L_HSYNC_HE_ADDR                            0x1956
#define L_HSYNC_VS_ADDR                            0x1957
#define L_HSYNC_VE_ADDR                            0x1958
#define L_VSYNC_HS_ADDR                            0x1959
#define L_VSYNC_HE_ADDR                            0x195a
#define L_VSYNC_VS_ADDR                            0x195b
#define L_VSYNC_VE_ADDR                            0x195c
// bit 8 -- vfifo_mcu_enable
// bit 7 -- halt_vs_de
// bit 6 -- R8G8B8_format
// bit 5 -- R6G6B6_format (round to 6 bits)
// bit 4 -- R5G6B5_format
// bit 3 -- dac_dith_sel
// bit 2 -- lcd_mcu_enable_de     -- ReadOnly
// bit 1 -- lcd_mcu_enable_vsync  -- ReadOnly
// bit 0 -- lcd_mcu_enable
#define L_LCD_MCU_CTL                              0x195d
#define P2P_CH_SWAP0                               0x195e
#define P2P_CH_SWAP1                               0x195f
#define P2P_BIT_REV                                0x1950

//**************************************************************************
//*  Vbyone registers  (Note: no MinLVDS in G9tv, share the register)
//**************************************************************************
#define VBO_CTRL_L                                 0x1960
#define VBO_CTRL_H                                 0x1961
#define VBO_SOFT_RST                               0x1962
#define VBO_LANES                                  0x1963
#define VBO_VIN_CTRL                               0x1964
#define VBO_ACT_VSIZE                              0x1965
#define VBO_REGION_00                              0x1966
#define VBO_REGION_01                              0x1967
#define VBO_REGION_02                              0x1968
#define VBO_REGION_03                              0x1969
#define VBO_VBK_CTRL_0                             0x196a
#define VBO_VBK_CTRL_1                             0x196b
#define VBO_HBK_CTRL                               0x196c
#define VBO_PXL_CTRL                               0x196d
#define VBO_LANE_SKEW_L                            0x196e
#define VBO_LANE_SKEW_H                            0x196f
#define VBO_GCLK_LANE_L                            0x1970
#define VBO_GCLK_LANE_H                            0x1971
#define VBO_GCLK_MAIN                              0x1972
#define VBO_STATUS_L                               0x1973
#define VBO_STATUS_H                               0x1974
#define VBO_LANE_OUTPUT                            0x1975
#define LCD_PORT_SWAP                              0x1976
#define VBO_TMCHK_THRD_L                           0x1978
#define VBO_TMCHK_THRD_H                           0x1979
#define VBO_FSM_HOLDER_L                           0x197a
#define VBO_FSM_HOLDER_H                           0x197b
#define VBO_INTR_STATE_CTRL                        0x197c
#define VBO_INTR_UNMASK                            0x197d
#define VBO_TMCHK_HSYNC_STATE_L                    0x197e
#define VBO_TMCHK_HSYNC_STATE_H                    0x197f
#define VBO_TMCHK_VSYNC_STATE_L                    0x19f4
#define VBO_TMCHK_VSYNC_STATE_H                    0x19f5
#define VBO_TMCHK_VDE_STATE_L                      0x19f6
#define VBO_TMCHK_VDE_STATE_H                      0x19f7
#define VBO_INTR_STATE                             0x19f8
#define VBO_INFILTER_CTRL                          0x19f9
#define VBO_INSGN_CTRL                             0x19fa
#define VBO_INFILTER_CTRL_H                        0x1977

#define LVDS_PACK_CNTL_ADDR                        0x19d0
   #define     LVDS_USE_TCON    7
   #define     LVDS_DUAL        6
   #define     PN_SWP           5
   #define     LSB_FIRST        4
   #define     LVDS_RESV        3
   #define     ODD_EVEN_SWP     2
   #define     LVDS_REPACK      0

#define LVDS_GEN_CNTL                              0x19e0
#define LVDS_CH_SWAP0                              0x19e1
#define LVDS_CH_SWAP1                              0x19e2
#define LVDS_CH_SWAP2                              0x19e3

#define VENC_INTCTRL                               0x1b6e
#define VPU_VENCI_STAT                             0x1ceb
#define VPU_VENCP_STAT                             0x1cec
#define VPU_VENCL_STAT                             0x1ced
#define VPU_VENC_CTRL                              0x1cef
#define VPP_MISC                                   0x1d26

#define ENCI_INFO_READ                             0x271c
#define ENCP_INFO_READ                             0x271d
#define ENCT_INFO_READ                             0x271e
#define ENCL_INFO_READ                             0x271f

#define VPU_VIU_VENC_MUX_CTRL                      0x271a

#define VPU_DISP_VIU0_CTRL                         0x2786
#define VPU_DISP_VIU1_CTRL                         0x2787
#define VPU_DISP_VIU2_CTRL                         0x2788

#define VPU_RDARB_MODE_L2C1                        0x279d
#define VPU_WRARB_MODE_L2C1                        0x27a2

//Bit 31    reg_vpu_pwm_inv, 1: invert the pwm signal, active low
//Bit 30:29 reg_vpu_pwm_src_sel, 00: encl, enct, encp
//Bit 28:16 reg_vpu_pwm_v_end0
//Bit 12:0  reg_vpu_pwm_v_start0
#define VPU_VPU_PWM_V0                             0x1ce0
//Bit 28:16 reg_vpu_pwm_v_end1
//Bit 12:0  reg_vpu_pwm_v_start1
#define VPU_VPU_PWM_V1                             0x1ce1
//Bit 28:16 reg_vpu_pwm_v_end2
//Bit 12:0  reg_vpu_pwm_v_start2
#define VPU_VPU_PWM_V2                             0x1ce2
//Bit 28:16 reg_vpu_pwm_v_end3
//Bit 12:0  reg_vpu_pwm_v_start3
#define VPU_VPU_PWM_V3                             0x1ce3
#define VPU_VPU_PWM_H0                             0x1ce4
#define VPU_VPU_PWM_H1                             0x1ce5
#define VPU_VPU_PWM_H2                             0x1ce6
#define VPU_VPU_PWM_H3                             0x1ce7

/* cbus */
#define RESET7_REGISTER                            0x0408

#define PWM_PWM_A                                  0x6c00
#define PWM_PWM_B                                  0x6c01
#define PWM_MISC_REG_AB                            0x6c02

#define PWM_PWM_C                                  0x6800
#define PWM_PWM_D                                  0x6801
#define PWM_MISC_REG_CD                            0x6802

#define PWM_PWM_E                                  0x6400
#define PWM_PWM_F                                  0x6401
#define PWM_MISC_REG_EF                            0x6402

/*tcon*/
#define REG_TCON_APB_BASE                          (0xFF660000L)
#define TCON_CORE_REG_START                        0x0000
#define TCON_CTRL_TIMING_BASE                      0x01b0
#define TCON_TOP_CTRL                              0x2000
#define TCON_RGB_IN_MUX                            0x2001
#define TCON_OUT_CH_SEL0                           0x2002
#define TCON_OUT_CH_SEL1                           0x2003
#define TCON_I2C_DEGLITCH_CNTL                     0x2004
#define TCON_STATUS0                               0x2008 /* read only */
#define TCON_PLLLOCK_CNTL                          0x2009
#define TCON_PLLLCK_RST_CNT                        0x200a
#define TCON_RST_CTRL                              0x200b
#define TCON_AXI_OFST                              0x200c
#define TCON_DDRIF_CTRL0                           0x200d
#define TCON_CLK_CTRL                              0x200e
#define TCON_DDRIF_CTRL1                           0x200f
#define TCON_STATUS1                               0x2010 /* read only */
#define TCON_DDRIF_CTRL2                           0x2011
#define TCON_INTR_MASKN                            0x2022
#define TCON_INTR                                  0x2023 /* read only */

#endif

