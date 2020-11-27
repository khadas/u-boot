/*
 * (C) Copyright 2018
 * Amlogic. Inc.
 *
 * This file is used to prefetch/varify/compare HDCP keys
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
#include <amlogic/media/vout/hdmitx/hdmitx.h>
#include <amlogic/media/dv/dolby_vision.h>
#ifdef CONFIG_AML_LCD
#include <amlogic/media/vout/lcd/aml_lcd.h>
#endif

#ifdef CONFIG_AML_LCD
static unsigned int hdmitx_parse_vout_name(char *name)
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
#endif

static int do_hpd_detect(cmd_tbl_t *cmdtp, int flag, int argc,
	char *const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();
#ifdef CONFIG_AML_LCD
	struct aml_lcd_drv_s *lcd_drv = NULL;
	char *mode;
	unsigned int frac;
#endif
	char* st;
	char* hdmimode;
	char* cvbsmode;
	char* colorattribute;
	int hpd_st = 0;

	st = env_get("hdmitx_hpd_bypass");
	if (st && (strcmp(st[0], "1") == 0)) {
		printf("hdmitx_hpd_bypass detect\n");
		return 0;
	}

#ifdef CONFIG_AML_LCD
	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv) {
		if (lcd_drv->lcd_outputmode_check) {
			mode = (char *)malloc(64 * sizeof(char));
			if (!mode) {
				printf("cmd_hpd: mode malloc falied\n");
				if (lcd_drv->lcd_outputmode_check(mode, 0) == 0)
					return 0;
			} else {
				memset(mode, 0, sizeof(mode));
				sprintf(mode, "%s", getenv("outputmode"));
				frac = hdmitx_parse_vout_name(mode);
				if (lcd_drv->lcd_outputmode_check(mode, frac) == 0) {
					free(mode);
					return 0;
				}
				free(mode);
			}
		}
	}
#endif

	hpd_st = hdev->hwop.get_hpd_state();
	if (!hpd_st) {
		/* For some TV, they cost extra time to pullup HPD after 5V */
		int loop = 10;

		while (loop--) {
			mdelay(100);
			hpd_st = hdev->hwop.get_hpd_state();
			if (hpd_st) {
				printf("hpd delay %d ms\n", (10 - loop) * 100);
				break;
			}
		}
	}
	printf("hpd_state=%d\n", hpd_st);

	/*get hdmi mode and colorattribute from env */
	hdmimode = env_get("hdmimode");
	if (hdmimode)
		printf("do_hpd_detect: hdmimode=%s\n", hdmimode);


	colorattribute = env_get("colorattribute");
	if (colorattribute)
		printf("do_hpd_detect: colorattribute=%s\n", colorattribute);

	if (hdmimode) {
		if (strstr(hdmimode, "null")) {
			env_set("hdmimode", "1080p60hz");
			//run_command("saveenv", 0);
		}
	} else {
		env_set("hdmimode", "1080p60hz");
		//run_command("saveenv", 0);
	}
	hdmimode = env_get("hdmimode");
	if (hpd_st) {
		env_set("outputmode", hdmimode);
	} else {
		cvbsmode = env_get("cvbsmode");
		if (cvbsmode)
			env_set("outputmode", cvbsmode);
		env_set("hdmichecksum", "0x00000000");
		//run_command("saveenv", 0);
	}
	return hpd_st;
}

static unsigned char edid_raw_buf[256] = {0};
static void dump_edid_raw_8bytes(unsigned char *buf)
{
	int i = 0;
	for (i = 0; i < 8; i++)
		printf("%02x ", buf[i]);
	printf("\n");
}

static void dump_full_edid(const unsigned char *buf)
{
	int i;
	int blk_no = buf[126] + 1;

	if (blk_no > 4)
		blk_no = 4;
	printf("Dump EDID Rawdata\n");
	for (i = 0; i < blk_no * EDID_BLK_SIZE; i++) {
		printk("%02x", buf[i]);
		if (((i+1) & 0x1f) == 0)    /* print 32bytes a line */
			printk("\n");
	}
}

static int do_edid(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned int tmp_addr = 0;
	unsigned char edid_addr = 0;
	unsigned char st = 0;
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	memset(edid_raw_buf, 0, ARRAY_SIZE(edid_raw_buf));
	if (argc < 2)
		return cmd_usage(cmdtp);
	if (strcmp(argv[1], "read") == 0) {
		tmp_addr = simple_strtoul(argv[2], NULL, 16);
		if (tmp_addr > 0xff)
			return cmd_usage(cmdtp);
		edid_addr = tmp_addr;
		/* read edid raw data */
		/* current only support read 1 byte edid data */
		st = hdev->hwop.read_edid(
			&edid_raw_buf[edid_addr & 0xf8], edid_addr & 0xf8, 8);
		printf("edid[0x%02x]: 0x%02x\n", edid_addr,
			edid_raw_buf[edid_addr]);
		if (0) /* Debug only */
			dump_edid_raw_8bytes(&edid_raw_buf[edid_addr & 0xf8]);
		if (!st)
			printf("edid read failed\n");
	}
	return st;
}

static int do_rx_det(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	unsigned char edid_addr = 0xf8;     // Fixed Address
	unsigned char st = 0;
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	memset(edid_raw_buf, 0, ARRAY_SIZE(edid_raw_buf));

	// read edid raw data
	// current only support read 1 byte edid data
	st = hdev->hwop.read_edid(&edid_raw_buf[edid_addr & 0xf8], edid_addr & 0xf8, 8);
	if (1)      // Debug only
		dump_edid_raw_8bytes(&edid_raw_buf[edid_addr & 0xf8]);
	if (st) {
#if 0
		// set fake value for debug
		edid_raw_buf[250] = 0xfb;
		edid_raw_buf[251] = 0x0c;
		edid_raw_buf[252] = 0x01;
#endif
		if ((edid_raw_buf[250] == 0xfb) & (edid_raw_buf[251] == 0x0c)) {
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
	} else
		printf("edid read failed\n");

	return st;
}

static int do_output(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();
	if (argc < 1)
		return cmd_usage(cmdtp);

	if (strcmp(argv[1], "list") == 0)
		hdev->hwop.list_support_modes();
	else if (strcmp(argv[1], "bist") == 0) {
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
		hdev->para->cs = HDMI_COLOR_FORMAT_RGB;
		hdev->para->cd = HDMI_COLOR_DEPTH_24B;
		hdev->vic = HDMI_1920x1080p60_16x9;
		hdmi_tx_set(hdev);
		hdev->hwop.test_prbs();
	} else if (strncmp(argv[1], "div40", 5) == 0) {
		bool div40 = 0;

		if (argv[1][5] == '1')
			div40 = 1;
		hdev->hwop.set_div40(div40);
	} else { /* "output" */
		if (!edid_parsing_ok(hdev)) {
			/* SWPL-34712: if EDID parsing error case, not save env,
			 * only forcely output default mode(480p,RGB,8bit).
			 */
			printf("edid parsing ng, forcely output 480p, rgb,8bit\n");
			hdev->vic = HDMI_720x480p60_16x9;
			hdev->para =
				hdmi_get_fmt_paras(hdev->vic);
			hdev->para->cs = HDMI_COLOR_FORMAT_RGB;
			hdev->para->cd = HDMI_COLOR_DEPTH_24B;
			hdmi_tx_set(hdev);
			return CMD_RET_SUCCESS;
		}
		hdev->vic = hdmi_get_fmt_vic(argv[1]);
		hdev->para = hdmi_get_fmt_paras(hdev->vic);
		if (hdev->vic == HDMI_unkown) {
			/* Not find VIC */
			printf("Not find '%s' mapped VIC\n", argv[1]);
			return CMD_RET_FAILURE;
		} else
			printf("set hdmitx VIC = %d\n", hdev->vic);
		if (env_get("colorattribute"))
			hdmi_parse_attr(hdev->para, env_get("colorattribute"));
		if (strstr(argv[1], "hz420") != NULL)
			hdev->para->cs = HDMI_COLOR_FORMAT_420;
		/* For RGB444 or YCbCr444 under 6Gbps mode, no deepcolor */
		/* Only 4k50/60 has 420 modes */
		switch (hdev->vic) {
		case HDMI_3840x2160p50_16x9:
		case HDMI_3840x2160p60_16x9:
		case HDMI_4096x2160p50_256x135:
		case HDMI_4096x2160p60_256x135:
		case HDMI_3840x2160p50_64x27:
		case HDMI_3840x2160p60_64x27:
		case HDMI_3840x2160p50_16x9_Y420:
		case HDMI_3840x2160p60_16x9_Y420:
		case HDMI_4096x2160p50_256x135_Y420:
		case HDMI_4096x2160p60_256x135_Y420:
		case HDMI_3840x2160p50_64x27_Y420:
		case HDMI_3840x2160p60_64x27_Y420:
			if ((hdev->para->cs == HDMI_COLOR_FORMAT_RGB) ||
			    (hdev->para->cs == HDMI_COLOR_FORMAT_444)) {
				if (hdev->para->cd != HDMI_COLOR_DEPTH_24B) {
					printf("vic %d cs %d has no cd %d\n",
						hdev->vic,
						hdev->para->cs,
						hdev->para->cd);
					hdev->para->cd = HDMI_COLOR_DEPTH_24B;
					printf("set cd as %d\n", HDMI_COLOR_DEPTH_24B);
				}
			}
			if (hdev->para->cs == HDMI_COLOR_FORMAT_420)
				hdev->vic |= HDMITX_VIC420_OFFSET;
			break;
		default:
			if (hdev->para->cs == HDMI_COLOR_FORMAT_420) {
				printf("vic %d has no cs %d\n", hdev->vic,
					hdev->para->cs);
				hdev->para->cs = HDMI_COLOR_FORMAT_444;
				printf("set cs as %d\n", HDMI_COLOR_FORMAT_444);
			}
			/* For VESA modes, should be RGB format */
			if (hdev->vic >= HDMITX_VESA_OFFSET) {
				hdev->para->cs = HDMI_COLOR_FORMAT_RGB;
				hdev->para->cd = HDMI_COLOR_DEPTH_24B;
			}
			break;
		}
		printf("set hdmitx VIC = %d CS = %d CD = %d\n",
		       hdev->vic, hdev->para->cs, hdev->para->cd);
		hdmi_tx_set(hdev);
	}
	return CMD_RET_SUCCESS;
}

static int do_blank(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	if (argc < 1)
		return cmd_usage(cmdtp);

	if (strcmp(argv[1], "1") == 0)
		hdev->hwop.output_blank(1);
	if (strcmp(argv[1], "0") == 0)
		hdev->hwop.output_blank(0);

	return CMD_RET_SUCCESS;
}

static int do_off(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	hdev->vic = HDMI_unkown;
	hdev->hwop.turn_off();
	printf("turn off hdmitx\n");
	return 1;
}

static int do_dump(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	hdev->hwop.dump_regs();
	return 1;
}

static int do_reg(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	unsigned long addr = 0;
	unsigned int data = 0;

	if (argc < 1)
		return cmd_usage(cmdtp);

	if (strncmp(argv[1], "rh", 2) == 0) {
		addr = strtoul(argv[1] + 2, NULL, 16);
		data = hdmitx_rd_reg((unsigned int)addr);
		printk("rd[0x%lx] 0x%x\n", addr, data);
	}

	if (strncmp(argv[1], "wh", 2) == 0) {
		addr = strtoul(argv[1] + 2, NULL, 16);
		data = strtoul(argv[2], NULL, 16);
		hdmitx_wr_reg(addr, data);
		printk("wr[0x%lx] 0x%x\n", addr, data);
	}

	return 1;
}

static int do_info(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();
	struct hdmi_format_para *para = hdev->para;

	printf("%s %d\n", para->ext_name, hdev->vic);
	printf("cd%d cs%d cr%d\n", para->cd, para->cs, para->cr);
	printf("frac_rate: %d\n", hdev->frac_rate_policy);
	return 1;
}

static int xtochar(int num, char* checksum)
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	if (((hdev->rawedid[num]  >> 4 ) & 0xf) <= 9)
		checksum[0] = ((hdev->rawedid[num]  >> 4 ) & 0xf) + '0';
	else
		checksum[0] = ((hdev->rawedid[num]  >> 4 ) & 0xf) -10 + 'a';

	if ((hdev->rawedid[num] & 0xf) <= 9)
		checksum[1] = (hdev->rawedid[num] & 0xf) + '0';
	else
		checksum[1] = (hdev->rawedid[num] & 0xf) -10 + 'a';

	return 0;
}

static void get_parse_edid_data(struct hdmitx_dev *hdev)
{
	unsigned char *edid = hdev->rawedid;
	unsigned int byte_num = 0;
	unsigned char blk_no = 1;
	char *hdr_priority = env_get("hdr_priority");

	/* get edid data */
	while (byte_num < 128 * blk_no) {
		hdev->hwop.read_edid(&edid[byte_num], byte_num & 0x7f, byte_num / 128);
		if (byte_num == 120) {
			blk_no = edid[126] + 1;
			if (blk_no > 4)
				blk_no = 4; /* MAX Read Blocks 4 */
		}
		byte_num += 8;
	}

	if (0)
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
	scene_output_info_t *scene_output_info)
{
	hdmi_data_t hdmidata;

	if (!hdev || !scene_output_info)
		return;
	/* 1.read dolby vision mode from prop(maybe need to env) */
	memset(&hdmidata, 0, sizeof(hdmi_data_t));
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
	if (is_dolby_enabled() && is_tv_support_dv(hdev)) {
		/* do nothing */
	} else {
		sdr_scene_process(&hdmidata, scene_output_info);
	}
}

static int do_get_parse_edid(cmd_tbl_t * cmdtp, int flag, int argc,
	char * const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();
	unsigned char *edid = hdev->rawedid;
	unsigned char *store_checkvalue;
	memset(edid, 0, EDID_BLK_SIZE * EDID_BLK_NO);
	unsigned int i;
	unsigned int checkvalue[4];
	unsigned int checkvalue1;
	unsigned int checkvalue2;
	char checksum[11];
	char* hdmimode;
	char* colorattribute;
	char dv_type[2] = {0};
	scene_output_info_t scene_output_info;

	if (!hdev->hwop.get_hpd_state()) {
		printf("HDMI HPD low, no need parse EDID\n");
		return 1;
	}
	memset(&scene_output_info, 0, sizeof(scene_output_info_t));

	get_parse_edid_data(hdev);

	/* check if the tv has changed or anything wrong */
	store_checkvalue = (unsigned char*)env_get("hdmichecksum");
	colorattribute = env_get("colorattribute");
	hdmimode = env_get("hdmimode");

	printf("read hdmichecksum: %s, hdmimode: %s, colorattribute: %s\n",
	       store_checkvalue, hdmimode, colorattribute);

	for (i = 0; i < 4; i++) {
		if (('0' <= store_checkvalue[i * 2 + 2]) && (store_checkvalue[i * 2 + 2] <= '9'))
			checkvalue1 = store_checkvalue[i * 2 + 2] -'0';
		else
			checkvalue1 = store_checkvalue[i * 2 + 2] -'W';
		if (('0' <= store_checkvalue[i * 2 + 3]) && (store_checkvalue[i * 2 + 3] <= '9'))
			checkvalue2 = store_checkvalue[i * 2 + 3] -'0';
		else
			checkvalue2 = store_checkvalue[i * 2 + 3] -'W';
		checkvalue[i] = checkvalue1 * 16 + checkvalue2;
	}

	if ((checkvalue[0] != hdev->rawedid[0x7f])  ||
	    (checkvalue[1] != hdev->rawedid[0xff])  ||
	    (checkvalue[2] != hdev->rawedid[0x17f]) ||
	    (checkvalue[3] != hdev->rawedid[0x1ff])) {
		hdev->RXCap.edid_changed = 1;

		checksum[0] = '0';
		checksum[1] = 'x';
		for (i = 0; i < 4; i++)
			xtochar(0x80 * i + 0x7f, &checksum[2* i + 2]);
		checksum[10] = '\0';
		memcpy(hdev->RXCap.checksum, checksum, 10);
		printf("TV has changed, initial mode: %s  attr: %s now crc: %s\n",
			env_get("outputmode"), env_get("colorattribute"), checksum);
	} else {
		memcpy(hdev->RXCap.checksum, store_checkvalue, 10);
		printf("TV is same, initial mode is: %s attr: %s, checksum: %s\n",
			env_get("outputmode"), env_get("colorattribute"),
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
			env_set("outputmode",
			       scene_output_info.final_displaymode);
			env_set("colorattribute",
			       scene_output_info.final_deepcolor);
			/* if change from DV TV to HDR/SDR TV, don't change
			 * DV status to disabled, as DV core need to be enabled.
			 */
			if ((scene_output_info.final_dv_type !=
			    get_ubootenv_dv_type()) &&
			    (scene_output_info.final_dv_type !=
			     DOLBY_VISION_DISABLE)) {
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
		printf("update outputmode: %s\n", env_get("outputmode"));
		printf("update colorattribute: %s\n", env_get("colorattribute"));
		printf("update hdmichecksum: %s\n", env_get("hdmichecksum"));
	}
	return 0;
}
static int do_get_preferred_mode(cmd_tbl_t * cmdtp, int flag, int argc,
	char * const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	unsigned char *edid = hdev->rawedid;

	struct hdmi_format_para *para;
	char pref_mode[64];
	char color_attr[64];
	char *hdmi_read_edid;

	hdmi_read_edid = env_get("hdmi_read_edid");
	if (hdmi_read_edid && (hdmi_read_edid[0] == '0'))
		return 0;

	memset(edid, 0, EDID_BLK_SIZE * EDID_BLK_NO);
	memset(pref_mode, 0, sizeof(pref_mode));
	memset(color_attr, 0, sizeof(color_attr));

	/* If sink is not detected there is a still a good chance it supports proper modes */
	/* 720p is chosen as a safe compromise: supported by most sinks and looks good enough */
	if (!hdev->hwop.get_hpd_state()) {
		para = hdmi_get_fmt_paras(HDMI_1280x720p60_16x9);
		snprintf(pref_mode, sizeof(pref_mode), "%s", para->sname);
		snprintf(color_attr, sizeof(color_attr), "%s", "rgb,8bit");
		printk("no sink, fallback to %s[%d]\n", para->sname, HDMI_1280x720p60_16x9);
		goto bypass_edid_read;
	}

	get_parse_edid_data(hdev);

	para = hdmi_get_fmt_paras(hdev->RXCap.preferred_mode);

	if (para) {
		sprintf(pref_mode, "preferred_mode %s", para->sname);
		if (hdev->RXCap.pref_colorspace & (1 << 5))
			sprintf(color_attr, "setenv colorattribute %s", "444,8bit");
		else if (hdev->RXCap.pref_colorspace & (1 << 4))
			sprintf(color_attr, "setenv colorattribute %s", "422,8bit");
		else
			sprintf(color_attr, "setenv colorattribute %s", "rgb,8bit");
	} else {
		hdev->RXCap.preferred_mode = HDMI_720x480p60_16x9;
		para = hdmi_get_fmt_paras(HDMI_720x480p60_16x9);
		sprintf(pref_mode, "setenv hdmimode %s", para->sname);
		sprintf(color_attr, "setenv colorattribute %s", "444,8bit");
	}
	printk("sink preferred_mode is %s[%d]\n", para->sname, hdev->RXCap.preferred_mode);

bypass_edid_read:
	/* save to ENV */
	/*
	run_command(pref_mode, 0);
	run_command(color_attr, 0);
	run_command("saveenv", 0);
	*/
	printk("hdr mode is %d\n", hdev->RXCap.hdr_info.hdr_sup_eotf_smpte_st_2084);
	printk("dv  mode is ver:%d  len: %x\n", hdev->RXCap.dv_info.ver, hdev->RXCap.dv_info.length);
	printk("hdr10+ mode is %d\n", hdev->RXCap.hdr10plus_info.application_version);

	return 0;
}

static cmd_tbl_t cmd_hdmi_sub[] = {
	U_BOOT_CMD_MKENT(hpd, 1, 1, do_hpd_detect, "", ""),
	U_BOOT_CMD_MKENT(edid, 3, 1, do_edid, "", ""),
	U_BOOT_CMD_MKENT(rx_det, 1, 1, do_rx_det, "", ""),
	U_BOOT_CMD_MKENT(output, 3, 1, do_output, "", ""),
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
#if 0
	"hdmitx edid read ADDRESS\n"
	"    Read hdmi rx edid from ADDRESS(0x00~0xff)\n"
#endif
	"hdmitx output [list | FORMAT | bist PATTERN]\n"
	"    list: list support formats\n"
	"    FORMAT can be 720p60/50hz, 1080i60/50hz, 1080p60hz, etc\n"
	"       extend with 8bits/10bits, y444/y422/y420/rgb\n"
	"       such as 2160p60hz,10bits,y420\n"
	"    PATTERN: can be as: line, dot, off, or 1920(width)\n"
	"hdmitx blank [0|1]\n"
	"    1: output blank  0: output normal\n"
	"hdmitx off\n"
	"    Turn off hdmitx output\n"
	"hdmitx info\n"
	"    current mode info\n"
	"hdmitx rx_det\n"
	"    Auto detect if RX is FBC and set outputmode\n"
);

struct hdr_info *hdmitx_get_rx_hdr_info(void)
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	return &hdev->RXCap.hdr_info;
}
