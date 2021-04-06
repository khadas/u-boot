/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_nftl_gc.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "aml_nftl.h"

extern int adjust_invaild_list(struct aml_nftl_part_t* part);
extern int put_phy_block_to_invalid_page_list(struct aml_nftl_part_t* part,_phy_block_info* phy_block_ptr);
extern void print_block_invalid_list(struct aml_nftl_part_t* part);
extern uint32 get_special_data_from_oob(uchar* buf);
extern uint32 nand_write_logic_page_no_gc(struct aml_nftl_part_t *part,uint32 page_no,uchar *buf);
extern _phy_block_info* out_phy_block_from_invalid_page_list_by_block(struct aml_nftl_part_t* part,_phy_block_info* phy_block_ptr);
extern _phy_block_info* out_phy_block_from_invalid_page_list(struct aml_nftl_part_t* part);
extern _nand_page* get_logic_page_map(struct aml_nftl_part_t *part,uint32 logic_page);
extern int put_phy_block_to_free_list(struct aml_nftl_part_t* part,_phy_block_info* phy_block_ptr);
extern uint32 get_logic_page_from_oob(uchar* buf);

uint32 garbage_collect_first(struct aml_nftl_part_t* part,_phy_block_info* block,uint16 page_num);
void add_prio_gc(struct aml_nftl_part_t* part,_phy_block_info* block,uint16 type);
uint32 prio_gc_all(struct aml_nftl_part_t* part);
uint32 do_prio_gc(struct aml_nftl_part_t* part);
uint32 gc_all(struct aml_nftl_part_t* part);
uint32 gc_one(struct aml_nftl_part_t* part);
uint32 garbage_collect(struct aml_nftl_part_t* part);
uint32 do_static_wear_leveling(struct aml_nftl_part_t* part);

uint32 garbage_collect(struct aml_nftl_part_t* part)
{
	uint16 page_num;
	_phy_block_info* phy_block;

	if(part->free_block_num <= part->gc_strategy.start_gc_free_blocks)
	{
		part->gc_strategy.process = GC_ON;
	}

	if(part->gc_strategy.process == GC_ON)
	{
		if(part->free_block_num < part->gc_strategy.stop_gc_free_blocks)
		{
			if(part->gc_strategy.flag_gc_block == 0)
			{
				phy_block = part->invalid_page_head.invalid_page_next;
				if(phy_block == NULL)
				{
					return 1;
				}
				if(phy_block->invalid_page_count >= (part->nand_chip->pages_per_blk>>1))
				{
					part->gc_strategy.gc_page = 4;
				}
				else if(phy_block->invalid_page_count >= (part->nand_chip->pages_per_blk>>2))
				{
					part->gc_strategy.gc_page = 8;
				}
				else if(phy_block->invalid_page_count >= (part->nand_chip->pages_per_blk>>3))
				{
					part->gc_strategy.gc_page = 16;
				}
				else
				{
					NPRINT("something is wrong 1! %d:%d\n",part->free_block_num,phy_block->invalid_page_count);
					part->gc_strategy.gc_page = 0xffff;
//					print_block_invalid_list(part->part_phy_block_invalid_page_head);
				}

//				if(part->free_block_num <= (part->gc_strategy.start_gc_free_blocks>>1))
//				{
//					//NPRINT("something is wrong 2! %d:%d\n",part->free_block_num,phy_block->invalid_page_count);
//					part->gc_strategy.gc_page = 0xffff;
//				}

				if(part->free_block_num <= part->cfg->nftl_min_free_block)
				{
					//NPRINT("something is wrong 2! %d:%d\n",part->free_block_num,phy_block->invalid_page_count);
					part->gc_strategy.gc_page = 0xffff;
				}
			}

			page_num = part->gc_strategy.gc_page;
			if(garbage_collect_first(part,NULL,page_num) != 0)
			{
				NPRINT("garbage_collect_first wrong! %d\n",part->free_block_num);
				return 1;
			}
		}
		else
		{
			part->gc_strategy.process = GC_STOP;
			part->gc_strategy.flag_gc_block = 0;
			part->gc_strategy.gc_page = 0;
		}
	}
	return 0;
}
uint32 prio_garbage_collect_no_last_page(struct aml_nftl_part_t* part,_phy_block_info* block,uint16 page_num)
{
	uint32 i = 0,ret = 0;
	_phy_block_info* p_phy_block;
	_physic_op_par phy_op_par;
	p_phy_block = block;	

	if(i == (part->nand_chip->pages_per_blk-1))
	{
		part->gc_strategy.flag_gc_block = 0;
	}
	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = p_phy_block->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	ret = part->nand_erase_superblk(part,&phy_op_par);
	p_phy_block->invalid_page_count = 0;
	p_phy_block->erase_count++;
	if(ret != 0)
	{
		part->nand_mark_bad_blk(part,&phy_op_par);
		NPRINT("prio_garbage_collect nand_mark_bad_blk!\n");
	}
	if (ret == 0){
		put_phy_block_to_free_list(part,p_phy_block);
		NPRINT("%s put_phy_block_to_free_list!\n",__func__);
	}
	return 0;
}

uint32 prio_garbage_collect(struct aml_nftl_part_t* part,_phy_block_info* block,uint16 page_num)
{
	uint32 i,ret = 0;
	uint16 invalid_page_count;
	uchar spare_data[BYTES_OF_USER_PER_PAGE];
	_phy_block_info* p_phy_block;
	_physic_op_par phy_op_par;
	p_phy_block = block;
	phy_op_par.phy_page.Page_NO = part->nand_chip->pages_per_blk - 1;
	phy_op_par.phy_page.blkNO_in_chip = p_phy_block->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	phy_op_par.main_data_addr = part->temp_page_buf;
	phy_op_par.spare_data_addr = spare_data;
	part->nand_read_page(part,&phy_op_par);
	invalid_page_count = 1;
	for(i=0;i<(part->nand_chip->pages_per_blk-1);i++) {
		/* empty  */
	}
	if(invalid_page_count != p_phy_block->invalid_page_count) {
		NPRINT("prio_garbage_collect  invaild_page num error:[%d] [%d]!!\n",invalid_page_count,p_phy_block->invalid_page_count);
		print_block_invalid_list(part);
	}
	
	if(i == (part->nand_chip->pages_per_blk-1)) {
		part->gc_strategy.flag_gc_block = 0;
	}
	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = p_phy_block->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	ret = part->nand_erase_superblk(part,&phy_op_par);
	p_phy_block->invalid_page_count = 0;
	p_phy_block->erase_count++;
	if(ret != 0) {
		part->nand_mark_bad_blk(part,&phy_op_par);
		NPRINT("prio_garbage_collect nand_mark_bad_blk!\n");
	}
	if (ret == 0){
		put_phy_block_to_free_list(part,p_phy_block);
		NPRINT("%s put_phy_block_to_free_list!\n",__func__);
	}
	return 0;
}

uint32 garbage_collect_first(struct aml_nftl_part_t* part,_phy_block_info* block,uint16 page_num)
{
	uint32 i = 0;
	uint16 invalid_page_count;
	uchar spare_data[BYTES_OF_USER_PER_PAGE];
	_phy_block_info* p_phy_block;
	_physic_op_par phy_op_par;

	if(block == NULL) {
		p_phy_block = out_phy_block_from_invalid_page_list(part);
	} else {
		if(block == part->part_current_used_block) {
			return 0;
		}
		p_phy_block = out_phy_block_from_invalid_page_list_by_block(part,block);
	}

	if(p_phy_block == NULL) {
		NPRINT("garbage_collect_first error!!\n");
		return 1;
	}

	phy_op_par.phy_page.Page_NO = part->nand_chip->pages_per_blk - 1;
	phy_op_par.phy_page.blkNO_in_chip = p_phy_block->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	phy_op_par.main_data_addr = part->temp_page_buf;
	phy_op_par.spare_data_addr = spare_data;
	part->nand_read_page(part,&phy_op_par);

	invalid_page_count = 1;

	if(invalid_page_count != p_phy_block->invalid_page_count) {
		NPRINT("garbage_collect_first invaild_page num error:[%d] [%d]!!\n",
		       invalid_page_count,p_phy_block->invalid_page_count);
		print_block_invalid_list(part);
	}

	if(i == (part->nand_chip->pages_per_blk-1)) {
		part->gc_strategy.flag_gc_block = 0;
	}
	put_phy_block_to_invalid_page_list(part,p_phy_block);
	adjust_invaild_list(part);

	return 0;
}

_prio_gc_node* get_empty_gc_node(_prio_gc *prio_gc)
{
	uint16 i;
	for(i=0;i<MAX_PRIO_GC_NUM;i++)
	{
		if(prio_gc->prio_gc_node[i].prio_type == PRIO_NONE)
			return &prio_gc->prio_gc_node[i];
	}
	return NULL;
}

_prio_gc_node *search_gc_node(_prio_gc *prio_gc,_phy_block_info* block)
{
	_prio_gc_node * p = &prio_gc->prio_gc_head;

	for(p=p->prio_gc_next; p; p=p->prio_gc_next)
	{
		if(p->phy_block_info == block)
		{
			return p;
		}
	}
	return NULL;
}

void print_gc_list(_prio_gc *prio_gc)
{
	_prio_gc_node * p = &prio_gc->prio_gc_head;
	NPRINT("print_gc_list list: \n");
	for(p=p->prio_gc_next; p; p=p->prio_gc_next) {
		NPRINT("block NO:%4d; \n ",p->phy_block_info->phy_block.blkNO_in_chip);
	}
}

int add_to_gc_list_tail(_prio_gc *prio_gc,_prio_gc_node* gc_node)
{
	_prio_gc_node * p = &prio_gc->prio_gc_head;

	while(p->prio_gc_next != NULL)
	{
		p = p->prio_gc_next;
	}

	p->prio_gc_next = gc_node;
	gc_node->prio_gc_next = NULL;
	gc_node->prio_gc_prev = p;
	prio_gc->gc_num += 1;
	return 0;
}

_prio_gc_node* del_from_gc_list(_prio_gc *prio_gc,_prio_gc_node* gc_node)
{
	_prio_gc_node * p = gc_node->prio_gc_prev;

	p->prio_gc_next = gc_node->prio_gc_next;
	if(gc_node->prio_gc_next != NULL)
		gc_node->prio_gc_next->prio_gc_prev = p;

	gc_node->prio_gc_next = NULL;
	gc_node->prio_gc_prev = NULL;
	prio_gc->gc_num -= 1;
	return gc_node;
}

void add_prio_gc(struct aml_nftl_part_t* part,_phy_block_info* block,uint16 type)
{
	_prio_gc_node * p;

	p = search_gc_node(&part->prio_gc,block);
	if(p != NULL)
	{
		return;
	}
	p = get_empty_gc_node(&part->prio_gc);
	if(p == NULL)
	{
		return;
	}
	p->phy_block_info = block;
	p->prio_type = type;
	p->prio_gc_next = NULL;
	p->prio_gc_prev = NULL;
	add_to_gc_list_tail(&part->prio_gc,p);
	return;
}

uint32 do_prio_gc(struct aml_nftl_part_t* part)
{
	uint32 ret;
	_prio_gc_node * p;

	if(part->ftl_status.unusual == FTL_UNUSUAL) {
		NPRINT("%s  %d: nftl status is unusual. do not write anything\n",__func__,__LINE__);
		return 0;
	}

	if(part->prio_gc.prio_gc_head.prio_gc_next == NULL) {
		return 0;
	}

	p = del_from_gc_list(&part->prio_gc,part->prio_gc.prio_gc_head.prio_gc_next);

	if(p->phy_block_info == part->part_current_used_block) {
		return 0;
	}

	part->prio_gc.prio_type_now = p->prio_type;

	NPRINT("do_prio_gc block:%d, type:%d\n",p->phy_block_info->phy_block.blkNO_in_chip,p->prio_type);

	ret = prio_garbage_collect(part,p->phy_block_info,0xffff);

	p->prio_type = PRIO_NONE;
	part->prio_gc.prio_type_now = PRIO_NONE;

	return ret;
}

uint32 prio_gc_all(struct aml_nftl_part_t* part)
{
	uint32 i,ret=0;
	for(i=0;i<MAX_PRIO_GC_NUM;i++) {
		ret |= do_prio_gc(part);
	}
	return ret;
}

uint32 do_static_wear_leveling(struct aml_nftl_part_t* part)
{
	_phy_block_info* block_ptr = NULL;
	_phy_block_info* p;
	uint16 earse_max,earse_min;

	if(part->s_wl.s_wl_status != WL_ON)
		return 0;

	if(part->invalid_page_head.invalid_page_next == NULL)
		return 0;

	earse_max = part->invalid_page_head.invalid_page_next->erase_count;
	earse_min = earse_max;

	for(p=&part->invalid_page_head;p->invalid_page_next;p=p->invalid_page_next) {
		if(p->invalid_page_next->erase_count > earse_max) {
			earse_max = p->invalid_page_next->erase_count;
		} else if(p->invalid_page_next->erase_count < earse_min) {
			earse_min = p->invalid_page_next->erase_count;
			block_ptr = p->invalid_page_next;
		} else {
			continue;
		}
	}

	if((earse_max - earse_min) >= part->s_wl.erase_span) {
		add_prio_gc(part,block_ptr,GC_WEAR_LEVELING);
	}

	return 0;
}

uint32 gc_one(struct aml_nftl_part_t* part)
{
	_phy_block_info* phy_block;

	phy_block = part->invalid_page_head.invalid_page_next;
	if(phy_block->invalid_page_count >= 10) {
		if(garbage_collect_first(part,NULL,0xffff) != 0) {
			NPRINT("gc_one error!\n");
		} else {
			NPRINT("gc_one ok!\n");
		}
		return 0;
	} else {
		return 1;
	}
}

uint32 gc_all(struct aml_nftl_part_t* part)
{
	uint32 ret = 0;

	while(!ret)
	{
		ret = gc_one(part);
	}
	NPRINT("gc all end\n");
	return 0;
}
