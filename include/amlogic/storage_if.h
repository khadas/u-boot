/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * include/amlogic/storage_if.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __STORAGE_IF_H__
#define __STORAGE_IF_H__


/***
upgrade_read_ops:

partition_name: env / logo / recovery /boot / system /cache /media

***/
int store_read_ops(unsigned char *partition_name,unsigned char * buf, uint64_t off, uint64_t size);


/***
upgrade_write_ops:

partition_name: env / logo / recovery /boot / system /cache /media

***/
int store_write_ops(unsigned char *partition_name,unsigned char * buf,uint64_t off, uint64_t size);


/***
upgrade_write_ops:

partition_name: env / logo / recovery /boot / system /cache /media

***/
int store_get_partition_size(unsigned char *partition_name, uint64_t *size);


/***
upgrade_erase_ops:

partition_name: boot / data

flag = 0; indicate erase partition ;
flag = 1; indicate scurb whole nand;

***/
int store_erase_ops(unsigned char *par_name, uint64_t off, uint64_t size, unsigned char flag);

/***
bootloader:
***/
int store_boot_read(unsigned char * buf, uint64_t off, uint64_t size);

int store_boot_write(unsigned char * buf,uint64_t off, uint64_t size);

int store_init(unsigned  flag);

int store_exit(void);

//dtb read/write
//@rwFlag: 0---read, 1---write, 2---iread
int store_dtb_rw(void* buf, unsigned dtbSz, int rwFlag);

//key read/write
int store_key_read(uint8_t * buffer,
			uint32_t length, uint32_t *actual_lenth);
int store_key_write(uint8_t * buffer,
			uint32_t length, uint32_t *actual_lenth);

extern int check_valid_dts(unsigned char *buffer, unsigned char **dts);

#endif//ifndef __STORAGE_IF_H__

