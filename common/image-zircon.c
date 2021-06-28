/*
 * Copyright (c) 2018 The Fuchsia Authors
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <image.h>
#include <zircon/image.h>

void zircon_append_boot_item(zbi_header_t* container, uint32_t type, uint32_t extra,
                             const void* payload, uint32_t length) {
	zbi_header_t* dest = (zbi_header_t*)((uintptr_t)container + container->length + sizeof(zbi_header_t));

	dest->type = type;
	dest->length = length;
	dest->extra = extra;
	dest->flags = ZBI_FLAG_VERSION;
	dest->reserved0 = 0;
	dest->reserved1 = 0;
	dest->magic = ZBI_ITEM_MAGIC;
	dest->crc32 = ZBI_ITEM_NO_CRC32;

	if (length) {
		memcpy(dest + 1, payload, length);
	}
	length = ZBI_ALIGN(length + sizeof(zbi_header_t));
	container->length += length;
}

int zircon_image_check_header(const void *hdr)
{
	const zbi_header_t* zbi = hdr;

	return (zbi[0].type == ZBI_TYPE_CONTAINER &&
			zbi[0].extra == ZBI_CONTAINER_MAGIC &&
			zbi[0].magic == ZBI_ITEM_MAGIC &&
			zbi[1].type == ZBI_TYPE_KERNEL_ARM64 &&
			zbi[1].magic == ZBI_ITEM_MAGIC) ? 0 : -1;
}

/**
 * zircon_image_get_kernel() - processes kernel part of Zircon bootdata
 * @bootdata:	Pointer to bootdata.
 * @verify:		Checksum verification flag. Currently unimplemented.
 * @os_data:	Pointer to a ulong variable, will hold os data start
 *			address.
 * @os_len:	Pointer to a ulong variable, will hold os data length.
 *
 * This function returns the os image's start address and length. Also,
 * it appends the kernel command line to the bootargs env variable.
 *
 * Return: Zero, os start address and length on success,
 *		otherwise on failure.
 */
int zircon_image_get_kernel(const void* hdr, int verify,
			     ulong *os_data, ulong *os_len)
{
	if (zircon_image_check_header(hdr))
		return -1;

	const zbi_header_t* zbi = hdr;

	*os_data = (ulong)zbi;
	*os_len = zbi->length + sizeof(*zbi);
	return 0;
}

ulong zircon_image_get_end(const void *hdr)
{
	const zbi_header_t* zbi = hdr;
	return (ulong)hdr + zbi->length + sizeof(*zbi);
}

ulong zircon_image_get_kload(const void *hdr)
{
	return 0x1080000;
}

ulong zircon_image_get_comp(const void *hdr)
{
	return IH_COMP_NONE;
}
