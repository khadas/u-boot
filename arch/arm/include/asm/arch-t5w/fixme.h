
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

#endif /*_BASE_REGISTER*/

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

/* canvas */
#define DC_CAV_LUT_DATAL                           (0x0012 << 2)
#define P_DC_CAV_LUT_DATAL                         (volatile unsigned int *)((0x0012  << 2) + 0xff638000)
#define DC_CAV_LUT_DATAH                           (0x0013 << 2)
#define P_DC_CAV_LUT_DATAH                         (volatile unsigned int *)((0x0013  << 2) + 0xff638000)
#define DC_CAV_LUT_ADDR                            (0x0014 << 2)
#define P_DC_CAV_LUT_ADDR                          (volatile unsigned int *)((0x0014  << 2) + 0xff638000)
#define DC_CAV_LUT_RDATAL                          (0x0015 << 2)
#define P_DC_CAV_LUT_RDATAL                        (volatile unsigned int *)((0x0015  << 2) + 0xff638000)
#define DC_CAV_LUT_RDATAH                          (0x0016 << 2)
#define P_DC_CAV_LUT_RDATAH                        (volatile unsigned int *)((0x0016  << 2) + 0xff638000)
