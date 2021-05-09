// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
#include <amlogic/storage.h>
#include <amlogic/aml_mtd.h>
#include "aml_nand.h"
#include <dm/device.h>

extern void mtd_store_mount_ops(struct storage_t* store);
struct aml_pre_scan *pre_scan;

int nand_pre(void)
{
	int ret = 0;

	pre_scan->pre_scan_flag = 1;
	board_nand_init();
	ret = (pre_scan->is_nand)? 0:1;
	pre_scan->pre_scan_flag = 0;
	return ret;
}

extern struct mtd_info *nand_info[CONFIG_SYS_MAX_NAND_DEVICE];
int slcnand_fit_storage(void)
{
	struct storage_t *slc_nand = NULL;

	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(nand_info[0]);
	struct aml_nand_flash_dev *type = NULL;

	type = aml_chip->platform->nand_flash_dev;

	if (slc_nand == NULL) {
		slc_nand = kzalloc(sizeof(struct storage_t), GFP_KERNEL);
		if (!slc_nand) {
		debug("%s %d no enough memory!\n", __func__, __LINE__);
		return -ENOMEM;
		}
	}

	slc_nand->type = BOOT_NAND_MTD;
	slc_nand->init_flag = 0;
	printf("storage dev type: 0x%x, storage device is slc NAND\n",slc_nand->type);

	memcpy(slc_nand->info.name, type->name, 32 * sizeof(char));
	memcpy(slc_nand->info.id, type->id, 8);
	printf("name: %s\n",slc_nand->info.name);

	slc_nand->info.read_unit = type->pagesize;
	slc_nand->info.write_unit = type->pagesize;
	slc_nand->info.erase_unit = type->erasesize;
	slc_nand->info.caps = ((type->chipsize) << 20);
	printf("cap: 0x%llx\n", slc_nand->info.caps);
	slc_nand->info.mode = BOOTLOADER_MODE_NAND;

	mtd_store_mount_ops(slc_nand);

	return store_register(slc_nand);
}

extern int meson_nfc_probe(struct udevice *dev);
struct udevice *nand_dev;
int nand_probe(uint32_t init_flag)
{

	meson_nfc_probe(nand_dev);

	return 0;
}

