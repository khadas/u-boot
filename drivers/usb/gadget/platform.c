/* platform dirver header */
/*
 * (C) Copyright 2010 Amlogic, Inc
 *
 * Victor Wan, victor.wan@amlogic.com,
 * 2010-03-24 @ Shanghai
 *
 */
#include "platform.h"
#include <amlogic/cpu_id.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/usb.h>

void dwc_write_reg32(unsigned int x, unsigned int v)
{
	unsigned int addr;

	addr = usb_get_dwc_a_base_addr();
	(*(volatile uint32_t *)(unsigned long)(x + addr))=v;
}
unsigned int dwc_read_reg32(unsigned int x)
{
	unsigned int addr;

	addr = usb_get_dwc_a_base_addr();
	return (*(volatile uint32_t*)((unsigned long)(x + addr)));
}

void dwc_modify_reg32(unsigned int x, unsigned int c, unsigned int s)
{
	unsigned int addr;

	addr = usb_get_dwc_a_base_addr();
	(*(volatile uint32_t *)(unsigned long)(x + addr)) =
		( ((dwc_read_reg32(x)) & (~c)) | (s));
}

void set_usb_phy21_tuning_update(void)
{
	unsigned long phy_reg_base = usb_get_device_mode_phy_base();

	if (phy_reg_base == 0)
		return;
	usb2_phy_tuning(phy_reg_base, 1);
	return;
}

void set_usb_phy21_tuning_update_reset(void)
{
	usb_phy_tuning_reset();
}


void set_usb_phy_config(int cfg)
{
	usb_device_mode_init();
}

//sleep sometime before and after disconnect,
//      to let usb_burning_tool.exe detect both fast plug-out and plug-in
extern void dwc_otg_pullup(int is_on);
void close_usb_phy_clock(int cfg)
{
    cfg = cfg;//avoid compiler warning

    run_command("sleep 1", 0);//improve pc compatibility!!
    dwc_otg_pullup(0);//disconnect
    __udelay(20);
    /*dwc_otg_power_off_phy();*///Don't call this as it may cause pull-down failed!!!!
    run_command("sleep 1", 0);

    return;
}

