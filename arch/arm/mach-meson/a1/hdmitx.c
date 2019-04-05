/*
 * arch/arm/mach-meson/g12a/hdmitx.c
 *
 * Copyright (C) 2018 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <common.h>
#include <asm/arch/io.h>
#include <asm/io.h>
#include <amlogic/media/vout/hdmitx.h>
#include <asm/arch/regs.h>
#include <asm/arch/secure_apb.h>

#define HDMITX_DWC_BASE_OFFSET  0xFF600000
#define HDMITX_TOP_BASE_OFFSET  0xFF608000

static int dbg_en =0;

unsigned long hd_read_reg(volatile unsigned int* addr)
{
	unsigned long val = 0;

	val = *addr;
	if (dbg_en)
		printk("R: 0x%8p   0x%08lx\n", addr, val);
	return val;
}

void hd_write_reg(volatile unsigned int* addr, unsigned long val)
{
	*addr = val;
	if (dbg_en)
		printk("W: 0x%8p  0x%08lx %s 0x%08lx\n", addr,
		val, (val == hd_read_reg(addr)) ? "==" : "!=", hd_read_reg(addr));
}

void hd_set_reg_bits(volatile unsigned int* addr, unsigned long value,
	unsigned long offset, unsigned long len)
{
	unsigned long data = 0;

	data = hd_read_reg(addr);
	data &= ~(((1L << len) - 1) << offset);
	data |= (value & ((1L << len) - 1)) << offset;
	hd_write_reg(addr, data);
}

#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"
unsigned int hdmitx_rd_reg(unsigned int addr)
{
	unsigned int large_offset = addr >> 24;
	unsigned int small_offset = addr & ((1 << 24)  - 1);
	unsigned int data = 0;
	register long x0 asm("x0") = 0x82000018;
	register long x1 asm("x1") = (unsigned long)addr;

	if (large_offset == 0x10) {
		large_offset = HDMITX_DWC_BASE_OFFSET;
		data = readb((unsigned long)(large_offset + small_offset));
	} else if (large_offset == 0x0){
		large_offset = HDMITX_TOP_BASE_OFFSET;
		if ((small_offset >= 0x2000) && (small_offset <= 0x365E)) {
			data = readb((unsigned long)(large_offset + small_offset));
		} else {
			small_offset = small_offset << 2;
			data = readl((unsigned long)(large_offset + small_offset));
		}
	} else {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x1")
			"smc #0\n"
			: "+r"(x0) : "r"(x1)
		);
		data = (unsigned)(x0&0xffffffff);
	}
	if (dbg_en)
		printk("%s wr[0x%x] 0x%x\n", large_offset ? "DWC" : "TOP",
			addr, data);

	return data;
}

void hdmitx_wr_reg(unsigned int addr, unsigned int data)
{
	unsigned int large_offset = addr >> 24;
	unsigned int small_offset = addr & ((1 << 24)  - 1);
	register long x0 asm("x0") = 0x82000019;
	register long x1 asm("x1") = (unsigned long)addr;
	register long x2 asm("x2") = data;

	if (large_offset == 0x10) {
		large_offset = HDMITX_DWC_BASE_OFFSET;
		writeb(data & 0xff, (unsigned long)(large_offset + small_offset));
	} else if (large_offset == 0x0){
		large_offset = HDMITX_TOP_BASE_OFFSET;
		if ((small_offset >= 0x2000) && (small_offset <= 0x365E)) {
			writeb(data & 0xff,(unsigned long)(large_offset + small_offset));
		} else {
			small_offset = small_offset << 2;
			writel(data,(unsigned long)(large_offset + small_offset));
		}
	} else {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x1")
			__asmeq("%2", "x2")
			"smc #0\n"
			: : "r"(x0), "r"(x1), "r"(x2)
		);
	}
	if (dbg_en)
		printk("%s wr[0x%x] 0x%x\n", large_offset ? "DWC" : "TOP",
			addr, data);
}

void hdmitx_set_reg_bits(unsigned int addr, unsigned int value,
	unsigned int offset, unsigned int len)
{
	unsigned int data32 = 0;

	data32 = hdmitx_rd_reg(addr);
	data32 &= ~(((1 << len) - 1) << offset);
	data32 |= (value & ((1 << len) - 1)) << offset;
	hdmitx_wr_reg(addr, data32);
}

void hdmitx_poll_reg(unsigned int addr, unsigned int val,
	unsigned long timeout)
{
	udelay(2000);
	if (!(hdmitx_rd_reg(addr) & val))
		printk("hdmitx poll:0x%x  val:0x%x t=%dms timeout\n",
			addr, val, 2000);
}

void hdmitx_rd_check_reg (unsigned long addr,
	unsigned long exp_data, unsigned long mask)
{
	unsigned long rd_data;
	rd_data = hdmitx_rd_reg(addr);
	if ((rd_data | mask) != (exp_data | mask)) {
	printk("HDMITX-DWC addr=0x%04x rd_data=0x%02x\n",
		(unsigned int)addr, (unsigned int)rd_data);
	printk("Error: HDMITX-DWC exp_data=0x%02x mask=0x%02x\n",
		(unsigned int)exp_data, (unsigned int)mask);
	}
}

static void set_phy_by_mode(unsigned int mode)
{
	switch (mode) {
	case 1: /* 5.94/4.5/3.7Gbps */
		hd_write_reg(P_HHI_HDMI_PHY_CNTL0, 0x37eb5584);
		hd_write_reg(P_HHI_HDMI_PHY_CNTL3, 0x2ab0ff3b);
		hd_write_reg(P_HHI_HDMI_PHY_CNTL5, 0x0000080b);
		break;
	case 2: /* 2.97Gbps */
		hd_write_reg(P_HHI_HDMI_PHY_CNTL0, 0x33eb4262);
		hd_write_reg(P_HHI_HDMI_PHY_CNTL3, 0x2ab0ff3b);
		hd_write_reg(P_HHI_HDMI_PHY_CNTL5, 0x00000003);
		break;
	case 3: /* 1.485Gbps, and below */
	default:
		hd_write_reg(P_HHI_HDMI_PHY_CNTL0, 0x33eb4242);
		hd_write_reg(P_HHI_HDMI_PHY_CNTL3, 0x2ab0ff3b);
		hd_write_reg(P_HHI_HDMI_PHY_CNTL5, 0x00000003);
		break;
	}
}
void hdmitx_set_phy(struct hdmitx_dev *hdev)
{
	if (!hdev)
		return;

	switch (hdev->vic) {
	case HDMI_3840x2160p50_16x9:
	case HDMI_3840x2160p60_16x9:
	case HDMI_4096x2160p50_256x135:
	case HDMI_4096x2160p60_256x135:
		if ((hdev->para->cs == HDMI_COLOR_FORMAT_420)
			&& (hdev->para->cd == HDMI_COLOR_DEPTH_24B))
			set_phy_by_mode(2);
		else
			set_phy_by_mode(1);
		break;
	case HDMI_3840x2160p50_16x9_Y420:
	case HDMI_3840x2160p60_16x9_Y420:
	case HDMI_4096x2160p50_256x135_Y420:
	case HDMI_4096x2160p60_256x135_Y420:
		if (hdev->para->cd == HDMI_COLOR_DEPTH_24B)
			set_phy_by_mode(2);
		else
			set_phy_by_mode(1);
		break;
	case HDMI_3840x2160p24_16x9:
	case HDMI_3840x2160p24_64x27:
	case HDMI_3840x2160p25_16x9:
	case HDMI_3840x2160p25_64x27:
	case HDMI_3840x2160p30_16x9:
	case HDMI_3840x2160p30_64x27:
	case HDMI_4096x2160p24_256x135:
	case HDMI_4096x2160p25_256x135:
	case HDMI_4096x2160p30_256x135:
		if ((hdev->para->cs == HDMI_COLOR_FORMAT_422)
			|| (hdev->para->cd == HDMI_COLOR_DEPTH_24B))
			set_phy_by_mode(2);
		else
			set_phy_by_mode(1);
		break;
	case HDMI_1920x1080p60_16x9:
	case HDMI_1920x1080p50_16x9:
	case HDMI_1920x1080i100_16x9:
	case HDMI_1920x1080i120_16x9:
	case HDMI_1280x720p100_16x9:
	case HDMI_1280x720p120_16x9:
	default:
		set_phy_by_mode(3);
		break;
	}
/* P_HHI_HDMI_PHY_CNTL1	bit[1]: enable clock	bit[0]: soft reset */
#define RESET_HDMI_PHY() \
do { \
	hd_set_reg_bits(P_HHI_HDMI_PHY_CNTL1, 0xf, 0, 4); \
	mdelay(2); \
	hd_set_reg_bits(P_HHI_HDMI_PHY_CNTL1, 0xe, 0, 4); \
	mdelay(2); \
} while (0)

	hd_set_reg_bits(P_HHI_HDMI_PHY_CNTL1, 0x0390, 16, 16);
	hd_set_reg_bits(P_HHI_HDMI_PHY_CNTL1, 0x0, 0, 4);
	RESET_HDMI_PHY();
	RESET_HDMI_PHY();
	RESET_HDMI_PHY();
#undef RESET_HDMI_PHY

	printk("hdmitx phy setting done\n");
}

void hdmitx_turnoff(void)
{
        /* Close HDMITX PHY */
        hd_write_reg(P_HHI_HDMI_PHY_CNTL0, 0);
        hd_write_reg(P_HHI_HDMI_PHY_CNTL3, 0);
        hd_write_reg(P_HHI_HDMI_PHY_CNTL5, 0);
        /* Disable HPLL */
        hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0);
}

#define WAIT_FOR_PLL_LOCKED(reg) \
	do { \
		unsigned int st = 0; \
		int cnt = 10; \
		while (cnt--) { \
			udelay(50); \
			st = (((hd_read_reg(reg) >> 30) & 0x3) == 3); \
			if (st) \
				break; \
			else { \
				/* reset hpll */ \
				hd_set_reg_bits(reg, 1, 29, 1); \
				hd_set_reg_bits(reg, 0, 29, 1); \
			} \
		} \
		if (cnt < 9) \
			printk("pll[0x%8p] reset %d times\n", reg, 9 - cnt);\
	} while (0)

static bool set_hpll_hclk_v1(unsigned int m, unsigned int frac_val,
	struct hdmitx_dev *hdev)
{
	int ret = 0;

	hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x0b3a0400 | (m & 0xff));
	hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x3, 28, 2);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL1, frac_val);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
	if (frac_val == 0x8148) {
		if (((hdev->para->vic == HDMI_3840x2160p50_16x9) ||
			(hdev->para->vic == HDMI_3840x2160p60_16x9) ||
			(hdev->para->vic == HDMI_3840x2160p50_64x27) ||
			(hdev->para->vic == HDMI_3840x2160p60_64x27)) &&
			(hdev->para->cs != HDMI_COLOR_FORMAT_420)) {
			hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x6a685c00);
			hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x11551293);
		} else {
			hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x6a685c00);
			hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x44331290);
		}
	} else {
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x6a68dc00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x65771290);
	}
	hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39272000);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x56540000);
	printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
	hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
	printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
	WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
	printk("HPLLv1: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));

	ret = (((hd_read_reg(P_HHI_HDMI_PLL_CNTL0) >> 30) & 0x3) == 0x3);
	return ret;
}

static bool set_hpll_hclk_v2(unsigned int m, unsigned int frac_val)
{
	int ret = 0;

	hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x0b3a0400 | (m & 0xff));
	hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x3, 28, 2);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL1, frac_val);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0xea68dc00);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x65771290);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39272000);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x56540000);
	printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
	hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
	printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
	WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
	printk("HPLLv2: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));

	ret = (((hd_read_reg(P_HHI_HDMI_PLL_CNTL0) >> 30) & 0x3) == 0x3);
	return ret;
}

static bool set_hpll_hclk_v3(unsigned int m, unsigned int frac_val)
{
	int ret = 0;

	hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x0b3a0400 | (m & 0xff));
	hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x3, 28, 2);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL1, frac_val);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0xea68dc00);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x65771290);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39272000);
	hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x55540000);
	printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
	hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
	printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
	WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
	printk("HPLLv3: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));

	ret = (((hd_read_reg(P_HHI_HDMI_PLL_CNTL0) >> 30) & 0x3) == 0x3);
	return ret;
}

void set_hpll_clk_out(unsigned clk, struct hdmitx_dev *hdev)
{
	unsigned int frac_rate ;

	frac_rate = hdev->frac_rate_policy;
	if (clk == -1)
		return;
	printk("config HPLL = %d frac_rate = %d\n", clk, frac_rate);

	switch (clk) {
	case 5940000:
		if (set_hpll_hclk_v1(0xf7, frac_rate ? 0x8148 : 0x10000, hdev))
			break;
		else if (set_hpll_hclk_v2(0x7b,0x18000))
			break;
		else if (set_hpll_hclk_v3(0xf7,0x10000))
			break;
		else
			break;
		break;
	case 5405400:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b0004e1);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00007333);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 4455000:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b0004b9);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00014000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x6a685c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x43231290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x29272000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x56540028);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 3712500:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b00049a);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00016000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x6a685c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x43231290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x29272000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x56540028);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 3450000:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b00048f);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00018000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 3243240:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b000487);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x0000451f);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 2970000:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b00047b);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00018000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	case 4324320:
		hd_write_reg(P_HHI_HDMI_PLL_CNTL0, 0x3b0004b4);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL1, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL2, 0x00000000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL3, 0x0a691c00);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL4, 0x33771290);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL5, 0x39270000);
		hd_write_reg(P_HHI_HDMI_PLL_CNTL6, 0x50540000);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0x0, 29, 1);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		WAIT_FOR_PLL_LOCKED(P_HHI_HDMI_PLL_CNTL0);
		printk("HPLL: 0x%lx\n", hd_read_reg(P_HHI_HDMI_PLL_CNTL0));
		break;
	default:
		printk("error hpll clk: %d\n", clk);
		break;
	}
	printk("config HPLL done\n");
}

void set_hpll_od1(unsigned div)
{
	switch (div) {
	case 1:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0, 16, 2);
		break;
	case 2:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 1, 16, 2);
		break;
	case 4:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 2, 16, 2);
		break;
	default:
		printk("Err %s[%d]\n", __func__, __LINE__);
		break;
	}
}

void set_hpll_od2(unsigned div)
{
	switch (div) {
	case 1:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0, 18, 2);
		break;
	case 2:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 1, 18, 2);
		break;
	case 4:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 2, 18, 2);
		break;
	default:
		printk("Err %s[%d]\n", __func__, __LINE__);
		break;
	}
}

void set_hpll_od3(unsigned div)
{
	switch (div) {
	case 1:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0, 20, 2);
		break;
	case 2:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 1, 20, 2);
		break;
	case 4:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 2, 20, 2);
		break;
	default:
		printk("Err %s[%d]\n", __func__, __LINE__);
		break;
	}
}

/* HERE MUST BE BIT OPERATION!!! */
void set_hpll_sspll(struct hdmitx_dev *hdev)
{
	enum hdmi_vic vic = hdev->vic;

	switch (vic) {
	case HDMI_1920x1080p60_16x9:
	case HDMI_1920x1080p50_16x9:
	case HDMI_1280x720p60_16x9:
	case HDMI_1280x720p50_16x9:
	case HDMI_1920x1080i60_16x9:
	case HDMI_1920x1080i50_16x9:
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 1, 29, 1);
		/* bit[22:20] hdmi_dpll_fref_sel
		 * bit[8] hdmi_dpll_ssc_en
		 * bit[7:4] hdmi_dpll_ssc_dep_sel
		 */
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL2, 1, 20, 3);
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL2, 1, 8, 1);
		/* 2: 1000ppm  1: 500ppm */
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL2, 2, 4, 4);
		/* bit[15] hdmi_dpll_sdmnc_en */
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL3, 0, 15, 1);
		hd_set_reg_bits(P_HHI_HDMI_PLL_CNTL0, 0, 29, 1);
		break;
	default:
		break;
	}
}

void ddc_pinmux_init(void)
{
	hd_set_reg_bits(P_PAD_PULL_UP_EN_REG3, 0, 0, 2);    /*Disable GPIOH_1/2 pull-up/down*/
	hd_set_reg_bits(P_PAD_PULL_UP_REG3, 0, 0, 2);
	hd_set_reg_bits(P_PREG_PAD_GPIO3_EN_N, 3, 0, 2);     /*GPIOH_1/2 input*/
	hd_set_reg_bits(P_PERIPHS_PIN_MUX_B, 0x11, 0, 8);      /*Mux DDC SDA/SCL*/
}

int hdmitx_get_hpd_state(void)
{
	int st = 0;

	st = !!(hd_read_reg(P_PREG_PAD_GPIO3_I) & (1 << 2));
	return st;
}

void hdmitx_prbs(void)
{
	unsigned int i;

	for (i = 0; i < 4; i ++) {
		hd_write_reg(P_HHI_HDMI_PHY_CNTL1, 0x0390000f);
		hd_write_reg(P_HHI_HDMI_PHY_CNTL1, 0x0390000e);
		hd_write_reg(P_HHI_HDMI_PHY_CNTL1, 0x03904002);
		hd_write_reg(P_HHI_HDMI_PHY_CNTL4, 0x0001efff | (i << 20));
		hd_write_reg(P_HHI_HDMI_PHY_CNTL1, 0xef904002);
		mdelay(10);
		if (i > 0)
			pr_info("prbs D[%d]:%lx\n", i -1, hd_read_reg(P_HHI_HDMI_PHY_STATUS));
		else
			pr_info("prbs clk :%lx\n",hd_read_reg(P_HHI_HDMI_PHY_STATUS));
	}
}

void hdmitx_enable_apb3(void)
{
	/*g12a doesn't need enable apb3*/
	return;
	/*hd_set_reg_bits(P_HDMITX_CTRL_PORT, 1, 15, 1);*/
	/*hd_set_reg_bits((P_HDMITX_CTRL_PORT + 0x10), 1, 15, 1);*/
}
