/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __HDMITX21_EXT_H__
#define __HDMITX21_EXT_H__

#include "../hdmitx_common.h"

void hdmitx21_init(void);
void hdmitx21_pxp_init(bool pxp_mode);
void hdmitx21_chip_type_init(enum amhdmitx_chip_e type);
//hpd state used by external module
bool hdmitx_get_hpd_state_ext(void);

#endif
