// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */
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

#include <serial.h>
#include <io.h>
#include <stdint.h>
#include <platform_def.h>

static int g_serial_disable;

int serial_disable(void)
{
	g_serial_disable = 1;
	return 0;
}

void __attribute__((weak))  plat_serial_setup(void)
{
	serial_init(0);
}

int serial_tx_is_empty(void)
{
	return (readl(P_UART_STATUS(UART_PORT_CONS)) & UART_STAT_MASK_TFIFO_EMPTY);
}

void serial_init(unsigned set)
{
	/*
	 *	Set UART MODE:
	 *
	 *	- Stop bit = 1 bit
	 *	- Parity check = disable
	 *	- Character lengther = 8bit
	 *	- TX, RX enable
	 *	- Hold reset signals
	 */
	writel(0 | UART_STP_BIT | UART_PRTY_BIT | UART_CHAR_LEN
		 | UART_MODE_MASK_TX_EN | UART_MODE_MASK_RX_EN
		 | UART_MODE_RESET_MASK | UART_MODE_MASK_TWO_WIRE
		, P_UART_MODE(UART_PORT_CONS));

	if (set == 1) {
		/*
		 *	Set UART Baud rate = 115200
		 *	- Use 24M XTAL clock /2  as ref clock
		 *	- Enable New baud rete setting
		 *	- XTAL clock sel = XTAL / 2
		 *	- 104 = (24,000,000 / 2) / 115200 (Error = +0.16%)
		 */
		writel(readl(P_AO_TIMEBASE_CNTL1) | BIT(0), P_AO_TIMEBASE_CNTL1);
		writel(104 | UART_CTRL_USE_XTAL_CLK | UART_CTRL_USE_NEW_BAUD_RATE
				| UART_CTRL_XTAL_CLK_DIV2
			, P_UART_CTRL(UART_PORT_CONS));
	} else {
		/*
		 *	Set UART Baud rate = 921600
		 *	- Use 24M XTAL clock /2 as ref clock
		 *	- Enable New baud rete setting
		 *	- XTAL clock sel = XTAL (1)
		 *	- 13 = 12,000,000 / 921600 (Error = +0.16%)
		 */
		writel(readl(P_AO_TIMEBASE_CNTL1) | BIT(0), P_AO_TIMEBASE_CNTL1);
		writel(13 | UART_CTRL_USE_XTAL_CLK | UART_CTRL_USE_NEW_BAUD_RATE
				| UART_CTRL_XTAL_CLK_DIV2
			, P_UART_CTRL(UART_PORT_CONS));
	}

	/*
	 *	Enable pinmux
	 */
	setbits_le32(P_AO_RTI_PINMUX_REG0, 0x11);

	/*
	 *	Release UART reset signals, enable UART
	 */
	clrbits_le32(P_UART_MODE(UART_PORT_CONS), UART_MODE_RESET_MASK);
}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise 0.
 */
int serial_tstc(void)
{
	return (readl(P_UART_STATUS(UART_PORT_CONS)) & UART_STAT_MASK_RFIFO_CNT);
}

int serial_putc(int c)
{
	if (0)
		return c;

	if (c == '\n') {
		while ((readl(P_UART_STATUS(UART_PORT_CONS)) & UART_STAT_MASK_TFIFO_FULL)) {
		};
		writel('\r', P_UART_WFIFO(UART_PORT_CONS));
	}

	/* Wait till dataTx register is not full */
	while ((readl(P_UART_STATUS(UART_PORT_CONS)) & UART_STAT_MASK_TFIFO_FULL)) {
	};
	writel(c, P_UART_WFIFO(UART_PORT_CONS));

	/* Wait till dataTx register is empty */
	return c;
}

int serial_getc(void)
{
	unsigned char ch = 0;

	if (g_serial_disable)
		return ch;

	/* Wait till character is placed in fifo */
	while ((readl(P_UART_STATUS(UART_PORT_CONS)) & UART_STAT_MASK_RFIFO_CNT) == 0) {
	};

	/* Also check for overflow errors */
	if (readl(P_UART_STATUS(UART_PORT_CONS)) &
	    (UART_STAT_MASK_PRTY_ERR | UART_STAT_MASK_FRAM_ERR)) {
		setbits_le32(P_UART_CONTROL(UART_PORT_CONS), UART_CNTL_MASK_CLR_ERR);
		clrbits_le32(P_UART_CONTROL(UART_PORT_CONS), UART_CNTL_MASK_CLR_ERR);
	}

	ch = readl(P_UART_RFIFO(UART_PORT_CONS)) & 0x00ff;
	return ((int)ch);
}

int serial_puts(const char *s)
{
	while (*s)
		serial_putc(*s++);

	return 0;
}

void serial_put_hex(unsigned long data, unsigned int bitlen)
{
	int i;
	unsigned char s;

	for (i = bitlen - 4; i >= 0; i -= 4) {
		s = (data >> i) & 0xf;
		if (s < 10)
			serial_putc(0x30 + s);
		else if (s < 16)
			serial_putc(0x61 + s - 10);
	}
}

void serial_put_dec(unsigned long data)
{
	int i = 0;
	char buff[32];

	do {
		buff[i++] = (data % 10) + 0x30;
		data = data / 10;
	} while (data && (i < (sizeof(buff) / sizeof(buff[0]))));

	for (--i; i >= 0; --i)
		serial_putc(buff[i]);
}

void bl2_print(const char *info, unsigned int value, unsigned int is_dec, const char *info2)
{
	if (info)
		serial_puts(info);

	if (is_dec)
		serial_put_dec(value);
	else
		serial_put_hex(value, 32);

	if (info2)
		serial_puts(info2);
}

#ifdef CONFIG_TEST_SERIAL_PRINT
typedef struct serial_print_test_pattern_s {
	unsigned char t_uchar;
	unsigned int t_uint;
	unsigned long t_ulong;
} serial_print_test_pattern_t;

serial_print_test_pattern_t test_data[] = {
	{ 0, 0, 0 },
	{ 0xf, 0xf, 0xf },
	{ 0xff, 0xff, 0xff },
	{ ~0, 0xffff, 0xffff },
	{ ~0, ~0, 0xffffffff },
	{ ~0, ~0, ~0 },
};

void serial_print_test(void)
{
	int i;
	unsigned char t_uchar = 0;
	unsigned int t_uint = 0;
	unsigned long t_ulong = 0;

	serial_puts("\nSerial Print Test\n");
	serial_puts("sizeof(t_uchar)=");
	serial_put_dec(sizeof(t_uchar));
	serial_puts("\n");
	serial_puts("sizeof(t_uint)=");
	serial_put_dec(sizeof(t_uint));
	serial_puts("\n");
	serial_puts("sizeof(t_ulong)=");
	serial_put_dec(sizeof(t_ulong));
	serial_puts("\n");

	for (i = 0; i < sizeof(test_data) / sizeof(serial_print_test_pattern_t); i++) {
		bl2_print("\nTest Data #", i, VALUE_DEC, "\n");

		t_uchar = test_data[i].t_uchar;
		t_uint = test_data[i].t_uint;
		t_ulong = test_data[i].t_ulong;

		serial_puts("printf(t_uchar=%x)=");
		serial_put_hex(t_uchar, 32);
		serial_puts("\n");
		serial_puts("printf(t_uchar=%x)=");
		serial_put_hex(t_uchar, 64);
		serial_puts("\n");
		serial_puts("printf(t_uchar=%d)=");
		serial_put_dec(t_uchar);
		serial_puts("\n");

		serial_puts("printf(t_uint=%x)=");
		serial_put_hex(t_uint, 32);
		serial_puts("\n");
		serial_puts("printf(t_uint=%x)=");
		serial_put_hex(t_uint, 64);
		serial_puts("\n");
		serial_puts("printf(t_uint=%d)=");
		serial_put_dec(t_uint);
		serial_puts("\n");

		serial_puts("printf(t_ulong=%x)=");
		serial_put_hex(t_ulong, 32);
		serial_puts("\n");
		serial_puts("printf(t_ulong=%x)=");
		serial_put_hex(t_ulong, 64);
		serial_puts("\n");
		serial_puts("printf(t_ulong=%d)=");
		serial_put_dec(t_ulong);
		serial_puts("\n");
	}
}
#endif /* CONFIG_TEST_SERIAL_PRINT */
