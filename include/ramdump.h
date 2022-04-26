/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __RAMDUMP_H__
#define __RAMDUMP_H__

#define RAMDUMP_STICKY_DATA_MASK		(0xFFFF)

extern void check_ramdump(void);
extern void ramdump_init(void);

extern unsigned long ramdump_base;
extern unsigned long ramdump_size;

int ramdump_save_compress_data(void);

#endif /* __RAMDUMP_H__ */
