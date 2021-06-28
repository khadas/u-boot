/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * board/amlogic/g12b_w400_v1/zircon.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <aml_i2c.h>
#include <asm/arch/secure_apb.h>
#include <common.h>
#include <part.h>
#include <zircon/zircon.h>

#define PDEV_VID_AMLOGIC            5
#define PDEV_PID_AMLOGIC_A311D      6


#define NVRAM_LENGTH		(8 * 1024)

#define CMDLINE_BUF_SIZE        1024
#define CMDLINE_ENTROPY_BITS        256 // random bits to pass to zircon.

#define ENTROPY_BITS_PER_CHAR       4

static char entropy_cmdline[CMDLINE_BUF_SIZE] = {0};
static const char zircon_entropy_arg[] = "kernel.entropy-mixin=";

#define static_assert _Static_assert
static_assert(CMDLINE_ENTROPY_BITS % 32 == 0,
              "Requested entropy must be a multiple of 32");

static_assert((CMDLINE_ENTROPY_BITS/ENTROPY_BITS_PER_CHAR) \
              + sizeof(zircon_entropy_arg) < CMDLINE_BUF_SIZE, \
              "Requested entropy doesn't fit in cmdline.");

static const char BOOTLOADER_VERSION[] = "zircon-bootloader=0.12";

/** cmdline for debugging */
/*
 * https://fuchsia.dev/fuchsia-src/reference/kernel/kernel_cmdline
 *
 ** kernel.bypass-debuglog=<bool>
 * Default: false
 *
 * When enabled, forces output to the console instead of buffering it.
 * The reason we have both a compile switch and a cmdline parameter is to
 * facilitate prints in the kernel before cmdline is parsed to be forced
 * to go to the console. The compile switch setting overrides the cmdline
 * parameter (if both are present). Note that both the compile switch and
 * the cmdline parameter have the side effect of disabling irq driven uart
 * Tx.
 */

static const char CMDLINE_DEBUG[] = "devmgr.log-to-debuglog=true";


static const zbi_cpu_config_t cpu_config = {
    .cluster_count = 2,
    .clusters = {
        {
            .cpu_count = 2,
        },
        {
            .cpu_count = 4,
        },
    },
};

static zbi_mem_range_t mem_config[] = {
    {
        .type = ZBI_MEM_RANGE_RAM,
        .length = 0x80000000, //2GB
    },
    {
        .type = ZBI_MEM_RANGE_PERIPHERAL,
		.paddr = 0xf5800000,
		.length = 0x0a800000,
    },
    {
        .type = ZBI_MEM_RANGE_RESERVED,
        .paddr = 0x07400000,
        .length = 0x00100000,
    },
    {
        .type = ZBI_MEM_RANGE_RESERVED,
        .paddr = 0x05000000,
        .length = 0x02300000,
    },
    /* linux,usable-memory */
    {
        .type = ZBI_MEM_RANGE_RESERVED,
        .paddr = 0x00000000,
        .length = 0x100000,
    },
};

static const dcfg_simple_t uart_driver = {
    .mmio_phys = 0xff803000,
    .irq = 225,
};

static const dcfg_arm_gicv2_driver_t gicv2_driver = {
    .mmio_phys = 0xffc00000,
    .gicd_offset = 0x1000,
    .gicc_offset = 0x2000,
    .gich_offset = 0x4000,
    .gicv_offset = 0x6000,
    .ipi_base = 0,
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

static const dcfg_amlogic_hdcp_driver_t hdcp_driver = {
    .preset_phys = 0xc1104000,
    .hiu_phys = 0xc883c000,
    .hdmitx_phys = 0xc883a000,
};

static const zbi_platform_id_t platform_id = {
	.vid = PDEV_VID_AMLOGIC,
	.pid = PDEV_PID_AMLOGIC_A311D,
	.board_name = "w400",
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

static void add_board_info(zbi_header_t* zbi)
{
    zbi_board_info_t board_info = {};
    char* s;
    if (((s = getenv("hw_id")) != NULL) && (*s != '\0')) {
                uint32_t hw_id = simple_strtoul(s, NULL, 16);
                board_info.revision = hw_id;
    } else {
        printf("Failed to retrieve Board Revision\n");
    }

    zircon_append_boot_item(zbi, ZBI_TYPE_DRV_BOARD_INFO, 0, &board_info,
                sizeof(board_info));
}

static void* mandatory_memset(void* dst, int c, size_t n) {
    volatile unsigned char* out = dst;
    size_t i = 0;

    for (i = 0; i < n; ++i) {
        out[i] = (unsigned char)c;
    }
    return dst;
}

// Define a cpu topology for the system that captures how all of the cores are
// connected and grouped. Since this is a GICv2 system we include the ID for the
// cores, especially since it is a tricky one with a gap between the little and
// big clusters.
static void add_cpu_topology(zbi_header_t* zbi)
{
    int index = 0;
    int logical_processor = 0;
    int big_cluster = 0, little_cluster = 0;
    zbi_topology_node_t nodes[8];
    little_cluster = index++;
    nodes[little_cluster] = (zbi_topology_node_t){
        .entity_type = ZBI_TOPOLOGY_ENTITY_CLUSTER,
        .parent_index = ZBI_TOPOLOGY_NO_PARENT,
        .entity = {
            .cluster = {
                .performance_class = 0, // low performance cluster
            }
        }
    };
    nodes[index++] = (zbi_topology_node_t){
        .entity_type = ZBI_TOPOLOGY_ENTITY_PROCESSOR,
        .parent_index = little_cluster,
        .entity = {
            .processor = {
                .logical_ids = {logical_processor++},
                .logical_id_count = 1,
                .flags = ZBI_TOPOLOGY_PROCESSOR_PRIMARY,
                .architecture = ZBI_TOPOLOGY_ARCH_ARM,
                .architecture_info = {
                    .arm = {
                        .cluster_1_id = 0,
                        .cpu_id = 0,
                        .gic_id = 0,
                    }
                }
            }
        }
    };
    nodes[index++] = (zbi_topology_node_t){
        .entity_type = ZBI_TOPOLOGY_ENTITY_PROCESSOR,
        .parent_index = little_cluster,
        .entity = {
            .processor = {
                .logical_ids = {logical_processor++},
                .logical_id_count = 1,
                .flags = 0,
                .architecture = ZBI_TOPOLOGY_ARCH_ARM,
                .architecture_info = {
                    .arm = {
                        .cluster_1_id = 0,
                        .cpu_id = 1,
                        .gic_id = 1,
                    }
                }
            }
        }
    };
    big_cluster = index++;
    nodes[big_cluster] = (zbi_topology_node_t){
        .entity_type = ZBI_TOPOLOGY_ENTITY_CLUSTER,
        .parent_index = ZBI_TOPOLOGY_NO_PARENT,
        .entity = {
            .cluster = {
                .performance_class = 1, // high performance cluster
            }
        }
    };
    nodes[index++] = (zbi_topology_node_t){
        .entity_type = ZBI_TOPOLOGY_ENTITY_PROCESSOR,
        .parent_index = big_cluster,
        .entity = {
            .processor = {
                .logical_ids = {logical_processor++},
                .logical_id_count = 1,
                .flags = 0,
                .architecture = ZBI_TOPOLOGY_ARCH_ARM,
                .architecture_info = {
                    .arm = {
                        .cluster_1_id = 1,
                        .cpu_id = 0,
                        .gic_id = 4,
                    }
                }
            }
        }
    };
    nodes[index++] = (zbi_topology_node_t){
        .entity_type = ZBI_TOPOLOGY_ENTITY_PROCESSOR,
        .parent_index = big_cluster,
        .entity = {
            .processor = {
                .logical_ids = {logical_processor++},
                .logical_id_count = 1,
                .flags = 0,
                .architecture = ZBI_TOPOLOGY_ARCH_ARM,
                .architecture_info = {
                    .arm = {
                        .cluster_1_id = 1,
                        .cpu_id = 1,
                        .gic_id = 5,
                    }
                }
            }
        }
    };
    nodes[index++] = (zbi_topology_node_t){
        .entity_type = ZBI_TOPOLOGY_ENTITY_PROCESSOR,
        .parent_index = big_cluster,
        .entity = {
            .processor = {
                .logical_ids = {logical_processor++},
                .logical_id_count = 1,
                .flags = 0,
                .architecture = ZBI_TOPOLOGY_ARCH_ARM,
                .architecture_info = {
                    .arm = {
                        .cluster_1_id = 1,
                        .cpu_id = 2,
                        .gic_id = 6,
                    }
                }
            }
        }
    };
    nodes[index++] = (zbi_topology_node_t){
        .entity_type = ZBI_TOPOLOGY_ENTITY_PROCESSOR,
        .parent_index = big_cluster,
        .entity = {
            .processor = {
                .logical_ids = {logical_processor++},
                .logical_id_count = 1,
                .flags = 0,
                .architecture = ZBI_TOPOLOGY_ARCH_ARM,
                .architecture_info = {
                    .arm = {
                        .cluster_1_id = 1,
                        .cpu_id = 3,
                        .gic_id = 7,
                    }
                }
            }
        }
    };
    zircon_append_boot_item(zbi, ZBI_TYPE_CPU_TOPOLOGY, sizeof(zbi_topology_node_t),
                &nodes, sizeof(zbi_topology_node_t) * index);
}

static void add_partition_map(zbi_header_t* zbi) {
    block_dev_desc_t* dev_desc;
    disk_partition_t bootloader_info;
    disk_partition_t boot_info;
    disk_partition_t misc_info;
    disk_partition_t recovery_info;
    disk_partition_t tee_info;
    disk_partition_t system_info;
    disk_partition_t data_info;

    dev_desc = get_dev("mmc", CONFIG_FASTBOOT_FLASH_MMC_DEV);
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

static int hex_digit(char ch) {
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    } else if (ch >= 'a' && ch <= 'f') {
        return ch - 'a' + 10;
    } else if (ch >= 'A' && ch <= 'F') {
        return ch - 'A' + 10;
    } else {
        return -1;
    }
}

static void add_eth_mac_address(zbi_header_t* zbi) {
    char* str = getenv("eth_mac");
    uint8_t addr[6];

    // this would be easier with sscanf
    int i;
    for (i = 0; i < 6; i++) {
        unsigned left, right;
        if (str[0] && str[1] && (left = hex_digit(*str++)) >= 0 &&
                (right = hex_digit(*str++)) >= 0) {
            addr[i] = (left << 4) | right;
        } else {
            goto failed;
        }
        if (i < 5 && *str++ != ':') {
            goto failed;
        }
    }

    zircon_append_boot_item(zbi, ZBI_TYPE_DRV_MAC_ADDRESS, 0, addr, sizeof(addr));
    return;

failed:
    printf("MAC address parsing failed for \"%s\"\n", getenv("eth_mac"));
}

// fills an 8 char buffer with the lowercase hex representation of the given
// value.
// WARNING this does not add a '\0' to the end of the buffer.
static inline void uint32_to_hex(uint32_t val, char buf[static 8]) {
  static const char hex[] = "0123456789abcdef";
  int i = 0;

  for (i = 7; i >= 0; i--) {
    buf[i] = hex[val & 0xF];
    val >>= 4;
  }
}

// Reads a value from the userspace hardware random number generator.
// NOTE that there is no guarantee of the quality of this rng.
static inline uint32_t read_hw_rng(void) {
  return readl(RNG_USR_DATA);
}

static void add_cmdline_entropy(zbi_header_t* zbi) {
    strcpy(entropy_cmdline, zircon_entropy_arg);

    char *entropy = entropy_cmdline + strlen(zircon_entropy_arg);
    int i = 0;

    for (i = 0; i < CMDLINE_ENTROPY_BITS; i += 32) {
      uint32_to_hex(read_hw_rng(), entropy);
      entropy += 8;
    }
    *entropy = '\0';

    zircon_append_boot_item(zbi, ZBI_TYPE_CMDLINE, 0, entropy_cmdline,
                            sizeof(entropy_cmdline));

    mandatory_memset(entropy_cmdline, '\0', sizeof(entropy_cmdline));
}

int zircon_preboot(zbi_header_t* zbi) {

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
    //zircon_append_boot_item(zbi, ZBI_TYPE_KERNEL_DRIVER, KDRV_AMLOGIC_HDCP, &hdcp_driver,
    //                sizeof(hdcp_driver));

    zircon_append_boot_item(zbi, ZBI_TYPE_CMDLINE, 0, BOOTLOADER_VERSION,
                            strlen(BOOTLOADER_VERSION) + 1);

    zircon_append_boot_item(zbi, ZBI_TYPE_CMDLINE, 0, CMDLINE_DEBUG,
                            strlen(CMDLINE_DEBUG) + 1);

    // add platform ID
    zircon_append_boot_item(zbi, ZBI_TYPE_PLATFORM_ID, 0, &platform_id, sizeof(platform_id));

    add_board_info(zbi);
    add_cmdline_entropy(zbi);
    add_partition_map(zbi);
    add_cpu_topology(zbi);
    add_eth_mac_address(zbi);
    return 0;
 }
