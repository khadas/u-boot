/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/g12b/usb.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <asm/arch/usb-v2.h>
#include <asm/arch/romboot.h>
#include <asm/cpu_id.h>

#define USB2_PHY_PLL_OFFSET_c	(0x34)

static struct amlogic_usb_config * g_usb_cfg[BOARD_USB_MODE_MAX][USB_PHY_PORT_MAX];
static int Rev_flag = 0;

/*Rev_flag == 1, g12b and revB, tl1 */
static void board_usb_check_g12b_revb (void)
{
	cpu_id_t cpu_id = get_cpu_id();

	if (cpu_id.family_id == MESON_CPU_MAJOR_ID_G12B) {
		if (cpu_id.chip_rev == 0xb)
			Rev_flag = 1;
		else
			Rev_flag = 0;
	} else {
		Rev_flag = 0;
	}
	return;
}

static int board_usb_get_revb_type (void)
{
	return Rev_flag;
}
struct amlogic_usb_config * board_usb_start(int mode,int index)
{
	printf("USB3.0 XHCI init start\n");
	board_usb_check_g12b_revb();

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

static void set_pll_Calibration_default(uint32_t volatile *phy2_pll_base)
{
    u32 tmp;

    tmp = (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x8));
    tmp &= 0xfff;
    tmp |= (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x10));
    (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x10))
     = tmp;
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

    (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0xC))
     = USB2_PHY_PLL_OFFSET_c;
	if (board_usb_get_revb_type() == 1) {
        (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x50))
         = USB_G12x_PHY_PLL_SETTING_1;
        (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x54))
         = 0x2a;
        set_pll_Calibration_default(phy2_pll_base);
    } else {
        (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x10))
         = USB_G12x_PHY_PLL_SETTING_2;
        (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x50))
         = USB_G12x_PHY_PLL_SETTING_1;
        (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x38))
         = 0x0;
    }

    (*(volatile uint32_t *)(unsigned long)((unsigned long)phy2_pll_base + 0x34))
     = USB_G12x_PHY_PLL_SETTING_3;
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

	if (board_usb_get_revb_type() == 1) {
		return;
	}

	(*(volatile uint32_t *)(unsigned long)((unsigned long)phy_reg_base + 0x38))
		= USB_G12x_PHY_PLL_SETTING_4;
	(*(volatile uint32_t *)(unsigned long)((unsigned long)phy_reg_base + 0x34))
		= USB_G12x_PHY_PLL_SETTING_3;
}
#endif

