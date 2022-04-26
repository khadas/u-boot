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
#include <amlogic/storage.h>
#include "sf_internal.h"

struct storage_t *snor_storage;
static struct spi_flash *spi_flash;


extern const struct spi_flash_info *spi_flash_read_id(struct spi_flash *flash);
extern void mtd_store_set(struct mtd_info *mtd, int dev);
extern void mtd_store_mount_ops(struct storage_t *store);
extern struct mtd_info *spi_flash_get_mtd(void);

inline void set_snor_storage(struct storage_t *snor)
{
	snor_storage = snor;
}

inline struct storage_t *get_snor_storage(void)
{
	return snor_storage;
}

void set_spi_flash(struct spi_flash *snor)
{
	spi_flash = snor;
}

struct spi_flash *get_spi_flash(void)
{
	return spi_flash;
}

int spi_flash_fit_storage(struct spi_flash *flash)
{
	const struct spi_flash_info *info = NULL;
	struct storage_t *spi_nor = NULL;
	int ret = 0;

	if (get_snor_storage())
		return 0;
	info = spi_flash_read_id(flash);
	if (IS_ERR_OR_NULL(info)) {
		debug("%s %d no matched flash\n", __func__, __LINE__);
		return 1;
	}
	spi_nor = kzalloc(sizeof(*spi_nor), GFP_KERNEL);
	if (!spi_nor) {
		debug("%s %d no enough memory!\n", __func__, __LINE__);
		return -ENOMEM;
	}
	spi_nor->type = BOOT_SNOR;
	spi_nor->init_flag = 0;
	memcpy(spi_nor->info.name, info->name,
	       strlen(info->name) > 32 ? 32 : strlen(info->name));
	memcpy(spi_nor->info.id, info->id, info->id_len);
	spi_nor->info.read_unit = flash->page_size;
	spi_nor->info.write_unit = flash->page_size;
	spi_nor->info.erase_unit = flash->erase_size;
	spi_nor->info.caps = flash->size;
	spi_nor->info.mode = BOOTLOADER_MODE_SNOR;
	set_snor_storage(spi_nor);
	mtd_store_mount_ops(spi_nor);
	ret = store_register(spi_nor);
	if (ret)
		return ret;
	mtd_store_set(spi_flash_get_mtd(), 0);
	return ret;
}

int spi_nor_pre(void)
{
	u32 bus = 0, cs = 0, speed = 0, mode = 0;
	struct storage_t *spi_nor = get_snor_storage();
	struct spi_flash *flash = NULL;

	if (spi_nor)
		return 0;

	flash = spi_flash_probe(bus, cs, speed, mode);
	if (!flash) {
		debug("spi flash probe fail!\n");
		return 1;
	}

	set_spi_flash(flash);

	printf("storage-sf mode 0x%x, speed %dHz\n", flash->spi->mode,
		flash->spi->max_hz);

	return 0;
}

int spi_nor_probe(u32 init_flag)
{
	static int probe_flag;
	struct storage_t *spi_nor = NULL;
	struct spi_flash *flash = NULL;
	int ret;

	flash = (struct spi_flash *)get_spi_flash();
	if (!flash) {
		printf("get spi flash fail!\n");
		return 1;
	}

	/* Maybe pinmux be modified by emmc, set again here */
	extern int pinctrl_select_state(struct udevice *dev, const char *statename);
	ret = pinctrl_select_state(flash->spi->dev->parent, "default");
	if (ret) {
		pr_err("select state %s failed\n", "default");
		return 1;
	}

	if (probe_flag)
		return 0;

	ret = spi_flash_mtd_register(flash);
	if (ret) {
		printf("spi flash mtd register fail!\n");
		return 1;
	}

	spi_nor = get_snor_storage();
	if (!spi_nor) {
		printf("can not get spi nor!\n");
		return 1;
	}

	spi_nor->init_flag = init_flag;
	probe_flag = 1;

	return 0;
}
