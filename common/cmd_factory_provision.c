/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_factory_provision.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <fs.h>
#include <fat.h>
#include <u-boot/sha256.h>
#include <emmc_partitions.h>

#define CMD_DEBUG         (0)
#define CMD_LOG_TAG       "[FACTORY-PROVISION] "

#if CMD_DEBUG
#define LOGD(fmt, ...)    printf("%s"fmt, CMD_LOG_TAG, ##__VA_ARGS__)
#else
#define LOGD(fmt, ...)
#endif

#define LOGE(fmt, ...)    printf("%sERROR: "fmt, CMD_LOG_TAG, ##__VA_ARGS__)
#define LOGI(fmt, ...)    printf("%s"fmt, CMD_LOG_TAG, ##__VA_ARGS__)

#define __asmeq(x, y) ".ifnc " x "," y " ; .err ; .endif\n\t"

#define FUNCID_PROVISION_SET_IV                0xB200E030
#define FUNCID_PROVISION_ENCRYPT               0xB200E031
#define FUNCID_PROVISION_GET_TRANSFER_ADDR     0xB2000007

#define SIZE_1K                 (1024)
#define SIZE_1M                 (SIZE_1K * SIZE_1K)
#define SIZE_TAG                (16)
#define SIZE_IV                 (16)
#define SIZE_AES_KEY            (16)
#define SIZE_HMAC_KEY           (32)
#define SIZE_HMAC_DIGEST        (32)
#define SIZE_BLOCK              (512)

#define MAX_SIZE_CMD            (256)
#define MAX_SIZE_KEYBOX_NAME    (256)
#define MAX_SIZE_PART_NAME      (32)
#define MAX_SIZE_KEYBOX         (SIZE_1K * 16)

#define MAX_CNT_FS_VALUE        (256)

#define KEYBOX_HDR_MAGIC        (0x6B626F78) //"kbox"
#define KEYBOX_HDR_MIN_VERSION  (5)

#define PROVISION_TYPE_FACTORY  (0)

#define ACTION_UNKNOWN          (0x00)
#define ACTION_INIT             (0x01)
#define ACTION_WRITE            (0x02)
#define ACTION_QUERY            (0x03)
#define ACTION_REMOVE           (0x04)
#define ACTION_CLEAR            (0x05)
#define ACTION_LIST             (0x06)

#define DEV_NAME                "mmc"
#define DEV_NO                  (1)
#define PART_TYPE               "user"
#define PART_NAME_RSV           "rsv"
#define PART_NAME_FTY           "factory"

#define CMD_RET_SUCCESS                                0x00000000
#define CMD_RET_KEYBOX_NOT_EXIST                       0x00000001
#define CMD_RET_KEYBOX_TOO_LARGE                       0x00000002
#define CMD_RET_KEYBOX_NAME_TOO_LONG                   0x00000003
#define CMD_RET_KEYBOX_BAD_FORMAT                      0x00000004
#define CMD_RET_DEVICE_NO_SPACE                        0x00000011
#define CMD_RET_DEVICE_NOT_AVAILABLE                   0x00000012
#define CMD_RET_BAD_PARAMETER                          0x00000021
#define CMD_RET_SMC_CALL_FAILED                        0x00000031
#define CMD_RET_UNKNOWN_ERROR                          0x0000FFFF

struct keybox_header {
	uint32_t magic;
	uint32_t version;
	uint32_t key_type;
	uint32_t key_size;
	uint32_t provision_type;
	uint32_t ta_uuid[4];
	uint32_t reserved[4];
};

struct encryption_context {
	char iv[SIZE_IV];
	char tag[SIZE_TAG];
	char epek[SIZE_AES_KEY];
	char rsv[64];
};

struct input_param {
	uint32_t action;
	const char *keybox_name;
	uint32_t keybox_phy_addr;
	uint32_t keybox_size;
	uint32_t ret_data_addr;
};

struct fs_value {
	int idx;
	uint32_t value;
};

static char g_keybox[MAX_SIZE_KEYBOX] = { 0 };
static char g_fs_data[SIZE_1K] = { 0 };
static char g_part_name[MAX_SIZE_PART_NAME] = { 0 };

static struct fs_value g_fs_vals_rsv[MAX_CNT_FS_VALUE] = {
	{ 0, 0xEB }, { 1, 0x3C }, { 2, 0x90 }, { 3, 0x6D },
	{ 4, 0x6B }, { 5, 0x66 }, { 6, 0x73 }, { 7, 0x2E },
	{ 8, 0x66 }, { 9, 0x61 }, { 10, 0x74 }, { 12, 0x02 },
	{ 13, 0x10 }, { 14, 0x01 }, { 16, 0x02 }, { 18, 0x02 },
	{ 20, 0x80 }, { 21, 0xF8 }, { 22, 0x10 }, { 24, 0x20 },
	{ 26, 0x40 }, { 36, 0x80 }, { 38, 0x29 }, { 39, 0x13 },
	{ 40, 0x4C }, { 41, 0x98 }, { 42, 0x6A }, { 43, 0x4B },
	{ 44, 0x45 }, { 45, 0x59 }, { 46, 0x42 }, { 47, 0x4F },
	{ 48, 0x58 }, { 49, 0x20 }, { 50, 0x50 }, { 51, 0x41 },
	{ 52, 0x52 }, { 53, 0x54 }, { 54, 0x46 }, { 55, 0x41 },
	{ 56, 0x54 }, { 57, 0x31 }, { 58, 0x32 }, { 59, 0x20 },
	{ 60, 0x20 }, { 61, 0x20 }, { 62, 0x0E }, { 63, 0x1F },
	{ 64, 0xBE }, { 65, 0x5B }, { 66, 0x7C }, { 67, 0xAC },
	{ 68, 0x22 }, { 69, 0xC0 }, { 70, 0x74 }, { 71, 0x0B },
	{ 72, 0x56 }, { 73, 0xB4 }, { 74, 0x0E }, { 75, 0xBB },
	{ 76, 0x07 }, { 78, 0xCD }, { 79, 0x10 }, { 80, 0x5E },
	{ 81, 0xEB }, { 82, 0xF0 }, { 83, 0x32 }, { 84, 0xE4 },
	{ 85, 0xCD }, { 86, 0x16 }, { 87, 0xCD }, { 88, 0x19 },
	{ 89, 0xEB }, { 90, 0xFE }, { 91, 0x54 }, { 92, 0x68 },
	{ 93, 0x69 }, { 94, 0x73 }, { 95, 0x20 }, { 96, 0x69 },
	{ 97, 0x73 }, { 98, 0x20 }, { 99, 0x6E }, { 100, 0x6F },
	{ 101, 0x74 }, { 102, 0x20 }, { 103, 0x61 }, { 104, 0x20 },
	{ 105, 0x62 }, { 106, 0x6F }, { 107, 0x6F }, { 108, 0x74 },
	{ 109, 0x61 }, { 110, 0x62 }, { 111, 0x6C }, { 112, 0x65 },
	{ 113, 0x20 }, { 114, 0x64 }, { 115, 0x69 }, { 116, 0x73 },
	{ 117, 0x6B }, { 118, 0x2E }, { 119, 0x20 }, { 120, 0x20 },
	{ 121, 0x50 }, { 122, 0x6C }, { 123, 0x65 }, { 124, 0x61 },
	{ 125, 0x73 }, { 126, 0x65 }, { 127, 0x20 }, { 128, 0x69 },
	{ 129, 0x6E }, { 130, 0x73 }, { 131, 0x65 }, { 132, 0x72 },
	{ 133, 0x74 }, { 134, 0x20 }, { 135, 0x61 }, { 136, 0x20 },
	{ 137, 0x62 }, { 138, 0x6F }, { 139, 0x6F }, { 140, 0x74 },
	{ 141, 0x61 }, { 142, 0x62 }, { 143, 0x6C }, { 144, 0x65 },
	{ 145, 0x20 }, { 146, 0x66 }, { 147, 0x6C }, { 148, 0x6F },
	{ 149, 0x70 }, { 150, 0x70 }, { 151, 0x79 }, { 152, 0x20 },
	{ 153, 0x61 }, { 154, 0x6E }, { 155, 0x64 }, { 156, 0x0D },
	{ 157, 0x0A }, { 158, 0x70 }, { 159, 0x72 }, { 160, 0x65 },
	{ 161, 0x73 }, { 162, 0x73 }, { 163, 0x20 }, { 164, 0x61 },
	{ 165, 0x6E }, { 166, 0x79 }, { 167, 0x20 }, { 168, 0x6B },
	{ 169, 0x65 }, { 170, 0x79 }, { 171, 0x20 }, { 172, 0x74 },
	{ 173, 0x6F }, { 174, 0x20 }, { 175, 0x74 }, { 176, 0x72 },
	{ 177, 0x79 }, { 178, 0x20 }, { 179, 0x61 }, { 180, 0x67 },
	{ 181, 0x61 }, { 182, 0x69 }, { 183, 0x6E }, { 184, 0x20 },
	{ 185, 0x2E }, { 186, 0x2E }, { 187, 0x2E }, { 188, 0x20 },
	{ 189, 0x0D }, { 190, 0x0A }, { 510, 0x55 }, { 511, 0xAA },
	{ 512, 0xF8 }, { 513, 0xFF }, { 514, 0xFF }, { 8704, 0xF8 },
	{ 8705, 0xFF }, { 8706, 0xFF }, { 16896, 0x4B }, { 16897, 0x45 },
	{ 16898, 0x59 }, { 16899, 0x42 }, { 16900, 0x4F }, { 16901, 0x58 },
	{ 16902, 0x20 }, { 16903, 0x50 }, { 16904, 0x41 }, { 16905, 0x52 },
	{ 16906, 0x54 }, { 16907, 0x08 }, { 16910, 0x1C }, { 16911, 0x78 },
	{ 16912, 0x44 }, { 16913, 0x50 }, { 16914, 0x44 }, { 16915, 0x50 },
	{ 16918, 0x1C }, { 16919, 0x78 }, { 16920, 0x44 }, { 16921, 0x50 },
	{ 0, 0 },
};

static struct fs_value g_fs_vals_fty[MAX_CNT_FS_VALUE] = {
	{ 0, 0xEB }, { 1, 0x3C }, { 2, 0x90 }, { 3, 0x6D },
	{ 4, 0x6B }, { 5, 0x66 }, { 6, 0x73 }, { 7, 0x2E },
	{ 8, 0x66 }, { 9, 0x61 }, { 10, 0x74 }, { 12, 0x02 },
	{ 13, 0x08 }, { 14, 0x01 }, { 16, 0x02 }, { 18, 0x02 },
	{ 20, 0x40 }, { 21, 0xF8 }, { 22, 0x08 }, { 24, 0x20 },
	{ 26, 0x40 }, { 36, 0x80 }, { 38, 0x29 }, { 39, 0xDE },
	{ 40, 0x62 }, { 41, 0xA6 }, { 42, 0xD0 }, { 43, 0x4B },
	{ 44, 0x45 }, { 45, 0x59 }, { 46, 0x42 }, { 47, 0x4F },
	{ 48, 0x58 }, { 49, 0x20 }, { 50, 0x50 }, { 51, 0x41 },
	{ 52, 0x52 }, { 53, 0x54 }, { 54, 0x46 }, { 55, 0x41 },
	{ 56, 0x54 }, { 57, 0x31 }, { 58, 0x32 }, { 59, 0x20 },
	{ 60, 0x20 }, { 61, 0x20 }, { 62, 0x0E }, { 63, 0x1F },
	{ 64, 0xBE }, { 65, 0x5B }, { 66, 0x7C }, { 67, 0xAC },
	{ 68, 0x22 }, { 69, 0xC0 }, { 70, 0x74 }, { 71, 0x0B },
	{ 72, 0x56 }, { 73, 0xB4 }, { 74, 0x0E }, { 75, 0xBB },
	{ 76, 0x07 }, { 78, 0xCD }, { 79, 0x10 }, { 80, 0x5E },
	{ 81, 0xEB }, { 82, 0xF0 }, { 83, 0x32 }, { 84, 0xE4 },
	{ 85, 0xCD }, { 86, 0x16 }, { 87, 0xCD }, { 88, 0x19 },
	{ 89, 0xEB }, { 90, 0xFE }, { 91, 0x54 }, { 92, 0x68 },
	{ 93, 0x69 }, { 94, 0x73 }, { 95, 0x20 }, { 96, 0x69 },
	{ 97, 0x73 }, { 98, 0x20 }, { 99, 0x6E }, { 100, 0x6F },
	{ 101, 0x74 }, { 102, 0x20 }, { 103, 0x61 }, { 104, 0x20 },
	{ 105, 0x62 }, { 106, 0x6F }, { 107, 0x6F }, { 108, 0x74 },
	{ 109, 0x61 }, { 110, 0x62 }, { 111, 0x6C }, { 112, 0x65 },
	{ 113, 0x20 }, { 114, 0x64 }, { 115, 0x69 }, { 116, 0x73 },
	{ 117, 0x6B }, { 118, 0x2E }, { 119, 0x20 }, { 120, 0x20 },
	{ 121, 0x50 }, { 122, 0x6C }, { 123, 0x65 }, { 124, 0x61 },
	{ 125, 0x73 }, { 126, 0x65 }, { 127, 0x20 }, { 128, 0x69 },
	{ 129, 0x6E }, { 130, 0x73 }, { 131, 0x65 }, { 132, 0x72 },
	{ 133, 0x74 }, { 134, 0x20 }, { 135, 0x61 }, { 136, 0x20 },
	{ 137, 0x62 }, { 138, 0x6F }, { 139, 0x6F }, { 140, 0x74 },
	{ 141, 0x61 }, { 142, 0x62 }, { 143, 0x6C }, { 144, 0x65 },
	{ 145, 0x20 }, { 146, 0x66 }, { 147, 0x6C }, { 148, 0x6F },
	{ 149, 0x70 }, { 150, 0x70 }, { 151, 0x79 }, { 152, 0x20 },
	{ 153, 0x61 }, { 154, 0x6E }, { 155, 0x64 }, { 156, 0x0D },
	{ 157, 0x0A }, { 158, 0x70 }, { 159, 0x72 }, { 160, 0x65 },
	{ 161, 0x73 }, { 162, 0x73 }, { 163, 0x20 }, { 164, 0x61 },
	{ 165, 0x6E }, { 166, 0x79 }, { 167, 0x20 }, { 168, 0x6B },
	{ 169, 0x65 }, { 170, 0x79 }, { 171, 0x20 }, { 172, 0x74 },
	{ 173, 0x6F }, { 174, 0x20 }, { 175, 0x74 }, { 176, 0x72 },
	{ 177, 0x79 }, { 178, 0x20 }, { 179, 0x61 }, { 180, 0x67 },
	{ 181, 0x61 }, { 182, 0x69 }, { 183, 0x6E }, { 184, 0x20 },
	{ 185, 0x2E }, { 186, 0x2E }, { 187, 0x2E }, { 188, 0x20 },
	{ 189, 0x0D }, { 190, 0x0A }, { 510, 0x55 }, { 511, 0xAA },
	{ 512, 0xF8 }, { 513, 0xFF }, { 514, 0xFF }, { 4608, 0xF8 },
	{ 4609, 0xFF }, { 4610, 0xFF }, { 8704, 0x4B }, { 8705, 0x45 },
	{ 8706, 0x59 }, { 8707, 0x42 }, { 8708, 0x4F }, { 8709, 0x58 },
	{ 8710, 0x20 }, { 8711, 0x50 }, { 8712, 0x41 }, { 8713, 0x52 },
	{ 8714, 0x54 }, { 8715, 0x08 }, { 8718, 0x49 }, { 8719, 0xA2 },
	{ 8720, 0x4C }, { 8721, 0x50 }, { 8722, 0x4C }, { 8723, 0x50 },
	{ 8726, 0x49 }, { 8727, 0xA2 }, { 8728, 0x4C }, { 8729, 0x50 },
	{ 0, 0 },
};

static uint32_t get_transfer_phy_addr(uint32_t *transfer_phy_addr)
{
	register uint32_t x0 asm("x0") = FUNCID_PROVISION_GET_TRANSFER_ADDR;
	register uint32_t x1 asm("x1") = 0;

	do {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x1")
			__asmeq("%2", "x0")
			"smc    #0\n"
			: "=r"(x0), "=r"(x1)
			: "r"(x0));
	} while (0);

	if (x0 == 0)
		*transfer_phy_addr = x1;

	return x0;
}

static uint32_t set_iv(uint32_t transfer_addr, uint32_t iv_size)
{
	register uint32_t x0 asm("x0") = FUNCID_PROVISION_SET_IV;
	register uint32_t x1 asm("x1") = iv_size;
	register uint32_t x2 asm("x2") = transfer_addr;

	do {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x0")
			__asmeq("%2", "x1")
			__asmeq("%3", "x2")
			"smc    #0\n"
			: "=r"(x0)
			: "r"(x0), "r"(x1), "r"(x2));
	} while (0);

	return x0;
}

static uint32_t encrypt(uint32_t transfer_addr, uint32_t *data_size)
{
	register uint32_t x0 asm("x0") = FUNCID_PROVISION_ENCRYPT;
	register uint32_t x1 asm("x1") = *data_size;
	register uint32_t x2 asm("x2") = transfer_addr;

	do {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x1")
			__asmeq("%2", "x2")
			__asmeq("%3", "x0")
			__asmeq("%4", "x1")
			__asmeq("%5", "x2")
			"smc    #0\n"
			: "=r"(x0), "=r"(x1), "=r"(x2)
			: "r"(x0), "r"(x1), "r"(x2));
	} while (0);

	if (x0 == 0)
		*data_size = x1;

	return x0;
}

static void usage(void)
{
	printf("factory_provision -- provision keybox\n\n"
	"Usage:\n"
	"factory_provision write <keybox_name> <keybox_addr> <keybox_size>\n"
	"	- write keybox to key partition\n\n"
	"factory_provision query <keybox_name> [ret_data_addr]\n"
	"	- query whether the keybox exists by keybox name\n"
	"	- when keybox exists, return data: keybox_size(4bytes)\n\n"
	"factory_provision remove <keybox_name>\n"
	"	- remove the keybox by keybox name\n\n");
}

static void parse_params(int argc, char * const argv[],
		struct input_param *params)
{
	char *load_addr_str = NULL;

	memset(params, 0, sizeof(struct input_param));
	switch (argc) {
	case 5:
		if (!memcmp(argv[1], "write", strlen("write"))) {
			params->action = ACTION_WRITE;
			params->keybox_name = argv[2];
			params->keybox_phy_addr =
				(uint32_t)simple_strtoul(argv[3], NULL, 0);
			params->keybox_size =
				(uint32_t)simple_strtoul(argv[4], NULL, 0);
		}
		break;
	case 4:
		if (!memcmp(argv[1], "query", strlen("query"))) {
			params->action = ACTION_QUERY;
			params->keybox_name = argv[2];
			params->ret_data_addr =
				(uint32_t)simple_strtoul(argv[3], NULL, 0);
		}
		break;
	case 3:
		if (!memcmp(argv[1], "query", strlen("query"))) {
			params->action = ACTION_QUERY;
			params->keybox_name = argv[2];
			load_addr_str = getenv("loadaddr");
			if (load_addr_str != NULL)
				params->ret_data_addr =
					(uint32_t)simple_strtoul(
							load_addr_str, NULL, 0);
			else
				params->ret_data_addr = CONFIG_SYS_LOAD_ADDR;
		} else if (!memcmp(argv[1], "remove", strlen("remove"))) {
			params->action = ACTION_REMOVE;
			params->keybox_name = argv[2];
		}
		break;
	case 2:
		if (!memcmp(argv[1], "init", strlen("init")))
			params->action = ACTION_INIT;
		else if (!memcmp(argv[1], "clear", strlen("clear")))
			params->action = ACTION_CLEAR;
		else if (!memcmp(argv[1], "list", strlen("list")))
			params->action = ACTION_LIST;
		break;
	default:
		break;
	}
}

static int check_params(const struct input_param *params)
{
	int ret = CMD_RET_SUCCESS;

	switch (params->action) {
	case ACTION_INIT:
	case ACTION_CLEAR:
	case ACTION_LIST:
		break;
	case ACTION_WRITE:
		if (strlen(params->keybox_name) > MAX_SIZE_KEYBOX_NAME) {
			LOGE("keybox name is too long, and max length is "
					"%d\n", MAX_SIZE_KEYBOX_NAME);
			ret = CMD_RET_KEYBOX_NAME_TOO_LONG;
			goto exit;
		}
		if (params->keybox_size > MAX_SIZE_KEYBOX) {
			LOGE("keybox size is too large, and max size is %d\n",
					MAX_SIZE_KEYBOX);
			ret = CMD_RET_KEYBOX_TOO_LARGE;
			goto exit;
		}
		if (!params->keybox_phy_addr) {
			LOGE("keybox addr error\n");
			ret = CMD_RET_BAD_PARAMETER;
			goto exit;
		}
		break;
	case ACTION_QUERY:
	case ACTION_REMOVE:
		if (strlen(params->keybox_name) > MAX_SIZE_KEYBOX_NAME) {
			LOGE("keybox name is too long, and max length is "
					"%d\n", MAX_SIZE_KEYBOX_NAME);
			ret = CMD_RET_KEYBOX_NAME_TOO_LONG;
			goto exit;
		}
		break;
	default:
		ret = CMD_RET_BAD_PARAMETER;
		break;
	}

exit:
	return ret;
}

static int preprocess_keybox(char *keybox, uint32_t size)
{
	uint32_t res = 0;
	struct encryption_context *enc_cxt = (struct encryption_context *)
		(keybox + sizeof(struct keybox_header));
	uint32_t epek_size = sizeof(enc_cxt->epek);
	uint32_t transfer_phy_addr = 0;
	char *transfer_buf = NULL;

	res = get_transfer_phy_addr(&transfer_phy_addr);
	if (res) {
		LOGE("get transfer address failed, "
				"smc fast call ret = 0x%08X\n", res);
		return CMD_RET_SMC_CALL_FAILED;
	}

	transfer_buf = map_sysmem(transfer_phy_addr, 0);

	memcpy(transfer_buf, enc_cxt->iv, sizeof(enc_cxt->iv));
	res = set_iv(transfer_phy_addr, sizeof(enc_cxt->iv));
	if (res) {
		LOGE("set iv failed, smc fast call ret = 0x%08X\n", res);
		return CMD_RET_SMC_CALL_FAILED;
	}

	memcpy(transfer_buf, enc_cxt->epek, sizeof(enc_cxt->epek));
	res = encrypt(transfer_phy_addr, &epek_size);
	if (res) {
		LOGE("encrypt epek failed, smc fast call ret = 0x%08X\n", res);
		return CMD_RET_SMC_CALL_FAILED;
	}
	memcpy(enc_cxt->epek, transfer_buf, epek_size);

	unmap_sysmem(transfer_buf);

	return CMD_RET_SUCCESS;
}

static const char* get_valid_part_name(void)
{
	int part_num = get_partition_num_by_name(PART_NAME_FTY);

	memset(g_part_name, 0, sizeof(g_part_name));
	if (part_num >= 0)
		strcpy(g_part_name, PART_NAME_FTY);
	else
		strcpy(g_part_name, PART_NAME_RSV);

	return g_part_name;
}

static const struct fs_value* get_fs_values(void)
{
	int part_num = get_partition_num_by_name(PART_NAME_FTY);

	if (part_num >= 0)
		return g_fs_vals_fty;
	else
		return g_fs_vals_rsv;
}

static int make_fat_fs(void)
{
	char cmd[MAX_SIZE_CMD] = { 0 };
	int i = 0;
	const char *part_name = get_valid_part_name();
	const struct fs_value *fs_vals = get_fs_values();
	uint32_t data_phy_addr = (uint32_t)virt_to_phys(g_fs_data);
	uint32_t offset = 0;

	/* erase partition */
	sprintf(cmd, "mmc dev %d;amlmmc switch %d %s;amlmmc erase %s;",
			DEV_NO, DEV_NO, PART_TYPE, part_name);
	if (run_command(cmd, 0)) {
		LOGE("command[%s] failed\n", cmd);
		return CMD_RET_UNKNOWN_ERROR;
	}

	/* write fat file system */
	memset(g_fs_data, 0, SIZE_1K);
	for (i = 0; i < MAX_CNT_FS_VALUE && fs_vals[i].value; i++) {
		if ((i > 0) && (fs_vals[i].idx / SIZE_1K !=
					fs_vals[i - 1].idx / SIZE_1K)) {
			memset(cmd, 0, sizeof(cmd));
			offset = fs_vals[i - 1].idx / SIZE_1K * SIZE_1K;
			sprintf(cmd, "amlmmc write %s 0x%08X 0x%X 0x%X;",
				part_name, data_phy_addr, offset, SIZE_1K);
			if (run_command(cmd, 0)) {
				LOGE("command[%s] failed\n", cmd);
				return CMD_RET_UNKNOWN_ERROR;
			}
		}
		g_fs_data[fs_vals[i].idx % SIZE_1K] = fs_vals[i].value;
	}
	memset(cmd, 0, sizeof(cmd));
	offset = fs_vals[i - 1].idx / SIZE_1K * SIZE_1K;
	sprintf(cmd, "amlmmc write %s 0x%08X 0x%X 0x%X;",
			part_name, data_phy_addr, offset, SIZE_1K);
	if (run_command(cmd, 0)) {
		LOGE("command[%s] failed\n", cmd);
		return CMD_RET_UNKNOWN_ERROR;
	}

	return CMD_RET_SUCCESS;
}

static int dev_writable(void)
{
	char cmd[MAX_SIZE_CMD] = { 0 };

	sprintf(cmd, "fatinfo %s 0x%X:0x%X", DEV_NAME, DEV_NO,
		get_partition_num_by_name((char *)get_valid_part_name()));
	if (run_command(cmd, 0)) {
		LOGD("command[%s] failed\n", cmd);
		return CMD_RET_DEVICE_NOT_AVAILABLE;
	}

	return CMD_RET_SUCCESS;
}

static int dev_write(const char *name, const char *data, uint32_t size)
{
	char cmd[MAX_SIZE_CMD] = { 0 };

	sprintf(cmd, "fatwrite %s 0x%X:0x%X 0x%08X %s 0x%X", DEV_NAME, DEV_NO,
		get_partition_num_by_name((char *)get_valid_part_name()),
		(uint32_t)virt_to_phys((void *)data), name, size);
	if (run_command(cmd, 0)) {
		LOGD("command[%s] failed\n", cmd);
		return CMD_RET_DEVICE_NOT_AVAILABLE;
	}

	return CMD_RET_SUCCESS;
}

static int init_partition(void)
{
	int ret = CMD_RET_SUCCESS;
	char dev_part_str[16] = { 0 };

	ret = dev_writable();
	if (ret) {
		ret = make_fat_fs();
		if (ret) {
			LOGE("make fat file system failed\n");
			return ret;
		}
		ret = dev_writable();
		if (ret) {
			LOGE("device not available\n");
			return ret;
		}
	}

	sprintf(dev_part_str, "0x%X:0x%X", DEV_NO,
		get_partition_num_by_name((char *)get_valid_part_name()));
	if (fs_set_blk_dev(DEV_NAME, dev_part_str, FS_TYPE_FAT)) {
		LOGE("set block device failed\n");
		ret = CMD_RET_UNKNOWN_ERROR;
	}

	return ret;
}

void convert_to_uuid_str(const char uuid[16], char uuid_str[40])
{
	int i = 0;
	const char *uuid_ptr = uuid;
	char *str_ptr = uuid_str;

	for (i = 0; i < 4; i++) {
		sprintf(str_ptr, "%02x", *uuid_ptr);
		str_ptr += 2;
		uuid_ptr++;
	}
	*str_ptr++ = '-';
	for (i = 0; i < 2; i++) {
		sprintf(str_ptr, "%02x", *uuid_ptr);
		str_ptr += 2;
		uuid_ptr++;
	}
	*str_ptr++ = '-';
	for (i = 0; i < 2; i++) {
		sprintf(str_ptr, "%02x", *uuid_ptr);
		str_ptr += 2;
		uuid_ptr++;
	}
	*str_ptr++ = '-';
	for (i = 0; i < 8; i++) {
		sprintf(str_ptr, "%02x", *uuid_ptr);
		str_ptr += 2;
		uuid_ptr++;
	}
}

static int check_keybox(const char *keybox, uint32_t size)
{
	const struct keybox_header *hdr = (const struct keybox_header *)keybox;
	uint32_t hdr_cxt_size = sizeof(struct keybox_header)
		+ sizeof(struct encryption_context);

	if (hdr->magic != KEYBOX_HDR_MAGIC) {
		LOGE("keybox header magic error"
			"(expected magic: 0x%08X; wrong magic: 0x%08X)\n",
			KEYBOX_HDR_MAGIC, hdr->magic);
		return CMD_RET_KEYBOX_BAD_FORMAT;
	}
	if (hdr->version < KEYBOX_HDR_MIN_VERSION) {
		LOGE("keybox header version error"
			"(min version: %d; wrong version: %d)\n",
			KEYBOX_HDR_MIN_VERSION, hdr->version);
		return CMD_RET_KEYBOX_BAD_FORMAT;
	}
	if (size > MAX_SIZE_KEYBOX || size <= hdr_cxt_size
			|| hdr->key_size != size - hdr_cxt_size) {
		LOGE("keybox length error\n");
		return CMD_RET_KEYBOX_BAD_FORMAT;
	}
	if (hdr->provision_type != PROVISION_TYPE_FACTORY) {
		LOGE("keybox provision type error"
			"(expected type: %d; wrong type: %d)\n",
			PROVISION_TYPE_FACTORY, hdr->provision_type);
		return CMD_RET_KEYBOX_BAD_FORMAT;
	}

	return CMD_RET_SUCCESS;
}

static void calc_sha256(const char *data, uint32_t data_size, char *sha256)
{
	sha256_csum_wd((const unsigned char *)data, data_size, (unsigned char *)sha256, 0);
}

static int verify_written_keybox(const char *keybox_name, const char *keybox,
		uint32_t keybox_size)
{
	char sha256[SHA256_SUM_LEN] = { 0 };
	char written_sha256[SHA256_SUM_LEN] = { 0 };
	loff_t act_read = 0;

	calc_sha256(keybox, keybox_size, sha256);

	memset(g_keybox, 0, sizeof(g_keybox));
	if (fat_read_file(keybox_name, g_keybox, 0, MAX_SIZE_KEYBOX, &act_read))
		return CMD_RET_UNKNOWN_ERROR;
	if (keybox_size != act_read)
		return CMD_RET_UNKNOWN_ERROR;

	calc_sha256(g_keybox, act_read, written_sha256);

	if (memcmp(sha256, written_sha256, SHA256_SUM_LEN))
		return CMD_RET_UNKNOWN_ERROR;

	return CMD_RET_SUCCESS;
}

static int append_keybox(const char *keybox_name, const char *keybox,
		uint32_t keybox_size)
{
	int ret = CMD_RET_SUCCESS;

	ret = init_partition();
	if (ret)
		return ret;

	ret = dev_write(keybox_name, keybox, keybox_size);
	if (ret)
		return ret;

	ret = verify_written_keybox(keybox_name, keybox, keybox_size);
	if (ret) {
		LOGE("verify written keybox '%s' failed\n", keybox_name);
		return ret;
	}

	LOGI("write keybox '%s' success\n", keybox_name);

	return ret;
}

static int get_keybox_size(const char *keybox_name, uint32_t *size)
{
	loff_t keybox_size = 0;

	if (fat_size(keybox_name, &keybox_size)) {
		LOGE("get keybox '%s' size failed\n", keybox_name);
		return CMD_RET_UNKNOWN_ERROR;
	} else {
		*size = (uint32_t)keybox_size;
		LOGI("keybox '%s' size is %d\n", keybox_name, *size);
		return CMD_RET_SUCCESS;
	}
}

static int query_keybox(const char *keybox_name, char *ret_data)
{
	int ret = CMD_RET_SUCCESS;

	ret = init_partition();
	if (ret)
		return ret;

	if (!fat_exists(keybox_name)) {
		LOGI("keybox '%s' not exists\n", keybox_name);
		return CMD_RET_KEYBOX_NOT_EXIST;
	}
	else {
		return get_keybox_size(keybox_name, (uint32_t *)ret_data);
	}
}

static int remove_keybox(const char *keybox_name)
{
	int ret = init_partition();

	if (ret)
		return ret;

	if (fs_unlink(keybox_name)) {
		LOGE("remove '%s' failed\n", keybox_name);
		return CMD_RET_UNKNOWN_ERROR;
	}

	LOGI("remove '%s' success\n", keybox_name);
	return CMD_RET_SUCCESS;
}

static int remove_all_keyboxes(void)
{
	int ret = CMD_RET_SUCCESS;
	struct fs_dir_stream *dirs = NULL;
	struct fs_dirent *dent = NULL;
	char cmd[MAX_SIZE_CMD] = { 0 };

	ret = init_partition();
	if (ret)
		return ret;

	if (fat_opendir("/", &dirs)) {
		LOGE("open '/' failed\n");
		return CMD_RET_UNKNOWN_ERROR;
	}

	while (!fat_readdir(dirs, &dent)) {
		if (dent->type != FS_DT_REG) // not regular file
			continue;

		sprintf(cmd, "fatrm %s 0x%X:0x%X %s", DEV_NAME, DEV_NO,
				get_partition_num_by_name(
					(char *)get_valid_part_name()),
				dent->name);
		if (run_command(cmd, 0)) {
			ret = CMD_RET_UNKNOWN_ERROR;
			LOGE("remove '%s' failed\n", dent->name);
		} else {
			LOGI("remove '%s' success\n", dent->name);
		}
	}

	fat_closedir(dirs);
	return ret;
}

static int list_all_keyboxes(void)
{
	int ret = CMD_RET_SUCCESS;
	char cmd[MAX_SIZE_CMD] = { 0 };

	ret = init_partition();
	if (ret)
		return ret;

	sprintf(cmd, "fatls %s 0x%X:0x%X", DEV_NAME, DEV_NO,
		get_partition_num_by_name((char *)get_valid_part_name()));
	if (run_command(cmd, 0)) {
		LOGE("command[%s] failed\n", cmd);
		ret = CMD_RET_UNKNOWN_ERROR;
	}

	return ret;
}

static int remove_same_type_keybox(uint32_t key_type, const char *uuid)
{
	int ret = CMD_RET_SUCCESS;
	struct fs_dir_stream *dirs = NULL;
	struct fs_dirent *dent = NULL;
	struct keybox_header hdr;
	loff_t act_read = 0;
	char uuid_str[40] = { 0 };

	ret = init_partition();
	if (ret)
		return ret;

	if (fat_opendir("/", &dirs)) {
		LOGE("open '/' failed\n");
		return CMD_RET_UNKNOWN_ERROR;
	}

	convert_to_uuid_str(uuid, uuid_str);
	while (!fat_readdir(dirs, &dent)) {
		if (dent->type != FS_DT_REG) // not regular file
			continue;

		memset(&hdr, 0, sizeof(hdr));
		if (fat_read_file(dent->name, &hdr, 0, sizeof(hdr), &act_read)
				|| act_read != sizeof(hdr)) {
			LOGE("read keybox '%s' failed\n", dent->name);
			ret = CMD_RET_UNKNOWN_ERROR;
			goto exit;
		}

		if (!memcmp(uuid, hdr.ta_uuid, sizeof(hdr.ta_uuid)) &&
				key_type == hdr.key_type) {
			ret = remove_keybox(dent->name);
			if (ret != CMD_RET_SUCCESS) {
				LOGE("remove the same type"
					"(uuid = %s, key_type = 0x%02X) "
					"keybox '%s' failed\n",
					uuid_str, key_type, dent->name);
				goto exit;
			}
		}
	}

exit:
	fat_closedir(dirs);
	return ret;
}

int cmd_func(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = CMD_RET_SUCCESS;
	struct input_param params;
	char *in_kb = NULL;
	char *ret_data = NULL;
	const struct keybox_header *hdr = NULL;

	parse_params(argc, argv, &params);

	ret = check_params(&params);
	if (ret != CMD_RET_SUCCESS)
		goto exit;

	switch (params.action) {
	case ACTION_INIT:
		ret = init_partition();
		break;
	case ACTION_WRITE:
		in_kb = map_sysmem(params.keybox_phy_addr, 0);

		ret = check_keybox(in_kb, params.keybox_size);
		if (ret != CMD_RET_SUCCESS)
			goto exit;

		hdr = (const struct keybox_header *)in_kb;
		ret = remove_same_type_keybox(hdr->key_type,
				(const char *)hdr->ta_uuid);
		if (ret != CMD_RET_SUCCESS)
			goto exit;

		memcpy(g_keybox, in_kb, params.keybox_size);

		ret = preprocess_keybox(g_keybox, params.keybox_size);
		if (ret != CMD_RET_SUCCESS)
			goto exit;

		ret = append_keybox(params.keybox_name, g_keybox,
				params.keybox_size);
		if (ret != CMD_RET_SUCCESS)
			goto exit;
		break;
	case ACTION_QUERY:
		ret_data = map_sysmem(params.ret_data_addr, 0);
		ret = query_keybox(params.keybox_name, ret_data);
		if (ret != CMD_RET_SUCCESS)
			goto exit;
		break;
	case ACTION_REMOVE:
		ret = remove_keybox(params.keybox_name);
		if (ret != CMD_RET_SUCCESS)
			goto exit;
		break;
	case ACTION_CLEAR:
		ret = remove_all_keyboxes();
		if (ret != CMD_RET_SUCCESS)
			goto exit;
		break;
	case ACTION_LIST:
		ret = list_all_keyboxes();
		if (ret != CMD_RET_SUCCESS)
			goto exit;
		break;
	default:
		break;
	}

exit:
	if (in_kb)
		unmap_sysmem(in_kb);
	if (ret_data)
		unmap_sysmem(ret_data);
	if (ret == CMD_RET_BAD_PARAMETER)
		usage();
	return ret;
}

/* -------------------------------------------------------------------- */
U_BOOT_CMD(
	factory_provision, CONFIG_SYS_MAXARGS, 0, cmd_func,
	"provision keybox\n",
	"write <keybox_name> <keybox_addr> <keybox_size>\n"
	"	- write keybox to key partition\n\n"
	"query <keybox_name> [ret_data_addr]\n"
	"	- query whether the keybox exists by keybox name\n"
	"	- when keybox exists, return data: keybox_size(4bytes)\n\n"
	"remove <keybox_name>\n"
	"	- remove the keybox by keybox name\n"
);
