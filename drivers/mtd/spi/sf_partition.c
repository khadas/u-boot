// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <malloc.h>
#include <spi.h>
#include <spi_flash.h>
#include <linux/mtd/partitions.h>
#include <linux/types.h>
#include <linux/sizes.h>
#include <malloc.h>
#include <linux/errno.h>
#include <mtd.h>
#include <amlogic/aml_mtd.h>
#include <amlogic/storage.h>

/* Hard code, all partitions are aligned in block size, fast erasing */
#define SPINOR_ALIGNED_SIZE		(64 * 1024)
extern struct storage_startup_parameter g_ssp;

/* do not use default value, rewrite this function in the board file */
uint64_t __weak spiflash_bootloader_size(void)
{
	return SZ_2M;
}

uint32_t __weak spiflash_rsv_block_num(void)
{
	return 0;
}

extern boot_area_entry_t general_boot_part_entry[MAX_BOOT_AREA_ENTRIES];
/* The size of the partition must be block aligned */
static int _spinor_add_partitions(struct mtd_info *mtd,
				  const struct mtd_partition *parts,
				  int nbparts)
{
	int part_num = 0, i = 0;
	struct mtd_partition *temp, *parts_nm;
	loff_t off;

	if (store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER)
		part_num = nbparts + 5;
	else
		part_num = nbparts + 1;

	temp = kzalloc(sizeof(*temp) * part_num, GFP_KERNEL);
	memset(temp, 0, sizeof(*temp) * part_num);
	if (store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER) {
		temp[BOOT_AREA_BB1ST].name = BOOT_LOADER;
		temp[BOOT_AREA_BB1ST].offset = general_boot_part_entry[BOOT_AREA_BB1ST].offset;
		temp[BOOT_AREA_BB1ST].size = general_boot_part_entry[BOOT_AREA_BB1ST].size * g_ssp.boot_bakups;
		if (temp[BOOT_AREA_BB1ST].size % SPINOR_ALIGNED_SIZE)
			WARN_ON(1);

		temp[BOOT_AREA_BL2E].name = BOOT_BL2E;
		temp[BOOT_AREA_BL2E].offset = general_boot_part_entry[BOOT_AREA_BL2E].offset;
		temp[BOOT_AREA_BL2E].size = general_boot_part_entry[BOOT_AREA_BL2E].size * g_ssp.boot_bakups;
		if (temp[0].size % SPINOR_ALIGNED_SIZE)
			WARN_ON(1);

		temp[BOOT_AREA_BL2X].name = BOOT_BL2X;
		temp[BOOT_AREA_BL2X].offset = general_boot_part_entry[BOOT_AREA_BL2X].offset;
		temp[BOOT_AREA_BL2X].size = general_boot_part_entry[BOOT_AREA_BL2X].size * g_ssp.boot_bakups;
		if (temp[0].size % SPINOR_ALIGNED_SIZE)
			WARN_ON(1);

		temp[BOOT_AREA_DDRFIP].name = BOOT_DDRFIP;
		temp[BOOT_AREA_DDRFIP].offset = general_boot_part_entry[BOOT_AREA_DDRFIP].offset;
		temp[BOOT_AREA_DDRFIP].size = general_boot_part_entry[BOOT_AREA_DDRFIP].size * g_ssp.boot_bakups;
		if (temp[0].size % SPINOR_ALIGNED_SIZE)
			WARN_ON(1);

		temp[BOOT_AREA_DEVFIP].name = BOOT_DEVFIP;
		temp[BOOT_AREA_DEVFIP].offset = general_boot_part_entry[BOOT_AREA_DEVFIP].offset;
		temp[BOOT_AREA_DEVFIP].size = general_boot_part_entry[BOOT_AREA_DEVFIP].size *
			CONFIG_NOR_TPL_COPY_NUM;
		if (temp[0].size % SPINOR_ALIGNED_SIZE)
			WARN_ON(1);

		off = temp[BOOT_AREA_DEVFIP].offset + temp[BOOT_AREA_DEVFIP].size;
		parts_nm = &temp[5];
	} else {
		temp[0].name = BOOT_LOADER;
		temp[0].offset = 0;
		temp[0].size = spiflash_bootloader_size();
		if (temp[0].size % SPINOR_ALIGNED_SIZE)
			WARN_ON(1);
		/* rsv size is aligned with blocksize(64K) */
		off = temp[0].size + spiflash_rsv_block_num() * SPINOR_ALIGNED_SIZE;
		parts_nm = &temp[1];
	}
	for (i = 0; i < nbparts; i++) {
		if (!parts[i].name) {
			pr_err("name can't be null! ");
			pr_err("please check your %d th partition name!\n",
				 i + 1);
			return 1;
		}
		if ((off + parts[i].size) > mtd->size) {
			pr_err("%s %d over nand size!\n",
				__func__, __LINE__);
			return 1;
		}
		parts_nm[i].name = parts[i].name;
		parts_nm[i].offset = off;
		if (parts[i].size % SPINOR_ALIGNED_SIZE) {
			pr_err("%s %d \"%s\" size auto align to block size\n",
				__func__, __LINE__, parts[i].name);
			parts_nm[i].size += parts[i].size % SPINOR_ALIGNED_SIZE;
		}
		/* it's ok "+=" here because size has been set to 0 */
		parts_nm[i].size += parts[i].size;
		off += parts_nm[i].size;
		if (i == (nbparts - 1))
			parts_nm[i].size = mtd->size - off;
	}

	return add_mtd_partitions(mtd, temp, part_num);
}

extern struct mtd_partition *get_spiflash_partition_table(int *partitions);
int spinor_add_partitions(struct mtd_info *mtd)
{
	struct mtd_partition *spiflash_partitions;
	int partition_count;

	spiflash_partitions = get_spiflash_partition_table(&partition_count);

	return _spinor_add_partitions(mtd, spiflash_partitions,
			       partition_count);
}

int spinor_del_partitions(struct mtd_info *mtd)
{
	return del_mtd_partitions(mtd);
}
