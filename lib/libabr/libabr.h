/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2016 The Fuchsia Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 */

#ifndef __ABR_H__
#define __ABR_H__

#include <stddef.h>

/* Return codes
 *
 * ABR_RESULT_OK is returned if the requested operation was
 * successful.
 *
 * ABR_RESULT_ERROR_IO is returned if the underlying hardware (disk
 * or other subsystem) encountered an I/O error.
 *
 * ABR_RESULT_ERROR_INVALID_DATA is returned if invalid argument
 * is passed
 *
 * ABR_RESULT_ERROR_UNSUPPORTED_VERSION is returned if wrong version
 * of ABRData is passed
 */
typedef enum {
	ABR_RESULT_OK,
	ABR_RESULT_ERROR_IO,
	ABR_RESULT_ERROR_INVALID_DATA,
	ABR_RESULT_ERROR_UNSUPPORTED_VERSION,
} ABRResult;

#include "abr_sysdeps.h"
#include "abr_data.h"
#include "abr_ops.h"
#include "abr_util.h"

#define ABR_R_SLOT_IDX 2

/* Describes a current status of the slot
 * |valid|, true if the slot can be used for booting
 * |successful_boot|, true if the slot was marked as successful/stable.
 * |retry_count|, number of retries are left. Should be 0 if |valid| == false
 * or |successful_boot| == true
 */
typedef struct {
	bool      valid;
	bool      successful_boot;
	uint8_t   retry_count;
} ABRSlotInfo;

#ifdef __cplusplus
extern "C" {
#endif

/* Convert |src| buffer to |dest| ABRData.
 * Returns false if the data is invalid (e.g. wrong magic,
 * wrong CRC32 etc.), true otherwise.
 */
ABRResult abr_convert_buffer_to_data_and_verify(const unsigned char *src,
		size_t size, ABRData *dest);

/* Convert |src| ABRData to |dest| buffer.
 * Also updates the |crc32| field in |dest|.
 */
ABRResult abr_convert_data_to_buffer(const ABRData *src,
		unsigned char *dest, size_t size);

/* High-level function to select a slot to boot. The following
 * algorithm is used:
 *
 * 1. A/B/R metadata is loaded and validated. if it's invalid then it's
 * reset and this reset metadata is returned.
 *
 * 2. if oneshot_recovery_mode is set in A/B/R metadata, R slot is
 * selected to boot
 *
 * 3. A valid slot with highest priority is chosen to boot.
 *
 * 4. If there are no valid slots, R slot is chosen
 *
 * Set |update_metadata| as true to update A/B/R metadata, 'false' - to
 * keep metadata unchanged.
 *
 * |is_successful| is set to true, if returned slot is marked as successful.
 */
unsigned int abr_get_boot_slot(ABROps *abr_ops, bool update_metadata, bool *is_successful);

/* Converts slot index into corresponding name
 * NULL will be returned in case of error.
 */
const char *abr_get_slot_name_by_index(unsigned int index);

/* Marks the slot with the given slot number as active. Returns
 * ABR_RESULT_OK on success, error code otherwise.
 *
 * This function is typically used by the OS updater when completing
 * an update. It can also used by the firmware for implementing the
 * "set_active" command.
 */
ABRResult abr_mark_slot_active(ABROps *abr_ops, unsigned int slot_number);

/* Marks the slot with the given slot number as unbootable. Returns
 * ABR_RESULT_OK on success, error code otherwise.
 *
 * This function is typically used by the OS updater before writing to
 * a slot.
 */
ABRResult abr_mark_slot_unbootable(ABROps *abr_ops, unsigned int slot_number);

/* Marks the slot with the given slot number as having booted
 * successfully. Returns ABR_RESULT_OK on success, error code
 * otherwise.
 *
 * Calling this on an unbootable slot is an error -
 * ABR_RESULT_ERROR_INVALID_DATA will be returned.
 *
 * This function is typically used by the OS updater after having
 * confirmed that the slot works as intended.
 */
ABRResult abr_mark_slot_successful(ABROps *abr_ops, unsigned int slot_number);

/* Get current slot info */
ABRResult abr_get_slot_info(ABROps *abr_ops,
		unsigned int slot_number, ABRSlotInfo *info);

#ifdef __cplusplus
}
#endif

#endif /* __ABR_H__ */

