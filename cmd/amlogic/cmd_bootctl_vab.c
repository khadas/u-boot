// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <config.h>
#include <asm/arch/io.h>
#include <partition_table.h>
#include <libavb.h>
#include <version.h>
#include <amlogic/storage.h>
#include <fastboot.h>
#include <u-boot/sha1.h>
#include <asm/arch/efuse.h>
#include <emmc_partitions.h>

#if defined(CONFIG_EFUSE_OBJ_API) && defined(CONFIG_CMD_EFUSE)
extern efuse_obj_field_t efuse_field;
#endif//#ifdef CONFIG_EFUSE_OBJ_API

#ifdef CONFIG_BOOTLOADER_CONTROL_BLOCK

// Spaces used by misc partition are as below:
// 0   - 2K	For bootloader_message
// 2K  - 16K	Used by Vendor's bootloader (the 2K - 4K range may be optionally used
//				as bootloader_message_ab struct)
// 16K - 32K	Used by uncrypt and recovery to store wipe_package for A/B devices
// 32K - 64K	System space, used for miscellanious AOSP features. See below.
// Note that these offsets are admitted by bootloader,recovery and uncrypt, so they
// are not configurable without changing all of them.
#define BOOTLOADER_MESSAGE_OFFSET_IN_MISC 0
#define VENDOR_SPACE_OFFSET_IN_MISC 2 * 1024
#define WIPE_PACKAGE_OFFSET_IN_MISC 16 * 1024
#define SYSTEM_SPACE_OFFSET_IN_MISC 32 * 1024
#define SYSTEM_SPACE_SIZE_IN_MISC 32 * 1024


#define AB_METADATA_MISC_PARTITION_OFFSET 2048

#define MISCBUF_SIZE  2080

/* Bootloader Message (2-KiB)
 *
 * This structure describes the content of a block in flash
 * that is used for recovery and the bootloader to talk to
 * each other.
 *
 * The command field is updated by linux when it wants to
 * reboot into recovery or to update radio or bootloader firmware.
 * It is also updated by the bootloader when firmware update
 * is complete (to boot into recovery for any final cleanup)
 *
 * The status field was used by the bootloader after the completion
 * of an "update-radio" or "update-hboot" command, which has been
 * deprecated since Froyo.
 *
 * The recovery field is only written by linux and used
 * for the system to send a message to recovery or the
 * other way around.
 *
 * The stage field is written by packages which restart themselves
 * multiple times, so that the UI can reflect which invocation of the
 * package it is.  If the value is of the format "#/#" (eg, "1/3"),
 * the UI will add a simple indicator of that status.
 *
 * We used to have slot_suffix field for A/B boot control metadata in
 * this struct, which gets unintentionally cleared by recovery or
 * uncrypt. Move it into struct bootloader_message_ab to avoid the
 * issue.
 */
struct bootloader_message {
	char command[32];
	char status[32];
	char recovery[768];

	// The 'recovery' field used to be 1024 bytes.	It has only ever
	// been used to store the recovery command line, so 768 bytes
	// should be plenty.  We carve off the last 256 bytes to store the
	// stage string (for multistage packages) and possible future
	// expansion.
	char stage[32];

	// The 'reserved' field used to be 224 bytes when it was initially
	// carved off from the 1024-byte recovery field. Bump it up to
	// 1184-byte so that the entire bootloader_message struct rounds up
	// to 2048-byte.
	char reserved[1184];
};

/**
 * The A/B-specific bootloader message structure (4-KiB).
 *
 * We separate A/B boot control metadata from the regular bootloader
 * message struct and keep it here. Everything that's A/B-specific
 * stays after struct bootloader_message, which should be managed by
 * the A/B-bootloader or boot control HAL.
 *
 * The slot_suffix field is used for A/B implementations where the
 * bootloader does not set the androidboot.ro.boot.slot_suffix kernel
 * commandline parameter. This is used by fs_mgr to mount /system and
 * other partitions with the slotselect flag set in fstab. A/B
 * implementations are free to use all 32 bytes and may store private
 * data past the first NUL-byte in this field. It is encouraged, but
 * not mandatory, to use 'struct bootloader_control' described below.
 *
 * The update_channel field is used to store the Omaha update channel
 * if update_engine is compiled with Omaha support.
 */
struct bootloader_message_ab {
	struct bootloader_message message;
	char slot_suffix[32];
	char update_channel[128];

	// Round up the entire struct to 4096-byte.
	char reserved[1888];
};

#define BOOT_CTRL_MAGIC   0x42414342 /* Bootloader Control AB */
#define BOOT_CTRL_VERSION 1

typedef struct slot_metadata {
	// Slot priority with 15 meaning highest priority, 1 lowest
	// priority and 0 the slot is unbootable.
	uint8_t priority : 4;
	// Number of times left attempting to boot this slot.
	uint8_t tries_remaining : 3;
	// 1 if this slot has booted successfully, 0 otherwise.
	uint8_t successful_boot : 1;
	// 1 if this slot is corrupted from a dm-verity corruption, 0
	// otherwise.
	uint8_t verity_corrupted : 1;
	// Reserved for further use.
	uint8_t reserved : 7;
} slot_metadata;

/* Bootloader Control AB
 *
 * This struct can be used to manage A/B metadata. It is designed to
 * be put in the 'slot_suffix' field of the 'bootloader_message'
 * structure described above. It is encouraged to use the
 * 'bootloader_control' structure to store the A/B metadata, but not
 * mandatory.
 */
typedef struct bootloader_control {
	// NUL terminated active slot suffix.
	char slot_suffix[4];
	// Bootloader Control AB magic number (see BOOT_CTRL_MAGIC).
	uint32_t magic;
	// Version of struct being used (see BOOT_CTRL_VERSION).
	uint8_t version;
	// Number of slots being managed.
	uint8_t nb_slot : 3;
	// Number of times left attempting to boot recovery.
	uint8_t recovery_tries_remaining : 3;
	// Status of any pending snapshot merge of dynamic partitions.
	uint8_t merge_status : 3;
	// Ensure 4-bytes alignment for slot_info field.
	uint8_t roll_flag;
	// Per-slot information.  Up to 4 slots.
	struct slot_metadata slot_info[4];
	// Reserved for further use.
	uint8_t reserved1[8];
	// CRC32 of all 28 bytes preceding this field (little endian
	// format).
	uint32_t crc32_le;
}bootloader_control;

#define MISC_VIRTUAL_AB_MESSAGE_VERSION 2
#define MISC_VIRTUAL_AB_MAGIC_HEADER 0x56740AB0

unsigned int kDefaultBootAttempts = 7;

/* Magic for the A/B struct when serialized. */
#define AVB_AB_MAGIC "\0AB0"
#define AVB_AB_MAGIC_LEN 4

/* Versioning for the on-disk A/B metadata - keep in sync with avbtool. */
#define AVB_AB_MAJOR_VERSION 1
#define AVB_AB_MINOR_VERSION 0

/* Size of AvbABData struct. */
#define AVB_AB_DATA_SIZE 32

/* Maximum values for slot data */
#define AVB_AB_MAX_PRIORITY 15
#define AVB_AB_MAX_TRIES_REMAINING 7

/* Struct used for recording per-slot metadata.
 *
 * When serialized, data is stored in network byte-order.
 */
typedef struct AvbABSlotData {
  /* Slot priority. Valid values range from 0 to AVB_AB_MAX_PRIORITY,
   * both inclusive with 1 being the lowest and AVB_AB_MAX_PRIORITY
   * being the highest. The special value 0 is used to indicate the
   * slot is unbootable.
   */
  uint8_t priority;

  /* Number of times left attempting to boot this slot ranging from 0
   * to AVB_AB_MAX_TRIES_REMAINING.
   */
  uint8_t tries_remaining;

  /* Non-zero if this slot has booted successfully, 0 otherwise. */
  uint8_t successful_boot;

  /* Reserved for future use. */
  uint8_t reserved[1];
} AvbABSlotData;

/* Struct used for recording A/B metadata.
 *
 * When serialized, data is stored in network byte-order.
 */
typedef struct AvbABData {
  /* Magic number used for identification - see AVB_AB_MAGIC. */
  uint8_t magic[AVB_AB_MAGIC_LEN];

  /* Version of on-disk struct - see AVB_AB_{MAJOR,MINOR}_VERSION. */
  uint8_t version_major;
  uint8_t version_minor;

  /* Padding to ensure |slots| field start eight bytes in. */
  uint8_t reserved1[2];

  /* Per-slot metadata. */
  AvbABSlotData slots[2];

  /* Reserved for future use. */
  uint8_t reserved2[12];

  /* CRC32 of all 28 bytes preceding this field. */
  uint32_t crc32;
}AvbABData;

bool boot_info_validate(bootloader_control* info)
{
	if (info->magic != BOOT_CTRL_MAGIC) {
		printf("Magic 0x%x is incorrect.\n", info->magic);
		return false;
	}
	return true;
}

bool boot_info_validate_normalAB(AvbABData* info)
{
	if (memcmp(info->magic, AVB_AB_MAGIC, AVB_AB_MAGIC_LEN) != 0) {
		printf("Magic %s is incorrect.\n", info->magic);
		return false;
	}
	if (info->version_major > AVB_AB_MAJOR_VERSION) {
		printf("No support for given major version.\n");
		return false;
	}
	return true;
}


void boot_info_reset(bootloader_control* boot_ctrl)
{
	int slot;

	memset(boot_ctrl, '\0', sizeof(bootloader_control));
	memcpy(boot_ctrl->slot_suffix, "_a", 2);
	boot_ctrl->magic = BOOT_CTRL_MAGIC;
	boot_ctrl->version = BOOT_CTRL_VERSION;
	boot_ctrl->nb_slot = 2;
	boot_ctrl->roll_flag = 0;

	for (slot = 0; slot < 4; ++slot) {
		slot_metadata entry = {};

		if (slot < boot_ctrl->nb_slot) {
			entry.priority = 7;
			entry.tries_remaining = kDefaultBootAttempts;
			entry.successful_boot = 0;
		} else {
			entry.priority = 0;  // Unbootable
			entry.tries_remaining = 0;
			entry.successful_boot = 0;
		}

		boot_ctrl->slot_info[slot] = entry;
	}
	boot_ctrl->slot_info[0].successful_boot = 1;
	boot_ctrl->recovery_tries_remaining = 0;
}

void dump_boot_info(bootloader_control* boot_ctrl)
{
#if 0
	int slot;

	printf("boot_ctrl->slot_suffix = %s\n", boot_ctrl->slot_suffix);
	printf("boot_ctrl->magic = 0x%x\n", boot_ctrl->magic);
	printf("boot_ctrl->version = %d\n", boot_ctrl->version);
	printf("boot_ctrl->nb_slot = %d\n", boot_ctrl->nb_slot);
	for (slot = 0; slot < 4; ++slot) {
		printf("boot_ctrl->slot_info[%d].priority = %d\n",
				slot, boot_ctrl->slot_info[slot].priority);
		printf("boot_ctrl->slot_info[%d].tries_remaining = %d\n",
				slot, boot_ctrl->slot_info[slot].tries_remaining);
		printf("boot_ctrl->slot_info[%d].successful_boot = %d\n",
				slot, boot_ctrl->slot_info[slot].successful_boot);
	}
	printf("boot_ctrl->recovery_tries_remaining = %d\n",
			boot_ctrl->recovery_tries_remaining);
#endif
}

static bool slot_is_bootable(slot_metadata* slot) {
	return slot->tries_remaining != 0;
}

int get_active_slot(bootloader_control* info) {
	if (info->slot_info[0].priority > info->slot_info[1].priority) {
		return 0;
	} else if (info->slot_info[0].priority == info->slot_info[1].priority) {
		if (info->slot_info[0].successful_boot == 1)
			return 0;
		else
			return 1;
	} else {
		return 1;
	}
}

static bool slot_is_bootable_normalAB(AvbABSlotData* slot) {
  return slot->priority > 0 &&
		 (slot->successful_boot || (slot->tries_remaining > 0));
}

int get_active_slot_normalAB(AvbABData* info) {
	if (info->slots[0].priority > info->slots[1].priority)
		return 0;
	else
		return 1;
}

static uint32_t vab_crc32(const uint8_t *buf, size_t size)
{
	static uint32_t crc_table[256];
	uint32_t ret = -1;

	// Compute the CRC-32 table only once.
	if (!crc_table[1]) {
		for (uint32_t i = 0; i < 256; ++i) {
			uint32_t crc = i;

			for (uint32_t j = 0; j < 8; ++j) {
				uint32_t mask = -(crc & 1);

				crc = (crc >> 1) ^ (0xEDB88320 & mask);
			}
			crc_table[i] = crc;
		}
	}

	for (size_t i = 0; i < size; ++i)
		ret = (ret >> 8) ^ crc_table[(ret ^ buf[i]) & 0xFF];

	return ~ret;
}

int boot_info_set_active_slot(bootloader_control* bootctrl, int slot)
{
	int i;
	// Set every other slot with a lower priority than the new "active" slot.
	const unsigned int kActivePriority = 15;
	const unsigned int kActiveTries = 6;

	for (i = 0; i < bootctrl->nb_slot; ++i) {
		if (i != slot) {
			//bootctrl->slot_info[i].priority -= 1;
			if (bootctrl->slot_info[i].priority >= kActivePriority)
				bootctrl->slot_info[i].priority = kActivePriority - 1;
		}
		printf("bootctrl->slot_info[%d].priority = %d\n", i,
				bootctrl->slot_info[i].priority);
	}

	// Note that setting a slot as active doesn't change the successful bit.
	// The successful bit will only be changed by setSlotAsUnbootable().
	bootctrl->slot_info[slot].priority = kActivePriority;
	bootctrl->slot_info[slot].tries_remaining = kActiveTries;

	printf("bootctrl->slot_info[%d].priority = %d\n", slot,
				bootctrl->slot_info[slot].priority);
	printf("bootctrl->slot_info[%d].tries_remaining = %d\n",
				slot, bootctrl->slot_info[slot].tries_remaining);

	// Setting the current slot as active is a way to revert the operation that
	// set *another* slot as active at the end of an updater. This is commonly
	// used to cancel the pending update. We should only reset the verity_corrpted
	// bit when attempting a new slot, otherwise the verity bit on the current
	// slot would be flip.
	if (slot != get_active_slot(bootctrl))
		bootctrl->slot_info[slot].verity_corrupted = 0;

	dump_boot_info(bootctrl);

	return 0;
}

int boot_info_open_partition(char *miscbuf)
{
	char *partition = "misc";

	printf("Start read %s partition datas!\n", partition);
	if (store_read((const char *)partition,
		0, MISCBUF_SIZE, (unsigned char *)miscbuf) < 0) {
		printf("failed to store read %s.\n", partition);
		return -1;
	}
	return 0;
}

bool boot_info_load(bootloader_control *out_info, char *miscbuf)
{
	memcpy(out_info, miscbuf + AB_METADATA_MISC_PARTITION_OFFSET, sizeof(bootloader_control));
	dump_boot_info(out_info);
	return true;
}

bool boot_info_load_normalAB(AvbABData *out_info, char *miscbuf)
{
	memcpy(out_info, miscbuf + AB_METADATA_MISC_PARTITION_OFFSET, AVB_AB_DATA_SIZE);
	return true;
}

bool boot_info_save(bootloader_control *info, char *miscbuf)
{
	char *partition = "misc";

	printf("save boot-info\n");
	info->crc32_le = vab_crc32((const uint8_t *)info,
		sizeof(bootloader_control) - sizeof(uint32_t));

	memcpy(miscbuf + AB_METADATA_MISC_PARTITION_OFFSET, info, sizeof(bootloader_control));
	dump_boot_info(info);
	store_write((const char *)partition, 0, MISCBUF_SIZE, (unsigned char *)miscbuf);
	return true;
}

static int is_BootSame(int srcindex, int dstindex)
{
	int iRet = 0;
	int ret = -1;
	unsigned char *buffer_src = NULL;
	unsigned char *buffer_dest = NULL;
	const int SHA1SUMLEN = 20;
	u8 gensum0[SHA1SUMLEN * 2];
	u8 *gensum1 = gensum0 + SHA1SUMLEN;
	int capacity_boot = 0;

#ifdef CONFIG_MMC_MESON_GX
	struct mmc *mmc = NULL;

	if (store_get_type() == BOOT_EMMC)
		mmc = find_mmc_device(1);

	if (mmc)
		capacity_boot = mmc->capacity_boot;
#endif

	printf("is_BootSame_capacity_boot: %x\n", capacity_boot);

	buffer_src = (unsigned char *)malloc(capacity_boot);
	if (!buffer_src) {
		printf("ERROR! fail to allocate memory ...\n");
		goto exit;
	}
	memset(buffer_src, 0, capacity_boot);

	iRet = store_boot_read("bootloader", srcindex, 0, buffer_src);
	if (iRet) {
		printf("Fail read bootloader %d\n", srcindex);
		goto exit;
	}
	sha1_csum(buffer_src, capacity_boot, gensum0);

	buffer_dest = buffer_src;
	memset(buffer_dest, 0, capacity_boot);
	iRet = store_boot_read("bootloader", dstindex, 0, buffer_dest);
	if (iRet) {
		printf("Fail read bootloader %d\n", dstindex);
		goto exit;
	}
	sha1_csum(buffer_dest, capacity_boot, gensum1);

	ret = memcmp(gensum0, gensum1, SHA1SUMLEN);
	printf("bootloader %d & %d %s same\n", srcindex, dstindex, ret ? "NOT" : "DO");

exit:
	if (buffer_src) {
		free(buffer_src);
		buffer_src = NULL;
	}
	return ret;
}

int write_bootloader(int copy, int dstindex) {
	int iRet = 0;
	int ret = -1;
	unsigned char *buffer = NULL;
	int capacity_boot = 0;

#ifdef CONFIG_MMC_MESON_GX
	struct mmc *mmc = NULL;

	if (store_get_type() == BOOT_EMMC)
		mmc = find_mmc_device(1);
#endif

	if (mmc)
		capacity_boot = mmc->capacity_boot;

	printf("write_bootloader_capacity_boot: %x\n", capacity_boot);

	buffer = (unsigned char *)malloc(capacity_boot);
	if (!buffer) {
		printf("ERROR! fail to allocate memory ...\n");
		goto exit;
	}
	memset(buffer, 0, capacity_boot);
	printf("copy from boot%d to boot%d\n", copy, dstindex);
	iRet = store_boot_read("bootloader", copy, 0, buffer);
	if (iRet) {
		printf("Fail read bootloader from rsv with sz\n");
		goto exit;
	}
	iRet = store_boot_write("bootloader", dstindex, 0, buffer);
	if (iRet) {
		printf("Failed to write bootloader\n");
		goto exit;
	} else {
		ret = 0;
	}

exit:
	if (buffer) {
		free(buffer);
		buffer = NULL;
	}
	return ret;
}

static int do_GetValidSlot(
	cmd_tbl_t *cmdtp,
	int flag,
	int argc,
	char * const argv[])
{
	char miscbuf[MISCBUF_SIZE] = {0};
	bootloader_control boot_ctrl;
	AvbABData info;
	int slot;
	int AB_mode = 0;
	bool bootable_a, bootable_b;
	bool nocs_mode = false;

	if (argc != 1)
		return cmd_usage(cmdtp);

	boot_info_open_partition(miscbuf);
	boot_info_load(&boot_ctrl, miscbuf);

	if (!boot_info_validate(&boot_ctrl)) {
		printf("boot-info virtual ab is invalid. Try normal ab.\n");
		boot_info_load_normalAB(&info, miscbuf);
		if (!boot_info_validate_normalAB(&info)) {
			printf("boot-info is invalid. Resetting.\n");
			boot_info_reset(&boot_ctrl);
			boot_info_save(&boot_ctrl, miscbuf);
		} else {
			printf("update from normal ab to virtual ab\n");
			AB_mode = 1;
		}
	}

	if (AB_mode == 1) {
		slot = get_active_slot_normalAB(&info);
		printf("active slot = %d\n", slot);
		bootable_a = slot_is_bootable_normalAB(&info.slots[0]);
		bootable_b = slot_is_bootable_normalAB(&info.slots[1]);
		boot_info_reset(&boot_ctrl);
		boot_ctrl.slot_info[0].successful_boot = info.slots[0].successful_boot;
		boot_ctrl.slot_info[1].successful_boot = info.slots[1].successful_boot;
		boot_info_set_active_slot(&boot_ctrl, slot);
		boot_info_save(&boot_ctrl, miscbuf);
		slot = get_active_slot(&boot_ctrl);
	} else {
		slot = get_active_slot(&boot_ctrl);
		printf("active slot = %d\n", slot);
		bootable_a = slot_is_bootable(&boot_ctrl.slot_info[0]);
		bootable_b = slot_is_bootable(&boot_ctrl.slot_info[1]);
	}

	if (dynamic_partition)
		env_set("partition_mode", "dynamic");
	else
		env_set("partition_mode", "normal");

	if (gpt_partition)
		env_set("gpt_mode", "true");
	else
		env_set("gpt_mode", "false");

	if (vendor_boot_partition) {
		env_set("vendor_boot_mode", "true");
		printf("set vendor_boot_mode true\n");
	} else {
		env_set("vendor_boot_mode", "false");
		printf("set vendor_boot_mode false\n");
	}

	if (slot == 0) {
		if (bootable_a) {
			if (has_boot_slot == 1) {
				env_set("active_slot", "_a");
				env_set("boot_part", "boot_a");
				env_set("recovery_part", "recovery_a");
				env_set("slot-suffixes", "0");
			} else {
				env_set("active_slot", "normal");
				env_set("boot_part", "boot");
				env_set("recovery_part", "recovery");
				env_set("slot-suffixes", "-1");
			}
			return 0;
		} else if (bootable_b) {
			printf("slot a is unbootable, back to b\n");
			boot_ctrl.roll_flag = 1;
			boot_info_save(&boot_ctrl, miscbuf);
#ifdef CONFIG_FASTBOOT
			struct misc_virtual_ab_message message;

			set_mergestatus_cancel(&message);
#endif
			run_command("set_active_slot b", 0);
			env_set("update_env", "1");
			env_set("reboot_status", "reboot_next");

#if defined(CONFIG_EFUSE_OBJ_API) && defined(CONFIG_CMD_EFUSE)
			run_command("efuse_obj get FEAT_DISABLE_EMMC_USER", 0);
			if (*efuse_field.data == 1)
				nocs_mode = true;
#endif//#ifdef CONFIG_EFUSE_OBJ_API
			if (gpt_partition || nocs_mode) {
				printf("gpt or nocs mode\n");
				write_bootloader(2, 1);
				env_set("expect_index", "1");
			} else {
				printf("normal mode\n");
				write_bootloader(2, 0);
				env_set("expect_index", "0");
			}
			run_command("saveenv", 0);
			run_command("reset", 0);
		} else {
			run_command("run init_display; run storeargs; run update;", 0);
		}
	}

	if (slot == 1) {
		if (bootable_b) {
			if (has_boot_slot == 1) {
				env_set("active_slot", "_b");
				env_set("boot_part", "boot_b");
				env_set("recovery_part", "recovery_b");
				env_set("slot-suffixes", "1");
			} else {
				env_set("active_slot", "normal");
				env_set("boot_part", "boot");
				env_set("recovery_part", "recovery");
				env_set("slot-suffixes", "-1");
			}
			return 0;
		} else if (bootable_a) {
			printf("slot b is unbootable, back to a\n");
			boot_ctrl.roll_flag = 1;
			boot_info_save(&boot_ctrl, miscbuf);
#ifdef CONFIG_FASTBOOT
			struct misc_virtual_ab_message message;

			set_mergestatus_cancel(&message);
#endif
			run_command("set_active_slot a", 0);
			env_set("update_env", "1");
			env_set("reboot_status", "reboot_next");

#if defined(CONFIG_EFUSE_OBJ_API) && defined(CONFIG_CMD_EFUSE)
			run_command("efuse_obj get FEAT_DISABLE_EMMC_USER", 0);
			if (*efuse_field.data == 1)
				nocs_mode = true;
#endif//#ifdef CONFIG_EFUSE_OBJ_API
			if (gpt_partition || nocs_mode) {
				printf("gpt or nocs mode\n");
				write_bootloader(2, 1);
				env_set("expect_index", "1");
			} else {
				printf("normal mode\n");
				write_bootloader(1, 0);
				env_set("expect_index", "0");
			}
			run_command("saveenv", 0);
			run_command("reset", 0);
		} else {
			run_command("run init_display; run storeargs; run update;", 0);
		}
	}

	return 0;
}

static int do_SetActiveSlot(
	cmd_tbl_t *cmdtp,
	int flag,
	int argc,
	char * const argv[])
{
	char miscbuf[MISCBUF_SIZE] = {0};
	bootloader_control info;

	if (argc != 2)
		return cmd_usage(cmdtp);

	if (has_boot_slot == 0) {
		printf("device is not ab mode\n");
		return -1;
	}

	boot_info_open_partition(miscbuf);
	boot_info_load(&info, miscbuf);

	if (!boot_info_validate(&info)) {
		printf("boot-info is invalid. Resetting.\n");
		boot_info_reset(&info);
		boot_info_save(&info, miscbuf);
	}

	if (strcmp(argv[1], "a") == 0) {
		env_set("active_slot", "_a");
		env_set("slot-suffixes", "0");
		env_set("boot_part", "boot_a");
		env_set("recovery_part", "recovery_a");
		printf("set active slot a\n");
		boot_info_set_active_slot(&info, 0);
	} else if (strcmp(argv[1], "b") == 0) {
		env_set("active_slot", "_b");
		env_set("slot-suffixes", "1");
		env_set("boot_part", "boot_b");
		env_set("recovery_part", "recovery_b");
		printf("set active slot b\n");
		boot_info_set_active_slot(&info, 1);
	} else {
		printf("error input slot\n");
		return -1;
	}

	boot_info_save(&info, miscbuf);

	printf("info.roll_flag = %d\n", info.roll_flag);

	if (!gpt_partition && info.roll_flag == 1) {
		printf("if null gpt, write dtb back when rollback\n");
		if (run_command("imgread dtb ${boot_part} ${dtb_mem_addr}", 0)) {
			printf("Fail in load dtb\n");
		} else {
			printf("write dtb back\n");
			run_command("emmc dtb_write ${dtb_mem_addr} 0", 0);
		}
	}

	return 0;
}

static int do_SetRollFlag
(cmd_tbl_t *cmdtp,
	int flag,
	int argc,
	char * const argv[])
{
	char miscbuf[MISCBUF_SIZE] = {0};
	bootloader_control info;

	if (argc != 2)
		return cmd_usage(cmdtp);

	if (has_boot_slot == 0) {
		printf("device is not ab mode\n");
		return -1;
	}

	boot_info_open_partition(miscbuf);
	boot_info_load(&info, miscbuf);

	if (!boot_info_validate(&info)) {
		printf("boot-info is invalid. Resetting.\n");
		return 0;
	}

	if (strcmp(argv[1], "1") == 0)
		info.roll_flag = 1;
	else
		info.roll_flag = 0;

	boot_info_save(&info, miscbuf);

	printf("set info.roll_flag = %d\n", info.roll_flag);

	return 0;
}

static int do_SetUpdateTries(
	cmd_tbl_t *cmdtp,
	int flag,
	int argc,
	char * const argv[])
{
	char miscbuf[MISCBUF_SIZE] = {0};
	bootloader_control boot_ctrl;
	bool bootable_a, bootable_b;
	int slot;
	int ret = -1;
	bool nocs_mode = false;

	if (has_boot_slot == 0) {
		printf("device is not ab mode\n");
		return -1;
	}

	boot_info_open_partition(miscbuf);
	boot_info_load(&boot_ctrl, miscbuf);

	if (!boot_info_validate(&boot_ctrl)) {
		printf("boot-info is invalid. Resetting\n");
		boot_info_reset(&boot_ctrl);
		boot_info_save(&boot_ctrl, miscbuf);
	}

	slot = get_active_slot(&boot_ctrl);
	bootable_a = slot_is_bootable(&boot_ctrl.slot_info[0]);
	bootable_b = slot_is_bootable(&boot_ctrl.slot_info[1]);

	if (slot == 0) {
		if (bootable_a) {
			if (boot_ctrl.slot_info[0].successful_boot == 0)
				boot_ctrl.slot_info[0].tries_remaining -= 1;
		}
	}

	if (slot == 1) {
		if (bootable_b) {
			if (boot_ctrl.slot_info[1].successful_boot == 0)
				boot_ctrl.slot_info[1].tries_remaining -= 1;
		}
	}

	boot_info_save(&boot_ctrl, miscbuf);

	printf("do_SetUpdateTries boot_ctrl.roll_flag = %d\n", boot_ctrl.roll_flag);
	if (boot_ctrl.roll_flag == 1) {
		env_set("rollback_flag", "1");
	}

#if defined(CONFIG_EFUSE_OBJ_API) && defined(CONFIG_CMD_EFUSE)
				run_command("efuse_obj get FEAT_DISABLE_EMMC_USER", 0);
				if (*efuse_field.data == 1)
					nocs_mode = true;
#endif//#ifdef CONFIG_EFUSE_OBJ_API

	if (boot_ctrl.slot_info[slot].successful_boot == 1) {
		if (gpt_partition || nocs_mode) {
			char *bootloaderindex = NULL;

			printf("current slot %d is successful_boot\n", slot);
			bootloaderindex = env_get("forUpgrade_bootloaderIndex");
			printf("bootloaderindex: %s\n", bootloaderindex);
			/*if boot from boot1, means boot0 is bab, don't need to copyback*/
			if (bootloaderindex && strcmp(bootloaderindex, "2")) {
				printf("check if boot0 = boot1\n");
				ret = is_BootSame(1, 2);
				if (ret) {
					printf("boot0 doesn't = boot1, write boot0 to boot1\n");
					write_bootloader(1, 2);
					printf("after write boot0 to boot1\n");
				}
			}
		}
	}
	return 0;
}

static int do_CopySlot(
	cmd_tbl_t *cmdtp,
	int flag,
	int argc,
	char * const argv[])
{
	char miscbuf[MISCBUF_SIZE] = {0};
	bootloader_control boot_ctrl;
	int copy = -1;
	int dest = -1;

	boot_info_open_partition(miscbuf);
	boot_info_load(&boot_ctrl, miscbuf);

	if (!boot_info_validate(&boot_ctrl)) {
		printf("boot-info is invalid. Resetting\n");
		boot_info_reset(&boot_ctrl);
		boot_info_save(&boot_ctrl, miscbuf);
	}

	if (strcmp(argv[1], "1") == 0)
		copy = 1;
	else if (strcmp(argv[1], "2") == 0)
		copy = 2;
	else if (strcmp(argv[1], "0") == 0)
		copy = 0;

	if (strcmp(argv[2], "1") == 0)
		dest = 1;
	else if (strcmp(argv[2], "2") == 0)
		dest = 2;
	else if (strcmp(argv[2], "0") == 0)
		dest = 0;

	if (copy == 1) {
		if (boot_ctrl.slot_info[0].successful_boot == 1)
			write_bootloader(copy, dest);
	} else if (copy == 2) {
		if (boot_ctrl.slot_info[1].successful_boot == 1) {
			write_bootloader(copy, dest);
		} else {
			env_set("update_env", "1");
			env_set("reboot_status", "reboot_next");
			env_set("expect_index", "2");
			run_command("saveenv", 0);
		}
	}

	return 0;
}

int do_GetSystemMode (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_SYSTEM_AS_ROOT
	env_set("system_mode", "1");
#else
	env_set("system_mode", "0");
#endif

	return 0;
}

int do_GetAvbMode (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_AVB2
	env_set("avb2", "1");
#else
	env_set("avb2", "0");
#endif

	return 0;
}


#endif /* CONFIG_BOOTLOADER_CONTROL_BLOCK */

U_BOOT_CMD(
	get_valid_slot, 2, 0, do_GetValidSlot,
	"get_valid_slot",
	"\nThis command will choose valid slot to boot up which saved in misc\n"
	"partition by mark to decide whether execute command!\n"
	"So you can execute command: get_valid_slot"
);

U_BOOT_CMD(
	set_active_slot, 2, 1, do_SetActiveSlot,
	"set_active_slot",
	"\nThis command will set active slot\n"
	"So you can execute command: set_active_slot a"
);

U_BOOT_CMD(
	set_roll_flag, 2, 1, do_SetRollFlag,
	"set_roll_flag",
	"\nThis command will set active slot\n"
	"So you can execute command: set_active_slot a"
);

U_BOOT_CMD
(copy_slot_bootable, 3, 1, do_CopySlot,
	"copy_slot_bootable",
	"\nThis command will set active slot\n"
	"So you can execute command: copy_slot_bootable 2 1"
);

U_BOOT_CMD(
	update_tries, 2, 0, do_SetUpdateTries,
	"update_tries",
	"\nThis command will change tries_remaining in misc\n"
	"So you can execute command: update_tries"
);

U_BOOT_CMD(
	get_system_as_root_mode, 1,	0, do_GetSystemMode,
	"get_system_as_root_mode",
	"\nThis command will get system_as_root_mode\n"
	"So you can execute command: get_system_as_root_mode"
);

U_BOOT_CMD(
	get_avb_mode, 1,	0, do_GetAvbMode,
	"get_avb_mode",
	"\nThis command will get avb mode\n"
	"So you can execute command: get_avb_mode"
);

