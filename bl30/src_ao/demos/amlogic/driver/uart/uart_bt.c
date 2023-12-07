/*
 * Copyright (C) 2023-2033 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License Agreement
 * (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 * only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission from
 * Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdbool.h>

#include "util.h"
#include "register.h"
#include "timer_source.h"
#include "myprintf.h"
#include "uart_bt.h"
#include "mailbox-api.h"

#define writel(val,reg) (*((volatile unsigned *)(reg)))=(val)
#define readl(reg)      (*((volatile unsigned *)(reg)))
#define setbits_le32(reg,val)   (*((volatile unsigned *)(reg)))|=(val)
#define clrbits_le32(reg,val)   (*((volatile unsigned *)(reg)))&=(~(val))
#define clrsetbits_le32(reg,clr,set)    {unsigned __v=readl(reg);__v&=(~(clr));__v|=(set);writel(__v,reg);}

#define UART_BASE_ADDR  UART_E_WFIFO

#define UART_WFIFO      (0 << 2)
#define UART_RFIFO      (1 << 2)
#define UART_CONTROL    (2 << 2)
#define UART_STATUS     (3 << 2)
#define UART_MISC       (4 << 2)
#define UART_REG5       (5 << 2)

#define P_UART(uart_base,reg)   (uart_base + (reg))

#define P_UART_CONTROL(uart_base)   P_UART(uart_base, UART_CONTROL)
#define UART_CNTL_MASK_BAUD_RATE        (0xfff)
#define UART_CNTL_MASK_TX_EN            (1<<12)
#define UART_CNTL_MASK_RX_EN            (1<<13)
#define UART_CNTL_MASK_2WIRE            (1<<15)
#define UART_CNTL_MASK_STP_BITS         (3<<16)
#define UART_CNTL_MASK_STP_1BIT         (0<<16)
#define UART_CNTL_MASK_STP_2BIT         (1<<16)
#define UART_CNTL_MASK_PRTY_EVEN        (0<<18)
#define UART_CNTL_MASK_PRTY_ODD         (1<<18)
#define UART_CNTL_MASK_PRTY_EN          (1<<19)
#define UART_CNTL_MASK_CHAR_LEN         (3<<20)
#define UART_CNTL_MASK_CHAR_8BIT        (0<<20)
#define UART_CNTL_MASK_CHAR_7BIT        (1<<20)
#define UART_CNTL_MASK_CHAR_6BIT        (2<<20)
#define UART_CNTL_MASK_CHAR_5BIT        (3<<20)
#define UART_CNTL_MASK_RST_TX           (1<<22)
#define UART_CNTL_MASK_RST_RX           (1<<23)
#define UART_CNTL_MASK_CLR_ERR          (1<<24)
#define UART_CNTL_MASK_INV_RX           (1<<25)
#define UART_CNTL_MASK_INV_TX           (1<<26)
#define UART_CNTL_MASK_RINT_EN          (1<<27)
#define UART_CNTL_MASK_TINT_EN          (1<<28)
#define UART_CNTL_MASK_INV_CTS          (1<<29)
#define UART_CNTL_MASK_MASK_ERR         (1<<30)
#define UART_CNTL_MASK_INV_RTS          (1<<31)

#define P_UART_STATUS(uart_base)    P_UART(uart_base, UART_STATUS )
#define UART_STAT_MASK_RFIFO_CNT        (0x7f<<0)
#define UART_STAT_MASK_TFIFO_CNT        (0x7f<<8)
#define UART_STAT_MASK_PRTY_ERR         (1<<16)
#define UART_STAT_MASK_FRAM_ERR         (1<<17)
#define UART_STAT_MASK_WFULL_ERR        (1<<18)
#define UART_STAT_MASK_RFIFO_FULL       (1<<19)
#define UART_STAT_MASK_RFIFO_EMPTY      (1<<20)
#define UART_STAT_MASK_TFIFO_FULL       (1<<21)
#define UART_STAT_MASK_TFIFO_EMPTY      (1<<22)
#define UART_STAT_MASK_XMIT_BUSY        (1<<25)
#define UART_STAT_MASK_RECV_BUSY        (1<<26)

#define P_UART_MISC(uart_base)  P_UART(uart_base, UART_MISC)
#define UART_CTRL_USE_XTAL_CLK          (1<<24)
#define UART_CTRL_USE_NEW_BAUD_RATE     (1<<23)
#define UART_CTRL_USE_XTAL_DIV2         (1<<27)

#define UART_BAUD_115200        115200
#define UART_BAUD_921600        912600
#define UART_BAUD_1M            1000000
#define UART_BAUD_2M            2000000
#define UART_BAUD_4M            4000000

#define HCI_MAX_CMD_SIZE        260
#define HCI_MAX_EVENT_SIZE      260

#define MBTAG "AOCPU"

/* debug log switch */
#define AML_DEBUG false

#ifndef INFO
#define INFO(fmt, args...) printf("[%s] " fmt "\n", __func__, ##args)
#endif

#ifndef DEBUG
#define DEBUG(fmt, args...) {if (AML_DEBUG) {printf("[%s] " fmt "\n", __func__, ##args);}}
#endif

static bool is_qcom_uart_bt = false;

static void set_uart_control(unsigned int val)
{
    writel(val, (UART_BASE_ADDR + UART_CONTROL));
    DEBUG("reset val: 0x%08x", val);
}

static unsigned int get_uart_control(void)
{
    return readl(UART_BASE_ADDR + UART_CONTROL);
}

static unsigned int get_uart_status(void)
{
    return readl(UART_BASE_ADDR + UART_STATUS);
}

static unsigned int get_uart_ck_cfg(void)
{
    return readl(CLKCTRL_SYS_CLK_EN0_REG1);
}

static void serial_init_uart_a(unsigned set)
{
    unsigned long baud_para;
    int val;

    /* set the pinmux */
    val = readl(PADCTRL_PIN_MUX_REG4);
    val &= 0xff00ffff;
    //val |= (0x11 << 16);
    val |= (0x1111 << 16);    // add CTS and RTS
    writel(val, PADCTRL_PIN_MUX_REG4);

    /* set baud rate */
    baud_para = (12000000 + set / 2) / set - 1;
    writel(0
        | UART_CNTL_MASK_STP_1BIT
        | UART_CNTL_MASK_CHAR_8BIT
        | UART_CNTL_MASK_RST_TX
        | UART_CNTL_MASK_RST_RX
        | UART_CNTL_MASK_CLR_ERR
        | UART_CNTL_MASK_TX_EN
        | UART_CNTL_MASK_RX_EN
        | UART_CNTL_MASK_2WIRE, (UART_BASE_ADDR + UART_CONTROL));

    writel(baud_para |
        UART_CTRL_USE_XTAL_CLK |
        UART_CTRL_USE_XTAL_DIV2 |
        UART_CTRL_USE_NEW_BAUD_RATE, (UART_BASE_ADDR + UART_REG5));

    clrbits_le32((UART_BASE_ADDR + UART_CONTROL),
        UART_CNTL_MASK_RST_TX | UART_CNTL_MASK_RST_RX |
        UART_CNTL_MASK_CLR_ERR);

    INFO("bt uart init, baud rate: %d", set);

    DEBUG("UART_STATUS: 0x%08x, UART_CONTROL: 0x%08x, UART_CLK_CFG: 0x%08x", get_uart_status(),
        get_uart_control(), get_uart_ck_cfg());
}

#ifdef USE_DEF
static void serial_set_flow_ctrl(unsigned set)
{
    if (set) {
        setbits_le32((UART_BASE_ADDR + UART_CONTROL),
            UART_CNTL_MASK_RST_TX | UART_CNTL_MASK_RST_RX);

    } else {
        clrbits_le32((UART_BASE_ADDR + UART_CONTROL),
            UART_CNTL_MASK_RST_TX | UART_CNTL_MASK_RST_RX |
            UART_CNTL_MASK_CLR_ERR);
    }
}

static void serial_put_hex_uart_a(unsigned int data, unsigned bitlen)
{
    int i;
    for (i = bitlen - 4; i >= 0; i -= 4) {
        if ((data >> i) == 0) {
            serial_putc_uart_a(0x30);
            continue;
        }

        unsigned char s = (data >> i) & 0xf;
        if (s < 10)
            serial_putc_uart_a(0x30 + s);
        else
            serial_putc_uart_a(0x61 + s - 10);
    }
}

static void serial_put_dec_uart_a(unsigned int data)
{
    char szTxt[10];
    szTxt[0] = 0x30;
    int i = 0;

    while (data) {
        szTxt[i++] = (data % 10) + 0x30;
        data = data / 10;
    }

    for (--i; i >= 0; --i)
        serial_putc_uart_a(szTxt[i]);

    serial_wait_tx_empty_uart_a();
}
#endif

/*
 * Read a single byte from the serial port. Returns 1 on success, otherwise 0.
 */
static int serial_tstc_uart_a(void)
{
    return (readl((UART_BASE_ADDR + UART_STATUS)) & UART_STAT_MASK_RFIFO_CNT);
}

static void serial_putc_uart_a(const char c)
{
    /* Wait till dataTx register is not full */
    while ((readl((UART_BASE_ADDR + UART_STATUS)) & UART_STAT_MASK_TFIFO_FULL)) ;
    writel(c, (UART_BASE_ADDR + UART_WFIFO));
    /* Wait till dataTx register is empty */
}

/*
 * Read a single byte from the serial port.
 */
static int serial_getc_uart_a(void)
{
    unsigned char ch;
    /* Wait till character is placed in fifo */
    while ((readl((UART_BASE_ADDR + UART_STATUS)) &
        UART_STAT_MASK_RFIFO_CNT) == 0) ;

    /* Also check for overflow errors */
    if (readl((UART_BASE_ADDR + UART_STATUS)) &
        (UART_STAT_MASK_PRTY_ERR | UART_STAT_MASK_FRAM_ERR)) {
        setbits_le32((UART_BASE_ADDR + UART_CONTROL), UART_CNTL_MASK_CLR_ERR);
        clrbits_le32((UART_BASE_ADDR + UART_CONTROL), UART_CNTL_MASK_CLR_ERR);

    }
    ch = readl((UART_BASE_ADDR + UART_RFIFO)) & 0x00ff;
    return ((int)ch);
}

#ifdef USE_DEF
static void serial_wait_tx_empty_uart_a(void)
{
    while ((readl((UART_BASE_ADDR + UART_STATUS)) & UART_STAT_MASK_TFIFO_EMPTY) == 0) ;
}

static void serial_puts_uart_a(const char *s)
{
    while (*s) {
        serial_putc_uart_a(*s++);
    }
    serial_wait_tx_empty_uart_a();
}

static void delay_us(int time)
{
    int i;

    /* bl30 clock 24M */
    for (i = 0; i < 8*time; i++) {
        ;
    }
}
#endif

#if (defined(AML_DEBUG) && (AML_DEBUG == true))
static void dump_buf(unsigned char *out, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        if (i && (!(i % 16))) {

            printf("\n");
        }

        printf("%02x ", out[i]);
    }

    printf("\n");
}
#endif

static int bt_plt_uart_write(unsigned char* buf, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        serial_putc_uart_a(buf[i]);
    }

    return size;
}

static int bt_plt_uart_read(unsigned char* buf, int size, int timeout)
{
    int cnt = 0;
    bool block = (timeout == 0);

    for (; cnt < size;) {
        if (serial_tstc_uart_a()) {
            buf[cnt] = (unsigned char)serial_getc_uart_a();
            cnt++;

            if (cnt == size) {
                break;
            }
        } else {
            if (block == false) {
                timeout--;
                udelay(1);
                //delay_us(1);
                if (timeout == 0) {
                    break;
                }
            }
        }
    }

    return cnt;
}

static void hci_send_cmd(unsigned char *buf, int len)
{
#if (defined(AML_DEBUG) && (AML_DEBUG == true))
    DEBUG("bt dump write cmd");
    dump_buf(buf, len);
#endif

    bt_plt_uart_write(buf, len);
}

static int hci_read_event(unsigned char* buf, int size, int timeout)
{
    int remain, r;
    int cnt = 0;

    if (size < HCI_MAX_EVENT_SIZE) {
        INFO("Invalid size arguement!");
        return -1;;
    }

    DEBUG("Wait for cmd compete event from SOC");

    /* The first byte identifies the packet type. For HCI event packets, it
     * should be 0x04, so we read until we get to the 0x04. */
    while (1) {
        r = bt_plt_uart_read(buf, 1, timeout);
        if (r <= 0) {
            INFO("read event char fail1");
            return -1;
        }
        if (buf[0] == 0x04)
            break;
    }
    cnt++;

    /* The next two bytes are the event code and parameter total length. */
    while (cnt < 3) {
        r = bt_plt_uart_read((buf + cnt), (3 - cnt), timeout);
        if (r <= 0) {
            INFO("read event char fail2");
            return -1;
        }
        cnt += r;
    }

    /* Now we read the parameters. */
    if (buf[2] < (size - 3))
        remain = buf[2];
    else
        remain = size - 3;

    while ((cnt - 3) < remain) {
        r = bt_plt_uart_read((buf + cnt), (remain - (cnt - 3)), timeout);
        if (r <= 0) {
            INFO("read event char fail3");
            return -1;
        }
        cnt += r;
    }

#if (defined(AML_DEBUG) && (AML_DEBUG == true))
        DEBUG("bt dump read cmd");
        dump_buf(buf, cnt);
#endif

    return cnt;
}

static int qca_hci_send_cmd(__attribute__((unused))int fd, unsigned char *cmd, int cmd_len,
    unsigned char *rsp, int rsp_len, int timeout)
{
    int ret = 0;

    if (rsp_len < HCI_MAX_EVENT_SIZE) {
        INFO("rsp buf len err, rsp_len: %d", rsp_len);
        ret = -1;
        goto err;
    }

    hci_send_cmd(cmd, cmd_len);

    memset(rsp, 0, HCI_MAX_EVENT_SIZE);

    /* Wait for command complete event */
    ret = hci_read_event(rsp, HCI_MAX_EVENT_SIZE, timeout);
    if (ret < 0) {
        INFO("read controller ack event fail, ret: %d", ret);
        goto err;
    }

err:
    return ret;
}

static void qca_hci_send_addr_req(__attribute__((unused))int fd, unsigned char *cmd, int cmd_len,
    unsigned char *rsp, int rsp_len)
{
    int ret = 0;

    qca_hci_send_cmd(fd, cmd, cmd_len, rsp, rsp_len, 1000);

    if ((rsp[4] != 0x09) || (rsp[5] != 0x10)) {
        INFO("qca bt read mac fail, wait for another 150ms");
        memset(rsp, 0, HCI_MAX_EVENT_SIZE);
        /* Wait for command complete event */
        ret = hci_read_event(rsp, HCI_MAX_EVENT_SIZE, 150000);
        if (ret < 0) {
            INFO("read controller ack event fail, ret: %d", ret);
        }
    }
}

static int qca_woble_configure(int fd)
{
    unsigned char rsp[HCI_MAX_EVENT_SIZE];
    unsigned char reset_cmd[] = {0x01, 0x03, 0x0C, 0x00};
    unsigned char read_bd_addr[] = {0x01, 0x09, 0x10, 0x00};
    unsigned char apcf_set_filtering_param[] = {
        0x01, 0x57, 0xFD, 0x12, 0x01, 0x00, 0x00, 0x20, 0x00, 0x00,0x00, 0x00, 0xA6, 0x00, 0x00, 0x00,
        0x00, 0xA6, 0x00, 0x00, 0x02, 0x00};
    unsigned char apcf_config_manf_data[] = {
        0x01, 0x57, 0xFD, 0x25, 0x06, 0x00, 0x00, 0xff, 0xff, 0x41, 0x6d, 0x6c, 0x6f, 0x67, 0x69, 0x63,
        0x01, 0x78, 0xc5, 0xe5, 0x9b, 0x61, 0xea, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00};
    /* Google standard of bluetooth remote control APCF configuration */
    unsigned char apcf_set_filtering_param1[] = {
        0x01, 0x57, 0xFD, 0x12, 0x01, 0x00, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0xA6, 0x00, 0x00, 0x00,
        0x00, 0xA6, 0x00, 0x00, 0x02, 0x00};
    unsigned char apcf_config_service_data[] = {
        0x01, 0x57, 0xFD, 0x19, 0x07, 0x00, 0x01, 0x36, 0xFD, 0x01, 0x01, 0x01, 0x08, 0x44, 0x32, 0xAA,
        0x47, 0x61, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    unsigned char apcf_enable[] = {0x01, 0x57, 0xFD, 0x02, 0x00, 0x01};
    unsigned char le_scan_param_setting[] = {0x01, 0x0b, 0x20, 0x07, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00};
    unsigned char host_sleep_vsc[] = {0x01, 0x6C, 0xFC, 0x02, 0x03, 0x01};
    unsigned char le_scan_enable[] = {0x01, 0x0c, 0x20, 0x02, 0x01, 0x00};

    qca_hci_send_addr_req(fd, read_bd_addr, sizeof(read_bd_addr), rsp, HCI_MAX_EVENT_SIZE);
    DEBUG("qca bt mac: 0x%02X:%02X:%02X:%02X:%02X:%02X", rsp[12], rsp[11], rsp[10], rsp[9], rsp[8], rsp[7]);
    memcpy(apcf_config_manf_data+17, rsp+7, 6);
    memcpy(apcf_config_service_data+12, rsp+7, 6);
    qca_hci_send_cmd(fd, reset_cmd, sizeof(reset_cmd), rsp, HCI_MAX_EVENT_SIZE, 150000);
    qca_hci_send_cmd(fd, apcf_set_filtering_param, sizeof(apcf_set_filtering_param), rsp, HCI_MAX_EVENT_SIZE, 1000);
    qca_hci_send_cmd(fd, apcf_config_manf_data, sizeof(apcf_config_manf_data), rsp, HCI_MAX_EVENT_SIZE, 1000);

    qca_hci_send_cmd(fd, apcf_set_filtering_param1, sizeof(apcf_set_filtering_param1),
        rsp, HCI_MAX_EVENT_SIZE, 1000);
    qca_hci_send_cmd(fd, apcf_config_service_data, sizeof(apcf_config_service_data),
        rsp, HCI_MAX_EVENT_SIZE, 1000);
    qca_hci_send_cmd(fd, apcf_enable, sizeof(apcf_enable), rsp, HCI_MAX_EVENT_SIZE, 1000);
    qca_hci_send_cmd(fd, le_scan_param_setting, sizeof(le_scan_param_setting), rsp, HCI_MAX_EVENT_SIZE, 1000);
    qca_hci_send_cmd(fd, host_sleep_vsc, sizeof(host_sleep_vsc), rsp, HCI_MAX_EVENT_SIZE, 1000);
    qca_hci_send_cmd(fd, le_scan_enable, sizeof(le_scan_enable), rsp, HCI_MAX_EVENT_SIZE, 1000);

    INFO();

    return 0;
}

static bool get_qcom_uart_bt_flag(void)
{
    return is_qcom_uart_bt;
}

static void set_qcom_uart_bt_flag(bool flag)
{
    is_qcom_uart_bt = flag;
}

void bt_suspend_handle(void)
{
    unsigned int val = 0;

    if (get_qcom_uart_bt_flag()) {
        val = get_uart_control();
        serial_init_uart_a(UART_BAUD_2M);
        qca_woble_configure(0);
        set_uart_control(val);
    } else {
        DEBUG("not qcom uart bt, ignore");
    }
}

/*
 * AOCPU received bt message sent by mailbox is handled here
 */
static void mbox_bt_name(void *msg)
{
    char recv[20] = {'\0'};
    char bt_name[] = {"qca_bt"};

    memcpy(recv, msg, sizeof(recv));
    memset(msg, 0, MBOX_BUF_LEN);
    DEBUG("[%s]: from ARM REE: %s", MBTAG, recv);

    if (memcmp(recv, bt_name, (sizeof(bt_name) - 1)) == 0) {
        INFO("bt_name is qca_bt");
        set_qcom_uart_bt_flag(true);
    }
}

void register_bt_rpc_callback(void)
{
    int ret;

    ret = xInstallRemoteMessageCallbackFeedBack(AOREE_CHANNEL, MBX_CMD_SET_MID, (void *)mbox_bt_name, 0);
    if (ret == MBOX_CALL_MAX) {
        INFO("[%s]: mbox cmd 0x%x register fail", MBTAG, MBX_CMD_SET_MID);
    }
}

