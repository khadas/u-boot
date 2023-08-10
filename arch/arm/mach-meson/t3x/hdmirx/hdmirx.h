/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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

/* EDID */
#define EDID_DEFAULT_START		132
#define EDID_DESCRIP_OFFSET		2
#define EDID_BLOCK1_OFFSET		128
#define EDID_DEFAULT_LEN	256

#define EDID_CLK_DIV 9 /* sys clk/(9+1) = 20M */
#define ACR_MODE 0
/**
 * Bit field mask
 * @param m	width
 * @param n shift
 */
#define MSK(m, n)		(((1  <<  (m)) - 1)  <<  (n))
/**
 * Bit mask
 * @param n shift
 */
#define _BIT(n)			MSK(1, (n))

/* t3x phy base addr */
#define AMLPHY_BASE_ADDR 0xfe39c000
#define T3X_PHY_OFFSET	0x400

/* t3x HIU apll register */
#define HHI_RX_APLL_CNTL0		(0x0 << 2)/*0x0*/
#define HHI_RX_APLL_CNTL1		(0x1 << 2)/*0x4*/
#define HHI_RX_APLL_CNTL2		(0x2 << 2)/*0x8*/
#define HHI_RX_APLL_CNTL3		(0x3 << 2)/*0xc*/
#define HHI_RX_APLL_CNTL4		(0x4 << 2)/*0x10*/

#define T3X_HDMIRX20PHY_DCHA_MISC1		(0x006 << 2)
	#define T3X_20_RTERM_CNTL			MSK(4, 12)

#define T3X_HDMIRX21PHY_MISC2           (0x47 << 2)
	#define T3X_21_RTERM_CNTL			MSK(4, 0)


void hdmirx_hw_init(unsigned int port_map,
						  unsigned char *pedid_data,
					  int edid_size);

int64_t meson_trustzone_efuse_caliItem(const char *str);

#endif
