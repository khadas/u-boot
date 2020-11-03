/*
* Copyright (C) 2017 Amlogic, Inc. All rights reserved.
* *
This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* *
This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
* *
You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
* *
Description:
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

#ifdef CONFIG_BOOTLOADER_CONTROL_BLOCK

// Spaces used by misc partition are as below:
// 0   - 2K     For bootloader_message
// 2K  - 16K    Used by Vendor's bootloader (the 2K - 4K range may be optionally used
//              as bootloader_message_ab struct)
// 16K - 32K    Used by uncrypt and recovery to store wipe_package for A/B devices
// 32K - 64K    System space, used for miscellanious AOSP features. See below.
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

    // The 'recovery' field used to be 1024 bytes.  It has only ever
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
}slot_metadata;

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
    uint8_t reserved0[1];
    // Per-slot information.  Up to 4 slots.
    struct slot_metadata slot_info[4];
    // Reserved for further use.
    uint8_t reserved1[8];
    // CRC32 of all 28 bytes preceding this field (little endian
    // format).
    uint32_t crc32_le;
}bootloader_control;

// Holds Virtual A/B merge status information. Current version is 1. New fields
// must be added to the end.
struct misc_virtual_ab_message {
    uint8_t version;
    uint32_t magic;
    uint8_t merge_status;  // IBootControl 1.1, MergeStatus enum.
    uint8_t source_slot;   // Slot number when merge_status was written.
    uint8_t reserved[57];
};

#define MISC_VIRTUAL_AB_MESSAGE_VERSION 2
#define MISC_VIRTUAL_AB_MAGIC_HEADER 0x56740AB0

unsigned int kDefaultBootAttempts = 7;

bool boot_info_validate(bootloader_control* info)
{
    if (info->magic != BOOT_CTRL_MAGIC) {
        printf("Magic 0x%x is incorrect.\n", info->magic);
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
    boot_ctrl->nb_slot = 1;

    for (slot = 0; slot < 4; ++slot) {
        slot_metadata entry = {};

        if (slot < boot_ctrl->nb_slot) {
            entry.priority = 7;
            entry.tries_remaining = kDefaultBootAttempts;
            entry.successful_boot = 1;
        } else {
            entry.priority = 0;  // Unbootable
            entry.tries_remaining = 0;
            entry.successful_boot = 0;
        }

        boot_ctrl->slot_info[slot] = entry;
    }
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
        printf("boot_ctrl->slot_info[%d].priority = %d\n", slot, boot_ctrl->slot_info[slot].priority);
        printf("boot_ctrl->slot_info[%d].tries_remaining = %d\n", slot, boot_ctrl->slot_info[slot].tries_remaining);
        printf("boot_ctrl->slot_info[%d].successful_boot = %d\n", slot, boot_ctrl->slot_info[slot].successful_boot);
    }
    printf("boot_ctrl->recovery_tries_remaining = %d\n", boot_ctrl->recovery_tries_remaining);
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


int boot_info_set_active_slot(bootloader_control* bootctrl, int slot)
{
    int i;
    // Set every other slot with a lower priority than the new "active" slot.
    const unsigned int kActivePriority = 15;
    const unsigned int kActiveTries = 6;
    for (i = 0; i < bootctrl->nb_slot; ++i) {
        if (i != slot) {
            if (bootctrl->slot_info[i].priority >= kActivePriority)
                bootctrl->slot_info[i].priority = kActivePriority - 1;
        }
        printf("bootctrl->slot_info[%d].priority = %d\n", i, bootctrl->slot_info[i].priority);
    }

    // Note that setting a slot as active doesn't change the successful bit.
    // The successful bit will only be changed by setSlotAsUnbootable().
    bootctrl->slot_info[slot].priority = kActivePriority;
    bootctrl->slot_info[slot].tries_remaining = kActiveTries;

    printf("bootctrl->slot_info[%d].priority = %d\n", slot, bootctrl->slot_info[slot].priority);
    printf("bootctrl->slot_info[%d].tries_remaining = %d\n", slot, bootctrl->slot_info[slot].tries_remaining);

    // Setting the current slot as active is a way to revert the operation that
    // set *another* slot as active at the end of an updater. This is commonly
    // used to cancel the pending update. We should only reset the verity_corrpted
    // bit when attempting a new slot, otherwise the verity bit on the current
    // slot would be flip.
    if (slot != get_active_slot(bootctrl)) bootctrl->slot_info[slot].verity_corrupted = 0;

    dump_boot_info(bootctrl);

    return 0;
}

int boot_info_open_partition(char *miscbuf)
{
    char *partition = "misc";
    printf("Start read %s partition datas!\n", partition);
    if (store_read((unsigned char *)partition,
        0, MISCBUF_SIZE, (unsigned char *)miscbuf) < 0) {
        printf("failed to store read %s.\n", partition);
        return -1;
    }
    return 0;
}

bool boot_info_load(bootloader_control *out_info, char *miscbuf)
{
    memcpy(out_info, miscbuf+AB_METADATA_MISC_PARTITION_OFFSET, sizeof(bootloader_control));
    dump_boot_info(out_info);
    return true;
}

bool boot_info_save(bootloader_control *info, char *miscbuf)
{
    char *partition = "misc";
    printf("save boot-info \n");
    info->crc32_le = avb_htobe32(
      avb_crc32((const uint8_t*)info, sizeof(bootloader_control) - sizeof(uint32_t)));

    memcpy(miscbuf+AB_METADATA_MISC_PARTITION_OFFSET, info, sizeof(bootloader_control));
    dump_boot_info(info);
    store_write((unsigned char *)partition, 0, MISCBUF_SIZE, (unsigned char *)miscbuf);
    return true;
}

static int do_GetValidSlot(
    cmd_tbl_t * cmdtp,
    int flag,
    int argc,
    char * const argv[])
{
    char miscbuf[MISCBUF_SIZE] = {0};
    bootloader_control boot_ctrl;
    int slot;
    bool bootable_a, bootable_b;

    if (argc != 1) {
        return cmd_usage(cmdtp);
    }

    boot_info_open_partition(miscbuf);
    boot_info_load(&boot_ctrl, miscbuf);

    if (!boot_info_validate(&boot_ctrl)) {
        printf("boot-info is invalid. Resetting.\n");
        boot_info_reset(&boot_ctrl);
        boot_info_save(&boot_ctrl, miscbuf);
    }

    slot = get_active_slot(&boot_ctrl);
    printf("active slot = %d\n", slot);

    bootable_a = slot_is_bootable(&(boot_ctrl.slot_info[0]));
    bootable_b = slot_is_bootable(&(boot_ctrl.slot_info[1]));

    if (dynamic_partition)
        env_set("partiton_mode","dynamic");
    else
        env_set("partiton_mode","normal");

    if (vendor_boot_partition) {
        env_set("vendor_boot_mode","true");
        printf("set vendor_boot_mode true\n");
    }
    else {
        env_set("vendor_boot_mode","false");
        printf("set vendor_boot_mode false\n");
    }

    if ((slot == 0) && (bootable_a)) {
        if (has_boot_slot == 1) {
            env_set("active_slot","_a");
            env_set("boot_part","boot_a");
            env_set("recovery_part","recovery_a");
            env_set("slot-suffixes","0");
        }
        else {
            env_set("active_slot","normal");
            env_set("boot_part","boot");
            env_set("recovery_part","recovery");
            env_set("slot-suffixes","-1");
        }
        return 0;
    }

    if ((slot == 1) && (bootable_b)) {
        if (has_boot_slot == 1) {
            env_set("active_slot","_b");
            env_set("boot_part","boot_b");
            env_set("recovery_part","recovery_b");
            env_set("slot-suffixes","1");
        }
        else {
            env_set("active_slot","normal");
            env_set("boot_part","boot");
            env_set("recovery_part","recovery");
            env_set("slot-suffixes","-1");
        }
        return 0;
    }

    return 0;
}

static int do_SetActiveSlot(
    cmd_tbl_t * cmdtp,
    int flag,
    int argc,
    char * const argv[])
{
    char miscbuf[MISCBUF_SIZE] = {0};
    bootloader_control info;

    if (argc != 2) {
        return cmd_usage(cmdtp);
    }

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
        env_set("active_slot","_a");
        env_set("slot-suffixes","0");
        env_set("boot_part","boot_a");
        env_set("recovery_part","recovery_a");
        printf("set active slot a \n");
        boot_info_set_active_slot(&info, 0);
    } else if (strcmp(argv[1], "b") == 0) {
        env_set("active_slot","_b");
        env_set("slot-suffixes","1");
        env_set("boot_part","boot_b");
        env_set("recovery_part","recovery_b");
        printf("set active slot b \n");
        boot_info_set_active_slot(&info, 1);
    } else {
        printf("error input slot\n");
        return -1;
    }

    boot_info_save(&info, miscbuf);

    return 0;
}

int do_GetSystemMode (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char* system;
#ifdef CONFIG_SYSTEM_AS_ROOT
    system = CONFIG_SYSTEM_AS_ROOT;
    strcpy(system, CONFIG_SYSTEM_AS_ROOT);
    printf("CONFIG_SYSTEM_AS_ROOT: %s \n", CONFIG_SYSTEM_AS_ROOT);
    if (strcmp(system, "systemroot") == 0) {
        env_set("system_mode","1");
    }
    else {
        env_set("system_mode","0");
    }
#else
    env_set("system_mode","0");
#endif

    return 0;
}

int do_GetAvbMode (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_AVB2
    char* avbmode;
    avbmode = CONFIG_AVB2;
    strcpy(avbmode, CONFIG_AVB2);
    printf("CONFIG_AVB2: %s \n", CONFIG_AVB2);
    if (strcmp(avbmode, "avb2") == 0) {
        env_set("avb2","1");
    }
    else {
        env_set("avb2","0");
    }
#else
    env_set("avb2","0");
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

