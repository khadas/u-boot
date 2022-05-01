/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/minui/bmp.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "bmp.h"
#include "minui_log.h"

//#define BMP1_DISPLAY  1
static int BMP_to_ARGB8888(const gfxbitmap *getmap, unsigned char *out_buffer)
{
	unsigned char *pget;
	unsigned char *pput;
	int i;
	int width;
	int height;

	if (!getmap || !getmap->pstart || !out_buffer) {
		ui_loge("BMP_to_ARGB8888 parameter err!\n");
		return -1;
	}

	pget = getmap->pstart;
	pput = out_buffer;
	width = getmap->wwidth;
	height = getmap->wheight;

	pget += getmap->wwidth * (getmap->wheight - 1) * 4;

	for (i = 0; i < height; i++) {
		memcpy(pput, pget, width * 4);
		pput += width * 4;
		pget -= width * 4;
	}

	return 0;
}

static int BMP24_to_ARGB8888(const gfxbitmap *getmap, unsigned char *out_buffer)
{
	unsigned char *pget;
	unsigned char *pput;
	int i,j;
	int width;
	int height;
	int rowsize = 0;
	unsigned char *ptemp;

	if (!getmap || !getmap->pstart || !out_buffer) {
		ui_loge("BMP24_to_ARGB8888 parameter err!\n");
		return -1;
	}

	pget = getmap->pstart;
	pput = out_buffer;
	width = getmap->wwidth;
	height = getmap->wheight;
	rowsize = 4 * (int)((getmap->wwidth * 24 + 31) / 32);
	pget += rowsize * (getmap->wheight - 1);

	for (i = 0; i < height; i++) {
		ptemp = pget;
		for (j = 0; j < width; j++) {
			*(pput++) = *ptemp++;
			*(pput++) = *ptemp++;
			*(pput++) = *ptemp++;
			*(pput++) = 0xFF;
		}
		pget -= rowsize;
	}

	return 0;
}

static int BMP16_to_ARGB8888(const gfxbitmap *getmap, unsigned char *out_buffer)
{
	unsigned char *pget;
	unsigned char *pput;
	int i,j;
	int width;
	int height;
	int rowsize = 0;
	unsigned char *ptemp;

	if (!getmap || !getmap->pstart || !out_buffer) {
		ui_loge("BMP24_to_ARGB8888 parameter err!\n");
		return -1;
	}

	pget = getmap->pstart;
	pput = out_buffer;
	width = getmap->wwidth;
	height = getmap->wheight;
	rowsize = 4 * (int)((getmap->wwidth * 16 + 31) / 32);
	pget += rowsize * (getmap->wheight - 1);

	for (i = 0; i < height; i++) {
		ptemp = pget;
		for (j = 0; j < width; j++) {
			*(pput++) = (*ptemp & 0x1F) << 3;
			*(pput++) = (((*(ptemp + 1) & 0x7) << 3) | ((*ptemp & 0xE0) >> 5)) << 2;
			*(pput++) = ((*(ptemp + 1)  & 0xF8) >> 3) << 3;
			*(pput++) = 0xFF;
			ptemp += 2;
		}
		pget -= rowsize;
	}

	return 0;
}

#ifdef BMP1_DISPLAY
static int BMP1_to_ARGB8888(const gfxbitmap *getmap, unsigned char *out_buffer)
{
	unsigned char *pget;
	unsigned char *pput;
	unsigned char *pbmpbuf = NULL;
	int i,j;
	int rowsize = 0;
	int width;
	int height;
	unsigned int usForeColor;
	unsigned int usBackColor;

	if (!getmap || !getmap->pstart || !out_buffer) {
		ui_loge("BMP1_to_ARGB8888 parameter err!\n");
		return -1;
	}

	pget = getmap->pstart;
	pput = out_buffer;

	width = getmap->wwidth;
	height = getmap->wheight;
	usForeColor = getmap->fgcolor;
	usBackColor = getmap->bgcolor;
	rowsize = 4 * (int)((getmap->wwidth + 31) / 32);
	pbmpbuf = getmap->pstart + rowsize * (getmap->wheight - 1);
	pget += getmap->wwidth * getmap->wheight;
	for (i = 0; i < height; i ++) {
		for (j = 0; j < width; j++) {
			if (((0x01 << (7 - (j & 0x07))) & pbmpbuf[j >> 3]) != 0) {
				/* it is foreground */
				*(pput++) = usForeColor & 0xFF;
				*(pput++) = (usForeColor >> 8) & 0xFF;
				*(pput++) = (usForeColor >> 16) & 0xFF;
				*(pput++) = 0xFF;
			} else {
				/* it is background */
				*(pput++) = usBackColor & 0xFF;
				*(pput++) = (usBackColor >> 8) & 0xFF;
				*(pput++) = (usBackColor >> 16) & 0xFF;
				*(pput++) = 0xFF;
			}
		}
		pbmpbuf -= rowsize;
	}
	return 0;
}
#endif

static int BMP1_to_ARGB8(const gfxbitmap *getmap, unsigned char *out_buffer)
{
	unsigned char *pget;
	unsigned char *pput;
	unsigned char *pbmpbuf = NULL;
	int i,j;
	int rowsize = 0;
	int width;
	int height;

	if (!getmap || !getmap->pstart || !out_buffer) {
		ui_loge("BMP1_to_ARGB8888 parameter err!\n");
		return -1;
	}
	pget = getmap->pstart;
	pput = out_buffer;

	width = getmap->wwidth;
	height = getmap->wheight;
	rowsize = 4 * (int)((getmap->wwidth + 31) / 32);
	pbmpbuf = getmap->pstart + rowsize * (getmap->wheight - 1);
	ui_logd("width=%d,heigth=%d, rowsize=%d\n",width, height, rowsize);
	pget += getmap->wwidth * getmap->wheight;
	for (i = 0; i < height; i ++) {
		for (j = 0; j < width; j++) {
			if (((0x01 << (7 - (j & 0x07))) & pbmpbuf[j >> 3]) != 0) {
				/* it is foreground */
				*(pput++) = 0xff;
			} else {
				/* it is background */
				*(pput++) = 0x0;
			}
		}
		pbmpbuf -= rowsize;
	}
	return 0;
}

int load_bmp(bmpinfo *pbmpinfo, unsigned char *out_buffer)
{
	int iret = 0;
	gfxbitmap tgfxbmp;

	if (!pbmpinfo) {
		ui_loge("pbmpinfo is NULL.\n");
		return -1;
	}
	ui_logd("[disp_bmp]: pbmpinfo 0x%p, Width %d, Height %d!\n",
		pbmpinfo, pbmpinfo->width, pbmpinfo->height);
	memset(&tgfxbmp, 0, sizeof(tgfxbmp));
	tgfxbmp.bgcolor = pbmpinfo->bgcolor;
	tgfxbmp.fgcolor = pbmpinfo->fgcolor;
	tgfxbmp.pstart = pbmpinfo->bmpbuf;
	tgfxbmp.wheight = pbmpinfo->height;
	tgfxbmp.wwidth = pbmpinfo->width;
	ui_logd("pbmpinfo->bmpformat=%d\n",pbmpinfo->bmpformat);
	switch (pbmpinfo->bmpformat) {
		case 1:
#ifdef BMP1_DISPLAY
			iret = BMP1_to_ARGB8888(&tgfxbmp, out_buffer);
#else
			iret = BMP1_to_ARGB8(&tgfxbmp, out_buffer);
#endif
			break;
		case 16:
			iret = BMP16_to_ARGB8888(&tgfxbmp, out_buffer);
			break;
		case 24:
			iret = BMP24_to_ARGB8888(&tgfxbmp, out_buffer);
			break;
		case 32:
			iret = BMP_to_ARGB8888(&tgfxbmp, out_buffer);
			break;
	}
	return iret;
}

long load_pic_from_partition(const char *pic_name)
{
	char str[128] = {0};
	int ret = 0;

	if (pic_name == NULL) {
		ui_loge("wrong pic_name.\n");
		return -1;
	}

	sprintf(str, "imgread pic logo %s $loadaddr", pic_name);
	ret = run_command(str, 0);
	if (ret != 0) {
		ui_loge("run_command fail.\n");
		return -2;
	}

	sprintf(str, "%s_offset", pic_name);
	return getenv_ulong(str, 16, 0);
}

int read_bmp(const char *filename, BITMAPINFOHEADER *pstbmpinfoheader, unsigned char **buffer)
{
	unsigned short bftype = 0x4d42;
	BITMAPFILEHEADER stbmpfileheader = {0};
	BITMAPINFOHEADER stbmpinfoheader = {0};
	unsigned char *data_buffer = NULL;
	long bmp_addr;

	bmp_addr = load_pic_from_partition(filename);
	if (bmp_addr <= 0) {
		ui_loge("load_pic_from_partition fail.\n");
		return -1;
	}
	bftype = le16_to_cpu(*(unsigned short *)(bmp_addr));

	if (bftype != 0x4d42) {
		ui_loge("bftype=0x%x failed\n", bftype);
		return -1;
	}

	stbmpfileheader  = *(BITMAPFILEHEADER *)(bmp_addr + sizeof(bftype));
	stbmpinfoheader = *(BITMAPINFOHEADER *)(bmp_addr +
		sizeof(bftype) + sizeof(stbmpfileheader));

	stbmpinfoheader.biwidth     = le32_to_cpu(stbmpinfoheader.biwidth);
	stbmpinfoheader.biheight    = le32_to_cpu(stbmpinfoheader.biheight);
	stbmpinfoheader.bibitcount = le16_to_cpu(stbmpinfoheader.bibitcount);
	stbmpfileheader.bfsize        = le32_to_cpu(stbmpfileheader.bfsize);
	stbmpfileheader.bfoffbits    =  le32_to_cpu(stbmpfileheader.bfoffbits);
	data_buffer = (unsigned char *)(bmp_addr + stbmpfileheader.bfoffbits);
	memcpy(pstbmpinfoheader, &stbmpinfoheader, sizeof(stbmpinfoheader));
	*buffer = data_buffer;
	ui_logd("bfsize=%d,bfoffbits=%d, data_buffer = 0x%p\n",
		stbmpfileheader.bfsize,
		stbmpfileheader.bfoffbits,
		data_buffer);
	ui_logd("BmpHeader:biwidth=%d, biheight=%d,bibitcount=%d\n",
		stbmpinfoheader.biwidth,
		stbmpinfoheader.biheight,
		stbmpinfoheader.bibitcount);

	return 0;
}

int read_bmp_from_addr(long bmp_addr, BITMAPINFOHEADER *pstbmpinfoheader, unsigned char **buffer)
{
	unsigned short bftype = 0x4d42;
	BITMAPFILEHEADER stbmpfileheader = {0};
	BITMAPINFOHEADER stbmpinfoheader = {0};
	unsigned char *data_buffer = NULL;

	if (bmp_addr <= 0) {
		ui_loge("load_pic_from_partition fail.\n");
		return -1;
	}
	bftype = le16_to_cpu(*(unsigned short *)(bmp_addr));

	if (bftype != 0x4d42) {
		ui_loge("bftype=0x%x failed\n", bftype);
		return -1;
	}

	stbmpfileheader  = *(BITMAPFILEHEADER *)(bmp_addr + sizeof(bftype));
	stbmpinfoheader = *(BITMAPINFOHEADER *)(bmp_addr +
		sizeof(bftype) + sizeof(stbmpfileheader));

	stbmpinfoheader.biwidth     = le32_to_cpu(stbmpinfoheader.biwidth);
	stbmpinfoheader.biheight    = le32_to_cpu(stbmpinfoheader.biheight);
	stbmpinfoheader.bibitcount = le16_to_cpu(stbmpinfoheader.bibitcount);
	stbmpfileheader.bfsize        = le32_to_cpu(stbmpfileheader.bfsize);
	stbmpfileheader.bfoffbits    =  le32_to_cpu(stbmpfileheader.bfoffbits);
	data_buffer = (unsigned char *)(bmp_addr + stbmpfileheader.bfoffbits);
	memcpy(pstbmpinfoheader, &stbmpinfoheader, sizeof(stbmpinfoheader));
	*buffer = data_buffer;
	ui_logd("bfsize=%d,bfoffbits=%d, data_buffer = 0x%p\n",
		stbmpfileheader.bfsize,
		stbmpfileheader.bfoffbits,
		data_buffer);
	ui_logd("BmpHeader:biwidth=%d, biheight=%d,bibitcount=%d\n",
		stbmpinfoheader.biwidth,
		stbmpinfoheader.biheight,
		stbmpinfoheader.bibitcount);

	return 0;
}
