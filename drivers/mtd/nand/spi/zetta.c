// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __UBOOT__
#include <linux/device.h>
#include <linux/kernel.h>
#endif
#include <linux/mtd/spinand.h>

#define SPINAND_MFR_ZETTA			0xba

static SPINAND_OP_VARIANTS(read_cache_variants,
		SPINAND_PAGE_READ_FROM_CACHE_X4_OP(0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_X2_OP(0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_OP(true, 0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_OP(false, 0, 1, NULL, 0));

static SPINAND_OP_VARIANTS(write_cache_variants,
		SPINAND_PROG_LOAD_X4(true, 0, NULL, 0),
		SPINAND_PROG_LOAD(true, 0, NULL, 0));

static SPINAND_OP_VARIANTS(update_cache_variants,
		//SPINAND_PROG_LOAD_X4(false, 0, NULL, 0),
		SPINAND_PROG_LOAD(false, 0, NULL, 0));

static int ZD35Q1GA_ooblayout_ecc(struct mtd_info *mtd, int section,
				  struct mtd_oob_region *region)
{
	/* the ECC parity code can be calculated
	 * properly and stored in the additional
	 * hidden spare area
	 */
	return -ERANGE;
}

static int ZD35Q1GA_ooblayout_free(struct mtd_info *mtd, int section,
				   struct mtd_oob_region *region)
{
	if (section)
		return -ERANGE;

	/* Reserve 2 bytes for the BBM. */
	region->offset = 2;
	region->length = 62;

	return 0;
}

static const struct mtd_ooblayout_ops ZD35Q1GA_ooblayout = {
	.ecc = ZD35Q1GA_ooblayout_ecc,
	.free = ZD35Q1GA_ooblayout_free,
};

static int ZD35Q1GA_ecc_get_status(struct spinand_device *spinand,
				   u8 status)
{
	switch (status & STATUS_ECC_MASK) {
	case STATUS_ECC_NO_BITFLIPS:
		return 0;

	case STATUS_ECC_HAS_BITFLIPS:
		/*
		 * We have no way to know exactly how many bitflips have been
		 * fixed, so let's return the maximum possible value so that
		 * wear-leveling layers move the data immediately.
		 */
		return 4;

	case STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;

	default:
		break;
	}

	return -EINVAL;
}

static const struct spinand_info zetta_spinand_table[] = {
	SPINAND_INFO("ZD35Q1GA 3.3V", 0x71,
		     NAND_MEMORG(1, 2048, 64, 64, 1024, 1, 1, 1),
		     NAND_ECCREQ(4, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&ZD35Q1GA_ooblayout,
				     ZD35Q1GA_ecc_get_status)),
#if 0
	SPINAND_INFO("ZD35M1GA 1.8V", 0x21,
		     NAND_MEMORG(1, 2048, 64, 64, 1024, 1, 1, 1),
		     NAND_ECCREQ(4, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&ZD35Q1GA_ooblayout,
				     ZD35Q1GA_ecc_get_status)),
#endif
	SPINAND_INFO("ZD35M2GA 3.3V", 0x72,
		     NAND_MEMORG(1, 2048, 64, 64, 2048, 2, 1, 1),
		     NAND_ECCREQ(4, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&ZD35Q1GA_ooblayout,
				     ZD35Q1GA_ecc_get_status)),
};

static int zetta_spinand_detect(struct spinand_device *spinand)
{
	u8 *id = spinand->id.data;
	int ret;

	if (id[0] != SPINAND_MFR_ZETTA)
		return 0;

	ret = spinand_match_and_init(spinand, zetta_spinand_table,
				     ARRAY_SIZE(zetta_spinand_table),
				     id[1]);
	if (ret)
		return ret;

	return 1;
}

static const struct spinand_manufacturer_ops zetta_spinand_manuf_ops = {
	.detect = zetta_spinand_detect,
};

const struct spinand_manufacturer zetta_spinand_manufacturer = {
	.id = SPINAND_MFR_ZETTA,
	.name = "Zetta",
	.ops = &zetta_spinand_manuf_ops,
};
