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

#define PHY23_RESET_LEVEL_BIT	16
#define PHY22_RESET_LEVEL_BIT	17
#define	PHY21_RESET_LEVEL_BIT	18
#define	USB_RESET_BIT			4

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
#define M31_PHY_SETTING 0x1E30CEB9

//#define PHY_23_BASE 0xfe030000
//#define PHY_22_BASE 0xfe032000
//#define PHY_21_BASE 0xfe03e000
//#define PHY_COMP_BASE 0xfe03a000
//#define RESET_BASE 0xFE002000
#define M31_PHY_BASE 0XFE02A000

#define AMLOGIC_CTR_COUNT		(0x2)
#define USB_RESET1       (unsigned long *)0xffd01008

#define AMLOGIC_USB_POWER
static int Rev_flag;

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
				printf("phy%d-addr= 0x%08x\n", j,
					usb2_priv->usb_phy2_pll_base_addr[j]);
		}
	}
	printf("PHY version is 0x%02x\n", Rev_flag);
}

static void usb_set_calibration_trim(uint32_t phy2_pll_base)
{
	uint32_t cali, value,i;
	uint8_t cali_en;

	cali = readl(AO_SEC_GP_CFG12);
	//printf("AO_SEC_GP_CFG12=0x%08x\n", cali);
	/*****if cali_en ==0, set 0x10 to the default value: 0x1700****/
	cali_en = (cali >> 12) & 0x1;
	cali = cali >> 8;
	if (cali_en) {
		cali =cali & 0xf;
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
	printf("0x10 trim value=0x%08x\n", value);;
}

static void usb_enable_phy_pll(void)
{
	*(uint32_t *)P_RESET1_LEVEL |= (0x7 << 16);
}

void set_usb_pll(uint32_t phy2_pll_base)
{
	(*(uint32_t *)((unsigned long)phy2_pll_base + 0x40)) =
		(USB_PHY2_PLL_PARAMETER_1 | USB_PHY2_RESET | USB_PHY2_ENABLE);
	(*(uint32_t *)((unsigned long)phy2_pll_base + 0x44)) =
		USB_PHY2_PLL_PARAMETER_2;
	(*(uint32_t *)((unsigned long)phy2_pll_base + 0x48)) =
		USB_PHY2_PLL_PARAMETER_3;
	udelay(100);
	(*(uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x40)) =
		(((USB_PHY2_PLL_PARAMETER_1) | (USB_PHY2_ENABLE))
		& (~(USB_PHY2_RESET)));

	(*(uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x50)) = 0xbe18;
	(*(uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x54)) = 0x2a;
	usb_set_calibration_trim(phy2_pll_base);
	(*(uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0xc)) = 0x34;
	(*(uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x34)) = 0x78000;
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

int usb2_phy_init(struct phy *phy)
{
	struct phy_aml_usb2_priv *priv = dev_get_priv(phy->dev);
	struct u2p_aml_regs *u2p_aml_reg;
	u2p_r0_t dev_u2p_r0;
	u2p_r1_t dev_u2p_r1;
	int i, cnt;

	usb_save_phy_dev(0, phy);
	usb_enable_phy_pll();
	//usb_set_power_domain();

	//*(volatile unsigned int *)(unsigned long)priv->reset_addr = (1 << USB_RESET_BIT);
	*P_RESET1_REGISTER |= (1 << 2);
	udelay(500);
	priv->usbphy_reset_bit[1] = PHY21_RESET_LEVEL_BIT;
	priv->usbphy_reset_bit[2] = PHY22_RESET_LEVEL_BIT;
	priv->usbphy_reset_bit[3] = PHY23_RESET_LEVEL_BIT;

	for (i = 1; i <= priv->u2_port_num; i++) {
		u2p_aml_reg =
			(struct u2p_aml_regs *)((ulong)(priv->base_addr + i * PHY_REGISTER_SIZE));
		dev_u2p_r0.d32 = u2p_aml_reg->u2p_r0;
		dev_u2p_r0.b.host_device = 1;
		dev_u2p_r0.b.POR = 0;
		u2p_aml_reg->u2p_r0  = dev_u2p_r0.d32;
		udelay(10);
		*P_RESET1_REGISTER = (1 << priv->usbphy_reset_bit[i]);
		udelay(50);

		/* wait for phy ready */
		dev_u2p_r1.d32  = u2p_aml_reg->u2p_r1;
		cnt = 0;
		while (dev_u2p_r1.b.phy_rdy != 1) {
			dev_u2p_r1.d32 = u2p_aml_reg->u2p_r1;
			/*we wait phy ready max 1ms, common is 100us*/
			if (cnt > 200)
				break;
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

/**************************************************************/
/*           device mode config                               */
/**************************************************************/
unsigned int usb_get_dwc_a_base_addr(void)
{
	struct phy_aml_usb2_priv *usb2_priv;
	int ret;

	if (!ctr[0].usb_phys[0].dev) {
		ret = get_usbphy_baseinfo();
		if (ret) {
			printf("get usb dts failed\n");
			return 0;
		}
	}
	usb2_priv = dev_get_priv(ctr[0].usb_phys[0].dev);
	return usb2_priv->dwc2_a_addr;
}

unsigned int usb_get_device_mode_phy_base(void)
{
	struct phy_aml_usb2_priv *usb2_priv;
	int ret;

	if (!ctr[0].usb_phys[0].dev) {
		ret = get_usbphy_baseinfo();
		if (ret) {
			printf("get usb dts failed\n");
			return 0;
		}
	}
	usb2_priv = dev_get_priv(ctr[0].usb_phys[0].dev);
	return usb2_priv->usb_phy2_pll_base_addr[1];
}

void usb_phy_tuning_reset(void)
{
}

void usb_device_mode_init(void)
{
	u2p_r0_t dev_u2p_r0;
	u2p_r1_t dev_u2p_r1;

	usb_r0_t dev_usb_r0;
	usb_r4_t dev_usb_r4;
	int cnt, ret;
	u2p_aml_regs_t *u2p_aml_regs;
	usb_aml_regs_t *usb_aml_regs;
	struct phy_aml_usb2_priv *usb2_priv;
	struct phy_aml_usb3_priv *usb3_priv;
	unsigned int phy_base_addr;
	//unsigned int reset_addr;

	usb_enable_phy_pll();
	ret = get_usbphy_baseinfo();
	if (ret) {
		printf("get usb dts failed\n");
		return;
	}
	usb2_priv = dev_get_priv(ctr[0].usb_phys[0].dev);
	usb3_priv = dev_get_priv(ctr[0].usb_phys[1].dev);
	if (!usb2_priv || !usb3_priv) {
		printf("get usb phy address from dts failed\n");
		return;
	}

	u2p_aml_regs =
		(u2p_aml_regs_t *)((unsigned long)(usb2_priv->base_addr + PHY_REGISTER_SIZE));
	usb_aml_regs = (usb_aml_regs_t *)((ulong)usb3_priv->base_addr);
	phy_base_addr = usb2_priv->usb_phy2_pll_base_addr[1];
	//reset_addr = usb2_priv->reset_addr;

	//printf("PHY2=0x%08x,PHY3=0x%08x\n", u2p_aml_regs, usb_aml_regs);
	printf("PHY2=%p,phy-base=0x%08x\n", u2p_aml_regs, phy_base_addr);
	if ((*(uint32_t *)(unsigned long)(phy_base_addr + 0x38)) != 0) {
		usb_phy_tuning_reset();
		mdelay(150);
	}

	//step 1: usb controller reset
	*USB_RESET1 |= (1 << 2);

	// step 3: enable usb INT internal USB
	dev_usb_r0.d32	 = usb_aml_regs->usb_r0;
	dev_usb_r0.b.u2d_ss_scaledown_mode = 0;
	dev_usb_r0.b.u2d_act			   = 1;
	usb_aml_regs->usb_r0 = dev_usb_r0.d32;

	// step 4: disable usb phy sleep
	dev_usb_r4.d32	 = usb_aml_regs->usb_r4;
	dev_usb_r4.b.p21_SLEEPM0   = 1;
	usb_aml_regs->usb_r4   = dev_usb_r4.d32;

	// step 5: config phy21 device mode
	dev_u2p_r0.d32	 = u2p_aml_regs->u2p_r0;
	dev_u2p_r0.b.host_device = 0;
	dev_u2p_r0.b.POR = 0;
	u2p_aml_regs->u2p_r0  = dev_u2p_r0.d32;

	udelay(10);
	//step 6: phy21 reset
	*USB_RESET1 |= (1 << 17);
	udelay(50);

	// step 6: wait for phy ready
	dev_u2p_r1.d32	= u2p_aml_regs->u2p_r1;
	cnt = 0;
	while ((dev_u2p_r1.d32 & 0x00000001) != 1) {
		dev_u2p_r1.d32 = u2p_aml_regs->u2p_r1;
		if (cnt > 200)
			break;
		cnt++;
		udelay(5);
	}

	set_usb_pll(phy_base_addr);
	//--------------------------------------------------

	// ------------- usb phy21 initial end ----------

	//--------------------------------------------------
}

void set_usb_power_off(void)
{
	*(uint32_t *)P_RESET1_LEVEL &= (~(0x7 << 16));
}
