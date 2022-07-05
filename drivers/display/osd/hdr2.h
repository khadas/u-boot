/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/osd/hdr2.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __HDR2_H__
#define __HDR2_H__

enum hdr_module_sel {
	VD1_HDR = 0x1,
	VD2_HDR = 0x2,
	OSD1_HDR = 0x4,
	VDIN0_HDR = 0x8,
	VDIN1_HDR = 0x10,
	DI_HDR = 0x20,
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
