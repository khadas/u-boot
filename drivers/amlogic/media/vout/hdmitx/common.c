// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#ifdef CONFIG_AML_HDMITX20
#include <amlogic/media/vout/hdmitx/hdmitx.h>
#else
#include <amlogic/media/vout/hdmitx21/hdmitx.h>
#endif

int hdmitx_likely_frac_rate_mode(char *m)
{
	if (strstr(m, "24hz") || strstr(m, "30hz") || strstr(m, "60hz") ||
		strstr(m, "120hz") || strstr(m, "240hz"))
		return 1;
	else
		return 0;
}

bool hdmitx_get_hpd_state_ext(void)
{
#ifdef CONFIG_AML_HDMITX20
	struct hdmitx_dev *hdev = hdmitx_get_hdev();
#else
	struct hdmitx_dev *hdev = get_hdmitx21_device();
#endif

	return hdev->hpd_state;
}
