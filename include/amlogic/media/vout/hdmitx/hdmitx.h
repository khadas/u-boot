#ifndef __HDMITX_H_
#define __HDMITX_H_
#include "hdmi_common.h"
#include "hdmitx_module.h"
#include "hdmitx_reg.h"
#include "mach_reg.h"

int hdmitx_likely_frac_rate_mode(char *m);
int hdmi_outputmode_check(char *mode, unsigned int frac);

#endif
