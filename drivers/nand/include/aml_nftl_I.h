/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logiclib/aml_nftl_I.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __AML_NFTL_I_H__
#define __AML_NFTL_I_H__

#include "amlnf_dev.h"

int __weak add_ntd_partitions(struct amlnand_phydev* master) {
	return 0;
}

/* #warning("just for complier!!
 * and the nand phy driver work well")
 */
int __weak amlnf_logic_init(unsigned flag) {
	return 0;
}

void __weak amlnf_logic_exit(void) {}

#endif
