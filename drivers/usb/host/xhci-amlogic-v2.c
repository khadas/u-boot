/*
 *amlogic USB HOST XHCI Controller
 *
 *	Yue Wang <yue.wang@amlogic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * This file is a conglomeration for DWC3-init sequence and further
 * exynos5 specific PHY-init sequence.
 */

#include <common.h>
/*#include <libfdt.h>*/
#include <malloc.h>
#include <usb.h>
//#include <watchdog.h>
#include <asm/arch/cpu.h>
/*#include <asm-generic/errno.h>*/
#include <linux/compat.h>
#include <linux/usb/dwc3.h>
#include <asm/arch/usb-v2.h>
#include "xhci.h"
#include <asm/arch/secure_apb.h>

/* Declare global data pointer */
DECLARE_GLOBAL_DATA_PTR;

/**
 * Contains pointers to register base addresses
 * for the usb controller.
 */
struct amlogic_xhci {
	struct u2p_aml_regs *usb2_phy;
	struct usb_aml_regs *usb3_phy;
	struct xhci_hccr *hcd;
	struct dwc3 *dwc3_reg;
	unsigned int u2_port_num;
	unsigned int u3_port_num;
	uint32_t volatile *phy_2[4];
	/* initialize/shutdown the phy */
	void	(*phy3_init)(struct usb_aml_regs *x, unsigned int u3portnum);
	void	(*phy2_init)(struct u2p_aml_regs *phy, unsigned int u2portnum,uint32_t volatile **phypl);
	void	(*phy_shutdown)(struct usb_aml_regs *phy, struct u2p_aml_regs *phy2);
};

static struct amlogic_xhci amlogic;

#ifdef CONFIG_USB_XHCI_AMLOGIC_USB3_V2
#if 0
void cr_bus_addr (u32 addr)
{
	usb_r2_t usb_r2 = {.d32 = 0};
	usb_r6_t usb_r6 = {.d32 = 0};

	// prepare addr
	usb_r2.b.p30_cr_data_in = addr;
	usb_aml_reg->usb_r2 = usb_r2.d32;

	// cap addr rising edge
	usb_r2.b.p30_cr_cap_addr = 0;
	usb_aml_reg->usb_r2 = usb_r2.d32;
	usb_r2.b.p30_cr_cap_addr = 1;
	usb_aml_reg->usb_r2 = usb_r2.d32;

	// wait ack 1
	do {
		usb_r6.d32 = usb_aml_reg->usb_r6;
	} while (usb_r6.b.p30_cr_ack == 0);

	// clear cap addr
	usb_r2.b.p30_cr_cap_addr = 0;
	usb_aml_reg->usb_r2 = usb_r2.d32;

	// wait ack 0
	do {
		usb_r6.d32 = usb_aml_reg->usb_r6;
	} while (usb_r6.b.p30_cr_ack == 1);
}

int cr_bus_read (u32 addr)
{
	int data;
	usb_r2_t usb_r2 = {.d32 = 0};
	usb_r6_t usb_r6 = {.d32 = 0};

	cr_bus_addr ( addr );

	// read rising edge
	usb_r2.b.p30_cr_read = 0;
	usb_aml_reg->usb_r2 = usb_r2.d32;
	usb_r2.b.p30_cr_read = 1;
	usb_aml_reg->usb_r2 = usb_r2.d32;

	// wait ack 1
	do {
		usb_r6.d32 = usb_aml_reg->usb_r6;
	} while (usb_r6.b.p30_cr_ack == 0);

	// save data
	data = usb_r6.b.p30_cr_data_out;

	// clear read
	usb_r2.b.p30_cr_read = 0;
	usb_aml_reg->usb_r2 = usb_r2.d32;

	// wait ack 0
	do {
		usb_r6.d32 = usb_aml_reg->usb_r6;
	} while (usb_r6.b.p30_cr_ack == 1);

	return data;
}

void cr_bus_write (u32 addr, u32 data)
{
	usb_r2_t usb_r2 = {.d32 = 0};
	usb_r6_t usb_r6 = {.d32 = 0};

	cr_bus_addr ( addr );

	// prepare data
	usb_r2.b.p30_cr_data_in = data;
	usb_aml_reg->usb_r2 = usb_r2.d32;

	// cap data rising edge
	usb_r2.b.p30_cr_cap_data = 0;
	usb_aml_reg->usb_r2 = usb_r2.d32;
	usb_r2.b.p30_cr_cap_data = 1;
	usb_aml_reg->usb_r2 = usb_r2.d32;

	// wait ack 1
	do {
		usb_r6.d32 = usb_aml_reg->usb_r6;
	} while (usb_r6.b.p30_cr_ack == 0);

	// clear cap data
	usb_r2.b.p30_cr_cap_data = 0;
	usb_aml_reg->usb_r2 = usb_r2.d32;

	// wait ack 0
	do {
		usb_r6.d32 = usb_aml_reg->usb_r6;
	} while (usb_r6.b.p30_cr_ack == 1);

	// write rising edge
	usb_r2.b.p30_cr_write = 0;
	usb_aml_reg->usb_r2 = usb_r2.d32;
	usb_r2.b.p30_cr_write = 1;
	usb_aml_reg->usb_r2 = usb_r2.d32;

	// wait ack 1
	do {
		usb_r6.d32 = usb_aml_reg->usb_r6;
	} while (usb_r6.b.p30_cr_ack == 0);

	// clear write
	usb_r2.b.p30_cr_write = 0;
	usb_aml_reg->usb_r2 = usb_r2.d32;

	// wait ack 0
	do {
		usb_r6.d32 = usb_aml_reg->usb_r6;
	} while (usb_r6.b.p30_cr_ack == 1);
}
#endif
#endif

void dwc3_set_mode(struct dwc3 *dwc3_reg, u32 mode)
{
	u32 reg;

	reg = xhci_readl(&dwc3_reg->g_ctl);
	reg &= ~(DWC3_GCTL_PRTCAPDIR(DWC3_GCTL_PRTCAP_OTG));
	reg |= DWC3_GCTL_PRTCAPDIR(mode);
	xhci_writel(&dwc3_reg->g_ctl, reg);
}

static void dwc3_core_soft_reset(struct dwc3 *dwc3_reg)
{
	u32		reg;
	int		i;

	/* Before Resetting PHY, put Core in Reset */
	reg = xhci_readl(&dwc3_reg->g_ctl);
	reg |= DWC3_GCTL_CORESOFTRESET;
	xhci_writel(&dwc3_reg->g_ctl, reg);

	for (i = 0; i < 1; i++) {
		/* Assert USB3 PHY reset */
		reg = xhci_readl(&dwc3_reg->g_usb3pipectl[i]);
		reg |= DWC3_GUSB3PIPECTL_PHYSOFTRST;
		xhci_writel(&dwc3_reg->g_usb3pipectl[i], reg);
	}

	for (i = 0; i < 3; i++) {
		/* Assert USB2 PHY reset */
		reg = xhci_readl(&dwc3_reg->g_usb2phycfg[i]);
		reg |= DWC3_GUSB2PHYCFG_PHYSOFTRST;
		xhci_writel(&dwc3_reg->g_usb2phycfg[i], reg);
	}

	//amlogic_usb2_phy_init(amlogic.usb2_phy);
	//amlogic_usb3_phy_init(amlogic.usb3_phy);
	amlogic.phy2_init(amlogic.usb2_phy, amlogic.u2_port_num, amlogic.phy_2);
	amlogic.phy3_init(amlogic.usb3_phy, amlogic.u3_port_num);
	mdelay(100);


	for (i = 0; i < 1; i++) {
		/* Clear USB3 PHY reset */
		reg = xhci_readl(&dwc3_reg->g_usb3pipectl[i]);
		reg &= ~DWC3_GUSB3PIPECTL_PHYSOFTRST;
		xhci_writel(&dwc3_reg->g_usb3pipectl[i], reg);
	}

	for (i = 0; i < 3; i++) {
			/* Clear USB2 PHY reset */
		reg = xhci_readl(&dwc3_reg->g_usb2phycfg[i]);
		reg &= ~DWC3_GUSB2PHYCFG_PHYSOFTRST;
		reg &= ~DWC3_GUSB2PHYCFG_SUSPHY;
		reg &= ~DWC3_GUSB2PHYCFG_PHYIF;
		xhci_writel(&dwc3_reg->g_usb2phycfg[i], reg);
	}

	mdelay(100);

	/* After PHYs are stable we can take Core out of reset state */
	reg = xhci_readl(&dwc3_reg->g_ctl);
	reg &= ~DWC3_GCTL_CORESOFTRESET;
	xhci_writel(&dwc3_reg->g_ctl, reg);
}

static int dwc3_core_init(struct dwc3 *dwc3_reg)
{
	u32 reg;
	u32 revision;
	unsigned int dwc3_hwparams1;
	unsigned long		timeout;

	revision = xhci_readl(&dwc3_reg->g_snpsid);
	/* This should read as U3 followed by revision number */
	if ((revision & DWC3_GSNPSID_MASK) != 0x55330000) {
		printf("this is not a DesignWare USB3 DRD Core\n");
		return -EINVAL;
	}

	/* issue device SoftReset too */
	timeout = 500;
	xhci_writel(&dwc3_reg->d_ctl, DWC3_DCTL_CSFTRST);
	do {
		reg = xhci_readl(&dwc3_reg->d_ctl);
		if (!(reg & DWC3_DCTL_CSFTRST))
			break;

		timeout--;
		mdelay(1);
	} while (timeout);

	if (!timeout) {
		printf("device SoftReset fail!\n");
		return -EINVAL;
	}

	dwc3_core_soft_reset(dwc3_reg);
	reg = xhci_readl(&dwc3_reg->g_uctl);
	reg |= DWC3_GUCTL_USBHSTINAUTORETRYEN;
	xhci_writel(&dwc3_reg->g_uctl, reg);

	reg = xhci_readl((uint32_t volatile *)DWC3_GFLADJ);
	reg &= ~DWC3_GFLADJ_30MHZ_MASK;
	reg |= GFLADJ_30MHZ_REG_SEL | 0x26;
	xhci_writel((uint32_t volatile *)DWC3_GFLADJ, reg);

	dwc3_hwparams1 = xhci_readl(&dwc3_reg->g_hwparams1);

	reg = xhci_readl(&dwc3_reg->g_ctl);
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

	xhci_writel(&dwc3_reg->g_ctl, reg);

	return 0;
}

static int amlogic_xhci_core_init(struct amlogic_xhci *amlogic)
{
	int ret;

	ret = dwc3_core_init(amlogic->dwc3_reg);
	if (ret) {
		debug("failed to initialize core\n");
		return -EINVAL;
	}

	/* We are hard-coding DWC3 core to Host Mode */
	dwc3_set_mode(amlogic->dwc3_reg, DWC3_GCTL_PRTCAP_HOST);

	return 0;
}

int xhci_hcd_init(int index, struct xhci_hccr **hccr, struct xhci_hcor **hcor)
{
	struct amlogic_xhci *ctx = &amlogic;
	struct amlogic_usb_config * usb_config;
	int ret;
	int i = 0;

#ifdef CONFIG_USB_XHCI_AMLOGIC_USB3_V2
	set_comb_phy_to_usb3_mode();
#endif

	//usb_config = board_usb_start(BOARD_USB_MODE_HOST, index);
	printf("l<%u>, f<%s>\n",__LINE__, __func__);
	//usb_config = NULL;
	usb_config_set_function(&usb_config, BOARD_USB_MODE_HOST, index);
	printf("l<%u>, f<%s>,ptr<%p>,index=%d\n",__LINE__, __func__, xhci_hcd_init, index);

	ctx->hcd = (struct xhci_hccr *)(ulong)(usb_config->base_addr);
	ctx->dwc3_reg = (struct dwc3 *)((char *)(ctx->hcd) + DWC3_REG_OFFSET);
	ctx->usb3_phy = (struct usb_aml_regs *)(ulong)(usb_config->usb_phy3_base_addr);
	ctx->usb2_phy = (struct u2p_aml_regs *)(ulong)(usb_config->usb_phy2_base_addr);
	ctx->u2_port_num = usb_config->u2_port_num;
	ctx->u3_port_num = usb_config->u3_port_num;
	for (i = 0; i < usb_config->u2_port_num; i++)
		ctx->phy_2[i] = (uint32_t volatile *)(ulong)usb_config->usb_phy2_pll_base_addr[i];

	ctx->phy2_init = usb_config->phy2_init;
    ctx->phy3_init = usb_config->phy3_init;
    ctx->phy_shutdown = usb_config->phy_shutdown;

	ret = amlogic_xhci_core_init(ctx);
	if (ret) {
		puts("XHCI: failed to initialize controller\n");
		return -EINVAL;
	}

	*hccr = (ctx->hcd);
	*hcor = (struct xhci_hcor *)((ulong) *hccr
				+ HC_LENGTH(xhci_readl(&(*hccr)->cr_capbase)));

	debug("amlogic-xhci: init hccr %lx and hcor %lx hc_length %d\n",
		(ulong)*hccr, (ulong)*hcor,
		(uint32_t)HC_LENGTH(xhci_readl(&(*hccr)->cr_capbase)));

	return 0;
}

void xhci_hcd_stop(int index)
{
	struct amlogic_xhci *ctx = &amlogic;

	//amlogic_xhci_core_exit(ctx);
	amlogic.phy_shutdown(ctx->usb3_phy, ctx->usb2_phy);
}
