/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __RAMDUMP_PARA_H__
#define __RAMDUMP_PARA_H__

#include <config.h>
#ifdef CONFIG_MDUMP_COMPRESS
#define CONFIG_COMPRESS_SECTION		6

#if CONFIG_COMPRESS_SECTION > 8
#error ---> CONFIG_COMPRESS_SECTION out of range, max should be 8
#endif
/*
 *   Full Memory lay out for RAM compress:
 *
 *                DDR_TOP -> +--------+
 *                           |        |
 *                           |        |
 *                           |   6    |
 *                           |        |
 *                           |        |
 *                           |~~~~~~~~| <- store compressing data
 *                           |~~~~~~~~|
 *                           |~~~~~~~~|
 *                           |~~~~~~~~|
 *                           |~~~~~~~~|
 *                           |~~~~~~~~|
 *                           |~~~~~~~~|
 *                           |~~~~~~~~|
 *                           |~~~~~~~~|
 *  COMPRESSED_DATA start -> +--------+
 *                           |        |
 *                           |   5    |
 *                           |        |
 *     BL32_LOAD_ADDR end -> +--------+ -- IGNORE_SIZE
 *                           ||||||||||
 *                           ||||4||||| <- load bl31 bl32(3+48 MB)
 *                           ||||||||||
 *   BL31_LOAD_ADDR start -> +--------+
 *                           |        |
 *                           |   3    |
 *                           |        |
 *     BL2E_LOAD_ADDR end -> +--------+
 *                           ||||||||||
 *                           ||||2||||| <- load bl2x bl2e
 *                           ||||||||||
 *   BL2X_LOAD_ADDR start -> +--------+
 *                           |        |
 *                           |   1    |
 *                           |        |
 *    COMPRESS_START_ADDR -> +--------+
 *
 */

//#define	CONFIG_DDR_SIZE 			2048
//#define CONFIG_DDR_TOTAL_SIZE		(CONFIG_DDR_SIZE << 20)
#define CONFIG_DDR_TOTAL_SIZE		(0x80000000)		/* ddr size: 2GB*/
#define CONFIG_COMPRESSED_DATA_ADDR	(0x0c000000)		/* section4 start addr */

#define CONFIG_COMPRESS_START_ADDR	(0x00000000)		/* ddr start addr */
#define CONFIG_BL2X_LOAD_ADDR		(0x00800000)		/* bl2x start addr */
#define CONFIG_BL2E_END_ADDR		(0x01280000)
#define CONFIG_BL31_LOAD_ADDR		(0x05000000)		/* bl31 start addr */
#define CONFIG_BL31_LOAD_SIZE		(0x00300000)		/* bl31 size: 3MB */
#define CONFIG_BL32_LOAD_ADDR		(CONFIG_BL31_LOAD_ADDR + CONFIG_BL31_LOAD_SIZE)
#define CONFIG_BL32_LOAD_SIZE		(0x03000000)		/* bl32 size: 48MB (sc2, rsvmem dump)*/
//#define CONFIG_ACS_DDR_LOAD_ADDR	(CONFIG_BL32_LOAD_ADDR)		/* bl2/core copy acs para form sdrm to ddr */
#define CONFIG_ACS_DDR_LOAD_ADDR	(0x05200000)
#define	CONFIG_ACS_DDR_LOAD_SIZE	(0x00000200)				/* 512 Bytes */


#define CONFIG_SECTION0_ADDR		(CONFIG_COMPRESSED_DATA_ADDR)	/* compress begin here */
#define CONFIG_SECTION0_SIZE		(CONFIG_DDR_TOTAL_SIZE - CONFIG_COMPRESSED_DATA_ADDR)
#define CONFIG_SECTION1_ADDR		(CONFIG_COMPRESS_START_ADDR)	/* 0 */
#define CONFIG_SECTION1_SIZE		(CONFIG_BL2X_LOAD_ADDR - CONFIG_SECTION1_ADDR)			/* 8MB */
#define CONFIG_SECTION2_ADDR		(CONFIG_BL2X_LOAD_ADDR)			/* BL2x start addr */
#define CONFIG_SECTION2_SIZE		(CONFIG_BL2E_END_ADDR - CONFIG_SECTION2_ADDR)
#define CONFIG_SECTION3_ADDR		(CONFIG_BL2E_END_ADDR)
#define CONFIG_SECTION3_SIZE		(CONFIG_BL31_LOAD_ADDR - CONFIG_SECTION3_ADDR)
#define CONFIG_SECTION4_ADDR		(CONFIG_BL31_LOAD_ADDR)
#define CONFIG_SECTION4_SIZE		(CONFIG_BL31_LOAD_SIZE + CONFIG_BL32_LOAD_SIZE) 	/* 3MB + 48MB */
#define CONFIG_SECTION5_ADDR		(CONFIG_SECTION4_ADDR + CONFIG_SECTION4_SIZE)
#define CONFIG_SECTION5_SIZE		(CONFIG_COMPRESSED_DATA_ADDR - CONFIG_SECTION5_ADDR)

#define CONFIG_BL2_IGNORE_ADDR		(0x05000000)
#define CONFIG_BL2_IGNORE_SIZE		(0x00300000)
#define CONFIG_SEG4_ADDR			(CONFIG_BL2_IGNORE_ADDR + CONFIG_BL2_IGNORE_SIZE)


/* For ram dump */
#define FM_FIP_BL2Z_TEMP_LOAD_ADDR		0x00700000
//#define FM_BL2Z_PARA_ADDR				(BL2_DUMP_PARA_OFFSET + TZRAM_BASE) /* 0x13008 + 0xFFFB0000 = 0xfffc3008*/
//#define FM_BL2Z_PARA_ADDR				(BL2_DUMP_PARA_OFFSET + TZRAM_BASE) /* 0xF008 + 0xF7000000 = 0xF700F008*/
#define FM_BL2Z_PARA_ADDR				0xF7008d80
#define FM_BL2Z_EXEC_ADDR				0x05200000
#define AMLOGIC_KERNEL_PANIC			0x0c
#define AMLOGIC_WATCHDOG_REBOOT			0x0d

enum {
	RAM_COMPRESS_NORMAL = 1,
	RAM_COMPRESS_COPY   = 2,
	RAM_COMPRESS_SET    = 3		/* set ram content to same vale */
};

struct ram_compress_section {
	void *phy_addr;
	unsigned int section_size;
	unsigned int section_index :  8;
	unsigned int compress_type :  8;
	unsigned int set_value     : 16;
	void *copy_dest_addr;
	unsigned int copy_size_limit;
};

typedef struct ram_compress_full {
	void *store_phy_addr;
	unsigned int full_memsize;
	unsigned int section_count;
	struct ram_compress_section sections[CONFIG_COMPRESS_SECTION];
} __attribute__ ((packed)) ram_compress_full_t;

#endif
#endif /* __RAMDUMP_PARA_H__ */
