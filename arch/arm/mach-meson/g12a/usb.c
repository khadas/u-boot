
/*
 * arch/arm/cpu/armv8/txl/usb.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <asm/arch/usb-v2.h>
#include <asm/arch/romboot.h>


static struct amlogic_usb_config * g_usb_cfg[BOARD_USB_MODE_MAX][USB_PHY_PORT_MAX];
struct usb_aml_regs *usb_aml_reg;

#ifdef CONFIG_USB_XHCI_AMLOGIC_USB3_V2
#define PCIE_PHY_CFG_BASE_ADDR      0xff646000
void set_comb_phy_to_usb3_mode(void)
{
	u32 val = 0;

	*P_RESET0_LEVEL &= ~((0x1<<12) | (0x3<<14));
	*P_RESET0_LEVEL |=	  (0x1<<12) | (0x3<<14);

	val = xhci_readl((uint32_t volatile *)PCIE_PHY_CFG_BASE_ADDR);
	val |= (3<<5);
	xhci_writel((uint32_t volatile *)PCIE_PHY_CFG_BASE_ADDR, val);
}
#endif



static void amlogic_usb3_phy_init(struct usb_aml_regs *phy,unsigned int u3portnum)
{
	usb_r1_t r1 = {.d32 = 0};
	usb_r2_t r2 = {.d32 = 0};
	usb_r3_t r3 = {.d32 = 0};
	int i;
#if 0
	u32 data = 0;
#endif

	if (u3portnum == 0) {
		usb_aml_reg = (struct usb_aml_regs *)((ulong)phy);
		r1.d32 = usb_aml_reg->usb_r1;
		usb_aml_reg->usb_r2 = r1.d32;
		r1.b.u3h_fladj_30mhz_reg = 0x26;
		usb_aml_reg->usb_r2 = r1.d32;
		udelay(100);
	}

	for (i = 0; i < u3portnum; i++) {
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
		r1.b.u3h_fladj_30mhz_reg = 32;
		usb_aml_reg->usb_r2 = r1.d32;
		udelay(2);
#if 0
		/*
		* WORKAROUND: There is SSPHY suspend bug due to which USB enumerates
		* in HS mode instead of SS mode. Workaround it by asserting
		* LANE0.TX_ALT_BLOCK.EN_ALT_BUS to enable TX to use alt bus mode
		*/
		data = cr_bus_read(0x102d);
		data |= (1 << 7);
		cr_bus_write(0x102D, data);

		data = cr_bus_read(0x1010);
		data &= ~0xff0;
		data |= 0x20;
		cr_bus_write(0x1010, data);

		/*
		* Fix RX Equalization setting as follows
		* LANE0.RX_OVRD_IN_HI. RX_EQ_EN set to 0
		* LANE0.RX_OVRD_IN_HI.RX_EQ_EN_OVRD set to 1
		* LANE0.RX_OVRD_IN_HI.RX_EQ set to 3
		* LANE0.RX_OVRD_IN_HI.RX_EQ_OVRD set to 1
		*/
		data = cr_bus_read(0x1006);
		data &= ~(1 << 6);
		data |= (1 << 7);
		data &= ~(0x7 << 8);
		data |= (0x3 << 8);
		data |= (0x1 << 11);
		cr_bus_write(0x1006, data);

		/*
		* Set EQ and TX launch amplitudes as follows
		* LANE0.TX_OVRD_DRV_LO.PREEMPH set to 22
		* LANE0.TX_OVRD_DRV_LO.AMPLITUDE set to 127
		 * LANE0.TX_OVRD_DRV_LO.EN set to 1.
		*/
		data = cr_bus_read(0x1002);
		data &= ~0x3f80;
		data |= (0x16 << 7);
		data &= ~0x7f;
		data |= (0x7f | (1 << 14));
		cr_bus_write(0x1002, data);

		/*
		* TX_FULL_SWING  to 127
		*/
		data = cr_bus_read(0x30);
		data &= ~(0xf << 4);
		cr_bus_write(0x30, data);

		/*
		* TX_FULL_SWING  to 127
		*/
		r1.d32 = usb_aml_reg->usb_r1;
		r1.b.p30_pcs_tx_swing_full = 127;
		r1.b.u3h_fladj_30mhz_reg = 0x20;
		usb_aml_reg->usb_r1 = r1.d32;
		udelay(2);

		/*
		* TX_DEEMPH_3_5DB  to 22
		*/
		r2.d32 = usb_aml_reg->usb_r2;
		r2.b.p30_pcs_tx_deemph_3p5db = 22;
		usb_aml_reg->usb_r2 = r2.d32;

		udelay(2);
		/*
		* LOS_BIAS	to 0x5
		* LOS_LEVEL to 0x9
		*/
		r3.d32 = usb_aml_reg->usb_r3;
		r3.b.p30_los_bias = 0x5;
		r3.b.p30_los_level = 0x9;
		r3.b.p30_ssc_en = 1;
		r3.b.p30_ssc_range = 2;
		usb_aml_reg->usb_r3 = r3.d32;
#endif
	}

	return;
}

static void amlogic_usb2_phy_init(struct u2p_aml_regs *phy, unsigned int u2portnum,uint32_t volatile **phypl)
{
	struct u2p_aml_regs *u2p_aml_reg;
	u2p_r0_t dev_u2p_r0;
	u2p_r1_t dev_u2p_r1;
	int i;
	int cnt;
	int time_dly = 500;

	*P_RESET1_REGISTER |= (1<<2);

	udelay(time_dly);

	for (i = 0; i < u2portnum; i++) {
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

	for (i = 0; i < u2portnum; i++) {
		printf("------set usb pll\n");
		set_usb_pll(phypl[i]);
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

static void amlogic_usb_phy_exit(struct usb_aml_regs *phy, struct u2p_aml_regs *phy2){
    amlogic_usb2_phy_exit(phy2);
    amlogic_usb3_phy_exit(phy);
    return;
}



void usb_config_set_function(struct amlogic_usb_config **usbcon, int mode,int index){

    *usbcon = board_usb_start(mode, index);
    if (!*usbcon) {
        printf("board_usb_start: the mode is error,f=%s,p=%p\n", __func__, usb_config_set_function);
        return;
    }
    (*usbcon)->phy2_init = amlogic_usb2_phy_init;
    (*usbcon)->phy3_init = amlogic_usb3_phy_init;
    (*usbcon)->phy_shutdown = amlogic_usb_phy_exit;
    return;
}

struct amlogic_usb_config * board_usb_start(int mode,int index)
{
	printf("USB3.0 XHCI init start\n");

	if (mode < 0 || mode >= BOARD_USB_MODE_MAX||!g_usb_cfg[mode][index])
		return 0;

	writel((1 << 2), P_RESET1_REGISTER);

	if (mode == BOARD_USB_MODE_HOST )
		if (g_usb_cfg[mode][index]->set_vbus_power)
			g_usb_cfg[mode][index]->set_vbus_power(1);

	return g_usb_cfg[mode][index];
}

int board_usb_stop(int mode,int index)
{
	printf("board_usb_stop cfg: %d\n",mode);

	return 0;
}

int usb_index = 0;
void board_usb_init(struct amlogic_usb_config * usb_cfg,int mode)
{
	if (mode < 0 || mode >= BOARD_USB_MODE_MAX || !usb_cfg)
		return ;

	if (mode == BOARD_USB_MODE_HOST) {
		if (usb_index >= USB_PHY_PORT_MAX)
			return;
		g_usb_cfg[mode][usb_index] = usb_cfg;
		usb_index++;
	} else
		g_usb_cfg[mode][0] = usb_cfg;
	printf("register usb cfg[%d][%d] = %p\n",mode,(mode==BOARD_USB_MODE_HOST)?usb_index:0,usb_cfg);
}

int get_usb_count(void)
{
    return  usb_index;
}

void set_usb_pll(uint32_t volatile *phy2_pll_base)
{
    (*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x40))
        = (USB_PHY2_PLL_PARAMETER_1 | USB_PHY2_RESET | USB_PHY2_ENABLE);
    (*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x44)) =
        USB_PHY2_PLL_PARAMETER_2;
    (*(volatile uint32_t *)((unsigned long)phy2_pll_base + 0x48)) =
        USB_PHY2_PLL_PARAMETER_3;
    udelay(100);
    (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x40))
        = (((USB_PHY2_PLL_PARAMETER_1) | (USB_PHY2_ENABLE))
        & (~(USB_PHY2_RESET)));
}

void board_usb_pll_disable(struct amlogic_usb_config *cfg)
{
    int i = 0;

	*(volatile uint32_t *)P_RESET1_LEVEL |= (3 << 16);
    for (i = 0; i < cfg->u2_port_num; i++) {
        (*(volatile uint32_t *)(unsigned long)
            (cfg->usb_phy2_pll_base_addr[i] + 0x40))
            = ((USB_PHY2_PLL_PARAMETER_1 | USB_PHY2_RESET)
            & (~(USB_PHY2_ENABLE)));
    }
}

#ifdef CONFIG_USB_DEVICE_V2
#define USB_REG_A 0xFF636000
#define USB_REG_B 0xFF63A000

void set_usb_phy_tuning_1(int port)
{
	unsigned long phy_reg_base;

	if (port > 2)
		return;

	if (port == 0 )
		phy_reg_base = USB_REG_A;
	else
		phy_reg_base = USB_REG_B;

	(*(volatile uint32_t *)(phy_reg_base + 0x10)) = 0xfff;
	(*(volatile uint32_t *)(phy_reg_base + 0x50)) = 0xfe18;
	(*(volatile uint32_t *)(phy_reg_base + 0x38)) = 0xe0004;
	(*(volatile uint32_t *)(phy_reg_base + 0x34)) = 0xc8000;
}
#endif

