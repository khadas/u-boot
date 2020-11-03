// SPDX-License-Identifier: GPL-2.0+
/*
 * Meson GXL and GXM USB2 PHY driver
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

#define U2_CTRL_SLEEP_SHIFT		17
#define U2_HHI_MEM_PD_MASK		0x3
#define U2_HHI_MEM_PD_SHIFT		30
#define U2_CTRL_ISO_SHIFT		17
#define PHY20_RESET_LEVEL_BIT	16
#define	PHY21_RESET_LEVEL_BIT	17
#define	USB_RESET_BIT			2

#define USB2_PHY_PLL_OFFSET_40	(0x09400414)
#define USB2_PHY_PLL_OFFSET_44	(0x927E0000)
#define USB2_PHY_PLL_OFFSET_48	(0xac5f49e5)
#define USB2_SM1_PHY_PLL_OFFSET_48	(0xac5f69e5)

#define USB2_PHY_PLL_OFFSET_10	(0x80000fff)
#define USB2_PHY_PLL_OFFSET_34	(0x78000)

#define USB2_PHY_PLL_OFFSET_38_CLEAR	(0)
#define USB2_PHY_PLL_OFFSET_38_SET	    (0xe0004)
#define USB2_PHY_PLL_OFFSET_50	(0xfe18)
#define USB2_PHY_PLL_OFFSET_54	(0x2a)

#define TUNING_DISCONNECT_THRESHOLD 0x34

static int Rev_flag = 0;

static struct phy usb_phys[2];

/*Rev_flag == 0XB, g12b and revB, tl1 */
/*Rev_flag == 1, sm1 */
static void phy_aml_usb2_check_rev (void)
{
	cpu_id_t cpu_id = get_cpu_id();

	if (cpu_id.family_id == MESON_CPU_MAJOR_ID_G12B) {
		if (cpu_id.chip_rev == 0xb)
			Rev_flag = 0xb;
		else
			Rev_flag = 0;
	} else if (cpu_id.family_id == MESON_CPU_MAJOR_ID_SM1) {
		Rev_flag = MESON_CPU_MAJOR_ID_SM1;
	} else if (cpu_id.family_id == MESON_CPU_MAJOR_ID_A1) {
		Rev_flag = MESON_CPU_MAJOR_ID_A1;
	} else if (cpu_id.family_id == MESON_CPU_MAJOR_ID_C1) {
		Rev_flag = MESON_CPU_MAJOR_ID_C1;
	}
	return;
}

static int phy_aml_usb2_get_rev_type (void)
{
	int val = 0;

	switch (Rev_flag) {
		case MESON_CPU_MAJOR_ID_SM1:
		case MESON_CPU_MAJOR_ID_A1:
		case MESON_CPU_MAJOR_ID_C1:
		case 0xb:
			val = 1;
			break;
		default:
			printk("amlogic usb phy need tuning\n");
			val = 0;
			break;
	}

	return val;
}

int get_usbphy_baseinfo(struct phy *usb_phys)
{
	struct udevice *bus;
	struct uclass *uc;
	int ret, i;
	unsigned int count;

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
	phy_aml_usb2_check_rev();
	printf("PHY version is 0x%02x\n", Rev_flag);
	return 0;
}

static void usb_set_power_domain (void)
{
	int rev_type;

	rev_type = phy_aml_usb2_get_rev_type();
	if (rev_type ) {
		writel((readl(P_AO_RTI_GEN_PWR_SLEEP0) & (~(0x1 << U2_CTRL_SLEEP_SHIFT))),
			P_AO_RTI_GEN_PWR_SLEEP0);
		writel((readl(P_AO_RTI_GEN_PWR_ISO0) & (~(0x1 << U2_CTRL_ISO_SHIFT))),
			P_AO_RTI_GEN_PWR_ISO0);
		writel((readl(P_HHI_MEM_PD_REG0)
			& (~(U2_HHI_MEM_PD_MASK << U2_HHI_MEM_PD_SHIFT))), P_HHI_MEM_PD_REG0);
	}

	return;
}

static void set_pll_Calibration_default(uint32_t phy2_pll_base)
{
    u32 tmp;

    tmp = (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x8));
    tmp &= 0xfff;
    tmp |= (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x10));
    (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x10))
     = tmp;
}

void usb_reset(unsigned int reset_addr, int bit){
	*(volatile unsigned int *)(unsigned long)reset_addr = (1 << bit);
}

void set_usb_pll(uint32_t phy2_pll_base)
{
	unsigned int pll_offset_48;
	int hardware_rev;

	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x40))
		= (USB2_PHY_PLL_OFFSET_40 | USB_PHY2_RESET | USB_PHY2_ENABLE);
	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x44)) =
		USB2_PHY_PLL_OFFSET_44;
	hardware_rev = phy_aml_usb2_get_rev_type();
	if (hardware_rev)
		pll_offset_48 = USB2_SM1_PHY_PLL_OFFSET_48;
	else
		pll_offset_48 = USB2_PHY_PLL_OFFSET_48;
	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x48)) =
		pll_offset_48;
	udelay(100);
	(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x40))
		= (((USB2_PHY_PLL_OFFSET_40) | (USB_PHY2_ENABLE))
			& (~(USB_PHY2_RESET)));
	if (hardware_rev) {
		(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x50))
			= USB2_PHY_PLL_OFFSET_50;
		(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x54))
			= USB2_PHY_PLL_OFFSET_54;
		set_pll_Calibration_default(phy2_pll_base);
	} else {
		(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x50))
			= USB2_PHY_PLL_OFFSET_50;
		(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x10))
			= USB2_PHY_PLL_OFFSET_10;
		(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x38))
			= USB2_PHY_PLL_OFFSET_38_CLEAR;
	}

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
	phy_aml_usb2_check_rev();
	usb_set_power_domain();

	usb_reset(priv->reset_addr, USB_RESET_BIT);

	udelay(500);
	priv->usbphy_reset_bit[0] = PHY20_RESET_LEVEL_BIT;
	priv->usbphy_reset_bit[1] = PHY21_RESET_LEVEL_BIT;

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
	}
	return 0;

}

int usb2_phy_tuning(uint32_t phy2_pll_base, int port)
{
	unsigned long phy_reg_base;
	unsigned int pll_set38, pll_set34, pll_set3, pll_set4;
	unsigned int rev_type = 0;

	if (port > 2)
		return 0;

	pll_set38 = USB2_PHY_PLL_OFFSET_38_SET;
	pll_set34 = USB2_PHY_PLL_OFFSET_34;

	debug("pll38=0x%08x, pll34=0x%08x,\n",
			pll_set38, pll_set34);


	phy_reg_base = phy2_pll_base;

	rev_type = phy_aml_usb2_get_rev_type();
	if (rev_type) {
		return 0;
	}

	(*(volatile uint32_t *)(phy_reg_base + 0x38)) = pll_set38;
	(*(volatile uint32_t *)(phy_reg_base + 0x34)) = pll_set34;
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
	return usb2_priv->usb_phy2_pll_base_addr[1];
}

void usb_phy_tuning_reset(void)
{
	unsigned long phy_reg_base = usb_get_device_mode_phy_base();
	int rev_type;

	rev_type = phy_aml_usb2_get_rev_type();

	if (rev_type)
		return;
	if (phy_reg_base == 0)
		return;

	(*(volatile uint32_t *)(phy_reg_base + 0x38)) = USB2_PHY_PLL_OFFSET_38_CLEAR;
	(*(volatile uint32_t *)(phy_reg_base + 0x34)) = USB2_PHY_PLL_OFFSET_34;
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

	phy_aml_usb2_check_rev();
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

	u2p_aml_regs = (u2p_aml_regs_t * )((ulong)(usb2_priv->base_addr + PHY_REGISTER_SIZE));
	usb_aml_regs = (usb_aml_regs_t * )((ulong)usb3_priv->base_addr);
	phy_base_addr = usb2_priv->usb_phy2_pll_base_addr[1];
	reset_addr = usb2_priv->reset_addr;

	usb_set_power_domain();

	printf("PHY2=0x%08x,PHY3=0x%08x\n", u2p_aml_regs, usb_aml_regs);
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
	usb_reset(reset_addr, PHY21_RESET_LEVEL_BIT);
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
	//--------------------------------------------------

	// ------------- usb phy21 initinal end ----------

	//--------------------------------------------------

}

