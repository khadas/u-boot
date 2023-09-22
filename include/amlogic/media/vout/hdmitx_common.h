/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __HDMITX_CMN_H__
#define __HDMITX_CMN_H__

/* the default max_tmds_clk is 165MHz/5 in H14b Table 8-16 */
#define DEFAULT_MAX_TMDS_CLK    33

#define GET_OUI_BYTE0(oui)      ((oui) & 0xff) /* Little Endian */
#define GET_OUI_BYTE1(oui)      (((oui) >> 8) & 0xff)
#define GET_OUI_BYTE2(oui)      (((oui) >> 16) & 0xff)

int hdmitx_likely_frac_rate_mode(char *m);

enum amhdmitx_chip_e {
	MESON_CPU_ID_SC2,
	MESON_CPU_ID_T7,
	MESON_CPU_ID_S5,
};

#endif
