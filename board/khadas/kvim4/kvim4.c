// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2022 Wesion, Inc. All rights reserved.
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
#include <i2c.h>
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
#ifdef CONFIG_POWER_FUSB302
#include <power/fusb302.h>
#endif
#include <asm/arch-meson/boot.h>


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

#ifdef CONFIG_AML_LCD

void board_lcd_detect(void)
{
	int ret = 0;
	int res = 0;
	u8 value = 0;
	uchar linebuf[1];
	struct udevice *bus;
	struct udevice *dev;

	uclass_get_device_by_seq(UCLASS_I2C, 3, &bus);
	ret = i2c_get_chip(bus, 0x38, 1, &dev);
	if (!ret) {
		res = dm_i2c_read(dev, 0xA8, linebuf, 1);
		if (!res) {
			printf("TP05 id=0x%x\n", linebuf[0]);
			if (linebuf[0] == 0x51) {//old ts050
				env_set("panel_type", "mipi_0");
				value = 1;
				printf("panel_type : mipi_0\n");
			} else if (linebuf[0] == 0x79) {//new ts050
				env_set("panel_type", "mipi_2");
				value = 1;
				printf("panel_type : mipi_2\n");
			}
		}
	}
	if (ret || res) {
		ret = i2c_get_chip(bus, 0x14, 1, &dev);
		if (!ret) {
			res = dm_i2c_read(dev, 0x9e, linebuf, 1);
			if (!res) {
				printf("TP10 id=0x%x\n", linebuf[0]);
				if (linebuf[0] == 0x00) {//TS101
					env_set("panel_type", "mipi_1");
					value = 1;
					printf("panel_type : mipi_1\n");
				}
			}
		}
	}

	env_set_ulong("mipi_lcd_exist", value);
	printf("mipi_lcd_exist : %d\n", value);
}
#endif /* CONFIG_AML_LCD */

unsigned int get_romcode_boot_id(void)
{
	const cpu_id_t cpuid = get_cpu_id();
	const int familyId   = cpuid.family_id;

	unsigned int boot_id = 0;
#ifdef SYSCTRL_SEC_STATUS_REG2
	if (MESON_CPU_MAJOR_ID_SC2 <= familyId && MESON_CPU_MAJOR_ID_C2 != familyId) {
		boot_id = readl(SYSCTRL_SEC_STATUS_REG2);
		debug("boot_id 0x%x\n", boot_id);
		boot_id = (boot_id>>4) & 0xf;
	}
	debug("boot_id 0x%x\n", boot_id);
#endif// #ifdef SYSCTRL_SEC_STATUS_REG2

	return boot_id;
}

const char *get_boot_source_str(unsigned int boot_id)
{
	const char *source;

	switch (boot_id) {
	case BOOT_DEVICE_EMMC:
		source = "emmc";
		break;

	case BOOT_DEVICE_NAND:
		source = "nand";
		break;

	case BOOT_DEVICE_SPI:
		source = "spi";
		break;

	case BOOT_DEVICE_SD:
		source = "sd";
		break;

	case BOOT_DEVICE_USB:
		source = "usb";
		break;

	default:
		source = "unknown";
	}

	return source;
}

static void set_boot_source(void)
{
	const char *source;

	source = get_boot_source_str(get_romcode_boot_id());

	env_set("boot_source", source);
}

static void select_fdtfile(void)
{
	cpu_id_t cpu_id;

	cpu_id = get_cpu_id();

	if (cpu_id.chip_rev == 0xA || cpu_id.chip_rev == 0xb) {
		env_set("chip_recv", "b");
		env_set("fdtfile", "amlogic/" CONFIG_DEFAULT_DEVICE_TREE".dtb");

	} else if (cpu_id.chip_rev == 0xC) {
		env_set("chip_recv", "c");
		env_set("fdtfile", "amlogic/" CONFIG_DEFAULT_DEVICE_TREE"n.dtb");
	}
}


#ifdef CONFIG_AML_HDMITX21
static void hdmitx_set_hdmi_5v(void)
{
	/*Power on VCC_5V for HDMI_5V*/
	writel(readl(PADCTRL_GPIOH_OEN) & (~(1 << 1)), PADCTRL_GPIOH_OEN);
	writel(readl(PADCTRL_GPIOH_O) | (1 << 1), PADCTRL_GPIOH_O);
}
#endif
void board_init_mem(void) {
	#if 1
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
	#endif
}

int board_init(void)
{
	printf("board init\n");

	/* The non-secure watchdog is enabled in BL2 TEE, disable it */
	run_command("watchdog off", 0);
	printf("watchdog disable\n");

	run_command("gpio set GPIOT_15", 0);//5G reset

#ifdef CONFIG_POWER_FUSB302
	fusb302_init();
#endif

	aml_set_bootsequence(0);
	//Please keep try usb boot first in board_init, as other init before usb may cause burning failure
#if defined(CONFIG_AML_V3_FACTORY_BURN) && defined(CONFIG_AML_V3_USB_TOOl)
	if ((0x1b8ec003 != readl(SYSCTRL_SEC_STICKY_REG2)) && (0x1b8ec004 != readl(SYSCTRL_SEC_STICKY_REG2)))
	{ aml_v3_factory_usb_burning(0, gd->bd); }
#endif//#if defined(CONFIG_AML_V3_FACTORY_BURN) && defined(CONFIG_AML_V3_USB_TOOl)

#if 0 //bypass below operations for pxp
	active_clk();
#endif
	pinctrl_devices_active(PIN_CONTROLLER_NUM);
	/*set vcc5V*/
	run_command("gpio set GPIOH_4", 0);
	run_command("gpio clear GPIOT_15", 0);//5G reset
	run_command("gpio clear GPIOH_3", 0); //pcie reset-gpio

	// FAN testing
	run_command("i2c dev 6", 0);
	run_command("i2c mw 0x18 0x96 1", 0);

	return 0;
}


int board_late_init(void)
{
	printf("board late init\n");

	//default uboot env need before anyone use it
	if (env_get("default_env")) {
		printf("factory reset, need default all uboot env.\n");
		run_command("defenv_reserv; setenv upgrade_step 2; saveenv;", 0);
	}

	run_command("echo upgrade_step $upgrade_step; if itest ${upgrade_step} == 1; then "\
			"defenv_reserv; setenv upgrade_step 2; saveenv; fi;", 0);
	board_init_mem();

	// Set boot source
	set_boot_source();

	// Select fdtfile
	select_fdtfile();

#ifndef CONFIG_SYSTEM_RTOS //prue rtos not need dtb
	if ( run_command("run common_dtb_load", 0) ) {
		printf("Fail in load dtb with cmd[%s]\n", env_get("common_dtb_load"));
	} else {
		//load dtb here then users can directly use 'fdt' command
		run_command("if fdt addr ${dtb_mem_addr}; then else echo no valid dtb at ${dtb_mem_addr};fi;", 0);
	}
#endif//#ifndef CONFIG_SYSTEM_RTOS //prue rtos not need dtb

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
	board_lcd_detect();
	lcd_probe();
#endif

	unsigned char chipid[16];

	memset(chipid, 0, 16);

	if (get_chip_id(chipid, 16) != -1) {
		char chipid_str[32];
		int i, j;
		char buf_tmp[4];

		memset(chipid_str, 0, 32);

		char *buff = &chipid_str[0];

		for (i = 0, j = 0; i < 12; ++i) {
			sprintf(&buf_tmp[0], "%02x", chipid[15 - i]);
			if (strcmp(buf_tmp, "00") != 0) {
				sprintf(buff + j, "%02x", chipid[15 - i]);
				j = j + 2;
			}
		}
		env_set("cpu_id", chipid_str);
		printf("buff: %s\n", buff);
	} else {
		env_set("cpu_id", "1234567890");
	}
	return 0;
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

ulong board_get_usable_ram_top(ulong total_size)
{
	unsigned long top = gd->ram_top;

	if (top >= 0xE0000000UL) {
		return 0xE0000000UL;
	}
	return top;
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
	unsigned long nddrSize = ((0x100000000 <= ((readl(SYSCTRL_SEC_STATUS_REG4) &
				0xFFFFFFFF0000) << 4)) ? 0xe0000000 :
				(((readl(SYSCTRL_SEC_STATUS_REG4)) &
				0xFFFFFFFF0000) << 4));


	if ( nddrSize <= 0xe0000000 )
		bd_mem_map[0].size = nddrSize;

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

#ifdef CONFIG_MULTI_DTB
phys_size_t get_ddr_memsize(void)
{
	phys_size_t ddr_size = env_get_hex("board_ddr_size", 0);

	if (!ddr_size) {
		ddr_size = (((readl(SYSCTRL_SEC_STATUS_REG4)) & ~0xffffUL) << 4);
		printf("init board ddr size  %llx\n", ddr_size);
		env_set_hex("board_ddr_size", ddr_size);
	}
	return ddr_size;
}
#endif

int checkhw(char * name)
{
#ifdef CONFIG_MULTI_DTB
	char *p_aml_dt = env_get("aml_dt");
	cpu_id_t cpu_id;

	printf("checkhw aml_dt:%s\n", p_aml_dt);
	if (!p_aml_dt) {
		char loc_name[64] = {0};
		phys_size_t ddr_size = get_ddr_memsize();
		cpu_id = get_cpu_id();

		switch (ddr_size) {
		case CONFIG_T7_3G_SIZE:
			if (cpu_id.chip_rev == 0xA || cpu_id.chip_rev == 0xb) {
				strcpy(loc_name, "t7_a311d2_an400-3g\0");
			} else if (cpu_id.chip_rev == 0xC) {
				strcpy(loc_name, "t7c_a311d2_an400-3g\0");
				//
			}
			break;
		case CONFIG_T7_4G_SIZE:
			if (cpu_id.chip_rev == 0xA || cpu_id.chip_rev == 0xb) {
				strcpy(loc_name, "t7_a311d2_an400\0");
			} else if (cpu_id.chip_rev == 0xC) {
				strcpy(loc_name, "t7c_a311d2_an400-4g\0");
				//
			}
			break;
		case CONFIG_T7_6G_SIZE:
			if (cpu_id.chip_rev == 0xA || cpu_id.chip_rev == 0xb) {
				strcpy(loc_name, "t7_a311d2_an400-6g\0");
			} else if (cpu_id.chip_rev == 0xC) {
				strcpy(loc_name, "t7c_a311d2_an400-6g\0");
				//
			}
			break;
		case CONFIG_T7_8G_SIZE:
			if (cpu_id.chip_rev == 0xA || cpu_id.chip_rev == 0xb) {
				strcpy(loc_name, "t7_a311d2_an400-8g\0");
			} else if (cpu_id.chip_rev == 0xC) {
				strcpy(loc_name, "t7c_a311d2_an400-8g\0");
				//
			}
			break;
		default:
			printf("DDR size: 0x%llx, multi-dt doesn't support, ", ddr_size);
			printf("set defalult t7_a311d2_an400\n");
			if (cpu_id.chip_rev == 0xA || cpu_id.chip_rev == 0xb) {
				strcpy(loc_name, "t7_a311d2_an400\0");
			} else if (cpu_id.chip_rev == 0xC) {
				strcpy(loc_name, "t7c_a311d2_an400-4g\0");
				//
			}
			break;
		}
		printf("init aml_dt to %s\n", loc_name);
		strcpy(name, loc_name);
		env_set("aml_dt", loc_name);
	} else {
		strcpy(name, env_get("aml_dt"));
	}
#else
	env_set("aml_dt", "t7_a311d2_an400\0");
#endif
	return 0;
}

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
