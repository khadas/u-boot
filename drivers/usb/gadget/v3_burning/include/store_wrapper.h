/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/usb/gadget/v3_burning/include/store_wrapper.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __STORE_WRAPPER_H__
#define __STORE_WRAPPER_H__
#include <amlogic/storage_if.h>
#include <partition_table.h>

int store_read(const char *name, loff_t off, size_t size, void *buf);
int store_write(const char *name, loff_t off, size_t size, void *buf);

//logic write/read, if not MTD, same as store_write
int store_logic_write(const char *name, loff_t off, size_t size, void *buf);

int store_logic_read(const char *name, loff_t off, size_t size, void *buf);

u64 store_logic_cap(const char* partName);

u32 store_rsv_size(const char *name);

/**
 * @usage: read the rsv info from storage device
 *
 * @name: rsv info name, please refer to
 *        RSV_KEY   "key"
 *        RSV_ENV   "env"
 *        RSV_DTB   "dtb"
 *        RSV_BBT   "bbt"
 * @size: the amount of bytes to read
 * @buf: pointer of the target buffer
 *
 * @return: result of the operation
 *          0 = success
 *          other = fail
 */
int store_rsv_read(const char *name, size_t size, void *buf);

/**
 * @usage: write the rsv info to the storage device
 *
 * @name: rsv info name, please refer to
 *        RSV_KEY   "key"
 *        RSV_ENV   "env"
 *        RSV_DTB   "dtb"
 *        RSV_BBT   "bbt"
 * @size: the amount of bytes to write
 * @buf: pointer of the source buffer
 *
 * @return: result of the operation
 *          0 = success
 *          other = fail
 */
int store_rsv_write(const char *name, size_t size, void *buf);

/**
 * @usage: erase the rsv info
 *
 * @name: rsv info name, please refer to
 *        RSV_KEY   "key"
 *        RSV_ENV   "env"
 *        RSV_DTB   "dtb"
 *        RSV_BBT   "bbt"
 *        it will erase all reserve information
 *        when name is null
 *
 * @return: result of the operation
 *          0 = success
 *          other = fail
 */
int store_rsv_erase(const char *name);

/**
 * @usage: turn on/off the protection of rsv info
 *
 * @name: rsv info name, please refer to
 *        RSV_KEY   "key"
 *        RSV_ENV   "env"
 *        RSV_DTB   "dtb"
 *        RSV_BBT   "bbt"
 *        it will operates all reserve information
 *        when name is null
 * @ops: turn on/off the rsv info protection
 *       true = turn on the protection
 *       flase = turn off the protection
 *
 * @return: result of the operation
 *          0 = success
 *          other = fail
 */
int store_rsv_protect(const char *name, bool ops);

#ifndef store_get_type
#define store_get_type() device_boot_flag
#define BOOT_EMMC       EMMC_BOOT_FLAG
#define BOOT_SD         CARD_BOOT_FLAG
#define BOOT_NAND_NFTL  NAND_BOOT_FLAG
#endif//#ifndef store_get_type

#define AML_MAGIC_HDR_L       (0x4c4d4140) //"@AML"
#define AML_MAGIC_HDR_R       (0x544f4f42) //"BOOT"

#define AML_MAGIC_CORE        (0X54534242) //"BBST"
#define AML_MAGIC_BL2E        (0x45324c42) //"BL2E"
#define AML_MAGIC_BL2X        (0x58324c42) //"BL2X"
#define AML_MAGIC_DDRF        (0x46524444) //"DDRF"
#define AML_MAGIC_DEVF        (0x46564544) //"DEVF"
typedef struct{
	unsigned int  nMagic;          //"BBST", "BL2E", "BL2X", "DDRF", "DEVF"
	unsigned int  nOffset;         //offset from file head of file
	unsigned int  nPayLoadSize;    //size of payload
	unsigned int  nReserved;       //reserved for future
}payload_info_item_t, *p_payload_info_item_t;

typedef struct{
	unsigned char  szSHA2[32];     //sha256 of sizeof(payload_info_t) - sizeof(szSHA2)
	unsigned int   nMagicL;        //"@AML"
	unsigned int   nMagicR;        //"BOOT"
	unsigned char  byVersion;      //version:0,1,2,....
	unsigned char  byItemNum;      //number of items
	unsigned short nSize;          //sizeof(this) = sizeof(hdr) + sizeof(item) * byItemNum
	unsigned char  szReserved1[4];
	char           szTimeStamp[16];//"SC2-YYYYMMDDHHmm" e.g "SC2-202007082259"

}payload_info_hdr_t, *p_payload_info_hdr_t;

typedef struct{
	payload_info_hdr_t  hdr;         //header
	payload_info_item_t arrItems[1]; //items
}payload_info_t, *p_payload_info_t;

#endif//#ifndef __STORE_WRAPPER_H__

