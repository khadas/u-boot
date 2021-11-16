/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_nftl_list.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "aml_nftl.h"

int adjust_invaild_list(struct aml_nftl_part_t* part);
int put_phy_block_to_invalid_page_list(struct aml_nftl_part_t* part,_phy_block_info* phy_block_ptr);
_phy_block_info* out_phy_block_from_invalid_page_list_by_block(struct aml_nftl_part_t* part,_phy_block_info* phy_block_ptr);
_phy_block_info* out_phy_block_from_free_list(struct aml_nftl_part_t* part);
int phy_block_from_invalid_page_incr(struct aml_nftl_part_t* part,_phy_block_info* block);
_phy_block_info* out_phy_block_from_invalid_page_list(struct aml_nftl_part_t* part);

static inline void print_p2l_tbl(struct aml_nftl_part_t* part, _phy_block_info* p_phy_block)
{
	int i;

	if (!p_phy_block)
		return;
	NPRINT("%s(): blk %d\n", __func__, p_phy_block->phy_block.blkNO_in_chip);
	for ( i=0; i<part->nand_chip->pages_per_blk; i+=4) {
		NPRINT("%s(): blk %d\n", __func__, i);
	}
}

static inline int dbg_check_p2l(struct aml_nftl_part_t* part)
{
	int ret = 0;
	int i;

	for (i=0; i< part->nand_chip->pages_per_blk-1; i++) {
		NPRINT("check p2l %d !\n", i);
	}

	return ret;
}

int add_free_list(struct aml_nftl_part_t* part, _phy_block_info* phy_block_ptr)
{
	_phy_block_info* p = &part->free_head;

	for(; p->free_next; p=p->free_next) {
		if(p->free_next->erase_count >= phy_block_ptr->erase_count) {
			phy_block_ptr->free_next = p->free_next;
			p->free_next = phy_block_ptr;
			return 0;
		}
	}
	p->free_next = phy_block_ptr;
	phy_block_ptr->free_next = NULL;
	dbg_check_p2l(part);
	print_p2l_tbl(part, phy_block_ptr);
	return 0;
}

_phy_block_info* del_free_list(struct aml_nftl_part_t* part)
{
	_phy_block_info* p = &part->free_head;

	p = p->free_next;
	if(p != NULL)
		part->free_head.free_next = p->free_next;

	return p;
}

void print_free_list(struct aml_nftl_part_t* part)
{
	_phy_block_info* p = &part->free_head;

	NPRINT("free list: \n");
	for(p=p->free_next; p; p=p->free_next)
	{
		NPRINT("block NO:%4d; erase_count:%3d; ",p->phy_block.blkNO_in_chip,p->erase_count);
		NPRINT("block_used_count:%4d\n",p->block_used_count);
	}
}

int add_block_invalid_list(struct aml_nftl_part_t* part, _phy_block_info* phy_block_ptr)
{
	_phy_block_info* p = phy_block_ptr;

	p->invalid_page_next = phy_block_ptr;
	phy_block_ptr->invalid_page_next = NULL;
	phy_block_ptr->invalid_page_prev = p;
	dbg_check_p2l(part);
	print_p2l_tbl(part, phy_block_ptr);
	return 0;
}

_phy_block_info* del_block_invalid_list(struct aml_nftl_part_t* part)
{
	_phy_block_info* p = &part->invalid_page_head;

	p = p->invalid_page_next;
	if(p != NULL)
	{
		part->invalid_page_head.invalid_page_next = p->invalid_page_next;
		if(p->invalid_page_next != NULL)
			p->invalid_page_next->invalid_page_prev = &part->invalid_page_head;
	}
	dbg_check_p2l(part);
	print_p2l_tbl(part, p);

	return p;
}

_phy_block_info* del_block_invalid_list_by_block(struct aml_nftl_part_t* part,_phy_block_info* phy_block_ptr)
{
	_phy_block_info* p = &part->invalid_page_head;

	for(p=p->invalid_page_next; p; p=p->invalid_page_next)
	{
		if(p == phy_block_ptr)
		{
			p->invalid_page_prev->invalid_page_next = p->invalid_page_next;
			if(p->invalid_page_next != NULL)
				p->invalid_page_next->invalid_page_prev = p->invalid_page_prev;
			p->invalid_page_prev = NULL;
			p->invalid_page_next = NULL;
			break;
		}
	}
	dbg_check_p2l(part);
	print_p2l_tbl(part, p);	
	return p;
}

void print_block_invalid_list(struct aml_nftl_part_t* part)
{
    NPRINT("invalid_page_count list: \n");
}

int add_block_count_list(struct aml_nftl_part_t* part, _phy_block_info* phy_block_ptr)
{
	_phy_block_info* p;

	for(p = &part->block_used_head; p->block_used_next; p=p->block_used_next)
	{
		if(p->block_used_next->block_used_count >= phy_block_ptr->block_used_count)
		{

			p->block_used_next->block_used_prev = phy_block_ptr;
			phy_block_ptr->block_used_next = p->block_used_next;
			p->block_used_next = phy_block_ptr;
			phy_block_ptr->block_used_prev = p;
			return 0;
		}
    }
    p->block_used_next = phy_block_ptr;
    phy_block_ptr->block_used_next = NULL;
    phy_block_ptr->block_used_prev = p;
    	dbg_check_p2l(part);
	print_p2l_tbl(part, p);
	return 0;
}

_phy_block_info* del_block_count_list(struct aml_nftl_part_t* part)
{
	_phy_block_info* p = &part->block_used_head;

	if(p->block_used_next != NULL)
		p->block_used_next = p->block_used_next->block_used_next;
	dbg_check_p2l(part);
	print_p2l_tbl(part, p);

	return p;
}

_phy_block_info* del_block_count_by_block(struct aml_nftl_part_t* part,_phy_block_info* phy_block_ptr)
{
	_phy_block_info* p = &part->block_used_head;

	for(p=p->block_used_next; p; p=p->block_used_next)
	{
		if(p == phy_block_ptr)
		{
			p->block_used_prev->block_used_next = p->block_used_next;
			if(p->block_used_next != NULL)
				p->block_used_next->block_used_prev = p->block_used_prev;
			p->block_used_prev = NULL;
			p->block_used_next = NULL;
			break;
		}
	}
	dbg_check_p2l(part);
	print_p2l_tbl(part, p);
	return p;
}

void print_block_count_list(struct aml_nftl_part_t* part)
{
	_phy_block_info* p = &part->block_used_head;

	NPRINT("block_used_count list: \n");
	for(p=p->block_used_next; p; p=p->block_used_next) {
		NPRINT("[%05d, %04d] ",p->phy_block.blkNO_in_chip, p->block_used_count);
	}
}

int put_phy_block_to_free_list(struct aml_nftl_part_t* part, _phy_block_info* phy_block_ptr)
{
	int ret;

	phy_block_ptr->free_next = NULL;
	ret = add_free_list(part,phy_block_ptr);

	if(ret == 0)
		part->free_block_num ++;
	else
		NPRINT("add free_list error !\n");

	dbg_check_p2l(part);
	print_p2l_tbl(part, phy_block_ptr);
	return ret;
}

_phy_block_info* out_phy_block_from_free_list(struct aml_nftl_part_t* part)
{
	_phy_block_info* block;

	block = del_free_list(part);
	if(block != NULL)
	{
		block->free_next = NULL;
		if(part->free_block_num > 0)
			part->free_block_num --;
	}
	dbg_check_p2l(part);
	print_p2l_tbl(part, block);
	return block;
}

int put_phy_block_to_invalid_page_list(struct aml_nftl_part_t* part,_phy_block_info* phy_block_ptr)
{
	int ret;

	phy_block_ptr->invalid_page_next = NULL;
	phy_block_ptr->invalid_page_prev = NULL;

	ret = add_block_invalid_list(part,phy_block_ptr);
	if(ret != 0)
		NPRINT("add invalid_list error !\n");
	dbg_check_p2l(part);
	print_p2l_tbl(part, phy_block_ptr);

	return ret;
}

_phy_block_info* out_phy_block_from_invalid_page_list(struct aml_nftl_part_t* part)
{
	_phy_block_info* block;

	block = del_block_invalid_list(part );
	if(block != NULL)
	{
		block->invalid_page_next = NULL;
		block->invalid_page_prev = NULL;
		return block;
	}
	dbg_check_p2l(part);
	print_p2l_tbl(part, block);
	return block;
}

_phy_block_info *out_phy_block_from_invalid_page_list_by_block(struct aml_nftl_part_t* part, _phy_block_info* phy_block_ptr)
{
	_phy_block_info* block;

	block = del_block_invalid_list_by_block(part,phy_block_ptr);
	if(block != NULL)
	{
		block->invalid_page_next = NULL;
		block->invalid_page_prev = NULL;
		return block;
	}
	dbg_check_p2l(part);
	print_p2l_tbl(part, block);
	return block;
}

int phy_block_from_invalid_page_incr(struct aml_nftl_part_t* part,_phy_block_info* block)
{
	block->invalid_page_count++;
	dbg_check_p2l(part);
	print_p2l_tbl(part, block);
	return 0;
}

int adjust_invaild_list(struct aml_nftl_part_t* part)
{
	_phy_block_info* phy_block = NULL;

	while(part->invalid_page_head.invalid_page_next!=NULL)
	{
		phy_block = part->invalid_page_head.invalid_page_next;
		if(phy_block->invalid_page_count == part->nand_chip->pages_per_blk)
		{
		}
		else
		{
			break;
		}
	}
	dbg_check_p2l(part);
	print_p2l_tbl(part, phy_block);
	return 0;
}
