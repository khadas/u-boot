// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 *
 * DWC3 controller driver
 *
 * Author: Ramneek Mehresh<ramneek.mehresh@freescale.com>
 */

#include <common.h>
#include <dm.h>
#include <fdtdec.h>
#include <generic-phy.h>
#include <usb.h>
#include <dwc3-uboot.h>

#include "xhci.h"
#include <asm/io.h>
#include <linux/usb/dwc3.h>
#include <linux/usb/otg.h>

struct xhci_dwc3_platdata {
	struct phy *usb_phys;
	int num_phys;
};
#ifdef CONFIG_AML_USB
unsigned int usb2portnum;
#endif

void dwc3_set_mode(struct dwc3 *dwc3_reg, u32 mode)
{
	clrsetbits_le32(&dwc3_reg->g_ctl,
			DWC3_GCTL_PRTCAPDIR(DWC3_GCTL_PRTCAP_OTG),
			DWC3_GCTL_PRTCAPDIR(mode));
}

static void dwc3_phy_reset(struct dwc3 *dwc3_reg)
{
	int i;
	u32 reg;

	/* Assert USB3 PHY reset */
	setbits_le32(&dwc3_reg->g_usb3pipectl[0], DWC3_GUSB3PIPECTL_PHYSOFTRST);

	/* Assert USB2 PHY reset */
	setbits_le32(&dwc3_reg->g_usb2phycfg, DWC3_GUSB2PHYCFG_PHYSOFTRST);
#ifdef CONFIG_AML_USB
	for (i=1; i <= usb2portnum; i++) {
		setbits_le32(&dwc3_reg->g_usb2phycfg[i], DWC3_GUSB2PHYCFG_PHYSOFTRST);
	}
#endif

	mdelay(100);

	/* Clear USB3 PHY reset */
	clrbits_le32(&dwc3_reg->g_usb3pipectl[0], DWC3_GUSB3PIPECTL_PHYSOFTRST);

#ifndef CONFIG_AML_USB
		/* Clear USB2 PHY reset */
	clrbits_le32(&dwc3_reg->g_usb2phycfg, DWC3_GUSB2PHYCFG_PHYSOFTRST);
#else
	for (i=0; i <= usb2portnum; i++) {
		/* Clear USB2 PHY reset, DWC3_GUSB2PHYCFG_PHYIF must clear, */
		/* otherwise the sof would occur many errors */
		reg = readl(&dwc3_reg->g_usb2phycfg[i]);
		reg &= ~DWC3_GUSB2PHYCFG_PHYSOFTRST;
		reg &= ~DWC3_GUSB2PHYCFG_SUSPHY;
		reg &= ~DWC3_GUSB2PHYCFG_PHYIF;
		writel(reg, &dwc3_reg->g_usb2phycfg[i]);
	}
#endif
}

void dwc3_core_soft_reset(struct dwc3 *dwc3_reg)
{
	/* Before Resetting PHY, put Core in Reset */
	setbits_le32(&dwc3_reg->g_ctl, DWC3_GCTL_CORESOFTRESET);

	/* reset USB3 phy - if required */
	dwc3_phy_reset(dwc3_reg);

	mdelay(100);

	/* After PHYs are stable we can take Core out of reset state */
	clrbits_le32(&dwc3_reg->g_ctl, DWC3_GCTL_CORESOFTRESET);
}

int dwc3_core_init(struct dwc3 *dwc3_reg)
{
	u32 reg;
	u32 revision;
	unsigned int dwc3_hwparams1;

	revision = readl(&dwc3_reg->g_snpsid);
	/* This should read as U3 followed by revision number */
	if ((revision & DWC3_GSNPSID_MASK) != 0x55330000) {
		puts("this is not a DesignWare USB3 DRD Core\n");
		return -1;
	}

	dwc3_core_soft_reset(dwc3_reg);

	dwc3_hwparams1 = readl(&dwc3_reg->g_hwparams1);

	reg = readl(&dwc3_reg->g_ctl);
	reg &= ~DWC3_GCTL_SCALEDOWN_MASK;
	reg &= ~DWC3_GCTL_DISSCRAMBLE;
	switch (DWC3_GHWPARAMS1_EN_PWROPT(dwc3_hwparams1)) {
	case DWC3_GHWPARAMS1_EN_PWROPT_CLK:
		reg &= ~DWC3_GCTL_DSBLCLKGTNG;
		break;
	default:
		debug("No power optimization available\n");
	}

	/*
	 * WORKAROUND: DWC3 revisions <1.90a have a bug
	 * where the device can fail to connect at SuperSpeed
	 * and falls back to high-speed mode which causes
	 * the device to enter a Connect/Disconnect loop
	 */
	if ((revision & DWC3_REVISION_MASK) < 0x190a)
		reg |= DWC3_GCTL_U2RSTECN;

	writel(reg, &dwc3_reg->g_ctl);

	return 0;
}

void dwc3_set_fladj(struct dwc3 *dwc3_reg, u32 val)
{
	setbits_le32(&dwc3_reg->g_fladj, GFLADJ_30MHZ_REG_SEL |
			GFLADJ_30MHZ(val));
}

#if CONFIG_IS_ENABLED(DM_USB)

void xhci_dwc3_phy_tuning_1(struct udevice *dev, int port)
{
    int ret, i;
    struct xhci_dwc3_platdata *plat;
    struct udevice *udev = dev;

	plat = dev_get_platdata(udev);

    for (i = 0; i < plat->num_phys; i++) {
		ret = generic_phy_tuning(&plat->usb_phys[i], port);
		if (ret) {
			pr_err("Can't tuning USB PHY%d for %s\n",
			       i, dev->name);
			return ;
		}
	}
}

static int xhci_dwc3_setup_phy(struct udevice *dev)
{
	struct xhci_dwc3_platdata *plat = dev_get_platdata(dev);
	int i, ret, count;

	/* Return if no phy declared */
	if (!dev_read_prop(dev, "phys", NULL))
		return 0;

	count = dev_count_phandle_with_args(dev, "phys", "#phy-cells");
	if (count <= 0)
		return count;

	plat->usb_phys = devm_kcalloc(dev, count, sizeof(struct phy),
				      GFP_KERNEL);
	if (!plat->usb_phys)
		return -ENOMEM;

#ifdef CONFIG_AML_USB
	plat->num_phys = 0;
#endif

	for (i = 0; i < count; i++) {
		ret = generic_phy_get_by_index(dev, i, &plat->usb_phys[i]);
		if (ret && ret != -ENOENT) {
			pr_err("Failed to get USB PHY%d for %s\n",
			       i, dev->name);
			return ret;
		}

		++plat->num_phys;
	}

	for (i = 0; i < plat->num_phys; i++) {
		ret = generic_phy_init(&plat->usb_phys[i]);
		if (ret) {
			pr_err("Can't init USB PHY%d for %s\n",
			       i, dev->name);
			goto phys_init_err;
		}
	}

	for (i = 0; i < plat->num_phys; i++) {
		ret = generic_phy_power_on(&plat->usb_phys[i]);
		if (ret) {
			pr_err("Can't power USB PHY%d for %s\n",
			       i, dev->name);
			goto phys_poweron_err;
		}
	}

#ifdef CONFIG_AML_USB
	unsigned int usb_type = 0;

	for (i = 0; i < plat->num_phys; i++) {
		dev_read_u32((&plat->usb_phys[i])->dev, "phy-version", &usb_type);
		if (usb_type == 2) {
			dev_read_u32((&plat->usb_phys[i])->dev, "portnum", &usb2portnum);
		}
	}
#endif

	return 0;

phys_poweron_err:
	for (; i >= 0; i--)
		generic_phy_power_off(&plat->usb_phys[i]);

	for (i = 0; i < plat->num_phys; i++)
		generic_phy_exit(&plat->usb_phys[i]);

	return ret;

phys_init_err:
	for (; i >= 0; i--)
		generic_phy_exit(&plat->usb_phys[i]);

	return ret;
}

static int xhci_dwc3_probe(struct udevice *dev)
{
	struct xhci_hcor *hcor;
	struct xhci_hccr *hccr;
	struct dwc3 *dwc3_reg;
	enum usb_dr_mode dr_mode;
#ifndef CONFIG_AML_USB
	struct xhci_dwc3_platdata *plat = dev_get_platdata(dev);
#endif
	int ret;

#ifdef CONFIG_AML_USB
	ret = xhci_dwc3_setup_phy(dev);
	if (ret)
		return ret;

	hccr = (struct xhci_hccr *)((uintptr_t)dev_read_addr(dev));
	hcor = (struct xhci_hcor *)((uintptr_t)hccr +
			HC_LENGTH(xhci_readl(&(hccr)->cr_capbase)));
#else
	hccr = (struct xhci_hccr *)((uintptr_t)dev_read_addr(dev));
	hcor = (struct xhci_hcor *)((uintptr_t)hccr +
			HC_LENGTH(xhci_readl(&(hccr)->cr_capbase)));

	ret = dwc3_setup_phy(dev, &plat->usb_phys, &plat->num_phys);
		if (ret && (ret != -ENOTSUPP))
			return ret;
#endif

	dwc3_reg = (struct dwc3 *)((char *)(hccr) + DWC3_REG_OFFSET);

	dwc3_core_init(dwc3_reg);

	dr_mode = usb_get_dr_mode(dev_of_offset(dev));
	if (dr_mode == USB_DR_MODE_UNKNOWN)
		/* by default set dual role mode to HOST */
		dr_mode = USB_DR_MODE_HOST;

	dwc3_set_mode(dwc3_reg, dr_mode);

	return xhci_register(dev, hccr, hcor);
}

static int xhci_dwc3_remove(struct udevice *dev)
{
	struct xhci_dwc3_platdata *plat = dev_get_platdata(dev);

	dwc3_shutdown_phy(dev, plat->usb_phys, plat->num_phys);

	return xhci_deregister(dev);
}

static const struct udevice_id xhci_dwc3_ids[] = {
	{ .compatible = "snps,dwc3" },
	{ }
};

U_BOOT_DRIVER(xhci_dwc3) = {
	.name = "xhci-dwc3",
	.id = UCLASS_USB,
	.of_match = xhci_dwc3_ids,
	.probe = xhci_dwc3_probe,
	.remove = xhci_dwc3_remove,
	.ops = &xhci_usb_ops,
	.priv_auto_alloc_size = sizeof(struct xhci_ctrl),
	.platdata_auto_alloc_size = sizeof(struct xhci_dwc3_platdata),
	.flags = DM_FLAG_ALLOC_PRIV_DMA,
};
#endif
