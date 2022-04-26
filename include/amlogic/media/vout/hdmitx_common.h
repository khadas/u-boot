/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __HDMITX_CMN_H__
#define __HDMITX_CMN_H__

#define GET_OUI_BYTE0(oui)      ((oui) & 0xff) /* Little Endian */
#define GET_OUI_BYTE1(oui)      (((oui) >> 8) & 0xff)
#define GET_OUI_BYTE2(oui)      (((oui) >> 16) & 0xff)

int hdmitx_likely_frac_rate_mode(char *m);

#endif
