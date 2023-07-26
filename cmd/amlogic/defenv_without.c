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
#include <amlogic/storage.h>

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
 * update_env_part, update env in flash
 *  usage: update_env_part <options -f/-s/-p> env1 env2 env3 ...
 *   Just add/update/delete env in flash, not replace all env like saveenv
 *   updaete include any of add/update/delete
 *  Reasons to replace saveenv with update_env_part
 *    >>Usually only save env u need, not include others like bootdelay
 *    >>Most cases, we need update env iff changed, and need speed up as save env to flash cost time
 *    todo: check not arg duplicated in argv
 */
#if CONFIG_IS_ENABLED(AML_UPDATE_ENV)
static int do_update_env_part(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i = 0;
	int ret = CMD_RET_SUCCESS;
	int silent	= 0;//don't tell empty env, 0 will print which env isnot exist
	int force	= 0;//when 1, even no all env list exist will update env part
	int print	= 0;//print env name/value after update env part
	const int BUF_SZ = CONFIG_ENV_SIZE * 2;
	char *env_part_buf = NULL;
	char *new_env_buf = NULL;
	env_t *ep = NULL;
	unsigned char *pdata = NULL;
	uint32_t crc;
	uint32_t n_env_in_flash = 0;//0 if all env not in flash, argc -1 if all env in flash
	int need_update_env = 0;
	const char *env_end = NULL;//current last kv's \0
	unsigned long env_len = 0;
	unsigned int save_buf = 0;//1 if need save new_env_buf

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
			continue;
		}
		if (!force) {//force mode allow update env part even some env not exist
			MsgP("env %s NOT exist and not -f, so cannot update flash env\n", argv[i]);
			return CMD_RET_FAILURE;
		}
	}
	env_part_buf = malloc(BUF_SZ);
	if (!env_part_buf) {
		errorP("Fail malloc buf sz 0x%x\n", BUF_SZ);
		return CMD_RET_FAILURE;
	}
	new_env_buf = env_part_buf + CONFIG_ENV_SIZE;

	//#1> record env list need to update
	debugP("argc %d, argv[1] %s\n", argc, argv[1]);
	env_set("_update_env_list", NULL);
	for (i = 1; i < argc; ++i) {
		env_set("_temp_env_", argv[i]);
		run_command("env set _update_env_list ${_update_env_list} ${_temp_env_}", 0);
	}
	env_set("_temp_env_", NULL);
	if (!silent) //print env list which updated in part env if not same
		MsgP("_update_env_list: %s\n", env_get("_update_env_list"));

	//#2> read env and check if valid
	if (store_get_type() == BOOT_NONE) {
		errorP("env_storage: must init before load\n");
		ret = CMD_RET_FAILURE; goto _update_env_part_err;
	}
	if (store_rsv_read(RSV_ENV, BUF_SZ, env_part_buf)) {
		errorP("fail read env from storage\n");
		ret = CMD_RET_FAILURE; goto _update_env_part_err;
	} else {
		ep = (env_t *)env_part_buf;
		pdata = ep->data;
		memcpy(&crc, &ep->crc, sizeof(crc));
		if (crc32(0, pdata, ENV_SIZE) != crc) {
			errorP("bad CRC in storage, so directly save all env to storage\n");
			ret = env_save();
			goto _update_env_part_err;
		}
	}

	//#3> parse storage env and check if need update, most cases true
	//compare if all env vars are same in flash
	//case 1: usr env and store env both exist, but value not same (include usr env val empty)
	//case 2: usr env not exist on store, but usr env val not empty
	if (!pdata) {
		errorP("err pdata\n");
		ret = CMD_RET_FAILURE; goto _update_env_part_err;
	} else {
		const char *current_kv = (char *)pdata;//env1=val\0
		unsigned int n_same_env = 0;
		const char *kvsep = "=";

		//3.1>get the true end postion
		for (env_end = (char *)pdata; env_end < (char *)pdata + ENV_SIZE;) {
			const char *p =
				env_end + strnlen(env_end, (char *)pdata + ENV_SIZE - env_end);

			if (*p != '\0') {
				errorP("env need end with 0 but %c\n", *p);
				ret = CMD_RET_FAILURE; goto _update_env_part_err;
			}
			if (++p - (char *)pdata >= ENV_SIZE) {
				env_end = --p;
				break;
			}
			if (*p == '\0') {
				env_end = --p;
				break;
			}
			env_end = p;
		}
		env_len = env_end - (char *)pdata;
		if (*env_end || env_len < 1024) {
			errorP("too short env or env end %c err\n", *env_end);
			ret = CMD_RET_FAILURE; goto _update_env_part_err;
		}
		debugP("part env addr 0x%p, end 0x%p, sz %lx\n",
		       env_part_buf, env_end, env_end - env_part_buf);

		//3.2> check if NOT need update flash, which is most used case
		for (; current_kv < env_end; current_kv += strlen(current_kv) + 1) {
			const char *s_env_v = strpbrk(current_kv, kvsep);//storage env value

			if (!s_env_v) {
				errorP("err env in storage, not k=v fmt\n%s\n", current_kv);
				ret = CMD_RET_FAILURE; goto _update_env_part_err;
			}
			++s_env_v;//skip '='
			//case 1, usr val == storage env val, skip
			//case 2, usr val empty, del it if exist in storage
			//case 2, usr val not empty, del it if exist in storage, append new to end
			for (i = 1; i < argc; ++i) {
				const char *usr_env = argv[i];
				const char *usr_env_val = env_get(usr_env);

				if (strncmp(current_kv, usr_env, s_env_v - current_kv - 1))
					continue;
				//Found key
				++n_env_in_flash;
				if (!usr_env_val) {//need delete env in flash
					MsgP("store env %s need remove\n", usr_env);
					need_update_env = 1;
					break;
				}
				if (strcmp(usr_env_val, s_env_v)) {//need modify env in flash
					MsgP("store env %s need modify\n", usr_env);
					need_update_env = 1;
					break;
				}
				debugP("store env %s NOT need update\n", usr_env);
				++n_same_env;
			}
		}
		if (!need_update_env) {//old user env not changed
			need_update_env = n_same_env < argc - 1;
			debugP("usr env num %d, not need update %d\n", argc - 1, n_same_env);
		}
		//if all env not in flash, not need update if all env not exist
		if (n_env_in_flash == 0) {//all env not in flash
			int all_env_empty = 1;//all env not exist in flash, and in memory

			for (i = 1; i < argc && all_env_empty; ++i)
				if (env_get(argv[i]))
					all_env_empty = 0;
			if (all_env_empty)
				if (!silent)
					MsgP("all env not exist in env and flash\n");
			need_update_env = !all_env_empty;
		}
	}

	if (print)
		run_command("printenv ${_update_env_list}", 0);
	env_set("_update_env_list", NULL);
	if (!need_update_env) {
		MsgP("all env NOT need update\n");
		ret = CMD_RET_SUCCESS; goto _update_env_part_err;
	}

	if (!n_env_in_flash) {//append all env to last as all not in env part
		char *new_end = (char *)env_end;
		unsigned int left_len = CONFIG_ENV_SIZE - env_len;

		for (i = 1; i < argc && left_len > 0; ++i) {
			char *usr_env = argv[i];
			char *val = env_get(usr_env);
			int cp_len = 0;

			if (!val)
				continue;
			if (!silent)
				MsgP("append new env %s\n", usr_env);
			cp_len = strlcpy(++new_end, usr_env, left_len);//cp key
			new_end += cp_len, left_len -= cp_len;
			*new_end = '=', --left_len;//cpy '='
			cp_len = strlcpy(++new_end, val, left_len) + 1;//cp val
			new_end += cp_len, left_len -= cp_len;
		}
		if (left_len == CONFIG_ENV_SIZE - env_len) {
			errorP("exception\n");
			ret = CMD_RET_FAILURE; goto _update_env_part_err;
		}
		env_len = CONFIG_ENV_SIZE - left_len;
		env_end = env_part_buf + env_len;

		save_buf = 0; goto _update_env_save_;
	} else {//not all env in flash
	//copy flash env to new buf, and skip env need modify/delete
		const char *current_kv = (char *)pdata;//env1=val\0
		const char *kvsep = "=";
		char *new_kv = new_env_buf + 4; //skip crc
		unsigned int new_env_len = 0;
		int env_need_update = 0;

		memset(new_env_buf, 0, CONFIG_ENV_SIZE);
		save_buf = 1;

		/* 1, if current k=v\0 not in usr input list, just copy it to new buffer
		 * 2, else if in input list but no value, skip it
		 * 3, else if value not change, just copy it to new buffer
		 * 4, else if in the usr input list but value changed, use input key value instead
		 */
		for (; current_kv < env_end; current_kv += strlen(current_kv) + 1) {
			const char *s_env_v = strpbrk(current_kv, kvsep);//storage env value
			const char *next = NULL;//next k=v
			int s_env_v_len = 1;
			unsigned int kv_len = 0;

			if (!s_env_v) {
				errorP("err env in flash, not k=v fmt\n%s\n", current_kv);
				ret = CMD_RET_FAILURE; goto _update_env_part_err;
			}
			++s_env_v;//skip '='
			s_env_v_len = strnlen(s_env_v, env_end - s_env_v);
			next = s_env_v + s_env_v_len + 1;//next k=v\0
			for (i = 1; i < argc; ++i) {
				const char *usr_env = argv[i];

				if (!usr_env)
					continue;//disposed
				if (!strncmp(current_kv, usr_env, s_env_v - current_kv - 1))
					break;
			}
			kv_len = (unsigned long)(next - current_kv);
			if (i == argc) {//this store env not in user input, so not need change
				memcpy(new_kv, current_kv, kv_len);
				new_kv += kv_len;
				new_env_len += kv_len;
			} else {
				const char *usr_env = argv[i];
				const char *usr_env_val = env_get(usr_env);

				if (!usr_env_val) {//2, in the input list but no value
					MsgP("store env %s will removed\n", usr_env);
					env_need_update = 1;
					continue;
				} else if (!strcmp(usr_env_val, s_env_v)) {//in input && not change
					MsgP("store env %s not need changed\n", usr_env);
					memcpy(new_kv, current_kv, kv_len);
					new_kv += kv_len;
					new_env_len += kv_len;
				} else {//4, in the list and value changed
					unsigned int cp_len = kv_len - 1 - s_env_v_len;

					MsgP("store env %s DO need changed\n", usr_env);
					env_need_update = 1;
					memcpy(new_kv, current_kv, cp_len);//copy k=
					new_kv += cp_len;
					new_env_len += cp_len;

					cp_len = strlen(usr_env_val) + 1;
					memcpy(new_kv, usr_env_val, cp_len);//copy k=
					new_kv += cp_len;
					new_env_len += cp_len;
				}
				//argv[i] = NULL;//mark as disposed
				*((char **)argv + i) = NULL;//mark as disposed
			}
		} //end to traverse all flash env

		for (i = 1; i < argc; ++i) {
			const char *usr_env = argv[i];
			char *usr_env_val = usr_env ? env_get(usr_env) : NULL;
			unsigned int left_len = 0;

			if (!usr_env)
				continue;
			if (!usr_env_val) {
				MsgP("new env %s NULL so skip\n", usr_env);
			} else {
				env_need_update = 1;
				left_len = CONFIG_ENV_SIZE - new_env_len - 1;
				snprintf(new_kv, left_len, "%s=%s", usr_env, usr_env_val);
				MsgP("new store env %s\n", new_kv);
				new_kv += strnlen(new_kv, left_len) + 1;
			}
		}
		if (!env_need_update) {
			MsgP("store env same, new env NULL, so not need update\n");
			ret = CMD_RET_SUCCESS; goto _update_env_part_err;
		}
	}

_update_env_save_:
	ep = (env_t *)(env_part_buf + save_buf * CONFIG_ENV_SIZE);
	pdata = ep->data;
	debugP("crc before update 0x%x\n", ep->crc);

	//ep->crc = crc32(0, pdata, ENV_SIZE);//not work...
	crc = crc32(0, pdata, ENV_SIZE);
	memcpy(&ep->crc, &crc, sizeof(crc));
	if (!silent)
		MsgP("new env part crc 0x%x\n", ep->crc);
	if (store_rsv_write(RSV_ENV, CONFIG_ENV_SIZE, ep)) {
		errorP("Fail to update env part\n");
		ret = CMD_RET_FAILURE; goto _update_env_part_err;
	}
	debugP("ok update env addr 0x%p\n", env_part_buf);

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

