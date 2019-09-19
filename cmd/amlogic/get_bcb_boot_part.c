/*
 * \file        get_bcb_boot_part.c
 * \brief       parse bootloader control blob and get booted part name
 *
 * \version     1.0.0
 * \date        09/19/2019
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2019 Amlogic. All Rights Reserved.
 *
 */
#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <config.h>
#include <asm/arch/io.h>
#include <partition_table.h>
#include <version.h>
#include <amlogic/store_wrapper.h>

#define COMMANDBUF_SIZE 32
#define STATUSBUF_SIZE      32
#define RECOVERYBUF_SIZE 768

#define BOOTINFO_OFFSET 864
#define SLOTBUF_SIZE    32
#define BCB_INFO_SZ  1024

#define DbgP(fmt...)    //printf("D[BCB]L%d:", __LINE__),printf(fmt)
#define ErrP(fmt...)    printf("E[BCB]L%d:", __LINE__),printf(fmt)
#define MsgP(fmt...)    printf("[BCB]"fmt)

#pragma pack(push, 4)
struct recovery_message {
    char command[32];
    char status[32];
    char recovery[768];

    // The 'recovery' field used to be 1024 bytes.  It has only ever
    // been used to store the recovery command line, so 768 bytes
    // should be plenty.  We carve off the last 256 bytes to store the
    // stage string (for multistage packages) and possible future
    // expansion.
    char stage[32];
};

typedef struct BrilloSlotInfo {
    uint8_t bootable;
    uint8_t online;
    uint8_t reserved[2];
} BrilloSlotInfo;

#define MAX_SLOT_NUM 4
typedef struct BrilloBootInfo {
    // Used by fs_mgr. Must be NUL terminated.
    char bootctrl_suffix[4];

    // Magic for identification - must be 'B', 'C', 'c' (short for
    // "boot_control copy" implementation).
    uint8_t magic[3];

    // Version of BrilloBootInfo struct, must be 0 or larger.
    uint8_t version;

    // Currently active slot.
    uint8_t active_slot;

    uint8_t attemp_times;
    uint8_t maxTryTimes;
    uint8_t numSlots;//slots number, >=2 if support multiple system switch

    // Information about each slot.
    BrilloSlotInfo slot_info[MAX_SLOT_NUM]; //at last and can extend to any number you need

} BrilloBootInfo;

struct bootloader_control_msg {
    struct recovery_message        recovery;
    struct BrilloBootInfo          bootInfo;
};
#pragma pack(pop)

static bool boot_info_validate(BrilloBootInfo* info)
{
    if (info->magic[0] != 'B' ||
        info->magic[1] != 'C' ||
        info->magic[2] != 'c')
        return false;
    if (info->active_slot >= MAX_SLOT_NUM)
        return false;
    return true;
}

static void dump_boot_info(BrilloBootInfo* info)
{
    MsgP("info->active_slot = %u\n", info->active_slot);
    MsgP("info->attemp_times = %u / %u\n", info->attemp_times, info->maxTryTimes);
    MsgP("info->slot_info[0].bootable = %u\n", info->slot_info[0].bootable);
    MsgP("info->slot_info[0].online = %u\n", info->slot_info[0].online);
    MsgP("info->slot_info[1].bootable = %u\n", info->slot_info[1].bootable);
    MsgP("info->slot_info[1].online = %u\n", info->slot_info[1].online);
}

/*
 * code flow:
 *      if recovery_message->command valid, yes then run into upgrade mode
 *      if info->numSlots == 1, then NOT a/b system mode, just set boot_part as 'boot'
 *      if info->numSlots > 1, then IS a/b system mode
 *           if attemp_times >= maxTryTimes; then
 *              try info[active_slot]
 *           else try next slot
 * */
static int parse_and_update_bcb(struct bootloader_control_msg* bcb, int* needUpdateMisc)
{
    struct recovery_message* recoveryInfo = &(bcb->recovery);
    BrilloBootInfo* info = &(bcb->bootInfo);

    *needUpdateMisc = 1;
    DbgP("command:%s, recovery:%s\n", recoveryInfo->command, recoveryInfo->recovery);
    if (!memcmp(recoveryInfo->command, "boot-recovery", strlen("boot-recovery"))) {
        MsgP("need boot recovery, maybe upgrading not completed\n");
        run_command("run storeargs; run update", 0);
        return 0;
    }

    if (!boot_info_validate(info)) {
        MsgP("boot-info is invalid. Resetting.\n");
        return CMD_RET_FAILURE;
    }
    dump_boot_info(info);

    const int numSlots = info->numSlots;
    if (numSlots < 1) {
        ErrP("slots num %d invalid\n", numSlots);
        return CMD_RET_FAILURE;
    }
    if (numSlots < 2) {
        MsgP("slots num %d < 2, so don't support multiple system switch\n", numSlots);
        env_set("active_slot","normal");
        env_set("boot_part","boot");
        *needUpdateMisc = 0;
        return 0;
    }

    int slotIndex = info->active_slot;
    MsgP("active slot = %d \n", slotIndex);
    BrilloSlotInfo* slotInf = info->slot_info + slotIndex;

    int attemp_times = info->attemp_times;
    MsgP("attemp_times = %d \n", attemp_times);
    do {
        if (slotInf->bootable) {
            MsgP("current slot %d already bootable, just boot it\n", slotIndex);
            *needUpdateMisc = 0;
            break;
        }

        //current active slot NOT bootable, try until system marked bootable
        if (attemp_times > info->maxTryTimes) {
            MsgP("Try next as slot[%d] tried times %d > max %d!!!\n", slotIndex, attemp_times, (int)(info->maxTryTimes));
            attemp_times = 0;
            slotIndex  += 1;
            slotIndex   = slotIndex < numSlots ? slotIndex : (slotIndex - numSlots);
            slotInf     = info->slot_info + slotIndex;
            continue;
        }
        attemp_times += 1;
        break;
    }while(1);
    info->attemp_times = attemp_times;
    info->active_slot  = slotIndex;

    char bootName[8];
    const int iPostfix = 4;
    const int nlen = strlen("bootA");
    memcpy(bootName, "bootA", nlen + 1);
    if (slotIndex > 0) bootName[iPostfix] += slotIndex;
    env_set("active_slot", bootName + iPostfix);//"_a" or "-b"
    env_set("boot_part", bootName);
    run_command("setenv dsp_part dsp${active_slot}", 0);
    memcpy(info->bootctrl_suffix, bootName + iPostfix, iPostfix);

    char* slotSuffix = bootName + iPostfix;//"a" or "b"
    slotSuffix[0] = slotSuffix[0] - 'A' + '0'; //"0" or "1"
    env_set("slot-suffixes", slotSuffix);
    run_command("printenv slot-suffixes boot_part active_slot dsp_part", 0);

    return 0;
}

static int do_GetValidSlot(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    struct bootloader_control_msg* loadaddr = NULL;
    const char* bcbPart = argc > 1 ? argv[1] : "misc";
    if ( argc < 3 ) {//default address is env $loadaddr
        loadaddr = (struct bootloader_control_msg*)env_get_hex("loadaddr", 0x4000);
    } else {
        loadaddr = (struct bootloader_control_msg*)simple_strtoul(argv[2], NULL, 16);
    }
    DbgP("bcbPart %s, loadaddr %p\n", bcbPart, loadaddr);
    int rc = store_logic_read(bcbPart, 0, BCB_INFO_SZ, loadaddr);
    if (rc) {
        ErrP("Fail read bcb from part %s\n", bcbPart);
        return CMD_RET_FAILURE;
    }

    int needUpdateMisc = 0;
    rc = parse_and_update_bcb(loadaddr, &needUpdateMisc);
    if (rc) {
        ErrP("Fail parse bcb from part %s\n", bcbPart);
        return CMD_RET_FAILURE;
    }
    if (!needUpdateMisc) return CMD_RET_SUCCESS;

    rc = store_erase(bcbPart, 0, 0, 0);//erase whole misc part
    if (rc) {
        ErrP("Fail erase bcb from part %s\n", bcbPart);
        return CMD_RET_FAILURE;
    }
    rc = store_logic_write(bcbPart, 0, BCB_INFO_SZ, loadaddr);
    if (rc) {
        ErrP("Fail read bcb from part %s\n", bcbPart);
        return CMD_RET_FAILURE;
    }
    return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
    get_boot_part, 3, 0, do_GetValidSlot,
    "get part name to load the bootable part",
    "\nThis command will get which partititon name should be used for booting\n"
    "    argv: get_boot_part <bcbPartName> <loadaddr>\n"   //usage
);

