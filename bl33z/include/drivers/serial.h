/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2014-2017 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License Agreement
 * (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 * only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission from
 * Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "platform_def.h"

#ifndef __SERIAL_H__
#define __SERIAL_H__
#ifndef BIT
#define BIT(nr)			(1 << nr)
#endif
#define UART_WFIFO      (0 << 2)
#define UART_RFIFO      BIT(2)
#define UART_CONTROL    (2 << 2)
#define UART_STATUS     (3 << 2)
#define UART_MISC       (4 << 2)

#define UART_WFIFO      (0 << 2)
#define UART_RFIFO      BIT(2)
#define UART_MODE       (2 << 2)
#define UART_STATUS     (3 << 2)
#define UART_IRQCTL     (4 << 2)
#define UART_CTRL       (5 << 2)

#define UART_PORT_CONS  AO_UART_WFIFO

#define P_UART(uart_base, reg)    	    (uart_base + reg)
#define P_UART_WFIFO(uart_base)         P_UART(uart_base, UART_WFIFO)
#define P_UART_RFIFO(uart_base)   	    P_UART(uart_base, UART_RFIFO)

#define P_UART_CONTROL(uart_base)       P_UART(uart_base, UART_CONTROL)
#define P_UART_MISC(uart_base)          P_UART(uart_base, UART_MISC)

#define UART_STAT_MASK_RFIFO_CNT                (0x3f << 0)
#define UART_STAT_MASK_TFIFO_CNT                (0x3f << 8)
#define UART_STAT_MASK_PRTY_ERR                 BIT(16)
#define UART_STAT_MASK_FRAM_ERR                 BIT(17)
#define UART_STAT_MASK_WFULL_ERR                BIT(18)
#define UART_STAT_MASK_RFIFO_FULL               BIT(19)
#define UART_STAT_MASK_RFIFO_EMPTY              BIT(20)
#define UART_STAT_MASK_TFIFO_FULL               BIT(21)
#define UART_STAT_MASK_TFIFO_EMPTY              BIT(22)

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

#define UART_MODE_MASK_TWO_WIRE                 BIT(15)
#define UART_MODE_MASK_STP_1BIT                 (0 << 16)
#define UART_MODE_MASK_CHAR_8BIT                (0 << 20)
#define UART_MODE_MASK_TX_EN                    BIT(12)
#define UART_MODE_MASK_RX_EN                    BIT(13)
#define UART_MODE_MASK_RST_TX                   BIT(22)
#define UART_MODE_MASK_RST_RX                   BIT(23)
#define UART_MODE_MASK_CLR_ERR                  BIT(24)
#define UART_STAT_MASK_TFIFO_FULL               BIT(21)
#define UART_CTRL_XTAL_CLK_SEL                  BIT(26)
#define UART_CTRL_XTAL_CLK_DIV2                 BIT(27)
#define UART_CTRL_USE_XTAL_CLK                  BIT(24)
#define UART_CTRL_USE_NEW_BAUD_RATE             BIT(23)

#define UART_STP_BIT                UART_MODE_MASK_STP_1BIT
#define UART_PRTY_BIT               0
#define UART_CHAR_LEN               UART_MODE_MASK_CHAR_8BIT
#define UART_MODE_RESET_MASK        (UART_MODE_MASK_RST_TX | UART_MODE_MASK_RST_RX | UART_MODE_MASK_CLR_ERR)

#define P_UART(uart_base, reg)      (uart_base + reg)
#define P_UART_WFIFO(uart_base)     P_UART(uart_base, UART_WFIFO)
#define P_UART_MODE(uart_base)      P_UART(uart_base, UART_MODE)
#define P_UART_CTRL(uart_base)      P_UART(uart_base, UART_CTRL)
#define P_UART_STATUS(uart_base)    P_UART(uart_base, UART_STATUS)

#define VALUE_DEC   1
#define VALUE_HEX   0

void serial_init(unsigned set);
int serial_tstc(void);
int serial_putc(int c);
int serial_getc(void);
int serial_puts(const char *s);
void serial_put_hex(unsigned long data, unsigned int bitlen);
void serial_put_dec(unsigned long data);

/* Disable serial support */
int serial_disable(void);

/* Simple print function */
void bl33z_print(const char *info, unsigned int value, unsigned int is_dec, const char *info2);

#endif /* __SERIAL_H__ */
