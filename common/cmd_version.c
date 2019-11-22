/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <version.h>
#include <linux/compiler.h>
#ifdef CONFIG_SYS_COREBOOT
#include <asm/arch/sysinfo.h>
#endif

const char __weak version_string[] = U_BOOT_VERSION_STRING;

static int do_version(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	printf("\n%s\n", version_string);
#ifdef CC_VERSION_STRING
	puts(CC_VERSION_STRING "\n");
#endif
#ifdef LD_VERSION_STRING
	puts(LD_VERSION_STRING "\n");
#endif
#ifdef CONFIG_SYS_COREBOOT
	printf("coreboot-%s (%s)\n", lib_sysinfo.version, lib_sysinfo.build);
#endif
	return 0;
}

static int do_bootloadr_version(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char s_version[64];
	strcpy(s_version, U_BOOT_VERSION);
	printf("s_version: %s\n", s_version);
	setenv("bootloader_version", s_version);
	return 0;
}

U_BOOT_CMD(
	get_bootloaderversion,	1,		0,	do_bootloadr_version,
	"print bootloader version",
	""
);

U_BOOT_CMD(
	version,	1,		1,	do_version,
	"print monitor, compiler and linker version",
	""
);
