/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_nftl_interface.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "aml_nftl.h"

uint32 aml_nftl_get_part_cap(void * _part)
{
    struct aml_nftl_part_t* part = _part;
    return part->logic_cap_in_sects;
}

void aml_nftl_set_part_test(void * _part, u32 test)
{
    struct aml_nftl_part_t* part = _part;
    part->test = test;
    return ;
}

void* aml_nftl_get_part_priv(void * _part)
{
    struct aml_nftl_part_t* part = _part;
    return part->priv;
}

void aml_nftl_add_part_total_read(void * _part)
{
    struct aml_nftl_part_t* part = _part;
    part->total_reads++;
    return ;
}

void aml_nftl_add_part_total_write(void * _part)
{
    struct aml_nftl_part_t* part = _part;
    part->total_write++;
    return ;
}

uint16 aml_nftl_get_part_write_cache_nums(void * _part)
{
    struct aml_nftl_part_t* part = _part;
    return part->cache.cache_write_nums;
}
