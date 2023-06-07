/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __BASE_ENV_H__
#define __BASE_ENV_H__

#include <asm/arch/cpu.h>

#ifdef CONFIG_DTB_BIND_KERNEL	//load dtb from kernel, such as boot partition
#define CONFIG_DTB_LOAD  "imgread dtb ${boot_part} ${dtb_mem_addr}"
#else
#define CONFIG_DTB_LOAD  "imgread dtb _aml_dtb ${dtb_mem_addr}"
#endif//#ifdef CONFIG_DTB_BIND_KERNEL	//load dtb from kernel, such as boot partition

/* args/envs */
#define CONFIG_SYS_MAXARGS  64
#define CONFIG_EXTRA_ENV_SETTINGS_BASE \
	"firstboot=1\0"\
	"upgrade_step=0\0"\
	"loadaddr=0x00020000\0"\
	"os_ident_addr=0x00500000\0"\
	"loadaddr_rtos=0x00001000\0"\
	"loadaddr_kernel=0x03000000\0"\
	"fb_addr=0x00300000\0" \
	"dolby_status=0\0" \
	"dolby_vision_on=0\0" \
	"dv_fw_dir_odm_ext=/odm_ext/firmware/dovi_fw.bin\0" \
	"dv_fw_dir_vendor=/vendor/firmware/dovi_fw.bin\0" \
	"dv_fw_dir=/oem/firmware/dovi_fw.bin\0" \
	"lock=10101000\0"\
	"recovery_offset=0\0"\
	"active_slot=normal\0"\
	"boot_part=boot\0"\
	"vendor_boot_part=vendor_boot\0"\
	"board_logo_part=odm_ext\0" \
	"rollback_flag=0\0"\
	"boot_flag=0\0"\
	"write_boot=0\0"\
	"ddr_size=0B\0"\
	"recovery_mode=false\0"\
	"retry_recovery_times=7\0"\
	"androidboot.dtbo_idx=0\0"\
	"common_dtb_load=" CONFIG_DTB_LOAD "\0"\
	"get_os_type=if store read ${os_ident_addr} ${boot_part} 0 0x1000; then "\
		"os_ident ${os_ident_addr}; fi\0"\
	"fatload_dev=usb\0"\
	"fs_type=""rootfstype=ramfs""\0"\
	"disable_ir=0\0"\
	"upgrade_check_base="\
		"echo recovery_status=${recovery_status};"\
		"if itest.s \"${recovery_status}\" == \"in_progress\"; then "\
			"run init_display;run storeargs; run recovery_from_flash;"\
		"else fi;"\
		"echo upgrade_step=${upgrade_step}; "\
		"if itest ${upgrade_step} == 3; then "\
			"run init_display;run storeargs; run update; fi;"\
		"\0"\
	"storeargs_base="\
		"get_bootloaderversion;" \
		"setenv bootargs ${initargs} otg_device=${otg_device} "\
		"logo=${display_layer},loaded,${fb_addr} "\
		"vout=${outputmode},${vout_init} panel_type=${panel_type} "\
		"hdmitx=${cecconfig},${colorattribute} hdmimode=${hdmimode} "\
		"hdmichecksum=${hdmichecksum} dolby_vision_on=${dolby_vision_on} "\
		"hdr_policy=${hdr_policy} hdr_priority=${hdr_priority} "\
		"frac_rate_policy=${frac_rate_policy} hdmi_read_edid=${hdmi_read_edid} "\
		"cvbsmode=${cvbsmode} "\
		"osd_reverse=${osd_reverse} video_reverse=${video_reverse} "\
		"disable_ir=${disable_ir};"\
		"setenv bootconfig androidboot.selinux=${EnableSelinux} "\
		"androidboot.firstboot=${firstboot} "\
		"androidboot.bootloader=${bootloader_version} "\
		"androidboot.hardware=amlogic "\
		"androidboot.ddr_size=${ddr_size} ;"\
		"\0"\
	"storeboot_base="\
		"run get_os_type;"\
	    "run storage_param;"\
		"if test ${os_type} = rtos; then "\
			"setenv loadaddr ${loadaddr_rtos};"\
			"store read ${loadaddr} ${boot_part} 0 0x400000;"\
			"bootm ${loadaddr};"\
		"else if test ${os_type} = kernel; then "\
			"get_system_as_root_mode;"\
			"echo system_mode in storeboot: ${system_mode};"\
			"echo active_slot in storeboot: ${active_slot};"\
			"if test ${system_mode} = 1; then "\
				"setenv bootargs ${bootargs} ro rootwait skip_initramfs;"\
			"else "\
				"setenv bootconfig ${bootconfig} "\
				"androidboot.force_normal_boot=1;"\
			"fi;"\
			"if test ${active_slot} != normal; then "\
				"setenv bootconfig ${bootconfig} "\
				"androidboot.slot_suffix=${active_slot};"\
			"fi;"\
			"setenv bootconfig ${bootconfig} androidboot.rollback=${rollback_flag};"\
			"if fdt addr ${dtb_mem_addr}; then else "\
				"echo retry common dtb; run common_dtb_load; fi;"\
			"setenv loadaddr ${loadaddr_kernel};"\
			"if imgread kernel ${boot_part} ${loadaddr}; then bootm ${loadaddr}; fi;"\
		"else echo wrong OS format ${os_type}; fi;fi;"\
		"echo try upgrade as booting failure; run update;"\
		"\0" \
	"update_base="\
		/*first usb burning, second sdc_burn, third ext-sd autoscr/recovery*/\
		/*last udisk autoscr/recovery*/\
		"run usb_burning; "\
		"run recovery_from_sdcard;"\
		"run recovery_from_udisk;"\
		"run recovery_from_flash;"\
		"\0"\
	"recovery_from_fat_dev_base="\
		"setenv loadaddr ${loadaddr_kernel};"\
		"if fatload ${fatload_dev} 0 ${loadaddr} aml_autoscript; then "\
			"autoscr ${loadaddr}; fi;"\
		"if fatload ${fatload_dev} 0 ${loadaddr} recovery.img; then "\
			"if fatload ${fatload_dev} 0 ${dtb_mem_addr} dtb.img; then "\
				"echo ${fatload_dev} dtb.img loaded; fi;"\
			"setenv bootargs ${bootargs} ${fs_type};"\
		"bootm ${loadaddr};fi;"\
		"\0"\
	"recovery_from_udisk_base="\
		"setenv fatload_dev usb;"\
		"if usb start 0; then run recovery_from_fat_dev; fi;"\
		"\0"\
	"recovery_from_sdcard_base="\
		"setenv fatload_dev mmc;"\
		"if mmcinfo; then run recovery_from_fat_dev; fi;"\
		"\0"\
	"recovery_from_flash_base="\
		"echo active_slot: ${active_slot};"\
		"setenv loadaddr ${loadaddr_kernel};"\
		"setenv recovery_mode true;"\
		"if test ${active_slot} = normal; then "\
			"setenv bootargs ${bootargs} ${fs_type} aml_dt=${aml_dt} "\
			"recovery_part=${recovery_part} recovery_offset=${recovery_offset};"\
			"if test ${upgrade_step} = 3; then "\
				"if ext4load mmc 1:2 ${dtb_mem_addr} /recovery/dtb.img; then "\
					"echo cache dtb.img loaded; fi;"\
				"if test ${vendor_boot_mode} = true; then "\
				"if imgread kernel ${recovery_part} ${loadaddr} "\
					"${recovery_offset}; then bootm ${loadaddr}; fi;"\
				"else "\
				"if ext4load mmc 1:2 ${loadaddr} /recovery/recovery.img;"\
				"then echo cache recovery.img loaded; bootm ${loadaddr}; fi;"\
				"fi;"\
			"else "\
				"if imgread dtb recovery ${dtb_mem_addr}; then "\
					"else echo restore dtb; run common_dtb_load;"\
				"fi;"\
			"fi;"\
			"if imgread kernel ${recovery_part} ${loadaddr} ${recovery_offset}; then "\
				"bootm ${loadaddr}; fi;"\
		"else "\
			"if fdt addr ${dtb_mem_addr}; then else "\
				"echo retry common dtb; run common_dtb_load; fi;"\
			"if test ${partition_mode} = normal; then "\
			"setenv bootargs ${bootargs} ${fs_type} aml_dt=${aml_dt} "\
			"recovery_part=${recovery_part} recovery_offset=${recovery_offset};"\
			"setenv bootconfig ${bootconfig} "\
			"androidboot.slot_suffix=${active_slot};"\
			"if imgread kernel ${boot_part} ${loadaddr}; then bootm ${loadaddr}; fi;"\
			"else "\
				"if test ${vendor_boot_mode} = true; then "\
				"setenv bootargs ${bootargs} ${fs_type} aml_dt=${aml_dt};"\
				"setenv bootconfig ${bootconfig} "\
					"androidboot.slot_suffix=${active_slot};"\
				"if imgread kernel ${boot_part} ${loadaddr}; then "\
					"bootm ${loadaddr}; fi;"\
				"else "\
				"setenv bootargs ${bootargs} ${fs_type} aml_dt=${aml_dt} "\
				"recovery_part=${recovery_part} "\
				"recovery_offset=${recovery_offset};"\
				"setenv bootconfig ${bootconfig} "\
					"androidboot.slot_suffix=${active_slot};"\
				"if imgread kernel ${recovery_part} ${loadaddr} "\
					"${recovery_offset}; then bootm ${loadaddr}; fi;"\
				"fi;"\
			"fi;"\
		"fi;"\
		"\0"\
	"bcb_cmd_base="\
		"get_avb_mode;"\
		"get_valid_slot;"\
		"if test ${vendor_boot_mode} = true; then "\
			"setenv loadaddr_kernel 0x3000000;"\
			"setenv dtb_mem_addr 0x1000000;"\
		"fi;"\
		"if test ${active_slot} != normal; then "\
			"echo ab mode, read dtb from kernel;"\
			"setenv common_dtb_load ""imgread dtb ${boot_part} ${dtb_mem_addr}"";"\
		"fi;"\
		"\0"\
	"load_bmp_logo_base="\
		"if rdext4pic ${board_logo_part} $loadaddr; then bmp display $logoLoadAddr; " \
		"else if imgread pic logo bootup $loadaddr; then "\
			"bmp display $bootup_offset; fi; fi;" \
		"\0"\
	"init_display_base="\
		"get_rebootmode;"\
		"echo reboot_mode:::: ${reboot_mode};"\
		"if test ${reboot_mode} = quiescent; then "\
			"setenv reboot_mode_android ""quiescent"";"\
			"setenv dolby_status 0;"\
			"setenv dolby_vision_on 0;"\
			"run storeargs;"\
			"setenv bootconfig ${bootconfig} androidboot.quiescent=1;"\
			"osd open;osd clear;"\
		"else if test ${reboot_mode} = recovery_quiescent; then "\
			"setenv reboot_mode_android ""quiescent"";"\
			"setenv dolby_status 0;"\
			"setenv dolby_vision_on 0;"\
			"run storeargs;"\
			"setenv bootconfig ${bootconfig} androidboot.quiescent=1;"\
			"osd open;osd clear;"\
		"else "\
			"setenv reboot_mode_android ""normal"";"\
			"run storeargs;"\
			"hdmitx hpd;hdmitx get_preferred_mode;hdmitx get_parse_edid;hdmitx edid;"\
			"dovi process;watermark_init;osd open;osd clear;run load_bmp_logo;"\
			"bmp scale;vout output ${outputmode};dovi set;dovi pkg;vpp hdrpkt;"\
		"fi;fi;"\
		"\0"\
	"storage_param_base="\
	    "store param;"\
	    "setenv bootargs ${bootargs} ${mtdbootparts}; "\
	    "\0"\
	"cmdline_keys_base="\
		"setenv region_code US;"\
		"if keyman init 0x1234; then "\
			"if keyman read usid ${loadaddr} str; then fi;"\
			"if keyman read region_code ${loadaddr} str; then fi;"\
			"if keyman read mac ${loadaddr} str; then "\
				"setenv bootargs ${bootargs} mac=${mac};"\
				"setenv bootconfig ${bootconfig} androidboot.mac=${mac};"\
			"fi;"\
			"if keyman read deviceid ${loadaddr} str; then "\
			"setenv bootconfig ${bootconfig} androidboot.deviceid=${deviceid};"\
			"fi;"\
		"fi;"\
		"setenv bootconfig ${bootconfig} androidboot.wificountrycode=${region_code};"\
		"setenv bootconfig ${bootconfig} androidboot.serialno=${usid};"\
		"setenv serial ${usid}; setenv serial# ${usid};"\
	    "factory_provision init;"\
		"\0"\

#endif

