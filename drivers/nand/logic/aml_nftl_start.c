/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_nftl_start.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "aml_nftl.h"

extern int nftl_cache_init(struct aml_nftl_part_t * part);
extern int nftl_cache_exit(struct aml_nftl_part_t * part);
extern uint32 get_vaild_blocks(struct aml_nftl_part_t * part,uint32 start_block,uint32 blocks);
extern int part_param_init(struct aml_nftl_part_t *part,uint16 start_block,uint32_t logic_sects,uint32_t backup_cap_in_sects);
extern uint32 is_no_use_device(struct aml_nftl_part_t * part,uint32 size);
extern uint32 create_part_list_first(struct aml_nftl_part_t * part,uint32 size);
extern uint32 create_part_list(struct aml_nftl_part_t * part);
extern int part_param_exit(struct aml_nftl_part_t *part);
extern void aml_nftl_ops_init(struct aml_nftl_part_t *part);

unsigned int  get_reserved_block_num(uint64_t physize,uint32_t phys_erase_shift,struct aml_nftl_part_t* part)
{
	unsigned int block_num=0,size_in_blk=0;
	size_in_blk = physize >> phys_erase_shift;
	part->free_block_num = (size_in_blk /part->cfg->nftl_part_reserved_block_ratio);
	block_num = part->free_block_num;
	return block_num;
}

int aml_nftl_start(void* priv,void* cfg, void ** ppart,uint64_t size,unsigned erasesize,unsigned writesize,unsigned oobavail,char* name,int no,char type)
{
	uint32_t size_in_blk,total_block,tmp_block,temp,i;
	uint32_t phys_erase_shift,phys_write_shift,phys_page_shift;
	uint32_t ret;
	struct aml_nftl_part_t* part;

	struct amlnand_phydev* nand_phydev = (struct amlnand_phydev*)priv;
	u64 phy_dev_size = 0, tmp_off_size = 0;
	uint64_t tmp_compensation_pages=0;
	u32 phy_dev_block = 0, phy_off_block;
	*ppart = aml_nftl_malloc(sizeof(struct aml_nftl_part_t));
	part = *ppart;
	if (!part)
		return -ENOMEM;

	memset(part,0,sizeof(struct aml_nftl_part_t));

	part->priv = priv;
	part->cfg = (struct _nftl_cfg *)cfg;
	/* notice! not the one in physic layer.*/
	part->nand_chip = aml_nftl_malloc(sizeof(struct nand_chip_t));
	if (!part->nand_chip)
		return -ENOMEM;

	memcpy(&part->version[0], DRV_FTL_VERSION, 8);
	part->version[8] = '\0';
	NPRINT("nftl version %s\n",part->version);

	if (!writesize || !erasesize)
		return -1;

	phys_write_shift = ffs(writesize) - 1;
	part->nand_chip->bytes_per_page = writesize;
	part->nand_chip->bytes_user_date = oobavail;
	phys_erase_shift = ffs(erasesize) - 1;
	size_in_blk =  (size >> phys_erase_shift);
	if (size_in_blk <= part->cfg->nftl_min_free_block_num)
		return -EPERM;
	phys_page_shift = phys_erase_shift - phys_write_shift;
	part->nand_chip->blk_per_chip = size_in_blk;
	part->nand_chip->pages_per_blk = erasesize / writesize;
	part->nand_chip->sector_per_page =  writesize >> 9;
	part->nand_chip->bitmap_per_page = part->nand_chip->sector_per_page;
	part->nand_chip->max_erase_times = 3000;

	total_block = size_in_blk;

	part->nand_chip->nand_block_info = (_phy_block_info*)aml_nftl_malloc((total_block+1) * sizeof(_phy_block_info));
	memset((void*)part->nand_chip->nand_block_info,0xff,total_block*sizeof(_phy_block_info));

	for(i=0;i<total_block+1;i++)
	{
		part->nand_chip->nand_block_info[i].phy_block.blkNO_in_chip = i;
		part->nand_chip->nand_block_info[i].invalid_page_count = 0;
		part->nand_chip->nand_block_info[i].invalid_page_next = NULL;
		part->nand_chip->nand_block_info[i].invalid_page_prev = NULL;
		part->nand_chip->nand_block_info[i].free_next = NULL;
		part->nand_chip->nand_block_info[i].block_used_next = NULL;
		part->nand_chip->nand_block_info[i].block_used_prev = NULL;
	}

	aml_nftl_ops_init(*ppart);

	total_block = get_vaild_blocks(*ppart,0,size_in_blk);
	if(total_block == 0)
	    NPRINT("get_vaild_blocks fail\n");

	if(total_block != size_in_blk)
		NPRINT("size_in_blk %d; total_block %d; \n",size_in_blk,total_block);

	size_in_blk = total_block;
	if(is_phydev_off_adjust()){
		tmp_block = ((size_in_blk -part->cfg->nftl_part_adjust_block_num)*part->cfg->nftl_part_reserved_block_ratio) /(1+part->cfg->nftl_part_reserved_block_ratio);
		part->free_block_num = size_in_blk - tmp_block;
	}else{
		part->free_block_num = (size_in_blk / part->cfg->nftl_part_reserved_block_ratio);
	}

	/*calculate the phy size*/
	for(i = 0; i < nand_phydev->nr_partitions; i++){
		phy_dev_size += nand_phydev->partitions[i].size;
	}
	if(phy_dev_size != -1) {
		if (phys_page_shift < 32) {
			tmp_compensation_pages =
			((phy_dev_size >> phys_write_shift) +
			 (1 << phys_page_shift) - 1) / (1 << phys_page_shift);
			tmp_off_size = tmp_compensation_pages << phys_write_shift;
			phy_off_block =
			(tmp_off_size + (1 << phys_erase_shift) - 1) >> phys_erase_shift;
			if (ffs(tmp_off_size) - 1 == phys_erase_shift)
				phy_off_block++;
			phy_dev_block = phy_off_block +
			((phy_dev_size + (1 << phys_erase_shift) - 1) >> phys_erase_shift);
		}
		NPRINT("get the phy_dev_size=%llxH tmp_off_size=%llxH phy_dev_block=%xH",phy_dev_size,tmp_off_size,phy_dev_block);
		if(phy_dev_block + part->cfg->nftl_min_free_block_num > size_in_blk )
		{
			NPRINT("not enough free space %xH + %xH > %xH",phy_dev_block,\
				part->cfg->nftl_min_free_block_num,size_in_blk);
				return -ENOMEM;
		}
		else{
			part->free_block_num = size_in_blk - phy_dev_block;
		}
	}
	else{
		NPRINT("The partition size == -1");
	}
	NPRINT("nftl start:size_in_blk=%d,free_block_num=%d",size_in_blk,part->free_block_num);

	temp = erasesize  >> 9;
	ret = part_param_init(*ppart,0,(size_in_blk - part->free_block_num)*temp,part->free_block_num*temp);
    if(ret != 0)
    {
        NPRINT(" part_param_init fail!\n");
        return ret;
    }

    part->part_no = no;
    part->part_attr = no;

//	if(memcmp(name, "system", 6)==0)
//	{
//		part->part_no = 0;
//		part->part_attr = 0;
//	}
//	if(memcmp(name, "cache", 5)==0)
//	{
//		part->part_no = 1;
//		part->part_attr = 0;
//	}
//	if(memcmp(name, "userdata", 8)==0)
//	{
//		part->part_no = 2;
//		part->part_attr = 0;
//	}
//	if(memcmp(name, "NFTL_Part", 9)==0)
//	{
//		part->part_no = 3;
//		part->part_attr = 0;
//	}
//	if(memcmp(name, "preloaded", 9)==0)
//	{
//		part->part_no = 4;
//		part->part_attr = 0;
//	}

    if(part->cfg->nftl_need_erase != 0)
    {
	    if(is_no_use_device(*ppart,size_in_blk) == 1)
	    {
	    	ret = create_part_list_first(*ppart,size_in_blk);
	    	if(ret != 0)
	    	{
	    	    NPRINT(" create_part_list_first fail!\n");
	    	    return ret;
	    	}
	    }
	}

	ret = create_part_list(*ppart);
    if(ret != 0)
    {
        NPRINT(" create_part_list fail!\n");
        return ret;
    }

    nftl_cache_init(*ppart);
    return 0;
}

void aml_nftl_part_release(struct aml_nftl_part_t *part)
{
	if(part != NULL){
		nftl_cache_exit(part);
		part_param_exit(part);
		aml_nftl_free(part->nand_chip->nand_block_info);
		aml_nftl_free(part->nand_chip);
		aml_nftl_free(part);
	}

}
