/*
 * Copyright (C)2018 Amlogic, Inc. All rights reserved.
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
 * Author:  Jian Hu <jian.hu@amlogic.com>
 */

#include "common.h"
#include "uart.h"
#include "FreeRTOS.h"
#include <task.h>
#include "meson_i2c.h"
#include <util.h>
#include "timer_source.h"

#define BIT(nr)			(1UL << (nr))
#define GENMASK(h, l) \
(((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#define I2C_TIMEOUT_MS			1000*100

/* Control register fields */
#define REG_CTRL_START			BIT(0)
#define REG_CTRL_ACK_IGNORE		BIT(1)
#define REG_CTRL_STATUS			BIT(2)
#define REG_CTRL_ERROR			BIT(3)
#define REG_CTRL_CLKDIV_SHIFT		12
#define REG_CTRL_CLKDIV_MASK		GENMASK(21, 12)
#define REG_CTRL_CLKDIVEXT_SHIFT	28
#define REG_CTRL_CLKDIVEXT_MASK		GENMASK(29, 28)

#define I2C_DEBUG			0

typedef enum { false, true } bool;

enum {
	TOKEN_END = 0,
	TOKEN_START,
	TOKEN_SLAVE_ADDR_WRITE,
	TOKEN_SLAVE_ADDR_READ,
	TOKEN_DATA,
	TOKEN_DATA_LAST,
	TOKEN_STOP,
};

struct xI2cRegs {
	uint32_t ctrl;
	uint32_t slave_addr;
	uint32_t tok_list0;
	uint32_t tok_list1;
	uint32_t tok_wdata0;
	uint32_t tok_wdata1;
	uint32_t tok_rdata0;
	uint32_t tok_rdata1;
};

struct xMesonI2c {
	struct xI2cRegs *regs;
	struct xI2cMsg *msg;	/* Current I2C message */
	bool last;		/* Whether the message is the last */
	uint32_t count;		/* Number of bytes in the current transfer */
	uint32_t pos;		/* Position of current transfer in message */
	uint32_t tokens[2];	/* Sequence of tokens to be written */
	uint32_t num_tokens;	/* Number of tokens to be written */
	uint32_t clock_frequency;
	uint32_t div_factor;
	uint32_t delay_adjust;
	uint32_t clkin_rate;
};

struct xMesonI2c i2cs[11];
struct xMesonI2cPlatdata *plat;

uint32_t current_id;

#define DEFAULT_CLK81	0

/* AXG/G12A/G12B/SM1/TM2 i2c data */
struct xMesonI2cPlatdata AxgI2cData[] = {
	{0, 0xffd1f000, 3, 15, 100000, MESON_I2C_CLK_RATE, DEFAULT_CLK81, 0},
	{1, 0xffd1e000, 3, 15, 100000, MESON_I2C_CLK_RATE, DEFAULT_CLK81, 0},
	{2, 0xffd1d000, 3, 15, 100000, MESON_I2C_CLK_RATE, DEFAULT_CLK81, 0},
	{3, 0xffd1c000, 3, 15, 100000, MESON_I2C_CLK_RATE, DEFAULT_CLK81, 0},
	{4, 0xff805000, 3, 15, 100000, MESON_I2C_CLK_RATE, DEFAULT_CLK81, 0},
};

/* A1 i2c data */
struct xMesonI2cPlatdata A1I2cData[] = {
	{0, 0xfe001400, 3, 15, 100000, 64000000, DEFAULT_CLK81, 0},	/* i2c A */
	{1, 0xfe005c00, 3, 15, 100000, 64000000, DEFAULT_CLK81, 0},	/* i2c B */
	{2, 0xfe006800, 3, 15, 100000, 64000000, DEFAULT_CLK81, 0},	/* i2c C */
	{3, 0xfe006c00, 3, 15, 100000, 64000000, DEFAULT_CLK81, 0},	/* i2c D */
};

/* C1 i2c data */
struct xMesonI2cPlatdata C1I2cData[] = {
	{0, 0xfe001400, 3, 15, 100000, MESON_I2C_CLK_RATE, DEFAULT_CLK81, 0},	/* i2c A */
	{1, 0xfe005c00, 3, 15, 100000, MESON_I2C_CLK_RATE, DEFAULT_CLK81, 0},	/* i2c B */
	{2, 0xfe006800, 3, 15, 100000, MESON_I2C_CLK_RATE, DEFAULT_CLK81, 0},	/* i2c C */
	{3, 0xfe006c00, 3, 15, 100000, MESON_I2C_CLK_RATE, DEFAULT_CLK81, 0},	/* i2c D */
	{4, 0xfe00b000, 3, 15, 100000, MESON_I2C_CLK_RATE, DEFAULT_CLK81, 0},	/* i2c E */
};

static void prvSetBitsLe32(uint32_t *reg, uint32_t set)
{
	uint32_t val;

	val = REG32(reg);
	val |= (set);
	REG32(reg) = val;
}

static void prvClrBitsLe32(uint32_t *reg, uint32_t clr)
{
	uint32_t val;

	val = REG32(reg);
	val &= (~(clr));
	REG32(reg) = val;
}

static void prvClrSetBitsLe32(uint32_t *reg, uint32_t clr, uint32_t set)
{
	uint32_t val;

	val = REG32(reg);
	val &= (~(clr));
	val |= (set);
	REG32(reg) = val;
}

void xI2cDumpRegs(void)
{
	iprintf("i2c reg : 0x%x = 0x%x\n", &i2cs[current_id].regs->ctrl,
		i2cs[current_id].regs->ctrl);
	iprintf("i2c reg : 0x%x = 0x%x\n", &i2cs[current_id].regs->slave_addr,
		i2cs[current_id].regs->slave_addr);
	iprintf("i2c reg : 0x%x = 0x%x\n", &i2cs[current_id].regs->tok_list0,
		i2cs[current_id].regs->tok_list0);
	iprintf("i2c reg : 0x%x = 0x%x\n", &i2cs[current_id].regs->tok_list1,
		i2cs[current_id].regs->tok_list1);
	iprintf("i2c reg : 0x%x = 0x%x\n", &i2cs[current_id].regs->tok_wdata0,
		i2cs[current_id].regs->tok_wdata0);
	iprintf("i2c reg : 0x%x = 0x%x\n", &i2cs[current_id].regs->tok_wdata1,
		i2cs[current_id].regs->tok_wdata1);
	iprintf("i2c reg : 0x%x = 0x%x\n", &i2cs[current_id].regs->tok_rdata0,
		i2cs[current_id].regs->tok_rdata0);
	iprintf("i2c reg : 0x%x = 0x%x\n", &i2cs[current_id].regs->tok_rdata1,
		i2cs[current_id].regs->tok_rdata1);
}

static void prvMesonI2cRegsInit(void)
{
	uint32_t i;

	for (i = 0; i < 8; i++)
		REG32(&i2cs[current_id].regs->ctrl + i) = 0;
}

static void prvMesonI2cResetTokens(void)
{
	i2cs[current_id].tokens[0] = 0;
	i2cs[current_id].tokens[1] = 0;
	i2cs[current_id].num_tokens = 0;
}

static void prvMesonI2cAddToken(uint32_t token)
{
	if (i2cs[current_id].num_tokens < 8)
		i2cs[current_id].tokens[0] |=
		    (token & 0xf) << (i2cs[current_id].num_tokens * 4);
	else
		i2cs[current_id].tokens[1] |=
		    (token & 0xf) << ((i2cs[current_id].num_tokens % 8) * 4);

	i2cs[current_id].num_tokens++;
}

/*
 * Retrieve data for the current transfer (which can be at most 8
 * bytes) from the device internal buffer.
 */
static void prvMesonI2cGetData(uint8_t *buf, uint32_t len)
{
	uint32_t rdata0, rdata1;
	uint32_t i;

	rdata0 = i2cs[current_id].regs->tok_rdata0;
	rdata1 = i2cs[current_id].regs->tok_rdata1;

#if I2C_DEBUG
	iprintf("meson i2c: read data %08x %08x len %d\n", rdata0, rdata1, len);
#endif

	for (i = 0; i < MIN((uint32_t) 4, len); i++)
		*buf++ = (rdata0 >> i * 8) & 0xff;

	for (i = 4; i < MIN((uint32_t) 8, len); i++)
		*buf++ = (rdata1 >> (i - 4) * 8) & 0xff;
}

/*
 * Write data for the current transfer (which can be at most 8 bytes)
 * to the device internal buffer.
 */
static void prvMesonI2cPutData(uint8_t *buf, uint32_t len)
{
	uint32_t wdata0 = 0, wdata1 = 0;
	uint32_t i;

	for (i = 0; i < MIN((uint32_t) 4, len); i++)
		wdata0 |= *buf++ << (i * 8);

	for (i = 4; i < MIN((uint32_t) 8, len); i++)
		wdata1 |= *buf++ << ((i - 4) * 8);

	i2cs[current_id].regs->tok_wdata0 = wdata0;
	i2cs[current_id].regs->tok_wdata1 = wdata1;

#if I2C_DEBUG
	iprintf("meson i2c: write data 0x%08x 0x%08x len %d\n", wdata0, wdata1,
		len);
#endif
}

/*
 * Prepare the next transfer: pick the next 8 bytes in the remaining
 * part of message and write tokens and data (if needed) to the
 * device.
 */
static void prvMesonI2cPrepareXfer(void)
{
	bool write = !(i2cs[current_id].msg->flags & I2C_M_RD);
	uint32_t i;

	i2cs[current_id].count =
	    MIN(i2cs[current_id].msg->len - i2cs[current_id].pos, 8u);

	for (i = 0; i + 1 < i2cs[current_id].count; i++)
		prvMesonI2cAddToken(TOKEN_DATA);

	if (i2cs[current_id].count) {
		if (write
		    || i2cs[current_id].pos + i2cs[current_id].count <
		    i2cs[current_id].msg->len)
			prvMesonI2cAddToken(TOKEN_DATA);
		else
			prvMesonI2cAddToken(TOKEN_DATA_LAST);
	}

	if (write)
		prvMesonI2cPutData(i2cs[current_id].msg->buf +
				   i2cs[current_id].pos,
				   i2cs[current_id].count);

	if (i2cs[current_id].last
	    && i2cs[current_id].pos + i2cs[current_id].count >=
	    i2cs[current_id].msg->len)
		prvMesonI2cAddToken(TOKEN_STOP);

	i2cs[current_id].regs->tok_list0 = i2cs[current_id].tokens[0];
	i2cs[current_id].regs->tok_list1 = i2cs[current_id].tokens[1];
}

static void prvMesonI2cDoStart(struct xI2cMsg *msg)
{
	uint32_t token;

	token = (msg->flags & I2C_M_RD) ? TOKEN_SLAVE_ADDR_READ :
	    TOKEN_SLAVE_ADDR_WRITE;
	/* change it if duty change */
	i2cs[current_id].regs->slave_addr = msg->addr << 1;
	prvMesonI2cAddToken(TOKEN_START);
	prvMesonI2cAddToken(token);
}

static int32_t prvMesonI2cXferMsg(struct xI2cMsg *msg, uint32_t last)
{
	uint32_t time_count = 0;
	uint32_t *ctrl = &i2cs[current_id].regs->ctrl;

	taskENTER_CRITICAL();
#if I2C_DEBUG
	iprintf("meson i2c: %s addr 0x%x len %u\n",
		(msg->flags & I2C_M_RD) ? "read" : "write",
		msg->addr, msg->len);
#endif
	i2cs[current_id].msg = msg;
	i2cs[current_id].last = last;
	i2cs[current_id].pos = 0;
	i2cs[current_id].count = 0;

	prvMesonI2cResetTokens();
	prvMesonI2cDoStart(msg);

	do {
		prvMesonI2cPrepareXfer();
		/* start the transfer */
		prvSetBitsLe32(&(i2cs[current_id].regs->ctrl), REG_CTRL_START);
		while (REG32(ctrl) & REG_CTRL_STATUS) {
			if (time_count > I2C_TIMEOUT_MS) {
				prvClrBitsLe32(&i2cs[current_id].regs->ctrl,
					       REG_CTRL_START);
				iprintf("meson i2c: timeout\n");
				return -1;
			}
			udelay(1);
			time_count++;
		}
		prvMesonI2cResetTokens();
		prvClrBitsLe32(&i2cs[current_id].regs->ctrl, REG_CTRL_START);

		if (REG32(ctrl) & REG_CTRL_ERROR) {
			iprintf("meson i2c: error(0x%x)\n", REG32(ctrl));
			return -1;
		}

		if ((msg->flags & I2C_M_RD) && i2cs[current_id].count) {
			prvMesonI2cGetData(i2cs[current_id].msg->buf +
					   i2cs[current_id].pos,
					   i2cs[current_id].count);
		}
		i2cs[current_id].pos += i2cs[current_id].count;

	} while (i2cs[current_id].pos < msg->len);
	taskEXIT_CRITICAL();

	return 0;
}

int32_t xI2cMesonXfer(struct xI2cMsg *msg, uint32_t nmsgs)
{
	uint32_t i;
	int32_t ret;

	for (i = 0; i < nmsgs; i++) {
		ret = prvMesonI2cXferMsg(msg + i, i == (nmsgs - 1));
		if (ret) {
			iprintf("meson_i2c_xfer  error\n");
			return ret;
		}
	}

	return 0;
}

int32_t xI2cMesonSetBusSpeed(uint32_t speed)
{
	uint32_t clk_rate = i2cs[current_id].clkin_rate;
	uint32_t div;

	taskENTER_CRITICAL();
	div = DIV_ROUND_UP(clk_rate, speed * i2cs[current_id].div_factor);

	/* clock divider has 12 bits */
	if (div >= (1 << 12)) {
		iprintf("meson i2c: requested bus frequency too low\n");
		div = (1 << 12) - 1;
	}
	prvClrSetBitsLe32(&i2cs[current_id].regs->ctrl, REG_CTRL_CLKDIV_MASK,
			  (div & GENMASK(9, 0)) << REG_CTRL_CLKDIV_SHIFT);

	prvClrSetBitsLe32(&i2cs[current_id].regs->ctrl, REG_CTRL_CLKDIVEXT_MASK,
			  (div >> 10) << REG_CTRL_CLKDIVEXT_SHIFT);
	taskEXIT_CRITICAL();

	return 0;
}

int32_t xI2cMesonRead(uint32_t addr, uint8_t offset,
		      uint8_t *buffer, uint32_t len)
{
	struct xI2cMsg msg[2], *ptr;
	uint32_t msg_count;
	int32_t ret;

	if (!len) {
		iprintf("invalid length\n");
		return -1;
	}

	ptr = msg;

	ptr->addr = addr;
	ptr->flags = 0;
	ptr->len = 1;
	ptr->buf = &offset;
	ptr++;

	ptr->addr = addr;
	ptr->flags = I2C_M_RD;
	ptr->len = len;
	ptr->buf = buffer;
	ptr++;

	msg_count = ptr - msg;
	ret = xI2cMesonXfer(msg, msg_count);
	if (ret < 0) {
		iprintf("i2c transfer read failed\n");
		return ret;
	}

	return 0;
}

int32_t xI2cMesonWrite(uint32_t addr, uint8_t offset,
		       uint8_t *buffer, uint32_t len)
{
	struct xI2cMsg msg[1];
	int32_t ret = 0;
	uint8_t buf[len + 1];

	buf[0] = offset;

	msg->addr = addr;
	msg->len = len + 1;	/* addr's length + len */
	msg->buf = buf;
	msg->flags = 0;

	memcpy(&buf[1], buffer, len);
	ret = xI2cMesonXfer(msg, 1);
	if (ret < 0) {
		iprintf("i2c transfer write failed\n");
		return ret;
	}

	return 0;
}

extern uint32_t suspend_flag;
/*
 *i2c master platform data init
 */
int32_t xI2cMesonPortInit(uint32_t id)
{
	struct xMesonI2cPlatdata *cur_plat = NULL;

	/* get the global i2c plat  data */
	meson_i2c_plat_init();
	cur_plat = &plat[id];

	i2cs[id].regs = (struct xI2cRegs *)cur_plat->reg;
	i2cs[id].div_factor = cur_plat->div_factor;
	i2cs[id].delay_adjust = cur_plat->delay_adjust;
	i2cs[id].clock_frequency = cur_plat->clock_frequency;

	if (cur_plat->clk_base)
		REG32_UPDATE_BITS(cur_plat->clk_base, BIT(cur_plat->clk_offset),
				BIT(cur_plat->clk_offset));

	if (suspend_flag) {
#if I2C_DEBUG
		iprintf("meson i2c: clkin is 24M\n");
#endif
		i2cs[id].clkin_rate = 24000000;
	}
	else {
#if I2C_DEBUG
		iprintf("meson i2c: clkin is 166M\n");
#endif
		i2cs[id].clkin_rate = cur_plat->clkin_rate;
	}
	current_id = id;

#if I2C_DEBUG
	iprintf
	    ("index = %u, reg = 0x%x, div = %u, delay = %u ,clock-frequency = %u\n",
	     cur_plat->bus_num, i2cs[id].regs, i2cs[id].div_factor,
	     i2cs[id].delay_adjust, i2cs[id].clock_frequency);
#endif
	prvMesonI2cRegsInit();

	xI2cMesonSetBusSpeed(i2cs[id].clock_frequency);	/*init i2c work speed */

	return 0;
}

void xI2cSetCurrentId(uint32_t id)
{
	taskENTER_CRITICAL();
	current_id = id;
	taskEXIT_CRITICAL();
}
