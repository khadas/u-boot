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

#define PHY20_RESET_LEVEL_BIT	8
#define	PHY21_RESET_LEVEL_BIT	9
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
#define DISCONNECT_THRESHOLD_ENHANCE 0x2

#define PHY_21_BASE 0xfe03e000
#define PHY_20_BASE 0xfe03c000
#define PHY_COMP_BASE 0xfe03a000
#define RESET_BASE 0xFE002000

#define AMLOGIC_CTR_COUNT		(0x2)

static int Rev_flag = 0;

struct ctr_info {
	struct phy usb_phys[4];
	unsigned long phy_count;
};

static struct ctr_info ctr[AMLOGIC_CTR_COUNT];
//static struct phy usb_phys[4];

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
	} else if (cpu_id.family_id == MESON_CPU_MAJOR_ID_SC2) {
		Rev_flag = MESON_CPU_MAJOR_ID_SC2;
	} else if (cpu_id.family_id == MESON_CPU_MAJOR_ID_T7) {
		Rev_flag = MESON_CPU_MAJOR_ID_T7;
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
		case MESON_CPU_MAJOR_ID_SC2:
		case MESON_CPU_MAJOR_ID_T7:
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

int get_usbphy_baseinfo(void)
{
	struct udevice *bus;
	struct uclass *uc;
	int ret, i, j=0;
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
		printf("controller index-%d: information:\n", i);
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
	phy_aml_usb2_check_rev();
	printf("PHY version is 0x%02x\n", Rev_flag);
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

static void usb_enable_phy_pll (u32 base_addr)
{
	if (base_addr == PHY_20_BASE) {
		*(volatile uint32_t *)(unsigned long)
			RESETCTRL_RESET0_LEVEL |= (1 << PHY20_RESET_LEVEL_BIT);
	} else if (base_addr == PHY_21_BASE) {
		*(volatile uint32_t *)(unsigned long)
			RESETCTRL_RESET0_LEVEL |= (1 << PHY21_RESET_LEVEL_BIT);
	}
}

void set_usb_pll(uint32_t phy2_pll_base)
{
	int hardware_rev, val;

	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x40))
		= (USB2_PHY_PLL_OFFSET_40 | USB_PHY2_RESET | USB_PHY2_ENABLE);
	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x44)) =
		USB2_PHY_PLL_OFFSET_44;
	hardware_rev = phy_aml_usb2_get_rev_type();
	(*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x48)) =
		USB2_PHY_PLL_OFFSET_48;
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
		val = (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x38));
		val &= ~0xc000000;
		val |= (DISCONNECT_THRESHOLD_ENHANCE << 26 & 0xc000000);
		(*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x38)) = val;
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
	u32 val;
#define USB_PHY30_BASE 0xFE062000

	val = readl(USB_PHY30_BASE);
	val &= (~(3 << 5));
	val |= 0x1;
	writel(val, USB_PHY30_BASE);
	udelay(12);

	val = readl(USB_PHY30_BASE + 0x18);
	val &= (~(0x3 << 17));
	val |= (0x1 << 17);
	writel(val, USB_PHY30_BASE + 0x18);
	udelay(12);
}

int usb2_phy_init (struct phy *phy) {
	struct phy_aml_usb2_priv *priv = dev_get_priv(phy->dev);
	struct u2p_aml_regs *u2p_aml_reg;
	u2p_r0_t dev_u2p_r0;
	u2p_r1_t dev_u2p_r1;
	int i,cnt;

	power_down_usb3();
	usb_save_phy_dev(0, phy);
	usb_enable_phy_pll(priv->base_addr);
	//usb_set_power_domain();
	phy_aml_usb2_check_rev();

	if (priv->usb_phy2_pll_base_addr[0] == PHY_20_BASE) {
		*(volatile unsigned int *)(unsigned long)priv->reset_addr = (1 << 6);

		udelay(500);
		priv->usbphy_reset_bit[0] = PHY20_RESET_LEVEL_BIT;
	} else if (priv->usb_phy2_pll_base_addr[0] == PHY_21_BASE) {
		*(volatile unsigned int *)(unsigned long)priv->reset_addr = (1 << 5);

		udelay(500);
		priv->usbphy_reset_bit[0] = PHY21_RESET_LEVEL_BIT;
	}

	for (i = 0; i < priv->u2_port_num; i++) {
		u2p_aml_reg = (struct u2p_aml_regs *)((ulong)(priv->base_addr + i * PHY_REGISTER_SIZE));
		dev_u2p_r0.d32 = u2p_aml_reg->u2p_r0;
		dev_u2p_r0.b.host_device= 1;
		dev_u2p_r0.b.POR= 0;
		u2p_aml_reg->u2p_r0  = dev_u2p_r0.d32;
		udelay(10);
		*(volatile unsigned int *)(unsigned long)priv->reset_addr = (1 << priv->usbphy_reset_bit[i]);
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
	return 0;
}

/**************************************************************/
/*           device mode config                               */
/**************************************************************/
void usb_device_mode_init(int phy_num) {
	u2p_r0_t dev_u2p_r0;
	u2p_r1_t dev_u2p_r1;

	usb_r0_t dev_usb_r0;
	usb_r4_t dev_usb_r4;
	int cnt;
	u2p_aml_regs_t * u2p_aml_regs;
	usb_aml_regs_t *usb_aml_regs;
	unsigned int phy_base_addr, reset_addr;
	int val;

	phy_num = (readl(SYSCTRL_POC) >> 1) & 0x1U;
	phy_aml_usb2_check_rev();
	if (phy_num == 1) {
		u2p_aml_regs = (u2p_aml_regs_t * )((unsigned long)(PHY_COMP_BASE + PHY_REGISTER_SIZE));
		usb_aml_regs = (usb_aml_regs_t * )((ulong)(PHY_COMP_BASE + 0x80));
		phy_base_addr = PHY_21_BASE;
		reset_addr = RESET_BASE;
	} else {
		u2p_aml_regs = (u2p_aml_regs_t * )((unsigned long)(PHY_COMP_BASE));
		usb_aml_regs = (usb_aml_regs_t * )((ulong)(PHY_COMP_BASE + 0x80));
		phy_base_addr = PHY_20_BASE;
		reset_addr = RESET_BASE;
	}

	printf("PHY2=%p,phy-base=0x%08x\n", u2p_aml_regs, phy_base_addr);
	//if ((*(volatile uint32_t *)(unsigned long)(phy_base_addr + 0x38)) != 0) {
		//usb_phy_tuning_reset(phy_num);
		//mdelay(150);
	//}

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
	if (phy_num == 1) {
		usb_reset(reset_addr, PHY21_RESET_LEVEL_BIT);
	} else {
		usb_reset(reset_addr, PHY20_RESET_LEVEL_BIT);
	}
	udelay(50);

	if (phy_num == 0) {
		val = readl((PHY_COMP_BASE + 0x90));
		val = (val | (1 << 4));
		writel(val, (PHY_COMP_BASE + 0x90));
	}

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

	//set_usb_phy21_pll();
	set_usb_pll(phy_base_addr);
	//--------------------------------------------------

	// ------------- usb phy21 initial end ----------

	//--------------------------------------------------
}
