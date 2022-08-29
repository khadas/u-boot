/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * board/amlogic/tl1_x301_v1/lcd_tconless_spi_data.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _AML_LCD_TCONLESS_SPI_DATA_H
#define _AML_LCD_TCONLESS_SPI_DATA_H

/* ================= data block format description =================
 *block header: (64bytes)
 *    data_check:          4bytes
 *    raw_data_check:      4bytes
 *    header_size:         2bytes  (64)
 *    ext_header_size:     2bytes
 *    block_size:          4bytes
 *    block_type:          4bytes
 *    block_flag:          4bytes, (01 00 00 00, 32bits for driver flag)
 *    init_priority:       2bytes
 *    chip_id:             2bytes
 *    name:                36bytes, (for reference)

 *ext header
 *    part_cnt:               2bytes
 *    part_mapping_size:      1bytes (4)
 *    reserved:               13bytes
 *    part_0_mapping:         nbyte
 *    .....
 *    part_n_mapping:         nbyte

 *part:(normal reg write, support lut and continuous ctrl reg)
 *    part_name:              48bytes
 *    part_id:                2byte
 *    part_flag:              1byte (0=lutm 1=ctrl, bit4=1 no need fine tune)
 *    data_type:              1byte (0xd0)
 *    reg_addr_byte_width:    1byte
 *    reg_data_byte_width:    1byte
 *    reg_auto_inc:           1byte
 *    reg_cnt:                1bytes
 *    data_cnt:               4bytes
 *    reg0_start:             nbytes
 *    reg1_start:             nbytes
 *    ......
 *    regn_start:             nbytes
 *    data0:                  nbytes
 *    ......
 *    datan:                  nbytes

 *part:(control reg mask write)
 *    part_name:              48bytes
 *    part_id:                2byte
 *    part_flag:              1byte (0=lutm 1=ctrl)
 *    data_type:              1byte (0xb0)
 *    reg_addr_byte_width:    1byte
 *    reg_data_byte_width:    1byte
 *    reg_addr:               nbytes
 *    data_mask:              nbytes
 *    data_value:             nbytes

 *part: (reg check write)
 *    part_name:              48bytes
 *    part_id:                2byte
 *    part_flag:              1byte (0=lutm 1=ctrl)
 *    data_type:              1byte (0xcd)
 *    reg_chk_addr_width:     1byte
 *    reg_chk_data_wdith:     1byte
 *    reg_check_addr:         nbytes
 *    data_check_mask:        nbytes
 *    reg_wr_addr_byte_size:  1byte
 *    reg_wr_data_byte_size:  1byte
 *    reg_wr_addr:            nbytes
 *    data_wr_mask:           nbytes
 *    data_chk_cnt:           1byte
 *    data_wr_dft:            nbytes
 *    data_checked0:          nbytes
 *    ...
 *    data_checkedn:          nbytes
 *    data_wr0:               nbytes
 *    ...
 *    data_wrn:               nbytes

 *part: (reg check exit)
 *    part_name:              48bytes
 *    part_id:                2byte
 *    part_flag:              1byte (0=lutm 1=ctrl)
 *    data_type:              1byte (0xce)
 *    reg_addr_byte_width:    1byte
 *    reg_data_byte_width:    1byte
 *    reg_addr_check:         nbytes
 *    data_check_mask:        nbytes
 *    data_checked:           nbytes

 *part: (delay)
 *    part_name:              48bytes
 *    part_id:                2byte
 *    part_flag:              1byte (0=lutm 1=ctrl)
 *    data_type:              1byte (0xfd)
 *    delay_val:              4bytes(unit: us)
 */

//demura:
//64+(16+4*7)+(48+10)*5+(48+19)+(48+16+cnt)

extern void lcd_tconless_spi_data_probe(void);

#endif
