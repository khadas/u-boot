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

#include <common.h>
#include <lcd.h>
#include <command.h>
#include <asm/byteorder.h>
#include <malloc.h>
#include <splash.h>
#include <video_fb.h>
#include <video.h>
#include <amlogic/fb.h>

#ifdef CONFIG_AML_HDMITX20
#include <amlogic/hdmi.h>
#endif

int osd_enabled = 0;
/* Graphic Device */
static GraphicDevice *gdev = NULL;

extern void osd_debug(void);
extern void osd_set_log_level(int);
extern void osd_test(void);
extern void osd_enable_hw(u32 index, u32 enable);
extern void osd_set_free_scale_enable_hw(u32 index, u32 enable);
extern int osd_rma_test(u32 osd_index);
extern int get_osd_layer(void);

#ifdef CONFIG_AML_HDMITX20
extern struct hdmitx_dev hdmitx_device;
#endif

static int do_osd_open(cmd_tbl_t *cmdtp, int flag, int argc,
		       char *const argv[])
{
	gdev = video_hw_init(RECT_MODE);
	if (gdev == NULL) {
		printf("Initialize video device failed!\n");
		return 1;
	}
	osd_enabled = 1;
	return 0;
}

static int do_osd_enable(cmd_tbl_t *cmdtp, int flag, int argc,
			char *const argv[])
{
	int index = 0;

	index = get_osd_layer();
	if (index >= 0)
		osd_enable_hw(index, 1);

	return 0;
}

static int do_osd_close(cmd_tbl_t *cmdtp, int flag, int argc,
			char *const argv[])
{

	uint index = 0;

	index = get_osd_layer();

	if (gdev == NULL)
		return 1;

	gdev = NULL;
	if (index >= VIU2_OSD1) {
		osd_enable_hw(VIU2_OSD1, 0);
	} else {
		osd_enable_hw(OSD1, 0);
		osd_enable_hw(OSD2, 0);
		osd_set_free_scale_enable_hw(OSD1, 0);
		osd_set_free_scale_enable_hw(OSD2, 0);
	}

	osd_enabled = 0;

	return 0;
}

static int do_osd_clear(cmd_tbl_t *cmdtp, int flag, int argc,
			char *const argv[])
{
#ifdef CONFIG_OSD_SCALE_ENABLE
	uint index = 0;
	ulong fb_addr;
	ulong fb_len;
#endif
	if (gdev == NULL) {
		printf("Please enable osd device first!\n");
		return 1;
	}

#ifdef CONFIG_OSD_SCALE_ENABLE
	index = get_osd_layer();
	if (index < VIU2_OSD1) {
		fb_addr = (ulong)gdev->frameAdrs;
		fb_len = CANVAS_ALIGNED(gdev->fb_width * gdev->gdfBytesPP) * gdev->fb_height;
	} else {
		fb_addr = (ulong)(gdev->frameAdrs +
			CANVAS_ALIGNED(gdev->fb_width * gdev->gdfBytesPP) * gdev->fb_height);
		fb_len = CANVAS_ALIGNED(gdev->winSizeX * gdev->gdfBytesPP) * gdev->winSizeY;
	}
	memset((void *)fb_addr, 0, fb_len);
	flush_cache(fb_addr, fb_len);
#else
	fb_len = CANVAS_ALIGNED(gdev->winSizeX * gdev->gdfBytesPP) * gdev->winSizeY;
	memset((void *)(long long)(gdev->frameAdrs), 0, fb_len);

	flush_cache(gdev->frameAdrs, fb_len);
#endif
	return 0;
}

static int do_osd_debug(cmd_tbl_t *cmdtp, int flag, int argc,
			char *const argv[])
{
	int ret = 0;
	int level = 0;

	switch (argc) {
	case 1:
		osd_debug();
		break;
	case 2:
		level = simple_strtoul(argv[1], NULL, 10);
		osd_set_log_level(level);
		break;
	default:
		return CMD_RET_USAGE;
	}

	return ret;
}

static int do_osd_test(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret = 0;
	switch (argc) {
	case 1:
		osd_test();
		break;
	case 2:
		osd_rma_test(simple_strtoul(argv[1], NULL, 10));
		break;
	default:
		return CMD_RET_USAGE;
	}
	return ret;
}

static int do_osd_display(cmd_tbl_t *cmdtp, int flag, int argc,
			  char *const argv[])
{
	int ret = 0;
	int x = 0, y = 0;
	ulong addr;

	if (gdev == NULL) {
		printf("do_osd_display, enable osd device first!\n");
		return 1;
	}

	splash_get_pos(&x, &y);

	switch (argc) {
	case 1:		/* use load_addr as default address */
		addr = load_addr;
		break;
	case 2:		/* use argument */
		addr = simple_strtoul(argv[1], NULL, 16);
		break;
	case 4:
		addr = simple_strtoul(argv[1], NULL, 16);
		x = simple_strtoul(argv[2], NULL, 10);
		y = simple_strtoul(argv[3], NULL, 10);
		break;
	default:
		return CMD_RET_USAGE;
	}

	ret = video_display_bitmap((unsigned long)addr, x, y);

	return ret;
}

static int do_osd_set(cmd_tbl_t *cmdtp, int flag, int argc,
			  char *const argv[])
{
	int i;
	long osdID;
	char *str = NULL;
	char *hist_env_key[12] = {"hist_max_min_osd0","hist_spl_val_osd0","hist_spl_pix_cnt_osd0","hist_cheoma_sum_osd0",
	                         "hist_max_min_osd1","hist_spl_val_osd1","hist_spl_pix_cnt_osd1","hist_cheoma_sum_osd1",
							 "hist_max_min_osd2","hist_spl_val_osd2","hist_spl_pix_cnt_osd2","hist_cheoma_sum_osd2"};
	if (argc != 6) {
		return CMD_RET_USAGE;
	}
	osdID = simple_strtol(argv[1], NULL, 10);

	if ((osdID < 0) || (osdID > 2)) {
		printf("=== osdID is wrong. ===\n");
		return 1;
	}

	for (i = osdID * 4; i < (osdID + 1) * 4; i++) {
		str = getenv(hist_env_key[i]);
		if (str) {
			setenv(hist_env_key[i], argv[i%4+2]);
			printf("set %s : %s\n", hist_env_key[i], getenv(hist_env_key[i]));
		}
	}
	return 0;
}

static int do_osd_get(cmd_tbl_t *cmdtp, int flag, int argc,
			  char *const argv[])
{
	int i;
	char *str = NULL;
	char *hist_env_key[12] = {"hist_max_min_osd0","hist_spl_val_osd0","hist_spl_pix_cnt_osd0","hist_cheoma_sum_osd0",
	                         "hist_max_min_osd1","hist_spl_val_osd1","hist_spl_pix_cnt_osd1","hist_cheoma_sum_osd1",
							 "hist_max_min_osd2","hist_spl_val_osd2","hist_spl_pix_cnt_osd2","hist_cheoma_sum_osd2"};
	for (i = 0; i < 12; i++) {
		str = getenv(hist_env_key[i]);
		if (str)
			printf("%s : %s\n", hist_env_key[i], str);
	}

	return 0;
}

static int do_osd_dual_logo(cmd_tbl_t *cmdtp, int flag, int argc,
			    char *const argv[])
{
#ifdef CONFIG_AML_HDMITX20
	int st = 0;

	/* detect hdmi plugin or not */
	st = hdmitx_device.HWOp.get_hpd_state();
	printf("osd: hpd_state=%c\n", st ? '1' : '0');

	if (st) {
		/* hdmi plugin, dual logo display
		 * CONFIG_RECOVERY_DUAL_LOGO is given priority in recovery
		 */
		if (!strncmp(getenv("reboot_mode"), "factory_reset", 13)) {
		#if defined(CONFIG_RECOVERY_DUAL_LOGO)
			run_command(CONFIG_RECOVERY_DUAL_LOGO, 0);
		#else
		#if defined(CONFIG_DUAL_LOGO)
			printf("osd: use dual logo cmd macro in recovery mode\n");
			run_command(CONFIG_DUAL_LOGO, 0);
		#else
			printf("osd: dual logo cmd macro is not defined in recovery mode\n");
		#endif
		#endif
		} else {
		#if defined(CONFIG_DUAL_LOGO)
			run_command(CONFIG_DUAL_LOGO, 0);
		#else
			printf("osd: dual logo cmd macro is not defined\n");
		#endif
		}
	} else {
		/* hdmi plugout, single logo display */
	#if defined(CONFIG_SINGLE_LOGO)
		run_command(CONFIG_SINGLE_LOGO, 0);
	#else
		printf("osd: single logo cmd macro is not defined\n");
	#endif
	}
#else
	printf("osd: no hdmitx_device defined\n");
	#if defined(CONFIG_SINGLE_LOGO)
		run_command(CONFIG_SINGLE_LOGO, 0);
	#else
		printf("osd: single logo cmd macro is not defined\n");
	#endif
#endif

	return 0;
}

static cmd_tbl_t cmd_osd_sub[] = {
	U_BOOT_CMD_MKENT(open, 2, 0, do_osd_open, "", ""),
	U_BOOT_CMD_MKENT(enable, 2, 0, do_osd_enable, "", ""),
	U_BOOT_CMD_MKENT(close, 2, 0, do_osd_close, "", ""),
	U_BOOT_CMD_MKENT(clear, 2, 0, do_osd_clear, "", ""),
	U_BOOT_CMD_MKENT(debug, 2, 0, do_osd_debug, "", ""),
	U_BOOT_CMD_MKENT(test, 2, 0, do_osd_test, "", ""),
	U_BOOT_CMD_MKENT(display, 5, 0, do_osd_display, "", ""),
	U_BOOT_CMD_MKENT(set, 7, 0, do_osd_set, "", ""),
	U_BOOT_CMD_MKENT(get, 2, 0, do_osd_get, "", ""),
	U_BOOT_CMD_MKENT(dual_logo, 2, 0, do_osd_dual_logo, "", ""),
};

static int do_osd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	cmd_tbl_t *c;

	/* Strip off leading 'osd' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_osd_sub[0], ARRAY_SIZE(cmd_osd_sub));

	if (c)
		return  c->cmd(cmdtp, flag, argc, argv);
	else
		return CMD_RET_USAGE;
}

U_BOOT_CMD(
	osd,	7,	1,	do_osd,
	"osd sub-system",
	"open                         - open osd device\n"
	"osd enable                       - enable osd device\n"
	"osd close                        - close osd device\n"
	"osd clear                        - clear osd framebuffer\n"
	"osd debug                        - debug osd device\n"
	"osd test [osdID]                 - test osd device\n"
	"osd display <imageAddr> [x y]    - display image\n"
	"osd set <osdID> <a> <b> <c> <d>  - set Hist GoldenData in env\n"
	"                                        a for hist_max_min\n"
	"                                        b for hist_spl_val\n"
	"                                        c for hist_spl_pix_cnt\n"
	"                                        d for hist_cheoma_sum\n"
	"osd get                          - get Hist GoldenData from env\n"
	"osd dual_logo                    - detect hdmi hpd, then display logo"
);

