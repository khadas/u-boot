/* SPDX-License-Identifier: GPL-2.0 */
#ifndef DECOMPRESS_UNLZ4_H
#define DECOMPRESS_UNLZ4_H

#include <linux/types.h>
#include <linux/string.h>

int unlz4(const void *src, size_t srcn, void *dst, size_t *dstn);
#endif
