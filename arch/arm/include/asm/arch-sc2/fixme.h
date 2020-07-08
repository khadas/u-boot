#ifndef _ROM_FIXME_H_
#define _ROM_FIXME_H_

#define ROM_FIXME 	1

#ifdef ROM_FIXME

/*AO*/
#define AO_METAL_REVISION                (SYSCTRL_STICKY_REG0)

#define SEC_AO_RTI_STATUS_REG3           (SYSCTRL_STICKY_REG0)

#define P_AO_SEC_SD_CFG0                 (SYSCTRL_STICKY_REG0)
#define P_AO_SEC_GP_CFG0                 (SYSCTRL_STICKY_REG0)
#define AO_RTI_STATUS_REG3               (SYSCTRL_STICKY_REG0)

#define SEC_AO_SEC_SD_CFG15              (SYSCTRL_STICKY_REG0)

#define SEC_AO_SEC_GP_CFG0               (SYSCTRL_STICKY_REG0)
#define SEC_AO_SEC_GP_CFG1               (SYSCTRL_STICKY_REG0)
#define SEC_AO_SEC_GP_CFG2               (SYSCTRL_STICKY_REG0)
#define SEC_AO_SEC_GP_CFG3               (SYSCTRL_STICKY_REG0)
#define SEC_AO_SEC_GP_CFG4               (SYSCTRL_STICKY_REG0)
#define SEC_AO_SEC_GP_CFG5               (SYSCTRL_STICKY_REG0)
#define SEC_AO_SEC_GP_CFG6               (SYSCTRL_STICKY_REG0)
#define SEC_AO_SEC_GP_CFG7               (SYSCTRL_STICKY_REG0)

#define AO_SEC_SD_CFG0                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG1                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG2                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG3                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG4                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG5                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG6                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG7                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG8                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG9                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG10                  (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG11                  (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG12                  (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG13                  (SYSCTRL_STICKY_REG0)
#define AO_SEC_SD_CFG14                  (SYSCTRL_STICKY_REG0)


#define AO_SEC_GP_CFG0                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_GP_CFG1                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_GP_CFG2                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_GP_CFG3                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_GP_CFG4                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_GP_CFG5                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_GP_CFG6                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_GP_CFG7                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_GP_CFG8                   (SYSCTRL_STICKY_REG0)
#define AO_SEC_GP_CFG9                   (SYSCTRL_STICKY_REG0)

/*maibox*/
#define SEC_HIU_MAILBOX_SET_0            (SYSCTRL_STICKY_REG0)
#define SEC_HIU_MAILBOX_STAT_0           (SYSCTRL_STICKY_REG0)
#define SEC_HIU_MAILBOX_CLR_0            (SYSCTRL_STICKY_REG0)

/*WDC*/
#define WATCHDOG_CNTL                    (SYSCTRL_STICKY_REG0)
#define WATCHDOG_CNTL1                   (SYSCTRL_STICKY_REG0)
#define WATCHDOG_TCNT                    (SYSCTRL_STICKY_REG0)
#define WATCHDOG_RESET                   (SYSCTRL_STICKY_REG0)
#define P_WATCHDOG_CNTL                  (volatile uint32_t *)(WATCHDOG_CNTL)
#define P_WATCHDOG_TCNT                  (volatile uint32_t *)(WATCHDOG_TCNT)
#define P_WATCHDOG_RESET                 (volatile uint32_t *)(WATCHDOG_RESET)


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
#define ISA_TIMERE                       (SYSCTRL_STICKY_REG0)
#define P_ISA_TIMERE                     (SYSCTRL_STICKY_REG0)

/*DMC*/
#define P_PREG_STICKY_REG0               (SYSCTRL_STICKY_REG0)

/*power*/
#define HHI_MEM_PD_REG0                  (SYSCTRL_STICKY_REG0)

/*UART*/
#define P_AO_TIMEBASE_CNTL1              (SYSCTRL_STICKY_REG0)
#define P_AO_RTI_PINMUX_REG0             (SYSCTRL_STICKY_REG0)

/*for SC2 DSP compatible*/
#define CLKTREE_DSPA_CLK_CTRL0 CLKCTRL_DSPA_CLK_CTRL0
#define CLKTREE_DSPB_CLK_CTRL0 CLKCTRL_DSPB_CLK_CTRL0

#endif


#endif
