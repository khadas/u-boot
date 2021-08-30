// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <environment.h>
#include <memalign.h>
#include <errno.h>
#include <amlogic/storage.h>
#include <asm/arch-meson/boot.h>

DECLARE_GLOBAL_DATA_PTR;

extern unsigned int get_romcode_boot_id(void);
extern const char *get_boot_source_str(unsigned int boot_id);

#ifdef CONFIG_CMD_SAVEENV
static int env_storage_save(void)
{
	int boot_id = get_romcode_boot_id();

	if (BOOT_DEVICE_SPI != boot_id && BOOT_DEVICE_SD != boot_id) {
		if (store_get_type() == BOOT_NONE) {
			printf("env_storage: must init before save\n");
			return -ENOENT;
		}
		ALLOC_CACHE_ALIGN_BUFFER(char, buf, CONFIG_ENV_SIZE);

		if (env_export((env_t *)buf)) {
			printf("env_storage: export failed\n");
			return -EINVAL;
		}

		if (store_rsv_write(RSV_ENV, CONFIG_ENV_SIZE, (void *)buf)) {
			printf("env_storage: write failed\n");
			return -EIO;
		}
	} else {
		printf("Current boot device: %s, do not save env to eMMC.\n", get_boot_source_str(get_romcode_boot_id()));
		return -EIO;
	}

	return 0;
}
#endif /* CONFIG_CMD_SAVEENV */

static int env_storage_load(void)
{
	int boot_id = get_romcode_boot_id();

	if (BOOT_DEVICE_SPI != boot_id && BOOT_DEVICE_SD != boot_id) {
		if (store_get_type() == BOOT_NONE) {
			printf("env_storage: must init before load\n");
			return -ENOENT;
		}
		ALLOC_CACHE_ALIGN_BUFFER(char, buf, CONFIG_ENV_SIZE);

		if (store_rsv_read(RSV_ENV, CONFIG_ENV_SIZE, (void *)buf)) {
			set_default_env("!env_storage: read failed", 0);
			return -EIO;
		}
		else if (env_import(buf, 1)) {
			set_default_env("!env_storage: import failed", 0);
			return -EINVAL;
		}
	} else {
		char str[64];

		sprintf(str, "load env from %s", get_boot_source_str(get_romcode_boot_id()));

		set_default_env(str, 0);
	}

	return 0;
}


U_BOOT_ENV_LOCATION(storage) = {
	.location	= ENVL_STORAGE,
	ENV_NAME("STORAGE")
	.load		= env_storage_load,
#ifdef CONFIG_CMD_SAVEENV
	.save		= env_save_ptr(env_storage_save),
#endif
};
