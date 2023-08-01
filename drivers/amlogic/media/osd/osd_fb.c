// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

/* System Headers */
#include <common.h>
#include <video_fb.h>
#include <stdio_dev.h>
#include <malloc.h>
#include <bmp_layout.h>
#include <amlogic/cpu_id.h>
#include <asm/arch/cpu.h>
#include <asm/arch/timer.h>

/* Local Headers */
#include <amlogic/fb.h>
#include <amlogic/color.h>
#include <amlogic/media/vout/aml_vinfo.h>
#include <amlogic/media/vout/aml_vout.h>

/* Local Headers */
#include "osd.h"
#include "osd_log.h"
#include "osd_hw.h"
#include "osd_fb.h"

DECLARE_GLOBAL_DATA_PTR;

#define INVALID_BPP_ITEM {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
static const struct color_bit_define_s default_color_format_array[] = {
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	{
		COLOR_INDEX_02_PAL4, 0, 0,
		0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0,
		FB_VISUAL_PSEUDOCOLOR, 2,
	},
	INVALID_BPP_ITEM,
	{
		COLOR_INDEX_04_PAL16, 0, 1,
		0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0,
		FB_VISUAL_PSEUDOCOLOR, 4,
	},
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	{
		COLOR_INDEX_08_PAL256, 0, 2,
		0, 8, 0, 0, 8, 0, 0, 8, 0, 0, 0, 0,
		FB_VISUAL_PSEUDOCOLOR, 8,
	},
	/*16 bit color*/
	{
		COLOR_INDEX_16_655, 0, 4,
		10, 6, 0, 5, 5, 0, 0, 5, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_844, 1, 4,
		8, 8, 0, 4, 4, 0, 0, 4, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_6442, 2, 4,
		10, 6, 0, 6, 4, 0, 2, 4, 0, 0, 2, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_4444_R, 3, 4,
		12, 4, 0, 8, 4, 0, 4, 4, 0, 0, 4, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_4642_R, 7, 4,
		12, 4, 0, 6, 6, 0, 2, 4, 0, 0, 2, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_1555_A, 6, 4,
		10, 5, 0, 5, 5, 0, 0, 5, 0, 15, 1, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_4444_A, 5, 4,
		8, 4, 0, 4, 4, 0, 0, 4, 0, 12, 4, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_565, 4, 4,
		11, 5, 0, 5, 6, 0, 0, 5, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	/*24 bit color*/
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	{
		COLOR_INDEX_24_6666_A, 4, 7,
		12, 6, 0, 6, 6, 0, 0, 6, 0, 18, 6, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	{
		COLOR_INDEX_24_6666_R, 3, 7,
		18, 6, 0, 12, 6, 0, 6, 6, 0, 0, 6, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	{
		COLOR_INDEX_24_8565, 2, 7,
		11, 5, 0, 5, 6, 0, 0, 5, 0, 16, 8, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	{
		COLOR_INDEX_24_5658, 1, 7,
		19, 5, 0, 13, 6, 0, 8, 5, 0, 0, 8, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	{
		COLOR_INDEX_24_888_B, 5, 7,
		0, 8, 0, 8, 8, 0, 16, 8, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	{
		COLOR_INDEX_24_RGB, 0, 7,
		16, 8, 0, 8, 8, 0, 0, 8, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	/*32 bit color*/
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	{
		COLOR_INDEX_32_BGRA, 3, 5,
		8, 8, 0, 16, 8, 0, 24, 8, 0, 0, 8, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	{
		COLOR_INDEX_32_ABGR, 2, 5,
		0, 8, 0, 8, 8, 0, 16, 8, 0, 24, 8, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	{
		COLOR_INDEX_32_RGBA, 0, 5,
		24, 8, 0, 16, 8, 0, 8, 8, 0, 0, 8, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	{
		COLOR_INDEX_32_ARGB, 1, 5,
		16, 8, 0, 8, 8, 0, 0, 8, 0, 24, 8, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	/*YUV color*/
	{COLOR_INDEX_YUV_422, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16},
};

GraphicDevice fb_gdev;
struct hw_para_s osd_hw;

typedef struct pic_info_t {
	unsigned int mode;
	unsigned int type;
	unsigned int pic_width;
	unsigned int pic_height;
	unsigned int bpp;
	ulong pic_image;
}pic_info_t;
static pic_info_t g_pic_info;
static int img_video_init = 0;
#if defined(CONFIG_AML_MINUI)
extern int in_fastboot_mode;
#endif
int rma_test;

static void osd_layer_init(GraphicDevice *gdev, int layer)
{
	u32 index = layer;
	u32 xoffset = 0;
	u32 yoffset = 0;
	u32 xres = 0;
	u32 yres = 0;
	u32 xres_virtual = 0;
	u32 yres_virtual = 0;
	u32 disp_start_x = 0;
	u32 disp_start_y = 0;
	u32 disp_end_x = 0;
	u32 disp_end_y = 0;
	u32 fbmem = gdev->frameAdrs;
	const struct color_bit_define_s *color =
			&default_color_format_array[gdev->gdfIndex];

	/* if viu2 has no scaler */
	if (is_vppx(index) && !osd_hw.viux_scale_cap) {
		xres = gdev->winSizeX;
		yres = gdev->winSizeY;
		xres_virtual = gdev->winSizeX;
		yres_virtual = gdev->winSizeY * 2;
		disp_end_x = gdev->winSizeX - 1;
		disp_end_y = gdev->winSizeY - 1;
	} else {
		xres = gdev->fb_width;
		yres = gdev->fb_height;
		xres_virtual = gdev->fb_width;
		yres_virtual = gdev->fb_height * 2;
		disp_end_x = gdev->fb_width - 1;
		disp_end_y = gdev->fb_height - 1;
	}
#ifdef AML_OSD_HIGH_VERSION
	if (is_vppx(index))
		osd_init_hw_viux(index);
	else
		osd_init_hw(index);
#else
	osd_init_hw(index);
#endif
	osd_setup_hw(index,
		     xoffset,
		     yoffset,
		     xres,
		     yres,
		     xres_virtual ,
		     yres_virtual,
		     disp_start_x,
		     disp_start_y,
		     disp_end_x,
		     disp_end_y,
		     fbmem,
		     color);
}

static unsigned long env_strtoul(const char *name, int base)
{
	unsigned long ret = 0;
	char *str = NULL;

	str = env_get(name);
	if (str) {
		ret = simple_strtoul(str, NULL, base);
	}
	if (base == 16)
		osd_logd("%s: 0x%lx\n", name, ret);
	else if (base == 10)
		osd_logd("%s: %ld\n", name, ret);

	return ret;
}

static int get_dts_node(const void *dt_addr, char *dtb_node)
{
#ifdef CONFIG_OF_LIBFDT
	int parent_offset = 0;
	char *propdata = NULL;

	parent_offset = fdt_path_offset(dt_addr, dtb_node);
	if (parent_offset < 0) {
		/* not found */
		return -1;
	} else {
		propdata = (char *)fdt_getprop(dt_addr, parent_offset, "status", NULL);
		if (propdata == NULL) {
			osd_logi("not find status, default to disabled\n");
			return -1;
		} else {
			if (strncmp(propdata, "okay", 2)) {
				osd_logi("status disabled\n");
				return -1;
			}
		}
	}
	return parent_offset;
#else
	return -1;
#endif
}

#ifdef VEHICLE_CONFIG
void osd2_layer_init(void)
{
	u32 xoffset = 0;
	u32 yoffset = 0;
	u32 xres = 0;
	u32 yres = 0;
	u32 xres_virtual = 0;
	u32 yres_virtual = 0;
	u32 disp_start_x = 0;
	u32 disp_start_y = 0;
	u32 disp_end_x = 0;
	u32 disp_end_y = 0;
	u32 fbmem = fb_gdev.frameAdrs;
	const struct color_bit_define_s *color =
			&default_color_format_array[fb_gdev.gdfIndex];

	xres = fb_gdev.fb_width;
	yres = fb_gdev.fb_height;
	xres_virtual = fb_gdev.fb_width;
	yres_virtual = fb_gdev.fb_height * 2;
	disp_end_x = fb_gdev.fb_width - 1;
	disp_end_y = fb_gdev.fb_height - 1;
	osd2_setup_hw(OSD2,
		     xoffset,
		     yoffset,
		     xres,
		     yres,
		     xres_virtual,
		     yres_virtual,
		     disp_start_x,
		     disp_start_y,
		     disp_end_x,
		     disp_end_y,
		     fbmem,
		     color);
}
#endif

unsigned long get_fb_addr(void)
{
	char *dt_addr = NULL;
	unsigned long fb_addr = 0;
	static int initrd_set = 0;
	char str_fb_addr[32];
	char fdt_node[32];
	int dt_loaded = 0;
#ifdef CONFIG_OF_LIBFDT
	int parent_offset = 0;
	char *propdata = NULL;
#endif

	fb_addr = env_strtoul("fb_addr", 16);
#ifdef CONFIG_OF_LIBFDT

#if defined(CONFIG_AML_MINUI)
	if (in_fastboot_mode == 1) {
		osd_logi("in fastboot mode, load default fb_addr parameters \n");
	} else
#endif
	{
		dt_addr = (char *) env_get_ulong("dtb_mem_addr",  16, 0x1000000);
		if (dt_addr == NULL) {
			osd_logi("dt_addr is null, load default parameters\n");
			goto ret_fb_addr;
		}
		if (fdt_check_header(dt_addr) < 0) {
			dt_addr = (char *)gd->fdt_blob;
			if (fdt_check_header(dt_addr) < 0) {
				dt_loaded = -1;
				osd_logi("check dts: %s, load default fb_addr parameters\n",
				fdt_strerror(fdt_check_header(dt_addr)));
			}
		}
		if (dt_loaded < 0) {
			osd_logi("check dts: %s, load default fb_addr parameters\n",
				fdt_strerror(fdt_check_header(dt_addr)));
		} else {
			strcpy(fdt_node, "/fb");
			osd_logi("load fb addr from dts:%s\n", fdt_node);
			parent_offset = get_dts_node(dt_addr, fdt_node);
			if (parent_offset < 0) {
				strcpy(fdt_node, "/drm-vpu");
				osd_logi("load fb addr from dts:%s\n", fdt_node);
				parent_offset = get_dts_node(dt_addr, fdt_node);
				if (parent_offset < 0) {
					osd_logi("not find node: %s\n",fdt_strerror(parent_offset));
					osd_logi("use default fb_addr parameters\n");
				} else {
					/* check fb_addr */
					propdata = (char *)fdt_getprop(dt_addr, parent_offset, "logo_addr", NULL);
					if (propdata == NULL) {
						osd_logi("failed to get fb addr for logo\n");
						osd_logi("use default fb_addr parameters\n");
					} else {
						fb_addr = simple_strtoul(propdata, NULL, 16);
					}
				}
			} else {
				/* check fb_addr */
				propdata = (char *)fdt_getprop(dt_addr, parent_offset, "logo_addr", NULL);
				if (propdata == NULL) {
					osd_logi("failed to get fb addr for logo\n");
					osd_logi("use default fb_addr parameters\n");
				} else {
					fb_addr = simple_strtoul(propdata, NULL, 16);
				}
			}
		}
	}
ret_fb_addr:
#endif

	if ((!initrd_set) && (osd_get_chip_type() >= MESON_CPU_MAJOR_ID_AXG)) {
		sprintf(str_fb_addr,"%lx",fb_addr);
		env_set("initrd_high", str_fb_addr);
		initrd_set = 1;
		osd_logi("set initrd_high: 0x%s\n", str_fb_addr);
	}

	osd_logi("fb_addr for logo: 0x%lx\n", fb_addr);
	return fb_addr;
}

static void get_osd_version(void)
{
	u32 family_id = osd_get_chip_type();

	if (family_id == MESON_CPU_MAJOR_ID_AXG ||
		family_id == MESON_CPU_MAJOR_ID_A4)
		osd_hw.osd_ver = OSD_SIMPLE;
	else if (family_id < MESON_CPU_MAJOR_ID_G12A)
		osd_hw.osd_ver = OSD_NORMAL;
	else
		osd_hw.osd_ver = OSD_HIGH_ONE;

	#if defined(AML_T7_DISPLAY) || defined(AML_S5_DISPLAY)
	osd_hw.viux_scale_cap = 1;
	#else
	osd_hw.viux_scale_cap = 0;
	#endif
}

int get_osd_viux_scale_cap(void)
{
	return osd_hw.viux_scale_cap;
}

u32 osd_canvas_align(u32 x)
{
	if (osd_hw.osd_ver >= OSD_HIGH_ONE)
		return (((x) + 63) & ~63);
	else
		return (((x) + 31) & ~31);
}

int get_osd_layer(void)
{
	char *layer_str;
	int osd_index = 0, vpp_index = 0;
	static int last_osd, last_vpp;

	layer_str = env_get("display_layer");
	if (strcmp(layer_str, "osd0") == 0) {
		osd_index = OSD1;
		vpp_index = VPU_VPP0;
	} else if (strcmp(layer_str, "osd1") == 0) {
		osd_index = OSD2;
		vpp_index = VPU_VPP0;
	} else if (strcmp(layer_str, "osd2") == 0) {
		osd_index = OSD3;
		vpp_index = VPU_VPP0;
	} else if (strcmp(layer_str, "osd3") == 0) {
		osd_index = OSD4;
		vpp_index = VPU_VPP0;
	} else if (strcmp(layer_str, "viu2_osd0") == 0) {
		osd_index = OSD3;
		vpp_index = VPU_VPP1;
	} else if (strcmp(layer_str, "viu3_osd0") == 0) {
		osd_index = OSD4;
		vpp_index = VPU_VPP2;
	} else {
		osd_loge("%s, error found\n", __func__);
	}

	osd_hw.vpp_index[osd_index] = vpp_index;
	if (osd_index != last_osd || vpp_index != last_vpp)
		osd_logd3("%s, osd%d --> vpp%d\n", __func__,
			  osd_index, osd_hw.vpp_index[osd_index]);

	last_osd = osd_index;
	last_vpp = vpp_index;

	return osd_index;
}

static bool is_osd_supported(int chip_id)
{
	if ((chip_id == MESON_CPU_MAJOR_ID_A1) ||
		(chip_id == MESON_CPU_MAJOR_ID_C1) ||
		(chip_id == MESON_CPU_MAJOR_ID_C2))
		return false;
	else
		return true;
}

static void *osd_hw_init(void)
{
	int osd_index = -1;
	u32 color_index = 0;

	color_index = env_strtoul("display_color_index", 10);

	if ((color_index < ARRAY_SIZE(default_color_format_array))
	    && (default_color_format_array[color_index].color_index !=
		COLOR_INDEX_NULL))
		fb_gdev.gdfIndex = color_index;
	else {
		osd_loge("color_index %d invalid\n", color_index);
		return NULL;
	}

	osd_index = get_osd_layer();
	if (osd_index < 0) {
		osd_loge("osd_hw_init: invalid osd_index\n");
		return NULL;
	}
	if (is_vpp0(osd_index)) {
		if (osd_hw.osd_ver == OSD_SIMPLE && osd_index >= OSD2) {
			osd_loge("AXG/A4/C3 not support osd%d\n", osd_index);
			return NULL;
		}
		osd_layer_init(&fb_gdev, osd_index);
	} else if (is_vpp1(osd_index)) {
		if (osd_hw.osd_ver == OSD_SIMPLE) {
			osd_loge("AXG/A4/C3 not support viu2 osd0\n");
			return NULL;
		}
		osd_layer_init(&fb_gdev, osd_index);
	}  else if (is_vpp2(osd_index)) {
		if (osd_hw.osd_ver == OSD_SIMPLE) {
			osd_loge("AXG/A4/C3 not support viu3 osd0\n");
			return NULL;
		}
		osd_layer_init(&fb_gdev, osd_index);
	} else {
		osd_loge("display_layer(%d) invalid\n", osd_index);
		return NULL;
	}
	return (void *)&fb_gdev;
}

void *video_hw_init(int display_mode)
{
	u32 fb_addr = 0;
	u32 display_width = 0;
	u32 display_height = 0;
	u32 display_bpp = 0;
	u32 fg = 0;
	u32 bg = 0;
	u32 fb_width = 0;
	u32 fb_height = 0;

	if (!is_osd_supported(osd_get_chip_type()))
		return NULL;
	get_osd_version();
#ifdef CONFIG_AML_VOUT
	vout_init();
#endif
	fb_addr = get_fb_addr();
	switch (display_mode) {
	case MIDDLE_MODE:
	case RECT_MODE:
#ifdef OSD_SCALE_ENABLE
		fb_width = env_strtoul("fb_width", 10);
		fb_height = env_strtoul("fb_height", 10);
#endif
		display_bpp = env_strtoul("display_bpp", 10);
		break;
	case FULL_SCREEN_MODE:
		fb_width = g_pic_info.pic_width;
		fb_height = g_pic_info.pic_height;
		display_bpp = g_pic_info.bpp;
		break;
	}
	display_width = env_strtoul("display_width", 10);
	display_height = env_strtoul("display_height", 10);
	fg = env_strtoul("display_color_fg", 16);
	bg = env_strtoul("display_color_bg", 16);

	/* fill in Graphic Device */
	fb_gdev.frameAdrs = fb_addr;
	fb_gdev.fb_width = fb_width;
	fb_gdev.fb_height = fb_height;
	fb_gdev.winSizeX = display_width;
	fb_gdev.winSizeY = display_height;
	fb_gdev.gdfBytesPP = display_bpp / 8;
	fb_gdev.fg = fg;
	fb_gdev.bg = bg;
	fb_gdev.mode = display_mode;
	return osd_hw_init();
}

int rle8_decode(uchar *ptr, bmp_image_t *bmap_rle8, ulong width_bmp, ulong height_bmp) {
	uchar a;
	uchar cnt, runlen;
	int i;
	int decode;
	int pixels;
	uchar *pic;
	int limit;

	a = 0xFF;
	decode = 1;
	pixels = 0;
	limit = width_bmp * height_bmp;
	pic = (uchar *)bmap_rle8 + le32_to_cpu(bmap_rle8->header.data_offset);

	while (decode) {
		switch (pic[0]) {
		case 0:
			switch (pic[1]) {
			case 0:
				/* end of row */
				pic += 2;
				continue;
			case 1:
				/* end of bmp */
				decode = 0;
				break;
			case 2:
				/* 00 02 mode */
				pic += 4;
				break;

			default:
				/* 00 (03~FF) mode */
				cnt = pic[1];
				runlen = cnt;
				pixels += cnt;
				if (pixels > limit)
				{
					osd_loge("Error: Too much encoded pixel data, validate your bitmap\n");
					decode = 0;
					return -1;
				}
				pic += 2;
				for (i = 0; i < cnt; i++) {

					*ptr = bmap_rle8->color_table[*pic].blue;
					ptr += 1;
					*ptr = bmap_rle8->color_table[*pic].green;
					ptr += 1;
					*ptr = bmap_rle8->color_table[*pic].red;
					ptr += 1;
					*ptr = a;
					ptr += 1;
					pic += 1;
				}
				if (runlen & 1)
					pic += 1;	/* 0 padding if length is odd */
				break;
			}
			break;

		default:
			/* normal mode */
			cnt = pic[0];
			runlen = cnt;
			pixels += cnt;
			if (pixels > limit) {
				osd_loge("Error: Too much encoded pixel data, validate your bitmap\n");
				return -1;
			}
			pic += 1;
			for (i = 0; i < cnt; i++) {

				*ptr = bmap_rle8->color_table[*pic].blue;
				ptr += 1;
				*ptr = bmap_rle8->color_table[*pic].green;
				ptr += 1;
				*ptr = bmap_rle8->color_table[*pic].red;
				ptr += 1;
				*ptr = a;
				ptr += 1;
			}
			pic += 1;
			break;
		}
	}
	return (0);
}

static int parse_bmp_info(ulong bmp_image)
{
	bmp_image_t *bmp = (bmp_image_t *)bmp_image;

	if (!((bmp->header.signature[0] == 'B') &&
		  (bmp->header.signature[1] == 'M'))) {
		osd_loge("no valid bmp image at 0x%lx\n", bmp_image);
		return 1;
	}
	g_pic_info.pic_width = le32_to_cpu(bmp->header.width);
	g_pic_info.pic_height = le32_to_cpu(bmp->header.height);
	g_pic_info.bpp = le16_to_cpu(bmp->header.bit_count);
	return 0;
}

int video_display_bitmap(ulong bmp_image, int x, int y)
{
	struct vinfo_s *info = NULL;
#if defined CONFIG_AML_VOUT
	info = vout_get_current_vinfo();
#endif
	/* ushort *cmap_base = NULL; */
	unsigned long byte_width;
	ushort i, j;
	uchar *fb;
	bmp_image_t *bmp = (bmp_image_t *)bmp_image;
	uchar *bmap;
	unsigned long width, height;
	unsigned long pheight;
	unsigned long pwidth;
	unsigned colors, bpix, bmp_bpix;
	uint lcd_line_length;
	int osd_index = -1;
	int bmp_line_bytes;
	int bmp_line_align_offset;

	osd_index = get_osd_layer();
	if (osd_index < 0) {
		osd_loge("video_display_bitmap: invalid osd_index\n");
		return (-1);
	}

	/* if viu2/viu3 has no scaler */
	if (is_vppx(osd_index) && !osd_hw.viux_scale_cap) {
		pwidth = info->width;
		pheight = info->height;
	} else {
		pheight = fb_gdev.fb_height;
		pwidth = fb_gdev.fb_width;
	}

	lcd_line_length = CANVAS_ALIGNED((pwidth * NBITS(info->vl_bpix)) / 8);
	if (fb_gdev.mode != FULL_SCREEN_MODE)
		if (parse_bmp_info(bmp_image))
			return -1;
	width = g_pic_info.pic_width;
	height = g_pic_info.pic_height;
	bmp_bpix = g_pic_info.bpp;
	colors = 1 << bmp_bpix;
	uchar *buffer_rgb = NULL;
	bpix = NBITS(info->vl_bpix);
	if ((bpix != 1) && (bpix != 8) && (bpix != 16) && (bpix != 24) &&
	    (bpix != 32)) {
		osd_loge("%d bit/pixel mode1, but BMP has %d bit/pixel\n",
			 bpix, bmp_bpix);
		return 1;
	}
	/* We support displaying 8bpp BMPs on 16bpp LCDs */
	/*if (bpix != bmp_bpix && (bmp_bpix != 8 || bpix != 16)) {
		osd_loge("%d bit/pixel mode2, but BMP has %d bit/pixel\n",
			 bpix,
			 le16_to_cpu(bmp->header.bit_count));
		return 1;
	}*/

	osd_logd("Display-bmp: %d x %d  with %d colors\n",
		 (int)width, (int)height, (int)colors);

	if ((x == -1) && (y == -1)) {
		/* use MIDDLE_MODE */
		if ((width > pwidth) || (height > pheight)) {
			x = 0;
			y = 0;
		} else {
			x = (pwidth - width) / 2;
			y = (pheight - height) / 2;
		}
	} else {
		switch (fb_gdev.mode) {
		case MIDDLE_MODE:
				if ((width > pwidth) || (height > pheight)) {
					x = 0;
					y = 0;
				} else {
					x = (pwidth - width) / 2;
					y = (pheight - height) / 2;
				}
			break;
		case RECT_MODE:
			break;
		case FULL_SCREEN_MODE:
			x = 0;
			y = 0;
			break;
		}
	}
	/*
	 *  BMP format for Monochrome assumes that the state of a
	 * pixel is described on a per Bit basis, not per Byte.
	 *  So, in case of Monochrome BMP we should align widths
	 * on a byte boundary and convert them from Bit to Byte
	 * units.
	 *  Probably, PXA250 and MPC823 process 1bpp BMP images in
	 * their own ways, so make the converting to be MCC200
	 * specific.
	 */
	bmp_line_bytes = width * bmp_bpix / 8;
	bmp_line_bytes = (bmp_line_bytes & 0x3) ?
		((bmp_line_bytes & ~0x3) + 4) : (bmp_line_bytes);

	bmp_line_align_offset = bmp_line_bytes - width * bmp_bpix / 8;

	if ((x + width) > pwidth)
		width = pwidth - x;
	if ((y + height) > pheight)
		height = pheight - y;

	bmap = (uchar *)bmp + le32_to_cpu(bmp->header.data_offset);
	fb   = (uchar *)(osd_hw.fb_gem[osd_index].addr +
			 (y + height - 1) * lcd_line_length + x * fb_gdev.gdfBytesPP);

	osd_logd("fb=0x%p; bmap=0x%p, width=%ld, height= %ld, lcd_line_length=%d, bmp_line_bytes=%d, fb_gdev.fb_width=%d, fb_gdev.fb_height=%d \n",
		 fb, bmap, width, height, lcd_line_length, bmp_line_bytes,
		 fb_gdev.fb_width, fb_gdev.fb_height);

	if (bmp_bpix == 8) {
		/* decode of RLE8 */
		buffer_rgb = (uchar *)malloc(height * width * 4 * sizeof(uchar) + 1);
		if (buffer_rgb == NULL) {
			printf("Error:fail to malloc the memory!");
			return (-1);
		}
	}
	uchar *ptr_rgb = buffer_rgb;
	switch (bmp_bpix) {
	case 8:
		if (bpix != 16)
			byte_width = width;
		else
			byte_width = width * 2;
		rle8_decode(ptr_rgb, bmp, width, height);
		for (i = 0; i < height; ++i) {
			for (j = 0; j < width; j++) {
				if (bpix != 16) {
					*(fb++) = *buffer_rgb;
					buffer_rgb += 1;
					*(fb++) = *buffer_rgb;
					buffer_rgb += 1;
					*(fb++) = *buffer_rgb;
					buffer_rgb += 1;
					*(fb++) = *buffer_rgb;
					buffer_rgb += 1;
				}
				/*
				else {
					*(uint16_t *)fb = cmap_base[*buffer_rgb++];
					fb += sizeof(uint16_t) / sizeof(*fb);
				}
				*/
			}
			buffer_rgb += bmp_line_align_offset;
			fb -= (byte_width * 4 + lcd_line_length);
		}
		buffer_rgb -= width*height*4;
		free(buffer_rgb);
		break;
	case 16:
		for (i = 0; i < height; ++i) {
			for (j = 0; j < width; j++) {

				*(fb++) = *(bmap++);
				*(fb++) = *(bmap++);
			}
			bmap += bmp_line_align_offset;
			fb   -= (width * 2 + lcd_line_length);
		}
		break;
	case 24:
		if (bpix == 32) {
			for (i = 0; i < height; ++i) {
				for (j = 0; j < width; j++) {

					*(fb++) = *(bmap++);
					*(fb++) = *(bmap++);
					*(fb++) = *(bmap++);
					*(fb++) = 0xff;
				}
				bmap += bmp_line_align_offset;
				fb   -= (width * 4 + lcd_line_length);
			}
		} else {
			for (i = 0; i < height; ++i) {
				for (j = 0; j < width; j++) {

					*(fb++) = *(bmap++);
					*(fb++) = *(bmap++);
					*(fb++) = *(bmap++);
				}
				bmap += bmp_line_align_offset;
				fb   -= (width * 3 + lcd_line_length);
			}
		}
		break;
	case 32:
		for (i = 0; i < height; ++i) {
			for (j = 0; j < width; j++) {

				*(fb++) = *(bmap++);
				*(fb++) = *(bmap++);
				*(fb++) = *(bmap++);
				*(fb++) = *(bmap++);
			}
			bmap += bmp_line_align_offset;
			fb   -= (width * 4 + lcd_line_length);
		}
		break;
	default:
		osd_loge("error: gdev.bpp %d, but bmp.bpp %d\n", fb_gdev.gdfBytesPP, bmp_bpix);
		return (-1);
	}
	buffer_rgb = NULL;
	ptr_rgb = NULL;

	flush_cache((unsigned long)osd_hw.fb_gem[osd_index].addr,
		    pheight * CANVAS_ALIGNED(pwidth * info->vl_bpix / 8));
	return (0);
}

int video_display_raw(ulong raw_image, int x, int y)
{
	struct vinfo_s *info = NULL;
#if defined CONFIG_AML_VOUT
	info = vout_get_current_vinfo();
#endif
	ushort i, j;
	uchar *fb;
	uchar *bmap = (uchar *)raw_image;
	ushort padded_line;
	unsigned long width, height;
#ifdef OSD_SCALE_ENABLE
	unsigned long pheight = fb_gdev.fb_height;
	unsigned long pwidth = fb_gdev.fb_width;
#else
	unsigned long pheight = info->width;
	unsigned long pwidth = info->height;
#endif
	unsigned colors, bpix, bmp_bpix;
	int lcd_line_length = (pwidth * NBITS(info->vl_bpix)) / 8;
	int osd_index = -1;

	osd_index = get_osd_layer();
	if (osd_index < 0) {
		osd_loge("video_display_raw: invalid osd_index\n");
		return (-1);
	}

	width = g_pic_info.pic_width;
	height = g_pic_info.pic_height;
	bmp_bpix = g_pic_info.bpp;
	colors = 1 << bmp_bpix;
	bpix = NBITS(info->vl_bpix);
	if ((bpix != 1) && (bpix != 8) && (bpix != 16) && (bpix != 24) &&
	    (bpix != 32)) {
		osd_loge("%d bit/pixel mode1, but BMP has %d bit/pixel\n",
			 bpix, bmp_bpix);
		return 1;
	}
	/* We support displaying 8bpp BMPs on 16bpp LCDs */
	/*if (bpix != bmp_bpix && (bmp_bpix != 8 || bpix != 16)) {
		osd_loge("%d bit/pixel mode2, but BMP has %d bit/pixel\n",
			 bpix,
			 le16_to_cpu(bmp->header.bit_count));
		return 1;
	}*/

	osd_logd("Display-bmp: %d x %d  with %d colors\n",
		 (int)width, (int)height, (int)colors);

	if ((x == -1) && (y == -1)) {
		/* use MIDDLE_MODE */
		if ((width > pwidth) || (height > pheight)) {
			x = 0;
			y = 0;
		} else {
			x = (pwidth - width) / 2;
			y = (pheight - height) / 2;
		}
	} else {
		switch (fb_gdev.mode) {
		case MIDDLE_MODE:
				if ((width > pwidth) || (height > pheight)) {
					x = 0;
					y = 0;
				} else {
					x = (pwidth - width) / 2;
					y = (pheight - height) / 2;
				}
			break;
		case RECT_MODE:
			break;
		case FULL_SCREEN_MODE:
			x = 0;
			y = 0;
			break;
		}
	}
	/*
	 *  BMP format for Monochrome assumes that the state of a
	 * pixel is described on a per Bit basis, not per Byte.
	 *  So, in case of Monochrome BMP we should align widths
	 * on a byte boundary and convert them from Bit to Byte
	 * units.
	 *  Probably, PXA250 and MPC823 process 1bpp BMP images in
	 * their own ways, so make the converting to be MCC200
	 * specific.
	 */
	padded_line = (width & 0x3) ? ((width & ~0x3) + 4) : (width);

	if ((x + width) > pwidth)
		width = pwidth - x;
	if ((y + height) > pheight)
		height = pheight - y;

	osd_enable_hw(osd_index, 1);

	fb   = (uchar *)(info->vd_base + y * lcd_line_length + x * fb_gdev.gdfBytesPP);

	osd_logd("fb=0x%p; bmap=0x%p, width=%ld, height= %ld, lcd_line_length=%d, padded_line=%d, fb_gdev.fb_width=%d, fb_gdev.fb_height=%d \n",
		 fb, bmap, width, height, lcd_line_length, padded_line,fb_gdev.fb_width,fb_gdev.fb_height);

	if (FULL_SCREEN_MODE == fb_gdev.mode) {
		memcpy(fb, bmap, height * width * bmp_bpix / 8);
	} else {
		switch (bmp_bpix) {
		case 16:
			for (i = 0; i < height; ++i) {
				for (j = 0; j < width; j++) {

					*(fb++) = *(bmap++);
					*(fb++) = *(bmap++);
				}
				bmap += (padded_line - width) * 2;
				fb   += (lcd_line_length - width * 2);
			}
			break;
		case 24:
			if (bpix == 32) {
				for (i = 0; i < height; ++i) {
					for (j = 0; j < width; j++) {

						*(fb++) = *(bmap++);
						*(fb++) = *(bmap++);
						*(fb++) = *(bmap++);
						*(fb++) = 0xff;
					}
					bmap += (padded_line - width) * 4;
					fb   += (lcd_line_length - width * 4);
				}
			} else {
				for (i = 0; i < height; ++i) {
					for (j = 0; j < width; j++) {

						*(fb++) = *(bmap++);
						*(fb++) = *(bmap++);
						*(fb++) = *(bmap++);
					}
					bmap += (padded_line - width) * 3;
					fb   += (lcd_line_length - width * 3);
				}
			}
			break;
		case 32:
			for (i = 0; i < height; ++i) {
				for (j = 0; j < width; j++) {

					*(fb++) = *(bmap++);
					*(fb++) = *(bmap++);
					*(fb++) = *(bmap++);
					*(fb++) = *(bmap++);
				}
				bmap += (padded_line - width) * 4;
				fb   += (lcd_line_length - width * 4);
			}
			break;
		default:
			osd_loge("error: gdev.bpp %d, but raw.bpp %d\n", fb_gdev.gdfBytesPP, bmp_bpix);
			return (-1);
		}
	}
	flush_cache((unsigned long)info->vd_base,
		    pheight * pwidth * info->vl_bpix / 8);
	return (0);
}

#ifdef AML_S5_DISPLAY
struct vpp_post_input_s vpp_input;
static struct vpp_post_info_t vpp_post_amdv;

static int get_vpp_slice_num(const struct vinfo_s *info)
{
	int slice_num = 1;

#ifdef AUTO_CAL
	/* 8k case 4 slice */
	if (info->width > 4096 && info->field_height > 2160)
		slice_num = 4;
	else
		slice_num = 1;
#else
	if (info->cur_enc_ppc)
		slice_num = info->cur_enc_ppc;
#endif
	return slice_num;
}

static void update_vpp_input_info(const struct vinfo_s *info)
{
	vpp_input.slice_num = get_vpp_slice_num(info);
	vpp_input.overlap_hsize = 32;
	vpp_input.bld_out_hsize = info->width;
	vpp_input.bld_out_vsize = info->field_height;
	vpp_input.vd1_padding_en = 0;
}

/* hw reg param info set */
static int vpp_post_hwincut_param_set(struct vpp_post_s *vpp_post)
{
	if (!vpp_post)
		return -1;
	/* need check vd1 padding or not */
	if (vpp_input.vd1_padding_en) {
		vpp_post->vd1_hwin.vd1_hwin_en = 1;
		vpp_post->vd1_hwin.vd1_hwin_in_hsize =
			vpp_input.vd1_size_after_padding;
		vpp_post->vd1_hwin.vd1_hwin_out_hsize =
			vpp_input.vd1_size_before_padding;

		vpp_input.din_hsize[0] = vpp_post->vd1_hwin.vd1_hwin_out_hsize;
	} else {
		vpp_post->vd1_hwin.vd1_hwin_en = 0;
	}
	return 0;
}

/* following is vpp post parameters calc for hw setting */
static int vpp_blend_param_set(struct vpp_post_blend_s *vpp_post_blend)
{
	if (!vpp_post_blend)
		return -1;
	vpp_post_blend->bld_dummy_data = 0x008080;
	vpp_post_blend->bld_out_en = 1;

	vpp_post_blend->bld_out_w = vpp_input.bld_out_hsize;
	vpp_post_blend->bld_out_h = vpp_input.bld_out_vsize;

	osd_logd2("vpp_post_blend:bld_out: %d, %d\n",
		vpp_post_blend->bld_out_w,
		vpp_post_blend->bld_out_h);
	return 0;
}

static int vpp_post_padding_param_set(struct vpp_post_s *vpp_post)
{
	u32 bld_out_w;
	u32 padding_en = 0, pad_hsize = 0;

	if (!vpp_post)
		return -1;

	/* need check post blend out hsize */
	bld_out_w = vpp_post->vpp_post_blend.bld_out_w;
	switch (vpp_post->slice_num) {
	case 4:
		/* bld out need 32 aligned if 4 slices */
		if (bld_out_w % 32) {
			padding_en = 1;
			pad_hsize = ALIGN(bld_out_w, 32);
		} else {
			padding_en = 0;
			pad_hsize = bld_out_w;
		}
		break;
	case 2:
		/* bld out need 8 aligned if 2 slices */
		if (bld_out_w % 8) {
			padding_en = 1;
			pad_hsize = ALIGN(bld_out_w, 8);
		} else {
			padding_en = 0;
			pad_hsize = bld_out_w;
		}
		break;
	case 1:
		padding_en = 0;
		pad_hsize = bld_out_w;
		break;
	default:
		osd_loge("invalid slice_num[%d] number\n", vpp_post->slice_num);
		return -1;
	}
	vpp_post->vpp_post_pad.vpp_post_pad_en = padding_en;
	vpp_post->vpp_post_pad.vpp_post_pad_hsize = pad_hsize;
	vpp_post->vpp_post_pad.vpp_post_pad_rpt_lcol = 1;
	return 0;
}

static int vpp_post_proc_slice_param_set(struct vpp_post_s *vpp_post)
{
	u32 frm_hsize, frm_vsize;
	u32 slice_num, overlap_hsize;
	struct vpp_post_proc_slice_s *vpp_post_proc_slice = NULL;
	int i;

	if (!vpp_post)
		return -1;

	vpp_post_proc_slice = &vpp_post->vpp_post_proc.vpp_post_proc_slice;
	frm_hsize = vpp_post->vpp_post_pad.vpp_post_pad_hsize;
	frm_vsize = vpp_post->vpp_post_blend.bld_out_h;
	slice_num = vpp_post->slice_num;
	overlap_hsize = vpp_post->overlap_hsize;
	switch (slice_num) {
	case 4:
		for (i = 0; i < POST_SLICE_NUM; i++) {
			if (i == 0 || i == 3)
				vpp_post_proc_slice->hsize[i] =
					(frm_hsize + POST_SLICE_NUM - 1) /
					POST_SLICE_NUM + overlap_hsize;
			else
				vpp_post_proc_slice->hsize[i] =
					(frm_hsize + POST_SLICE_NUM - 1) /
					POST_SLICE_NUM + overlap_hsize * 2;
			vpp_post_proc_slice->vsize[i] = frm_vsize;
		}
		break;
	case 2:
		for (i = 0; i < POST_SLICE_NUM; i++) {
			if (i < 2) {
				vpp_post_proc_slice->hsize[i] =
					(frm_hsize + 2 - 1) /
					2 + overlap_hsize;
				vpp_post_proc_slice->vsize[i] = frm_vsize;
			} else {
				vpp_post_proc_slice->hsize[i] = 0;
				vpp_post_proc_slice->vsize[i] = 0;
			}
		}
		break;
	case 1:
		for (i = 0; i < POST_SLICE_NUM; i++) {
			if (i == 0) {
				vpp_post_proc_slice->hsize[i] = frm_hsize;
				vpp_post_proc_slice->vsize[i] = frm_vsize;
			} else {
				vpp_post_proc_slice->hsize[i] = 0;
				vpp_post_proc_slice->vsize[i] = 0;
			}
		}
		break;
	default:
		break;
	}
	return 0;
}

static int vpp_post_proc_hwin_param_set(struct vpp_post_s *vpp_post)
{
	u32 slice_num, overlap_hsize;
	struct vpp_post_proc_slice_s *vpp_post_proc_slice = NULL;
	struct vpp_post_proc_hwin_s *vpp_post_proc_hwin = NULL;
	int i;

	if (!vpp_post)
		return -1;
	vpp_post_proc_slice = &vpp_post->vpp_post_proc.vpp_post_proc_slice;
	vpp_post_proc_hwin = &vpp_post->vpp_post_proc.vpp_post_proc_hwin;
	slice_num = vpp_post->slice_num;
	overlap_hsize = vpp_post->overlap_hsize;

	switch (slice_num) {
	case 4:
		vpp_post_proc_hwin->hwin_en[0] = 1;
		vpp_post_proc_hwin->hwin_bgn[0] = 0;
		vpp_post_proc_hwin->hwin_end[0] =
			vpp_post_proc_slice->hsize[0] - overlap_hsize - 1;

		vpp_post_proc_hwin->hwin_en[1] = 1;
		vpp_post_proc_hwin->hwin_bgn[1] = overlap_hsize;
		vpp_post_proc_hwin->hwin_end[1] =
			vpp_post_proc_slice->hsize[1] - overlap_hsize - 1;

		vpp_post_proc_hwin->hwin_en[2] = 1;
		vpp_post_proc_hwin->hwin_bgn[2] = overlap_hsize;
		vpp_post_proc_hwin->hwin_end[2] =
			vpp_post_proc_slice->hsize[2] - overlap_hsize - 1;

		vpp_post_proc_hwin->hwin_en[3] = 1;
		vpp_post_proc_hwin->hwin_bgn[3] = overlap_hsize;
		vpp_post_proc_hwin->hwin_end[3] =
			vpp_post_proc_slice->hsize[3] - 1;
		break;
	case 2:
		vpp_post_proc_hwin->hwin_en[0] = 1;
		vpp_post_proc_hwin->hwin_bgn[0] = 0;
		vpp_post_proc_hwin->hwin_end[0] =
			vpp_post_proc_slice->hsize[0] - overlap_hsize - 1;

		vpp_post_proc_hwin->hwin_en[1] = 1;
		vpp_post_proc_hwin->hwin_bgn[1] = overlap_hsize;
		vpp_post_proc_hwin->hwin_end[1] =
			vpp_post_proc_slice->hsize[1] - 1;
		for (i = 2; i < POST_SLICE_NUM; i++) {
			vpp_post_proc_hwin->hwin_en[i] = 0;
			vpp_post_proc_hwin->hwin_bgn[i] = 0;
			vpp_post_proc_hwin->hwin_end[i] = 0;
		}
		break;
	case 1:
		vpp_post_proc_hwin->hwin_en[0] = 1;
		vpp_post_proc_hwin->hwin_bgn[0] = 0;
		vpp_post_proc_hwin->hwin_end[0] =
			vpp_post_proc_slice->hsize[0] - 1;
		for (i = 1; i < POST_SLICE_NUM; i++) {
			vpp_post_proc_hwin->hwin_en[i] = 0;
			vpp_post_proc_hwin->hwin_bgn[i] = 0;
			vpp_post_proc_hwin->hwin_end[i] = 0;
		}
		break;
	default:
		break;
	}
	return 0;
}

static int vpp_post_proc_param_set(struct vpp_post_s *vpp_post)
{
	struct vpp_post_proc_s *vpp_post_proc = NULL;

	vpp_post_proc = &vpp_post->vpp_post_proc;
	vpp_post_proc_slice_param_set(vpp_post);
	vpp_post_proc_hwin_param_set(vpp_post);
	vpp_post_proc->align_fifo_size[0] = 2048;
	vpp_post_proc->align_fifo_size[1] = 1536;
	vpp_post_proc->align_fifo_size[2] = 1024;
	vpp_post_proc->align_fifo_size[3] = 512;
	return 0;
}

/* calc all related vpp_post_param */
int vpp_post_param_set(struct vpp_post_s *vpp_post)
{
	int ret = 0;

	if (!vpp_post)
		return -1;
	memset(vpp_post, 0, sizeof(struct vpp_post_s));

	vpp_post->slice_num = vpp_input.slice_num;
	vpp_post->overlap_hsize = vpp_input.overlap_hsize;
	vpp_post_hwincut_param_set(vpp_post);

	ret = vpp_blend_param_set(&vpp_post->vpp_post_blend);
	if (ret < 0)
		return ret;

	ret = vpp_post_padding_param_set(vpp_post);
	if (ret < 0)
		return ret;

	ret = vpp_post_proc_param_set(vpp_post);
	if (ret < 0)
		return ret;

	return 0;
}

#if 0
static void vpp_post_win_cut_set(u32 vpp_index,
	struct vpp_post_s *vpp_post)
{
	//rdma_wr_op rdma_wr = cur_dev->rdma_func[vpp_index].rdma_wr;
	//struct vpp_post_misc_reg_s *vpp_reg = &vpp_post_reg.vpp_post_misc_reg;
	struct vpp_post_pad_s *vpp_post_pad = NULL;

	vpp_post_pad = &vpp_post->vpp_post_pad;
	//if (vpp_post_pad->vpp_post_pad_en &&
	//	)
}
#endif

void vpp_post_set(u32 vpp_index, struct vpp_post_s *vpp_post)
{
	if (!vpp_post)
		return;
	/* cfg slice mode */
	vpp_post_slice_set(vpp_index, vpp_post);
	/* cfg vd1 hwin cut */
	vpp_vd1_hwin_set(vpp_index, vpp_post);
	/* cfg vpp_blend */
	vpp_post_blend_set(vpp_index, &vpp_post->vpp_post_blend);
	/* vpp post units set */
	vpp_post_proc_set(vpp_index, vpp_post);
	/* cfg vpp_post pad if enable */
	vpp_post_padding_set(vpp_index, vpp_post);
	/* cfg vpp_post hwin cut if expected vpp post
	 * dout hsize less than blend or pad hsize
	 */
	//vpp_post_win_cut_set(vpp_index, vpp_post);
}

static void update_vpp_post_amdv_info(struct vpp_post_s *vpp_post)
{
	int i;

	vpp_post_amdv.slice_num = vpp_post->slice_num;
	vpp_post_amdv.overlap_hsize = vpp_post->overlap_hsize;
	vpp_post_amdv.vpp_post_blend_hsize = vpp_post->vpp_post_blend.bld_out_w;
	vpp_post_amdv.vpp_post_blend_vsize = vpp_post->vpp_post_blend.bld_out_h;
	for (i = 0; i < vpp_post->slice_num; i++) {
		vpp_post_amdv.slice[i].hsize =
			vpp_post->vpp_post_proc.vpp_post_proc_slice.hsize[i];
		vpp_post_amdv.slice[i].vsize =
			vpp_post->vpp_post_proc.vpp_post_proc_slice.vsize[i];
	}
}

static void vpp_post_blend_update_s5(void)
{
	struct vpp_post_s vpp_post;

	osd_logd2("%s,slice_num=%d, bld_out =%d, %d\n",
		__func__,
		vpp_input.slice_num,
		vpp_input.bld_out_hsize,
		vpp_input.bld_out_vsize);

	vpp_post_param_set(&vpp_post);
	vpp_post_set(0, &vpp_post);
	update_vpp_post_amdv_info(&vpp_post);
}

static void set_vpp_post_amdv_info(void)
{
	struct vinfo_s *vinfo = NULL;

	vinfo = vout_get_current_vinfo();
	update_vpp_input_info(vinfo);
	vpp_post_blend_update_s5();
}

struct vpp_post_info_t *get_vpp_post_amdv_info(void)
{
	set_vpp_post_amdv_info();

	return &vpp_post_amdv;
}
#endif

#ifdef OSD_SCALE_ENABLE
int video_scale_bitmap(void)
{
	int osd_index = -1;
	int axis[4] = {};
#ifdef AML_OSD_HIGH_VERSION
	struct pandata_s disp_data;
#endif
#ifdef AML_S5_DISPLAY
	struct vinfo_s *vinfo = NULL;

	vinfo = vout_get_current_vinfo();
#endif
	osd_logd2("video_scale_bitmap src w=%d, h=%d, dst w=%d, dst h=%d\n",
		fb_gdev.fb_width, fb_gdev.fb_height, fb_gdev.winSizeX, fb_gdev.winSizeY);

	vout_get_current_axis(axis);
	osd_index = get_osd_layer();
	if (osd_index < 0) {
		osd_loge("%s: invalid display_layer\n", __func__);
		return -1;
	}

	/* if viu2/viu3 has no scaler */
	if (is_vppx(osd_index) && !osd_hw.viux_scale_cap)
		goto no_scale;

#ifdef OSD_SUPERSCALE_ENABLE
	if ((fb_gdev.fb_width * 2 != fb_gdev.winSizeX) ||
	    (fb_gdev.fb_height * 2 != fb_gdev.winSizeY)) {
		osd_enable_hw(osd_index, 1);
		return (-1);
	}
	osd_set_free_scale_mode_hw(osd_index, 2);
#else
	osd_set_free_scale_mode_hw(osd_index, 1);
#endif
	osd_set_free_scale_axis_hw(osd_index, 0, 0, fb_gdev.fb_width - 1,
				   fb_gdev.fb_height - 1);
	osd_set_window_axis_hw(osd_index, axis[0], axis[1], axis[0] + axis[2] - 1,
			       axis[1] + axis[3] - 1);
	osd_set_free_scale_enable_hw(osd_index, 0x10001);

no_scale:
#ifdef AML_OSD_HIGH_VERSION
	disp_data.x_start = axis[0];
	disp_data.y_start = axis[1];
	disp_data.x_end = axis[0] + axis[2] - 1;
	disp_data.y_end = axis[1] + axis[3] - 1;
	if (osd_hw.osd_ver == OSD_HIGH_ONE && is_vpp0(osd_index))
		osd_update_blend(&disp_data);
#endif
#ifdef AML_S5_DISPLAY
	if (is_vpp0(osd_index)) {
		update_vpp_input_info(vinfo);
		vpp_post_blend_update_s5();
	}
#endif
	osd_enable_hw(osd_index, 1);

#ifdef VEHICLE_CONFIG
	/* default: OSD1 for logo, OSD2 for RTOS */
	if (!is_osd2_configed() && (osd_index == OSD1 || osd_index == OSD2)) {
		osd2_config_with_dimm(axis);
		transfer_info_to_rtos();
	}
#endif

	return (1);
}
#endif

/*
MIDDLE_MODE,RECT_MODE:
	fixed framebuffer size get from uboot env.
	bmp_image can be 0.
FULL_SCREEN_MODE:
	usd bmp size as framebuffer size;
	 must set bmp_image;
*/
void img_mode_set(u32 display_mode)
{
	fb_gdev.mode = display_mode;
}

void img_addr_set(ulong pic_image)
{
	g_pic_info.pic_image = pic_image;
}

void img_type_set(u32 type)
{
	g_pic_info.type = type;
}

void img_raw_size_set(u32 raw_width, u32 raw_height, u32 raw_bpp)
{
		g_pic_info.pic_width = raw_width;
		g_pic_info.pic_height = raw_height;
		g_pic_info.bpp = raw_bpp;
}

static int img_raw_init(void)
{
	unsigned int display_mode;

	if (img_video_init)
		return 0;

	display_mode = fb_gdev.mode;
#if 0
	if (display_mode == FULL_SCREEN_MODE) {
		g_pic_info.pic_width = env_strtoul("pic_width", 10);
		g_pic_info.pic_height = env_strtoul("pic_height", 10);
		g_pic_info.bpp = env_strtoul("pic_bpp", 10);
	}
#endif
	if (NULL == video_hw_init(display_mode)) {
		printf("Initialize video device failed!\n");
		return -1;
	}
	osd_logd2("raw_width=%d, raw_height=%d, raw_bpp=%d\n", g_pic_info.pic_width, g_pic_info.pic_height, g_pic_info.bpp);
	img_video_init = 1;
	return 0;
}

static int img_bmp_init(void)
{
	unsigned int display_mode;

	if (img_video_init)
		return 0;
	display_mode = fb_gdev.mode;
	if (display_mode == FULL_SCREEN_MODE) {
		if (g_pic_info.pic_image != 0)
			if (parse_bmp_info(g_pic_info.pic_image))
				return -1;
	}
	if (NULL == video_hw_init(display_mode)) {
		printf("Initialize video device failed!\n");
		return -1;
	}
	img_video_init = 1;
	return 0;
}

int img_osd_init(void)
{
	int ret = -1;

	if (g_pic_info.type == BMP_PIC)
		ret = img_bmp_init();
	else if(g_pic_info.type == RAW_PIC)
		ret = img_raw_init();
	return ret;
}

int img_bmp_display(ulong bmp_image, int x, int y)
{
	return video_display_bitmap(bmp_image, x, y);
}

int img_raw_display(ulong raw_image, int x, int y)
{
	return video_display_raw(raw_image, x, y);
}

int img_display(ulong bmp_image, int x, int y)
{
	int ret = -1;
	if (g_pic_info.type == BMP_PIC)
		ret = img_bmp_display(bmp_image, x, y);
	else if(g_pic_info.type == RAW_PIC)
		ret = img_raw_display(bmp_image, x, y);
	return ret;
}

int img_scale(void)
{
	int ret = -1;

	if (!img_video_init) {
		printf("fastboot osd not enabled!\n");
	}
#ifdef OSD_SCALE_ENABLE
	ret = video_scale_bitmap();
#endif
	return ret;
}

int img_osd_clear(void)
{
	if (!img_video_init) {
		printf("Please enable osd device first!\n");
		return 1;
	}

#ifdef OSD_SCALE_ENABLE
	memset((void *)(long long)(fb_gdev.frameAdrs), 0,
	       (fb_gdev.fb_width * fb_gdev.fb_height)*fb_gdev.gdfBytesPP);

	flush_cache(fb_gdev.frameAdrs,
		    ((fb_gdev.fb_width * fb_gdev.fb_height)*fb_gdev.gdfBytesPP));
#else
	memset((void *)(long long)(fb_gdev.frameAdrs), 0,
	       (fb_gdev.winSizeX * fb_gdev.winSizeY)*fb_gdev.gdfBytesPP);

	flush_cache(fb_gdev.frameAdrs,
		    ((fb_gdev.winSizeX * fb_gdev.winSizeY)*fb_gdev.gdfBytesPP));
#endif
	return 0;
}

void img_osd_uninit(void)
{
	img_video_init = 0;
}

static int _osd_hw_init(void)
{
	u32 fb_addr = 0;
	u32 display_width = 0;
	u32 display_height = 0;
	u32 display_bpp = 0;
	u32 color_index = 0;
	u32 fg = 0;
	u32 bg = 0;
	u32 fb_width = 0;
	u32 fb_height = 0;

	get_osd_version();
#ifdef CONFIG_AML_VOUT
	vout_init();
#endif
	fb_addr = get_fb_addr();
#ifdef OSD_SCALE_ENABLE
	fb_width = env_strtoul("fb_width", 10);
	fb_height = env_strtoul("fb_height", 10);
#endif
	display_width = env_strtoul("display_width", 10);
	display_height = env_strtoul("display_height", 10);
	display_bpp = env_strtoul("display_bpp", 10);
	color_index = env_strtoul("display_color_index", 10);
	fg = env_strtoul("display_color_fg", 16);
	bg = env_strtoul("display_color_bg", 16);

	/* fill in Graphic Device */
	fb_gdev.frameAdrs = fb_addr;
	fb_gdev.fb_width = fb_width;
	fb_gdev.fb_height = fb_height;
	fb_gdev.winSizeX = display_width;
	fb_gdev.winSizeY = display_height;
	fb_gdev.gdfBytesPP = display_bpp / 8;
	fb_gdev.fg = fg;
	fb_gdev.bg = bg;
	if ((color_index < ARRAY_SIZE(default_color_format_array))
	    && (default_color_format_array[color_index].color_index !=
		COLOR_INDEX_NULL))
		fb_gdev.gdfIndex = color_index;
	else {
		osd_loge("color_index %d invalid\n", color_index);
		return -1;
	}
	return 0;
}

static int osd_hw_init_by_index(u32 osd_index)
{
	if (_osd_hw_init() < 0)
		return -1;
	if (osd_index == OSD1) {
		osd_layer_init(&fb_gdev, OSD1);
	} else if (osd_index == OSD2) {
		if (osd_hw.osd_ver == OSD_SIMPLE) {
			osd_loge("AXG/A4/C3 not support osd2\n");
			return -1;
		}
		osd_layer_init(&fb_gdev, OSD2);
	} else if (osd_index == OSD3) {
		if (osd_hw.osd_ver == OSD_SIMPLE) {
			osd_loge("AXG/A4/C3 not support osd3\n");
			return -1;
		}
		osd_layer_init(&fb_gdev, OSD3);
	} else if (osd_index == OSD4) {
		if (osd_hw.osd_ver == OSD_SIMPLE) {
			osd_loge("AXG/A4/C3 not support osd4\n");
			return -1;
		}
		osd_layer_init(&fb_gdev, OSD4);
	} else {
		osd_loge("display_layer(%d) invalid\n", osd_index);
		return -1;
	}

	return 0;
}

static int video_display_osd(u32 osd_index)
{
	struct vinfo_s *info = NULL;
#if defined CONFIG_AML_VOUT
	info = vout_get_current_vinfo();
#endif
	ushort i, j;
	uchar *fb;
	unsigned long width, height;
#ifdef OSD_SCALE_ENABLE
	unsigned long pheight = fb_gdev.fb_height;
	unsigned long pwidth = fb_gdev.fb_width;
#else
	unsigned long pheight = info->width;
	unsigned long pwidth = info->height;
#endif
	int bpp, ret = 0;

	bpp = fb_gdev.gdfBytesPP;
	width = fb_gdev.fb_width;
	height = fb_gdev.fb_height;
	osd_set_free_scale_enable_hw(osd_index, 0x0);  // disable free_scale
	osd_set_dimm(osd_index, 0, 0);
	osd_enable_hw(osd_index, 1);
	fb   = (uchar *)(info->vd_base);
	osd_logd("fb=0x%p; width=%ld, height= %ld, bpp=%d\n",
		 fb, width, height, bpp);

	switch (bpp) {
	case 2:
		for (j = 0; j < height; j++) {
			for (i = 0; i < width; i++) {
				*(fb++) = 0x0;
				*(fb++) = 0xf8;
			}
		}
		break;
	case 3:
		for (j = 0; j < height; j++) {
			for (i = 0; i < width; i++) {
				*(fb++) = 0x0;
				*(fb++) = 0x0;
				*(fb++) = 0xff;
			}
		}
		break;
	case 4:
		for (j = 0; j < height; j++) {
			for (i = 0; i < width; i++) {
				*(fb++) = 0x0;
				*(fb++) = 0x0;
				*(fb++) = 0xff;
				*(fb++) = 0xff;
			}
		}
		break;
	default:
		osd_loge("%d bit/pixel is not support\n", bpp);
		ret = -1;
		break;
	}

	flush_cache((unsigned long)info->vd_base, pheight * pwidth * bpp);
	return ret;
}

u32 hist_max_min[4][100], hist_spl_val[4][100];
u32 hist_spl_pix_cnt[4][100], hist_cheoma_sum[4][100];

void hist_set_golden_data(void)
{
	u32 i = 0;
	u32 family_id = osd_get_chip_type();
	char *str = NULL;
	char *hist_env_key[16] = {"hist_max_min_osd0", "hist_spl_val_osd0",
				"hist_spl_pix_cnt_osd0", "hist_cheoma_sum_osd0",
				"hist_max_min_osd1", "hist_spl_val_osd1",
				"hist_spl_pix_cnt_osd1", "hist_cheoma_sum_osd1",
				"hist_max_min_osd2", "hist_spl_val_osd2",
				"hist_spl_pix_cnt_osd2", "hist_cheoma_sum_osd2",
				"hist_max_min_osd3", "hist_spl_val_osd3",
				"hist_spl_pix_cnt_osd3", "hist_cheoma_sum_osd3"};
	// GXL
	hist_max_min[OSD1][MESON_CPU_MAJOR_ID_GXL] = 0x3d3d;
	hist_spl_val[OSD1][MESON_CPU_MAJOR_ID_GXL] = 0xc4dad1;
	hist_spl_pix_cnt[OSD1][MESON_CPU_MAJOR_ID_GXL] = 0x33a25;
	hist_cheoma_sum[OSD1][MESON_CPU_MAJOR_ID_GXL] = 0xd4fd8a;

	hist_max_min[OSD2][MESON_CPU_MAJOR_ID_GXL] = 0x4f4f;
	hist_spl_val[OSD2][MESON_CPU_MAJOR_ID_GXL] = 0xfef16b;
	hist_spl_pix_cnt[OSD2][MESON_CPU_MAJOR_ID_GXL] = 0x33a25;
	hist_cheoma_sum[OSD2][MESON_CPU_MAJOR_ID_GXL] = 0xe85a68;

	// TXL
	hist_max_min[OSD1][MESON_CPU_MAJOR_ID_TXL] = 0x3d3d;
	hist_spl_val[OSD1][MESON_CPU_MAJOR_ID_TXL] = 0x78a1400;
	hist_spl_pix_cnt[OSD1][MESON_CPU_MAJOR_ID_TXL] = 0x1fa400;
	hist_cheoma_sum[OSD1][MESON_CPU_MAJOR_ID_TXL] = 0x8284800;

	hist_max_min[OSD2][MESON_CPU_MAJOR_ID_TXL] = 0x4f4f;
	hist_spl_val[OSD2][MESON_CPU_MAJOR_ID_TXL] = 0x9c39c00;
	hist_spl_pix_cnt[OSD2][MESON_CPU_MAJOR_ID_TXL] = 0x1fa400;
	hist_cheoma_sum[OSD2][MESON_CPU_MAJOR_ID_TXL] = 0x8e62000;

	// G12A
	hist_max_min[OSD1][MESON_CPU_MAJOR_ID_G12A]
						= hist_max_min[OSD2][MESON_CPU_MAJOR_ID_G12A]
						= 0x3d3d;
	hist_spl_val[OSD1][MESON_CPU_MAJOR_ID_G12A]
						= hist_spl_val[OSD2][MESON_CPU_MAJOR_ID_G12A]
						= 0xc4dcf6;
	hist_spl_pix_cnt[OSD1][MESON_CPU_MAJOR_ID_G12A]
						= hist_spl_pix_cnt[OSD2][MESON_CPU_MAJOR_ID_G12A]
						= 0x33a2e;
	hist_cheoma_sum[OSD1][MESON_CPU_MAJOR_ID_G12A]
						= hist_cheoma_sum[OSD2][MESON_CPU_MAJOR_ID_G12A]
						= 0xd4ffdc;
	//SC2
	hist_max_min[OSD1][MESON_CPU_MAJOR_ID_SC2]
						= hist_max_min[OSD2][MESON_CPU_MAJOR_ID_SC2]
						= 0xf700;
	hist_spl_val[OSD1][MESON_CPU_MAJOR_ID_SC2]
						= hist_spl_val[OSD2][MESON_CPU_MAJOR_ID_SC2]
						= 0x1e873c00;
	hist_spl_pix_cnt[OSD1][MESON_CPU_MAJOR_ID_SC2]
						= hist_spl_pix_cnt[OSD2][MESON_CPU_MAJOR_ID_SC2]
						= 0x1fa409;
	hist_cheoma_sum[OSD1][MESON_CPU_MAJOR_ID_SC2]
						= hist_cheoma_sum[OSD2][MESON_CPU_MAJOR_ID_SC2]
						= 0xfd20480;
	for (i = 0; i < 16; i++) {
		str = env_get(hist_env_key[i]);
		if (str) {
			switch (i%4) {
				case 0:
					hist_max_min[i/4][family_id] = simple_strtoul(str, NULL, 16);
					break;
				case 1:
					hist_spl_val[i/4][family_id] = simple_strtoul(str, NULL, 16);
					break;
				case 2:
					hist_spl_pix_cnt[i/4][family_id] = simple_strtoul(str, NULL, 16);
					break;
				case 3:
					hist_cheoma_sum[i/4][family_id] = simple_strtoul(str, NULL, 16);
					break;
			}
		}
	}
}

int osd_rma_test(u32 osd_index)
{
	u32 i = osd_index, osd_max = 1;
	u32 hist_result[4], osd_state;

	rma_test = 1;
	get_osd_version();
	if (osd_hw.osd_ver == OSD_SIMPLE) {
		osd_max = 0;
	} else if (osd_hw.osd_ver == OSD_HIGH_ONE) {
		osd_max = 3; /* 0/1/2/3 */
	}
	if (osd_index > osd_max) {
		osd_loge("=== osd%d is not supported, osd_max is %d ===\n", osd_index, osd_max);
		return (-1);
	}

	hist_set_golden_data();
	osd_logi("=== osd_rma_test for osd%d ===\n", i);
	osd_hw_init_by_index(i);
	if (-1 == video_display_osd(i)) {
		return (-1);
	}
	osd_hist_enable(osd_index);
	_udelay(50000);
	osd_state = osd_get_state(osd_index);
	osd_logd2("osd%d state:0x%x\n", osd_index, osd_state);
	osd_get_hist_stat(hist_result);
	_udelay(50000);
	osd_state = osd_get_state(osd_index);
	osd_logd2("osd%d state:0x%x\n", osd_index, osd_state);
	osd_get_hist_stat(hist_result);
	rma_test = 0;
#ifndef AML_C3_DISPLAY
	u32 family_id = osd_get_chip_type();

	if ((hist_result[0] == hist_max_min[osd_index][family_id]) && (hist_result[1] == hist_spl_val[osd_index][family_id]) &&
	    (hist_result[2] == hist_spl_pix_cnt[osd_index][family_id]) && (hist_result[3] == hist_cheoma_sum[osd_index][family_id])) {
			osd_logi("=== osd%d, osd_rma_test pass. ===\n", osd_index);
			return (0);
	} else {
			osd_loge("osd hist stat result:0x%x, 0x%x, 0x%x, 0x%x\n",
					hist_result[0], hist_result[1], hist_result[2], hist_result[3]);
			osd_loge("osd hist golden data:0x%x, 0x%x, 0x%x, 0x%x\n",
					hist_max_min[osd_index][family_id], hist_spl_val[osd_index][family_id],
					hist_spl_pix_cnt[osd_index][family_id], hist_cheoma_sum[osd_index][family_id]);
			osd_loge("=== osd%d, osd_rma_test failed. ===\n", osd_index);
			return (-1);
	}
#else
	/*second parameter according to  data in video_display_osd function which write to fb*/
	return test_for_c3(osd_index, 0xf800);
#endif
}
