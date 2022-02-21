/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __DEV_PARAMETER_H__
#define __DEV_PARAMETER_H__

/* gpio/pinmux/pwm */
typedef struct _register_ops {
    /* register address*/
    uint32_t reg;
    /* value to set*/
    uint32_t value;
    /* bitmask of the value setting */
    uint32_t mask;
    /* for HW stable consideration */
    uint16_t udelay;
    /* setting stage and etc. */
    uint8_t flag;
    /* reserved. */
    uint8_t rsv_0;
}__attribute__ ((packed)) register_ops_t;

#define MAX_REG_OPS_ENTRIES     (32)
typedef struct pin_pwm_parameter {
    register_ops_t pin_pwm[MAX_REG_OPS_ENTRIES];
}__attribute__ ((packed)) pin_pwm_parameter_t;

typedef struct common_storage_parameter {
    /* version info of the common storage parameter */
    uint32_t version;
    /* fip sector counts */
    uint32_t device_fip_container_size;
    /* fip copies */
    uint32_t device_fip_container_copies;
    /*ddrfip size*/
    uint32_t ddr_fip_container_size;

    uint8_t reserved[16];
}__attribute__ ((packed)) common_storage_parameter_t;


typedef struct nand_parameter {
    /* version info of the common storage parameter */
    uint32_t version;
    /* the same as bbt_start_block, tell the bbt size for scanning mechanism */
    uint32_t bbt_pages;
    /* for bl2 stage, it can quickly generate the small part of the fromt bbt table. 20 for start block of bbt scanning */
    uint32_t bbt_start_block;
    /* 1: bl2 and fip is stored separately in different area 0: bl2 and fip is stored in first 1024 pages. 1 for slc nand flash. */
    uint32_t discrete_mode;
    /* set the setup_data the same as rom code reading from page0. see union cmdinfo in nand.h. */
    union {
        uint32_t nand_setup_data;
        uint32_t spi_nand_page_size;
    } setup_data;
    union {
        uint32_t nand_reserved;
        uint32_t spi_nand_planes_per_lun;
    } reserved;
    /* Block counts of the reserved area */
    uint32_t reserved_area_blk_cnt;
    /* Page number of each block */
    uint32_t page_per_block;
    /* Page list source of the bl2 NAND driver. 0: calculated in source code; 1: get from byte32~63 */
    uint8_t use_param_page_list;
    /* List of page addresses, 8-bit per entry */
    uint8_t page_list[32];
    uint8_t reserved1[63];
}__attribute__ ((packed)) nand_parameter_t;


typedef struct storage_parameter {
    /* for all the storage media */
    common_storage_parameter_t common;
    /* for NAND and SPINAND */
    nand_parameter_t nand;
}__attribute__ ((packed)) storage_parameter_t;

#endif //__AML_TIMING_H_
