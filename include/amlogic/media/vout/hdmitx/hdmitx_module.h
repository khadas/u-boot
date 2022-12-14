/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __HDMITX_MODULE_H__
#define __HDMITX_MODULE_H__
#ifndef __HDMITX_MODULE20_H__
#define __HDMITX_MODULE20_H__

#include "hdmi_common.h"
#include "hdmitx_ext.h"

struct hdmitx_dev {
	unsigned char rx_edid[512]; /* some RX may exceeds 256Bytes */
	struct {
		int (*get_hpd_state)(void);
		int (*read_edid)(unsigned char *buf, unsigned char addr,
				 unsigned char blk_no);
		void (*turn_off)(void);
		void (*list_support_modes)(void);
		void (*dump_regs)(void);
		void (*test_bist)(unsigned int mode);
		void (*test_prbs)(void);
		void (*set_div40)(bool div40);
		void (*output_blank)(unsigned int blank);
	} hwop;
	unsigned char rawedid[EDID_BLK_SIZE * EDID_BLK_NO];
	struct rx_cap RXCap;
	struct hdmi_format_para *para;
	enum hdmi_vic vic;
	unsigned int frac_rate_policy;
	unsigned int mode420;
	unsigned int dc30;
	enum eotf_type hdmi_current_eotf_type;
	enum mode_type hdmi_current_tunnel_mode;
	/* Add dongle_mode, clock, phy may be different from mbox */
	unsigned int dongle_mode;
	unsigned char limit_res_1080p;
};

struct hdmitx_dev *hdmitx_get_hdev(void);
struct hdmi_format_para *hdmi_get_fmt_paras(enum hdmi_vic vic);
enum hdmi_vic hdmi_get_fmt_vic(char const *name);
void hdmi_parse_attr(struct hdmi_format_para *para, char const *name);
int hdmitx_edid_VIC_support(enum hdmi_vic vic);
enum hdmi_vic hdmitx_edid_vic_tab_map_vic(const char *disp_mode);
const char *hdmitx_edid_vic_tab_map_string(enum hdmi_vic vic);
const char *hdmitx_edid_vic_to_string(enum hdmi_vic vic);
bool hdmitx_edid_check_valid_mode(struct hdmitx_dev *hdev,
	struct hdmi_format_para *para);
enum hdmi_vic hdmitx_edid_get_VIC(struct hdmitx_dev *hdev,
	const char *disp_mode, char force_flag);
bool edid_parsing_ok(struct hdmitx_dev *hdev);
bool is_dolby_enabled(void);
bool is_tv_support_dv(struct hdmitx_dev *hdev);
bool is_dv_preference(struct hdmitx_dev *hdev);
bool is_hdr_preference(struct hdmitx_dev *hdev);
void dolbyvision_scene_process(hdmi_data_t *hdmi_data,
	scene_output_info_t *output_info);
void sdr_scene_process(hdmi_data_t *hdmi_data,
	scene_output_info_t *output_info);
void hdr_scene_process(struct input_hdmi_data *hdmi_data,
	scene_output_info_t *output_info);

void get_hdmi_data(struct hdmitx_dev *hdev, hdmi_data_t *data);
bool pre_process_str(char *name);
struct hdmi_format_para *hdmi_tst_fmt_name(char const *name, char const *attr);
bool is_support_4k(void);
bool is_supported_mode_attr(hdmi_data_t *hdmi_data, char *mode_attr);
bool hdmitx_chk_mode_attr_sup(hdmi_data_t *hdmi_data, char *mode, char *attr);
int get_ubootenv_dv_type(void);
int hdmi_tx_set(struct hdmitx_dev *hdev);
/* Parsing RAW EDID data from edid to pRXCap */
unsigned int hdmi_edid_parsing(unsigned char *edid, struct rx_cap *prxcap);
struct hdmi_format_para *hdmi_match_dtd_paras(struct dtd *t);
bool hdmimode_is_interlaced(enum hdmi_vic vic);
void hdmitx_ddc_init(void);
void hdmitx_set_clk(struct hdmitx_dev *hdev);
void hdmitx_turnoff(void);
int hdmitx_get_hpd_state(void);
void hdmitx_prbs(void);
unsigned int hdmitx_rd_check_reg(unsigned int addr, unsigned int exp_data,
	unsigned int mask);
void hdmitx_hdcp_init(void);
void hdmitx_set_phypara(enum hdmi_phy_para mode);
void hdmitx_test_prbs(void);
void hdmitx_set_div40(bool div40);

struct hdr_info *hdmitx_get_rx_hdr_info(void);
void hdmitx_set_drm_pkt(struct master_display_info_s *data);
bool hdmitx_find_vendor(struct hdmitx_dev *hdev);
bool hdmitx_find_vendor_null_pkt(struct hdmitx_dev *hdev);

void hdmitx_set_vsif_pkt(enum eotf_type type, enum mode_type tunnel_mode,
	struct dv_vsif_para *data);
void hdmitx_set_hdr10plus_pkt(unsigned int flag,
	struct hdr10plus_para *data);
bool is_hdmi_mode(char *mode);

/* the hdmitx output limits to 1080p */
bool is_hdmitx_limited_1080p(void);

#ifndef printk
#define printk printf
#endif
#ifndef pr_info
#define pr_info printf
#endif

#define hdmitx_debug() /* printf("hd: %s[%d]\n", __func__, __LINE__) */
#endif
#endif
