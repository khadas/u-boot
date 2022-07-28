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
#include <amlogic/storage.h>

#ifdef CONFIG_BOOTLOADER_CONTROL_BLOCK

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

static const char *  const temp_for_compile[] = {"__test1", "__test2", "__test3", NULL};
extern const char * const _env_list_execute_[0] __attribute__((weak, alias("temp_for_compile")));

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

static bool env_command_check(const char *cmd)
{
    int index = 0;
    int envListNum = 0;
    char *s = NULL;
    char *v = NULL;
    char *strcopy = NULL;
    const char **envListArr = (const char **)_env_list_execute_;

    const char **pArr = (const char **)envListArr;

    while (*pArr++) {
	++envListNum;
    }

    printf("envListNum = %d\n", envListNum);

    if (envListNum == 0) {
	return false;
    }

    strcopy = strdup(cmd);
    if (!strcopy) {
	return false;
    }

    s = strcopy;
    while ((v = strsep(&s, ";")) !=  NULL) {
	char *p = v;

	strsep(&p, " ");
	for (index = 0; index < envListNum; index++) {
	if (!strcmp(envListArr[index], v)) {
		break;
	}
	}

	if (index == envListNum) {
	    printf("%s not in the white list.\n", v);
	    free(strcopy);
	    strcopy = NULL;
	return false;
	}
    }

    free(strcopy);
    strcopy = NULL;
    return true;
}

static int clear_misc_partition(char *clearbuf, int size)
{
    char *partition = "misc";

    memset(clearbuf, 0, size);
    if (store_write((const char *)partition,
        0, size, (unsigned char *)clearbuf) < 0) {
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
        store_write((const char *)partition, 0, sizeof(miscbuf), (unsigned char *)miscbuf);
    } else if (!memcmp(command_mark, CMD_SYSTEM_CRASH, strlen(command_mark))) {
        printf("Start to write --system_crash to %s\n", partition);
        memcpy(miscbuf, CMD_RUN_RECOVERY, sizeof(CMD_RUN_RECOVERY));
        memcpy(miscbuf+sizeof(command)+sizeof(status), "recovery\n--system_crash", sizeof("recovery\n--system_crash"));
        store_write((const char *)partition, 0, sizeof(miscbuf), (unsigned char *)miscbuf);
    } else if (!memcmp(command_mark, CMD_RESIZE_DATA, strlen(command_mark))) {
        printf("Start to write --resize2fs_data to %s\n", partition);
        memcpy(miscbuf, CMD_RUN_RECOVERY, sizeof(CMD_RUN_RECOVERY));
        memcpy(miscbuf+sizeof(command)+sizeof(status), "recovery\n--resize2fs_data", sizeof("recovery\n--resize2fs_data"));
        store_write((const char *)partition, 0, sizeof(miscbuf), (unsigned char *)miscbuf);
    } else if (!memcmp(command_mark, CMD_FOR_RECOVERY, strlen(CMD_FOR_RECOVERY))) {
        memcpy(miscbuf, CMD_RUN_RECOVERY, sizeof(CMD_RUN_RECOVERY));
        sprintf(recovery, "%s%s", "recovery\n--", command_mark);
        memcpy(miscbuf+sizeof(command)+sizeof(status), recovery, strlen(recovery));
        store_write((const char *)partition, 0, sizeof(miscbuf), (unsigned char *)miscbuf);
        return 0;
    } else if (!memcmp(command_mark, CMD_FASTBOOTD, strlen(command_mark))) {
        printf("write cmd to enter fastbootd \n");
        memcpy(miscbuf, CMD_RUN_RECOVERY, sizeof(CMD_RUN_RECOVERY));
        memcpy(miscbuf+sizeof(command)+sizeof(status), "recovery\n--fastboot", sizeof("recovery\n--fastboot"));
        store_write((const char *)partition, 0, sizeof(miscbuf), (unsigned char *)miscbuf);
        return 0;
    }

    printf("Start read %s partition datas!\n", partition);
    if (store_read((const char *)partition,
        0, sizeof(miscbuf), (unsigned char *)miscbuf) < 0) {
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
    RebootMode = env_get("reboot_mode");
    if (strstr(RebootMode, "quiescent") != NULL) {
        printf("quiescent mode.\n");
        run_command("run storeargs", 0);
	run_command("setenv bootconfig ${bootconfig} androidboot.quiescent=1;", 0);
    }

    if (!memcmp(command, CMD_RUN_RECOVERY, strlen(CMD_RUN_RECOVERY))) {
        if (run_command("run recovery_from_flash", 0) < 0) {
            printf("run_command for cmd:run recovery_from_flash failed.\n");
            return -1;
        }
        printf("run command:run recovery_from_flash successful.\n");
        return 0;
    }

    //uboot-command only valid once, not matter success or not
    if (clear_misc_partition(clearbuf, sizeof(clearbuf)) < 0) {
	printf("clear misc partition failed.\n");
	goto ERR;
    }

    if (!memcmp(command_mark, command, strlen(command_mark))) {
        printf("%s\n", recovery);
        if (run_command((char *)recovery, 0) < 0) {
            printf("run_command for cmd:%s failed.\n", recovery);
            goto ERR;
        }
        printf("run command successful.\n");
    }  else if (!strncmp(command_mark, "uboot-command", strlen("uboot-command"))) {
		printf("uboot-command: %s\n", command);

		if (!env_command_check(command)) {
			printf("not all uboot-command in white-list\n");
			goto ERR;
		}
		if (run_command((char *)command, 0) < 0) {
			printf("run_command for cmd:%s failed.\n", command);
			goto ERR;
		}
		printf("run uboot-command successful.\n");

    } else {
        printf("command mark(%s) not match %s,don't execute.\n",
            command_mark, command);
    }

    return 0;

 ERR:
    return -1;
}
#else
static int do_RunBcbCommand(
    cmd_tbl_t * cmdtp,
    int flag,
    int argc,
    char * const argv[])
{
    if (argc != 2) {
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
