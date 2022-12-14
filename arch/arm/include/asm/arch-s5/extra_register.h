
#ifndef FIXME_H
#define FIXME_H

#define OTP_TEE_RDY                                ((0x0000  << 2) + 0xfe440000)
#define OTP_TEE_DEBUG                              ((0x0001  << 2) + 0xfe440000)
#define OTP_TEE_CFG                                ((0x0002  << 2) + 0xfe440000)

#define OTP_LIC                                    ((0x0010  << 2) + 0xfe440000)
#define OTP_LIC00                                  (OTP_LIC + 0x00)
#define OTP_LIC01                                  (OTP_LIC + 0x04)
#define OTP_LIC02                                  (OTP_LIC + 0x08)
#define OTP_LIC03                                  (OTP_LIC + 0x0C)

#define OTP_LIC10                                  (OTP_LIC + 0x10)
#define OTP_LIC11                                  (OTP_LIC + 0x14)
#define OTP_LIC12                                  (OTP_LIC + 0x18)
#define OTP_LIC13                                  (OTP_LIC + 0x1C)

#define OTP_LIC20                                  (OTP_LIC + 0x20)
#define OTP_LIC21                                  (OTP_LIC + 0x24)
#define OTP_LIC22                                  (OTP_LIC + 0x28)
#define OTP_LIC23                                  (OTP_LIC + 0x2C)

#define OTP_LIC30                                  (OTP_LIC + 0x30)
#define OTP_LIC31                                  (OTP_LIC + 0x34)
#define OTP_LIC32                                  (OTP_LIC + 0x38)
#define OTP_LIC33                                  (OTP_LIC + 0x3C)

#define AO_SEC_SD_CFG10							   (0xff800000 + (0x08a << 2))
#define SEC_AO_SEC_GP_CFG0            				SYSCTRL_SEC_STATUS_REG4
#define SEC_AO_SEC_GP_CFG2  						SYSCTRL_SEC_STATUS_REG6
#define SYSCTRL_SEC_STATUS_REG4                    ((0x00c4  << 2) + 0xfe010000)
#define SYSCTRL_SEC_STATUS_REG6                    ((0x00c6  << 2) + 0xfe010000)

#define CLKCTRL_DSPA_CLK_CTRL0                     ((0x0027  << 2) + 0xfe000000)
#define CLKCTRL_DSPB_CLK_CTRL0                     ((0x0028  << 2) + 0xfe000000)

#define ANACTRL_SYS1PLL_CTRL1                      ((0x0009  << 2) + 0xfe008000)
#define ANACTRL_SYS1PLL_CTRL2                      ((0x000a  << 2) + 0xfe008000)
#define ANACTRL_SYS1PLL_CTRL3                      ((0x000b  << 2) + 0xfe008000)

#define ANACTRL_FIXPLL_CTRL1                       ((0x0011  << 2) + 0xfe008000)
#define ANACTRL_FIXPLL_CTRL2                       ((0x0012  << 2) + 0xfe008000)
#define ANACTRL_FIXPLL_CTRL3                       ((0x0013  << 2) + 0xfe008000)
#define ANACTRL_FIXPLL_CTRL4                       ((0x0014  << 2) + 0xfe008000)
#define ANACTRL_FIXPLL_CTRL5                       ((0x0015  << 2) + 0xfe008000)
#define ANACTRL_FIXPLL_CTRL6                       ((0x0016  << 2) + 0xfe008000)

#define CLKCTRL_FIXPLL_CTRL4                       ((0x0224  << 2) + 0xfe000000)
#define CLKCTRL_FIXPLL_CTRL5                       ((0x0225  << 2) + 0xfe000000)
#define CLKCTRL_FIXPLL_CTRL6                       ((0x0226  << 2) + 0xfe000000)

#define VDIN_TOP_SECURE_REG0                       ((0x410e  << 2) + 0xff000000)
#define VDIN_TOP_SECURE_REG1                       ((0x410f  << 2) + 0xff000000)

#define ANACTRL_FIXPLL_CTRL0                       ((0x0010  << 2) + 0xfe008000)
#define ANACTRL_SYS0PLL_CTRL0                      ((0x0000  << 2) + 0xfe008000)
#define ANACTRL_SYS0PLL_CTRL1                      ((0x0001  << 2) + 0xfe008000)
#define ANACTRL_SYS0PLL_CTRL2                      ((0x0002  << 2) + 0xfe008000)
#define ANACTRL_SYS0PLL_CTRL3                      ((0x0003  << 2) + 0xfe008000)

#define ANACTRL_SYS1PLL_CTRL0                      ((0x0008  << 2) + 0xfe008000)
#define SYSCTRL_VPU_SECURE_REG0                    ((0x0061  << 2) + 0xfe010000)

#define CPUCTRL_SYS_A73_CFG2                       ((0x00c4  << 2) + 0xfe00e000)
#define CPUCTRL_SYS_A73_CFG7                       ((0x00c9  << 2) + 0xfe00e000)
#define CPUCTRL_SYS_A73_CFG8                       ((0x00ca  << 2) + 0xfe00e000)

#define CPUCTRL_SYS_A73_CLK_CTRL0                  ((0x0041  << 2) + 0xfe00e000)
#define CPUCTRL_SYS_A73_CLK_CTRL1                  ((0x0042  << 2) + 0xfe00e000)
#define CPUCTRL_SYS_A73_CLK_CTRL2                  ((0x0043  << 2) + 0xfe00e000)

#define ANACTRL_GP1PLL_CTRL0                       ((0x0030  << 2) + 0xfe008000)
#define DMC0_SEC_STATUS				((0x051a  << 2) + 0xfe036000)
#define DMC0_VIO_ADDR3				((0x051e  << 2) + 0xfe036000)
#define DMC1_SEC_STATUS				((0x051a  << 2) + 0xfe034000)
#define DMC1_VIO_ADDR3				((0x051e  << 2) + 0xfe034000)
#define DMC2_SEC_STATUS				((0x051a  << 2) + 0xfe032000)
#define DMC2_VIO_ADDR3				((0x051e  << 2) + 0xfe032000)
#define DMC3_SEC_STATUS				((0x051a  << 2) + 0xfe030000)
#define DMC3_VIO_ADDR3				((0x051e  << 2) + 0xfe030000)
#endif /*FIXME_H*/
