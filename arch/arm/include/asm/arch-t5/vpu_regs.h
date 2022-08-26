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
#define L_GAMMA_CNTL_PORT                          0x1400
#define L_GAMMA_DATA_PORT                          0x1401
#define L_GAMMA_ADDR_PORT                          0x1402
#define L_GAMMA_VCOM_HSWITCH_ADDR                  0x1403
#define L_RGB_BASE_ADDR                            0x1405
#define L_RGB_COEFF_ADDR                           0x1406
#define L_POL_CNTL_ADDR                            0x1407
#define L_DITH_CNTL_ADDR                           0x1408
#define L_GAMMA_PROBE_CTRL                         0x1409
//read only
#define L_GAMMA_PROBE_COLOR_L                      0x140a
#define L_GAMMA_PROBE_COLOR_H                      0x140b
#define L_GAMMA_PROBE_HL_COLOR                     0x140c
#define L_GAMMA_PROBE_POS_X                        0x140d
#define L_GAMMA_PROBE_POS_Y                        0x140e
#define L_STH1_HS_ADDR                             0x1410
#define L_STH1_HE_ADDR                             0x1411
#define L_STH1_VS_ADDR                             0x1412
#define L_STH1_VE_ADDR                             0x1413
#define L_STH2_HS_ADDR                             0x1414
#define L_STH2_HE_ADDR                             0x1415
#define L_STH2_VS_ADDR                             0x1416
#define L_STH2_VE_ADDR                             0x1417
#define L_OEH_HS_ADDR                              0x1418
#define L_OEH_HE_ADDR                              0x1419
#define L_OEH_VS_ADDR                              0x141a
#define L_OEH_VE_ADDR                              0x141b
#define L_VCOM_HSWITCH_ADDR                        0x141c
#define L_VCOM_VS_ADDR                             0x141d
#define L_VCOM_VE_ADDR                             0x141e
#define L_CPV1_HS_ADDR                             0x141f
#define L_CPV1_HE_ADDR                             0x1420
#define L_CPV1_VS_ADDR                             0x1421
#define L_CPV1_VE_ADDR                             0x1422
#define L_CPV2_HS_ADDR                             0x1423
#define L_CPV2_HE_ADDR                             0x1424
#define L_CPV2_VS_ADDR                             0x1425
#define L_CPV2_VE_ADDR                             0x1426
#define L_STV1_HS_ADDR                             0x1427
#define L_STV1_HE_ADDR                             0x1428
#define L_STV1_VS_ADDR                             0x1429
#define L_STV1_VE_ADDR                             0x142a
#define L_STV2_HS_ADDR                             0x142b
#define L_STV2_HE_ADDR                             0x142c
#define L_STV2_VS_ADDR                             0x142d
#define L_STV2_VE_ADDR                             0x142e
#define L_OEV1_HS_ADDR                             0x142f
#define L_OEV1_HE_ADDR                             0x1430
#define L_OEV1_VS_ADDR                             0x1431
#define L_OEV1_VE_ADDR                             0x1432
#define L_OEV2_HS_ADDR                             0x1433
#define L_OEV2_HE_ADDR                             0x1434
#define L_OEV2_VS_ADDR                             0x1435
#define L_OEV2_VE_ADDR                             0x1436
#define L_OEV3_HS_ADDR                             0x1437
#define L_OEV3_HE_ADDR                             0x1438
#define L_OEV3_VS_ADDR                             0x1439
#define L_OEV3_VE_ADDR                             0x143a
#define L_LCD_PWR_ADDR                             0x143b
#define L_LCD_PWM0_LO_ADDR                         0x143c
#define L_LCD_PWM0_HI_ADDR                         0x143d
#define L_LCD_PWM1_LO_ADDR                         0x143e
#define L_LCD_PWM1_HI_ADDR                         0x143f
#define L_INV_CNT_ADDR                             0x1440
#define L_TCON_MISC_SEL_ADDR                       0x1441
#define L_DUAL_PORT_CNTL_ADDR                      0x1442
#define MLVDS_CLK_CTL1_HI                          0x1443
#define MLVDS_CLK_CTL1_LO                          0x1444
//  [31:30] enable mlvds clocks
//  [24]    mlvds_clk_half_delay       24 // Bit 0
//  [23:0]  mlvds_clk_pattern           0 // Bit 23:0
#define L_TCON_DOUBLE_CTL                          0x1449
#define L_TCON_PATTERN_HI                          0x144a
#define L_TCON_PATTERN_LO                          0x144b
#define LDIM_BL_ADDR_PORT                          0x144e
#define LDIM_BL_DATA_PORT                          0x144f
#define L_DE_HS_ADDR                               0x1451
#define L_DE_HE_ADDR                               0x1452
#define L_DE_VS_ADDR                               0x1453
#define L_DE_VE_ADDR                               0x1454
#define L_HSYNC_HS_ADDR                            0x1455
#define L_HSYNC_HE_ADDR                            0x1456
#define L_HSYNC_VS_ADDR                            0x1457
#define L_HSYNC_VE_ADDR                            0x1458
#define L_VSYNC_HS_ADDR                            0x1459
#define L_VSYNC_HE_ADDR                            0x145a
#define L_VSYNC_VS_ADDR                            0x145b
#define L_VSYNC_VE_ADDR                            0x145c
// bit 8 -- vfifo_mcu_enable
// bit 7 -- halt_vs_de
// bit 6 -- R8G8B8_format
// bit 5 -- R6G6B6_format (round to 6 bits)
// bit 4 -- R5G6B5_format
// bit 3 -- dac_dith_sel
// bit 2 -- lcd_mcu_enable_de     -- ReadOnly
// bit 1 -- lcd_mcu_enable_vsync  -- ReadOnly
// bit 0 -- lcd_mcu_enable
#define L_LCD_MCU_CTL                              0x145d

//**************************************************************************
//*  Vbyone registers  (Note: no MinLVDS in G9tv, share the register)
//**************************************************************************
#define VBO_CTRL_L                                 0x1460
#define VBO_CTRL_H                                 0x1461
#define VBO_SOFT_RST                               0x1462
#define VBO_LANES                                  0x1463
#define VBO_VIN_CTRL                               0x1464
#define VBO_ACT_VSIZE                              0x1465
#define VBO_REGION_00                              0x1466
#define VBO_REGION_01                              0x1467
#define VBO_REGION_02                              0x1468
#define VBO_REGION_03                              0x1469
#define VBO_VBK_CTRL_0                             0x146a
#define VBO_VBK_CTRL_1                             0x146b
#define VBO_HBK_CTRL                               0x146c
#define VBO_PXL_CTRL                               0x146d
#define VBO_LANE_SKEW_L                            0x146e
#define VBO_LANE_SKEW_H                            0x146f
#define VBO_GCLK_LANE_L                            0x1470
#define VBO_GCLK_LANE_H                            0x1471
#define VBO_GCLK_MAIN                              0x1472
#define VBO_STATUS_L                               0x1473
#define VBO_STATUS_H                               0x1474
#define VBO_LANE_OUTPUT                            0x1475
#define LCD_PORT_SWAP                              0x1476
#define VBO_TMCHK_THRD_L                           0x1478
#define VBO_TMCHK_THRD_H                           0x1479
#define VBO_FSM_HOLDER_L                           0x147a
#define VBO_FSM_HOLDER_H                           0x147b
#define VBO_INTR_STATE_CTRL                        0x147c
#define VBO_INTR_UNMASK                            0x147d
#define VBO_TMCHK_HSYNC_STATE_L                    0x147e
#define VBO_TMCHK_HSYNC_STATE_H                    0x147f
#define VBO_TMCHK_VSYNC_STATE_L                    0x14f4
#define VBO_TMCHK_VSYNC_STATE_H                    0x14f5
#define VBO_TMCHK_VDE_STATE_L                      0x14f6
#define VBO_TMCHK_VDE_STATE_H                      0x14f7
#define VBO_INTR_STATE                             0x14f8
#define VBO_INFILTER_CTRL                          0x14f9
#define VBO_INSGN_CTRL                             0x14fa
#define VBO_INFILTER_CTRL_H                        0x1477

#define LVDS_PACK_CNTL_ADDR                        0x14d0
   #define     LVDS_USE_TCON    7
   #define     LVDS_DUAL        6
   #define     PN_SWP           5
   #define     LSB_FIRST        4
   #define     LVDS_RESV        3
   #define     ODD_EVEN_SWP     2
   #define     LVDS_REPACK      0

#define LVDS_GEN_CNTL                              0x14e0
#define LVDS_CH_SWAP0                              0x14e1
#define LVDS_CH_SWAP1                              0x14e2
#define LVDS_CH_SWAP2                              0x14e3

#define VENC_INTCTRL                               0x1b6e
#define VPP_MISC                                   0x1d26

#define ENCI_INFO_READ                             0x271c
#define ENCP_INFO_READ                             0x271d
#define ENCT_INFO_READ                             0x271e
#define ENCL_INFO_READ                             0x271f

#define VPU_VIU_VENC_MUX_CTRL                      0x271a
#define VPU_RDARB_MODE_L1C1                        0x2790
#define VPU_RDARB_MODE_L1C2                        0x2799
#define VPU_RDARB_MODE_L2C1                        0x279d
#define VPU_WRARB_MODE_L2C1                        0x27a2

//Bit 31    reg_vpu_pwm_inv, 1: invert the pwm signal, active low
//Bit 30:29 reg_vpu_pwm_src_sel, 00: encl, enct, encp
//Bit 28:16 reg_vpu_pwm_v_end0
//Bit 12:0  reg_vpu_pwm_v_start0
#define VPU_VPU_PWM_V0                             0x2730
//Bit 28:16 reg_vpu_pwm_v_end1
//Bit 12:0  reg_vpu_pwm_v_start1
#define VPU_VPU_PWM_V1                             0x2731
//Bit 28:16 reg_vpu_pwm_v_end2
//Bit 12:0  reg_vpu_pwm_v_start2
#define VPU_VPU_PWM_V2                             0x2732
//Bit 28:16 reg_vpu_pwm_v_end3
//Bit 12:0  reg_vpu_pwm_v_start3
#define VPU_VPU_PWM_V3                             0x2733
//Bit 28:16 reg_vpu_pwm_h_end0
//Bit 12:0  reg_vpu_pwm_h_start0
#define VPU_VPU_PWM_H0                             0x2734
//Bit 28:16 reg_vpu_pwm_h_end1
//Bit 12:0  reg_vpu_pwm_h_start1
#define VPU_VPU_PWM_H1                             0x2735
//Bit 28:16 reg_vpu_pwm_h_end2
//Bit 12:0  reg_vpu_pwm_h_start2
#define VPU_VPU_PWM_H2                             0x2736
//Bit 28:16 reg_vpu_pwm_h_end3
//Bit 12:0  reg_vpu_pwm_h_start3
#define VPU_VPU_PWM_H3                             0x2737

#define P2P_CH_SWAP0                               0x4200
#define P2P_CH_SWAP1                               0x4201

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

