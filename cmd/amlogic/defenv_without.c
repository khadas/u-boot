// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <environment.h>
#include <cli.h>
#include <errno.h>
#include <malloc.h>
#include <linux/stddef.h>
#include <asm/byteorder.h>

static const char*  const temp_for_compile[] = {"__test1","__test2","__test3",NULL};
extern const char * const _env_args_reserve_[0] __attribute__((weak, alias("temp_for_compile")));

#define debugP(fmt...) //printf("dbg[ENV]" fmt)
#define errorP(fmt...) do {printf("ERR[ENV]L%d:", __LINE__); printf(fmt); } while (0)
#define wrnP(fmt...)   printf("WRN[ENV]" fmt)
#define MsgP(fmt...)   printf("MSG[ENV]" fmt)

static int _reserve_env_list_after_defenv(const int reservNum, const char* const reservNameList[])
{
        int ret = 0;
        int index = 0;
        unsigned sumOfEnvVal = 0;//sum of strlen(getenv(env_i))
        const int MaxReservNum = CONFIG_SYS_MAXARGS - 1;
        const char* valListBuf[MaxReservNum];//store at most 64 envs
        char* tmpEnvBuf = NULL;

        if (reservNum > MaxReservNum) {
                errorP("max reserved env list num %d < wanted %d\n", MaxReservNum, reservNum);
                return __LINE__;
        }
        //1, cal the total buf size needed to save the envs
        for (index = 0; index < reservNum; ++index)
        {
                const char* cfgEnvKey = reservNameList[index];
                const char* cfgEnvVal = env_get(cfgEnvKey);

                if (cfgEnvVal) {
                        sumOfEnvVal += strlen(cfgEnvVal) + 1;
                }
                valListBuf[index] = cfgEnvVal;
        }

        //2, transfer the env values to buffer
        if (sumOfEnvVal)
        {
                tmpEnvBuf = (char*)malloc(sumOfEnvVal);
                if (!tmpEnvBuf) {
                        errorP("Fail in malloc(%d)\n", sumOfEnvVal);
                        return __LINE__;
                }
                memset(tmpEnvBuf, 0, sumOfEnvVal);

                char* tmpbuf    = tmpEnvBuf;
                for (index = 0; index < reservNum; ++index )
                {
                        const char*    valBeforeDef     = valListBuf[index];

                        if (!valBeforeDef) continue;

                        const unsigned thisValLen       = strlen(valBeforeDef) + 1;
                        memcpy(tmpbuf, valBeforeDef, thisValLen);
                        valListBuf[index] = tmpbuf;
                        tmpbuf += thisValLen ;
                        debugP("tmpEnvBuf=%p, tmpbuf=%p, thisValLen=%d\n", tmpEnvBuf, tmpbuf, thisValLen);
                        debugP("cp:k[%s]%s-->%s\n", reservNameList[index], valBeforeDef, tmpEnvBuf);
                }
        }

        set_default_env("## defenv_reserve ##", 0);

        if (sumOfEnvVal)
        {
                for (index = 0; index < reservNum; ++index)
                {
                        const char* cfgEnvKey           = reservNameList[index];
                        const char* valAftDef           = valListBuf[index];

                        if (valAftDef)
                        {
                                env_set(cfgEnvKey, valAftDef);
                                debugP("set[%s=%s]\n", cfgEnvKey, valAftDef);
                        }
                }
        }

        if (tmpEnvBuf) free(tmpEnvBuf) ;
        return ret;
}

static int do_defenv_reserv(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
        int envListNum = argc - 1;
        const char** envListArr = (const char**)(argv + 1);

        if (!envListNum)
        {
                envListArr = (const char**)_env_args_reserve_;

                const char** pArr = (const char**)envListArr;
                while (*pArr++) ++envListNum;
        }

        int ret = _reserve_env_list_after_defenv(envListNum, envListArr);

        return ret ? CMD_RET_FAILURE : CMD_RET_SUCCESS;
}

U_BOOT_CMD_COMPLETE(
	defenv_reserve,		//command name
   CONFIG_SYS_MAXARGS,  //maxargs
   0,                   //repeatable
   do_defenv_reserv,    //command function
   "reserve some specified envs after defaulting env",           //description
   "    argv: defenv_reserve <reserv_en0 reserv_env1 ...> \n"   //usage
   "    - e.g. \n"
   "        defenv_reserve :\n"   //usage
   "               NOT env list , reserv cfg array '_env_args_reserve_' in gxbb_p200.c\n"
   "        defenv_reserve reserv_en0, reserv_env1, ...\n"   //usage
   "               reserve specified envs after defaulting env\n",   //usage
   var_complete
);

/*
 * update_env_part, depends on env export/import and saveenv
 * flow:
 *	1> save argv env list value to malloc memory
 *	2> back env with env export
 *	3> load env from flash(env part) to memory
 *	4> modify env list with saved value list
 *	5> saveenv to update the env part
 *	6> restore env with env import
 *	7> free the malloc memory
 */
#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
static int do_update_env_part(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i = 0;
	int ret = CMD_RET_SUCCESS;
	int silent	= 0;//don't tell empty env, 0 will print which env isnot exist
	int force	= 0;//when 1, even no all env list exist will update env part
	int print	= 0;//print env name/value after update env part
	int allEnvEmpty = 1;
	int need_update_env = 0;
	const int MAX_ENV_PART = CONFIG_ENV_SIZE;
	const int UPDATE_ENV_SZ = CONFIG_ENV_SIZE;//env export -b will always need CONFIG_ENV_SIZE
	const int BUF_SZ = MAX_ENV_PART + UPDATE_ENV_SZ;
	char *env_part_buf = NULL;
	char *env_val_buf  = NULL;
	char cmd_buf[128];

	if (argc < 2) {
		MsgP("Need at least one env specify to update\n");
		return CMD_RET_USAGE;
	}
	while (argc > 1 && **(argv + 1) == '-') {
		char *arg = *++argv;

		--argc;
		while (*++arg) {
			switch (*arg) {
			case 'p':/* print */
				print = true;
				break;
			case 's':/* silent */
				silent = true;
				break;
			case 'f':/* force */
				force = true;
				break;
			default:
				return CMD_RET_USAGE;
			}
		}
	}
	for (i = 1; i < argc; ++i) {
		if (env_get(argv[i])) {
			allEnvEmpty = false;
			continue;
		}
		if (!force) {
			MsgP("env %s NOT exist, so cannot update flash env\n", argv[i]);
			return CMD_RET_FAILURE;
		}
	}
	if (allEnvEmpty) {
		MsgP("All ENVs empty NOT need update flash\n");
		return CMD_RET_FAILURE;
	}
	env_part_buf = malloc(BUF_SZ);
	if (!env_part_buf) {
		errorP("Fail malloc buf sz 0x%x\n", BUF_SZ);
		return CMD_RET_FAILURE;
	}
	env_val_buf = env_part_buf + MAX_ENV_PART;
	memset(env_part_buf, 0, BUF_SZ);

	//#1, backup current total env and specified env list
	MsgP("argc %d, argv %s\n", argc, argv[1]);
	env_set("_update_env_list", argv[1]);
	for (i = 2; i < argc; ++i) {
		if (!env_get(argv[i]))
			continue;
		env_set("_temp_env_", argv[i]);
		run_command("setenv _update_env_list ${_update_env_list} ${_temp_env_}", 0);
	}
	if (!silent)
		MsgP("_update_env_list: %s\n", env_get("_update_env_list"));
	sprintf(cmd_buf, "env export -s %x -t %p ${_update_env_list}", UPDATE_ENV_SZ, env_val_buf);
	MsgP("run cmd: %s\n", cmd_buf);
	if (run_command(cmd_buf, 0)) {
		errorP("Fail in backup env list\n");
		ret = CMD_RET_FAILURE; goto _update_env_part_err;
	}
	//2> back env with env export
	sprintf(cmd_buf, "env export -s %x -t %p", MAX_ENV_PART, env_part_buf);
	MsgP("run cmd: %s\n", cmd_buf);
	if (run_command(cmd_buf, 0)) {
		errorP("Fail in backup env buf, errno %d\n", errno);
		ret = CMD_RET_FAILURE; goto _update_env_part_err;
	}
	if (print)
		run_command("printenv ${_update_env_list}", 0);

	/*3> load env from flash(env part) to memory */
	env_relocate();
	if (!silent)
		MsgP("reloaded env from flash\n");

	//3.1 compare if all env vars are same in flash
	for (i = 1; i < argc && !need_update_env; ++i) {
		char *env_name = argv[i];
		char *flash_env_val = env_get(env_name);
		char *export_val_buf = env_val_buf;
		int env_name_len, input_val_len;

		for (; !need_update_env && export_val_buf; ) {
			debugP("val buf: %s\n", export_val_buf);
			export_val_buf = strstr(export_val_buf, env_name);
			if (!export_val_buf) {
				if (!silent)
					MsgP("skip no val input env[%s]\n", env_name);
				break;
			}
			env_name_len = strlen(env_name);
			export_val_buf += env_name_len;
			if (*export_val_buf == '=') {//found it
				char *input_env_val = export_val_buf + 1;
				char *end_env = strstr(input_env_val, "\n");
				int flash_val_len = 0;

				debugP("env:%s has input val\n", env_name);
				if (!end_env) {
					errorP("invalid env val buf\n");
					ret = CMD_RET_FAILURE; goto _update_env_part_err;
				}
				if (!flash_env_val) {
					MsgP("no env[%s] in flash, need update\n", env_name);
					need_update_env = 1;
					break;
				}
				flash_val_len = strlen(flash_env_val);
				input_val_len = end_env - input_env_val;
				debugP("env val: old %s, new %s\n", flash_env_val, input_env_val);
				if (flash_val_len != input_val_len) {
					MsgP("update env part as env[%s] len diff\n", env_name);
					need_update_env = 1;
				} else if (strncmp(flash_env_val, input_env_val, flash_val_len)) {
					MsgP("update env part as env[%s] val diff\n", env_name);
					need_update_env = 1;
				} else {
					if (!silent)
						MsgP("env[%s]: input val = flash val\n", env_name);
				}

				break;
			}
		}
	}

	if (need_update_env) {
		/*4>modify env list with saved value list */
		sprintf(cmd_buf, "env import -t %p", env_val_buf);
		run_command(cmd_buf, 0);
		if (!silent)
			MsgP("Update env in flash\n");

		/*5> saveenv to update the env part */
		ret = env_save();
		if (ret) {
			errorP("Fail in save env part, ret %d\n", ret);
			ret = CMD_RET_FAILURE; goto _update_env_part_err;
		}
	} else {
		MsgP("All env value is same in flash, NOT need update\n");
	}

	/*6> restore env with env import */
	sprintf(cmd_buf, "env import -d -t %p", env_part_buf);
	ret = run_command(cmd_buf, 0);
	if (ret) {
		errorP("Fail in restore backup env\n");
		ret = CMD_RET_FAILURE; goto _update_env_part_err;
	}

_update_env_part_err:
	free(env_part_buf);
	return ret;
}

U_BOOT_CMD_COMPLETE(update_env_part,       //command name
	CONFIG_SYS_MAXARGS,  //maxargs
	0,                   //repeatable
	do_update_env_part,    //command function
	"update env part with argv list",           //usage
	"    argv: update_env_part <-p> <-f> <-s> env0 env1 env2 ...\n"
	"	-p print, will print env value\n"
	"	-f force, will allow to update env part even some specified env vars not exist\n"
	"	-s silent, least log if specify -s\n"
	"    - e.g.\n"
	"        update_env_part -p lock:\n"
	"               update env var lock if it's diff in flash\n"
	"        update_env_part -f -p lock lock1:\n"
	"               update env var lock if it's diff and even lock1 is not exist\n",
	var_complete
);
#endif //#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)

