/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __MESON_FIRM_UART_H_
#define __MESON_FIRM_UART_H_
#include <config.h>
#include <common.h>
#include "register.h"
//#include "io.h"
#include <asm/arch/secure_apb.h>

#ifndef CONFIG_CONS_INDEX
#error Please define CONFIG_CONS_INDEX==[0|1]
#endif

#if CONFIG_CONS_INDEX == 0
#define UART_PORT_CONS UART_PORT_0
#elif CONFIG_CONS_INDEX == 1
#define UART_PORT_CONS UART_PORT_1
#elif CONFIG_CONS_INDEX == 2
#define UART_PORT_CONS UART_PORT_AO
#define USE_AO_UART   1
#else
#error Please define CONFIG_CONS_INDEX==[0|1]
#endif
/*
#define UART_PORT_0     CBUS_REG_ADDR(UART0_WFIFO)
#define UART_PORT_1     CBUS_REG_ADDR(UART1_WFIFO)
#define UART_PORT_2     CBUS_REG_ADDR(UART2_WFIFO)
#define UART_PORT_AO    P_AO_UART_WFIFO

#define UART_WFIFO      (0<<2)
#define UART_RFIFO      (1<<2)
#define UART_CONTROL    (2<<2)
#define UART_STATUS     (3<<2)
#define UART_MISC       (4<<2)
*/

#include "clock.h"

#define UART_CLK_SRC    CLK81
#define UART_PORT_0     0xfe001c00
#define UART_PORT_1     0xfe001c00
#define UART_PORT_AO    0xfe001c00
#define UART_WFIFO      0
#define UART_RFIFO      1
#define UART_CONTROL    2
#define UART_STATUS     3
#define UART_MISC       4

#if USE_AO_UART == 1
#define P_UART(uart_base, reg)	  (uart_base + (reg << 2))
#else
#define P_UART(uart_base, reg)	CBUS_REG_ADDR(uart_base + reg)
#endif
#define P_UART_WFIFO(uart_base)	P_UART(uart_base, UART_WFIFO)
#define P_UART_RFIFO(uart_base)	P_UART(uart_base, UART_RFIFO)

#define P_UART_CONTROL(uart_base)    P_UART(uart_base, UART_CONTROL)
#define UART_CNTL_MASK_BAUD_RATE                (0xfff)
#define UART_CNTL_MASK_TX_EN                    BIT(12)
#define UART_CNTL_MASK_RX_EN                    BIT(13)
#define UART_CNTL_MASK_2WIRE                    BIT(15)
#define UART_CNTL_MASK_STP_BITS                 (3 << 16)
#define UART_CNTL_MASK_STP_1BIT                 (0 << 16)
#define UART_CNTL_MASK_STP_2BIT                 BIT(16)
#define UART_CNTL_MASK_PRTY_EVEN                (0 << 18)
#define UART_CNTL_MASK_PRTY_ODD                 BIT(18)
#define UART_CNTL_MASK_PRTY_TYPE                BIT(18)
#define UART_CNTL_MASK_PRTY_EN                  BIT(19)
#define UART_CNTL_MASK_CHAR_LEN                 (3 << 20)
#define UART_CNTL_MASK_CHAR_8BIT                (0 << 20)
#define UART_CNTL_MASK_CHAR_7BIT                BIT(20)
#define UART_CNTL_MASK_CHAR_6BIT                (2 << 20)
#define UART_CNTL_MASK_CHAR_5BIT                (3 << 20)
#define UART_CNTL_MASK_RST_TX                   BIT(22)
#define UART_CNTL_MASK_RST_RX                   BIT(23)
#define UART_CNTL_MASK_CLR_ERR                  BIT(24)
#define UART_CNTL_MASK_INV_RX                   BIT(25)
#define UART_CNTL_MASK_INV_TX                   BIT(26)
#define UART_CNTL_MASK_RINT_EN                  BIT(27)
#define UART_CNTL_MASK_TINT_EN                  BIT(28)
#define UART_CNTL_MASK_INV_CTS                  BIT(29)
#define UART_CNTL_MASK_MASK_ERR                 BIT(30)
#define UART_CNTL_MASK_INV_RTS                  BIT(31)
#define P_UART_STATUS(uart_base)  P_UART(uart_base, UART_STATUS)
#define UART_STAT_MASK_RFIFO_CNT                (0x7f << 0)
#define UART_STAT_MASK_TFIFO_CNT                (0x7f << 8)
#define UART_STAT_MASK_PRTY_ERR                 BIT(16)
#define UART_STAT_MASK_FRAM_ERR                 BIT(17)
#define UART_STAT_MASK_WFULL_ERR                BIT(18)
#define UART_STAT_MASK_RFIFO_FULL               BIT(19)
#define UART_STAT_MASK_RFIFO_EMPTY              BIT(20)
#define UART_STAT_MASK_TFIFO_FULL               BIT(21)
#define UART_STAT_MASK_TFIFO_EMPTY              BIT(22)
#define UART_STAT_MASK_XMIT_BUSY					BIT(25)
#define UART_STAT_MASK_RECV_BUSY					BIT(26)
#define P_UART_MISC(uart_base)    P_UART(uart_base, UART_MISC)

#ifndef CONFIG_SERIAL_STP_BITS
#define CONFIG_SERIAL_STP_BITS 1	// 1 , 2
#endif
#if CONFIG_SERIAL_STP_BITS == 1
#define UART_STP_BIT UART_CNTL_MASK_STP_1BIT
#elif CONFIG_SERIAL_STP_BITS == 2
#define UART_STP_BIT UART_CNTL_MASK_STP_2BIT
#else
#error CONFIG_SERIAL_STP_BITS wrong
#endif

#ifndef CONFIG_SERIAL_PRTY_TYPE
#define CONFIG_SERIAL_PRTY_TYPE 0	//0 ,2 ,3
#endif
#if CONFIG_SERIAL_PRTY_TYPE == 0
#define UART_PRTY_BIT 0
#elif CONFIG_SERIAL_PRTY_TYPE == 2
#define UART_PRTY_BIT    (UART_CNTL_MASK_PRTY_EN | UART_CNTL_MASK_PRTY_EVEN)
#elif CONFIG_SERIAL_PRTY_TYPE == 3
#define UART_PRTY_BIT    (UART_CNTL_MASK_PRTY_EN | UART_CNTL_MASK_PRTY_ODD)
#else
#error CONFIG_SERIAL_PRTY_TYPE wrong
#endif

#ifndef CONFIG_SERIAL_CHAR_LEN
#define CONFIG_SERIAL_CHAR_LEN 8	//5,6,7,8
#endif
#if CONFIG_SERIAL_CHAR_LEN == 5
#define UART_CHAR_LEN   UART_CNTL_MASK_CHAR_5BIT
#elif CONFIG_SERIAL_CHAR_LEN == 6
#define UART_CHAR_LEN   UART_CNTL_MASK_CHAR_6BIT
#elif CONFIG_SERIAL_CHAR_LEN == 7
#define UART_CHAR_LEN   UART_CNTL_MASK_CHAR_7BIT
#elif CONFIG_SERIAL_CHAR_LEN == 8
#define UART_CHAR_LEN   UART_CNTL_MASK_CHAR_8BIT
#else
#error CONFIG_SERIAL_CHAR_LEN wrong
#endif
#define UART_CONTROL_SET(baud, clk81)                        \
			(((clk81) / (baud * 4) - 1)              \
			| UART_STP_BIT                      \
			| UART_PRTY_BIT                     \
			| UART_CHAR_LEN                     \
			| UART_CNTL_MASK_TX_EN              \
			| UART_CNTL_MASK_RX_EN              \
			| UART_CNTL_MASK_RST_TX             \
			| UART_CNTL_MASK_RST_RX             \
			| UART_CNTL_MASK_CLR_ERR)
#endif
