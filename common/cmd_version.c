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
#include <asm/arch/cpu_config.h>

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

#ifdef CONFIG_DDR_VER_BUFF
#include <malloc.h>
#include <libfdt.h>
#include <fdt_support.h>

#define TAG_NAME "\nBootloader version:\n"
char *bootloader_build_message;
#endif
int get_bootloader_build_message(void)
{
#ifdef CONFIG_DDR_VER_BUFF
	static build_messages_t *build_info;
	int index, size_all = 0;

	build_info = (build_messages_t *)(uintptr_t)(DDR_VER_BUFF_BASE_ADDR);
	if (build_info->h.type != PARAM_MESSAGE ||
		build_info->h.version != VERSION_1 ||
		build_info->h.size != sizeof(build_messages_t)) {
		printf("invalid build messages info head!\n");
		return -1;
	}

	for (index = 0; index < 6; index++) {
		uint64_t size = *(uint64_t *)(&build_info->bl2_message_size +
				(uint64_t)index * 2);
		uint64_t addr = *(uint64_t *)(&build_info->bl2_message_addr +
				(uint64_t)index * 2);
		if (size > 0 && size < DDR_VER_BUFF_SIZE &&
				addr > DDR_VER_BUFF_BASE_ADDR &&
				size + addr < DDR_VER_BUFF_BASE_ADDR + DDR_VER_BUFF_SIZE) {
			size_all += *(uint64_t *)(&build_info->bl2_message_size +
					(uint64_t)index * 2);
		}
	}

	if (size_all == 0) {
		printf("no build message.\n");
		return -1;
	}

	bootloader_build_message = (char *)malloc(size_all + 2 + strlen(version_string));
	if (!bootloader_build_message)
		return -1;

	memset((void *)bootloader_build_message, 0x0, size_all);

	memcpy((void *)(uintptr_t)bootloader_build_message,
			(void *)(uintptr_t)build_info->bl2_message_addr,
			size_all);

	memcpy((void *)(uintptr_t)(bootloader_build_message + size_all),
			(void *)"\n", 1);
	size_all += 1;

	memcpy((void *)(uintptr_t)(bootloader_build_message + size_all),
			(void *)version_string,
			strlen(version_string));
	size_all += strlen(version_string);
	memcpy((void *)(uintptr_t)(bootloader_build_message + size_all),
			(void *)"\n", 1);
#endif
	return 0;
}

int fdt_bl_version(void *fdt)
{
#ifdef CONFIG_DDR_VER_BUFF
	int   nodeoffset;
	int   err;

	if (bootloader_build_message) {
		err = fdt_check_header(fdt);
		if (err < 0) {
			printf("fdt_chosen: %s\n", fdt_strerror(err));
			return err;
		}

		/* find or create "/chosen" node. */
		nodeoffset = fdt_find_or_add_subnode(fdt, 0, "chosen");
		if (nodeoffset < 0)
			return nodeoffset;

		err = fdt_setprop(fdt, nodeoffset, "bootloader_build", bootloader_build_message,
				strlen(bootloader_build_message) + 1);
		if (err < 0) {
			printf("WARNING: could not set bl2_build %s.\n",
			       fdt_strerror(err));
			return err;
		}
	}
#endif
	return 0;
}

static int do_bootloader_version(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char s_version[64];
	strcpy(s_version, "01.01.");
	strcat(s_version, U_BOOT_DATE_TIME);
	printf("s_version: %s\n", s_version);
	setenv("bootloader_version", s_version);
	printf("%s\n", version_string);
#ifdef CONFIG_DDR_VER_BUFF
	if (bootloader_build_message) {
		int all_number = strlen(bootloader_build_message);
		int number = printf(TAG_NAME "%s", bootloader_build_message) - strlen(TAG_NAME);

		while (all_number - number > 0) {
			all_number = all_number - number;
			number = printf("%s", bootloader_build_message + number);
		}
	}
#endif
	return 0;
}

U_BOOT_CMD(
	get_bootloaderversion,	1,		0,	do_bootloader_version,
	"print bootloader version",
	""
);

U_BOOT_CMD(
	version,	1,		1,	do_version,
	"print monitor, compiler and linker version",
	""
);
