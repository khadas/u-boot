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

#define SPINAND_MFR_GIGADEVICE			0xc8

#define GIGADEVICE_STATUS_ECC_MASK		GENMASK(5, 4)
#define GIGADEVICE_STATUS_ECC_NO_BITFLIPS	(0 << 4)
#define GIGADEVICE_STATUS_ECC_1TO7_BITFLIPS	(1 << 4)
#define GIGADEVICE_STATUS_ECC_8_BITFLIPS	(3 << 4)

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

static int gd5f1gq4u_ooblayout_ecc(struct mtd_info *mtd, int section,
				   struct mtd_oob_region *region)
{
	if (section)
		return -ERANGE;

	region->offset = 64;
	region->length = 64;

	return 0;
}

static int gd5f1gq4u_ooblayout_free(struct mtd_info *mtd, int section,
				    struct mtd_oob_region *region)
{
	if (section)
		return -ERANGE;

	/* Reserve 2 bytes for the BBM. */
	region->offset = 2;
	region->length = 62;

	return 0;
}

static const struct mtd_ooblayout_ops gd5f1gq4u_ooblayout = {
	.ecc = gd5f1gq4u_ooblayout_ecc,
	.free = gd5f1gq4u_ooblayout_free,
};

static int gd5f1gq4u_ecc_get_status(struct spinand_device *spinand,
				    u8 status)
{
	if (status)
		debug("%s (%d): status=%02x\n", __func__, __LINE__, status);

	switch (status & GIGADEVICE_STATUS_ECC_MASK) {
	case STATUS_ECC_NO_BITFLIPS:
		return 0;

	case GIGADEVICE_STATUS_ECC_1TO7_BITFLIPS:
		return 7;

	case GIGADEVICE_STATUS_ECC_8_BITFLIPS:
		return 8;

	case STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;

	default:
		break;
	}

	return -EINVAL;
}

static int gd5f4gq4u_ooblayout_ecc(struct mtd_info *mtd, int section,
				   struct mtd_oob_region *region)
{
	if (section)
		return -ERANGE;

	region->offset = 128;
	region->length = 128;

	return 0;
}

static int gd5f4gq4u_ooblayout_free(struct mtd_info *mtd, int section,
				    struct mtd_oob_region *region)
{
	if (section)
		return -ERANGE;

	/* Reserve 2 bytes for the BBM. */
	region->offset = 2;
	region->length = 126;

	return 0;
}

static const struct mtd_ooblayout_ops gd5f4gq4u_ooblayout = {
	.ecc = gd5f4gq4u_ooblayout_ecc,
	.free = gd5f4gq4u_ooblayout_free,
};

static int gd5f4gq4u_ecc_get_status(struct spinand_device *spinand,
				    u8 status)
{
	if (status)
		debug("%s (%d): status=%02x\n", __func__, __LINE__, status);

	switch (status & GIGADEVICE_STATUS_ECC_MASK) {
	case STATUS_ECC_NO_BITFLIPS:
		return 0;

	case GIGADEVICE_STATUS_ECC_1TO7_BITFLIPS:
		return 7;

	case GIGADEVICE_STATUS_ECC_8_BITFLIPS:
		return 8;

	case STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;

	default:
		break;
	}

	return -EINVAL;
}

static int f50l1g41lb_ooblayout_ecc(struct mtd_info *mtd, int section,
					 struct mtd_oob_region *region)
{
	/* Unable to know the layout of ECC */
	return -ERANGE;
}

static int f50l1g41lb_ooblayout_free(struct mtd_info *mtd, int section,
					  struct mtd_oob_region *region)
{
	if (section > 3)
		return -ERANGE;

	/* Reserve 2 bytes for the BBM. */
	region->offset = (14 * section) + 2;
	region->length = 6;

	return 0;
}

static const struct mtd_ooblayout_ops f50l1g41lb_ooblayout = {
	.ecc = f50l1g41lb_ooblayout_ecc,
	.free = f50l1g41lb_ooblayout_free,
};

static int f50l1g41lb_ecc_get_status(struct spinand_device *spinand,
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

	case STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;

	default:
		break;
	}

	return -EINVAL;
}

static const struct spinand_info gigadevice_spinand_table[] = {
	SPINAND_INFO("GD5F1GQ4UB 3.3v", 0xd1,
		     NAND_MEMORG(1, 2048, 128, 64, 1024, 1, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&gd5f1gq4u_ooblayout,
				     gd5f1gq4u_ecc_get_status)),

	SPINAND_INFO("GD5F4GQ4UB 3.3v", 0xd4,
		     NAND_MEMORG(1, 4096, 256, 64, 2048, 1, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&gd5f4gq4u_ooblayout,
				     gd5f4gq4u_ecc_get_status)),

	SPINAND_INFO("GD5F2GQ5UE 3.3v", 0x52,
		     NAND_MEMORG(1, 2048, 128, 64, 2048, 2, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&gd5f1gq4u_ooblayout,
				     gd5f1gq4u_ecc_get_status)),

	SPINAND_INFO("GD5F4GM8RE 1.8V", 0x85,
		     NAND_MEMORG(1, 2048, 128, 64, 4096, 1, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     SPINAND_HAS_QE_BIT,
		     SPINAND_ECCINFO(&gd5f1gq4u_ooblayout,
				     gd5f1gq4u_ecc_get_status)),

	SPINAND_INFO("F50L1G41LB-104YG2M 3.3V", 0x01,
		     NAND_MEMORG(1, 2048, 64, 64, 1024, 1, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     0,
		     SPINAND_ECCINFO(&f50l1g41lb_ooblayout,
				     f50l1g41lb_ecc_get_status)),
};

static int gigadevice_spinand_detect(struct spinand_device *spinand)
{
	u8 *id = spinand->id.data;
	int ret;

	if (id[0] != SPINAND_MFR_GIGADEVICE)
		return 0;

	ret = spinand_match_and_init(spinand, gigadevice_spinand_table,
				     ARRAY_SIZE(gigadevice_spinand_table),
				     id[1]);
	if (ret)
		return ret;

	return 1;
}

static const struct spinand_manufacturer_ops gigadevice_spinand_manuf_ops = {
	.detect = gigadevice_spinand_detect,
};

const struct spinand_manufacturer gigadevice_spinand_manufacturer = {
	.id = SPINAND_MFR_GIGADEVICE,
	.name = "GigaDevice",
	.ops = &gigadevice_spinand_manuf_ops,
};
