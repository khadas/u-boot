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
#include <asm/arch/grf_rv1103b.h>
#include <asm/arch/ioc_rv1103b.h>

DECLARE_GLOBAL_DATA_PTR;

#define PERI_CRU_BASE			0x20000000
#define PERICRU_PERISOFTRST_CON10	0x0a28

#define PMU0_CRU_BASE			0x20070000
#define PMUCRU_PMUSOFTRST_CON02		0x0a08

#define GRF_SYS_BASE			0x20150000
#define GRF_SYS_HPMCU_CACHE_MISC	0x0214

#define GPIO0_IOC_BASE			0x201B0000
#define GPIO0A_IOMUX_SEL_H		0x04
#define GPIO0_BASE			0x20520000
#define GPIO_SWPORT_DR_L		0x00
#define GPIO_SWPORT_DDR_L		0x08

#define GPIO1_IOC_BASE			0x20170000
#define GPIO1A_IOMUX_SEL_0		0x20
#define GPIO1A_IOMUX_SEL_1_0		0x24
#define GPIO1A_IOMUX_SEL_1_1		0x10024
#define GPIO1B_IOMUX_SEL_0		0x10028
#define GPIO1B_IOMUX_SEL_1		0x1002c
#define GPIO1_IOC_GPIO1A_PULL_0		0x210
#define GPIO1_IOC_GPIO1A_PULL_1		0x10210
#define GPIO1_IOC_GPIO1B_PULL		0x10214
#define GPIO1_IOC_JTAG_M2_CON		0x10810

#define GPIO2_IOC_BASE			0x20840000
#define GPIO2A_IOMUX_SEL_1_1		0x44

#define SGRF_SYS_BASE			0x20250000
#define SGRF_SYS_SOC_CON2		0x0008
#define SGRF_SYS_SOC_CON3		0x000c
#define SGRF_SYS_OTP_CON		0x0018
#define FIREWALL_CON0			0x0020
#define FIREWALL_CON1			0x0024
#define FIREWALL_CON2			0x0028
#define FIREWALL_CON3			0x002c
#define FIREWALL_CON4			0x0030
#define FIREWALL_CON5			0x0034
#define FIREWALL_CON7			0x003c
#define SGRF_SYS_HPMCU_BOOT_DDR		0x0080

#define SGRF_PMU_BASE			0x20260000
#define SGRF_PMU_SOC_CON0		0x0000
#define SGRF_PMU_PMUMCU_BOOT_ADDR	0x0020

#define SYS_GRF_BASE			0x20150000
#define GRF_SYS_PERI_CON2		0x08
#define GRF_SYS_USBPHY_CON0		0x50

#define TOP_CRU_BASE			0x20060000
#define TOPCRU_CRU_GLB_RST_CON 		0xc10

#define USBPHY_APB_BASE			0x20e10000
#define USBPHY_FSLS_DIFF_RECEIVER	0x0100

void board_debug_uart_init(void)
{
	/* No need to change uart */
}

void board_set_iomux(enum if_type if_type, int devnum, int routing)
{
	switch (if_type) {
	case IF_TYPE_MMC:
		if (devnum == 0) {
			writel(0xffff1111, GPIO1_IOC_BASE + GPIO1A_IOMUX_SEL_0);
			writel(0x00ff0011, GPIO1_IOC_BASE + GPIO1A_IOMUX_SEL_1_0);
		} else if (devnum == 1) {
#if CONFIG_SPL_BUILD
			/* set SDMMC D0-3/CMD/CLK to gpio and pull down */
			writel(0xf0000000, GPIO1_IOC_BASE + GPIO1A_IOMUX_SEL_1_1);
			writel(0xffff0000, GPIO1_IOC_BASE + GPIO1B_IOMUX_SEL_0);
			writel(0x000f0000, GPIO1_IOC_BASE + GPIO1B_IOMUX_SEL_1);
			writel(0xc0008000, GPIO1_IOC_BASE + GPIO1_IOC_GPIO1A_PULL_1);
			writel(0x03ff02AA, GPIO1_IOC_BASE + GPIO1_IOC_GPIO1B_PULL);

			/* SDMMC PWREN GPIO0A4 power down and power up */
			writel(0x00100010, GPIO0_BASE + GPIO_SWPORT_DR_L);
			writel(0x00100010, GPIO0_BASE + GPIO_SWPORT_DDR_L);
			mdelay(50);
			writel(0x00100000, GPIO0_BASE + GPIO_SWPORT_DR_L);
#endif
			/* set SDMMC D0-3/CMD/CLK and pull up */
			writel(0xff001100, GPIO1_IOC_BASE + GPIO1A_IOMUX_SEL_1_1);
			writel(0xffff1111, GPIO1_IOC_BASE + GPIO1B_IOMUX_SEL_0);
			writel(0x000f0001, GPIO1_IOC_BASE + GPIO1B_IOMUX_SEL_1);
			writel(0xc0004000, GPIO1_IOC_BASE + GPIO1_IOC_GPIO1A_PULL_1);
			writel(0x03ff0155, GPIO1_IOC_BASE + GPIO1_IOC_GPIO1B_PULL);
		}
		break;
	case IF_TYPE_MTD:
		if (routing == 0) {
			/* FSPI0 M0 */
			writel(0xffff2222, GPIO1_IOC_BASE + GPIO1A_IOMUX_SEL_0);
			writel(0x00ff0022, GPIO1_IOC_BASE + GPIO1A_IOMUX_SEL_1_0);
		}
		break;
	default:
		printf("Bootdev 0x%x is not support\n", if_type);
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

int spl_fit_standalone_release(char *id, uintptr_t entry_point)
{
	if (!strcmp(id, "mcu0")) {
		/* set the hpmcu boot address */
		writel(entry_point, SGRF_SYS_BASE + SGRF_SYS_HPMCU_BOOT_DDR);
	} else if (!strcmp(id, "mcu1")) {
		/* reset lpmcu */
		writel(0x000f000f, PMU0_CRU_BASE + PMUCRU_PMUSOFTRST_CON02);
		/* set the lpmcu boot address */
		writel(entry_point, SGRF_PMU_BASE + SGRF_PMU_PMUMCU_BOOT_ADDR);
		writel(0x00800000, SGRF_PMU_BASE + SGRF_PMU_SOC_CON0);
		/* release lpmcu */
		writel(0x000f0000, PMU0_CRU_BASE + PMUCRU_PMUSOFTRST_CON02);
	}

	return 0;
}

void rk_meta_process(void)
{
	/* trigger software irq to hpmcu that means meta was ready */
	writel(0x00080008, GRF_SYS_BASE + GRF_SYS_HPMCU_CACHE_MISC);
}
#endif

#ifndef CONFIG_TPL_BUILD
int arch_cpu_init(void)
{
#if defined(CONFIG_SPL_BUILD) || defined(CONFIG_SUPPORT_USBPLUG)
	/* Set all devices to Non-secure */
	writel(0xffff0000, SGRF_SYS_BASE + FIREWALL_CON0);
	writel(0xffff0000, SGRF_SYS_BASE + FIREWALL_CON1);
	writel(0xffff0000, SGRF_SYS_BASE + FIREWALL_CON2);
	writel(0xffff0000, SGRF_SYS_BASE + FIREWALL_CON3);
	writel(0xffff0000, SGRF_SYS_BASE + FIREWALL_CON4);
	writel(0xffff0000, SGRF_SYS_BASE + FIREWALL_CON5);
	writel(0x01f00000, SGRF_SYS_BASE + FIREWALL_CON7);
	/* Set OTP to none secure mode */
	writel(0x00020000, SGRF_SYS_BASE + SGRF_SYS_OTP_CON);

#if defined(CONFIG_ROCKCHIP_EMMC_IOMUX)
	/* Set the emmc iomux */
	board_set_iomux(IF_TYPE_MMC, 0, 0);
#elif defined(CONFIG_ROCKCHIP_SFC_IOMUX)
	/* Set the fspi iomux */
	board_set_iomux(IF_TYPE_MTD, 0, 0);
#endif

#if defined(CONFIG_MMC_DW_ROCKCHIP)
	/* Set the sdmmc iomux and power cycle */
	board_set_iomux(IF_TYPE_MMC, 1, 0);
	/* Enable force_jtag */
	writel(0x00010001, GPIO1_IOC_BASE + GPIO1_IOC_JTAG_M2_CON);
#endif

	/* no-secure WDT reset output will reset SoC system. */
	writel(0x00010001, SYS_GRF_BASE + GRF_SYS_PERI_CON2);
	/* secure WDT reset output will reset SoC system. */
	writel(0x00010001, SGRF_SYS_BASE + SGRF_SYS_SOC_CON2);
	/*
	 * enable tsadc trigger global reset and select first reset.
	 * enable global reset and wdt trigger pmu reset.
	 * select first reset trigger pmu reset.
	 */
	writel(0x0000ffdf, TOP_CRU_BASE + TOPCRU_CRU_GLB_RST_CON);

	/*
	 * Set the USB2 PHY in suspend mode and turn off the
	 * USB2 PHY FS/LS differential receiver to save power:
	 * VCC1V8_USB : reduce 3.8 mA
	 * VDD_0V9 : reduce 4.4 mA
	 */
	writel(0x01ff01d1, SYS_GRF_BASE + GRF_SYS_USBPHY_CON0);
	writel(0x00000000, USBPHY_APB_BASE + USBPHY_FSLS_DIFF_RECEIVER);

#ifdef CONFIG_SPI_FLASH_AUTO_MERGE
	/* gpio1a5/gpio2a6 cs-gpio */
	writel(0x00F00000, GPIO1_IOC_BASE + GPIO1A_IOMUX_SEL_1_0);
	writel(0x0F000000, GPIO2_IOC_BASE + GPIO2A_IOMUX_SEL_1_1);
#endif
#endif

	return 0;
}
#endif

#ifdef CONFIG_ROCKCHIP_IMAGE_TINY
int rk_board_scan_bootdev(void)
{
	char *devtype, *devnum;

	if (!run_command("blk dev mmc 1", 0) &&
	    !run_command("rkimgtest mmc 1", 0)) {
		devtype = "mmc";
		devnum = "1";
	} else {
		run_command("blk dev mtd 2", 0);
		devtype = "mtd";
		devnum = "2";
	}
	env_set("devtype", devtype);
	env_set("devnum", devnum);

	return 0;
}
#endif

#if defined(CONFIG_ROCKCHIP_EMMC_IOMUX) && defined(CONFIG_ROCKCHIP_SFC_IOMUX)
#error FSPI and eMMC iomux is incompatible for rv1103b Soc. You should close one of them.
#endif
