// SPDX-License-Identifier:     BSD-3-Clause
/*
 * Copyright (c) 2018 The Fuchsia Authors
 *
 */
#include <common.h>
#include <linux/mtd/partitions.h>
#include <nand.h>
#include <part.h>
#include <emmc_storage.h>
#include <zircon/zircon.h>

#include <asm/arch/reboot.h>
#include <asm/arch/secure_apb.h>
#include <asm/io.h>

#define PDEV_VID_AMLOGIC            5
#define PDEV_PID_AV400              0xF
#define NVRAM_LENGTH                (8 * 1024)
#define AV400_NUM_CPU               4
static const char CMDLINE_DEBUG[] = "devmgr.log-to-debuglog=true";
const char *BOOTLOADER_VERSION = "zircon-bootloader=0.10";
static const zbi_mem_range_t mem_config[] = {
	{
		.type = ZBI_MEM_RANGE_RAM,
		.length = 0x40000000, // 1 GB
	},
	{
		.type = ZBI_MEM_RANGE_PERIPHERAL,
		.paddr = 0xf5000000,
		.length = 0x0b000000,
	},
	// secmon_reserved:linux,secmon
	{
		.type = ZBI_MEM_RANGE_RESERVED,
		.paddr = 0x05000000,
		.length = 0x3400000,
	},
	// logo_reserved:linux,meson-fb
	{
		.type = ZBI_MEM_RANGE_RESERVED,
		.paddr = 0x3f800000,
		.length = 0x800000,
	},
	/* linux,usable-memory */
	{
		.type = ZBI_MEM_RANGE_RESERVED,
		.paddr = 0x00000000,
		.length = 0x100000,
	},
};

static const dcfg_simple_t uart_driver = {
	.mmio_phys = 0xfe07a000,
	.irq = 201,
};

static const dcfg_arm_gicv2_driver_t gicv2_driver = {
	.mmio_phys = 0xfff00000,
	.gicd_offset = 0x1000,
	.gicc_offset = 0x2000,
	.gich_offset = 0x4000,
	.gicv_offset = 0x6000,
	.ipi_base = 5,
};

static const dcfg_arm_psci_driver_t psci_driver = {
	.use_hvc = false,
	.reboot_args = { 1, 0, 0 },
	.reboot_bootloader_args = { 4, 0, 0 },
	.reboot_recovery_args = { 2, 0, 0 },
};

static const dcfg_arm_generic_timer_driver_t timer_driver = {
	.irq_phys = 30,
};

static const zbi_platform_id_t platform_id = {
	.vid = PDEV_VID_AMLOGIC,
	.pid = PDEV_PID_AV400,
	.board_name = "av400",
};

enum {
	PART_BOOTLOADER,
	PART_ZIRCON_A,
	PART_ZIRCON_B,
	PART_ZIRCON_R,
	PART_SYS_CONFIG,
	PART_FACTORY_CONFIG,
	PART_FVM,
	PART_COUNT,
};

static zbi_partition_map_t partition_map = {
	// .block_count filled in below
	// .block_size filled in below
	.guid = {},
	.partition_count = PART_COUNT,
	.partitions = {
		{
			.type_guid = GUID_BOOTLOADER_VALUE,
			.uniq_guid = {},
			// .first_block filled in below
			// .last_block filled in below
			.flags = 0,
			.name = "bootloader",
		},
		{
			.type_guid = GUID_ZIRCON_A_VALUE,
			.uniq_guid = {},
			// .first_block filled in below
			// .last_block filled in below
			.flags = 0,
			.name = "zircon-a",
		},
		{
			.type_guid = GUID_ZIRCON_B_VALUE,
			.uniq_guid = {},
			// .first_block filled in below
			// .last_block filled in below
			.flags = 0,
			.name = "zircon-b",
		},
		{
			.type_guid = GUID_ZIRCON_R_VALUE,
			.uniq_guid = {},
			// .first_block filled in below
			// .last_block filled in below
			.flags = 0,
			.name = "zircon-r",
		},
		{
			.type_guid = GUID_SYS_CONFIG_VALUE,
			.uniq_guid = {},
			// .first_block filled in below
			// .last_block filled in below
			.flags = 0,
			.name = "sys-config",
		},
		{
			.type_guid = GUID_FACTORY_CONFIG_VALUE,
			.uniq_guid = {},
			// .first_block filled in below
			// .last_block filled in below
			.flags = 0,
			.name = "factory",
		},
		{
			.type_guid = GUID_FVM_VALUE,
			.uniq_guid = {},
			// .first_block filled in below
			// .last_block filled in below
			.flags = 0,
			.name = "fvm",
		},
	},
};

static void add_cpu_topology(zbi_header_t *zbi)
{
	zbi_topology_node_t nodes[AV400_NUM_CPU + 1];
	zbi_topology_cluster_t cluster = {
		.performance_class = 1,
	};
	zbi_topology_node_t cluster_node = {
		.entity_type = ZBI_TOPOLOGY_ENTITY_CLUSTER,
		.parent_index = ZBI_TOPOLOGY_NO_PARENT,
		.entity = { .cluster = cluster },
	};
	nodes[0] = cluster_node;
	for (int cpu = 0; cpu < AV400_NUM_CPU; cpu++) {
		zbi_topology_arm_info_t arm_info = {
			.cluster_1_id = cpu,
			.cpu_id = 0,
			.gic_id = cpu,
		};
		zbi_topology_processor_t processor = {
			.logical_ids = { cpu },
			.logical_id_count = 1,
			.flags =
				(cpu == 0) ? ZBI_TOPOLOGY_PROCESSOR_PRIMARY : 0,
			.architecture = ZBI_TOPOLOGY_ARCH_ARM,
			.architecture_info = { arm_info },
		};
		zbi_topology_node_t node = {
			.entity_type = ZBI_TOPOLOGY_ENTITY_PROCESSOR,
			.parent_index = 0,
			.entity = { .processor = processor },
		};
		nodes[cpu + 1] = node;
	}
	zircon_append_boot_item(zbi, ZBI_TYPE_CPU_TOPOLOGY,
				      sizeof(zbi_topology_node_t), &nodes,
				      sizeof(nodes));
}

static void add_partition_map(zbi_header_t *zbi)
{
	struct blk_desc *dev_desc;
	disk_partition_t bootloader_info;
	disk_partition_t boot_info;
	disk_partition_t misc_info;
	disk_partition_t recovery_info;
	disk_partition_t tee_info;
	//disk_partition_t crypt_info;
	disk_partition_t system_info;
	disk_partition_t data_info;

	dev_desc = blk_get_dev("mmc", CONFIG_FASTBOOT_FLASH_MMC_DEV);
	if (!dev_desc || dev_desc->type == DEV_TYPE_UNKNOWN) {
		printf("could not find MMC device for partition map\n");
		return;
	}

	if (get_partition_info_aml_by_name(dev_desc, "bootloader", &bootloader_info)) {
		printf("could not find bootloader partition\n");
		return;
	}
	if (get_partition_info_aml_by_name(dev_desc, "boot", &boot_info)) {
		printf("could not find boot partition\n");
		return;
	}
	if (get_partition_info_aml_by_name(dev_desc, "misc", &misc_info)) {
		printf("could not find misc partition\n");
		return;
	}
	if (get_partition_info_aml_by_name(dev_desc, "recovery", &recovery_info)) {
		printf("could not find recovery partition\n");
		return;
	}
	if (get_partition_info_aml_by_name(dev_desc, "tee", &tee_info)) {
		printf("could not find tee partition\n");
		return;
	}
	if (get_partition_info_aml_by_name(dev_desc, "system", &system_info)) {
		printf("could not find system partition\n");
		return;
	}
	if (get_partition_info_aml_by_name(dev_desc, "data", &data_info)) {
		printf("could not find data partition\n");
		return;
	}

	// map bootloader partition to BOOTLOADER
	partition_map.partitions[PART_BOOTLOADER].first_block = bootloader_info.start;
	partition_map.partitions[PART_BOOTLOADER].last_block = bootloader_info.start +
								bootloader_info.size - 1;

	// map boot partition to ZIRCON_A
	partition_map.partitions[PART_ZIRCON_A].first_block = boot_info.start;
	partition_map.partitions[PART_ZIRCON_A].last_block = boot_info.start + boot_info.size - 1;

	// map misc partition to ZIRCON_B
	partition_map.partitions[PART_ZIRCON_B].first_block = misc_info.start;
	partition_map.partitions[PART_ZIRCON_B].last_block = misc_info.start + misc_info.size - 1;

	// map recovery partition to ZIRCON_R
	partition_map.partitions[PART_ZIRCON_R].first_block = recovery_info.start;
	partition_map.partitions[PART_ZIRCON_R].last_block = recovery_info.start +
								recovery_info.size - 1;

	// map tee partition to SYS_CONFIG
	partition_map.partitions[PART_SYS_CONFIG].first_block = tee_info.start;
	partition_map.partitions[PART_SYS_CONFIG].last_block = tee_info.start + tee_info.size - 1;

	// map system and data partitions to FVM
	partition_map.partitions[PART_FVM].first_block = system_info.start;
	partition_map.partitions[PART_FVM].last_block = data_info.start + data_info.size - 1;

	partition_map.block_count = data_info.start + data_info.size;
	partition_map.block_size = data_info.blksz;

	zircon_append_boot_item(zbi, ZBI_TYPE_DRV_PARTITION_MAP, 0, &partition_map,
				sizeof(zbi_partition_map_t) +
				partition_map.partition_count * sizeof(zbi_partition_t));
}

static void add_reboot_reason(zbi_header_t *zbi)
{
	// see cmd/amlogic/cmd_reboot.c
	const uint32_t reboot_mode_val = ((readl(AO_SEC_SD_CFG15) >> 12) & 0xf);

	zbi_hw_reboot_reason_t reboot_reason;

	switch (reboot_mode_val) {
	case AMLOGIC_COLD_BOOT:
		reboot_reason = ZBI_HW_REBOOT_COLD;
		break;
	case AMLOGIC_NORMAL_BOOT:
	case AMLOGIC_FACTORY_RESET_REBOOT:
	case AMLOGIC_UPDATE_REBOOT:
	case AMLOGIC_FASTBOOT_REBOOT:
	case AMLOGIC_SUSPEND_REBOOT:
	case AMLOGIC_HIBERNATE_REBOOT:
	case AMLOGIC_BOOTLOADER_REBOOT:
	case AMLOGIC_SHUTDOWN_REBOOT:
	case AMLOGIC_RPMBP_REBOOT:
	case AMLOGIC_QUIESCENT_REBOOT:
	case AMLOGIC_RESCUEPARTY_REBOOT:
	case AMLOGIC_KERNEL_PANIC:
	case AMLOGIC_RECOVERY_QUIESCENT_REBOOT:
		reboot_reason = ZBI_HW_REBOOT_WARM;
		break;
	case AMLOGIC_WATCHDOG_REBOOT:
		reboot_reason = ZBI_HW_REBOOT_WATCHDOG;
		break;
	default:
		reboot_reason = ZBI_HW_REBOOT_UNDEFINED;
		break;
	}

	zircon_append_boot_item(zbi, ZBI_TYPE_HW_REBOOT_REASON, 0,
				&reboot_reason, sizeof(reboot_reason));
}

static void add_board_info(zbi_header_t *zbi)
{
	zbi_board_info_t board_info = {};
	char *s = NULL;

	s = env_get("hw_id");
	if (s && (*s != '\0')) {
		uint32_t hw_id = simple_strtoul(s, NULL, 16);

		board_info.revision = hw_id;
	} else {
		board_info.revision = 0x0;
	}

	zircon_append_boot_item(zbi, ZBI_TYPE_DRV_BOARD_INFO, 0,
			&board_info, sizeof(board_info));
}

extern int thermal_cali_data_read(uint32_t type, uint32_t *outbuf, int32_t size);
static void add_tsensor1_trim_info(void)
{
	uint32_t trim_info = 0;

	thermal_cali_data_read(1, &trim_info, 4);
	// sticky reg2,7 used
	writel(trim_info, SYSCTRL_STICKY_REG3);
}

static int hex_digit(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	else if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	else if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	else
		return -1;
}

static void add_eth_mac_address(zbi_header_t *zbi)
{
	char *str = env_get("ethaddr");
	uint8_t addr[6];

	int i;

	for (i = 0; i < 6; i++) {
		unsigned left, right;

		left = hex_digit(*str++);
		right = hex_digit(*str++);
		if (str[0] && str[1] && left >= 0 && right >= 0)
			addr[i] = (left << 4) | right;
		else
			goto failed;

		if (i < 5 && *str++ != ':')
			goto failed;
	}

	zircon_append_boot_item(zbi, ZBI_TYPE_DRV_MAC_ADDRESS, 0, addr, sizeof(addr));

	printf("set MAC address to \"%s\"\n", addr);
	return;

failed:
	printf("MAC address parsing failed for \"%s\"\n", env_get("ethaddr"));
}

int zircon_preboot(zbi_header_t *zbi)
{
	// allocate crashlog save area before 0x5f800000-0x60000000 reserved area
	zbi_nvram_t nvram;

	nvram.base = 0x3f800000 - NVRAM_LENGTH;
	nvram.length = NVRAM_LENGTH;
	zircon_append_boot_item(zbi, ZBI_TYPE_NVRAM, 0, &nvram, sizeof(nvram));

	zircon_append_boot_item(zbi, ZBI_TYPE_CMDLINE, 0, CMDLINE_DEBUG,
			strlen(CMDLINE_DEBUG) + 1);
	// add memory configuration
	zircon_append_boot_item(zbi, ZBI_TYPE_MEM_CONFIG, 0, &mem_config, sizeof(mem_config));
	// add kernel drivers
	zircon_append_boot_item(zbi, ZBI_TYPE_KERNEL_DRIVER, KDRV_AMLOGIC_UART, &uart_driver,
			sizeof(uart_driver));
	zircon_append_boot_item(zbi, ZBI_TYPE_KERNEL_DRIVER, KDRV_ARM_GIC_V2, &gicv2_driver,
			sizeof(gicv2_driver));
	zircon_append_boot_item(zbi, ZBI_TYPE_KERNEL_DRIVER, KDRV_ARM_PSCI, &psci_driver,
			sizeof(psci_driver));
	zircon_append_boot_item(zbi, ZBI_TYPE_KERNEL_DRIVER, KDRV_ARM_GENERIC_TIMER, &timer_driver,
			sizeof(timer_driver));
	zircon_append_boot_item(zbi, ZBI_TYPE_CMDLINE, 0, BOOTLOADER_VERSION,
			strlen(BOOTLOADER_VERSION) + 1);
	// add platform ID
	add_board_info(zbi);
	// add tsensor trim info
	add_tsensor1_trim_info();
	zircon_append_boot_item(zbi, ZBI_TYPE_PLATFORM_ID, 0, &platform_id, sizeof(platform_id));
	add_partition_map(zbi);
	add_cpu_topology(zbi);
	add_reboot_reason(zbi);
	add_eth_mac_address(zbi);
	return 0;
}
