/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/g12a/firmware/scp_task/uart_rcu.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "registers.h"
#include <config.h>
#include "config.h"
#include <stdint.h>
#include "task_apis.h"


#define A_UART_WFIFO 0xffd24000

#define UART_PORT_CONS A_UART_WFIFO

#define UART_STP_BIT UART_MODE_MASK_STP_1BIT
#define UART_PRTY_BIT 0
#define UART_CHAR_LEN   UART_MODE_MASK_CHAR_8BIT
/*#define UART_MODE_RESET_MASK	\
					(UART_MODE_MASK_RST_TX \
					| UART_MODE_MASK_RST_RX \
					| UART_MODE_MASK_CLR_ERR)*/

#define UART_WFIFO      (0<<2)
#define UART_RFIFO	(1<<2)
#define UART_MODE	(2<<2)
#define UART_STATUS (3<<2)
#define UART_IRQCTL	(4<<2)
#define UART_REG5	(5<<2)
#define UART_MODE_MASK_STP_1BIT                 (0<<16)
#define UART_MODE_MASK_CHAR_8BIT                (0<<20)
#define UART_MODE_MASK_TX_EN                    (1<<12)
#define UART_MODE_MASK_RX_EN                    (1<<13)
//#define UART_MODE_MASK_RST_TX                   (1<<22)
//#define UART_MODE_MASK_RST_RX                   (1<<23)
//#define UART_MODE_MASK_CLR_ERR                  (1<<24)
//#define UART_CTRL_USE_XTAL_CLK			(1<<24)
//#define UART_CTRL_USE_NEW_BAUD_RATE		(1<<23)

#define UART_STAT_MASK_TFIFO_FULL			(1<<21)
#define UART_STAT_MASK_TFIFO_EMPTY			(1<<22)

#define UART_STAT_MASK_RFIFO_CNT                (0x7f<<0)
#define UART_STAT_MASK_PRTY_ERR                 (1<<16)
#define UART_STAT_MASK_FRAM_ERR                 (1<<17)
#define UART_CNTL_MASK_CLR_ERR                  (1<<24)
#define UART_CNTL_MASK_RST_TX                   (1<<22)
#define UART_CNTL_MASK_RST_RX                   (1<<23)
//#define UART_CNTL_MASK_TX_EN                    (1<<12)
//#define UART_CNTL_MASK_RX_EN                    (1<<13)
#define UART_CNTL_MASK_2WIRE                    (1<<15)
#define UART_CNTL_MASK_PRTY_EN                  (1<<19)

#define P_UART(uart_base, reg)		(uart_base+reg)
#define P_UART_WFIFO(uart_base)		P_UART(uart_base, UART_WFIFO)
#define P_UART_RFIFO(uart_base)		P_UART(uart_base, UART_RFIFO)
#define P_UART_MODE(uart_base)		P_UART(uart_base, UART_MODE)

#define P_UART_STATUS(uart_base)	P_UART(uart_base, UART_STATUS)

#define P_UART_REG5(uart_base)    P_UART(uart_base,UART_REG5)
#define UART_REG5_BAUD_XTAL_DIV2    			(1<<27)
#define UART_REG5_BAUD_XTAL_TICK    			(1<<26)
#define UART_REG5_BAUD_XTAL                   (1<<24)
#define UART_REG5_BAUD_USE                    (1<<23)
#define UART_REG5_BAUD_MASK                   0x7fffff

#define setbits_le32(reg,val)	(*((volatile unsigned *)(reg)))|=(val)
#define clrbits_le32(reg,val)	(*((volatile unsigned *)(reg)))&=(~(val))

char buffer[100];

char hci_reset[] =
    { 0x01, 0x03, 0x0c, 0x00 };
char hci_pcf_manu_filter[] =
	{ 0xff,0xff,0x41,0x6d,0x6c,0x6f,0x67,0x69,0x63};



void serial_init_uart_a(unsigned set)
//void serial_init_uart_a(void)
{
	unsigned long baud_para,value;
    //uart_puts("serial_init_uart_a start");
	//uart_puts("\n");

	writel(0,P_UART_REG5(UART_PORT_CONS));

    setbits_le32(P_UART_MODE(UART_PORT_CONS),
        UART_CNTL_MASK_RST_TX | UART_CNTL_MASK_RST_RX | UART_CNTL_MASK_CLR_ERR);
    clrbits_le32(P_UART_MODE(UART_PORT_CONS),
        UART_CNTL_MASK_RST_TX | UART_CNTL_MASK_RST_RX | UART_CNTL_MASK_CLR_ERR);

	//uart_puts("serial_init_uart_a  1  P_UART_REG5= ");
	//uart_put_hex(readl(P_UART_REG5(UART_PORT_CONS)),32);
	//uart_puts("\n");
	/* baud rate */
	baud_para = 24000000/3/set + set/2/set - 1;
	//baud_para = 67;
	//uart_puts("serial_init_uart_a  baud_para= ");
	//uart_put_hex(baud_para,32);
	//uart_puts("\n");

	baud_para &= UART_REG5_BAUD_MASK;
	baud_para &= ~(UART_REG5_BAUD_XTAL_TICK | UART_REG5_BAUD_XTAL_DIV2);
	baud_para |= UART_REG5_BAUD_XTAL | UART_REG5_BAUD_USE;

	//uart_puts("serial_init_uart_a 2 baud_para= ");
	//uart_put_hex(baud_para,32);
	//uart_puts("\n");

	setbits_le32(P_UART_REG5(UART_PORT_CONS), baud_para);


	/*uart config 2000000-8-N-1*/
    value = UART_STP_BIT
            | UART_CHAR_LEN
            | UART_MODE_MASK_TX_EN
            | UART_MODE_MASK_RX_EN;
    clrbits_le32(P_UART_MODE(UART_PORT_CONS), UART_CNTL_MASK_PRTY_EN);
    setbits_le32(P_UART_MODE(UART_PORT_CONS), value);
	//uart_puts("serial_init_uart_a 3 P_UART_REG5= ");
	//uart_put_hex(readl(P_UART_REG5(UART_PORT_CONS)), 32);
	//uart_puts("\n");

}

void serial_putc_uart_a(const char c)
{

	/* Wait till dataTx register is not full */
	while ((readl(P_UART_STATUS(UART_PORT_CONS)) & UART_STAT_MASK_TFIFO_FULL));
	writel(c, P_UART_WFIFO(UART_PORT_CONS));
	/* Wait till dataTx register is empty */
}



unsigned char serial_getc_uart_a(void)
{
	unsigned char ch;
	/* Wait till character is placed in fifo */
	while ((readl(P_UART_STATUS(UART_PORT_CONS)) & UART_STAT_MASK_RFIFO_CNT) == 0);

    /* Also check for overflow errors */
	if (readl(P_UART_STATUS(UART_PORT_CONS)) & (UART_STAT_MASK_PRTY_ERR | UART_STAT_MASK_FRAM_ERR))
	{
		setbits_le32(P_UART_MODE(UART_PORT_CONS),UART_CNTL_MASK_CLR_ERR);
		clrbits_le32(P_UART_MODE(UART_PORT_CONS),UART_CNTL_MASK_CLR_ERR);
	}
	ch = readl(P_UART_RFIFO(UART_PORT_CONS)) & 0x00ff;
	//uart_puts("rev data ch=");
	//uart_put_hex(ch, 32);
	//uart_puts("\n");
	return ch;
}

int read_event(void)
{
	int i = 0;
	int len = 1;
    uart_puts("read_event start");
	uart_puts("\n");

	do {
		if (i ==0) {
			if ((serial_getc_uart_a() == 0x04) && (serial_getc_uart_a() == 0x3e)) {
				buffer[i] = serial_getc_uart_a();
				i++;
			}
		}

	}while(i<len);

	len += buffer[0];

	do {
		buffer[i] = serial_getc_uart_a();
		i++;
	} while(i<len);

	if (len >= 23) {
		if ((buffer[14] == hci_pcf_manu_filter[0]) && (buffer[15] == hci_pcf_manu_filter[1]) && (buffer[16] == hci_pcf_manu_filter[2]) && (buffer[17] == hci_pcf_manu_filter[3]) &&
			(buffer[18] == hci_pcf_manu_filter[4]) && (buffer[19] == hci_pcf_manu_filter[5]) && (buffer[20] == hci_pcf_manu_filter[6]) && (buffer[21] == hci_pcf_manu_filter[7]) && (buffer[22] == hci_pcf_manu_filter[8])) {

			if (buffer[23] == 0x1) {
				uart_puts("receive power data");
				uart_puts("\n");
				return 1;
			}
			else if (buffer[23] == 0x2) {
				uart_puts("receive netflix data");
				uart_puts("\n");
				return 2;
			}
		}
	}

return 0;

}


void host_init_uart(void)
{
	//uart_puts("host_init_uart 1.P_UART_MODE= ");
	//uart_put_hex(readl(P_UART_MODE(UART_PORT_CONS)), 32);
	//uart_puts("\n");

	//pinmux
	writel(0x11110000 & (readl(PERIPHS_PIN_MUX_4)), PERIPHS_PIN_MUX_4);
	//
	writel(0,P_UART_MODE(UART_PORT_CONS));
	//run_command("gpio c GPIOX_15", 0);
	_udelay(1000);
	//uart_puts("host_init_uart 2.P_UART_MODE= ");
	//uart_put_hex(readl(P_UART_MODE(UART_PORT_CONS)), 32);
	//uart_puts("\n");

	//2M
	serial_init_uart_a(2000000);

	//uart_puts("host_init_uart 3.P_UART_MODE= ");
	//uart_put_hex(readl(P_UART_MODE(UART_PORT_CONS)), 32);
	//uart_puts("\n");

	//serial_putc_uart_a(0);
	//serial_putc_uart_a(0xff);
}
