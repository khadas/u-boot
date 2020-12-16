/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_rpmb.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <command.h>
#include <malloc.h>
#include <asm/arch/secure_apb.h>

static int do_rpmb_state(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int nReturn = __LINE__;

#define AML_RPMB_STATE (((readl(AO_SEC_GP_CFG7))>>22) & 0x01)

	char *pARG = getenv("bootargs");
	char env_cmd[128] = {0};

	if (pARG)
	{
		char *pFind = strstr(pARG,"androidboot.rpmb_state");
		if (!pFind)
		{
			sprintf(env_cmd,"setenv bootargs ${bootargs} androidboot.rpmb_state=%d",AML_RPMB_STATE);
			run_command(env_cmd,0);
		}
		else
			pFind[23] = AML_RPMB_STATE ? '1':'0';

		setenv("rpmb_state",AML_RPMB_STATE?"1":"0");

		nReturn = 0;
	}
	else
		printf("aml log : internal sys error!\n");

	return nReturn;
}

U_BOOT_CMD(rpmb_state, CONFIG_SYS_MAXARGS, 0, do_rpmb_state,
		"RPMB sub-system",
		"RPMB state\n");


/****************************************************/
