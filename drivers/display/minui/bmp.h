/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/minui/bmp.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _BMP_H_
#define _BMP_H_

typedef struct tagBITMAPFILEHEADER {
	unsigned int bfsize;
	unsigned int bfReserved1;
	unsigned int bfoffbits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	unsigned int biSize;
	unsigned int biwidth;
	unsigned int biheight;
	unsigned short biPlanes;
	unsigned short bibitcount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned int biXPelsPerMeter;
	unsigned int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
} BITMAPINFOHEADER;

typedef struct
{
	unsigned short wwidth;  // in pixels
	unsigned short wheight; // in pixels
	unsigned long bgcolor;  //background color, used in bmp 1
	unsigned long fgcolor;  //foreground color, used in bmp 1
	unsigned char *pstart;  // bitmap data pointer
} gfxbitmap;

typedef struct {
	unsigned int bmpformat;
	unsigned int bgcolor;
	unsigned int fgcolor;
	unsigned short width;
	unsigned short height;
	unsigned char *bmpbuf;
} bmpinfo;

int read_bmp(const char *filename, BITMAPINFOHEADER *pstbmpinfoheader, unsigned char **buffer);
int load_bmp(bmpinfo *pbmpinfo, unsigned char *out_buffer);
int read_bmp_from_addr(long bmp_addr, BITMAPINFOHEADER *pstbmpinfoheader, unsigned char **buffer);
#endif
