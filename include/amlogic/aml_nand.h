/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __AML_NAND_H__
#define __AML_NAND_H__

int amlnf_dtb_save(u8 *buf, unsigned int len);
int amlnf_dtb_erase(void);
int get_boot_num(struct mtd_info *mtd, size_t rwsize);

/*#define RESERVED_BLOCK_NUM 48*/
#ifdef CONFIG_YAFFS2
void cmd_yaffs_devconfig(char *_mp, int flash_dev,
			int start_block, int end_block);
void cmd_yaffs_mount(char *mp);
#endif
#endif/* __AML_NAND_H__ */
