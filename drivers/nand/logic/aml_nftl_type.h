/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_nftl_type.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __AML_NFTL_TYPE_H__
#define __AML_NFTL_TYPE_H__

#define	FACTORY_BAD_BLOCK_ERROR  	              2
#define BYTES_PER_SECTOR                          512
#define SHIFT_PER_SECTOR                          9
#define BYTES_OF_USER_PER_PAGE                    16
#define MIN_BYTES_OF_USER_PER_PAGE                16

#ifndef AML_NAND_UBOOT
typedef unsigned char         uchar;
#endif

typedef struct{
    unsigned short  Page_NO;
    unsigned short  blkNO_in_chip;
    unsigned char   page_status;
}_nand_page;

typedef struct{
    _nand_page    phy_page;
    unsigned short        page_bitmap;
    unsigned char*        main_data_addr;
    unsigned char*        spare_data_addr;
    int		op_ret_sta;
}_physic_op_par;

#endif /* __AML_NFTL_TYPE_H__ */

