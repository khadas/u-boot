// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (C) 2016 The Android Open Source Project
 */

#include <common.h>
#include <fastboot.h>
#include <fastboot-internal.h>
#include <fb_mmc.h>
#include <fb_nand.h>
#include <part.h>
#include <stdlib.h>
#include <emmc_partitions.h>

/**
 * image_size - final fastboot image size
 */
static u32 image_size;

/**
 * fastboot_bytes_received - number of bytes received in the current download
 */
static u32 fastboot_bytes_received;

/**
 * fastboot_bytes_expected - number of bytes expected in the current download
 */
static u32 fastboot_bytes_expected;

int busy_flag;

static void okay(char *, char *);
static void getvar(char *, char *);
static void download(char *, char *);
#if CONFIG_IS_ENABLED(FASTBOOT_FLASH)
static void flash(char *, char *);
static void erase(char *, char *);
#endif
static void flashing(char *, char *);
static void reboot_bootloader(char *, char *);
static void reboot_fastboot(char *, char *);
#if CONFIG_IS_ENABLED(FASTBOOT_CMD_OEM_FORMAT)
static void oem_format(char *, char *);
#endif

static const struct {
	const char *command;
	void (*dispatch)(char *cmd_parameter, char *response);
} commands[FASTBOOT_COMMAND_COUNT] = {
	[FASTBOOT_COMMAND_GETVAR] = {
		.command = "getvar",
		.dispatch = getvar
	},
	[FASTBOOT_COMMAND_DOWNLOAD] = {
		.command = "download",
		.dispatch = download
	},
	[FASTBOOT_COMMAND_FLASHING] =  {
		.command = "flashing",
		.dispatch = flashing
	},
#if CONFIG_IS_ENABLED(FASTBOOT_FLASH)
	[FASTBOOT_COMMAND_FLASH] =  {
		.command = "flash",
		.dispatch = flash
	},
	[FASTBOOT_COMMAND_ERASE] =  {
		.command = "erase",
		.dispatch = erase
	},
#endif
	[FASTBOOT_COMMAND_BOOT] =  {
		.command = "boot",
		.dispatch = okay
	},
	[FASTBOOT_COMMAND_CONTINUE] =  {
		.command = "continue",
		.dispatch = okay
	},
	[FASTBOOT_COMMAND_REBOOT_BOOTLOADER] =  {
		.command = "reboot-bootloader",
		.dispatch = reboot_bootloader
	},
	[FASTBOOT_COMMAND_REBOOT_FASTBOOT] =  {
		.command = "reboot-fastboot",
		.dispatch = reboot_fastboot
	},
	[FASTBOOT_COMMAND_REBOOT] =  {
		.command = "reboot",
		.dispatch = okay
	},
	[FASTBOOT_COMMAND_SET_ACTIVE] =  {
		.command = "set_active",
		.dispatch = okay
	},
#if CONFIG_IS_ENABLED(FASTBOOT_CMD_OEM_FORMAT)
	[FASTBOOT_COMMAND_OEM_FORMAT] = {
		.command = "oem format",
		.dispatch = oem_format,
	},
#endif
};

static int strcmp_l1(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return -1;
	return strncmp(s1, s2, strlen(s1));
}

/**
 * fastboot_handle_command - Handle fastboot command
 *
 * @cmd_string: Pointer to command string
 * @response: Pointer to fastboot response buffer
 *
 * Return: Executed command, or -1 if not recognized
 */
int fastboot_handle_command(char *cmd_string, char *response)
{
	int i;
	char *cmd_parameter;

	cmd_parameter = cmd_string;
	strsep(&cmd_parameter, ":");

	for (i = 0; i < FASTBOOT_COMMAND_COUNT; i++) {
		if (!strcmp_l1(commands[i].command, cmd_string)) {
			if (commands[i].dispatch) {
				if (strcmp(cmd_parameter, NULL)) {
					commands[i].dispatch(cmd_parameter,
							response);
				} else {
					commands[i].dispatch(cmd_string,
							response);
				}
				return i;
			} else {
				break;
			}
		}
	}

	pr_err("command %s not recognized.\n", cmd_string);
	fastboot_fail("unrecognized command", response);
	return -1;
}

/**
 * okay() - Send bare OKAY response
 *
 * @cmd_parameter: Pointer to command parameter
 * @response: Pointer to fastboot response buffer
 *
 * Send a bare OKAY fastboot response. This is used where the command is
 * valid, but all the work is done after the response has been sent (e.g.
 * boot, reboot etc.)
 */
static void okay(char *cmd_parameter, char *response)
{
	fastboot_okay(NULL, response);
}

void dump_lock_info(LockData_t* info)
{
	printf("info->version_major = %d\n", info->version_major);
	printf("info->version_minor = %d\n", info->version_minor);
	printf("info->unlock_ability = %d\n", info->unlock_ability);
	printf("info->lock_state = %d\n", info->lock_state);
	printf("info->lock_critical_state = %d\n", info->lock_critical_state);
	printf("info->lock_bootloader = %d\n", info->lock_bootloader);
}

static const char* getvar_list[] = {
	"version-baseband", "version-bootloader", "version", "hw-revision", "max-download-size",
	"serialno", "product", "off-mode-charge", "variant", "battery-soc-ok",
	"battery-voltage", "partition-type:boot", "partition-size:boot",
	"partition-type:system", "partition-size:system", "partition-type:vendor", "partition-size:vendor",
	"partition-type:odm", "partition-size:odm", "partition-type:data", "partition-size:data",
	"erase-block-size", "logical-block-size", "secure", "unlocked",
};

static const char* getvar_list_dynamic[] = {
	"hw-revision", "battery-voltage", "is-userspace", "is-logical:data",
	"is-logical:metadata", "is-logical:misc", "is-logical:super", "is-logical:boot",
	"is-logical:system", "is-logical:vendor", "is-logical:product", "is-logical:odm",
	"slot-count", "max-download-size", "serialno", "product", "unlocked",
	"secure", "super-partition-name", "version-baseband", "version-bootloader",
	"partition-size:boot", "partition-size:metadata", "partition-size:misc",
	"partition-size:super", "partition-size:data", "version",
};

static const char* getvar_list_dynamic_ab[] = {
	"hw-revision", "battery-voltage", "is-userspace", "is-logical:data",
	"is-logical:misc", "is-logical:super",
	"is-logical:boot_a", "is-logical:boot_b", "is-logical:system_a", "is-logical:system_b",
	"is-logical:vendor_a", "is-logical:vendor_b", "is-logical:product_a", "is-logical:product_b",
	"is-logical:odm_a", "is-logical:odm_b",
	"slot-count", "max-download-size", "serialno", "product", "unlocked", "has-slot:data",
	"has-slot:metadata", "has-slot:misc", "has-slot:super", "has-slot:boot",
	"has-slot:system", "has-slot:vendor", "has-slot:product", "has-slot:odm", "current-slot",
	"secure", "super-partition-name", "version-baseband", "version-bootloader",
	"partition-size:super",
	"partition-size:boot_a", "partition-size:boot_b", "partition-size:misc",
	"partition-size:data", "version",
};


static const char* getvar_list_ab[] = {
	"version-baseband", "version-bootloader", "version", "hw-revision", "max-download-size",
	"serialno", "product", "off-mode-charge", "variant", "battery-soc-ok",
	"battery-voltage", "partition-type:boot_a", "partition-size:boot_a",
	"partition-type:system_a", "partition-size:system_a", "partition-type:vendor_a", "partition-size:vendor_a",
	"partition-type:odm_a", "partition-size:odm_a", "partition-type:data", "partition-size:data",
	"erase-block-size", "logical-block-size", "secure", "unlocked",
	"slot-count", "slot-suffixes","current-slot", "has-slot:bootloader", "has-slot:boot",
	"has-slot:system", "has-slot:vendor", "has-slot:odm", "has-slot:vbmeta",
	"has-slot:metadata", "has-slot:product", "has-slot:dtbo",
	"slot-successful:a", "slot-unbootable:a", "slot-retry-count:a",
	"slot-successful:b", "slot-unbootable:b", "slot-retry-count:b",
};


/**
 * getvar() - Read a config/version variable
 *
 * @cmd_parameter: Pointer to command parameter
 * @response: Pointer to fastboot response buffer
 */
static void getvar(char *cmd_parameter, char *response)
{
	run_command("get_valid_slot", 0);
	if (!strncmp(cmd_parameter, "all", 3)) {
		static int cmdIndex = 0;
		int getvar_num = 0;
		char* cmd=cmd_parameter;

		busy_flag = 1;

		if (dynamic_partition && has_boot_slot == 1 && strlen(getvar_list_dynamic_ab[cmdIndex]) < 64) {
			strncpy(cmd, getvar_list_dynamic_ab[cmdIndex], 64);
			getvar_num = (sizeof(getvar_list_dynamic_ab) / sizeof(getvar_list_dynamic_ab[0]));
		} else if (has_boot_slot == 1 && strlen(getvar_list_ab[cmdIndex]) < 64) {
			strncpy(cmd, getvar_list_ab[cmdIndex], 64);
			getvar_num = (sizeof(getvar_list_ab) / sizeof(getvar_list_ab[0]));
		} else if (dynamic_partition && strlen(getvar_list_dynamic[cmdIndex]) < 64) {
			strncpy(cmd, getvar_list_dynamic[cmdIndex], 64);//only support no-arg cmd
			getvar_num = (sizeof(getvar_list_dynamic) / sizeof(getvar_list_dynamic[0]));
		} else if (strlen(getvar_list[cmdIndex]) < 64) {
			strncpy(cmd, getvar_list[cmdIndex], 64);//only support no-arg cmd
			getvar_num = (sizeof(getvar_list) / sizeof(getvar_list[0]));
		}
		//printf("getvar_num: %d\n", getvar_num);
		//printf("all cmd:%s\n", cmd);
		if ( ++cmdIndex >= getvar_num) {
			cmdIndex = 0;
			busy_flag = 0;
			fastboot_okay(NULL, response);
		} else {
			fastboot_getvar(cmd, response);
		}

	}else {
		fastboot_getvar(cmd_parameter, response);
	}
}

/**
 * fastboot_download() - Start a download transfer from the client
 *
 * @cmd_parameter: Pointer to command parameter
 * @response: Pointer to fastboot response buffer
 */
static void download(char *cmd_parameter, char *response)
{
	char *tmp;

	if (!cmd_parameter) {
		fastboot_fail("Expected command parameter", response);
		return;
	}
	fastboot_bytes_received = 0;
	fastboot_bytes_expected = simple_strtoul(cmd_parameter, &tmp, 16);
	if (fastboot_bytes_expected == 0) {
		fastboot_fail("Expected nonzero image size", response);
		return;
	}
	/*
	 * Nothing to download yet. Response is of the form:
	 * [DATA|FAIL]$cmd_parameter
	 *
	 * where cmd_parameter is an 8 digit hexadecimal number
	 */
	if (fastboot_bytes_expected > fastboot_buf_size) {
		fastboot_fail(cmd_parameter, response);
	} else {
		printf("Starting download of %d bytes\n",
		       fastboot_bytes_expected);
		fastboot_response("DATA", response, "%s", cmd_parameter);
	}
}

/**
 * fastboot_data_remaining() - return bytes remaining in current transfer
 *
 * Return: Number of bytes left in the current download
 */
u32 fastboot_data_remaining(void)
{
	return fastboot_bytes_expected - fastboot_bytes_received;
}

/**
 * fastboot_data_download() - Copy image data to fastboot_buf_addr.
 *
 * @fastboot_data: Pointer to received fastboot data
 * @fastboot_data_len: Length of received fastboot data
 * @response: Pointer to fastboot response buffer
 *
 * Copies image data from fastboot_data to fastboot_buf_addr. Writes to
 * response. fastboot_bytes_received is updated to indicate the number
 * of bytes that have been transferred.
 *
 * On completion sets image_size and ${filesize} to the total size of the
 * downloaded image.
 */
void fastboot_data_download(const void *fastboot_data,
			    unsigned int fastboot_data_len,
			    char *response)
{
#define BYTES_PER_DOT	0x20000
	u32 pre_dot_num, now_dot_num;

	if (fastboot_data_len == 0 ||
	    (fastboot_bytes_received + fastboot_data_len) >
	    fastboot_bytes_expected) {
		fastboot_fail("Received invalid data length",
			      response);
		return;
	}
	/* Download data to fastboot_buf_addr */
	memcpy(fastboot_buf_addr + fastboot_bytes_received,
	       fastboot_data, fastboot_data_len);

	pre_dot_num = fastboot_bytes_received / BYTES_PER_DOT;
	fastboot_bytes_received += fastboot_data_len;
	now_dot_num = fastboot_bytes_received / BYTES_PER_DOT;

	if (pre_dot_num != now_dot_num) {
		putc('.');
		if (!(now_dot_num % 74))
			putc('\n');
	}
	*response = '\0';
}

/**
 * fastboot_data_complete() - Mark current transfer complete
 *
 * @response: Pointer to fastboot response buffer
 *
 * Set image_size and ${filesize} to the total size of the downloaded image.
 */
void fastboot_data_complete(char *response)
{
	/* Download complete. Respond with "OKAY" */
	fastboot_okay(NULL, response);
	printf("\ndownloading of %d bytes finished\n", fastboot_bytes_received);
	image_size = fastboot_bytes_received;
	env_set_hex("filesize", image_size);
	fastboot_bytes_expected = 0;
	fastboot_bytes_received = 0;
}

#if CONFIG_IS_ENABLED(FASTBOOT_FLASH)
/**
 * flash() - write the downloaded image to the indicated partition.
 *
 * @cmd_parameter: Pointer to partition name
 * @response: Pointer to fastboot response buffer
 *
 * Writes the previously downloaded image to the partition indicated by
 * cmd_parameter. Writes to response.
 */
static void flash(char *cmd_parameter, char *response)
{
	char name[32];

	if (strcmp(cmd_parameter, "userdata") == 0)
		strncpy(name, "data", 4);
	else if (strcmp(cmd_parameter, "dts") == 0)
		strncpy(name, "dtb", 3);
	else
		strncpy(name, cmd_parameter, 32);

	if (check_lock()) {
		printf("device is locked, can not run this cmd.Please flashing unlock & flashing unlock_critical\n");
		fastboot_fail("locked device", response);
		return;
	}

#if CONFIG_IS_ENABLED(FASTBOOT_FLASH_MMC)
	fastboot_mmc_flash_write(name, fastboot_buf_addr, image_size,
				 response);
#endif
#if CONFIG_IS_ENABLED(FASTBOOT_FLASH_NAND)
	fastboot_nand_flash_write(name, fastboot_buf_addr, image_size,
				  response);
#endif
}

/**
 * erase() - erase the indicated partition.
 *
 * @cmd_parameter: Pointer to partition name
 * @response: Pointer to fastboot response buffer
 *
 * Erases the partition indicated by cmd_parameter (clear to 0x00s). Writes
 * to response.
 */
static void erase(char *cmd_parameter, char *response)
{
	char name[32];

	if (strcmp(cmd_parameter, "userdata") == 0)
		strncpy(name, "data", 4);
	else if (strcmp(cmd_parameter, "dts") == 0)
		strncpy(name, "dtb", 3);
	else
		strncpy(name, cmd_parameter, 32);

	if (check_lock()) {
		printf("device is locked, can not run this cmd.Please flashing unlock & flashing unlock_critical\n");
		fastboot_fail("locked device", response);
		return;
	}

#if CONFIG_IS_ENABLED(FASTBOOT_FLASH_MMC)
	fastboot_mmc_erase(name, response);
#endif
#if CONFIG_IS_ENABLED(FASTBOOT_FLASH_NAND)
	fastboot_nand_erase(name, response);
#endif
}
#endif

/**
 * flashing() - lock/unlock.
 *
 * @cmd_parameter: Pointer to partition name
 * @response: Pointer to fastboot response buffer
 *
 * Writes the previously downloaded image to the partition indicated by
 * cmd_parameter. Writes to response.
 */
static void flashing(char *cmd_parameter, char *response)
{
	char *cmd;
	char* lock_s;
	LockData_t* info;
	char lock_d[LOCK_DATA_SIZE];

	lock_s = env_get("lock");
	if (!lock_s) {
		printf("lock state is NULL \n");
		strncpy(lock_d, "10101000", 8);
		lock_s = "10101000";
		env_set("lock", "10101000");
		run_command("defenv_reserv; saveenv;", 0);
	} else {
		printf("lock state: %s\n", lock_s);
		if (strlen(lock_s) > 15)
			strncpy(lock_d, lock_s, 15);
		else
			strncpy(lock_d, lock_s, strlen(lock_s));
	}

	info = malloc(sizeof(struct LockData));
	if (!info) {
		printf("malloc error\n");
		fastboot_fail("malloc error", response);
		return;
	}
	memset(info,0,LOCK_DATA_SIZE);
	info->version_major = (int)(lock_d[0] - '0');
	info->version_minor = (int)(lock_d[1] - '0');
	info->unlock_ability = (int)(lock_d[2] - '0');
	info->lock_state = (int)(lock_d[4] - '0');
	info->lock_critical_state = (int)(lock_d[5] - '0');
	info->lock_bootloader = (int)(lock_d[6] - '0');

	printf("cb_flashing cmd_parameter: %s\n", cmd_parameter);
	cmd = cmd_parameter;
	strsep(&cmd, " ");
	printf("cb_flashing: %s\n", cmd);
	if (!cmd) {
		printf("missing variable\n");
		fastboot_fail("missing var", response);
		free(info);
		return;
	}

	if (!strcmp_l1("unlock_critical", cmd)) {
		info->lock_critical_state = 0;
		fastboot_okay(NULL, response);
	} else if (!strcmp_l1("lock_critical", cmd)) {
		info->lock_critical_state = 1;
		fastboot_okay(NULL, response);
	} else if (!strcmp_l1("get_unlock_ability", cmd)) {
		char str[32];
		static bool is_unlock_ability_sent = false;
		if (is_unlock_ability_sent) {
			is_unlock_ability_sent = false;
			fastboot_okay(NULL, response);
			busy_flag = 0;
		} else {
			sprintf(str, "get_unlock_ability: %d",
				info->unlock_ability);
			fastboot_response("INFO", response, "%s", str);
			is_unlock_ability_sent = true;
			busy_flag = 1;
		}
		free(info);
		return;
	} else if (!strcmp_l1("get_unlock_bootloader_nonce", cmd)) {
		char str_num[8];
		sprintf(str_num, "%d", info->lock_critical_state);
		fastboot_response("OKAY", response, "%s", str_num);
	} else if (!strcmp_l1("lock_bootloader", cmd)) {
		info->lock_bootloader = 1;
	} else if (!strcmp_l1("unlock", cmd)) {
		if (info->unlock_ability == 1 ) {
			if (info->lock_state == 1 ) {
				char *avb_s;
				avb_s = env_get("avb2");
				if (avb_s == NULL) {
					run_command("get_avb_mode;", 0);
					avb_s = env_get("avb2");
				}
				printf("avb2: %s\n", avb_s);
				if (strcmp(avb_s, "1") == 0) {
#ifdef CONFIG_AML_ANTIROLLBACK
					if (avb_unlock()) {
						printf("unlocking device.  Erasing userdata partition!\n");
						run_command("store erase data 0 0", 0);
					} else {
						printf("unlock failed!\n");
					}
#else
					printf("unlocking device.  Erasing userdata partition!\n");
					run_command("store erase data 0 0", 0);
#endif
				}
			}
			info->lock_state = 0;
			info->lock_critical_state = 0;
			env_set("lock_state", "green");
			fastboot_okay(NULL, response);
		} else {
			printf("unlock_ability is 0, can not unlock, please set it in android setting\n");
			fastboot_response("FAIL", response, "%s", "unlock_ability is 0, can not unlock");
		}
	} else if (!strcmp_l1("lock", cmd)) {
		if (info->lock_state == 0 ) {
			char *avb_s;
			avb_s = env_get("avb2");
			if (avb_s == NULL) {
				run_command("get_avb_mode;", 0);
				avb_s = env_get("avb2");
			}
			printf("avb2: %s\n", avb_s);
			if (strcmp(avb_s, "1") == 0) {
#ifdef CONFIG_AML_ANTIROLLBACK
				if (avb_lock()) {
					printf("lock failed!\n");
				} else {
					printf("locking device.  Erasing userdata partition!\n");
					run_command("store erase data 0 0", 0);
				}
#else
				printf("locking device.  Erasing userdata partition!\n");
				run_command("store erase data 0 0", 0);
#endif
			}
		}
		info->lock_state = 1;
		env_set("lock_state", "orange");
		fastboot_okay(NULL, response);
	} else {
		printf("unknown variable: %s\n", cmd);
		fastboot_response("FAIL", response, "%s", "Variable not implemented");
	}

	sprintf(lock_d, "%d%d%d0%d%d%d0", info->version_major, info->version_minor, info->unlock_ability, info->lock_state, info->lock_critical_state, info->lock_bootloader);
	printf("lock_d state: %s\n", lock_d);
	env_set("lock", lock_d);
	run_command("defenv_reserv; saveenv;", 0);
	free(info);
	return;
}

/**
 * reboot_bootloader() - Sets reboot bootloader flag.
 *
 * @cmd_parameter: Pointer to command parameter
 * @response: Pointer to fastboot response buffer
 */
static void reboot_bootloader(char *cmd_parameter, char *response)
{
	fastboot_okay(NULL, response);
}

/**
 * reboot_fastboot() - Sets reboot fastboot flag.
 *
 * @cmd_parameter: Pointer to command parameter
 * @response: Pointer to fastboot response buffer
 */
static void reboot_fastboot(char *cmd_parameter, char *response)
{
	fastboot_okay(NULL, response);
}

#if CONFIG_IS_ENABLED(FASTBOOT_CMD_OEM_FORMAT)
/**
 * oem_format() - Execute the OEM format command
 *
 * @cmd_parameter: Pointer to command parameter
 * @response: Pointer to fastboot response buffer
 */
static void oem_format(char *cmd_parameter, char *response)
{
	char cmdbuf[32];

	if (!env_get("partitions")) {
		fastboot_fail("partitions not set", response);
	} else {
		sprintf(cmdbuf, "gpt write mmc %x $partitions",
			CONFIG_FASTBOOT_FLASH_MMC_DEV);
		if (run_command(cmdbuf, 0))
			fastboot_fail("", response);
		else
			fastboot_okay(NULL, response);
	}
}
#endif
