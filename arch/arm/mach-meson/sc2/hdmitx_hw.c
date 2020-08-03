#include <common.h>
#include <asm/io.h>
#include <amlogic/media/vout/hdmitx/hdmitx_module.h>
#include <amlogic/media/vout/hdmitx/hdmitx_reg.h>
#include <amlogic/media/vout/hdmitx/mach_reg.h>
#include <asm/arch/bl31_apis.h>
#include "hdmitx_misc.h"

static struct reg_map reg_maps[] = {
	[CBUS_REG_IDX] = { /* CBUS */
		.base_addr = 0xfe001000,
	},
	[PERIPHS_REG_IDX] = { /* PERIPHS */
		.base_addr = 0xfe004000,
	},
	[VCBUS_REG_IDX] = { /* VPU */
		.base_addr = 0xff000000,
	},
	[HHI_REG_IDX] = { /* HIU */
		.base_addr = 0xfe000000,
	},
	[SYSCTRL_REG_IDX] = {
		.base_addr = 0xfe010000,
	},
	[RESETCTRL_REG_IDX] = {
		.base_addr = 0xfe002000,
	},
	[ANACTRL_REG_IDX] = {
		.base_addr = 0xfe008000,
	},
	[PWRCTRL_REG_IDX] = {
		.base_addr = 0xfe00c000,
	},
	[HDMITX_SEC_REG_IDX] = { /* HDMITX DWC LEVEL*/
		.base_addr = 0xfe300000,
	},
	[HDMITX_REG_IDX] = { /* HDMITX TOP LEVEL*/
		.base_addr = 0xfe308000,
	},
	[REG_IDX_END] = {
	},
};

#define HDMITX_DWC_BASE_OFFSET (reg_maps[HDMITX_SEC_REG_IDX].base_addr)
#define HDMITX_TOP_BASE_OFFSET (reg_maps[HDMITX_REG_IDX].base_addr)

int hdmitx_get_hpd_state(void)
{
	int st = 0;

	st = !!(hd_read_reg(P_PADCTRL_GPIOH_I) & (1 << 2));
	return st;
}

void hdmitx_ddc_init(void)
{
	/*Mux DDC SDA/SCL*/
	hd_set_reg_bits(P_PADCTRL_PIN_MUX_REGB, 1, 0, 4);
	hd_set_reg_bits(P_PADCTRL_PIN_MUX_REGB, 1, 4, 4);
}

static uint32_t get_base_addr(uint32_t idx)
{
	if (idx >= REG_IDX_END)
		return 0;
	if (!reg_maps[idx].base_addr)
		printf("%s[%d] idx = %d\n", __func__, __LINE__, idx);

	return reg_maps[idx].base_addr;
}

uint32_t hd_get_paddr(uint32_t addr)
{
	uint32_t idx = (addr) >> BASE_REG_OFFSET;
	uint32_t offset_addr = ((1 << BASE_REG_OFFSET) - 1) & (addr);
	uint32_t paddr;

	paddr = get_base_addr(idx) + offset_addr;
	return paddr;
}

uint32_t hd_read_reg(uint32_t addr)
{
	uint32_t val;
	uint32_t idx = (addr) >> BASE_REG_OFFSET;
	uint32_t offset_addr = ((1 << BASE_REG_OFFSET) - 1) & (addr);
	void *paddr;

	paddr = (void *)((unsigned long)(get_base_addr(idx) + offset_addr));
	val = readl(paddr);

	return val;
}

void hd_write_reg(unsigned int addr, unsigned int val)
{
	uint32_t idx = (addr) >> BASE_REG_OFFSET;
	uint32_t offset_addr = ((1 << BASE_REG_OFFSET) - 1) & (addr);
	void *paddr;

	paddr = (void *)((unsigned long)(get_base_addr(idx) + offset_addr));
	writel(val, paddr);
}

void hd_set_reg_bits(unsigned int addr, unsigned int value,
	unsigned int offset, unsigned int len)
{
	unsigned int data32 = 0;

	data32 = hd_read_reg(addr);
	data32 &= ~(((1 << len) - 1) << offset);
	data32 |= (value & ((1 << len) - 1)) << offset;
	hd_write_reg(addr, data32);
}

static unsigned int hdmitx_rd_reg_normal(unsigned int addr)
{
	unsigned int data;
	register long x0 asm("x0") = 0x82000018;
	register long x1 asm("x1") = (unsigned long)addr;

	asm volatile(
		__asmeq("%0", "x0")
		__asmeq("%1", "x1")
		"smc #0\n"
		: "+r"(x0) : "r"(x1)
	);
	data = (unsigned int)(x0&0xffffffff);

	return data;
}

static void hdmitx_wr_reg_normal(unsigned int addr, unsigned int data)
{
	register long x0 asm("x0") = 0x82000019;
	register long x1 asm("x1") = (unsigned long)addr;
	register long x2 asm("x2") = data;

	asm volatile(
		__asmeq("%0", "x0")
		__asmeq("%1", "x1")
		__asmeq("%2", "x2")
		"smc #0\n"
		: : "r"(x0), "r"(x1), "r"(x2)
	);
}

unsigned int hdmitx_rd_reg(unsigned int addr)
{
	unsigned int large_offset = addr >> 24;
	unsigned int small_offset = addr & ((1 << 24)  - 1);
	unsigned long hdmitx_addr = 0;
	unsigned int data;

	switch (large_offset) {
	case 0x10:
		/*DWC*/
		hdmitx_addr = HDMITX_DWC_BASE_OFFSET + small_offset;
		data = readb(hdmitx_addr);
		break;
	case 0x11:
	case 0x01:
		data = hdmitx_rd_reg_normal(addr);
		break;
	case 0x00:
	default:
		/* TOP */
		if ((small_offset >= 0x2000) && (small_offset <= 0x365E)) {
			hdmitx_addr = HDMITX_TOP_BASE_OFFSET + small_offset;
			data = readb(hdmitx_addr);
		} else {
			hdmitx_addr = HDMITX_TOP_BASE_OFFSET + (small_offset << 2);
			data = readl(hdmitx_addr);
		}
		break;
	}

	return data;
}

void hdmitx_wr_reg(unsigned int addr, unsigned int data)
{
	unsigned int large_offset = addr >> 24;
	unsigned int small_offset = addr & ((1 << 24)  - 1);
	unsigned long hdmitx_addr = 0;

	switch (large_offset) {
	case 0x10:
		/*DWC*/
		hdmitx_addr = HDMITX_DWC_BASE_OFFSET + small_offset;
		writeb(data & 0xff, hdmitx_addr);
		break;
	case 0x11:
	case 0x01:
		/*SECURITY DWC/TOP*/
		hdmitx_wr_reg_normal(addr, data);
		break;
	case 00:
	default:
		/*TOP*/
		if ((small_offset >= 0x2000) && (small_offset <= 0x365E)) {
			hdmitx_addr = HDMITX_TOP_BASE_OFFSET + small_offset;
			writeb(data & 0xff, hdmitx_addr);
		} else {
			hdmitx_addr = HDMITX_TOP_BASE_OFFSET + (small_offset << 2);
			writel(data, hdmitx_addr);
		}
	}
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

void hdmitx_poll_reg(unsigned int addr, unsigned int val, unsigned long timeout)
{
}

unsigned int hdmitx_rd_check_reg(unsigned int addr, unsigned int exp_data,
	unsigned int mask)
{
	return 0;
}

void hdmitx_hdcp_init(void)
{
	register long x0 asm("x0") = 0x82000012;
	asm volatile(
		__asmeq("%0", "x0")
		"smc #0\n"
		: : "r"(x0)
	);
}

void hdmitx_set_phypara(enum hdmi_phy_para mode)
{
	hd_write_reg(P_ANACTRL_HDMIPHY_CTRL0, 0x0);
/* P_ANACTRL_HDMIPHY_CTRL1	bit[1]: enable clock	bit[0]: soft reset */
#define RESET_HDMI_PHY() \
do { \
	hd_set_reg_bits(P_ANACTRL_HDMIPHY_CTRL1, 0xf, 0, 4); \
	mdelay(2); \
	hd_set_reg_bits(P_ANACTRL_HDMIPHY_CTRL1, 0xe, 0, 4); \
	mdelay(2); \
} while (0)

	hd_set_reg_bits(P_ANACTRL_HDMIPHY_CTRL1, 0x0390, 16, 16);
	hd_set_reg_bits(P_ANACTRL_HDMIPHY_CTRL1, 0x0, 0, 4);
	RESET_HDMI_PHY();
	RESET_HDMI_PHY();
#undef RESET_HDMI_PHY

	switch (mode) {
	case HDMI_PHYPARA_6G: /* 5.94/4.5/3.7Gbps */
	case HDMI_PHYPARA_4p5G:
	case HDMI_PHYPARA_3p7G:
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL0, 0x37eb65c4);
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL3, 0x2ab0ff3b);
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL5, 0x0000080b);
		break;
	case HDMI_PHYPARA_3G: /* 2.97Gbps */
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL0, 0x33eb42a2);
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL3, 0x2ab0ff3b);
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL5, 0x00000003);
		break;
	case HDMI_PHYPARA_270M: /* 1.485Gbps, and below */
	case HDMI_PHYPARA_DEF:
	default:
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL0, 0x33eb4252);
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL3, 0x2ab0ff3b);
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL5, 0x00000003);
		break;
	}
}

void hdmitx_turnoff(void)
{
	/* Close HDMITX PHY */
	hd_write_reg(P_ANACTRL_HDMIPHY_CTRL0, 0);
	hd_write_reg(P_ANACTRL_HDMIPHY_CTRL3, 0);
	hd_write_reg(P_ANACTRL_HDMIPHY_CTRL5, 0);
	/* Disable HPLL */
	hd_write_reg(ANACTRL_HDMIPLL_CTRL0, 0);
}

void hdmitx_test_prbs(void)
{
	unsigned int i;

	for (i = 0; i < 4; i ++) {
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL1, 0x0390000f);
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL1, 0x0390000e);
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL1, 0x03904002);
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL4, 0x0001efff | (i << 20));
		hd_write_reg(P_ANACTRL_HDMIPHY_CTRL1, 0xef904002);
		mdelay(1);
		if (i == 0)
			printf("prbs clk: %08x\n",hd_read_reg(P_ANACTRL_HDMIPLL_CTRL6));
		else
			printf("prbs D[%d]: %08x\n", i -1, hd_read_reg(P_ANACTRL_HDMIPLL_CTRL6));
	}
}
