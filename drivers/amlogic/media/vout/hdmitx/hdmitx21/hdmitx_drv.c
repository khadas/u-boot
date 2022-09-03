// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <amlogic/media/vout/aml_vout.h>
#include <amlogic/media/vout/hdmitx21/hdmitx.h>
#include <amlogic/auge_sound.h>
#include <linux/arm-smccc.h>
#include "hdmitx_drv.h"

#define HZ 100000000 // TODO

static struct hdmitx_dev hdmitx_device;

static void hdmitx_set_phy(struct hdmitx_dev *hdev);
static void hdmitx_set_div40(bool div40);
static void hdmitx21_dither_config(struct hdmitx_dev *hdev);

struct hdmitx_dev *get_hdmitx21_device(void)
{
	return &hdmitx_device;
}

DECLARE_GLOBAL_DATA_PTR;

#define HSYNC_POLARITY      1   /*HSYNC polarity: active high*/
#define VSYNC_POLARITY      1   /*VSYNC polarity: active high*/

/*Pixel format: 0=RGB444; 1=YCbCr422; 2=YCbCr444; 3=YCbCr420.*/
#define TX_INPUT_COLOR_FORMAT   HDMI_COLORSPACE_YUV444
/*Pixel range: 0=limited; 1=full.*/
#define TX_INPUT_COLOR_RANGE    HDMI_COLOR_RANGE_LIM
/*Pixel range: 0=limited; 1=full.*/
#define TX_OUTPUT_COLOR_RANGE   HDMI_COLOR_RANGE_LIM

static void hdmitx_set_hw(struct hdmitx_dev *hdev);
static int hdmitx_set_audmode(struct hdmitx_dev *hdev);

/*
 * Note: read 8 Bytes of EDID data every time
 */
static int hdmitx_read_edid(u8 *_rx_edid)
{
	u32 i;
	u32 byte_num = 0;
	u8 edid_extension = 1;
	u8 *rx_edid = _rx_edid;

	// Program SLAVE/SEGMENT/ADDR
	hdmitx21_wr_reg(LM_DDC_IVCTX, 0x80); //sel edid
	hdmitx21_wr_reg(DDC_CMD_IVCTX, 0x09); //clear fifo
	hdmitx21_wr_reg(DDC_ADDR_IVCTX, DDC_EDID_ADDR & BIT_DDC_ADDR_REG); //edid slave addr

	// Read complete EDID data sequentially
	while (byte_num < (128 * (1 + edid_extension))) {
		if ((byte_num % 256) == 0)
			hdmitx21_wr_reg(DDC_SEGM_IVCTX, byte_num >> 8); //segment
		hdmitx21_wr_reg(DDC_OFFSET_IVCTX, byte_num & 0xff); //offset
		hdmitx21_wr_reg(DDC_DIN_CNT1_IVCTX, 1 << 3); //data length lo
		hdmitx21_wr_reg(DDC_DIN_CNT2_IVCTX, 0x00); //data length hi
		hdmitx21_wr_reg(DDC_CMD_IVCTX, 0x04); //CMD
		// Wait until I2C done
		hdmitx21_poll_reg(DDC_STATUS_IVCTX, 1 << 4, ~(1 << 4), HZ / 100); //i2c process
		hdmitx21_poll_reg(DDC_STATUS_IVCTX, 0 << 4, ~(1 << 4), HZ / 100); //i2c done
		// Read back 8 bytes
		for (i = 0; i < 8; i++) {
			if (byte_num == 126) {
				edid_extension  = hdmitx21_rd_reg(DDC_DATA_AON_IVCTX);
				rx_edid[byte_num] = edid_extension;
				if (edid_extension > 3)
					edid_extension = 3;
			} else {
				rx_edid[byte_num] = hdmitx21_rd_reg(DDC_DATA_AON_IVCTX);
			}
			byte_num++;
		}
	}
	return 1;
}

static void hdmi_hwp_init(void)
{
	u32 data32;

	// --------------------------------------------------------
	// Program core_pin_mux to enable HDMI pins
	// --------------------------------------------------------
	data32 = 0;
	data32 |= (1 << 28);     // [31:28] GPIOW_15_SEL=1 for hdmitx_hpd
	data32 |= (1 << 24);     // [27:24] GPIOW_14_SEL=1 for hdmitx_scl
	data32 |= (1 << 20);     // [23:20] GPIOW_13_SEL=1 for hdmitx_sda
	hd21_write_reg(PADCTRL_PIN_MUX_REGN, data32);

	hdmitx21_set_default_clk();    // set MPEG, audio and default video
	// [8]      hdcp_topology_err
	// [7]      rxsense_fall
	// [6]      rxsense_rise
	// [5]      err_i2c_timeout_pls
	// [4]      hs_intr
	// [3]      core_aon_intr_rise
	// [2]      hpd_fall
	// [1]      hpd_rise
	// [0]      core_pwd_intr_rise
	hdmitx21_wr_reg(HDMITX_TOP_INTR_STAT_CLR, 0x000001ff);

	// Enable internal pixclk, tmds_clk, spdif_clk, i2s_clk, cecclk
	// [   31] free_clk_en
	// [   13] aud_mclk_sel: 0=Use i2s_mclk; 1=Use spdif_clk. For ACR.
	// [   12] i2s_ws_inv
	// [   11] i2s_clk_inv
	// [    9] tmds_clk_inv
	// [    8] pixel_clk_inv
	// [    3] i2s_clk_enable
	// [    1] tmds_clk_enable
	// [ 0] pixel_clk_enable
	data32 = 0;
	data32 |= (0 << 31);
	data32 |= ((1 - 0) << 13);
	data32 |= (0 << 12);
	data32 |= (0 << 11);
	data32 |= (0 << 9);
	data32 |= (0 << 8);
	data32 |= (1 << 3);
	data32 |= (1 << 1);
	data32 |= (1 << 0);
	hdmitx21_wr_reg(HDMITX_TOP_CLK_CNTL,  data32);
	data32 = 0;
	data32 |= (1 << 8);  // [  8] hdcp_topology_err
	data32 |= (1 << 7);  // [  7] rxsense_fall
	data32 |= (1 << 6);  // [  6] rxsense_rise
	data32 |= (1 << 5);  // [  5] err_i2c_timeout_pls
	data32 |= (1 << 4);  // [  4] hs_intr
	data32 |= (1 << 3);  // [  3] core_aon_intr_rise
	data32 |= (1 << 2);  // [  2] hpd_fall_intr
	data32 |= (1 << 1);  // [  1] hpd_rise_intr
	data32 |= (1 << 0);  // [ 0] core_pwd_intr_rise
	hdmitx21_wr_reg(HDMITX_TOP_INTR_MASKN, 0x6);

	//--------------------------------------------------------------------------
	// Configure E-DDC interface
	//--------------------------------------------------------------------------
	data32 = 0;
	data32 |= (1 << 24); // [26:24] infilter_ddc_intern_clk_divide
	data32 |= (0 << 16); // [23:16] infilter_ddc_sample_clk_divide
	hdmitx21_wr_reg(HDMITX_TOP_INFILTER, data32);
	hdmitx21_wr_reg(AON_CYP_CTL_IVCTX, 2);
	hdmitx21_set_reg_bits(GCP_CNTL_IVCTX, 1, 0, 1);
	// clear avmute
	hdmitx21_set_reg_bits(GCP_AUTO_GEN_IVCTX, 2, 2, 2);
	hdmitx21_set_reg_bits(TPI_SC_IVCTX, 0, 3, 1);
	hdmitx21_set_reg_bits(TPI_SC_IVCTX, 1, 7, 1);
}

static struct hdmi_support_mode gxbb_modes[] = {
	{HDMI_97_3840x2160p60_16x9, "2160p60hz", 0},
	{HDMI_96_3840x2160p50_16x9, "2160p50hz", 0},
	{HDMI_95_3840x2160p30_16x9, "2160p30hz", 0},
	{HDMI_94_3840x2160p25_16x9, "2160p25hz", 0},
	{HDMI_93_3840x2160p24_16x9, "2160p24hz", 0},
	{HDMI_102_4096x2160p60_256x135, "smpte60hz", 0},
	{HDMI_101_4096x2160p50_256x135, "smpte50hz", 0},
	{HDMI_100_4096x2160p30_256x135, "smpte30hz", 0},
	{HDMI_99_4096x2160p25_256x135, "smpte25hz", 0},
	{HDMI_98_4096x2160p24_256x135, "smpte24hz", 0},
	{HDMI_97_3840x2160p60_16x9, "2160p60hz420", 1},
	{HDMI_96_3840x2160p50_16x9, "2160p50hz420", 1},
	{HDMI_101_4096x2160p50_256x135, "smpte50hz420", 1},
	{HDMI_102_4096x2160p60_256x135, "smpte60hz420", 1},
	{HDMI_63_1920x1080p120_16x9, "1080p120hz", 0},
	{HDMI_16_1920x1080p60_16x9, "1080p60hz", 0},
	{HDMI_31_1920x1080p50_16x9, "1080p50hz", 0},
	{HDMI_34_1920x1080p30_16x9, "1080p30hz", 0},
	{HDMI_33_1920x1080p25_16x9, "1080p25hz", 0},
	{HDMI_32_1920x1080p24_16x9, "1080p24hz", 0},
	{HDMI_5_1920x1080i60_16x9, "1080i60hz", 0},
	{HDMI_20_1920x1080i50_16x9, "1080i50hz", 0},
	{HDMI_4_1280x720p60_16x9, "720p60hz", 0},
	{HDMI_19_1280x720p50_16x9, "720p50hz", 0},
	{HDMI_18_720x576p50_16x9, "576p50hz", 0},
	{HDMI_3_720x480p60_16x9, "480p60hz", 0},
	{HDMI_22_720x576i50_16x9, "576i50hz", 0},
	{HDMI_7_720x480i60_16x9, "480i60hz", 0},
};

static void hdmitx_list_support_modes(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(gxbb_modes); i++)
		printf("%s\n", gxbb_modes[i].sname);
}

static void hdmitx21_test_bist(unsigned int mode)
{
	u32 value = 1920;
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	switch (mode) {
	case 1:
	case 2:
	case 3:
		value = hdev->para->timing.h_active;
		hd21_write_reg(VENC_VIDEO_TST_CLRBAR_WIDTH, value / 8);
		hd21_set_reg_bits(ENCP_VIDEO_MODE_ADV, 0, 3, 1);
		hd21_write_reg(VENC_VIDEO_TST_EN, 1);
		hd21_write_reg(VENC_VIDEO_TST_MDSEL, mode);
		break;
	case 0:
	default:
		hd21_set_reg_bits(ENCP_VIDEO_MODE_ADV, 1, 3, 1);
		hd21_write_reg(VENC_VIDEO_TST_EN, 0);
		break;
	}
}

static void hdmitx21_output_blank(unsigned int blank)
{
	// TODO
}

static void hdmitx_load_dts_config(struct hdmitx_dev *hdev)
{
	const void *dt_blob;
	int node;
	char *propdata;
	int ret;

	hdev->limit_res_1080p = 0;
	hdev->enc_idx = 0;
	dt_blob = gd->fdt_blob;
	if (!dt_blob) {
		printf("ERR: hdmitx: dt_blob is null\n");
		return;
	}

	ret = fdt_check_header(dt_blob);
	if (ret < 0) {
		printf("ERR: hdmitx: check dts: %s\n", fdt_strerror(ret));
		return;
	}

	node = fdt_path_offset(dt_blob, "/soc/amhdmitx");
	if (node < 0) {
		printf("ERR: hdmitx: not find /amhdmitx node: %s\n", fdt_strerror(node));
		return;
	}

	propdata = (char *)fdt_getprop(dt_blob, node, "res_1080p", NULL);
	if (propdata) {
		if (be32_to_cpup((u32 *)propdata) == 1)
			hdev->limit_res_1080p = 1;
	}
	printf("limit_res_1080p: %d\n", hdev->limit_res_1080p);

	propdata = (char *)fdt_getprop(dt_blob, node, "enc_idx", NULL);
	if (propdata) {
		if (be32_to_cpup((u32 *)propdata) == 2)
			hdev->enc_idx = 2;
	}
	printf("enc_idx: %d\n", hdev->enc_idx);
}

static void amhdmitx_infoframe_init(struct hdmitx_dev *hdev)
{
	int ret = 0;

	ret = hdmi_vendor_infoframe_init(&hdev->infoframes.vend.vendor.hdmi);
	if (ret)
		pr_info("%s[%d] init vendor infoframe failed %d\n", __func__, __LINE__, ret);
	hdmi_avi_infoframe_init(&hdev->infoframes.avi.avi);
	// TODO, panic
	// hdmi_spd_infoframe_init(&hdev->infoframes.spd.spd,
	//      hdev->config_data.vend_data->vendor_name,
	//      hdev->config_data.vend_data->product_desc);
	hdmi_audio_infoframe_init(&hdev->infoframes.aud.audio);
	hdmi_drm_infoframe_init(&hdev->infoframes.drm.drm);
}

void hdmitx21_init(void)
{
	struct hdmitx_dev *hdev = get_hdmitx21_device();
	char *dongle_mode = NULL;
	static struct hdmi_format_para para;

	dongle_mode = env_get("dongle_mode");
	if (dongle_mode && (dongle_mode[0] == '1'))
		hdev->dongle_mode = 1;
	hdev->hwop.get_hpd_state = hdmitx_get_hpd_state;
	hdev->hwop.read_edid = hdmitx_read_edid;
	hdev->hwop.turn_off = hdmitx_turnoff;
	hdev->hwop.list_support_modes = hdmitx_list_support_modes;
	hdev->hwop.dump_regs = hdmitx21_dump_regs;
	hdev->hwop.test_bist = hdmitx21_test_bist;
	hdev->hwop.test_prbs = hdmitx_test_prbs;
	hdev->hwop.set_div40 = hdmitx_set_div40;
	hdev->hwop.output_blank = hdmitx21_output_blank;
	hdmitx_load_dts_config(hdev);
	hdmi_hwp_init();
	amhdmitx_infoframe_init(hdev);
	hdev->para = &para;
}

static void set_vid_clk_div(u32 div)
{
	hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL, 0, 16, 3);
	hd21_set_reg_bits(CLKCTRL_VID_CLK0_DIV, div - 1, 0, 8);
	hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL, 7, 0, 3);
}

static void set_hdmi_tx_pixel_div(u32 div)
{
	hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL2, 1, 5, 1);
}

static void set_encp_div(u32 div)
{
	hd21_set_reg_bits(CLKCTRL_VID_CLK0_DIV, div, 24, 4);
	hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL, 1, 19, 1);
}

static void hdmitx_enable_encp_clk(void)
{
	hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL2, 1, 2, 1);
}

static void set_hdmitx_fe_clk(void)
{
	u32 tmp = 0;
	u32 vid_clk_cntl2;
	u32 vid_clk_div;
	u32 hdmi_clk_cntl;

	vid_clk_cntl2 = CLKCTRL_VID_CLK0_CTRL2;
	vid_clk_div = CLKCTRL_VID_CLK0_DIV;
	hdmi_clk_cntl = CLKCTRL_HDMI_CLK_CTRL;

	hd21_set_reg_bits(vid_clk_cntl2, 1, 9, 1);

	tmp = (hd21_read_reg(vid_clk_div) >> 24) & 0xf;
	hd21_set_reg_bits(hdmi_clk_cntl, tmp, 20, 4);
}

static void _hdmitx21_set_clk(void)
{
	set_vid_clk_div(1);
	set_hdmi_tx_pixel_div(1);
	set_encp_div(1);
	hdmitx_enable_encp_clk();
	set_hdmitx_fe_clk();
}

//Enable CLK_ENCL
void enable_crt_video_encl(u32 enable, u32 in_sel)
{
	//encl_clk_sel:hi_viid_clk_div[15:12]
	hd21_set_reg_bits(CLKCTRL_VIID_CLK0_DIV, in_sel,  12, 4);
	if (in_sel <= 4) { //V1
		//#if (SDF_CORNER == 0 || SDF_CORNER == 2)    //ss_corner
		//      hd21_set_reg_bits(CLKCTRL_VID_CLK_CTRL, 1, 16, 3);  //sel div4 : 500M
		//#endif
		hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL, 1, in_sel, 1);
	} else {
		hd21_set_reg_bits(CLKCTRL_VIID_CLK0_CTRL, 1, in_sel - 8, 1);
	}

	//gclk_encl_clk:hi_vid_clk_cntl2[3]
	hd21_set_reg_bits(CLKCTRL_VID_CLK0_CTRL2, enable, 3, 1);
}

//Enable CLK_ENCP
void enable_crt_video_encp(u32 enable, u32 in_sel)
{
	enable_crt_video_encl(enable, in_sel);
}

void enable_crt_video_encl2(u32 enable, u32 in_sel)
{
	//encl_clk_sel:hi_viid_clk_div[15:12]
	hd21_set_reg_bits(CLKCTRL_VIID_CLK2_DIV, in_sel, 12, 4);
	if (in_sel <= 4) { //V1
	//#if (SDF_CORNER == 0 || SDF_CORNER == 2)    //ss_corner
	//      hd21_set_reg_bits(CLKCTRL_VID_CLK_CTRL, 1, 16, 3);  //sel div4 : 500M
	//#endif
		hd21_set_reg_bits(CLKCTRL_VID_CLK2_CTRL, 3, 0, 2);
	} else {
		hd21_set_reg_bits(CLKCTRL_VIID_CLK2_CTRL, 1, in_sel - 8, 1);
	}
	//gclk_encl_clk:hi_vid_clk_cntl2[3]
	hd21_set_reg_bits(CLKCTRL_VID_CLK2_CTRL2, enable, 3, 1); /* cts_enc2_clk */
}

//Enable CLK_ENCP
void enable_crt_video_encp2(u32 enable, u32 in_sel)
{
	enable_crt_video_encl2(enable, in_sel);
}

//Enable HDMI_TX_PIXEL_CLK
//Note: when in_sel == 15, select tcon_clko
void enable_crt_video_hdmi(u32 enable, u32 in_sel, u8 enc_sel)
{
	u32 data32;
	u32 addr_enc02_hdmi_clk;
	u32 addr_vid_clk02;
	u32 addr_viid_clk02;
	u32 addr_vid_clk022;
	u32 val = 0;
	struct hdmitx_dev *hdev = get_hdmitx21_device();
	struct hdmi_format_para *para = hdev->para;

	if (para->cs == HDMI_COLORSPACE_YUV420)
		val = 1;
	addr_enc02_hdmi_clk = (enc_sel == 0) ?
				CLKCTRL_ENC0_HDMI_CLK_CTRL : CLKCTRL_ENC2_HDMI_CLK_CTRL;
	addr_vid_clk02 = (enc_sel == 0) ? CLKCTRL_VID_CLK0_CTRL : CLKCTRL_VID_CLK2_CTRL;
	addr_viid_clk02 = (enc_sel == 0) ? CLKCTRL_VIID_CLK0_CTRL : CLKCTRL_VIID_CLK2_CTRL;
	addr_vid_clk022 = (enc_sel == 0) ? CLKCTRL_VID_CLK0_CTRL2 : CLKCTRL_VID_CLK2_CTRL2;

	// hdmi_tx_pnx_clk
	//clk_sel:hi_hdmi_clk_cntl[27:24];
	hd21_set_reg_bits(addr_enc02_hdmi_clk, val, 24, 4);
	// hdmi_tx_fe_clk: for 420 mode, Freq(hdmi_tx_pixel_clk) = Freq(hdmi_tx_fe_clk)/2,
	// otherwise Freq(hdmi_tx_pixel_clk) = Freq(hdmi_tx_fe_clk).
	// clk_sel:hi_hdmi_clk_cntl[23:20];
	hd21_set_reg_bits(addr_enc02_hdmi_clk, (in_sel == 1) ? 0 : in_sel, 20, 4);
	// hdmi_tx_pixel_clk
	//clk_sel:hi_hdmi_clk_cntl[19:16];
	hd21_set_reg_bits(addr_enc02_hdmi_clk, val, 16, 4);
	if (in_sel <= 4) { //V1
		if (in_sel == 1)
			// If 420 mode, need to turn on div1_clk for hdmi_tx_fe_clk
			// For hdmi_tx_fe_clk and hdmi_tx_pnx_clk
			hd21_set_reg_bits(addr_vid_clk02, 3, 0, 2);
	} else if (in_sel <= 9) { //V2
		// For hdmi_tx_pixel_clk
		hd21_set_reg_bits(addr_viid_clk02, 1, in_sel - 8, 1);
	}

	// Enable hdmi_tx_pnx_clk
	hd21_set_reg_bits(addr_vid_clk022, enable, 10, 1);
	// Enable hdmi_tx_fe_clk
	hd21_set_reg_bits(addr_vid_clk022, enable, 9, 1);
	// Enable hdmi_tx_pixel_clk
	hd21_set_reg_bits(addr_vid_clk022, enable, 5, 1);

	// [22:21] clk_sl: 0=enc0_hdmi_tx_pnx_clk, 1=enc2_hdmi_tx_pnx_clk.
	// [   20] clk_en for hdmi_tx_pnx_clk
	// [19:16] clk_div for hdmi_tx_pnx_clk
	// [14:13] clk_sl: 0=enc0_hdmi_tx_fe_clk, 1=enc2_hdmi_tx_fe_clk.
	// [   12] clk_en for hdmi_tx_fe_clk
	// [11: 8] clk_div for hdmi_tx_fe_clk
	// [ 6: 5] clk_sl: 0=enc0_hdmi_tx_pixel_clk, 1=enc2_hdmi_tx_pixel_clk.
	// [    4] clk_en for hdmi_tx_pixel_clk
	// [ 3: 0] clk_div for hdmi_tx_pixel_clk
	data32 = 0;
	data32 = (enc_sel << 21) |
		 (0 << 20) |
		 (0 << 16) |
		 (enc_sel << 13) |
		 (0 << 12) |
		 (0 << 8) |
		 (enc_sel << 5) |
		 (0 << 4) |
		 (0 << 0);
	hd21_write_reg(CLKCTRL_ENC_HDMI_CLK_CTRL, data32);
	hd21_set_reg_bits(CLKCTRL_ENC_HDMI_CLK_CTRL, 1, 20, 1);
	hd21_set_reg_bits(CLKCTRL_ENC_HDMI_CLK_CTRL, 1, 12, 1);
	hd21_set_reg_bits(CLKCTRL_ENC_HDMI_CLK_CTRL, 1, 4, 1);
}   // enable_crt_video_hdmi

static void construct_avi_packet(struct hdmitx_dev *hdev)
{
	struct hdmi_avi_infoframe *info = &hdev->infoframes.avi.avi;
	struct hdmi_format_para *para = hdev->para;

	hdmi_avi_infoframe_init(info);

	info->colorspace = para->cs;
	info->scan_mode = HDMI_SCAN_MODE_NONE;
	info->colorimetry = HDMI_COLORIMETRY_ITU_709;
	info->picture_aspect = HDMI_PICTURE_ASPECT_16_9;
	info->active_aspect = HDMI_ACTIVE_ASPECT_PICTURE;
	info->itc = 0;
	info->extended_colorimetry = HDMI_EXTENDED_COLORIMETRY_XV_YCC_601;
	info->quantization_range = HDMI_QUANTIZATION_RANGE_LIMITED;
	info->nups = HDMI_NUPS_UNKNOWN;
	info->video_code = para->timing.vic;
	if (para->timing.vic == HDMI_95_3840x2160p30_16x9 ||
		para->timing.vic == HDMI_94_3840x2160p25_16x9 ||
		para->timing.vic == HDMI_93_3840x2160p24_16x9 ||
		para->timing.vic == HDMI_98_4096x2160p24_256x135)
		/*HDMI Spec V1.4b P151*/
		info->video_code = 0;
	info->ycc_quantization_range = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
	info->content_type = HDMI_CONTENT_TYPE_GRAPHICS;
	info->pixel_repeat = 0;
	if (para->timing.pi_mode == 0) { /* interlaced modes */
		if (para->timing.h_active == 1440)
			info->pixel_repeat = 1;
		if (para->timing.h_active == 2880)
			info->pixel_repeat = 3;
	}
	info->top_bar = 0;
	info->bottom_bar = 0;
	info->left_bar = 0;
	info->right_bar = 0;
	hdmi_avi_infoframe_set(info);
}

static void construct_audio_packet(struct hdmitx_dev *hdev)
{
	struct hdmi_audio_infoframe *info = &hdev->infoframes.aud.audio;

	hdmi_audio_infoframe_init(info);

	info->coding_type = HDMI_AUDIO_CODING_TYPE_STREAM;
	info->channels = CC_2CH;
	hdmi_audio_infoframe_set(info);
}

static void hdmi_set_vend_spec_infofram(struct hdmitx_dev *hdev,
					enum hdmi_vic videocode)
{
	int i;
	u8 db[28];
	u8 *ven_db = &db[1];
	u8 ven_hb[3];

	ven_hb[0] = 0x81;
	ven_hb[1] = 0x01;
	ven_hb[2] = 0x5;
	memset(db, 0, sizeof(db));

	if (videocode == 0) {      /* For non-4kx2k mode setting */
		hdmi_vend_infoframe_set(NULL);
		return;
	}

	for (i = 0; i < 0x6; i++)
		ven_db[i] = 0;
	ven_db[0] = GET_OUI_BYTE0(HDMI_IEEE_OUI);
	ven_db[1] = GET_OUI_BYTE1(HDMI_IEEE_OUI);
	ven_db[2] = GET_OUI_BYTE2(HDMI_IEEE_OUI);
	ven_db[3] = 0x00;    /* 4k x 2k  Spec P156 */

	if (videocode == HDMI_95_3840x2160p30_16x9) {
		ven_db[3] = 0x20;
		ven_db[4] = 0x1;
	} else if (videocode == HDMI_94_3840x2160p25_16x9) {
		ven_db[3] = 0x20;
	ven_db[4] = 0x2;
	} else if (videocode == HDMI_93_3840x2160p24_16x9) {
		ven_db[3] = 0x20;
		ven_db[4] = 0x3;
	} else if (videocode == HDMI_98_4096x2160p24_256x135) {
		ven_db[3] = 0x20;
		ven_db[4] = 0x4;
	} else {
		;
	}
	hdmi_vend_infoframe_rawset(ven_hb, db);
}

static void set_hdmitx_enc_idx(unsigned int val)
{
	struct arm_smccc_res res;

	arm_smccc_smc(HDCPTX_IOOPR, CONF_ENC_IDX, 1, !!val, 0, 0, 0, 0, &res);
}

void hdmitx21_set(struct hdmitx_dev *hdev)
{
	struct hdmi_format_para *para = hdev->para;
	u32 enc_sel = 0;
	u32 data32;
	enum hdmi_vic vic = para->timing.vic;
	unsigned char checksum[11];
	enum hdmi_vic videocode;

	hdmi_hwp_init();
	aml_audio_init();  /* Init audio hw firstly */
	hdmitx_set_audmode(hdev);
	if (hdev->enc_idx == 2) {
		set_hdmitx_enc_idx(2);
		hd21_set_reg_bits(VPU_DISP_VIU2_CTRL, 1, 29, 1);
		hd21_set_reg_bits(VPU_VIU_VENC_MUX_CTRL, 2, 2, 2);
	}
	hdmitx21_venc_en(0, 0);
	hd21_set_reg_bits(VPU_HDMI_SETTING, 0, (hdev->enc_idx == 0) ? 0 : 1, 1);
	if (hdev->enc_idx == 0)
		enable_crt_video_encp(1, 0);
	else
		enable_crt_video_encp2(1, 0);
	enable_crt_video_hdmi(1,
			      (TX_INPUT_COLOR_FORMAT == HDMI_COLORSPACE_YUV420) ? 1 : 0,
			      hdev->enc_idx);
	// configure GCP
	/* for 8bit depth or y422: non-merge gcp mode + clr_avmute,
	 * for dc mode: merge gcp mode + clr_avmute
	 */
	if (para->cs == HDMI_COLORSPACE_YUV422 || para->cd == COLORDEPTH_24B) {
		hdmitx21_set_reg_bits(GCP_CNTL_IVCTX, 0, 0, 1);
		/* hdmi_gcppkt_manual_set(1); */
	} else {
		/* hdmi_gcppkt_manual_set(0); */
		hdmitx21_set_reg_bits(GCP_CNTL_IVCTX, 1, 0, 1);
	}

	// --------------------------------------------------------
	// Enable viu vsync interrupt, enable hdmitx interrupt, enable htx_hdcp22 interrupt
	// --------------------------------------------------------
	hd21_write_reg(VPU_VENC_CTRL, 1);
	if (hdev->enc_idx == 2) {
		// Enable VENC2 to HDMITX path
		hd21_set_reg_bits(SYSCTRL_VPU_SECURE_REG0, 1, 16, 1);
	}
	// --------------------------------------------------------
	// Set TV encoder for HDMI
	// --------------------------------------------------------
	pr_info("configure venc\n");
	//              enc_index   output_type enable)
	// only 480i / 576i use the ENCI
	if (para->timing.pi_mode == 0 &&
	    (para->timing.v_active == 480 || para->timing.v_active == 576)) {
		hd21_write_reg(VPU_VENC_CTRL, 0); // sel enci timming
		set_tv_enci_new(hdev, enc_sel, vic, 1);
	} else {
		hd21_write_reg(VPU_VENC_CTRL, 1); // sel encp timming
		set_tv_encp_new(hdev, enc_sel, vic, 1);
	}

	// --------------------------------------------------------
	// Configure video format timing for HDMI:
	// Based on the corresponding settings in set_tv_enc.c, calculate
	// the register values to meet the timing requirements defined in CEA-861-D
	// --------------------------------------------------------
	pr_info("configure VIC%d timing\n", vic);

	// [ 1: 0] hdmi_vid_fmt. 0=444; 1=convert to 422; 2=convert to 420.
	// [ 3: 2] chroma_dnsmp_h. 0=use pixel 0; 1=use pixel 1; 2=use average.
	// [    4] dith_en. 1=enable dithering before HDMI TX input.
	// [    5] hdmi_dith_md: random noise selector.
	// [ 9: 6] hdmi_dith10_cntl.
	// [   10] hdmi_round_en. 1= enable 12-b rounded to 10-b.
	// [   11] tunnel_en
	// [21:12] hdmi_dith_new
	// [23:22] chroma_dnsmp_v. 0=use line 0; 1=use line 1; 2=use average.
	// [27:24] pix_repeat
	data32 = 0;
	data32 = (((para->cs == HDMI_COLORSPACE_YUV420) ? 2 :
		  (para->cs == HDMI_COLORSPACE_YUV422) ? 1 : 0) << 0) |
		  (2 << 2) |
		  (0 << 4) |
		  (0 << 5) |
		  (0 << 6) |
		  (((para->cd == COLORDEPTH_24B) ? 1 : 0) << 10) |
		  (0 << 11) |
		  (0 << 12) |
		  (2 << 22) |
		  (0 << 24);
	hd21_write_reg(VPU_HDMI_FMT_CTRL, data32);

	// [    2] inv_hsync_b
	// [    3] inv_vsync_b
	// [    4] hdmi_dith_en_b. For 10-b to 8-b.
	// [    5] hdmi_dith_md_b. For 10-b to 8-b.
	// [ 9: 6] hdmi_dith10_b. For 10-b to 8-b.
	// [   10] hdmi_round_en_b. For 10-b to 8-b.
	// [21:12] hdmi_dith_new_b. For 10-b to 8-b.
	data32 = 0;
	data32 = (0 << 2) |
		(0 << 3) |
		(0 << 4) |
		(0 << 5) |
		(0 << 6) |
		(((para->cd == COLORDEPTH_24B) ? 1 : 0) << 10) |
		(0 << 12);
	hd21_write_reg(VPU_HDMI_DITH_CNTL, data32);
	hdmitx21_dither_config(hdev);

	_hdmitx21_set_clk();

	// Set this timer very small on purpose, to test the new function
	hdmitx21_wr_reg(HDMITX_TOP_I2C_BUSY_CNT_MAX,  30);

	data32 = 0;
	data32 |= (1 << 31); // [   31] cntl_hdcp14_min_size_v_en
	data32 |= (240 << 16); // [28:16] cntl_hdcp14_min_size_v
	data32 |= (1 << 15); // [   15] cntl_hdcp14_min_size_h_en
	data32 |= (640 << 0);  // [13: 0] cntl_hdcp14_min_size_h
	hdmitx21_wr_reg(HDMITX_TOP_HDCP14_MIN_SIZE, data32);

	data32 = 0;
	data32 |= (1 << 31); // [   31] cntl_hdcp22_min_size_v_en
	data32 |= (1080 << 16); // [28:16] cntl_hdcp22_min_size_v
	data32 |= (1 << 15); // [   15] cntl_hdcp22_min_size_h_en
	data32 |= (1920 << 0);  // [13: 0] cntl_hdcp22_min_size_h
	hdmitx21_wr_reg(HDMITX_TOP_HDCP22_MIN_SIZE, data32);

	hdmitx21_set_clk(hdev);
	hdmitx_set_hw(hdev);
	if (para->timing.pi_mode == 0 &&
	    (para->timing.v_active == 480 || para->timing.v_active == 576))
		hdmitx21_venc_en(1, 0);
	else
		hdmitx21_venc_en(1, 1);
	construct_avi_packet(hdev);

	videocode = para->timing.vic;
	if (videocode == HDMI_95_3840x2160p30_16x9 ||
		videocode == HDMI_94_3840x2160p25_16x9 ||
		videocode == HDMI_93_3840x2160p24_16x9 ||
		videocode == HDMI_98_4096x2160p24_256x135)
		hdmi_set_vend_spec_infofram(hdev, videocode);

	// [    0] src_sel_enci
	// [    1] src_sel_encp
	// [    2] inv_hsync. 1=Invert Hsync polarity.
	// [    3] inv_vsync. 1=Invert Vsync polarity.
	// [    4] inv_dvi_clk. 1=Invert clock to external DVI,
	//         (clock invertion exists at internal HDMI).
	// YUV420. Output Y1Y0C to hdmitx.
	// YUV444/422/RGB. Output CrYCb, CY0, or RGB to hdmitx.
	// [ 7: 5] comp_map_post. Data from vfmt is CrYCb(444), CY0(422), CY0Y1(420) or RGB,
	//         map the data to desired format before go to hdmitx:
	// 0=output {2, 1,0};
	// 1=output {1,0,2};
	// 2=output {1,2,0};
	// 3=output {0,2,1};
	// 4=output {0, 1,2};
	// 5=output {2.0,1};
	// 6,7=Rsrv.
	// [11: 8] wr_rate_pre. 0=A write every clk1; 1=A write every 2 clk1; ...;
	//                      15=A write every 16 clk1.
	// [15:12] rd_rate_pre. 0=A read every clk2; 1=A read every 2 clk2; ...;
	//                      15=A read every 16 clk2.
	// RGB. Output RGB to vfmt.
	// YUV. Output CrYCb to vfmt.
	// [18:16] comp_map_pre. Data from VENC is YCbCr or RGB, map data to desired
	//                       format before go to vfmt:
	// 0=output YCbCr(RGB);
	// 1=output CbCrY(GBR);
	// 2=output CbYCr(GRB);
	// 3=output CrYCb(BRG);
	// 4=output CrCbY(BGR);
	// 5=output YCrCb(RBG);
	// 6,7=Rsrv.
	// [23:20] wr_rate_post. 0=A write every clk1; 1=A write every 2 clk1; ...;
	//                       15=A write every 16 clk1.
	// [27:24] rd_rate_post. 0=A read every clk2; 1=A read every 2 clk2; ...;
	//                       15=A read every 16 clk2.
	data32 = 0;
	data32 = (0 << 0) |
		 (0 << 1) |
		 (para->timing.h_pol << 2) |
		 (para->timing.v_pol << 3) |
		 (0 << 4) |
		 (((para->cs == HDMI_COLORSPACE_YUV420) ? 4 : 0) << 5) |
		 (0 << 8) |
		 (0 << 12) |
		 (((TX_INPUT_COLOR_FORMAT == HDMI_COLORSPACE_RGB) ? 0 : 3) << 16) |
		 (((para->cs == HDMI_COLORSPACE_YUV420) ? 1 : 0) << 20) |
		 (0 << 24);
	hd21_write_reg(VPU_HDMI_SETTING, data32);
	// [    1] src_sel_encp: Enable ENCI or ENCP output to HDMI
	hd21_set_reg_bits(VPU_HDMI_SETTING, 1, (hdev->enc_idx == 0) ? 0 : 1, 1);

	hdmitx_set_phy(hdev);

	/* null char needed to terminate the string
	 * otherwise garbage in checksum logopara
	 */
	memcpy(checksum, hdev->RXCap.checksum, 10);
	checksum[10] = '\0';
	env_set("hdmichecksum", (const char *)checksum);
	printf("hdmi_tx_set: save mode: %s, attr: %s, hdmichecksum: %s\n",
		env_get("outputmode"), env_get("colorattribute"), env_get("hdmichecksum"));
	run_command("saveenv", 0);
}

/***********************************************
 * parameters:  mode, such as 720p60hz, 1080p50hz...
 *              frac, 1=59.94/29.97hz.
 * return:      viu_mux
 ************************************************/
unsigned int hdmi_outputmode_check(char *mode, unsigned int frac)
{
	int i, ret = 0xff;
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	for (i = 0; i < ARRAY_SIZE(gxbb_modes); i++) {
		if (!strcmp(mode, gxbb_modes[i].sname)) {
			ret = 0;
			break;
		}
	}

	if (ret) {
		//printf("hdmitx: outputmode[%s] is invalid\n", mode);
		return VIU_MUX_MAX;
	}

	if (frac) {
		if (hdmitx_likely_frac_rate_mode(mode) == 0) {
			printf("hdmitx: outputmode[%s] don't support frac\n", mode);
			return VIU_MUX_MAX;
		}
	}

	if ((!strcmp(mode, "480i60hz")) || (!strcmp(mode, "576i50hz")))
		ret = VIU_MUX_ENCI;
	else
		ret = VIU_MUX_ENCP;
	if (hdev->enc_idx == 2)
		ret |= 2 << 4;
	return ret;
}

bool is_hdmi_mode(char *mode)
{
	int i;
	bool ret = false;

	if (!mode)
		return false;

	for (i = 0; i < ARRAY_SIZE(gxbb_modes); i++) {
		if (!strcmp(mode, gxbb_modes[i].sname)) {
			ret = true;
			break;
		}
	}
	return ret;
}

static int hdmitx_set_audmode(struct hdmitx_dev *hdev)
{
	hdmitx21_set_audioclk(1);
	construct_audio_packet(hdev);

	return 0;
}

static void hdmitx_set_scdc_div40(bool div40)
{
	u32 addr = 0x20;
	u32 data;

	if (div40)
		data = 0x3;
	else
		data = 0;

	hdmitx21_wr_reg(DDC_ADDR_IVCTX, 0xa8); //SCDC slave addr
	hdmitx21_wr_reg(DDC_OFFSET_IVCTX, addr & 0xff); //SCDC slave offset
	hdmitx21_wr_reg(DDC_DATA_AON_IVCTX, data & 0xff); //SCDC slave offset data to ddc fifo
	hdmitx21_wr_reg(DDC_DIN_CNT1_IVCTX, 0x01); //data length lo
	hdmitx21_wr_reg(DDC_DIN_CNT2_IVCTX, 0x00); //data length hi
	hdmitx21_wr_reg(DDC_CMD_IVCTX, 0x06); //DDC Write CMD
	hdmitx21_poll_reg(DDC_STATUS_IVCTX, 1 << 4, ~(1 << 4), HZ / 100); //i2c process
	hdmitx21_poll_reg(DDC_STATUS_IVCTX, 0 << 4, ~(1 << 4), HZ / 100); //i2c done
}

static void set_top_div40(bool div40)
{
	u32 data32;

	pr_info("div40: %d\n", div40);
	// Enable normal output to PHY
	if (div40) {
		data32 = 0;
		data32 |= (0 << 16); // [25:16] tmds_clk_pttn[19:10]
		data32 |= (0 << 0);  // [ 9: 0] tmds_clk_pttn[ 9: 0]
		hdmitx21_wr_reg(HDMITX_TOP_TMDS_CLK_PTTN_01, data32);

		data32 = 0;
		data32 |= (0x3ff << 16); // [25:16] tmds_clk_pttn[39:30]
		data32 |= (0x3ff << 0);  // [ 9: 0] tmds_clk_pttn[29:20]
		hdmitx21_wr_reg(HDMITX_TOP_TMDS_CLK_PTTN_23, data32);
	} else {
		hdmitx21_wr_reg(HDMITX_TOP_TMDS_CLK_PTTN_01, 0x001f001f);
		hdmitx21_wr_reg(HDMITX_TOP_TMDS_CLK_PTTN_23, 0x001f001f);
	}
	hdmitx21_wr_reg(HDMITX_TOP_TMDS_CLK_PTTN_CNTL, 0x1);
	// [14:12] tmds_sel: 0=output 0; 1=output normal data;
	//                   2=output PRBS; 4=output shift pattn
	// [11: 8] shift_pttn
	// [ 4: 0] prbs_pttn
	data32 = 0;
	data32 |= (1 << 12);
	data32 |= (0 << 8);
	data32 |= (0 << 0);
	hdmitx21_wr_reg(HDMITX_TOP_BIST_CNTL, data32);

	if (div40)
		hdmitx21_wr_reg(HDMITX_TOP_TMDS_CLK_PTTN_CNTL, 0x2);
}

static void hdmitx_set_div40(bool div40)
{
	hdmitx_set_scdc_div40(div40);
	set_top_div40(div40);
	hdmitx21_wr_reg(SCRCTL_IVCTX, (1 << 5) | !!div40);
}

#define NUM_INT_VSYNC   INT_VEC_VIU1_VSYNC

/*Pixel bit width: 4=24-bit; 5=30-bit; 6=36-bit; 7=48-bit.
 *Pixel format: 0=RGB444; 1=YCbCr422; 2=YCbCr444; 3=YCbCr420.
 *Pixel range: 0=limited; 1=full.
 *Pixel format: 0=RGB444; 1=YCbCr422; 2=YCbCr444; 3=YCbCr420.
 *Pixel range: 0=limited; 1=full.
 *0:TMDS_CLK_rate=TMDS_Character_rate; 1:TMDS_CLK_rate=TMDS_Character_rate/4,
 *for TMDS_Character_rate>340Mcsc.
 */
static void config_hdmi21_tx(struct hdmitx_dev *hdev)
{
	struct hdmi_format_para *para = hdev->para;
	u8 color_depth = COLORDEPTH_24B; // Pixel bit width: 4=24-bit; 5=30-bit; 6=36-bit; 7=48-bit.
	// Pixel format: 0=RGB444; 1=YCbCr422; 2=YCbCr444; 3=YCbCr420.
	// Pixel format: 0=RGB444; 1=YCbCr422; 2=YCbCr444; 3=YCbCr420.
	u8 output_color_format = HDMI_COLORSPACE_YUV444;
	u8 vic = 16; // Video format identification code
	u32 active_pixels = 1920; // Number of active pixels per line
	u32 active_lines = 1080; // Number of active lines per field
	u8 scrambler_en = 0;
	u32 aud_n = 6144; // ACR N
	// 0=I2S 2-channel; 1=I2S 4 x 2-channel; 2=channel 0/1, 4/5 valid.
	// 2=audio sample packet; 7=one bit audio; 8=DST audio packet; 9=HBR audio packet.
	u8 audio_packet_type = 2;
	u32 data32;
	u8 data8;
	u8 dp_color_depth = 0;

	vic = para->timing.vic;
	if (vic)
		audio_packet_type = 2;
	else
		audio_packet_type = 9;
	if (para->tmds_clk > 340000) {
		para->scrambler_en = 1;
		para->tmds_clk_div40 = 1;
	} else {
		para->scrambler_en = 0;
		para->tmds_clk_div40 = 0;
	}
	color_depth = para->cd;
	output_color_format = para->cs;
	active_pixels = para->timing.h_active;
	active_lines = para->timing.v_active;
	scrambler_en = para->scrambler_en;
	dp_color_depth = (output_color_format == HDMI_COLORSPACE_YUV422) ?
				COLORDEPTH_24B : color_depth;

	pr_info("configure hdmitx21\n");
	hdmitx21_wr_reg(HDMITX_TOP_SW_RESET, 0);
	hdmitx_set_div40(para->tmds_clk_div40);

	//--------------------------------------------------------------------------
	// Glitch-filter HPD and RxSense
	//--------------------------------------------------------------------------
	// [31:28] rxsense_glitch_width: Filter out glitch width <= hpd_glitch_width
	// [27:16] rxsense_valid_width: Filter out signal stable width <= hpd_valid_width*1024
	// [15:12] hpd_glitch_width: Filter out glitch width <= hpd_glitch_width
	// [11: 0] hpd_valid_width: Filter out signal stable width <= hpd_valid_width*1024
	data32 = 0;
	data32 |= (8 << 28);
	data32 |= (0 << 16);
	data32 |= (7 << 12);
	data32 |= (0 << 0);
	hdmitx21_wr_reg(HDMITX_TOP_HPD_FILTER,    data32);

	//-------------
	//config video
	//-------------
	hdmi_drm_infoframe_set(NULL);
	hdmi_vend_infoframe_set(NULL);

	data8 = 0;
	data8 |= (dp_color_depth & 0x03); // [1:0]color depth. 00:8bpp;01:10bpp;10:12bpp;11:16bpp
	data8 |= (((dp_color_depth != 4) ? 1 : 0) << 7);  // [7]  deep color enable bit
	hdmitx21_wr_reg(P2T_CTRL_IVCTX, data8);
	hdmitx21_set_reg_bits(AON_CYP_CTL_IVCTX, 2, 0, 2);
	data32 = 0;
	data32 |= (1 << 5);  // [  5] reg_hdmi2_on
	data32 |= (scrambler_en & 0x01 << 0);  // [ 0] scrambler_en.
	hdmitx21_wr_reg(SCRCTL_IVCTX, data32 & 0xff);

	hdmitx21_wr_reg(CLK_DIV_CNTRL_IVCTX, 0x1);
	//hdmitx21_wr_reg(H21TXSB_PKT_PRD_IVCTX, 0x1);
	//hdmitx21_wr_reg(HOST_CTRL2_IVCTX, 0x80); //INT active high
	hdmitx21_wr_reg(CLKPWD_IVCTX, 0xf4);
	hdmitx21_wr_reg(SOC_FUNC_SEL_IVCTX, 0x01);
	//hdmitx21_wr_reg(SYS_MISC_IVCTX, 0x00); config same with default
	//hdmitx21_wr_reg(DIPT_CNTL_IVCTX, 0x06); config same with default
	hdmitx21_wr_reg(TEST_TXCTRL_IVCTX, 0x02); //[1] enable hdmi
	//hdmitx21_wr_reg(TX_ZONE_CTL4_IVCTX, 0x04); config same with default
	hdmitx21_wr_reg(CLKRATIO_IVCTX, 0x8a);

	//---------------
	//config vp core
	//---------------
	//some common register are configuration here TODO why config this value
	hdmitx21_wr_reg(VP_CMS_CSC0_MULTI_CSC_CONFIG_IVCTX, 0x00);
	hdmitx21_wr_reg((VP_CMS_CSC0_MULTI_CSC_CONFIG_IVCTX + 1), 0x08);
	hdmitx21_wr_reg(VP_CMS_CSC1_MULTI_CSC_CONFIG_IVCTX, 0x00);
	hdmitx21_wr_reg((VP_CMS_CSC1_MULTI_CSC_CONFIG_IVCTX + 1), 0x08);
	if (output_color_format == HDMI_COLORSPACE_RGB) {
		hdmitx21_wr_reg(VP_CMS_CSC0_MULTI_CSC_CONFIG_IVCTX, 0x65);
		hdmitx21_wr_reg((VP_CMS_CSC0_MULTI_CSC_CONFIG_IVCTX + 1), 0x08);
	}
	// [5:4] disable_lsbs_cr / [3:2] disable_lsbs_cb / [1:0] disable_lsbs_y
	// 0=12bit; 1=10bit(disable 2-LSB), 2=8bit(disable 4-LSB), 3=6bit(disable 6-LSB)
	data8 = 0;
	data8 |= ((6 - color_depth) << 4);
	data8 |= ((6 - color_depth) << 2);
	data8 |= ((6 - color_depth) << 0);
	hdmitx21_wr_reg(VP_INPUT_MASK_IVCTX, data8);

	hdmitx21_wr_reg(VP_CMS_CSC1_C444_C422_CONFIG_IVCTX, 0x00);

	// Output 422
	if (output_color_format == HDMI_COLORSPACE_YUV422) {
		// [11: 9] select_cr: 0=y; 1=cb; 2=Cr; 3={cr[11:4],cb[7:4]};
		//                    4={cr[3:0],y[3:0],cb[3:0]}; 5={y[3:0],cr[3:0],cb[3:0]};
		//                    6={cb[3:0],y[3:0],cr[3:0]}; 7={y[3:0],cb[3:0],cr[3:0]}.
		// [ 8: 6] select_cb: 0=y; 1=cb; 2=Cr; 3={cr[11:4],cb[7:4]};
		//                    4={cr[3:0],y[3:0],cb[3:0]}; 5={y[3:0],cr[3:0],cb[3:0]};
		//                    6={cb[3:0],y[3:0],cr[3:0]}; 7={y[3:0],cb[3:0],cr[3:0]}.
		// [ 5: 3] select_y : 0=y; 1=cb; 2=Cr; 3={y[11:4],cb[7:4]};
		//                    4={cb[3:0],cr[3:0],y[3:0]}; 5={cr[3:0],cb[3:0],y[3:0]};
		//                    6={y[3:0],cb[3:0],cr[3:0]}; 7={y[3:0],cr[3:0],cb[3:0]}.
		// [    2] reverse_cr
		// [    1] reverse_cb
		// [ 0] reverse_y
		data32 = 0;
		data32 |= (2 << 9);
		data32 |= (4 << 6);
		data32 |= (0 << 3);
		data32 |= (0 << 2);
		data32 |= (0 << 1);
		data32 |= (0 << 0);

		// [5:4] disable_lsbs_cr / [3:2] disable_lsbs_cb / [1:0] disable_lsbs_y
		// 0=12bit; 1=10bit(disable 2-LSB), 2=8bit(disable 4-LSB), 3=6bit(disable 6-LSB)
		data8 = 0;
		data8 |= (2 << 4);
		data8 |= (2 << 2);
		data8 |= (2 << 0);
		hdmitx21_wr_reg(VP_OUTPUT_MAPPING_IVCTX, data32 & 0xff);
		//mapping for yuv422 12bit
		hdmitx21_wr_reg(VP_OUTPUT_MAPPING_IVCTX + 1, (data32 >> 8) & 0xff);
		hdmitx21_wr_reg(VP_OUTPUT_MASK_IVCTX, data8);
	} else {
		// [11: 9] select_cr: 0=y; 1=cb; 2=Cr; 3={cr[11:4],cb[7:4]};
		//                    4={cr[3:0],y[3:0],cb[3:0]}; 5={y[3:0],cr[3:0],cb[3:0]};
		//                    6={cb[3:0],y[3:0],cr[3:0]}; 7={y[3:0],cb[3:0],cr[3:0]}.
		// [ 8: 6] select_cb: 0=y; 1=cb; 2=Cr; 3={cr[11:4],cb[7:4]};
		//                    4={cr[3:0],y[3:0],cb[3:0]}; 5={y[3:0],cr[3:0],cb[3:0]};
		//                    6={cb[3:0],y[3:0],cr[3:0]}; 7={y[3:0],cb[3:0],cr[3:0]}.
		// [ 5: 3] select_y : 0=y; 1=cb; 2=Cr; 3={y[11:4],cb[7:4]};
		//                    4={cb[3:0],cr[3:0],y[3:0]}; 5={cr[3:0],cb[3:0],y[3:0]};
		//                    6={y[3:0],cb[3:0],cr[3:0]}; 7={y[3:0],cr[3:0],cb[3:0]}.
		// [    2] reverse_cr
		// [    1] reverse_cb
		// [ 0] reverse_y
		data32 = 0;
		data32 |= (2 << 9);
		data32 |= (1 << 6);
		data32 |= (0 << 3);
		data32 |= (0 << 2);
		data32 |= (0 << 1);
		data32 |= (0 << 0);

		// [5:4] disable_lsbs_cr / [3:2] disable_lsbs_cb / [1:0] disable_lsbs_y
		// 0=12bit; 1=10bit(disable 2-LSB), 2=8bit(disable 4-LSB), 3=6bit(disable 6-LSB)
		data8 = 0;
		data8 |= (0 << 4);
		hdmitx21_wr_reg(VP_OUTPUT_MAPPING_IVCTX, data32 & 0xff);
		hdmitx21_wr_reg(VP_OUTPUT_MAPPING_IVCTX + 1, (data32 >> 8) & 0xff);
		hdmitx21_wr_reg(VP_OUTPUT_MASK_IVCTX, data8);
	}

	//---------------
	// config I2S
	//---------------
	//some common reister config,why config this value ?? TODO
	hdmitx21_wr_reg(AIP_HDMI2MHL_IVCTX, 0x00); //AIP
	hdmitx21_wr_reg(PKT_FILTER_0_IVCTX, 0x02); //PKT FILTER
	hdmitx21_wr_reg(ASRC_IVCTX, 0x00); //ASRC
	hdmitx21_wr_reg(VP_INPUT_SYNC_ADJUST_CONFIG_IVCTX, 0x01); //vp__

	data32 = 0;
	//AUDP_TXCTRL : [1] layout; [7] aud_mute_en
	hdmitx21_wr_reg(AUDP_TXCTRL_IVCTX, data32 & 0xff);
	//ACR_CTRL  bit[3]:reg_no_mclk_ctsgen_sel_pclk. bit[0]: make hw_cts_hw_sw_sel = 0
	hdmitx21_wr_reg(ACR_CTRL_IVCTX, 0x02);
	hdmitx21_wr_reg(N_SVAL1_IVCTX, (aud_n >> 0) & 0xff); //N_SVAL1
	hdmitx21_wr_reg(N_SVAL2_IVCTX, (aud_n >> 8) & 0xff); //N_SVAL2
	hdmitx21_wr_reg(N_SVAL3_IVCTX, (aud_n >> 16) & 0xff); //N_SVAL3

	//FREQ 00:mclk=128*Fs;01:mclk=256*Fs;10:mclk=384*Fs;11:mclk=512*Fs;...
	hdmitx21_wr_reg(FREQ_SVAL_IVCTX, 0);

	// [7:6] reg_tpi_spdif_sample_size: 0=Refer to stream header; 1=16-bit; 2=20-bit; 3=24-bit
	// [  4] reg_tpi_aud_mute
	data32 = 0;
	data32 |= (3 << 6);
	data32 |= (0 << 4);
	hdmitx21_wr_reg(TPI_AUD_CONFIG_IVCTX,    data32);

	if (0) { /* i2s_spdif */
		hdmitx21_wr_reg(I2S_IN_MAP_IVCTX, 0xE4); //I2S_IN_MAP
		hdmitx21_wr_reg(I2S_IN_CTRL_IVCTX, 0x20); //I2S_IN_CTRL [5] reg_cbit_order TODO
		hdmitx21_wr_reg(I2S_IN_SIZE_IVCTX, 0x0b); //I2S_IN_SIZE
		hdmitx21_wr_reg(I2S_CHST0_IVCTX, 0x15); //I2S_CHST0
		hdmitx21_wr_reg(I2S_CHST1_IVCTX, 0x55); //I2S_CHST1
		hdmitx21_wr_reg(I2S_CHST2_IVCTX, 0xfa); //I2S_CHST2
		hdmitx21_wr_reg(I2S_CHST3_IVCTX, 0x32); //I2S_CHST3
		hdmitx21_wr_reg(I2S_CHST4_IVCTX, 0x2b); //I2S_CHST4
	}

	//[7:4] I2S_EN SD0~SD3
	//[  3] DSD_EN
	//[  2] HBRA_EN
	//[  1] SPID_EN  Enable later in test.c, otherwise initial junk data will be sent
	//[ 0] PKT_EN
	data32 = 0;
	data32 |= (0 << 4);
	data32 |= (0 << 3);
	data32 |= ((audio_packet_type == HDMI_AUDIO_PACKET_HBR) << 2);
	data32 |= (0 << 1);
	data32 |= (0 << 0);
	hdmitx21_wr_reg(AUD_MODE_IVCTX, 0x2);  //AUD_MODE

	hdmitx21_wr_reg(AUD_EN_IVCTX, 0x03);           //AUD_EN

	//---------------
	// config Packet
	//---------------
	hdmitx21_wr_reg(VTEM_CTRL_IVCTX, 0x04); //[2] reg_vtem_ctrl

	//drm,emp pacekt
	hdmitx21_wr_reg(HDMITX_TOP_HS_INTR_CNTL, 0x010); //set TX hs_int h_cnt
	//tx_program_drm_emp(int_ext,active_lines,blank_lines,128);

	//--------------------------------------------------------------------------
	// Configure HDCP
	//--------------------------------------------------------------------------

	data32 = 0;
	data32 |= (0xef << 22); // [29:20] channel2 override
	data32 |= (0xcd << 12); // [19:10] channel1 override
	data32 |= (0xab << 2);  // [ 9: 0] channel0 override
	hdmitx21_wr_reg(HDMITX_TOP_SEC_VIDEO_OVR, data32);

	hdmitx21_wr_reg(HDMITX_TOP_HDCP14_MIN_SIZE, 0);
	hdmitx21_wr_reg(HDMITX_TOP_HDCP22_MIN_SIZE, 0);

	data32 = 0;
	data32 |= (active_lines << 16); // [30:16] cntl_vactive
	data32 |= (active_pixels << 0);  // [14: 0] cntl_hactive
	hdmitx21_wr_reg(HDMITX_TOP_HV_ACTIVE, data32);
	hdmitx21_set_reg_bits(PWD_SRST_IVCTX, 3, 1, 2);
	hdmitx21_set_reg_bits(PWD_SRST_IVCTX, 0, 1, 2);
	hdmitx21_set_reg_bits(TPI_SC_IVCTX, 1, 0, 1);
} /* config_hdmi21_tx */

#define GET_LOW8BIT(a)	((a) & 0xff)
#define GET_HIGH8BIT(a)	(((a) >> 8) & 0xff)
void hdmitx_set_drm_pkt(struct master_display_info_s *data)
{
	unsigned char drm_hb[3] = {0x87, 0x1, 26};
	unsigned char db[28] = {0x0};
	unsigned char *drm_db = &db[1];
	unsigned int hdr_transfer_feature = 0;
	unsigned int hdr_color_feature = 0;
	unsigned int hdr_mode = 0;

	/*
	 *hdr_color_feature: bit 23-16: color_primaries
	 *	1:bt709  0x9:bt2020
	 *hdr_transfer_feature: bit 15-8: transfer_characteristic
	 *	1:bt709 0xe:bt2020-10 0x10:smpte-st-2084 0x12:hlg(todo)
	 */
	if (!data)
		return;

	hdr_transfer_feature = (data->features >> 8) & 0xff;
	hdr_color_feature = (data->features >> 16) & 0xff;
	drm_db[1] = 0x0;
	drm_db[2] = GET_LOW8BIT(data->primaries[0][0]);
	drm_db[3] = GET_HIGH8BIT(data->primaries[0][0]);
	drm_db[4] = GET_LOW8BIT(data->primaries[0][1]);
	drm_db[5] = GET_HIGH8BIT(data->primaries[0][1]);
	drm_db[6] = GET_LOW8BIT(data->primaries[1][0]);
	drm_db[7] = GET_HIGH8BIT(data->primaries[1][0]);
	drm_db[8] = GET_LOW8BIT(data->primaries[1][1]);
	drm_db[9] = GET_HIGH8BIT(data->primaries[1][1]);
	drm_db[10] = GET_LOW8BIT(data->primaries[2][0]);
	drm_db[11] = GET_HIGH8BIT(data->primaries[2][0]);
	drm_db[12] = GET_LOW8BIT(data->primaries[2][1]);
	drm_db[13] = GET_HIGH8BIT(data->primaries[2][1]);
	drm_db[14] = GET_LOW8BIT(data->white_point[0]);
	drm_db[15] = GET_HIGH8BIT(data->white_point[0]);
	drm_db[16] = GET_LOW8BIT(data->white_point[1]);
	drm_db[17] = GET_HIGH8BIT(data->white_point[1]);
	drm_db[18] = GET_LOW8BIT(data->luminance[0]);
	drm_db[19] = GET_HIGH8BIT(data->luminance[0]);
	drm_db[20] = GET_LOW8BIT(data->luminance[1]);
	drm_db[21] = GET_HIGH8BIT(data->luminance[1]);
	drm_db[22] = GET_LOW8BIT(data->max_content);
	drm_db[23] = GET_HIGH8BIT(data->max_content);
	drm_db[24] = GET_LOW8BIT(data->max_frame_average);
	drm_db[25] = GET_HIGH8BIT(data->max_frame_average);

	/* SMPTE ST 2084 and (BT2020 or NON_STANDARD) */
	if (hdr_transfer_feature == T_SMPTE_ST_2084 &&
		hdr_color_feature == C_BT2020)
		hdr_mode = 1;
	else if (hdr_transfer_feature == T_SMPTE_ST_2084 &&
		hdr_color_feature != C_BT2020)
		hdr_mode = 2;

	/*HLG and BT2020*/
	if (hdr_color_feature == C_BT2020 &&
		(hdr_transfer_feature == T_BT2020_10 ||
		hdr_transfer_feature == T_HLG))
		hdr_mode = 3;

	switch (hdr_mode) {
	case 1:
		/*standard HDR*/
		drm_db[0] = 0x02; /* SMPTE ST 2084 */
		hdmi_drm_infoframe_rawset(drm_hb, db);
		hdmi_avi_infoframe_config(CONF_AVI_BT2020, SET_AVI_BT2020);
		break;
	case 2:
		/*non standard*/
		drm_db[0] = 0x02; /* no standard SMPTE ST 2084 */
		hdmi_drm_infoframe_rawset(drm_hb, db);
		hdmi_avi_infoframe_config(CONF_AVI_BT2020, CLR_AVI_BT2020);
		break;
	case 3:
		/*HLG*/
		drm_db[0] = 0x03;/* HLG is 0x03 */
		hdmi_drm_infoframe_rawset(drm_hb, db);
		hdmi_avi_infoframe_config(CONF_AVI_BT2020, SET_AVI_BT2020);
		break;
	case 0:
	default:
		/*other case*/
		hdmi_drm_infoframe_rawset(NULL, NULL);
		hdmi_avi_infoframe_config(CONF_AVI_BT2020, CLR_AVI_BT2020);
		break;
	}

	pr_info("%s[%d]\n", __func__, __LINE__);
}

void hdmitx_set_vsif_pkt(enum eotf_type type,
	enum mode_type tunnel_mode, struct dv_vsif_para *data)
{
	struct hdmitx_dev *hdev = &hdmitx_device;
	struct dv_vsif_para para = {0};
	unsigned char VEN_HB[3] = {0x81, 0x01};
	unsigned char db1[28] = {0x00};
	unsigned char db2[28] = {0x00};
	unsigned char *VEN_DB1 = &db1[1];
	unsigned char *VEN_DB2 = &db2[1];
	unsigned char len = 0;
	unsigned int vic = hdev->vic;
	unsigned int hdmi_vic_4k_flag = 0;

	if (hdev->RXCap.dv_info.ieeeoui != DV_IEEE_OUI)
		return;

	hdev->hdmi_current_eotf_type = type;
	hdev->hdmi_current_tunnel_mode = tunnel_mode;
	/*ver0 and ver1_15 and ver1_12bit with ll= 0 use hdmi 1.4b VSIF*/
	if (hdev->RXCap.dv_info.ver == 0 || (hdev->RXCap.dv_info.ver == 1 &&
		hdev->RXCap.dv_info.length == 0xE) || (hdev->RXCap.dv_info.ver == 1 &&
		hdev->RXCap.dv_info.length == 0xB && hdev->RXCap.dv_info.low_latency == 0)) {
		if (vic == HDMI_95_3840x2160p30_16x9 ||
		    vic == HDMI_94_3840x2160p25_16x9 ||
		    vic == HDMI_93_3840x2160p24_16x9 ||
		    vic == HDMI_98_4096x2160p24_256x135)
			hdmi_vic_4k_flag = 1;

		switch (type) {
		case EOTF_T_DOLBYVISION:
			len = 0x18;
			break;
		case EOTF_T_HDR10:
		case EOTF_T_SDR:
		case EOTF_T_NULL:
		default:
			len = 0x05;
			break;
		}

		VEN_HB[2] = len;
		VEN_DB1[0] = 0x03;
		VEN_DB1[1] = 0x0c;
		VEN_DB1[2] = 0x00;
		VEN_DB1[3] = 0x00;

		if (hdmi_vic_4k_flag) {
			VEN_DB1[3] = 0x20;
			if (vic == HDMI_95_3840x2160p30_16x9)
				VEN_DB1[4] = 0x1;
			else if (vic == HDMI_94_3840x2160p25_16x9)
				VEN_DB1[4] = 0x2;
			else if (vic == HDMI_93_3840x2160p24_16x9)
				VEN_DB1[4] = 0x3;
			else/*vic == HDMI_98_4096x2160p24_256x135*/
				VEN_DB1[4] = 0x4;
		}
		if (type == EOTF_T_DOLBYVISION) {
			hdmi_drm_infoframe_set(NULL);
			hdmi_vend_infoframe_rawset(VEN_HB, db1);
			/* Dolby Vision Source System-on-Chip Platform Kit Version 2.6:
			 * 4.4.1 Expected AVI-IF for Dolby Vision output, need BT2020 for DV
			 */
			hdmi_avi_infoframe_config(CONF_AVI_BT2020, SET_AVI_BT2020);/*BT2020*/
			if (tunnel_mode == RGB_8BIT) {
				hdmi_avi_infoframe_config(CONF_AVI_CS, HDMI_COLORSPACE_RGB);
				hdmi_avi_infoframe_config(CONF_AVI_Q01, RGB_RANGE_FUL);
			} else {
				hdmi_avi_infoframe_config(CONF_AVI_CS, HDMI_COLORSPACE_YUV422);
				hdmi_avi_infoframe_config(CONF_AVI_YQ01, YCC_RANGE_FUL);
			}
			hdev->dv_en = 1;
		} else {
			if (hdmi_vic_4k_flag)
				hdmi_vend_infoframe_rawset(VEN_HB, db1);
			else
				hdmi_vend_infoframe_set(NULL);
			hdmi_avi_infoframe_config(CONF_AVI_CS, hdev->para->cs);
			hdmi_avi_infoframe_config(CONF_AVI_Q01, RGB_RANGE_LIM);
			hdmi_avi_infoframe_config(CONF_AVI_YQ01, YCC_RANGE_LIM);
			hdmi_avi_infoframe_config(CONF_AVI_BT2020, CLR_AVI_BT2020);/*BT709*/
			hdev->dv_en = 0;
		}
	}
	/*ver1_12  with low_latency = 1 and ver2 use Dolby VSIF*/
	if (hdev->RXCap.dv_info.ver == 2 || (hdev->RXCap.dv_info.ver == 1 &&
		hdev->RXCap.dv_info.length == 0xB &&
		hdev->RXCap.dv_info.low_latency == 1) || type == EOTF_T_LL_MODE) {
		if (!data)
			data = &para;
		/*4k vsif package */
		if (vic == HDMI_95_3840x2160p30_16x9 ||
		    vic == HDMI_94_3840x2160p25_16x9 ||
		    vic == HDMI_93_3840x2160p24_16x9 ||
		    vic == HDMI_98_4096x2160p24_256x135)
			hdmi_vic_4k_flag = 1;

		switch (type) {
		case EOTF_T_DOLBYVISION:
		case EOTF_T_LL_MODE:
			len = 0x1b;
			break;
		case EOTF_T_HDR10:
		case EOTF_T_SDR:
		case EOTF_T_NULL:
		default:
			len = 0x5;
			break;
		}
		VEN_HB[2] = len;
		VEN_DB2[0] = 0x46;
		VEN_DB2[1] = 0xd0;
		VEN_DB2[2] = 0x00;
		VEN_DB2[3] = (data->vers.ver2.low_latency) |
			(data->vers.ver2.dobly_vision_signal << 1);
		VEN_DB2[4] = (data->vers.ver2.eff_tmax_PQ_hi)
			| (data->vers.ver2.auxiliary_MD_present << 6)
			| (data->vers.ver2.backlt_ctrl_MD_present << 7);
		VEN_DB2[5] = data->vers.ver2.eff_tmax_PQ_low;
		VEN_DB2[6] = data->vers.ver2.auxiliary_runmode;
		VEN_DB2[7] = data->vers.ver2.auxiliary_runversion;
		VEN_DB2[8] = data->vers.ver2.auxiliary_debug0;

		/*Dolby Vision standard case*/
		if (type == EOTF_T_DOLBYVISION) {
			hdmi_drm_infoframe_set(NULL);
			hdmi_vend_infoframe_rawset(VEN_HB, db2);
			/* Dolby Vision Source System-on-Chip Platform Kit Version 2.6:
			 * 4.4.1 Expected AVI-IF for Dolby Vision output, need BT2020 for DV
			 */
			hdmi_avi_infoframe_config(CONF_AVI_BT2020, SET_AVI_BT2020);/*BT2020*/
			if (tunnel_mode == RGB_8BIT) {/*RGB444*/
				hdmi_avi_infoframe_config(CONF_AVI_CS, HDMI_COLORSPACE_RGB);
				hdmi_avi_infoframe_config(CONF_AVI_Q01, RGB_RANGE_FUL);
			} else {/*YUV422*/
				hdmi_avi_infoframe_config(CONF_AVI_CS, HDMI_COLORSPACE_YUV422);
				hdmi_avi_infoframe_config(CONF_AVI_YQ01, YCC_RANGE_FUL);
			}
			hdev->dv_en = 1;
		}
		/*Dolby Vision low-latency case*/
		else if  (type == EOTF_T_LL_MODE) {
			hdmi_drm_infoframe_set(NULL);
			hdmi_vend_infoframe_rawset(VEN_HB, db2);
			/* Dolby vision HDMI Signaling Case25,
			 * UCD323 not declare bt2020 colorimetry,
			 * need to forcely send BT.2020
			 */
			hdmi_avi_infoframe_config(CONF_AVI_BT2020,
				SET_AVI_BT2020);/*BT2020*/
			if (tunnel_mode == RGB_10_12BIT) {/*10/12bit RGB444*/
				hdmi_avi_infoframe_config(CONF_AVI_CS, HDMI_COLORSPACE_RGB);
				hdmi_avi_infoframe_config(CONF_AVI_Q01, RGB_RANGE_LIM);
			} else if (tunnel_mode == YUV444_10_12BIT) {
				/*10/12bit YUV444*/
				hdmi_avi_infoframe_config(CONF_AVI_CS, HDMI_COLORSPACE_YUV444);
				hdmi_avi_infoframe_config(CONF_AVI_YQ01, YCC_RANGE_LIM);
			} else {/*YUV422*/
				hdmi_avi_infoframe_config(CONF_AVI_CS, HDMI_COLORSPACE_YUV422);
				hdmi_avi_infoframe_config(CONF_AVI_YQ01, YCC_RANGE_LIM);
			}
			hdev->dv_en = 1;
		} else { /*SDR case*/
			if (hdmi_vic_4k_flag) {
				VEN_DB1[0] = 0x03;
				VEN_DB1[1] = 0x0c;
				VEN_DB1[2] = 0x00;
				hdmi_vend_infoframe_rawset(VEN_HB, db1);
			} else {
				hdmi_vend_infoframe_set(NULL);
			}
			hdmi_avi_infoframe_config(CONF_AVI_CS, hdev->para->cs);
			hdmi_avi_infoframe_config(CONF_AVI_Q01, RGB_RANGE_LIM);
			hdmi_avi_infoframe_config(CONF_AVI_YQ01, YCC_RANGE_LIM);
			hdmi_avi_infoframe_config(CONF_AVI_BT2020, CLR_AVI_BT2020);/*BT709*/
			hdev->dv_en = 0;
		}
	}
	hdmitx21_dither_config(hdev);
	pr_info("%s[%d]\n", __func__, __LINE__);
}

void hdmitx_set_hdr10plus_pkt(unsigned int flag,
	struct hdr10plus_para *data)
{
	unsigned char VEN_HB[3] = {0x81, 0x01, 0x1b};
	unsigned char VEN_DB[27] = {0x00};

	if (!data || !flag) {
		pr_info("%s: null vsif\n", __func__);
		hdmi_vend_infoframe_set(NULL);
		hdmi_avi_infoframe_config(CONF_AVI_BT2020, CLR_AVI_BT2020);
		return;
	}

	VEN_DB[0] = 0x8b;
	VEN_DB[1] = 0x84;
	VEN_DB[2] = 0x90;

	VEN_DB[3] = ((data->application_version & 0x3) << 6) |
		 ((data->targeted_max_lum & 0x1f) << 1);
	VEN_DB[4] = data->average_maxrgb;
	VEN_DB[5] = data->distribution_values[0];
	VEN_DB[6] = data->distribution_values[1];
	VEN_DB[7] = data->distribution_values[2];
	VEN_DB[8] = data->distribution_values[3];
	VEN_DB[9] = data->distribution_values[4];
	VEN_DB[10] = data->distribution_values[5];
	VEN_DB[11] = data->distribution_values[6];
	VEN_DB[12] = data->distribution_values[7];
	VEN_DB[13] = data->distribution_values[8];
	VEN_DB[14] = ((data->num_bezier_curve_anchors & 0xf) << 4) |
		((data->knee_point_x >> 6) & 0xf);
	VEN_DB[15] = ((data->knee_point_x & 0x3f) << 2) |
		((data->knee_point_y >> 8) & 0x3);
	VEN_DB[16] = data->knee_point_y  & 0xff;
	VEN_DB[17] = data->bezier_curve_anchors[0];
	VEN_DB[18] = data->bezier_curve_anchors[1];
	VEN_DB[19] = data->bezier_curve_anchors[2];
	VEN_DB[20] = data->bezier_curve_anchors[3];
	VEN_DB[21] = data->bezier_curve_anchors[4];
	VEN_DB[22] = data->bezier_curve_anchors[5];
	VEN_DB[23] = data->bezier_curve_anchors[6];
	VEN_DB[24] = data->bezier_curve_anchors[7];
	VEN_DB[25] = data->bezier_curve_anchors[8];
	VEN_DB[26] = ((data->graphics_overlay_flag & 0x1) << 7) |
		((data->no_delay_flag & 0x1) << 6);

	hdmi_vend_infoframe_rawset(VEN_HB, VEN_DB);
	hdmi_avi_infoframe_config(CONF_AVI_BT2020, SET_AVI_BT2020);
}

static void hdmitx_set_phy(struct hdmitx_dev *hdev)
{
	if (!hdev)
		return;

	switch (hdev->vic) {
	case HDMI_96_3840x2160p50_16x9:
	case HDMI_97_3840x2160p60_16x9:
	case HDMI_101_4096x2160p50_256x135:
	case HDMI_102_4096x2160p60_256x135:
		if (hdev->para->cs == HDMI_COLORSPACE_YUV420 && hdev->para->cd == COLORDEPTH_24B)
			hdmitx_set_phypara(HDMI_PHYPARA_3G);
		else
			hdmitx_set_phypara(HDMI_PHYPARA_6G);
		break;
	case HDMI_93_3840x2160p24_16x9:
	case HDMI_103_3840x2160p24_64x27:
	case HDMI_94_3840x2160p25_16x9:
	case HDMI_104_3840x2160p25_64x27:
	case HDMI_95_3840x2160p30_16x9:
	case HDMI_105_3840x2160p30_64x27:
	case HDMI_98_4096x2160p24_256x135:
	case HDMI_99_4096x2160p25_256x135:
	case HDMI_100_4096x2160p30_256x135:
		if (hdev->para->cs == HDMI_COLORSPACE_YUV422 || hdev->para->cd == COLORDEPTH_24B)
			hdmitx_set_phypara(HDMI_PHYPARA_3G);
		else
			hdmitx_set_phypara(HDMI_PHYPARA_4p5G);
		break;
	case HDMI_16_1920x1080p60_16x9:
	case HDMI_31_1920x1080p50_16x9:
	case HDMI_40_1920x1080i100_16x9:
	case HDMI_46_1920x1080i120_16x9:
	case HDMI_41_1280x720p100_16x9:
	case HDMI_47_1280x720p120_16x9:
		hdmitx_set_phypara(HDMI_PHYPARA_DEF);
		break;
	default:
		hdmitx_set_phypara(HDMI_PHYPARA_270M);
		break;
	}
	debug("hdmitx phy setting done\n");
}

static void hdmitx_set_hw(struct hdmitx_dev *hdev)
{
	/* --------------------------------------------------------*/
	/* Set up HDMI*/
	/* --------------------------------------------------------*/
	config_hdmi21_tx(hdev);
}

bool hdmitx_dv_en(struct hdmitx_dev *hdev)
{
	return hdev->dv_en;
}

void hdmitx21_dither_config(struct hdmitx_dev *hdev)
{
	struct hdmi_format_para *para = hdev->para;

	if (para->cd == COLORDEPTH_24B && hdmitx_dv_en(hdev) == 0)
		hd21_set_reg_bits(VPU_HDMI_DITH_CNTL, 1, 4, 1);
	else
		hd21_set_reg_bits(VPU_HDMI_DITH_CNTL, 0, 4, 1);
}

