/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2016 The Fuchsia Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 */

#ifndef __ABR_SYSDEPS_H__
#define __ABR_SYSDEPS_H__

#include <linux/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ABR_ATTR_PACKED __attribute__((packed))

/* Copy |n| bytes from |src| to |dest|. returns |dest|. */
void *abr_memcpy(void *dest, const void *src, size_t n);

/* Set |n| bytes starting at |s| to |c|.  Returns |dest|. */
void *abr_memset(void *dest, const int c, size_t n);

/* Prints out a NUL-terminated string. */
void abr_print(const char *message);

/* Aborts the program or reboots the device if
 * |abort| is not implemented in a platform.
 */
void abr_abort(void);

/* Calculates the CRC-32 for data in |buf| of size |buf_size|. */
uint32_t abr_crc32(const uint8_t *buf, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* __ABR_SYSDEPS_H__ */
