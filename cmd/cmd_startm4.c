/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_startdsp.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <command.h>
#include <asm/arch/cpu.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/timer.h>
#include <asm/arch/bl31_apis.h>

#include <serial.h>

static int do_startm4(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t cpu_id;
    uint32_t bin_addr;

    int ret=0;

    if (argc != 3) {
        printf("Plese input M4 boot args:cpu_id, bin_addr!\n");
        return CMD_RET_USAGE;
    }

    cpu_id = simple_strtoul(argv[1], NULL, 16);
    bin_addr = simple_strtoul(argv[2], NULL, 16);

    if (cpu_id >=2)
    {
        printf("Plese input M4 boot cpu_id less than 2! \nThe cpu id 0 is start M4A. \nThe cpu id 1 is start M4B.\n");
        return CMD_RET_USAGE;
    }

    if (bin_addr & 0x3u)
    {
        printf("Plese load the bin in ddr memory address in word(4 bytes) align.\n");
        return CMD_RET_USAGE;
    }

    if (0 == cpu_id)
    {
        printf("M4A is boot, cpu_id:%d\n",cpu_id);
        printf("M4A bin address:0x%x\n",bin_addr);
    }
    else
    {
        printf("M4B is boot, cpu_id:%d\n",cpu_id);
        printf("M4A bin address:0x%x\n",bin_addr);
    }



    start_m4(cpu_id, bin_addr);
    return ret;
}

U_BOOT_CMD(
    startm4, 3, 1, do_startm4,
    "load bin from DDR memory address ",
    "\narg[0]: cmd\n"
    "arg[1]: cpu_id, the cpu id 0 is start M4A, the cpu id 1 is start M4B\n"
    "arg[2]: bin_ddr, load execution bin from ddr memory address\n"
);

