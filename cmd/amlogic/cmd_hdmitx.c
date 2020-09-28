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


static int vic_priority_table[16] = {
	HDMI_3840x2160p60_16x9_Y420,
	HDMI_3840x2160p60_16x9,
	HDMI_3840x2160p50_16x9_Y420,
	HDMI_3840x2160p50_16x9,
	HDMI_1920x1080p60_16x9,
	HDMI_1920x1080p50_16x9,
	HDMI_1280x720p60_16x9,
	HDMI_1280x720p50_16x9,
	HDMI_3840x2160p30_16x9,
	HDMI_3840x2160p25_16x9,
	HDMI_3840x2160p24_16x9,
	HDMI_1920x1080p30_16x9,
	HDMI_1920x1080p25_16x9,
	HDMI_1920x1080p24_16x9,
	HDMI_720x480p60_16x9,
	HDMI_720x576p50_16x9,
};

//vic 2 mode and mode 2 vic conversions
static struct color_attr_to_string color_attr_lut[] =
{
	{ COLOR_ATTR_YCBCR444_12BIT,  "444,12bit" },
	{ COLOR_ATTR_YCBCR422_12BIT,  "422,12bit" },
	{ COLOR_ATTR_YCBCR420_12BIT,  "420,12bit" },
	{ COLOR_ATTR_RGB_12BIT,       "rgb,12bit" },
	{ COLOR_ATTR_YCBCR444_10BIT,  "444,10bit" },
	{ COLOR_ATTR_YCBCR422_10BIT,  "422,10bit" },
	{ COLOR_ATTR_YCBCR420_10BIT,  "420,10bit" },
	{ COLOR_ATTR_RGB_10BIT,       "rgb,10bit" },
	{ COLOR_ATTR_YCBCR444_8BIT,   "444,8bit"  },
	{ COLOR_ATTR_YCBCR422_8BIT,   "422,8bit"  },
	{ COLOR_ATTR_YCBCR420_8BIT,   "420,8bit"  },
	{ COLOR_ATTR_RGB_8BIT,        "rgb,8bit"  },
};

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
			break;
		/* For VESA modes, should be RGB format */
		if (hdev->vic >= HDMITX_VESA_OFFSET)
			hdev->para->cs = HDMI_COLOR_FORMAT_RGB;
			hdev->para->cd = HDMI_COLOR_DEPTH_24B;
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

static int xtochar(int num, unsigned char* checksum)
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

static int getBestHdmiColorAttributes(struct rx_cap *pRXCap,
				      int resolution,
				      int inColorSpace,
				      int inColorDepth)
{
	int maxTMDSRate = 0;
	//determine maximum bandwidth available
	if (pRXCap->Max_TMDS_Clock2 != 0) {
		maxTMDSRate = pRXCap->Max_TMDS_Clock2 * 5;
		printf("getBestHdmiColorAttributes: maxTMDSRate1 = %d\n", maxTMDSRate);
	} else {
		if (pRXCap->Max_TMDS_Clock1 < 0xf)
			pRXCap->Max_TMDS_Clock1 = 0x1e;
		maxTMDSRate = pRXCap->Max_TMDS_Clock1 * 5;
		printf("getBestHdmiColorAttributes: maxTMDSRate2 = %d\n", maxTMDSRate);
	}

	/* check 4k50/60 modes first */
	if (resolution == HDMI_3840x2160p60_16x9_Y420 ||
	    resolution == HDMI_3840x2160p60_16x9      ||
	    resolution == HDMI_3840x2160p50_16x9_Y420 ||
	    resolution == HDMI_3840x2160p50_16x9) {
		//4k50 dovi
		if ((pRXCap->dv_info.ieeeoui == 0x00d046) &&
		    (maxTMDSRate >= 594) && (pRXCap->dv_info.sup_2160p60hz)) {
			//std dovi vs LL dovi
			//1.LL only
			if ((pRXCap->dv_info.ver == 2) &&
			    ((pRXCap->dv_info.Interface == 0) ||
			     (pRXCap->dv_info.Interface == 1))) {
				printf("getBestHdmiColorAttributes: 4k LL dovi, COLOR_ATTR_YCBCR422_12BIT\n");
				return COLOR_ATTR_YCBCR422_12BIT;
			//2.user request LL mdoe && sink support LL mode
			} else if (request_ll_mode() && (pRXCap->dv_info.ver == 1) &&
				(pRXCap->dv_info.length == 0xB) && pRXCap->dv_info.low_latency == 1) {
				printf("getBestHdmiColorAttributes: 4k LL dovi, COLOR_ATTR_YCBCR422_12BIT\n");
				return COLOR_ATTR_YCBCR422_12BIT;
			}
			//std dovi
			printf("getBestHdmiColorAttributes: 4k std dovi, COLOR_ATTR_YCBCR444_8BIT\n");
			return COLOR_ATTR_YCBCR444_8BIT;
		}
		if ((resolution == HDMI_3840x2160p60_16x9_Y420) ||
		    (resolution == HDMI_3840x2160p50_16x9_Y420)) {
			//rate needed = 594/2 * 12/8 Mcsc
			if ((inColorDepth == HDMI_COLOR_DEPTH_36B) &&
			    (pRXCap->dc_36bit_420) &&
			   ((maxTMDSRate * 8) >= (297 * 12))) {
				printf("getBestHdmiColorAttributes: 4k420, COLOR_ATTR_YCBCR420_12BIT\n");
				return COLOR_ATTR_YCBCR420_12BIT;
			}
			//rate needed = 594/2 * 10/8 Mcsc
			if (((inColorDepth == HDMI_COLOR_DEPTH_36B) ||
			     (inColorDepth == HDMI_COLOR_DEPTH_30B)) &&
			    (pRXCap->dc_30bit_420) &&
			   ((maxTMDSRate * 8) >= (297 * 10))) {
				printf("getBestHdmiColorAttributes: 4k420, COLOR_ATTR_YCBCR420_10BIT\n");
				return COLOR_ATTR_YCBCR420_10BIT;
			}
			printf("getBestHdmiColorAttributes: 4k420, COLOR_ATTR_YCBCR420_8BIT\n");
			return COLOR_ATTR_YCBCR420_8BIT;
		}
		//non 420 mode */
		if (inColorSpace == HDMI_COLOR_FORMAT_444) {
			if ((pRXCap->support_ycbcr444_flag) || (pRXCap->dc_y444)) {
				printf("getBestHdmiColorAttributes: pure 4k, COLOR_ATTR_YCBCR444_8BIT\n");
				return COLOR_ATTR_YCBCR444_8BIT;
			}
			if (pRXCap->support_ycbcr422_flag) {
				printf("getBestHdmiColorAttributes: pure 4k, COLOR_ATTR_YCBCR422_8BIT\n");
				return COLOR_ATTR_YCBCR422_8BIT;
			}
		}
		printf("getBestHdmiColorAttributes: pure 4k, default: COLOR_ATTR_RGB_8BIT\n");
		return COLOR_ATTR_RGB_8BIT;
	}
	//non 4k60 dovi
	if (pRXCap->dv_info.ieeeoui == 0x00d046) {
		//std dovi vs LL dovi
		//1.LL only
		if ((pRXCap->dv_info.ver == 2) &&
		    ((pRXCap->dv_info.Interface == 0) ||
		     (pRXCap->dv_info.Interface == 1))) {
			printf("getBestHdmiColorAttributes: non-4k LL dovi, COLOR_ATTR_YCBCR422_12BIT\n");
			return COLOR_ATTR_YCBCR422_12BIT;
		//2.user request LL mdoe && sink support LL mode
		} else if (request_ll_mode() && (pRXCap->dv_info.ver == 1) &&
			(pRXCap->dv_info.length == 0xB) && pRXCap->dv_info.low_latency == 1) {
			printf("getBestHdmiColorAttributes: non-4k LL dovi, COLOR_ATTR_YCBCR422_12BIT\n");
			return COLOR_ATTR_YCBCR422_12BIT;
		}
		//std dovi
		printf("getBestHdmiColorAttributes: non-4k std dovi, COLOR_ATTR_YCBCR444_8BIT\n");
		return COLOR_ATTR_YCBCR444_8BIT;
	}
	//find the best color attributes starting with preferred/supplied values
	//assuming no 4k30 modes as uboot should never be in that mode
	switch (inColorDepth)
	{
	case HDMI_COLOR_DEPTH_36B:
		if (inColorSpace == HDMI_COLOR_FORMAT_444) {
			if ((pRXCap->dc_y444) && (pRXCap->dc_36bit)) {
				printf("getBestHdmiColorAttributes: COLOR_ATTR_YCBCR444_12BIT\n");
				return COLOR_ATTR_YCBCR444_12BIT;
			}
			if (pRXCap->dc_36bit_420) {
				printf("getBestHdmiColorAttributes: COLOR_ATTR_YCBCR444_12BIT\n");
				return COLOR_ATTR_YCBCR420_12BIT;
			}
		}
		if ((inColorSpace == HDMI_COLOR_FORMAT_RGB) &&
		    (pRXCap->dc_36bit)) {
			printf("getBestHdmiColorAttributes: COLOR_ATTR_RGB_12BIT\n");
			return COLOR_ATTR_RGB_12BIT;
		}
		break;
	case HDMI_COLOR_DEPTH_30B:
		if (inColorSpace == HDMI_COLOR_FORMAT_444) {
			if ((pRXCap->dc_y444) && (pRXCap->dc_30bit)) {
				printf("getBestHdmiColorAttributes: COLOR_ATTR_YCBCR444_10BIT\n");
				return COLOR_ATTR_YCBCR444_10BIT;
			}
			if (pRXCap->dc_30bit_420) {
				printf("getBestHdmiColorAttributes: COLOR_ATTR_YCBCR420_10BIT\n");
				return COLOR_ATTR_YCBCR420_10BIT;
			}
		}
		if ((inColorSpace == HDMI_COLOR_FORMAT_RGB) &&
		    (pRXCap->dc_30bit)) {
			printf("getBestHdmiColorAttributes: COLOR_ATTR_RGB_10BIT\n");
			return COLOR_ATTR_RGB_10BIT;
		};
		break;
	default:
		if (inColorSpace == HDMI_COLOR_FORMAT_444) {
			if (pRXCap->support_ycbcr444_flag) {
				printf("getBestHdmiColorAttributes: COLOR_ATTR_YCBCR444_8BIT\n");
				return COLOR_ATTR_YCBCR444_8BIT;
			}
			if (pRXCap->support_ycbcr422_flag) {
				printf("getBestHdmiColorAttributes: COLOR_ATTR_YCBCR422_8BIT\n");
				return COLOR_ATTR_YCBCR422_8BIT;
			}
			if (pRXCap->dc_y420) {
				printf("getBestHdmiColorAttributes: COLOR_ATTR_YCBCR420_8BIT\n");
				return COLOR_ATTR_YCBCR420_8BIT;
			}
		}
	}
	//default to rgb,8 bits always
	printf("getBestHdmiColorAttributes: COLOR_ATTR_RGB_8BIT\n");
	return COLOR_ATTR_RGB_8BIT;
}

bool isYuv4kSink(struct rx_cap *pRXCap)
{
	int maxTMDSRate = 0;
	int i = 0;
	int VIC = -1;

	//determine maximum bandwidth available
	if (pRXCap->Max_TMDS_Clock2 != 0) {
		maxTMDSRate = pRXCap->Max_TMDS_Clock2 * 5;
	} else {
		if (pRXCap->Max_TMDS_Clock1 < 0xf)
			pRXCap->Max_TMDS_Clock1 = 0x1e;
		maxTMDSRate = pRXCap->Max_TMDS_Clock1 * 5;
	}

	for (i = 0; i < pRXCap->VIC_count; i++) {
		VIC = pRXCap->VIC[i];
		if ((VIC == HDMI_3840x2160p60_16x9_Y420 ||
			VIC == HDMI_3840x2160p50_16x9_Y420) &&
			maxTMDSRate >= 297) {
			printf("isYuv4kSink: true, maxTMDSRate=%d\n", maxTMDSRate);
			return true;
		}
		if  ((VIC == HDMI_3840x2160p60_16x9  ||
			VIC == HDMI_3840x2160p50_16x9) &&
			maxTMDSRate > 594) {
			printf("isYuv4kSink: true, maxTMDSRate=%d\n", maxTMDSRate);
			return true;
		}
	}
	printf("isYuv4kSink: false, maxTMDSRate=%d\n",maxTMDSRate);
	return false;
}

static int selectBestMode(struct rx_cap *pRXCap, bool isAuto, int manualMode)
{
	int maxTMDSRate = 0;
	bool is4k60Dovi = false;
	bool isDoviSink = false;
	int bestVIC = -1;
	int i = 0, j = 0;

	//determine maximum bandwidth available
	if (pRXCap->Max_TMDS_Clock2 != 0) {
		maxTMDSRate = pRXCap->Max_TMDS_Clock2 * 5;
		printf("selectBestMode: maxTMDSRate1 = %d\n", maxTMDSRate);
	} else {
		if (pRXCap->Max_TMDS_Clock1 < 0xf)
			pRXCap->Max_TMDS_Clock1 = 0x1e;
		maxTMDSRate = pRXCap->Max_TMDS_Clock1 * 5;
		printf("selectBestMode: maxTMDSRate2 = %d\n", maxTMDSRate);
	}
	//dovi 4k60 or not
	if (pRXCap->dv_info.ieeeoui == 0x00d046 && is_dolby_enable()) {
		isDoviSink = true;
		if ((maxTMDSRate >= 594) && (pRXCap->dv_info.sup_2160p60hz)) {
			printf("selectBestMode: is4k60Dovi=true, maxTMDSRate = %d\n", maxTMDSRate);
			is4k60Dovi = true;
		}
	}
	printf("isDoviSink %d, is4k60Dovi %d, manualMode %d\n", isDoviSink, is4k60Dovi, manualMode);

	//check if it was user selected manual mode or auto mode
	if (!isAuto) {
		/* update preferred vic for manuam mode if 4k60dovi sink or not */
		if (is4k60Dovi) {
			if (manualMode == HDMI_3840x2160p60_16x9_Y420)
				manualMode = HDMI_3840x2160p60_16x9;
			if (manualMode == HDMI_3840x2160p50_16x9_Y420)
				manualMode = HDMI_3840x2160p50_16x9;
		} else {
			if (manualMode == HDMI_3840x2160p60_16x9)
				manualMode = HDMI_3840x2160p60_16x9_Y420;
			if (manualMode == HDMI_3840x2160p50_16x9)
				manualMode = HDMI_3840x2160p50_16x9_Y420;
		}
		for (i = 0; i < pRXCap->VIC_count; i++) {
			if ((pRXCap->VIC[i] == HDMI_3840x2160p60_16x9_Y420  ||
			     pRXCap->VIC[i] == HDMI_3840x2160p50_16x9_Y420) &&
			     maxTMDSRate < 297) {
			     printf("selectBestMode: 4k420 vic, maxTMDSRate = %d, skipping\n", maxTMDSRate);
			     continue;
			}
			if ((pRXCap->VIC[i] == HDMI_3840x2160p60_16x9  ||
			     pRXCap->VIC[i] == HDMI_3840x2160p50_16x9) &&
			     maxTMDSRate < 340) {
			     printf("selectBestMode: 4k vic, maxTMDSRate = %d, skipping\n", maxTMDSRate);
			     continue;
			}
			if (manualMode == pRXCap->VIC[i]) {
				printf("selectBestMode: found manualMode=%d, maxTMDSRate = %d\n", manualMode, maxTMDSRate);
				return manualMode;
			}
		}
	}

	//if we are here, we need to choose best auto mode as per the priority table
	for (i = 0; i < sizeof(vic_priority_table)/sizeof(vic_priority_table[0]); i++) {
		bestVIC = vic_priority_table[i];
		if ((is4k60Dovi) &&
		    (bestVIC == HDMI_3840x2160p60_16x9_Y420  ||
		     bestVIC == HDMI_3840x2160p50_16x9_Y420)) {
			printf("selectBestMode: is4k60Dovi=true, 4k420 vic, skipping\n");
			continue;
		}
		if (isDoviSink && !is4k60Dovi &&
		    (bestVIC == HDMI_3840x2160p60_16x9_Y420 ||
		     bestVIC == HDMI_3840x2160p60_16x9      ||
		     bestVIC == HDMI_3840x2160p50_16x9_Y420 ||
		     bestVIC == HDMI_3840x2160p50_16x9)) {
			printf("selectBestMode: is4k30Dovi=true, 4k vic, skipping\n");
			continue;
		}
		if ((bestVIC == HDMI_3840x2160p60_16x9_Y420  ||
		     bestVIC == HDMI_3840x2160p50_16x9_Y420) &&
		     maxTMDSRate < 297) {
			printf("selectBestMode: 4k420 vic, maxTMDSRate = %d, skipping\n", maxTMDSRate);
			continue;
		}
		if ((bestVIC == HDMI_3840x2160p60_16x9  ||
		     bestVIC == HDMI_3840x2160p50_16x9) &&
		     maxTMDSRate < 340) {
			printf("selectBestMode: 4k vic, maxTMDSRate = %d, skipping\n", maxTMDSRate);
			continue;
		}
		for (j = 0; j < pRXCap->VIC_count; j++) {
			printf("selectBestMode: vic_priority_table[%d]=%d, bestVIC=%d, pRXCap->VIC[%d]=%d\n",
				i, vic_priority_table[i], bestVIC, j, pRXCap->VIC[j]);
			if (bestVIC == pRXCap->VIC[j]) {
				printf("selectBestMode: bestVIC=%d\n", bestVIC);
				return bestVIC;
			}
		}
	}
	printf("selectBestMode: return default 1080p as bestVIC=%d\n", bestVIC);
	return HDMI_1920x1080p60_16x9; //default
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
	unsigned char checksum[11];
	struct hdmi_format_para *para;
	char* hdmimode;
	char* colorattribute;
	char* colorspace;
	char* colordepth;
	int manualMode = -1, bestMode = -1, autoMode = -1;
	int inColorSpace = -1, inColorDepth = -1;
	int bestColorAttributes = -1;

	get_parse_edid_data(hdev);

	/*check if the tv has changed or anything wrong*/

	store_checkvalue = (unsigned char*)env_get("hdmichecksum");

	colorattribute = env_get("colorattribute");

	printf("do_get_parse_edid read hdmichecksum %s, colorattribute %s\n", store_checkvalue, colorattribute);

	/* read the input params used by systemcontrol to determine
	 * mode and use the same params to determine the uboot mode
	 * independently.
	 */

	hdmimode = env_get("hdmimode");
	colorspace = env_get("hdmi_colorspace");
	colordepth = env_get("hdmi_colordepth");
	printf("read hdmimode %s, colorspace %s, colordepth %s\n", hdmimode, colorspace, colordepth);

	if (hdmimode && strcmp(hdmimode, "auto")) {
		manualMode = hdmi_get_fmt_vic(hdmimode);
		printf("do_get_parse_edid: autoMode = false, manualMode=%d\n", manualMode);
		autoMode = false;
	} else {
		printf("do_get_parse_edid: autoMode = true, manualMode=%d\n", manualMode);
		autoMode = true;
	}

	if (colorspace && (!strcmp(colorspace, "rgb"))) {
		inColorSpace = HDMI_COLOR_FORMAT_RGB;
		printf("do_get_parse_edid: inColorSpace: HDMI_COLOR_FORMAT_RGB\n");
	} else if (colorspace && (!strcmp(colorspace, "ycbcr"))) {
		inColorSpace = HDMI_COLOR_FORMAT_444;
		printf("do_get_parse_edid: inColorSpace: HDMI_COLOR_FORMAT_444\n");
	} else {
		/* no logoparam or garbage value or auto */
		if (isYuv4kSink(&hdev->RXCap)) {
			inColorSpace = HDMI_COLOR_FORMAT_444;
			printf("do_get_parse_edid: yuv4k sink: inColorSpace: HDMI_COLOR_FORMAT_444\n");
		} else {
			inColorSpace = HDMI_COLOR_FORMAT_RGB;
			printf("do_get_parse_edid: non-yuv4k sink: inColorSpace: HDMI_COLOR_FORMAT_RGB\n");
		}
	}

	if (colordepth && (!strcmp(colordepth, "12"))) {
		inColorDepth = HDMI_COLOR_DEPTH_36B;
		printf("do_get_parse_edid: inColorDepth: HDMI_COLOR_DEPTH_36B\n");
	} else if (colordepth && (!strcmp(colordepth, "10"))) {
		inColorDepth = HDMI_COLOR_DEPTH_30B;
		printf("do_get_parse_edid: inColorDepth: HDMI_COLOR_DEPTH_30B\n");
	} else {
		/* no logoparam or garbage value */
		inColorDepth = HDMI_COLOR_DEPTH_24B;
		printf("do_get_parse_edid: default inColorDepth: HDMI_COLOR_DEPTH_24B\n");
	}
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
		printf("TV has changed, initial mode is: %s  attr: %s crc is :%s\n",
			env_get("outputmode"), env_get("colorattribute"), checksum);
	} else {
		memcpy(hdev->RXCap.checksum, store_checkvalue, 10);
		printf("TV is same, initial mode is: %s attr: %s\n",
			env_get("outputmode"), env_get("colorattribute"));
	}

	if (!is_dolby_enable())
		return 0;

	if (hdev->RXCap.edid_changed && strncmp("0x00000000", (const char*)(hdev->RXCap.checksum), 10)) {
		/* find best mode if edid available */
		bestMode = selectBestMode(&hdev->RXCap, autoMode, manualMode);
		hdev->RXCap.preferred_mode = bestMode;
		printf("do_get_parse_edid: bestMode = %d\n", bestMode);

		para = hdmi_get_fmt_paras(bestMode);
		if (para) {
			if (!strcmp(para->sname, "2160p50hz420"))
				env_set("outputmode", "2160p50hz");
			else if (!strcmp(para->sname, "2160p60hz420"))
				env_set("outputmode", "2160p60hz");
			else
				env_set("outputmode", para->sname);
		}
		else
			env_set("outputmode", "1080p60hz");
		printf("update outputmode: %s\n", env_get("outputmode"));

		/*find best color attributes for the selected hdmi mode */
		bestColorAttributes = getBestHdmiColorAttributes(&hdev->RXCap,
								  bestMode,
								  inColorSpace,
								  inColorDepth);
		printf("do_get_parse_edid: bestColorAttributes = %d\n", bestColorAttributes);

		if (bestColorAttributes >=0 && bestColorAttributes < COLOR_ATTR_RESERVED) {
			for (i = 0; i < sizeof(color_attr_lut) / sizeof(color_attr_lut[0]); i++) {
				if (bestColorAttributes == color_attr_lut[i].color_attr) {
					/* set the colorattribute string from the LUT match */
					env_set("colorattribute", color_attr_lut[i].color_attr_string);
					printf("do_get_parse_edid: setenv colorattribute = %s\n",
								color_attr_lut[i].color_attr_string);
					break;
				}
			}
		} else
			env_set("colorattribute", "rgb,8bit");
		printf("update colorattribute: %s\n", env_get("colorattribute"));
	} else {
		printf("Tv not changed or null checksum: outputmode: %s attr: %s, checksum %s\n",
			env_get("outputmode"), env_get("colorattribute"), hdev->RXCap.checksum);
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
