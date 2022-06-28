/*
 * SPDX-License-Identifier:     GPL-2.0+
 *
 * Copyright (c) 2022 Wesion Technology Co., Ltd
 */

#ifndef __CONFIGS_KEDGE2_H
#define __CONFIGS_KEDGE2_H

#include <configs/rk3588_common.h>

#ifndef CONFIG_SPL_BUILD

#undef ROCKCHIP_DEVICE_SETTINGS
#define ROCKCHIP_DEVICE_SETTINGS \
		"logo_addr_r=0x07000000\0" \
		"logo_file_dir=/usr/share/fenix/logo\0" \
		"logocmd_mmc=load $devtype $devnum:2 $logo_addr_r $logo_file_dir/$logo_file || load $devtype $devnum:3 $logo_addr_r $logo_file_dir/$logo_file\0" \
		"logocmd_usb=run logocmd_mmc\0" \
		"logocmd=echo Load logo: $logo_file_dir/$logo_file; run logocmd_${devtype}\0" \
		"stdout=serial,vidconsole\0" \
		"stderr=serial,vidconsole\0"

#define CONFIG_SYS_MMC_ENV_DEV		0

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND  "run distro_bootcmd"

#endif
#endif
