/*
* Copyright (C) 2017 Amlogic, Inc. All rights reserved.
* *
This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* *
This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
* *
You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
* *
Description:
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

#define ModPrefix(pre) printf(pre"[def_wi]")
#define debugP(fmt...) //ModPrefix("Dbg"),printf("L%d:", __LINE__),printf(fmt)
#define errorP(fmt...) ModPrefix("Err"), printf("L%d:", __LINE__),printf(fmt)
#define wrnP(fmt...)   ModPrefix("Wrn"), printf(fmt)
#define MsgP(fmt...)   ModPrefix("Msg"), printf(fmt)
#define MaxEnvLen      2048

static int _reserve_env_list_after_defenv(const int reservNum, const char* const reservNameList[])
{
        int ret = 0;
        int index = 0;
        const int MaxReservNum = CONFIG_SYS_MAXARGS - 1;
        const char* valListBuf[MaxReservNum];//store at most 64 envs
        char tmpBuf[MaxEnvLen];

        if (reservNum > MaxReservNum) {
                errorP("max reserved env list num %d < wanted %d\n", MaxReservNum, reservNum);
                return __LINE__;
        }
        //1, cal the total buf size needed to save the envs
        for (index = 0; index < reservNum; ++index)
        {
                const char* cfgEnvKey = reservNameList[index];

                int i = 0;
                const char* envVal = getenv(cfgEnvKey);
                if (!envVal) {valListBuf[index] = NULL; continue;}

                int envLen = MaxEnvLen;
                for (; i < MaxEnvLen; ++i) {
                    if ('\0' == envVal[i]) {
                        envLen = i + 1;//include '\0'
                        break;
                    }
                }
                memcpy(tmpBuf, envVal, envLen);
                tmpBuf[--envLen] = '\0';
                valListBuf[index] = strdup(tmpBuf);
        }

        set_default_env("## defenv_reserve\n");

        for (index = 0; index < reservNum; ++index)
        {
            const char* cfgEnvKey           = reservNameList[index];
            const char* valAftDef           = valListBuf[index];

            if (!valAftDef) continue;

            setenv(cfgEnvKey, valAftDef);
            debugP("set[%s=%s]\n", cfgEnvKey, valAftDef);
            free((char*)valAftDef);
        }

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

U_BOOT_CMD(
   defenv_reserv,       //command name
   CONFIG_SYS_MAXARGS,  //maxargs
   0,                   //repeatable
   do_defenv_reserv,    //command function
   "reserve some specified envs after defaulting env",           //description
   "    argv: defenv_reserve <reserv_en0 reserv_env1 ...> \n"   //usage
   "    - e.g. \n"
   "        defenv_reserve :\n"   //usage
   "               NOT env list , reserv cfg array '_env_args_reserve_' in gxbb_p200.c\n"
   "        defenv_reserve reserv_en0, reserv_env1, ...\n"   //usage
   "               reserve specified envs after defaulting env\n"   //usage
);

