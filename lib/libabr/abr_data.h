/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2019 The Fuchsia Authors. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 */

#ifndef __ABR_DATA_H__
#define __ABR_DATA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ABR data structures have backward compatibility with libavb_ab.
 * ABRSlotData and ABRData are inhereted from avb:
 *	   https://android.googlesource.com/platform/external/avb/+/37f5946d0e1159273eff61dd8041377fedbf55a9/libavb_ab/
 */

/* Magic for the A/B struct when serialized. */
#define ABR_MAGIC "\0AB0"
#define ABR_MAGIC_LEN 4

/* Versioning for the on-disk A/B metadata */
#define ABR_MAJOR_VERSION 2
#define ABR_MINOR_VERSION 0

/* Size of ABRData struct. */
#define ABR_DATA_SIZE 32

/* Maximum values for slot data */
#define ABR_MAX_PRIORITY 15
#define ABR_MAX_TRIES_REMAINING 7

/* Struct used for recording per-slot metadata.
 *
 * When serialized, data is stored in network byte-order.
 */
typedef struct ABRSlotData {
	/* Slot priority. Valid values range from 0 to ABR_MAX_PRIORITY,
	 * both inclusive with 1 being the lowest and ABR_MAX_PRIORITY
	 * being the highest. The special value 0 is used to indicate the
	 * slot is unbootable.
	 */
	uint8_t priority;

	/* Number of times left attempting to boot this slot ranging from 0
	 * to ABR_MAX_TRIES_REMAINING.
	 */
	uint8_t tries_remaining;

	/* Non-zero if this slot has booted successfully, 0 otherwise. */
	uint8_t successful_boot;

	/* Reserved for future use. */
	uint8_t reserved[1];
} ABR_ATTR_PACKED ABRSlotData;

/* Struct used for recording A/B/R metadata.
 *
 * When serialized, data is stored in network byte-order.
 */
typedef struct ABRData {
	/* Magic number used for identification - see ABR_MAGIC. */
	uint8_t magic[ABR_MAGIC_LEN];

	/* Version of on-disk struct - see ABR_{MAJOR,MINOR}_VERSION. */
	uint8_t version_major;
	uint8_t version_minor;

	/* Padding to ensure |slots| field start eight bytes in. */
	uint8_t reserved1[2];

	/* A/B per-slot metadata. Recovery boot does not have its own
	 * data and will be used if both A/B slots are not bootable
	 */
	ABRSlotData slots[2];

	/* oneshot force recovery boot. */
	uint8_t oneshot_recovery_boot;

	/* Reserved for future use. */
	uint8_t reserved2[11];

	/* CRC32 of all 28 bytes preceding this field. */
	uint32_t crc32;
} ABR_ATTR_PACKED ABRData;

#ifdef __cplusplus
}
#endif

#endif /* __ABR_DATA_H__ */
