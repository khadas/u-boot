/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2016 The Fuchsia Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 */

#ifndef __ABR_UTIL_H__
#define __ABR_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ABR_STRINGIFY(x) #x
#define ABR_TO_STRING(x) ABR_STRINGIFY(x)

#ifdef ABR_ENABLE_DEBUG
/* Aborts the program if |expr| is false.
 *
 * This has no effect unless ABR_ENABLE_DEBUG is defined.
 */
#define abr_assert(expr)                                       \
	do {                                                   \
		if (!(expr)) {                                 \
			abr_fatal("assert fail: " #expr "\n"); \
		}                                              \
	} while (0)
#else
#define abr_assert(expr)
#endif /* ABR_ENABLE_DEBUG */

#ifdef ABR_ENABLE_DEBUG
/* Print a message, used for diagnostics.
 * This has no effect unless ABR_ENABLE_DEBUG is defined.
 */
#define abr_debug(message)               \
	abr_print(__FILE__               \
		":"                      \
		ABR_TO_STRING(__LINE__)  \
		": DEBUG: "              \
		message)
#else
#define abr_debug(message)
#endif /* ABR_ENABLE_DEBUG */

#define abr_error(message)               \
	abr_print(__FILE__               \
		":"                      \
		ABR_TO_STRING(__LINE__)  \
		": ERROR: "              \
		message)

#define abr_fatal(message)                       \
	do {                                     \
		abr_print(__FILE__               \
			":"                      \
			ABR_TO_STRING(__LINE__)  \
			": FATAL: "              \
			message);                \
		abr_abort();                     \
	} while (0)

/* Converts a 32-bit unsigned integer from big-endian to host byte order. */
uint32_t abr_be32toh(uint32_t in);

/* Converts a 32-bit unsigned integer from host to big-endian byte order. */
uint32_t abr_htobe32(uint32_t in);

/* Compare |n| bytes starting at |s1| with |s2| and return 0 if they
 * match, 1 if they don't.  Returns 0 if |n|==0, since no bytes
 * mismatched.
 *
 * Time taken to perform the comparison is only dependent on |n| and
 * not on the relationship of the match between |s1| and |s2|.
 *
 * Note that unlike abr_memcmp(), this only indicates inequality, not
 * whether |s1| is less than or greater than |s2|.
 */
int abr_safe_memcmp(const void *s1, const void *s2, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* __ABR_UTIL_H__ */
