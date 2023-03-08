// SPDX-License-Identifier:     BSD-3-Clause
/*
 * Copyright (c) 2019 The Fuchsia Authors
 */

#include <common.h>
#include <amlogic/image_check.h>
#include <libabr/libabr.h>
#include <zircon/abr.h>
#include <zircon/image.h>
#include <zircon/partition.h>
#include <zircon/vboot.h>
#include <zircon/zircon.h>

/* === ABR sysdeps and Ops === */
void *abr_memcpy(void *dest, const void *src, size_t n)
{
	return memcpy(dest, src, n);
}

void *abr_memset(void *dest, const int c, size_t n)
{
	return memset(dest, c, n);
}

void abr_print(const char *message)
{
	puts(message);
}

void abr_abort(void)
{
	panic("ABORT!");
}

uint32_t abr_crc32(const uint8_t *buf, size_t buf_size)
{
	return crc32(0, buf, buf_size);
}

#define ABR_PARTITION_NAME "misc"
#define ABR_OFFSET 0
static ABRResult read_abr_metadata_buffer(unsigned char *data, size_t *size)
{
	assert(data);
	assert(size);

	if (zircon_partition_read(ABR_PARTITION_NAME, ABR_OFFSET, data,
				  *size) != 0) {
		fprintf(stderr, "failed to read A/B/R metadata.\n");
		return ABR_RESULT_ERROR_IO;
	}

	return ABR_RESULT_OK;
}

static ABRResult write_abr_metadata_buffer(const unsigned char *data,
					   size_t size)
{
	assert(data);

	if (zircon_partition_write(ABR_PARTITION_NAME, ABR_OFFSET, data,
				   size) != 0) {
		fprintf(stderr, "failed to read A/B/R metadata.\n");
		return ABR_RESULT_ERROR_IO;
	}

	return ABR_RESULT_OK;
}

ABROps zircon_abr_ops = {
	.read_abr_metadata_buffer = read_abr_metadata_buffer,
	.write_abr_metadata_buffer = write_abr_metadata_buffer,
};

#define ZIRCON_IMAGE_HEADER_BUF_SIZE 1024

static int zircon_find_headers(const char *partition, bool *out_aml_hdr_found,
			       struct aml_boot_header_t *out_aml_hdr,
			       bool *out_zbi_hdr_found,
			       zbi_header_t *out_zbi_hdr)
{
	unsigned char buf[ZIRCON_IMAGE_HEADER_BUF_SIZE];

	// Minimum read size needed to verify existence of a ZBI and get length
	uint64_t read_size =
		sizeof(struct aml_boot_header_t) + sizeof(zbi_header_t) * 2;

	assert(sizeof(buf) >= read_size);

	if (zircon_partition_read(partition, 0, buf, read_size))
		return -1;

	uint64_t zbi_offset = 0;

	struct aml_boot_header_t *aml_hdr = (struct aml_boot_header_t *)buf;

	bool aml_hdr_found = aml_hdr->magic == AML_BOOT_IMAGE_MAGIC;

	if (out_aml_hdr_found)
		*out_aml_hdr_found = aml_hdr_found;

	if (aml_hdr_found) {
		zbi_offset = sizeof(struct aml_boot_header_t);
		if (out_aml_hdr)
			*out_aml_hdr = *aml_hdr;
	}

	zbi_header_t *zbi_hdr = (zbi_header_t *)(buf + zbi_offset);

	bool zbi_hdr_found = !zircon_image_check_header(zbi_hdr);

	if (out_zbi_hdr_found)
		*out_zbi_hdr_found = zbi_hdr_found;

	if (zbi_hdr_found) {
		if (out_zbi_hdr)
			*out_zbi_hdr = *zbi_hdr;
	}

	return 0;
}

bool zircon_find_zbi(const char *partition)
{
	bool zbi_hdr_found;

	if (zircon_find_headers(partition, NULL, NULL, &zbi_hdr_found, NULL)) {
		fprintf(stderr, "Failed to find headers");
		return false;
	}
	return zbi_hdr_found;
}

/**
 * zircon_get_img_size() - Gets upper bound size for image in partition
 *
 * The size is derived from the Amlogic boot header if it exists.
 * If not, the first ZBI header is used. If neither header is found,
 * the entire partition size is used.
 *
 * @partition: The zircon partition in which the image resides.
 * @size: Set to an upper bound of the full image size, including amlogic
 * secure boot headers and ZBI headers.
 * @img_offset: Set to image offset.
 *
 * Return: 0 on success, negative value on failure
 */
int zircon_get_img_size(const char *partition, uint64_t *size,
			size_t *img_offset)
{
	bool aml_hdr_found;
	struct aml_boot_header_t aml_hdr;
	bool zbi_hdr_found;
	zbi_header_t zbi_hdr;

	if (zircon_find_headers(partition, &aml_hdr_found, &aml_hdr,
				&zbi_hdr_found, &zbi_hdr)) {
		fprintf(stderr, "Failed to find headers\n");
		return -1;
	}

	// Always set to 0 unless Amlogic boot header found
	*img_offset = 0;

	// Get an upper bound on image size by using the outermost header of
	// the image and falling back on the partition size.
	// The Amlogic header is always found first if it exists.
	if (aml_hdr_found) {
		*size = aml_hdr.img_size + sizeof(struct aml_boot_header_t);
		*img_offset = sizeof(struct aml_boot_header_t);
		printf("AML header found: img_size: %llu\n", *size);
	} else if (zbi_hdr_found) {
		*size = zbi_hdr.length + sizeof(zbi_header_t);
		printf("ZBI header found: img_size: %llu\n", *size);
	} else {
		if (zircon_get_partition_size(partition, size)) {
			fprintf(stderr, "Failed to get partition size\n");
			return -1;
		}
		printf("Headers not found. Using partition size: %llu\n", *size);
	}

	return 0;
}

/** zircon_load_kernel() - Loads zircon kernel into specified address.
 *
 * @loadaddr: The address at which to load the kernel.
 * @slot_idx: The slot index from which to read the kernel.
 * @has_successfully_booted: True if this slot has previously booted successfully.
 * @img_offset: Set to the image offset, if known, otherwise, set to 0.
 *
 * Return: 0 if successful, negative value on failure.
 */
static int zircon_load_kernel(unsigned char *loadaddr, unsigned int slot_idx,
			      bool has_successfully_booted, size_t *img_offset)
{
	const char *ab_suffix = abr_get_slot_name_by_index(slot_idx);

	if (!ab_suffix) {
		fprintf(stderr, "Invalid slot_idx\n");
		return -1;
	}

	const char *partition = zircon_slot_idx_to_pname[slot_idx];

	printf("ABR: loading kernel from %s...\n", partition);

	uint64_t img_size;

	if (zircon_get_img_size(partition, &img_size, img_offset)) {
		fprintf(stderr, "unable to get zircon image size\n");
		return -1;
	}

	if (zircon_partition_read(partition, 0, loadaddr, img_size)) {
		fprintf(stderr, "Failed to read partition\n");
		return -1;
	}

	if (zircon_vboot_slot_verify(loadaddr, img_size, ab_suffix,
				     has_successfully_booted)) {
		fprintf(stderr, "Failed to verify slot: %s\n", ab_suffix);
		return -1;
	}

	// After verifying image, add current slot boot item.

	zbi_header_t *zbi = (zbi_header_t *)(loadaddr + *img_offset);

	char slot_info[32];

	snprintf(slot_info, sizeof(slot_info), "zvb.current_slot=%s",
		 ab_suffix);
	zircon_append_boot_item(zbi, ZBI_TYPE_CMDLINE, 0, slot_info,
				strlen(slot_info) + 1);

	printf("Successfully loaded slot: %s\n", ab_suffix);

	return 0;
}

int zircon_abr_img_load(unsigned char *loadaddr, bool force_recovery,
			size_t *img_offset)
{
	int ret = 0;
	unsigned int slot_idx;

	do {
		bool has_successfully_booted = false;

		/* check recovery mode */
		if (force_recovery) {
			slot_idx = ABR_R_SLOT_IDX;
		} else {
			slot_idx = abr_get_boot_slot(&zircon_abr_ops, true,
						     &has_successfully_booted);
			assert(slot_idx < ARRAY_SIZE(zircon_slot_idx_to_pname));
		}

		ret = zircon_load_kernel(loadaddr, slot_idx,
					 has_successfully_booted, img_offset);

		if (ret) {
			fprintf(stderr, "ABR: failed to load slot %d: %s\n",
				slot_idx, zircon_slot_idx_to_pname[slot_idx]);
			if (abr_mark_slot_unbootable(&zircon_abr_ops,
						     slot_idx) !=
			    ABR_RESULT_OK) {
				return -1;
			}
			continue;
		}

	} while ((ret != 0) && (slot_idx != ABR_R_SLOT_IDX));

	if (ret != 0) {
		fprintf(stderr, "Fail to boot: no valid slots\n");
		return -1;
	}

	return 0;
}
