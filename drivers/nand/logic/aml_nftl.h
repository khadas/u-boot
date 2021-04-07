/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_nftl.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __AML_NFTL_H
#define __AML_NFTL_H

#include "../include/amlnf_dev.h"
#include "../include/aml_nftl_cfg.h"	/*for config settings.*/
#include "aml_nftl_type.h"

#define DRV_FTL_VERSION	   	 "1.01.002"

typedef enum {
    AML_NFTL_SUCCESS                =0,
    AML_NFTL_FAILURE                =1,
    AML_NFTL_INVALID_PARTITION      =2,
    AML_NFTL_INVALID_ADDRESS        =3,
    AML_NFTL_DELETED_SECTOR         =4,
    AML_NFTL_FLUSH_ERROR            =5,
    AML_NFTL_UNFORMATTED            =6,
    AML_NFTL_UNWRITTEN_SECTOR       =7,
    AML_NFTL_PAGENOTFOUND           =0x08,
    AML_NFTL_NO_FREE_BLOCKS         =0x10,
    AML_NFTL_STRUCTURE_FULL         =0x11,
    AML_NFTL_NO_INVALID_BLOCKS      =0x12,
    AML_NFTL_SECTORDELETED          =0x50,
    AML_NFTL_ABT_FAILURE            =0x51,
    AML_NFTL_MOUNTED_PARTITION      =0,
    AML_NFTL_UNMOUNTED_PARTITION    =1,
    AML_NFTL_SPAREAREA_ERROR        =0x13,
    AML_NFTL_STATIC_WL_FINISH       =0x14,
    AML_NFTL_BLKNOTFOUND            =0x15,
    AML_NFTL_WRITE_ERROR			=5
}t_AML_NFTL_error;

/* debug line for nftl... */
#define NPRINT printf
//#define NFTL_LINE			do {aml_nand_msg("%s() %d", __FUNCTION__, __LINE__);} while(0);
#define NFTL_LINE			do {aml_nand_msg("%s() %d", __FUNCTION__, __LINE__);} while(0);
//#define NCACHE_LINE			do {aml_nand_msg("%s() %d", __FUNCTION__, __LINE__);} while(0);
#define NCACHE_LINE
//#define NFTL_CREAT_LINE 	do {aml_nand_msg("%s() %d", __FUNCTION__, __LINE__);} while(0);
#define NFTL_CREAT_LINE
//#define NFTL_CREAT_LINE2 	do {aml_nand_msg("%s() %d", __FUNCTION__, __LINE__);} while(0);
#define NFTL_CREAT_LINE2 	do {;} while(0);
//#define NFTL_CREAT_LINE3 	do {aml_nand_msg("%s() %d", __FUNCTION__, __LINE__);} while(0);
#define NFTL_CREAT_LINE3 	do {;} while(0);

//#define NFTL_LINE

#define MAX_PAGE_PER_BLOCK                1024
#define MAX_CACHE_NUM       			  32
#define MAX_PRIO_GC_NUM        		      10

#define BLOCK_FOR_BAD_TABLE               0xee



#define SPARE_OFFSET_LOGIC_PAGE             0



//#define SPARE_OFFSET_NO                     10
//#define SPARE_OFFSET_TYPE                   11
//#define SPARE_OFFSET_MAGIC                  12


#define BLOCK_NO_USED                     1
#define BLOCK_HAVE_USED                   0
#define BLOCK_IS_BAD                      0xff

#define RET_YES                           1
#define RET_NO                            0

#define FTL_NORMAL       0
#define FTL_UNUSUAL      1

//nand block
typedef struct{
    uint16  blkNO_in_chip;
}_nand_block;


typedef struct __phy_block  _phy_block_info;

typedef struct{
    uint16          pages_per_blk;
    uint16          page_offset_in_nextblk;
	uint16          *phy_conversion_table;
	uchar (*nand_erase_superblk)(_physic_op_par *p);
	uchar (*nand_read_page)(_physic_op_par *p);
	uchar (*nand_write_page)(_physic_op_par *p);
	uchar (*nand_copy_page)(_physic_op_par* a, _physic_op_par* b,uchar *buf);
}_nand_safe_property;


struct  nand_chip_t{
    uchar           nand_id[8];
    uchar           sector_per_page;
    uchar           frequence_par;
    uchar           bytes_user_date;
    uchar           bytes_ecc_date;
    uchar           ecc_bits;            //8,24,32
    uchar           ecc_for_bytes;       //512 or 1024
	uchar           two_plane_support;
	uint16          max_erase_times;
	uint16          two_plane_offset;
 	uint16          pages_per_blk;
    uint16          page_offset_in_nextblk;
    uint16          blk_per_chip;
    uint32          bytes_per_page;
    uint16          bitmap_per_page;
	_nand_safe_property  nand_safe;
	_phy_block_info* nand_block_info;
};

typedef struct __phy_block{
	_nand_block			phy_block;
	uint16				info;
	uint16				invalid_page_count;       //[0,pages_per_blk-1]
	uint16				erase_count;            //[0,10000]
	sint32				block_used_count;
	_phy_block_info*	invalid_page_next;
	_phy_block_info*	invalid_page_prev;
	_phy_block_info*	free_next;
	_phy_block_info*	block_used_next;
	_phy_block_info*	block_used_prev;

}_phy_block_t;

#define  GC_ON             0
#define  GC_STOP           1
//Schmidt oscillator
typedef struct{
    uint16   start_gc_free_blocks;
    uint16   stop_gc_free_blocks;
    uint16   process;
    uint16   flag_gc_block;
    uint16   gc_page;
}_gc_strategy;


#define   PRIO_NONE              0
#define   GC_READ_RECLAIM        1
#define   GC_WEAR_LEVELING       2

typedef struct __prio_gc_node  _prio_gc_node;
typedef struct __prio_gc_node
{
	uint16                   gc_no;
	uint16                   prio_type;
	_phy_block_info*	     phy_block_info;
	_prio_gc_node*           prio_gc_next;
	_prio_gc_node*           prio_gc_prev;
}_prio_gc_node_t;

typedef struct
{
	uint16                   gc_num;
	uint16                   prio_type_now;
	_prio_gc_node            prio_gc_node[MAX_PRIO_GC_NUM];
	_prio_gc_node            prio_gc_head;
}_prio_gc;



#define CACHE_WRITE          1
#define CACHE_READ           0
#define CACHE_EMPTY          0xff

typedef struct __cache_node  _cache_node;
typedef struct __cache_node
{
	uint16 cache_no;
	uint16 cache_info;                  //[CACHE_EMPTY,CACHE_READ,CACHE_WRITE]
	uint32 page_no;
	uchar* buf;
	_cache_node*   cache_read_next;
	_cache_node*   cache_read_prev;
	_cache_node*   cache_write_next;
	_cache_node*   cache_write_prev;
}_cache_node_t;

typedef struct
{
	uint16 cache_totals;
	uint16 cache_read_nums;
	uint16 cache_write_nums;
	uchar* cache_buf;
	_cache_node   cache_node[MAX_CACHE_NUM];
	_cache_node   cache_read_head;
	_cache_node   cache_write_head;
}_cache;


#define  WL_ON             1
#define  WL_STOP           0

typedef struct
{
	uint16    erase_threshold;
	uint16    erase_span;
	uint16    s_wl_status;
}_s_wl;

typedef struct
{
	unsigned char unusual;
}_ftl_status;
#define  MAX_MALLOC_BYTES   0x100000
#define  MAX_LOGIC_MAP_NUM  12
#define  MALLOC_FLAG         1

struct aml_nftl_part_t;

struct aml_nftl_part_t{
//	struct mtd_info *               mtd;
    void*                           priv;
//	struct mutex 			        aml_nftl_lock;
	struct  nand_chip_t*  			nand_chip;
	char                            version[16];
	uchar                  		    part_no;
	uchar                  		    part_attr;
	uint16                  		blocks;
	uint16                          bad_block;
	uint16                          free_block_num;
	uint32          				logic_cap_in_sects;
	uint32                          backup_cap_in_sects;
	_nand_block                     part_start_blk_NO;
	_phy_block_info* 				part_start_phy_block;
	_phy_block_info* 				part_end_phy_block;
#if  MALLOC_FLAG
    _nand_page*                     part_logic_page_map[MAX_LOGIC_MAP_NUM];
#else
    _nand_page*                     part_logic_page_map;
#endif

    _phy_block_info                 free_head;
	_phy_block_info                	invalid_page_head;
	_phy_block_info					block_used_head;
	_phy_block_info  *			    part_current_used_block;
	uint16                          current_block_used_page;
	uint32                          current_block_used_count;
	uchar*                          temp_page_buf;

    uint32  						total_reads;
    uint32  						total_write;

    uint32          				test;
    _ftl_status		ftl_status;

	_gc_strategy                    gc_strategy;
	_prio_gc                        prio_gc;
	_s_wl                           s_wl;
	_cache  				        cache;
	struct _nftl_cfg*               cfg;

//    struct class                    debug;
	int (*nand_erase_superblk)(struct aml_nftl_part_t* part,_physic_op_par *p);
	int (*nand_read_page)(struct aml_nftl_part_t* part,_physic_op_par *p);
	int (*nand_write_page)(struct aml_nftl_part_t* part,_physic_op_par *p);
    	int (*nand_copy_page)(struct aml_nftl_part_t *part,_physic_op_par* a, _physic_op_par* b,
				uchar *buf,uint32 block_used_count,uint16 erase_times,uint32 logic_no);
	int (*nand_is_blk_good)(struct aml_nftl_part_t* part,_physic_op_par *p);
	int (*nand_mark_bad_blk)(struct aml_nftl_part_t* part,_physic_op_par *p);
	uint32 (*nand_write_logic_page)(struct aml_nftl_part_t* part,uint32 page_no,uchar *buf);
	uint32 (*nand_read_logic_page)(struct aml_nftl_part_t* part,uint32 page_no,uchar *buf);

};

//extern void *aml_nftl_malloc(uint32 size);
//extern void aml_nftl_free(const void *ptr);
//extern int aml_nand_msg(const char * fmt,...);
extern uint32 nftl_nand_read(void *priv,uint32 start_sector,uint32 len,unsigned char *buf);
extern uint32 nftl_nand_write(void *priv,uint32 start_sector,uint32 len,unsigned char *buf);

extern uint32 do_static_wear_leveling(struct aml_nftl_part_t* part);
extern uint32 garbage_collect(struct aml_nftl_part_t* part);
extern uint32 do_prio_gc(struct aml_nftl_part_t* part);

extern void* aml_nftl_get_part_priv(void * _part);
extern void aml_nftl_add_part_total_read(void * _part);
extern void aml_nftl_add_part_total_write(void * _part);
extern uint32 aml_nftl_get_part_cap(void * _part);
extern int aml_nftl_start(void* priv,void* cfg,void ** ppart,uint64_t size,unsigned erasesize,\
			unsigned writesize,unsigned oobavail,char* name,int no,char type);
#endif


