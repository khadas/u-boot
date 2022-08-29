/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __AMLKEY_IF_H__
#define __AMLKEY_IF_H__

#define AMLKEY_NAME_LEN_MAX     (80)

struct amlkey_if {
	int32_t (*init)(uint8_t *seed, uint32_t len, int encrypt_type);
	int32_t (*exsit)(const uint8_t *name);
	ssize_t (*size)(const uint8_t *name);
	uint32_t (*get_attr)(const uint8_t *name);
	ssize_t (*read)(const uint8_t *name, uint8_t *buffer, uint32_t len);
	ssize_t (*write)(const uint8_t *name, uint8_t *buffer, uint32_t len, uint32_t attr);
	int32_t (*hash)(const uint8_t *name, uint8_t *hash);
};

extern struct amlkey_if *amlkey_if;

static inline uint32_t amlkey_get_attr(const uint8_t *name)
{
	return amlkey_if->get_attr(name);
}

int amlkey_if_init(const void* dt_addr);

//1.init
static inline int32_t amlkey_init(uint8_t *seed, uint32_t len, int encrypt_type)
{
	return amlkey_if->init(seed, len, encrypt_type);
}

//2. query if the key already programmed, exsit 1, non 0
static inline int32_t amlkey_isexsit(const uint8_t * name)
{
	return amlkey_if->exsit(name);
}

//3. query attr, key must exsit before those functions were called.
	//3.1 whether the programmed key is secure, secure 1, non 0
int32_t amlkey_issecure(const uint8_t * name);

	//3.2 whether the programmed key is encrypt, encrypt 1, non 0
int32_t amlkey_isencrypt(const uint8_t * name);

//4. actual bytes of key value
static inline ssize_t amlkey_size(const uint8_t *name)
{
	return amlkey_if->size(name);
}

//5. read non-secure key in bytes, return byets readback actually.
static inline ssize_t amlkey_read(const uint8_t *name, uint8_t *buffer, uint32_t len)
{
	return amlkey_if->read(name, buffer, len);
}

//6.write key with attribute in bytes , return byets readback actually
	//attr: bit0: secure/non-secure;
	//		bit8: encrypt/non-encrypt
#define UNIFYKEY_ATTR_SECURE_MASK	(1<<0)
#define UNIFYKEY_ATTR_ENCRYPT_MASK	(1<<8)
static inline ssize_t amlkey_write(const uint8_t *name, uint8_t *buffer, uint32_t len,
				   uint32_t attr)
{
	return amlkey_if->write(name, buffer, len, attr);
}

//7. get the hash value of programmed secure key | 32bytes length, sha256
static inline int32_t amlkey_hash_4_secure(const uint8_t *name, uint8_t *hash)
{
	return amlkey_if->hash(name, hash);
}

#endif// #ifndef __AMLKEY_IF_H__

