// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <amlogic/storage.h>
#include <amlogic/aml_mtd.h>
#include <asm/arch/cpu_config.h>

extern int spi_nand_probe(u32 init_flag);
static int do_spinand(cmd_tbl_t *cmdtp,
		      int flag, int argc, char * const argv[])
{
	u32 init_flag = 0;
	int ret = 0;
	ulong addr;
	size_t size;

	if (argc != 3 && argc != 4)
		return CMD_RET_USAGE;

	if (!strcmp(argv[1], "init")) {
		init_flag = (ulong)simple_strtoul(argv[2], NULL, 16);
		return spi_nand_probe(init_flag);
	} else if (!strcmp(argv[1], "update")) {
		addr = (ulong)simple_strtoul(argv[2], NULL, 16);
		size = (size_t)simple_strtoul(argv[3], NULL, 16);
		if (store_device_valid(BOOT_SNAND)) {
			ret = store_set_device(BOOT_SNAND);
			if (!ret) {
			if (store_get_device_bootloader_mode() == DISCRETE_BOOTLOADER) {
				ret = store_boot_erase(BOOT_BL2, BOOT_OPS_ALL);
				if (ret) {
					pr_info("%s %d erase failed in %s",
						__func__, __LINE__, BOOT_BL2);
					return ret;
				}
				ret = store_boot_erase(BOOT_TPL, BOOT_OPS_ALL);
				if (ret) {
					pr_info("%s %d erase failed in %s",
						__func__, __LINE__, BOOT_TPL);
					return ret;
				}
				ret = store_boot_write(BOOT_BL2, BOOT_OPS_ALL,
						       size, (void *)addr);
				if (ret) {
					pr_info("%s %d write failed in %s",
						__func__, __LINE__, BOOT_BL2);
					return ret;
				}
				return store_boot_write(BOOT_TPL, BOOT_OPS_ALL,
						size - BL2_SIZE,
						(void *)(addr + BL2_SIZE));
				} else {
				ret = store_boot_erase(BOOT_LOADER,
						       BOOT_OPS_ALL);
				if (ret) {
					pr_info("%s %d erase failed in %s",
						__func__,
						__LINE__,
						BOOT_LOADER);
					return ret;
				}
				return store_boot_write(BOOT_LOADER,
					    BOOT_OPS_ALL,
						size, (void *)addr);
			}
			}
		} else {
			pr_info("%s %d no valid spi nand\n",
				__func__, __LINE__);
			return 1;
		}
	} else {
		return CMD_RET_USAGE;
	}
	return ret;
}

U_BOOT_CMD(spinand, CONFIG_SYS_MAXARGS, 1, do_spinand,
	   "spinand sub-system",
	"init [flag]\n"
	"	init spi nand with flag\n"
	"spinand update addr size\n"
	"	update bootloader in spi nand\n"
);
