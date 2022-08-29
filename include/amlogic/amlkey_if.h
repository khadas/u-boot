/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * include/amlogic/amlkey_if.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __AMLKEY_IF_H__
#define __AMLKEY_IF_H__

#define AMLKEY_NAME_LEN_MAX     (80)
//1.init
int32_t amlkey_init(uint8_t *seed, uint32_t len, int encrypt_type);

//2. query if the key already programmed, exsit 1, non 0
int32_t amlkey_isexsit(const uint8_t * name);

//3. query attr, key must exsit before those functions were called.
	//3.1 whether the programmed key is secure, secure 1, non 0
int32_t amlkey_issecure(const uint8_t * name);
	//3.2 whether the programmed key is encrypt, encrypt 1, non 0
int32_t amlkey_isencrypt(const uint8_t * name);

//4. actual bytes of key value
ssize_t amlkey_size(const uint8_t *name);

//5. read non-secure key in bytes, return byets readback actually.
ssize_t amlkey_read(const uint8_t *name, uint8_t *buffer, uint32_t len)	;

//6.write key with attribute in bytes , return byets readback actually
	//attr: bit0: secure/non-secure;
	//		bit8: encrypt/non-encrypt
#define UNIFYKEY_ATTR_SECURE_MASK	(1<<0)
#define UNIFYKEY_ATTR_ENCRYPT_MASK	(1<<8)
ssize_t amlkey_write(const uint8_t *name, uint8_t *buffer, uint32_t len, uint32_t attr);

//7. get the hash value of programmed secure key | 32bytes length, sha256
int32_t amlkey_hash_4_secure(const uint8_t * name, uint8_t * hash);

#endif// #ifndef __AMLKEY_IF_H__

