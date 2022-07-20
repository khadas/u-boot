// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __UBOOT__
#include <linux/device.h>
#include <linux/kernel.h>
#endif
#include <linux/mtd/spinand.h>

#define SPINAND_MFR_TOSHIBA		0x98

/* Bit errors were detected and corrected.
 * Bit error count exceeded the bit flip detection threshold.
 * The threshold is set by bits [7:4] in address 10h in the feature table
 */
#define TOSHIBA_STATUS_ECC_8_BITFLIPS	(3 << 4)

static SPINAND_OP_VARIANTS(read_cache_variants,
		SPINAND_PAGE_READ_FROM_CACHE_X4_OP(0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_X2_OP(0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_OP(true, 0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_OP(false, 0, 1, NULL, 0));

static SPINAND_OP_VARIANTS(write_cache_variants,
		//SPINAND_PROG_LOAD_X4(true, 0, NULL, 0),
		SPINAND_PROG_LOAD(true, 0, NULL, 0));

static SPINAND_OP_VARIANTS(update_cache_variants,
		SPINAND_PROG_LOAD_X4(false, 0, NULL, 0),
		SPINAND_PROG_LOAD(false, 0, NULL, 0));

static int tc58cvg0s3hraig_ooblayout_ecc(struct mtd_info *mtd, int section,
					 struct mtd_oob_region *region)
{
	/* Unable to know the layout of ECC */
	return -ERANGE;
}

static int tc58cvg0s3hraig_ooblayout_free(struct mtd_info *mtd, int section,
					  struct mtd_oob_region *region)
{
	if (section)
		return -ERANGE;

	/* Reserve 2 bytes for the BBM. */
	region->offset = 2;
	region->length = 62;

	return 0;
}

static const struct mtd_ooblayout_ops tc58cvg0s3hraig_ooblayout = {
	.ecc = tc58cvg0s3hraig_ooblayout_ecc,
	.free = tc58cvg0s3hraig_ooblayout_free,
};

static int tc58cvg0s3hraig_ecc_get_status(struct spinand_device *spinand,
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
		return 7;

	case TOSHIBA_STATUS_ECC_8_BITFLIPS:
		/*
		 * We have no way to know exactly how many bitflips have been
		 * fixed, so let's return the maximum possible value so that
		 * wear-leveling layers move the data immediately.
		 */
		return 8;

	case STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;

	default:
		break;
	}

	return -EINVAL;
}

static const struct spinand_info toshiba_spinand_table[] = {
	SPINAND_INFO("TC58CVG0S3HRAIG 3.3V", 0xc2,
		     NAND_MEMORG(1, 2048, 128, 64, 1024, 1, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     0,
		     SPINAND_ECCINFO(&tc58cvg0s3hraig_ooblayout,
				     tc58cvg0s3hraig_ecc_get_status)),

	SPINAND_INFO("TC58CYG1S3HRAIG 1.8V", 0xdb,
		     NAND_MEMORG(1, 2048, 128, 64, 2048, 1, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     0,
		     SPINAND_ECCINFO(&tc58cvg0s3hraig_ooblayout,
				     tc58cvg0s3hraig_ecc_get_status)),

	SPINAND_INFO("TC58CVG1S3HRAIG 3.3V", 0xcb,
		     NAND_MEMORG(1, 2048, 128, 64, 2048, 1, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     0,
		     SPINAND_ECCINFO(&tc58cvg0s3hraig_ooblayout,
				     tc58cvg0s3hraig_ecc_get_status)),

	SPINAND_INFO("TC58CVG1S3HRAIJ 3.3V", 0xeb,
		     NAND_MEMORG(1, 2048, 128, 64, 2048, 1, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&tc58cvg0s3hraig_ooblayout,
				     tc58cvg0s3hraig_ecc_get_status)),

	SPINAND_INFO("TC58CVG2S0HRAIG 3.3V", 0xcd,
			 NAND_MEMORG(1, 4096, 128, 64, 2048, 1, 1, 1),
			 NAND_ECCREQ(8, 512),
			 SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
						  &write_cache_variants,
						  &update_cache_variants),
			 SPINAND_HAS_QE_BIT,
			 SPINAND_ECCINFO(&tc58cvg0s3hraig_ooblayout,
					 tc58cvg0s3hraig_ecc_get_status)),

	SPINAND_INFO("TC58CVG2S0HRAIJ 3.3V", 0xed,
			 NAND_MEMORG(1, 4096, 128, 64, 2048, 1, 1, 1),
			 NAND_ECCREQ(8, 512),
			 SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
						  &write_cache_variants,
						  &update_cache_variants),
			 0,
			 SPINAND_ECCINFO(&tc58cvg0s3hraig_ooblayout,
					 tc58cvg0s3hraig_ecc_get_status)),
};

static int toshiba_spinand_detect(struct spinand_device *spinand)
{
	u8 *id = spinand->id.data;
	int ret;

	if (id[0] != SPINAND_MFR_TOSHIBA)
		return 0;

	ret = spinand_match_and_init(spinand, toshiba_spinand_table,
				     ARRAY_SIZE(toshiba_spinand_table),
				     id[1]);
	if (ret)
		return ret;

	return 1;
}

static const struct spinand_manufacturer_ops toshiba_spinand_manuf_ops = {
	.detect = toshiba_spinand_detect,
};

const struct spinand_manufacturer toshiba_spinand_manufacturer = {
	.id = SPINAND_MFR_TOSHIBA,
	.name = "Toshiba",
	.ops = &toshiba_spinand_manuf_ops,
};
