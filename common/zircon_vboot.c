// SPDX-License-Identifier:     BSD-3-Clause
/*
 * Copyright (c) 2019 The Fuchsia Authors
 *
 */

#include <common.h>

#include <asm/arch/efuse.h>
#include <asm/arch/secure_apb.h>
#include <asm/io.h>
#include <libavb.h>
#ifdef CONFIG_LIBAVB_ATX
#include <libavb_atx/libavb_atx.h>
#include <tee/ta_vx_helper.h>
#else
#include <emmc_partitions.h>
#include <amlogic/storage.h>
#include <asm/arch/bl31_apis.h>
#if defined(CONFIG_AML_ANTIROLLBACK) || defined(CONFIG_AML_AVB2_ANTIROLLBACK)
#include <amlogic/anti-rollback.h>
#endif
#include <version.h>
#include <amlogic/aml_efuse.h>
#include <amlogic/store_wrapper.h>
#endif
#include <zircon/partition.h>
#include <zircon/vboot.h>
#include <zircon/zircon.h>

/* By convention, when a rollback index is not used, the value remains zero. */
#define ROLLBACK_INDEX_NOT_USED (0)

static uint8_t *preloaded_img_addr;
static uint64_t preloaded_img_size;
static uint64_t key_versions[AVB_MAX_NUMBER_OF_ROLLBACK_INDEX_LOCATIONS] = { 0 };

/* If a negative offset is given, computes the unsigned offset. */
static inline int64_t calc_offset(uint64_t size, int64_t offset)
{
	if (offset < 0)
		return size + offset;

	return offset;
}

static AvbIOResult read_from_partition(AvbOps *ops, const char *partition,
				       int64_t offset, size_t num_bytes,
				       void *buffer, size_t *out_num_read)
{
	int rc;
	uint64_t size;
	int64_t abs_offset;

	rc = zircon_get_partition_size(partition, &size);
	if (rc)
		return AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;

	abs_offset = calc_offset(size, offset);
	if (abs_offset > size || abs_offset < 0)
		return AVB_IO_RESULT_ERROR_RANGE_OUTSIDE_PARTITION;

	if ((abs_offset + num_bytes) > size)
		num_bytes = size - abs_offset;

	rc = zircon_partition_read(partition, abs_offset,
				   (unsigned char *)buffer, num_bytes);

	if (rc)
		return AVB_IO_RESULT_ERROR_IO;

	*out_num_read = num_bytes;
	return AVB_IO_RESULT_OK;
}

#ifdef CONFIG_LIBAVB_ATX
static AvbIOResult get_preloaded_partition(AvbOps *ops, const char *partition,
					   size_t num_bytes,
					   uint8_t **out_pointer,
					   size_t *out_num_bytes_preloaded)
{
	*out_pointer = NULL;
	*out_num_bytes_preloaded = 0;

	if (!strncmp(partition, ZIRCON_PARTITION_PREFIX,
		     strlen(ZIRCON_PARTITION_PREFIX))) {
		*out_pointer = preloaded_img_addr;

		if (num_bytes <= preloaded_img_size)
			*out_num_bytes_preloaded = num_bytes;
		else
			*out_num_bytes_preloaded = preloaded_img_size;
	}
	return AVB_IO_RESULT_OK;
}
#endif

static AvbIOResult write_to_partition(AvbOps *ops, const char *partition,
				      int64_t offset, size_t num_bytes,
				      const void *buffer)
{
	int rc;
	uint64_t size;
	int64_t abs_offset;

	rc = zircon_get_partition_size(partition, &size);
	if (rc)
		return AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;

	abs_offset = calc_offset(size, offset);
	if (abs_offset < 0 || (abs_offset + num_bytes > size))
		return AVB_IO_RESULT_ERROR_RANGE_OUTSIDE_PARTITION;

	rc = zircon_partition_write(partition, abs_offset,
				    (unsigned char *)buffer, num_bytes);
	if (rc)
		return AVB_IO_RESULT_ERROR_IO;

	return AVB_IO_RESULT_OK;
}

/* avb_slot_verify uses this call to check that a partition exists.
 * Checks for existence but ignores GUID because it's unused.
 */
static AvbIOResult get_unique_guid_for_partition(AvbOps *ops,
						 const char *partition,
						 char *guid_buf,
						 size_t guid_buf_size)
{
	uint64_t size;
	int rc = zircon_get_partition_size(partition, &size);

	if (rc)
		return AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;

	guid_buf[0] = '\0';
	return AVB_IO_RESULT_OK;
}

static AvbIOResult get_size_of_partition(AvbOps *ops, const char *partition,
					 uint64_t *out_size_num_bytes)
{
	int rc = zircon_get_partition_size(partition, out_size_num_bytes);

	if (rc)
		return AVB_IO_RESULT_ERROR_IO;

	return AVB_IO_RESULT_OK;
}

#ifdef CONFIG_LIBAVB_ATX
static AvbIOResult get_random(AvbAtxOps *atx_ops, size_t num_bytes,
			      uint8_t *output)
{
	if (ta_vx_cprng_draw(output, num_bytes))
		return AVB_IO_RESULT_ERROR_IO;

	return AVB_IO_RESULT_OK;
}

static AvbIOResult read_is_device_unlocked(AvbOps *ops, bool *out_is_unlocked)
{
	if (ta_vx_is_unlocked(out_is_unlocked))
		return AVB_IO_RESULT_ERROR_IO;

	return AVB_IO_RESULT_OK;
}

static AvbIOResult read_persistent_value(AvbOps *ops, const char *name,
					 size_t buffer_size,
					 uint8_t *out_buffer,
					 size_t *out_num_bytes_read)
{
	if (ta_vx_read_persistent_value(name, out_buffer, buffer_size,
					out_num_bytes_read)) {
		// Per contract with avb_ops->read_persistent_value, we should
		// return AVB_IO_RESULT_ERROR_NO_SUCH_VALUE when the specified
		// value does not exist, is not supported, or is not populated.
		//
		// In order to relieve the complexity with error propagation
		// from the TA, we consider any error as if the error was
		// "no such value".
		return AVB_IO_RESULT_ERROR_NO_SUCH_VALUE;
	}

	return AVB_IO_RESULT_OK;
}

static AvbIOResult write_persistent_value(AvbOps *ops, const char *name,
					  size_t value_size,
					  const uint8_t *value)
{
	// Per contract with avb_ops->write_persistent_value, if |value_size|
	// is zero, future calls to |read_persisent_value| shall return
	// AVB_IO_RESULT_ERROR_NO_SUCH_VALUE. That means we should delete the
	// value if |value_size| is zero.
	if (value_size == 0 && ta_vx_delete_persistent_value(name))
		return AVB_IO_RESULT_ERROR_IO;

	if (value_size > 0 &&
	    ta_vx_write_persistent_value(name, value, value_size)) {
		return AVB_IO_RESULT_ERROR_IO;
	}

	return AVB_IO_RESULT_OK;
}

static AvbIOResult avb_read_rollback_index(AvbOps *ops,
					   size_t rollback_index_location,
					   uint64_t *out_rollback_index)
{
	if (ta_vx_read_rollback_index(rollback_index_location,
				      out_rollback_index)) {
		return AVB_IO_RESULT_ERROR_IO;
	}
	return AVB_IO_RESULT_OK;
}

static AvbIOResult avb_write_rollback_index(AvbOps *ops,
					    size_t rollback_index_location,
					    uint64_t rollback_index)
{
	if (ta_vx_write_rollback_index(rollback_index_location,
				       rollback_index)) {
		return AVB_IO_RESULT_ERROR_IO;
	}
	return AVB_IO_RESULT_OK;
}

static void set_key_version(AvbAtxOps *atx_ops, size_t rollback_index_location,
			    uint64_t key_version)
{
	if (rollback_index_location < ARRAY_SIZE(key_versions)) {
		key_versions[rollback_index_location] = key_version;
	} else {
		printf("ERROR: rollback index location out of bounds: %lu\n",
		       rollback_index_location);
	}
}

static AvbOps ops;

static AvbAtxOps atx_ops = {
	.ops = &ops,
	.read_permanent_attributes = avb_read_permanent_attributes,
	.read_permanent_attributes_hash = avb_read_permanent_attributes_hash,
	.set_key_version = set_key_version,
	.get_random = get_random,
};
#else
// The flow functions all copied from cmd/amlogic/cmd_avb.c

#if CONFIG_AVB2_KPUB_FROM_FIP
int compare_avbkey_with_fipkey(const uint8_t *public_key_data, size_t public_key_length);
#endif

/**
 * normally, we should read vendor avb public key from a virtual partition
 * with the name avb_custom_key. Flashing and erasing this partition only
 * works in the UNLOCKED state. Setting the custom key is done like this:
 * $ fastboot flash avb_custom_key pkmd.bin
 *
 * Erasing the key is done by erasing the virtual partition:
 * $ fastboot erase avb_custom_key
 */
static AvbIOResult avb_atx_validate_vbmeta_public_key(AvbOps *ops, const uint8_t *public_key_data,
		size_t public_key_length, const uint8_t *public_key_metadata,
		size_t public_key_metadata_length, bool *out_is_trusted)
{
	*out_is_trusted = false;
	AvbIOResult ret = AVB_IO_RESULT_ERROR_IO;
	char *keybuf = NULL;
	char *partition = "misc";
	AvbKey_t key;
	u64 size = 0;
#if CONFIG_AVB2_KPUB_FROM_FIP
	int result = 0;
#endif
	int i = 0;

#if CONFIG_AVB2_KPUB_FROM_FIP
	printf("AVB2 verifying with fip key\n");
	result = compare_avbkey_with_fipkey(public_key_data, public_key_length);
	if (result == -2) {
		printf("AVB2 verified with fip key failed\n");
		*out_is_trusted = false;
		ret = AVB_IO_RESULT_OK;
		return ret;
	} else if (result == -1) {
		printf("AVB2 cannot find fip key\n");
	} else if (result == 0) {
		printf("AVB2 verified with fip key success\n");
		*out_is_trusted = true;
		ret = AVB_IO_RESULT_OK;
		return ret;
	}
#endif

	/*
	 * disable AVB custom key and test key
	 * if device secure boot enabled
	 */
	if (!IS_FEAT_BOOT_VERIFY()) {
		key.size = 0;
		keybuf = (char *)malloc(AVB_CUSTOM_KEY_LEN_MAX);
		if (keybuf) {
			memset(keybuf, 0, AVB_CUSTOM_KEY_LEN_MAX);
			size = store_logic_cap(partition);
			if (size != 1) {
				/* no need workaround for nand. The size is 4K multiple,
				 * and AVB_CUSTOM_KEY_LEN_MAX is 4K.  The offset will lay on
				 * 4K boundary.
				 */
				if (store_logic_read((const char *)partition,
							size - AVB_CUSTOM_KEY_LEN_MAX,
							AVB_CUSTOM_KEY_LEN_MAX,
							(unsigned char *)keybuf) >= 0)  {
					memcpy(&key, keybuf, sizeof(AvbKey_t));
				}
			}
		}

		if (keybuf && (strncmp(keybuf, "AVBK", 4) == 0)) {
			printf("AVB2 verify with avb_custom_key\n");
			if (key.size == public_key_length &&
					!avb_safe_memcmp(public_key_data,
						keybuf + sizeof(AvbKey_t), public_key_length)) {
				*out_is_trusted = true;
				ret = AVB_IO_RESULT_OK;
			}
		} else {
			/**
			 * When the custom key is set
			 * and the device is in the LOCKED state
			 * it will boot images signed with both the built-in key
			 * as well as the custom key
			 */
			printf("AVB2 verify with fuchsia default kpub:%d, vbmeta kpub:%ld\n",
					avb2_kpub_fuchsia_len, public_key_length);
			if (avb2_kpub_fuchsia_len == public_key_length &&
					!avb_safe_memcmp(public_key_data,
						avb2_kpub_fuchsia, public_key_length)) {
				*out_is_trusted = true;
				ret = AVB_IO_RESULT_OK;
			}
		}
	} else {
		printf("AVB2 verify with production kpub:%d, vbmeta kpub:%ld\n",
				avb2_kpub_production_len, public_key_length);
		if (avb2_kpub_production_len == public_key_length &&
				!avb_safe_memcmp(public_key_data,
					avb2_kpub_production, public_key_length)) {
			*out_is_trusted = true;
			ret = AVB_IO_RESULT_OK;
		}
		for (i = 0; i < avb2_kpub_production_len; i++) {
			if (avb2_kpub_production[i] != 0)
				break;
		}
		if (i == avb2_kpub_production_len)
			printf("ERROR: DID YOU FORGET TO CHANGE AVB2 KEY FOR SECURE BOOT?");
	}

	if (keybuf)
		free(keybuf);
	if (ret != AVB_IO_RESULT_OK)
		printf("AVB2 key in bootloader does not match with the key in vbmeta\n");
	return ret;
}

static AvbIOResult avb_read_rollback_index(AvbOps *ops, size_t rollback_index_location,
		uint64_t *out_rollback_index)
{
#if defined(CONFIG_AML_ANTIROLLBACK) || defined(CONFIG_AML_AVB2_ANTIROLLBACK)
	uint32_t version;

	if (get_avb_antirollback(rollback_index_location, &version)) {
		*out_rollback_index = version;
	} else {
		printf("failed to read rollback index: %zd\n", rollback_index_location);
		return AVB_IO_RESULT_ERROR_NO_SUCH_VALUE;
	}
#else
	*out_rollback_index = 0;
#endif
	return AVB_IO_RESULT_OK;
}

static AvbIOResult avb_write_rollback_index(AvbOps *ops, size_t rollback_index_location,
		uint64_t rollback_index)
{
	AvbIOResult result = AVB_IO_RESULT_OK;
#if defined(CONFIG_AML_ANTIROLLBACK) || defined(CONFIG_AML_AVB2_ANTIROLLBACK)
	uint32_t version = rollback_index;

	if (set_avb_antirollback(rollback_index_location, version)) {
		result = AVB_IO_RESULT_OK;
		goto out;
	} else {
		printf("failed to set rollback index: %zd, version: %u\n",
			rollback_index_location, version);
		result = AVB_IO_RESULT_ERROR_NO_SUCH_VALUE;
		goto out;
	}
out:
#endif
	return result;
}

static AvbIOResult read_is_device_unlocked(AvbOps *ops, bool *out_is_unlocked)
{
	AvbIOResult result = AVB_IO_RESULT_OK;
#if defined(CONFIG_AML_ANTIROLLBACK) || defined(CONFIG_AML_AVB2_ANTIROLLBACK)
	uint32_t lock_state;
	char *lock_s;

	if (get_avb_lock_state(&lock_state)) {
		*out_is_unlocked = !lock_state;
		lock_s = env_get("lock");
		if (*out_is_unlocked)
			lock_s[4] = '0';
		else
			lock_s[4] = '1';
		lock_s = env_get("lock");
		result = AVB_IO_RESULT_OK;
		goto out;
	} else {
		printf("failed to read device lock status from rpmb\n");
		result = AVB_IO_RESULT_ERROR_IO;
		goto out;
	}
#else
	char *lock_s;
	LockData_t info;

	lock_s = env_get("lock");
	if (!lock_s) {
		result = AVB_IO_RESULT_ERROR_IO;
		goto out;
	}

	memset(&info, 0, sizeof(struct LockData));

	info.version_major = (int)(lock_s[0] - '0');
	info.version_minor = (int)(lock_s[1] - '0');
	info.lock_state = (int)(lock_s[4] - '0');
	info.lock_critical_state = (int)(lock_s[5] - '0');
	info.lock_bootloader = (int)(lock_s[6] - '0');

	if (info.lock_state == 1)
		*out_is_unlocked = false;
	else
		*out_is_unlocked = true;
	result = AVB_IO_RESULT_OK;
#endif
out:
	return result;
}
#endif

static AvbOps ops = {
#ifdef CONFIG_LIBAVB_ATX
	.atx_ops = &atx_ops,
#endif
	.read_from_partition = read_from_partition,
#ifdef CONFIG_LIBAVB_ATX
	.get_preloaded_partition = get_preloaded_partition,
#else
	.get_preloaded_partition = NULL,
#endif
	.write_to_partition = write_to_partition,
	.validate_vbmeta_public_key = avb_atx_validate_vbmeta_public_key,
	.read_rollback_index = avb_read_rollback_index,
	.write_rollback_index = avb_write_rollback_index,
	.read_is_device_unlocked = read_is_device_unlocked,
	.get_unique_guid_for_partition = get_unique_guid_for_partition,
	.get_size_of_partition = get_size_of_partition,
#ifdef CONFIG_LIBAVB_ATX
	.read_persistent_value = read_persistent_value,
	.write_persistent_value = write_persistent_value,
#else
	.read_persistent_value = NULL,
	.write_persistent_value = NULL,
#endif
};

int zircon_vboot_slot_verify(unsigned char *loadaddr, uint64_t img_size,
			     const char *ab_suffix,
			     bool has_successfully_booted)
{
	preloaded_img_addr = loadaddr;
	preloaded_img_size = img_size;

	bool unlocked;

	if (ops.read_is_device_unlocked(&ops, &unlocked)) {
		fprintf(stderr, "Failed to read lock state.\n");
		return -1;
	}

	if (unlocked) {
		printf("Device unlocked: skipping slot verification.\n");
		return 0;
	}

	// TODO(http://fxb/44928, improve factory integrity check performance).
	const char *const requested_partitions[] = { ZIRCON_PARTITION_PREFIX,
						     "factory", NULL };

	AvbSlotVerifyData *verify_data = NULL;

	AvbSlotVerifyResult result =
		avb_slot_verify(&ops, requested_partitions, ab_suffix,
				AVB_SLOT_VERIFY_FLAGS_NONE,
				AVB_HASHTREE_ERROR_MODE_EIO, &verify_data);

	if (result != AVB_SLOT_VERIFY_RESULT_OK) {
		fprintf(stderr,
			"Failed to verify slot: %s, err_code: %s\n", ab_suffix,
			avb_slot_verify_result_to_string(result));
		if (verify_data)
			avb_slot_verify_data_free(verify_data);

		return -1;
	}

	// Increase rollback index values to match the verified slot only if
	// it has already successfully booted.
	if (has_successfully_booted) {
		int i;

		for (i = 0; i < ARRAY_SIZE(verify_data->rollback_indexes);
		     i++) {
			uint64_t rollback_index_value =
				verify_data->rollback_indexes[i];

			if (rollback_index_value == ROLLBACK_INDEX_NOT_USED)
				continue;

			result = ops.write_rollback_index(&ops, i,
							  rollback_index_value);
			if (result != AVB_SLOT_VERIFY_RESULT_OK) {
				avb_slot_verify_data_free(verify_data);
				fprintf(stderr,
					"Failed to write rollback index: %d\n",
					i);
				return -1;
			}
		}

		/* Also increase rollback index values for Fuchsia key version locations.
		 */
		for (i = 0; i < ARRAY_SIZE(key_versions); i++) {
			uint64_t rollback_index_value = key_versions[i];

			if (rollback_index_value == ROLLBACK_INDEX_NOT_USED)
				continue;

			result = ops.write_rollback_index(&ops, i,
							  rollback_index_value);
			if (result != AVB_SLOT_VERIFY_RESULT_OK) {
				avb_slot_verify_data_free(verify_data);
				fprintf(stderr,
					"Failed to write rollback index: %d\n",
					i);
				return -1;
			}
		}
	}

	avb_slot_verify_data_free(verify_data);

	printf("slot: %s successfully verified.\n", ab_suffix);
	return 0;
}

#ifdef CONFIG_LIBAVB_ATX
int zircon_vboot_generate_unlock_challenge(AvbAtxUnlockChallenge
		*out_unlock_challenge)
{
	AvbIOResult ret = avb_atx_generate_unlock_challenge(&atx_ops,
			out_unlock_challenge);
	if (ret != AVB_IO_RESULT_OK) {
		fprintf(stderr, "Failed to generate unlock challenge\n");
		return -1;
	}

	return 0;
}

int zircon_vboot_validate_unlock_credential(AvbAtxUnlockCredential
		*unlock_credential, bool *out_is_trusted)
{
	AvbIOResult ret = avb_atx_validate_unlock_credential(&atx_ops,
			unlock_credential, out_is_trusted);
	if (ret != AVB_IO_RESULT_OK) {
		fprintf(stderr, "Failed to validate unlock challenge\n");
		return -1;
	}

	return 0;
}
#endif
