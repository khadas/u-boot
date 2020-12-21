/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/usb/gadget/v3_burning/v3_usb_tool/cmd_aml_dnl.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <command.h>
//#include <console.h>
#include <g_dnl.h>

extern void dwc_otg_power_off_phy_fb(void);
extern int aml_dnl_register(const char *name);
extern void aml_dnl_unregister(void);
#define SOF_WAIT_TIME_MIN	500 //400ms for wait sof, need more than wcp

extern unsigned int _sofintr_not_occur;
unsigned int adnl_enum_timeout;
static unsigned int time_out_val;
unsigned int adnl_identify_timeout = 0;
static unsigned int time_out_wait_sof;
#if (defined CONFIG_USB_DEVICE_V2)
extern unsigned int fb_sofintr;
extern unsigned fb_curTime_sof;
#endif

static void usb_parameter_init(int time_out)
{
	adnl_enum_timeout = adnl_identify_timeout = 0;//clear every time

	if (time_out) {
		time_out_val = time_out;
		adnl_enum_timeout = get_timer(0);/* clear utimer */
	}

	time_out_wait_sof   =
		(SOF_WAIT_TIME_MIN * 2 > time_out_val) ? SOF_WAIT_TIME_MIN : (time_out_val >> 1);
	_sofintr_not_occur  = 1;

	return;
}

static int do_aml_DNL(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret;
	unsigned timeout = (2 <= argc) ? simple_strtoul(argv[1], NULL, 0) : 0;
	unsigned pcToolWaitTime	= (3 <= argc) ? simple_strtoul(argv[2], NULL, 0) : 0;

	usb_parameter_init(timeout);

	ret = aml_dnl_register("usb_dnl_amlogic");
	if (ret)
		return ret;

	while (1) {
		if (ctrlc()) break;

		if (adnl_enum_timeout) {
			unsigned curTime	= get_timer(adnl_enum_timeout);
			if (curTime > time_out_wait_sof && _sofintr_not_occur) {
				printf("no sof when timeout %d\n", time_out_wait_sof);
				dwc_otg_power_off_phy_fb();
				aml_dnl_unregister();
				return 2;
			}
			if (!_sofintr_not_occur && curTime > time_out_val) {
				dwc_otg_power_off_phy_fb();
				aml_dnl_unregister();
				printf("get sof but timeout: cur %u > %u + %u\n",
						curTime, time_out_val, adnl_enum_timeout);
				return 2;
			}
			if (!_sofintr_not_occur) {
				adnl_enum_timeout = 0;//sof int already, not need checked from now on
				adnl_identify_timeout = get_timer(0);
			}
		}

		if (adnl_identify_timeout && pcToolWaitTime) {
			const unsigned waitIdentifyTime = get_timer(adnl_identify_timeout);
			if (waitIdentifyTime > pcToolWaitTime) {
				printf("waitIdentifyTime(%u) > timeout(%u)\n", waitIdentifyTime, pcToolWaitTime);
				aml_dnl_unregister();
				return __LINE__;
			}
		}

#if (defined CONFIG_USB_DEVICE_V2)
		if ((get_timer(fb_curTime_sof) > 0x200) && (fb_sofintr)) {
			fb_sofintr = 0;
			dwc_otg_power_off_phy_fb();
		}
#endif
		usb_gadget_handle_interrupts();
	}

	aml_dnl_unregister();
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
        adnl,	3,	0,	do_aml_DNL,
        "use Amlogic DNL protocol",
        "    [timeout_enum] [timeout_identify]"
);

