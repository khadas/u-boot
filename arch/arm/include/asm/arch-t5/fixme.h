
/*SPIFC*/
#define SPIFC_AHB_REQ_CTRL				PREG_STICKY_REG0
#define SPIFC_AHB_REQ_CTRL1				PREG_STICKY_REG0
#define SPIFC_AHB_REQ_CTRL2				PREG_STICKY_REG0
#define SPIFC_USER_DBUF_ADDR			PREG_STICKY_REG0
#define SPIFC_AHB_CTRL       			PREG_STICKY_REG0
#define CLKTREE_SPIFC_CLK_CTRL 			PREG_STICKY_REG0
#define SPIFC_ACTIMING0      			PREG_STICKY_REG0
#define SPI_ADDR_BASE      			    PREG_STICKY_REG0

#define PADCTRL_GPIOB_DS   			    PREG_STICKY_REG0
#define PADCTRL_PIN_MUX_REG3		    PREG_STICKY_REG0
#define PADCTRL_PIN_MUX_REG4		    PREG_STICKY_REG0
#define PADCTRL_PIN_MUX_REG5		    PREG_STICKY_REG0
#define PADCTRL_GPIOC_DS     		    PREG_STICKY_REG0
#define PADCTRL_GPIOC_OEN     		    PREG_STICKY_REG0
#define PADCTRL_GPIOC_PULL_EN 		    PREG_STICKY_REG0
#define PADCTRL_GPIOC_PULL_UP 		    PREG_STICKY_REG0
#define PADCTRL_GPIOC_I     		    PREG_STICKY_REG0
#define SYSCTRL_SEC_STATUS_REG4         PREG_STICKY_REG0

#define CPUCTRL_CTRL2                   PREG_STICKY_REG0
#define CPUCTRL_RESET_CTRL              PREG_STICKY_REG0

#define SYSCTRL_DEBUG_REG5              PREG_STICKY_REG0

#define SYSCTRL_SEC_STICKY_REG1         PREG_STICKY_REG0

#define SYSCTRL_SEC_STATUS_REG15        PREG_STICKY_REG0
#define SYSCTRL_SEC_STATUS_REG16        PREG_STICKY_REG0
#define SYSCTRL_SEC_STATUS_REG17        PREG_STICKY_REG0

#define HHI_VPU_MEM_PD_REG4             PREG_STICKY_REG0

#define UART_B_WFIFO                    ((0x8c00  << 2) + 0xffd00000)

// need fix
#define VIU_EOTF_LUT_ADDR_PORT 0x31d6
#define VIU_EOTF_CTL 0x31d0
#define VIU_EOTF_LUT_DATA_PORT 0x31d7

#ifndef _BASE_REGISTER
#define _BASE_REGISTER
#define REG_BASE_AOBUS                  (0xFF800000L)
#define REG_BASE_PERIPHS                (0xFF634000L)
#define REG_BASE_CBUS                   (0xFFD00000L)
#define REG_BASE_HIU                    (0xFF63C000L)
#define REG_BASE_VCBUS                  (0xFF900000L)
#define DMC_REG_BASE                    (0xFF638000L)
#define REG_TCON_APB_BASE               (0xFF660000L)

#endif /*_BASE_REGISTER*/

#define L_GAMMA_CNTL_PORT                          ((0x1400  << 2) + 0xff900000)
#define L_GAMMA_DATA_PORT                          ((0x1401  << 2) + 0xff900000)
#define L_GAMMA_ADDR_PORT                          ((0x1402  << 2) + 0xff900000)

#define VIU_MISC_CTRL0                             ((0x1a06  << 2) + 0xff900000)
#define VIU_MISC_CTRL1                             ((0x1a07  << 2) + 0xff900000)

#define VIU_OSD1_BLK0_CFG_W0                       ((0x1a1b  << 2) + 0xff900000)

#define VIU_OSD1_MATRIX_CTRL                       ((0x1a90  << 2) + 0xff900000)
#define VIU_OSD1_MATRIX_COEF00_01                  ((0x1a91  << 2) + 0xff900000)
#define VIU_OSD1_MATRIX_COEF02_10                  ((0x1a92  << 2) + 0xff900000)
#define VIU_OSD1_MATRIX_COEF11_12                  ((0x1a93  << 2) + 0xff900000)
#define VIU_OSD1_MATRIX_COEF20_21                  ((0x1a94  << 2) + 0xff900000)
#define VIU_OSD1_MATRIX_COLMOD_COEF42              ((0x1a95  << 2) + 0xff900000)
#define VIU_OSD1_MATRIX_OFFSET0_1                  ((0x1a96  << 2) + 0xff900000)
#define VIU_OSD1_MATRIX_OFFSET2                    ((0x1a97  << 2) + 0xff900000)
#define VIU_OSD1_MATRIX_PRE_OFFSET0_1              ((0x1a98  << 2) + 0xff900000)
#define VIU_OSD1_MATRIX_PRE_OFFSET2                ((0x1a99  << 2) + 0xff900000)

#define VIU_OSD1_MATRIX_COEF22_30                  ((0x1a9d  << 2) + 0xff900000)
#define VIU_OSD1_MATRIX_COEF31_32                  ((0x1a9e  << 2) + 0xff900000)
#define VIU_OSD1_MATRIX_COEF40_41                  ((0x1a9f  << 2) + 0xff900000)

#define VIU_OSD1_EOTF_CTL                          ((0x1ad4  << 2) + 0xff900000)
#define VIU_OSD1_EOTF_LUT_ADDR_PORT                ((0x1ada  << 2) + 0xff900000)
#define VIU_OSD1_EOTF_LUT_DATA_PORT                ((0x1adb  << 2) + 0xff900000)
#define VIU_OSD1_OETF_CTL                          ((0x1adc  << 2) + 0xff900000)
#define VIU_OSD1_OETF_LUT_ADDR_PORT                ((0x1add  << 2) + 0xff900000)
#define VIU_OSD1_OETF_LUT_DATA_PORT                ((0x1ade  << 2) + 0xff900000)

#define VENC_INTCTRL                               ((0x1b6e  << 2) + 0xff900000)

#define ENCL_VIDEO_EN                              ((0x1ca0  << 2) + 0xff900000)

#define VPP_POSTBLEND_H_SIZE                       ((0x1d21  << 2) + 0xff900000)
#define VPP_HOLD_LINES                             ((0x1d22  << 2) + 0xff900000)
#define VPP_OFIFO_SIZE                             ((0x1d27  << 2) + 0xff900000)
#define VPP_VADJ_CTRL                              ((0x1d40  << 2) + 0xff900000)

#define VPP_MATRIX_CTRL                            ((0x1d5f  << 2) + 0xff900000)
#define VPP_MATRIX_COEF00_01                       ((0x1d60  << 2) + 0xff900000)
#define VPP_MATRIX_COEF02_10                       ((0x1d61  << 2) + 0xff900000)
#define VPP_MATRIX_COEF11_12                       ((0x1d62  << 2) + 0xff900000)
#define VPP_MATRIX_COEF20_21                       ((0x1d63  << 2) + 0xff900000)
#define VPP_MATRIX_COEF22                          ((0x1d64  << 2) + 0xff900000)
#define VPP_MATRIX_OFFSET0_1                       ((0x1d65  << 2) + 0xff900000)
#define VPP_MATRIX_OFFSET2                         ((0x1d66  << 2) + 0xff900000)
#define VPP_MATRIX_PRE_OFFSET0_1                   ((0x1d67  << 2) + 0xff900000)
#define VPP_MATRIX_PRE_OFFSET2                     ((0x1d68  << 2) + 0xff900000)
#define VPP_DUMMY_DATA1                            ((0x1d69  << 2) + 0xff900000)

#define VPP_DOLBY_CTRL                             ((0x1d93  << 2) + 0xff900000)
#define VPP_MATRIX_COEF13_14                       ((0x1ddb  << 2) + 0xff900000)
#define VPP_MATRIX_COEF23_24                       ((0x1ddc  << 2) + 0xff900000)
#define VPP_MATRIX_COEF15_25                       ((0x1ddd  << 2) + 0xff900000)
#define VPP_MATRIX_CLIP                            ((0x1dde  << 2) + 0xff900000)

#define VPU_VIU_VENC_MUX_CTRL                      ((0x271a  << 2) + 0xff900000)
#define VPU_RDARB_MODE_L1C1                        ((0x2790  << 2) + 0xff900000)
#define VPU_RDARB_MODE_L1C2                        ((0x2799  << 2) + 0xff900000)
#define VPU_RDARB_MODE_L2C1                        ((0x279d  << 2) + 0xff900000)
#define VPU_WRARB_MODE_L2C1                        ((0x27a2  << 2) + 0xff900000)

#define XVYCC_LUT_R_DATA_PORT                      ((0x315f  << 2) + 0xff900000)
#define XVYCC_LUT_CTL                              ((0x3165  << 2) + 0xff900000)
#define XVYCC_LUT_R_ADDR_PORT                      ((0x315e  << 2) + 0xff900000)

#define VPP_VADJ2_Y                                ((0x32a2  << 2) + 0xff900000)
#define VPP_VADJ2_MA_MB                            ((0x32a3  << 2) + 0xff900000)
#define VPP_VADJ2_MC_MD                            ((0x32a4  << 2) + 0xff900000)


#define     HHI_VPU_MEM_PD_REG0                    (0xff63c000 + (0x041 << 2))
#define SEC_HHI_VPU_MEM_PD_REG0                    (0xff63c000 + (0x041 << 2))
#define   P_HHI_VPU_MEM_PD_REG0                    (volatile uint32_t *)(0xff63c000 + (0x041 << 2))

#define     HHI_VPU_MEM_PD_REG1                    (0xff63c000 + (0x042 << 2))
#define SEC_HHI_VPU_MEM_PD_REG1                    (0xff63c000 + (0x042 << 2))
#define   P_HHI_VPU_MEM_PD_REG1                    (volatile uint32_t *)(0xff63c000 + (0x042 << 2))

#define P_WATCHDOG_CNTL                            (volatile uint32_t *)((0x3c34  << 2) + 0xffd00000)
#define P_WATCHDOG_CNTL1                           (volatile uint32_t *)((0x3c35  << 2) + 0xffd00000)
#define P_WATCHDOG_TCNT                            (volatile uint32_t *)((0x3c36  << 2) + 0xffd00000)
#define P_WATCHDOG_RESET                           (volatile uint32_t *)((0x3c37  << 2) + 0xffd00000)

#define P_RESET1_REGISTER                          (volatile unsigned int *)((0x0402  << 2) + 0xffd00000)
#define P_RESET1_LEVEL                             (volatile uint32_t *)((0x0421  << 2) + 0xffd00000)

//#define ENCL_INFO_READ                             ((0x271f  << 2) + 0xff900000)
//#define ENCL_VIDEO_MODE                            ((0x1ca7  << 2) + 0xff900000)
//#define ENCL_VIDEO_MODE_ADV                        ((0x1ca8  << 2) + 0xff900000)
//#define ENCL_VIDEO_MAX_PXCNT                       ((0x1cb0  << 2) + 0xff900000)
//#define ENCL_VIDEO_HAVON_END                       ((0x1cb1  << 2) + 0xff900000)
//#define ENCL_VIDEO_HAVON_BEGIN                     ((0x1cb2  << 2) + 0xff900000)
//#define ENCL_VIDEO_VAVON_ELINE                     ((0x1cb3  << 2) + 0xff900000)
//#define ENCL_VIDEO_VAVON_BLINE                     ((0x1cb4  << 2) + 0xff900000)
//#define ENCL_VIDEO_HSO_BEGIN                       ((0x1cb5  << 2) + 0xff900000)
//#define ENCL_VIDEO_HSO_END                         ((0x1cb6  << 2) + 0xff900000)
//#define ENCL_VIDEO_VSO_BEGIN                       ((0x1cb7  << 2) + 0xff900000)
//#define ENCL_VIDEO_VSO_END                         ((0x1cb8  << 2) + 0xff900000)
//#define ENCL_VIDEO_VSO_BLINE                       ((0x1cb9  << 2) + 0xff900000)
//#define ENCL_VIDEO_VSO_ELINE                       ((0x1cba  << 2) + 0xff900000)
//#define ENCL_VIDEO_MAX_LNCNT                       ((0x1cbb  << 2) + 0xff900000)
//#define ENCL_VIDEO_RGB_CTRL                        ((0x1cc1  << 2) + 0xff900000)
//#define ENCL_VIDEO_RGBIN_CTRL                      ((0x1cc7  << 2) + 0xff900000)
//#define L_RGB_BASE_ADDR                            ((0x1405  << 2) + 0xff900000)
//#define L_RGB_COEFF_ADDR                           ((0x1406  << 2) + 0xff900000)
//#define L_POL_CNTL_ADDR                            ((0x1407  << 2) + 0xff900000)
//#define L_DITH_CNTL_ADDR                           ((0x1408  << 2) + 0xff900000)
//#define RESET7_REGISTER                            ((0x0408  << 2) + 0xffd00000)
//#define PWM_MISC_REG_AB                            ((0x6c02  << 2) + 0xffd00000)
//#define PWM_MISC_REG_CD                            ((0x6802  << 2) + 0xffd00000)
//#define PWM_PWM_A                                  ((0x6c00  << 2) + 0xffd00000)
//#define PWM_PWM_B                                  ((0x6c01  << 2) + 0xffd00000)
//#define PWM_PWM_C                                  ((0x6800  << 2) + 0xffd00000)
//#define PWM_PWM_D                                  ((0x6801  << 2) + 0xffd00000)

//#define L_DUAL_PORT_CNTL_ADDR                      ((0x1442  << 2) + 0xff900000)
//#define L_STH1_HS_ADDR                             ((0x1410  << 2) + 0xff900000)

//#define L_STH1_VS_ADDR                             ((0x1412  << 2) + 0xff900000)
//#define L_STH1_VE_ADDR                             ((0x1413  << 2) + 0xff900000)
//#define L_STV1_HS_ADDR                             ((0x1427  << 2) + 0xff900000)
//#define L_STV1_HE_ADDR                             ((0x1428  << 2) + 0xff900000)
//#define L_STV1_VS_ADDR                             ((0x1429  << 2) + 0xff900000)
//#define L_STV1_VE_ADDR                             ((0x142a  << 2) + 0xff900000)

//#define L_OEH_HS_ADDR                              ((0x1418  << 2) + 0xff900000)
//#define L_OEH_HE_ADDR                              ((0x1419  << 2) + 0xff900000)
//#define L_OEH_VS_ADDR                              ((0x141a  << 2) + 0xff900000)
//#define L_OEH_VE_ADDR                              ((0x141b  << 2) + 0xff900000)

//#define LVDS_PACK_CNTL_ADDR                        ((0x14d0  << 2) + 0xff900000)
//#define LVDS_GEN_CNTL                              ((0x14e0  << 2) + 0xff900000)


//define LCD_PORT_SWAP                              ((0x1476  << 2) + 0xff900000)

//#define VPU_VPU_PWM_V1                             ((0x2731  << 2) + 0xff900000)
//#define VPU_VPU_PWM_V2                             ((0x2732  << 2) + 0xff900000)
//#define VPU_VPU_PWM_V3                             ((0x2733  << 2) + 0xff900000)

//#define VBO_FSM_HOLDER_L                           ((0x147a  << 2) + 0xff900000)
//#define VBO_FSM_HOLDER_H                           ((0x147b  << 2) + 0xff900000)
//#define VBO_INTR_STATE_CTRL                        ((0x147c  << 2) + 0xff900000)
//#define VBO_INTR_UNMASK                            ((0x147d  << 2) + 0xff900000)
//#define VBO_INSGN_CTRL                             ((0x14fa  << 2) + 0xff900000)

//#define TCON_CLK_CTRL                              0x200e
//#define P_TCON_CLK_CTRL                            (volatile unsigned int *)((0x200e << 2) + 0xff660000)
//#define TCON_TOP_CTRL                              0x2000
//#define P_TCON_TOP_CTRL                            (volatile unsigned int *)((0x2000 << 2) + 0xff660000)
//#define TCON_PLLLOCK_CNTL                          0x2009
//#define P_TCON_PLLLOCK_CNTL                        (volatile unsigned int *)((0x2009 << 2) + 0xff660000)
//#define TCON_RGB_IN_MUX                            0x2001
//#define P_TCON_RGB_IN_MUX                          (volatile unsigned int *)((0x2001 << 2) + 0xff660000)
//#define TCON_DDRIF_CTRL0                           0x200d
//#define P_TCON_DDRIF_CTRL0                         (volatile unsigned int *)((0x200d << 2) + 0xff660000)
//#define TCON_DDRIF_CTRL1                           0x200f
//#define P_TCON_DDRIF_CTRL1                         (volatile unsigned int *)((0x200f << 2) + 0xff660000)
//#define TCON_CORE_REG_START                        (0x0000)
//#define P_TCON_CORE_REG_START                      (volatile unsigned int *)((0x0000 << 2) + 0xff660000)
//#define TCON_RST_CTRL                              0x200b
//#define P_TCON_RST_CTRL                            (volatile unsigned int *)((0x200b << 2) + 0xff660000)
//#define TCON_OUT_CH_SEL0                           0x2002
//#define P_TCON_OUT_CH_SEL0                         (volatile unsigned int *)((0x2002 << 2) + 0xff660000)
//#define TCON_OUT_CH_SEL1                           0x2003
//#define P_TCON_OUT_CH_SEL1                         (volatile unsigned int *)((0x2003 << 2) + 0xff660000)
//#define TCON_RST_CTRL                              0x200b
//#define P_TCON_RST_CTRL                            (volatile unsigned int *)((0x200b << 2) + 0xff660000)

//#define DC_CAV_LUT_DATAL                           (0x0012 << 2)
//#define P_DC_CAV_LUT_DATAL                         (volatile unsigned int *)((0x0012  << 2) + 0xff638000)

