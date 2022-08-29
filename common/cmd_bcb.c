/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_bcb.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <config.h>
#include <asm/arch/io.h>

#ifdef CONFIG_BOOTLOADER_CONTROL_BLOCK
extern int store_read_ops(
    unsigned char *partition_name,
    unsigned char * buf, uint64_t off, uint64_t size);
extern int store_write_ops(
    unsigned char *partition_name,
    unsigned char * buf, uint64_t off, uint64_t size);

#define COMMANDBUF_SIZE 32
#define STATUSBUF_SIZE      32
#define RECOVERYBUF_SIZE 768

#define BOOTINFO_OFFSET 864
#define SLOTBUF_SIZE    32
#define MISCBUF_SIZE  1088

#define CMD_WIPE_DATA          "wipe_data"
#define CMD_SYSTEM_CRASH    "system_crash"
#define CMD_RUN_RECOVERY   "boot-recovery"
#define CMD_RESIZE_DATA    "resize2fs_data"
#define CMD_FOR_RECOVERY "recovery_"
#define CMD_FASTBOOTD          "fastbootd"

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
    char slot_suffix[32];
    char reserved[192];
};


static int clear_misc_partition(char *clearbuf, int size)
{
    char *partition = "misc";

    memset(clearbuf, 0, size);
    if (store_write_ops((unsigned char *)partition,
        (unsigned char *)clearbuf, 0, size) < 0) {
        printf("failed to clear %s.\n", partition);
        return -1;
    }

    return 0;
}

static int do_RunBcbCommand(
    cmd_tbl_t * cmdtp,
    int flag,
    int argc,
    char * const argv[])
{
    int i = 0;
    char command[COMMANDBUF_SIZE] = {0};
    char status[STATUSBUF_SIZE] = {0};
    char recovery[RECOVERYBUF_SIZE] = {0};
    char miscbuf[MISCBUF_SIZE] = {0};
    char clearbuf[COMMANDBUF_SIZE+STATUSBUF_SIZE+RECOVERYBUF_SIZE] = {0};
    char* RebootMode;
    char* ActiveSlot;

    if (argc != 2) {
        return cmd_usage(cmdtp);
    }

    printf("Command: ");
    for (i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");

    char *partition = "misc";
    char *command_mark = (char *)argv[1];

    if (strlen(command_mark) > sizeof(command)) {
        //printf("Bcb command mark range out of length(%d > %d).\n",
            //strlen(command_mark), sizeof(command));
        goto ERR;
    }

    if (!memcmp(command_mark, CMD_WIPE_DATA, strlen(command_mark))) {
        printf("Start to write --wipe_data to %s\n", partition);
        memcpy(miscbuf, CMD_RUN_RECOVERY, sizeof(CMD_RUN_RECOVERY));
        memcpy(miscbuf+sizeof(command)+sizeof(status), "recovery\n--wipe_data", sizeof("recovery\n--wipe_data"));
        store_write_ops((unsigned char *)partition, (unsigned char *)miscbuf, 0, sizeof(miscbuf));
    } else if (!memcmp(command_mark, CMD_SYSTEM_CRASH, strlen(command_mark))) {
        printf("Start to write --system_crash to %s\n", partition);
        memcpy(miscbuf, CMD_RUN_RECOVERY, sizeof(CMD_RUN_RECOVERY));
        memcpy(miscbuf+sizeof(command)+sizeof(status), "recovery\n--system_crash", sizeof("recovery\n--system_crash"));
        store_write_ops((unsigned char *)partition, (unsigned char *)miscbuf, 0, sizeof(miscbuf));
    } else if (!memcmp(command_mark, CMD_RESIZE_DATA, strlen(command_mark))) {
        printf("Start to write --resize2fs_data to %s\n", partition);
        memcpy(miscbuf, CMD_RUN_RECOVERY, sizeof(CMD_RUN_RECOVERY));
        memcpy(miscbuf+sizeof(command)+sizeof(status), "recovery\n--resize2fs_data", sizeof("recovery\n--resize2fs_data"));
        store_write_ops((unsigned char *)partition, (unsigned char *)miscbuf, 0, sizeof(miscbuf));
    } else if (!memcmp(command_mark, CMD_FOR_RECOVERY, strlen(CMD_FOR_RECOVERY))) {
        memcpy(miscbuf, CMD_RUN_RECOVERY, sizeof(CMD_RUN_RECOVERY));
        sprintf(recovery, "%s%s", "recovery\n--", command_mark);
        memcpy(miscbuf+sizeof(command)+sizeof(status), recovery, strlen(recovery));
        store_write_ops((unsigned char *)partition, (unsigned char *)miscbuf, 0, sizeof(miscbuf));
        return 0;
    } else if (!memcmp(command_mark, CMD_FASTBOOTD, strlen(command_mark))) {
        printf("write cmd to enter fastbootd \n");
        memcpy(miscbuf, CMD_RUN_RECOVERY, sizeof(CMD_RUN_RECOVERY));
        memcpy(miscbuf+sizeof(command)+sizeof(status), "recovery\n--fastboot", sizeof("recovery\n--fastboot"));
        store_write_ops((unsigned char *)partition, (unsigned char *)miscbuf, 0, sizeof(miscbuf));
        return 0;
    }

    printf("Start read %s partition datas!\n", partition);
    if (store_read_ops((unsigned char *)partition,
        (unsigned char *)miscbuf, 0, sizeof(miscbuf)) < 0) {
        printf("failed to store read %s.\n", partition);
        goto ERR;
    }

    // judge misc partition whether has datas
    char tmpbuf[MISCBUF_SIZE];
    memset(tmpbuf, 0, sizeof(tmpbuf));
    if (!memcmp(tmpbuf, miscbuf, strlen(miscbuf))) {
        printf("BCB hasn't any datas,exit!\n");
        return 0;
    }

    memcpy(command, miscbuf, sizeof(command));
    memcpy(status, miscbuf+sizeof(command), sizeof(status));
    memcpy(recovery, miscbuf+sizeof(command)+sizeof(status), sizeof(recovery));
    memcpy(clearbuf, miscbuf, sizeof(clearbuf));

    printf("get bootloader message from misc partition:\n");
    printf("[command:%s]\n[status:%s]\n[recovery:%s]\n",
            command, status, recovery);

    run_command("get_rebootmode", 0);
    RebootMode = getenv("reboot_mode");
    if (RebootMode && (strstr(RebootMode, "quiescent") != NULL)) {
        printf("quiescent mode.\n");
        run_command("run storeargs", 0);
        run_command("setenv bootargs ${bootargs} androidboot.quiescent=1;", 0);
    }

	ActiveSlot = getenv("active_slot");
	if (!ActiveSlot) {
		run_command("get_valid_slot", 0);
		ActiveSlot = getenv("active_slot");
	}
	if (ActiveSlot && !strstr(ActiveSlot, "normal")) {
		printf("ab update mode\n");
		run_command("setenv bootargs ${bootargs} androidboot.slot_suffix=${active_slot};",
			0);
	}

    if (!memcmp(command, CMD_RUN_RECOVERY, strlen(CMD_RUN_RECOVERY))) {
        if (run_command("run recovery_from_flash", 0) < 0) {
            printf("run_command for cmd:run recovery_from_flash failed.\n");
            return -1;
        }
        printf("run command:run recovery_from_flash successful.\n");
        return 0;
    }

    if (!memcmp(command_mark, command, strlen(command_mark))) {
        printf("%s\n", recovery);
        if (run_command((char *)recovery, 0) < 0) {
            printf("run_command for cmd:%s failed.\n", recovery);
            goto ERR;
        }
        printf("run command successful.\n");

        if (clear_misc_partition(clearbuf, sizeof(clearbuf)) < 0) {
            printf("clear misc partition failed.\n");
            goto ERR;
        } else {
            printf("clear misc partition successful.\n");
        }
    } else {
        printf("command mark(%s) not match %s,don't execute.\n",
            command_mark, command);
    }

    return 0;

 ERR:
    return -1;
}

static int do_RunAmlBcbCommand(
    cmd_tbl_t * cmdtp,
    int flag,
    int argc,
    char * const argv[]) {
    if (argc != 1) {
        return cmd_usage(cmdtp);
    }

    static int status = 0;
    char *partition = "misc";
    char recovery[RECOVERYBUF_SIZE] = {0};
    char miscbuf[MISCBUF_SIZE] = {0};
    unsigned long long cmdaddr;

    if (status == 1) {
        setenv("reboot_mode","update");
        return 0;
    }

    printf("Start read %s partition datas!\n", partition);
    if (store_read_ops((unsigned char *)partition,
        (unsigned char *)miscbuf, 0, sizeof(miscbuf)) < 0) {
        printf("failed to store read %s.\n", partition);
        return 0;
    }

    // judge misc partition whether has datas
    char tmpbuf[MISCBUF_SIZE];
    memset(tmpbuf, 0, sizeof(tmpbuf));
    if (!memcmp(tmpbuf, miscbuf, strlen(miscbuf))) {
        printf("BCB hasn't any datas,exit!\n");
    } else {
        memcpy(recovery, miscbuf+COMMANDBUF_SIZE+STATUSBUF_SIZE, sizeof(recovery));
        if (strstr(recovery, CMD_WIPE_DATA)) {
            return 0;
        }
    }

    int ret = run_command("ext4load mmc 1:2 ${loadaddr} /recovery/command 100", 1);
    if (ret != 0) {
        ret = run_command("ext4load mmc 1:2 ${loadaddr} /recovery/command", 1);
    }

    if (ret == 0) {
        const char *paddr =  getenv("loadaddr");
        if (paddr != NULL) {
            cmdaddr = simple_strtoul(paddr, NULL, 16);
            char *command = (char *)map_sysmem(cmdaddr, 0);
            if (command && strstr(command, CMD_WIPE_DATA)) {
                unmap_sysmem(command);
                return 0;
            }
            unmap_sysmem(command);
        }
    }

    printf("run recovery, but not factory reset.\n");
    setenv("reboot_mode","update");
    status = 1;
    // Do-Nothing!
    return 0;
}
#else
static int do_RunBcbCommand(
    cmd_tbl_t * cmdtp,
    int flag,
    int argc,
    char * const argv[]) {
    if (argc != 2) {
        return cmd_usage(cmdtp);
    }

    // Do-Nothing!
    return 0;
}

static int do_RunAmlBcbCommand(
    cmd_tbl_t * cmdtp,
    int flag,
    int argc,
    char * const argv[]) {
    if (argc != 1) {
        return cmd_usage(cmdtp);
    }

    // Do-Nothing!
    return 0;
}
#endif /* CONFIG_BOOTLOADER_CONTROL_BLOCK */


// BCB: Bootloader Control Block
U_BOOT_CMD(
    bcb, 2, 0, do_RunBcbCommand,
    "bcb",
    "\nThis command will run some commands which saved in misc\n"
    "partition by mark to decide whether execute command!\n"
    "Command format:\n"
    "  bcb bcb_mark\n"
    "Example:\n"
    "  /dev/block/misc partiton is saved some contents:\n"
    "  uboot-command\n" // command mark
    "  N/A\n"
    "  setenv aa 11;setenv bb 22;setenv cc 33;saveenv;\n"   // command
    "So you can execute command: bcb uboot-command"
);

U_BOOT_CMD(
    aml_bcb, 1, 0, do_RunAmlBcbCommand,
    "aml_bcb",
    "\nThis command will change reboot_mode from factory_reset\n"
    "to update if not contain wipe_data !"
);
