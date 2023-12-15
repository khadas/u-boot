// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2018, Linaro Limited
 */

#include <common.h>
#include <log.h>
#include <tee.h>
#include <linux/types.h>

#include "optee_msg.h"
#include "optee_msg_supplicant.h"
#include "optee_private.h"
#include "optee_smc.h"

static void cmd_shm_alloc(struct udevice *dev, struct optee_msg_arg *arg,
			  void **page_list)
{
	int rc;
	struct tee_shm *shm;
	void *pl;
	u64 ph_ptr;

	arg->ret_origin = TEE_ORIGIN_COMMS;

	if (arg->num_params != 1 ||
	    arg->params[0].attr != OPTEE_MSG_ATTR_TYPE_VALUE_INPUT) {
		arg->ret = TEE_ERROR_BAD_PARAMETERS;
		return;
	}

	rc = __tee_shm_add(dev, 0, NULL, arg->params[0].u.value.b,
			   TEE_SHM_REGISTER | TEE_SHM_ALLOC, &shm);
	if (rc) {
		if (rc == -ENOMEM)
			arg->ret = TEE_ERROR_OUT_OF_MEMORY;
		else
			arg->ret = TEE_ERROR_GENERIC;
		return;
	}

	pl = optee_alloc_and_init_page_list(shm->addr, shm->size, &ph_ptr);
	if (!pl) {
		arg->ret = TEE_ERROR_OUT_OF_MEMORY;
		tee_shm_free(shm);
		return;
	}

	*page_list = pl;
	arg->params[0].attr = OPTEE_MSG_ATTR_TYPE_TMEM_OUTPUT |
			      OPTEE_MSG_ATTR_NONCONTIG;
	arg->params[0].u.tmem.buf_ptr = ph_ptr;
	arg->params[0].u.tmem.size = shm->size;
	arg->params[0].u.tmem.shm_ref = (ulong)shm;
	arg->ret = TEE_SUCCESS;
}

static void cmd_shm_free(struct optee_msg_arg *arg)
{
	arg->ret_origin = TEE_ORIGIN_COMMS;

	if (arg->num_params != 1 ||
	    arg->params[0].attr != OPTEE_MSG_ATTR_TYPE_VALUE_INPUT) {
		arg->ret = TEE_ERROR_BAD_PARAMETERS;
		return;
	}

	tee_shm_free((struct tee_shm *)(ulong)arg->params[0].u.value.b);
	arg->ret = TEE_SUCCESS;
}

#ifdef CONFIG_TEE_UTA
static int get_value(u32 num_params, struct tee_param *params,
		     const u32 idx, struct optee_msg_param_value **value)
{
	int ret = 0;

	if (idx >= num_params)
		return -ENOMEM;

	switch (params[idx].attr & TEE_PARAM_ATTR_TYPE_MASK) {
	case TEE_PARAM_ATTR_TYPE_VALUE_INPUT:
	case TEE_PARAM_ATTR_TYPE_VALUE_OUTPUT:
	case TEE_PARAM_ATTR_TYPE_VALUE_INOUT:
		*value = (void *)&params[idx].u.value.a;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

/* Get parameter allocated by secure world */
static int get_param(u32 num_params, struct tee_param *params,
		     const u32 idx, size_t *sz)
{
	int ret = 0;

	if (idx >= num_params)
		return -ENOMEM;

	switch (params[idx].attr & TEE_PARAM_ATTR_TYPE_MASK) {
	case TEE_PARAM_ATTR_TYPE_MEMREF_INPUT:
	case TEE_PARAM_ATTR_TYPE_MEMREF_OUTPUT:
	case TEE_PARAM_ATTR_TYPE_MEMREF_INOUT:
		*sz = params[idx].u.memref.size;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static uint32_t load_secure_module(struct tee_param *params,
		struct tee_optee_ta_uuid *uuid, size_t *sz)
{
	int i = 0;
	size_t uta_num = 0;
	struct tee_optee_uta *uta_tb = NULL;
	u32 ret = TEE_ERROR_ITEM_NOT_FOUND;

	tee_optee_get_uta(&uta_tb, &uta_num);

	for (i = 0; i < uta_num; i++) {
		if (memcmp((void *)uuid, (void *)&uta_tb[i].uuid, sizeof(uuid)) == 0) {
			if (uta_tb[i].sz > *sz) {
				params[1].u.memref.size = uta_tb[i].sz;
			} else {
				if (*sz < uta_tb[i].sz)
					return TEE_ERROR_OUT_OF_MEMORY;
				memcpy(params[1].u.memref.shm->addr, uta_tb[i].ta_payload,
						uta_tb[i].sz);
			}
			ret = TEE_SUCCESS;
			break;
		}
	}

	return ret;
}

static uint32_t load_ta(struct tee_param *params, u32 num_params)
{
	size_t size = 0;
	struct optee_msg_param_value *val_cmd = NULL;
	struct tee_optee_ta_uuid uuid;

	memset(&uuid, 0, sizeof(uuid));

	if (num_params != 2 || get_value(num_params, params, 0, &val_cmd) ||
	    get_param(num_params, params, 1, &size))
		return TEE_ERROR_BAD_PARAMETERS;

	tee_optee_ta_uuid_from_octets(&uuid, (void *)val_cmd);

	return load_secure_module(params, &uuid, &size);
}
#endif

static uint32_t optee_rpc_cmd(struct optee_msg_arg *arg)
{
	u32 ret = 0;
	struct tee_param params[arg->num_params];

	if (from_msg_param(params, arg->num_params, arg->params))
		return TEE_ERROR_COMMUNICATION;
	switch (arg->cmd) {
	case OPTEE_MSG_RPC_CMD_LOAD_TA:
#ifdef CONFIG_TEE_UTA
		ret = load_ta(params, arg->num_params);
		if (ret != TEE_SUCCESS) {
			debug("TA Not Found...");
			return ret;
		}
#else
		return TEE_ERROR_NOT_IMPLEMENTED;
#endif
		break;
	default:
		return TEE_ERROR_NOT_IMPLEMENTED;
	}

	if (to_msg_param(arg->params, arg->num_params, params))
		ret = TEE_ERROR_BAD_PARAMETERS;
	return ret;
}

void optee_suppl_cmd(struct udevice *dev, struct tee_shm *shm_arg,
		     void **page_list)
{
	struct optee_msg_arg *arg = shm_arg->addr;

	switch (arg->cmd) {
	case OPTEE_MSG_RPC_CMD_SHM_ALLOC:
		cmd_shm_alloc(dev, arg, page_list);
		break;
	case OPTEE_MSG_RPC_CMD_SHM_FREE:
		cmd_shm_free(arg);
		break;
	case OPTEE_MSG_RPC_CMD_FS:
		debug("REE FS storage isn't available\n");
		arg->ret = TEE_ERROR_STORAGE_NOT_AVAILABLE;
		break;
	case OPTEE_MSG_RPC_CMD_RPMB:
		optee_suppl_cmd_rpmb(dev, arg);
		break;
	default:
		arg->ret = optee_rpc_cmd(arg);
	}

	arg->ret_origin = TEE_ORIGIN_COMMS;
}
