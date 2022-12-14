// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <nand.h>
#include <asm/io.h>
#include <malloc.h>
#include <linux/err.h>
#include <asm/cache.h>
//#include <asm/arch/secure_apb.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
#include <linux/mtd/nand_ecc.h>

#include "aml_nand.h"
#include "version.h"

extern struct mtd_info *nand_info[CONFIG_SYS_MAX_NAND_DEVICE];
extern struct hw_controller *controller;
/* provide a policy that calculate the bakups of bootloader */
int get_boot_num(struct mtd_info *mtd, size_t rwsize)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
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

	aml_chip->boot_copy_num = ret;
	printk("self-adaption boot count:%d\n", ret);

	return ret;
}

/*set nand info into page0_buf for romboot.*/
void nand_info_page_prepare(struct aml_nand_chip *aml_chip, u8 *page0_buf)
{
	struct nand_chip *chip = &aml_chip->chip;
	struct mtd_info *mtd = &chip->mtd;
	struct aml_nand_chip *aml_chip_normal = mtd_to_nand_chip(nand_info[1]);
	u32 configure_data;
	nand_page0_t *p_nand_page0 = NULL;
	nand_page0_sc2_t *p_nand_page0_sc2 = NULL;
	ext_info_t *p_ext_info = NULL;
	nand_setup_t *p_nand_setup = NULL;
	nand_setup_sc2_t * p_nand_setup_sc2 = NULL;
	int each_boot_pages, boot_num, bbt_pages;
	unsigned int pages_per_blk_shift ,bbt_size;
	fip_info_t *p_fip_info = NULL;
	uint32_t ddrp_start_block = 0;

	pages_per_blk_shift = (chip->phys_erase_shift - chip->page_shift);
	bbt_size = aml_chip_normal->rsv->bbt->size;
	if ((store_get_device_bootloader_mode() == DISCRETE_BOOTLOADER) ||
		(store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER)) {
		boot_num = CONFIG_NAND_TPL_COPY_NUM;
		each_boot_pages = CONFIG_TPL_SIZE_PER_COPY / mtd->writesize;
	} else {
		boot_num = (!aml_chip->boot_copy_num)? 1: aml_chip->boot_copy_num;
		each_boot_pages = BOOT_TOTAL_PAGES / boot_num;
	}

	p_nand_page0 = (nand_page0_t *) page0_buf;
	p_nand_setup = &p_nand_page0->nand_setup;
	p_ext_info = &p_nand_page0->ext_info;

	configure_data = NFC_CMD_N2M(aml_chip->ran_mode,
			aml_chip->bch_mode, 0, (chip->ecc.size >> 3),
			chip->ecc.steps);

	memset(p_nand_page0, 0x0, sizeof(nand_page0_t));
	if (store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER) {
		p_nand_page0_sc2 = (nand_page0_sc2_t *) page0_buf;
		p_nand_setup_sc2 = &p_nand_page0_sc2->nand_setup;
		p_ext_info = &p_nand_page0_sc2->ext_info;
		p_nand_setup_sc2->cfg.d32 = configure_data;
		p_nand_setup_sc2->cfg.b.page_list = 0;
		p_nand_setup_sc2->cfg.b.new_type = 0;
		p_fip_info = &p_nand_page0_sc2->fip_info;
		printk("advance cfg.d32 0x%x\n", p_nand_setup_sc2->cfg.d32);
	} else {
		p_nand_page0 = (nand_page0_t *) page0_buf;
		p_nand_setup = &p_nand_page0->nand_setup;
		p_ext_info = &p_nand_page0->ext_info;
		p_nand_setup->cfg.d32 = (configure_data | (1<<23) | (1<<22) | (2<<20));
		memset(p_nand_page0->page_list, 0, NAND_PAGELIST_CNT);
		p_fip_info = &p_nand_page0->fip_info;
		printk("cfg.d32 0x%x\n", p_nand_setup->cfg.d32);
	}
	p_ext_info->page_per_blk = aml_chip->block_size / aml_chip->page_size;
	p_ext_info->boot_num = boot_num;
	p_ext_info->each_boot_pages = each_boot_pages;
	bbt_pages =
	(bbt_size + mtd->writesize - 1) / mtd->writesize;
	p_ext_info->bbt_occupy_pages = bbt_pages;
	p_ext_info->bbt_start_block =
		(BOOT_TOTAL_PAGES >> pages_per_blk_shift) + NAND_GAP_BLOCK_NUM;
	if ((store_get_device_bootloader_mode() == DISCRETE_BOOTLOADER) ||
	    (store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER)) {
		p_fip_info->version = 1;
		p_fip_info->mode = NAND_FIPMODE_DISCRETE;
		/* in pages, fixme, should it stored in amlchip? */
		p_fip_info->fip_start =
			1024 + NAND_RSV_BLOCK_NUM * p_ext_info->page_per_blk;
		ddrp_start_block = aml_chip_normal->rsv->ddr_para->nvalid->blk_addr;
		p_nand_page0->ddrp_start_page = (ddrp_start_block << pages_per_blk_shift) +
			aml_chip_normal->rsv->ddr_para->nvalid->page_addr;
		printk("ddrp_start_page = 0x%x ddr_start_block = 0x%x\n",
			p_nand_page0->ddrp_start_page, ddrp_start_block);
		printk("bl: version %d, mode %d, start 0x%x\n",
			p_fip_info->version, p_fip_info->mode, p_fip_info->fip_start);
	}
	printk("page_per_blk = 0x%x bbt_pages = 0x%x \n",
		p_ext_info->page_per_blk, bbt_pages);
	printk("boot_num = %d each_boot_pages = %d\n", boot_num,
		each_boot_pages);
}

/* mtd support interface:
 * function:int (*_erase) (struct mtd_info *mtd, struct erase_info *instr);
 */
int m3_nand_boot_erase_cmd(struct mtd_info *mtd, int page)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	struct nand_chip *chip = mtd->priv;
	loff_t ofs;

	ofs = ((loff_t)page << chip->page_shift);

	if (chip->block_bad(mtd, ofs))
		return 0;
	aml_chip->aml_nand_select_chip(aml_chip, 0);
	aml_chip->aml_nand_command(aml_chip,
		NAND_CMD_ERASE1, -1, page, 0);
	aml_chip->aml_nand_command(aml_chip,
		NAND_CMD_ERASE2, -1, -1, 0);
	chip->waitfunc(mtd, chip);

	return 0;
}

/* mtd support interface:
 * chip->ecc.read_page
 * function:int (*read_page)(struct mtd_info *mtd, struct nand_chip *chip,
 *		uint8_t *buf, int oob_required, int page);
 */
int m3_nand_boot_read_page_hwecc(struct mtd_info *mtd,
	struct nand_chip *chip, uint8_t *buf, int oob_required, int page)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	uint8_t *oob_buf = chip->oob_poi;
	unsigned nand_page_size = chip->ecc.steps * chip->ecc.size;
	unsigned pages_per_blk_shift =chip->phys_erase_shift - chip->page_shift;
	int user_byte_num = (chip->ecc.steps * aml_chip->user_byte_mode);
	int bch_mode = aml_chip->bch_mode, ran_mode=0;
	int error = 0, i = 0, stat = 0;
	int ecc_size, configure_data_w, pages_per_blk_w, configure_data;
	int pages_per_blk, read_page;
	int en_slc = 0;
	/* using info page structure */
	nand_page0_t *p_nand_page0 = NULL;
	ext_info_t *p_ext_info = NULL;
	nand_setup_t * p_nand_setup = NULL;
	int each_boot_pages, boot_num;
	loff_t ofs;

	if ((store_get_device_bootloader_mode() == DISCRETE_BOOTLOADER) ||
	    (store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER))
		boot_num = CONFIG_BL2_COPY_NUM; /* TODO: need add advance mode support */
	else
		boot_num = (!aml_chip->boot_copy_num)? 1: aml_chip->boot_copy_num;

	each_boot_pages = BOOT_TOTAL_PAGES/boot_num;

	if (page >= (each_boot_pages * boot_num)) {
		memset(buf, 0, (1 << chip->page_shift));
		printk("nand boot read out of uboot failed, page:%d\n", page);
		goto exit;
	}
	/* nand page info */
	if ((page % each_boot_pages) == 0) {
		if (aml_chip->bch_mode == NAND_ECC_BCH_SHORT)
			configure_data_w =
				NFC_CMD_N2M(aml_chip->ran_mode,
		NAND_ECC_BCH60_1K, 1, (chip->ecc.size >> 3), chip->ecc.steps);
		else
			configure_data_w =
				NFC_CMD_N2M(aml_chip->ran_mode,
		aml_chip->bch_mode, 0, (chip->ecc.size >> 3), chip->ecc.steps);

		ecc_size = chip->ecc.size;  //backup ecc size

		if (aml_chip->bch_mode != NAND_ECC_BCH_SHORT) {
			nand_page_size =
				(mtd->writesize / 512) * NAND_ECC_UNIT_SHORT;
			bch_mode = NAND_ECC_BCH_SHORT;
			chip->ecc.size = NAND_ECC_UNIT_SHORT;
		} else
			bch_mode = aml_chip->bch_mode;

		chip->cmdfunc(mtd, NAND_CMD_READ0, 0x00, page);
		memset(buf, 0xff, (1 << chip->page_shift));
		/* read back page0 and check it */
		if (aml_chip->valid_chip[0]) {
			if (!aml_chip->aml_nand_wait_devready(aml_chip, i)) {
				printk("don't found selected chip:%d ready\n",
					i);
				//error = -EBUSY;
			}
			if (aml_chip->ops_mode & AML_CHIP_NONE_RB)
				chip->cmd_ctrl(mtd, NAND_CMD_READ0 & 0xff,
					NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
			if (en_slc == 0) {
				ran_mode = aml_chip->ran_mode;
				aml_chip->ran_mode = 1;
			}
			error = aml_chip->aml_nand_dma_read(aml_chip,
				buf, nand_page_size, bch_mode);

			if (error) {
				printk(" page0 aml_nand_dma_read failed\n");
			}

			aml_chip->aml_nand_get_user_byte(aml_chip,
				oob_buf, user_byte_num);
			stat = aml_chip->aml_nand_hwecc_correct(aml_chip,
				buf, nand_page_size, oob_buf);
			if (stat < 0) {
				if(aml_chip->ran_mode
				&& (aml_chip->zero_cnt <  aml_chip->ecc_max)) {
					memset(buf, 0xff, nand_page_size);
					memset(oob_buf, 0xff, user_byte_num);
				} else {
					mtd->ecc_stats.failed++;
					printk("page0 read ecc failed at blk0 chip0\n");
				}
			} else
				mtd->ecc_stats.corrected += stat;
			if (en_slc == 0)
				aml_chip->ran_mode = ran_mode;
		} else {
			printk("nand boot page 0 no valid chip failed\n");
			error = -ENODEV;
			//goto exit;
		}

		//check page 0 info here
		p_nand_page0 = (nand_page0_t *) buf;
		p_nand_setup = &p_nand_page0->nand_setup;
		p_ext_info = &p_nand_page0->ext_info;

		configure_data = p_nand_setup->cfg.b.cmd;
		pages_per_blk = p_ext_info->page_per_blk;
		pages_per_blk_w =
			(1 << (chip->phys_erase_shift - chip->page_shift));

		if ((pages_per_blk_w != pages_per_blk)
			|| (configure_data != configure_data_w))
			printk("page%d warning, configure:0x%x-0x%x "
				"pages_per_blk:0x%x-0x%x\n",
				page, configure_data_w, configure_data,
				pages_per_blk_w, pages_per_blk);
		if ((store_get_device_bootloader_mode() == DISCRETE_BOOTLOADER) ||
			(store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER))
			/* fixme, check fip_info_t */
			printk(" TODO: check fip info\n");

		bch_mode = aml_chip->bch_mode;
		chip->ecc.size = ecc_size;
		nand_page_size = chip->ecc.steps * chip->ecc.size;
	}

	read_page = page;
        read_page++;
READ_BAD_BLOCK:
        ofs = ((loff_t)read_page << chip->page_shift);
	if (!(ofs % mtd->erasesize)) {
		if (chip->block_bad(mtd, ofs)) {
			read_page +=
				1 << (chip->phys_erase_shift-chip->page_shift);
			goto READ_BAD_BLOCK;
		}
	}

        chip->cmdfunc(mtd, NAND_CMD_READ0, 0x00, read_page);

        memset(buf, 0xff, (1 << chip->page_shift));
        if (aml_chip->valid_chip[0]) {
		if (!aml_chip->aml_nand_wait_devready(aml_chip, 0)) {
			printk("don't found selected chip0 ready, page: %d \n",
				page);
			error = -EBUSY;
			goto exit;
		}
		if (aml_chip->ops_mode & AML_CHIP_NONE_RB)
			chip->cmd_ctrl(mtd, NAND_CMD_READ0 & 0xff,
				NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);

		error = aml_chip->aml_nand_dma_read(aml_chip,
			buf, nand_page_size, bch_mode);
		if (error) {
			error = -ENODEV;
			printk("aml_nand_dma_read failed: page:%d \n", page);
			goto exit;
		}

		aml_chip->aml_nand_get_user_byte(aml_chip,
			oob_buf, user_byte_num);
		stat = aml_chip->aml_nand_hwecc_correct(aml_chip,
			buf, nand_page_size, oob_buf);
		if (stat < 0) {
			error = -ENODEV;
			mtd->ecc_stats.failed++;
			printk("read data ecc failed at page%d blk%d chip%d\n",
				page, (page >> pages_per_blk_shift), i);
		} else
			mtd->ecc_stats.corrected += stat;
        } else
		error = -ENODEV;

exit:
	return error;
}


/* mtd support interface:
 * chip->ecc.write_page
 * function:int (*write_page)(struct mtd_info *mtd, struct nand_chip *chip,
 *		uint8_t *buf, int oob_required, int page);
 */
int m3_nand_boot_write_page_hwecc(struct mtd_info *mtd,
	struct nand_chip *chip, const uint8_t *buf, int oob_required,
	int page)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	uint8_t *oob_buf = chip->oob_poi;
	unsigned nand_page_size = chip->ecc.steps * chip->ecc.size;
	int user_byte_num = (chip->ecc.steps * aml_chip->user_byte_mode);
	int error = 0, i = 0, bch_mode, ecc_size;
	int each_boot_pages, boot_num;

	if ((store_get_device_bootloader_mode() == DISCRETE_BOOTLOADER) ||
		(store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER))
		boot_num = CONFIG_BL2_COPY_NUM; /* TODO: need add advance mode support */
	else
		boot_num = (!aml_chip->boot_copy_num)? 1: aml_chip->boot_copy_num;
	each_boot_pages = BOOT_TOTAL_PAGES/boot_num;

	ecc_size = chip->ecc.size;
	if (((aml_chip->page_addr % each_boot_pages) == 0)
		&& (aml_chip->bch_mode != NAND_ECC_BCH_SHORT)) {
		nand_page_size = (mtd->writesize / 512) * NAND_ECC_UNIT_SHORT;
		bch_mode = NAND_ECC_BCH_SHORT;
		chip->ecc.size = NAND_ECC_UNIT_SHORT;
	} else
		bch_mode = aml_chip->bch_mode;
	/* setting magic for romboot checks. */
	for (i = 0; i < mtd->oobavail; i += 2) {
		oob_buf[i] = 0x55;
		oob_buf[i+1] = 0xaa;
	}

	i = 0;
	if (aml_chip->valid_chip[i]) {
		aml_chip->aml_nand_select_chip(aml_chip, i);
		aml_chip->aml_nand_set_user_byte(aml_chip,
			oob_buf, user_byte_num);
		error = aml_chip->aml_nand_dma_write(aml_chip,
			(unsigned char *)buf, nand_page_size, bch_mode);
		if (error)
			goto exit;
		aml_chip->aml_nand_command(aml_chip,
			NAND_CMD_PAGEPROG, -1, -1, i);
	} else {
		error = -ENODEV;
		goto exit;
	}
exit:
	if (((aml_chip->page_addr % each_boot_pages) == 0)
			&& (aml_chip->bch_mode != NAND_ECC_BCH_SHORT))
		chip->ecc.size = ecc_size;
	return error;
}

/* mtd support interface:
 * chip->write_page
 * function:	int (*write_page)(struct mtd_info *mtd, struct nand_chip *chip,
 *			uint32_t offset, int data_len, const uint8_t *buf,
 *			int oob_required, int page, int cached, int raw);
 */
int m3_nand_boot_write_page(struct mtd_info *mtd, struct nand_chip *chip,
	uint32_t offset, int data_len, const uint8_t *buf,
	int oob_required, int page, int raw)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	int status, write_page, ran_mode=0;
	int en_slc = 0, each_boot_pages, boot_num;
	loff_t ofs;

	if ((store_get_device_bootloader_mode() == DISCRETE_BOOTLOADER) ||
		(store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER))
		boot_num = CONFIG_BL2_COPY_NUM; /* TODO: need add advance mode support */
	else
		boot_num = (!aml_chip->boot_copy_num)? 1: aml_chip->boot_copy_num;

	each_boot_pages = BOOT_TOTAL_PAGES / boot_num;

	/* actual page to be written */
	write_page = page;
	/* zero page of each copy */
	if ((write_page % each_boot_pages) == 0) {
		nand_info_page_prepare(aml_chip, chip->buffers->databuf);
		chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00, write_page);
		/* must enable ran_mode for info page */
		if (en_slc == 0) {
			ran_mode = aml_chip->ran_mode;
			aml_chip->ran_mode = 1;
		}
		chip->ecc.write_page(mtd, chip, chip->buffers->databuf, 0, 0);
		if (en_slc == 0)
			aml_chip->ran_mode = ran_mode;

		status = chip->waitfunc(mtd, chip);

		if (status & NAND_STATUS_FAIL) {
			printk("uboot wr 0 page=0x%x, status=0x%x\n",
				page, status);
			return -EIO;
		}
	}
	/* +1 for skipping nand info page */
	if (en_slc) {
	} else
		write_page++;

WRITE_BAD_BLOCK:
	ofs = ((loff_t)write_page << chip->page_shift);
	if (!(ofs % mtd->erasesize)) {
		if (chip->block_bad(mtd, ofs)) {
			write_page +=
			1 << (chip->phys_erase_shift-chip->page_shift);
			goto WRITE_BAD_BLOCK;
		}
	}
	chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00, write_page);

	if (unlikely(raw))
		chip->ecc.write_page_raw(mtd, chip, buf, 0, 0);
	else
		chip->ecc.write_page(mtd, chip, buf, 0, 0);

	if (!(chip->options & NAND_CACHEPRG)) {
		status = chip->waitfunc(mtd, chip);

		if (status & NAND_STATUS_FAIL) {
			printk("uboot wr page=0x%x, status=0x%x\n",
				page,status);
			return -EIO;
		}
	} else
		status = chip->waitfunc(mtd, chip);
	return 0;
}
