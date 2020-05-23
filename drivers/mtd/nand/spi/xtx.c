// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __UBOOT__
#include <linux/device.h>
#include <linux/kernel.h>
#endif
#include <linux/mtd/spinand.h>

#define SPINAND_MFR_XTX			0x0b
#define ZETTA_STATUS_ECC_NO_BITFLIPS	(0 << 2)
#define ZETTA_STATUS_ECC_1_BITFLIPS		(1 << 2)
#define ZETTA_STATUS_ECC_2_BITFLIPS		(2 << 2)
#define ZETTA_STATUS_ECC_3_BITFLIPS		(3 << 2)
#define ZETTA_STATUS_ECC_4_BITFLIPS		(4 << 2)
#define ZETTA_STATUS_ECC_5_BITFLIPS		(5 << 2)
#define ZETTA_STATUS_ECC_6_BITFLIPS		(6 << 2)
#define ZETTA_STATUS_ECC_7_BITFLIPS		(7 << 2)
#define ZETTA_STATUS_ECC_ECC_UNCOR_ERROR		(8 << 2)
#define ZETTA_STATUS_ECC_8_BITFLIPS		(0xc << 2)
#define ZETTA_STATUS_ECC_MASK	GENMASK(5, 2)

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

static int ZXT26G01AWSEGA_ooblayout_ecc(struct mtd_info *mtd, int section,
				  struct mtd_oob_region *region)
{
	if (section)
		return -ERANGE;

	region->offset = 48;
	region->length = 16;

	return 0;

}

static int ZXT26G01AWSEGA_ooblayout_free(struct mtd_info *mtd, int section,
				   struct mtd_oob_region *region)
{
	if (section)
		return -ERANGE;

	/* Reserve 2 bytes for the BBM. */
	region->offset = 2;
	region->length = 46;

	return 0;
}

static const struct mtd_ooblayout_ops ZXT26G01AWSEGA_ooblayout = {
	.ecc = ZXT26G01AWSEGA_ooblayout_ecc,
	.free = ZXT26G01AWSEGA_ooblayout_free,
};

static int ZXT26G01AWSEGA_ecc_get_status(struct spinand_device *spinand,
				   u8 status)
{
	switch (status & ZETTA_STATUS_ECC_MASK) {
	case ZETTA_STATUS_ECC_NO_BITFLIPS:
		return 0;

	case ZETTA_STATUS_ECC_1_BITFLIPS:
		return 1;

	case ZETTA_STATUS_ECC_2_BITFLIPS:
		return 2;

	case ZETTA_STATUS_ECC_3_BITFLIPS:
		return 3;

	case ZETTA_STATUS_ECC_4_BITFLIPS:
		return 4;

	case ZETTA_STATUS_ECC_5_BITFLIPS:
		return 5;

	case ZETTA_STATUS_ECC_6_BITFLIPS:
		return 6;

	case ZETTA_STATUS_ECC_7_BITFLIPS:
		return 7;

	case ZETTA_STATUS_ECC_8_BITFLIPS:
		return 8;

	case ZETTA_STATUS_ECC_ECC_UNCOR_ERROR:
		return -EBADMSG;

	default:
		break;
	}

	return -EINVAL;
}

static const struct spinand_info xtx_spinand_table[] = {
	SPINAND_INFO("XT26G01AWSEGA 3.3V", 0xe1,
		     NAND_MEMORG(1, 2048, 64, 64, 1024, 1, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&ZXT26G01AWSEGA_ooblayout,
				     ZXT26G01AWSEGA_ecc_get_status)),

	SPINAND_INFO("XT26G02AWSEGA 3.3V", 0xe2,
		     NAND_MEMORG(1, 2048, 64, 64, 2048, 1, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&ZXT26G01AWSEGA_ooblayout,
				     ZXT26G01AWSEGA_ecc_get_status)),
};

static int xtx_spinand_detect(struct spinand_device *spinand)
{
	u8 *id = spinand->id.data;
	int ret;

	if (id[0] != SPINAND_MFR_XTX)
		return 0;

	ret = spinand_match_and_init(spinand, xtx_spinand_table,
				     ARRAY_SIZE(xtx_spinand_table),
				     id[1]);
	if (ret)
		return ret;

	return 1;
}

static const struct spinand_manufacturer_ops xtx_spinand_manuf_ops = {
	.detect = xtx_spinand_detect,
};

const struct spinand_manufacturer xtx_spinand_manufacturer = {
	.id = SPINAND_MFR_XTX,
	.name = "xtx",
	.ops = &xtx_spinand_manuf_ops,
};
