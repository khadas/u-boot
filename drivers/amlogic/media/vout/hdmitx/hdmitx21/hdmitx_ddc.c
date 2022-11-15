// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <amlogic/media/vout/aml_vout.h>
#include <amlogic/media/vout/hdmitx21/hdmitx.h>
#include "hdmitx_drv.h"

#define TPI_DDC_CMD_ENHANCED_DDC_READ  0x04
#define TPI_DDC_CMD_SEQUENTIAL_READ    0x02
#define LEN_TPI_DDC_FIFO_SIZE          16

static u32 ddc_write_1byte(u8 slave, u8 offset_addr, u8 data)
{
	u32 st = 0;

	// Programe I2C operation
	//SCDC slave addr
	hdmitx21_wr_reg(DDC_ADDR_IVCTX, 0xa8);
	//SCDC slave offset
	hdmitx21_wr_reg(DDC_OFFSET_IVCTX, offset_addr & 0xff);
	//SCDC slave offset data to ddc fifo
	hdmitx21_wr_reg(DDC_DATA_AON_IVCTX, data & 0xff);
	hdmitx21_wr_reg(DDC_DIN_CNT1_IVCTX, 0x01); //data length lo
	hdmitx21_wr_reg(DDC_DIN_CNT2_IVCTX, 0x00); //data length hi
	hdmitx21_wr_reg(DDC_CMD_IVCTX, 0x06); //DDC Write CMD
	// Wait until I2C done
	hdmitx21_poll_reg(DDC_STATUS_IVCTX, 1 << 4, ~(1 << 4), 0xffffffff); //i2c process
	hdmitx21_poll_reg(DDC_STATUS_IVCTX, 0 << 4, ~(1 << 4), 0xffffffff); //i2c done

	return st;
}

static u32 ddc_read_1byte(u8 slave, u8 offset_addr, u8 *rd_data)
{
	u32 st = 0;

	// Programe I2C operation
	hdmitx21_wr_reg(DDC_CMD_IVCTX, 0x09); //clear fifo
	hdmitx21_wr_reg(DDC_ADDR_IVCTX, 0xa8); //SCDC slave addr
	hdmitx21_wr_reg(DDC_OFFSET_IVCTX, offset_addr & 0xff); //SCDC slave offset
	hdmitx21_wr_reg(DDC_DIN_CNT1_IVCTX, 0x01); //data length lo
	hdmitx21_wr_reg(DDC_DIN_CNT2_IVCTX, 0x00); //data length hi
	hdmitx21_wr_reg(DDC_CMD_IVCTX, 0x02); //DDC Write CMD
	// Wait until I2C done
	hdmitx21_poll_reg(DDC_STATUS_IVCTX, 1 << 4, ~(1 << 4), 0xffffffff); //i2c process
	hdmitx21_poll_reg(DDC_STATUS_IVCTX, 0 << 4, ~(1 << 4), 0xffffffff); //i2c done
	// Read back 1 byte
	*rd_data = hdmitx21_rd_reg(DDC_DATA_AON_IVCTX);

	return st;
}

void scdc21_rd_sink(u8 adr, u8 *val)
{
	ddc_read_1byte(DDC_SCDC_ADDR, adr, val);
}

void scdc21_wr_sink(u8 adr, u8 val)
{
	ddc_write_1byte(DDC_SCDC_ADDR, adr, val);
}

/*
 * Note: read 8 Bytes of EDID data every time
 */
int hdmitx21_read_edid(u8 *_rx_edid)
{
	u32 i;
	u32 byte_num = 0;
	u8 edid_extension = 1;
	u8 *rx_edid = _rx_edid;
	struct hdmitx_dev *hdev = get_hdmitx21_device();

	if (hdev->pxp_mode)
		return 0; /* skip edid reading in pxp */

	// Program SLAVE/SEGMENT/ADDR
	hdmitx21_wr_reg(LM_DDC_IVCTX, 0x80); //sel edid
	hdmitx21_wr_reg(DDC_CMD_IVCTX, 0x09); //clear fifo
	hdmitx21_wr_reg(DDC_ADDR_IVCTX, DDC_EDID_ADDR & BIT_DDC_ADDR_REG); //edid slave addr

	// Read complete EDID data sequentially
	while (byte_num < (128 * (1 + edid_extension))) {
		if ((byte_num % 256) == 0)
			hdmitx21_wr_reg(DDC_SEGM_IVCTX, byte_num >> 8); //segment
		hdmitx21_wr_reg(DDC_OFFSET_IVCTX, byte_num & 0xff); //offset
		hdmitx21_wr_reg(DDC_DIN_CNT1_IVCTX, 1 << 3); //data length lo
		hdmitx21_wr_reg(DDC_DIN_CNT2_IVCTX, 0x00); //data length hi
		hdmitx21_wr_reg(DDC_CMD_IVCTX, 0x04); //CMD
		// Wait until I2C done
		hdmitx21_poll_reg(DDC_STATUS_IVCTX, 1 << 4, ~(1 << 4), HZ / 100); //i2c process
		hdmitx21_poll_reg(DDC_STATUS_IVCTX, 0 << 4, ~(1 << 4), HZ / 100); //i2c done
		// Read back 8 bytes
		for (i = 0; i < 8; i++) {
			if (byte_num == 126) {
				edid_extension  = hdmitx21_rd_reg(DDC_DATA_AON_IVCTX);
				rx_edid[byte_num] = edid_extension;
				if (edid_extension > 3)
					edid_extension = 3;
			} else {
				rx_edid[byte_num] = hdmitx21_rd_reg(DDC_DATA_AON_IVCTX);
			}
			byte_num++;
		}
		if (byte_num > 127 && byte_num < 256)
			if (rx_edid[128 + 4] == 0xe2 && rx_edid[128 + 5] == 0x78)
				edid_extension = rx_edid[128 + 6];
		if (edid_extension > 7) {
			pr_info("edid extension block number:");
			pr_info(" %d, reset to MAX 7\n", edid_extension);
			edid_extension = 7; /* Max extended block */
		}
	}
	return 1;
} /* hdmitx21_read_edid */
