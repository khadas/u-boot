/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __AML_REG_H__
#define __AML_REG_H__
/*
 * all ramdump used regs
 */
/*
 * REG_MDUMP_ISA_TIMERE    use ISA_TIMERE    (SYSCTRL_TIMERE)
 * REG_MDUMP_WATCHDOG_CNTL use WATCHDOG_CNTL (RESETCTRL_WATCHDOG_CTRL0)
 * REG_MDUMP_WATCHDOG_TCNT use WATCHDOG_TCNT (RESETCTRL_WATCHDOG_CNT)
 * REG_MDUMP_WATCHDOG_RST  use WATCHDOG_RST  (RESETCTRL_WATCHDOG_CLR)
 */
#define REG_MDUMP_ISA_TIMERE        (volatile uint32_t *)((0x0041  << 2) + 0xfe005800)
#define REG_MDUMP_WATCHDOG_CNTL     (volatile uint32_t *)((0x0040  << 2) + 0xfe000000)
#define REG_MDUMP_WATCHDOG_TCNT     (volatile uint32_t *)((0x0042  << 2) + 0xfe000000)
#define REG_MDUMP_WATCHDOG_RST      (volatile uint32_t *)((0x0043  << 2) + 0xfe000000)

/*
 * REG_MDUMP_CPUBOOT_STATUS  use SYSCTRL_STICKY_REG6/8
 * REG_MDUMP_COMPRESS_BASE   use SYSCTRL_DEBUG_REG6 (P_AO_SEC_GP_CFG12)
 * REG_MDUMP_COMPRESS_SIZE   use SYSCTRL_DEBUG_REG7 (P_AO_SEC_GP_CFG13)
 * REG_MDUMP_REBOOT_MODE     use AO_SEC_SD_CFG15 (AO_RTI_STATUS_REG3)
 */
#define REG_MDUMP_CPUBOOT_STATUS    ((0x00b3  << 2) + 0xfe005800)/* boot status */
#define REG_MDUMP_COMPRESS_BASE     ((0x00cd  << 2) + 0xfe005800)/* ram compress size */
#define REG_MDUMP_COMPRESS_SIZE     ((0x00ce  << 2) + 0xfe005800) /* ram compress start addrs */
#define REG_MDUMP_REBOOT_MODE_ORI   ((0x00e1  << 2) + 0xfe005800) /* reboot mode older(bit0-12) */
#define REG_MDUMP_REBOOT_MODE       ((0x00c2  << 2) + 0xfe005800) /* reboot mode new(bit12-24) */

/*
 * REG_MDUMP_SERIAL_PINMUX   use P_AO_RTI_PINMUX_REG0 (PADCTRL_PIN_MUX_REG0)
 * REG_MDUMP_TIMEBASE_CNTL   use P_AO_TIMEBASE_CNTL1 (CLKTREE_TIMEBASE_CTRL1)
 * REG_MDUMP_UART_WFIFO      use UART_A_WFIFO(0xfe078000)  or  UART_B_WFIFO(0xfe07a000)
 * #define CONSOLE_USE_UART_B  in  bl2/core/plat/sc2/include/platform_def.h:351
 */
#define REG_MDUMP_SERIAL_PINMUX     ((0x0000  << 2) + 0xfe000400)
#define REG_MDUMP_TIMEBASE_CNTL     ((0x0016  << 2) + 0xfe000800)
#define REG_MDUMP_UART_WFIFO        ((0x0000  << 2) + 0xfe002000)

#endif /* __AML_REG_H__ */
