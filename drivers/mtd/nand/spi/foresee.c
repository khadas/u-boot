// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2018 Stefan Roese <sr@denx.de>
 *
 * Derived from drivers/mtd/nand/spi/micron.c
 *   Copyright (c) 2016-2017 Micron Technology, Inc.
 */

#ifndef __UBOOT__
#include <linux/device.h>
#include <linux/kernel.h>
#endif
#include <linux/mtd/spinand.h>

#define SPINAND_MFR_FORESEE			0xcd

#define FORESEE_STATUS_ECC_MASK		GENMASK(5, 4)
#define FORESEE_STATUS_ECC_NO_BITFLIPS	(0 << 4)
#define FORESEE_STATUS_ECC_1_BITFLIPS	(1 << 4)
#define FORESEE_STATUS_ECC_UNCOR_ERROR	(2 << 4)
#define FORESEE_STATUS_ECC_UNCOR_ERROR1	(3 << 4)

static SPINAND_OP_VARIANTS(read_cache_variants,
		//SPINAND_PAGE_READ_FROM_CACHE_QUADIO_OP(0, 2, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_X4_OP(0, 1, NULL, 0),
		//SPINAND_PAGE_READ_FROM_CACHE_DUALIO_OP(0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_X2_OP(0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_OP(true, 0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_OP(false, 0, 1, NULL, 0));

static SPINAND_OP_VARIANTS(write_cache_variants,
		SPINAND_PROG_LOAD_X4(true, 0, NULL, 0),
		SPINAND_PROG_LOAD(true, 0, NULL, 0));

static SPINAND_OP_VARIANTS(update_cache_variants,
		SPINAND_PROG_LOAD_X4(false, 0, NULL, 0),
		SPINAND_PROG_LOAD(false, 0, NULL, 0));

static int f35sqa002g_ooblayout_ecc(struct mtd_info *mtd, int section,
					 struct mtd_oob_region *region)
{
	/* Unable to know the layout of ECC */
	return -ERANGE;
}

static int f35sqa002g_ooblayout_free(struct mtd_info *mtd, int section,
					  struct mtd_oob_region *region)
{
	if (section)
		return -ERANGE;

	/* Reserve 2 bytes for the BBM. */
	region->offset = 2;
	region->length = 62;

	return 0;
}

static const struct mtd_ooblayout_ops f35sqa002g_ooblayout = {
	.ecc = f35sqa002g_ooblayout_ecc,
	.free = f35sqa002g_ooblayout_free,
};

static int f35sqa002g_ecc_get_status(struct spinand_device *spinand,
					  u8 status)
{
	switch (status & STATUS_ECC_MASK) {
	case FORESEE_STATUS_ECC_NO_BITFLIPS:
		return 0;

	case FORESEE_STATUS_ECC_1_BITFLIPS:
		/*
		 * We have no way to know exactly how many bitflips have been
		 * fixed, so let's return the maximum possible value so that
		 * wear-leveling layers move the data immediately.
		 */
		return 1;

	case FORESEE_STATUS_ECC_UNCOR_ERROR:
	case FORESEE_STATUS_ECC_UNCOR_ERROR1:
		return -EBADMSG;

	default:
		break;
	}

	return -EINVAL;
}

static const struct spinand_info foresee_spinand_table[] = {
	SPINAND_INFO("F35SQA002G 3.3v", 0x72,
		     NAND_MEMORG(1, 2048, 64, 64, 2048, 1, 1, 1),
		     NAND_ECCREQ(1, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&f35sqa002g_ooblayout,
				     f35sqa002g_ecc_get_status)),
};

static int foresee_spinand_detect(struct spinand_device *spinand)
{
	u8 *id = spinand->id.data;
	int ret;

	if (id[0] != SPINAND_MFR_FORESEE)
		return 0;

	ret = spinand_match_and_init(spinand, foresee_spinand_table,
				     ARRAY_SIZE(foresee_spinand_table),
				     id[1]);
	if (ret)
		return ret;

	return 1;
}

static const struct spinand_manufacturer_ops foresee_spinand_manuf_ops = {
	.detect = foresee_spinand_detect,
};

const struct spinand_manufacturer foresee_spinand_manufacturer = {
	.id = SPINAND_MFR_FORESEE,
	.name = "foresee",
	.ops = &foresee_spinand_manuf_ops,
};
