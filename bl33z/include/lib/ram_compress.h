/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2018 Amlogic, Inc. All rights reserved.
*/

#ifndef __RAM_COMPRESS_H__
#define __RAM_COMPRESS_H__

#define COMPRESS_TAG_SIZE			16
#define COMPRESS_TAG_BL3Z			"AML_RAMDUMPBL3Z"
#define COMPRESS_TAG_HEAD			"AML_RAMDUMPHEAD"
#define COMPRESS_TAG_TAIL			"AML_RAMDUMPTAIL"

/* compress unit size */
#define SZ_1M						(1024 * 1024)
#define SELF_COMPRESS_THRESHOLD		(4 * SZ_1M)
#define MAX_COMPRESS_SEGMENTS		8

#define ECOMPRESS_FAILE				(-1L)
#define EINVALID_ARG				(-2L)
#define ECOMPRESS_OVERFLOW			(-3L)
#define NEED_RETRY					(-32L)

#define MAX_RETRY					8
#define RETRY_STEP					(2 * 1024 * 1024)

#include <stddef.h>
#include <platform_def.h>

/********************************************************************************
 * For reboot mode
 ********************************************************************************/
#define AMLOGIC_COLD_BOOT			0x00
#define AMLOGIC_NORMAL_BOOT			0x01
#define AMLOGIC_FACTORY_RESET_REBOOT	0x02
#define AMLOGIC_UPDATE_REBOOT		0x03
#define AMLOGIC_FASTBOOT_REBOOT		0x04
#define AMLOGIC_SUSPEND_REBOOT		0x05
#define AMLOGIC_HIBERNATE_REBOOT	0x06
#define AMLOGIC_BOOTLOADER_REBOOT	0x07
#define AMLOGIC_SHUTDOWN_REBOOT		0x08
#define AMLOGIC_RPMBP_REBOOT		0x09
#define AMLOGIC_CRASH_REBOOT		0x0b
#define AMLOGIC_KERNEL_PANIC		0x0c
#define AMLOGIC_WATCHDOG_REBOOT		0x0d

#define AMLOGIC_KERNEL_BOOTED		BIT(15)
#define AMLOGIC_USERAM_MASK		    (0xffff7fff)

#define CONFIG_COMPRESS_SECTION		7

//typedef unsigned int   u32;
//typedef unsigned short u16;
//typedef unsigned char  u8;

/********************************************************************************
 * For ramdump compress func
 ********************************************************************************/
enum {
	RAM_COMPRESS_NORMAL = 1,
	RAM_COMPRESS_COPY   = 2,
	RAM_COMPRESS_SET    = 3		/* set ram content to same vale */
};

enum {
	SECTION_ADDR_NORMAL = 1,	/* 0GB ~ 3.5GB */
	RECTION_ADDR_IS_REG	= 2,	/* 3.5GB ~ 4.0GB */
	SECTION_ADDR_EXTEND	= 3		/* 4.0GB ~ endGB */
};

struct ram_compress_section {
	void *phy_addr;
	unsigned long section_size;
	unsigned int section_index :  8;
	unsigned int compress_type :  8;
	unsigned int set_value     : 16;
	unsigned int copy_size_limit;
	void *copy_dest_addr;
};

struct ram_compress_full {
	void *store_phy_addr;
	unsigned long full_memsize;
	unsigned int section_count;
	struct ram_compress_section sections[MAX_COMPRESS_SEGMENTS];
};

struct ram_compress_work_info {
	void (*progress_update)(unsigned long processed,
				unsigned long total);
	void (*hear_beat)(void);
	void *temp_work_buffer;
	void *temp_output_buffer;
	unsigned int work_buffer_size;
	unsigned int output_buffer_size;
	unsigned int max_zip_section_size;
};

struct ram_compress_file_header {
	char ram_compress_tag[COMPRESS_TAG_SIZE];
	unsigned int section_count;
	unsigned long file_size;
};

struct ram_compress_section_header {
	unsigned long raw_size;
	unsigned long zip_size;
	unsigned int section_index : 8;
	unsigned int compress_type : 8;
	unsigned int set_value     :16;
};

struct ram_compress_info {
	unsigned long total_processed;
	unsigned long total_zip_size;
	unsigned long curr_processed;
	unsigned long curr_zip_size;
	unsigned char *output_fp;
	struct ram_compress_work_info *rcwi;
};

void aml_ramdump_compress(struct ram_compress_full *rcf,
	       void *arg, unsigned long flag, unsigned long ctrl);

unsigned long ram_compress_all(struct ram_compress_full *rcf,
			    struct ram_compress_work_info *rcwi);

unsigned long ramdump_compress_all(struct ram_compress_full *rcf,
			 void *arg,
			 unsigned long flag,
			 unsigned long ctrl,
			 unsigned long size);

int check_reboot_reason(unsigned long *mem_size);

void save_ramp_dump(unsigned long addr, long size);

#endif /* __RAM_COMPRESS_H__ */
