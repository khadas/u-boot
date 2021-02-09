/*
 * drivers/amlogic/media/vout/vout.h
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _VOUT_H
#define _VOUT_H

struct vout_conf_s {
	unsigned int viu_valid[3];
	unsigned int viu_mux_reg;

	void (*viu_mux)(int index, unsigned int mux_sel);
	void (*reg_dump)(void);
};

struct vout_set_s {
	char *name;
	int mode;
	ulong width;
	ulong height;
	ulong field_height;
	int viu_color_fmt;
	unsigned int viu_mux;
};

#endif
