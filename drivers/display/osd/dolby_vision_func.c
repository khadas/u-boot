// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/kernel.h>
#include <amlogic/dolby_vision_func.h>
#include <common.h>
#include "dv_func.h"
#include "../../common/ini/ini_platform.h"
#include <malloc.h>
#include <u-boot/md5.h>
#include <u-boot/sha256.h>
#include <libavb.h>

#define DV_FIRMWARE_ADDR 0x30000000
#define DV_FW_ADDR 0xa00000
#define DV_FW_MAX_SIZE 0x80000 /*500k*/
#define LOAD_FROM_ARRAY 0 /*only for debug*/

struct cp_struct {
	int src_format;
	int dst_format;
	void *in_comp;
	int in_comp_size;
	void *in_md;
	int in_md_size;
	int set_priority;
	int set_bit_depth;
	int set_chroma_format;
	int set_yuv_range;
	int set_graphic_min_lum;
	int set_graphic_max_lum;
	int set_target_min_lum;
	int set_target_max_lum;
	int set_no_el;
	void *p_hdr10_param;
	void *dovi_setting;
};

static unsigned long my_hex_atol(const char *str)
{
	unsigned long result = 0;
	char p1;
	char p2;
	const char *p = str;

	if (str) {
		//remove ”0x“
		p1 = *str;
		p2 = *(++str);
		if (p1 == '0' && (p2 == 'x' || p2 == 'X')) {
			p += 2;
		}
		while (*p) {
			if (*p >= '0' && *p <= '9')
				result = result * 16 + (*p++ -'0');
			else if (*p >= 'a' && *p <= 'f')
				result = result * 16 + (*p++ -'a' + 10);
			else if (*p >= 'A' && *p <= 'F')
				result = result * 16 + (*p++ -'A' + 10);
			else {
				printf("invalid character!\n");
				return 0;
			}
		}
	}

	return result;
}

unsigned long do_go_exec_dv(ulong (*entry)(int, char *), int argc, char *p)
{
	return entry(argc, p);
}

// load dovi_func in dv_func.h, only for debug
int load_dv_func(void *cp_info)
{
	ulong addr = DV_FW_ADDR;
	char *p[8] = {NULL};
	char *func_addr = NULL;

	func_addr = getenv("dv_fw_addr");
	if (func_addr) {
		addr = my_hex_atol(func_addr);
		p[0] = (char*)(cp_info);

		if (addr) {
			memcpy((void*)addr, dv_func, sizeof(dv_func));

			printf("dv fw addr %p, cp_info %p\n", (void *)addr, cp_info);
			do_go_exec_dv((void*)addr, 1, (char *const)p);
		} else {
			printf("load_dv_func: invalid dv_fw_addr %s\n", func_addr);
		}
	} else {
		printf("load_dv_func: not found dv_fw_addr!\n");
	}
	return 0;
}

#define MD5_DIGEST_LENGTH 16
#define SHA256_DIGEST_LENGTH 32
#define MAGIC_CODE 0x49564f44
#define MAGIC_CODE_SIZE 4
#define MD5SUM_SIZE 64
#define USER_INFO_SIZE 64
#define GIT_INFO_SIZE 64
#define HEADER_SIZE (MAGIC_CODE_SIZE+MD5SUM_SIZE+USER_INFO_SIZE+GIT_INFO_SIZE)

char* md5_hex2str(unsigned char* in_md5_hex, char* out_md5_str)
{
	int i = 0;

	for (i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		sprintf(out_md5_str + i * 2, "%.2x", in_md5_hex[i]);
	}
	out_md5_str[MD5_DIGEST_LENGTH * 2] = '\0';
	return out_md5_str;
}

char* sha256_hex2str(unsigned char* in_sha_hex, char* out_sha_str)
{
	int i = 0;

    for (i = 0; i < SHA256_SUM_LEN; ++i) {
        sprintf(out_sha_str + i * 2, "%.2x", in_sha_hex[i]);
    }
    out_sha_str[SHA256_SUM_LEN * 2] = '\0';
    return out_sha_str;
}

#ifdef CONFIG_CMD_BOOTCTOL_AVB
static const char *avb_get_dovi_hash_from_vbmeta(AvbSlotVerifyData* data)
{
	int i;
	AvbVBMetaData *p;
	const char *ret;
	size_t len = 0;

	if (!data)
		return NULL;

	for (i = 0; i < data->num_vbmeta_images; i++) {
		p = &data->vbmeta_images[i];
		if (strcmp(p->partition_name, "vbmeta") == 0) { /* match */
			if (p->verify_result != AVB_VBMETA_VERIFY_RESULT_OK) {// not verified
				printf("vbmeta verify_result %d\n", p->verify_result);

				/*device lock, treat as error*/
				if (!is_device_unlocked()) {
					printf("device lock, but vbmeta verify fail\n");
					return NULL;
				}
			}

			ret = avb_property_lookup(p->vbmeta_data,
						  p->vbmeta_size,
						  "dovi_hash",
						  0,
						  &len);
			if (ret)
				return ret;
			else
				printf("not found dovi hash, len = %d\n", (int)len);
		}
	}
	return NULL;
}
#endif

/*pass: return 1; fail: return -1*/
int check_dovi_sha256(unsigned char* dovi_body, int size) {

	unsigned char sha256[SHA256_SUM_LEN] = {0};
	char sha256_str[SHA256_SUM_LEN * 2 + 1] = {0};
	sha256_context sha256_cxt;
#ifdef CONFIG_CMD_BOOTCTOL_AVB
	int nRet = 0;
	AvbSlotVerifyData* out_data = NULL;
	const char *tmp = NULL;
#endif
	char dovi_expect_hash[SHA256_SUM_LEN * 2 + 1] = {0};

	if (!dovi_body)
		return -1;

#ifdef CONFIG_CMD_BOOTCTOL_AVB
	/*step 1: check if avb enabled, if not enable, no check dv sha256 */
	char *avb_s = getenv("avb2");
	if (avb_s == NULL) {
		run_command("get_avb_mode;", 0);
		avb_s = getenv("avb2");
	}
	printf("check_dovi_sha256, avb2: %s\n", avb_s);
	if (avb_s == NULL || (avb_s && (strcmp(avb_s, "1") != 0))) {
		return -1;
	}
	/*step 2: avb_verify */
	nRet = avb_verify(&out_data);
	printf("check_dovi_sha256: locked=%d, result=%d\n", !is_device_unlocked(), nRet);
	if (is_device_unlocked()) {
		if(nRet != AVB_SLOT_VERIFY_RESULT_OK &&
			nRet != AVB_SLOT_VERIFY_RESULT_ERROR_VERIFICATION &&
			nRet != AVB_SLOT_VERIFY_RESULT_ERROR_ROLLBACK_INDEX &&
			nRet != AVB_SLOT_VERIFY_RESULT_ERROR_PUBLIC_KEY_REJECTED) {
			if (out_data)
				avb_slot_verify_data_free(out_data);
			return -1;
		}
	} else {
		if (nRet != AVB_SLOT_VERIFY_RESULT_OK) {
			if (out_data)
				avb_slot_verify_data_free(out_data);
			return -1;
		}
	}
	if (out_data) {
		tmp = avb_get_dovi_hash_from_vbmeta(out_data);

		if (tmp) {
			printf("success find dovi_hash:%s\n", tmp);
			strncpy(dovi_expect_hash, tmp, SHA256_SUM_LEN * 2);
			avb_slot_verify_data_free(out_data);
		} else {
			printf("not find dovi_hash\n");
			avb_slot_verify_data_free(out_data);
			return -1;
		}
	} else {
		return -1;
	}
#endif//CONFIG_CMD_BOOTCTOL_AVB

	/*step 3: calculate dovi.fw sha256*/
	sha256_starts(&sha256_cxt);
	sha256_update(&sha256_cxt, (const uint8_t *)dovi_body, (uint32_t)size);
	sha256_finish(&sha256_cxt, (uint8_t *)sha256);
	sha256_hex2str(sha256, sha256_str);
	printf("dovi fw hash: %s\n", sha256_str);

	/*step 4: compare expect sha256 with dovi.fw */
	if (memcmp(dovi_expect_hash, sha256_str, SHA256_SUM_LEN * 2) != 0) {
		printf("expect:%s, real: %s\n", dovi_expect_hash, sha256_str);
		return -1;
	}
	return 1;
}


/*valid: return 1; invalid: return -1*/
int check_fw_valid(unsigned char* fw_buf, int size) {

	unsigned char md5_hex[MD5_DIGEST_LENGTH + 1] = {0};
	char md5_str[MD5_DIGEST_LENGTH * 2 + 1] = {0};
	char bin_md5str[MD5_DIGEST_LENGTH * 2+ 1] = {0};
	int bin_magic_code = 0;

	if (!fw_buf || size <= HEADER_SIZE)
		return -1;

	printf("***Magic code: %x\n", *(int *)fw_buf);
	printf("***Body MD5:   %s\n", fw_buf + MAGIC_CODE_SIZE);
	printf("***Userinfo:   %s\n", fw_buf + MAGIC_CODE_SIZE + MD5SUM_SIZE);
	printf("***Gitinfo:    %s\n", fw_buf + MAGIC_CODE_SIZE + MD5SUM_SIZE + USER_INFO_SIZE);

	/*read MAGIC CODE  from header*/
	bin_magic_code = *(int *)fw_buf;

	/*step 1: check MAGIC CODE*/
	if (bin_magic_code != MAGIC_CODE) {
		printf("%s, MAGIC_CODE %x check error\n", __func__, bin_magic_code);
		return -1;
	}

	/*read md5 from header*/
	memcpy(bin_md5str, fw_buf + MAGIC_CODE_SIZE, MD5_DIGEST_LENGTH * 2);
	bin_md5str[MD5_DIGEST_LENGTH * 2] = '\0';

	/*calulate md5*/
	md5(fw_buf + HEADER_SIZE, size- HEADER_SIZE, md5_hex);
	md5_hex[MD5_DIGEST_LENGTH] = '\0';
	md5_hex2str(md5_hex, md5_str);

	//printf("md5_str %s\n", md5_str);

	/*step 2: check md5*/
	if (memcmp(md5_str, bin_md5str, MD5_DIGEST_LENGTH*2)) {
		printf("md5 check fail %s, %s\n", md5_str, bin_md5str);
		return -1;
	}

	/*step 3: check sha256*/
	if (check_dovi_sha256(fw_buf + HEADER_SIZE, size- HEADER_SIZE) == -1) {
		printf("sha256 check fail\n");
		return -1;
	}
	return 1;
}

/*load fail: return -1; load success: return 0*/
int load_dv_fw(char *fw_dir, void *cp_info)
{
	unsigned long addr = DV_FW_ADDR;
	char *p[8] = {NULL};
	int rd_cnt = 0, tmp_size = 0;
	char *fw_addr = NULL;
	unsigned char *tmp_buf = NULL;

	printf("%s: load %s\n", __func__, fw_dir);

	if (!fw_dir) {
		printf("%s, null dir!\n", __func__);
		return -1;
	}

	tmp_size = iniGetFileSize(fw_dir);

	if (tmp_size <= HEADER_SIZE || tmp_size > DV_FW_MAX_SIZE) {
		printf("%s, file \"%s\" size error!\n", __func__, fw_dir);
		return -1;
	}

	fw_addr = getenv("dv_fw_addr");
	if (fw_addr)
		addr = my_hex_atol(fw_addr);
	if (!fw_addr || addr == 0) {
		printf("%s, not found dv_fw_addr %s!\n", __func__, fw_addr);
		return -1;
	}

	tmp_buf = (unsigned char *)malloc(tmp_size);
	if (!tmp_buf) {
		printf("%s, malloc failed!\n", __func__);
		return -1;
	}

	if (tmp_buf) {
		rd_cnt = iniReadFileToBuffer(fw_dir, 0, tmp_size, (unsigned char *)tmp_buf);
		if (rd_cnt <= 0) {
			printf("%s, read %s fw_dir error\n", __func__, fw_dir);
			free(tmp_buf);
			return -1;
		}
	}

	if (check_fw_valid(tmp_buf, tmp_size) == 1) {
		memcpy((void *)addr, tmp_buf + HEADER_SIZE, tmp_size - HEADER_SIZE);
		p[0] = (char*)(cp_info);

		printf("dv fw addr %p, cp_info %p\n", (void *)addr, cp_info);
		do_go_exec_dv((void*)addr, 1, (char *const)p);
		free(tmp_buf);
		return 0;
	} else {
		free(tmp_buf);
		return -1;
	}

	return 0;
}

struct cp_struct cp_info;

/*return 0: success*/
int control_path(
	enum signal_format_enum in_format,
	enum signal_format_enum out_format,
	char *in_comp, int in_comp_size,
	char *in_md, int in_md_size,
	enum priority_mode_enum set_priority,
	int set_bit_depth, int set_chroma_format, int set_yuv_range,
	int set_graphic_min_lum, int set_graphic_max_lum,
	int set_target_min_lum, int set_target_max_lum,
	int set_no_el,
	struct hdr10_parameter *p_hdr10_param,
	struct dovi_setting_s *output)
{
    char *fw_dir_odm_ext = NULL;
    char *fw_dir_vendor = NULL;
    char *fw_dir = NULL;
    int ret = 0;

	fw_dir_odm_ext = getenv("dv_fw_dir_odm_ext");
	fw_dir_vendor = getenv("dv_fw_dir_vendor");
	fw_dir = getenv("dv_fw_dir");

	//struct cp_struct cp_info;
	cp_info.src_format = in_format;
	cp_info.dst_format = out_format;
	cp_info.in_comp = NULL;
	cp_info.in_comp_size = 0;
	cp_info.in_md = NULL;
	cp_info.in_md_size = 0;
	cp_info.set_priority = set_priority;
	cp_info.set_bit_depth = set_bit_depth;
	cp_info.set_chroma_format = set_chroma_format;
	cp_info.set_yuv_range = set_yuv_range;
	cp_info.set_graphic_min_lum = set_graphic_min_lum;
	cp_info.set_graphic_max_lum = set_graphic_max_lum;
	cp_info.set_target_min_lum = set_target_min_lum;
	cp_info.set_target_max_lum = set_target_max_lum;
	cp_info.set_no_el = set_no_el;
	cp_info.p_hdr10_param = p_hdr10_param;
	cp_info.dovi_setting = output;

	printf("control_path %p\n", &cp_info);
#if LOAD_FROM_ARRAY
	load_dv_func(&cp_info);// load dovi_func[] in dv_func.h and run
#else
	if (fw_dir_odm_ext && iniIsFileExist(fw_dir_odm_ext)) {
		ret = load_dv_fw(fw_dir_odm_ext, &cp_info);
	} else if (fw_dir_vendor && iniIsFileExist(fw_dir_vendor)) {
		ret = load_dv_fw(fw_dir_vendor, &cp_info);
	} else if (fw_dir && iniIsFileExist(fw_dir)) {
		ret = load_dv_fw(fw_dir, &cp_info);
	} else {
		printf("load dv fw fail\n");
		ret = -1;
	}
#endif
	return ret;
}
