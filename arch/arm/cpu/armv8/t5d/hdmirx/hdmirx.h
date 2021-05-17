/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/t5/hdmirx/hdmirx.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _HDMIRX_H
#define _HDMIRX_H

enum port_sts {
	E_PORT0,
	E_PORT1,
	E_PORT2,
	E_PORT3,
	E_PORT_NUM,
	E_5V_LOST = 0xfd,
	E_HPD_RESET = 0xfe,
	E_INIT = 0xff,
};

struct hdmirx_data_s {
	unsigned char repeater;/*if hdmitx exist,set 1*/
	unsigned char dbg_en;
	unsigned int port_map;
	int up_phy_addr;/*d c b a 4bit*/
	unsigned char *edid_buf;
	int edid_size;
};

/**
 * Bit field mask
 * @param m	width
 * @param n shift
 */
#define MSK(m, n)		(((1 << (m)) - 1) << (n))
/**
 * Bit mask
 * @param n shift
 */
#define _BIT(n)			MSK(1, (n))

/* EDID */
#define EDID_DEFAULT_START		132
#define EDID_DESCRIP_OFFSET		2
#define EDID_BLOCK1_OFFSET		128
#define EDID_DEFAULT_LEN	256

#define EDID_CLK_DIV 9 /* sys clk/(9+1) = 20M */
#define ACR_MODE 0

/* T5 phy base addr */
#define AMLPHY_BASE_ADDR 0xff61c000

/* T5 HIU apll register */
#define HHI_RX_APLL_CNTL0		(0x0<<2)/*0x0*/
#define HHI_RX_APLL_CNTL1		(0x1<<2)/*0x4*/
#define HHI_RX_APLL_CNTL2		(0x2<<2)/*0x8*/
#define HHI_RX_APLL_CNTL3		(0x3<<2)/*0xc*/
#define HHI_RX_APLL_CNTL4		(0x4<<2)/*0x10*/

/* T5 HIU PHY register */
#define HHI_RX_PHY_MISC_CNTL0		(0x5<<2)/*0x14*/
	#define SQO_GATE		_BIT(30)
	#define PLL_SRC_SEL		_BIT(29)
#define HHI_RX_PHY_MISC_CNTL1		(0x6<<2)/*0x18*/
	#define RTERM_CNTL		MSK(4, 12)
	#define RTERM_VALID		_BIT(0)
#define HHI_RX_PHY_MISC_CNTL2		(0x7<<2)/*0x1c*/
#define HHI_RX_PHY_MISC_CNTL3		(0x8<<2)/*0x20*/
#define HHI_RX_PHY_DCHA_CNTL0		(0x9<<2)/*0x24*/
	#define HYPER_GAIN		MSK(3, 12)
#define HHI_RX_PHY_DCHA_CNTL1		(0xa<<2)/*0x28*/
#define HHI_RX_PHY_DCHA_CNTL2		(0xb<<2)/*0x2c*/
	#define EYE_MONITOR_EN1		_BIT(27)/*The same as dchd_eye[19]*/
	#define AFE_EN			_BIT(17)
#define HHI_RX_PHY_DCHA_CNTL3		(0xc<<2)/*0x30*/
#define HHI_RX_PHY_DCHD_CNTL0		(0xd<<2)/*0x34*/
	#define CDR_MODE		_BIT(31)
	#define CDR_FR_EN		_BIT(30)
	#define EQ_EN			_BIT(29)
	#define CDR_PH_DIV		MSK(3, 0)
#define HHI_RX_PHY_DCHD_CNTL1		(0xe<<2)/*0x38*/
	#define IQ_OFST_SIGN		_BIT(27)
	#define IQ_OFST_VAL		MSK(5, 22)
	#define EQ_BYP_VAL2		MSK(5, 17)
	#define EQ_BYP_VAL1		MSK(5, 12)
#define HHI_RX_PHY_DCHD_CNTL2		(0xf<<2)/*0x3c*/
	#define DFE_HOLD		_BIT(31)
	#define DFE_RST			_BIT(26)
	#define TAP0_BYP		_BIT(23)
	#define EYE_STATUS		MSK(3, 28)
	#define ERROR_CNT		0x0
	#define SCAN_STATE		0x1
	#define POSITIVE_EYE_HEIGHT	0x2
	#define NEGATIVE_EYE_HEIGHT	0x3
	#define LEFT_EYE_WIDTH		0x4
	#define RIGHT_EYE_WIDTH		0x5
#define HHI_RX_PHY_DCHD_CNTL3		(0x10<<2)/*0x40*/
#define HHI_RX_PHY_DCHD_CNTL4		(0x11<<2)/*0x44*/
	#define EYE_MONITOR_EN		_BIT(19)
	#define EYE_STATUS_EN		_BIT(18)
	#define EYE_EN_HW_SCAN		_BIT(16)
#define HHI_RX_PHY_MISC_STAT		(0x12<<2)/*0x48*/
#define HHI_RX_PHY_DCHD_STAT		(0x13<<2)/*0x4c*/

void hdmirx_hw_init(unsigned int port_map,
						  unsigned char *pedid_data,
						  int edid_size);
#endif

