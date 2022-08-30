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

#ifdef CONFIG_CMD_CAR_PARAMS

// calibrator_message used 1024K-1026K by misc partition
typedef struct calibrator_message {
	uint32_t magic; //magic number
	uint8_t rsv[4];
	int64_t camera[9]; //camera Internal parameter matrix
	int64_t distortion[5]; //Distortion matrix
	int64_t matrix_rtos[9]; //projection matrix for rtos
	int64_t matrix_gles[9];//projection matrix for gles
	int64_t car[5]; //car parameters

	// rounds up to 2048-byte.
	uint8_t reserve[1740];

	// CRC32 of all 28 bytes preceding this field (little endian
	// format).
	uint32_t crc32_le;
} calibrator_message;

#define CALIBRATOR_OFFSET  (1024 * 1024)
#define CALIBRATOR_SIZE    2048
#define CALIBRATOR_MAGIC   0x51504948 /* magic number */

static uint32_t car_crc32(const uint8_t *buf, size_t size)
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

bool calibrator_info_validate(calibrator_message *info)
{
	uint32_t crc;

	if (info->magic != CALIBRATOR_MAGIC) {
		printf("Magic 0x%x is incorrect.\n", info->magic);
		return false;
	}

	crc = car_crc32((const uint8_t *)info,
		sizeof(calibrator_message) - sizeof(uint32_t));
	if (info->crc32_le != crc) {
		printf("crc 0x%x != 0x%x\n", info->crc32_le, crc);
		return false;
	}

	return true;
}

void calibrator_info_reset(calibrator_message *info)
{
	memset(info, '\0', sizeof(calibrator_message));
	info->magic = CALIBRATOR_MAGIC;
}

void dump_info(calibrator_message *info)
{
	int i;

	printf("info->magic = 0x%x\n", info->magic);
	for (i = 0; i < 9; ++i) {
		printf("info->camera[%d] = %lld\n",
				i, info->camera[i]);
	}
	for (i = 0; i < 5; ++i) {
		printf("info->distortion[%d] = %lld\n",
				i, info->distortion[i]);
	}
	for (i = 0; i < 9; ++i) {
		printf("info->matrix_rtos[%d] = %lld\n",
				i, info->matrix_rtos[i]);
	}
	for (i = 0; i < 9; ++i) {
		printf("info->matrix_gles[%d] = %lld\n",
				i, info->matrix_gles[i]);
	}
	for (i = 0; i < 5; ++i) {
		printf("info->car[%d] = %lld\n",
				i, info->car[i]);
	}
}

int calibrator_info_open_partition(unsigned char *miscbuf)
{
	char *partition = "misc";

	printf("Start read %s partition datas!\n", partition);
	if (store_read((const char *)partition,
		CALIBRATOR_OFFSET, CALIBRATOR_SIZE, miscbuf) < 0) {
		printf("failed to store read %s.\n", partition);
		return -1;
	}
	return 0;
}

bool calibrator_info_save(calibrator_message *info)
{
	char *partition = "misc";

	printf("save calibrator-info\n");
	info->crc32_le = car_crc32((const uint8_t *)info,
		sizeof(calibrator_message) - sizeof(uint32_t));

	dump_info(info);
	store_write((const char *)partition, CALIBRATOR_OFFSET, CALIBRATOR_SIZE,
		(unsigned char *)info);
	return true;
}

static int do_ReadCarParams(cmd_tbl_t *cmdtp,
	int flag,
	int argc,
	char * const argv[])
{
	calibrator_message *info = NULL;
	unsigned char *loadaddr = 0;

	if (argc > 2)
		loadaddr = (unsigned char *)simple_strtoul(argv[2], NULL, 16);
	else
		loadaddr = (unsigned char *)simple_strtoul(env_get("car_mem_addr"), NULL, 16);

	info = (calibrator_message *)loadaddr;

	calibrator_info_open_partition(loadaddr);

	if (!calibrator_info_validate(info)) {
		printf("calibrator-info is invalid. resetting\n");
		calibrator_info_reset(info);
		calibrator_info_save(info);
	} else {
		dump_info(info);
	}

	return 0;
}

#endif

U_BOOT_CMD(read_car_params, 3, 0, do_ReadCarParams,
	"read_car_params",
	"\nThis command will read car params to mem\n"
	"partition by mark to decide whether execute command!\n"
	"So you can execute command: read_car_params"
);

