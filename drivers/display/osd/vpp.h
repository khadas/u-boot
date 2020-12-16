/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/osd/vpp.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _VPP_H_
#define _VPP_H_

#define VPP_OSD2_PREBLEND           (1 << 17)
#define VPP_OSD1_PREBLEND           (1 << 16)
#define VPP_VD2_PREBLEND            (1 << 15)
#define VPP_VD1_PREBLEND            (1 << 14)
#define VPP_OSD2_POSTBLEND          (1 << 13)
#define VPP_OSD1_POSTBLEND          (1 << 12)
#define VPP_VD2_POSTBLEND           (1 << 11)
#define VPP_VD1_POSTBLEND           (1 << 10)
#define VPP_POSTBLEND_EN            (1 << 7)
#define VPP_PRE_FG_OSD2             (1 << 5)
#define VPP_PREBLEND_EN             (1 << 6)
#define VPP_POST_FG_OSD2            (1 << 4)

void set_vpp_matrix(int m_select, int *s, int on);

#endif
