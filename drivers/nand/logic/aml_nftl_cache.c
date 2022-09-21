/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_nftl_cache.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#define AML_NFTL_CACHE

#include "aml_nftl.h"

extern uint32 nand_write_logic_page(struct aml_nftl_part_t* part,uint32 page_no,uchar *buf);
extern uint32 nand_read_logic_page(struct aml_nftl_part_t* part,uint32 page_no,uchar *buf);
extern uint32 prio_gc_all(struct aml_nftl_part_t* part);
extern int write_phy_page_map(struct aml_nftl_part_t* part);
extern int set_new_current_block(struct aml_nftl_part_t* part);

uint32 nand_cache_read(struct aml_nftl_part_t* part,uint32 page_no,uint16 bitmap,uchar *buf);
uint32 nand_cache_write(struct aml_nftl_part_t* part,uint32 page_no,uint16 bitmap,uchar *buf);
uint32 nand_op(uchar flag,struct aml_nftl_part_t* part,uint32 start_sector,uint32 len,uchar *buf);
uint32 nftl_nand_flush_write_cache(struct aml_nftl_part_t* part);

_cache_node *search_from_cache_read_list(_cache *cache, uint32 page);
_cache_node *search_from_cache_write_list(_cache *cache, uint32 page);
_cache_node *get_empty_cache_node(_cache *cache);
int add_to_cache_read_list_tail(_cache *cache,_cache_node* cache_node);
int add_to_cache_write_list_tail(_cache *cache,_cache_node* cache_node);
_cache_node* del_from_cache_read_list(_cache *cache,_cache_node* cache_node);
_cache_node* del_from_cache_write_list(_cache *cache,_cache_node* cache_node);
int nftl_cache_init(struct aml_nftl_part_t* part);
int nftl_cache_exit(struct aml_nftl_part_t* part);

int nftl_cache_init(struct aml_nftl_part_t* part)
{
	uint32 i;
	uchar *ptr;
	_cache* cache;

	cache = &part->cache;

	cache->cache_totals = MAX_CACHE_NUM;
	cache->cache_read_nums = 0;
	cache->cache_write_nums = 0;
	cache->cache_buf = NULL;

	cache->cache_buf = aml_nftl_malloc(part->nand_chip->bytes_per_page * MAX_CACHE_NUM);
	if(cache->cache_buf == NULL)
	{
		return 1;
	}

	cache->cache_read_head.cache_read_next = NULL;
	cache->cache_read_head.cache_read_prev = NULL;
	cache->cache_read_head.cache_write_next = NULL;
	cache->cache_read_head.cache_write_prev = NULL;

	cache->cache_write_head.cache_read_next = NULL;
	cache->cache_write_head.cache_read_prev = NULL;
	cache->cache_write_head.cache_write_next = NULL;
	cache->cache_write_head.cache_write_prev = NULL;

	ptr = cache->cache_buf;
	for(i=0;i<MAX_CACHE_NUM;i++)
	{
		cache->cache_node[i].cache_no = i;
		cache->cache_node[i].cache_info = CACHE_EMPTY;
		cache->cache_node[i].page_no = 0xffffffff;
		cache->cache_node[i].cache_read_next = NULL;
		cache->cache_node[i].cache_read_prev = NULL;
		cache->cache_node[i].cache_write_next = NULL;
		cache->cache_node[i].cache_write_prev = NULL;
		cache->cache_node[i].buf = ptr;
		ptr += part->nand_chip->bytes_per_page;
	}

//	aml_nftl_blk->read_data = nand_read;
//	aml_nftl_blk->write_data = nand_write;
//	aml_nftl_blk->flush_write_cache = nand_flush_write_cache;

	return 0;
}

int nftl_cache_exit(struct aml_nftl_part_t* part)
{
	_cache* cache;

	cache = &part->cache;
	aml_nftl_free(cache->cache_buf);
	return 0;

}

_cache_node *search_from_cache_read_list(_cache* cache,uint32 page)
{
	_cache_node* p = &cache->cache_read_head;

	for(p=p->cache_read_next; p; p=p->cache_read_next)
	{
		if(p->page_no == page)
		{
			return p;
		}
	}
	return NULL;
}

_cache_node *search_from_cache_write_list(_cache* cache,uint32 page)
{
	_cache_node * p = &cache->cache_write_head;

	for(p=p->cache_write_next; p; p=p->cache_write_next)
	{
		if(p->page_no == page)
		{
			return p;
		}
	}
	return NULL;
}

_cache_node *get_empty_cache_node(_cache *cache)
{
	uint16 i;
	for(i=0;i<MAX_CACHE_NUM;i++)
	{
		if(cache->cache_node[i].cache_info == CACHE_EMPTY)
			return &cache->cache_node[i];
	}
	return NULL;
}

int add_to_cache_read_list_tail(_cache *cache,_cache_node* cache_node)
{
	_cache_node * p = &cache->cache_read_head;

	while(p->cache_read_next != NULL)
	{
		p = p->cache_read_next;
	}

	p->cache_read_next = cache_node;
	cache_node->cache_read_next = NULL;
	cache_node->cache_read_prev = p;
	cache_node->cache_info = CACHE_READ;
	cache->cache_read_nums += 1;
	return 0;
}

int add_to_cache_write_list_tail(_cache *cache,_cache_node* cache_node)
{
	_cache_node * p = &cache->cache_write_head;

	while(p->cache_write_next != NULL)
	{
		p = p->cache_write_next;
	}

	p->cache_write_next = cache_node;
	cache_node->cache_write_next = NULL;
	cache_node->cache_write_prev = p;
	cache_node->cache_info = CACHE_WRITE;
	cache->cache_write_nums += 1;

	return 0;
}

_cache_node* del_from_cache_read_list(_cache *cache,_cache_node* cache_node)
{
	_cache_node * p = cache_node->cache_read_prev;

	p->cache_read_next = cache_node->cache_read_next;
	if(cache_node->cache_read_next != NULL)
		cache_node->cache_read_next->cache_read_prev = p;

	cache_node->cache_read_next = NULL;
	cache_node->cache_read_prev = NULL;
	cache_node->cache_info = CACHE_EMPTY;
	cache->cache_read_nums -= 1;
	return cache_node;
}

_cache_node* del_from_cache_write_list(_cache *cache,_cache_node* cache_node)
{
	_cache_node * p = cache_node->cache_write_prev;

	p->cache_write_next = cache_node->cache_write_next;
	if(cache_node->cache_write_next != NULL)
		cache_node->cache_write_next->cache_write_prev = p;

	cache_node->cache_write_next = NULL;
	cache_node->cache_write_prev = NULL;
	cache_node->cache_info = CACHE_EMPTY;
	cache->cache_write_nums -= 1;
	return cache_node;
}

uint32 nftl_nand_read(void *priv,uint32 start_sector,uint32 len,unsigned char *buf)
{
	struct aml_nftl_part_t *part = (struct aml_nftl_part_t* )priv;

    if(part->test == 1)
	    NPRINT("read sector:0x%x,len:0x%x\n",start_sector,len);
	return nand_op(0,part,start_sector,len,buf);
}

uint32 nftl_nand_write(void *priv,uint32 start_sector,uint32 len,unsigned char *buf)
{
	uint32 ret;
	struct aml_nftl_part_t *part = (struct aml_nftl_part_t* )priv;
	//NFTL_LINE
    if(part->test == 1)
	    NPRINT("write sector:0x%x,len:0x%x\n",start_sector,len);
	//NFTL_LINE
	ret = nand_op(1,part,start_sector,len,buf);
	//ktime_get_ts(&aml_nftl_blk->ts_write_start);
	return  ret;
}

uint32 nftl_nand_flush(void *priv)
{
	uint32 ret = 0;
	struct aml_nftl_part_t *part = (struct aml_nftl_part_t* )priv;
	NFTL_LINE
	ret = nftl_nand_flush_write_cache(part);

	return ret;

}

uint32 nand_op(uchar flag,struct aml_nftl_part_t* part,uint32 start_sector,uint32 len,uchar *buf)
{
    uint16  temp1,temp2,len_save,bitmap;
    uchar  *buf_save;
    uint32 page_save,page_no,ret;
	uint32 (*nand_op2)(struct aml_nftl_part_t* part,uint32 page_no,uint16 bitmap,uchar *buf);

	//NFTL_LINE
	if(len == 0)
		return 0;
	//NFTL_LINE
	if(flag == 0)
	{
		//NPRINT("read %d, %d, %p\n", start_sector, len, buf);
		nand_op2 = nand_cache_read;
	}
	else
	{
		//NPRINT("write %d, %d, %p\n", start_sector, len, buf);
		nand_op2 = nand_cache_write;
	}
//////////////////////////////////////////////////////////////
	page_no = start_sector / part->nand_chip->sector_per_page;
	page_save = page_no;

    temp1 = (uint16)(start_sector % part->nand_chip->sector_per_page);    //start sector
    temp2 = part->nand_chip->sector_per_page - temp1;                    //sector len
    if(len <= temp2)
    {
        temp2 = len;
        len = 0;
    }
    else
    {
        len -= temp2;
    }

    buf_save = buf;
    len_save = temp2;
    bitmap = (temp1 << 8) | temp2;
	ret = nand_op2(part, page_save, bitmap, buf_save);
	//NFTL_LINE
    if(len == 0)
    {
        return ret;
    }

//////////////////////////////////////////////////////////////
    while(len > part->nand_chip->sector_per_page)
    {
        temp2 = part->nand_chip->sector_per_page;
        page_save += 1;
        buf_save += len_save << SHIFT_PER_SECTOR;
        bitmap = temp2;
        len -= part->nand_chip->sector_per_page;
        len_save = part->nand_chip->sector_per_page;
//        NPRINT("page :%d; bitmap: %x; buf %x\n",page_save,bitmap,buf_save);
	ret = nand_op2(part, page_save, bitmap, buf_save);
	if (ret)
		return ret;
    }

//////////////////////////////////////////////////////////////
    temp2 = (uint16)len;
    page_save += 1;
    buf_save += len_save << SHIFT_PER_SECTOR;
    bitmap = temp2;
//    NPRINT("page :%d; bitmap: %x; buf %x\n",page_save,bitmap,buf_save);
	ret = nand_op2(part,page_save,bitmap,buf_save);
//////////////////////////////////////////////////////////////
	//NPRINT("op end!\n");
    return ret;
}

uint32 nand_cache_read(struct aml_nftl_part_t* part,uint32 page_no,uint16 bitmap,uchar *buf)
{
	uint32 ret;
	uint32 start,len;
	uchar* t_buf;
	_cache_node * node;
	_cache *cache;

	start = bitmap >> 8;
	len = bitmap & 0x00ff;
	start <<= SHIFT_PER_SECTOR;
	len <<= SHIFT_PER_SECTOR;

	cache = &part->cache;

	//NFTL_LINE
    if(part->cfg->nftl_use_cache)
    {
		//NFTL_LINE
	    t_buf = cache->cache_node[0].buf;
	    ret = nand_read_logic_page(part, page_no, t_buf);
	    memcpy(buf, t_buf+start, len);
	    return ret;
    }

	node = search_from_cache_write_list(cache,page_no);
	if(node != NULL)
	{
		t_buf = node->buf;
		memcpy(buf,t_buf+start,len);
		return 0;
	}

	node = search_from_cache_read_list(cache,page_no);
	if(node != NULL)
	{
		del_from_cache_read_list(cache,node);
		t_buf = node->buf;
		memcpy(buf,t_buf+start,len);
		add_to_cache_read_list_tail(cache,node);
		return 0;
	}

	node = get_empty_cache_node(cache);
	if(node == NULL)
	{
		node = del_from_cache_read_list(cache,cache->cache_read_head.cache_read_next);
		if(node == NULL)
		{
			NPRINT("error1 node %d,%d!\n",cache->cache_write_nums,cache->cache_read_nums);
			return 1;
		}
	}

	node->page_no = page_no;
	t_buf = node->buf;
	ret = nand_read_logic_page(part,page_no,t_buf);
	add_to_cache_read_list_tail(cache,node);
	memcpy(buf,t_buf+start,len);

	return ret;
}

uint32 nand_cache_write(struct aml_nftl_part_t* part,uint32 page_no,uint16 bitmap,uchar *buf)
{
	uint32 ret;
	uint16 start,len;
	uchar* t_buf;
	_cache_node * node;
	_cache *cache;

	start = bitmap >> 8;
	len = bitmap & 0x00ff;
	start <<= SHIFT_PER_SECTOR;
	len <<= SHIFT_PER_SECTOR;

	cache = &part->cache;
	//NFTL_LINE
    if(part->cfg->nftl_use_cache)
    {
		//NFTL_LINE
	    t_buf = cache->cache_node[0].buf;
	    nand_read_logic_page(part,page_no,t_buf);
	    memcpy(t_buf+start,buf,len);
	    ret = nand_write_logic_page(part,page_no,t_buf);
	    return ret;
    }
	//NFTL_LINE
// case 1
	if(bitmap == part->nand_chip->sector_per_page)
	{
		node = search_from_cache_write_list(cache,page_no);
		if(node != NULL)
		{
			del_from_cache_write_list(cache,node);
		}

		node = search_from_cache_read_list(cache,page_no);
		if(node != NULL)
		{
			del_from_cache_read_list(cache,node);
		}

		ret = nand_write_logic_page(part,page_no,buf);
		if(ret != 0)
		{
			NPRINT("error1 bitmap %x!\n",bitmap);
			return 1;
		}
		else
		{
			return 0;
		}
	}

// case 2
	node = search_from_cache_write_list(cache,page_no);
	if(node != NULL)
	{
		del_from_cache_write_list(cache,node);
		t_buf = node->buf;
		memcpy(t_buf+start,buf,len);
		add_to_cache_write_list_tail(cache,node);

		node = search_from_cache_read_list(cache,page_no);
		if(node != NULL)
		{
			del_from_cache_read_list(cache,node);
		}
		//nand_flush_write_cache(part);
		return 0;
	}

// case 3
	node = search_from_cache_read_list(cache,page_no);
	if(node != NULL)
	{
		t_buf = node->buf;
		memcpy(t_buf+start,buf,len);
		del_from_cache_read_list(cache,node);
		add_to_cache_write_list_tail(cache,node);
		if(cache->cache_write_nums >= part->cfg->nftl_max_cache_write_num)
		{
			node = del_from_cache_write_list(cache,cache->cache_write_head.cache_write_next);
			t_buf = node->buf;
			ret = nand_write_logic_page(part,node->page_no,t_buf);
			if(ret != 0)
			{
				NPRINT("error2 bitmap %x!\n",bitmap);
				return 1;
			}
		}
		return 0;
	}

// case 4
	if(cache->cache_write_nums >= part->cfg->nftl_max_cache_write_num)
	{
		node = del_from_cache_write_list(cache,cache->cache_write_head.cache_write_next);
		t_buf = node->buf;
		ret = nand_write_logic_page(part,node->page_no,t_buf);
		if(ret != 0)
		{
			NPRINT("error2 bitmap %x!\n",bitmap);
			return 1;
		}
	}
	else
	{
		node = get_empty_cache_node(cache);
		if(node == NULL)
		{
			node = del_from_cache_read_list(cache,cache->cache_read_head.cache_read_next);
		}
	}

	node->page_no = page_no;
	t_buf = node->buf;
	ret = nand_read_logic_page(part,page_no,t_buf);
	if(ret != 0)
	{
		NPRINT("error3 bitmap %x!\n",bitmap);
		return 1;
	}

	memcpy(t_buf+start,buf,len);
	add_to_cache_write_list_tail(cache,node);
	//nand_flush_write_cache(part);

	return 0;
}

uint32 nftl_nand_flush_write_cache(struct aml_nftl_part_t* part)
{
	uint32 ret;
	uchar* t_buf;
	_cache_node * node;
	_cache *cache;

	cache = &part->cache;

	ret = 0;
	while(cache->cache_write_head.cache_write_next != NULL)
	{
		node = del_from_cache_write_list(cache,cache->cache_write_head.cache_write_next);
		t_buf = node->buf;
		ret |= nand_write_logic_page(part,node->page_no,t_buf);
	}

	if(ret != 0)
	{
		NPRINT("nand_flush_write_cache write error\n");
	}

	return ret;
}
