// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <linux/errno.h>
#include <linux/io.h>
#include <linux/mtd/mtd.h>
#include <amlogic/aml_rsv.h>
#include <amlogic/aml_mtd.h>
#include <partition_table.h>
#include <amlogic/storage.h>

extern int info_disprotect;
static struct meson_rsv_handler_t *rsv_handler;

static struct free_node_t *get_free_node(struct meson_rsv_info_t *rsv_info)
{
	struct meson_rsv_handler_t *handler = rsv_info->handler;
	u32 index;

	index =
		find_first_zero_bit((void *)&handler->fn_bitmask,
				    NAND_RSV_BLOCK_NUM);
	if (index >= NAND_RSV_BLOCK_NUM) {
		pr_info("%s %d index :%d is greater than max rsv block num\n",
			__func__, __LINE__, index);
		return NULL;
	}
	WARN_ON(test_and_set_bit(index, (void *)&handler->fn_bitmask));

	return handler->free_node[index];
}

static void release_free_node(struct meson_rsv_info_t *rsv_info,
			      struct free_node_t *free_node)
{
	struct meson_rsv_handler_t *handler = rsv_info->handler;
	u32 index = free_node->index;

	pr_info("%s %d: bitmask = 0x%llx\n",
		__func__, __LINE__, handler->fn_bitmask);
	if (index >= NAND_RSV_BLOCK_NUM) {
		pr_info("%s %d index :%d is greater than max rsv block num\n",
			__func__, __LINE__, index);
		return;
	}
	WARN_ON(!test_and_clear_bit(index, (void *)&handler->fn_bitmask));
	memset(free_node, 0, sizeof(struct free_node_t));
	free_node->index = index;
	pr_info("%s %d: bitmask = 0x%llx\n",
		__func__, __LINE__, handler->fn_bitmask);
}

static inline void menson_rsv_disprotect(void)
{
	/*disprotect*/
	info_disprotect |= DISPROTECT_KEY;
	info_disprotect |= DISPROTECT_FBBT;
}
static inline void menson_rsv_protect(void)
{
	/*protect*/
	info_disprotect &= ~DISPROTECT_KEY;
	info_disprotect &= ~DISPROTECT_FBBT;

}

int meson_rsv_erase_protect(struct meson_rsv_handler_t *handler,
			    u32 block_addr)
{
	if (handler->key && handler->key->valid) {
		if (!(info_disprotect & DISPROTECT_KEY) &&
		    block_addr >= handler->key->start &&
		    block_addr < handler->key->end)
			return -1;
	}
	if (handler->bbt && handler->bbt->valid) {
		if ((!(info_disprotect & DISPROTECT_FBBT)) &&
			(block_addr >= handler->bbt->start) &&
			(block_addr < handler->bbt->end))
			return -1;
	}
	return 0;
}

int meson_rsv_free(struct meson_rsv_info_t *rsv_info)
{
	struct mtd_info *mtd = rsv_info->mtd;
	struct free_node_t *tmp_node, *next_node = NULL;
	int error = 0;
	loff_t addr = 0;
	struct erase_info erase_info;

	pr_info("free %s\n", rsv_info->name);

	if (rsv_info->valid) {
		addr = rsv_info->nvalid->blk_addr;
		addr *= mtd->erasesize;
		memset(&erase_info, 0, sizeof(struct erase_info));
		erase_info.mtd = mtd;
		erase_info.addr = addr;
		erase_info.len = mtd->erasesize;
		menson_rsv_disprotect();
		error = mtd_erase(mtd, &erase_info);
		menson_rsv_protect();
		pr_info("erasing valid info block: %llx\n", addr);
		rsv_info->nvalid->blk_addr = -1;
		rsv_info->nvalid->ec = -1;
		rsv_info->nvalid->page_addr = 0;
		rsv_info->nvalid->timestamp = 0;
		rsv_info->nvalid->status = 0;
		rsv_info->valid = 0;
	}
	tmp_node = rsv_info->nfree;
	while (tmp_node) {
		next_node = tmp_node->next;
		release_free_node(rsv_info, tmp_node);
		tmp_node = next_node;
	}
	rsv_info->nfree = NULL;

	return error;
}

int meson_rsv_save(struct meson_rsv_info_t *rsv_info, u_char *buf)
{
	struct mtd_info *mtd = rsv_info->mtd;
	struct free_node_t *free_node, *temp_node;
	struct erase_info erase_info;
	int ret = 0, i = 1, pages_per_blk;
	loff_t offset = 0;

	pages_per_blk = 1 << (mtd->erasesize_shift - mtd->writesize_shift);
	if ((rsv_info->nvalid->status & POWER_ABNORMAL_FLAG) ||
	    (rsv_info->nvalid->status & ECC_ABNORMAL_FLAG))
		rsv_info->nvalid->page_addr = pages_per_blk;
	if (mtd->writesize < rsv_info->size)
		i = (rsv_info->size + mtd->writesize - 1) / mtd->writesize;
	pr_info("%s %d: %s, valid = %d, pages = %d\n", __func__, __LINE__,
		rsv_info->name, rsv_info->valid, i);
RE_SEARCH:
	if (rsv_info->valid) {
		rsv_info->nvalid->page_addr += i;
		if ((rsv_info->nvalid->page_addr + i) > pages_per_blk) {
			if ((rsv_info->nvalid->page_addr - i) ==
				pages_per_blk) {
				offset = rsv_info->nvalid->blk_addr;
				offset *= mtd->erasesize;
				erase_info.mtd = mtd;
				erase_info.addr = offset;
				erase_info.len = mtd->erasesize;
				menson_rsv_disprotect();
				mtd_erase(mtd, &erase_info);
				menson_rsv_protect();
				rsv_info->nvalid->ec++;
				pr_info("%s %d: erasing bad info block:0x%llx\n",
					__func__, __LINE__, offset);
			}
			free_node = get_free_node(rsv_info);
			if (!free_node)
				return -ENOMEM;
			/* set current valid node to free list */
			free_node->blk_addr = rsv_info->nvalid->blk_addr;
			free_node->ec = rsv_info->nvalid->ec;
			temp_node = rsv_info->nfree;
			while (temp_node->next)
				temp_node = temp_node->next;
			temp_node->next = free_node;
			/* get one node from free list and set to current */
			temp_node = rsv_info->nfree;
			rsv_info->nvalid->blk_addr = temp_node->blk_addr;
			rsv_info->nvalid->page_addr = 0;
			rsv_info->nvalid->ec = temp_node->ec;
			rsv_info->nvalid->timestamp++;
			rsv_info->nfree = temp_node->next;
			release_free_node(rsv_info, temp_node);
		}
	} else {
		temp_node = rsv_info->nfree;
		rsv_info->nvalid->blk_addr = temp_node->blk_addr;
		rsv_info->nvalid->page_addr = 0;
		rsv_info->nvalid->ec = temp_node->ec;
		rsv_info->nvalid->timestamp++;
		rsv_info->nfree = temp_node->next;
		release_free_node(rsv_info, temp_node);
	}
	offset = rsv_info->nvalid->blk_addr;
	offset *= mtd->erasesize;
	offset += ((u64)rsv_info->nvalid->page_addr) * mtd->writesize;
	if (rsv_info->nvalid->page_addr == 0) {
		ret = mtd_block_isbad(mtd, offset);
		if (ret) {
			/**
			 * cause our rsv list includes bad block,
			 * so we need check it here and for fear
			 * of data lost.
			 */
			pr_info("%s %d: %s bad block here 0x%llx\n",
				__func__, __LINE__, rsv_info->name, offset);
			rsv_info->nvalid->page_addr = pages_per_blk - i;
			goto RE_SEARCH;
		}
		memset(&erase_info, 0, sizeof(struct erase_info));
		erase_info.mtd = mtd;
		erase_info.addr = offset;
		erase_info.len = mtd->erasesize;
		menson_rsv_disprotect();
		ret = mtd_erase(mtd, &erase_info);
		menson_rsv_protect();
		if (ret) {
			pr_info("%s %d %s erase failed at 0x%llx ,mark it bad\n",
				__func__, __LINE__, rsv_info->name, offset);
			mtd_block_markbad(mtd, offset);
			//return ret;
			rsv_info->nvalid->page_addr = pages_per_blk;
			goto RE_SEARCH;
		}
		rsv_info->nvalid->ec++;
	}
	ret = meson_rsv_write(rsv_info, buf);
	if (ret) {
		pr_info("%s %d rsv info: %s save failed!\n",
			__func__, __LINE__, rsv_info->name);
		return ret;
	}
	rsv_info->valid = 1;
	rsv_info->nvalid->status = 0;
	return ret;
}

int meson_rsv_write(struct meson_rsv_info_t *rsv_info, u_char *buf)
{
	struct mtd_info *mtd = rsv_info->mtd;
	struct oobinfo_t oobinfo;
	struct mtd_oob_ops oob_ops;
	size_t length = 0;
	loff_t offset;
	int ret = 0;

	offset = rsv_info->nvalid->blk_addr;
	offset *= mtd->erasesize;
	offset += ((u64)rsv_info->nvalid->page_addr) * mtd->writesize;
	pr_info("%s %d write %s to 0x%llx\n",
		__func__, __LINE__, rsv_info->name, offset);
	memcpy(oobinfo.name, rsv_info->name, 4);
	oobinfo.ec = rsv_info->nvalid->ec;
	/* TODO: prevent the unrolling situation here */
	oobinfo.timestamp = rsv_info->nvalid->timestamp;
	while (length < rsv_info->size) {
		oob_ops.mode = MTD_OPS_AUTO_OOB;
		oob_ops.len = min_t(u32, mtd->writesize,
				    (rsv_info->size - length));
		oob_ops.ooblen = sizeof(struct oobinfo_t);
		oob_ops.ooboffs = 0;
		oob_ops.datbuf = buf + length;
		oob_ops.oobbuf = (u8 *)&oobinfo;
		ret = mtd_write_oob(mtd, offset, &oob_ops);
		if (ret) {
			pr_info("fail to write %s to 0x%llx ret:%d\n",
				rsv_info->name, offset, ret);
			return -EIO;
		}
		offset += mtd->writesize;
		length += oob_ops.len;
	}
	return ret;
}

int meson_rsv_read(struct meson_rsv_info_t *rsv_info, u_char *buf)
{
	struct mtd_info *mtd = rsv_info->mtd;
	struct oobinfo_t oobinfo;
	struct mtd_oob_ops oob_ops;
	size_t length = 0;
	loff_t offset;
	int ret = 0;

READ_RSV_AGAIN:
	offset = rsv_info->nvalid->blk_addr;
	offset *= mtd->erasesize;
	offset += ((u64)rsv_info->nvalid->page_addr) * mtd->writesize;
	pr_info("%s %d read %s from 0x%llx\n",
		__func__, __LINE__, rsv_info->name, offset);
	memset(buf, 0, rsv_info->size);
	while (length < rsv_info->size) {
		oob_ops.mode = MTD_OPS_AUTO_OOB;
		oob_ops.len = min_t(u32, mtd->writesize,
				    (rsv_info->size - length));
		oob_ops.ooblen = sizeof(struct oobinfo_t);
		oob_ops.ooboffs = 0;
		oob_ops.datbuf = buf + length;
		oob_ops.oobbuf = (u8 *)&oobinfo;

		memset((u8 *)&oobinfo, 0, oob_ops.ooblen);
		ret = mtd_read_oob(mtd, offset, &oob_ops);
		if (ret && (ret != -EUCLEAN)) {
			pr_info("blk good but read failed: %llx, %d\n",
				(u64)offset, ret);
			ret = meson_rsv_scan(rsv_info);
			if (ret)
				return -EIO;
			goto READ_RSV_AGAIN;
		}
		/* Do not use strlen ,Use ARRAY_SIZE to make the length 4 */
		if (memcmp(oobinfo.name, rsv_info->name,
			   4))
			pr_info("invalid %s info in %llx:%s\n",
				rsv_info->name, offset, oobinfo.name);
		offset += mtd->writesize;
		length += oob_ops.len;
	}
	return ret;
}

int meson_rsv_erase(struct meson_rsv_info_t *rsv_info)
{
	struct mtd_info *mtd = rsv_info->mtd;
	struct free_node_t *free_node, *temp_node = NULL;
	int ret = 0;
	struct erase_info erase_info;


	pr_info("%s %d rsv erasing %s\n",
			__func__, __LINE__, rsv_info->name);

	if (rsv_info->valid) {
		rsv_info->nvalid->ec++;
		rsv_info->nvalid->page_addr = -1;
		rsv_info->nvalid->timestamp = 1;
		rsv_info->valid = 0;

		free_node = get_free_node(rsv_info);
		if (!free_node)
			return -ENOMEM;
		/* set current valid node to free list */
		free_node->blk_addr = rsv_info->nvalid->blk_addr;
		free_node->ec = rsv_info->nvalid->ec;
		temp_node = rsv_info->nfree;
		while (temp_node->next)
			temp_node = temp_node->next;
		temp_node->next = free_node;
	}

	temp_node = rsv_info->nfree;
	while (temp_node) {
		memset(&erase_info,	0, sizeof(struct erase_info));
		erase_info.mtd = mtd;
		erase_info.addr = temp_node->blk_addr* mtd->erasesize;
		erase_info.len = mtd->erasesize;
		menson_rsv_disprotect();
		ret = mtd_erase(mtd, &erase_info);
		menson_rsv_protect();
		printk("erasing valid info block: %llx \n", erase_info.addr);
		rsv_info->nvalid->ec = -1;
		temp_node->dirty_flag = 0;
		temp_node = temp_node->next;
	}
	return ret;
}

int meson_rsv_scan(struct meson_rsv_info_t *rsv_info)
{
	struct mtd_info *mtd = rsv_info->mtd;
	struct mtd_oob_ops oob_ops;
	struct oobinfo_t oobinfo;
	struct free_node_t *free_node, *temp_node;
	loff_t offset;
	u32 start, end;
	int ret = 0, error, rsv_status, i, k;

	u8 scan_status;
	u8 good_addr[256] = {0};
	u32 page_num, pages_per_blk;

RE_RSV_INFO_EXT:
	start = rsv_info->start;
	end = rsv_info->end;
	pr_info("%s:info size = 0x%x, start blk = %d, end blk = %d\n",
		rsv_info->name, rsv_info->size, start, end);
	do {
		offset = start;
		offset *= mtd->erasesize;
		scan_status = 0;
RE_RSV_INFO:
		oob_ops.mode = MTD_OPS_AUTO_OOB;
		oob_ops.len = 0;
		oob_ops.ooblen = sizeof(struct oobinfo_t);
		oob_ops.ooboffs = 0;
		oob_ops.datbuf = NULL;
		oob_ops.oobbuf = (u8 *)&oobinfo;
		memset((u8 *)&oobinfo, 0, sizeof(struct oobinfo_t));
		error = mtd_read_oob(mtd, offset, &oob_ops);
		if (error && (error != -EUCLEAN)) {
			pr_info("%s %d blk check good but read failed: %llx, %d\n",
				__func__, __LINE__, (u64)offset, error);
			offset += rsv_info->size;
			if ((scan_status++ > 6) ||
			    (!(offset % mtd->erasesize))) {
				pr_info("ECC error, scan ONE block exit\n");
				scan_status = 0;
				continue;
			}
			goto RE_RSV_INFO;
		}
		rsv_info->init = 1;
		rsv_info->nvalid->status = 0;
		/* Do not use strlen ,Use ARRAY_SIZE to make the length 4 */
		if (!memcmp(oobinfo.name, rsv_info->name,
			    4)) {
			rsv_info->valid = 1;
			if (rsv_info->nvalid->blk_addr >= 0) {
				free_node = get_free_node(rsv_info);
				if (!free_node)
					return -ENOMEM;
				free_node->dirty_flag = 1;
				if (oobinfo.timestamp >
				    rsv_info->nvalid->timestamp) {
					free_node->blk_addr =
						rsv_info->nvalid->blk_addr;
					free_node->ec = rsv_info->nvalid->ec;
					rsv_info->nvalid->blk_addr = start;
					rsv_info->nvalid->page_addr = 0;
					rsv_info->nvalid->ec = oobinfo.ec;
					rsv_info->nvalid->timestamp =
						oobinfo.timestamp;
				} else {
					free_node->blk_addr = start;
					free_node->ec = oobinfo.ec;
				}
				if (!rsv_info->nfree) {
					rsv_info->nfree = free_node;
				} else {
					temp_node = rsv_info->nfree;
					while (temp_node->next)
						temp_node = temp_node->next;
					temp_node->next = free_node;
				}
			} else {
				rsv_info->nvalid->blk_addr = start;
				rsv_info->nvalid->page_addr = 0;
				rsv_info->nvalid->ec = oobinfo.ec;
				rsv_info->nvalid->timestamp = oobinfo.timestamp;
			}
		} else {
			free_node = get_free_node(rsv_info);
			if (!free_node)
				return -ENOMEM;
			free_node->blk_addr = start;
			free_node->ec = oobinfo.ec;
			if (!rsv_info->nfree) {
				rsv_info->nfree = free_node;
			} else {
				temp_node = rsv_info->nfree;
				while (temp_node->next)
					temp_node = temp_node->next;
				temp_node->next = free_node;
			}
		}
	} while ((++start) < end);

	pr_info("%s blk = %d, ec = %d, page = %d, timestamp = %d\n",
			rsv_info->name, rsv_info->nvalid->blk_addr, rsv_info->nvalid->ec,
			rsv_info->nvalid->page_addr, rsv_info->nvalid->timestamp);
	pr_info("%s free list:\n", rsv_info->name);
	temp_node = rsv_info->nfree;
	while (temp_node) {
		pr_info("block num = %d, ec = %d, dirty_flag = %d\n",
			temp_node->blk_addr,
			temp_node->ec,
			temp_node->dirty_flag);
		temp_node = temp_node->next;
	}
	/**
	 * step 2, find the newest in the block
	 * watch out here, cause erase size and write size must be
	 * power of 2, and write size must equal page size.
	 */
	pages_per_blk = 1 << (mtd->erasesize_shift - mtd->writesize_shift);
	page_num = rsv_info->size >> mtd->writesize_shift;
	if (!page_num)
		page_num++;
	if (rsv_info->valid == 1) {
		pr_info("%s %d selecting in block: %d\n",
			__func__, __LINE__, rsv_info->nvalid->blk_addr);
		oob_ops.mode = MTD_OPS_AUTO_OOB;
		oob_ops.len = 0;
		oob_ops.ooblen = sizeof(struct oobinfo_t);
		oob_ops.ooboffs = 0;
		oob_ops.datbuf = NULL;
		oob_ops.oobbuf = (u8 *)&oobinfo;
		for (i = 0; i < pages_per_blk; i++) {
			memset((u8 *)&oobinfo, 0, oob_ops.ooblen);
			offset = rsv_info->nvalid->blk_addr;
			offset *= mtd->erasesize;
			offset += ((u64)mtd->writesize) * i;
			error = mtd_read_oob(mtd, offset, &oob_ops);
			if (error && error != -EUCLEAN) {
				pr_info("%s %d blk good but read failed:%llx, %d\n",
					__func__, __LINE__, (u64)offset, error);
				rsv_info->nvalid->status |= ECC_ABNORMAL_FLAG;
				ret = -1;
				continue;
			}
			/* Do not use strlen ,Use ARRAY_SIZE to make the length 4 */
			if (!memcmp(oobinfo.name, rsv_info->name,
				    4)) {
				good_addr[i] = 1;
				rsv_info->nvalid->page_addr = i;
			} else {
				break;
			}
		}
	}
	if (mtd->writesize < rsv_info->size &&
	    rsv_info->valid == 1) {
		i = rsv_info->nvalid->page_addr;
		if (((i + 1) % page_num) != 0) {
			ret = -1;
			rsv_info->nvalid->status |= POWER_ABNORMAL_FLAG;
			pr_info("find %s incomplete\n", rsv_info->name);
		}
		pr_info("%s %d page_num %d\n", __func__, __LINE__, page_num);
		if (ret == -1) {
			for (i = 0; i < (pages_per_blk / page_num); i++) {
				rsv_status = 0;
				for (k = 0; k < page_num; k++) {
					if (!good_addr[k + i * page_num]) {
						rsv_status = 1;
						break;
					}
				}
				if (!rsv_status) {
					pr_info("find %d page ok\n",
						i * page_num);
					rsv_info->nvalid->page_addr =
						k + i * page_num - 1;
					ret = 0;
				}
			}
		}
		if (ret == -1) {
			rsv_info->nvalid->status = 0;
			meson_rsv_free(rsv_info);
			goto RE_RSV_INFO_EXT;
		}
		i = (rsv_info->size + mtd->writesize - 1) / mtd->writesize;
		rsv_info->nvalid->page_addr -= (i - 1);
	}
	if (rsv_info->valid != 1)
		ret = -1;
	offset = rsv_info->nvalid->blk_addr;
	offset *= mtd->erasesize;
	offset += ((u64)rsv_info->nvalid->page_addr) * mtd->writesize;
	pr_info("%s valid address 0x%llx\n", rsv_info->name, offset);
	return ret;
}

int meson_rsv_check(struct meson_rsv_info_t *rsv_info)
{
	int ret = 0;

	ret = meson_rsv_scan(rsv_info);
	if (ret)
		pr_info("%s %d %s info check failed ret %d\n",
			__func__, __LINE__, rsv_info->name, ret);
	if (!rsv_info->valid) {
		pr_info("%s %d no %s info exist\n",
			__func__, __LINE__, rsv_info->name);
		ret = 1;
	}
	return ret;
}

int meson_rsv_init(struct mtd_info *mtd,
		   struct meson_rsv_handler_t *handler)
{
	int i, ret = 0;
	u32 pages_per_blk_shift, start, vernier;
	enum boot_type_e medium_type = store_get_type();

	pages_per_blk_shift = mtd->erasesize_shift - mtd->writesize_shift;
	start = BOOT_TOTAL_PAGES >> pages_per_blk_shift;
	start += NAND_GAP_BLOCK_NUM;
	vernier = start;
	handler->fn_bitmask = 0;
	for (i = 0; i < NAND_RSV_BLOCK_NUM; i++) {
		handler->free_node[i] =
			kzalloc(sizeof(struct free_node_t), GFP_KERNEL);
		if (!handler->free_node[i]) {
			ret = -ENOMEM;
			goto error0;
		}
		memset(handler->free_node[i], 0, sizeof(struct free_node_t));
		handler->free_node[i]->index = i;
	}

	handler->bbt =
		kzalloc(sizeof(*handler->bbt), GFP_KERNEL);
	if (!handler->bbt) {
		ret = -ENOMEM;
		goto error0;
	}
	handler->bbt->nvalid =
		kzalloc(sizeof(*handler->bbt->nvalid), GFP_KERNEL);
	if (!handler->bbt->nvalid) {
		ret = -ENOMEM;
		goto error1;
	}
	handler->bbt->mtd = mtd;
	handler->bbt->start = vernier;
	handler->bbt->end = vernier + NAND_BBT_BLOCK_NUM;
	handler->bbt->nvalid->blk_addr = -1;
	handler->bbt->size = mtd->size >> mtd->erasesize_shift;
	handler->bbt->handler = handler;
	memcpy(handler->bbt->name, BBT_NAND_MAGIC, 4);
	vernier += NAND_BBT_BLOCK_NUM;
#ifndef CONFIG_ENV_IS_IN_NAND
	handler->env =
		kzalloc(sizeof(*handler->env), GFP_KERNEL);
	if (!handler->env) {
		ret = -ENOMEM;
		goto error2;
	}
	handler->env->nvalid =
		kzalloc(sizeof(*handler->env->nvalid), GFP_KERNEL);
	if (!handler->env->nvalid) {
		ret = -ENOMEM;
		goto error3;
	}
	handler->env->mtd = mtd;
	handler->env->start = vernier;
	handler->env->end = vernier + NAND_ENV_BLOCK_NUM;
	handler->env->nvalid->blk_addr = -1;
	handler->env->size = CONFIG_ENV_SIZE;
	handler->env->handler = handler;
	memcpy(handler->env->name, ENV_NAND_MAGIC, 4);
	vernier += NAND_ENV_BLOCK_NUM;
#endif
	handler->key =
		kzalloc(sizeof(*handler->key), GFP_KERNEL);
	if (!handler->key) {
		ret = -ENOMEM;
		goto error4;
	}
	handler->key->nvalid =
		kzalloc(sizeof(*handler->key->nvalid), GFP_KERNEL);
	if (!handler->key->nvalid) {
		ret = -ENOMEM;
		goto error5;
	}
	handler->key->mtd = mtd;
	handler->key->start = vernier;
	handler->key->end = vernier + NAND_KEY_BLOCK_NUM;
	handler->key->nvalid->blk_addr = -1;
	handler->key->size = 0;
	handler->key->handler = handler;
	memcpy(handler->key->name, KEY_NAND_MAGIC, 4);
	vernier += NAND_KEY_BLOCK_NUM;

	handler->dtb =
		kzalloc(sizeof(*handler->dtb), GFP_KERNEL);
	if (!handler->dtb) {
		ret = -ENOMEM;
		goto error6;
	}
	handler->dtb->nvalid =
		kzalloc(sizeof(*handler->dtb->nvalid), GFP_KERNEL);
	if (!handler->dtb->nvalid) {
		ret = -ENOMEM;
		goto error7;
	}
	handler->dtb->mtd = mtd;
	handler->dtb->start = vernier;
	handler->dtb->end = vernier + NAND_DTB_BLOCK_NUM;
	handler->dtb->nvalid->blk_addr = -1;
	handler->dtb->size = 0;
	handler->dtb->handler = handler;
	memcpy(handler->dtb->name, DTB_NAND_MAGIC, 4);
	vernier += NAND_DTB_BLOCK_NUM;

	handler->ddr_para =
		kzalloc(sizeof(*handler->ddr_para), GFP_KERNEL);
	if (!handler->ddr_para) {
		ret = -ENOMEM;
		goto error8;
	}
	handler->ddr_para->nvalid =
		kzalloc(sizeof(*handler->ddr_para->nvalid), GFP_KERNEL);
	if (!handler->ddr_para->nvalid) {
		ret = -ENOMEM;
		goto error9;
	}
	handler->ddr_para->mtd = mtd;
	handler->ddr_para->start = vernier;
	handler->ddr_para->end = vernier + NAND_DDR_BLOCK_NUM;
	handler->ddr_para->nvalid->blk_addr = -1;
	handler->ddr_para->size = DDR_PARA_SIZE;
	handler->ddr_para->handler = handler;
	memcpy(handler->ddr_para->name, DDR_NAND_MAGIC, 4);
	vernier += NAND_DDR_BLOCK_NUM;

	if (mtd->erasesize < 0x40000) {
		handler->key->size = mtd->erasesize >> 2;
		/* reduce memory usage in sram */
		handler->dtb->size = mtd->erasesize >> 1;
	} else {
		if (BOOT_SNAND == medium_type) {
			/* Reduce space use, malloc may fail */
			handler->key->size = mtd->erasesize >> 2;
			handler->dtb->size = mtd->erasesize >> 2;
		} else {
			handler->key->size = 0x40000;
			handler->dtb->size = 0x40000;
		}
	}
#if AML_RSV_KEY_SIZE
	if (mtd->erasesize >= AML_RSV_KEY_SIZE && !(AML_RSV_KEY_SIZE & 0x3ff))
		handler->key->size = AML_RSV_KEY_SIZE;
#endif//#if AML_RSV_KEY_SIZE
#if AML_RSV_DTB_SIZE
	if (mtd->erasesize >= AML_RSV_DTB_SIZE && !(AML_RSV_DTB_SIZE & 0x3ff))
		handler->dtb->size = AML_RSV_DTB_SIZE;
#endif// #if AML_RSV_DTB_SIZE

	if ((vernier - start) > NAND_RSV_BLOCK_NUM) {
		pr_info("ERROR: total blk number is over the limit\n");
		ret = -ENOMEM;
		goto error10;
	}
	rsv_handler = handler;

	pr_info("bbt_start=%d, size:0x%x\n", handler->bbt->start, handler->bbt->size);
#ifndef CONFIG_ENV_IS_IN_NAND
	pr_info("env_start=%d, size:0x%x\n", handler->env->start, handler->env->size);
#endif
	pr_info("key_start=%d, size:0x%x\n", handler->key->start, handler->key->size);
	pr_info("dtb_start=%d, size:0x%x\n", handler->dtb->start, handler->dtb->size);
	pr_info("ddr_start=%d, size:0x%x\n", handler->ddr_para->start,
		handler->ddr_para->size);

	return ret;

error10:
	kfree(handler->ddr_para->nvalid);
	handler->ddr_para->nvalid = NULL;
error9:
	kfree(handler->ddr_para);
	handler->ddr_para = NULL;
error8:
	kfree(handler->dtb->nvalid);
	handler->dtb->nvalid = NULL;
error7:
	kfree(handler->dtb);
	handler->dtb = NULL;
error6:
	kfree(handler->key->nvalid);
	handler->key->nvalid = NULL;
error5:
	kfree(handler->key);
	handler->key = NULL;
#ifndef CONFIG_ENV_IS_IN_NAND
error4:
	kfree(handler->env->nvalid);
	handler->env->nvalid = NULL;
error3:
	kfree(handler->env);
	handler->env = NULL;
#endif
error2:
	kfree(handler->bbt->nvalid);
	handler->bbt->nvalid = NULL;
error1:
	kfree(handler->bbt);
	handler->bbt = NULL;

error0:
	for (i = 0; i < NAND_RSV_BLOCK_NUM; i++) {
		kfree(handler->free_node[i]);
		handler->free_node[i] = NULL;
	}

	return ret;
}

int meson_rsv_bbt_read(u_char *dest, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->bbt) {
		pr_info("%s %d: not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}

	if (!rsv_handler->bbt->valid) {
		pr_info("%s, %d, %s invalid!, read exit!\n",
			__func__, __LINE__,
			rsv_handler->bbt->name);
		return RSV_UNVAIL;
	}
	if (!dest || size == 0) {
		pr_info("%s %d parameter error %p %ld\n",
			__func__, __LINE__, dest, size);
		return 1;
	}
	len = rsv_handler->bbt->size;
	temp = kzalloc(len, GFP_KERNEL);
	if (!temp) {
		pr_err("%s %d kzalloc fail size = %ld\n",
			__func__, __LINE__, len);
		return -ENOMEM;
	}
	memset(temp, 0, len);
	ret = meson_rsv_read(rsv_handler->bbt, temp);
	memcpy(dest, temp, len > size ? size : len);
	pr_info("%s %d read 0x%lx bytes from bbt, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	kfree(temp);
	return ret;
}

int meson_rsv_key_read(u_char *dest, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->key) {
		pr_info("%s %d: not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}

	if (!rsv_handler->key->valid) {
		pr_info("%s, %d, %s invalid!, read exit!\n",
			__func__, __LINE__,
			rsv_handler->key->name);
		return RSV_UNVAIL;
	}
	if (!dest || size == 0) {
		pr_info("%s %d parameter error %p %ld\n",
			__func__, __LINE__, dest, size);
		return 1;
	}
	len = rsv_handler->key->size;
	temp = kzalloc(len, GFP_KERNEL);
	if (!temp) {
		pr_err("%s %d kzalloc fail size = 0x%lx\n",
			__func__, __LINE__, len);
		return -ENOMEM;
	}
	memset(temp, 0, len);
	ret = meson_rsv_read(rsv_handler->key, temp);
	memcpy(dest, temp, len > size ? size : len);
	pr_info("%s %d read 0x%lx bytes from key, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	kfree(temp);
	return ret;
}

int meson_rsv_ddr_para_read(u_char *dest, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->ddr_para) {
		pr_info("%s %d: not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}

	if (!rsv_handler->ddr_para->valid) {
		pr_info("%s, %d, %s invalid!, read exit!\n",
			__func__, __LINE__,
			rsv_handler->ddr_para->name);
		return RSV_UNVAIL;
	}
	if (!dest || size == 0) {
		pr_info("%s %d parameter error %p %ld\n",
			__func__, __LINE__, dest, size);
		return 1;
	}
	len = rsv_handler->ddr_para->size;
	temp = kzalloc(len, GFP_KERNEL);
	if (!temp) {
		pr_err("%s %d kzalloc fail size = 0x%lx\n",
			__func__, __LINE__, len);
		return -ENOMEM;
	}
	memset(temp, 0, len);
	ret = meson_rsv_read(rsv_handler->ddr_para, temp);
	memcpy(dest, temp, len > size ? size : len);
	pr_info("%s %d read 0x%lx bytes from ddr_para, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	kfree(temp);
	return ret;
}

int meson_rsv_env_read(u_char *dest, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->env) {
		pr_info("%s %d: not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!rsv_handler->env->valid) {
		pr_info("%s, %d, %s invalid!, read exit!\n",
			__func__, __LINE__,
			rsv_handler->env->name);
		return RSV_UNVAIL;
	}
	if (!dest || size == 0) {
		pr_info("%s %d parameter error %p %ld\n",
			__func__, __LINE__, dest, size);
		return 1;
	}
	len = rsv_handler->env->size;
	temp = kzalloc(len, GFP_KERNEL);
	if (!temp) {
		pr_err("%s %d kzalloc fail size = 0x%lx\n",
			__func__, __LINE__, len);
		return -ENOMEM;
	}
	memset(temp, 0, len);
	ret = meson_rsv_read(rsv_handler->env, temp);
	memcpy(dest, temp, len > size ? size : len);
	pr_info("%s %d read 0x%lx bytes from env, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	kfree(temp);
	return ret;
}

int meson_rsv_dtb_read(u_char *dest, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->dtb) {
		pr_info("%s %d: rsv info not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!rsv_handler->dtb->valid) {
		pr_info("%s, %d, %s invalid!, read exit!\n",
			__func__, __LINE__,
			rsv_handler->dtb->name);
		return RSV_UNVAIL;
	}
	if (!dest || size == 0) {
		pr_info("%s %d parameter error %p %ld\n",
			__func__, __LINE__, dest, size);
		return 1;
	}
	len = rsv_handler->dtb->size;
	temp = kzalloc(len, GFP_KERNEL);
	if (!temp) {
		pr_err("%s %d kzalloc fail size = 0x%lx\n",
			__func__, __LINE__, len);
		return -ENOMEM;
	}
	memset(temp, 0, len);
	ret = meson_rsv_read(rsv_handler->dtb, temp);
	memcpy(dest, temp, len > size ? size : len);
	pr_info("%s %d read 0x%lx bytes from dtb, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	kfree(temp);
	return ret;
}

/*update bbt*/
int meson_rsv_bbt_write(u_char *source, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->bbt) {
		pr_info("%s %d rsv info not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!source || size == 0) {
		pr_info("%s %d parameter error %p %ld\n",
			__func__, __LINE__, source, size);
		return 1;
	}
	len = rsv_handler->bbt->size;
	temp = kzalloc(len, GFP_KERNEL);
	if (!temp) {
		pr_err("%s %d kzalloc fail size = 0x%lx\n",
			__func__, __LINE__, len);
		return -ENOMEM;
	}
	memset(temp, 0, len);
	memcpy(temp, source, len > size ? size : len);
	ret = meson_rsv_save(rsv_handler->bbt, temp);
	pr_info("%s %d write 0x%lx bytes to bbt, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	kfree(temp);
	return ret;
}

int meson_rsv_key_write(u_char *source, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->key) {
		pr_info("%s %d rsv info not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!source || size == 0) {
		pr_info("%s %d parameter error %p %ld\n",
			__func__, __LINE__, source, size);
		return 1;
	}
	len = rsv_handler->key->size;
	temp = kzalloc(len, GFP_KERNEL);
	if (!temp) {
		pr_err("%s %d kzalloc fail size = 0x%lx\n",
			__func__, __LINE__, len);
		return -ENOMEM;
	}
	memset(temp, 0, len);
	memcpy(temp, source, len > size ? size : len);
	ret = meson_rsv_save(rsv_handler->key, temp);
	pr_info("%s %d write 0x%lx bytes to key, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	kfree(temp);
	return ret;
}

int meson_rsv_ddr_para_write(u_char *source, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->ddr_para) {
		pr_info("%s %d rsv info not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!source || size == 0) {
		pr_info("%s %d parameter error %p %ld\n",
			__func__, __LINE__, source, size);
		return 1;
	}
	len = rsv_handler->ddr_para->size;
	temp = kzalloc(len, GFP_KERNEL);
	if (!temp) {
		pr_err("%s %d kzalloc fail size = 0x%lx\n",
			__func__, __LINE__, len);
		return -ENOMEM;
	}
	memset(temp, 0, len);
	memcpy(temp, source, len > size ? size : len);
	ret = meson_rsv_save(rsv_handler->ddr_para, temp);
	pr_info("%s %d write 0x%lx bytes to key, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	kfree(temp);
	return ret;
}


int meson_rsv_env_write(u_char *source, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->env) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!source || size == 0) {
		pr_info("%s %d parameter error %p %ld\n",
			__func__, __LINE__, source, size);
		return 1;
	}
	len = rsv_handler->env->size;
	temp = kzalloc(len, GFP_KERNEL);
	if (!temp) {
		pr_err("%s %d kzalloc fail size = 0x%lx\n",
			__func__, __LINE__, len);
		return -ENOMEM;
	}
	memset(temp, 0, len);
	memcpy(temp, source, len > size ? size : len);
	ret = meson_rsv_save(rsv_handler->env, temp);
	pr_info("%s %d write 0x%lx bytes to env, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	kfree(temp);
	return ret;
}

int meson_rsv_dtb_write(u_char *source, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->dtb) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!source || size == 0) {
		pr_info("%s %d parameter error %p %ld\n",
			__func__, __LINE__, source, size);
		return 1;
	}
	len = rsv_handler->dtb->size;
	temp = kzalloc(len, GFP_KERNEL);
	if (!temp) {
		pr_err("%s %d kzalloc fail size = 0x%lx\n",
			__func__, __LINE__, len);
		return -ENOMEM;
	}
	memset(temp, 0, len);
	memcpy(temp, source, len > size ? size : len);
	ret = meson_rsv_save(rsv_handler->dtb, temp);
	pr_info("%s %d write 0x%lx bytes to dtb, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	kfree(temp);
	return ret;
}

u32 meson_rsv_bbt_size(void)
{
	if (!rsv_handler ||
	    !rsv_handler->bbt) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 0;
	}
	return rsv_handler->bbt->size;
}

u32 meson_rsv_key_size(void)
{
	if (!rsv_handler ||
	    !rsv_handler->key) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 0;
	}
	return rsv_handler->key->size;
}

u32 meson_rsv_ddr_para_size(void)
{
	if (!rsv_handler ||
	    !rsv_handler->ddr_para) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 0;
	}
	return rsv_handler->ddr_para->size;
}


u32 meson_rsv_env_size(void)
{
	if (!rsv_handler ||
	    !rsv_handler->env) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 0;
	}
	return rsv_handler->env->size;
}

u32 meson_rsv_dtb_size(void)
{
	if (!rsv_handler ||
	    !rsv_handler->dtb) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 0;
	}
	return rsv_handler->dtb->size;
}

int meson_rsv_bbt_erase(void)
{
	if (!rsv_handler ||
	    !rsv_handler->bbt) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}

	if (rsv_handler->bbt->valid) {
		return meson_rsv_erase(rsv_handler->bbt);
	}
	return 0;
}

int meson_rsv_key_erase(void)
{
	if (!rsv_handler ||
	    !rsv_handler->key) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (rsv_handler->key->valid) {
		return meson_rsv_erase(rsv_handler->key);
	}
	return 0;
}

int meson_rsv_ddr_para_erase(void)
{
	if (!rsv_handler ||
	    !rsv_handler->ddr_para) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (rsv_handler->ddr_para->valid) {
		return meson_rsv_erase(rsv_handler->ddr_para);
	}
	return 0;
}


int meson_rsv_env_erase(void)
{
	if (!rsv_handler ||
	    !rsv_handler->env) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (rsv_handler->env->valid) {
		return meson_rsv_erase(rsv_handler->env);
	}
	return 0;
}

int meson_rsv_dtb_erase(void)
{
	if (!rsv_handler ||
	    !rsv_handler->dtb) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (rsv_handler->dtb->valid) {
		return meson_rsv_erase(rsv_handler->dtb);
	}
	return 0;

}
