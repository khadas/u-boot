/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __RAMDUMP_H__
#define __RAMDUMP_H__

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT              12
#define PAGE_SIZE               ((1) << PAGE_SHIFT)

/* align addr on a size boundary - adjust address up/down if needed */
#define _ALIGN_UP(addr, size)	(((addr) + ((size) - 1)) & (~((typeof(addr))(size) - 1)))
#define _ALIGN_DOWN(addr, size)	((addr) & (~((typeof(addr))(size) - 1)))

/* align addr on a size boundary - adjust address up if needed */
#define _ALIGN(addr, size)      _ALIGN_UP(addr, size)

/* to align the pointer to the (next) page boundary */
#define PAGE_ALIGN(addr)        _ALIGN(addr, PAGE_SIZE)

#define RAMDUMP_STICKY_DATA_MASK		(0xFFFF)

extern void check_ramdump(void);
extern void ramdump_init(void);

extern unsigned long ramdump_base;
extern unsigned long ramdump_size;

int ramdump_save_compress_data(void);

#endif /* __RAMDUMP_H__ */
