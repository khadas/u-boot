// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <spi.h>
//#include <nand.h>
#include <dm/device-internal.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
#include <linux/mtd/partitions.h>
#include <amlogic/storage.h>
#include <amlogic/aml_mtd.h>
#include <linux/mtd/spinand.h>
#include <dm/pinctrl.h>


static struct storage_t *snand_storage;
extern void mtd_store_set(struct mtd_info *mtd, int dev);
extern void mtd_store_mount_ops(struct storage_t *store);
extern int spinand_add_partitions(struct mtd_info *mtd,
				  const struct mtd_partition *parts,
				  int nbparts);

static inline void set_snand_storage(struct storage_t *snand)
{
	snand_storage = snand;
}

static inline struct storage_t *get_snand_storage(void)
{
	return snand_storage;
}

int board_nand_init(void)
{
	u32 bus = 0, cs = 1, speed = 0, mode = 0;
	struct spi_slave *slave;
	struct udevice *dev;
	int ret;

	ret = spi_get_bus_and_cs(bus, cs, speed, mode,
				 NULL, NULL, &dev, &slave);
	if (ret) {
		printf("%s %d probe spi nand fail!\n",
		       __func__, __LINE__);
		return 1;
	}

	return 0;
}

int spinand_fit_storage(struct mtd_info *info, char *name, u8 *id)
{
	struct storage_t *spi_nand = NULL;
	struct mtd_info *mtd = info;
	int ret = 0;

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
	       name, strlen(name) > 32 ? 32 : strlen(name));
	memcpy(spi_nand->info.id, id, NAND_MAX_ID_LEN);
	spi_nand->info.read_unit = mtd->writesize;
	spi_nand->info.write_unit = mtd->writesize;
	spi_nand->info.erase_unit = mtd->erasesize;
	spi_nand->info.caps = mtd->size;
	spi_nand->info.mode = BOOTLOADER_MODE_SNAND;

	set_snand_storage(spi_nand);
	mtd_store_mount_ops(spi_nand);
	ret = store_register(spi_nand);
	if (ret)
		return ret;
	mtd_store_set(mtd, 0);
	return ret;
}

int spi_nand_pre(void)
{
	struct storage_t *spi_nand = get_snand_storage();

	if (spi_nand)
		return 0;

	return board_nand_init();
}

int spi_nand_probe(u32 init_flag)
{
	struct spinand_device *spinand_dev;
	struct storage_t *spi_nand = get_snand_storage();
	const struct mtd_partition *spinand_partitions;
	struct mtd_info *mtd;
	int partition_count, ret;
	static int probe_flag;

	/* Maybe pinmux be modified by emmc, set again here */
	extern struct mtd_info *mtd_store_get(int dev);
	mtd = mtd_store_get(0);
	spinand_dev = mtd_to_spinand(mtd);
	//dm_spi_claim_bus(spinand_dev->slave->dev);
	ret = pinctrl_select_state(spinand_dev->slave->dev->parent, "default");
	if (ret) {
		pr_err("select state %s failed\n", "default");
		return 1;
	}

	if (probe_flag)
		return 0;

#ifdef CONFIG_AML_MTDPART
	extern const struct mtd_partition *get_spinand_partition_table(int *partitions);
	spinand_partitions = get_spinand_partition_table(&partition_count);
	ret = spinand_add_partitions(mtd, spinand_partitions,
						partition_count);
	if (ret) {
		printf("%s %d can not add spinand partition!\n",
		       __func__, __LINE__);
		return 1;
	}
#endif

	spi_nand = get_snand_storage();
	if (!spi_nand) {
		printf("%s %d can not get spi nand!\n",
		       __func__, __LINE__);
		return 1;
	}
	spi_nand->init_flag = init_flag;
	probe_flag = 1;

	return 0;
}
