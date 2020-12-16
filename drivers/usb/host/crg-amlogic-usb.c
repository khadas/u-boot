/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/usb/host/crg-amlogic-usb.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

/*
 * This file is a conglomeration for DWC3-init sequence and further
 * exynos5 specific PHY-init sequence.
 */

#include <common.h>
#include <libfdt.h>
#include <malloc.h>
#include <usb.h>
//#include <watchdog.h>
#include <asm/arch/cpu.h>
#include <asm-generic/errno.h>
#include <linux/compat.h>
//#include <linux/usb/dwc3.h>
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
	unsigned int u2_port_num;
	unsigned int u3_port_num;
	uint32_t volatile *phy_2[4];
};
struct usb_aml_regs *usb_aml_reg;

static struct amlogic_xhci amlogic;

#ifdef CONFIG_USB_XHCI_AMLOGIC_USB3_V2
#define PCIE_PHY_CFG_BASE_ADDR      0xff646000
static void set_comb_phy_to_usb3_mode(void)
{
	u32 val = 0;

	*P_RESET0_LEVEL &= ~((0x1<<12) | (0x3<<14));
	*P_RESET0_LEVEL |=	  (0x1<<12) | (0x3<<14);

	val = xhci_readl((uint32_t volatile *)PCIE_PHY_CFG_BASE_ADDR);
	val |= (3<<5);
	xhci_writel((uint32_t volatile *)PCIE_PHY_CFG_BASE_ADDR, val);
}
#endif

static void amlogic_usb2_phy_init(struct u2p_aml_regs *phy)
{
	struct u2p_aml_regs *u2p_aml_reg;
	u2p_r0_t dev_u2p_r0;
	u2p_r1_t dev_u2p_r1;
	int i;
	int cnt;
	int time_dly = 500;

	*P_RESET1_REGISTER |= (1<<2);

	udelay(time_dly);

	for (i = 0; i < amlogic.u2_port_num; i++) {
		u2p_aml_reg = (struct u2p_aml_regs *)((ulong)phy + i * PHY_REGISTER_SIZE);
		dev_u2p_r0.d32 = u2p_aml_reg->u2p_r0;
		dev_u2p_r0.b.host_device= 1;
		dev_u2p_r0.b.POR= 0;
		u2p_aml_reg->u2p_r0  = dev_u2p_r0.d32;
		udelay(10);
		*P_RESET1_REGISTER |= (1 << (16 + i));
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

	for (i = 0; i < amlogic.u2_port_num; i++) {
		set_usb_pll(amlogic.phy_2[i]);
	}
	return;
}

static void amlogic_usb3_phy_init(struct usb_aml_regs *phy)
{
	usb_r1_t r1 = {.d32 = 0};
	usb_r2_t r2 = {.d32 = 0};
	usb_r3_t r3 = {.d32 = 0};
	int i;

	for (i = 0; i < amlogic.u3_port_num; i++) {
		usb_aml_reg = (struct usb_aml_regs *)((ulong)phy+i*PHY_REGISTER_SIZE);

		r3.d32 = usb_aml_reg->usb_r3;
		r3.b.p30_ssc_en = 1;
		r3.b.p30_ref_ssp_en = 1;
		usb_aml_reg->usb_r3 = r3.d32;
		udelay(2);
		r2.d32 = usb_aml_reg->usb_r2;
		r2.b.p30_pcs_tx_deemph_3p5db = 0x15;
		r2.b.p30_pcs_tx_deemph_6db = 0x20;
		usb_aml_reg->usb_r2 = r2.d32;
		udelay(2);
		r1.d32 = usb_aml_reg->usb_r1;
		r1.b.u3h_host_port_power_control_present = 1;
		r1.b.u3h_fladj_30mhz_reg = 0x20;
		usb_aml_reg->usb_r2 = r1.d32;
		udelay(2);
	}

	return;
}

static void amlogic_usb2_phy_exit(struct u2p_aml_regs *phy)
{
	return;
}

static void amlogic_usb3_phy_exit(struct usb_aml_regs *phy)
{
	return;
}

static void amlogic_xhci_phy_init(void)
{
	amlogic_usb2_phy_init(amlogic.usb2_phy);
	amlogic_usb3_phy_init(amlogic.usb3_phy);
	mdelay(100);
}

static void amlogic_xhci_core_exit(struct amlogic_xhci *amlogic)
{
	amlogic_usb2_phy_exit(amlogic->usb2_phy);

	amlogic_usb3_phy_exit(amlogic->usb3_phy);
}

int xhci_hcd_init(int index, struct xhci_hccr **hccr, struct xhci_hcor **hcor)
{
	struct amlogic_xhci *ctx = &amlogic;
	struct amlogic_usb_config * usb_config;
	int i = 0;

#ifdef CONFIG_USB_XHCI_AMLOGIC_USB3_V2
	set_comb_phy_to_usb3_mode();
#endif

	usb_config = board_usb_start(BOARD_USB_MODE_HOST, index);

	ctx->hcd = (struct xhci_hccr *)(ulong)(usb_config->base_addr);
	ctx->usb3_phy = (struct usb_aml_regs *)(ulong)(usb_config->usb_phy3_base_addr);
	ctx->usb2_phy = (struct u2p_aml_regs *)(ulong)(usb_config->usb_phy2_base_addr);
	ctx->u2_port_num = usb_config->u2_port_num;
	ctx->u3_port_num = usb_config->u3_port_num;
	for (i = 0; i < usb_config->u2_port_num; i++)
		ctx->phy_2[i] = (uint32_t volatile *)(ulong)usb_config->usb_phy2_pll_base_addr[i];

	amlogic_xhci_phy_init();

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

	amlogic_xhci_core_exit(ctx);
}
