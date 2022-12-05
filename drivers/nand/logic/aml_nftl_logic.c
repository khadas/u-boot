/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_nftl_logic.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "aml_nftl.h"

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/

uint32 nand_write_logic_page(struct aml_nftl_part_t* part,uint32 page_no,uchar *buf)
{
    if(part->ftl_status.unusual == FTL_UNUSUAL){
		NPRINT("%s  %d: nftl status is unusual. do not write anything\n",__func__,__LINE__);
		return 0;
    }
	NFTL_LINE
    if(nand_write_logic_page_no_gc(part,page_no,buf) != 0)
    {
        return 1;
    }
    if(garbage_collect(part) != 0)
    {
        return 1;
    }

//	if(do_prio_gc(part) != 0)
//	{
//		return 1;
//	}

    return 0;
}
extern void _dump_mem_u8(uint8_t * buf, uint32_t len);
/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
uint32 nand_read_logic_page(struct aml_nftl_part_t* part,uint32 page_no,uchar *buf)
{
	int ret;
	uchar spare_data[BYTES_OF_USER_PER_PAGE];
	_physic_op_par phy_op_par;
	_phy_block_info* p_phy_block;

	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = 0;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	phy_op_par.main_data_addr = buf;
	phy_op_par.spare_data_addr = spare_data;

	ret = part->nand_read_page(part, &phy_op_par);
	if((ret == -EUCLEAN)|| (ret  == -EBADMSG)) {
		if((part->cfg->nftl_support_gc_read_reclaim != 0) || (ret  == -EUCLEAN)) {
			p_phy_block = get_phy_block_addr(part, 0);
			add_prio_gc(part,p_phy_block,GC_READ_RECLAIM);
			NPRINT("read a page: %d READ_RECLAIM!\n",page_no);
		}
		if (ret  == -EBADMSG){
			//memset(buf, 0x0, part->nand_chip->bytes_per_page);
			NPRINT("nand_read_logic_page : read a page ecc failed \n");
			p_phy_block = get_phy_block_addr(part, 0);
			add_prio_gc(part,p_phy_block, GC_READ_RECLAIM);
		}
		ret = 0;
	}

	if(do_prio_gc(part) != 0) {
		NPRINT("do prio gc failed here after read page\n");
	}

	return ret;
}

/*****************************************************************************
*Name         :
*Description  :
*Parameter    :
*Return       :
*Note         :
*****************************************************************************/
uint32 do_write_error(struct aml_nftl_part_t* part)
{
	uint32 ret;
	_phy_block_info* p_phy_block_free;
	_physic_op_par phy_op_par;

write_error:
	p_phy_block_free = out_phy_block_from_free_list(part);
	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = p_phy_block_free->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	ret = part->nand_erase_superblk(part,&phy_op_par);
	p_phy_block_free->invalid_page_count = 0;
	p_phy_block_free->erase_count++;
	if(ret != 0) {
		part->nand_mark_bad_blk(part,&phy_op_par);
		NPRINT("do_write_error erase error!\n");
		if(part->free_block_num > (part->gc_strategy.start_gc_free_blocks>>2)) {
			goto write_error;
		} else {
			NPRINT("%s %d :no free block\n",__func__,__LINE__);
			return 1;
		}
	}

	p_phy_block_free->invalid_page_count = 0;

	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = part->part_current_used_block->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	part->nand_mark_bad_blk(part,&phy_op_par);
	return 0;
}

int nand_write_logic_page_no_gc(struct aml_nftl_part_t* part,uint32 page_no,uchar *buf)
{
	uint32 ret;
	_phy_block_info* p_phy_block_free;
	_physic_op_par phy_op_par;

write_error:
	p_phy_block_free = out_phy_block_from_free_list(part);
	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = p_phy_block_free->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	ret = part->nand_write_page(part,&phy_op_par);
	p_phy_block_free->invalid_page_count = 0;
	if(ret != 0) {
		part->nand_mark_bad_blk(part,&phy_op_par);
		NPRINT("do_write_error erase error!\n");
		if(part->free_block_num > (part->gc_strategy.start_gc_free_blocks>>2)) {
			goto write_error;
		} else {
			NPRINT("%s %d :no free block\n",__func__,__LINE__);
			return 1;
		}
	}

	p_phy_block_free->invalid_page_count = 0;
	p_phy_block_free = out_phy_block_from_free_list(part);
	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = p_phy_block_free->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	ret = part->nand_erase_superblk(part,&phy_op_par);
	p_phy_block_free->invalid_page_count = 0;
	p_phy_block_free->erase_count++;
	if(ret != 0) {
		part->nand_mark_bad_blk(part,&phy_op_par);
		NPRINT("do_write_error erase error!\n");
		if(part->free_block_num > (part->gc_strategy.start_gc_free_blocks>>2)) {
			goto write_error;
		} else {
			NPRINT("%s %d :no free block\n",__func__,__LINE__);
			return 1;
		}
	}

	p_phy_block_free->invalid_page_count = 0;

	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = part->part_current_used_block->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	part->nand_mark_bad_blk(part,&phy_op_par);	
	return 0;
}

int write_phy_page_map(struct aml_nftl_part_t* part)
{
	int ret;
	uchar spare_data[BYTES_OF_USER_PER_PAGE];
	_physic_op_par phy_op_par;
    
write_data1:
	phy_op_par.phy_page.Page_NO = part->current_block_used_page;
	phy_op_par.phy_page.blkNO_in_chip = part->part_current_used_block->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	phy_op_par.main_data_addr = NULL;
	phy_op_par.spare_data_addr = spare_data;
	ret = part->nand_write_page(part,&phy_op_par);
	if(part->part_current_used_block->invalid_page_count > part->nand_chip->pages_per_blk) {
		NPRINT("invalid_page_count more than pages_per_blk 3!!\n");
	}
	if(ret <0) {
		NPRINT("nand_write_logic_page_no_gc write error!\n");
		if(do_write_error(part) == 0) {
			goto write_data1;
		} else {
			return 1;
		}
	}
	adjust_invalid_list(part);
	return 0;
}

int set_new_current_block(struct aml_nftl_part_t* part)
{
	part->part_current_used_block = out_phy_block_from_free_list(part);
	if(part->part_current_used_block == NULL) {
		NPRINT("%s %d :no free block\n",__func__,__LINE__);
		return 1;
	}
	part->current_block_used_page = 0;
	part->current_block_used_count++;
	return 0;
}

void aml_nftl_ops_init(struct aml_nftl_part_t *part)
{
	part->nand_erase_superblk = nand_erase_superblk;
	part->nand_read_page = nand_read_page;
	part->nand_write_page = nand_write_page;
	part->nand_is_blk_good = nand_is_blk_good;
	part->nand_mark_bad_blk = nand_mark_bad_blk;

	part->nand_read_logic_page = nand_read_logic_page;
	part->nand_write_logic_page = nand_write_logic_page;
	part->nand_copy_page = nand_copy_page;

	return;
}
