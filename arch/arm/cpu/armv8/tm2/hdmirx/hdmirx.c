 /*
  * arch/arm/cpu/armv8/tl1/hdmirx/hdmirx.c
  *
  * Copyright (C) 2012 AMLOGIC, INC. All Rights Reserved.
  * Author: hongmin hua <hongmin hua@amlogic.com>
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the smems of the GNU General Public License as published by
  * the Free Software Foundation; version 2 of the License.
  */

#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/cpu_id.h>
#include <asm/arch/io.h>
#include <asm/arch/hdmirx_reg.h>
#include "hdmirx.h"

struct hdmirx_data_s hdmirx_data;

unsigned char edid_14[] = {
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x05, 0xAC, 0x30, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x2B, 0x1B, 0x01, 0x03, 0x80, 0x73, 0x41, 0x78, 0x0A, 0xCF, 0x74, 0xA3, 0x57, 0x4C, 0xB0, 0x23,
	0x09, 0x48, 0x4C, 0x21, 0x08, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
	0x45, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20,
	0xB8, 0x28, 0x55, 0x40, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x41,
	0x4D, 0x4C, 0x20, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
	0x00, 0x3B, 0x46, 0x1F, 0x8C, 0x3C, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x94,
	0x02, 0x03, 0x3E, 0xF0, 0x53, 0x10, 0x1F, 0x14, 0x05, 0x13, 0x04, 0x20, 0x22, 0x3C, 0x3E, 0x12,
	0x16, 0x03, 0x07, 0x11, 0x15, 0x02, 0x06, 0x01, 0x2F, 0x09, 0x7F, 0x05, 0x15, 0x07, 0x50, 0x57,
	0x07, 0x01, 0x3D, 0x07, 0xC0, 0x5F, 0x7E, 0x01, 0x83, 0x01, 0x00, 0x00, 0x6E, 0x03, 0x0C, 0x00,
	0x10, 0x00, 0xB8, 0x3C, 0x2F, 0x80, 0x80, 0x01, 0x02, 0x03, 0x04, 0xE2, 0x00, 0xFB, 0x01, 0x1D,
	0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28, 0x55, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E,
	0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 0x13, 0x8E, 0x21, 0x00,
	0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84,
};

static unsigned first_bit_set(uint32_t data)
{
	unsigned n = 32;

	if (data != 0) {
		for (n = 0; (data & 1) == 0; n++)
			data >>= 1;
	}
	return n;
}

uint32_t get(uint32_t data, uint32_t mask)
{
	return (data & mask) >> first_bit_set(mask);
}

uint32_t set(uint32_t data, uint32_t mask, uint32_t value)
{
	return ((value << first_bit_set(mask)) & mask) | (data & ~mask);
}

unsigned int rx_rd_reg(unsigned long reg_addr)
{
	unsigned int val = 0;

	val = readl(reg_addr);

	return val;
}

void rx_wr_reg(unsigned long reg_addr, unsigned int val)
{
	writel(val, reg_addr);
}

unsigned int rx_rd_reg_b(unsigned long reg_addr)
{
	unsigned int val = 0;

	val = readb(reg_addr);

	return val;
}

void rx_wr_reg_b(unsigned long reg_addr, unsigned int val)
{
	writeb(val, reg_addr);
}

uint32_t hdmirx_rd_dwc(unsigned long addr)
{
	int data;

	data = rx_rd_reg(HDMIRX_DWC_BASE_OFFSET + addr);
	return data;
}

uint32_t hdmirx_rd_bits_dwc(unsigned long addr, uint32_t mask)
{
	return get(hdmirx_rd_dwc(addr), mask);
}

/**
 * Write data to HDMI RX CTRL
 * @param[in] addr register address
 * @param[in] data new register value
 */
void hdmirx_wr_dwc(unsigned long addr, uint32_t data)
{
	rx_wr_reg(HDMIRX_DWC_BASE_OFFSET + addr, data);
}

void hdmirx_wr_bits_dwc(unsigned long addr, uint32_t mask, uint32_t value)
{
	hdmirx_wr_dwc(addr, set(hdmirx_rd_dwc(addr), mask, value));
}

unsigned int hdmirx_rd_top(unsigned long addr)
{
	int data;
	unsigned int dev_offset = 0;

	dev_offset = TOP_DWC_BASE_OFFSET +
		HDMIRX_DWC_BASE_OFFSET;
	if ((addr >= TOP_EDID_ADDR_S) &&
		(addr <= TOP_EDID_PORT3_ADDR_E)) {
		data = rx_rd_reg_b(dev_offset + addr);
	} else {
		data = rx_rd_reg(dev_offset + (addr << 2));
	}

	return data;
}

uint32_t hdmirx_rd_bits_top(unsigned long addr, uint32_t mask)
{
	return get(hdmirx_rd_top(addr), mask);
}

void hdmirx_wr_top(unsigned long addr, unsigned long data)
{
	unsigned long dev_offset = 0;

	dev_offset = TOP_DWC_BASE_OFFSET +
		HDMIRX_DWC_BASE_OFFSET;
	if ((addr >= TOP_EDID_ADDR_S) &&
		(addr <= TOP_EDID_PORT3_ADDR_E)) {
		rx_wr_reg_b(dev_offset + addr, (unsigned char)data);
	} else {
		rx_wr_reg(dev_offset + (addr << 2), data);
	}
}

void hdmirx_wr_bits_top(unsigned long addr, uint32_t mask, uint32_t value)
{
	hdmirx_wr_top(addr, set(hdmirx_rd_top(addr), mask, value));
}

unsigned int rd_reg_hhi(unsigned long offset)
{
	unsigned long addr = offset + HHI_BASE_ADDR;

	return rx_rd_reg(addr);
}

void wr_reg_hhi(unsigned long offset, unsigned int val)
{
	unsigned long addr = offset + HHI_BASE_ADDR;
	rx_wr_reg(addr, val);
}

bool hdmirx_repeat_support(void)
{
	return hdmirx_data.repeater;
}

unsigned char *rx_get_edid(void)
{
	if (hdmirx_data.edid_buf != NULL)
		return hdmirx_data.edid_buf;
	else
		return edid_14;
}

unsigned int rx_exchange_bits(unsigned int value)
{
	unsigned int temp;
	printf("bfe:%#x\n", value);
	temp = value & 0xF;
	value = (((value >> 4) & 0xF) | (value & 0xFFF0));
	value = ((value & 0xFF0F) | (temp << 4));
	temp = value & 0xF00;
	value = (((value >> 4) & 0xF00) | (value & 0xF0FF));
	value = ((value & 0x0FFF) | (temp << 4));
	printf("aft:%#x\n", value);
	return value;
}

unsigned int rx_edid_cal_phy_addr(
					unsigned int brepeat,
					unsigned int up_addr,
					unsigned int portmap,
					unsigned char *pedid,
					unsigned int *phy_offset,
					unsigned int *phy_addr)
{
	unsigned int root_offset = 0;
	unsigned int i;
	unsigned int flag = 0;

	if (!(pedid && phy_offset && phy_addr))
		return -1;

	for (i = 0; i <= 255; i++) {
		/*find phy_addr_offset*/
		if (pedid[i] == 0x03) {
			if ((pedid[i+1] == 0x0c) &&
			(pedid[i+2] == 0x00)) {
				if (brepeat)
					pedid[i+3] = 0x00;
				else
					pedid[i+3] = 0x10;
				*phy_offset = i+3;
				flag = 1;
				break;
			}
		}
	}

	if (brepeat) {
		/*get the root index*/
		i = 0;
		while (i < 4) {
			if (((up_addr << (i*4)) & 0xf000) != 0) {
				root_offset = i;
				break;
			}
			i++;
		}
		if (i == 4)
			root_offset = 4;

		for (i = 0; i < E_PORT_NUM; i++) {
			if (root_offset == 0)
				phy_addr[i] = 0xFFFF;
			else
				phy_addr[i] = (up_addr |
				((((portmap >> i*4) & 0xf) << 12) >>
				(root_offset - 1)*4));

			phy_addr[i] = rx_exchange_bits(phy_addr[i]);
		}
	} else {
		for (i = 0; i < E_PORT_NUM; i++)
			phy_addr[i] = ((portmap >> i*4) & 0xf) << 4;
	}

	return flag;
}

void rx_edid_fill_to_register(
						unsigned char *pedid,
						unsigned int brepeat,
						unsigned int *pphy_addr,
						unsigned char *pchecksum)
{
	unsigned int i;
	unsigned int checksum = 0;
	unsigned int value = 0;
	unsigned int tmp_addr;

	if (!(pedid && pphy_addr && pchecksum))
		return;

	/*printf("rx_edid_fill_to_register start\n");*/

	/* clear all and disable all overlay register 0~7*/
	for (i = TOP_EDID_RAM_OVR0; i <= TOP_EDID_RAM_OVR7; ) {
		hdmirx_wr_top(i, 0);
		i += 2;
	}

	/* physical address info at second block */
	for (i = 128; i <= 255; i++) {
		value = pedid[i];
		if (i < 255) {
			checksum += pedid[i];
			checksum &= 0xff;
		} else if (i == 255) {
			value = (0x100 - checksum)&0xff;
		}
	}

	/* physical address info at second block */
	tmp_addr = TOP_EDID_ADDR_S;
	for (i = 0; i <= 255; i++) {
		/* fill first edid buffer */
		hdmirx_wr_top(tmp_addr + i, pedid[i]);
		/* fill second edid buffer */
		hdmirx_wr_top(0x100 + tmp_addr + i, pedid[i]);
	}
	for (i = 0; i <= 255; i++) {
		/* fill first edid buffer */
		hdmirx_wr_top(TOP_EDID_PORT2_ADDR_S + i,
			pedid[i]);
		/* fill second edid buffer */
		hdmirx_wr_top(TOP_EDID_PORT2_ADDR_S + 0x100 + i,
			pedid[i]);
	}
	for (i = 0; i <= 255; i++) {
		/* fill first edid buffer */
		hdmirx_wr_top(TOP_EDID_PORT3_ADDR_S + i,
			pedid[i]);
		/* fill second edid buffer */
		hdmirx_wr_top(TOP_EDID_PORT3_ADDR_S + 0x100 + i,
			pedid[i]);
	}
	/* caculate 4 port check sum */
	if (brepeat) {
		for (i = 0; i < E_PORT_NUM; i++) {
			pchecksum[i] = (0x100 + value - (pphy_addr[i] & 0xFF) -
			((pphy_addr[i] >> 8) & 0xFF)) & 0xff;
			/*printf("port %d phy:%d\n", i, pphy_addr[i]);*/
		}
	} else {
		for (i = 0; i < E_PORT_NUM; i++) {
			pchecksum[i] = (0x100 - (checksum +
				(pphy_addr[i] - 0x10))) & 0xff;
		}
	}
}

void rx_edid_update_overlay(
						unsigned int phy_addr_offset,
						unsigned int *pphy_addr,
						unsigned char *pchecksum)
{
	/*unsigned int i;*/
	unsigned int tmp_addr;

	if (!(pphy_addr && pchecksum))
		return;
	/*printf("rx_edid_update_overlay statrt\n");*/

	/* replace the first edid ram data */
	/* physical address byte 1 */
	tmp_addr = TOP_EDID_ADDR_S + phy_addr_offset + 1;
	hdmirx_wr_top(tmp_addr, pphy_addr[E_PORT0] >> 8);
	hdmirx_wr_top(tmp_addr + 0x200, pphy_addr[E_PORT1] >> 8);
	hdmirx_wr_top(tmp_addr + 0x400, pphy_addr[E_PORT2] >> 8);
	/* physical address byte 0 */
	tmp_addr = TOP_EDID_ADDR_S + phy_addr_offset;
	hdmirx_wr_top(tmp_addr, pphy_addr[E_PORT0] & 0xff);
	hdmirx_wr_top(tmp_addr + 0x200, pphy_addr[E_PORT1] & 0xff);
	hdmirx_wr_top(tmp_addr + 0x400, pphy_addr[E_PORT2] & 0xff);

	/* checksum */
	tmp_addr = TOP_EDID_ADDR_S + 0xff;
	hdmirx_wr_top(tmp_addr, pchecksum[E_PORT0]);
	hdmirx_wr_top(tmp_addr + 0x200, pchecksum[E_PORT1]);
	hdmirx_wr_top(tmp_addr + 0x400, pchecksum[E_PORT2]);

	/* for (i = 0; i < E_PORT_NUM; i++) { */
		/* printf(">port %d,addr 0x%x,checksum 0x%x\n", */
		/* i, pphy_addr[i], pchecksum[i]); */
	/* } */
}

int rx_get_tag_code(uint8_t *edid_data)
{
	int tag_code;

	if ((*edid_data >> 5) != 7)
		tag_code = (*edid_data >> 5);
	else
		tag_code = (7 << 8) | *(edid_data + 1);/*extern tag*/

	return tag_code;
}

int rx_get_ceadata_offset(uint8_t *cur_edid, uint8_t *addition)
{
	int i;
	int type;

	if ((cur_edid == NULL) || (addition == NULL))
		return 0;

	type = rx_get_tag_code(addition);
	i = EDID_DEFAULT_START;/*block check start index*/
	while (i < 255) {
		if (type == rx_get_tag_code(cur_edid + i))
			return i;
		else
			i += (1 + (*(cur_edid + i) & 0x1f));
	}
	if (hdmirx_data.dbg_en)
		printf("type: %#x, start addr: %#x\n", type, i);

	return 0;
}

int hdmirx_ctrl_edid_update(void)
{
	bool brepeat = hdmirx_repeat_support();
	unsigned char *pedid_data;
	unsigned int sts;
	unsigned int phy_addr_offset;
	unsigned int phy_addr[E_PORT_NUM] = {0, 0, 0, 0};
	unsigned char checksum[E_PORT_NUM] = {0, 0, 0, 0};

	/* get edid from buffer, return buffer addr */
	pedid_data = rx_get_edid();

	/* caculate physical address and checksum */
	sts = rx_edid_cal_phy_addr(brepeat,
					hdmirx_data.up_phy_addr,
					hdmirx_data.port_map,
					pedid_data, &phy_addr_offset,
					phy_addr);
	if (!sts) {
		/* not find physical address info */
		printf("err: not finded phy addr info\n");
	}

	/* write edid to edid register */
	rx_edid_fill_to_register(pedid_data, brepeat,
				 phy_addr, checksum);
	if (sts) {
		/* update physical and checksum */
		rx_edid_update_overlay(phy_addr_offset, phy_addr, checksum);
	}
	return true;
}

static int rx_top_init(void)
{
	int err = 0;
	int data32 = 0;

	data32 |= (0xf	<< 13); /* bit[16:13] */
	data32 |= 0	<< 11;
	data32 |= 0	<< 10;
	data32 |= 0	<< 9;
	data32 |= 0 << 8;
	data32 |= EDID_CLK_DIV << 0;
	hdmirx_wr_top(TOP_EDID_GEN_CNTL,  data32);

	data32 = 0;
	/* SDA filter internal clk div */
	data32 |= 1 << 29;
	/* SDA sampling clk div */
	data32 |= 1 << 16;
	/* SCL filter internal clk div */
	data32 |= 1 << 13;
	/* SCL sampling clk div */
	data32 |= 1 << 0;
	hdmirx_wr_top(TOP_INFILTER_HDCP, data32);
	hdmirx_wr_top(TOP_INFILTER_I2C0, data32);
	hdmirx_wr_top(TOP_INFILTER_I2C1, data32);
	hdmirx_wr_top(TOP_INFILTER_I2C2, data32);
	hdmirx_wr_top(TOP_INFILTER_I2C3, data32);

	data32 = 0;
	/* conversion mode of 422 to 444 */
	data32 |= 0	<< 19;
	/* !!!!dolby vision 422 to 444 ctl bit */
	data32 |= 0	<< 0;
	hdmirx_wr_top(TOP_VID_CNTL,	data32);

	data32 = 0;
	data32 |= 0	<< 20;
	data32 |= 0	<< 8;
	data32 |= 0x0a	<< 0;
	hdmirx_wr_top(TOP_VID_CNTL2,  data32);

	data32 = 0;
	/* n_cts_auto_mode: */
	/*	0-every ACR packet */
	/*	1-on N or CTS value change */
	data32 |= 1 << 4;
	/* delay cycles before n/cts update pulse */
	data32 |= 7 << 0;

	hdmirx_wr_top(TOP_TL1_ACR_CNTL2, data32);

	data32 = hdmirx_rd_dwc(DWC_HDCP_CTRL);
	/* 0: Original behaviour */
	/* 1: Balance path delay between non-HDCP and HDCP */
	data32 |= 1 << 27; /* none & hdcp */
	/* 0: Original behaviour */
	/* 1: Balance path delay between HDCP14 and HDCP22. */
	data32 |= 1 << 26; /* 1.4 & 2.2 */
	hdmirx_wr_dwc(DWC_HDCP_CTRL, data32);

	/* Configure channel switch */
	data32  = 0;
	data32 |= (0 << 4); /* [  4]  valid_always*/
	data32 |= (7 << 0); /* [3:0]  decoup_thresh*/
	hdmirx_wr_top(TOP_CHAN_SWITCH_1, data32);

	data32  = 0;
	data32 |= (2 << 28); /* [29:28]      source_2 */
	data32 |= (1 << 26); /* [27:26]      source_1 */
	data32 |= (0 << 24); /* [25:24]      source_0 */
	hdmirx_wr_top(TOP_CHAN_SWITCH_0, data32);

	/* Configure TMDS algin */
	data32	= 0;
	hdmirx_wr_top(TOP_TMDS_ALIGN_CNTL0,	data32);
	data32	= 0;
	hdmirx_wr_top(TOP_TMDS_ALIGN_CNTL1,	data32);

	/* Enable channel output */
	data32 = hdmirx_rd_top(TOP_CHAN_SWITCH_0);
	hdmirx_wr_top(TOP_CHAN_SWITCH_0, data32 | (1<<0));

	/* configure cable clock measure */
	data32 = 0;
	data32 |= (1 << 28); /* [31:28] meas_tolerance */
	data32 |= (8192 << 0); /* [23: 0] ref_cycles */
	hdmirx_wr_top(TOP_METER_CABLE_CNTL, data32);

	/* configure hdmi clock measure */
	data32 = 0;
	data32 |= (1 << 28); /* [31:28] meas_tolerance */
	data32 |= (8192 << 0); /* [23: 0] ref_cycles */
	hdmirx_wr_top(TOP_METER_HDMI_CNTL, data32);

	data32 = 0;
	/* bit4: hpd override, bit5: hpd reverse */
	data32 |= 1 << 4;
	data32 |= 1 << 5;
	/* pull down all the hpd */
	hdmirx_wr_top(TOP_HPD_PWR5V, data32);
	return err;
}

void rx_clk_init(void)
{
	unsigned int data32;

	/* DWC clock enable */
	/* Turn on clk_hdmirx_pclk, also = sysclk */
	wr_reg_hhi(HHI_GCLK_MPEG0,
		rd_reg_hhi(HHI_GCLK_MPEG0) | (1 << 21));

	/* Enable APB3 fail on error */
	/* APB3 to HDMIRX-TOP err_en */
	/* default 0x3ff, | bit15 = 1 | bit12 = 1 */
	/* no need for chip from TL1 */
	/* hdmirx_wr_ctl_port(0, 0x93ff); */
	/* hdmirx_wr_ctl_port(0x10, 0x93ff); */

	/* turn on clocks: md, cfg... */
	/* G9 clk tree */
	/* fclk_div5 400M ----- mux sel = 3 */
	/* fclk_div3 850M ----- mux sel = 2 */
	/* fclk_div4 637M ----- mux sel = 1 */
	/* XTAL		24M  ----- mux sel = 0 */
	/* [26:25] HDMIRX mode detection clock mux select: osc_clk */
	/* [24]    HDMIRX mode detection clock enable */
	/* [22:16] HDMIRX mode detection clock divider */
	/* [10: 9] HDMIRX config clock mux select: */
	/* [    8] HDMIRX config clock enable */
	/* [ 6: 0] HDMIRX config clock divider: */
	data32  = 0;
	data32 |= 0 << 25;
	data32 |= 1 << 24;
	data32 |= 0 << 16;
	data32 |= 3 << 9;
	data32 |= 1 << 8;
	data32 |= 2 << 0;
	wr_reg_hhi(HHI_HDMIRX_CLK_CNTL, data32);

	data32 = 0;
	data32 |= 2	<< 25;
	data32 |= ACR_MODE << 24;
	data32 |= 0	<< 16;
	data32 |= 2	<< 9;
	data32 |= 1	<< 8;
	data32 |= 2	<< 0;
	wr_reg_hhi(HHI_HDMIRX_AUD_CLK_CNTL, data32);
	/* [15] hdmirx_aud_pll4x_en override enable */
	/* [14] hdmirx_aud_pll4x_en override value */
	/* [6:5] clk_sel for cts_hdmirx_aud_pll_clk: */
	/* 0=hdmirx_aud_pll_clk */
	/* [4] clk_en for cts_hdmirx_aud_pll_clk */
	/* [2:0] clk_div for cts_hdmirx_aud_pll_clk */
	data32  = 0;
	data32 |= (0 << 15);
	data32 |= (1 << 14);
	data32 |= (0 << 5);
	data32 |= (0 << 4);
	data32 |= (0 << 0);
	wr_reg_hhi(HHI_AUDPLL_CLK_OUT_CNTL, data32);
	data32 |= (1 << 4);
	wr_reg_hhi(HHI_AUDPLL_CLK_OUT_CNTL, data32);

	data32 = hdmirx_rd_top(TOP_CLK_CNTL);
	data32 |= 0 << 31;  /* [31]     disable clkgating */
	data32 |= 1 << 17;  /* [17]     audfifo_rd_en */
	data32 |= 1 << 16;  /* [16]     pktfifo_rd_en */
	data32 |= 0 << 8;   /* [8]      tmds_ch2_clk_inv */
	data32 |= 0 << 7;  /* [7]     tmds_ch1_clk_inv */
	data32 |= 0 << 6;  /* [6]     tmds_ch0_clk_inv */
	data32 |= 0 << 5;   /* [5]      pll4x_cfg */
	/* force_pll4x:
	 * 1=Force pll4x_en value to be pll4x_cfg.
	 * 0=Use auto detect.
	 */
	data32 |= 0 << 4;   /* [4]      force_pll4x */
	data32 |= 0 << 3;   /* [3]      phy_clk_inv: 1-invert */
	hdmirx_wr_top(TOP_CLK_CNTL, data32);    /* DEFAULT: {32'h0} */
}

/* pinmux SCL/SDA/HPD/PWR5V of all port */
void rx_set_pinmux(void)
{
	unsigned int mux = readl(P_PERIPHS_PIN_MUX_B);
	writel(0x11111111, P_PERIPHS_PIN_MUX_A);
	writel((mux & 0xFFFF0000) | 0x1111 , P_PERIPHS_PIN_MUX_B);
	printf("set pinmux A:0x%x\n", readl(P_PERIPHS_PIN_MUX_A));
	printf("set pinmux B:0x%x\n", readl(P_PERIPHS_PIN_MUX_B));
}

void hdmirx_hw_init(unsigned int port_map,
						  unsigned char *pedid_data,
						  int edid_size)
{
	/*int i, j;*/
	if ((pedid_data != NULL) && (edid_size >= EDID_DEFAULT_LEN)) {
		printf("load %#x,len:%d\n", pedid_data[0], edid_size);
		hdmirx_data.edid_buf = pedid_data;
		hdmirx_data.edid_size = edid_size;
	} else {
		hdmirx_data.edid_buf = edid_14;
		hdmirx_data.edid_size = sizeof(edid_14);
	}
	hdmirx_wr_top(TOP_MEM_PD, 0);
	hdmirx_wr_top(TOP_INTR_MASKN, 0);
	hdmirx_wr_top(TOP_SW_RESET, 0);
	rx_clk_init();
	rx_top_init();
	hdmirx_data.port_map = port_map;
	hdmirx_ctrl_edid_update();
	/* for (i = 0; i < 16; i++) { */
		/* printf("[%2d] ", i); */
		/* for (j = 0; j < 16; j++) { */
			/* printf("0x%02lx, ", */
			      /*  hdmirx_rd_top(TOP_EDID_ADDR_S + */
					/* (i * 16 + j))); */
		/* } */
		/* printf("\n"); */
	/* } */

	hdmirx_wr_top(TOP_PORT_SEL, 0x10);
	hdmirx_wr_bits_top(TOP_HPD_PWR5V, MSK(4, 0), 0xF);
	rx_set_pinmux();
	printf("%s Done port map:%#x !\n", __func__, port_map);
}

