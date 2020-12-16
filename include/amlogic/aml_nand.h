/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * include/amlogic/aml_nand.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __AML_NAND_H__
#define __AML_NAND_H__

int amlnf_dtb_save(u8 *buf, unsigned int len);
int amlnf_dtb_erase(void);
int get_boot_num(struct mtd_info *mtd, size_t rwsize);

#define RESERVED_BLOCK_NUM 48

#endif/* __AML_NAND_H__ */
