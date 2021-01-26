
/*
 * common/cmd_rpmb.c
 *
 * Copyright (C) 2017 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <command.h>
#include <malloc.h>
#include <tee.h>

#define TEE_ERROR_RPMB_AUTH_KEY_PROGRAMMED                 0x7FFFFFFD

#define CMD_RPMB_AUTH_KEY_INIT      0
#define CMD_RPMB_AUTH_KEY_STATE     1

#define TA_RPMB_UUID { 0x0ab5a718, 0xee63, 0x4115, \
	{ 0xa0, 0xad, 0xef, 0x6e, 0xe9, 0x8f, 0xcb, 0xc7 } }

static int do_rpmb_init(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret = 0;
	struct udevice *dev;
	struct tee_open_session_arg open_arg;
	struct tee_invoke_arg invoke_arg;
	const struct tee_optee_ta_uuid uuid = TA_RPMB_UUID;

	dev = tee_find_device(NULL, NULL, NULL, NULL);
	if (!dev) {
		printf("tee_find_device() failed");
		return -ENODEV;
	}

	memset(&open_arg, 0, sizeof(open_arg));
	tee_optee_ta_uuid_to_octets(open_arg.uuid, &uuid);
	ret = tee_open_session(dev, &open_arg, 0, NULL);
	if (ret) {
		printf("tee_open_session() failed, ret = 0x%x\n", ret);
		return ret;
	}
	if (open_arg.ret) {
		printf("tee_open_session() failed, ret = 0x%x\n",
				open_arg.ret, open_arg.ret_origin);
		return open_arg.ret;
	}

	memset(&invoke_arg, 0, sizeof(invoke_arg));
	invoke_arg.session = open_arg.session;
	invoke_arg.func = CMD_RPMB_AUTH_KEY_INIT;

	ret = tee_invoke_func(dev, &invoke_arg, 0, NULL);
	if (ret) {
		printf("tee_invoke_func() failed, ret = 0x%x\n", ret);
		goto exit;
	}
	if (invoke_arg.ret == TEE_SUCCESS) {
		printf("program RPMB auth key success\n");
	} else if (invoke_arg.ret == TEE_ERROR_RPMB_AUTH_KEY_PROGRAMMED) {
		printf("RPMB auth key is programmed already\n");
	} else {
		printf("tee_invoke_func() failed, ret = 0x%x, origin = %d\n",
				invoke_arg.ret, invoke_arg.ret_origin);
		ret = invoke_arg.ret;
		goto exit;
	}

exit:
	tee_close_session(dev, open_arg.session);

	return ret;
}

U_BOOT_CMD(rpmb_init, CONFIG_SYS_MAXARGS, 0, do_rpmb_init,
		"RPMB sub-system",
		"RPMB auth key init\n");

static int do_rpmb_state(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret = 0;
	struct udevice *dev;
	struct tee_open_session_arg open_arg;
	struct tee_invoke_arg invoke_arg;
	const struct tee_optee_ta_uuid uuid = TA_RPMB_UUID;
	struct tee_param param = { .attr = TEE_PARAM_ATTR_TYPE_VALUE_OUTPUT };

	dev = tee_find_device(NULL, NULL, NULL, NULL);
	if (!dev) {
		printf("tee_find_device() failed");
		return -ENODEV;
	}

	memset(&open_arg, 0, sizeof(open_arg));
	tee_optee_ta_uuid_to_octets(open_arg.uuid, &uuid);
	ret = tee_open_session(dev, &open_arg, 0, NULL);
	if (ret) {
		printf("tee_open_session() failed, ret = 0x%x\n", ret);
		return ret;
	}
	if (open_arg.ret) {
		printf("tee_open_session() failed, ret = 0x%x\n",
				open_arg.ret, open_arg.ret_origin);
		return open_arg.ret;
	}

	memset(&invoke_arg, 0, sizeof(invoke_arg));
	invoke_arg.session = open_arg.session;
	invoke_arg.func = CMD_RPMB_AUTH_KEY_STATE;

	ret = tee_invoke_func(dev, &invoke_arg, 1, &param);
	if (ret) {
		printf("tee_invoke_func() failed, ret = 0x%x\n", ret);
		goto exit;
	}
	if (invoke_arg.ret) {
		printf("tee_invoke_func() failed, ret = 0x%x, origin = %d\n",
				invoke_arg.ret, invoke_arg.ret_origin);
		ret = invoke_arg.ret;
		goto exit;
	}

	char *parg = env_get("bootargs");
	if (parg)
	{
		char *buff =malloc(strlen(parg) + 64);
		if (!buff) {
			ret = -ENOMEM;
			goto exit;
		}

		memset(buff + strlen(parg), 0, 64);
		strcpy(buff, parg);
		char *find = strstr(buff, "androidboot.rpmb_state");
		if (!find)
			sprintf(buff,"%s androidboot.rpmb_state=%d", parg, param.u.value.a);
		else
			find[23] = param.u.value.a ? '1':'0';

		//printf("2 bootargs=%s\n",buff);

		env_set("rpmb_state", param.u.value.a?"1":"0"); //need this?
		env_set("bootargs", buff);
		free(buff);
		buff = NULL;
	}

exit:
	tee_close_session(dev, open_arg.session);

	return ret;
}

U_BOOT_CMD(rpmb_state, CONFIG_SYS_MAXARGS, 0, do_rpmb_state,
		"RPMB sub-system",
		"RPMB state\n");


/****************************************************/