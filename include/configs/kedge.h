/*
 * (C) Copyright 2018 Wesion Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __KEDGE_H
#define __KEDGE_H

#include <configs/rk3399_common.h>

#ifndef CONFIG_SPL_BUILD
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND "run distro_bootcmd"
#endif

#define CONFIG_MMC_SDHCI_SDMA
#define CONFIG_SYS_MMC_ENV_DEV 0

#define SDRAM_BANK_SIZE			(2UL << 30)
#define CONFIG_MISC_INIT_R
#define CONFIG_SERIAL_TAG
#define CONFIG_ENV_OVERWRITE

#define CONFIG_BMP_16BPP
#define CONFIG_BMP_24BPP
#define CONFIG_BMP_32BPP

#define ROCKCHIP_DEVICE_SETTINGS \
		"stdout=serial,vidconsole\0" \
		"stderr=serial,vidconsole\0"

#endif
