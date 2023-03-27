// SPDX-License-Identifier: GPL-2.0+
/*
 * Meson g12b, revB USB2 PHY driver
 *
 * Copyright (C) 2017 Martin Blumenstingl <martin.blumenstingl@googlemail.com>
 * Copyright (C) 2018 BayLibre, SAS
 * Author: Neil Armstrong <narmstron@baylibre.com>
 */

#include <common.h>
#include <asm/io.h>
#include <bitfield.h>
#include <dm.h>
#include <errno.h>
#include <generic-phy.h>
#include <regmap.h>
#include <power/regulator.h>
#include <clk.h>
#include <asm/arch/usb.h>
#include <amlogic/cpu_id.h>

#include <linux/compat.h>
#include <linux/ioport.h>
#include <asm-generic/gpio.h>

#define USB_NONE		0
#define USB_NOVALID		1
#define USB_PHY_NO_READY	2
#define USB_PLL_NO_LOCK		3
#define USB_TIMEOUT		4
#define USB_CNTL_ID		5
#define USB_MODE		6

//--------------------------------------------------------------
// AHB slave and APB3 slave
#define U3D_BASE 0xFE3B0000
#define USB_AML_BASE 0xfe03a000
#define RESET_CTRL_BASE	0xfe002000
#define M31_PHY_BASE 0xFE3BC000
#define M31_PHY_DEVICE_SETTING 0x1E30CE89
#define PIPE_CLK_REGS 0xfe00015c
#define PIPE_CLK_GATE_REGS 0xfe3b409c

#define M31_SETTING 0x1E30CEB9
#define M31_PHY_0_BASE  0xFE3BC000
#define M31_PHY_1_BASE  0xFE31C000

#define USB_AML_U21_REGS (u2p_aml_regs_t *)(USB_AML_BASE + 0x20)
#define USB_AML_U30_REGS (usb_aml_regs_t *)(USB_AML_BASE + 0x80)

#define USB_RESET_REGISTER				(0x0 + RESET_CTRL_BASE)
#define USB_RESET_REGISTER_1			(0x4 + RESET_CTRL_BASE)
#define USB_RESET_LEVEL_REGISTER		(0x40 + RESET_CTRL_BASE)
#define USB_RESET_LEVEL_REGISTER_1		(0x40 + RESET_CTRL_BASE + 0x4)

#define USB_PHY_BIT	8
#define USB_2_DRD_BIT	4

#define USB30_DRD_BIT	6
#define USB30_UTMI_BIT		2
#define USB30_PCIEPHY_BIT	10

#define M31_0_RESET_LEVEL_BIT 10
#define M31_1_UTMI_RESET_LEVEL_BIT 1
#define M31_0_UTMI_RESET_LEVEL_BIT 2
#define CRG_U3DRD_0_RESET_LEVEL_BIT 6

#define M31_1_RESET_LEVEL_BIT 9

#define CRG_U3DRD_1_RESET_LEVEL_BIT 5

#define USBPLL_RESET_BIT	29
#define USBPLL_EN_BIT		28

#define	PHY20_RESET_LEVEL_BIT	8
#define	PHY21_RESET_LEVEL_BIT	7

#define RESET_BASE 0xFE002000

#define USB2_PHY_PLL_OFFSET_40	(0x09400414)
#define USB2_PHY_PLL_OFFSET_44	(0x927E0000)
#define USB2_PHY_PLL_OFFSET_48	(0xac5f69e5)

#define USB2_PHY_PLL_OFFSET_10	(0x80000fff)
#define USB2_PHY_PLL_OFFSET_34	(0x78000)
#define USB2_REVB_PHY_PLL_OFFSET_34	(0x70000)

#define USB2_PHY_PLL_OFFSET_38_CLEAR	(0)
#define USB2_PHY_PLL_OFFSET_38_SET	    (0xe0004)
#define USB2_PHY_PLL_OFFSET_50	(0xfe18)
#define USB2_PHY_PLL_OFFSET_54	(0x2a)

#define TUNING_DISCONNECT_THRESHOLD 0x3C
#define DISCONNECT_THRESHOLD_ENHANCE (0x2)

#define AMLOGIC_CTR_COUNT		(0x1)

static int Rev_flag = 0;

struct ctr_info {
	struct phy usb_phys[4];
	unsigned int phy_count;
};

static struct ctr_info ctr[AMLOGIC_CTR_COUNT];

int get_usbphy_baseinfo(void)
{
	struct udevice *bus;
	struct uclass *uc;
	int ret, i, j = 0;
	int count;

	ret = uclass_get(UCLASS_USB, &uc);
	if (ret)
		return ret;
	uclass_foreach_dev(bus, uc) {
		debug("bus->name=%s, bus->driver->name =%s\n",
			bus->name, bus->driver->name);
		count = dev_count_phandle_with_args(bus, "phys", "#phy-cells");
		debug("usb phy cells=%d\n", count);
		if (count <= 0) {
			ctr[j].phy_count = 0;
			continue;
		}
		for (i = 0; i < count; i++) {
			if (ctr[j].usb_phys[i].dev)
				continue;
			ret = generic_phy_get_by_index(bus, i, &ctr[j].usb_phys[i]);
			if (ret && ret != -ENOENT) {
				pr_err("Failed to get USB PHY%d for %s\n",
				       i, bus->name);
				return ret;
			}
			ret = generic_phy_getinfo(&ctr[j].usb_phys[i]);
			if (ret)
				return ret;
		}
		ctr[j].phy_count = count;
		j++;
	}
	return 0;
}

void usb_aml_detect_operation(int argc, char * const argv[])
{
	struct phy_aml_usb2_priv *usb2_priv;
	struct phy_aml_usb3_priv *usb3_priv;
	int ret, i, j;

	ret = get_usbphy_baseinfo();
	if (ret) {
		printf("get usb dts failed\n");
		return;
	}
	for (i = 0; i < AMLOGIC_CTR_COUNT; i++) {
		usb2_priv = NULL;
		usb3_priv = NULL;
		if (ctr[i].usb_phys[0].dev)
			usb2_priv = dev_get_priv(ctr[i].usb_phys[0].dev);
		if (ctr[i].usb_phys[1].dev)
			usb3_priv = dev_get_priv(ctr[i].usb_phys[1].dev);
		printf("controller index=%d: phy-cells=%d:\n", i, ctr[i].phy_count);
		if (usb3_priv) {
			printf("priv->usb3 port num = %d, config addr=0x%08x\n",
				usb3_priv->usb3_port_num, usb3_priv->base_addr);
		}
		if (usb2_priv) {
			printf("usb2 phy: config addr = 0x%08x, reset addr=0x%08x\n",
				usb2_priv->base_addr, usb2_priv->reset_addr);
			printf("usb2 phy: portnum=%d\n", usb2_priv->u2_port_num);
			for (j = 0; j < usb2_priv->u2_port_num; j++)
				printf("phy%d-addr= 0x%08x\n", j, usb2_priv->usb_phy2_pll_base_addr[j]);
		}
	}
	printf("PHY version is 0x%02x\n", Rev_flag);
}

static void usb_set_calibration_trim(uint32_t phy2_pll_base)
{
	uint32_t cali, value, i;
	uint32_t cali_en;

	cali = readl(SYSCTRL_SEC_STATUS_REG12);
	/*****if cali_en ==0, set 0x10 to the default value: 0x1700****/
	cali_en = (cali >> 12) & 0x1;
	cali = cali >> 8;
	if (cali_en) {
		cali = cali & 0xf;
		if (cali > 12)
			cali = 12;
	} else {
		cali = 0x7;
	}
	value = (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x10));
	value &= (~0xfff);
	for (i = 0; i < cali; i++)
		value |= (1 << i);

	(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x10))
		= value;
	printf("0x10 trim value=0x%08x\n", value);
}

void usb_reset(unsigned int reset_addr, int bit)
{
	*(unsigned int *)(unsigned long)reset_addr = (1 << bit);
}

static void usb_enable_phy_pll(u32 base_addr)
{
	*(volatile uint32_t *)(unsigned long)
		RESETCTRL_RESET0_LEVEL |= (1 << PHY20_RESET_LEVEL_BIT);
	*(volatile uint32_t *)(unsigned long)
		RESETCTRL_RESET0_LEVEL |= (1 << PHY21_RESET_LEVEL_BIT);
}

void set_usb_pll(uint32_t phy2_pll_base)
{
	int val;

	val = USB2_PHY_PLL_OFFSET_40 | USB_PHY2_RESET | USB_PHY2_ENABLE;
	writel(val, phy2_pll_base + 0x40);
	val = USB2_PHY_PLL_OFFSET_44;
	writel(val, phy2_pll_base + 0x44);
	val = USB2_PHY_PLL_OFFSET_48;
	writel(val, phy2_pll_base + 0x48);
	udelay(100);
	val = ((USB2_PHY_PLL_OFFSET_40) | (USB_PHY2_ENABLE))
			& (~(USB_PHY2_RESET));
	writel(val, phy2_pll_base + 0x40);

	//(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x50))
		//= USB2_PHY_PLL_OFFSET_50;
	writel(USB2_PHY_PLL_OFFSET_50, phy2_pll_base + 0x50);
	writel(USB2_PHY_PLL_OFFSET_54, phy2_pll_base + 0x54);
	usb_set_calibration_trim(phy2_pll_base);
	val = readl(phy2_pll_base + 0x38);
	val &= ~0x18000000;
	val |= ((DISCONNECT_THRESHOLD_ENHANCE << 27) & 0x18000000);
	writel(val, phy2_pll_base + 0x38);
	writel(TUNING_DISCONNECT_THRESHOLD, phy2_pll_base + 0xc);
	writel(USB2_PHY_PLL_OFFSET_34, phy2_pll_base + 0x34);

	debug("tuning_disconnect_threshold=0x%x\n", TUNING_DISCONNECT_THRESHOLD);
}

int usb_save_phy_dev(unsigned int number, struct phy *phy)
{
	int i;

	for (i = 0; i < AMLOGIC_CTR_COUNT; i++) {
		if (!ctr[i].usb_phys[number].dev) {
			ctr[i].usb_phys[number].dev = phy->dev;
			ctr[i].usb_phys[number].id = phy->id;
		} else {
			if (ctr[i].usb_phys[number].dev == phy->dev)
				break;
		}
	}
	return 0;
}

void power_down_usb3(void)
{
	*(unsigned int *)(unsigned long)M31_PHY_0_BASE = M31_SETTING;
	*(unsigned int *)(unsigned long)M31_PHY_1_BASE = M31_SETTING;
}

int usb2_phy_init (struct phy *phy)
{
	struct phy_aml_usb2_priv *priv = dev_get_priv(phy->dev);
	struct u2p_aml_regs *u2p_aml_reg;
	u2p_r0_t dev_u2p_r0;
	u2p_r1_t dev_u2p_r1;
	int i, cnt;

	power_down_usb3();
	usb_save_phy_dev(0, phy);
	usb_enable_phy_pll(priv->base_addr);
	//usb_set_power_domain();

	//*(unsigned int *)(unsigned long)(priv->reset_addr + 0X4) = (1 << USB_GENERAL_BIT);
	udelay(500);
	priv->usbphy_reset_bit[0] = PHY20_RESET_LEVEL_BIT;
	priv->usbphy_reset_bit[1] = PHY21_RESET_LEVEL_BIT;

	for (i = 0; i < priv->u2_port_num; i++) {
		u2p_aml_reg = (struct u2p_aml_regs *)((ulong)(priv->base_addr + i * PHY_REGISTER_SIZE));
		dev_u2p_r0.d32 = u2p_aml_reg->u2p_r0;
		dev_u2p_r0.b.host_device = 1;
		dev_u2p_r0.b.POR = 0;
		u2p_aml_reg->u2p_r0  = dev_u2p_r0.d32;
		udelay(10);
		*(unsigned int *)(unsigned long)priv->reset_addr = (1 << priv->usbphy_reset_bit[i]);
		udelay(50);

		/* wait for phy ready */
		dev_u2p_r1.d32  = u2p_aml_reg->u2p_r1;
		cnt = 0;
		while ((dev_u2p_r1.b.phy_rdy != 1) && (cnt <= 200)) {
			dev_u2p_r1.d32 = u2p_aml_reg->u2p_r1;
			/*we wait phy ready max 1ms, common is 100us*/
			cnt++;
			udelay(5);
		}
	}

	for (i = 0; i < priv->u2_port_num; i++) {
		debug("------set usb pll\n");
		set_usb_pll(priv->usb_phy2_pll_base_addr[i]);
	}
	return 0;

}

int usb2_phy_tuning(uint32_t phy2_pll_base, int port)
{
	return 0;
}

int m31_phy_init(unsigned int phy_num)
{
	if (phy_num == 0) {
		*(unsigned int *)(unsigned long)M31_PHY_0_BASE = M31_SETTING;
		//udelay(20);
		//usb_reset(RESET_BASE, USB_RESET_BIT);
		udelay(20);
		usb_reset(RESET_BASE, M31_0_RESET_LEVEL_BIT | M31_0_UTMI_RESET_LEVEL_BIT);
		udelay(20);
		usb_reset(RESET_BASE, CRG_U3DRD_0_RESET_LEVEL_BIT);
		udelay(20);
	} else if (phy_num == 1) {
		*(unsigned int *)(unsigned long)M31_PHY_1_BASE = M31_SETTING;
		udelay(20);
		usb_reset(RESET_BASE, M31_1_RESET_LEVEL_BIT | M31_1_UTMI_RESET_LEVEL_BIT);
		udelay(20);
		usb_reset(RESET_BASE, CRG_U3DRD_1_RESET_LEVEL_BIT);
		udelay(20);
	}

	return 0;
}


/**************************************************************/
/*           device mode config                               */
/**************************************************************/

static int set_usb3_phy_config(void)
{
	uint32_t val;

	/*step 1: power on domain, if default is not on*/
	/*default is power on*/

	/*step 2: usb bus clock*/
	/*sys_clk  gate*/
	setbits_le32(CLKCTRL_SYS_CLK_EN0_REG1, ((1 << 26) | (1 << 27)));
	udelay(12);

	/*step 3: power on*/
	writel((readl(USB_RESET_LEVEL_REGISTER) | (1 << USB30_DRD_BIT) |
		(1 << USB30_UTMI_BIT) | (1 << USB30_PCIEPHY_BIT)), USB_RESET_LEVEL_REGISTER);
	udelay(12);

	/*step 4: usb controller reset*/
	/*bit21: u3drd0 general reset*/
	writel((1 << USB_GENERAL_BIT), USB_RESET_REGISTER_1);
	udelay(12);

	/*bit2: usb3drd0 utmi reset   bit6: usb3drd0 reset   bit10: usb30 phy reset*/
	writel((1 << USB30_DRD_BIT) | (1 << USB30_UTMI_BIT) | (1 << USB30_PCIEPHY_BIT),
		USB_RESET_REGISTER);
	udelay(12);

	/*step 5: m31 phy setting*/
	*(unsigned int *)(unsigned long)M31_PHY_BASE = M31_PHY_DEVICE_SETTING;
	udelay(50);

	/*setp 6: bypass usb3*/
	val = readl(M31_PHY_BASE + 0x8);
	val |= (0x1830 << 6);
	writel(val, M31_PHY_BASE + 0x8);
	udelay(12);

	val = readl(M31_PHY_BASE + 0xc);
	val |= (0xe0 << 3);
	writel(val, M31_PHY_BASE + 0xc);
	udelay(12);

	/*step 7: pipe clk setting*/
	writel(0x103, PIPE_CLK_REGS);
	udelay(12);

	val = readl(PIPE_CLK_GATE_REGS);
	val |= (1 << 13);
	writel(val, PIPE_CLK_GATE_REGS);
	udelay(12);

	/*step 8: reset u3drdx0 & M31phy*/
	writel((1 << USB30_DRD_BIT) | (1 << USB30_PCIEPHY_BIT), USB_RESET_REGISTER);
	udelay(12);

	return 0;
}

void usb_device_mode_init(int phy_num)
{
	set_usb3_phy_config();
}

void set_usb_power_off(void)
{
	unsigned int val;

	val = readl(USB_RESET_LEVEL_REGISTER);
	val &= ~(0xf << 7);
	writel(val, USB_RESET_LEVEL_REGISTER);
}
