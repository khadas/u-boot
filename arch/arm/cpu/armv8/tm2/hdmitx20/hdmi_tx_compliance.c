// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <linux/types.h>
#include <amlogic/hdmi.h>
/* Base Block, Vendor/Product Information, byte[8]~[17] */
struct edid_venddat_t {
	unsigned char data[10];
};

static struct edid_venddat_t vendor_null_pkt[] = {
	/* changhong LT32876 */
	{ {0x0d, 0x04, 0x21, 0x90, 0x01, 0x00, 0x00, 0x00, 0x20, 0x12} }
	/* Add new vendor data here */
};

/* need to forcely enable null pkt for such TV */
bool hdmitx_find_vendor_null_pkt(struct hdmitx_dev *hdev)
{
	int i;

	if (!hdev)
		return false;
	for (i = 0; i < ARRAY_SIZE(vendor_null_pkt); i++) {
		if (memcmp(&hdev->rawedid[8], vendor_null_pkt[i].data,
		    sizeof(vendor_null_pkt[i].data)) == 0)
			return true;
	}
	return false;
}
