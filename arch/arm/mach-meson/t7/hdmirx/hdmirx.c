// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/arch/io.h>
#include <asm/arch-t3/hdmirx_reg.h>
/* #include <asm/arch-t7/register.h> */
#include <amlogic/aml_hdmirx.h>
#include "hdmirx.h"
#include <asm/arch/pwr_ctrl.h>

#define PADCTRL_PIN_MUX_REGM                       ((0x0016  << 2) + 0xfe004000)
#define PADCTRL_PIN_MUX_REGN                       ((0x0017  << 2) + 0xfe004000)

static unsigned int first_bit_set(u32 data)
{
	unsigned int n = 32;

	if (data != 0) {
		for (n = 0; (data & 1) == 0; n++)
			data >>= 1;
	}
	return n;
}

u32 get(u32 data, u32 mask)
{
	return (data & mask) >> first_bit_set(mask);
}

u32 set(u32 data, u32 mask, u32 value)
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

struct hdmirx_data_s hdmirx_data;

unsigned char edid_14[] = {
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
	0x05, 0xAC, 0x30, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x2B, 0x1B, 0x01, 0x03, 0x80, 0x73, 0x41, 0x78,
	0x0A, 0xCF, 0x74, 0xA3, 0x57, 0x4C, 0xB0, 0x23,
	0x09, 0x48, 0x4C, 0x21, 0x08, 0x00, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A,
	0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
	0x45, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E,
	0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20,
	0xB8, 0x28, 0x55, 0x40, 0x20, 0xC2, 0x31, 0x00,
	0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x41,
	0x4D, 0x4C, 0x20, 0x54, 0x56, 0x0A, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
	0x00, 0x3B, 0x46, 0x1F, 0x8C, 0x3C, 0x00, 0x0A,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x94,
	0x02, 0x03, 0x3E, 0xF0, 0x53, 0x10, 0x1F, 0x14,
	0x05, 0x13, 0x04, 0x20, 0x22, 0x3C, 0x3E, 0x12,
	0x16, 0x03, 0x07, 0x11, 0x15, 0x02, 0x06, 0x01,
	0x2F, 0x09, 0x7F, 0x05, 0x15, 0x07, 0x50, 0x57,
	0x07, 0x01, 0x3D, 0x07, 0xC0, 0x5F, 0x7E, 0x01,
	0x83, 0x01, 0x00, 0x00, 0x6E, 0x03, 0x0C, 0x00,
	0x10, 0x00, 0xB8, 0x3C, 0x2F, 0x80, 0x80, 0x01,
	0x02, 0x03, 0x04, 0xE2, 0x00, 0xFB, 0x01, 0x1D,
	0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28,
	0x55, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E,
	0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10,
	0x10, 0x3E, 0x96, 0x00, 0x13, 0x8E, 0x21, 0x00,
	0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84,
};

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

u32 hdmirx_rd_dwc(unsigned long addr)
{
	int data;

	data = rx_rd_reg(HDMIRX_DWC_BASE_OFFSET + addr);
	return data;
}

u32 hdmirx_rd_bits_dwc(unsigned long addr, u32 mask)
{
	return get(hdmirx_rd_dwc(addr), mask);
}

/**
 * Write data to HDMI RX CTRL
 * @param[in] addr register address
 * @param[in] data new register value
 */
void hdmirx_wr_dwc(unsigned long addr, u32 data)
{
	rx_wr_reg(HDMIRX_DWC_BASE_OFFSET + addr, data);
}

void hdmirx_wr_bits_dwc(unsigned long addr, u32 mask, u32 value)
{
	hdmirx_wr_dwc(addr, set(hdmirx_rd_dwc(addr), mask, value));
}

unsigned int hdmirx_rd_top(unsigned long addr)
{
	int data;
	unsigned int dev_offset = 0;

	dev_offset = TOP_BASE_OFFSET;
	if (addr >= TOP_EDID_ADDR_S &&
		addr <= TOP_EDID_PORT3_ADDR_E) {
		data = rx_rd_reg_b(dev_offset + addr);
	} else {
		data = rx_rd_reg(dev_offset + (addr << 2));
	}

	return data;
}

u32 hdmirx_rd_bits_top(unsigned long addr, u32 mask)
{
	return get(hdmirx_rd_top(addr), mask);
}

void hdmirx_wr_top(unsigned long addr, unsigned long data)
{
	unsigned long dev_offset = 0;

	dev_offset = TOP_BASE_OFFSET;
	if (addr >= TOP_EDID_ADDR_S &&
		addr <= TOP_EDID_PORT3_ADDR_E) {
		rx_wr_reg_b(dev_offset + addr, (unsigned char)data);
	} else {
		rx_wr_reg(dev_offset + (addr << 2), data);
	}
}

void hdmirx_wr_bits_top(unsigned long addr, u32 mask, u32 value)
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

unsigned int rd_reg_clk_ctl(unsigned int offset)
{
	unsigned long addr = offset + CLK_CTRL_ADDR;

	return rx_rd_reg(addr);
}

void wr_reg_clk_ctl(unsigned int offset, unsigned int val)
{
	unsigned long addr = offset + CLK_CTRL_ADDR;

	rx_wr_reg(addr, val);
}

bool hdmirx_repeat_support(void)
{
	return hdmirx_data.repeater;
}

unsigned char *rx_get_edid(void)
{
	if (hdmirx_data.edid_buf)
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

unsigned int rx_edid_cal_phy_addr(unsigned int brepeat,
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
			if (pedid[i + 1] == 0x0c &&
			pedid[i + 2] == 0x00) {
				if (brepeat)
					pedid[i + 3] = 0x00;
				else
					pedid[i + 3] = 0x10;
				*phy_offset = i + 3;
				flag = 1;
				break;
			}
		}
	}

	if (brepeat) {
		/*get the root index*/
		i = 0;
		while (i < 4) {
			if (((up_addr << (i * 4)) & 0xf000) != 0) {
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
				((((portmap >> i * 4) & 0xf) << 12) >>
				(root_offset - 1) * 4));

			phy_addr[i] = rx_exchange_bits(phy_addr[i]);
		}
	} else {
		for (i = 0; i < E_PORT_NUM; i++)
			phy_addr[i] = ((portmap >> i * 4) & 0xf) << 4;
	}

	return flag;
}

void rx_edid_fill_to_register(unsigned char *pedid,
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
			value = (0x100 - checksum) & 0xff;
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
	/* calculate 4 port check sum */
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

void rx_edid_update_overlay(unsigned int phy_addr_offset,
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

int rx_get_tag_code(u8 *edid_data)
{
	int tag_code;

	if ((*edid_data >> 5) != 7)
		tag_code = (*edid_data >> 5);
	else
		tag_code = (7 << 8) | *(edid_data + 1);/*extern tag*/

	return tag_code;
}

int rx_get_ceadata_offset(u8 *cur_edid, u8 *addition)
{
	int i;
	int type;

	if (!cur_edid || !addition)
		return 0;

	type = rx_get_tag_code(addition);
	i = EDID_DEFAULT_START;/*block check start index*/
	while (i < 255) {
		if (type == rx_get_tag_code(cur_edid + i))
			return i;
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

	/* calculate physical address and checksum */
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

	writel(1, 0xfe398288);
	/* Filter 100ns glitch */
	hdmirx_wr_top(TOP_AUDPLL_LOCK_FILTER,  32);
	data32  = 0;
	data32 |= (1 << 1);// [1:0]  sel
	hdmirx_wr_top(TOP_PHYIF_CNTL0, data32);
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

	data32 = 0;
	/* SDA filter internal clk div */
	data32 |= 2 << 29; /* change to 2 for nvidia pc */
	/* SDA sampling clk div */
	data32 |= 0x9 << 16; /* change to 8 for nvidia pc */
	/* SCL filter internal clk div */
	data32 |= 1 << 13;
	/* SCL sampling clk div */
	data32 |= 1 << 0;
	hdmirx_wr_top(TOP_INFILTER_I2C0, data32);
	hdmirx_wr_top(TOP_INFILTER_I2C1, data32);
	hdmirx_wr_top(TOP_INFILTER_I2C2, data32);
	hdmirx_wr_top(TOP_INFILTER_I2C3, data32);

	data32 = 0;
	/*420to444_en*/
	data32 |= 1	<< 21;
	/*422to444_en*/
	data32 |= 1	<< 20;
	/* conversion mode of 422 to 444 */
	data32 |= 0	<< 19;
	/* pixel_repeat_ovr */
	data32 |= 1 << 7;
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

	/* Configure TMDS align T7 unused */
	data32	= 0;
	hdmirx_wr_top(TOP_TMDS_ALIGN_CNTL0,	data32);
	data32	= 0;
	hdmirx_wr_top(TOP_TMDS_ALIGN_CNTL1,	data32);

	/* Enable channel output */
	data32 = hdmirx_rd_top(TOP_CHAN_SWITCH_0);
	hdmirx_wr_top(TOP_CHAN_SWITCH_0, data32 | (1 << 0));

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
	printf("%s\n", __func__);
	return err;
}

void rx_clk_init(void)
{
	unsigned int data32;

	/* DWC clock enable */
	/* Turn on clk_hdmirx_pclk, also = sysclk */
	wr_reg_clk_ctl(CLKCTRL_SYS_CLK_EN0_REG2,
		       rd_reg_clk_ctl(CLKCTRL_SYS_CLK_EN0_REG2) | (1 << 9));

	data32	= 0;
	data32 |= (0 << 25);// [26:25] clk_sel for cts_hdmirx_2m_clk: 0=cts_oscin_clk
	data32 |= (0 << 24);// [   24] clk_en for cts_hdmirx_2m_clk
	data32 |= (11 << 16);// [22:16] clk_div for cts_hdmirx_2m_clk: 24/12=2M
	data32 |= (3 << 9);// [10: 9] clk_sel for cts_hdmirx_5m_clk: 3=fclk_div5
	data32 |= (0 << 8);// [    8] clk_en for cts_hdmirx_5m_clk
	data32 |= (79 << 0);// [ 6: 0] clk_div for cts_hdmirx_5m_clk: fclk_dvi5/80=400/80=5M
	wr_reg_clk_ctl(RX_CLK_CTRL, data32);
	data32 |= (1 << 24);// [   24] clk_en for cts_hdmirx_2m_clk
	data32 |= (1 << 8);// [    8] clk_en for cts_hdmirx_5m_clk
	wr_reg_clk_ctl(RX_CLK_CTRL, data32);

	data32  = 0;
	data32 |= (3 << 25);// [26:25] clk_sel for cts_hdmirx_hdcp2x_eclk: 3=fclk_div5
	data32 |= (0 << 24);// [   24] clk_en for cts_hdmirx_hdcp2x_eclk
	data32 |= (15 << 16);// [22:16] clk_div for cts_hdmirx_hdcp2x_eclk: fclk_dvi5/16=400/16=25M
	data32 |= (3 << 9);// [10: 9] clk_sel for cts_hdmirx_cfg_clk: 3=fclk_div5
	data32 |= (0 << 8);// [    8] clk_en for cts_hdmirx_cfg_clk
	data32 |= (7 << 0);// [ 6: 0] clk_div for cts_hdmirx_cfg_clk: fclk_dvi5/8=400/8=50M
	wr_reg_clk_ctl(RX_CLK_CTRL1, data32);
	data32 |= (1 << 24);// [   24] clk_en for cts_hdmirx_hdcp2x_eclk
	data32 |= (1 << 8);// [    8] clk_en for cts_hdmirx_cfg_clk
	wr_reg_clk_ctl(RX_CLK_CTRL1, data32);

	data32  = 0;
	data32 |= (1 << 25);// [26:25] clk_sel for cts_hdmirx_acr_ref_clk: 1=fclk_div4
	data32 |= (0 << 24);// [   24] clk_en for cts_hdmirx_acr_ref_clk
	data32 |= (0 << 16);// [22:16] clk_div for cts_hdmirx_acr_ref_clk: fclk_div4/1=500M
	data32 |= (0 << 9);// [10: 9] clk_sel for cts_hdmirx_aud_pll_clk: 0=hdmirx_aud_pll_clk
	data32 |= (0 << 8);// [    8] clk_en for cts_hdmirx_aud_pll_clk
	data32 |= (0 << 0);// [ 6: 0] clk_div for cts_hdmirx_aud_pll_clk
	wr_reg_clk_ctl(RX_CLK_CTRL2, data32);
	data32 |= (1 << 24);// [   24] clk_en for cts_hdmirx_acr_ref_clk
	data32 |= (1 << 8);// [    8] clk_en for cts_hdmirx_aud_pll_clk
	wr_reg_clk_ctl(RX_CLK_CTRL2, data32);

	data32  = 0;
	data32 |= (0 << 9);// [10: 9] clk_sel for cts_hdmirx_meter_clk: 0=cts_oscin_clk
	data32 |= (0 << 8);// [    8] clk_en for cts_hdmirx_meter_clk
	data32 |= (0 << 0);// [ 6: 0] clk_div for cts_hdmirx_meter_clk: 24M
	wr_reg_clk_ctl(RX_CLK_CTRL3, data32);
	data32 |= (1 << 8);// [    8] clk_en for cts_hdmirx_meter_clk
	wr_reg_clk_ctl(RX_CLK_CTRL3, data32);

	data32  = 0;
	data32 |= (0 << 31);// [31]	  free_clk_en
	data32 |= (0 << 15);// [15]	  hbr_spdif_en
	data32 |= (0 << 8);// [8]	  tmds_ch2_clk_inv
	data32 |= (0 << 7);// [7]	  tmds_ch1_clk_inv
	data32 |= (0 << 6);// [6]	  tmds_ch0_clk_inv
	data32 |= (0 << 5);// [5]	  pll4x_cfg
	data32 |= (1 << 4);// [4]	  force_pll4x
	data32 |= (0 << 3);// [3]	  phy_clk_inv
	hdmirx_wr_top(TOP_CLK_CNTL,	data32);
	printf("%s\n", __func__);
}

/* pinmux SCL/SDA/HPD/PWR5V of all port */
void rx_set_pinmux(void)
{
	unsigned int mux = readl(PADCTRL_PIN_MUX_REGN);

	writel(0x11111111, PADCTRL_PIN_MUX_REGM);
	writel((mux & 0xFFFF0000) | 0x1111, PADCTRL_PIN_MUX_REGN);
	printf("set pinmux A:0x%x\n", readl(PADCTRL_PIN_MUX_REGM));
	printf("set pinmux B:0x%x\n", readl(PADCTRL_PIN_MUX_REGN));
}

void hdmirx_hw_init(unsigned int port_map,
						  unsigned char *pedid_data,
						  int edid_size)
{
	/*int i, j;*/
	if (pedid_data && edid_size >= EDID_DEFAULT_LEN) {
		printf("load %#x,len:%d\n", pedid_data[0], edid_size);
		hdmirx_data.edid_buf = pedid_data;
		hdmirx_data.edid_size = edid_size;
	} else {
		hdmirx_data.edid_buf = edid_14;
		hdmirx_data.edid_size = sizeof(edid_14);
	}
	/*power domain control for hdmirx */
	pwr_ctrl_psci_smc(PM_HDMIRX, PWR_ON);
	//hdmirx_wr_top(TOP_MEM_PD, 0);
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

/*
 * get - get masked bits of data
 */
unsigned int rx_get_bits(unsigned int data, unsigned int mask)
{
	unsigned int fstbs_rtn;
	unsigned int rtn_val;

	fstbs_rtn = first_bit_set(mask);
	if (fstbs_rtn < 32)
		rtn_val = (data & mask) >> fstbs_rtn;
	else
		rtn_val = 0;
	return rtn_val;
}

unsigned int rx_set_bits(unsigned int data,
	unsigned int mask, unsigned int value)
{
	unsigned int fstbs_rtn;
	unsigned int rtn_val;

	fstbs_rtn = first_bit_set(mask);
	if (fstbs_rtn < 32)
		rtn_val = ((value << fstbs_rtn) & mask) | (data & ~mask);
	else
		rtn_val = 0;
	return rtn_val;
}

/*
 * hdmirx_rd_amlphy - read hdmirx amlphy reg
 * @addr: register address
 *
 * return data read value
 */
unsigned int hdmirx_rd_amlphy(unsigned long offset)
{
	unsigned long addr = offset + AMLPHY_BASE_ADDR;

	return rx_rd_reg(addr);
}

/*
 * hdmirx_rd_bits_amlphy - read specified bits of hdmirx amlphy reg
 * @addr: register address
 * @mask: bits mask
 *
 * return masked bits of register value
 */
u32 hdmirx_rd_bits_amlphy(u16 offset, u32 mask)
{
	return rx_get_bits(hdmirx_rd_amlphy(offset), mask);
}

/*
 * hdmirx_wr_amlphy - Write data to hdmirx amlphy reg
 * @addr: register address
 * @data: new register value
 */
void hdmirx_wr_amlphy(unsigned int offset, unsigned int val)
{
	unsigned long addr = offset + AMLPHY_BASE_ADDR;

	rx_wr_reg(addr, val);
}

/*
 * hdmirx_wr_bits_amlphy - write specified bits of hdmirx amlphy reg
 * @addr: register address
 * @mask: bits mask
 * @value: new register value
 */
void hdmirx_wr_bits_amlphy(unsigned int offset,
	unsigned int mask, unsigned int value)
{
	hdmirx_wr_amlphy(offset, rx_set_bits(hdmirx_rd_amlphy(offset), mask, value));
}

/* set rx phy termination */
void rx_set_phy_rterm(void)
{
	s64 rterm_val;
	unsigned int data32;

	/*power domain control for hdmirx */
	pwr_ctrl_psci_smc(PM_HDMIRX, PWR_ON);
	rterm_val = meson_trustzone_efuse_caliItem("hdmirx");
	if (rterm_val >= 0) {
		data32 = hdmirx_rd_amlphy(HHI_RX_PHY_MISC_CNTL1);
		data32 &= (~(0xf << 12));
		/* rterm val */
		data32 |= (rterm_val << 12);
		/* rterm flag */
		data32 |= 0x1 << 0;
		hdmirx_wr_amlphy(HHI_RX_PHY_MISC_CNTL1, data32);
		printf("rx trim:0x%x\n", data32);
	} else {
		printf("no trim val!\n");
	}
}

