/*
 * \file        store_wrapper.h
 * \brief       wraaper for storage apis
 *
 * \version     1.0.0
 * \date        2019/5/30
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2019 Amlogic. All Rights Reserved.
 *
 */
#ifndef __STORE_WRAPPER_H__
#define __STORE_WRAPPER_H__
#include <amlogic/storage.h>

//logic write/read, if not MTD, same as store_write
int store_logic_write(const char *name, loff_t off, size_t size, void *buf);

int store_logic_read(const char *name, loff_t off, size_t size, void *buf);

u64 store_logic_cap(const char* partName);

#endif//#ifndef __STORE_WRAPPER_H__

