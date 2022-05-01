/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "minui.h"
#include "bmp.h"
#include "minui_log.h"

#define SURFACE_DATA_ALIGNMENT 8

static grsurface *malloc_surface(size_t data_size)
{
	size_t size;
	unsigned char *temp;
	grsurface *surface;

	size = sizeof(grsurface) + data_size + SURFACE_DATA_ALIGNMENT;
	temp = (unsigned char*)(malloc(size));
	memset(temp, 0x0, size);
	if (temp == NULL)
		return NULL;
	surface = (grsurface *)(temp);
	surface->data = temp + sizeof(grsurface) +
		(SURFACE_DATA_ALIGNMENT - (sizeof(grsurface) % SURFACE_DATA_ALIGNMENT));
	return surface;
}

#ifdef NOT_USED
// "display" surfaces are transformed into the framebuffer's required
// pixel format (currently only RGBX is supported) at load time, so
// gr_blit() can be nothing more than a memcpy() for each row.  The
// next two functions are the only ones that know anything about the
// framebuffer pixel format; they need to be modified if the
// framebuffer format changes (but nothing else should).

// Allocate and return a grsurface* sufficient for storing an image of
// the indicated size in the framebuffer pixel format.
static grsurface *init_display_surface(unsigned int width, unsigned int height)
{
	grsurface *surface = malloc_surface(width * height * 4);

	if (!surface)
		return NULL;

	surface->width = width;
	surface->height = height;
	surface->row_bytes = width * 4;
	surface->pixel_bytes = 4;

	return surface;
}

// Copy 'input_row' to 'output_row', transforming it to the
// framebuffer pixel format.  The input format depends on the value of
// 'channels':
//
//   1 - input is 8-bit grayscale
//   3 - input is 24-bit RGB
//   4 - input is 32-bit RGBA/RGBX
//
// 'width' is the number of pixels in the row.
static void transform_rgb_to_draw(unsigned char* input_row,
                                  unsigned char* output_row,
                                  int channels, int width) {
	int x;
	unsigned char* ip = input_row;
	unsigned char* op = output_row;

	switch (channels) {
		case 1:
			// expand gray level to RGBX
			for (x = 0; x < width; ++x) {
				*op++ = *ip;
				*op++ = *ip;
				*op++ = *ip;
				*op++ = 0xff;
				ip++;
			}
			break;

		case 3:
			// expand RGBA to RGBX
			for (x = 0; x < width; ++x) {
				*op++ = *ip++;
				*op++ = *ip++;
				*op++ = *ip++;
				*op++ = 0xff;
			}
			break;

		case 4:
			// copy RGBA to RGBX
			memcpy(output_row, input_row, width*4);
			break;
		default:
			ui_logd("wrong channels.\n");
			break;
	}
}
#endif

int res_create_display_surface_from_addr(ulong addr, grsurface **psurface)
{
	grsurface *surface = NULL;
	BITMAPINFOHEADER stbmpinfoheader = {0};
	bmpinfo stbmpinfo = {0};
	unsigned int width = 0, height = 0;
	unsigned char *buffer = NULL;
	int ret = 0;

	*psurface = NULL;
	ret = read_bmp_from_addr(addr, &stbmpinfoheader, &buffer);
	if (ret < 0) {
		ui_loge("res_create_display_surface failed\n");
		return ret;
	}

	ui_logd("res_create_display_surface,name = %lx, buffer=%p\n", addr, buffer);
	ui_logd("stbmpinfoheader.bibitcount =%d\n", stbmpinfoheader.bibitcount);

	stbmpinfo.bgcolor = 0xff0000;
	stbmpinfo.fgcolor = 0xffff00;
	stbmpinfo.width = stbmpinfoheader.biwidth;
	stbmpinfo.height = stbmpinfoheader.biheight;
	stbmpinfo.bmpformat = stbmpinfoheader.bibitcount;
	stbmpinfo.bmpbuf = buffer;
	if (stbmpinfoheader.bibitcount < 32)
		stbmpinfoheader.bibitcount = 32;
	width = stbmpinfoheader.biwidth * stbmpinfoheader.bibitcount >> 3;
	height = stbmpinfoheader.biheight;

	surface = malloc_surface(width * height);
	if (!surface) {
		ret = -2;
		goto exit;
	}

	surface->width = stbmpinfoheader.biwidth;
	surface->height = height;
	surface->row_bytes = width;
	surface->pixel_bytes = stbmpinfoheader.bibitcount >> 3;

	ret = load_bmp(&stbmpinfo, surface->data);
	if (ret < 0) {
		ret = -3;
		goto exit;
	}

	*psurface = surface;

exit:
	if (ret < 0 && surface)
		free(surface);
	return ret;
}

int res_create_display_surface(const char *name, grsurface **psurface)
{
	grsurface *surface = NULL;
	BITMAPINFOHEADER stbmpinfoheader = {0};
	bmpinfo stbmpinfo = {0};
	unsigned int width = 0, height = 0;
	unsigned char *buffer = NULL;
	int ret = 0;

	*psurface = NULL;
	ret = read_bmp(name, &stbmpinfoheader, &buffer);
	if (ret < 0) {
		ui_loge("res_create_display_surface failed\n");
		return ret;
	}

	ui_logd("res_create_display_surface,name = %s, buffer=%p\n", name, buffer);
	ui_logd("stbmpinfoheader.bibitcount =%d\n", stbmpinfoheader.bibitcount);

	stbmpinfo.bgcolor = 0xff0000;
	stbmpinfo.fgcolor = 0xffff00;
	stbmpinfo.width = stbmpinfoheader.biwidth;
	stbmpinfo.height = stbmpinfoheader.biheight;
	stbmpinfo.bmpformat = stbmpinfoheader.bibitcount;
	stbmpinfo.bmpbuf = buffer;
	if (stbmpinfoheader.bibitcount < 32)
		stbmpinfoheader.bibitcount = 32;
	width = stbmpinfoheader.biwidth * stbmpinfoheader.bibitcount >> 3;
	height = stbmpinfoheader.biheight;

	surface = malloc_surface(width * height);
	if (surface == NULL) {
		ret = -2;
		goto exit;
	}

	surface->width = stbmpinfoheader.biwidth;
	surface->height = height;
	surface->row_bytes = width;
	surface->pixel_bytes = stbmpinfoheader.bibitcount >> 3;

	ret = load_bmp(&stbmpinfo, surface->data);
	if (ret < 0) {
		ret = -3;
		goto exit;
	}

	*psurface = surface;

exit:
	if (ret < 0 && surface != NULL) {
		free(surface);
	}
	return ret;
}

int res_create_alpha_surface(const char *name, grsurface **psurface)
{
	grsurface *surface = NULL;
	BITMAPINFOHEADER stbmpinfoheader = {0};
	bmpinfo stbmpinfo = {0};
	unsigned int width = 0, height = 0;
	unsigned char *buffer = NULL;
	int ret = 0;

	*psurface = NULL;
	ret = read_bmp(name, &stbmpinfoheader, &buffer);
	if (ret < 0) {
		return ret;
	}

	stbmpinfo.bgcolor = 0xfffffff;
	stbmpinfo.fgcolor = 0xfffffff;
	stbmpinfo.width = stbmpinfoheader.biwidth;
	stbmpinfo.height = stbmpinfoheader.biheight;
	stbmpinfo.bmpformat = stbmpinfoheader.bibitcount;
	stbmpinfo.bmpbuf = buffer;
	width = stbmpinfoheader.biwidth * stbmpinfoheader.bibitcount;
	height = stbmpinfoheader.biheight;
	ui_logd("width=%d, height =%d stbmpinfoheader.biwidth=%d, stbmpinfoheader.bibitcount=%d\n",
		width, height,
		stbmpinfoheader.biwidth,
		stbmpinfoheader.bibitcount);

	surface = malloc_surface(width * height);
	if (surface == NULL) {
		ret = -2;
		goto exit;
	}

	surface->width = stbmpinfoheader.biwidth;
	surface->height = height;
	surface->row_bytes = width;
	surface->pixel_bytes = 1;

	ret = load_bmp(&stbmpinfo, surface->data);
	if (ret < 0) {
		ret = -3;
		goto exit;
	}

	*psurface = surface;

exit:
	if (ret < 0 && surface != NULL) {
		free(surface);
	}
	return ret;
}

void res_free_surface(grsurface *surface)
{
	if (surface) {
		free(surface);
		surface = NULL;
	}
}
