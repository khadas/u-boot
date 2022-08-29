// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

/* extern from bl31 */
/*
 * when RET_OK
 * query: retval=1: key exsit,=0: key not exsit；
 * tell: retvak = key size
 * status: retval=1: secure, retval=0: non-secure

 */

#include <common.h>
#include <linux/types.h>
#include <amlogic/secure_storage.h>
#include <amlogic/amlkey_if.h>
#include <amlogic/storage.h>
#include <linux/libfdt.h>
#ifdef CONFIG_STORE_COMPATIBLE
#include <partition_table.h>
#endif
#include "normal_key.h"

/* key buffer status */
/* bit0, dirty flag*/
#define KEYBUFFER_CLEAN		(0 << 0)
#define KEYBUFFER_DIRTY		(1 << 0)
#define SECUESTORAGE_HEAD_SIZE		(256)
#define SECUESTORAGE_WHOLE_SIZE		(0x40000)

struct storagekey_info_t {
	uint8_t * buffer;
	uint32_t size;
	uint32_t status;
};

static struct storagekey_info_t storagekey_info = {
	.buffer = NULL,
	/* default size */
	.size = SECUESTORAGE_WHOLE_SIZE,
	.status = KEYBUFFER_CLEAN,
};

/**
 *1.init
 * return ok 0, fail 1
 */
static int32_t _amlkey_init(uint8_t *seed, uint32_t len, int encrypt_type)
{
	int32_t ret = 0;
	uint32_t buffer_size, actual_size;

	/* do nothing for now*/
	pr_info("%s() enter!\n", __func__);
	if (storagekey_info.buffer != NULL) {
		pr_info("%s() %d: already init!\n", __func__, __LINE__);
		goto _out;
	}

	/* get buffer from bl31 */
	storagekey_info.buffer = secure_storage_getbuffer(&buffer_size);
	if (storagekey_info.buffer == NULL) {
		pr_info("%s() %d: can't get buffer from bl31!\n",
			__func__, __LINE__);
		ret = -1;
		goto _out;
	}

	if (encrypt_type == -1)
		encrypt_type = 0;
	secure_storage_set_enctype(encrypt_type);
	actual_size = store_rsv_size("key");
	pr_info("%s %d actual_size: 0x%x\n", __func__, __LINE__,
		actual_size);

	storagekey_info.size = min_t(uint32_t, actual_size, buffer_size);
	ret = store_rsv_read("key", storagekey_info.size, storagekey_info.buffer);
	/* When the key is invalid at the first burn, it should be initialized again. */
	if (ret == RSV_UNVAIL)
		ret = 0;
	if (ret) {
		pr_info("amlkey init rsv read key fail\n");
		/* memset head info for bl31 */
		memset(storagekey_info.buffer, 0, SECUESTORAGE_HEAD_SIZE);
		ret = 0;
		goto _out;
	}
#if CONFIG_AML_FLUSH_CACHE
	flush_cache((unsigned long)storagekey_info.buffer, buffer_size);
#endif
	secure_storage_notifier_ex(storagekey_info.size, 0);

	storagekey_info.buffer = secure_storage_getbuffer(&buffer_size);
	if (buffer_size != storagekey_info.size) {
		ret = -1;
		goto _out;
	}

#ifdef CONFIG_STORE_COMPATIBLE
	info_disprotect &= ~DISPROTECT_KEY;  //protect
#endif
_out:
	return ret;
}

/**
 *2. query if the key already programmed
 * return: exsit 1, non 0
 */
static int32_t _amlkey_isexsit(const uint8_t * name)
{
	int32_t ret = 0;
	uint32_t retval;

	if ( NULL == name ) {
		printf("%s() %d, invalid key ", __func__, __LINE__);
		return 0;
	}

	ret = secure_storage_query((uint8_t *)name, &retval);
	if (ret) {
		printf("%s() %d: ret %d\n", __func__, __LINE__, ret);
		retval = 0;
	}

	return (int32_t)retval;
}

static uint32_t _amlkey_get_attr(const uint8_t * name)
{
	int32_t ret = 0;
	uint32_t retval;

	if ( NULL == name ) {
		printf("%s() %d, invalid key ", __func__, __LINE__);
		return 0;
	}

	ret = secure_storage_status((uint8_t *)name, &retval);
	if (ret) {
		printf("%s() %d: ret %d\n", __func__, __LINE__, ret);
		retval = 0;
	}

	return retval;
}

/**
 * 3.1 query if the programmed key is secure. key must exsit!
 * return secure 1, non 0;
 */
int32_t amlkey_issecure(const uint8_t * name)
{
	return (amlkey_get_attr(name)&UNIFYKEY_ATTR_SECURE_MASK);
}

/**
 * 3.2 query if the programmed key is encrypt
 * return encrypt 1, non-encrypt 0;
 */
int32_t amlkey_isencrypt(const uint8_t * name)
{
	return (amlkey_get_attr(name)&UNIFYKEY_ATTR_ENCRYPT_MASK);
}
/**
 * 4. actual bytes of key value
 *  return actual size.
 */
static ssize_t _amlkey_size(const uint8_t *name)
{
	ssize_t size = 0;
	int32_t ret = 0;
	uint32_t retval;

	if ( NULL == name ) {
		printf("%s() %d, invalid key ", __func__, __LINE__);
		return 0;
	}

	ret = secure_storage_tell((uint8_t *)name, &retval);
	if (ret) {
		printf("%s() %d: ret %d\n", __func__, __LINE__, ret);
		retval = 0;
	}
	size = (ssize_t)retval;
	return size;
}

/**
 *5. read non-secure key in bytes, return bytes readback actually.
 * return actual size read back.
 */
static ssize_t _amlkey_read(const uint8_t *name, uint8_t *buffer, uint32_t len)
{
	int32_t ret = 0;
	ssize_t retval = 0;
	uint32_t actul_len;

	if ( NULL == name ) {
		printf("%s() %d, invalid key ", __func__, __LINE__);
		return 0;
	}
	ret = secure_storage_read((uint8_t *)name, buffer, len, &actul_len);
	if (ret) {
		printf("%s() %d: return %d\n", __func__, __LINE__, ret);
		retval = 0;
		goto _out;
	}
	retval = actul_len;
_out:
	return retval;
}

/**
 * 6.write secure/non-secure key in bytes , return bytes readback actually
 * attr: bit0, secure/non-secure;
 *		 bit8, encrypt/non-encrypt;
 * return actual size write down.
 */
static ssize_t _amlkey_write(const uint8_t *name, uint8_t *buffer, uint32_t len, uint32_t attr)
{
	int32_t ret = 0;
	ssize_t retval = 0;
	//uint32_t actual_size;

	if ( NULL == name ) {
		printf("%s() %d, invalid key ", __func__, __LINE__);
		return retval;
	}
	ret = secure_storage_write((uint8_t *)name, buffer, len, attr);
	if (ret) {
		printf("%s() %d: return %d\n", __func__, __LINE__, ret);
		retval = 0;
		goto _out;
	} else {
		retval = (ssize_t)len;
		/* write down! */
		if (storagekey_info.buffer != NULL) {
            ret = store_rsv_write("key", storagekey_info.size, storagekey_info.buffer);
			if (ret) {
				printf("%s() %d, store_key_write fail\n",
					__func__, __LINE__);
				retval = 0;
			}
			//actual_size = storagekey_info.size;
		}
	}
_out:
	return retval;
}
/**
 * 7. get the hash value of programmed secure key | 32bytes length, sha256
 * return success 0, fail -1
 */
static int32_t _amlkey_hash_4_secure(const uint8_t * name, uint8_t * hash)
{
	int32_t ret = 0;

	ret = secure_storage_verify((uint8_t *)name, hash);

	return ret;
}

/**
 * 7. del key by name
 * return success 0, fail -1
 */
static int32_t __maybe_unused _amlkey_del(const uint8_t * name)
{
	int32_t ret = 0;

	ret = secure_storage_remove((uint8_t *)name);
	if ((ret == 0) && (storagekey_info.buffer != NULL)) {
		/* flush back */
        ret = store_rsv_write("key", storagekey_info.size, storagekey_info.buffer);
		if (ret) {
			printf("%s() %d, store_key_write fail\n",
				__func__, __LINE__);
		}
	} else {
		printf("%s() %d, remove key fail\n",
			__func__, __LINE__);
	}

	return ret;
}



#define DEFINE_MUTEX(...)
#define mutex_lock(...)
#define mutex_unlock(...)
#define PAGE_ALIGNED(a) (((a)&0xFFF)==0)
#undef pr_info
#define pr_info(fmt ...)     printf("[KM]Msg:"fmt)
#undef pr_err
#define pr_err(fmt ...)     printf("[KM]Error:f[%s]L%d:", __func__, __LINE__),printf(fmt)

#define DEF_NORMAL_BLOCK_SIZE	(256 * 1024)
//static DEFINE_MUTEX(normalkey_lock);
static uint32_t normal_blksz = DEF_NORMAL_BLOCK_SIZE;
static uint32_t normal_flashsize = DEF_NORMAL_BLOCK_SIZE;
static uint8_t *normal_block;

static int _store_key_read(uint8_t * buffer, uint32_t length, uint32_t *actual_lenth)
{
	int ret;
	uint32_t actual_size;
	actual_size = store_rsv_size("key");
	if (actual_size <= 1024)
		return -1;

	length = min_t(uint32_t, actual_size, length);
	ret = store_rsv_read("key", length, buffer);
	/* When the key is invalid at the first burn, it should be initialized again. */
	if (ret && ret != RSV_UNVAIL) {
		printf("amlkey init rsv read key faill\n");
		return -1;
	}
	if (actual_lenth)
		*actual_lenth = length;
	return 0;
}
static int _store_key_write(uint8_t * buffer, uint32_t length, uint32_t *actual_lenth)
{
	int32_t ret;
	ret = store_rsv_write("key", length, buffer);
	if (ret)
		return -1;
	if (actual_lenth)
		*actual_lenth = length;
	return 0;
}

static int32_t _amlkey_init_normal(uint8_t *seed, uint32_t len, int encrypt_type)
{
	static int inited = 0;
	int ret;

	if (inited)
		return 0;
	inited = 1;

	if (!normal_block)
		return -1;

	if (normalkey_init())
		return -1;

	mutex_lock(&normalkey_lock);
	ret = _store_key_read(normal_block,
			     normal_blksz,
			     &normal_flashsize);
	if (ret) {
		pr_err("read storage fail\n");
		goto finish;
	}

	ret = normalkey_readfromblock(normal_block, normal_flashsize);
	if (ret) {
		pr_err("init block key fail\n");
		goto finish;
	}

#ifdef CONFIG_STORE_COMPATIBLE
	info_disprotect &= ~DISPROTECT_KEY;  //protect
#endif
	ret = 0;
finish:
	if (ret)
		normalkey_deinit();
	mutex_unlock(&normalkey_lock);

	return ret;
}

static int32_t _amlkey_exist_normal(const uint8_t *name)
{
	struct storage_object *obj;

	mutex_lock(&normalkey_lock);
	obj = normalkey_get(name);
	mutex_unlock(&normalkey_lock);

	return !!obj;
}

static uint32_t _amlkey_get_attr_normal(const uint8_t *name)
{
	uint32_t attr = 0;
	struct storage_object *obj;

	mutex_lock(&normalkey_lock);
	obj = normalkey_get(name);
	if (obj)
		attr = obj->attribute;
	mutex_unlock(&normalkey_lock);

	return attr;
}

static ssize_t _amlkey_size_normal(const uint8_t *name)
{
	unsigned int size = 0;
	struct storage_object *obj;

	mutex_lock(&normalkey_lock);
	obj = normalkey_get(name);
	if (obj)
		size = obj->datasize;
	mutex_unlock(&normalkey_lock);

	return size;
}

static ssize_t _amlkey_read_normal(const uint8_t *name, uint8_t *buffer, uint32_t len)
{
	unsigned int size = 0;
	struct storage_object *obj;

	mutex_lock(&normalkey_lock);
	obj = normalkey_get(name);
	if (obj && len >= obj->datasize) {
		size = obj->datasize;
		memcpy(buffer, obj->dataptr, size);
	}
	mutex_unlock(&normalkey_lock);

	return size;
}

static ssize_t _amlkey_write_normal(const uint8_t *name, uint8_t *buffer,
				    uint32_t len, uint32_t attr)
{
	int ret;
	uint32_t wrtsz = 0;

	if (attr & OBJ_ATTR_SECURE) {
		pr_err("can't write secure key\n");
		return 0;
	}

	mutex_lock(&normalkey_lock);
	ret = normalkey_add(name, buffer, len, attr);
	if (ret) {
		pr_err("write key fail\n");
		ret = 0;
		goto unlock;
	}

	ret = normalkey_writetoblock(normal_block, normal_flashsize);
	if (ret) {
		pr_err("write block fail\n");
		ret = 0;
		goto unlock;
	}

	ret = _store_key_write(normal_block,
			      normal_flashsize,
			      &wrtsz);
	if (ret) {
		pr_err("write storage fail\n");
		ret = 0;
		goto unlock;
	}
	ret = len;
unlock:
	mutex_unlock(&normalkey_lock);
	return ret;
}

static int32_t _amlkey_hash_normal(const uint8_t *name, uint8_t *hash)
{
	int ret = -1;
	struct storage_object *obj;

	mutex_lock(&normalkey_lock);
	obj = normalkey_get(name);
	if (obj) {
		ret = 0;
		memcpy(hash, obj->hashptr, 32);
	}
	mutex_unlock(&normalkey_lock);

	return ret;
}

int normal_key_init(const void* dt_addr)
{
	uint32_t blksz = 0;
	int nodeoffset;
	char *pblksz;

	if (!dt_addr || fdt_check_header(dt_addr)!= 0) {
		goto blkalloc;
	}

	nodeoffset = fdt_path_offset(dt_addr, "/unifykey");
	if (nodeoffset < 0) {
		goto blkalloc;
	}

	pblksz = (char*)fdt_getprop((const void *)dt_addr, nodeoffset, "blocksize",NULL);
	if (pblksz) {
		blksz = be32_to_cpup((unsigned int*)pblksz);
	}

	if (blksz && PAGE_ALIGNED(blksz)) {
		normal_blksz = blksz;
		pr_info("block size from config: %x\n", blksz);
	}

blkalloc:
	normal_block = malloc(normal_blksz);
	if (!normal_block) {
		pr_err("malloc block buffer fail: %x\n", normal_blksz);
		return -1;
	}

	return 0;
}

enum amlkey_if_type {
	IFTYPE_SECURE_STORAGE,
	IFTYPE_NORMAL_STORAGE,
	IFTYPE_MAX
};

struct amlkey_if amlkey_ifs[] = {
	[IFTYPE_SECURE_STORAGE] = {
		.init = _amlkey_init,
		.exsit = _amlkey_isexsit,
		.get_attr = _amlkey_get_attr,
		.size = _amlkey_size,
		.read = _amlkey_read,
		.write = _amlkey_write,
		.hash = _amlkey_hash_4_secure,
	},
	[IFTYPE_NORMAL_STORAGE] = {
		.init = _amlkey_init_normal,
		.exsit = _amlkey_exist_normal,
		.get_attr = _amlkey_get_attr_normal,
		.size = _amlkey_size_normal,
		.read = _amlkey_read_normal,
		.write = _amlkey_write_normal,
		.hash = _amlkey_hash_normal,
	}
};

struct amlkey_if *amlkey_if = &amlkey_ifs[IFTYPE_SECURE_STORAGE];
int amlkey_if_init(const void* dt_addr)
{
	static int inited = 0;
	uint32_t buffer_size;
	int ret = 0;

	if (inited)
		return 0;
	inited = 1;

	if (secure_storage_getbuffer(&buffer_size)) {
		amlkey_if = &amlkey_ifs[IFTYPE_SECURE_STORAGE];
		return ret;
	}

	pr_info("normal key used!\n");
	ret = normal_key_init(dt_addr);
	amlkey_if = &amlkey_ifs[IFTYPE_NORMAL_STORAGE];

	return ret;
}
