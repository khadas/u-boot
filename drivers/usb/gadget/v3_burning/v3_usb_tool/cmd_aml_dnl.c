// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <console.h>
#include <g_dnl.h>
#include <asm/io.h>
#include <asm/arch/register.h>

int optimus_erase_bootloader(const char* extBootDev);
extern int aml_dnl_register(const char *name);
extern void aml_dnl_unregister(void);
#define SOF_WAIT_TIME_MIN	500 //400ms for wait sof, need more than wcp

extern unsigned int _sofintr_not_occur;
unsigned int adnl_enum_timeout;
static unsigned int time_out_val;
unsigned int adnl_identify_timeout = 0;
static unsigned int time_out_wait_sof;
#if (defined CONFIG_USB_DEVICE_V2)
extern unsigned int _sofintr;
extern unsigned curTime_sof;
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

int aml_v3_usbburning(unsigned timeout, unsigned pcToolWaitTime)
{
	int ret;

	usb_parameter_init(timeout);

	ret = aml_dnl_register("usb_dnl_amlogic");
	if (ret)
		return ret;

#ifdef  CONFIG_USB_GADGET_CRG
	if (!g_dnl_board_usb_cable_connected()) {
		puts("\rUSB cable not detected.\n" \
				"Command exit.\n");
		ret = CMD_RET_FAILURE;
		return ret;
	}
#endif

	while (1) {
		if (ctrlc()) break;

		if (adnl_enum_timeout) {
			unsigned curTime	= get_timer(adnl_enum_timeout);
			if (curTime > time_out_wait_sof && _sofintr_not_occur) {
				printf("noSof\n");
				dwc_otg_power_off_phy_fb();
				aml_dnl_unregister();
				return 2;
			}
			if (curTime > time_out_val) {
				dwc_otg_power_off_phy_fb();
				aml_dnl_unregister();
				printf("Try connect time out %u, %u, %u\n",
						curTime, time_out_val, adnl_enum_timeout);
				return 2;
			}
		}

		if (adnl_identify_timeout && pcToolWaitTime) {
			const unsigned waitIdentifyTime = get_timer(adnl_identify_timeout);
			if (waitIdentifyTime > pcToolWaitTime) {
				printf("waitIdentifyTime(%u) > timeout(%u)\n", waitIdentifyTime, pcToolWaitTime);
				pcToolWaitTime = 0;//don't re-enter even plug-out and plug-in
				adnl_identify_timeout = 0;
				aml_dnl_unregister();
				return __LINE__;
			}
		}

#if (defined CONFIG_USB_DEVICE_V2) && !defined(CONFIG_USB_GADGET_CRG)
		if ((get_timer(curTime_sof) > 0x200) && (_sofintr)) {
			_sofintr = 0;
			dwc_otg_power_off_phy_fb();
		}
#endif// #if (defined CONFIG_USB_DEVICE_V2) && !defined(CONFIG_USB_GADGET_CRG)
		usb_gadget_handle_interrupts(0);
	}

	aml_dnl_unregister();
	return CMD_RET_SUCCESS;
}

#ifndef CONFIG_AML_NO_USB_MODULE
static int do_aml_DNL(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned timeout = (2 <= argc) ? simple_strtoul(argv[1], NULL, 0) : 0;
	unsigned pcToolWaitTime	= (3 <= argc) ? simple_strtoul(argv[2], NULL, 0) : 0;

	return aml_v3_usbburning(timeout, pcToolWaitTime);
}
#else
static int do_aml_DNL(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned timeout = (2 <= argc) ? simple_strtoul(argv[1], NULL, 0) : 0;

	optimus_erase_bootloader(timeout ? "usb-timeout" : "usb");//skip to bl1 usb rom driver

#ifdef CONFIG_AML_REBOOT
	run_command("reboot", 0);
	udelay(2*1000*1000);
#endif//#ifdef CONFIG_AML_REBOOT
	printf("call reset as reboot not work\n");//should not be reach here
	do_reset(NULL, 0, 0, NULL);//call reset if reboot undefined

	return CMD_RET_SUCCESS;
}
#endif// #ifndef CONFIG_AML_NO_USB_MODULE

U_BOOT_CMD(
        adnl,	3,	0,	do_aml_DNL,
        "use Amlogic DNL protocol",
        "    [timeout_enum] [timeout_identify]"
);

