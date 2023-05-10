/* SPDX-License-Identifier:     BSD-3-Clause */
/*
 * Copyright (c) 2019 The Fuchsia Authors
 *
 */

#ifndef _ZIRCON_PARTITION_H_
#define _ZIRCON_PARTITION_H_

/**
 * DOC: Zircon Partitions
 *
 * This header describes an interface for "zircon partitions".
 * A zircon partition is an abstraction for interacting with
 * the standard partitions required by zircon:
 * Ex. zircon_a, zircon_b, zircon_r, misc, vbmeta_a, etc.
 *
 * These partitions may or may not exist in the GPT, but this
 * interface will abstract the details of how to access them.
 */

#define ZIRCON_PARTITION_PREFIX "zircon"

/**
 * zircon_get_partition_size() - Get partition size in bytes.
 *
 * @name:	Zircon partition name
 * @size:	Address to write size in bytes to. Can be NULL.
 *
 * Return:	0 if OK, negative value on error.
 */
int zircon_get_partition_size(const char *name, uint64_t *size);

/**
 * zircon_partition_write() - Write to zircon partition.
 *
 * @name:	Zircon partition name
 * @offset:	Offset in bytes at which to write data.
 * @data:	Buffer of data to write
 * @size:	Number of bytes to write.
 *
 * Return:	0 if OK, negative value on error.
 */
int zircon_partition_write(const char *name, uint64_t offset,
			   const unsigned char *data, size_t size);

/**
 * zircon_partition_read() - Read from zircon partition.
 *
 * @name:	Zircon partition name
 * @offset:	Offset in bytes at which to read data.
 * @data:	Buffer to read into
 * @size:	Number of bytes to read.
 *
 * Return:	0 if OK, negative value on error.
 */
int zircon_partition_read(const char *name, uint64_t offset,
			  unsigned char *data, size_t size);

/**
 * zircon_partition_erase() - Erase zircon partition.
 *
 * @name:	Zircon partition name
 *
 * Return:	0 if OK, negative value on error.
 */
int zircon_partition_erase(const char *name);
#endif /* _ZIRCON_PARTITION_H_ */
