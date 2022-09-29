// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2022 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <mapmem.h>
#include <tee.h>

#define RMA_OS_DEBUG_ENABLE (0)

#define CMD_RMA_OS_INIT_DATA      (0)
#define CMD_RMA_OS_GET_CMD_LIST   (1)
#define CMD_RMA_OS_RUN_CMD        (2)
#define CMD_RMA_OS_GET_INFO       (3)

#define TA_RMA_TEE_OS_UUID \
		{ 0x99a216e5, 0x6718, 0x4dfd, \
		{ 0xbb, 0xb1, 0x37, 0xcd, 0x0d, 0x00, 0x10, 0xc6 } }

#define RMA_CMD_MAX_NUM           (64)
#define RMA_CMD_MAX_LENGTH        (128)
#define RMA_AUTH_HDR_MAGIC        (0x414D52) // "RMA"
#define RMA_AUTH_DATA_MAX_SIZE    (32 + 256 + 16 * RMA_CMD_MAX_NUM)

struct rma_info_output {
	char name[RMA_CMD_MAX_LENGTH];
	char info[RMA_CMD_MAX_LENGTH];
};

static struct rma_info_output rma_info[RMA_CMD_MAX_NUM + 1];

static void dump_rma_info(struct rma_info_output *result, uint32_t len,
		char *title, char *comment)
{
	uint32_t i;

	for (i = 0; i < 10; i++)
		printf("---------");

	printf("\n");

	printf("%-55s | %s\n", title, comment);

	for (i = 0; i < 10; i++)
		printf("---------");
	printf("\n");

	for (i = 0; i < len; i++)
		printf("%-55s | %s\n", result[i].name, result[i].info);

	for (i = 0; i < 10; i++)
		printf("---------");

	printf("\n");
}

static int do_rma_init(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret = CMD_RET_SUCCESS;
	uint8_t *buf;
	uint32_t map_addr = 0;
	uint32_t magic;
	uint32_t number;
	uint32_t real_size;
	char rma_data[RMA_AUTH_DATA_MAX_SIZE];
	struct tee_param params;
	struct udevice *dev;
	struct tee_open_session_arg open_arg;
	struct tee_invoke_arg invoke_arg;
	const struct tee_optee_ta_uuid uuid = TA_RMA_TEE_OS_UUID;

	if (argc != 2) {
		printf("Usage: rma init ADDR, ADDR is memory address\n");
		return CMD_RET_USAGE;
	}

	/* Get RMA data memory address */
	map_addr = simple_strtoul(argv[1], NULL, 16);
	if (map_addr <= 0 || map_addr >= 0xffffffff) {
		printf("RMA data memory address: %#x invalid!\n", map_addr);
		return CMD_RET_USAGE;
	}

	/* Map RMA data memory */
	buf = map_sysmem(map_addr, RMA_AUTH_DATA_MAX_SIZE);

	/* Check RMA data magic */
	magic = *(uint32_t *)buf;
	if (magic != RMA_AUTH_HDR_MAGIC) {
		printf("RMA data magic: %#x invalid, it must be %#x\n",
			magic, RMA_AUTH_HDR_MAGIC);
		unmap_sysmem(buf);
		return -CMD_RET_FAILURE;
	}

	/* Check RMA data size */
	number = *(uint32_t *)(buf + 16);
	real_size = RMA_AUTH_DATA_MAX_SIZE - (16 * (RMA_CMD_MAX_NUM - number));
	if (real_size > RMA_AUTH_DATA_MAX_SIZE) {
		printf("RMA data size %#x larger than %#x\n",
			real_size, RMA_AUTH_DATA_MAX_SIZE);
		unmap_sysmem(buf);
		return -CMD_RET_FAILURE;
	}

	/* Get RMA data */
	memset(rma_data, 0, sizeof(rma_data));
	memcpy(rma_data, buf, real_size);

	/* Unmap RMA data memory */
	unmap_sysmem(buf);

#if RMA_OS_DEBUG_ENABLE
	/* Debug print RMA data */
	int index;

	printf("[Debug] RMA Data:\n");
	for (index = 0; index < real_size; index++) {
		if ((index % 16) == 0)
			printf("\n");
		printf("%02x ", rma_data[index]);
	}
	printf("\nPrint data finished!\n");
#endif

	/* Find TEE device */
	dev = tee_find_device(NULL, NULL, NULL, NULL);
	if (!dev) {
		printf("tee_find_device failed");
		return -CMD_RET_FAILURE;
	}

	/* Open TEE session */
	memset(&open_arg, 0, sizeof(open_arg));
	tee_optee_ta_uuid_to_octets(open_arg.uuid, &uuid);
	ret = tee_open_session(dev, &open_arg, 0, NULL);
	if (ret) {
		printf("tee_open_session failed, ret: %#x\n", ret);
		return CMD_RET_FAILURE;
	}
	if (open_arg.ret) {
		printf("tee_open_session failed, ret: %#x, origin: %#x\n",
				open_arg.ret, open_arg.ret_origin);
		return CMD_RET_FAILURE;
	}

	/* Alloc share memory for RMA data */
	memset(&params, 0, sizeof(params));
	params.attr = TEE_PARAM_ATTR_TYPE_MEMREF_INPUT;
	params.u.memref.size = real_size;
	ret = tee_shm_alloc(dev, real_size, 0, &params.u.memref.shm);
	if (ret) {
		printf("tee_shm_alloc failed, ret: %#x\n", ret);
		tee_close_session(dev, open_arg.session);
		return CMD_RET_FAILURE;
	}
	memcpy(params.u.memref.shm->addr, rma_data, real_size);

	/* Invoke TEE command */
	memset(&invoke_arg, 0, sizeof(invoke_arg));
	invoke_arg.session = open_arg.session;
	invoke_arg.func = CMD_RMA_OS_INIT_DATA;
	ret = tee_invoke_func(dev, &invoke_arg, 1, &params);
	if (ret) {
		printf("tee_invoke_func failed, ret = %#x\n", ret);
		ret = CMD_RET_FAILURE;
		goto exit;
	}
	if (invoke_arg.ret) {
		printf("RMA OS init failed, ret: %#x, origin: %#x\n",
				invoke_arg.ret, invoke_arg.ret_origin);
		ret = CMD_RET_FAILURE;
		goto exit;
	}
	printf("RMA OS init and verify authentication data successfully!\n");

exit:
	tee_shm_free(params.u.memref.shm);
	tee_close_session(dev, open_arg.session);

	return ret;
}

static int do_rma_list(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret = CMD_RET_SUCCESS;
	uint32_t info_size;
	struct tee_param params;
	struct udevice *dev;
	struct tee_open_session_arg open_arg;
	struct tee_invoke_arg invoke_arg;
	const struct tee_optee_ta_uuid uuid = TA_RMA_TEE_OS_UUID;

	/* Find TEE device */
	dev = tee_find_device(NULL, NULL, NULL, NULL);
	if (!dev) {
		printf("call tee_find_device failed");
		return -CMD_RET_FAILURE;
	}

	/* Open TEE session */
	memset(&open_arg, 0, sizeof(open_arg));
	tee_optee_ta_uuid_to_octets(open_arg.uuid, &uuid);
	ret = tee_open_session(dev, &open_arg, 0, NULL);
	if (ret) {
		printf("call tee_open_session failed, ret: %#x\n", ret);
		return CMD_RET_FAILURE;
	}
	if (open_arg.ret) {
		printf("call tee_open_session failed, ret: %#x, origin: %#x\n",
				open_arg.ret, open_arg.ret_origin);
		return CMD_RET_FAILURE;
	}

	/* Alloc share memory for RMA command list */
	memset((void *)rma_info, 0, sizeof(rma_info));
	info_size = sizeof(rma_info);
	memset(&params, 0, sizeof(params));
	params.attr = TEE_PARAM_ATTR_TYPE_MEMREF_INOUT;
	params.u.memref.size = info_size;
	ret = tee_shm_alloc(dev, info_size, 0, &params.u.memref.shm);
	if (ret) {
		printf("tee_shm_alloc failed, ret: %#x\n", ret);
		tee_close_session(dev, open_arg.session);
		return CMD_RET_FAILURE;
	}

	/* Invoke TEE command */
	memset(&invoke_arg, 0, sizeof(invoke_arg));
	invoke_arg.session = open_arg.session;
	invoke_arg.func = CMD_RMA_OS_GET_CMD_LIST;
	ret = tee_invoke_func(dev, &invoke_arg, 1, &params);
	if (ret) {
		printf("tee_invoke_func failed, ret = %#x\n", ret);
		ret = CMD_RET_FAILURE;
		goto exit;
	}
	if (invoke_arg.ret) {
		printf("Get RMA Command List failed, ret: %#x, origin: %#x\n",
					invoke_arg.ret, invoke_arg.ret_origin);
		ret = CMD_RET_FAILURE;
		goto exit;
	}

	/* Dump RMA command list */
	info_size = (params.u.memref.size / sizeof(struct rma_info_output));
	memcpy(rma_info, params.u.memref.shm->addr, params.u.memref.size);
	dump_rma_info(rma_info, info_size, "Command", "Status");

exit:
	tee_shm_free(params.u.memref.shm);
	tee_close_session(dev, open_arg.session);

	return ret;
}

static int do_rma_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret = CMD_RET_SUCCESS;
	int cmd = 0;
	struct tee_param params[4];
	struct udevice *dev;
	struct tee_open_session_arg open_arg;
	struct tee_invoke_arg invoke_arg;
	const struct tee_optee_ta_uuid uuid = TA_RMA_TEE_OS_UUID;

	if (argc != 2) {
		printf("Usage: rma cmd ID, ID is command index\n");
		return CMD_RET_USAGE;
	}

	/* Check RMA command range */
	cmd = simple_strtoul(argv[1], NULL, 10);
	if (cmd <= 0 || cmd > RMA_CMD_MAX_NUM) {
		printf("Command ID: %d invalid! Range: 1~%d\n", cmd, RMA_CMD_MAX_NUM);
		return CMD_RET_USAGE;
	}

	/* Find TEE device */
	dev = tee_find_device(NULL, NULL, NULL, NULL);
	if (!dev) {
		printf("tee_find_device failed");
		return -CMD_RET_FAILURE;
	}

	/* Open TEE session */
	memset(&open_arg, 0, sizeof(open_arg));
	tee_optee_ta_uuid_to_octets(open_arg.uuid, &uuid);
	ret = tee_open_session(dev, &open_arg, 0, NULL);
	if (ret) {
		printf("tee_open_session failed, ret: %#x\n", ret);
		ret = CMD_RET_FAILURE;
		return ret;
	}
	if (open_arg.ret) {
		printf("tee_open_session failed, ret: %#x, origin: %#x\n",
				open_arg.ret, open_arg.ret_origin);
		ret = CMD_RET_FAILURE;
		return ret;
	}

	/* Set TEE invoke params */
	memset(params, 0, sizeof(params));
	params[0].attr = TEE_PARAM_ATTR_TYPE_VALUE_INOUT;
	params[0].u.value.a = cmd;
	params[1].attr = TEE_PARAM_ATTR_TYPE_VALUE_INOUT;
	params[2].attr = TEE_PARAM_ATTR_TYPE_MEMREF_INOUT;
	params[3].attr = TEE_PARAM_ATTR_TYPE_MEMREF_INOUT;

	/* Invoke TEE command */
	memset(&invoke_arg, 0, sizeof(invoke_arg));
	invoke_arg.session = open_arg.session;
	invoke_arg.func = CMD_RMA_OS_RUN_CMD;
	ret = tee_invoke_func(dev, &invoke_arg, 4, params);
	if (ret) {
		printf("tee_invoke_func failed, ret = %#x\n", ret);
		ret = CMD_RET_FAILURE;
		goto exit;
	}
	if (invoke_arg.ret) {
		printf("Run RMA Command %d failed, ret: %#x, origin: %#x\n",
				cmd, invoke_arg.ret, invoke_arg.ret_origin);
		ret = CMD_RET_FAILURE;
		goto exit;
	}
	printf("Run RMA Command %d successfully!\n", cmd);

exit:
	tee_close_session(dev, open_arg.session);

	return ret;
}

static int do_rma_info(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret = CMD_RET_SUCCESS;
	uint32_t info_size;
	struct tee_param params;
	struct udevice *dev;
	struct tee_open_session_arg open_arg;
	struct tee_invoke_arg invoke_arg;
	const struct tee_optee_ta_uuid uuid = TA_RMA_TEE_OS_UUID;

	/* Find TEE device */
	dev = tee_find_device(NULL, NULL, NULL, NULL);
	if (!dev) {
		printf("call tee_find_device failed");
		return -CMD_RET_FAILURE;
	}

	/* Open TEE session */
	memset(&open_arg, 0, sizeof(open_arg));
	tee_optee_ta_uuid_to_octets(open_arg.uuid, &uuid);
	ret = tee_open_session(dev, &open_arg, 0, NULL);
	if (ret) {
		printf("call tee_open_session failed, ret: %#x\n", ret);
		return CMD_RET_FAILURE;
	}
	if (open_arg.ret) {
		printf("call tee_open_session failed, ret: %#x, origin: %#x\n",
				open_arg.ret, open_arg.ret_origin);
		return CMD_RET_FAILURE;
	}

	/* Alloc share memory for RMA OS info */
	memset((void *)rma_info, 0, sizeof(rma_info));
	info_size = sizeof(rma_info);
	memset(&params, 0, sizeof(params));
	params.attr = TEE_PARAM_ATTR_TYPE_MEMREF_INOUT;
	params.u.memref.size = info_size;
	ret = tee_shm_alloc(dev, info_size, 0, &params.u.memref.shm);
	if (ret) {
		printf("tee_shm_alloc failed, ret: %#x\n", ret);
		tee_close_session(dev, open_arg.session);
		return CMD_RET_FAILURE;
	}

	/* Invoke TEE command */
	memset(&invoke_arg, 0, sizeof(invoke_arg));
	invoke_arg.session = open_arg.session;
	invoke_arg.func = CMD_RMA_OS_GET_INFO;
	ret = tee_invoke_func(dev, &invoke_arg, 1, &params);
	if (ret) {
		printf("tee_invoke_func failed, ret = %#x\n", ret);
		ret = CMD_RET_FAILURE;
		goto exit;
	}
	if (invoke_arg.ret) {
		printf("Get RMA OS information failed, ret: %#x, origin: %#x\n",
					invoke_arg.ret, invoke_arg.ret_origin);
		ret = CMD_RET_FAILURE;
		goto exit;
	}

	/* Dump RMA OS information */
	info_size = (params.u.memref.size / sizeof(struct rma_info_output));
	memcpy(rma_info, params.u.memref.shm->addr, params.u.memref.size);
	dump_rma_info(rma_info, info_size, "Information", "Status");

exit:
	tee_shm_free(params.u.memref.shm);
	tee_close_session(dev, open_arg.session);

	return ret;
}

static cmd_tbl_t cmd_rma_sub[] = {
	U_BOOT_CMD_MKENT(init, 2, 0, do_rma_init, "", ""),
	U_BOOT_CMD_MKENT(list, 1, 0, do_rma_list, "", ""),
	U_BOOT_CMD_MKENT(cmd, 2, 0, do_rma_cmd, "", ""),
	U_BOOT_CMD_MKENT(info, 1, 0, do_rma_info, "", ""),
};

static int do_rma(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2)
		return cmd_usage(cmdtp);

	argc--;
	argv++;

	/* Check uboot sub command */
	c = find_cmd_tbl(argv[0], &cmd_rma_sub[0], ARRAY_SIZE(cmd_rma_sub));

	if (c)
		return  c->cmd(cmdtp, flag, argc, argv);
	else
		return cmd_usage(cmdtp);
}

static char rma_help_text[] =
	"\n"
	"[rma info] -Show RMA OS information\n"
	"[rma init] -Init RMA OS and verify authentication data\n"
	"            e.g: rma init ADDR; ADDR is memory address, e.g: 0x1080000\n"
	"[rma list] -Show RMA OS command permission list\n"
	"[rma cmd]  -Run a RMA OS command\n"
	"            e.g: rma cmd ID; ID is command index, e.g: 1,2,3...\n";

U_BOOT_CMD(rma, CONFIG_SYS_MAXARGS, 0, do_rma,
	"RMA OS Function", rma_help_text);

