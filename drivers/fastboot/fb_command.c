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
#include <amlogic/storage.h>
#include <amlogic/aml_efuse.h>
#include <amlogic/aml_mmc.h>

#if defined(CONFIG_AML_ANTIROLLBACK) || defined(CONFIG_AML_AVB2_ANTIROLLBACK)
#include <amlogic/anti-rollback.h>
#endif

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
static void reboot_bootloader(char *, char *);
static void reboot_fastboot(char *, char *);

#ifdef CONFIG_FASTBOOT_WRITING_CMD
static void download(char *, char *);
#if CONFIG_IS_ENABLED(FASTBOOT_FLASH)
static void flash(char *, char *);
static void erase(char *, char *);
#endif
static void fetch(char *, char *);
#if !CONFIG_IS_ENABLED(NO_FASTBOOT_FLASHING)
static void flashing(char *, char *);
#endif//#if !CONFIG_IS_ENABLED(NO_FASTBOOT_FLASHING)
#if CONFIG_IS_ENABLED(FASTBOOT_CMD_OEM_FORMAT)
static void oem_format(char *, char *);
#endif
static void oem_cmd(char *, char *);
static void set_active_cmd(char *, char *);
static void snapshot_update_cmd(char *, char *);
#endif

#ifdef CONFIG_BOOTLOADER_CONTROL_BLOCK
extern int is_partition_logical(char* parition_name);
#endif

static const struct {
	const char *command;
	void (*dispatch)(char *cmd_parameter, char *response);
} commands[FASTBOOT_COMMAND_COUNT] = {
	[FASTBOOT_COMMAND_GETVAR] = {
		.command = "getvar",
		.dispatch = getvar
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
	[FASTBOOT_COMMAND_BOOT] =  {
		.command = "boot",
		.dispatch = okay
	},
#ifdef CONFIG_FASTBOOT_WRITING_CMD
	[FASTBOOT_COMMAND_DOWNLOAD] = {
		.command = "download",
		.dispatch = download
	},
#if !CONFIG_IS_ENABLED(NO_FASTBOOT_FLASHING)
	[FASTBOOT_COMMAND_FLASHING] =  {
		.command = "flashing",
		.dispatch = flashing
	},
#endif// #if !CONFIG_IS_ENABLED(NO_FASTBOOT_FLASHING)
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
	[FASTBOOT_COMMAND_FETCH] =  {
		.command = "fetch",
		.dispatch = fetch
	},
	[FASTBOOT_COMMAND_SET_ACTIVE] =  {
		.command = "set_active",
		.dispatch = set_active_cmd
	},
	[FASTBOOT_COMMAND_SNAOSHOT_UPDATE] =  {
		.command = "snapshot-update",
		.dispatch = snapshot_update_cmd
	},
#if CONFIG_IS_ENABLED(FASTBOOT_CMD_OEM_FORMAT)
	[FASTBOOT_COMMAND_OEM_FORMAT] = {
		.command = "oem format",
		.dispatch = oem_format,
	},
#endif
	[FASTBOOT_COMMAND_OEM] = {
		.command = "oem",
		.dispatch = oem_cmd,
	},
#endif
};

struct fastboot_read fastboot_readInfo;

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

void dump_lock_info(LockData_t info)
{
#if 0
	printf("info.version_major = %d\n", info.version_major);
	printf("info.version_minor = %d\n", info.version_minor);
	printf("info.unlock_ability = %d\n", info.unlock_ability);
	printf("info.lock_state = %d\n", info.lock_state);
	printf("info.lock_critical_state = %d\n", info.lock_critical_state);
	printf("info.lock_bootloader = %d\n", info.lock_bootloader);
#endif
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
#ifdef CONFIG_FASTBOOT_WRITING_CMD
static void download(char *cmd_parameter, char *response)
{
	char *tmp;

	if (check_lock() == 1) {
		printf("device is locked, can not run this cmd.Please flashing unlock & flashing unlock_critical\n");
		fastboot_fail("locked device", response);
		return;
	}

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
		char str[128] = {0};

		printf("fastboot_bytes_expected %d > fastboot_buf_size %d\n",
		       fastboot_bytes_expected, fastboot_buf_size);
		sprintf(str, "data too large, please add -S %dM",
			(fastboot_buf_size / 1024 / 1024));
		fastboot_fail(str, response);
	} else {
		printf("Starting download of %d bytes\n",
		       fastboot_bytes_expected);
		fastboot_response("DATA", response, "%s", cmd_parameter);
	}
}
#endif
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

#ifdef CONFIG_FASTBOOT_WRITING_CMD
#if CONFIG_IS_ENABLED(FASTBOOT_FLASH)
static void write_dts_reserve(void)
{
	int ret;
	void *addr = NULL;
	char *mem_addr;

	if (run_command("imgread dtb ${boot_part} ${dtb_mem_addr}", 0)) {
		printf("Fail in load dtb\n");
	} else {
		mem_addr = env_get("dtb_mem_addr");

		if (mem_addr) {
			addr = (void *)simple_strtoul(mem_addr, NULL, 16);
			ret = dtb_write(addr);
			if (ret)
				printf("write dtb error\n");
			else
				printf("write dtb ok\n");
		}
	}
}

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
	char name[32] = {0};
	u64 rc = 0;

	if (check_lock() == 1) {
		printf("device is locked, can not run this cmd.Please flashing unlock & flashing unlock_critical\n");
		fastboot_fail("locked device", response);
		return;
	}

	printf("cmd_parameter: %s\n", cmd_parameter);

#ifdef CONFIG_FASTBOOT_FLASH_MMC_DEV
	struct mmc *mmc = find_mmc_device(CONFIG_FASTBOOT_FLASH_MMC_DEV);
	if (mmc && strcmp(cmd_parameter, "bootloader") == 0) {
		printf("try to read gpt data from bootloader.img\n");
		struct blk_desc *dev_desc;
		int erase_flag = 0;

		/* the max size of bootloader.img is 4M, we reserve 128k for gpt.bin
		 * so we put gpt.bin at offset 0x3DFE00
		 * 0 ~ 512 bootloader secure boot, we don't care it here.
		 * 512 ~ 0x3DFDFF  original bootloader.img and 0
		 * 0x3DFE00 ~ end  gpt.bin
		 */

		dev_desc = blk_get_dev("mmc", CONFIG_FASTBOOT_FLASH_MMC_DEV);
		if (!dev_desc || dev_desc->type == DEV_TYPE_UNKNOWN) {
			printf("invalid mmc device\n");
			fastboot_fail("invalid mmc device", response);
			return;
		}

		if (is_valid_gpt_buf(dev_desc, fastboot_buf_addr + 0x3DFE00)) {
			printf("printf normal bootloader.img, no gpt partition table\n");
		} else {
			printf("find gpt parition table, update it\n"
				"and write bootloader to boot0/boot1\n");

			erase_flag = check_gpt_part(dev_desc, fastboot_buf_addr + 0x3DFE00);

			if (erase_flag == 1) {
				printf("partition changes, erase emmc\n");
				run_command("store erase.chip 0;", 0);
			}

			if (write_mbr_and_gpt_partitions(dev_desc, fastboot_buf_addr + 0x3DFE00)) {
				printf("%s: writing GPT partitions failed\n", __func__);
				fastboot_fail("writing GPT partitions failed", response);
				return;
			}

			if (mmc_device_init(mmc) != 0) {
				printf(" update gpt partition table fail\n");
				fastboot_fail("fastboot update gpt partition fail", response);
				return;
			}
			printf("%s: writing GPT partitions ok\n", __func__);

			char *mem_addr;
			void *addr = NULL;
			int ret;

			mem_addr = env_get("dtb_mem_addr");

			if (mem_addr && erase_flag == 1) {
				printf("partition changes, erase emmc\n");
				run_command("store erase.chip 0;", 0);
				printf("write _aml_dtb\n");
				addr = (void *)simple_strtoul(mem_addr, NULL, 16);
				ret = dtb_write(addr);
				if (ret)
					printf("write _aml_dtb error\n");
				else
					printf("write _aml_dtb ok\n");
			}
		}

		if (aml_gpt_valid(mmc) == 0) {
			printf("gpt mode\n");
#if CONFIG_IS_ENABLED(FASTBOOT_FLASH_MMC)
			fastboot_mmc_flash_write("bootloader-boot0", fastboot_buf_addr, image_size,
				 response);
			fastboot_mmc_flash_write("bootloader-boot1", fastboot_buf_addr, image_size,
				 response);
			run_command("mmc dev 1 0;", 0);
#endif
			env_set("default_env", "1");
#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
			run_command("update_env_part -p default_env;", 0);
#else
			run_command("defenv_reserve;saveenv;", 0);
#endif//#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
			return;
		}
	}
#endif

	if (strcmp(cmd_parameter, "avb_custom_key") == 0) {
		char* buffer = NULL;
		char *partition = "misc";
		AvbKey_t key;
		printf("avb_custom_key image_size: %d\n", image_size);

		if (image_size > AVB_CUSTOM_KEY_LEN_MAX - sizeof(AvbKey_t)) {
			printf("key size is too large\n");
			fastboot_fail("size error", response);
			return;
		}

		memcpy(key.magic_name, "AVBK", 4);
		key.size = image_size;
		rc = store_part_size(partition);

		buffer = (char *)malloc(AVB_CUSTOM_KEY_LEN_MAX);
		if (!buffer) {
			printf("malloc error\n");
			fastboot_fail("malloc error", response);
			return;
		}
		memset(buffer, 0, AVB_CUSTOM_KEY_LEN_MAX);
		memcpy(buffer, &key, sizeof(AvbKey_t));
		memcpy(buffer + sizeof(AvbKey_t), fastboot_buf_addr, image_size);

		store_write((const char *)partition, rc - AVB_CUSTOM_KEY_LEN_MAX, AVB_CUSTOM_KEY_LEN_MAX, (unsigned char *)buffer);

		fastboot_okay(NULL, response);
		free(buffer);
		return;
	}

	if (strcmp(cmd_parameter, "userdata") == 0 || strcmp(cmd_parameter, "data") == 0) {
		rc = store_part_size("userdata");
		if (-1 == rc)
			strcpy(name, "data");
		else
			strcpy(name, "userdata");
	} else if (strcmp(cmd_parameter, "dts") == 0) {
		strcpy(name, "dtb");
	} else {
		strncpy(name, cmd_parameter, 31);
	}
	strcat(name, "\0");

#ifdef CONFIG_BOOTLOADER_CONTROL_BLOCK
	if (dynamic_partition) {
		if (is_partition_logical(name) == 0) {
			printf("logic partition, can not write here.......\n");
			fastboot_fail("logic partition", response);
			return;
		}
	}
#endif

#if CONFIG_IS_ENABLED(FASTBOOT_FLASH_MMC)
	fastboot_mmc_flash_write(name, fastboot_buf_addr, image_size,
				 response);
#endif
#if CONFIG_IS_ENABLED(FASTBOOT_FLASH_NAND)
	fastboot_nand_flash_write(name, fastboot_buf_addr, image_size,
				  response);
#endif

	if (strcmp(name, "bootloader") == 0) {
		env_set("default_env", "1");
#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
		run_command("update_env_part -p default_env;", 0);
#else
		run_command("defenv_reserve;saveenv;", 0);
#endif// #if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
	}

	if (aml_gpt_valid(mmc) == 0) {
		if (vendor_boot_partition) {
			if (strcmp_l1("vendor_boot", name) == 0) {
				printf("gpt mode, write dts to reserve\n");
				write_dts_reserve();
			}
		} else {
			if (strcmp_l1("boot", name) == 0) {
				printf("gpt mode, write dts to reserve\n");
				write_dts_reserve();
			}
		}
	}
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
	char name[32] = {0};
	u64 rc = 0;

	if (check_lock() == 1) {
		printf("device is locked, can not run this cmd.Please flashing unlock & flashing unlock_critical\n");
		fastboot_fail("locked device", response);
		return;
	}

	printf("cmd_parameter: %s\n", cmd_parameter);

#ifdef CONFIG_FASTBOOT_FLASH_MMC_DEV
	struct mmc *mmc = find_mmc_device(CONFIG_FASTBOOT_FLASH_MMC_DEV);
	if ((mmc != NULL) && strcmp(cmd_parameter, "bootloader") == 0 && (aml_gpt_valid(mmc) == 0)) {
		printf("we write gpt partition table to bootloader now\n");
		printf("plese write bootloader to bootloader-boot0/bootloader-boot1\n");
		fastboot_okay("gpt mode, skip", response);
		return;
	}
#endif

	if (strcmp(cmd_parameter, "avb_custom_key") == 0) {
		char* buffer = NULL;
		char *partition = "misc";

		rc = store_part_size(partition);
		buffer = (char *)malloc(AVB_CUSTOM_KEY_LEN_MAX);
		if (!buffer) {
			printf("malloc error\n");
			fastboot_fail("malloc error", response);
			return;
		}
		memset(buffer, 0, AVB_CUSTOM_KEY_LEN_MAX);

		store_write((const char *)partition, rc - AVB_CUSTOM_KEY_LEN_MAX, AVB_CUSTOM_KEY_LEN_MAX, (unsigned char *)buffer);

		fastboot_okay(NULL, response);
		free(buffer);
		return;
	}

	if (strcmp(cmd_parameter, "misc") == 0) {
		char* buffer = NULL;
		char *partition = "misc";

		rc = store_part_size(partition);
		buffer = (char *)malloc(rc - AVB_CUSTOM_KEY_LEN_MAX);
		if (!buffer) {
			printf("malloc error\n");
			fastboot_fail("malloc error", response);
			return;
		}
		memset(buffer, 0, rc - AVB_CUSTOM_KEY_LEN_MAX);

		store_write((const char *)partition, 0, rc - AVB_CUSTOM_KEY_LEN_MAX, (unsigned char *)buffer);

		fastboot_okay(NULL, response);
		free(buffer);
		return;
	}

	if (strcmp(cmd_parameter, "userdata") == 0 || strcmp(cmd_parameter, "data") == 0) {
		rc = store_part_size("userdata");
		if (-1 == rc)
			strcpy(name, "data");
		else
			strcpy(name, "userdata");
	} else if (strcmp(cmd_parameter, "dts") == 0) {
		strcpy(name, "dtb");
	} else {
		strncpy(name, cmd_parameter, 31);
	}
	strcat(name, "\0");

#ifdef CONFIG_BOOTLOADER_CONTROL_BLOCK
		if (dynamic_partition) {
			if (is_partition_logical(name) == 0) {
				printf("logic partition, can not erase here.......\n");
				fastboot_fail("logic partition", response);
				return;
			}
		}
#endif

#if CONFIG_IS_ENABLED(FASTBOOT_FLASH_MMC)
	fastboot_mmc_erase(name, response);
#endif
#if CONFIG_IS_ENABLED(FASTBOOT_FLASH_NAND)
	fastboot_nand_erase(name, response);
#endif
}
#endif
#endif

#ifdef CONFIG_FASTBOOT_WRITING_CMD
static u64 my_ato11(const char *str)
{
	u64 result = 0;
	int len, i;

	len = strlen(str);
	for (i = 0; i < len; i++) {
		if (*str >= '0' && *str <= '9')
			result = result * 16 + (*str - '0');
		else if (*str >= 'A' && *str <= 'F')
			result = result * 16 + (*str - 'A') + 10;
		else if (*str >= 'a' && *str <= 'f')
			result = result * 16 + (*str - 'a') + 10;
		str++;
	}

	return result;
}

static void fetch(char *cmd_parameter, char *response)
{
	int len;
	int i = 0;
	char *cmd;
	char name[32] = {0};
	u64 offset = 0;
	u64 read_size = 0;
	size_t size = 0;

	if (check_lock() == 1) {
		printf("device is locked, can not run this cmd\n");
		fastboot_fail("locked device", response);
		return;
	}

	cmd = cmd_parameter;
	len = strlen(cmd_parameter);
	while (strsep(&cmd, ":"))
		i++;

	for (cmd = cmd_parameter, i = 0; cmd < (cmd_parameter + len); i++) {
		/* Skip to next assignment */
		if (i == 0) {
			strncpy(name, cmd, 31);
			strcat(name, "\0");
		} else if (i == 1) {
			offset = my_ato11(cmd);
		} else if (i == 2) {
			read_size = my_ato11(cmd);
		}

		for (cmd += strlen(cmd); cmd < (cmd_parameter + len) && !*cmd;)
			cmd++;
	}

	printf("name: %s\n", name);
	if (strncmp("vendor_boot", name, strlen("vendor_boot")) != 0) {
		printf("We can only %s vendor_boot\n", __func__);
		fastboot_fail("Fetch is only allowed on vendor_boot", response);
		return;
	}

#if CONFIG_IS_ENABLED(FASTBOOT_FLASH_MMC)
	struct blk_desc *dev_desc;
	disk_partition_t part_info;
	int r;

	r = fastboot_mmc_get_part_info(name, &dev_desc, &part_info,
				       response);
	if (r >= 0)
		size = part_info.size * 512;
#endif
#if CONFIG_IS_ENABLED(FASTBOOT_FLASH_NAND)
	struct part_info *part_info;
	int r;

	r = fastboot_nand_get_part_info(name, &part_info, response);
	if (r >= 0)
		size = part_info->size * 512;
#endif

	if (offset > size) {
		printf("Invalid offset: 0x%llx, partition size is 0x%lx\n",
			offset, size);
		fastboot_fail("Invalid offset", response);
		return;
	}

	if (read_size == 0 || read_size > size - offset ||
			read_size > kMaxFetchSizeDefault) {
		printf("Invalid read size: 0x%llx, partition size is 0x%lx\n",
			offset, size);
		fastboot_fail("Invalid read size", response);
		return;
	}

	printf("Start read %s partition datas!\n", name);
	void *buffer;
	char str[128] = {0};

	buffer = (void *)CONFIG_FASTBOOT_BUF_ADDR;
	sprintf(str, "DATA%12llx", read_size);
	printf("str: %s, len: %ld\n", str, strlen(str));
	memcpy(buffer, str, strlen(str));

	if (store_read((const char *)name, offset, read_size,
		(unsigned char *)buffer + strlen(str)) < 0) {
		printf("failed to store read %s.\n", name);
		fastboot_fail("read partition data error", response);
		return;
	}

	fastboot_readInfo.transferredBytes = 0;
	fastboot_readInfo.totalBytes = read_size + strlen(str);
	fastboot_response("DATA", response, "%12llx", read_size);
}

static void set_active_cmd(char *cmd_parameter, char *response)
{
	char *cmd;
	int ret = 0;
	char str[128];

	printf("cmd cb_set_active is %s\n", cmd_parameter);
	cmd = cmd_parameter;

	if (check_lock() == 1) {
		printf("device is locked, can not run this cmd.Please flashing unlock & flashing unlock_critical\n");
		fastboot_fail("locked device", response);
		return;
	}

	sprintf(str, "set_active_slot %s", cmd);
	printf("command:    %s\n", str);
	ret = run_command(str, 0);
	printf("ret = %d\n", ret);
	if (ret == 0)
		fastboot_okay(NULL, response);
	else
		fastboot_fail("set slot error", response);
}

static void snapshot_update_cmd(char *cmd_parameter, char *response)
{
	char *cmd;

	printf("cmd snapshot_update_cmd is %s\n", cmd_parameter);
	cmd = cmd_parameter;

	if (check_lock() == 1) {
		printf("device is locked, can not run this cmd.Please flashing unlock & flashing unlock_critical\n");
		fastboot_fail("locked device", response);
		return;
	}

	if (!strcmp_l1("cancel", cmd)) {
		struct misc_virtual_ab_message message;
		set_mergestatus_cancel(&message);
	}
	fastboot_okay(NULL, response);
}

#if !CONFIG_IS_ENABLED(NO_FASTBOOT_FLASHING)
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
	LockData_t info = {0};
	char lock_d[LOCK_DATA_SIZE];
	u64 rc;

	if (IS_FEAT_BOOT_VERIFY()) {
		printf("device is secure mode, can not run this cmd.\n");
		fastboot_fail("secure boot device", response);
		return;
	}

	lock_s = env_get("lock");
	if (!lock_s) {
		printf("lock state is NULL \n");
		memcpy(lock_d, "10101000", 8);
		lock_s = "10101000";
		env_set("lock", "10101000");
#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
		run_command("update_env_part -p lock;", 0);
#else
		run_command("defenv_reserv; saveenv;", 0);
#endif//#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
	} else {
		printf("lock state: %s\n", lock_s);
		if (strlen(lock_s) > 15)
			strncpy(lock_d, lock_s, 15);
		else
			strncpy(lock_d, lock_s, strlen(lock_s));
	}

	info.version_major = (int)(lock_d[0] - '0');
	info.version_minor = (int)(lock_d[1] - '0');
	info.unlock_ability = (int)(lock_d[2] - '0');
	info.lock_state = (int)(lock_d[4] - '0');
	info.lock_critical_state = (int)(lock_d[5] - '0');
	info.lock_bootloader = (int)(lock_d[6] - '0');
	dump_lock_info(info);

	printf("cb_flashing cmd_parameter: %s\n", cmd_parameter);
	cmd = cmd_parameter;
	strsep(&cmd, " ");
	printf("cb_flashing: %s\n", cmd);
	if (!cmd) {
		printf("missing variable\n");
		fastboot_fail("missing var", response);
		return;
	}

	rc = store_part_size("userdata");

	if (!strcmp_l1("unlock_critical", cmd)) {
		info.lock_critical_state = 0;
		fastboot_okay(NULL, response);
	} else if (!strcmp_l1("lock_critical", cmd)) {
		info.lock_critical_state = 1;
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
				info.unlock_ability);
			fastboot_response("INFO", response, "%s", str);
			is_unlock_ability_sent = true;
			busy_flag = 1;
		}
		return;
	} else if (!strcmp_l1("get_unlock_bootloader_nonce", cmd)) {
		char str_num[8];
		sprintf(str_num, "%d", info.lock_critical_state);
		fastboot_response("OKAY", response, "%s", str_num);
	} else if (!strcmp_l1("lock_bootloader", cmd)) {
		info.lock_bootloader = 1;
	} else if (!strcmp_l1("unlock", cmd)) {
		if (info.unlock_ability == 1) {
			if (info.lock_state == 1) {
				char *avb_s;
				avb_s = env_get("avb2");
				if (avb_s == NULL) {
					run_command("get_avb_mode;", 0);
					avb_s = env_get("avb2");
				}
				printf("avb2: %s\n", avb_s);
				if (strcmp(avb_s, "1") == 0) {
#if defined(CONFIG_AML_ANTIROLLBACK) || defined(CONFIG_AML_AVB2_ANTIROLLBACK)
					if (avb_unlock()) {
						if (-1 == rc) {
							printf("unlocking device.  Erasing data partition!\n");
							run_command("store erase data 0 0", 0);
						} else {
							printf("unlocking device.  Erasing userdata partition!\n");
							run_command("store erase userdata 0 0", 0);
						}
						printf("unlocking device.  Erasing metadata partition!\n");
						run_command("store erase metadata 0 0", 0);
					} else {
						printf("unlock failed!\n");
					}
#else
					if (-1 == rc) {
						printf("unlocking device.  Erasing data partition!\n");
						run_command("store erase data 0 0", 0);
					} else {
						printf("unlocking device.  Erasing userdata partition!\n");
						run_command("store erase userdata 0 0", 0);
					}
					printf("unlocking device.  Erasing metadata partition!\n");
					run_command("store erase metadata 0 0", 0);
#endif
				}
			}
			info.lock_state = 0;
			info.lock_critical_state = 0;
			env_set("lock_state", "green");
			fastboot_okay(NULL, response);
		} else {
			printf("unlock_ability is 0, can not unlock, please set it in android setting\n");
			fastboot_response("FAIL", response, "%s", "unlock_ability is 0, can not unlock");
		}
	} else if (!strcmp_l1("lock", cmd)) {
		if (info.lock_state == 0) {
			char *avb_s;
			avb_s = env_get("avb2");
			if (avb_s == NULL) {
				run_command("get_avb_mode;", 0);
				avb_s = env_get("avb2");
			}
			printf("avb2: %s\n", avb_s);
			if (strcmp(avb_s, "1") == 0) {
#if defined(CONFIG_AML_ANTIROLLBACK) || defined(CONFIG_AML_AVB2_ANTIROLLBACK)
				if (avb_lock()) {
					if (-1 == rc) {
						printf("locking device.  Erasing data partition!\n");
						run_command("store erase data 0 0", 0);
					} else {
						printf("locking device.  Erasing userdata partition!\n");
						run_command("store erase userdata 0 0", 0);
					}
					printf("locking device.  Erasing metadata partition!\n");
					run_command("store erase metadata 0 0", 0);
				} else {
					printf("lock failed!\n");
				}
#else
				if (-1 == rc) {
					printf("locking device.  Erasing data partition!\n");
					run_command("store erase data 0 0", 0);
				} else {
					printf("locking device.  Erasing userdata partition!\n");
					run_command("store erase userdata 0 0", 0);
				}
				printf("locking device.  Erasing metadata partition!\n");
				run_command("store erase metadata 0 0", 0);

#endif
			}
		}
		info.lock_state = 1;
		env_set("lock_state", "orange");
		fastboot_okay(NULL, response);
	} else {
		printf("unknown variable: %s\n", cmd);
		fastboot_response("FAIL", response, "%s", "Variable not implemented");
	}

	sprintf(lock_d, "%d%d%d0%d%d%d0", info.version_major, info.version_minor,
		info.unlock_ability, info.lock_state, info.lock_critical_state,
		info.lock_bootloader);
	printf("lock_d state: %s\n", lock_d);
	env_set("lock", lock_d);
#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
	run_command("update_env_part -p lock;", 0);
#else
	run_command("defenv_reserv; saveenv;", 0);
#endif//#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
	return;
}
#endif// #if !CONFIG_IS_ENABLED(NO_FASTBOOT_FLASHING)
#endif

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

#ifdef CONFIG_FASTBOOT_WRITING_CMD
static void oem_cmd(char *cmd_parameter, char *response)
{
	char *cmd;
	int i = 0, len = 0, j = 0;
	char cmd_str[FASTBOOT_RESPONSE_LEN];
	printf("oem cmd_parameter: %s\n", cmd_parameter);

	if (IS_FEAT_BOOT_VERIFY()) {
		printf("device is secure mode, can not run this cmd.\n");
		fastboot_fail("secure boot device", response);
		return;
	}

	if (check_lock() == 1) {
		printf("device is locked, can not run this cmd.Please flashing unlock & flashing unlock_critical\n");
		fastboot_fail("locked device", response);
		return;
	}

	cmd = cmd_parameter;
	strsep(&cmd, " ");
	printf("To run cmd[%s]\n", cmd);

	len = strlen(cmd);
	for (i = 0; i < len; i++) {
		if (cmd[i] != '\'') {
			cmd_str[j++] = cmd[i];
		}
	}
	cmd_str[j] = '\0';
	printf("cmd_str2: %s\n", cmd_str);

	run_command(cmd_str, 0);

	fastboot_okay(NULL, response);
	return;
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

	if (IS_FEAT_BOOT_VERIFY()) {
		printf("device is secure mode, can not run this cmd.\n");
		fastboot_fail("secure boot device", response);
		return;
	}

	if (check_lock() == 1) {
		printf("device is locked, can not run this cmd.Please flashing unlock & flashing unlock_critical\n");
		fastboot_fail("locked device", response);
		return;
	}

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
#endif

