/*
 * Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
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

/*
 * UART driver
 */

#include "common.h"
#include "uart.h"
#include "register.h"
#include "soc.h"

//#define UART_PORT_CONS UART_B_WFIFO

#define UART_STP_BIT UART_MODE_MASK_STP_1BIT
#define UART_PRTY_BIT 0
#define UART_CHAR_LEN   UART_MODE_MASK_CHAR_8BIT
#define UART_MODE_RESET_MASK	\
			(UART_MODE_MASK_RST_TX \
			| UART_MODE_MASK_RST_RX \
			| UART_MODE_MASK_CLR_ERR)

#define UART_WFIFO      (0<<2)
#define UART_RFIFO	(1<<2)
#define UART_MODE	(2<<2)
#define UART_STATUS     (3<<2)
#define UART_IRQCTL	(4<<2)
#define UART_CTRL	(5<<2)
#define UART_MODE_MASK_STP_1BIT                 (0<<16)
#define UART_MODE_MASK_CHAR_8BIT                (0<<20)
#define UART_MODE_MASK_TX_EN                    (1<<12)
#define UART_MODE_MASK_RX_EN                    (1<<13)
#define UART_MODE_MASK_RST_TX                   (1<<22)
#define UART_MODE_MASK_RST_RX                   (1<<23)
#define UART_MODE_MASK_CLR_ERR                  (1<<24)
#define UART_CTRL_USE_XTAL_CLK			(1<<24)
#define UART_CTRL_USE_NEW_BAUD_RATE		(1<<23)

#define UART_STAT_MASK_RFIFO_FULL		(1<<19)
#define UART_STAT_MASK_RFIFO_EMPTY		(1<<20)
#define UART_STAT_MASK_TFIFO_FULL		(1<<21)
#define UART_STAT_MASK_TFIFO_EMPTY		(1<<22)

#define P_UART(uart_base, reg)		(uart_base+reg)
#define P_UART_WFIFO(uart_base)		P_UART(uart_base, UART_WFIFO)
#define P_UART_MODE(uart_base)		P_UART(uart_base, UART_MODE)
#define P_UART_CTRL(uart_base)		P_UART(uart_base, UART_CTRL)
#define P_UART_STATUS(uart_base)	P_UART(uart_base, UART_STATUS)

static int prvUartTxIsFull(void)
{
	return REG32(P_UART_STATUS(UART_PORT_CONS)) & UART_STAT_MASK_TFIFO_FULL;
}

void vUartTxFlush(void)
{
	while (!
	       (REG32(P_UART_STATUS(UART_PORT_CONS)) &
		UART_STAT_MASK_TFIFO_EMPTY));
}

void vUartPutc(const char c)
{
	if (c == '\n')
		vUartPutc('\r');

	while (prvUartTxIsFull());
	REG32(P_UART_WFIFO(UART_PORT_CONS)) = (char)c;
	vUartTxFlush();
}

void vUartPuts(const char *s)
{
	while (*s)
		vUartPutc(*s++);
}

void vUartTxStart(void)
{
	/* Do not allow deep sleep while transmit in progress */
#ifdef CONFIG_LOW_POWER_IDLE
	disable_sleep(SLEEP_MASK_UART);
#endif

	//uart_flush_output();
}

void vUartTxStop(void)
{

}

long lUartTxReady(void)
{
	return !(REG32(P_UART_STATUS(UART_PORT_CONS)) &
		 UART_STAT_MASK_TFIFO_FULL);
}
#if 0
void vUartWriteChar(char c)
{
	vUartPutc(c);
}

int uart_tx_char(int c)
{
	vUartPutc(c);

	return c;
}
/*print BCD*/
void print_u32_dec(unsigned int num) {
	char buf[16];
	char *s = buf + (sizeof(buf) / sizeof(buf[0])) - 1;
	char *e = s;

	do {
		*--s = '0' + num % 10;
	} while (num /= 10);

	while (s < e)
		uart_tx_char(*s++);
}

void serial_put_hex(unsigned long data, unsigned int bitlen)
{
	int i;
	unsigned char s;

	for (i = bitlen - 4; i >= 0; i -= 4)
	{
		s = (data >> i) & 0xf;
		if (s < 10)
			vUartPutc(0x30 + s);
		else if (s < 16)
			vUartPutc(0x61 + s - 10);
	}
}
#endif
#if 0
/* Interrupt handler for console UART */
void uart_interrupt(void)
{
	/* Fill output FIFO */
	uart_process_output();
}

DECLARE_IRQ(IRQ_AO_UART_NUM, uart_interrupt, 1);
#endif

/*
 *	Set UART to 115200-8-N-1
 *
 *	Using 24M XTAL as UART reference clock, *NOT* clk81
 *	So the clk81 can be dynamically changed and not
 *	disturb UART transfers.
 */
void vUartInit(void)
{
}
