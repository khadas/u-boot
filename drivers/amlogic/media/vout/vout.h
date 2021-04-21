/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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
