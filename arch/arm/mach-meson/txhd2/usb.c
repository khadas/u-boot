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

#define PHY20_RESET_LEVEL_BIT	16
#define PHY21_RESET_LEVEL_BIT	17
#define PHY22_RESET_LEVEL_BIT	18
#define	USB_RESET_BIT			2
#define USB_2_DRD_BIT	19
#define USB2H_BIT	20

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

#define TUNING_DISCONNECT_THRESHOLD 0x3f
#define DISCONNECT_THRESHOLD_ENHANCE 0x2

#define PHY_20_BASE 0xff636000
#define PHY_COMP_BASE 0xffe09000
#define RESET1_BASE ((0x0402  << 2) + 0xffd00000)
#define RESET1_LEVEL_BASE ((0x0421  << 2) + 0xffd00000)

#define PLL_REG32_4		(PHY_20_BASE + 0x10)
#define PLL_REG32_16	(PHY_20_BASE + 0x40)
#define PLL_REG32_17	(PHY_20_BASE + 0x44)
#define PLL_REG32_18	(PHY_20_BASE + 0x48)
#define USBPLL_LK_RESET_BIT	28
#define USBPLL_EN_BIT		11
#define USBPLL_RESET_BIT	18

#define AMLOGIC_CTR_COUNT		(0x2)

void usb_udelay(unsigned int us)
{
	uint32_t t0 = get_time();

	while (get_time() - t0 <= us)
		;
}

struct ctr_info {
	struct phy usb_phys[4];
	unsigned long phy_count;
};

static struct ctr_info ctr[AMLOGIC_CTR_COUNT];

int get_usbphy_baseinfo(void)
{
	struct udevice *bus;
	struct uclass *uc;
	int ret, i, j = 0;
	int count;

	for (i = 0; i < AMLOGIC_CTR_COUNT; i++) {
		if (ctr[i].usb_phys[0].dev && ctr[i].usb_phys[1].dev)
			return 0;
	}

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
		if (j >= AMLOGIC_CTR_COUNT) {
			pr_err("AMLOGIC_CTR_COUNT is small: %d\n", j);
			return -1;
		}
		for (i = 0; i < count; i++) {
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
	int ret, i;

	ret = get_usbphy_baseinfo();
	if (ret) {
		printf("get usb dts failed\n");
		return;
	}
	for (i = 0; i < AMLOGIC_CTR_COUNT; i++) {
		usb2_priv = dev_get_priv(ctr[i].usb_phys[0].dev);
		usb3_priv = dev_get_priv(ctr[i].usb_phys[1].dev);

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
}

static void usb_set_calibration_trim(uint32_t phy2_pll_base)
{
	uint32_t regval, i;
	uint32_t cali = 0x13;
	uint32_t cali_en = 0;
	/*cali<5 bit> : cali_en cali_3 cali_2 cali_1 cali_0*/

	cali = readl(AO_SEC_GP_CFG12);
	cali_en = (cali >> 12) & 0x1;
	cali = cali >> 8;
	if (cali_en) {
		cali = (cali & 0xf);

		if (cali > 12)
			cali = 12;
		regval = readl(PLL_REG32_4);
		regval &= (~0xfff);

		for (i = 0; i < cali; i++)
			regval |= (1 << i);

		writel(regval, PLL_REG32_4);
	} else {
		regval = readl(PLL_REG32_4);
		regval &= (~0xfff);
		regval |= 0x7f;
		writel(regval, PLL_REG32_4);
	}
	return;
}

void usb_reset(unsigned int reset_addr, int bit)
{
	*(unsigned int *)(unsigned long)reset_addr = (1 << bit);
}

static void usb_enable_phy_pll(u32 base_addr)
{
	*(uint32_t *)(unsigned long)
		RESET1_LEVEL_BASE |= (1 << PHY20_RESET_LEVEL_BIT);
	*(uint32_t *)(unsigned long)
		RESET1_LEVEL_BASE |= (1 << PHY21_RESET_LEVEL_BIT);
	*(uint32_t *)(unsigned long)
		RESET1_LEVEL_BASE |= (1 << PHY22_RESET_LEVEL_BIT);
}

void set_usb_pll(uint32_t phy2_pll_base)
{
	uint32_t retry = 5;
	uint32_t pll_val0, pll_val1;

	pll_val0 = 0x040a06f4;
	pll_val1 = 0x00000022;

__retry:
	writel(pll_val0, PLL_REG32_16);
	writel(pll_val1, PLL_REG32_17);
	usb_udelay(5);
	writel(pll_val1 | (1 << USBPLL_RESET_BIT), PLL_REG32_17);
	writel((pll_val0 | (1 << USBPLL_LK_RESET_BIT) | (1 << USBPLL_EN_BIT)), PLL_REG32_16);
	usb_udelay(50);
	writel(pll_val1, PLL_REG32_17);
	usb_udelay(50);
	writel((pll_val0 | (1 << USBPLL_EN_BIT)), PLL_REG32_16);

	// wait for 200us
	usb_udelay(200);
	//check lock bit
	if (readl(PLL_REG32_16) >> 31)
		return;

	retry--;
	if (!retry)
		return;

	goto __retry;
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
	usb_enable_phy_pll(priv->base_addr);

	if (priv->usb_phy2_pll_base_addr[0] == PHY_20_BASE) {
		*(unsigned int *)(unsigned long)priv->reset_addr =
			(1 << USB_2_DRD_BIT) | (1 << USB_RESET_BIT);

		usb_udelay(500);
		priv->usbphy_reset_bit[0] = PHY20_RESET_LEVEL_BIT;
	} else {
		*(unsigned int *)(unsigned long)priv->reset_addr =
			(1 << USB2H_BIT) | (1 << USB_RESET_BIT);

		usb_udelay(500);
		priv->usbphy_reset_bit[0] = PHY21_RESET_LEVEL_BIT;
		priv->usbphy_reset_bit[1] = PHY22_RESET_LEVEL_BIT;
	}

	for (i = 0; i < priv->u2_port_num; i++) {
		u2p_aml_reg = (struct u2p_aml_regs *)
			((ulong)(priv->base_addr + i * PHY_REGISTER_SIZE));
		dev_u2p_r0.d32 = u2p_aml_reg->u2p_r0;
		dev_u2p_r0.b.host_device = 1;
		dev_u2p_r0.b.POR = 0;
		u2p_aml_reg->u2p_r0  = dev_u2p_r0.d32;
		usb_udelay(10);
		*(unsigned int *)(unsigned long)priv->reset_addr = (1 << priv->usbphy_reset_bit[i]);
		usb_udelay(50);

		usb_set_calibration_trim(priv->usb_phy2_pll_base_addr[i]);
		usb_udelay(50);

		/* wait for phy ready */
		dev_u2p_r1.d32  = u2p_aml_reg->u2p_r1;
		cnt = 0;
		while (dev_u2p_r1.b.phy_rdy != 1) {
			dev_u2p_r1.d32 = u2p_aml_reg->u2p_r1;
			/*we wait phy ready max 1ms, common is 100us*/
			if (cnt > 200) {
				break;
			} else {
				cnt++;
				usb_udelay(5);
			}
		}
	}

	for (i = 0; i < priv->u2_port_num; i++)
		set_usb_pll(priv->usb_phy2_pll_base_addr[i]);

	return 0;
}

int usb2_phy_tuning(uint32_t phy2_pll_base, int port)
{
	return 0;
}

/**************************************************************/
/*           device mode config                               */
/**************************************************************/
void usb_device_mode_init(int phy_num)
{
	u2p_r0_t dev_u2p_r0;
	u2p_r1_t dev_u2p_r1;

	usb_r0_t dev_usb_r0;
	usb_r4_t dev_usb_r4;
	int cnt;
	u2p_aml_regs_t *u2p_aml_regs;
	usb_aml_regs_t *usb_aml_regs;
	unsigned int phy_base_addr, reset_addr;

	u2p_aml_regs = (u2p_aml_regs_t *)((unsigned long)(PHY_COMP_BASE));
	usb_aml_regs = (usb_aml_regs_t *)((ulong)(PHY_COMP_BASE + 0x80));
	phy_base_addr = PHY_20_BASE;
	reset_addr = RESET1_BASE;

	printf("PHY2=%p,phy-base=0x%08x\n", u2p_aml_regs, phy_base_addr);
	//if ((*(volatile uint32_t *)(unsigned long)(phy_base_addr + 0x38)) != 0) {
		//usb_phy_tuning_reset(phy_num);
		//mdelay(150);
	//}

	//step 1: usb phy power
	writel((readl(RESET1_LEVEL_BASE) & (~(0x1 << 16))), RESET1_LEVEL_BASE);
	usb_udelay(500);
	writel((readl(RESET1_LEVEL_BASE) | (0x1 << 16)), RESET1_LEVEL_BASE);

	//step 2: usb controller reset
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

	// step 5: config phy20 device mode
	dev_u2p_r0.d32	 = u2p_aml_regs->u2p_r0;
	dev_u2p_r0.b.host_device = 0;
	dev_u2p_r0.b.POR = 0;
	u2p_aml_regs->u2p_r0  = dev_u2p_r0.d32;

	usb_udelay(10);
	//step 6: phy20 reset
	usb_reset(reset_addr, PHY20_RESET_LEVEL_BIT);
	usb_udelay(50);

	//step 7: phy20 trim
	usb_set_calibration_trim(phy_base_addr);
	usb_udelay(50);

	// step 8: wait for phy ready
	dev_u2p_r1.d32	= u2p_aml_regs->u2p_r1;
	cnt = 0;
	while ((dev_u2p_r1.d32 & 0x00000001) != 1) {
		dev_u2p_r1.d32 = u2p_aml_regs->u2p_r1;
		if (cnt > 200) {
			break;
		} else {
			cnt++;
			usb_udelay(5);
		}
	}

	// step 9: set usb pll
	set_usb_pll(phy_base_addr);
	//--------------------------------------------------

	// ------------- usb phy20 initial end ----------

	//--------------------------------------------------
}
