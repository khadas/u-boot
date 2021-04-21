/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _ROM_FIXME_H_
#define _ROM_FIXME_H_

#define ROM_FIXME 	1

#ifdef ROM_FIXME

/*OTP*/
#define OTP_LIC0                         (SYSCTRL_STICKY_REG0)
#define OTP_LIC1                         (SYSCTRL_STICKY_REG0)
#define OTP_LIC2                         (SYSCTRL_STICKY_REG0)
#define OTP_LIC3                         (SYSCTRL_STICKY_REG0)

/*PLL*/
#define HHI_GP1_PLL_CNTL0                (SYSCTRL_STICKY_REG0)
#define HHI_GP1_PLL_CNTL1                (SYSCTRL_STICKY_REG0)
#define HHI_GP1_PLL_CNTL2                (SYSCTRL_STICKY_REG0)
#define HHI_GP1_PLL_CNTL3                (SYSCTRL_STICKY_REG0)
#define HHI_GP1_PLL_CNTL4                (SYSCTRL_STICKY_REG0)
#define HHI_GP1_PLL_CNTL5                (SYSCTRL_STICKY_REG0)
#define HHI_GP1_PLL_CNTL6                (SYSCTRL_STICKY_REG0)

#define HHI_SYS_CPU_CLK_CNTL0            (SYSCTRL_STICKY_REG0)
#define HHI_SYS_CPU_CLK_CNTL1            (SYSCTRL_STICKY_REG0)
#define HHI_SYS_CPU_CLK_CNTL2            (SYSCTRL_STICKY_REG0)
#define HHI_SYS_CPU_CLK_CNTL3            (SYSCTRL_STICKY_REG0)
#define HHI_SYS_CPU_CLK_CNTL4            (SYSCTRL_STICKY_REG0)
#define HHI_SYS_CPU_CLK_CNTL5            (SYSCTRL_STICKY_REG0)
#define HHI_SYS_CPU_CLK_CNTL6            (SYSCTRL_STICKY_REG0)

#define HHI_FIX_PLL_CNTL0                (SYSCTRL_STICKY_REG0)
#define HHI_FIX_PLL_CNTL1                (SYSCTRL_STICKY_REG0)
#define HHI_FIX_PLL_CNTL2                (SYSCTRL_STICKY_REG0)
#define HHI_FIX_PLL_CNTL3                (SYSCTRL_STICKY_REG0)
#define HHI_FIX_PLL_CNTL4                (SYSCTRL_STICKY_REG0)
#define HHI_FIX_PLL_CNTL5                (SYSCTRL_STICKY_REG0)
#define HHI_FIX_PLL_CNTL6                (SYSCTRL_STICKY_REG0)
#define P_HHI_FIX_PLL_CNTL0              (SYSCTRL_STICKY_REG0)

#define P_HHI_MPEG_CLK_CNTL              (SYSCTRL_STICKY_REG0)

#define HHI_SYS_PLL_CNTL0                (SYSCTRL_STICKY_REG0)
#define HHI_SYS_PLL_CNTL1                (SYSCTRL_STICKY_REG0)
#define HHI_SYS_PLL_CNTL2                (SYSCTRL_STICKY_REG0)
#define HHI_SYS_PLL_CNTL3                (SYSCTRL_STICKY_REG0)
#define HHI_SYS_PLL_CNTL4                (SYSCTRL_STICKY_REG0)
#define HHI_SYS_PLL_CNTL5                (SYSCTRL_STICKY_REG0)
#define HHI_SYS_PLL_CNTL6                (SYSCTRL_STICKY_REG0)

#define HHI_SYS_CPU_CLK_CNTL             (SYSCTRL_STICKY_REG0)

/*ADC*/
#define P_AO_SAR_ADC_REG11               (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_AO_SAR_ADC_REG12               (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_AO_SAR_ADC_REG13               (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_AO_SAR_ADC_REG3                (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_AO_SAR_CLK                     (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_AO_SAR_ADC_REG0                (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_AO_SAR_ADC_CHAN_LIST           (volatile uint32_t *)(SYSCTRL_STICKY_REG0)

#define P_AO_SAR_ADC_AVG_CNTL            (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_AO_SAR_ADC_DELAY               (volatile uint32_t *)(SYSCTRL_STICKY_REG0)

#define P_AO_SAR_ADC_AUX_SW              (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_AO_SAR_ADC_CHAN_10_SW          (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_AO_SAR_ADC_DETECT_IDLE_SW      (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_AO_SAR_ADC_FIFO_RD             (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_AO_SAR_ADC_AVG_CNTL            (volatile uint32_t *)(SYSCTRL_STICKY_REG0)


/*SPI*/
#define SPI_START_ADDR                   (SYSCTRL_STICKY_REG0)

#define P_PAD_PULL_UP_EN_REG0            (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_PERIPHS_PIN_MUX_0              (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define P_PERIPHS_PIN_MUX_1              (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define AO_RTI_PINMUX_REG0               (SYSCTRL_STICKY_REG0)
#define AO_GPIO_O_EN_N                   (SYSCTRL_STICKY_REG0)
#define AO_GPIO_I                        (SYSCTRL_STICKY_REG0)
#define P_PAD_PULL_UP_REG0               (volatile uint32_t *)(SYSCTRL_STICKY_REG0)
#define AO_RTI_PULL_UP_EN_REG            (SYSCTRL_STICKY_REG0)
#define AO_RTI_PULL_UP_REG               (SYSCTRL_STICKY_REG0)
#define P_AO_PAD_DS_B                    (SYSCTRL_STICKY_REG0)

/*TIMER*/
//#define ISA_TIMERE                       (SYSCTRL_STICKY_REG0)
//#define P_ISA_TIMERE                     (SYSCTRL_STICKY_REG0)

/*DMC*/
#define P_PREG_STICKY_REG0               (SYSCTRL_STICKY_REG0)

/*power*/
#define HHI_MEM_PD_REG0                  (SYSCTRL_STICKY_REG0)

/*UART*/
//#define P_AO_TIMEBASE_CNTL1              (SYSCTRL_STICKY_REG0)
//#define P_AO_RTI_PINMUX_REG0             (SYSCTRL_STICKY_REG0)

#endif


#endif
