/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/tm2/firmware/scp_task/hdmi_cec_arc.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifdef CONFIG_CEC_WAKEUP
#include <asm/arch-tm2/cec_tx_reg.h>
#include <amlogic/aml_cec.h>
#include "hdmi_cec_arc.h"
#include "task_apis.h"
#include <linux/types.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#define CEC_DBG_PRINT
#define CEC_REG_DEBUG				0

cec_msg_t cec_msg;
unsigned char hdmi_cec_func_config;
cec_wakeup_t cec_wakup;
static unsigned int cec_wait_addr;

#ifdef CEC_DBG_PRINT
static void cec_dbg_print(char *s, int v)
{
	uart_puts(s);
	uart_put_hex(v,8);
	_udelay(100);
}
static void cec_dbg_prints(char *s)
{
	uart_puts(s);
	_udelay(100);
}
#if CEC_REG_DEBUG
static void cec_print_reg(char *s, int v, int l)
{
	uart_puts(s);
	uart_put_hex(v, l);
	uart_puts("\n");
	_udelay(1000);
}
#endif
#else
	#define cec_dbg_print(s,v)
	#define cec_dbg_prints(s)
	#define cec_print_reg(s, v, l)
#endif

static void cec_reset_addr(void);
struct cec_tx_msg_t {
	unsigned char buf[16];
	unsigned char retry;
	unsigned char len;
};

#define CEX_TX_MSG_BUF_NUM	  4
#define CEC_TX_MSG_BUF_MASK	 (CEX_TX_MSG_BUF_NUM - 1)

struct cec_tx_msg {
	struct cec_tx_msg_t msg[CEX_TX_MSG_BUF_NUM];
	unsigned char send_idx;
	unsigned char queue_idx;
};

struct cec_tx_msg cec_tx_msgs = {};

/* [0] for msg len */
static unsigned char cec_otp_msg[17];
static unsigned char cec_as_msg[17];

static int cec_strlen(char *p)
{
	int i=0;

	while (*p++)
		i++;
	return i;
}

static void *cec_memcpy(void *memto, const void *memfrom, unsigned int size)
{
	char *tempfrom = (char *)memfrom;
	char *tempto = (char *)memto;

	if ((memto == NULL) || (memfrom == NULL))
		return NULL;
	while (size -- > 0)
		*tempto++ = *tempfrom++;
	return memto;
}

static unsigned long cec_rd_reg(unsigned long addr)
{
	unsigned long data32;

	data32  = 0;
	data32 |= 0    << 16;  // [16]   cec_reg_wr
	data32 |= 0    << 8;   // [15:8] cec_reg_wrdata
	data32 |= addr << 0;   // [7:0]  cec_reg_addr
	writel(data32, AO_CECB_RW_REG);
	data32 = ((readl(AO_CECB_RW_REG)) >> 24) & 0xff;
	return (data32);
} /* cec_rd_reg */

static void cec_wr_reg (unsigned long addr, unsigned long data)
{
	unsigned long data32;

	data32  = 0;
	data32 |= 1    << 16;  // [16]   cec_reg_wr
	data32 |= data << 8;   // [15:8] cec_reg_wrdata
	data32 |= addr << 0;   // [7:0]  cec_reg_addr
	writel(data32, AO_CECB_RW_REG);
} /* aocec_wr_only_reg */

static inline void cec_set_bits_dwc(uint32_t reg, uint32_t bits,
				       uint32_t start, uint32_t len)
{
	unsigned int tmp;
	tmp = cec_rd_reg(reg);
	tmp &= ~(((1 << len) - 1) << start);
	tmp |=  (bits << start);
	cec_wr_reg(reg, tmp);
}

static void cec_set_reg_bits(unsigned long addr, unsigned int value,
	unsigned int offset, unsigned int len)
{
	unsigned int data32 = 0;

	data32 = readl(addr);
	data32 &= ~(((1 << len) - 1) << offset);
	data32 |= (value & ((1 << len) - 1)) << offset;
	writel(data32, addr);
}

static void cec_rx_read_pos_plus(void)
{
	(cec_msg.rx_read_pos ==  cec_msg.rx_buf_size - 1) ?
				(cec_msg.rx_read_pos = 0) :
				(cec_msg.rx_read_pos++);
}

#if CEC_REG_DEBUG
static void dump_cecb_reg(void)
{
	int i = 0;
	unsigned char reg;
	unsigned int reg32;

	reg32 = readl(AO_CECB_CLK_CNTL_REG0);
	cec_print_reg("CLK_CNTL0:", reg32, 32);
	reg32 = readl(AO_CECB_CLK_CNTL_REG1);
	cec_print_reg("CLK_CNTL1:", reg32, 32);
	reg32 = readl(AO_CECB_GEN_CNTL);
	cec_print_reg("GEN_CNTL:", reg32, 32);
	reg32 = readl(AO_CECB_RW_REG);
	cec_print_reg("RW:", reg32, 32);
	reg32 = readl(AO_CECB_INTR_MASKN);
	cec_print_reg("INT_MASKN:", reg32, 32);
	reg32 = readl(AO_CECB_INTR_STAT);
	cec_print_reg("INT_STAT:", reg32, 32);

	cec_print_reg("CEC_CTRL:", cec_rd_reg(DWC_CECB_CTRL), 8);
	cec_print_reg("CEC_MASK:", cec_rd_reg(DWC_CECB_INTR_MASK), 8);
	cec_print_reg("CEC_ADDR_L:", cec_rd_reg(DWC_CECB_LADD_LOW), 8);
	cec_print_reg("CEC_ADDR_H:", cec_rd_reg(DWC_CECB_LADD_HIGH), 8);
	cec_print_reg("CEC_TX_CNT:", cec_rd_reg(DWC_CECB_TX_CNT), 8);
	cec_print_reg("CEC_RX_CNT:", cec_rd_reg(DWC_CECB_RX_CNT), 8);
	cec_print_reg("CEC_LOCK:", cec_rd_reg(DWC_CECB_LOCK_BUF), 8);
	cec_print_reg("CEC_WKUPCTRL:", cec_rd_reg(DWC_CECB_WAKEUPCTRL), 8);

	cec_dbg_prints("RX buffer:");
	for (i = 0; i < 16; i++) {
		reg = cec_rd_reg(DWC_CECB_RX_DATA00 + i);
		cec_dbg_print(" ", reg);
	}
	cec_dbg_prints("\n");

	cec_dbg_prints("TX buffer:");
	for (i = 0; i < 16; i++) {
		reg = cec_rd_reg(DWC_CECB_TX_DATA00 + i);
		cec_dbg_print(" ", reg);
	}
	cec_dbg_prints("\n");
}
#else
static inline void dump_cecb_reg(void) {}
#endif

void cec_hw_reset(void)
{
	unsigned int reg;
	unsigned int data32;
	cec_dbg_prints("cec reset\n");

	reg =   (0 << 31) |
		(0 << 30) |
		(1 << 28) |		/* clk_div0/clk_div1 in turn */
		((732-1) << 12) |	/* Div_tcnt1 */
		((733-1) << 0);		/* Div_tcnt0 */
	writel(reg, AO_CECB_CLK_CNTL_REG0);
	reg =   (0 << 13) |
		((11-1)  << 12) |
		((8-1)  <<  0);
	writel(reg, AO_CECB_CLK_CNTL_REG1);

	reg = readl(AO_CECB_CLK_CNTL_REG0);
	reg |= (1 << 31);
	writel(reg, AO_CECB_CLK_CNTL_REG0);

	_udelay(200);
	reg |= (1 << 30);
	writel(reg, AO_CECB_CLK_CNTL_REG0);

	reg = readl(AO_RTI_PWR_CNTL_REG0);
	reg |=  (0x01 << 14);	/* xtal gate */
	writel(reg, AO_RTI_PWR_CNTL_REG0);

	data32  = 0;
	data32 |= (7 << 12);	/* filter_del */
	data32 |= (1 <<  8);	/* filter_tick: 1us */
	data32 |= (1 <<  3);	/* enable system clock */
	data32 |= 0 << 1;	/* [2:1]	cntl_clk: */
				/* 0=Disable clk (Power-off mode); */
				/* 1=Enable gated clock (Normal mode); */
				/* 2=Enable free-run clk (Debug mode). */
	data32 |= 1 << 0;	/* [0]	  sw_reset: 1=Reset */
	writel(data32, AO_CECB_GEN_CNTL);
	/* Enable gated clock (Normal mode). */
	cec_set_reg_bits(AO_CECB_GEN_CNTL, 1, 1, 1);
	/* Release SW reset */
	cec_set_reg_bits(AO_CECB_GEN_CNTL, 0, 0, 1);
	/* set up pinmux */
	writel(((readl(AO_RTI_PINMUX_REG1) & (~(0xF << 8))) | (2 << 8)),
		AO_RTI_PINMUX_REG1);
	/*enable the interrupt*/
	writel(CECB_IRQ_EN_MASK, AO_CECB_INTR_MASKN);
	cec_wr_reg(DWC_CECB_WAKEUPCTRL, 0);
	cec_dbg_print("Set cec pinmux:0x", (readl(AO_RTI_PINMUX_REG1) >> 8));
	cec_dbg_prints("\n");
}

static unsigned char remote_cec_ll_rx(void)
{
	int i;
	int len;

	len = cec_rd_reg(DWC_CECB_RX_CNT);
	cec_dbg_prints("cec R:");
	for (i = 0; len <= 16 && i < len; i++) {
		cec_msg.buf[cec_msg.rx_write_pos].msg[i] = cec_rd_reg(DWC_CECB_RX_DATA00 + i);
		cec_dbg_print(" ", cec_msg.buf[cec_msg.rx_write_pos].msg[i]);
	}
	/* clr CEC lock bit */
	cec_wr_reg(DWC_CECB_LOCK_BUF, 0);
	cec_msg.buf[cec_msg.rx_write_pos].msg_len = len;
	cec_dbg_prints("\n");

	return 0;
}
static void cec_buf_clear(void)
{
	int i;

	for (i = 0; i < 16; i++)
		cec_msg.buf[cec_msg.rx_read_pos].msg[i] = 0;
}

static void cec_tx_buf_init(void)
{
	int i, j;
	for (j = 0; j < CEX_TX_MSG_BUF_NUM; j++) {
		for (i = 0; i < 16; i++) {
			cec_tx_msgs.msg[j].buf[i] = 0;
		}
		cec_tx_msgs.msg[j].retry = 0;
		cec_tx_msgs.msg[j].len = 0;
	}
}

static int cec_queue_tx_msg(unsigned char *msg, unsigned char len)
{
	int s_idx, q_idx;

	s_idx = cec_tx_msgs.send_idx;
	q_idx = cec_tx_msgs.queue_idx;
	if (((q_idx + 1) & CEC_TX_MSG_BUF_MASK) == s_idx) {
		cec_dbg_prints("tx buffer full, abort msg\n");
		cec_reset_addr();
		return -1;
	}
	if (len && msg) {
		cec_memcpy(cec_tx_msgs.msg[q_idx].buf, msg, len);
		cec_tx_msgs.msg[q_idx].len = len;
		cec_tx_msgs.queue_idx = (q_idx + 1) & CEC_TX_MSG_BUF_MASK;
	}
	return 0;
}

static int cec_triggle_tx(unsigned char *msg, unsigned char len)
{
	int i = 0, lock;

	while (1) {
		/* send is in process */
		lock = cec_rd_reg(DWC_CECB_LOCK_BUF);
		if (lock) {
			cec_dbg_prints("rx msg in tx\n");
			return -1;
		}
		if (cec_rd_reg(DWC_CECB_CTRL) & 0x01)
			i++;
		else
			break;
		if (i > 25) {
			cec_dbg_prints("wait busy timeout\n");
			return -1;
		}
	}

	cec_dbg_prints("cec T:");
	for (i = 0; i < len; i++) {
		cec_wr_reg(DWC_CECB_TX_DATA00 + i, msg[i]);
		cec_dbg_print(" ", msg[i]);
	}
	cec_dbg_prints("\n");

	/* start send */
	cec_wr_reg(DWC_CECB_TX_CNT, len);
	cec_set_bits_dwc(DWC_CECB_CTRL, 3, 0, 3);
	return 0;
}

static int remote_cec_ll_tx(unsigned char *msg, unsigned char len)
{
	cec_queue_tx_msg(msg, len);
	cec_triggle_tx(msg, len);

	return 0;
}

static int ping_cec_ll_tx(unsigned char *msg, unsigned char len)
{
	unsigned int reg, ret = 0;
	unsigned int cnt = 0;

	remote_cec_ll_tx(msg, len);

	while (cec_tx_msgs.queue_idx != cec_tx_msgs.send_idx) {
		reg = readl(AO_CECB_INTR_STAT);
		writel(reg, AO_CECB_INTR_CLR);
		if (reg & CECB_IRQ_TX_DONE) {
			ret = TX_DONE;
			cec_tx_msgs.send_idx = (cec_tx_msgs.send_idx + 1) & CEC_TX_MSG_BUF_MASK;
			cec_dbg_prints("ping_cec_tx:TX_DONE\n");
			break;
		}

		if (reg & CECB_IRQ_TX_NACK) {
			ret = TX_ERROR;
			cec_tx_msgs.send_idx = (cec_tx_msgs.send_idx + 1) & CEC_TX_MSG_BUF_MASK;
			cec_dbg_prints("ping_cec_tx:TX_ERROR\n");
			break;
		}
		if (reg & CECB_IRQ_TX_ARB_LOST) {
			ret = TX_BUSY;
			cec_tx_msgs.send_idx = (cec_tx_msgs.send_idx + 1) & CEC_TX_MSG_BUF_MASK;
			cec_dbg_prints("ping_cec_tx:TX_ABT_LOST\n");
			break;
		}
		if (reg & CECB_IRQ_TX_ERR_INITIATOR) {
			ret = TX_BUSY;
			cec_tx_msgs.send_idx = (cec_tx_msgs.send_idx + 1) & CEC_TX_MSG_BUF_MASK;
			cec_dbg_prints("ping_cec_tx:TX_ERR_INIT\n");
			break;
		}
		_udelay(500);
		if (cnt++ > 2000) {
			uart_puts("err: tx not finish flag\n");
			break;
		}
	}

	return ret;
}

#define DEVICE_TV		0
#define DEVICE_RECORDER		1
#define DEVICE_RESERVED		2
#define DEVICE_TUNER		3
#define DEVICE_PLAYBACK		4
#define DEVICE_AUDIO_SYSTEM	5
#define DEVICE_PURE_CEC_SWITCH	6
#define DEVICE_VIDEO_PROCESSOR	7

static unsigned char log_addr_to_devtye(unsigned int addr)
{
	static unsigned char addr_map[] = {
		DEVICE_TV,
		DEVICE_RECORDER,
		DEVICE_RECORDER,
		DEVICE_TUNER,
		DEVICE_PLAYBACK,
		DEVICE_AUDIO_SYSTEM,
		DEVICE_TUNER,
		DEVICE_TUNER,
		DEVICE_PLAYBACK,
		DEVICE_RECORDER,
		DEVICE_TUNER,
		DEVICE_PLAYBACK,
		DEVICE_RESERVED,
		DEVICE_RESERVED,
		DEVICE_TV
	};
	return addr_map[addr & 0xf];
}

/* if multi-addr is configured in kernel(4 + 5 for soundbar),
 * then use CEC_AUDIO_SYSTEM_ADDR as default logic addr
 * cec_msg.log_addr is configured as 5
 */
static void cec_report_physical_address(unsigned char source)
{
	unsigned char msg[5];
	unsigned char phy_addr_ab = (readl(AO_DEBUG_REG1) >> 8) & 0xff;
	unsigned char phy_addr_cd = readl(AO_DEBUG_REG1) & 0xff;

	msg[0] = ((source & 0xf) << 4) | CEC_BROADCAST_ADDR;
	msg[1] = CEC_OC_REPORT_PHYSICAL_ADDRESS;
	msg[2] = phy_addr_ab;
	msg[3] = phy_addr_cd;
	msg[4] = log_addr_to_devtye(cec_msg.log_addr);

	remote_cec_ll_tx(msg, 5);
}

static void cec_report_device_power_status(unsigned char source, unsigned char dst)
{
	unsigned char msg[3];

	msg[0] = ((source & 0xf) << 4) | (dst & 0xf);
	msg[1] = CEC_OC_REPORT_POWER_STATUS;
	msg[2] = cec_msg.power_status;

	remote_cec_ll_tx(msg, 3);
}

static void cec_set_stream_path(void)
{
	unsigned char phy_addr_ab = (readl(AO_DEBUG_REG1) >> 8) & 0xff;
	unsigned char phy_addr_cd = readl(AO_DEBUG_REG1) & 0xff;
	unsigned char msg_len = cec_msg.buf[cec_msg.rx_read_pos].msg_len;

	if ((hdmi_cec_func_config >> CEC_FUNC_MASK) & 0x1) {
		if ((hdmi_cec_func_config >> AUTO_POWER_ON_MASK) & 0x1) {
			if ((phy_addr_ab == cec_msg.buf[cec_msg.rx_read_pos].msg[2]) &&
			    (phy_addr_cd == cec_msg.buf[cec_msg.rx_read_pos].msg[3]))  {
				cec_msg.cec_power = 0x1;
				memset(cec_otp_msg, 0, sizeof(cec_otp_msg));
				cec_otp_msg[0] = msg_len;
				memcpy(&cec_otp_msg[1],
				       cec_msg.buf[cec_msg.rx_read_pos].msg,
				       cec_otp_msg[0]);
				set_cec_wk_msg(CEC_OC_IMAGE_VIEW_ON,
					       cec_otp_msg);
			}
		}
	}
}

void cec_routing_change(void)
{
	unsigned char phy_addr_ab = (readl(P_AO_DEBUG_REG1) >> 8) & 0xff;
	unsigned char phy_addr_cd = readl(P_AO_DEBUG_REG1) & 0xff;
	unsigned char msg_len = cec_msg.buf[cec_msg.rx_read_pos].msg_len;

	if ((hdmi_cec_func_config >> CEC_FUNC_MASK) & 0x1) {
		if ((hdmi_cec_func_config >> AUTO_POWER_ON_MASK) & 0x1) {
			/* wake up if routing destination is self */
			if (phy_addr_ab ==
			    cec_msg.buf[cec_msg.rx_read_pos].msg[4] &&
			    phy_addr_cd ==
			    cec_msg.buf[cec_msg.rx_read_pos].msg[5]) {
				cec_msg.cec_power = 0x1;
				memset(cec_otp_msg, 0, sizeof(cec_otp_msg));
				cec_otp_msg[0] = msg_len;
				memcpy(&cec_otp_msg[1],
				       cec_msg.buf[cec_msg.rx_read_pos].msg,
				       cec_otp_msg[0]);
				set_cec_wk_msg(CEC_OC_IMAGE_VIEW_ON,
					       cec_otp_msg);
			}
		}
	}
}

static void cec_device_vendor_id(unsigned char source)
{
	unsigned char msg[5];

	msg[0] = ((source & 0xf) << 4) | CEC_BROADCAST_ADDR;
	msg[1] = CEC_OC_DEVICE_VENDOR_ID;
	msg[2] = 0x00;
	msg[3] = 0x00;
	msg[4] = 0x00;

	remote_cec_ll_tx(msg, 5);
}

static void cec_menu_status_smp(int menu_status, unsigned char source, unsigned char dst)
{
	unsigned char msg[3];

	msg[0] = ((source & 0xf) << 4) | (dst & 0xf);
	msg[1] = CEC_OC_MENU_STATUS;
	msg[2] = menu_status;

	remote_cec_ll_tx(msg, 3);
}

static void cec_give_deck_status(unsigned char source, unsigned char dst)
{
	unsigned char msg[3];

	msg[0] = ((source & 0xf) << 4) | (dst & 0xf);
	msg[1] = CEC_OC_DECK_STATUS;
	msg[2] = 0x1a;

	remote_cec_ll_tx(msg, 3);
}

/*static void cec_standby(void)
{
	unsigned char msg[2];

	msg[0] = ((cec_msg.log_addr & 0xf) << 4) | CEC_BROADCAST_ADDR;
	msg[1] = CEC_OC_STANDBY;

	remote_cec_ll_tx(msg, 2);
}*/

static void cec_set_osd_name(unsigned char source, unsigned char dst)
{
	unsigned char msg[16];
	unsigned char osd_len = cec_strlen(CONFIG_CEC_OSD_NAME);

	msg[0] = ((source & 0xf) << 4) | (dst & 0xf);
	msg[1] = CEC_OC_SET_OSD_NAME;
	cec_memcpy(&msg[2], CONFIG_CEC_OSD_NAME, osd_len);

	remote_cec_ll_tx(msg, osd_len + 2);
}

static void cec_get_version(unsigned char source, unsigned char dst)
{
	unsigned char dest_log_addr = cec_msg.log_addr & 0xf;
	unsigned char msg[3];

	if (0xf != dest_log_addr) {
		msg[0] = ((source & 0xf) << 4) | (dst & 0xf);
		msg[1] = CEC_OC_CEC_VERSION;
		msg[2] = CEC_VERSION_14A;
		remote_cec_ll_tx(msg, 3);
	}
}

static int check_addr(int phy_addr)
{
	unsigned int local_addr = (readl(AO_DEBUG_REG1)) & 0xffff;
	unsigned int i, mask = 0xf000, a, b;

	for (i = 0; i < 4; i++) {
		if (!(local_addr & mask)) {
			break;
		}
		a = local_addr & mask;
		b = phy_addr & mask;
		if (a != b)	{// node is not same
			cec_dbg_prints("addr fail 1\n");
			return 0;
		}
		mask >>= 4;
	}
	cec_dbg_prints("addr ok\n");
	return 1;
}

//static bool is_playback_dev(int addr)
//{
//	if (addr != CEC_PLAYBACK_DEVICE_1_ADDR &&
//	    addr != CEC_PLAYBACK_DEVICE_2_ADDR &&
//	    addr != CEC_PLAYBACK_DEVICE_3_ADDR) {
//		return false;
//	}
//	return true;
//}

static bool is_tv_dev(int addr)
{
	if ((addr & 0xf) == CEC_TV_ADDR)
		return true;

	return false;
}

static bool is_audio_system_dev(int addr)
{
	if ((addr & 0xf) == CEC_AUDIO_SYSTEM_ADDR)
		return true;

	return false;
}

int is_phy_addr_ready(cec_msg_t *msg)
{
	/*unsigned int source_addr = readl(AO_RTI_STATUS_REG1);*/

	if (msg == NULL)
		return 0;
	if (((hdmi_cec_func_config >> CEC_FUNC_MASK) & 0x1) &&
	    ((hdmi_cec_func_config >> AUTO_POWER_ON_MASK) & 0x1) &&
	    msg->cec_power) {
		if (cec_wakup.wk_phy_addr &&
		    cec_wakup.wk_logic_addr &&
			(cec_wakup.wk_phy_addr != 0xFFFF)) {
				uart_puts("is_phy_addr_ready 0x");
				uart_put_hex(cec_wakup.wk_phy_addr, 16);
				uart_puts("\n");
				return 1;
		}
	}

	return 0;
}

void cec_save_port_id(void)
{
	unsigned int phy_addr;
	int i, port_id;

	phy_addr = cec_wakup.wk_phy_addr;
	if ((phy_addr == 0xFFFF) || ((phy_addr & 0xF000) == 0)) {
		cec_wakup.wk_port_id = 0xFF;
		set_cec_val1(*((unsigned int *)&cec_wakup));
		return;
	}

	for (i = 0; i < PHY_ADDR_LEN; i++) {
		port_id = (phy_addr >> (PHY_ADDR_LEN - i - 1)*4) & 0xF;
		if (port_id == 0) {
			 port_id = (phy_addr >> (PHY_ADDR_LEN - i)*4) & 0xF;
			 break;
		}
	}
	cec_wakup.wk_port_id = port_id;
	set_cec_val1(*((unsigned int *)&cec_wakup));
}

static unsigned int cec_handle_message(void)
{
	unsigned char opcode;
	unsigned char source;
	unsigned char dest;
	unsigned int  phy_addr/*, wake*/;
	bool cec_func_on;
	bool cec_auto_wake_en;
	unsigned char msg_len = cec_msg.buf[cec_msg.rx_read_pos].msg_len;
	unsigned char *msg;

	source = (cec_msg.buf[cec_msg.rx_read_pos].msg[0] >> 4) & 0xf;
	dest = cec_msg.buf[cec_msg.rx_read_pos].msg[0] & 0xf;
	cec_func_on = ((hdmi_cec_func_config >> CEC_FUNC_MASK) & 0x1) == 0x1;
	cec_auto_wake_en =
		((hdmi_cec_func_config >> AUTO_POWER_ON_MASK) & 0x1) == 0x1;
	if (cec_func_on &&
	    cec_msg.buf[cec_msg.rx_read_pos].msg_len > 1) {
		opcode = cec_msg.buf[cec_msg.rx_read_pos].msg[1];
		switch (opcode) {
		case CEC_OC_GET_CEC_VERSION:
			cec_get_version(dest, source);
			break;
		case CEC_OC_GIVE_DECK_STATUS:
			cec_give_deck_status(dest, source);
			break;
		case CEC_OC_GIVE_PHYSICAL_ADDRESS:
			cec_report_physical_address(dest);
			break;
		case CEC_OC_GIVE_DEVICE_VENDOR_ID:
			cec_device_vendor_id(dest);
			break;
		case CEC_OC_GIVE_OSD_NAME:
			cec_set_osd_name(dest, source);
			break;
		case CEC_OC_SET_STREAM_PATH:
			cec_set_stream_path();
			break;
		case CEC_OC_ROUTING_CHANGE:
			cec_routing_change();
			break;
		case CEC_OC_GIVE_DEVICE_POWER_STATUS:
			cec_report_device_power_status(dest, source);
			break;
		case CEC_OC_USER_CONTROL_PRESSED:
			if (cec_func_on &&
			    cec_auto_wake_en &&
			    cec_msg.buf[cec_msg.rx_read_pos].msg_len == 3 &&
			    ((0x40 == cec_msg.buf[cec_msg.rx_read_pos].msg[2]) ||
			     (0x6d == cec_msg.buf[cec_msg.rx_read_pos].msg[2]) ||
			     (0x09 ==
			     cec_msg.buf[cec_msg.rx_read_pos].msg[2]))) {
				cec_msg.cec_power = 0x1;
				memset(cec_otp_msg, 0, sizeof(cec_otp_msg));
				cec_otp_msg[0] = msg_len;
				memcpy(&cec_otp_msg[1],
				       cec_msg.buf[cec_msg.rx_read_pos].msg,
				       cec_otp_msg[0]);
				set_cec_wk_msg(CEC_OC_IMAGE_VIEW_ON,
					       cec_otp_msg);
			}
			break;
		case CEC_OC_MENU_REQUEST:
			cec_menu_status_smp(DEVICE_MENU_INACTIVE, dest, source);
			break;

		/* TV Wake up by image/text view on */
		case CEC_OC_IMAGE_VIEW_ON:
		case CEC_OC_TEXT_VIEW_ON:
			if (cec_func_on &&
			    cec_auto_wake_en &&
			    (is_tv_dev(cec_msg.log_addr))) {
				/* request active source needed */
				phy_addr = 0xffff;
				cec_msg.cec_power = 0x1;
				/*wake =  (phy_addr << 0) |*/
				/*	(source << 16);*/
				/*writel(wake, AO_RTI_STATUS_REG1);*/
				cec_wakup.wk_logic_addr = source;
				cec_wakup.wk_phy_addr = phy_addr;
				set_cec_val1(*((unsigned int *)&cec_wakup));
				memset(cec_otp_msg, 0, sizeof(cec_otp_msg));
				cec_otp_msg[0] = msg_len;
				memcpy(&cec_otp_msg[1],
				       cec_msg.buf[cec_msg.rx_read_pos].msg,
				       cec_otp_msg[0]);
				set_cec_wk_msg(CEC_OC_IMAGE_VIEW_ON,
					       cec_otp_msg);
			}
			break;

		/* TV Wake up by active source*/
		case CEC_OC_ACTIVE_SOURCE:
			phy_addr =
				(cec_msg.buf[cec_msg.rx_read_pos].msg[2] << 8) +
				(cec_msg.buf[cec_msg.rx_read_pos].msg[3] << 0);
			if (cec_func_on &&
			    cec_auto_wake_en &&
			    (is_tv_dev(cec_msg.log_addr) && check_addr(phy_addr))) {
				cec_msg.cec_power = 0x1;
				cec_msg.active_source = 1;
				/*wake =  (phy_addr << 0) |*/
				/*	(source << 16);*/
				uart_puts("source : 0x");
				uart_put_hex(source, 8);
				uart_puts("\n");
				/*writel(wake, AO_RTI_STATUS_REG1);*/
				cec_wakup.wk_logic_addr = source;
				cec_wakup.wk_phy_addr = phy_addr;
				set_cec_val1(*((unsigned int *)&cec_wakup));
				memset(cec_as_msg, 0, sizeof(cec_as_msg));
				cec_as_msg[0] = msg_len;
				msg = cec_msg.buf[cec_msg.rx_read_pos].msg;
				if (cec_as_msg[0] <= MAX_MSG)
					memcpy(&cec_as_msg[1], msg,
					       cec_as_msg[0]);
				set_cec_wk_msg(CEC_OC_ACTIVE_SOURCE,
					       cec_as_msg);
			}
			break;
		case CEC_OC_REPORT_PHYSICAL_ADDRESS:
			phy_addr = (cec_msg.buf[cec_msg.rx_read_pos].msg[2] << 8) |
				   (cec_msg.buf[cec_msg.rx_read_pos].msg[3] << 0);
			cec_wakup.wk_logic_addr = source;
			cec_wakup.wk_phy_addr = phy_addr;
			set_cec_val1(*((unsigned int *)&cec_wakup));
			break;
		case CEC_OC_SYSTEM_AUDIO_MODE_REQUEST:
			/* soundbar will wakeup when receive this msg */
			if (cec_func_on &&
			    cec_auto_wake_en &&
			    cec_msg.log_addr == CEC_AUDIO_SYSTEM_ADDR) {
				cec_msg.cec_power = 0x1;
				memset(cec_otp_msg, 0, sizeof(cec_otp_msg));
				cec_otp_msg[0] = msg_len;
				if (cec_otp_msg[0] <= MAX_MSG)
					memcpy(&cec_otp_msg[1],
						cec_msg.buf[cec_msg.rx_read_pos].msg,
						cec_otp_msg[0]);
				set_cec_wk_msg(CEC_OC_IMAGE_VIEW_ON, cec_otp_msg);
				uart_puts("system audio mode request wakeup\n");
			}
			break;
		default:
			break;
		}
	}
	cec_rx_read_pos_plus();
	return 0;
}

/* CEC B */
static void cecb_addr_add(unsigned int l_add)
{
	unsigned int addr;

	if (l_add > 15)
		return;

	if (l_add < 8) {
		addr = cec_rd_reg(DWC_CECB_LADD_LOW);
		addr |= (1 << l_add);
		cec_wr_reg(DWC_CECB_LADD_LOW, addr);
	} else {
		addr = cec_rd_reg(DWC_CECB_LADD_HIGH);
		addr |= (1 << (l_add - 8));
		cec_wr_reg(DWC_CECB_LADD_HIGH, addr);
	}
	uart_puts("cec b add addr: 0x");
	uart_put_hex(l_add, 8);
	uart_puts("\n");
}

void cec_clear_all_logical_addr(void)
{
	uart_puts("clear all logical addr\n");

	cec_wr_reg(DWC_CECB_LADD_LOW, 0);
	cec_wr_reg(DWC_CECB_LADD_HIGH, 0);
	/*udelay(100);*/
}

void cec_restore_logical_addr(unsigned int addr_en)
{
	unsigned int i;
	unsigned int addr_enable = addr_en;

	cec_clear_all_logical_addr();
	for (i = 0; i < 15; i++) {
		if (addr_enable & 0x1)
			cecb_addr_add(i);

		addr_enable = addr_enable >> 1;
	}
	uart_puts("cec restore_logical_addr: 0x");
	uart_put_hex(addr_en, 16);
	uart_puts("\n");
	_udelay(100);
}

static void cec_set_log_addr(int addr)
{
	cecb_addr_add(addr);
	cec_msg.addr_enable |= (1 << addr);
	_udelay(100);
}

static void cec_reset_addr(void)
{
	cec_hw_reset();
	cec_restore_logical_addr(cec_msg.addr_enable);
}

static int cec_get_log_addr(void)
{
	int reg;

	reg = cec_rd_reg(DWC_CECB_LADD_LOW);
	reg = (cec_rd_reg(DWC_CECB_LADD_HIGH) << 8) | reg;

	return reg & 0xffff;
}

unsigned int cec_handler(void)
{
	unsigned char s_idx;
	static int busy_count = 0;
	int irq;

	/*dump_cecb_reg();*/
	irq = readl(AO_CECB_INTR_STAT);
	writel(irq, AO_CECB_INTR_CLR);
	if (irq & CECB_IRQ_RX_EOM) {
		remote_cec_ll_rx();
		(cec_msg.rx_write_pos == cec_msg.rx_buf_size - 1) ? (cec_msg.rx_write_pos = 0) : (cec_msg.rx_write_pos++);
		cec_dbg_prints("RX_OK\n");
	}
	if (irq & CECB_IRQ_RX_ERR_FOLLOWER) {
		cec_dbg_prints("RX_ERROR\n");
		cec_wr_reg(DWC_CECB_LOCK_BUF, 0);
	}
	if (irq & CECB_IRQ_RX_WAKEUP) {
		cec_dbg_prints("rx wake up\n");
		cec_wr_reg(DWC_CECB_WAKEUPCTRL, 0);
		/* TODO: wake up system if needed */
	}

	if (irq & CECB_IRQ_TX_DONE) {
		cec_tx_msgs.send_idx = (cec_tx_msgs.send_idx + 1) & CEC_TX_MSG_BUF_MASK;
		s_idx = cec_tx_msgs.send_idx;
		if (cec_tx_msgs.send_idx != cec_tx_msgs.queue_idx) {
			cec_dbg_prints("TX_OK\n");
			cec_triggle_tx(cec_tx_msgs.msg[s_idx].buf,
				       cec_tx_msgs.msg[s_idx].len);
		} else {
			cec_dbg_prints("TX_END\n");
		}
		busy_count = 0;
	}

	if (irq & CECB_IRQ_TX_NACK) {
		cec_dbg_prints("@TX_NACK\n");
		s_idx = cec_tx_msgs.send_idx;
		if (cec_tx_msgs.msg[s_idx].retry < 5) {
			cec_tx_msgs.msg[s_idx].retry++;
			cec_triggle_tx(cec_tx_msgs.msg[s_idx].buf,
				       cec_tx_msgs.msg[s_idx].len);
		} else {
			cec_dbg_prints("TX retry too much, abort msg\n");
			cec_tx_msgs.send_idx = (cec_tx_msgs.send_idx + 1) & CEC_TX_MSG_BUF_MASK;
		}
		busy_count = 0;
	}

	if (irq & CECB_IRQ_TX_ERR_INITIATOR) {
		cec_dbg_prints("@TX_ERR_INIT\n");
		s_idx = cec_tx_msgs.send_idx;
		if (cec_tx_msgs.send_idx != cec_tx_msgs.queue_idx) { // triggle tx if idle
			cec_triggle_tx(cec_tx_msgs.msg[s_idx].buf,
				       cec_tx_msgs.msg[s_idx].len);
		}
		busy_count = 0;
	}

	if (irq & CECB_IRQ_TX_ARB_LOST) {
	    busy_count++;
		if (busy_count >= 2000) {
			uart_puts("busy too long, reset hw\n");
			cec_reset_addr();
			busy_count = 0;
		}
	}
	if (cec_msg.rx_read_pos != cec_msg.rx_write_pos) {
		cec_handle_message();
		dump_cecb_reg();
	}

	return 0;
}

/*static void check_standby(void)
{
	if (((cec_msg.log_addr & 0xf) == 0) &&
	    ((hdmi_cec_func_config >> CEC_FUNC_MASK) & 0x1) &&
	    ((hdmi_cec_func_config >> ONE_TOUCH_STANDBY_MASK) & 0x1)) {
		cec_standby();
	}
}*/

void cec_node_init(void)
{
	static int i = 0;
	static unsigned int retry = 0;
	static int regist_devs = 0;
	static int *probe = NULL;

	int tx_stat;
	unsigned char msg[1];
	unsigned int kern_log_addr = (readl(AO_DEBUG_REG1) >> 16) & 0xf;
	unsigned int kern_log_addr2 = 0;
	unsigned int start_poll_log_addr = CEC_PLAYBACK_DEVICE_1_ADDR;
	int player_dev[3][3] =
		{{CEC_PLAYBACK_DEVICE_1_ADDR, CEC_PLAYBACK_DEVICE_2_ADDR, CEC_PLAYBACK_DEVICE_3_ADDR},
		 {CEC_PLAYBACK_DEVICE_2_ADDR, CEC_PLAYBACK_DEVICE_3_ADDR, CEC_PLAYBACK_DEVICE_1_ADDR},
		 {CEC_PLAYBACK_DEVICE_3_ADDR, CEC_PLAYBACK_DEVICE_1_ADDR, CEC_PLAYBACK_DEVICE_2_ADDR}};

	if (kern_log_addr != 0)
		kern_log_addr2 = (readl(AO_DEBUG_REG1) >> 24) & 0xf;
	cec_wait_addr = 0;
	if (retry >= 12) {  // retry all device addr
		cec_msg.log_addr = 0x0f;
		uart_puts("failed on retried all possible address\n");
		return ;
	}
	writel(0, AO_RTI_STATUS_REG1);
	memset(&cec_wakup, 0, sizeof(cec_wakup));
	memset(cec_otp_msg, 0, sizeof(cec_otp_msg));
	memset(cec_as_msg, 0, sizeof(cec_as_msg));
	if (probe == NULL) {
		cec_msg.rx_read_pos = 0;
		cec_msg.rx_write_pos = 0;
		cec_msg.rx_buf_size = 2;

		cec_msg.power_status = 1;
		cec_msg.active_source = 0;
		cec_msg.cec_power = 0;
		cec_tx_msgs.send_idx = 0;
		cec_tx_msgs.queue_idx = 0;
		cec_msg.addr_enable = 0;
		cec_clear_all_logical_addr();
		cec_tx_buf_init();
		cec_buf_clear();
		_udelay(100);
		/*
		 * use kernel cec logic address to detect which logic address is the
		 * started one to allocate.
		 */
		cec_dbg_print("kern log_addr:0x", kern_log_addr);
		cec_dbg_print(" kern log_addr2:0x", kern_log_addr2);
		uart_puts("\n");
		/* we don't need probe TV address nor audio system.
		 * if audio system logic addr is configured in kernel,
		 * then use CEC_AUDIO_SYSTEM_ADDR as default logic addr
		 * witch is used to respond to directly addressed message
		 * cec_report_physical_address()
		 */
		if (is_tv_dev(kern_log_addr)) {
			cec_set_log_addr(kern_log_addr);
			msg[0] = (kern_log_addr << 4) | kern_log_addr;
			ping_cec_ll_tx(msg, 1);
			cec_msg.log_addr = 0x10 | kern_log_addr;
			_udelay(100);
			cec_dbg_print("Set cec log_addr:0x", cec_msg.log_addr);
			cec_dbg_print(",ADDR0:", cec_get_log_addr());
			uart_puts("\n");
			probe = NULL;
			regist_devs = 0;
			i = 0;
			retry = 0;
			/*check_standby();*/
			return ;
		} else if (is_audio_system_dev(kern_log_addr)) {
			/* set logic addr for audio system directly
			 * mostly, there's only one avr in system,
			 * just for simple check
			 */
			cec_set_log_addr(kern_log_addr);
			msg[0] = (kern_log_addr << 4) | kern_log_addr;
			ping_cec_ll_tx(msg, 1);
			_udelay(100);
			cec_dbg_print("Set cec log_addr:0x", kern_log_addr);
			cec_dbg_print(",ADDR0:", cec_get_log_addr());
			uart_puts("\n");
			probe = NULL;
			regist_devs = 0;
			i = 0;
			retry = 0;
			/* continue polling addr for second playback logic
			 * addr if it's configured in kernel, here
			 * we assume the second logic addr is playback addr
			 */
			if (kern_log_addr2 != 0) {
				start_poll_log_addr = kern_log_addr2;
			} else {
				/* only 1 logic addr for soundbar */
				cec_msg.log_addr = kern_log_addr;
				return;
			}
		} else if (is_audio_system_dev(kern_log_addr2)) {
			cec_set_log_addr(kern_log_addr2);
			msg[0] = (kern_log_addr2 << 4) | kern_log_addr2;
			ping_cec_ll_tx(msg, 1);
			_udelay(100);
			cec_dbg_print("Set cec log_addr:0x", kern_log_addr2);
			cec_dbg_print(",ADDR0:", cec_get_log_addr());
			uart_puts("\n");
			probe = NULL;
			regist_devs = 0;
			i = 0;
			retry = 0;
			/* continue polling addr for second logic
			 * if it's configured in kernel
			 */
			if (kern_log_addr != 0) {
				start_poll_log_addr = kern_log_addr;
			} else {
				/* only 1 logic addr for soundbar */
				cec_msg.log_addr = kern_log_addr2;
				return;
			}
		}

		for (i = 0; i < 3; i++) {
			if (start_poll_log_addr == player_dev[i][0]) {
				probe = player_dev[i];
				break;
			}
		}
		if (probe == NULL) {
			probe = player_dev[0];
		}
		i = 0;
	}

	/* don't need to set logic addr before poll logic addr */
	/* cec_set_log_addr(probe[i]); */
	msg[0] = (probe[i]<<4) | probe[i];
	tx_stat = ping_cec_ll_tx(msg, 1);
	if (tx_stat == TX_BUSY) {   // can't get cec bus
		retry++;
		cec_hw_reset();
		if (!(retry & 0x03)) {
			cec_dbg_print("retry too much, log_addr:0x", probe[i]);
			uart_puts("\n");
		} else {
			i -= 1;
		}
	} else if (tx_stat == TX_ERROR) {
		_udelay(100);
		/* if there're 2 logic addr(soundbar):
		 * use audio system addr as main logic addr
		 */
		if (is_audio_system_dev(kern_log_addr) ||
			is_audio_system_dev(kern_log_addr2))
			cec_msg.log_addr = CEC_AUDIO_SYSTEM_ADDR;
		else
			cec_msg.log_addr = probe[i];
		cec_set_log_addr(probe[i]);
		cec_dbg_print("Set cec log_addr:0x", probe[i]);
		cec_dbg_print(", ADDR0:", cec_get_log_addr());
		uart_puts("\n");
		probe = NULL;
		regist_devs = 0;
		i = 0;
		retry = 0;
		dump_cecb_reg();
		return ;
	} else if (tx_stat == TX_DONE) {
		cec_dbg_print("somebody takes cec log_addr:0x", probe[i]);
		uart_puts("\n");
		regist_devs |= (1 << i);
		retry += (4 - (retry & 0x03));
		if (regist_devs == 0x07) {
			// No avilable logical address
			cec_msg.log_addr = 0x0f;
			cec_set_log_addr(15);
			uart_puts("CEC allocate logic address failed\n");
		}
	}
	i++;
	if (i == 3) {
		i = 0;
	}
}

int cec_suspend_wakeup_chk(void)
{
	int exit_reason = 0;

	if ((cec_msg.cec_power == 0x1) &&
		(hdmi_cec_func_config & 0x1)) {
		if (cec_wait_addr++ < 100) {
			if (cec_msg.active_source) {
				cec_save_port_id();
				exit_reason = CEC_WAKEUP;
				uart_puts("check wakeup\n");
			}
			uart_puts(".");
		} else {
			exit_reason = CEC_WAKEUP;
			uart_puts("timeout wakeup\n");
		}
	}

	if (exit_reason == CEC_WAKEUP)
		return 1;
	else
		return 0;
}

int cec_suspend_handle(void)
{
	int exit_reason = 0;

	if (cec_msg.log_addr) {
		if (hdmi_cec_func_config & 0x1) {
			cec_handler();
			if (cec_msg.cec_power == 0x1) {
				if (cec_msg.active_source) {
					cec_save_port_id();
					/*cec power key*/
					exit_reason = CEC_WAKEUP;
					uart_puts("message wakeup\n");
				}
			}
		}
	} else if (hdmi_cec_func_config & 0x1)
		cec_node_init();

	if (exit_reason == CEC_WAKEUP) {
		uart_puts("active source:");
		uart_put_hex(cec_msg.active_source, 8);
		uart_puts("\n");

		uart_puts("wk_logic_addr:");
		uart_put_hex(cec_wakup.wk_logic_addr, 8);
		uart_puts("\n");

		uart_puts("wk_phy_addr:0x");
		uart_put_hex(cec_wakup.wk_phy_addr, 16);
		uart_puts("\n");

		uart_puts("wk_port_id:0x");
		uart_put_hex(cec_wakup.wk_port_id, 8);
		uart_puts("\n");
		return 1;
	} else
		return 0;
}
#endif

