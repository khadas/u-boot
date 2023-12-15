/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __AML_PAGEINFO_H_
#define __AML_PAGEINFO_H_
#include <linux/mtd/mtd.h>

int page_info_pre_init(void);
bool page_info_is_page(int page);
unsigned char *page_info_post_init(struct mtd_info *mtd, struct udevice *dev);
#endif
