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
#include <amlogic/edid-decode.h>

static unsigned char edid_raw_buf[512] = {0};
/* there may be outputmode/2/3 when in multi-display case,
 * sel_hdmimode is used to save the selected hdmi mode
 */
static char sel_hdmimode[MODE_LEN] = {0};

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
		printf("%02x", buf[i]);
	printf("\n");
}

#define EDID_RETRY_WAITTIME 500

static int do_edid(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned char st = 0;
	unsigned char count = 0;

	struct hdmitx_dev *hdev = get_hdmitx21_device();

	memset(edid_raw_buf, 0, ARRAY_SIZE(edid_raw_buf));

READ_EDID:
	st = hdev->hwop.read_edid(edid_raw_buf);

	if (!st)
		printf("edid read failed\n");
	else if (hdev->hwop.get_hpd_state() == 1) {
		if (-EDID_ERR_RETRY == parse_edid(edid_raw_buf, st, count)) {
				printf("hdmitx: read edid fails.. retry..\n");
				mdelay(EDID_RETRY_WAITTIME);
				count++;
				goto READ_EDID;
		}

		/* select best resolution */
		env_set("hdmimode", select_best_resolution());
		env_set("outputmode", env_get("hdmimode"));
	}

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

static void save_default_720p(void)
{
	memcpy(sel_hdmimode, DEFAULT_HDMI_MODE, sizeof(DEFAULT_HDMI_MODE));
	if (is_hdmi_mode(env_get("outputmode")))
		env_set("outputmode", DEFAULT_HDMI_MODE);
	else if (is_hdmi_mode(env_get("outputmode2")))
		env_set("outputmode2",	DEFAULT_HDMI_MODE);
	else if (is_hdmi_mode(env_get("outputmode3")))
		env_set("outputmode3", DEFAULT_HDMI_MODE);
	env_set("colorattribute", DEFAULT_COLOR_FORMAT);
}

static void hdmitx_mask_rx_info(struct hdmitx_dev *hdev)
{
	if (!hdev || !hdev->para)
		return;

	if (env_get("colorattribute"))
		hdmitx21_get_fmtpara(sel_hdmimode, env_get("colorattribute"));

	/* when current output color depth is 8bit, mask hdr capability */
	/* refer to SWPL-44445 for more detail */
	if (hdev->para->cd == COLORDEPTH_24B)
		memset(&hdev->RXCap.hdr_info, 0, sizeof(struct hdr_info));
}

static int do_output(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	const struct hdmi_timing *timing = NULL;
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
		else if (strcmp(argv[2], "x") == 0)
			mode = 'x';
		else if (strcmp(argv[2], "X") == 0)
			mode = 'X';
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
			hdev->vic = HDMI_4_1280x720p60_16x9;
			hdev->para =
				hdmitx21_get_fmtpara("720p60hz", "rgb,8bit");
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
		/* S5 support over 6G, T7 not support */
		switch (hdev->vic) {
		case HDMI_96_3840x2160p50_16x9:
		case HDMI_97_3840x2160p60_16x9:
		case HDMI_101_4096x2160p50_256x135:
		case HDMI_102_4096x2160p60_256x135:
		case HDMI_106_3840x2160p50_64x27:
		case HDMI_107_3840x2160p60_64x27:
			if (hdev->chip_type == MESON_CPU_ID_T7) {
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
			}
			break;
		default:
			/* In Spec2.1 Table 7-34, greater than 2160p30hz will support y420 */
			timing = hdmitx21_gettiming_from_vic(hdev->vic);
			if (!timing)
				break;
			if (timing->v_active > 2160 && timing->v_freq > 30000)
				break;
			if (timing->v_active >= 4320)
				break;
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
		if (hdev->frl_rate && !hdev->flt_train_st) {
			/* FLT training failed, need go to tmds mode */
			printf("hdmitx frl training failed, set tmds mode\n");
			run_command("setenv hdmimode 1080p60hz", 0);
			run_command("setenv colorattribute 422,12bit", 0);
			run_command("run init_display_base", 0);
		}
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
	const struct hdmi_timing *timing = NULL;
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
		timing = hdmitx21_gettiming_from_vic(vic);
		if (timing && vic < HDMITX_VESA_OFFSET && !is_vic_over_limited_1080p(vic))
			printf("  %s\n", timing->sname ? timing->sname : timing->name);
	}
}

static void vesa_cap_show(struct hdmitx_dev *hdev)
{
}

static void dc_cap_show(struct hdmitx_dev *hdev)
{
	enum hdmi_vic vic = HDMI_0_UNKNOWN;
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
		if (vic != HDMI_0_UNKNOWN) {
			printf("420,8bit\n");
			goto next444;
		}
		vic = hdmitx_edid_get_VIC(hdev, "2160p50hz420", 0);
		if (vic != HDMI_0_UNKNOWN) {
			printf("420,8bit\n");
			goto next444;
		}
		vic = hdmitx_edid_get_VIC(hdev, "smpte60hz420", 0);
		if (vic != HDMI_0_UNKNOWN) {
			printf("420,8bit\n");
			goto next444;
		}
		vic = hdmitx_edid_get_VIC(hdev, "smpte50hz420", 0);
		if (vic != HDMI_0_UNKNOWN) {
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

static int do_info(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();
	struct hdmi_format_para *para;

	if (!hdev) {
		pr_info("null hdmitx dev\n");
		return CMD_RET_FAILURE;
	}
	if (!hdev->para) {
		printf("null hdmitx para\n");
		return CMD_RET_FAILURE;
	}

	para = hdev->para;
	printf("current mode %s vic %d\n", para->timing.name, hdev->vic);
	printf("cd%d cs%d cr%d\n", para->cd, para->cs, para->cr);
	printf("enc_idx %d\n", hdev->enc_idx);
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
	struct hdmi_format_para *para = NULL;
	bool mode_support = false;
	/* hdmi_mode / colorattribute may be null or "none".
	 * if either is null or "none", it means user not
	 * selected manually, and need to select the best
	 * mode or colorattribute by policy
	 */
	bool no_manual_output = false;

	if (!hdev->hwop.get_hpd_state()) {
		printf("HDMI HPD low, no need parse EDID\n");
		return 1;
	}
	memset(&scene_output_info, 0, sizeof(struct scene_output_info));

	get_parse_edid_data(hdev);

	/* check if the tv has changed or anything wrong */
	store_checkvalue = (unsigned char *)env_get("hdmichecksum");
	/* get user selected output mode/color */
	colorattribute = env_get("user_colorattribute");
	hdmimode = env_get("hdmimode");
	if (!store_checkvalue)
		store_checkvalue = def_cksum;

	printf("read hdmichecksum: %s, user hdmimode: %s, colorattribute: %s\n",
	       store_checkvalue, hdmimode ? hdmimode : "null",
	       colorattribute ? colorattribute : "null");

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
		printf("TV has changed, now crc: %s\n", checksum);
	} else {
		memcpy(hdev->RXCap.checksum, store_checkvalue, 10);
		printf("TV is same, checksum: %s\n", hdev->RXCap.checksum);
	}

	/* check user have selected both mode/color or not */
	if (!hdmimode || !strcmp(hdmimode, "none") ||
		!colorattribute || !strcmp(colorattribute, "none"))
		no_manual_output = true;
	else
		no_manual_output = false;

	if (!no_manual_output) {
		/* check current user selected mode + color support or not */
		para = hdmitx21_get_fmtpara(hdmimode, colorattribute);
		if (hdmitx_edid_check_valid_mode(hdev, para))
			mode_support = true;
		else
			mode_support = false;
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
	if (hdev->RXCap.edid_changed || no_manual_output || !mode_support) {
		/* find proper mode if EDID changed */
		scene_process(hdev, &scene_output_info);
		env_set("hdmichecksum", hdev->RXCap.checksum);
		if (edid_parsing_ok(hdev)) {
			/* SWPL-34712: if EDID parsing error case, not save env,
			 * only output default mode(480p,RGB,8bit). after
			 * EDID read OK, systemcontrol will recover the hdmi
			 * mode from env, to avoid keep the default hdmi output
			 */
			memcpy(sel_hdmimode, scene_output_info.final_displaymode,
				sizeof(scene_output_info.final_displaymode));
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
			 * that's to say connect DV TV & output DV-> power down box ->
			 * connect HDR/SDR TV -> power on box, the dolby_status
			 * will keep the same as that when connect DV TV under follow sink.
			 */
			if (scene_output_info.final_dv_type != get_ubootenv_dv_status() &&
			    scene_output_info.final_dv_type != DOLBY_VISION_DISABLE) {
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
		printf("update outputmode: %s\n", sel_hdmimode);
		printf("update colorattribute: %s\n", env_get("colorattribute"));
		printf("update hdmichecksum: %s\n", env_get("hdmichecksum"));
	} else {
		memset(sel_hdmimode, 0, sizeof(sel_hdmimode));
		memcpy(sel_hdmimode, hdmimode, strlen(hdmimode));
		if (is_hdmi_mode(env_get("outputmode")))
			env_set("outputmode", hdmimode);
		else if (is_hdmi_mode(env_get("outputmode2")))
			env_set("outputmode2", hdmimode);
		else if (is_hdmi_mode(env_get("outputmode3")))
			env_set("outputmode3", hdmimode);
		env_set("colorattribute", colorattribute);
	}
	env_set("save_outputmode", sel_hdmimode);
	/* ubootenv dolby_status is used for is_dv_preference() decision,
	 * system_control save current dv output status in it.
	 * it will be used by dv module later to decide DV output later.
	 * if currently adaptive hdr, then we should set dolby_status to
	 * 0, so that DV module won't enable DV.
	 */
	if (get_hdr_policy() == 1)
		env_set("dolby_status", 0);
	hdev->para = hdmitx21_get_fmtpara(sel_hdmimode, env_get("colorattribute"));
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
