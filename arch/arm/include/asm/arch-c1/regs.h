/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __AML_REG_H__
#define __AML_REG_H__
/*
 * all used regs
 */
/*
 * ISA_TIMERE use SYSCTRL_TIMERE
 * WATCHDOG_CNTL use RESETCTRL_WATCHDOG_CTRL0
 * WATCHDOG_TCNT use RESETCTRL_WATCHDOG_CNT
 * WATCHDOG_RST use RESETCTRL_WATCHDOG_CLR
 */
#define ISA_TIMERE			((0x0041  << 2) + 0xfe005800)
#define WATCHDOG_CNTL			((0x0040  << 2) + 0xfe000000)
#define WATCHDOG_TCNT			((0x0042  << 2) + 0xfe000000)
#define WATCHDOG_RST			((0x0043  << 2) + 0xfe000000)

/*
 * SEC_AO_SEC_GP_CFG0 use SYSCTRL_SEC_STATUS_REG4
 * AO_SEC_GP_CFG0 use SYSCTRL_SEC_STATUS_REG4
 * AO_RTI_STATUS_REG3 use SYSCTRL_SEC_STICKY_REG1
 * AO_SEC_SD_CFG15 use SYSCTRL_SEC_STATUS_REG2
 */
#define AO_SEC_GP_CFG0			((0x00c4  << 2) + 0xfe005800)
#define AO_RTI_STATUS_REG3		((0x00e1  << 2) + 0xfe005800)
#define AO_SEC_SD_CFG15                 ((0x00c2  << 2) + 0xfe005800)

/* PREG_STICKY_REG8  use SYSCTRL_STICKY_REG7
 * P_AO_SEC_GP_CFG12 use SYSCTRL_DEBUG_REG6
 * P_AO_SEC_GP_CFG13 use SYSCTRL_DEBUG_REG7
 */
#define PREG_STICKY_REG8		((0x00b7  << 2) + 0xfe005800)
#define P_AO_SEC_GP_CFG12		(volatile uint32_t *)((0x0096  << 2) + 0xfe005800)
#define P_AO_SEC_GP_CFG13		(volatile uint32_t *)((0x0097  << 2) + 0xfe005800)

/*
 * P_AO_RTI_PINMUX_REG0 use PADCTRL_PIN_MUX_REG0
 * P_AO_TIMEBASE_CNTL1 use CLKTREE_TIMEBASE_CTRL1
 * AO_UART_WFIFO use UART_B_WFIFO
 */
#define P_AO_RTI_PINMUX_REG0		((0x0000  << 2) + 0xfe000400)
#define P_AO_TIMEBASE_CNTL1		((0x0016  << 2) + 0xfe000800)
#define AO_UART_WFIFO			((0x0000  << 2) + 0xfe002000)

/*
 * Not AO watch dog
 */
#define P_WATCHDOG_CNTL			(volatile uint32_t *)(WATCHDOG_CNTL)
#define P_WATCHDOG_TCNT			(volatile uint32_t *)(WATCHDOG_TCNT)
#define P_WATCHDOG_RST			(volatile uint32_t *)(WATCHDOG_RST)
#define P_ISA_TIMERE			(volatile uint32_t *)(ISA_TIMERE)

#endif /* __AML_REG_H__ */
