/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_nftl_common.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "aml_nftl.h"

extern int nand_read_page(struct aml_nftl_part_t *part,_physic_op_par *p);
extern int nand_write_page(struct aml_nftl_part_t *part,_physic_op_par *p);

int nand_copy_page(struct aml_nftl_part_t *part,_physic_op_par* a, _physic_op_par* b,uchar *buf,uint32 block_used_count,uint16 erase_times,uint32 logic_no);
void set_oob_special_page(struct aml_nftl_part_t *part,uchar* buf,uint32 special_data,uint32 block_used_count,uint16 erase_times,uchar type);
uint32 get_logic_page_from_oob(uchar* buf);
uint16 get_erase_count_from_oob(uchar* buf);
void set_oob_logic_page(struct aml_nftl_part_t *part,uchar* buf,uint32 logic_page,uint32 block_used_count,uint16 erase_times);
uint32 get_special_data_from_oob(uchar* buf);
_nand_page* get_logic_page_map(struct aml_nftl_part_t *part,uint32 logic_page);
int init_logic_page_map(struct aml_nftl_part_t *part,uint32 total_pages);
int check_logic_page_map(struct aml_nftl_part_t *part);

int nand_copy_page(struct aml_nftl_part_t *part,_physic_op_par* a, _physic_op_par* b,uchar *buf,uint32 block_used_count,uint16 erase_times,uint32 logic_no)
{
	int ret;
    uchar spare[BYTES_OF_USER_PER_PAGE];
    uint32 logic_page;

    ret = 0;
    a->page_bitmap = part->nand_chip->bitmap_per_page;
    b->page_bitmap = part->nand_chip->bitmap_per_page;
    a->main_data_addr = buf;
    a->spare_data_addr = spare;
    b->main_data_addr = buf;
    b->spare_data_addr = spare;
    ret = nand_read_page(part,a);
   if(ret == -EUCLEAN)
	ret = 0;
   else if(ret){
	NPRINT("%s %d: nand read page failed\n",__func__,__LINE__);
    }
    if(ret)
	logic_page = logic_no;
     else
	logic_page = get_logic_page_from_oob(spare);

    set_oob_logic_page(part,spare,logic_page,block_used_count,erase_times);
   // ret |= nand_write_page(part,b);
    ret = nand_write_page(part,b);

    return ret;
}

void set_spare_data(uchar* buf,uint32 data,uchar offset,uchar num)
{
    uint32 shift,i;
    for(i=0;i<num;i++)
    {
        shift = num-i;
        shift -= 1;
        shift <<= 3;
        buf[offset+i] = data>>shift;
    }
}

uint32 get_spare_data(uchar* buf,uchar offset,uchar num)
{
    uint32 data,i;
    for(i=0,data=0;i<num;i++)
    {
        data <<= 8;
        data |= buf[offset+i];
    }
    return data;

}

void set_oob_logic_page(struct aml_nftl_part_t *part,uchar* buf,uint32 logic_page,uint32 block_used_count,uint16 erase_times)
{
}

void set_oob_special_page(struct aml_nftl_part_t *part,uchar* buf,uint32 special_data,uint32 block_used_count,uint16 erase_times,uchar type)
{

}

uchar get_bad_flag_from_oob(uchar* buf)
{
	return 0;
}

uint32 get_block_used_count_from_oob(uchar* buf)
{
	uint32 block_used_count = 0;

	return block_used_count;
}

uint32 get_logic_page_from_oob(uchar* buf)
{
	uint32 logic_page = 0;

	logic_page &= 0x0fffffff;
	return logic_page;
}

uint32 get_special_data_from_oob(uchar* buf)
{
	uint32 special_data = 0;

	return special_data;
}

uint16 get_erase_count_from_oob(uchar* buf)
{
	uint16 erase_count = 0;

	return erase_count;
}

_nand_page* get_logic_page_map(struct aml_nftl_part_t *part,uint32 logic_page)
{
#if   MALLOC_FLAG
    _nand_page* page = NULL;

    return page;

#endif
}

int init_logic_page_map(struct aml_nftl_part_t *part,uint32 total_pages)
{
#if   MALLOC_FLAG
	uint32 i;

	for(i=0;i<MAX_LOGIC_MAP_NUM;i++)
		part->part_logic_page_map[i] = NULL;

	if(total_pages != 0) {
		NPRINT(" part_param_init init_logic_page_map error %d\n",total_pages);
		return -ENOMEM;
	}
#endif
	return 0;
}

int check_logic_page_map(struct aml_nftl_part_t *part)
{
    return 0;
}
