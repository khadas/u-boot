/*
 * (C) Copyright 2024 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_RV1103B_COMMON_H
#define __CONFIG_RV1103B_COMMON_H

#include "rockchip-common.h"

#define COUNTER_FREQUENCY		24000000
#define CONFIG_SYS_MALLOC_LEN		(16 << 20)
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_NS16550_MEM32

#ifdef CONFIG_SUPPORT_USBPLUG
#define CONFIG_SYS_TEXT_BASE		0x00000000
#else
#define CONFIG_SYS_TEXT_BASE		0x00200000
#endif
#define CONFIG_SYS_INIT_SP_ADDR		0x00400000
#define CONFIG_SYS_LOAD_ADDR		0x00008000
#define CONFIG_SYS_BOOTM_LEN		(64 << 20)
#define CONFIG_SYS_SDRAM_BASE		0
#define SDRAM_MAX_SIZE			0x20000000
#define CONFIG_SYS_NONCACHED_MEMORY    (1 << 20)       /* 1 MiB */

/* SPL */
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_TEXT_BASE		0x00000000
#define CONFIG_SPL_MAX_SIZE		0x40000
#define CONFIG_SPL_BSS_START_ADDR	0x001fe000
#define CONFIG_SPL_BSS_MAX_SIZE		0x20000
#define CONFIG_SPL_STACK		0x001fe000

/* secure otp */
#define OTP_UBOOT_ROLLBACK_OFFSET	0x350
#define OTP_UBOOT_ROLLBACK_WORDS	2	/* 64 bits, 2 words */
#define OTP_ALL_ONES_NUM_BITS		32
#define OTP_SECURE_BOOT_ENABLE_ADDR	0x20
#define OTP_SECURE_BOOT_ENABLE_SIZE	1
#define OTP_RSA_HASH_ADDR		0x180
#define OTP_RSA_HASH_SIZE		32

/* MMC/SD IP block */
#define CONFIG_BOUNCE_BUFFER

#ifndef CONFIG_SPL_BUILD
#ifdef CONFIG_OPTEE_CLIENT
#define CONFIG_PRAM			2048
#endif
/* usb mass storage */
#define CONFIG_USB_FUNCTION_MASS_STORAGE
#define CONFIG_ROCKUSB_G_DNL_PID	0x110e

#define CONFIG_LIB_HW_RAND
#define CONFIG_PREBOOT

#ifdef CONFIG_ENV_MEM_LAYOUT
#define ENV_MEM_LAYOUT_SETTINGS		CONFIG_ENV_MEM_LAYOUT_SETTINGS
#else
/*
 *   Image:  0 - 8M
 *  zImage:  8 - 12M
 *     fdt: 12 - 13M
 * ramdisk: 14 ...
 */
#define ENV_MEM_LAYOUT_SETTINGS		\
	"scriptaddr=0x00b00000\0"	\
	"pxefile_addr_r=0x00c00000\0"	\
	"fdt_addr_r=0x00c00000\0"	\
	"kernel_addr_c=0x00808000\0"	\
	"kernel_addr_r=0x00008000\0"	\
	"ramdisk_addr_r=0x000e00000\0"
#endif

#define CONFIG_EXTRA_ENV_SETTINGS	\
	ENV_MEM_LAYOUT_SETTINGS		\
	ROCKCHIP_DEVICE_SETTINGS	\
	RKIMG_DET_BOOTDEV

#undef RKIMG_BOOTCOMMAND
#ifdef CONFIG_FIT_SIGNATURE
#define RKIMG_BOOTCOMMAND		\
	"boot_fit;"
#else
#define RKIMG_BOOTCOMMAND		\
	"boot_fit;"			\
	"boot_android ${devtype} ${devnum};"
#endif

/* Update define for tiny image */
#ifdef CONFIG_ROCKCHIP_IMAGE_TINY
#undef RKIMG_BOOTCOMMAND
#undef RKIMG_DET_BOOTDEV
#undef CONFIG_EXTRA_ENV_SETTINGS
#undef CONFIG_AUTO_COMPLETE
#undef CONFIG_SYS_LONGHELP
#undef CONFIG_ZLIB
#undef CONFIG_GZIP
/* TODO: #define CONFIG_LIB_HW_RAND */

#define RKIMG_BOOTCOMMAND		"boot_fit;"
#define CONFIG_EXTRA_ENV_SETTINGS	ENV_MEM_LAYOUT_SETTINGS
#endif
#endif	/* !CONFIG_SPL_BUILD */

#endif
