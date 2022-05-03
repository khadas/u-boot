// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <amlogic/clk_measure.h>
#include <amlogic/media/vout/hdmitx21/hdmitx.h>
#include <amlogic/media/dv/dolby_vision.h>

static unsigned char edid_raw_buf[512] = {0};

static void dump_full_edid(const unsigned char *buf)
{
	int i;
	int blk_no = buf[126] + 1;

	if (blk_no > 4)
		blk_no = 4;
	printf("Dump EDID Rawdata\n");
	for (i = 0; i < blk_no * EDID_BLK_SIZE; i++) {
		printf("%02x", buf[i]);
		if (((i + 1) & 0x1f) == 0)    /* print 32bytes a line */
			printf("\n");
	}
}

static int do_edid(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned char st = 0;
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	memset(edid_raw_buf, 0, ARRAY_SIZE(edid_raw_buf));

	st = hdev->hwop.read_edid(edid_raw_buf);

	if (!st)
		printf("edid read failed\n");

	return st;
}

static int do_rx_det(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned char st = 0;
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	memset(edid_raw_buf, 0, ARRAY_SIZE(edid_raw_buf));

	// read edid raw data
	// current only support read 1 byte edid data
	st = hdev->hwop.read_edid(edid_raw_buf);

	if (st) {
		if (edid_raw_buf[250] == 0xfb && edid_raw_buf[251] == 0x0c) {
			printf("RX is FBC\n");

			// set outputmode ENV
			switch (edid_raw_buf[252] & 0x0f) {
			case 0x0:
				run_command("setenv outputmode 1080p50hz", 0);
				break;
			case 0x1:
				run_command("setenv outputmode 2160p50hz420", 0);
				break;
			case 0x2:
				run_command("setenv outputmode 1080p50hz44410bit", 0);
				break;
			case 0x3:
				run_command("setenv outputmode 2160p50hz42010bit", 0);
				break;
			case 0x4:
				run_command("setenv outputmode 2160p50hz42210bit", 0);
				break;
			case 0x5:
				run_command("setenv outputmode 2160p50hz", 0);
				break;
			default:
				run_command("setenv outputmode 1080p50hz", 0);
				break;
			}

			/*et RX 3D Info*/
			switch ((edid_raw_buf[252] >> 4) & 0x0f) {
			case 0x00:
				run_command("setenv rx_3d_info 0", 0);
				break;
			case 0x01:
				run_command("setenv rx_3d_info 1", 0);
				break;
			case 0x02:
				run_command("setenv rx_3d_info 2", 0);
				break;
			case 0x03:
				run_command("setenv rx_3d_info 3", 0);
				break;
			case 0x04:
				run_command("setenv rx_3d_info 4", 0);
				break;
			default:
				break;
			}

			switch (edid_raw_buf[253]) {
			case 0x1:
				/*TODO*/
				break;
			case 0x2:
				/*TODO*/
				break;
			default:
				break;
			}
		}
	} else {
		printf("edid read failed\n");
	}

	return st;
}

static void hdmitx_mask_rx_info(struct hdmitx_dev *hdev)
{
	if (env_get("colorattribute"))
		hdmitx21_get_fmtpara(env_get("hdmimode"), env_get("colorattribute"));

	if (!hdev || !hdev->para)
		return;

	/* when current output color depth is 8bit, mask hdr capability */
	/* refer to SWPL-44445 for more detail */
	if (hdev->para->cd == COLORDEPTH_24B)
		memset(&hdev->RXCap.hdr_info, 0, sizeof(struct hdr_info));
}

static int do_output(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	if (argc < 1)
		return cmd_usage(cmdtp);

	if (strcmp(argv[1], "list") == 0) {
		hdev->hwop.list_support_modes();
	} else if (strcmp(argv[1], "bist") == 0) {
		unsigned int mode = 0;

		if (strcmp(argv[2], "off") == 0)
			mode = 0;
		else if (strcmp(argv[2], "line") == 0)
			mode = 2;
		else if (strcmp(argv[2], "dot") == 0)
			mode = 3;
		else
			mode = simple_strtoul(argv[2], NULL, 10);
		hdev->hwop.test_bist(mode);
	} else if (strcmp(argv[1], "prbs") == 0) {
		hdev->para->cs = HDMI_COLORSPACE_RGB;
		hdev->para->cd = COLORDEPTH_24B;
		hdev->vic = HDMI_16_1920x1080p60_16x9;
		hdmitx21_set(hdev);
		hdev->hwop.test_prbs();
	} else if (strncmp(argv[1], "div40", 5) == 0) {
		bool div40 = 0;

		if (argv[1][5] == '1')
			div40 = 1;
		hdev->hwop.set_div40(div40);
	} else { /* "output" */
		if (0) { // TODO if (!edid_parsing_ok(hdev)) {
			/* SWPL-34712: if EDID parsing error case, not save env,
			 * only forcely output default mode(480p,RGB,8bit).
			 */
			printf("edid parsing ng, forcely output 480p, rgb,8bit\n");
			hdev->vic = HDMI_3_720x480p60_16x9;
			hdev->para = hdmitx21_get_fmtpara("480p60hz", "rgb,8bit");
			hdev->para->cs = HDMI_COLORSPACE_RGB;
			hdev->para->cd = COLORDEPTH_24B;
			hdmitx21_set(hdev);
			return CMD_RET_SUCCESS;
		}
		if (!env_get("colorattribute"))
			env_set("colorattribute", "444,8bit");
		hdev->para = hdmitx21_get_fmtpara(argv[1], env_get("colorattribute"));
		hdev->vic = hdev->para->timing.vic;
		if (hdev->vic == HDMI_UNKNOWN) {
			/* Not find VIC */
			printf("Not find '%s' mapped VIC\n", argv[1]);
			return CMD_RET_FAILURE;
		}
		if (strstr(argv[1], "hz420"))
			hdev->para->cs = HDMI_COLORSPACE_YUV420;
		/* For RGB444 or YCbCr444 under 6Gbps mode, no deepcolor */
		/* Only 4k50/60 has 420 modes */
		switch (hdev->vic) {
		case HDMI_96_3840x2160p50_16x9:
		case HDMI_97_3840x2160p60_16x9:
		case HDMI_101_4096x2160p50_256x135:
		case HDMI_102_4096x2160p60_256x135:
		case HDMI_106_3840x2160p50_64x27:
		case HDMI_107_3840x2160p60_64x27:
			if (hdev->para->cs == HDMI_COLORSPACE_RGB ||
			    hdev->para->cs == HDMI_COLORSPACE_YUV444) {
				if (hdev->para->cd != COLORDEPTH_24B) {
					printf("vic %d cs %d has no cd %d\n",
						hdev->vic,
						hdev->para->cs,
						hdev->para->cd);
					hdev->para->cd = COLORDEPTH_24B;
					printf("set cd as %d\n", COLORDEPTH_24B);
				}
			}
			break;
		default:
			if (hdev->para->cs == HDMI_COLORSPACE_YUV420) {
				printf("vic %d has no cs %d\n", hdev->vic,
					hdev->para->cs);
				hdev->para->cs = HDMI_COLORSPACE_YUV444;
				printf("set cs as %d\n", HDMI_COLORSPACE_YUV444);
			}
			break;
		}
		printf("set hdmitx VIC = %d CS = %d CD = %d\n",
		       hdev->vic, hdev->para->cs, hdev->para->cd);
		/* currently, hdmi mode is always set, if
		 * mode set abort/exit, need to add return
		 * result of mode setting, so that vout
		 * driver will pass it to kernel, and do
		 * mode setting again when vout init in kernel
		 */
		hdmitx21_set(hdev);
	}
	return CMD_RET_SUCCESS;
}

static int do_clkmsr(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	clk_msr(51);
	clk_msr(59);
	clk_msr(61);
	clk_msr(76);
	clk_msr(77);
	clk_msr(78);
	clk_msr(80);
	clk_msr(81);
	clk_msr(82);
	clk_msr(83);
	clk_msr(219);
	clk_msr(220);
	clk_msr(221);
	clk_msr(222);
	return CMD_RET_SUCCESS;
}

static int do_blank(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	if (argc < 1)
		return cmd_usage(cmdtp);

	if (strcmp(argv[1], "1") == 0)
		hdev->hwop.output_blank(1);
	if (strcmp(argv[1], "0") == 0)
		hdev->hwop.output_blank(0);

	return CMD_RET_SUCCESS;
}

static int do_off(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	hdev->vic = HDMI_UNKNOWN;
	hdev->hwop.turn_off();
	printf("turn off hdmitx\n");
	return 1;
}

static int do_dump(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	hdev->hwop.dump_regs();
	return 1;
}

static int do_reg(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned long addr = 0;
	unsigned int data = 0;

	if (argc < 1)
		return cmd_usage(cmdtp);

	if (strncmp(argv[1], "rh", 2) == 0) {
		addr = strtoul(argv[1] + 2, NULL, 16);
		data = hdmitx21_rd_reg((unsigned int)addr);
		printf("rd[0x%lx] 0x%x\n", addr, data);
	}

	if (strncmp(argv[1], "wh", 2) == 0) {
		addr = strtoul(argv[1] + 2, NULL, 16);
		data = strtoul(argv[2], NULL, 16);
		hdmitx21_wr_reg(addr, data);
		printf("wr[0x%lx] 0x%x\n", addr, data);
	}

	return 1;
}

static int do_info(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();
	struct hdmi_format_para *para = hdev->para;

	printf("%s %d\n", para->timing.name, hdev->vic);
	printf("cd%d cs%d cr%d\n", para->cd, para->cs, para->cr);
	printf("enc_idx %d\n", hdev->enc_idx);
	printf("frac_rate: %d\n", hdev->frac_rate_policy);
	return 1;
}

static int xtochar(int num, char *checksum)
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	if (((hdev->rawedid[num] >> 4) & 0xf) <= 9)
		checksum[0] = ((hdev->rawedid[num] >> 4) & 0xf) + '0';
	else
		checksum[0] = ((hdev->rawedid[num] >> 4) & 0xf) - 10 + 'a';

	if ((hdev->rawedid[num] & 0xf) <= 9)
		checksum[1] = (hdev->rawedid[num] & 0xf) + '0';
	else
		checksum[1] = (hdev->rawedid[num] & 0xf) - 10 + 'a';

	return 0;
}

static void get_parse_edid_data(struct hdmitx_dev *hdev)
{
	char *hdr_priority = env_get("hdr_priority");

	hdev->hwop.read_edid(hdev->rawedid);

	if (1)
		dump_full_edid(hdev->rawedid);

	/* parse edid data */
	hdmi_edid_parsing(hdev->rawedid, &hdev->RXCap);

	/* if hdr_priority is 1, then mark dv_info */
	if (hdr_priority && (strcmp(hdr_priority, "1") == 0)) {
		memset(&hdev->RXCap.dv_info, 0, sizeof(struct dv_info));
		pr_info("hdr_priority: %s and clear dv_info\n", hdr_priority);
	}
}

/* policy process: to find the output mode/attr/dv_type */
void scene_process(struct hdmitx_dev *hdev,
	struct scene_output_info *scene_output_info)
{
	struct input_hdmi_data hdmidata;

	if (!hdev || !scene_output_info)
		return;
	/* 1.read dolby vision mode from prop(maybe need to env) */
	memset(&hdmidata, 0, sizeof(struct input_hdmi_data));
	get_hdmi_data(hdev, &hdmidata);

	/* 2. dolby vision scene process */
	/* only for tv support dv and box enable dv */
	if (is_dolby_enabled() && is_tv_support_dv(hdev)) {
		dolbyvision_scene_process(&hdmidata, scene_output_info);
	} else if (is_dolby_enabled()) {
		/* for enable dolby vision core when
		 * first boot connecting non dv tv
		 * NOTE: let systemcontrol to enable DV core
		 */
		/* scene_output_info->final_dv_type = DOLBY_VISION_ENABLE; */
	} else {
		/* for UI disable dolby vision core and boot keep the status
		 * NOTE: TBD if need to disable DV here
		 */
		/* scene_output_info->final_dv_type = DOLBY_VISION_DISABLE; */
	}
	/* 3.sdr scene process */
	/* decide final display mode and deepcolor */
	if (is_dolby_enabled() && is_tv_support_dv(hdev))
		; /* do nothing */
	else
		sdr_scene_process(&hdmidata, scene_output_info);
}

static int do_get_parse_edid(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();
	unsigned char *edid = hdev->rawedid;
	unsigned char *store_checkvalue;

	memset(edid, 0, EDID_BLK_SIZE * EDID_BLK_NO);
	unsigned int i;
	unsigned int checkvalue[4];
	unsigned int checkvalue1;
	unsigned int checkvalue2;
	char checksum[11];
	unsigned char def_cksum[] = {'0', 'x', '0', '0', '0', '0', '0', '0', '0', '0', '\0'};
	char *hdmimode;
	char *colorattribute;
	char dv_type[2] = {0};
	struct scene_output_info scene_output_info;

	if (!hdev->hwop.get_hpd_state()) {
		printf("HDMI HPD low, no need parse EDID\n");
		return 1;
	}
	memset(&scene_output_info, 0, sizeof(struct scene_output_info));

	get_parse_edid_data(hdev);

	/* check if the tv has changed or anything wrong */
	store_checkvalue = (unsigned char *)env_get("hdmichecksum");
	colorattribute = env_get("colorattribute");
	hdmimode = env_get("hdmimode");
	if (!store_checkvalue)
		store_checkvalue = def_cksum;

	printf("read hdmichecksum: %s, hdmimode: %s, colorattribute: %s\n",
	       store_checkvalue, hdmimode, colorattribute);

	for (i = 0; i < 4; i++) {
		if (('0' <= store_checkvalue[i * 2 + 2]) && (store_checkvalue[i * 2 + 2] <= '9'))
			checkvalue1 = store_checkvalue[i * 2 + 2] - '0';
		else
			checkvalue1 = store_checkvalue[i * 2 + 2] - 'W';
		if (('0' <= store_checkvalue[i * 2 + 3]) && (store_checkvalue[i * 2 + 3] <= '9'))
			checkvalue2 = store_checkvalue[i * 2 + 3] - '0';
		else
			checkvalue2 = store_checkvalue[i * 2 + 3] - 'W';
		checkvalue[i] = checkvalue1 * 16 + checkvalue2;
	}

	if (checkvalue[0] != hdev->rawedid[0x7f]  ||
	    checkvalue[1] != hdev->rawedid[0xff]  ||
	    checkvalue[2] != hdev->rawedid[0x17f] ||
	    checkvalue[3] != hdev->rawedid[0x1ff]) {
		hdev->RXCap.edid_changed = 1;

		checksum[0] = '0';
		checksum[1] = 'x';
		for (i = 0; i < 4; i++)
			xtochar(0x80 * i + 0x7f, &checksum[2 * i + 2]);
		checksum[10] = '\0';
		memcpy(hdev->RXCap.checksum, checksum, 10);
		printf("TV has changed, initial mode: %s  attr: %s now crc: %s\n",
			env_get("hdmimode"), env_get("colorattribute"), checksum);
	} else {
		memcpy(hdev->RXCap.checksum, store_checkvalue, 10);
		printf("TV is same, initial mode is: %s attr: %s, checksum: %s\n",
			env_get("hdmimode"), env_get("colorattribute"),
			hdev->RXCap.checksum);
	}

	if (hdev->RXCap.edid_changed) {
		/* find proper mode if EDID changed */
		scene_process(hdev, &scene_output_info);
		env_set("hdmichecksum", hdev->RXCap.checksum);
		if (edid_parsing_ok(hdev)) {
			/* SWPL-34712: if EDID parsing error case, not save env,
			 * only output default mode(480p,RGB,8bit). after
			 * EDID read OK, systemcontrol will recover the hdmi
			 * mode from env, to avoid keep the default hdmi output
			 */
			env_set("hdmimode", scene_output_info.final_displaymode);
			if (is_hdmi_mode(env_get("outputmode"))) {
				env_set("outputmode",
					scene_output_info.final_displaymode);
			} else if (is_hdmi_mode(env_get("outputmode2"))) {
				env_set("outputmode2",
					scene_output_info.final_displaymode);
			} else if (is_hdmi_mode(env_get("outputmode3"))) {
				env_set("outputmode3",
					scene_output_info.final_displaymode);
			}
			env_set("colorattribute",
			       scene_output_info.final_deepcolor);
			/* if change from DV TV to HDR/SDR TV, don't change
			 * DV status to disabled, as DV core need to be enabled.
			 */
			if (scene_output_info.final_dv_type != get_ubootenv_dv_type() &&
			    scene_output_info.final_dv_type != DOLBY_VISION_DISABLE) {
				sprintf(dv_type, "%d", scene_output_info.final_dv_type);
				env_set("dolby_status", dv_type);
				/* according to the policy of systemcontrol,
				 * if current DV mode is not supported by TV
				 * EDID, DV type maybe changed to one witch
				 * TV support, and need VPP/DV module to
				 * update new DV output mode.
				 */
				printf("update dv_type: %d\n",
				       scene_output_info.final_dv_type);
			}
		}
		printf("update hdmimode: %s\n", env_get("hdmimode"));
		printf("update colorattribute: %s\n", env_get("colorattribute"));
		printf("update hdmichecksum: %s\n", env_get("hdmichecksum"));
	}
	hdev->para = hdmitx21_get_fmtpara(env_get("hdmimode"), env_get("colorattribute"));
	hdev->vic = hdev->para->timing.vic;
	hdmitx_mask_rx_info(hdev);
	return 0;
}

static int do_get_preferred_mode(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();
	const struct hdmi_timing *tp;
	unsigned char *edid = hdev->rawedid;

	struct hdmi_format_para *para;
	char pref_mode[64];
	char color_attr[64];
	char *hdmi_read_edid;

	para = hdmitx21_get_fmtpara("480p60hz", "444,8bit");
	hdmi_read_edid = env_get("hdmi_read_edid");
	if (hdmi_read_edid && (hdmi_read_edid[0] == '0'))
		return 0;

	memset(edid, 0, EDID_BLK_SIZE * EDID_BLK_NO);
	memset(pref_mode, 0, sizeof(pref_mode));
	memset(color_attr, 0, sizeof(color_attr));

	/* If sink is not detected there is a still a good chance it supports proper modes */
	/* 720p is chosen as a safe compromise: supported by most sinks and looks good enough */
	if (!hdev->hwop.get_hpd_state()) {
		para = hdmitx21_get_fmtpara("720p60hz", "444,8bit");
		snprintf(pref_mode, sizeof(pref_mode), "%s", para->sname);
		snprintf(color_attr, sizeof(color_attr), "%s", "rgb,8bit");
		printf("no sink, fallback to %s[%d]\n", para->sname, HDMI_4_1280x720p60_16x9);
		goto bypass_edid_read;
	}

	get_parse_edid_data(hdev);
	tp = hdmitx21_gettiming_from_vic(hdev->RXCap.preferred_mode);
	if (tp)
		para = hdmitx21_get_fmtpara(tp->name, env_get("colorattribute"));

	if (para) {
		sprintf(pref_mode, "preferred_mode %s", para->sname);
		if (hdev->RXCap.pref_colorspace & (1 << 5))
			sprintf(color_attr, "setenv colorattribute %s", "444,8bit");
		else if (hdev->RXCap.pref_colorspace & (1 << 4))
			sprintf(color_attr, "setenv colorattribute %s", "422,8bit");
		else
			sprintf(color_attr, "setenv colorattribute %s", "rgb,8bit");
	} else {
		hdev->RXCap.preferred_mode = HDMI_3_720x480p60_16x9;
		para = hdmitx21_get_fmtpara("480p60hz", "444,8bit");
		sprintf(pref_mode, "setenv hdmimode %s", para->sname);
		sprintf(color_attr, "setenv colorattribute %s", "444,8bit");
	}
	printf("sink preferred_mode is %s[%d]\n", para->sname, hdev->RXCap.preferred_mode);

bypass_edid_read:
	printf("hdr mode is %d\n",
		hdev->RXCap.hdr_info.hdr_sup_eotf_smpte_st_2084);
	printf("dv  mode is ver:%d  len: %x\n", hdev->RXCap.dv_info.ver,
		hdev->RXCap.dv_info.length);
	printf("hdr10+ mode is %d\n",
		hdev->RXCap.hdr10plus_info.application_version);

	return 0;
}

static cmd_tbl_t cmd_hdmi_sub[] = {
	U_BOOT_CMD_MKENT(hpd, 1, 1, do_hpd_detect, "", ""),
	U_BOOT_CMD_MKENT(edid, 3, 1, do_edid, "", ""),
	U_BOOT_CMD_MKENT(rx_det, 1, 1, do_rx_det, "", ""),
	U_BOOT_CMD_MKENT(output, 3, 1, do_output, "", ""),
	U_BOOT_CMD_MKENT(clkmsr, 3, 1, do_clkmsr, "", ""),
	U_BOOT_CMD_MKENT(blank, 3, 1, do_blank, "", ""),
	U_BOOT_CMD_MKENT(off, 1, 1, do_off, "", ""),
	U_BOOT_CMD_MKENT(dump, 1, 1, do_dump, "", ""),
	U_BOOT_CMD_MKENT(info, 1, 1, do_info, "", ""),
	U_BOOT_CMD_MKENT(get_preferred_mode, 1, 1, do_get_preferred_mode, "", ""),
	U_BOOT_CMD_MKENT(reg, 3, 1, do_reg, "", ""),
	U_BOOT_CMD_MKENT(get_parse_edid, 1, 1, do_get_parse_edid, "", ""),
};

static int do_hdmitx(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2)
		return cmd_usage(cmdtp);

	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_hdmi_sub[0], ARRAY_SIZE(cmd_hdmi_sub));

	if (c)
		return  c->cmd(cmdtp, flag, argc, argv);
	else
		return cmd_usage(cmdtp);
}

U_BOOT_CMD(hdmitx, CONFIG_SYS_MAXARGS, 0, do_hdmitx,
	   "HDMITX sub-system",
	"hdmitx version:20200618\n"
	"hdmitx hpd\n"
	"    Detect hdmi rx plug-in\n"
	"hdmitx get_preferred_mode\n"
	"    Read full edid data, parse edid, and get preferred mode\n"
	"hdmitx output [list | FORMAT | bist PATTERN]\n"
	"    list: list support formats\n"
	"    FORMAT can be 720p60/50hz, 1080i60/50hz, 1080p60hz, etc\n"
	"       extend with 8bits/10bits, y444/y422/y420/rgb\n"
	"       such as 2160p60hz,10bits,y420\n"
	"    PATTERN: can be as: line, dot, off, or 1920(width)\n"
	"hdmitx blank [0|1]\n"
	"    1: output blank  0: output normal\n"
	"hdmitx clkmsr\n"
	"    show hdmitx clocks\n"
	"hdmitx off\n"
	"    Turn off hdmitx output\n"
	"hdmitx info\n"
	"    current mode info\n"
	"hdmitx rx_det\n"
	"    Auto detect if RX is FBC and set outputmode\n"
);

struct hdr_info *hdmitx_get_rx_hdr_info(void)
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	return &hdev->RXCap.hdr_info;
}
