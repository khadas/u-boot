// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/io.h>
#include <amlogic/media/vout/hdmitx21/hdmitx_module.h>
#include <amlogic/media/vout/hdmitx21/hdmitx_reg.h>
#include <amlogic/media/vout/hdmitx21/mach_reg.h>
#include <asm/arch/bl31_apis.h>
#include <linux/arm-smccc.h>

static int hdmi_dbg;

static const struct reg_map reg21_maps[] = {
	[VPUCTRL_REG_IDX] = {
		.phy_addr = 0xff000000,
	},
	[HDMITX_COR_REG_IDX] = {
		.phy_addr = 0xfe380000,
	},
	[HDMITX_TOP_REG_IDX] = {
		.phy_addr = 0xfe300000,
	},
	[SYSCTRL_REG_IDX] = {
		.phy_addr = 0xfe010000,
	},
	[PWRCTRL_REG_IDX] = {
		.phy_addr = 0xfe00c000,
	},
	[ANACTRL_REG_IDX] = {
		.phy_addr = 0xfe008000,
	},
	[RESETCTRL_REG_IDX] = {
		.phy_addr = 0xfe002000,
	},
	[CLKCTRL_REG_IDX] = {
		.phy_addr = 0xfe000000,
	},
	[PADCTRL_REG_IDX] = {
		.phy_addr = 0xfe004000,
	},
};

int hdmitx_get_hpd_state(void)
{
	int st = 0;

	st = !!(hd21_read_reg(PADCTRL_GPIOW_I) & (1 << 15));
	return st;
}

static void sec_wr(u32 addr, u32 data)
{
	struct arm_smccc_res res;

	arm_smccc_smc(0x82000019, (unsigned long)addr, data, 32, 0, 0, 0, 0, &res);
	if (hdmi_dbg)
		pr_info("sec_wr32[0x%08x] 0x%08x\n", addr, data);
}

static void sec_wr8(u32 addr, u8 data)
{
	struct arm_smccc_res res;

	arm_smccc_smc(0x82000019, (unsigned long)addr, data & 0xff, 8, 0, 0, 0, 0, &res);
	if (hdmi_dbg)
		pr_info("[0x%08x] 0x%02x\n", addr, data);
}

static u32 sec_rd(u32 addr)
{
	u32 data;
	struct arm_smccc_res res;

	arm_smccc_smc(0x82000018, (unsigned long)addr, 32, 0, 0, 0, 0, 0, &res);
	data = (unsigned int)((res.a0) & 0xffffffff);

	if (hdmi_dbg)
		pr_info("[0x%08x] 0x%08x\n", addr, data);
	return data;
}

static u8 sec_rd8(u32 addr)
{
	u32 data;
	struct arm_smccc_res res;

	arm_smccc_smc(0x82000018, (unsigned long)addr, 8, 0, 0, 0, 0, 0, &res);
	data = (unsigned int)((res.a0) & 0xffffffff);

	if (hdmi_dbg)
		pr_info("[0x%08x] 0x%02x\n", addr, data);
	return data;
}

u32 TO21_PHY_ADDR(u32 addr)
{
	u32 index;
	u32 offset;

	index = addr >> BASE_REG_OFFSET;
	offset = addr & (((1 << BASE_REG_OFFSET) - 1));

	return (reg21_maps[index].phy_addr + offset);
}

static u32 get_enc_paddr(unsigned int addr)
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();
	unsigned int idx = addr >> BASE_REG_OFFSET;
	unsigned int offset = (addr & 0xffff) >> 2;

	if (hdev->enc_idx == 2 && idx == VPUCTRL_REG_IDX) {
		if (offset >= 0x1b00 && offset < 0x1d00)
			return addr + (0x800 << 2);
	}
	return addr;
}

u32 hd21_read_reg(u32 vaddr)
{
	u32 val;
	u32 paddr = TO21_PHY_ADDR(get_enc_paddr(vaddr));

	val = readl(paddr);
	if (hdmi_dbg)
		pr_info("Rd32[0x%08x] 0x%08x\n", paddr, val);
	return val;
}

void hd21_write_reg(u32 vaddr, u32 val)
{
	u32 rval;
	u32 paddr = TO21_PHY_ADDR(get_enc_paddr(vaddr));

	writel(val, paddr);
	rval = readl(paddr);
	if (!hdmi_dbg)
		return;
	if (val != rval)
		pr_info("Wr32[0x%08x] 0x%08x != Rd32 0x%08x\n", paddr, val, rval);
	else
		pr_info("Wr32[0x%08x] 0x%08x\n", paddr, val);
}

void hd21_set_reg_bits(u32 addr, u32 value,
		     u32 offset, u32 len)
{
	u32 data32 = 0;

	data32 = hd21_read_reg(addr);
	data32 &= ~(((1 << len) - 1) << offset);
	data32 |= (value & ((1 << len) - 1)) << offset;
	hd21_write_reg(addr, data32);
}

static u32 hdmitx_rd_top(u32 addr)
{
	u32 base_offset;
	u32 data;

	base_offset = reg21_maps[HDMITX_TOP_REG_IDX].phy_addr;

	data = sec_rd(base_offset + addr);
	return data;
} /* hdmitx_rd_top */

static u8 hdmitx_rd_cor(u32 addr)
{
	u32 base_offset;
	u8 data;

	base_offset = reg21_maps[HDMITX_COR_REG_IDX].phy_addr;
	data = sec_rd8(base_offset + addr);
	return data;
} /* hdmitx_rd_cor */

static void hdmitx_wr_top(u32 addr, u32 data)
{
	u32 base_offset;

	base_offset = reg21_maps[HDMITX_TOP_REG_IDX].phy_addr;
	sec_wr(base_offset + addr, data);
} /* hdmitx_wr_top */

static void hdmitx_wr_cor(u32 addr, u8 data)
{
	u32 base_offset;

	base_offset = reg21_maps[HDMITX_COR_REG_IDX].phy_addr;
	sec_wr8(base_offset + addr, data);
} /* hdmitx_wr_cor */

u32 hdmitx21_rd_reg(u32 addr)
{
	u32 offset = (addr & TOP_OFFSET_MASK) >> 24;
	u32 data;

	addr = addr & 0xffff;
	if (offset)
		data = hdmitx_rd_top(addr);
	else
		data = hdmitx_rd_cor(addr);

	return data;
}

void hdmitx21_wr_reg(u32 addr, u32 val)
{
	u32 offset = (addr & TOP_OFFSET_MASK) >> 24;

	addr = addr & 0xffff;
	if (offset)
		hdmitx_wr_top(addr, val);
	else
		hdmitx_wr_cor(addr, val);
}

bool hdmitx21_get_bit(u32 addr, u32 bit_nr)
{
	return (hdmitx21_rd_reg(addr) & (1 << bit_nr)) == (1 << bit_nr);
}

void hdmitx21_set_reg_bits(u32 addr, u32 value,
			 u32 offset, u32 len)
{
	u32 data32 = 0;

	data32 = hdmitx21_rd_reg(addr);
	data32 &= ~(((1 << len) - 1) << offset);
	data32 |= (value & ((1 << len) - 1)) << offset;
	hdmitx21_wr_reg(addr, data32);
}

void hdmitx21_set_bit(u32 addr, u32 bit_val, bool st)
{
	u32 data32 = 0;

	data32 = hdmitx21_rd_reg(addr);
	data32 = st ? (data32 | bit_val) : (data32 & ~bit_val);
	hdmitx21_wr_reg(addr, data32);
}

void hdmitx21_poll_reg(unsigned int addr, unsigned int exp_data, unsigned int mask,
	unsigned int max_try)
{
	unsigned int rd_data;
	unsigned int cnt = 0;
	unsigned int done = 0;

	rd_data = hdmitx21_rd_reg(addr);
	while (((cnt < max_try) || (max_try == 0)) && (done != 1)) {
		if ((rd_data | mask) == (exp_data | mask)) {
			done = 1;
		} else {
			cnt++;
			rd_data = hdmitx21_rd_reg(addr);
		}
	}
	if (done == 0)
		pr_info("%s 0x%x access time-out!\n", __func__, addr);
} /* hdmitx21_poll_reg */

u32 hdmitx21_rd_check_reg(u32 addr, u32 exp_data,
				 u32 mask)
{
	unsigned long rd_data;

	rd_data = hdmitx21_rd_reg(addr);
	if ((rd_data | mask) != (exp_data | mask)) {
		pr_info("HDMITX-DWC addr=0x%04x rd_data=0x%02x\n",
			(unsigned int)addr, (unsigned int)rd_data);
		pr_info("HDMITX-DWC exp_data=0x%02x mask=0x%02x\n",
			(unsigned int)exp_data, (unsigned int)mask);
		return 1;
	}
	return 0;
}

static u32 get_base_addr(u32 idx)
{
	if (idx >= REG_IDX_END)
		return 0;
	if (!reg21_maps[idx].phy_addr)
		printf("%s[%d] idx = %d\n", __func__, __LINE__, idx);

	return reg21_maps[idx].phy_addr;
}

u32 hd_get_paddr(u32 addr)
{
	u32 idx = (addr) >> BASE_REG_OFFSET;
	u32 offset_addr = ((1 << BASE_REG_OFFSET) - 1) & (addr);
	u32 paddr;

	paddr = get_base_addr(idx) + offset_addr;
	return paddr;
}

void hdmitx_set_phypara(enum hdmi_phy_para mode)
{
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL0, 0x0);
/* P_ANACTRL_HDMIPHY_CTRL1	bit[1]: enable clock	bit[0]: soft reset */
#define RESET_HDMI_PHY() \
do { \
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 0xf, 0, 4); \
	mdelay(2); \
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 0xe, 0, 4); \
	mdelay(2); \
} while (0)

	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 0x0390, 16, 16);
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 0x0, 0, 4);
	RESET_HDMI_PHY();
	RESET_HDMI_PHY();
#undef RESET_HDMI_PHY

	switch (mode) {
	case HDMI_PHYPARA_6G: /* 5.94/4.5/3.7Gbps */
	case HDMI_PHYPARA_4p5G:
	case HDMI_PHYPARA_3p7G:
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL5, 0x0000080b);
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL0, 0x37eb65c4);
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL3, 0x2ab0ff3b);
		break;
	case HDMI_PHYPARA_3G: /* 2.97Gbps */
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL5, 0x00000003);
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL0, 0x33eb42a2);
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL3, 0x2ab0ff3b);
		break;
	case HDMI_PHYPARA_270M: /* 1.485Gbps, and below */
	case HDMI_PHYPARA_DEF:
	default:
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL5, 0x00000003);
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL0, 0x33eb4252);
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL3, 0x2ab0ff3b);
		break;
	}
}

void hdmitx_turnoff(void)
{
	/* Close HDMITX PHY */
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL0, 0);
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL3, 0);
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL5, 0);
	/* Disable HPLL */
	hd21_write_reg(ANACTRL_HDMIPLL_CTRL0, 0);
}

void hdmitx_test_prbs(void)
{
	unsigned int i;

	for (i = 0; i < 4; i ++) {
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL1, 0x0390000f);
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL1, 0x0390000e);
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL1, 0x03904002);
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL4, 0x0001efff | (i << 20));
		hd21_write_reg(ANACTRL_HDMIPHY_CTRL1, 0xef904002);
		mdelay(1);
		if (i == 0)
			printf("prbs clk: %08x\n", hd21_read_reg(ANACTRL_HDMIPLL_CTRL6));
		else
			printf("prbs D[%d]: %08x\n", i - 1, hd21_read_reg(ANACTRL_HDMIPLL_CTRL6));
	}
}
