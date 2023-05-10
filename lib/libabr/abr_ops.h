/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2016 The Fuchsia Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 */

#ifndef __ABR_OPS_H__
#define __ABR_OPS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* High-level operations/functions/methods for A/B/R that are platform
 * dependent.
 */
typedef struct ABROps {
	/* Reads a buffer of A/B/R metadata from persistent storage.
	 * Returns ABR_RESULT_OK on success, error code otherwise.
	 * |size| is in-out parameter:
	 *     in:  requested size,
	 *     out: actual read size
	 */
	ABRResult (*read_abr_metadata_buffer)(unsigned char *data, size_t *size);

	/* Writes a buffer of A/B/R metadata to persistent storage.
	 * Returns ABR_RESULT_OK on success, error code otherwise.
	 */
	ABRResult (*write_abr_metadata_buffer)(const unsigned char *data, size_t size);
} ABROps;

#ifdef __cplusplus
}
#endif

#endif /* __ABR_OPS_H__ */
