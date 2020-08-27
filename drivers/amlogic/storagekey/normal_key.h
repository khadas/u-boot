/*
 * drivers/amlogic/storagekey/normal_key.h
 *
 * Copyright (C) 2017 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#ifndef _AML_NORMAL_KEY_H_
#define _AML_NORMAL_KEY_H_

#define MAX_OBJ_NAME_LEN	80

/*Attribute*/
#define OBJ_ATTR_SECURE	BIT(0)
#define OBJ_ATTR_OTP	BIT(1)
#define OBJ_ATTR_ENC	BIT(8)

/* Type */
#define OBJ_TYPE_GENERIC	0xA00000BF

struct storage_object {
	char name[MAX_OBJ_NAME_LEN];
	u32 namesize;
	u32 attribute; /*secure, OTP*/
	u32 type; /*AES, RSA, GENERIC, ...*/
	u32 datasize;
	u8 *dataptr;
	u8 hashptr[32];
};

int normalkey_init(void);
void normalkey_deinit(void);
struct storage_object *normalkey_get(const u8 *name);
int normalkey_add(const u8 *name, u8 *buffer, u32 len, u32 attr);
int normalkey_del(const u8 *name);
int normalkey_readfromblock(void *block, unsigned long size);
int normalkey_writetoblock(void *block, unsigned long size);

#endif
