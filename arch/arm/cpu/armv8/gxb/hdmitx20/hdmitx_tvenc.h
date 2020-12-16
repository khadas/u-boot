/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/gxb/hdmitx20/hdmitx_tvenc.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __HDMI_TX_TVENC_H__
#define __HDMI_TX_TVENC_H__

#include <common.h>
#include <amlogic/hdmi.h>

struct reg_t {
	unsigned int reg;
	unsigned int val;
};

struct enc_reg_set {
	unsigned int addr;
	unsigned int val;
};

struct enc_reg_map {
	enum hdmi_vic vic;
	struct enc_reg_set *set;
};

void set_vmode_enc_hw(enum hdmi_vic vic);

#endif
