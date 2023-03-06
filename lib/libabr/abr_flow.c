// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright 2019 The Fuchsia Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 */

#include "libabr.h"
#include <stdbool.h>
#include <stddef.h>

#define MIN_PAGE_SIZE 4096
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static void abr_data_init(ABRData *data)
{
	abr_memset(data, '\0', sizeof(ABRData));
	abr_memcpy(data->magic, ABR_MAGIC, ABR_MAGIC_LEN);
	data->version_major = ABR_MAJOR_VERSION;
	data->version_minor = ABR_MINOR_VERSION;
	data->slots[0].priority = ABR_MAX_PRIORITY;
	data->slots[0].tries_remaining = ABR_MAX_TRIES_REMAINING;
	data->slots[0].successful_boot = 0;
	data->slots[1].priority = ABR_MAX_PRIORITY - 1;
	data->slots[1].tries_remaining = ABR_MAX_TRIES_REMAINING;
	data->slots[1].successful_boot = 0;
}

static bool slot_is_bootable(ABRSlotData *slot)
{
	return (slot->priority > 0) &&
	    (slot->successful_boot || (slot->tries_remaining > 0));
}

static void slot_set_unbootable(ABRSlotData *slot)
{
	slot->priority = 0;
	slot->tries_remaining = 0;
	slot->successful_boot = 0;
}

/* Ensure all unbootable and/or illegal states are marked as the
 * canonical 'unbootable' state, e.g. priority=0, tries_remaining=0,
 * and successful_boot=0.
 */
static void slot_normalize(ABRSlotData *slot)
{
	if (slot->priority > 0) {
		if (slot->tries_remaining == 0 && !slot->successful_boot) {
			/* We've exhausted all tries -> unbootable. */
			slot_set_unbootable(slot);
		}
		if (slot->tries_remaining > 0 && slot->successful_boot) {
			/* Illegal state - abr_mark_slot_successful() will clear
			 * tries_remaining when setting successful_boot.
			 * Reset to default state.
			 */
			slot->tries_remaining = ABR_MAX_TRIES_REMAINING;
			slot->successful_boot = 0;
			/* having two slots with high priority will not cause an error */
			slot->priority = ABR_MAX_PRIORITY;
		}
	} else {
		slot_set_unbootable(slot);
	}
}

static ABRResult save_metadata(ABROps *abr_ops, ABRData *data)
{
	unsigned char serialized[MIN_PAGE_SIZE] = { 0 };
	ABRResult ret;
	size_t size = sizeof(ABRData);

	abr_debug("Writing A/B metadata to disk.\n");

	ret = abr_convert_data_to_buffer(data, serialized, size);
	/* abr_convert_data_to_buffer fails only if buffer size is less
	 * than sizeof(ABRData). This situation should not happen here.
	 */
	abr_assert(ret == ABR_RESULT_OK);

	abr_assert(abr_ops->write_abr_metadata_buffer);

	ret =
	    abr_ops->write_abr_metadata_buffer(serialized, sizeof(serialized));
	if (ret != ABR_RESULT_OK)
		abr_error("Error writing A/B metadata.\n");

	return ret;
}

/* Helper function to load A/B/R metadata - returns ABR_RESULT_OK on
 * success, error code otherwise.
 */
static ABRResult load_metadata(ABROps *abr_ops,
			       ABRData *abr_data, ABRData *abr_data_orig)
{
	unsigned char buf[MIN_PAGE_SIZE];
	ABRResult ret;
	size_t size = sizeof(buf);

	abr_assert(abr_ops->read_abr_metadata_buffer);

	ret = abr_ops->read_abr_metadata_buffer(buf, &size);
	if (ret != ABR_RESULT_OK) {
		abr_error("I/O error while loading A/B metadata buffer.\n");
		return ret;
	}

	if (size != sizeof(buf)) {
		abr_error("Wrong size of A/B/R metadata buffer.\n");
		return ABR_RESULT_ERROR_IO;
	}

	if (abr_convert_buffer_to_data_and_verify(buf,
						  sizeof(ABRData),
						  abr_data) != ABR_RESULT_OK) {
		abr_error("Error validating A/B metadata from disk.\n");

		abr_data_init(abr_data);

		abr_error("Resetting and writing new A/B metadata to disk.\n");
		ret = save_metadata(abr_ops, abr_data);
		if (ret != ABR_RESULT_OK)
			return ret;
	}

	*abr_data_orig = *abr_data;

	/* Ensure data is normalized, e.g. illegal states will be marked as
	 * unbootable and all unbootable states are represented with
	 * (priority=0, tries_remaining=0, successful_boot=0).
	 */
	slot_normalize(&abr_data->slots[0]);
	slot_normalize(&abr_data->slots[1]);
	return ABR_RESULT_OK;
}

/* Writes A/B/R metadata to disk only if it has changed - returns
 * ABR_RESULT_OK on success, error code otherwise.
 */
static ABRResult save_metadata_if_changed(ABROps *abr_ops,
					  ABRData *abr_data,
					  ABRData *abr_data_orig)
{
	if (abr_safe_memcmp(abr_data, abr_data_orig, sizeof(ABRData)) == 0)
		return ABR_RESULT_OK;

	return save_metadata(abr_ops, abr_data);
}

/* ==================== public API =================== */

const char *abr_get_slot_name_by_index(unsigned int index)
{
	static const char * const slot_suffixes[] = { "_a", "_b", "_r" };

	if (index >= ARRAY_SIZE(slot_suffixes)) {
		abr_error("Invalid slot index\n");
		return NULL;
	}

	return slot_suffixes[index];
}

unsigned int abr_get_boot_slot(ABROps *abr_ops, bool update_metadata,
			       bool *is_successful)
{
	ABRData abr_data, abr_data_orig;
	ABRResult ret;
	unsigned int slot_idx_to_boot = ABR_R_SLOT_IDX;

	if (is_successful)
		*is_successful = false;

	ret = load_metadata(abr_ops, &abr_data, &abr_data_orig);
	if (ret != ABR_RESULT_OK) {
		abr_error
		    ("Failed to load metadata. Switching to 'recovery' mode.\n");
		return ABR_R_SLOT_IDX;
	}
	// oneshot recovery boot has the highest priority
	if (abr_data.oneshot_recovery_boot) {
		abr_data.oneshot_recovery_boot = 0;
		save_metadata(abr_ops, &abr_data);
		return ABR_R_SLOT_IDX;
	}

	if (slot_is_bootable(&abr_data.slots[0])) {
		if (is_successful)
			*is_successful = true;

		slot_idx_to_boot = 0;
		if (slot_is_bootable(&abr_data.slots[1]) &&
		    abr_data.slots[1].priority > abr_data.slots[0].priority) {
			slot_idx_to_boot = 1;
		}
	} else if (slot_is_bootable(&abr_data.slots[1])) {
		if (is_successful)
			*is_successful = true;
		slot_idx_to_boot = 1;
	}

	if (update_metadata) {
		/* ... and decrement tries remaining, if applicable.
		 * Note: no needs to check if tries_remainig > 0, because
		 * a slot will be marked as unbootable in `slot_normalize'
		 * if tries_remainig value is 0
		 */
		if (slot_idx_to_boot != ABR_R_SLOT_IDX &&
		    !abr_data.slots[slot_idx_to_boot].successful_boot) {
			abr_data.slots[slot_idx_to_boot].tries_remaining--;
			if (is_successful)
				*is_successful = false;
			save_metadata(abr_ops, &abr_data);
		}
	}

	return slot_idx_to_boot;
}

ABRResult abr_mark_slot_active(ABROps *abr_ops, unsigned int slot_number)
{
	ABRData abr_data, abr_data_orig;
	unsigned int other_slot_number;
	ABRResult ret;

	if (slot_number > ABR_R_SLOT_IDX) {
		abr_error("Wrong slot number.\n");
		return ABR_RESULT_ERROR_INVALID_DATA;
	}

	if (slot_number == ABR_R_SLOT_IDX)
		/* do nothing. R slot does not have metadata */
		return ABR_RESULT_OK;

	ret = load_metadata(abr_ops, &abr_data, &abr_data_orig);
	if (ret != ABR_RESULT_OK)
		return ret;

	/* Make requested slot top priority, unsuccessful, and with max tries. */
	abr_data.slots[slot_number].priority = ABR_MAX_PRIORITY;
	abr_data.slots[slot_number].tries_remaining = ABR_MAX_TRIES_REMAINING;
	abr_data.slots[slot_number].successful_boot = 0;

	/* Ensure other slot doesn't have as high a priority. */
	other_slot_number = 1 - slot_number;
	if (abr_data.slots[other_slot_number].priority == ABR_MAX_PRIORITY) {
		abr_data.slots[other_slot_number].priority =
		    ABR_MAX_PRIORITY - 1;
	}

	ret = save_metadata_if_changed(abr_ops, &abr_data, &abr_data_orig);
	return ret;
}

ABRResult abr_mark_slot_unbootable(ABROps *abr_ops, unsigned int slot_number)
{
	ABRData abr_data, abr_data_orig;
	ABRResult ret;

	if (slot_number > ABR_R_SLOT_IDX) {
		abr_error("Wrong slot number.\n");
		return ABR_RESULT_ERROR_INVALID_DATA;
	}

	if (slot_number == ABR_R_SLOT_IDX)
		/* do nothing. R slot does not have metadata */
		return ABR_RESULT_OK;

	ret = load_metadata(abr_ops, &abr_data, &abr_data_orig);
	if (ret != ABR_RESULT_OK)
		return ret;

	slot_set_unbootable(&abr_data.slots[slot_number]);

	ret = save_metadata_if_changed(abr_ops, &abr_data, &abr_data_orig);
	return ret;
}

ABRResult abr_mark_slot_successful(ABROps *abr_ops, unsigned int slot_number)
{
	ABRData abr_data, abr_data_orig;
	ABRResult ret;

	if (slot_number > ABR_R_SLOT_IDX) {
		abr_error("Wrong slot number.\n");
		return ABR_RESULT_ERROR_INVALID_DATA;
	}

	if (slot_number == ABR_R_SLOT_IDX)
		/* Do nothing. The R slot does not have metadata */
		return ABR_RESULT_OK;

	ret = load_metadata(abr_ops, &abr_data, &abr_data_orig);
	if (ret != ABR_RESULT_OK)
		return ret;

	if (!slot_is_bootable(&abr_data.slots[slot_number])) {
		abr_error("Cannot mark unbootable slot as successful.\n");
		return ABR_RESULT_ERROR_INVALID_DATA;
	}

	abr_data.slots[slot_number].tries_remaining = 0;
	abr_data.slots[slot_number].successful_boot = 1;

	ret = save_metadata_if_changed(abr_ops, &abr_data, &abr_data_orig);
	return ret;
}

ABRResult abr_get_slot_info(ABROps *abr_ops,
			    unsigned int slot_number, ABRSlotInfo *info)
{
	ABRData abr_data, abr_data_orig;
	ABRResult ret;

	if (!info) {
		abr_error("|info| is NULL.\n");
		return ABR_RESULT_ERROR_INVALID_DATA;
	}

	abr_memset(info, 0, sizeof(ABRSlotInfo));

	if (slot_number > ABR_R_SLOT_IDX) {
		abr_error("Wrong slot number.\n");
		return ABR_RESULT_ERROR_INVALID_DATA;
	}

	if (slot_number == ABR_R_SLOT_IDX) {
		/* assume that R slot is always OK */
		info->valid = true;
		info->successful_boot = true;
		info->retry_count = 0;
		return ABR_RESULT_OK;
	}

	ret = load_metadata(abr_ops, &abr_data, &abr_data_orig);
	if (ret != ABR_RESULT_OK)
		return ret;

	info->valid = slot_is_bootable(&abr_data.slots[slot_number]);
	info->successful_boot = abr_data.slots[slot_number].successful_boot;
	info->retry_count = abr_data.slots[slot_number].tries_remaining;

	return ABR_RESULT_OK;
}

ABRResult abr_convert_buffer_to_data_and_verify(const unsigned char *src,
						size_t size, ABRData *dest)
{
	ABRData *data = (ABRData *)src;

	if (size != sizeof(ABRData)) {
		abr_error("Wrong buffer size.\n");
		return ABR_RESULT_ERROR_INVALID_DATA;
	}

	/* Ensure magic is correct. */
	if (abr_safe_memcmp(data->magic, ABR_MAGIC, ABR_MAGIC_LEN) != 0) {
		abr_error("Magic is incorrect.\n");
		return ABR_RESULT_ERROR_INVALID_DATA;
	}

	abr_memcpy(dest, data, sizeof(ABRData));
	dest->crc32 = abr_be32toh(dest->crc32);

	/* Ensure we don't attempt to access any fields if the major version
	 * is not supported.
	 */
	if (dest->version_major > ABR_MAJOR_VERSION) {
		abr_error("No support for given major version.\n");
		return ABR_RESULT_ERROR_UNSUPPORTED_VERSION;
	}

	/* Bail if CRC32 doesn't match. */
	if (dest->crc32 !=
	    abr_crc32((const uint8_t *)dest,
		      sizeof(ABRData) - sizeof(uint32_t))) {
		abr_error("CRC32 does not match.\n");
		return ABR_RESULT_ERROR_INVALID_DATA;
	}

	return ABR_RESULT_OK;
}

ABRResult abr_convert_data_to_buffer(const ABRData *src,
				     unsigned char *dest, size_t size)
{
	ABRData *dest_data = (ABRData *)dest;

	if (size != sizeof(ABRData)) {
		abr_error("Wrong buffer size.\n");
		return ABR_RESULT_ERROR_INVALID_DATA;
	}

	abr_memcpy(dest, src, sizeof(ABRData));
	dest_data->crc32 =
	    abr_htobe32(abr_crc32(dest, sizeof(ABRData) - sizeof(uint32_t)));

	return ABR_RESULT_OK;
}
