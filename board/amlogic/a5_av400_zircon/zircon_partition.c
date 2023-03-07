// SPDX-License-Identifier:     BSD-3-Clause
/*
 * Copyright (c) 2019 The Fuchsia Authors
 *
 */

#include <common.h>
#include <amlogic/storage.h>
#include <emmc_partitions.h>
#include <zircon/partition.h>

typedef struct {
	char name[PART_NAME_LEN];
	char gpt_name[PART_NAME_LEN];

	// Offset relative to start of disk.
	uint64_t abs_offset;
	// Offset within gpt partition in bytes.
	uint64_t offset;
	// Size in bytes.
	uint64_t size;
} zircon_partition_t;

/*
 * ABR and AVB's metadatas are currently located on the 'sysconfig' partition,
 * but will be split out into their own GPT partitions at the same locations.
 * Once the GPT entry is added, these offsets will no longer be used.
 *
 * TODO: Remove hardcoded offsets after the GPT is updated.
 *
 * 00000 - CEFFF (828K) sysconfig data
 * CF000 - CFFFF   (4K) A/B/R metadata
 * D0000 - DFFFF  (64K) AVB -a metadata
 * E0000 - EFFFF  (64K) AVB -b metadata
 * F0000 - FFFFF  (64K) AVB -r metadata
 */
#define ABR_OFFSET 0xCF000
#define AVB_A_OFFSET 0xD0000
#define AVB_B_OFFSET 0xE0000
#define AVB_R_OFFSET 0xF0000
#define ABR_SIZE (4 * 1024)
#define AVB_SIZE (64 * 1024)

#define ZIRCON_PART_SIZE (32 * 1024 * 1024)

static uint8_t erase_buffer[AVB_SIZE] = { 0 };

/**
 * This partition mapping is used if the partition names are
 * not found in GPT.
 *
 * The size is set to the gpt partition size if set to 0 / unspecified.
 *
 * The offset and size must be multiples of block size (normally 512).
 */
static zircon_partition_t partition_map[] = {
	{
		.name = ZIRCON_PARTITION_PREFIX "_a",
		.gpt_name = "boot",
	},
	{
		.name = ZIRCON_PARTITION_PREFIX "_b",
		.gpt_name = "system",
		// Only the first 32MiB of system is zircon_b
		// The rest is part of fvm.
		.size = ZIRCON_PART_SIZE,
	},
	{
		.name = ZIRCON_PARTITION_PREFIX "_r",
		.gpt_name = "recovery",
	},
	{
		.name = "misc",
		.gpt_name = "sysconfig",
		.offset = ABR_OFFSET,
		.size = ABR_SIZE,
	},
	{
		.name = "vbmeta_a",
		.gpt_name = "sysconfig",
		.offset = AVB_A_OFFSET,
		.size = AVB_SIZE,
	},
	{
		.name = "vbmeta_b",
		.gpt_name = "sysconfig",
		.offset = AVB_B_OFFSET,
		.size = AVB_SIZE,
	},
	{
		.name = "vbmeta_r",
		.gpt_name = "sysconfig",
		.offset = AVB_R_OFFSET,
		.size = AVB_SIZE,
	},
};

/**
 * find_zircon_partition() - Finds zircon partition.
 *
 * @name:		Zircon partition name
 * @zircon_part:	Outputs zircon partition
 *
 * This first searches the GPT to find the given partition name.
 * If the partition name is not found, it looks at the hardcoded
 * translation layer to create a disk_partition_t.
 *
 * Return:	0 if OK, negative value on error.
 */
static int find_zircon_partition(const char *name,
				 zircon_partition_t *zircon_part)
{
	if (store_set_device(BOOT_EMMC)) {
		fprintf(stderr, "Error: Unable to set storage device\n");
		return -1;
	}

	struct partitions *part_info = find_mmc_partition_by_name(name);

	if (part_info) {
		strncpy(zircon_part->name, name, PART_NAME_LEN - 1);
		strncpy(zircon_part->gpt_name, name, PART_NAME_LEN - 1);
		zircon_part->offset = 0;
		zircon_part->size = part_info->size;

		return 0;
	}

	for (int i = 0; i < ARRAY_SIZE(partition_map); i++) {
		if (strcmp(name, partition_map[i].name))
			continue;

		part_info =
			find_mmc_partition_by_name(partition_map[i].gpt_name);

		if (!part_info) {
			fprintf(stderr,
				"Error: unable to find mapped gpt partition: %s, zircon partition: %s\n",
				partition_map[i].gpt_name,
				partition_map[i].name);
			return -1;
		}

		if (partition_map[i].offset + partition_map[i].size >
		    part_info->size) {
			fprintf(stderr,
				"Error: zircon partition, %s, exceeds its parent gpt partition, %s",
				partition_map[i].name,
				partition_map[i].gpt_name);
			return -1;
		}

		memcpy(zircon_part, &partition_map[i],
		       sizeof(zircon_partition_t));

		if (zircon_part->size == 0) {
			zircon_part->size =
				part_info->size - zircon_part->offset;
		}
		zircon_part->abs_offset = part_info->offset;

		return 0;
	}

	return -1;
}

/* === public API === */

int zircon_partition_write(const char *name, uint64_t offset,
			   const unsigned char *data, size_t size)
{
	zircon_partition_t zircon_part;

	if (find_zircon_partition(name, &zircon_part)) {
		fprintf(stderr, "Error: Unable to find partition: %s\n", name);
		return -1;
	}

	if (offset + size > zircon_part.size) {
		fprintf(stderr,
			"Error: Write cannot exceed partition boundary\n");
		return -1;
	}

	if (store_set_device(BOOT_EMMC)) {
		fprintf(stderr, "Error: Unable to set storage device\n");
		return -1;
	}

	if (store_write((const char *)zircon_part.gpt_name, zircon_part.offset + offset, size,
			(void *)data)) {
		fprintf(stderr, "Error: mmc write failed\n");
		return -1;
	}

	return 0;
}

int zircon_partition_read(const char *name, uint64_t offset,
			  unsigned char *data, size_t size)
{
	zircon_partition_t zircon_part;

	if (find_zircon_partition(name, &zircon_part)) {
		fprintf(stderr, "Error: Unable to find partition: %s\n", name);
		return -1;
	}

	if (offset + size > zircon_part.size) {
		fprintf(stderr,
			"Error: Write cannot exceed partition boundary\n");
		return -1;
	}

	if (store_set_device(BOOT_EMMC)) {
		fprintf(stderr, "Error: Unable to set storage device\n");
		return -1;
	}

	if (store_read(zircon_part.gpt_name, zircon_part.offset + offset, size,
		       data)) {
		fprintf(stderr, "Error: mmc read failed\n");
		return -1;
	}

	return 0;
}

int zircon_get_partition_size(const char *name, uint64_t *size)
{
	zircon_partition_t zircon_part;

	if (find_zircon_partition(name, &zircon_part)) {
		fprintf(stderr, "Error: Unable to find partition: %s\n", name);
		return -1;
	}

	if (size)
		*size = zircon_part.size;

	return 0;
}

int zircon_partition_erase(const char *name)
{
	zircon_partition_t zircon_part;

	if (find_zircon_partition(name, &zircon_part)) {
		fprintf(stderr, "Error: Unable to find partition: %s\n", name);
		return -1;
	}

	if (store_set_device(BOOT_EMMC)) {
		fprintf(stderr, "Error: Unable to set storage device\n");
		return -1;
	}

	struct mmc *mmc = find_mmc_device(STORAGE_DEV_EMMC);

	if (!mmc)
		return -1;

	size_t min_erase_size = mmc->erase_grp_size * mmc->write_bl_len;

	// store_erase can only erase block aligned partitions.
	// For unaligned partitions, this uses store_write to write 0xFF to the whole partition.
	if (zircon_part.abs_offset % min_erase_size ||
	    zircon_part.size % min_erase_size) {
		if (zircon_part.size > sizeof(erase_buffer)) {
			fprintf(stderr,
				"%s%s larger than %ld bytes.\n",
				"Zircon partition does not support erasing",
				"no erase-group aligned partitions",
				sizeof(erase_buffer));
			return -1;
		}
		memset(erase_buffer, 0xff, zircon_part.size);
		if (store_write(zircon_part.gpt_name, zircon_part.offset,
				zircon_part.size, erase_buffer)) {
			fprintf(stderr, "Error: mmc write-to-erase failed\n");
			return -1;
		}
		return 0;
	}

	if (store_erase(zircon_part.gpt_name, zircon_part.offset,
			zircon_part.size, 0)) {
		fprintf(stderr, "Error: mmc erase failed\n");
		return -1;
	}

	return 0;
}
