// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <asm/byteorder.h>
#ifdef CONFIG_AML_HDMITX20
#include <amlogic/media/vout/hdmitx/hdmitx.h>
#else
#include <amlogic/media/vout/hdmitx21/hdmitx.h>
#endif
#include <amlogic/media/dv/dolby_vision.h>
#include <asm/arch/cpu.h>
#include <amlogic/cpu_id.h>

#define DV_MODE_720P50HZ                "720p50hz"
#define DV_MODE_720P                    "720p60hz"
#define DV_MODE_1080P24HZ               "1080p24hz"
#define DV_MODE_1080P50HZ               "1080p50hz"
#define DV_MODE_1080P                   "1080p60hz"
#define DV_MODE_4K2K24HZ                "2160p24hz"
#define DV_MODE_4K2K25HZ                "2160p25hz"
#define DV_MODE_4K2K30HZ                "2160p30hz"
#define DV_MODE_4K2K50HZ                "2160p50hz"
#define DV_MODE_4K2K60HZ                "2160p60hz"
#define DV_MODE_LIST_SIZE               10

#define MODE_480I                       "480i60hz"
#define MODE_480P                       "480p60hz"
#define MODE_480CVBS                    "480cvbs"
#define MODE_576I                       "576i50hz"
#define MODE_576P                       "576p50hz"
#define MODE_576CVBS                    "576cvbs"
#define MODE_720P50HZ                   "720p50hz"
#define MODE_720P                       "720p60hz"
#define MODE_768P                       "768p60hz"
#define MODE_1080P24HZ                  "1080p24hz"
#define MODE_1080P25HZ                  "1080p25hz"
#define MODE_1080P30HZ                  "1080p30hz"
#define MODE_1080I50HZ                  "1080i50hz"
#define MODE_1080P50HZ                  "1080p50hz"
#define MODE_1080I                      "1080i60hz"
#define MODE_1080P                      "1080p60hz"
#define MODE_4K2K24HZ                   "2160p24hz"
#define MODE_4K2K25HZ                   "2160p25hz"
#define MODE_4K2K30HZ                   "2160p30hz"
#define MODE_4K2K50HZ                   "2160p50hz"
#define MODE_4K2K60HZ                   "2160p60hz"
#define MODE_4K2KSMPTE                  "smpte24hz"
#define MODE_4K2KSMPTE30HZ              "smpte30hz"
#define MODE_4K2KSMPTE50HZ              "smpte50hz"
#define MODE_4K2KSMPTE60HZ              "smpte60hz"
#define MODE_PANEL                      "panel"
#define MODE_PAL_M                      "pal_m"
#define MODE_PAL_N                      "pal_n"
#define MODE_NTSC_M                     "ntsc_m"

#define COLOR_YCBCR444_12BIT             "444,12bit"
#define COLOR_YCBCR444_10BIT             "444,10bit"
#define COLOR_YCBCR444_8BIT              "444,8bit"
#define COLOR_YCBCR422_12BIT             "422,12bit"
#define COLOR_YCBCR422_10BIT             "422,10bit"
#define COLOR_YCBCR422_8BIT              "422,8bit"
#define COLOR_YCBCR420_12BIT             "420,12bit"
#define COLOR_YCBCR420_10BIT             "420,10bit"
#define COLOR_YCBCR420_8BIT              "420,8bit"
#define COLOR_RGB_12BIT                  "rgb,12bit"
#define COLOR_RGB_10BIT                  "rgb,10bit"
#define COLOR_RGB_8BIT                   "rgb,8bit"

static const char *DISPLAY_MODE_LIST[] = {
	MODE_480I,
	MODE_480P,
	/* MODE_480CVBS, */
	MODE_576I,
	MODE_576P,
	/* MODE_576CVBS, */
	MODE_720P,
	MODE_720P50HZ,
	MODE_1080P24HZ,
	MODE_1080P25HZ,
	MODE_1080P30HZ,
	MODE_1080I50HZ,
	MODE_1080P50HZ,
	MODE_1080I,
	MODE_1080P,
	MODE_4K2K24HZ,
	MODE_4K2K25HZ,
	MODE_4K2K30HZ,
	MODE_4K2K50HZ,
	MODE_4K2K60HZ,
	MODE_4K2KSMPTE,
	MODE_4K2KSMPTE30HZ,
	MODE_4K2KSMPTE50HZ,
	MODE_4K2KSMPTE60HZ
	/* MODE_768P,  */
	/* MODE_PANEL, */
	/* MODE_PAL_M, */
	/* MODE_PAL_N, */
	/* MODE_NTSC_M, */
};

/* for check hdr 4k support or not */
static const char * const MODE_4K_LIST[] = {
	MODE_4K2K60HZ,
	MODE_4K2K50HZ,
};

/* for check hdr non-4k support or not */
static const char * const MODE_NON4K_LIST[] = {
	MODE_1080P,
	MODE_1080P50HZ,
	MODE_1080I,
	MODE_1080I50HZ,
	MODE_720P,
	MODE_720P50HZ,
	MODE_576P,
	MODE_480P,
	MODE_576I,
	MODE_480I,
};

static const char *MODE_RESOLUTION_FIRST[] = {
	MODE_480I,
	MODE_576I,
	MODE_480P,
	MODE_576P,
	MODE_720P50HZ,
	MODE_720P,
	MODE_1080I50HZ,
	MODE_1080I,
	MODE_1080P50HZ,
	MODE_1080P,
	MODE_4K2K24HZ,
	MODE_4K2K25HZ,
	MODE_4K2K30HZ,
	MODE_4K2K50HZ,
	MODE_4K2K60HZ
};

static const char *MODE_FRAMERATE_FIRST[] = {
	MODE_480I,
	MODE_576I,
	MODE_480P,
	MODE_576P,
	MODE_720P50HZ,
	MODE_720P,
	MODE_1080I50HZ,
	MODE_1080I,
	MODE_4K2K24HZ,
	MODE_4K2K25HZ,
	MODE_4K2K30HZ,
	MODE_1080P50HZ,
	MODE_1080P,
	MODE_4K2K50HZ,
	MODE_4K2K60HZ
};

/* this is prior selected list of
 * 4k2k50hz, 4k2k60hz smpte50hz, smpte60hz
 */
static const char *COLOR_ATTRIBUTE_LIST1[] = {
	COLOR_YCBCR420_10BIT,
	COLOR_YCBCR422_12BIT,
	COLOR_YCBCR420_8BIT,
	COLOR_YCBCR444_8BIT,
	COLOR_RGB_8BIT
};

/* this is prior selected list  of other display mode */
static const char *COLOR_ATTRIBUTE_LIST2[] = {
	COLOR_YCBCR444_10BIT,
	COLOR_YCBCR422_12BIT,
	COLOR_RGB_10BIT,
	COLOR_YCBCR444_8BIT,
	COLOR_RGB_8BIT
};

/* this is prior selected list  of Low Power Mode
 * 4k2k50hz, 4k2k60hz smpte50hz, smpte60hz
 */
static const char *COLOR_ATTRIBUTE_LIST3[] = {
	COLOR_YCBCR420_8BIT,
	COLOR_YCBCR420_10BIT,
	COLOR_YCBCR422_8BIT,
	COLOR_YCBCR422_10BIT,
	COLOR_YCBCR444_8BIT,
	COLOR_RGB_8BIT,
	COLOR_YCBCR420_12BIT,
	COLOR_YCBCR422_12BIT
};

/* this is prior selected list of
 * Low Power Mode other display mode
 */
static const char *COLOR_ATTRIBUTE_LIST4[] = {
	COLOR_YCBCR444_8BIT,
	COLOR_YCBCR422_8BIT,
	COLOR_RGB_8BIT,
	COLOR_YCBCR444_10BIT,
	COLOR_YCBCR422_10BIT,
	COLOR_RGB_10BIT,
	COLOR_YCBCR444_12BIT,
	COLOR_YCBCR422_12BIT,
	COLOR_RGB_12BIT
};

/* this is prior selected list of HDR non 4k colorspace */
static const char * const HDR_NON4K_COLOR_ATTRIBUTE_LIST[] = {
	COLOR_YCBCR444_10BIT,
	COLOR_YCBCR422_12BIT,
	COLOR_RGB_10BIT,
	COLOR_YCBCR444_12BIT,
	COLOR_RGB_12BIT,
};

/* this is prior selected list of HDR 4k colorspace(2160p60hz/2160p50hz) */
static const char * const HDR_4K_COLOR_ATTRIBUTE_LIST[] = {
	COLOR_YCBCR420_10BIT,
	COLOR_YCBCR422_12BIT,
};

/* support format lists */
static const char *disp_mode_t[] = {
	"480i60hz", /* 16:9 */
	"576i50hz",
	"480p60hz",
	"576p50hz",
	"720p60hz",
	"1080i60hz",
	"1080p60hz",
	"1080p120hz",
	"720p50hz",
	"1080i50hz",
	"1080p30hz",
	"1080p50hz",
	"1080p25hz",
	"1080p24hz",
	"2560x1080p50hz",
	"2560x1080p60hz",
	"2160p30hz",
	"2160p25hz",
	"2160p24hz",
	"smpte24hz",
	"smpte25hz",
	"smpte30hz",
	"smpte50hz",
	"smpte60hz",
	"2160p50hz",
	"2160p60hz",
	/* VESA modes */
	"640x480p60hz",
	"800x480p60hz",
	"800x600p60hz",
	"852x480p60hz",
	"854x480p60hz",
	"1024x600p60hz",
	"1024x768p60hz",
	"1152x864p75hz",
	"1280x600p60hz",
	"1280x768p60hz",
	"1280x800p60hz",
	"1280x960p60hz",
	"1280x1024p60hz",
	"1360x768p60hz",
	"1366x768p60hz",
	"1400x1050p60hz",
	"1440x900p60hz",
	"1440x2560p60hz",
	"1600x900p60hz",
	"1600x1200p60hz",
	"1680x1050p60hz",
	"1920x1200p60hz",
	"2160x1200p90hz",
	"2560x1440p60hz",
	"2560x1600p60hz",
	"3440x1440p60hz",
	"2400x1200p90hz",
	"3840x1080p60hz",
	NULL
};

static bool is_best_outputmode(void)
{
	char *is_bestmode = env_get("is.bestmode");

	return !is_bestmode || (strcmp(is_bestmode, "true") == 0);
}

static bool is_framerate_priority(void)
{
	char *framerate_priority = env_get("framerate_priority");

	return !framerate_priority || (strcmp(framerate_priority, "true") == 0);
}

/* import from kernel */
static inline bool package_id_is(unsigned int id)
{
	return get_cpu_id().package_id == id;
}

static inline bool is_meson_gxl_cpu(void)
{
	return get_cpu_id().family_id == MESON_CPU_MAJOR_ID_GXL;
}

static inline bool is_meson_gxl_package_805X(void)
{
	return is_meson_gxl_cpu() && package_id_is(0x30);
}

static inline bool is_meson_gxl_package_805Y(void)
{
	return is_meson_gxl_cpu() && package_id_is(0xb0);
}

/* below items has feature limited, may need extra judgement */
bool is_hdmitx_limited_1080p(void)
{
#ifdef CONFIG_AML_HDMITX20
	struct hdmitx_dev *hdev = hdmitx_get_hdev();
#else
	struct hdmitx_dev *hdev = get_hdmitx21_device();
#endif

	if (is_meson_gxl_package_805X())
		return true;
	else if (is_meson_gxl_package_805Y())
		return true;
	else if (hdev->limit_res_1080p == 1)
		return true;
	else
		return false;
}

bool is_support_4k(void)
{
	if (is_hdmitx_limited_1080p())
		return false;
	return true;
}

static bool is_support_deepcolor(void)
{
	return true;
}

static bool is_low_powermode(void)
{
	return false;
}

static int is_4k50_fmt(char *mode)
{
	int i;
	static char const *hdmi4k50[] = {
		"2160p50hz",
		"2160p60hz",
		"smpte50hz",
		"smpte60hz",
		NULL
	};

	for (i = 0; hdmi4k50[i]; i++) {
		if (strcmp(hdmi4k50[i], mode) == 0)
			return 1;
	}
	return 0;
}

static int get_hdr_priority(void)
{
	char *hdr_priority = env_get("hdr_priority");
	hdr_priority_e value = DOLBY_VISION_PRIORITY;

	if (hdr_priority) {
		if (strcmp(hdr_priority, "2") == 0)
			value = SDR_PRIORITY;
		else if (strcmp(hdr_priority, "1") == 0)
			value = HDR10_PRIORITY;
		else
			value = DOLBY_VISION_PRIORITY;
	} else {
		value = DOLBY_VISION_PRIORITY;
	}

	return (int)value;
}

bool is_tv_support_hdr(struct hdmitx_dev *hdev)
{
	struct hdr_info *hdr;
	struct hdr10_plus_info *hdr10p;

	if (!hdev)
		return false;
	hdr = &hdev->RXCap.hdr_info;
	hdr10p = &hdev->RXCap.hdr10plus_info;
	if (hdr->hdr_sup_eotf_smpte_st_2084 || hdr->hdr_sup_eotf_hlg)
		return true;
	if (hdr10p->ieeeoui == HDR10_PLUS_IEEE_OUI &&
		hdr10p->application_version != 0xFF)
		return true;
	return false;
}

bool is_tv_support_dv(struct hdmitx_dev *hdev)
{
	/*todo*/
	struct dv_info *dv;

	if (!hdev)
		return false;
	dv = &(hdev->RXCap.dv_info);

	if ((dv->ieeeoui != DV_IEEE_OUI) || (dv->block_flag != CORRECT))
		return false;
	return true;
}

bool is_dv_preference(struct hdmitx_dev *hdev)
{
	int hdr_priority = get_hdr_priority();

	if (!hdev)
		return false;
	if (is_dolby_enabled() &&
		hdr_priority == DOLBY_VISION_PRIORITY &&
		is_tv_support_dv(hdev))
		return true;
	else
		return false;
}

bool is_hdr_preference(struct hdmitx_dev *hdev)
{
	int hdr_priority = get_hdr_priority();

	if (!hdev)
		return false;
	if ((hdr_priority == DOLBY_VISION_PRIORITY ||
		hdr_priority == HDR10_PRIORITY) &&
		is_tv_support_hdr(hdev))
		return true;
	else
		return false;
}

int get_ubootenv_dv_type(void)
{
	char *dolby_status = NULL;

	dolby_status = env_get("dolby_status");

	if (!dolby_status) {
		printf("no ubootenv dolby_status\n");
		return DOLBY_VISION_DISABLE;
	}
	if (!strcmp(dolby_status, DOLBY_VISION_SET_STD))
		return DOLBY_VISION_STD_ENABLE;
	else if (!strcmp(dolby_status, DOLBY_VISION_SET_LL_YUV))
		return DOLBY_VISION_LL_YUV;
	else if (!strcmp(dolby_status, DOLBY_VISION_SET_LL_RGB))
		return DOLBY_VISION_LL_RGB;
	else
		return DOLBY_VISION_DISABLE;
}

bool is_dolby_enabled(void)
{
	if (get_ubootenv_dv_type() != DOLBY_VISION_DISABLE)
		return true;
	else
		return false;
}

static int resolve_resolution_value(const char *mode, int flag)
{
	bool validmode = false;
	int i;

	if (!mode)
		return -1;

	if (strlen(mode) != 0) {
		for (i = 0; i < ARRAY_SIZE(DISPLAY_MODE_LIST); i++) {
			if (strcmp(mode, DISPLAY_MODE_LIST[i]) == 0) {
				validmode = true;
				break;
			}
		}
	}
	if (!validmode) {
		printf("the resolveResolution mode [%s] is not valid\n", mode);
		return -1;
	}

	if (is_framerate_priority() && flag == FRAMERATE_PRIORITY) {
		for (i = 0; i < ARRAY_SIZE(MODE_FRAMERATE_FIRST); i++) {
			if (strcmp(mode, MODE_FRAMERATE_FIRST[i]) == 0)
				return i;
		}
	} else {
		for (i = 0; i < ARRAY_SIZE(MODE_RESOLUTION_FIRST); i++) {
			if (strcmp(mode, MODE_RESOLUTION_FIRST[i]) == 0)
				return i;
		}
	}

	return -1;
}

/* need update DV CAP */
static int update_dv_type(struct input_hdmi_data *hdmi_data)
{
	/* 1.read dolby vision mode from prop(env) */
	int type;
	struct dv_info *dv = NULL;

	if (!hdmi_data)
		return DOLBY_VISION_DISABLE;

	type = hdmi_data->ubootenv_dv_type;
	dv = &hdmi_data->prxcap->dv_info;

	/* 2.check tv support or not */
	if ((type == 1) && (dv->support_DV_RGB_444_8BIT == 1)) {
		return DOLBY_VISION_STD_ENABLE;
	} else if ((type == 2) && (dv->support_LL_YCbCr_422_12BIT == 1)) {
		return DOLBY_VISION_LL_YUV;
	} else if ((type == 3) &&
		((dv->support_LL_RGB_444_10BIT == 1) ||
		(dv->support_LL_RGB_444_12BIT == 1))) {
		return DOLBY_VISION_LL_RGB;
	} else if (type == 0) {
		return DOLBY_VISION_DISABLE;
	}

	/* 3.dolby vision best policy:
	 * STD->LL_YUV->LL_RGB for netflix request
	 * LL_YUV->STD->LL_RGB for dolby vision request
	 */
	printf("NOTE: DV type is changed!\n");
	if ((dv->support_DV_RGB_444_8BIT == 1) ||
	    (dv->support_LL_YCbCr_422_12BIT == 1)) {
		if (dv->support_DV_RGB_444_8BIT == 1)
			return DOLBY_VISION_STD_ENABLE;
		else if (dv->support_LL_YCbCr_422_12BIT == 1)
			return DOLBY_VISION_LL_YUV;
	} else if ((dv->support_LL_RGB_444_10BIT == 1) ||
		   (dv->support_LL_RGB_444_12BIT == 1)) {
		return DOLBY_VISION_LL_RGB;
	}

	return DOLBY_VISION_DISABLE;
}

static void update_dv_attr(struct input_hdmi_data *hdmi_data, char *dv_attr)
{
	int dv_type;
	struct dv_info *dv = NULL;
	if (!hdmi_data || !dv_attr)
		return;

	dv_type = hdmi_data->ubootenv_dv_type;
	dv = &hdmi_data->prxcap->dv_info;

	switch (dv_type) {
	case DOLBY_VISION_STD_ENABLE:
		strcpy(dv_attr, "444,8bit");
		break;
	case DOLBY_VISION_LL_YUV:
		strcpy(dv_attr, "422,12bit");
		break;
	case DOLBY_VISION_LL_RGB:
		if (dv->support_LL_RGB_444_12BIT == 1)
			strcpy(dv_attr, "444,12bit");
		else if (dv->support_LL_RGB_444_10BIT == 1)
			strcpy(dv_attr, "444,10bit");

		break;
	default:
		strcpy(dv_attr, "444,8bit");
		break;
	}

	printf("dv_type :%d dv_attr:%s", dv_type, dv_attr);
}

static void update_dv_displaymode(struct input_hdmi_data *hdmi_data,
	char *final_displaymode)
{
	char dv_displaymode[MODE_LEN] = {0};
	char cur_outputmode[MODE_LEN] = {0};
	int dv_type;
	struct dv_info *dv = NULL;

	if (!hdmi_data || !final_displaymode)
		return;
	dv_type = hdmi_data->ubootenv_dv_type;
	strcpy(cur_outputmode, hdmi_data->ubootenv_hdmimode);
	dv = &hdmi_data->prxcap->dv_info;
	if (dv->sup_2160p60hz == 1)
		strcpy(dv_displaymode, DV_MODE_4K2K60HZ);
	else
		strcpy(dv_displaymode, DV_MODE_4K2K30HZ);
	if (is_best_outputmode()) {
		if (!strcmp(dv_displaymode, DV_MODE_4K2K60HZ)) {
			if (dv_type == DOLBY_VISION_LL_RGB)
				strcpy(final_displaymode, DV_MODE_1080P);
			 else
				strcpy(final_displaymode, DV_MODE_4K2K60HZ);
		} else {
			if (!strcmp(dv_displaymode, DV_MODE_4K2K30HZ) ||
			    !strcmp(dv_displaymode, DV_MODE_4K2K25HZ) ||
			    !strcmp(dv_displaymode, DV_MODE_4K2K24HZ))
				strcpy(final_displaymode, DV_MODE_1080P);
			else
				strcpy(final_displaymode, dv_displaymode);
		}
	} else {
		/* if current disp_mode is outside of maximum dv disp_mode */
		if ((resolve_resolution_value(cur_outputmode, RESOLUTION_PRIORITY) >
		     resolve_resolution_value(dv_displaymode, RESOLUTION_PRIORITY)) ||
		    (strstr(cur_outputmode, "smpte") != NULL) ||
		    (strstr(cur_outputmode, "i") != NULL))
			strcpy(final_displaymode, dv_displaymode);
		else
			strcpy(final_displaymode, cur_outputmode);
	}

	printf("final_displaymode:%s, cur_outputmode:%s, dv_displaymode:%s",
	       final_displaymode, cur_outputmode, dv_displaymode);
}

/* for some non-std TV, it declare 4k while MAX_TMDS_CLK
 * not match 4K format, so filter out mode list by
 * check if basic color space/depth is supported
 * or not under this resolution
 * note that disp_mode should not contain colorspace, such as 420
 */
static bool hdmi_sink_disp_mode_sup(struct input_hdmi_data *hdmi_data, char *disp_mode)
{
	if (!hdmi_data || !disp_mode)
		return false;

	if (is_4k50_fmt(disp_mode)) {
		if (hdmitx_chk_mode_attr_sup(hdmi_data, disp_mode, "420,8bit"))
			return true;
		if (hdmitx_chk_mode_attr_sup(hdmi_data, disp_mode, "rgb,8bit"))
			return true;
		if (hdmitx_chk_mode_attr_sup(hdmi_data, disp_mode, "444,8bit"))
			return true;
		if (hdmitx_chk_mode_attr_sup(hdmi_data, disp_mode, "422,12bit"))
			return true;
	} else {
		if (hdmitx_chk_mode_attr_sup(hdmi_data, disp_mode, "rgb,8bit"))
			return true;
		if (hdmitx_chk_mode_attr_sup(hdmi_data, disp_mode, "444,8bit"))
			return true;
		if (hdmitx_chk_mode_attr_sup(hdmi_data, disp_mode, "422,12bit"))
			return true;
	}
	return false;
}

/* get the highest hdmi mode by edid */
static void get_highest_hdmimode(struct input_hdmi_data *hdmi_data, char *mode)
{
	char value[MODE_LEN] = {0};
	char mode_tmp[MODE_LEN];
	int i;

	if (!hdmi_data || !mode)
		return;

	strcpy(value, DEFAULT_HDMI_MODE);

	for (i = 0; disp_mode_t[i]; i++) {
		memset(mode_tmp, 0, sizeof(mode_tmp));
		strncpy(mode_tmp, disp_mode_t[i], MODE_LEN - 1);
		if (!hdmi_sink_disp_mode_sup(hdmi_data, mode_tmp))
			continue;
		if (resolve_resolution_value(mode_tmp, FRAMERATE_PRIORITY) >
		    resolve_resolution_value(value, FRAMERATE_PRIORITY)) {
			memset(value, 0, MODE_LEN);
			strcpy(value, mode_tmp);
		}
	}

	strcpy(mode, value);
	printf("set HDMI to highest edid mode: %s\n", mode);
}

/* check if the edid support current hdmi mode */
static void filter_hdmimode(struct input_hdmi_data *hdmi_data, char *mode)
{
	if (!hdmi_data || !mode)
		return;

	if (hdmi_sink_disp_mode_sup(hdmi_data, mode)) {
		strcpy(mode, hdmi_data->ubootenv_hdmimode);
	} else {
		/* old mode is not support in this TV,
		 * so switch to best mode.
		 */
		get_highest_hdmimode(hdmi_data, mode);
	}
}

static void get_hdmi_outputmode(struct input_hdmi_data *hdmi_data, char *mode)
{
	struct hdmitx_dev *hdev = NULL;

	if (!hdmi_data || !mode)
		return;

	hdev = container_of(hdmi_data->prxcap,
			struct hdmitx_dev, RXCap);

    /* Fall back to 480p if EDID can't be parsed */
	if (!edid_parsing_ok(hdev)) {
		strcpy(mode, DEFAULT_HDMI_MODE);
		printf("EDID parsing error detected\n");
		return;
	}

	if (is_best_outputmode())
		get_highest_hdmimode(hdmi_data, mode);
	else
		filter_hdmimode(hdmi_data, mode);
}

static void get_best_color_attr(struct input_hdmi_data *hdmi_data,
	const char *outputmode, char *colorattribute)
{
	int length = 0;
	const char **color_list = NULL;
	char temp_mode[MODE_LEN] = {0};
	int i;

	if (!hdmi_data || !outputmode || !colorattribute)
		return;

	/* filter some color value options, aimed at some modes. */
	if (!strcmp(outputmode, MODE_4K2K60HZ) ||
	    !strcmp(outputmode, MODE_4K2K50HZ) ||
	    !strcmp(outputmode, MODE_4K2KSMPTE60HZ) ||
	    !strcmp(outputmode, MODE_4K2KSMPTE50HZ)) {
		if (is_low_powermode()) {
			color_list = COLOR_ATTRIBUTE_LIST3;
			length = ARRAY_SIZE(COLOR_ATTRIBUTE_LIST3);
		} else {
			color_list = COLOR_ATTRIBUTE_LIST1;
			length = ARRAY_SIZE(COLOR_ATTRIBUTE_LIST1);
		}
	} else {
		if (is_low_powermode()) {
			color_list = COLOR_ATTRIBUTE_LIST4;
			length = ARRAY_SIZE(COLOR_ATTRIBUTE_LIST4);
		} else {
			color_list = COLOR_ATTRIBUTE_LIST2;
			length = ARRAY_SIZE(COLOR_ATTRIBUTE_LIST2);
		}
	}

	for (i = 0; i < length; i++) {
		strcpy(temp_mode, outputmode);
		strcat(temp_mode, color_list[i]);
		 if (is_supported_mode_attr(hdmi_data, temp_mode)) {
			printf("support current mode:[%s], deep color:[%s]\n",
			       outputmode, color_list[i]);
			strcpy(colorattribute, color_list[i]);
			break;
		}
	}
}

static void get_hdmi_colorattribute(struct input_hdmi_data *hdmi_data,
	const char *outputmode, char *colorattribute)
{
	char temp_mode[MODE_LEN] = {0};

	if (!hdmi_data || !outputmode || !colorattribute)
		return;
	/* if dc_cap is null, use default color format */
	/* should never encounter this case */
	if (false) {
		if (!strcmp(outputmode, MODE_4K2K60HZ) ||
		    !strcmp(outputmode, MODE_4K2K50HZ) ||
		    !strcmp(outputmode, MODE_4K2KSMPTE60HZ) ||
		    !strcmp(outputmode, MODE_4K2KSMPTE50HZ)) {
			strcpy(colorattribute, DEFAULT_COLOR_FORMAT_4K);
		} else {
			strcpy(colorattribute, DEFAULT_COLOR_FORMAT);
		}

		printf("Do not find sink color list, use default color attribute:%s\n",
		       colorattribute);
		return;
	}

	/* if bestpolicy is disabled, use ubootenv.var.colorattribute */
	if (!is_best_outputmode()) {
		strcpy(temp_mode, hdmi_data->ubootenv_hdmimode);
		strcat(temp_mode, hdmi_data->ubootenv_colorattribute);
		if (is_supported_mode_attr(hdmi_data, temp_mode))
			strcpy(colorattribute,
			       hdmi_data->ubootenv_colorattribute);
		else
			get_best_color_attr(hdmi_data, outputmode,
					    colorattribute);
	} else {
		get_best_color_attr(hdmi_data, outputmode, colorattribute);
	}

	/* if colorAttr is null above steps,
	 * will defines a initial value
	 */
	if (!strstr(colorattribute, "bit"))
		strcpy(colorattribute, DEFAULT_COLOR_FORMAT);
}

static void update_hdmi_deepcolor(struct input_hdmi_data *hdmi_data,
	const char *outputmode, char *colorattribute)
{
	if (!hdmi_data || !outputmode || !colorattribute)
		return;
	if (is_support_deepcolor())
		get_hdmi_colorattribute(hdmi_data, outputmode, colorattribute);
	else
		strcpy(colorattribute, "default");

	printf("colorattribute = %s\n", colorattribute);
}

void dolbyvision_scene_process(struct input_hdmi_data *hdmi_data,
	struct scene_output_info *output_info)
{
	int dv_type = DOLBY_VISION_DISABLE;

	if (!hdmi_data || !output_info)
		return;
	/* 1.update dolby vision output type */
	dv_type = update_dv_type(hdmi_data);
	output_info->final_dv_type = dv_type;
	printf("dv final_type:%d\n", output_info->final_dv_type);

	/* update param */
	hdmi_data->ubootenv_dv_type = dv_type;
	/* 2. update dolby vision output output mode and colorspace */
	/* 2.1 update dolby vision deepcolor */
	update_dv_attr(hdmi_data, output_info->final_deepcolor);
	printf("dv final_deepcolor:%s\n", output_info->final_deepcolor);

	/* 2.2 update dolby vision output mode */
	update_dv_displaymode(hdmi_data, output_info->final_displaymode);
	printf("dv final_displaymode:%s", output_info->final_displaymode);
}

/* check 4k50/4k60 hdr support or not */
static bool is_support_4k60hdr(struct input_hdmi_data *hdmi_data,
	scene_output_info_t *output_info)
{
	int colorList_length = 0;
	int resolutionList_length	= 0;
	int i = 0;
	int j = 0;
	char temp_mode[MODE_LEN] = {0};

	if (!hdmi_data || !output_info)
		return false;

	colorList_length = ARRAY_SIZE(HDR_4K_COLOR_ATTRIBUTE_LIST);
	resolutionList_length = ARRAY_SIZE(MODE_4K_LIST);

	for (i = 0; i < colorList_length; i++) {
		for (j = 0; j < resolutionList_length; j++) {
			memset(temp_mode, 0, sizeof(temp_mode));
			strncpy(temp_mode, MODE_4K_LIST[j], MODE_LEN - 1);
			if (strlen(temp_mode) + strlen(HDR_4K_COLOR_ATTRIBUTE_LIST[i]) <
				MODE_LEN)
				strcat(temp_mode, HDR_4K_COLOR_ATTRIBUTE_LIST[i]);
			if (is_supported_mode_attr(hdmi_data, temp_mode)) {
				printf("%s mode:[%s], deep color:[%s]\n",
					__func__, MODE_4K_LIST[j], HDR_4K_COLOR_ATTRIBUTE_LIST[i]);
				strcpy(output_info->final_deepcolor,
					HDR_4K_COLOR_ATTRIBUTE_LIST[i]);
				strcpy(output_info->final_displaymode,
					MODE_4K_LIST[j]);
				return true;
			}
		}
	}
	printf("%s 4k50/60hz hdr not support\n", __func__);
	return false;
}

/* check non 4k hdr support or not */
static bool is_support_non4k_hdr(struct input_hdmi_data *hdmi_data,
	scene_output_info_t *output_info)
{
	int colorList_length = 0;
	int resolutionList_length   = 0;
	int i = 0;
	int j = 0;
	char temp_mode[MODE_LEN] = {0};

	if (!hdmi_data || !output_info)
		return false;

	colorList_length = ARRAY_SIZE(HDR_NON4K_COLOR_ATTRIBUTE_LIST);
	resolutionList_length = ARRAY_SIZE(MODE_NON4K_LIST);

	for (i = 0; i < colorList_length; i++) {
		for (j = 0; j < resolutionList_length; j++) {
			memset(temp_mode, 0, sizeof(temp_mode));
			strncpy(temp_mode, MODE_NON4K_LIST[j], MODE_LEN - 1);
			if (strlen(temp_mode) + strlen(HDR_NON4K_COLOR_ATTRIBUTE_LIST[i]) <
				MODE_LEN)
				strcat(temp_mode, HDR_NON4K_COLOR_ATTRIBUTE_LIST[i]);
			if (is_supported_mode_attr(hdmi_data, temp_mode)) {
				printf("%s mode:[%s], deep color:[%s]\n",
					__func__, MODE_NON4K_LIST[j],
					HDR_NON4K_COLOR_ATTRIBUTE_LIST[i]);
				strcpy(output_info->final_deepcolor,
					HDR_NON4K_COLOR_ATTRIBUTE_LIST[i]);
				strcpy(output_info->final_displaymode,
					MODE_NON4K_LIST[j]);
				return true;
			}
		}
	}

	printf("%s non 4k hdr not support\n", __func__);
	return false;
}

void hdr_scene_process(struct input_hdmi_data *hdmi_data,
	scene_output_info_t *output_info)
{
	bool find = false;
	char outputmode[MODE_LEN] = {0};
	char colorattribute[MODE_LEN] = {0};

	if (!hdmi_data || !output_info)
		return;

	if (is_best_outputmode()) {
		if (is_support_4k())
			find = is_support_4k60hdr(hdmi_data, output_info);

		if (!find) {
			find = is_support_non4k_hdr(hdmi_data, output_info);
			if (!find)
				printf("%s not find hdr support\n", __func__);
		}
	} else {
		/* 1. choose resolution, frame rate */
		get_hdmi_outputmode(hdmi_data, outputmode);
		if (strlen(outputmode) == 0)
			strcpy(outputmode, DEFAULT_HDMI_MODE);
		strcpy(output_info->final_displaymode, outputmode);

		/* 2.choose color format, bit-depth */
		update_hdmi_deepcolor(hdmi_data, outputmode, colorattribute);
		strcpy(output_info->final_deepcolor, colorattribute);
		printf("hdr final displaymode:%s, deepcolor:%s\n",
			output_info->final_displaymode, output_info->final_deepcolor);
	}
}

/* SDR scene policy process */
void sdr_scene_process(struct input_hdmi_data *hdmi_data, struct scene_output_info *output_info)
{
	char outputmode[MODE_LEN] = {0};
	char colorattribute[MODE_LEN] = {0};

	if (!hdmi_data || !output_info)
		return;
	/* 1.choose resolution, frame rate */
	get_hdmi_outputmode(hdmi_data, outputmode);
	if (strlen(outputmode) == 0)
		strcpy(outputmode, DEFAULT_HDMI_MODE);
	strcpy(output_info->final_displaymode, outputmode);

	/* 2.choose color format, bit-depth */
	update_hdmi_deepcolor(hdmi_data, outputmode, colorattribute);
	strcpy(output_info->final_deepcolor, colorattribute);
	printf("sdr final_displaymode:%s, final_deepcolor:%s\n",
		output_info->final_displaymode, output_info->final_deepcolor);
}

void get_hdmi_data(struct hdmitx_dev *hdev, struct input_hdmi_data *data)
{
	char *hdmimode;
	char *colorattribute;

	if (!hdev || !data)
		return;

	hdmimode = env_get("hdmimode");
	colorattribute = env_get("colorattribute");

	if (!hdmimode)
		hdmimode = DEFAULT_HDMIMODE_ENV;
	if (!colorattribute)
		colorattribute = DEFAULT_COLORATTRIBUTE_ENV;
	strcpy(data->ubootenv_hdmimode, hdmimode);
	strcpy(data->ubootenv_colorattribute, colorattribute);
	data->ubootenv_dv_type = get_ubootenv_dv_type();
	data->hdr_priority = get_hdr_priority();
	#if 0
	data->isbestpolicy = is_best_outputmode();
	data->isSupport4K30Hz = is_support_4k30hz();
	data->isSupport4K = is_support_4k();
	data->isDeepColor = is_support_deepcolor();
	data->isLowPowerMode = is_low_powermode();
	data->isframeratepriority = is_framerate_priority();
	#endif
	data->prxcap = &hdev->RXCap;
	/* memcpy(&(data->prxcap), &(hdev->RXCap), sizeof(hdev->RXCap)); */
	printf("ubootenv dv_type: %d, hdr_priority: %d\n",
	       data->ubootenv_dv_type,
	       data->hdr_priority);
	printf("ubootenv best_output: %d, framerate_priority: %d\n",
	       is_best_outputmode(),
	       is_framerate_priority());
}
