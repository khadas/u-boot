// SPDX-License-Identifier: BSD-3-Clause
/* Copyright 2016 The Fuchsia Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 */

#include "libabr.h"

#include <stdarg.h>

static uint32_t change_byte_order(uint32_t in)
{
	union {
		uint32_t word;
		uint8_t bytes[4];
	} ret;
	ret.bytes[0] = (in >> 24) & 0xff;
	ret.bytes[1] = (in >> 16) & 0xff;
	ret.bytes[2] = (in >> 8) & 0xff;
	ret.bytes[3] = in & 0xff;
	return ret.word;
}

/* only LE host is supported for now */
uint32_t abr_be32toh(uint32_t in)
{
	return change_byte_order(in);
}

/* Converts a 32-bit unsigned integer from host (LE only for now)
 * to big-endian byte order.
 */
uint32_t abr_htobe32(uint32_t in)
{
	return change_byte_order(in);
}

int abr_safe_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *us1 = s1;
	const unsigned char *us2 = s2;
	int result = 0;

	if (n == 0)
		return 0;

	/*
	 * Code snippet without data-dependent branch due to Nate Lawson
	 * (nate@root.org) of Root Labs.
	 */
	while (n--)
		result |= *us1++ ^ *us2++;

	return result != 0;
}

