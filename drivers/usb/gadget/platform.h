/* platform header */
/*
 * (C) Copyright 2010 Amlogic, Inc
 *
 * Victor Wan, victor.wan@amlogic.com,
 * 2010-03-24 @ Shanghai
 *
 */

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <asm/arch/register.h>

#define PREI_USB_PHY_A_POR      (1 << 0)
#define PREI_USB_PHY_B_POR      (1 << 1)
#define PREI_USB_PHY_CLK_SEL    (7 << 5)
#define PREI_USB_PHY_CLK_GATE 	(1 << 8)
#define PREI_USB_PHY_B_AHB_RSET     (1 << 11)
#define PREI_USB_PHY_B_CLK_RSET     (1 << 12)
#define PREI_USB_PHY_B_PLL_RSET     (1 << 13)
#define PREI_USB_PHY_A_AHB_RSET     (1 << 17)
#define PREI_USB_PHY_A_CLK_RSET     (1 << 18)
#define PREI_USB_PHY_A_PLL_RSET     (1 << 19)
#define PREI_USB_PHY_A_DRV_VBUS     (1 << 20)
#define PREI_USB_PHY_B_DRV_VBUS			(1 << 21)
#define PREI_USB_PHY_B_CLK_DETECT   (1 << 22)
#define PREI_USB_PHY_CLK_DIV        (0x7f << 24)
#define PREI_USB_PHY_A_CLK_DETECT   (1 << 31)

#define PREI_USB_PHY_A_REG3_IDDIG_OVR	(1 << 23)
#define PREI_USB_PHY_A_REG3_IDDIG_VAL	(1 << 24)

#define PREI_USB_PHY_B_REG4_IDDIG_OVR	(1 << 23)
#define PREI_USB_PHY_B_REG4_IDDIG_VAL	(1 << 24)

#define IREG_TIMER_E_COUNT            0x2655


#define flush_cpu_cache()

void dwc_write_reg32(unsigned int x, unsigned int v);
unsigned int dwc_read_reg32(unsigned int x);
void dwc_modify_reg32(unsigned int x, unsigned int c, unsigned int s);

#define get_unaligned(ptr)      (  ((unsigned long)ptr & 3) ? \
                                (((__u8 *)ptr)[0] | (((__u8 *)ptr)[1]<<8) | (((__u8 *)ptr)[2]<<16) | (((__u8 *)ptr)[3]<<24)) : \
                                (*(uint32_t*)ptr) )
#define get_unaligned_16(ptr)				(((__u8 *)ptr)[0] | (((__u8 *)ptr)[1]<<8))
#define get_unaligned_32(ptr)				(((__u8 *)ptr)[0] | (((__u8 *)ptr)[1]<<8) | (((__u8 *)ptr)[2]<<16) | (((__u8 *)ptr)[3]<<24))

#define EXT_CLOCK	0
#define INT_CLOCK	1

#define USB_ROM_CONN_TIMEOUT		5*1000*1000


/* Meet with spec */
#define USB_ROM_VER_MAJOR	0
#define USB_ROM_STAGE_MAJOR	0
#define USB_ROM_STAGE_MINOR	16

#ifdef CONFIG_M6
#define USB_ROM_VER_MINOR	8
#else
#define USB_ROM_VER_MINOR	7
#endif

#define PRINTF(x...)	do{}while(0)

#define ERR(x...)       printf(x)
#define DBG(x...)       PRINTF(x)

#define USB_ERR(x...)	printf("USBErr:%d", __LINE__),printf(x)
#define USB_DBG(x...)   PRINTF(x)

void set_usb_phy_config(int cfg);
void set_usb_phy21_tuning_update(void);
void set_usb_phy21_tuning_update_reset(void);
void close_usb_phy_clock(int cfg);

void usb_parameter_init(int timeout);
int chip_utimer_set(int val);
int chip_watchdog(void);
#define udelay __udelay
#define wait_ms(a) udelay(a*1000);
int update_utime(void);
int get_utime(void);

#endif
