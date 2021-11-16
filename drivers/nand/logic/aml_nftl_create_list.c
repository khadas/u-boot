/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_nftl_create_list.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "aml_nftl.h"

extern uint32 get_logic_page_from_oob(uchar* buf);
extern uint32 get_special_data_from_oob(uchar* buf);
extern uint16 get_erase_count_from_oob(uchar* buf);
extern _phy_block_info* del_block_invalid_list(struct aml_nftl_part_t* part);
extern _phy_block_info* del_block_invalid_list_by_block(struct aml_nftl_part_t* part,_phy_block_info* phy_block_ptr);
extern void print_block_invalid_list(struct aml_nftl_part_t* part);
extern void print_block_count_list(struct aml_nftl_part_t* part);
extern int put_phy_block_to_free_list(struct aml_nftl_part_t* part,_phy_block_info* phy_block_ptr);
extern _phy_block_info* out_phy_block_from_free_list(struct aml_nftl_part_t* part);
extern int put_phy_block_to_invalid_page_list(struct aml_nftl_part_t* part,_phy_block_info* phy_block_ptr);
extern _phy_block_info* out_phy_block_from_invalid_page_list(struct aml_nftl_part_t* part);
extern uint32 get_block_used_count_from_oob(uchar* buf);
extern int add_block_count_list(struct aml_nftl_part_t* part, _phy_block_info* phy_block_ptr);
extern int adjust_invaild_list(struct aml_nftl_part_t* part);
extern uint32 garbage_collect_first(struct aml_nftl_part_t* part,_phy_block_info* block,uint16 page_num);;
extern void print_free_list(struct aml_nftl_part_t* part);
extern _phy_block_info* del_block_count_list(struct aml_nftl_part_t* part);
extern uint32 gc_all(struct aml_nftl_part_t* part);
extern void set_spare_data(uchar* buf,uint32 data,uchar offset,uchar num);
extern uint32 get_spare_data(uchar* buf,uchar offset,uchar num);
extern int init_logic_page_map(struct aml_nftl_part_t *part,uint32 total_pages);
extern _nand_page* get_logic_page_map(struct aml_nftl_part_t *part,uint32 logic_page);
extern void add_prio_gc(struct aml_nftl_part_t* part,_phy_block_info* block,uint16 type);

_phy_block_info* get_phy_block_addr(struct aml_nftl_part_t *part,uint16 block);
uint32 is_phy_block_valid(_phy_block_info* p_phy_block_info,struct aml_nftl_part_t * part);
uint32 is_last_phy_block(_phy_block_info* p_phy_block_info,struct aml_nftl_part_t * part);
uint32 recover_current_block_mapping(struct aml_nftl_part_t * part,_phy_block_info* phy_block_ptr);
void recover_block_log2pyh_mapping(struct aml_nftl_part_t * part,_phy_block_info* phy_block_ptr);
uint32 do_write_error_in_create_list(struct aml_nftl_part_t* part,_phy_block_info* block1, _phy_block_info* block2,uint16 page_num);
uint32 get_used_page_num(struct aml_nftl_part_t * part,_phy_block_info* phy_block_ptr);
uint32 get_used_block_count(struct aml_nftl_part_t * part,_phy_block_info* phy_block_ptr);
void print_nftl_part(struct aml_nftl_part_t * part);
int part_param_exit(struct aml_nftl_part_t *part);
uint32 get_vaild_blocks(struct aml_nftl_part_t * part,uint32 start_block,uint32 blocks);
uint32 do_write_error_in_create_list(struct aml_nftl_part_t* part,_phy_block_info* block1, _phy_block_info* block2,uint16 page_num);
#define DBG_SCAN_SKIP_PAGE	(0)

int part_param_init(struct aml_nftl_part_t *part,uint16 start_block,uint32_t logic_sects,uint32_t backup_cap_in_sects)
{
	uint32_t i,total_pages;

	part->temp_page_buf = (uchar *)aml_nftl_malloc(part->nand_chip->bytes_per_page);
	if (part->temp_page_buf == NULL){
	    NPRINT(" part_param_init error1\n");
		return -ENOMEM;
	}

	part->logic_cap_in_sects = logic_sects;
	part->backup_cap_in_sects = backup_cap_in_sects;
	part->part_start_blk_NO.blkNO_in_chip = start_block;
	part->part_start_phy_block = get_phy_block_addr(part,start_block);

	part->blocks = 0;
	part->bad_block = 0;
	part->free_block_num = 0;
	part->part_end_phy_block = NULL;

	total_pages = part->logic_cap_in_sects / part->nand_chip->sector_per_page;

#if   MALLOC_FLAG
	if( init_logic_page_map(part,total_pages) != 0) {
		NPRINT(" part_param_init error2\n");
		return -ENOMEM;
	}

#else
	part->part_logic_page_map = (_nand_page*)aml_nftl_malloc(total_pages * sizeof(_nand_page));
	if (part->part_logic_page_map == NULL) {
		NPRINT(" part_param_init error2 %d\n",total_pages);
		return -ENOMEM;
	}
	memset((void*) part->part_logic_page_map, 0xff, total_pages*sizeof(_nand_page));
#endif
	part->total_reads = 0;
	part->total_write = 0;
	part->test = 0;

	part->s_wl.erase_threshold = part->nand_chip->max_erase_times >> 1;
	part->s_wl.erase_span  = part->nand_chip->max_erase_times >> 4;
	part->s_wl.s_wl_status = WL_STOP;

	part->part_current_used_block = NULL;

	part->free_head.free_next = NULL;
	part->free_head.invalid_page_next = NULL;
	part->free_head.invalid_page_prev = NULL;
	part->free_head.block_used_next = NULL;
	part->free_head.block_used_prev = NULL;


	part->invalid_page_head.free_next = NULL;
	part->invalid_page_head.invalid_page_next = NULL;
	part->invalid_page_head.invalid_page_prev = NULL;
	part->invalid_page_head.block_used_next = NULL;
	part->invalid_page_head.block_used_prev = NULL;

	part->block_used_head.free_next = NULL;
	part->block_used_head.invalid_page_next = NULL;
	part->block_used_head.invalid_page_prev = NULL;
	part->block_used_head.block_used_next = NULL;
	part->block_used_head.block_used_prev = NULL;

	part->current_block_used_page = 0xffff;
	part->current_block_used_count = 0xffffffff;

	part->prio_gc.gc_num = 0;
	for(i=0;i<MAX_PRIO_GC_NUM;i++) {
		part->prio_gc.prio_gc_node[i].gc_no = i;
		part->prio_gc.prio_gc_node[i].prio_type = PRIO_NONE;
		part->prio_gc.prio_gc_node[i].phy_block_info = NULL;
		part->prio_gc.prio_gc_node[i].prio_gc_next = NULL;
		part->prio_gc.prio_gc_node[i].prio_gc_prev = NULL;
	}

	part->prio_gc.prio_gc_head.gc_no = 0xff;
	part->prio_gc.prio_type_now = PRIO_NONE;
	part->prio_gc.prio_gc_head.prio_type = PRIO_NONE;
	part->prio_gc.prio_gc_head.phy_block_info = NULL;
	part->prio_gc.prio_gc_head.prio_gc_next = NULL;
	part->prio_gc.prio_gc_head.prio_gc_prev = NULL;

	part->ftl_status.unusual = FTL_NORMAL;

	return 0;
}

int part_param_exit(struct aml_nftl_part_t *part)
{
    uint32 i;

#if   MALLOC_FLAG
    for(i=0;i<MAX_LOGIC_MAP_NUM;i++)
    {
        if(part->part_logic_page_map[i] != NULL)
            aml_nftl_free(part->part_logic_page_map[i]);
    }
#else

    aml_nftl_free(part->part_logic_page_map);

#endif

    aml_nftl_free(part->temp_page_buf);
    return 0;
}

uint32 is_no_use_device(struct aml_nftl_part_t * part,uint32 size)
{
	uint32 total_blocks;
	uchar spare_data[BYTES_OF_USER_PER_PAGE];
	_physic_op_par phy_op_par;
	_phy_block_info* p_phy_block_info;

	p_phy_block_info = part->part_start_phy_block;

	if(part->logic_cap_in_sects != 0) {
		total_blocks = (part->logic_cap_in_sects+part->backup_cap_in_sects) / part->nand_chip->sector_per_page;
		total_blocks /= part->nand_chip->pages_per_blk;
	} else {
		total_blocks = 0xffffffff;
	}

	for(part->blocks=0; part->blocks<total_blocks; p_phy_block_info++) {
		part->part_end_phy_block = p_phy_block_info;
		phy_op_par.phy_page.Page_NO = 0;
		phy_op_par.phy_page.blkNO_in_chip = p_phy_block_info->phy_block.blkNO_in_chip;
		phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
		phy_op_par.main_data_addr = part->temp_page_buf;
		phy_op_par.spare_data_addr = spare_data;
		if(part->nand_is_blk_good(part,&phy_op_par) == RET_YES) {
			part->nand_read_page(part,&phy_op_par);
		} else {
			p_phy_block_info->info = BLOCK_IS_BAD;
		}
		part->blocks++;
		if(is_last_phy_block(p_phy_block_info,part)==RET_YES) {
			break;
		}
	}

	return 1;
}

uint32 get_vaild_blocks(struct aml_nftl_part_t * part, uint32 start_block, uint32 blocks)
{
	uint32 total_blocks,i,num;
	_physic_op_par phy_op_par;
	_phy_block_info* p_phy_block_info;

	p_phy_block_info = (_phy_block_info*)(&part->nand_chip->nand_block_info[start_block]);
	num = blocks - start_block;
	total_blocks = num;

	for(i=start_block; i<num; i++) {
		phy_op_par.phy_page.blkNO_in_chip = p_phy_block_info->phy_block.blkNO_in_chip;
		if(part->nand_is_blk_good(part,&phy_op_par) == FACTORY_BAD_BLOCK_ERROR) {
			total_blocks--;
			if(total_blocks == 0) {
				return 0;
			}
		}
		if(is_last_phy_block(p_phy_block_info,part)==RET_YES) {
			break;
		}
		p_phy_block_info++;
	}
	return total_blocks;
}

uint32 create_part_list_first(struct aml_nftl_part_t * part,uint32 size)
{
	uint32 ret,total_blocks,i;
	uchar spare_data[BYTES_OF_USER_PER_PAGE];
	_physic_op_par phy_op_par;
	_phy_block_info* p_phy_block_info;

	NPRINT("create_part_list_first.\n");

	p_phy_block_info = part->part_start_phy_block;

	if(part->logic_cap_in_sects != 0)
	{
		total_blocks = (part->logic_cap_in_sects+part->backup_cap_in_sects) / part->nand_chip->sector_per_page;
		total_blocks /= part->nand_chip->pages_per_blk;
	}
	else
	{
		total_blocks = 0xffffffff;
	}

	for(part->blocks=0; part->blocks<total_blocks; p_phy_block_info++)
	{
		part->part_end_phy_block = p_phy_block_info;
		phy_op_par.phy_page.Page_NO = 0;
		phy_op_par.phy_page.blkNO_in_chip = p_phy_block_info->phy_block.blkNO_in_chip;
		phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
		phy_op_par.main_data_addr = part->temp_page_buf;
		phy_op_par.spare_data_addr = spare_data;
		if(part->nand_is_blk_good(part,&phy_op_par) == RET_YES)
		{
			ret = part->nand_erase_superblk(part,&phy_op_par);
			if(ret != 0)
			{
				p_phy_block_info->info = BLOCK_IS_BAD;
				part->nand_mark_bad_blk(part,&phy_op_par);
			}
			else
			{
				part->blocks++;
				p_phy_block_info->info = BLOCK_NO_USED;
				p_phy_block_info->erase_count = 0;
				if(part->blocks == 1)
				{
					memset((void*) spare_data,0xff,BYTES_OF_USER_PER_PAGE);
					set_spare_data(spare_data,size,2,sizeof(size));
					phy_op_par.phy_page.Page_NO = 0;
					phy_op_par.phy_page.blkNO_in_chip = p_phy_block_info->phy_block.blkNO_in_chip;
					phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
					phy_op_par.main_data_addr = part->temp_page_buf;
					phy_op_par.spare_data_addr = spare_data;
					for(i=0;i<part->nand_chip->pages_per_blk;i++)
					{
						part->nand_write_page(part,&phy_op_par);
						phy_op_par.phy_page.Page_NO++;
					}
					part->part_start_phy_block = p_phy_block_info + 1;
				}
			}
		}
		else
		{
			p_phy_block_info->info = BLOCK_IS_BAD;
		}
		p_phy_block_info->invalid_page_count = 0xffff;
		p_phy_block_info->free_next = NULL;
		p_phy_block_info->invalid_page_next = NULL;
		p_phy_block_info->invalid_page_prev = NULL;

		part->part_end_phy_block = p_phy_block_info;

		if(is_last_phy_block(p_phy_block_info,part)==RET_YES)
		{
			break;
		}
	}

	if(part->blocks > 1)
	{
		part->blocks -= 1;
	}
	else
	{
	    NPRINT("no block\n");
		return 1;
	}

	if(total_blocks == 0xffffffff)
	{
		return 0;
	}

	if(is_last_phy_block(p_phy_block_info,part)==RET_YES)
	{
		return 0;
	}

	for(; p_phy_block_info->phy_block.blkNO_in_chip < part->nand_chip->blk_per_chip; p_phy_block_info++)
	{
		phy_op_par.phy_page.Page_NO = 0;
		phy_op_par.phy_page.blkNO_in_chip = p_phy_block_info->phy_block.blkNO_in_chip;
//		phy_op_par.phy_page.chip_NO = p_phy_block_info->phy_block.chip_NO;
		phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
		phy_op_par.main_data_addr = part->temp_page_buf;
		phy_op_par.spare_data_addr = spare_data;
		if(part->nand_is_blk_good(part,&phy_op_par) == RET_YES)
		{
			ret = part->nand_erase_superblk(part,&phy_op_par);
			if(ret != 0)
			{
				p_phy_block_info->info = BLOCK_IS_BAD;
				part->nand_mark_bad_blk(part,&phy_op_par);
			}
			else
			{
				memset((void*) spare_data,0xff,BYTES_OF_USER_PER_PAGE);
				phy_op_par.phy_page.Page_NO = 0;
				phy_op_par.phy_page.blkNO_in_chip = p_phy_block_info->phy_block.blkNO_in_chip;
				phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
				phy_op_par.main_data_addr = part->temp_page_buf;
				phy_op_par.spare_data_addr = spare_data;
				part->nand_write_page(part,&phy_op_par);
				break;
			}
		}
		else
		{
			p_phy_block_info->info = BLOCK_IS_BAD;
		}
	}
	return 0;
}

uint32 check_block_accroding_p2l(_phy_block_info* p_phy_block_info, struct aml_nftl_part_t * part)
{
	uint32 ret = 0;
	uint32 page;

	for (page=0; page<part->nand_chip->pages_per_blk - 1; page++)  {
	}
	return ret;
}

uint32 create_part_list_last(struct aml_nftl_part_t * part)
{
	uint32 total_blocks;
	_physic_op_par phy_op_par;
	uchar spare_data[BYTES_OF_USER_PER_PAGE];
	_phy_block_info* p_phy_block_info;

	p_phy_block_info = part->part_start_phy_block;

	for(part->blocks = 0; p_phy_block_info->phy_block.blkNO_in_chip < part->nand_chip->blk_per_chip; p_phy_block_info++) {
		phy_op_par.phy_page.Page_NO = 0;
		phy_op_par.phy_page.blkNO_in_chip = p_phy_block_info->phy_block.blkNO_in_chip;
		phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
		phy_op_par.main_data_addr = part->temp_page_buf;
		phy_op_par.spare_data_addr = spare_data;
		phy_op_par.op_ret_sta =0;
		if(part->nand_is_blk_good(part,&phy_op_par) == RET_YES) {
			part->blocks++;
			part->nand_read_page(part, &phy_op_par);
			part->part_end_phy_block = p_phy_block_info;
		} else {
			p_phy_block_info->info = BLOCK_IS_BAD;
			p_phy_block_info->erase_count = 0;
			part->bad_block++;
		}
		if(phy_op_par.op_ret_sta == 1)
			add_prio_gc(part,p_phy_block_info,GC_READ_RECLAIM);
	}

	total_blocks = part->logic_cap_in_sects / part->nand_chip->sector_per_page;
	total_blocks /= part->nand_chip->pages_per_blk;

	NPRINT("-blks(%d), total_blocks(%d)\n",part->blocks,total_blocks);
	NPRINT("aaaa00\n");

	if(part->blocks <= total_blocks) {
		NPRINT("this part not enough data block!!\n");
		return 1;
	}

	part->backup_cap_in_sects = (part->blocks-total_blocks) * part->nand_chip->sector_per_page * part->nand_chip->pages_per_blk;

	if((part->blocks-total_blocks) < (part->cfg->nftl_min_free_block_num -1))
	{
		NPRINT("this part not enough free block %d , %d!!\n",part->blocks,total_blocks);
		return 1;
	}
	part->gc_strategy.start_gc_free_blocks = (part->blocks-total_blocks) / part->cfg->nftl_part_reserved_block_ratio;
	if(part->gc_strategy.start_gc_free_blocks < part->cfg->nftl_gc_threshold_free_block_num)
	{
		part->gc_strategy.start_gc_free_blocks = part->cfg->nftl_gc_threshold_free_block_num;
	}

	part->gc_strategy.stop_gc_free_blocks = ((part->blocks-total_blocks) * part->cfg->nftl_gc_threshold_ratio_numerator)/part->cfg->nftl_gc_threshold_ratio_denominator;
	if(part->gc_strategy.stop_gc_free_blocks < (part->gc_strategy.start_gc_free_blocks+2))
	{
		part->gc_strategy.stop_gc_free_blocks = part->gc_strategy.start_gc_free_blocks + 2;
	}
	part->gc_strategy.process = GC_STOP;
	part->gc_strategy.flag_gc_block = 0;

	for(p_phy_block_info=part->block_used_head.block_used_next; p_phy_block_info!=NULL; p_phy_block_info=p_phy_block_info->block_used_next) {
		phy_op_par.phy_page.Page_NO = part->nand_chip->pages_per_blk - 1;
		phy_op_par.phy_page.blkNO_in_chip = p_phy_block_info->phy_block.blkNO_in_chip;
		phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
		phy_op_par.main_data_addr = part->temp_page_buf;
		phy_op_par.spare_data_addr = spare_data;
		part->nand_read_page(part,&phy_op_par);
	}

	NPRINT("all block full!!\n");
	p_phy_block_info = NULL;
	part->part_current_used_block = NULL;

	for(p_phy_block_info=part->block_used_head.block_used_next; p_phy_block_info!=NULL; p_phy_block_info=p_phy_block_info->block_used_next) {
		if(part->part_current_used_block != p_phy_block_info)
			put_phy_block_to_invalid_page_list(part,p_phy_block_info);
	}

	adjust_invaild_list(part);

	return 0;
}

u32 do_write_error_in_create_list_for_discard(struct aml_nftl_part_t *part, _phy_block_info *block1, _phy_block_info *block2, u16 page_num)
{
	u32 ret, i;
	_phy_block_info *p_phy_block_free;
	u8 spare_data[BYTES_OF_USER_PER_PAGE];
	_physic_op_par phy_op_par;
	p_phy_block_free = block2;

	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = p_phy_block_free->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	ret = part->nand_erase_superblk(part, &phy_op_par);
	p_phy_block_free->invalid_page_count = 0;
	p_phy_block_free->erase_count++;

	if (ret != 0) {
		PRINT("do_write_error_in_create_list erase error!\n");
		return 1;
	}

	/* recopy: */
	/* page_num--; */
	for (i = 0; i < page_num; i++) {
		phy_op_par.phy_page.Page_NO = i;
		phy_op_par.phy_page.blkNO_in_chip = block1->phy_block.blkNO_in_chip;
		phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
		phy_op_par.main_data_addr = part->temp_page_buf;
		phy_op_par.spare_data_addr = spare_data;
		ret = part->nand_read_page(part, &phy_op_par);
		if (ret) {
			PRINT("%s: %d current blk is %d\n", __func__, __LINE__, part->part_current_used_block->phy_block.blkNO_in_chip);
			break;
		}
	}
	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = block1->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	ret = part->nand_erase_superblk(part, &phy_op_par);
	block1->invalid_page_count = 0;
	block1->erase_count++;
	if (ret == 0)
		put_phy_block_to_free_list(part, block1);
	else {
		PRINT("do_write_error_in_create_list erase error,mark bad now!\n");
	}
	return 0;
}


uint32 create_part_list(struct aml_nftl_part_t * part)
{
	uint32 total_blocks;
	_physic_op_par phy_op_par;
	uchar spare_data[BYTES_OF_USER_PER_PAGE];
	_phy_block_info* p_phy_block_info;

	p_phy_block_info = part->part_start_phy_block;

	for(part->blocks = 0; p_phy_block_info->phy_block.blkNO_in_chip < part->nand_chip->blk_per_chip; p_phy_block_info++) {
		phy_op_par.phy_page.Page_NO = 0;
		phy_op_par.phy_page.blkNO_in_chip = p_phy_block_info->phy_block.blkNO_in_chip;
		phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
		phy_op_par.main_data_addr = part->temp_page_buf;
		phy_op_par.spare_data_addr = spare_data;
		phy_op_par.op_ret_sta =0;
		if(part->nand_is_blk_good(part,&phy_op_par) == RET_YES) {
			part->blocks++;
			part->nand_read_page(part, &phy_op_par);
			part->part_end_phy_block = p_phy_block_info;
		} else {
			p_phy_block_info->info = BLOCK_IS_BAD;
			p_phy_block_info->erase_count = 0;
			part->bad_block++;
		}
		if(phy_op_par.op_ret_sta == 1)
			add_prio_gc(part,p_phy_block_info,GC_READ_RECLAIM);
	}

	total_blocks = part->logic_cap_in_sects / part->nand_chip->sector_per_page;
	total_blocks /= part->nand_chip->pages_per_blk;

	NPRINT("-blks(%d), total_blocks(%d)\n",part->blocks,total_blocks);
	NPRINT("aaaa00\n");

	if(part->blocks <= total_blocks) {
		NPRINT("this part not enough data block!!\n");
		return 1;
	}

	part->backup_cap_in_sects = (part->blocks-total_blocks) * part->nand_chip->sector_per_page * part->nand_chip->pages_per_blk;

	if((part->blocks-total_blocks) < (part->cfg->nftl_min_free_block_num -1))
	{
		NPRINT("this part not enough free block %d , %d!!\n",part->blocks,total_blocks);
		return 1;
	}
	part->gc_strategy.start_gc_free_blocks = (part->blocks-total_blocks) / part->cfg->nftl_part_reserved_block_ratio;
	if(part->gc_strategy.start_gc_free_blocks < part->cfg->nftl_gc_threshold_free_block_num)
	{
		part->gc_strategy.start_gc_free_blocks = part->cfg->nftl_gc_threshold_free_block_num;
	}

	part->gc_strategy.stop_gc_free_blocks = ((part->blocks-total_blocks) * part->cfg->nftl_gc_threshold_ratio_numerator)/part->cfg->nftl_gc_threshold_ratio_denominator;
	if(part->gc_strategy.stop_gc_free_blocks < (part->gc_strategy.start_gc_free_blocks+2))
	{
		part->gc_strategy.stop_gc_free_blocks = part->gc_strategy.start_gc_free_blocks + 2;
	}
	part->gc_strategy.process = GC_STOP;
	part->gc_strategy.flag_gc_block = 0;

	for(p_phy_block_info=part->block_used_head.block_used_next; p_phy_block_info!=NULL; p_phy_block_info=p_phy_block_info->block_used_next) {
		phy_op_par.phy_page.Page_NO = part->nand_chip->pages_per_blk - 1;
		phy_op_par.phy_page.blkNO_in_chip = p_phy_block_info->phy_block.blkNO_in_chip;
		phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
		phy_op_par.main_data_addr = part->temp_page_buf;
		phy_op_par.spare_data_addr = spare_data;
		part->nand_read_page(part,&phy_op_par);
	}

	NPRINT("all block full!!\n");
	p_phy_block_info = NULL;
	part->part_current_used_block = NULL;

	for(p_phy_block_info=part->block_used_head.block_used_next; p_phy_block_info!=NULL; p_phy_block_info=p_phy_block_info->block_used_next) {
		if(part->part_current_used_block != p_phy_block_info)
			put_phy_block_to_invalid_page_list(part,p_phy_block_info);
	}

	adjust_invaild_list(part);

	create_part_list_last(part);

	return 0;
}

uint32 do_write_error_in_create_list(struct aml_nftl_part_t* part,_phy_block_info* block1, _phy_block_info* block2,uint16 page_num)
{
	uint32 ret,i,logic_page = 0;
	_phy_block_info* p_phy_block_free;
	_physic_op_par phy_op_par;
	_physic_op_par phy_op_par2;

	p_phy_block_free = block2;

	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = p_phy_block_free->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	ret = part->nand_erase_superblk(part,&phy_op_par);
	p_phy_block_free->invalid_page_count = 0;
	p_phy_block_free->erase_count++;

	if(ret != 0)
	{
		part->nand_mark_bad_blk(part,&phy_op_par);
		NPRINT("do_write_error_in_create_list erase error!\n");
		return 1;
	}

	page_num--;
	for(i=0;i<page_num;i++) {
		phy_op_par.phy_page.Page_NO = i;
		phy_op_par.phy_page.blkNO_in_chip = block1->phy_block.blkNO_in_chip;
		phy_op_par2.phy_page.Page_NO = i;
		phy_op_par2.phy_page.blkNO_in_chip = p_phy_block_free->phy_block.blkNO_in_chip;
		ret = part->nand_copy_page(part, &phy_op_par,
					   &phy_op_par2,
					   part->temp_page_buf,
					   p_phy_block_free->block_used_count,
					   p_phy_block_free->erase_count,
					   logic_page);
		if (ret != 0) {
			phy_op_par.phy_page.Page_NO = 0;
			phy_op_par.phy_page.blkNO_in_chip =
				p_phy_block_free->phy_block.blkNO_in_chip;
			phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
			part->nand_mark_bad_blk(part, &phy_op_par);
			NPRINT("%s erase error!\n", __func__);
			return 1;
		}
	}

	phy_op_par.phy_page.Page_NO = 0;
	phy_op_par.phy_page.blkNO_in_chip = block1->phy_block.blkNO_in_chip;
	phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
	part->nand_mark_bad_blk(part,&phy_op_par);

	for(i=0;i<page_num;i++) {
		phy_op_par.phy_page.Page_NO = i;
		phy_op_par.phy_page.blkNO_in_chip = block1->phy_block.blkNO_in_chip;
		phy_op_par2.phy_page.Page_NO = i;
		phy_op_par2.phy_page.blkNO_in_chip = p_phy_block_free->phy_block.blkNO_in_chip;
		ret = part->nand_copy_page(part, &phy_op_par, &phy_op_par2,
					   part->temp_page_buf,
					   p_phy_block_free->block_used_count,
					   p_phy_block_free->erase_count, logic_page);
		if (ret != 0) {
			phy_op_par.phy_page.Page_NO = 0;
			phy_op_par.phy_page.blkNO_in_chip =
				p_phy_block_free->phy_block.blkNO_in_chip;
			phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
			part->nand_mark_bad_blk(part, &phy_op_par);
			NPRINT("%s erase error!\n", __func__);
			return 1;
		}
	}

	return 0;
}

uint32 is_last_phy_block(_phy_block_info* p_phy_block_info,struct aml_nftl_part_t * part)
{
	if(p_phy_block_info->phy_block.blkNO_in_chip == (part->nand_chip->blk_per_chip-1) )
	{
		return RET_YES;
	}
	return RET_NO;
}

uint32 is_phy_block_valid(_phy_block_info* p_phy_block_info,struct aml_nftl_part_t * part)
{
	if(p_phy_block_info->phy_block.blkNO_in_chip < part->nand_chip->blk_per_chip)
	{
		return RET_YES;
	}
	return RET_NO;
}


_phy_block_info* get_phy_block_addr(struct aml_nftl_part_t *part,uint16 block)
{
	return (_phy_block_info*)(&part->nand_chip->nand_block_info[block]);
}


uint32 get_used_page_num(struct aml_nftl_part_t * part,_phy_block_info* phy_block_ptr)
{
	uint32 i,ret;
	_physic_op_par phy_op_par;
	uchar spare_data[BYTES_OF_USER_PER_PAGE];
	for(i=0;i<part->nand_chip->pages_per_blk;i++)
	{
		phy_op_par.phy_page.Page_NO = i;
		phy_op_par.phy_page.blkNO_in_chip = phy_block_ptr->phy_block.blkNO_in_chip;
		phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
		phy_op_par.main_data_addr = part->temp_page_buf;
		phy_op_par.spare_data_addr = spare_data;

		ret = part->nand_read_page(part,&phy_op_par);
		if (ret) {
			/*do somthing.*/
		}
	}
	return i;
}


uint32 get_used_block_count(struct aml_nftl_part_t * part,_phy_block_info* phy_block_ptr)
{
	uint32 oob_special_data = 0;
	return oob_special_data;
}


uint32 recover_current_block_mapping(struct aml_nftl_part_t * part,_phy_block_info* phy_block_ptr)
{
	_physic_op_par phy_op_par;
	int i;
	uchar spare_data[BYTES_OF_USER_PER_PAGE];

	for(i=0;i<part->nand_chip->pages_per_blk;i++)
	{
		phy_op_par.phy_page.Page_NO = i;
		phy_op_par.phy_page.blkNO_in_chip = phy_block_ptr->phy_block.blkNO_in_chip;
		phy_op_par.page_bitmap = part->nand_chip->bitmap_per_page;
		phy_op_par.main_data_addr = part->temp_page_buf;
		phy_op_par.spare_data_addr = spare_data;
		part->nand_read_page(part,&phy_op_par);
	}
	return i;
}

void recover_block_log2pyh_mapping(struct aml_nftl_part_t * part,_phy_block_info* phy_block_ptr)
{
	part->part_current_used_block = phy_block_ptr;
	part->current_block_used_page = recover_current_block_mapping(part,phy_block_ptr);
	part->current_block_used_count = phy_block_ptr->block_used_count;
}


void dump_l2p(struct aml_nftl_part_t * part)
{
	unsigned int pages;
	
	if (part == NULL)
		return;
	pages = part->logic_cap_in_sects / part->nand_chip->sector_per_page;
	
	PRINT("L2P of part <%s>, pages %d, len %d\n", part->name,  pages, 0);
	
	if (1) {
		unsigned int pages_per_malloc;
		unsigned int malloc_count, i;
		
		pages_per_malloc = MAX_MALLOC_BYTES / sizeof(_nand_page);
		malloc_count = pages / pages_per_malloc;
		if (pages % pages_per_malloc)
			malloc_count++;
		
		for (i = 0; i < malloc_count; i++) {
			if (i == (malloc_count - 1)) {
			}
			else {
			}
		}
	}

	return;
}

void print_nftl_part(struct aml_nftl_part_t * part)
{
	NPRINT("part->nand_chip->blk_per_chip: %d \n",part->nand_chip->blk_per_chip);
	NPRINT("part->nand_chip->bytes_per_page: %d \n",part->nand_chip->bytes_per_page);
	NPRINT("part->nand_chip->pages_per_blk: %d \n",part->nand_chip->pages_per_blk);
	NPRINT("part->test: %d \n",part->test);
	NPRINT("part->part_no: %d \n",part->part_no);
	NPRINT("part->part_attr: %d \n",part->part_attr);
	NPRINT("part->blocks: %d \n",part->blocks);
	NPRINT("part->bad_block: %d \n",part->bad_block);
	NPRINT("part->logic_cap_in_sects: %d \n",part->logic_cap_in_sects);
	NPRINT("part->backup_cap_in_sects: %d \n",part->backup_cap_in_sects);
	NPRINT("part->free_block_num: %d \n",part->free_block_num);
	NPRINT("part->gc_strategy.start_gc_free_blocks: %d \n",part->gc_strategy.start_gc_free_blocks);
	NPRINT("part->gc_strategy.stop_gc_free_blocks: %d \n",part->gc_strategy.stop_gc_free_blocks);
	NPRINT("part->gc_strategy.gc_page: %d \n",part->gc_strategy.gc_page);
	NPRINT("part->gc_strategy.process: %d \n",part->gc_strategy.process);
	NPRINT("part->prio_gc.prio_type : %d \n",part->prio_gc.prio_type_now );
	NPRINT("part->total_reads : %d \n",part->total_reads);
	NPRINT("part->total_write: %d \n",part->total_write);
	NPRINT("part->part_start_phy_block->phy_block.blkNO_in_chip: %d \n",part->part_start_phy_block->phy_block.blkNO_in_chip);
	NPRINT("part->part_end_phy_block->phy_block.blkNO_in_chip: %d \n",part->part_end_phy_block->phy_block.blkNO_in_chip);
	NPRINT("part->part_current_used_block: %p \n",part->part_current_used_block);
	NPRINT("part->part_current_used_block->phy_block.blkNO_in_chip: %d \n",part->part_current_used_block->phy_block.blkNO_in_chip);
	NPRINT("part->current_block_used_page: %d \n",part->current_block_used_page);
	NPRINT("part->current_block_used_count: %d \n",part->current_block_used_count);
	NPRINT("part->temp_page_buf: %p \n",part->temp_page_buf);
	dump_l2p(part);
}

