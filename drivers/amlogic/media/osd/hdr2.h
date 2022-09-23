/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __HDR2_H__
#define __HDR2_H__

enum hdr_module_sel {
	VD1_HDR = 0x1,
	VD2_HDR = 0x2,
	VD3_HDR = 0x4,
	OSD1_HDR = 0x8,
	OSD2_HDR = 0x10,
	VDIN0_HDR = 0x20,
	VDIN1_HDR = 0x40,
	DI_HDR = 0x80,
	OSD3_HDR = 0x100,
	OSD4_HDR = 0x200,
	RGB_OSD = 0x20000000,
	HDR_MAX
};

enum hdr_process_sel {
	HDR_BYPASS = 0x1,
	HDR_SDR = 0x2,
	SDR_HDR = 0x4,
	HLG_BYPASS = 0x8,
	HLG_SDR = 0x10,
	HLG_HDR = 0x20,
	SDR_HLG = 0X40,
	HDRPLUS_SDR = 0x80,
	HDR_OFF = 0x100,
	RGB_BYPASS = 0x200,
	HDR_p_MAX
};

extern void hdr_func(enum hdr_module_sel module_sel,
	enum hdr_process_sel hdr_process_select);

#endif
