// SPDX-License-Identifier:     BSD-3-Clause
/*
 * Copyright (c) 2019 The Fuchsia Authors
 */

#include <common.h>
#include <amlogic/image_check.h>
#include <bootm.h>
#include <command.h>
#include <config.h>
#include <environment.h>
#include <image.h>

#include <zircon/image.h>
#include <zircon/partition.h>
#include <zircon/abr.h>

#include <asm/arch/bl31_apis.h>
#include <asm/arch/secure_apb.h>
#include <linux/compat.h>

size_t img_offset;

static int do_zbi_load(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned long loadaddr;
	const char *type;
	bool force_recovery = false;
	int ret;

	if (argc < 3)
		return 1;

	type = argv[1];
	loadaddr = simple_strtoul(argv[2], NULL, 16);

	/* check recovery mode */
	if (!strcmp(type, "recovery")) {
		force_recovery = true;
	} else if (strcmp(type, "kernel")) {
		fprintf(stderr, "Err zbi_load: unknown image type '%s'", type);
		return 1;
	}

	ret = zircon_abr_img_load((unsigned char *)loadaddr, force_recovery,
				  &img_offset);
	if (ret) {
		fprintf(stderr, "Err zbi_load: failed to load the image\n");
		return 1;
	}

	return 0;
}

#define MAX_ADDR_LEN_HEX 20

static int do_zbi_boot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret;
	unsigned int loadaddr;

	if (argc < 2)
		return 1;

	loadaddr = simple_strtoul(argv[1], NULL, 16);

	char argv0_new[MAX_ADDR_LEN_HEX + 1] = { 0 };
	char *argv_new = (char *)&argv0_new;

	snprintf(argv0_new, sizeof(argv0_new), "%lx", loadaddr + img_offset);

	ret = do_bootm_states(cmdtp, flag, 1, &argv_new,
		BOOTM_STATE_START | BOOTM_STATE_FINDOS | BOOTM_STATE_FINDOTHER |
			BOOTM_STATE_LOADOS | BOOTM_STATE_OS_PREP |
			BOOTM_STATE_OS_FAKE_GO | BOOTM_STATE_OS_GO,
		&images, 1);

	return ret;
}

static int do_zbi_existence_check(cmd_tbl_t *cmdtp, int flag, int argc,
				  char *const argv[])
{
	int i;

	/* check if one of the A/B/R slots has valid ZBI image */
	for (i = 0; i < ARRAY_SIZE(zircon_slot_idx_to_pname); i++) {
		const char *partition = zircon_slot_idx_to_pname[i];

		printf("looking for zbi in partition '%s'\n", partition);
		if (zircon_find_zbi(partition)) {
			printf("found zbi in partition %s\n", partition);
			return 0;
		}
	}

	printf("No ZBI header found\n");
	return 1;
}

U_BOOT_CMD(zbi_load, 3, 0, do_zbi_load,
	   "Read the image from internal flash with actual size",
	   "argv: <imageType> <loadaddr>\n"
	   "\t<imageType>: current support is kernel or recovery\n"
	   "\t<loadaddr>: memory address in hex to store the image\n");

U_BOOT_CMD(zbi_boot, 2, 0, do_zbi_boot, "Boot the ZBI at the specified address",
	   "argv: <addr>\n"
	   "\t<addr>: memory address in hex of the image to boot\n");

U_BOOT_CMD(zbi_existence_check, 1, 0, do_zbi_existence_check,
	   "Check if there exists a ZBI in the A, B, or R slot.", "");
