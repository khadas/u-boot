/*
 * Copyright (c) 2024 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <dm.h>
#include <spl.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/grf_rk3506.h>
#include <asm/arch/ioc_rk3506.h>
#include <asm/arch-rockchip/rockchip_smccc.h>

DECLARE_GLOBAL_DATA_PTR;

#define GRF_BASE		0xff288000
#define GRF_SOC_CON28		0X0070

#define USBPHY_APB_BASE		0xff2b0000
#define USBPHY_DIFF_RECEIVER_0	0x0030
#define USBPHY_DIFF_RECEIVER_1	0x0430

#define FIREWALL_DDR_BASE	0xff5f0000
#define FW_DDR_MST1_REG 	0x24

#define GPIO0_IOC_BASE		0xff950000
#define GPIO1_IOC_BASE		0xff660000

#define GPIO2_IOC_BASE		0xff4d8000
#define GPIO2A_IOMUX_SEL_0	0x40
#define GPIO2A_IOMUX_SEL_1	0x44

#define GPIO3_IOC_BASE	GPIO2_IOC_BASE
#define GPIO3A_IOMUX_SEL_0	0x60
#define GPIO3A_IOMUX_SEL_1	0x64
#define GPIO3A_PULL		0x230

#define CRU_BASE		0xff9a0000
#define CRU_GLB_RST_CON		0xc10
#define CRU_GATE_CON5           0x0814
#define CRU_SOFTRST_CON5        0x0a14

void board_debug_uart_init(void)
{
	/* No need to change uart*/
}

void board_set_iomux(enum if_type if_type, int devnum, int routing)
{
	switch (if_type) {
	case IF_TYPE_MMC:
		if (devnum == 0) {
			/* MMC */
			writel(0xffff1111, GPIO3_IOC_BASE + GPIO3A_IOMUX_SEL_0);
			writel(0x00ff0011, GPIO3_IOC_BASE + GPIO3A_IOMUX_SEL_1);
			/* Pull up */
			writel(0x0ffc0554, GPIO3_IOC_BASE + GPIO3A_PULL);
		}
		break;
	case IF_TYPE_MTD:
		if (routing == 0) {
			/* FSPI M0 */
			writel(0xffff1111, GPIO2_IOC_BASE + GPIO2A_IOMUX_SEL_0);
			writel(0x00ff0011, GPIO2_IOC_BASE + GPIO2A_IOMUX_SEL_1);
		}
		break;
	default:
		break;
	}
}

void board_unset_iomux(enum if_type if_type, int devnum, int routing)
{
	switch (if_type) {
	case IF_TYPE_MMC:
		if (devnum == 0) {
			/* MMC */
			writel(0xffff0000, GPIO3_IOC_BASE + GPIO3A_IOMUX_SEL_0);
			writel(0x00ff0022, GPIO3_IOC_BASE + GPIO3A_IOMUX_SEL_1);
			/* Pull down */
			writel(0x0ffc0aa8, GPIO3_IOC_BASE + GPIO3A_PULL);
		}
		break;
	case IF_TYPE_MTD:
		if (routing == 0) {
			/* FSPI M0 */
			writel(0xffff0000, GPIO2_IOC_BASE + GPIO2A_IOMUX_SEL_0);
			writel(0x00ff0000, GPIO2_IOC_BASE + GPIO2A_IOMUX_SEL_1);
		}
		break;
	default:
		break;
	}
}

#ifdef CONFIG_SPL_BUILD
void rockchip_stimer_init(void)
{
	/* If Timer already enabled, don't re-init it */
	u32 reg = readl(CONFIG_ROCKCHIP_STIMER_BASE + 0x4);
	if (reg & 0x1)
		return;
	writel(0x00010000, CONFIG_ROCKCHIP_STIMER_BASE + 0x4);

	asm volatile("mcr p15, 0, %0, c14, c0, 0"
		     : : "r"(COUNTER_FREQUENCY));
	writel(0xffffffff, CONFIG_ROCKCHIP_STIMER_BASE + 0x14);
	writel(0xffffffff, CONFIG_ROCKCHIP_STIMER_BASE + 0x18);
	writel(0x00010001, CONFIG_ROCKCHIP_STIMER_BASE + 0x4);
}

void spl_board_storages_fixup(struct spl_image_loader *loader)
{
	if (!loader)
		return;

	/* Only have one mmc controller. */
	if (loader->boot_device == BOOT_DEVICE_MMC1)
		/* Unset the sdmmc0 iomux */
		board_unset_iomux(IF_TYPE_MMC, 0, 0);
}
#endif

int arch_cpu_init(void)
{
#if defined(CONFIG_SPL_BUILD)
	u32 val;

	/* Set the sdmmc/emmc to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST1_REG);
	writel(val & 0xffff00ff, FIREWALL_DDR_BASE + FW_DDR_MST1_REG);

#if defined(CONFIG_MMC_DW_ROCKCHIP)
	/* Set the sdmmc/emmc iomux */
	board_set_iomux(IF_TYPE_MMC, 0, 0);
#endif
	/* Set the fspi to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST1_REG);
	writel(val & 0xff00ffff, FIREWALL_DDR_BASE + FW_DDR_MST1_REG);

	/* Set the fspi iomux */
	board_set_iomux(IF_TYPE_MTD, 0, 0);

	/*
	 * Wdt0 and WDT1 trigger global reset enable.
	 * Wdt0 and WDT1 trigger first global reset.
	 */
	writel(0x18c0, CRU_BASE + CRU_GLB_RST_CON);

	/*
	 * Set the USB2 PHY Port1 in suspend mode and
	 * turn off the differential receiver for both
	 * Port0 and Port1 to save power.
	 */
	writel(0x01ff01d1, GRF_BASE + GRF_SOC_CON28);
	writel(0x00000079, USBPHY_APB_BASE + USBPHY_DIFF_RECEIVER_0);
	writel(0x00000079, USBPHY_APB_BASE + USBPHY_DIFF_RECEIVER_1);
#endif
	return 0;
}

int fit_standalone_release(char *id, uintptr_t entry_point)
{
	/* address map: map 0 to sram, enable TCM mode for sram
	 * 0xfff84000 for sram
	 * 0x03e00000 for ddr */
	sip_smc_mcu_config(ROCKCHIP_SIP_CONFIG_BUSMCU_0_ID,
		ROCKCHIP_SIP_CONFIG_MCU_CODE_START_ADDR,
		entry_point);

	/*
	* bus m0 configuration:
	* open m0 swclktck & hclk
	*/
	writel(0x0c000000, CRU_BASE + CRU_GATE_CON5);

	/* set m0 system time calibration GRF->GRF_SOC_CON36 */
	writel(0xbcd3d80, 0xff288090);

	/* enable m0 interrupt: PMU->PMU_INT_MASK_CON mcu_rst_dis_cfg=1,glb_int_mask_mcu=0 */
	writel(0x00060004, 0xff90000c);

	/* select jtag m1 GPIO0C6 GPIO0C7 */
	//writel(0x00220000, 0xff960000);
	//writel(0x00300020, 0xff288000);
	//writel(0x00ff0022, 0xff4d8064);
	//writel(0xff002200, 0xff950014);
	return 0;
}
