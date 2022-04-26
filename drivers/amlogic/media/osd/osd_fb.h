/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _OSD_FB_H_
#define _OSD_FB_H_

void img_mode_set(u32 display_mode);
void img_addr_set(ulong pic_image);
void img_type_set(u32 type);
int img_osd_init(void);
int img_bmp_display(ulong bmp_image, int x, int y);
int img_raw_display(ulong raw_image, int x, int y);
int img_osd_clear(void);
void img_osd_uninit(void);
int img_display(ulong bmp_image, int x, int y);
int img_scale(void);
void img_raw_size_set(u32 raw_width, u32 raw_height, u32 raw_bpp);

#endif
