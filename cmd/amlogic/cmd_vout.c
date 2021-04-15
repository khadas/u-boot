/*
 * (C) Copyright 2012
 * Amlogic. Inc. jets.yan@amlogic.com
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <asm/byteorder.h>
#ifdef CONFIG_AML_VPP
#include <amlogic/media/vpp/vpp.h>
#endif
#include <amlogic/media/vout/aml_vout.h>
#ifdef CONFIG_AML_HDMITX
#include <amlogic/media/vout/hdmitx/hdmitx.h>
#endif

#ifdef CONFIG_AML_CVBS
#include <amlogic/media/vout/aml_cvbs.h>
#endif
#ifdef CONFIG_AML_LCD
#include <amlogic/media/vout/lcd/aml_lcd.h>
#endif

static unsigned int vout_parse_vout_name(char *name)
{
	char *p, *frac_str;
	unsigned int frac = 0;

	p = strchr(name, ',');
	if (!p) {
		frac = 0;
	} else {
		frac_str = p + 1;
		*p = '\0';
		if (strcmp(frac_str, "frac") == 0)
			frac = 1;
	}

	return frac;
}

static int do_vout_list(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_AML_HDMITX
	struct hdmitx_dev *hdmitx_device = hdmitx_get_hdev();
#endif

#ifdef CONFIG_AML_HDMITX
	if (!hdmitx_device) {
		printf("\nerror: hdmitx device is null\n");
	} else {
		printf("\nvalid hdmi mode:\n");
		hdmitx_device->hwop.list_support_modes();
	}
#endif

#ifdef CONFIG_AML_CVBS
	printf("\nvalid cvbs mode:\n");
	cvbs_show_valid_vmode();
#endif

#ifdef CONFIG_AML_LCD
	printf("\nvalid lcd mode:\n");
	aml_lcd_driver_list_support_mode();
#endif

	return CMD_RET_SUCCESS;
}

static int do_vout_output(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *mode;
	unsigned int frac;
#if defined(CONFIG_AML_CVBS) || defined(CONFIG_AML_HDMITX) || defined(CONFIG_AML_LCD)
	unsigned int mux_sel = VIU_MUX_MAX, venc_sel = VIU_MUX_MAX;
#endif
#ifdef CONFIG_AML_HDMITX
	char str[64];
#endif
#ifdef CONFIG_AML_LCD
	unsigned int venc_index;
#endif

	if (argc != 2)
		return CMD_RET_FAILURE;

	mode = (char *)malloc(64 * sizeof(char));
	if (!mode) {
		printf("cmd_vout: mode malloc falied, exit\n");
		return CMD_RET_FAILURE;
	}
	memset(mode, 0, (sizeof(char) * 64));
	sprintf(mode, "%s", argv[1]);
	frac = vout_parse_vout_name(mode);

#ifdef CONFIG_AML_CVBS
	mux_sel = cvbs_outputmode_check(mode, frac);
	venc_sel = mux_sel & 0xf;
	if (venc_sel == VIU_MUX_ENCI) {
		vout_viu_mux(VOUT_VIU1_SEL, mux_sel);
#ifdef CONFIG_AML_VPP
		vpp_matrix_update(VPP_CM_YUV);
#endif
		if (cvbs_set_vmode(mode) == 0) {
			free(mode);
			run_command("setenv vout_init enable", 0);
			return CMD_RET_SUCCESS;
		}
	}
#endif

#ifdef CONFIG_AML_HDMITX
	if (frac == 0) { /* remove frac support in outputmode */
		mux_sel = hdmi_outputmode_check(mode, frac);
		venc_sel = mux_sel & 0xf;
		if (venc_sel < VIU_MUX_MAX) {
			vout_viu_mux(VOUT_VIU1_SEL, mux_sel);
#ifdef CONFIG_AML_VPP
			vpp_matrix_update(VPP_CM_YUV);
#endif
			/* //remove frac support in outputmode
			 *if (frac)
			 *	setenv("frac_rate_policy", "1");
			 *else
			 *	setenv("frac_rate_policy", "0");
			 */
			memset(str, 0, sizeof(str));
			sprintf(str, "hdmitx output %s", mode);
			run_command(str, 0);
			free(mode);
			run_command("setenv vout_init enable", 0);
			return CMD_RET_SUCCESS;
		}
	}
#endif

#ifdef CONFIG_AML_LCD
	mux_sel = aml_lcd_driver_outputmode_check(mode, frac);
	venc_sel = mux_sel & 0xf;
	venc_index = (mux_sel >> 4) & 0xf;
	if (venc_sel == VIU_MUX_ENCL) {
		vout_viu_mux(VOUT_VIU1_SEL, mux_sel);
#ifdef CONFIG_AML_VPP
		vpp_matrix_update(VPP_CM_RGB);
#endif
		aml_lcd_driver_enable(venc_index, mode, frac);
		free(mode);
		run_command("setenv vout_init enable", 0);
		return CMD_RET_SUCCESS;
	}
#endif

	printf("outputmode[%s] is invalid\n", argv[1]);
	do { (void)frac; } while(0);

	free(mode);
	return CMD_RET_FAILURE;
}

static int do_vout2_list(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_AML_HDMITX
	struct hdmitx_dev *hdmitx_device = hdmitx_get_hdev();
#endif

#ifdef CONFIG_AML_HDMITX
	if (!hdmitx_device) {
		printf("\nerror: hdmitx device is null\n");
	} else {
		printf("\nvalid hdmi mode:\n");
		hdmitx_device->hwop.list_support_modes();
	}
#endif

#ifdef CONFIG_AML_CVBS
	printf("\nvalid cvbs mode:\n");
	cvbs_show_valid_vmode();
#endif

#ifdef CONFIG_AML_LCD
	printf("\nvalid lcd mode:\n");
	aml_lcd_driver_list_support_mode();
#endif

	return CMD_RET_SUCCESS;
}

static int do_vout2_output(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *mode;
	unsigned int frac;
#if defined(CONFIG_AML_CVBS) || defined(CONFIG_AML_HDMITX) || defined(CONFIG_AML_LCD)
	unsigned int mux_sel = VIU_MUX_MAX, venc_sel = VIU_MUX_MAX;
#endif
#ifdef CONFIG_AML_HDMITX20
	char str[64];
#endif
#ifdef CONFIG_AML_LCD
	unsigned int venc_index;
#endif

	if (argc != 2)
		return CMD_RET_FAILURE;

	mode = (char *)malloc(64 * sizeof(char));
	if (!mode) {
		printf("cmd_vout: mode malloc falied, exit\n");
		return CMD_RET_FAILURE;
	}
	memset(mode, 0, (sizeof(char) * 64));
	sprintf(mode, "%s", argv[1]);
	frac = vout_parse_vout_name(mode);

#ifdef CONFIG_AML_CVBS
	mux_sel = cvbs_outputmode_check(mode, frac);
	venc_sel = mux_sel & 0xf;
	if (venc_sel == VIU_MUX_ENCI) {
		if (cvbs_set_vmode(mode) == 0) {
			free(mode);
			return CMD_RET_SUCCESS;
		}
	}
#endif

#ifdef CONFIG_AML_HDMITX20
	if (frac == 0) { /* remove frac support in outputmode */
		mux_sel = hdmi_outputmode_check(mode, frac);
		venc_sel = mux_sel & 0xf;
		if (venc_sel < VIU_MUX_MAX) {
			/* //remove frac support in outputmode
			 *if (frac)
			 *	setenv("frac_rate_policy", "1");
			 *else
			 *	setenv("frac_rate_policy", "0");
			 */
			memset(str, 0, sizeof(str));
			sprintf(str, "hdmitx output %s", mode);
			run_command(str, 0);
			free(mode);
			return CMD_RET_SUCCESS;
		}
	}
#endif

#ifdef CONFIG_AML_LCD
	mux_sel = aml_lcd_driver_outputmode_check(mode, frac);
	venc_sel = mux_sel & 0xf;
	venc_index = (mux_sel >> 4) & 0xf;
	if (venc_sel == VIU_MUX_ENCL) {
		aml_lcd_driver_enable(venc_index, mode, frac);
		free(mode);
		return CMD_RET_SUCCESS;
	}
#endif

	printf("outputmode[%s] is invalid\n", argv[1]);
	do { (void)frac; } while(0);

	free(mode);
	return CMD_RET_FAILURE;
}

static int do_vout2_prepare(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *mode;
	unsigned int frac;
#if defined(CONFIG_AML_CVBS) || defined(CONFIG_AML_HDMITX) || defined(CONFIG_AML_LCD)
	unsigned int mux_sel = VIU_MUX_MAX, venc_sel = VIU_MUX_MAX;
#endif
#ifdef CONFIG_AML_LCD
	unsigned int venc_index;
#endif

	if (argc != 2)
		return CMD_RET_FAILURE;

	mode = (char *)malloc(64 * sizeof(char));
	if (!mode) {
		printf("cmd_vout: mode malloc falied, exit\n");
		return CMD_RET_FAILURE;
	}
	memset(mode, 0, (sizeof(char) * 64));
	sprintf(mode, "%s", argv[1]);
	frac = vout_parse_vout_name(mode);

#ifdef CONFIG_AML_CVBS
	mux_sel = cvbs_outputmode_check(mode, frac);
	venc_sel = mux_sel & 0xf;
	if (venc_sel == VIU_MUX_ENCI) {
		vout_viu_mux(VOUT_VIU2_SEL, mux_sel);
#ifdef CONFIG_AML_VPP
		vpp_viu2_matrix_update(VPP_CM_YUV);
#endif
		free(mode);
		return CMD_RET_SUCCESS;
	}
#endif

#ifdef CONFIG_AML_HDMITX20
	mux_sel = hdmi_outputmode_check(mode, frac);
	venc_sel = mux_sel & 0xf;
	if (venc_sel < VIU_MUX_MAX) {
		vout_viu_mux(VOUT_VIU2_SEL, mux_sel);
#ifdef CONFIG_AML_VPP
		vpp_viu2_matrix_update(VPP_CM_YUV);
#endif
		free(mode);
		return CMD_RET_SUCCESS;
	}
#endif

#ifdef CONFIG_AML_LCD
	mux_sel = aml_lcd_driver_outputmode_check(mode, frac);
	venc_sel = mux_sel & 0xf;
	venc_index = (mux_sel >> 4) & 0xf;
	if (venc_sel == VIU_MUX_ENCL) {
		vout_viu_mux(VOUT_VIU2_SEL, mux_sel);
#ifdef CONFIG_AML_VPP
		vpp_viu2_matrix_update(VPP_CM_RGB);
#endif
		aml_lcd_driver_prepare(venc_index, mode, frac);
		free(mode);
		return CMD_RET_SUCCESS;
	}
#endif

	do { (void)frac; } while(0);

	free(mode);
	return CMD_RET_FAILURE;
}

static int do_vout_info(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	vout_vinfo_dump();

	return CMD_RET_SUCCESS;
}

static cmd_tbl_t cmd_vout_sub[] = {
	U_BOOT_CMD_MKENT(list, 1, 1, do_vout_list, "", ""),
	U_BOOT_CMD_MKENT(output, 3, 1, do_vout_output, "", ""),
	U_BOOT_CMD_MKENT(info, 1, 1, do_vout_info, "", ""),
};

static int do_vout(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2)
		return cmd_usage(cmdtp);

	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_vout_sub[0], ARRAY_SIZE(cmd_vout_sub));

	if (c)
		return  c->cmd(cmdtp, flag, argc, argv);
	else
		return cmd_usage(cmdtp);
}

U_BOOT_CMD(vout, CONFIG_SYS_MAXARGS, 1, do_vout,
	"VOUT sub-system",
	"vout [list | output format | info]\n"
	"    list : list for valid video mode names.\n"
	"    format : perfered output video mode\n"
	"    info : dump vinfo\n"
);

static cmd_tbl_t cmd_vout2_sub[] = {
	U_BOOT_CMD_MKENT(list, 1, 1, do_vout2_list, "", ""),
	U_BOOT_CMD_MKENT(prepare, 3, 1, do_vout2_prepare, "", ""),
	U_BOOT_CMD_MKENT(output, 3, 1, do_vout2_output, "", ""),
	U_BOOT_CMD_MKENT(info, 1, 1, do_vout_info, "", ""),
};

static int do_vout2(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2)
		return cmd_usage(cmdtp);

	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_vout2_sub[0], ARRAY_SIZE(cmd_vout2_sub));

	if (c)
		return  c->cmd(cmdtp, flag, argc, argv);
	else
		return cmd_usage(cmdtp);
}

U_BOOT_CMD(vout2, CONFIG_SYS_MAXARGS, 1, do_vout2,
	"VOUT2 sub-system",
	"vout2 [list | prepare format | output format | info]\n"
	"    list : list for valid video mode names.\n"
	"    format : perfered output video mode\n"
	"    info : dump vinfo\n"
);
