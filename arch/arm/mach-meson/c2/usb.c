// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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

#define U2_CTRL_SLEEP_SHIFT		24
#define U2_HHI_MEM_PD_MASK		0x3
#define U2_HHI_MEM_PD_SHIFT		0x0
#define U2_CTRL_ISO_SHIFT		24
#define PHY20_RESET_LEVEL_BIT	4
#define	USB_RESET_BIT			3

#define USB2_PHY_PLL_OFFSET_40	(0x0816a010)
#define USB2_PHY_PLL_OFFSET_44	(0x000a72f2)

#define USB2_PHY_PLL_OFFSET_34	(0x70000)

#define USB2_PHY_PLL_OFFSET_50	(0xfe18)
#define USB2_PHY_PLL_OFFSET_54	(0x2a)

#define TUNING_DISCONNECT_THRESHOLD 0x34

#define P_AO_RTI_GEN_PWR_SLEEP0 0xfe013008
#define P_AO_RTI_GEN_PWR_ISO0   0xfe013004
#define P_HHI_MEM_PD_REG0       0xfe013050

struct phy usb_phys[2];

int get_usbphy_baseinfo(struct phy *usb_phys)
{
	struct udevice *bus;
	struct uclass *uc;
	int ret, i;
	int count;

	if (usb_phys[0].dev && usb_phys[1].dev)
		return 0;

	ret = uclass_get(UCLASS_USB, &uc);
	if (ret)
		return ret;
	uclass_foreach_dev(bus, uc) {
		debug("bus->name=%s, bus->driver->name =%s\n",
			bus->name, bus->driver->name);
		count = dev_count_phandle_with_args(bus, "phys", "#phy-cells");
		debug("usb phy count=%u\n", count);
		if (count <= 0)
			return count;
		for (i = 0; i < count; i++) {
			ret = generic_phy_get_by_index(bus, i, &usb_phys[i]);
			if (ret && ret != -ENOENT) {
				pr_err("Failed to get USB PHY%d for %s\n",
				       i, bus->name);
				return ret;
			}
			ret = generic_phy_getinfo(&usb_phys[i]);
			if (ret)
				return ret;
		}
	}
	return 0;
}

static void usb_set_power_domain (void)
{
	writel((readl(P_AO_RTI_GEN_PWR_SLEEP0) & (~(0x1 << U2_CTRL_SLEEP_SHIFT))),
		P_AO_RTI_GEN_PWR_SLEEP0);
	writel((readl(P_AO_RTI_GEN_PWR_ISO0) & (~(0x1 << U2_CTRL_ISO_SHIFT))),
		P_AO_RTI_GEN_PWR_ISO0);
	writel((readl(P_HHI_MEM_PD_REG0)
		& (~(U2_HHI_MEM_PD_MASK << U2_HHI_MEM_PD_SHIFT))), P_HHI_MEM_PD_REG0);

	return;
}

static void usb_set_calibration_trim(uint32_t phy2_pll_base, uint8_t mode)
{
	uint32_t cali, value,i, tmp;
	uint8_t cali_en;

	if (mode == DEVICE_MODE) {
		value = readl((unsigned long)(phy2_pll_base + 0x10));
		value |= 0xfff;
		writel(value, (unsigned long)(phy2_pll_base + 0x10));
		return;
	}

	cali = readl((unsigned long)SYSCTRL_SEC_STATUS_REG12);
	cali_en = (cali >> 30) & 0x1;
	tmp = cali >> 26;

	if (cali_en) {
		tmp =tmp & 0xf;
		cali = tmp;
		if (cali > 12)
			cali = 12;

		value = readl((unsigned long)(phy2_pll_base + 0x10));
		value &= (~0xfff);
		for (i = 0; i < cali; i++)
			value |= (1 << i);

		writel(value, (unsigned long)(phy2_pll_base + 0x10));
	}

}

static void usb_set_clock_freq(unsigned int clock_addr)
{
	unsigned int val = (1 << 8) | (2 << 9) | (9 << 0);
	writel(val, (unsigned long)clock_addr);
	return;
}

void usb_reset(unsigned int reset_addr, int bit){
	*(volatile unsigned int *)(unsigned long)reset_addr = (1 << bit);
}

static void usb_enable_phy_pll (void)
{
	*(volatile uint32_t *)RESETCTRL_RESET1_LEVEL |= (1 << PHY20_RESET_LEVEL_BIT);
}

static void usb_disable_phy_pll (void)
{
	*(volatile uint32_t *)RESETCTRL_RESET1_LEVEL &= ~(1 << PHY20_RESET_LEVEL_BIT);
}

void set_usb_pll(uint32_t phy2_pll_base)
{
	uint32_t tmp, retry = 5;

__retry:
	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x40))=
		(USB2_PHY_PLL_OFFSET_40 | USB_PHY2_RESET);
	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x44)) =
		USB2_PHY_PLL_OFFSET_44;
	udelay(5);

	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x40))=
		(USB2_PHY_PLL_OFFSET_40 | USB_PHY2_RESET | USB_PHY2_ENABLE);
	udelay(5);

	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x40))=
		(USB2_PHY_PLL_OFFSET_40 | USB_PHY2_ENABLE);
	udelay(10);

	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x44))=
		(USB2_PHY_PLL_OFFSET_44 | USBPLL_LK_OD_EN);

	udelay(200);

	tmp = *(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x40);

	if (tmp >> USBPLL_LOCKFLAG_BIT) {
		goto __setphyparameter;
	} else {
		retry --;
		if (!retry)
			goto __setphyparameter;
		goto __retry;
	}

__setphyparameter:
	(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x50))
		= USB2_PHY_PLL_OFFSET_50;
	(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x54))
		= USB2_PHY_PLL_OFFSET_54;
	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0xc)) =
		TUNING_DISCONNECT_THRESHOLD;
	(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x34))
		= USB2_PHY_PLL_OFFSET_34;

	debug("tuning_disconnect_threshold=0x%x\n", TUNING_DISCONNECT_THRESHOLD);
}

int usb_save_phy_dev (unsigned int number, struct phy *phy)
{
	usb_phys[number].dev = phy->dev;
	usb_phys[number].id = phy->id;
	return 0;
}

int usb2_phy_init (struct phy *phy) {
	struct phy_aml_usb2_priv *priv = dev_get_priv(phy->dev);
	struct u2p_aml_regs *u2p_aml_reg;
	u2p_r0_t dev_u2p_r0;
	u2p_r1_t dev_u2p_r1;
	int i,cnt;

	usb_save_phy_dev(0, phy);
	usb_enable_phy_pll();
	usb_set_power_domain();

	priv->clktree_usb_bus_ctrl_addr = dev_read_addr_index(phy->dev, 2 + priv->u2_port_num);
	if (priv->clktree_usb_bus_ctrl_addr == FDT_ADDR_T_NONE) {
		pr_err("Coun't get clktree_usb_bus_ctrl addr index %d\n", 2 + priv->u2_port_num);
	} else {
		usb_set_clock_freq(priv->clktree_usb_bus_ctrl_addr);
	}

	usb_reset(priv->reset_addr, USB_RESET_BIT);

	udelay(500);
	priv->usbphy_reset_bit[0] = PHY20_RESET_LEVEL_BIT;

	for (i = 0; i < priv->u2_port_num; i++) {
		u2p_aml_reg = (struct u2p_aml_regs *)((ulong)(priv->base_addr + i * PHY_REGISTER_SIZE));
		dev_u2p_r0.d32 = u2p_aml_reg->u2p_r0;
		dev_u2p_r0.b.host_device= 1;
		dev_u2p_r0.b.POR= 0;
		u2p_aml_reg->u2p_r0  = dev_u2p_r0.d32;
		udelay(10);
		/***USB PHY RESET : reset1 and reset1_level both need set ***/
		/***reset1:  usb_reset ***/
		/***reset1_level:  usb_enable_phy_pll ***/
		usb_reset(priv->reset_addr, priv->usbphy_reset_bit[i]);

		usb_set_calibration_trim(priv->usb_phy2_pll_base_addr[i], HOST_MODE);

		udelay(50);

		/* wait for phy ready */
		dev_u2p_r1.d32  = u2p_aml_reg->u2p_r1;
		cnt = 0;
		while (dev_u2p_r1.b.phy_rdy != 1) {
			dev_u2p_r1.d32 = u2p_aml_reg->u2p_r1;
			/*we wait phy ready max 1ms, common is 100us*/
			if (cnt > 200)
				break;
			else {
				cnt++;
				udelay(5);
			}
		}
	}

	for (i = 0; i < priv->u2_port_num; i++) {
		debug("------set usb pll\n");
		set_usb_pll(priv->usb_phy2_pll_base_addr[i]);
		writel(0xfe18, (unsigned long)(priv->usb_phy2_pll_base_addr[i] + 0x50));
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

	if (!usb_phys[0].dev) {
		ret = get_usbphy_baseinfo(usb_phys);
		if (ret) {
			printf("get usb dts failed\n");
			return 0;
		}
	}
	usb2_priv = dev_get_priv(usb_phys[0].dev);
	return usb2_priv->dwc2_a_addr;
}

unsigned int usb_get_device_mode_phy_base(void)
{
	struct phy_aml_usb2_priv *usb2_priv;
	int ret;

	if (!usb_phys[0].dev) {
		ret = get_usbphy_baseinfo(usb_phys);
		if (ret) {
			printf("get usb dts failed\n");
			return 0;
		}
	}
	usb2_priv = dev_get_priv(usb_phys[0].dev);
	return usb2_priv->usb_phy2_pll_base_addr[0];
}

void usb_phy_tuning_reset(void)
{
	return;
}

void usb_device_mode_init(void){
	u2p_r0_t dev_u2p_r0;
	u2p_r1_t dev_u2p_r1;

	usb_r0_t dev_usb_r0;
	usb_r4_t dev_usb_r4;
	int cnt, ret;
	u2p_aml_regs_t * u2p_aml_regs;
	usb_aml_regs_t *usb_aml_regs;
	struct phy_aml_usb2_priv *usb2_priv;
	struct phy_aml_usb3_priv *usb3_priv;
	unsigned int phy_base_addr, reset_addr;

	usb_enable_phy_pll();
	ret = get_usbphy_baseinfo(usb_phys);
	if (ret) {
		printf("get usb dts failed\n");
		return;
	}
	usb2_priv = dev_get_priv(usb_phys[0].dev);
	usb3_priv = dev_get_priv(usb_phys[1].dev);
	if (!usb2_priv || !usb3_priv) {
		printf("get usb phy address from dts failed\n");
		return;
	}

	u2p_aml_regs = (u2p_aml_regs_t * )((ulong)usb2_priv->base_addr);
	usb_aml_regs = (usb_aml_regs_t * )((ulong)usb3_priv->base_addr);
	phy_base_addr = usb2_priv->usb_phy2_pll_base_addr[0];
	reset_addr = usb2_priv->reset_addr;

	usb_set_power_domain();
	usb_set_clock_freq(CLKTREE_USB_BUSCLK_CTRL);

	printf("PHY2=0x%08x\n", usb2_priv->base_addr);
	if ((*(volatile uint32_t *)((ulong)(phy_base_addr + 0x38))) != 0) {
		usb_phy_tuning_reset();
		mdelay(150);
	}

	//step 1: usb controller reset
	usb_reset(reset_addr, USB_RESET_BIT);

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
	dev_u2p_r0.b.host_device= 0;
	dev_u2p_r0.b.POR= 0;
	u2p_aml_regs->u2p_r0  = dev_u2p_r0.d32;

	udelay(10);
	//step 6: phy21 reset
	usb_reset(reset_addr, PHY20_RESET_LEVEL_BIT);
	udelay(50);
	usb_set_calibration_trim(phy_base_addr, DEVICE_MODE);
	udelay(50);

	// step 6: wait for phy ready
	dev_u2p_r1.d32	= u2p_aml_regs->u2p_r1;
	cnt = 0;
	while ((dev_u2p_r1.d32 & 0x00000001) != 1) {
		dev_u2p_r1.d32 = u2p_aml_regs->u2p_r1;
		if (cnt > 200)
			break;
		else {
			cnt++;
			udelay(5);
		}
	}

	set_usb_pll(phy_base_addr);
	writel(0xbe18, (unsigned long)(phy_base_addr + 0x50));
	//--------------------------------------------------

	// ------------- usb phy21 initial end ----------

	//--------------------------------------------------

}

static void usb_disable_phy(uint32_t phy2_pll_base)
{
	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x40))=
		((USB2_PHY_PLL_OFFSET_40 | USB_PHY2_RESET) & (~USB_PHY2_ENABLE));
	udelay(5);
}

static void usb_print_usb_baseinfo
	(struct phy_aml_usb2_priv *usb2_priv, struct phy_aml_usb3_priv *usb3_priv)
{
	if (usb3_priv) {
		printf("priv->usb3 port num = %d, config addr=0x%08x\n",
			usb3_priv->usb3_port_num, usb3_priv->base_addr);
	}
	if (usb2_priv) {
		printf("usb2 phy: config addr = 0x%08x, reset addr=0x%08x\n",
			usb2_priv->base_addr, usb2_priv->reset_addr);

		printf("usb2 phy: portnum=%d, phy-addr1= 0x%08x, phy-addr2= 0x%08x\n",
			usb2_priv->u2_port_num, usb2_priv->usb_phy2_pll_base_addr[0],
			usb2_priv->usb_phy2_pll_base_addr[1]);
		printf("dwc2_a base addr: 0x%08x\n", usb2_priv->dwc2_a_addr);
	}
}

int usb_aml_detect_operation(int argc, char * const argv[])
{
	struct phy_aml_usb2_priv *usb2_priv;
	struct phy_aml_usb3_priv *usb3_priv;
	int ret;

	ret = get_usbphy_baseinfo(usb_phys);
	if (ret) {
		printf("get usb dts failed\n");
		return 0;
	}
	usb2_priv = dev_get_priv(usb_phys[0].dev);
	usb3_priv = dev_get_priv(usb_phys[1].dev);

	if (argc >= 2) {
		if (strncmp(argv[1], "disable", 7) == 0) {
			usb_disable_phy_pll();
			usb_disable_phy(usb2_priv->usb_phy2_pll_base_addr[0]);
			printf("disable USB phy\n");
			return 0;
		}

		if (strncmp(argv[1], "info", 4) == 0) {
			usb_print_usb_baseinfo(usb2_priv, usb3_priv);
			return 0;
		}
	}
	return CMD_RET_USAGE;
}
