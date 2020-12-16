/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/vout/vdac.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _AML_VDAC_H
#define _AML_VDAC_H

#define VDAC_MODULE_MASK      (0x1f)
#define VDAC_MODULE_AVOUT_ATV (1 << 0) /*0x1*/
#define VDAC_MODULE_DTV_DEMOD (1 << 1) /*0x2*/
#define VDAC_MODULE_AVOUT_AV  (1 << 2) /*0x4*/
#define VDAC_MODULE_CVBS_OUT  (1 << 3) /*0x8*/
#define VDAC_MODULE_AUDIO_OUT (1 << 4) /*0x10*/

#define VDAC_CTRL_MAX         10

#define VDAC_REG_MAX          0xffff

struct meson_vdac_ctrl_s {
	unsigned int reg;
	unsigned int val;
	unsigned int bit;
	unsigned int len;
};

void vdac_enable(bool on, unsigned int module_sel);
int vdac_vref_adj(unsigned int value);
void vdac_ctrl_config_probe(void);

#endif
