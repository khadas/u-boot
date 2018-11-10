// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * SPI-NAND driver to storage interface module
 *
 * Copyright (C) 2018 Amlogic Corporation
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <dm.h>
#include <spi.h>
#include <dm/device-internal.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
#include <amlogic/storage.h>
#include <amlogic/aml_mtd.h>

static struct storage_t *snand_storage;

extern void mtd_store_init_map(void);
extern void mtd_store_set(struct mtd_info *mtd, int dev);
extern void mtd_store_mount_ops(struct storage_t *store);

static inline void set_snand_storage(struct storage_t *snand)
{
	snand_storage = snand;
}

static inline struct storage_t *get_snand_storage(void)
{
	return snand_storage;
}

int spinand_fit_storage(struct nand_chip *chip)
{
	struct storage_t *spi_nand = NULL;
	struct mtd_info *mtd = &chip->mtd;

	if (get_snand_storage())
		return 0;
	spi_nand = kzalloc(sizeof(*spi_nand), GFP_KERNEL);
	if (!spi_nand) {
		debug("%s %d no enough memory!\n", __func__, __LINE__);
		return -ENOMEM;
	}
	spi_nand->type = BOOT_SNAND;
	spi_nand->init_flag = 0;
	/* TODO:set name and id parameter */
	memcpy(spi_nand->info.name,
	       "test spi-nand", strlen("test spi-nand"));
	memset(spi_nand->info.id, 0x5a, NAND_MAX_ID_LEN);
	spi_nand->info.read_unit = mtd->writesize;
	spi_nand->info.write_unit = mtd->writesize;
	spi_nand->info.erase_unit = mtd->erasesize;
	spi_nand->info.caps = mtd->size;
#ifdef CONFIG_DISCRETE_BOOTLOADER
	spi_nand->info.mode = 1;
#else
	spi_nand->info.mode = 0;
#endif
	set_snand_storage(spi_nand);
	mtd_store_mount_ops(spi_nand);
	mtd_store_set(mtd, 0);
	mtd_store_init_map();
	return store_register(spi_nand);
}

int spi_nand_pre(void)
{
	return 0;
}

int spi_nand_probe(u32 init_flag)
{
	u32 bus = 0, cs = 0, speed = 40000000, mode = 0;
	struct spi_slave *slave;
	struct udevice *dev;
	int ret;
	char name[30], *str;
	struct storage_t *spi_nand = get_snand_storage();

	if (spi_nand) {
		spi_nand->init_flag = init_flag;
		return 0;
	}
	snprintf(name, sizeof(name), "spinand@%d:%d", bus, cs);
	str = strdup(name);
	ret = spi_get_bus_and_cs(bus, cs, speed, mode,
				 "spinand", str, &dev, &slave);
	if (ret) {
		printf("%s %d probe spi nand fail!\n",
		       __func__, __LINE__);
		return ret;
	}
	spi_nand = get_snand_storage();
	if (!spi_nand) {
		printf("%s %d can not get spi nand!\n",
		       __func__, __LINE__);
		return 1;
	}
	spi_nand->init_flag = init_flag;
	return 0;
}
