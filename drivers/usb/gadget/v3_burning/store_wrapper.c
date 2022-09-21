/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/usb/gadget/v3_burning/store_wrapper.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <storage.h>
#include <amlogic/storage_if.h>
#include <linux/mtd/mtd.h>

#define debugP(fmt...) //printf("Dbg[WRP]L%d:", __LINE__),printf(fmt)
#define errorP(fmt...) printf("Err[WRP]L%d:", __LINE__),printf(fmt)
#define wrnP(fmt...)   printf("wrn:"fmt)
#define MsgP(fmt...)   printf("[WRP]"fmt)

int store_read(const char *name, loff_t off, size_t size, void *buf)
{
    return store_read_ops((unsigned char*)name, buf, off, size);
}

int store_write(const char *name, loff_t off, size_t size, void *buf)
{
    return store_write_ops((unsigned char*)name, buf, off, size);
}

int store_logic_write(const char *name, loff_t off, size_t size, void *buf)
{
    return store_write_ops((unsigned char*)name, buf, off, size);
}

int store_logic_read(const char *name, loff_t off, size_t size, void *buf)
{
    return store_read_ops((unsigned char*)name, buf, off, size);
}

//get partition logic size
u64 store_logic_cap(const char* partName)
{
    u64 cap = 0;
	int ret = store_get_partition_size((unsigned char *)partName, &cap);
    return ret ? 0 : cap*512;
}

u32 store_rsv_size(const char *name)
{
    if (!strcmp("dtb", name)) {
        return 256 * 1024;
    }

    MsgP("unsupported rsv %s\n", name);
    return 0;
}

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
int store_rsv_read(const char *name, size_t size, void *buf)
{
    if (strcmp("dtb", name)) {
        errorP("unsupported name %s\n", name);
        return -__LINE__;
    }

    return store_dtb_rw(buf, size, 2);
}

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
int store_rsv_write(const char *name, size_t size, void *buf)
{
    if (strcmp("dtb", name)) {
        errorP("unsupported name %s\n", name);
        return -__LINE__;
    }

    return store_dtb_rw(buf, size, 1);
}

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
int store_rsv_erase(const char *name)
{
    if (!strcmp("dtb", name)) {
        return run_command("store erase dtb", 0);
    } else if (!strcmp("key", name)) {
        return run_command("store erase key", 0);
    }

    errorP("unsupported name %s\n", name);
    return -__LINE__;
}

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
int store_rsv_protect(const char *name, bool on)
{
    if (!name && !on) {
        return run_command("store disprotect key; store disprotect bbt", 0);
    }
    if (!name) {
        errorP("store_rsv_protect null invalid");
        return -__LINE__;
    }
    char cmd[128];
    snprintf(cmd, 128, "store %s %s", on ? "protect" : "disprotect", name);
    MsgP("runcmd:%s\n", cmd);
    return run_command(cmd, 0);
}


