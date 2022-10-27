// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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
#include <asm/arch/usb.h>

struct xhci_crg_platdata {
	struct phy *usb_phys;
	int num_phys;
#ifdef CONFIG_AML_USB
	unsigned int usbportnum;
#endif
};

unsigned int m31_phys;

void crg_set_mode(struct xhci_hccr *hccr, u32 mode)
{
	u64 tmp;

	if (mode == USB_DR_MODE_HOST) {
		/* set controller host role*/
		tmp = readl((u64)hccr + 0x20FC) & ~0x1;
		writel(tmp, (u64)hccr + 0x20FC);
	}
}


#if CONFIG_IS_ENABLED(DM_USB)

void xhci_crg_phy_tuning_1(struct udevice *dev, int port)
{
	/* nothing */
}

static int xhci_crg_setup_phy(struct udevice *dev)
{
	struct xhci_crg_platdata *plat = dev_get_platdata(dev);
	int i, ret, count;
#ifdef CONFIG_AML_USB
	unsigned int usb_type = 0;
#endif

	/* Return if no phy declared */
	if (!dev_read_prop(dev, "phys", NULL)) {
		dev_read_u32(dev, "portnum", &plat->usbportnum);
		return 0;
	}

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
	for (i = 0; i < plat->num_phys; i++) {
		dev_read_u32((&plat->usb_phys[i])->dev, "phy-version", &usb_type);
		if (usb_type == 2) {
			dev_read_u32((&plat->usb_phys[i])->dev, "portnum", &plat->usbportnum);
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

static int xhci_crg_probe(struct udevice *dev)
{
	struct xhci_hcor *hcor;
	struct xhci_hccr *hccr;
	enum usb_dr_mode dr_mode;
	struct xhci_crg_platdata *plat = dev_get_platdata(dev);
	int ret;

#ifdef CONFIG_AML_USB
	if (dev_read_prop(dev, "m31", NULL)) {
		printf("M31 PHY\n");
#ifdef AML_USB_M31_PHY_ONLY
		m31_phy_init(m31_phys);
#endif
		m31_phys++;
	}

	ret = xhci_crg_setup_phy(dev);
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

	dr_mode = usb_get_dr_mode(dev_of_offset(dev));
	if (dr_mode == USB_DR_MODE_UNKNOWN)
		/* by default set dual role mode to HOST */
		dr_mode = USB_DR_MODE_HOST;

	crg_set_mode(hccr, dr_mode);

#ifndef CONFIG_AML_USB
	return xhci_register(dev, hccr, hcor);
#else
	return xhci_register(dev, hccr, hcor, plat->usbportnum);
#endif
}

static int xhci_crg_remove(struct udevice *dev)
{
	struct xhci_crg_platdata *plat = dev_get_platdata(dev);

	dwc3_shutdown_phy(dev, plat->usb_phys, plat->num_phys);

	return xhci_deregister(dev);
}

static const struct udevice_id xhci_crg_ids[] = {
	{ .compatible = "crg-xhci" },
	{ }
};

U_BOOT_DRIVER(xhci_crg) = {
	.name = "xhci-crg",
	.id = UCLASS_USB,
	.of_match = xhci_crg_ids,
	.probe = xhci_crg_probe,
	.remove = xhci_crg_remove,
	.ops = &xhci_usb_ops,
	.priv_auto_alloc_size = sizeof(struct xhci_ctrl),
	.platdata_auto_alloc_size = sizeof(struct xhci_crg_platdata),
	.flags = DM_FLAG_ALLOC_PRIV_DMA,
};
#endif
