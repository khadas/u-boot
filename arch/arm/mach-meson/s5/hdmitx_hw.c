// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <amlogic/media/vout/hdmitx21/hdmitx_module.h>
#include <amlogic/media/vout/hdmitx21/hdmitx_reg.h>
#include <amlogic/media/vout/hdmitx21/mach_reg.h>
#include <asm/arch/bl31_apis.h>
#include <linux/arm-smccc.h>
#include "hdmitx_misc.h"

static int hdmi_dbg;

static const struct reg_map reg21_maps[] = {
	[VPUCTRL_REG_IDX] = {
		.phy_addr = 0xff800000,
	},
	[HDMITX_COR_REG_IDX] = {
		.phy_addr = 0xfe3a0000,
	},
	[HDMITX_TOP_REG_IDX] = {
		.phy_addr = 0xfe390000,
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

	st = !!(hd21_read_reg(PADCTRL_GPIOH_I) & (1 << 2));
	return st;
}

void hdmitx21_mux_ddc(void)
{
	hd21_set_reg_bits(PADCTRL_PIN_MUX_REG8, 1, 0, 4); // hdmitx_sda
	hd21_set_reg_bits(PADCTRL_PIN_MUX_REG8, 1, 4, 4); // hdmitx_scl
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

/* only for S5 */
void hdmitx_phy_pre_init(struct hdmitx_dev *hdev)
{
	enum frl_rate_enum frl_rate = hdev->frl_rate;

	/* Stage1: reset registers */
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL0, 0x0);
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL1, 0x0);
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL3, 0x0);
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL5, 0x0);
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL6, 0x0);
	ndelay(10);

	/* Stage2: enable Bandgap */
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL5, 0x03, 0, 8);
	udelay(10);

	/* Stage3: enable LDO */
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL3, 0xbd, 24, 8);
	udelay(10);

	/* Stage4: enable dcc */
	if (frl_rate == FRL_3G3L || frl_rate == FRL_6G3L)
		hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL6, 0x57, 8, 8); /* power down channel 4 */
	else
		hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL6, 0x77, 8, 8);
	ndelay(10);

	/* Stage5: enable p2s */
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL6, 0x0b, 0, 8);
	if (frl_rate == FRL_3G3L || frl_rate == FRL_6G3L)
		hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL5, 0x3f, 8, 8); /* power down channel 4 */
	else
		hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL5, 0xff, 8, 8);
	ndelay(10);

	/* set phy ch0/1/2/3 swap as default */
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 0, 18, 2);
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 1, 20, 2);
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 2, 22, 2);
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 3, 24, 2);
	// wire    wr_enable = control[3];
	// wire    fifo_enable = control[2];
	// assign  phy_clk_en = control[1];
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 0, 0, 1); // Enable Soft_reset
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 1, 0, 1); // Enable Soft_reset
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 0, 0, 1); // Enable Soft_reset
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 1, 1, 1); // Enable tmds_clk
	// Enable enable the write/read decoupling state machine
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 1, 3, 1);
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL1, 1, 2, 1); // Enable the decoupling FIFO
}

static void hdmitx_set_s5_phypara(enum frl_rate_enum frl_rate, u32 tmds_clk)
{
	const u16 swing[] = {
		[FRL_NONE] = 0x0000,
		[FRL_3G3L] = 0x00d5,
		[FRL_6G3L] = 0x00cf,
		[FRL_6G4L] = 0xcfcf,
		[FRL_8G4L] = 0xcfcf,
		[FRL_10G4L] = 0xcfcf,
		[FRL_12G4L] = 0xafaf,
	};
	const u8 ffe[] = {
		[FRL_NONE] = 0x00,
		[FRL_3G3L] = 0x00,
		[FRL_6G3L] = 0x00,
		[FRL_6G4L] = 0x00,
		[FRL_8G4L] = 0x01,
		[FRL_10G4L] = 0x0a,
		[FRL_12G4L] = 0x0b,
	};
	const u8 drv[] = {
		[FRL_NONE] = 0x11,
		[FRL_3G3L] = 0x03,
		[FRL_6G3L] = 0x03,
		[FRL_6G4L] = 0x33,
		[FRL_8G4L] = 0x77,
		[FRL_10G4L] = 0x77,
		[FRL_12G4L] = 0x77,
	};
	u8 rterm = 0; /* this will get from ufuse */
	struct arm_smccc_res res;

	/* Stage6: enable Rterm */
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL0, 0xd8, 16, 8);
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL0, 0x3, 24, 2);
	arm_smccc_smc(HDCPTX_IOOPR, HDMITX_GET_RTERM, 0, 0, 0, 0, 0, 0, &res);
	rterm = (unsigned int)((res.a0) & 0xffffffff);
	rterm = rterm & 0x3f;
	printf("%s[%d] rterm = %d\n", __func__, __LINE__, rterm);
	if (!rterm)
		rterm = 9; /* default value when efuse invalid */
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL0, rterm, 26, 6);
	hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL3, 0x06, 8, 8);
	ndelay(10);

	/* Stage7: set output swing */
	if (frl_rate != FRL_NONE && frl_rate <= FRL_12G4L) {
		hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL0, swing[frl_rate], 0, 16);
	} else {
		u32 swing = 0;

		if (tmds_clk >= 290000)
			swing = 0x90d5;
		else if (tmds_clk >= 148000)
			swing = 0x90d4;
		else
			swing = 0x90d3;
		hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL0, swing, 0, 16);
	};
	ndelay(10);

	/* Stage8: set ffe */
	if (frl_rate != FRL_NONE && frl_rate <= FRL_12G4L)
		hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL3, ffe[frl_rate], 0, 8);
	else
		hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL3, 0, 0, 8);
	ndelay(10);

	/* Stage9: enable driver */
	if (frl_rate != FRL_NONE && frl_rate <= FRL_12G4L) {
		hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL3, drv[frl_rate], 16, 8);
	} else {
		u32 drv = 0;

		if (tmds_clk >= 290000)
			drv = 0x17;
		else if (tmds_clk >= 148000)
			drv = 0x13;
		else
			drv = 0x11;
		hd21_set_reg_bits(ANACTRL_HDMIPHY_CTRL3, drv, 16, 8);
	}
}

void hdmitx_set_phypara(enum hdmi_phy_para mode)
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();
	u32 tmds_clk = 0;

	tmds_clk = hdev->para->tmds_clk;
	printf("%s[%d] tmds_clk %d\n", __func__, __LINE__, tmds_clk);
	hdmitx_set_s5_phypara(hdev->frl_rate, tmds_clk);
}

void hdmitx_turnoff(void)
{
	/* Close HDMITX PHY */
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL0, 0);
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL3, 0);
	hd21_write_reg(ANACTRL_HDMIPHY_CTRL6, 0);
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
