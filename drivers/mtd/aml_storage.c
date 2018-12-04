// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * mtd adaptive to storage interface module
 *
 * Copyright (C) 2018 Amlogic Corporation
 *
 * Licensed under the GPL-2 or later.
 *
 */

#include <common.h>
#include <malloc.h>
#include <nand.h>
#include <linux/errno.h>
#include <linux/sizes.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
#include <amlogic/storage.h>
#include <amlogic/aml_mtd.h>
#include <amlogic/aml_rsv.h>
#include <asm/arch/cpu_config.h>
#include <partition_table.h>
#include <time.h>

#undef pr_info
#define pr_info	printf

struct map_handler_t {
	u16 *map;
	u16 valid_max;
	u8 init_flag;
};

static struct map_handler_t mtd_map;
static struct mtd_info *mtd_store_list[MAX_MTD_CNT];
extern int info_disprotect;

static inline struct mtd_info *mtd_store_get(int dev)
{
#if defined(CONFIG_SPI_FLASH) || defined(CONFIG_SPI_NAND)
	return mtd_store_list[0];
#else
	return mtd_store_list[dev];
#endif
}

#ifndef CONFIG_DISCRETE_BOOTLOADER
static u8 boot_num_get(void)
{
	/* TODO get from page0, if page0 0xff, return 1 */
	return 1;
}
#endif

static struct mtd_info *mtd_store_get_by_name(const char *part_name,
					      int boot)
{
	if (!strcmp(part_name, BOOT_LOADER) ||
	    !strcmp(part_name, BOOT_BL2) ||
	    !strcmp(part_name, BOOT_SPL)) {
		return mtd_store_get(0);
	} else if (!strcmp(part_name, BOOT_TPL) ||
			   !strcmp(part_name, BOOT_FIP)) {
		return mtd_store_get(1);
	} else if (boot) {
		pr_info("%s %d invalid name: %s\n",
			__func__, __LINE__, part_name);
		return ERR_PTR(-ENXIO);
	}
	return mtd_store_get(1);
}

void mtd_store_set(struct mtd_info *mtd, int dev)
{
#if defined(CONFIG_SPI_FLASH) || defined(CONFIG_SPI_NAND)
	mtd_store_list[0] = mtd;
#else
	mtd_store_list[dev] = mtd;
#endif
}

#ifdef CONFIG_CMD_NAND_TRIMFFS
static size_t drop_ffs(const struct mtd_info *mtd,
		       const u_char *buf, const size_t *len)
{
	size_t l = *len;
	ssize_t i;

	for (i = l - 1; i >= 0; i--)
		if (buf[i] != 0xFF)
			break;

	/* The resulting length must be aligned to the minimum flash I/O size */
	l = i + 1;
	l = (l + mtd->writesize - 1) / mtd->writesize;
	l *=  mtd->writesize;

	/*
	 * since the input length may be unaligned, prevent access past the end
	 * of the buffer
	 */
	return min(l, *len);
}
#endif

static int check_skip_len(struct mtd_info *mtd,
			  loff_t offset, size_t length, size_t *used)
{
	size_t len_excl_bad = 0;
	int ret = 0;

	while (len_excl_bad < length) {
		size_t block_len, block_off;
		loff_t block_start;

		if (offset >= mtd->size)
			return -1;

		block_start = offset & ~(loff_t)(mtd->erasesize - 1);
		block_off = offset & (mtd->erasesize - 1);
		block_len = mtd->erasesize - block_off;

		if (!mtd_block_isbad(mtd, block_start))
			len_excl_bad += block_len;
		else
			ret = 1;

		offset += block_len;
		*used += block_len;
	}

	/* If the length is not a multiple of block_len, adjust. */
	if (len_excl_bad > length)
		*used -= (len_excl_bad - length);

	return ret;
}

static int mtd_store_read_skip_bad(struct mtd_info *mtd,
				   loff_t offset, size_t *length,
				   size_t *actual, loff_t lim,
				   u_char *buffer)
{
	int rval;
	size_t left_to_read = *length;
	size_t used_for_read = 0;
	u_char *p_buffer = buffer;
	int need_skip;

	if ((offset & (mtd->writesize - 1)) != 0) {
		pr_info("Attempt to read non page-aligned data\n");
		*length = 0;
		if (actual)
			*actual = 0;
		return -EINVAL;
	}

	need_skip = check_skip_len(mtd, offset, *length, &used_for_read);

	if (actual)
		*actual = used_for_read;

	if (need_skip < 0) {
		pr_info("Attempt to read outside the flash area\n");
		*length = 0;
		return -EINVAL;
	}

	if (used_for_read > lim) {
		puts("Size of read exceeds partition or device limit\n");
		*length = 0;
		return -EFBIG;
	}

	if (!need_skip) {
		rval = mtd_read(mtd, offset, *length, length, buffer);
		if (!rval || rval == -EUCLEAN)
			return 0;

		*length = 0;
		pr_info("%s %d read from offset %llx failed %d\n",
			__func__, __LINE__, offset, rval);
		return rval;
	}

	while (left_to_read > 0) {
		size_t block_offset = offset & (mtd->erasesize - 1);
		size_t read_length;

		WATCHDOG_RESET();

		if (mtd_block_isbad(mtd, offset & ~(mtd->erasesize - 1))) {
			pr_info("Skipping bad block 0x%08llx\n",
				offset & ~(mtd->erasesize - 1));
			offset += mtd->erasesize - block_offset;
			continue;
		}

		if (left_to_read < (mtd->erasesize - block_offset))
			read_length = left_to_read;
		else
			read_length = mtd->erasesize - block_offset;

		rval = mtd_read(mtd, offset,
				read_length, &read_length, p_buffer);
		if (rval && rval != -EUCLEAN) {
			pr_info("%s %d read from offset %llx failed %d\n",
				__func__, __LINE__, offset, rval);
			*length -= left_to_read;
			return rval;
		}

		left_to_read -= read_length;
		offset       += read_length;
		p_buffer     += read_length;
	}

	return 0;
}

int mtd_store_write_skip_bad(struct mtd_info *mtd,
			     loff_t offset, size_t *length,
			     size_t *actual, loff_t lim,
			     u_char *buffer, int flags)
{
	int rval = 0, blocksize;
	size_t left_to_write = *length;
	size_t used_for_write = 0;
	u_char *p_buffer = buffer;
	int need_skip;

	if (actual)
		*actual = 0;

#ifdef CONFIG_CMD_NAND_YAFFS
	if (flags & WITH_YAFFS_OOB) {
		if (flags & ~WITH_YAFFS_OOB)
			return -EINVAL;

		int pages;

		pages = mtd->erasesize / mtd->writesize;
		blocksize = (pages * mtd->oobsize) + mtd->erasesize;
		if (*length % (mtd->writesize + mtd->oobsize)) {
			pr_info("Attempt to write incomplete page");
			pr_info(" in yaffs mode\n");
			return -EINVAL;
		}
	} else
#endif
	{
		blocksize = mtd->erasesize;
	}

	/*
	 * nand_write() handles unaligned, partial page writes.
	 *
	 * We allow length to be unaligned, for convenience in
	 * using the $filesize variable.
	 *
	 * However, starting at an unaligned offset makes the
	 * semantics of bad block skipping ambiguous (really,
	 * you should only start a block skipping access at a
	 * partition boundary).  So don't try to handle that.
	 */
	if ((offset & (mtd->writesize - 1)) != 0) {
		pr_info("Attempt to write non page-aligned data\n");
		*length = 0;
		return -EINVAL;
	}

	need_skip = check_skip_len(mtd, offset, *length, &used_for_write);

	if (actual)
		*actual = used_for_write;

	if (need_skip < 0) {
		pr_info("Attempt to write outside the flash area\n");
		*length = 0;
		return -EINVAL;
	}

	if (used_for_write > lim) {
		puts("Size of write exceeds partition or device limit\n");
		*length = 0;
		return -EFBIG;
	}

	if (!need_skip && !(flags & WITH_DROP_FFS)) {
		rval = mtd_write(mtd, offset, *length, length, buffer);
		if (rval == 0)
			return 0;

		*length = 0;
		pr_info("%s %d write to offset %llx failed %d\n",
			__func__, __LINE__, offset, rval);
		return rval;
	}

	while (left_to_write > 0) {
		size_t block_offset = offset & (mtd->erasesize - 1);
		size_t write_size, truncated_write_size;

		WATCHDOG_RESET();

		if (mtd_block_isbad(mtd, offset & ~(mtd->erasesize - 1))) {
			pr_info("Skip bad block 0x%08llx\n",
				offset & ~(mtd->erasesize - 1));
			offset += mtd->erasesize - block_offset;
			continue;
		}

		if (left_to_write < (blocksize - block_offset))
			write_size = left_to_write;
		else
			write_size = blocksize - block_offset;

#ifdef CONFIG_CMD_NAND_YAFFS
		if (flags & WITH_YAFFS_OOB) {
			int page, pages;
			size_t pagesize = mtd->writesize;
			size_t pagesize_oob = pagesize + mtd->oobsize;
			struct mtd_oob_ops ops;

			ops.len = pagesize;
			ops.ooblen = mtd->oobsize;
			ops.mode = MTD_OPS_AUTO_OOB;
			ops.ooboffs = 0;

			pages = write_size / pagesize_oob;
			for (page = 0; page < pages; page++) {
				WATCHDOG_RESET();

				ops.datbuf = p_buffer;
				ops.oobbuf = ops.datbuf + pagesize;

				rval = mtd_write_oob(mtd, offset, &ops);
				if (rval != 0)
					break;

				offset += pagesize;
				p_buffer += pagesize_oob;
			}
		} else
#endif
		{
			truncated_write_size = write_size;
#ifdef CONFIG_CMD_NAND_TRIMFFS
			if (flags & WITH_DROP_FFS)
				truncated_write_size = drop_ffs(mtd, p_buffer,
								&write_size);
#endif

			rval = mtd_write(mtd, offset, truncated_write_size,
					 &truncated_write_size, p_buffer);
			offset += write_size;
			p_buffer += write_size;
		}

		if (rval != 0) {
			pr_info("%s %d write to offset %llx failed %d\n",
				__func__, __LINE__, offset, rval);
			*length -= left_to_write;
			return rval;
		}

		left_to_write -= write_size;
	}

	return 0;
}

static loff_t mtd_store_ltop(loff_t off)
{
	struct mtd_info *mtd = mtd_store_get(1);
	loff_t remainder = off % mtd->erasesize;
	u16 logic_blk = off / mtd->erasesize;

	if (!mtd_map.init_flag) {
		pr_info("%s %d error: no valid map handler\n",
			__func__, __LINE__);
		return (loff_t)-1;
	}
	if (logic_blk > mtd_map.valid_max) {
		pr_info("%s %d error: over valid address\n",
			__func__, __LINE__);
		return (loff_t)-1;
	}
	return (mtd_map.map[logic_blk] * mtd->erasesize) + remainder;
}

static size_t mtd_store_logic_part_size(struct mtd_info *mtd,
					struct part_info *part)
{
	loff_t start, end;
	u32 cnt = 0;

	start = part->offset / mtd->erasesize;
	end = (part->offset + part->size) / mtd->erasesize;

	while (start++ < end)
		if (mtd_block_isbad(mtd, start))
			cnt++;
	return part->size - cnt * mtd->erasesize;
}

void mtd_store_init_map(void)
{
	struct mtd_info *mtd = mtd_store_get(1);
	u16 blk_cnt = 0, skip = 0, page_per_blk = 0;
	int i, j = 0;

	if (mtd->erasesize)
		blk_cnt = mtd->size / mtd->erasesize;
	else
		pr_info("%s %d can not calculate block cnt\n",
			__func__, __LINE__);

	printf("%s %d block cnt: 0x%x\n",
			   __func__, __LINE__, blk_cnt);
	page_per_blk = mtd->erasesize / mtd->writesize;
	skip = BOOT_TOTAL_PAGES / page_per_blk;
	skip += NAND_RSV_BLOCK_NUM;
#ifdef CONFIG_DISCRETE_BOOTLOADER
	skip += ((CONFIG_TPL_SIZE_PER_COPY * CONFIG_TPL_COPY_NUM +
			mtd->erasesize - 1) / mtd->erasesize);
#endif
	mtd_map.map = (u16 *)malloc(sizeof(u16) * blk_cnt);
	for (i = 0; i < blk_cnt; i++) {
		if (mtd_block_isbad(mtd, i * mtd->erasesize) &&
		    i >= skip)
			continue;
		else
			mtd_map.map[j++] = i;
	}
	mtd_map.valid_max = j;
	mtd_map.init_flag = 1;
	pr_info("%s %d: skipped %d, bad block need re-map: %d\n",
		__func__, __LINE__, skip, blk_cnt - j);
}

static int mtd_store_get_offset(const char *partname,
				loff_t *retoff, loff_t off)
{
	int ret = 0;
	loff_t offset;
#if defined(CONFIG_CMD_MTDPARTS)
	u8 pnum;
	struct mtd_device *dev;
	struct part_info *part;
#endif
	*retoff = 0;
	if (!partname) {
		offset = off;
	}
#if defined(CONFIG_CMD_MTDPARTS)
	else if (!mtdparts_init()) {
#ifdef CONFIG_DISCRETE_BOOTLOADER
		if (!strcmp(partname, BOOT_LOADER) ||
		    !strcmp(partname, BOOT_BL2) ||
		    !strcmp(partname, BOOT_SPL)) {
			ret = find_dev_and_part(BOOT_LOADER,
					  &dev,
					  &pnum,
					  &part);
			if (ret)
				return ret; /* avoid operates null pointer */
			*retoff = part->offset + off;
			return 0;
		}
		if (!strcmp(partname, BOOT_TPL) ||
		    !strcmp(partname, BOOT_FIP)) {
			ret = find_dev_and_part(BOOT_TPL,
					  &dev,
					  &pnum,
					  &part);
			if (ret)
				return ret;
			*retoff = part->offset + off;
			return 0;
		}
#endif
		ret = find_dev_and_part(partname,
					&dev,
					&pnum,
					&part);
		if (ret) {
			pr_info("%s %d can not find part:%s",
				__func__, __LINE__, partname);
			ret = -EINVAL;
		}
		offset = part->offset + off;
	}
#endif
	else {
		pr_info("%s %d no mtd partition, offset to device 0 address\n",
			__func__, __LINE__);
		offset = off;
	}
	if (!ret)
		*retoff = offset;
	return ret;
}

static u64 mtd_store_size(const char *part_name)
{
	struct mtd_info *mtd = mtd_store_get(1);

	if (!part_name)
		return mtd->size;
#ifndef CONFIG_CMD_MTDPARTS
	if (!strcmp(part_name, BOOT_LOADER))
		mtd = mtd_store_get(0);
	else
		pr_info("no partition, whole device size\n");
	return mtd->size;
#else
	u8 pnum;
	struct mtd_device *dev;
	struct part_info *part;
	int ret;

	if (!mtdparts_init()) {
		ret = find_dev_and_part(part_name,
					&dev,
					&pnum,
					&part);
		if (ret) {
			pr_info("%s %d can not find part:%s",
				__func__, __LINE__, part_name);
			return 0;
		}
		return mtd_store_logic_part_size(mtd, part);
	}
#endif
	return 0;
}

static int mtd_store_read(const char *part_name,
			  loff_t off, size_t size, void *dest)
{
	struct mtd_info *mtd = mtd_store_get(1);
	loff_t offset = 0;
	size_t retlen = 0;
	int ret;

	ret = mtd_store_get_offset((const char *)part_name, &offset, off);
	if (ret)
		return ret;
	if (!part_name) {/*normal area except tpl*/
		offset = off;
		off += BOOT_TOTAL_PAGES * mtd->writesize;
		off += NAND_RSV_BLOCK_NUM * mtd->erasesize;
#ifdef CONFIG_DISCRETE_BOOTLOADER
		off += CONFIG_TPL_SIZE_PER_COPY * CONFIG_TPL_COPY_NUM;
#endif
	}

	offset = mtd_store_ltop(offset);

	ret = mtd_store_read_skip_bad(mtd, offset, &size,
				      &retlen, mtd->size, (u_char *)dest);
	if (ret) {
		pr_info("%s %d mtd read err, ret %d\n",
			__func__, __LINE__, ret);
		return ret;
	}
	if (retlen != size) {
		ret = -EIO;
		pr_info("%s %d mtd read err, size(%ld), retlen(%ld)\n",
			__func__, __LINE__, size, retlen);
		return ret;
	}
	return ret;
}

static int mtd_store_write(const char *part_name,
			   loff_t off, size_t size, void *source)
{
	struct mtd_info *mtd = mtd_store_get(1);
	loff_t offset = 0;
	size_t retlen = 0;
	int ret;

	ret = mtd_store_get_offset((const char *)part_name, &offset, off);
	if (ret)
		return ret;
	if (!part_name) {/*normal area except tpl*/
		offset = off;
		off += BOOT_TOTAL_PAGES * mtd->writesize;
		off += NAND_RSV_BLOCK_NUM * mtd->erasesize;
#ifdef CONFIG_DISCRETE_BOOTLOADER
		off += CONFIG_TPL_SIZE_PER_COPY * CONFIG_TPL_COPY_NUM;
#endif
	}
	offset = mtd_store_ltop(offset);

	ret = mtd_store_write_skip_bad(mtd, offset, &size,
				       &retlen, mtd->size, (u_char *)source, 0);
	if (ret) {
		pr_info("%s %d mtd write err, ret %d\n",
			__func__, __LINE__, ret);
		return ret;
	}
	if (retlen != size) {
		ret = -EIO;
		pr_info("%s %d mtd write err, size(%ld), retlen(%ld)\n",
			__func__, __LINE__, size, retlen);
		return ret;
	}
	return ret;
}

static int mtd_store_erase(const char *part_name,
			   loff_t off, size_t size, int scrub_flag)
{
	struct mtd_info *mtd;
	loff_t offset = 0;
	unsigned long erased_size, erase_len;
	struct erase_info info;
	int ret;

	if (!part_name)	{
		mtd = mtd_store_get(1);
		printf("!!!warn: erase all chip\n");
		size = mtd->size;
		mtd = mtd_store_get(0);
	}
	else
		mtd = mtd_store_get_by_name(part_name, 0);
	if (IS_ERR(mtd))
		return -ENXIO;

	ret = mtd_store_get_offset((const char *)part_name, &offset, off);
	if (ret)
		return ret;
	offset = mtd_store_ltop(offset);
	erase_len = lldiv(size + mtd->erasesize - 1,
			  mtd->erasesize);

	printf("erasing from 0x%llx, length 0x%lx\n",
		   offset, size);

	for (erased_size = 0; erased_size < erase_len;
		 offset += mtd->erasesize) {
		if (!part_name)/*erase chip,erase_len include bb*/
			erased_size++;
		WATCHDOG_RESET();
		if (!scrub_flag) {
			ret = mtd_block_isbad(mtd, offset);
			if (ret > 0) {
				pr_info("skip bad block in 0x%08llx\n", offset);
				continue;
			} else if (ret < 0) {
				pr_info("MTD get bad block failed in 0x%08llx\n",
					offset);
				return ret;
			}
		}
		info.mtd = mtd;
		info.addr = offset;
		info.len = mtd->erasesize;
		info.scrub = scrub_flag;
		info.callback = NULL;
		if (part_name) /*erase partition,erase_len except bb*/
			erased_size++;

		loff_t bootloader_max_addr = BOOT_TOTAL_PAGES * mtd->writesize;
		if (offset >= bootloader_max_addr) {
			mtd = mtd_store_get(1);
		}

		ret = mtd_erase(mtd, &info);
		if (ret)
			pr_info("%s %d mtd erase err, ret %d\n",
				__func__, __LINE__, ret);
	}
	return ret;
}

static u8 mtd_store_boot_copy_num(const char *part_name)
{
	if (!part_name) {
		pr_info("%s %d invalid name!\n",
			__func__, __LINE__);
		return 0;
	}
#ifdef CONFIG_DISCRETE_BOOTLOADER
	if (!strcmp(part_name, BOOT_LOADER) ||
	    !strcmp(part_name, BOOT_BL2) ||
	    !strcmp(part_name, BOOT_SPL))
		return CONFIG_BL2_COPY_NUM;

	if (!strcmp(part_name, BOOT_TPL) ||
	    !strcmp(part_name, BOOT_FIP))
		return CONFIG_TPL_COPY_NUM;
#else
	if (!strcmp(part_name, BOOT_LOADER))
		return boot_num_get();
#endif

	pr_info("%s %d invalid name: %s!\n",
		__func__, __LINE__, part_name);
	return 0;
}

int is_mtd_store_boot_area(const char *part_name)
{
	if (!part_name) {
		pr_info("%s %d invalid name!\n",
			__func__, __LINE__);
		return 0;
	}

#ifdef CONFIG_DISCRETE_BOOTLOADER
	if (!strcmp(part_name, BOOT_LOADER) ||
	    !strcmp(part_name, BOOT_BL2) ||
	    !strcmp(part_name, BOOT_SPL))
		return 1;

	if (!strcmp(part_name, BOOT_TPL) ||
	    !strcmp(part_name, BOOT_FIP))
		return 1;
#else
	if (!strcmp(part_name, BOOT_LOADER))
		return 1;
#endif
	return 0;
}

static u64 mtd_store_boot_copy_size(const char *part_name)
{
	struct mtd_info *mtd = mtd_store_get(0);
	int pages_per_copy = 0;

	if (!part_name) {
		pr_info("%s %d invalid name!\n",
			__func__, __LINE__);
		return 0;
	}
#ifdef CONFIG_DISCRETE_BOOTLOADER
	if (!strcmp(part_name, BOOT_LOADER) ||
	    !strcmp(part_name, BOOT_BL2) ||
	    !strcmp(part_name, BOOT_SPL)) {
		pages_per_copy = BOOT_TOTAL_PAGES / CONFIG_BL2_COPY_NUM;
		return mtd->writesize * pages_per_copy;
	} else if (!strcmp(part_name, BOOT_TPL) ||
			   !strcmp(part_name, BOOT_FIP)) {
		return CONFIG_TPL_SIZE_PER_COPY;
	}
	return 0;
#else
	u8 num;

	if (strcmp(part_name, BOOT_LOADER))
		return 0;
	num = mtd_store_boot_copy_num(part_name);
	if (!num)
		return 0;
	pages_per_copy = BOOT_TOTAL_PAGES / num;
	return mtd->writesize * pages_per_copy;
#endif
}

static int mtd_store_boot_read(const char *part_name,
			       u8 cpy, size_t size, void *dest)
{
	struct mtd_info *mtd;
	loff_t offset, limit;
	int ret = 1;
	size_t retlen = 0, len = size;
	u8 num = 0;
	u64 size_per_copy = 0;

	if (!part_name) {
		pr_info("%s %d invalid name!\n",
			__func__, __LINE__);
		return -ENXIO;
	}
	mtd = mtd_store_get_by_name(part_name, 1);
	if (IS_ERR(mtd))
		return -ENXIO;
	ret = mtd_store_get_offset(part_name, &offset, 0);
	if (ret)
		return -ENXIO;
	num = mtd_store_boot_copy_num(part_name);
	if (num == 0)
		return -ENXIO;
	size_per_copy = mtd_store_boot_copy_size(part_name);
	if (size_per_copy == 0)
		return -ENXIO;
	if (cpy >= num)
		return -ENXIO;
	offset += (cpy * size_per_copy);
	limit = offset + size_per_copy;
#if defined(CONFIG_SPI_NAND)
	/**
	 * TODO:
	 * Need delete this part of code when we fix the
	 * romcode read size limit bug and afunction of
	 * bad block skipping.
	 */
	if (!strcmp(part_name, BOOT_LOADER) ||
	    !strcmp(part_name, BOOT_BL2) ||
	    !strcmp(part_name, BOOT_SPL)) {
		int i, read_cnt;
		loff_t off = offset;
		size_t sz_read = SZ_2K;
		size_t length = (size > BL2_SIZE) ? BL2_SIZE : size;

		read_cnt = (length + sz_read - 1) / sz_read;
		for (i = 0; i < read_cnt; i++) {
			len = min(sz_read, (length - i * sz_read));
			ret = mtd_store_read_skip_bad(mtd,
						      off,
						      &len,
						      &retlen,
						      limit,
						(u_char *)(dest + i * sz_read));
			if (ret)
				return -EIO;
			off += mtd->writesize;
			if (retlen > len)
				off += (retlen - len);
		}
#ifndef CONFIG_DISCRETE_BOOTLOADER
		if (size <= BL2_SIZE)
			return ret;
		ret = mtd_store_read_skip_bad(mtd,
					      off,
					      &(size - BL2_SIZE),
					      &retlen,
					      limit,
					      (u_char *)(dest + BL2_SIZE));
		if (ret)
			return -EIO;
#endif
		return ret;
	}
#endif
	ret = mtd_store_read_skip_bad(mtd,
				      offset,
				      &len,
				      &retlen,
				      limit,
				      (u_char *)dest);
	if (ret)
		return -EIO;
	return ret;
}

static int mtd_store_boot_write(const char *part_name,
				u8 cpy, size_t size, void *source)
{
	struct mtd_info *mtd;
	loff_t offset, limit, endoff = 0;
	int ret = 1;
	size_t retlen = 0, len = size;
	u8 num = 0;
	u64 size_per_copy = 0;

	if (!part_name) {
		pr_info("%s %d invalid name!\n",
			__func__, __LINE__);
		return -ENXIO;
	}
	mtd = mtd_store_get_by_name(part_name, 1);
	if (IS_ERR(mtd))
		return -ENXIO;
	ret = mtd_store_get_offset(part_name, &offset, 0);
	if (ret)
		return -ENXIO;
	num = mtd_store_boot_copy_num(part_name);
	if (num == 0)
		return -ENXIO;
	size_per_copy = mtd_store_boot_copy_size(part_name);
	if (size_per_copy == 0)
		return -ENXIO;
	if (cpy != BOOT_OPS_ALL) {
		if (cpy >= num)
			return -ENXIO;
		offset += (cpy * size_per_copy);
		endoff = offset + size_per_copy;
	} else {
		endoff = offset + num * size_per_copy;
	}
	for (; offset < endoff; offset += size_per_copy) {
		pr_info("write %lx bytes to %llx\n",
			size, offset);
		limit = offset + size_per_copy;
#if defined(CONFIG_SPI_NAND)
		/**
		 * TODO:
		 * 1.Need delete this part of code when we fix the
		 * romcode read size limit bug and afunction of
		 * bad block skipping.
		 * 2.Need handle info page position there, not in
		 * SPI NAND drvier, which we can not know the bad
		 * block skiped or not.
		 */
		if (!strcmp(part_name, BOOT_LOADER) ||
		    !strcmp(part_name, BOOT_BL2) ||
		    !strcmp(part_name, BOOT_SPL)) {
			int i, write_cnt;
			loff_t off = offset;
			size_t sz_write = SZ_2K;
			size_t length = (size > BL2_SIZE) ? BL2_SIZE : size;

			write_cnt = (length + sz_write - 1) / sz_write;
			for (i = 0; i < write_cnt; i++) {
				len = min(sz_write, (length - i * sz_write));
				ret = mtd_store_write_skip_bad(mtd,
							       off,
							       &len,
							       &retlen,
							       limit,
							(u_char *)(source +
								i * sz_write),
							       0);
				if (ret)
					return -EIO;
				off += mtd->writesize;
				if (retlen > len)
					off += (retlen - len);
			}
#ifndef CONFIG_DISCRETE_BOOTLOADER
			if (size <= BL2_SIZE)
				return ret;
			ret = mtd_store_write_skip_bad(mtd,
						       off,
						       &(size - BL2_SIZE),
						       &retlen,
						       limit,
						       (u_char *)(source +
								BL2_SIZE),
						       0);
			if (ret)
				return -EIO;
#endif
			continue;
		}
#endif
		ret = mtd_store_write_skip_bad(mtd,
					       offset,
					       &len,
					       &retlen,
					       limit,
					       (u_char *)source,
					       0);
		if (ret)
			return -EIO;
	}
	return ret;
}

static int mtd_store_boot_erase(const char *part_name, u8 cpy)
{
	u8 num;
	size_t size_per_copy = 0, erasesize = 0;
	loff_t offset = 0;

	if (!part_name) {
		pr_info("%s %d invalid name!\n",
			__func__, __LINE__);
		return 1;
	}

	num = mtd_store_boot_copy_num(part_name);
	size_per_copy = mtd_store_boot_copy_size(part_name);
	if ((cpy > (num - 1)) && cpy != BOOT_OPS_ALL) {
		pr_info("%s %d invalid copy number %d\n",
			__func__, __LINE__, cpy);
		return 1;
	} else if (cpy == BOOT_OPS_ALL) {
		erasesize = num * size_per_copy;
	} else {
		offset = cpy * size_per_copy;
		erasesize = size_per_copy;
	}
	return mtd_store_erase(part_name, offset, erasesize, 0);
}

static u32 mtd_store_rsv_size(const char *rsv_name)
{
	if (!rsv_name) {
		pr_info("%s %d invalid rsv name, null\n",
			__func__, __LINE__);
		return 0;
	}
	if (!strcmp(rsv_name, RSV_BBT))
		return meson_rsv_bbt_size();
	else if (!strcmp(rsv_name, RSV_KEY))
		return meson_rsv_key_size();
	else if (!strcmp(rsv_name, RSV_ENV))
		return meson_rsv_env_size();
	else if (!strcmp(rsv_name, RSV_DTB))
		return meson_rsv_dtb_size();
	pr_info("%s %d invalid rsv info name: %s\n",
		__func__, __LINE__, rsv_name);
	return 0;
}

static int mtd_store_rsv_read(const char *rsv_name, size_t size, void *dest)
{
	if (!rsv_name) {
		pr_info("%s %d rsv info name can not be null\n",
			__func__, __LINE__);
		return 1;
	}
	if (!strcmp(rsv_name, RSV_BBT))
		return meson_rsv_bbt_read((u_char *)dest, size);
	else if (!strcmp(rsv_name, RSV_KEY))
		return meson_rsv_key_read((u_char *)dest, size);
	else if (!strcmp(rsv_name, RSV_ENV))
		return meson_rsv_env_read((u_char *)dest, size);
	else if (!strcmp(rsv_name, RSV_DTB))
		return meson_rsv_dtb_read((u_char *)dest, size);
	pr_info("%s %d invalid rsv info name: %s\n",
		__func__, __LINE__, rsv_name);
	return 1;
}

static int mtd_store_rsv_write(const char *rsv_name, size_t size, void *source)
{
	if (!rsv_name) {
		pr_info("%s %d rsv info name can not be null\n",
			__func__, __LINE__);
		return 1;
	}
	if (!strcmp(rsv_name, RSV_BBT))
		pr_info("%s %d can not write bbt!\n",
			__func__, __LINE__);
	else if (!strcmp(rsv_name, RSV_KEY))
		return meson_rsv_key_write((u_char *)source, size);
	else if (!strcmp(rsv_name, RSV_ENV))
		return meson_rsv_env_write((u_char *)source, size);
	else if (!strcmp(rsv_name, RSV_DTB))
		return meson_rsv_dtb_write((u_char *)source, size);

	pr_info("%s %d invalid rsv info name: %s\n",
		__func__, __LINE__, rsv_name);
	return 1;
}

static int mtd_store_rsv_erase(const char *rsv_name)
{
	if (!rsv_name) {
		pr_info("%s %d rsv info name can not be null\n",
			__func__, __LINE__);
		return 1;
	}
	if (!strcmp(rsv_name, RSV_BBT))
		pr_info("%s %d can not erase bbt!\n",
			__func__, __LINE__);
	else if (!strcmp(rsv_name, RSV_KEY))
		return meson_rsv_key_erase();
	else if (!strcmp(rsv_name, RSV_ENV))
		return meson_rsv_env_erase();
	else if (!strcmp(rsv_name, RSV_DTB))
		return meson_rsv_dtb_erase();
	pr_info("%s %d invalid rsv info name: %s\n",
		__func__, __LINE__, rsv_name);
	return 1;
}

static int mtd_store_rsv_protect(const char *rsv_name, bool ops)
{
	if (!rsv_name)
		return 1;
	if (!strcmp(rsv_name, RSV_KEY)) {
		if (ops)
			info_disprotect &= ~DISPROTECT_KEY;/*protect*/
		else

			info_disprotect |= DISPROTECT_KEY;/*disprotect*/

		printf("info_disprotect: %d\n", info_disprotect);
	} else if (!strcmp(rsv_name, RSV_BBT)) {
		if (ops)
			info_disprotect &= ~DISPROTECT_FBBT;/*protect*/
		else
			info_disprotect |= DISPROTECT_FBBT;/*disprotect*/
		printf("info_disprotect: %d\n", info_disprotect);

	} else {
		pr_info("no matched info in protection!\n");
	}
	return 0;
}

static u32 nor_rsv_size(const char *name)
{
	if (!name)
		return 0;
	return mtd_store_size(name);
}

static int nor_rsv_read(const char *name, size_t size, void *buf)
{
	struct mtd_info *mtd;
	loff_t offset;
	size_t length, total;
	int ret = 0;

	if (!name)
		return 1;
	length = size;
	mtd = mtd_store_get(0);
	total = mtd_store_size(name);
	if (length > total) {
		pr_info("request 0x%lx over the rsv size 0x%lx\n",
			length, total);
		return 1;
	}
	ret = mtd_store_get_offset(name, &offset, 0);
	if (ret)
		return ret;
	return mtd_read(mtd, offset, length, &length, buf);
}

static int nor_rsv_write(const char *name, size_t size, void *buf)
{
	struct mtd_info *mtd;
	loff_t offset;
	size_t length, total;
	int ret = 0;

	if (!name)
		return 1;
	length = size;
	mtd = mtd_store_get(0);
	total = mtd_store_size(name);
	if (length > total) {
		pr_info("request 0x%lx over the rsv size 0x%lx\n",
			length, total);
		return 1;
	}
	ret = mtd_store_get_offset(name, &offset, 0);
	if (ret)
		return ret;
	return mtd_write(mtd, offset, length, &length, buf);
}

static int nor_rsv_erase(const char *name)
{
	struct mtd_info *mtd;
	loff_t offset;
	size_t length;
	struct erase_info erase;
	int ret = 0;

	if (!name)
		return 1;
	length = nor_rsv_size(name);
	mtd = mtd_store_get(0);
	ret = mtd_store_get_offset(name, &offset, 0);
	if (ret)
		return ret;
	erase.mtd = mtd;
	erase.addr = offset;
	erase.len = length;
	return mtd_erase(mtd, &erase);
}

static int nor_rsv_protect(const char *name, bool ops)
{
	pr_info("spi nor flash not support this yet!\n");
	return 0;
}

void mtd_store_mount_ops(struct storage_t *store)
{
	store->get_part_size = mtd_store_size;
	store->read = mtd_store_read;
	store->write = mtd_store_write;
	store->erase = mtd_store_erase;
	store->boot_read = mtd_store_boot_read;
	store->boot_write = mtd_store_boot_write;
	store->boot_erase = mtd_store_boot_erase;
	store->get_copies = mtd_store_boot_copy_num;
	store->get_copy_size = mtd_store_boot_copy_size;
	if (store->type == BOOT_SNOR) {
		store->get_rsv_size = nor_rsv_size;
		store->read_rsv = nor_rsv_read;
		store->write_rsv = nor_rsv_write;
		store->erase_rsv = nor_rsv_erase;
		store->protect_rsv = nor_rsv_protect;
	} else {
		store->get_rsv_size = mtd_store_rsv_size;
		store->read_rsv = mtd_store_rsv_read;
		store->write_rsv = mtd_store_rsv_write;
		store->erase_rsv = mtd_store_rsv_erase;
		store->protect_rsv = mtd_store_rsv_protect;
	}
}
