
/*
 * arch/arm/include/asm/arch-txl/mailbox.h
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

 /*
  *
 * Copyright (C) 2012 Amlogic, Inc.
 *
 * Author: Platform-SH@amlogic.com
 *
 */

#ifndef __T5_MAILBOX_H_
#define __T5_MAILBOX_H_

#define MHU_PL_SIZE_SHIFT       16

#define REE2AO_SET_ADDR         (0xff63c400 + 0x4 * 0xa)//MAILBOX_SET_3
#define REE2AO_STS_ADDR         (0xff63c400 + 0x4 * 0xb)//MAILBOX_STS_3
#define REE2AO_CLR_ADDR         (0xff63c400 + 0x4 * 0xc)//MAILBOX_CLR_3
/*ree async rev addr not use, use by tee send addr*/
#define REE2AO_PL_ADDR          0xFFFC7600

/*tee to aocpu, only support sync api*/
#define MHU_SYNC                (1 << 26)
#define MHU_CMD_BUILD(command, size) \
        (((command) & 0xffff) | (((size) & 0x1ff) << 16) | MHU_SYNC)

#define MHU_PAYLOAD_SIZE        0x80
#define MHU_DATA_OFFSET         0x1c

#define MAILBOX_USER_DATA_SIZE  96

/* ...Message composition with module(6bits), function(10bits) */
#define __MBX_COMPOSE_MSG(mod, func)    (((mod) << 10) | ((func) & 0x3FF))


/*******************************************************************************
 * Define moudle type here, 6bits valid
 ******************************************************************************/
#define MBX_SYSTEM              0x0

/*******************************************************************************
 * Define function here, 10bits valid
 ******************************************************************************/
         /*SYSTEM*/
#define CMD_UNDEFINE            0x0
#define CMD_TEST                0x6

/*******************************************************************************
 * Mssage Comopsition
 ******************************************************************************/
#define MBX_CMD_TEST    __MBX_COMPOSE_MSG(MBX_SYSTEM, CMD_TEST)

enum {
        HIFIA_REE_CHANNEL = 1,
        HIFIB_REE_CHANNEL = 2,
        SECPU_REE_CHANNEL = 3,
        AOCPU_REE_CHANNEL = 4,
};

enum {
        CMD_SUSPEND = 0xff,
};

void open_scp_log(unsigned int channel);
int thermal_calibration(unsigned int type, unsigned int data);
int thermal_get_value(unsigned int sensor_id, unsigned int *value);
void send_pwm_delt(int32_t vcck_delt, int32_t ee_delt);
void init_dsp(unsigned int id,unsigned int addr,unsigned int cfg0,unsigned int jtag_ctrl,unsigned int cfg1);
void init_dsp_jtag(unsigned int id);

/*t5 only support to aocpu donot add more api, please use commom api*/
void scpi_send_data(uint32_t chan, uint32_t command, void *sendmessage,
		    uint32_t sendsize, void *revmessage, uint32_t revsize);
#endif
