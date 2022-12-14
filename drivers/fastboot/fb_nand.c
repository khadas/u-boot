// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2014 Broadcom Corporation.
 * Copyright 2015 Free Electrons.
 */

#include <config.h>
#include <common.h>

#include <fastboot.h>
#include <image-sparse.h>

#include <linux/mtd/mtd.h>
#include <jffs2/jffs2.h>
#include <nand.h>
#include <amlogic/storage.h>
#include <amlogic/aml_mtd.h>
#include <amlogic/aml_nand.h>
#include <amlogic/aml_rsv.h>
#include <amlogic/storage.h>
#include <amlogic/cpu_id.h>

struct fb_nand_sparse {
	struct mtd_info		*mtd;
	struct part_info	*part;
};

__weak int board_fastboot_erase_partition_setup(char *name)
{
	return 0;
}

__weak int board_fastboot_write_partition_setup(char *name)
{
	return 0;
}

extern struct mtd_info *mtd_store_get(int dev);
static int fb_nand_lookup(const char *partname,
			  struct mtd_info **mtd,
			  struct part_info **part,
			  char *response)
{
	struct mtd_device *dev;
	int ret;
	u8 pnum;

	ret = mtdparts_init();
	if (ret) {
		pr_err("Cannot initialize MTD partitions\n");
		fastboot_fail("cannot init mtdparts", response);
		return ret;
	}

	if (strcmp(partname, "dtb") == 0)
		return 0;
	ret = find_dev_and_part(partname, &dev, &pnum, part);
	if (ret) {
		pr_err("cannot find partition: '%s'", partname);
		fastboot_fail("cannot find partition", response);
		return ret;
	}

	if (dev->id->type != MTD_DEV_TYPE_NAND) {
		pr_err("partition '%s' is not stored on a NAND device",
		      partname);
		fastboot_fail("not a NAND device", response);
		return -EINVAL;
	}

	*mtd = mtd_store_get(dev->id->num);

	return 0;
}

static int _fb_nand_erase(struct mtd_info *mtd, struct part_info *part)
{
	nand_erase_options_t opts;
	int ret;

	memset(&opts, 0, sizeof(opts));
	opts.offset = part->offset;
	opts.length = part->size;
	opts.quiet = 1;

	printf("Erasing blocks 0x%llx to 0x%llx\n",
	       part->offset, part->offset + part->size);

	ret = nand_erase_opts(mtd, &opts);
	if (ret)
		return ret;

	printf("........ erased 0x%llx bytes from '%s'\n",
	       part->size, part->name);

	return 0;
}

static int _fb_nand_write(struct mtd_info *mtd, struct part_info *part,
			  void *buffer, u32 offset,
			  size_t length, size_t *written)
{
	int flags = WITH_WR_VERIFY;

#ifdef CONFIG_FASTBOOT_FLASH_NAND_TRIMFFS
	flags |= WITH_DROP_FFS;
#endif

	return nand_write_skip_bad(mtd, offset, &length, written,
				   part->size - (offset - part->offset),
				   buffer, flags);
}

static lbaint_t fb_nand_sparse_write(struct sparse_storage *info,
		lbaint_t blk, lbaint_t blkcnt, const void *buffer)
{
	struct fb_nand_sparse *sparse = info->priv;
	size_t written;
	int ret;

	ret = _fb_nand_write(sparse->mtd, sparse->part, (void *)buffer,
			     blk * info->blksz,
			     blkcnt * info->blksz, &written);
	if (ret < 0) {
		printf("Failed to write sparse chunk\n");
		return ret;
	}

/* TODO - verify that the value "written" includes the "bad-blocks" ... */

	/*
	 * the return value must be 'blkcnt' ("good-blocks") plus the
	 * number of "bad-blocks" encountered within this space...
	 */
	return written / info->blksz;
}

static lbaint_t fb_nand_sparse_reserve(struct sparse_storage *info,
		lbaint_t blk, lbaint_t blkcnt)
{
	int bad_blocks = 0;

/*
 * TODO - implement a function to determine the total number
 * of blocks which must be used in order to reserve the specified
 * number ("blkcnt") of "good-blocks", starting at "blk"...
 * ( possibly something like the "check_skip_len()" function )
 */

	/*
	 * the return value must be 'blkcnt' ("good-blocks") plus the
	 * number of "bad-blocks" encountered within this space...
	 */
	return blkcnt + bad_blocks;
}

/**
 * fastboot_nand_get_part_info() - Lookup NAND partion by name
 *
 * @part_name: Named device to lookup
 * @part_info: Pointer to returned part_info pointer
 * @response: Pointer to fastboot response buffer
 */
int fastboot_nand_get_part_info(char *part_name, struct part_info **part_info,
				char *response)
{
	struct mtd_info *mtd = NULL;

	return fb_nand_lookup(part_name, &mtd, part_info, response);
}

int get_bootnum(struct mtd_info *mtd, size_t rwsize)
{
	size_t bad_blk_len_low = 0, bad_blk_len_up = 0, skip;
	size_t available_space;
	size_t block_len, block_off;
	loff_t block_start;
	loff_t offset = 0;
	int ret = 1; /*inital for only one copy*/

	if (!rwsize) { /*not need to policy call, only one */
		ret = 1;
		return ret;
	}

	/* align with page size */
	rwsize = ((rwsize + mtd->writesize - 1)/mtd->writesize)*mtd->writesize;

	while (offset < mtd->size) {
		block_start = offset & ~(loff_t)(mtd->erasesize - 1);
		block_off = offset & (mtd->erasesize - 1);
		block_len = mtd->erasesize - block_off;

		if (nand_block_isbad(mtd, block_start)) {
			if ( offset < mtd->size / 2)   /*no understand*/
				bad_blk_len_low += block_len;
			else if (offset > mtd->size / 2)
				bad_blk_len_up += block_len;
			else {
				bad_blk_len_up = offset;
			}
		}
		offset += block_len;
	}

	printk("rwsize:0x%zx skip_low:0x%zx skip_up:0x%zx\n",
		rwsize, bad_blk_len_low, bad_blk_len_up);

	skip = bad_blk_len_low + bad_blk_len_up;
	available_space = mtd->size - skip - 2 * mtd->writesize; /*no understand*/

	if (rwsize * 2 <= available_space) {
		ret = 1;
		if (rwsize + mtd->writesize + bad_blk_len_low > mtd->size / 2)
			return 1; /*1st must be write*/
		if (rwsize + mtd->writesize + bad_blk_len_up <= mtd->size / 2)
			ret ++;
	} else /*needn't consider bad block length, unlikely so many bad blocks*/
		ret = 1;

	printk("self-adaption boot count:%d\n", ret);

	return ret;
}

/**
 * fastboot_nand_flash_write() - Write image to NAND for fastboot
 *
 * @cmd: Named device to write image to
 * @download_buffer: Pointer to image data
 * @download_bytes: Size of image data
 * @response: Pointer to fastboot response buffer
 */
void fastboot_nand_flash_write(const char *cmd, void *download_buffer,
			       u32 download_bytes, char *response)
{
	struct part_info *part;
	struct mtd_info *mtd = NULL;
	int ret, err;
	int copy_num = 0, i = 0;
	int pages_per_copy;
	u64 off = 0;
	size_t rwsize = 0, limit = 0;

	enum boot_type_e medium_type = store_get_type();

	ret = fb_nand_lookup(cmd, &mtd, &part, response);
	if (ret) {
		pr_err("invalid NAND device");
		fastboot_fail("invalid NAND device", response);
		return;
	}

	if (strcmp(cmd, "bootloader") == 0) {
		rwsize = download_bytes;
		if ((store_get_device_bootloader_mode() == DISCRETE_BOOTLOADER) ||
			(store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER)) {
			/* TODO: Need to add support for advance mode */
			copy_num = CONFIG_BL2_COPY_NUM;
			pages_per_copy = BOOT_TOTAL_PAGES / CONFIG_BL2_COPY_NUM;
			limit = mtd->writesize * pages_per_copy;
		} else {
			copy_num = get_bootnum(mtd, rwsize);
			limit = mtd->size / copy_num;
		}

		for (i = 0; i < copy_num; i++) {
			printf("off = 0x%llx,wsize = 0x%lx\n",
				off, rwsize);
			err = nand_write_skip_bad(mtd, off, &rwsize,
						NULL, limit,
						(u_char *)download_buffer, 0);
			if (err) {
				rwsize = download_bytes;
				printf("bootloader write err,code = %d\n",err);
			}
			off += limit;
		}
		fastboot_okay("write bootloader", response);
		return;
	}

	if ((store_get_device_bootloader_mode() == DISCRETE_BOOTLOADER) ||
		(store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER)) {
		/* TODO: Need to add support for advance mode */
			if (strcmp(cmd, "tpl") == 0) {
				if (BOOT_SNAND == medium_type ||
				   BOOT_NAND_MTD == medium_type)
					copy_num = CONFIG_NAND_TPL_COPY_NUM;
				else if (medium_type == BOOT_SNOR)
					copy_num = CONFIG_NOR_TPL_COPY_NUM;
				rwsize = download_bytes;
				limit = CONFIG_TPL_SIZE_PER_COPY;
				off = 1024 * mtd->writesize +
					NAND_RSV_BLOCK_NUM * mtd->erasesize;

				for (i = 0; i < copy_num; i++) {
					printf("off = 0x%llx,wsize = 0x%lx\n", off, rwsize);
					err = nand_write_skip_bad(mtd, off, &rwsize,
								NULL, limit,
								(u_char *)download_buffer, 0);
					if (err) {
						rwsize = download_bytes;
						printf("tpl write err,code = %d\n",err);
					}
					off += CONFIG_TPL_SIZE_PER_COPY;
				}
				fastboot_okay("write tpl", response);
				return;
			}
		}

	if (strcmp(cmd, "dtb") == 0) {
		ret = store_rsv_write("dtb", download_bytes, (u8 *)download_buffer);
		printf("Flashing dtb...len:0x%x\n", download_bytes);
		if (ret) {
			printf("write dtb fail,result code %d\n", ret);
			fastboot_fail("write dtb", response);
		} else {
			fastboot_okay("write dtb", response);
		}
		return;
	}

	ret = board_fastboot_write_partition_setup(part->name);
	if (ret)
		return;

	if (is_sparse_image(download_buffer)) {
		struct fb_nand_sparse sparse_priv;
		struct sparse_storage sparse;

		sparse_priv.mtd = mtd;
		sparse_priv.part = part;

		sparse.blksz = mtd->writesize;
		sparse.start = part->offset / sparse.blksz;
		sparse.size = part->size / sparse.blksz;
		sparse.write = fb_nand_sparse_write;
		sparse.reserve = fb_nand_sparse_reserve;
		sparse.mssg = fastboot_fail;

		printf("Flashing sparse image at offset " LBAFU "\n",
		       sparse.start);

		sparse.priv = &sparse_priv;
		ret = write_sparse_image(&sparse, cmd, download_buffer,
					 response);
		if (!ret)
			fastboot_okay(NULL, response);
	} else {
		printf("Flashing raw image at offset 0x%llx\n",
		       part->offset);

		ret = _fb_nand_write(mtd, part, download_buffer, part->offset,
				     download_bytes, NULL);

		printf("........ wrote %u bytes to '%s'\n",
		       download_bytes, part->name);
	}

	if (ret) {
		fastboot_fail("error writing the image", response);
		return;
	}

	fastboot_okay(NULL, response);
}

/**
 * fastboot_nand_flash_erase() - Erase NAND for fastboot
 *
 * @cmd: Named device to erase
 * @response: Pointer to fastboot response buffer
 */
void fastboot_nand_erase(const char *cmd, char *response)
{
	struct part_info *part;
	struct mtd_info *mtd = NULL;
	int ret;

	ret = fb_nand_lookup(cmd, &mtd, &part, response);
	if (ret) {
		pr_err("invalid NAND device");
		fastboot_fail("invalid NAND device", response);
		return;
	}

	if (strcmp(cmd, "dtb") == 0) {
		ret = store_rsv_erase("dtb");
		if (ret) {
			pr_err("erase dtb fail,ret = %d\n", ret);
			fastboot_fail("erase dtb",
				response);
		} else {
			fastboot_okay("erase dtb", response);
		}
		return;
	}

	ret = board_fastboot_erase_partition_setup(part->name);
	if (ret)
		return;

	ret = _fb_nand_erase(mtd, part);
	if (ret) {
		pr_err("failed erasing from device %s", mtd->name);
		fastboot_fail("failed erasing from device", response);
		return;
	}

	fastboot_okay(NULL, response);
}
