
/*
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
#ifdef CONFIG_AML_HDMITX20
#include <amlogic/media/vout/hdmitx/hdmitx_module.h>
#endif
#ifdef CONFIG_AML_LCD
#include <amlogic/media/vout/lcd/lcd_vout.h>
#endif

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
 * this function should be write with asm, here, is is only for compiling pass
 * */
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


#ifdef CONFIG_AML_HDMITX20
static void hdmitx_set_hdmi_5v(void)
{
	/*Power on VCC_5V for HDMI_5V*/
}
#endif
void board_init_mem(void) {
	#if 1
	/* config bootm low size, make sure whole dram/psram space can be used */
	phys_size_t ram_size;
	char *env_tmp;
	env_tmp = env_get("bootm_size");
	if (!env_tmp) {
		ram_size = (readl(SYSCTRL_SEC_STATUS_REG4) & ~0xffffUL) << 4;
		if (ram_size >= 0xe0000000)
			ram_size = 0xe0000000;

		env_set_hex("bootm_low", 0);
		env_set_hex("bootm_size", ram_size);
	}
	#endif
}

int board_init(void)
{
	printf("board init\n");

	/* The non-secure watchdog is enabled in BL2 TEE, disable it */
	run_command("watchdog off", 0);
	printf("watchdog disable\n");

#if 0
	run_command("startdsp 0 0x300a0000 0", 0);
	printf("dsp start!\n");
	while (1);
#endif

#ifdef CONFIG_PXP_DDR
	return 0;
#else
	aml_set_bootsequence(0);
	//Please keep try usb boot first in board_init, as other init before usb may cause burning failure
#if defined(CONFIG_AML_V3_FACTORY_BURN) && defined(CONFIG_AML_V3_USB_TOOl)
	if ((0x1b8ec003 != readl(SYSCTRL_SEC_STICKY_REG2)) && (0x1b8ec004 != readl(SYSCTRL_SEC_STICKY_REG2)))
	{ aml_v3_factory_usb_burning(0, gd->bd); }
#endif//#if defined(CONFIG_AML_V3_FACTORY_BURN) && defined(CONFIG_AML_V3_USB_TOOl)

#if 0
	active_clk();
#ifdef CONFIG_AML_HDMITX20
	hdmitx_set_hdmi_5v();
	hdmitx_init();
#endif
#endif
	pinctrl_devices_active(PIN_CONTROLLER_NUM);
	/*set vcc5V*/
	run_command("gpio set GPIOH_1", 0);
	return 0;
#endif
}

int board_late_init(void)
{
	printf("board late init\n");
#ifdef CONFIG_PXP_DDR
	return 0;
#else
	run_command("echo upgrade_step $upgrade_step; if itest ${upgrade_step} == 1; then "\
			"defenv_reserv; setenv upgrade_step 2; saveenv; fi;", 0);
	board_init_mem();

#ifndef CONFIG_SYSTEM_RTOS //pure rtos not need dtb
	if ( run_command("run common_dtb_load", 0) ) {
		printf("Fail in load dtb with cmd[%s]\n", env_get("common_dtb_load"));
	} else {
		//load dtb here then users can directly use 'fdt' command
		run_command("if fdt addr ${dtb_mem_addr}; then else echo no valid dtb at ${dtb_mem_addr};fi;", 0);
	}
#endif//#ifndef CONFIG_SYSTEM_RTOS //pure rtos not need dtb

#ifdef CONFIG_AML_FACTORY_BURN_LOCAL_UPGRADE //try auto upgrade from ext-sdcard
	aml_try_factory_sdcard_burning(0, gd->bd);
#endif//#ifdef CONFIG_AML_FACTORY_BURN_LOCAL_UPGRADE
	//auto enter usb mode after board_late_init if 'adnl.exe setvar burnsteps 0x1b8ec003'
#if defined(CONFIG_AML_V3_FACTORY_BURN) && defined(CONFIG_AML_V3_USB_TOOl)
	if (0x1b8ec003 == readl(SYSCTRL_SEC_STICKY_REG2))
	{ aml_v3_factory_usb_burning(0, gd->bd); }
#endif//#if defined(CONFIG_AML_V3_FACTORY_BURN) && defined(CONFIG_AML_V3_USB_TOOl)

#ifdef CONFIG_AML_VPU
	vpu_probe();
#endif
#ifdef CONFIG_AML_VPP
	vpp_init();
#endif
#ifdef CONFIG_AML_VOUT
	vout_probe();
#endif
#ifdef CONFIG_AML_LCD
	lcd_probe();
#endif
	return 0;
#endif
}

ulong board_get_usable_ram_top(ulong total_size)
{
	unsigned long top = gd->ram_top;

	if (top >= 0xE0000000UL) {
		return 0xE0000000UL;
	}
	return top;
}

phys_size_t get_effective_memsize(void)
{
	phys_size_t ddr_size;

	// >>16 -> MB, <<20 -> real size, so >>16<<20 = <<4
#if defined(CONFIG_SYS_MEM_TOP_HIDE)
	ddr_size = (readl(SYSCTRL_SEC_STATUS_REG4) & ~0xffffUL) << 4;
	if (ddr_size >= 0xe0000000)
		ddr_size = 0xe0000000;
	return (ddr_size - CONFIG_SYS_MEM_TOP_HIDE);
#else
	ddr_size = (readl(SYSCTRL_SEC_STATUS_REG4) & ~0xffffUL) << 4;
	if (ddr_size >= 0xe0000000)
		ddr_size = 0xe0000000;
	return ddr_size
#endif /* CONFIG_SYS_MEM_TOP_HIDE */

}

phys_size_t get_ddr_info_size(void)
{
	phys_size_t ddr_size = (((readl(SYSCTRL_SEC_STATUS_REG4)) & ~0xffffUL) << 4);

	return ddr_size;
}

static struct mm_region bd_mem_map[] = {
	{
		.virt = 0x00000000UL,
		.phys = 0x00000000UL,
		.size = 0xe0000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0xe0000000UL,
		.phys = 0xe0000000UL,
		.size = 0x20000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = bd_mem_map;

int mach_cpu_init(void) {
	//printf("\nmach_cpu_init\n");
#ifdef 	CONFIG_UPDATE_MMU_TABLE
	unsigned long nddrSize = (readl(SYSCTRL_SEC_STATUS_REG4) & ~0xffffUL) << 4;

	if ( nddrSize < 0xe0000000 )
		bd_mem_map[0].size = nddrSize;

#endif
	return 0;
}

#ifdef CONFIG_MULTI_DTB
int checkhw(char * name)
{
	char loc_name[64] = {0};
	unsigned long ddr_size=0;

	int i;
	for (i=0; i<CONFIG_NR_DRAM_BANKS; i++) {
		ddr_size += gd->bd->bi_dram[i].size;
	}
#if defined(CONFIG_SYS_MEM_TOP_HIDE)
	ddr_size += CONFIG_SYS_MEM_TOP_HIDE;
#endif

	switch (ddr_size) {
	case 0xe0000000UL:
		strcpy(loc_name, "p1_skt_4g\0");
		break;
	case 0x200000000UL:
		strcpy(loc_name, "p1_skt_8g\0");
		break;
	default:
		strcpy(loc_name, "p1_as408_unsupport");
		break;
	}
	/* set aml_dt */
	strcpy(name, loc_name);
	env_set("aml_dt", loc_name);
	return 0;
}
#endif

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
	.size = 2*SZ_1M,
},
{
	.name = "recovery",
	.offset = 0,
	.size = 16*SZ_1M,
},
{
	.name = "boot",
	.offset = 0,
	.size = 16*SZ_1M,
},
{
	.name = "system",
	.offset = 0,
	.size = 64*SZ_1M,
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

const char * const _env_args_reserve_[] =
{
	"lock",
	"upgrade_step",
	"bootloader_version",

	NULL//Keep NULL be last to tell END
};

int __attribute__((weak)) mmc_initialize(bd_t *bis){ return 0;}

int __attribute__((weak)) do_bootm(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]){ return 0;}

void __attribute__((weak)) set_working_fdt_addr(ulong addr) {}

int __attribute__((weak)) ofnode_read_u32_default(ofnode node, const char *propname, u32 def) {return 0;}

void __attribute__((weak)) md5_wd (unsigned char *input, int len, unsigned char output[16],	unsigned int chunk_sz){}
