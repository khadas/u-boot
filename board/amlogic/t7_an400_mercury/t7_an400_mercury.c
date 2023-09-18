// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/io.h>
#include <malloc.h>
#include <errno.h>
#include <environment.h>
#include <fdt_support.h>
#include <linux/libfdt.h>
#include <amlogic/cpu_id.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/pinctrl_init.h>
#include <linux/sizes.h>
#include <asm-generic/gpio.h>
#include <dm.h>
#include <asm/armv8/mmu.h>
#include <amlogic/aml_v3_burning.h>
#include <amlogic/aml_v2_burning.h>
#include <linux/mtd/partitions.h>
#include <asm/arch/bl31_apis.h>
#ifdef CONFIG_AML_VPU
#include <amlogic/media/vpu/vpu.h>
#endif
#ifdef CONFIG_AML_VPP
#include <amlogic/media/vpp/vpp.h>
#endif
#ifdef CONFIG_AML_VOUT
#include <amlogic/media/vout/aml_vout.h>
#endif
#ifdef CONFIG_AML_HDMITX21
#include <amlogic/media/vout/hdmitx21/hdmitx_module.h>
#endif
#ifdef CONFIG_AML_LCD
#include <amlogic/media/vout/lcd/lcd_vout.h>
#endif
#ifdef CONFIG_RX_RTERM
#include <amlogic/aml_hdmirx.h>
#endif
#include <amlogic/storage.h>
#include <asm/arch/pwr_ctrl.h>
#include <amlogic/board.h>

DECLARE_GLOBAL_DATA_PTR;

void sys_led_init(void)
{
}

int serial_set_pin_port(unsigned long port_base)
{
	return 0;
}

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;
	return 0;
}

/* secondary_boot_func
 * this function should be write with asm, here, is only for compiling pass
 */
void secondary_boot_func(void)
{
}

int board_eth_init(bd_t *bis)
{
	return 0;
}

int active_clk(void)
{
	struct udevice *clk = NULL;
	int err;

	err = uclass_get_device_by_name(UCLASS_CLK,
			"xtal-clk", &clk);
	if (err) {
		pr_err("Can't find xtal-clk clock (%d)\n", err);
		return err;
	}
	err = uclass_get_device_by_name(UCLASS_CLK,
			"clock-controller@0", &clk);
	if (err) {
		pr_err("Can't find clock-controller@0 clock (%d)\n", err);
		return err;
	}

	return 0;
}

#ifdef CONFIG_AML_HDMITX21
static void hdmitx_set_hdmi_5v(void)
{
	/*Power on VCC_5V for HDMI_5V*/
	writel(readl(PADCTRL_GPIOH_OEN) & (~(1 << 1)), PADCTRL_GPIOH_OEN);
	writel(readl(PADCTRL_GPIOH_O) | (1 << 1), PADCTRL_GPIOH_O);
}
#endif

void board_init_mem(void)
{
	/* config bootm low size, make sure whole dram/psram space can be used */
	phys_size_t ram_size;
	char *env_tmp;

	env_tmp = env_get("bootm_size");
	if (!env_tmp) {
		ram_size = ((0x100000000 <= ((readl(SYSCTRL_SEC_STATUS_REG4) &
				0xFFFFFFFF0000) << 4)) ? 0xe0000000 :
					(((readl(SYSCTRL_SEC_STATUS_REG4)) &
					0xFFFFFFFF0000) << 4));
		env_set_hex("bootm_low", 0);
		env_set_hex("bootm_size", ram_size);
	}
}

int board_init(void)
{
	printf("board init\n");

	/* The non-secure watchdog is enabled in BL2 TEE, disable it */
	run_command("watchdog off", 0);
	printf("watchdog disable\n");

	aml_set_bootsequence(0);
	//Please keep try usb boot first in board_init
	//as other init before usb may cause burning failure
#if defined(CONFIG_AML_V3_FACTORY_BURN) && defined(CONFIG_AML_V3_USB_TOOl)
	if ((readl(SYSCTRL_SEC_STICKY_REG2) != 0x1b8ec003) &&
		(readl(SYSCTRL_SEC_STICKY_REG2) != 0x1b8ec004))
		aml_v3_factory_usb_burning(0, gd->bd);
#endif//#if defined(CONFIG_AML_V3_FACTORY_BURN) && defined(CONFIG_AML_V3_USB_TOOl)

	pinctrl_devices_active(PIN_CONTROLLER_NUM);
	/*set vcc5V*/
	run_command("gpio set GPIOH_1", 0);
	run_command("gpio set GPIOY_8", 1);
	return 0;
}

int board_boot_freertos(void)
{
	int rc;
	image_header_t imghd;
	uint64_t loadaddr;
	uint32_t imagesize;
	char *slot_name;
	char partName[20] = {0};

	slot_name = env_get("slot-suffixes");
	if (strcmp(slot_name, "0") == 0)
		strcpy((char *)partName, "freertos_a");
	else if (strcmp(slot_name, "1") == 0)
		strcpy((char *)partName, "freertos_b");

	printf("freertos_partName = %s\n", partName);
	rc = store_read(partName, 0, sizeof(imghd), (unsigned char *)&imghd);
	if (rc) {
		printf("Fail to read from part[freertos] at offset 0\n");
		return __LINE__;
	}

	if (strcmp((char *)imghd.ih_name, "rtos") || imghd.ih_size == 0)
		return __LINE__;

	loadaddr = (imghd.ih_load & 0x000000FFU) << 24 | (imghd.ih_load & 0x0000FF00U) << 8 |
		(imghd.ih_load & 0x00FF0000U) >> 8 | (imghd.ih_load & 0xFF000000U) >> 24;
	imagesize = (imghd.ih_size & 0x000000FFU) << 24 | (imghd.ih_size & 0x0000FF00U) << 8 |
		(imghd.ih_size & 0x00FF0000U) >> 8 | (imghd.ih_size & 0xFF000000U) >> 24;

	printf("freertos run addr:0x%08llx size=%d\n", loadaddr, imagesize);
	store_read(partName, 0, imagesize + sizeof(imghd), (unsigned char *)loadaddr);
	memmove((unsigned char *)loadaddr, (unsigned char *)(loadaddr + sizeof(imghd)), imagesize);

	flush_cache(loadaddr, imagesize);
	power_core_for_freetos(0x01, loadaddr);

	return 0;
}

void media_clock_init(void)
{
	unsigned int val = 0;

	/* enable mipi dsi A/B clk81 */
	writel(readl(CLKCTRL_SYS_CLK_EN0_REG0) | (1 << 2) | (1 << 3), CLKCTRL_SYS_CLK_EN0_REG0);

	/* enable dsi 0/1 PHY clock, rate depends on the lcd Resolution*/
	writel(readl(CLKCTRL_MIPIDSI_PHY_CLK_CTRL) | (1 << 8) | (1 << 24),
			CLKCTRL_MIPIDSI_PHY_CLK_CTRL);

	/* config dsi A/B mean clock to 50M */
	val = (1 << 9) | (1 << 21);/* select fclk4 */
	val |= (9 << 0) | (9 << 12);/* 500M/10 */
	val |= (1 << 8) | (1 << 20);
	writel(val, CLKCTRL_MIPI_DSI_MEAS_CLK_CTRL);

	/* dwarp(amlgdc) = 800M */
	val = 0;
	val = (0x7 << 9);/* select fclk2p5 */
	val |= (0 << 31);/* select gdcclk0 */
	val |= (1 << 8) | (1 << 30);
	writel(val, CLKCTRL_AMLGDC_CLK_CTRL);

	/* vapb = 667M */
	val = 0;
	val = (0x1 << 9);/* select fclk3 */
	val &= ~(1 << 31);/* select vapb0 */
	val |= (1 << 8) | (1 << 30); /* enable vapb0 and ge2d gate*/
	writel(val, CLKCTRL_VAPBCLK_CTRL);
	printf("CLKCTRL_VAPBCLK_CTRL = %x\n", readl(CLKCTRL_VAPBCLK_CTRL));

	/* mipi csi phy 0 = 200M */
	val = readl(CLKCTRL_MIPI_CSI_PHY_CLK_CTRL);
	val &= ~0xfff;
	val |= (0x6 << 9) | (1 << 0) | (1 << 8);
	val &= ~(1 << 31);
	writel(val, CLKCTRL_MIPI_CSI_PHY_CLK_CTRL);

	/* mipi isp clk = 500 */
	val = 0;
	val = (0x1 << 9) | (1 << 8);/* select fclk4 and enable*/
	writel(val, CLKCTRL_MIPI_ISP_CLK_CTRL);

	/* set mclk pll = 74.25M */
	writel(0x20014063, ANACTRL_MCLK_PLL_CNTL0);
	udelay(20);
	writel(0x30014063, ANACTRL_MCLK_PLL_CNTL0);
	writel(0x1470500f, ANACTRL_MCLK_PLL_CNTL1);
	writel(0x00023041, ANACTRL_MCLK_PLL_CNTL2);
	writel(0x18180000, ANACTRL_MCLK_PLL_CNTL3);
	writel(0x20202, ANACTRL_MCLK_PLL_CNTL4);
	writel(0x10014063, ANACTRL_MCLK_PLL_CNTL0);
	udelay(20);
	writel(0x00023001, ANACTRL_MCLK_PLL_CNTL2);

	/* set mclk0, mclk1 = 37.125M */
	writel(readl(ANACTRL_MCLK_PLL_CNTL4) | 0x1 | (1 << 2) | (1 << 10) | (1 << 8),
		ANACTRL_MCLK_PLL_CNTL4);
}

void board_power_domain_on(void)
{
	printf("t7 power domain on\n");
	// csi & isp
	pwr_ctrl_psci_smc(PM_ISP, PWR_ON);
	pwr_ctrl_psci_smc(PM_MIPI_ISP, PWR_ON);
	// ge2d & dewarp parent
	pwr_ctrl_psci_smc(PM_NIC3, PWR_ON);
	// ge2d
	pwr_ctrl_psci_smc(PM_GE2D, PWR_ON);
	// dewarp
	pwr_ctrl_psci_smc(PM_DEWARP, PWR_ON);
	// osd
	//pwr_ctrl_psci_smc(PM_DEWARP, PWR_ON);
	// dsi
	pwr_ctrl_psci_smc(PM_MIPI_DSI0, PWR_ON);
	pwr_ctrl_psci_smc(PM_MIPI_DSI1, PWR_ON);
	// hdmi
	pwr_ctrl_psci_smc(PM_VPU_HDMI, PWR_ON);
	pwr_ctrl_psci_smc(PM_HDMIRX, PWR_ON);
	// spicc1
+	pwr_ctrl_psci_smc(PM_SPICC1, PWR_ON);
}

int board_late_init(void)
{
	printf("board late init\n");
	aml_board_late_init_front(NULL);

#ifdef CONFIG_AML_VPU
	vpu_probe();
#endif
#ifdef CONFIG_AML_HDMITX21
	hdmitx_set_hdmi_5v();
	hdmitx21_init();
#endif
#ifdef CONFIG_AML_VPP
	vpp_init();
#endif
#ifdef CONFIG_RX_RTERM
	rx_set_phy_rterm();
#endif
#ifdef CONFIG_AML_VOUT
	vout_probe();
#endif
#ifdef CONFIG_AML_LCD
	lcd_probe();
#endif

	/* The board id is used to determine if the NN needs to adjust voltage */
	switch (readl(SYSCTRL_SEC_STATUS_REG4) >> 8 & 0xff) {
	case 2:
		/* The NN needs to adjust the voltage */
		env_set_ulong("nn_adj_vol", 1);
		break;
	default:
		/* The NN does not need to adjust the voltage */
		env_set_ulong("nn_adj_vol", 0);
	}

	aml_board_late_init_tail(NULL);
	return 0;
}

phys_size_t get_effective_memsize(void)
{
	phys_size_t ddr_size;

	// >>16 -> MB, <<20 -> real size, so >>16<<20 = <<4
#if defined(CONFIG_SYS_MEM_TOP_HIDE)
	ddr_size = (readl(SYSCTRL_SEC_STATUS_REG4) & ~0xffffUL) << 4;
	return (ddr_size - CONFIG_SYS_MEM_TOP_HIDE);
#else
	ddr_size = (readl(SYSCTRL_SEC_STATUS_REG4) & ~0xffffUL) << 4;
	return ddr_size;
#endif /* CONFIG_SYS_MEM_TOP_HIDE */
}

phys_size_t get_ddr_info_size(void)
{
	phys_size_t ddr_size = (((readl(SYSCTRL_SEC_STATUS_REG4)) & ~0xffffUL) << 4);

	return ddr_size;
}

ulong board_get_usable_ram_top(ulong total_size)
{
	unsigned long top = gd->ram_top;

	if (top >= 0xE0000000UL)
		return 0xE0000000UL;

	return top;
}

static struct mm_region bd_mem_map[] = {
	{
		.virt = 0x00000000UL,
		.phys = 0x00000000UL,
		.size = 0x05100000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0x09000000UL,
		.phys = 0x09000000UL,
		.size = 0x77000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0x80000000UL,
		.phys = 0x80000000UL,
		.size = 0x80000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = bd_mem_map;

int mach_cpu_init(void)
{
	//printf("\nmach_cpu_init\n");
#ifdef CONFIG_UPDATE_MMU_TABLE
	unsigned long nddrSize = ((0x100000000 <= ((readl(SYSCTRL_SEC_STATUS_REG4) &
				0xFFFFFFFF0000) << 4)) ? 0xe0000000 :
				(((readl(SYSCTRL_SEC_STATUS_REG4)) &
				0xFFFFFFFF0000) << 4));

	if (nddrSize >= 0xe0000000) {
		bd_mem_map[1].size = 0xe0000000UL - bd_mem_map[1].phys;
		bd_mem_map[2].virt = 0xe0000000UL;
		bd_mem_map[2].phys = 0xe0000000UL;
		bd_mem_map[2].size = 0x20000000UL;
	}

#endif
	return 0;
}

int ft_board_setup(void *blob, bd_t *bd)
{
	/* eg: bl31/32 rsv */
	return 0;
}

/* partition table for spinor flash */
#ifdef CONFIG_SPI_FLASH
static const struct mtd_partition spiflash_partitions[] = {
	{
		.name = "env",
		.offset = 0,
		.size = 1 * SZ_256K,
	},
	{
		.name = "dtb",
		.offset = 0,
		.size = 1 * SZ_256K,
	},
	{
		.name = "boot",
		.offset = 0,
		.size = 2 * SZ_1M,
	},
	/* last partition get the rest capacity */
	{
		.name = "user",
		.offset = MTDPART_OFS_APPEND,
		.size = MTDPART_SIZ_FULL,
	}
};

const struct mtd_partition *get_spiflash_partition_table(int *partitions)
{
	*partitions = ARRAY_SIZE(spiflash_partitions);
	return spiflash_partitions;
}
#endif /* CONFIG_SPI_FLASH */

#ifdef CONFIG_MESON_NFC
static struct mtd_partition normal_partition_info[] = {
{
	.name = BOOT_BL2E,
	.offset = 0,
	.size = 0,
},
{
	.name = BOOT_BL2X,
	.offset = 0,
	.size = 0,
},
{
	.name = BOOT_DDRFIP,
	.offset = 0,
	.size = 0,
},
{
	.name = BOOT_DEVFIP,
	.offset = 0,
	.size = 0,
},
{
	.name = "logo",
	.offset = 0,
	.size = 2 * SZ_1M,
},
{
	.name = "recovery",
	.offset = 0,
	.size = 16 * SZ_1M,
},
{
	.name = "boot",
	.offset = 0,
	.size = 16 * SZ_1M,
},
{
	.name = "system",
	.offset = 0,
	.size = 64 * SZ_1M,
},
/* last partition get the rest capacity */
{
	.name = "data",
	.offset = MTDPART_OFS_APPEND,
	.size = MTDPART_SIZ_FULL,
},
};

struct mtd_partition *get_aml_mtd_partition(void)
{
	return normal_partition_info;
}

int get_aml_partition_count(void)
{
	return ARRAY_SIZE(normal_partition_info);
}

#endif

/* partition table */
/* partition table for spinand flash */
#if (defined(CONFIG_SPI_NAND) || defined(CONFIG_MTD_SPI_NAND))
static const struct mtd_partition spinand_partitions[] = {
	{
		.name = "logo",
		.offset = 0,
		.size = 2 * SZ_1M,
	},
	{
		.name = "recovery",
		.offset = 0,
		.size = 16 * SZ_1M,
	},
	{
		.name = "boot",
		.offset = 0,
		.size = 16 * SZ_1M,
	},
	{
		.name = "system",
		.offset = 0,
		.size = 64 * SZ_1M,
	},
	/* last partition get the rest capacity */
	{
		.name = "data",
		.offset = MTDPART_OFS_APPEND,
		.size = MTDPART_SIZ_FULL,
	}
};

const struct mtd_partition *get_partition_table(int *partitions)
{
	*partitions = ARRAY_SIZE(spinand_partitions);
	return spinand_partitions;
}
#endif /* CONFIG_SPI_NAND */

#ifdef CONFIG_MULTI_DTB
phys_size_t get_ddr_memsize(void)
{
	phys_size_t ddr_size = (((readl(SYSCTRL_SEC_STATUS_REG4)) & ~0xffffUL) << 4);

	printf("init board ddr size  %llx\n", ddr_size);
	env_set_hex("board_ddr_size", ddr_size);
	return ddr_size;
}
#endif

int checkhw(char *name)
{
#ifdef CONFIG_MULTI_DTB
	char loc_name[64] = {0};
	phys_size_t ddr_size = get_ddr_memsize();

	switch (ddr_size) {
	case CONFIG_T7_3G_SIZE:
		strcpy(loc_name, "t7_a311d2_an400-3g\0");
		break;
	case CONFIG_T7_4G_SIZE:
		strcpy(loc_name, "t7_a311d2_an400\0");
		break;
	case CONFIG_T7_6G_SIZE:
		strcpy(loc_name, "t7_a311d2_an400-6g\0");
		break;
	case CONFIG_T7_8G_SIZE:
		strcpy(loc_name, "t7_a311d2_an400-8g\0");
		break;
	default:
		printf("DDR size: 0x%llx, multi-dt doesn't support, ", ddr_size);
		printf("set default t7_a311d2_an400\n");
		strcpy(loc_name, "t7_a311d2_an400\0");
		break;
	}
	printf("init aml_dt to %s\n", loc_name);
	strcpy(name, loc_name);
	env_set("aml_dt", loc_name);
#else
	env_set("aml_dt", "t7_a311d2_an400\0");
#endif
	return 0;
}

const char * const _env_args_reserve_[] = {
	"lock",
	"upgrade_step",
	"bootloader_version",
	"dts_to_gpt",
	"fastboot_step",
	"reboot_status",
	"expect_index",

	NULL//Keep NULL be last to tell END
};

int __attribute__((weak)) mmc_initialize(bd_t *bis) { return 0; }

int __attribute__((weak)) do_bootm(cmd_tbl_t *cmdtp, int flag, int argc,
	char * const argv[]) { return 0; }

void __attribute__((weak)) set_working_fdt_addr(ulong addr) {}

int __attribute__((weak)) ofnode_read_u32_default(ofnode node,
	const char *propname, u32 def) { return 0; }

void __attribute__((weak)) md5_wd(unsigned char *input, int len, unsigned char output[16],
	unsigned int chunk_sz) {}
