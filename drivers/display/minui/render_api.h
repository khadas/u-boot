/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/minui/render_api.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _RENDER_API_H_
#define _RENDER_API_H_

#include "minui.h"

void set_fastboot_flag(int flag);
int screen_init(void);
void screen_uninit(void);
int gr_init_ext_font(const char *font, grfont **dest);
int surface_loadbmp(grsurface **surface, const char *filename);
int surface_loadbmp_from_addr(grsurface **surface, long addr);
void surface_disaplay(grsurface *surface, int sx, int sy, int dx, int dy);
void screen_setcolor(unsigned int color);
void screen_drawtextline(const grfont *font, int x, int y, const char *s, bool bold);
void screen_fillrect(int x, int y, int w, int h);
void screen_update(void);

#endif
