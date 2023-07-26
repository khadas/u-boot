// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <amlogic/media/vout/hdmitx/hdmitx.h>
#include <amlogic/media/dv/dolby_vision.h>

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
	int blk_no;

	if (!buf)
		return;
	blk_no = buf[126] + 1;
	if (blk_no > 4)
		blk_no = 4;

	if (blk_no == 2)
		if (buf[128 + 4] == 0xe2 && buf[128 + 5] == 0x78)
			blk_no = buf[128 + 6] + 1;
	if (blk_no > EDID_BLK_NO)
		blk_no = EDID_BLK_NO;

	printf("dump EDID rawdata\n");
	printf("  ");
	for (i = 0; i < blk_no * EDID_BLK_SIZE; i++)
		printk("%02x", buf[i]);
	printf("\n");
}

static int do_edid(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned int tmp_addr = 0;
	unsigned char edid_addr = 0;
	unsigned char st = 0;
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	if (!hdev) {
		printf("null hdmitx dev\n");
		return CMD_RET_FAILURE;
	}
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

	if (!hdev) {
		printf("null hdmitx dev\n");
		return CMD_RET_FAILURE;
	}
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

static void save_default_720p(void)
{
	env_set("outputmode", DEFAULT_HDMI_MODE);
	env_set("colorattribute", DEFAULT_COLOR_FORMAT);
}

static void hdmitx_mask_rx_info(struct hdmitx_dev *hdev)
{
	if (!hdev || !hdev->para)
		return;

	if (env_get("colorattribute"))
		hdmi_parse_attr(hdev->para, env_get("colorattribute"));

	/* when current output color depth is 8bit, mask hdr capability */
	/* refer to SWPL-44445 for more detail */
	if (hdev->para->cd == HDMI_COLOR_DEPTH_24B)
		memset(&hdev->RXCap.hdr_info, 0, sizeof(struct hdr_info));
}

static void hdmitx_config_csc_en(struct hdmitx_dev *hdev)
{
	if (!hdev)
		return;

	if (env_get("config_csc_en")) {
		if (strncmp("0", env_get("config_csc_en"), 1) == 0)
			hdev->config_csc_en = 0;
		else
			hdev->config_csc_en = 1;
		printf("config_csc_en:%d\n", hdev->config_csc_en);
	} else {
		hdev->config_csc_en = 0;
	}
}

static int do_output(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();
	int ret;

	if (argc < 1)
		return cmd_usage(cmdtp);
	if (!hdev) {
		printf("null hdmitx dev\n");
		return CMD_RET_FAILURE;
	}

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
		if (!hdev->para) {
			printf("null hdmitx para\n");
			return CMD_RET_FAILURE;
		}
		hdev->para->cs = HDMI_COLOR_FORMAT_RGB;
		hdev->para->cd = HDMI_COLOR_DEPTH_24B;
		hdev->vic = HDMI_1920x1080p60_16x9;
		ret = hdmi_tx_set(hdev);
		hdev->hwop.test_prbs();
		if (ret != 0)
			return CMD_RET_FAILURE;
	} else if (strncmp(argv[1], "div40", 5) == 0) {
		bool div40 = 0;

		if (argv[1][5] == '1')
			div40 = 1;
		hdev->hwop.set_div40(div40);
	} else { /* "output" */
		if (!edid_parsing_ok(hdev)) {
			/* in SWPL-34712: if EDID parsing error in kernel,
			 * only forcely output default mode(480p,RGB,8bit)
			 * in sysctl, not save the default mode to env.
			 * if uboot follow this rule, will cause issue OTT-19333:
			 * uboot read edid error and then output default mode,
			 * without save it mode env. if then kernel edid normal,
			 * sysctrl/kernel get mode from env, the actual output
			 * mode differs with outputmode env,it will
			 * cause display abnormal(such as stretch). so don't
			 * follow this rule in uboot, that's to say the actual
			 * output mode needs to stays with the outputmode env.
			 */
			printf("edid parsing ng, forcely output 720p, rgb,8bit\n");
			save_default_720p();
			hdev->vic = HDMI_1280x720p60_16x9;
			hdev->para =
				hdmi_get_fmt_paras(hdev->vic);
			hdev->para->cs = HDMI_COLOR_FORMAT_RGB;
			hdev->para->cd = HDMI_COLOR_DEPTH_24B;
			hdmi_tx_set(hdev);
			return CMD_RET_SUCCESS;
		}
		hdev->vic = hdmi_get_fmt_vic(argv[1]);
		hdev->para = hdmi_get_fmt_paras(hdev->vic);
		if (hdev->vic == HDMI_unknown) {
			/* Not find VIC */
			printf("Not find '%s' mapped VIC\n", argv[1]);
			return CMD_RET_FAILURE;
		} else {
			printf("set hdmitx VIC = %d\n", hdev->vic);
		}
		if (!hdev->para) {
			printf("null hdmitx para\n");
			return CMD_RET_FAILURE;
		}
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
		hdmitx_config_csc_en(hdev);
		ret = hdmi_tx_set(hdev);
		if (ret != 0)
			return CMD_RET_FAILURE;
	}
	return CMD_RET_SUCCESS;
}

static int do_blank(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	if (argc < 1)
		return cmd_usage(cmdtp);
	if (!hdev) {
		printf("null hdmitx dev\n");
		return CMD_RET_FAILURE;
	}

	if (strcmp(argv[1], "1") == 0)
		hdev->hwop.output_blank(1);
	if (strcmp(argv[1], "0") == 0)
		hdev->hwop.output_blank(0);

	return CMD_RET_SUCCESS;
}

static int do_off(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	if (!hdev) {
		printf("null hdmitx dev\n");
		return CMD_RET_FAILURE;
	}
	hdev->vic = HDMI_unknown;
	hdev->hwop.turn_off();
	printf("turn off hdmitx\n");
	return CMD_RET_SUCCESS;
}

static int do_dump(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	if (!hdev) {
		printf("null hdmitx dev\n");
		return CMD_RET_FAILURE;
	}

	hdev->hwop.dump_regs();
	return CMD_RET_SUCCESS;
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

static bool check_vic_exist(struct hdmitx_dev *hdev, enum hdmi_vic vic,
					int count)
{
	struct rx_cap *rxcap = NULL;
	int i;

	rxcap = &hdev->RXCap;
	for (i = 0; i < count; i++)
		if (vic == rxcap->VIC[i])
			return 1;

	return 0;
}

static void disp_cap_show(struct hdmitx_dev *hdev)
{
	struct rx_cap *rxcap = NULL;
	struct hdmi_format_para *para = NULL;
	enum hdmi_vic vic;
	int i;

	if (!hdev)
		return;

	rxcap = &hdev->RXCap;
	printf("disp_cap\n");
	for (i = 0; i < rxcap->VIC_count && i < VIC_MAX_NUM; i++) {
		vic = rxcap->VIC[i];
		if (check_vic_exist(hdev, vic, i))
			continue;
		para = hdmi_get_fmt_paras(vic);
		if (para && vic < HDMITX_VESA_OFFSET && !is_vic_over_limited_1080p(vic))
			printf("  %s\n", para->sname ? para->sname : para->name);
	}
}

static void vesa_cap_show(struct hdmitx_dev *hdev)
{
}

static void dc_cap_show(struct hdmitx_dev *hdev)
{
	enum hdmi_vic vic = HDMI_unknown;
	struct rx_cap *prxcap = &hdev->RXCap;
	const struct dv_info *dv = &hdev->RXCap.dv_info;

	printf("dc_cap\n");
	if (prxcap->dc_36bit_420)
		printf("420,12bit\n");
	if (prxcap->dc_30bit_420) {
		printf("420,10bit\n");
		printf("420,8bit\n");
	} else {
		vic = hdmitx_edid_get_VIC(hdev, "2160p60hz420", 0);
		if (vic != HDMI_unknown) {
			printf("420,8bit\n");
			goto next444;
		}
		vic = hdmitx_edid_get_VIC(hdev, "2160p50hz420", 0);
		if (vic != HDMI_unknown) {
			printf("420,8bit\n");
			goto next444;
		}
		vic = hdmitx_edid_get_VIC(hdev, "smpte60hz420", 0);
		if (vic != HDMI_unknown) {
			printf("420,8bit\n");
			goto next444;
		}
		vic = hdmitx_edid_get_VIC(hdev, "smpte50hz420", 0);
		if (vic != HDMI_unknown) {
			printf("420,8bit\n");
			goto next444;
		}
	}
next444:
	if (prxcap->native_Mode & (1 << 5)) {
		if (prxcap->dc_y444) {
			if (prxcap->dc_36bit || dv->sup_10b_12b_444 == 0x2)
				printf("444,12bit\n");
			if (prxcap->dc_30bit || dv->sup_10b_12b_444 == 0x1)
				printf("444,10bit\n");
		}
		printf("444,8bit\n");
	}
	/* y422, not check dc */
	if (prxcap->native_Mode & (1 << 4)) {
		printf("422,12bit\n");
		printf("422,10bit\n");
		printf("422,8bit\n");
	}

	if (prxcap->dc_36bit || dv->sup_10b_12b_444 == 0x2)
		printf("rgb,12bit\n");
	if (prxcap->dc_30bit || dv->sup_10b_12b_444 == 0x1)
		printf("rgb,10bit\n");
	printf("rgb,8bit\n");
}

static void aud_cap_show(struct hdmitx_dev *hdev)
{
}

static void hdr_cap_show(struct hdmitx_dev *hdev)
{
	int hdr10plugsupported = 0;
	struct hdr_info *hdr = &hdev->RXCap.hdr_info;
	const struct hdr10_plus_info *hdr10p = &hdev->RXCap.hdr10plus_info;

	printf("\nhdr_cap\n");
	if (hdr10p->ieeeoui == HDR10_PLUS_IEEE_OUI &&
		hdr10p->application_version != 0xFF)
		hdr10plugsupported = 1;
	printf("HDR10Plus Supported: %d\n", hdr10plugsupported);
	printf("HDR Static Metadata:\n");
	printf("    Supported EOTF:\n");
	printf("        Traditional SDR: %d\n", !!hdr->hdr_sup_eotf_sdr);
	printf("        Traditional HDR: %d\n", !!hdr->hdr_sup_eotf_hdr);
	printf("        SMPTE ST 2084: %d\n", !!hdr->hdr_sup_eotf_smpte_st_2084);
	printf("        Hybrid Log-Gamma: %d\n", !!hdr->hdr_sup_eotf_hlg);
	printf("    Supported SMD type1: %d\n", hdr->hdr_sup_SMD_type1);
	printf("    Luminance Data\n");
	printf("        Max: %d\n", hdr->hdr_lum_max);
	printf("        Avg: %d\n", hdr->hdr_lum_avg);
	printf("        Min: %d\n\n", hdr->hdr_lum_min);
	printf("HDR Dynamic Metadata:");
}

static void _dv_cap_show(const struct dv_info *dv)
{
	int i;

	if (dv->ieeeoui != DV_IEEE_OUI || dv->block_flag != CORRECT) {
		printf("The Rx don't support DolbyVision\n");
		return;
	}
	printf("DolbyVision RX support list:\n");

	if (dv->ver == 0) {
		printf("VSVDB Version: V%d\n", dv->ver);
		printf("2160p%shz: 1\n", dv->sup_2160p60hz ? "60" : "30");
		printf("Support mode:\n");
		printf("  DV_RGB_444_8BIT\n");
		if (dv->sup_yuv422_12bit)
			printf("  DV_YCbCr_422_12BIT\n");
	}
	if (dv->ver == 1) {
		printf("VSVDB Version: V%d(%d-byte)\n", dv->ver, dv->length + 1);
		if (dv->length == 0xB) {
			printf("2160p%shz: 1\n", dv->sup_2160p60hz ? "60" : "30");
		printf("Support mode:\n");
		printf("  DV_RGB_444_8BIT\n");
		if (dv->sup_yuv422_12bit)
			printf("  DV_YCbCr_422_12BIT\n");
		if (dv->low_latency == 0x01)
			printf("  LL_YCbCr_422_12BIT\n");
		}

		if (dv->length == 0xE) {
			printf("2160p%shz: 1\n", dv->sup_2160p60hz ? "60" : "30");
			printf("Support mode:\n");
			printf("  DV_RGB_444_8BIT\n");
			if (dv->sup_yuv422_12bit)
				printf("  DV_YCbCr_422_12BIT\n");
		}
	}
	if (dv->ver == 2) {
		printf("VSVDB Version: V%d\n", dv->ver);
		printf("2160p%shz: 1\n", dv->sup_2160p60hz ? "60" : "30");
		printf("Support mode:\n");
		if (dv->Interface != 0x00 && dv->Interface != 0x01) {
			printf("  DV_RGB_444_8BIT\n");
			if (dv->sup_yuv422_12bit)
				printf("  DV_YCbCr_422_12BIT\n");
		}
		printf("  LL_YCbCr_422_12BIT\n");
		if (dv->Interface == 0x01 || dv->Interface == 0x03) {
			if (dv->sup_10b_12b_444 == 0x1)
				printf("  LL_RGB_444_10BIT\n");
			if (dv->sup_10b_12b_444 == 0x2)
				printf("  LL_RGB_444_12BIT\n");
		}
	}
	printf("IEEEOUI: 0x%06x\n", dv->ieeeoui);
	printf("VSVDB: ");
	for (i = 0; i < (dv->length + 1); i++)
		printf("%02x", dv->rawdata[i]);
	printf("\n");
}

static void dv_cap_show(struct hdmitx_dev *hdev)
{
	const struct dv_info *dv = &hdev->RXCap.dv_info;

	printf("dv_cap\n");
	if (dv->ieeeoui != DV_IEEE_OUI) {
		printf("The Rx don't support DolbyVision\n");
		return;
	}
	_dv_cap_show(dv);
}

static void edid_cap_show(struct hdmitx_dev *hdev)
{
	int i;
	struct rx_cap *prxcap = &hdev->RXCap;

	printf("EDID Version: %d.%d\n", prxcap->edid_version, prxcap->edid_revision);

	printf("EDID block number: 0x%x\n", hdev->rawedid[0x7e]);
	printf("blk0 chksum: 0x%02x\n", prxcap->chksum);

	printf("native Mode %x, VIC (native %d):\n",
		prxcap->native_Mode, prxcap->native_VIC);

	printf("ColorDeepSupport %x\n", prxcap->ColorDeepSupport);

	for (i = 0 ; i < prxcap->VIC_count ; i++)
		printf("%d ", prxcap->VIC[i]);
	printf("\n");
	printf("Vendor: 0x%x ( %s device)\n",
		prxcap->IEEEOUI, (prxcap->IEEEOUI) ? "HDMI" : "DVI");

	printf("MaxTMDSClock1 %d MHz\n", prxcap->Max_TMDS_Clock1 * 5);

	if (prxcap->HF_IEEEOUI) {
		printf("Vendor2: 0x%x\n", prxcap->HF_IEEEOUI);
		printf("MaxTMDSClock2 %d MHz\n", prxcap->Max_TMDS_Clock2 * 5);
	}

	printf("Video_Latency: ");
	if (prxcap->Video_Latency == 0)
		printf(" Invalid/Unknown\n");
	else if (prxcap->Video_Latency == 0xffff)
		printf(" UnSupported\n");
	else
		printf(" %d\n", prxcap->Video_Latency);

	printf("Audio_Latency: ");
	if (prxcap->Audio_Latency == 0)
		printf(" Invalid/Unknown\n");
	else if (prxcap->Audio_Latency == 0xffff)
		printf(" UnSupported\n");
	else
		printf(" %d\n", prxcap->Audio_Latency);

	printf("Interlaced_Video_Latency: ");
	if (prxcap->Interlaced_Video_Latency == 0)
		printf(" Invalid/Unknown\n");
	else if (prxcap->Interlaced_Video_Latency == 0xffff)
		printf(" UnSupported\n");
	else
		printf(" %d\n", prxcap->Interlaced_Video_Latency);

	printf("Interlaced_Audio_Latency: ");
	if (prxcap->Interlaced_Audio_Latency == 0)
		printf(" Invalid/Unknown\n");
	else if (prxcap->Interlaced_Audio_Latency == 0xffff)
		printf(" UnSupported\n");
	else
		printf(" %d\n", prxcap->Interlaced_Audio_Latency);

	if (prxcap->colorimetry_data)
		printf("ColorMetry: 0x%x\n", prxcap->colorimetry_data);
	printf("SCDC: %x\n", prxcap->scdc_present);
	printf("RR_Cap: %x\n", prxcap->scdc_rr_capable);
	printf("LTE_340M_Scramble: %x\n", prxcap->lte_340mcsc_scramble);

	if (prxcap->dv_info.ieeeoui == DV_IEEE_OUI)
		printf("  DolbyVision%d", prxcap->dv_info.ver);
	if (prxcap->hdr_info.hdr_sup_eotf_smpte_st_2084)
		printf("  HDR/%d", prxcap->hdr_info.hdr_sup_eotf_smpte_st_2084);
	if (prxcap->dc_y444 || prxcap->dc_30bit || prxcap->dc_30bit_420)
		printf("  DeepColor");
	printf("\n");
}

static int do_info(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();
	struct hdmi_format_para *para;

	if (!hdev) {
		printf("null hdmitx dev\n");
		return CMD_RET_FAILURE;
	}
	if (!hdev->para)  {
		printf("null hdmitx para\n");
		return CMD_RET_FAILURE;
	}
	para = hdev->para;
	printf("current mode %s vic %d\n", para->ext_name, hdev->vic);
	printf("cd%d cs%d cr%d\n", hdev->para->cd, hdev->para->cs, hdev->para->cr);
	printf("frac_rate: %d\n", hdev->frac_rate_policy);
	printf("Rx EDID info\n");
	dump_full_edid(hdev->rawedid);
	disp_cap_show(hdev);
	vesa_cap_show(hdev);
	aud_cap_show(hdev);
	hdr_cap_show(hdev);
	dv_cap_show(hdev);
	dc_cap_show(hdev);
	edid_cap_show(hdev);
	return CMD_RET_SUCCESS;
}

static int xtochar(int num, char* checksum)
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();

	if (!hdev) {
		printf("null hdmitx dev\n");
		return -1;
	}
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
		if (byte_num == 128) {
			if (edid[128 + 4] == 0xe2 && edid[128 + 5] == 0x78)
				blk_no = edid[128 + 6] + 1;
			if (blk_no > EDID_BLK_NO)
				blk_no = EDID_BLK_NO; /* MAX Read Blocks 8 */
		}
		byte_num += 8;
	}

	if (0)
		dump_full_edid(hdev->rawedid);

	/* parse edid data */
	hdmi_edid_parsing(hdev->rawedid, &hdev->RXCap);

	if (!hdr_priority)
		return;
	/* if hdr_priority is 2, then mark both dv_info and hdr_info */
	if (strcmp(hdr_priority, "2") == 0) {
		memset(&hdev->RXCap.dv_info, 0, sizeof(struct dv_info));
		memset(&hdev->RXCap.hdr_info, 0, sizeof(struct hdr_info));
		memset(&hdev->RXCap.hdr10plus_info, 0, sizeof(struct hdr10_plus_info));
		pr_info("hdr_priority: %s and clear dv/hdr_info\n", hdr_priority);
		return;
	}
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
	if (is_dv_preference(hdev)) {
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
	if (is_dv_preference(hdev)) {
		/* do nothing
		 * already done above, just sync with sysctrl
		 */
	} else if (is_hdr_preference(hdev)) {
		hdr_scene_process(&hdmidata, scene_output_info);
	} else {
		sdr_scene_process(&hdmidata, scene_output_info);
	}
	/* not find outputmode and use default mode */
	if (strlen(scene_output_info->final_displaymode) == 0)
		strcpy(scene_output_info->final_displaymode, DEFAULT_HDMI_MODE);
	/* not find color space and use default mode */
	if (!strstr(scene_output_info->final_deepcolor, "bit"))
		strcpy(scene_output_info->final_deepcolor, DEFAULT_COLOR_FORMAT);
}

static int do_get_parse_edid(cmd_tbl_t * cmdtp, int flag, int argc,
	char * const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();
	unsigned char *store_checkvalue;
	unsigned int i;
	unsigned int checkvalue[4];
	unsigned int checkvalue1;
	unsigned int checkvalue2;
	char checksum[11];
	unsigned char def_cksum[] = {'0', 'x', '0', '0', '0', '0', '0', '0', '0', '0', '\0'};
	char *hdmimode;
	char *colorattribute;
	int user_dv_mode;
	char *last_output_mode;
	char *last_colorattribute;
	int last_dv_status;
	bool over_write = false;
	char dv_type[2] = {0};
	scene_output_info_t scene_output_info;
	struct hdmi_format_para *para = NULL;
	bool mode_support = false;
	/* hdmi_mode / colorattribute may be null or "none".
	 * if either is null or "none", it means user not
	 * selected manually, and need to select the best
	 * mode or colorattribute by policy
	 */
	bool no_manual_output = false;

	if (!hdev) {
		printf("null hdmitx dev\n");
		return CMD_RET_FAILURE;
	}
	if (!hdev->hpd_state) {
		printf("HDMI HPD low, no need parse EDID\n");
		return 1;
	}
	memset(&scene_output_info, 0, sizeof(scene_output_info_t));
	memset(hdev->rawedid, 0, EDID_BLK_SIZE * EDID_BLK_NO);

	get_parse_edid_data(hdev);

	/* check if the tv has changed or anything wrong */
	store_checkvalue = (unsigned char*)env_get("hdmichecksum");
	/* get user selected output mode/color */
	colorattribute = env_get("user_colorattribute");
	hdmimode = env_get("hdmimode");
	user_dv_mode = get_ubootenv_dv_type();

	last_output_mode = env_get("outputmode");
	last_colorattribute = env_get("colorattribute");
	last_dv_status = get_ubootenv_dv_status();

	if (!store_checkvalue)
		store_checkvalue = def_cksum;

	printf("read hdmichecksum: %s, user hdmimode: %s, colorattribute: %s, dv_type: %d\n",
	       store_checkvalue, hdmimode ? hdmimode : "null",
	       colorattribute ? colorattribute : "null", user_dv_mode);

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
		printf("TV has changed, now crc: %s\n", checksum);
	} else {
		memcpy(hdev->RXCap.checksum, store_checkvalue, 10);
		printf("TV is the same, checksum: %s\n", hdev->RXCap.checksum);
	}

	/* check user have selected both mode/color or not */
	if (!hdmimode || !strcmp(hdmimode, "none") ||
		!colorattribute || !strcmp(colorattribute, "none"))
		no_manual_output = true;
	else
		no_manual_output = false;

	if (!no_manual_output) {
		/* check current user selected mode + color support or not */
		para = hdmi_tst_fmt_name(hdmimode, colorattribute);
		if (hdmitx_edid_check_valid_mode(hdev, para))
			mode_support = true;
		else
			mode_support = false;

		/* if user selected mode/color/dv type which saved in ubootenv of
		 * hdmimode/user_colorattribute/user_prefer_dv_type are different
		 * with last actual output mode/color/dv type which saved in
		 * ubootenv of outputmode/colorattribute/dolby_status, then it means
		 * that the user selected format is over-writen by policy(for example:
		 * firstly user has selected HDR priority to HDR, and select color
		 * to rgb,12bit(now the "user_colorattribute" env will be "rgb,12bit"),
		 * but then it selected HDR priority to DV, the actual output color
		 * will be "444,8bit" or "422,12bit" according to dv type, and
		 * the ubootenv "colorattribute" will be "444,8bit" or "422,12bit"),
		 * then uboot should use the policy to select the output format,
		 * otherwise, uboot use hdmimode/user_colorattribute/user_prefer_dv_type
		 * env, while system use outputmode/colorattribute/dolby_status env,
		 * there will be always a mode change during bootup
		 */
		if (mode_support) {
			if (!last_output_mode || strcmp(hdmimode, last_output_mode))
				over_write = true;
			else if (!last_colorattribute ||
				strcmp(colorattribute, last_colorattribute))
				over_write = true;
			else if (user_dv_mode != last_dv_status)
				over_write = true;
			else
				over_write = false;

			if (over_write)
				printf("last output_mode:%s, colorattribute:%s, dolby_status:%d\n",
				last_output_mode ? last_output_mode : "null",
				last_colorattribute ? last_colorattribute : "null",
				last_dv_status);
		}
	}
	/* three cases need to decide output by uboot mode select policy:
	 * 1.TV changed
	 * 2.either hdmimode or colorattribute is NULL or "none",
	 * which means that user have not slected mode or colorattribute,
	 * and need to select the auto best mode or best colorattribute.
	 * 3.user selected mode not supportted by uboot (probably
	 * means mode select policy or edid parse between sysctrl and
	 * uboot have some gap), then need to find proper output mode
	 * with uboot policy.
	 */
	if (hdev->RXCap.edid_changed || no_manual_output || !mode_support || over_write) {
		/* find proper mode if EDID changed */
		scene_process(hdev, &scene_output_info);
		env_set("hdmichecksum", hdev->RXCap.checksum);
		if (edid_parsing_ok(hdev)) {
			/* SWPL-34712: if EDID parsing error case, not save env,
			 * only output default mode(480p,RGB,8bit). after
			 * EDID read OK, systemcontrol will recover the hdmi
			 * mode from env, to avoid keep the default hdmi output
			 */
			env_set("outputmode",
			       scene_output_info.final_displaymode);
			env_set("colorattribute",
			       scene_output_info.final_deepcolor);
			/* if change from DV TV to HDR/SDR TV, don't change
			 * DV status to disabled, as DV core need to be enabled.
			 * that's to say connect DV TV & output DV-> power down box ->
			 * connect HDR/SDR TV -> power on box, the dolby_status
			 * will keep the same as that when connect DV TV under follow sink.
			 */
			if ((scene_output_info.final_dv_type !=
			    get_ubootenv_dv_status()) &&
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
				printf("update dolby_status: %d\n",
				       scene_output_info.final_dv_type);
			}
		} else {
			save_default_720p();
		}
		printf("update outputmode: %s\n", env_get("outputmode"));
		printf("update colorattribute: %s\n", env_get("colorattribute"));
		printf("update hdmichecksum: %s\n", env_get("hdmichecksum"));
	} else {
		env_set("outputmode", hdmimode);
		env_set("colorattribute", colorattribute);
	}
	/* ubootenv dolby_status is used for is_dv_preference() decision,
	 * system_control save current dv output status in it.
	 * it will be used by dv module later to decide DV output later.
	 * if currently adaptive hdr, then we should set dolby_status to
	 * 0, so that DV module won't enable DV.
	 */
	if (get_hdr_policy() == 1)
		env_set("dolby_status", 0);
	hdev->vic = hdmi_get_fmt_vic(env_get("outputmode"));
	hdev->para = hdmi_get_fmt_paras(hdev->vic);
	hdmitx_mask_rx_info(hdev);
	return 0;
}
static int do_get_preferred_mode(cmd_tbl_t * cmdtp, int flag, int argc,
	char * const argv[])
{
	struct hdmitx_dev *hdev = hdmitx_get_hdev();
	struct hdmi_format_para *para;
	char pref_mode[64];
	char color_attr[64];
	char *hdmi_read_edid;

	hdmi_read_edid = env_get("hdmi_read_edid");
	if (hdmi_read_edid && (hdmi_read_edid[0] == '0'))
		return 0;

	if (!hdev) {
		printf("null hdmitx dev\n");
		return CMD_RET_FAILURE;
	}

	memset(hdev->rawedid, 0, EDID_BLK_SIZE * EDID_BLK_NO);
	memset(pref_mode, 0, sizeof(pref_mode));
	memset(color_attr, 0, sizeof(color_attr));

	/* If sink is not detected there is a still a good chance it supports proper modes */
	/* 720p is chosen as a safe compromise: supported by most sinks and looks good enough */
	if (!hdev->hpd_state) {
		para = hdmi_get_fmt_paras(HDMI_1280x720p60_16x9);
		if (!para)
			goto bypass_edid_read;
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
		if (!para)
			goto bypass_edid_read;
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
