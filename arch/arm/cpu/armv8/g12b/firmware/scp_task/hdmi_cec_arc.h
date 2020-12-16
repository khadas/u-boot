/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/g12b/firmware/scp_task/hdmi_cec_arc.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _HDMI_CEC_ARC_H
#define _HDMI_CEC_ARC_H

#ifndef CONFIG_CEC_OSD_NAME
#define CONFIG_CEC_OSD_NAME "AML_TV"
#endif

#define CEC_VERSION "g12b ceca ver:2020/08/20 mailbox\n"

/* cec irq bit flags for AO_CEC_B */
#define CECB_IRQ_TX_DONE		(1 << 0)
#define CECB_IRQ_RX_EOM			(1 << 1)
#define CECB_IRQ_TX_NACK		(1 << 2)
#define CECB_IRQ_TX_ARB_LOST		(1 << 3)
#define CECB_IRQ_TX_ERR_INITIATOR	(1 << 4)
#define CECB_IRQ_RX_ERR_FOLLOWER	(1 << 5)
#define CECB_IRQ_RX_WAKEUP		(1 << 6)
#define CECB_IRQ_EN_MASK		(0xf << 0)

/*CEC UI MASK*/
#define CEC_FUNC_MASK                        0
#define ONE_TOUCH_PLAY_MASK                  1
#define ONE_TOUCH_STANDBY_MASK               2
#define AUTO_POWER_ON_MASK                   3

/*source addr param to kernel define*/
#define LOGIC_ADDR_MASK			0xFFFF0000
#define PHY_ADDR_MASK			0x0000FFFF
#define get_logic_addr(a)		(((a) & LOGIC_ADDR_MASK) >> 16)
#define get_phy_addr(a)			(((a) & PHY_ADDR_MASK) >> 0)

/*wake up param to kernel define*/
#define WAKE_UP_PORT_ID_MASK	0xFFFF0000
#define WAKE_UP_REASON_MASK		0x0000FFFF
#define PHY_ADDR_LEN			4 /*16bit/4bit*/

#define CEC_CFG_FUNC_EN		0x01
#define CEC_CFG_DBG_EN		0x80000000

struct st_cec_mailbox_data {
	unsigned int cec_config;
	/*
	 * 0-15 : phy addr
	 * 16-20: logical address
	 * 21-23: device type
	 */
	unsigned int phy_addr;
	unsigned int vendor_id;
	unsigned char osd_name[16];
	unsigned int rev[10];
} __attribute__((packed));

extern unsigned char hdmi_cec_func_config;
void cec_node_init(void);
unsigned int cec_handler(void);
void cec_hw_reset(void);
/* void cec_give_device_power_status(void); */
void udelay(int i);
void check_standby(void);
/*int is_phy_addr_ready(cec_msg_t *msg);*/
void cec_save_port_id(void);
int cec_suspend_handle(void);
int cec_suspend_wakeup_chk(void);
void cec_start_config(void);
#endif  /* _HDMI_CEC_ARC_H */

