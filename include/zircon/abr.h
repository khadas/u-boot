/* SPDX-License-Identifier:     BSD-3-Clause */
/*
 * Copyright (c) 2019 The Fuchsia Authors
 *
 */

#ifndef _ZIRCON_ABR_H_
#define _ZIRCON_ABR_H_

#include <common.h>
#include <amlogic/image_check.h>
#include <libabr/libabr.h>
#include <zircon/image.h>
#include <zircon/partition.h>

/**
 * zircon_slot_idx_to_pname - Map from slot index to partition name
 */
static const char *zircon_slot_idx_to_pname[3] = {
	ZIRCON_PARTITION_PREFIX "_a",
	ZIRCON_PARTITION_PREFIX "_b",
	ZIRCON_PARTITION_PREFIX "_r",
};

/**
 * zircon_abr_ops - ABROps using zircon partitioning
 */
extern ABROps zircon_abr_ops;

/**
 * zircon_find_zbi() - Finds ZBI in partition
 *
 * @partition: The zircon partition to look in.
 *
 * Return: true if ZBI found, false otherwise
 */
bool zircon_find_zbi(const char *partition);

/**
 * zircon_abr_img_load() - Loads zircon image using A/B/R scheme
 *
 * @loadaddr: Address at which to load the zircon image.
 * @force_recovery: trigger a force boot into recovery (R slot).
 * @img_offset: Set to entry point offset. Zero if unknown.
 *
 * Return: 0 if successful, negative value on failure.
 */
int zircon_abr_img_load(unsigned char *loadaddr, bool force_recovery,
			size_t *img_offset);

#endif /* _ZIRCON_ABR_H_ */
