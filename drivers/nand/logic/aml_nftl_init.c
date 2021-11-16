/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_nftl_init.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "aml_nftl.h"

static int nftl_num = 0;
extern uint32_t nftl_nand_flush(void *priv);
extern void aml_nftl_part_release(struct aml_nftl_part_t *part);

extern struct list_head nlogic_dev_list;

#ifndef AML_NAND_UBOOT

ssize_t show_part_struct(struct class *class,struct class_attribute *attr,char *buf)
{
    return 0;
}

ssize_t show_list(struct class *class, struct class_attribute *attr, const char *buf)
{
    //struct amlnf_dev* nf_dev = container_of(class, struct amlnf_dev, debug);

    //print_free_list(nftl_dev -> aml_nftl_part);
    //print_block_invalid_list(nftl_dev -> aml_nftl_part);
//    print_block_count_list(nftl_dev -> aml_nftl_part);

    return 0;
}

ssize_t do_gc_all(struct class *class, struct class_attribute *attr, const char *buf)
{
    //struct amlnf_dev* nf_dev = container_of(class, struct amlnf_dev, debug);

	//gc_all(nftl_dev -> aml_nftl_part);
    return 0;
}

ssize_t do_gc_one(struct class *class, struct class_attribute *attr, const char *buf)
{
    //struct amlnf_dev* nf_dev = container_of(class, struct amlnf_dev, debug);

	//gc_one(nftl_dev -> aml_nftl_part);
    return 0;
}

ssize_t do_test(struct class *class, struct class_attribute *attr,	const char *buf, size_t count)
{
    //struct amlnf_dev* nf_dev = container_of(class, struct amlnf_dev, debug);
    //struct aml_nftl_part_t *part = nftl_dev -> aml_nftl_part;

	//uint32_t num;

	//sscanf(buf, "%x", &num);

	//aml_nftl_set_part_test(part,num);

    return count;
}

int logicdev_bg_handle(void *priv)
{
	struct aml_nftl_part_t* part = (struct aml_nftl_part_t*)priv;

#if  SUPPORT_WEAR_LEVELING
	if(do_static_wear_leveling(part) != 0){
		NPRINT("aml_nftl_thread do_static_wear_leveling error!\n");
	}
#endif

	if(garbage_collect(part) != 0){
		NPRINT("aml_nftl_thread garbage_collect error!\n");
	}

	if(do_prio_gc(part) != 0){
		NPRINT("aml_nftl_thread do_prio_gc error!\n");
	}

	return 0;
}
#endif

static int logicdev_read_data(struct amlnf_logicdev_t *amlnf_logicdev, uint32_t start_sector, uint32_t len,unsigned char *buf)
{
	int ret = 0;

	amlnf_get_logicdev(amlnf_logicdev);
	//NFTL_LINE
	ret = nftl_nand_read(amlnf_logicdev->priv, start_sector, len, buf);
	//NFTL_LINE
	amlnf_free_logicdev(amlnf_logicdev);

	return ret;
}

static int logicdev_write_data(struct amlnf_logicdev_t *amlnf_logicdev, uint32_t start_sector, uint32_t len,unsigned char *buf)
{
	int ret = 0;

	amlnf_get_logicdev(amlnf_logicdev);

	ret = nftl_nand_write(amlnf_logicdev->priv, start_sector, len, buf);

	amlnf_free_logicdev(amlnf_logicdev);


	return ret;
}

static int logicdev_flush(struct amlnf_logicdev_t *amlnf_logicdev)
{
	uint32_t ret = 0;

	amlnf_get_logicdev(amlnf_logicdev);

	ret = nftl_nand_flush(amlnf_logicdev->priv);
	if(ret != 0)
	{
		NPRINT("logicdev_flush flush cache error\n");
	}
	amlnf_free_logicdev(amlnf_logicdev);


	return ret;
}

int aml_nftl_initialize(struct amlnf_logicdev_t *amlnf_logicdev,int no)
{
	struct amlnand_phydev *nand =  amlnf_logicdev->nand_dev;
	uint32_t ret;

	if (nand->oobavail < MIN_BYTES_OF_USER_PER_PAGE){
		NPRINT("%s() %d: not enough oob(%d) for nftl ", __func__, __LINE__, nand->oobavail);
		return -EPERM;
	}

	amlnf_logicdev->nftl_cfg.nftl_use_cache = NFTL_DONT_CACHE_DATA;
	amlnf_logicdev->nftl_cfg.nftl_support_gc_read_reclaim = SUPPORT_GC_READ_RECLAIM;
	amlnf_logicdev->nftl_cfg.nftl_support_wear_leveling = SUPPORT_WEAR_LEVELING;
	amlnf_logicdev->nftl_cfg.nftl_need_erase = NFTL_ERASE;
	if(!is_phydev_off_adjust()){
		amlnf_logicdev->nftl_cfg.nftl_part_reserved_block_ratio = 8;
	}else{
		amlnf_logicdev->nftl_cfg.nftl_part_reserved_block_ratio = 10;
	}
	amlnf_logicdev->nftl_cfg.nftl_part_adjust_block_num = get_adjust_block_num();
	amlnf_logicdev->nftl_cfg.nftl_min_free_block_num = MIN_FREE_BLOCK_NUM;
	amlnf_logicdev->nftl_cfg.nftl_min_free_block = MIN_FREE_BLOCK;
	amlnf_logicdev->nftl_cfg.nftl_gc_threshold_free_block_num = GC_THRESHOLD_FREE_BLOCK_NUM ;
	amlnf_logicdev->nftl_cfg.nftl_gc_threshold_ratio_numerator = GC_THRESHOLD_RATIO_NUMERATOR;
	amlnf_logicdev->nftl_cfg.nftl_gc_threshold_ratio_denominator = GC_THRESHOLD_RATIO_DENOMINATOR;
	amlnf_logicdev->nftl_cfg.nftl_max_cache_write_num = MAX_CACHE_WRITE_NUM;

	ret = aml_nftl_start((void*)amlnf_logicdev->nand_dev,&amlnf_logicdev->nftl_cfg,(void*)&amlnf_logicdev->priv,\
				nand->size,nand->erasesize,nand->writesize,nand->oobavail,(char*)nand->name,no,10);
	if(ret != 0)
	    return ret;

	amlnf_logicdev->read_data = logicdev_read_data;
	amlnf_logicdev->write_data = logicdev_write_data;
	amlnf_logicdev->flush = logicdev_flush;

	return 0;
}

uint32_t amlnf_read_sector(struct amlnf_dev* dev, uint32_t start_sector, uint32_t len, unsigned char *buf)
{
	struct amlnf_logicdev_t *amlnf_logicdev = dev->logicdev;
	NFTL_LINE
	return amlnf_logicdev->read_data(amlnf_logicdev, (dev->offset_sector + start_sector), len, buf);
}

uint32_t amlnf_write_sector(struct amlnf_dev* dev, uint32_t start_sector, uint32_t len, unsigned char *buf)
{
	struct amlnf_logicdev_t *amlnf_logicdev = dev->logicdev;

	return amlnf_logicdev->write_data(amlnf_logicdev, (dev->offset_sector + start_sector), len, buf);
}

uint32_t amlnf_flush(struct amlnf_dev* dev)
{
	struct amlnf_logicdev_t *amlnf_logicdev = dev->logicdev;

	return amlnf_logicdev->flush(amlnf_logicdev);
}

int aml_create_nftl_dev(struct amlnf_logicdev_t *amlnf_logicdev)
{
	int i;
	uint32_t size;
	uint32_t offset;
	struct amlnf_partition* partition;
	struct amlnf_dev* nf_dev;

	//INIT_LIST_HEAD (&nf_dev_list);

	size = aml_nftl_get_part_cap(amlnf_logicdev->priv);
	offset = 0;

	for(i=0;i<amlnf_logicdev->nand_dev->nr_partitions;i++){
		partition = &amlnf_logicdev->nand_dev->partitions[i];
		nf_dev = aml_nftl_malloc(sizeof(struct amlnf_dev));
		if(!nf_dev){
			NPRINT("aml_create_nftl_dev fail 1\n");
			return 1;
		}

		memset(nf_dev, 0, sizeof(struct amlnf_dev));

		memcpy(nf_dev->name, partition->name,strlen(partition->name));
		aml_nand_dbg("%s() %d: name %s\n", __func__, __LINE__, nf_dev->name);
		if(size > (partition->size >> 9)) {
			nf_dev->size_sector = partition->size >> 9;
			size -= partition->size >> 9;
		} else{
			nf_dev->size_sector = size;
			if((i+1) !=amlnf_logicdev->nand_dev->nr_partitions)	{
				NPRINT("aml_create_nftl_dev fail 2\n");
				aml_nftl_free(nf_dev);
				return 1;
			}
		}

		nf_dev->offset_sector = offset;
		offset += nf_dev->size_sector;
		nf_dev->mask_flags = partition->mask_flags;
		nf_dev->logicdev = amlnf_logicdev;
		nf_dev->nand_dev= amlnf_logicdev->nand_dev;

		nf_dev->read_sector = amlnf_read_sector;
		nf_dev->write_sector = amlnf_write_sector;
		nf_dev->flush = amlnf_flush;

		aml_nand_dbg("nf_dev->name %s, nf_dev->offset %d,nf_dev->size_sector %llx",nf_dev->name,nf_dev->offset_sector,nf_dev->size_sector );
		list_add_tail(&nf_dev->list, &nf_dev_list);
	}
	return 0;
}

static int amlnf_nftl_init(struct amlnand_phydev *nand)
{
	struct amlnf_logicdev_t *amlnf_logicdev;
	int ret =0;

	//printf("%s: size 0x%llx\n", nand->name, nand->size);

	if(nand->size > (21 << nand->erasesize_shift)) {  // >128M
		NPRINT("(%s), size:%llx\n",nand->name,nand->size);
	} else {
		NPRINT("PLS adjust nand size !!!(%s), size:%llx\n",
			nand->name,nand->size);
		return 1;
	}
	NFTL_LINE
	amlnf_logicdev = aml_nftl_malloc(sizeof(struct amlnf_logicdev_t));
	if (amlnf_logicdev == NULL){
		NPRINT("(%s), size:%llx\n",nand->name,nand->size);
		return 1;
	}
	NFTL_LINE
	memset(amlnf_logicdev, 0, sizeof(struct amlnf_logicdev_t));

	amlnf_logicdev->nand_dev = nand;

	if (aml_nftl_initialize(amlnf_logicdev,nftl_num++)){
		NPRINT("aml_nftl_initialize: fail\n");
		return 1;
	}
	NFTL_LINE
	ret = amlnf_logicdev_mis_init(amlnf_logicdev);
	if(ret != 0){
		NPRINT("create_nftl_dev fail %s\n",nand->name);
		return 1;
	}
	NFTL_LINE
	ret = aml_create_nftl_dev(amlnf_logicdev);
	if(ret != 0){
	    NPRINT("create_nftl_dev fail %s\n",nand->name);
	    return 1;
	}
	NFTL_LINE
	list_add_tail(&amlnf_logicdev->list, &nlogic_dev_list);

	return 0;
}

int amlnf_logic_init(unsigned flag)
{
	struct amlnand_phydev *phydev = NULL;
	int ret=0;
	aml_nand_msg("%s() start", __func__);

	//INIT_LIST_HEAD (&nf_dev_list);
	//amlnand_show_dev_partition(aml_chip);
	list_for_each_entry(phydev, &nphy_dev_list, list){
		if (strncmp((char *)phydev->name,
			    NAND_BOOT_NAME,
			    strlen((const char *)NAND_BOOT_NAME))) {
#ifdef AML_NAND_UBOOT
			if (flag == NAND_BOOT_NORMAL) {
				if (!strncmp((char *)phydev->name,
					     NAND_DATA_NAME,
					     strlen((const char *)NAND_DATA_NAME))) {
					aml_nand_dbg("nand normal boot: not init NAND_DATA DEVICE");
					continue;
				}
			}
#endif
			ret = amlnf_nftl_init(phydev);
			if (ret < 0) {
				aml_nand_msg("%s() failed", __func__);
				goto exit_error;
			}
		}
	}

	aml_nand_msg("%s() done", __func__);
exit_error:

	return ret;
}

void amlnf_logic_exit(void)
{
	struct amlnf_logicdev_t *amlnf_logicdev = NULL;

	list_for_each_entry(amlnf_logicdev, &nlogic_dev_list, list) {
		if (strncmp((char *)amlnf_logicdev->nand_dev->name,
			    NAND_BOOT_NAME,
			    strlen((const char *)NAND_BOOT_NAME))) {
			aml_nftl_part_release(amlnf_logicdev->priv);
			kfree(amlnf_logicdev->nand_dev);
		}
	}

	list_del_init(&nlogic_dev_list);

}

