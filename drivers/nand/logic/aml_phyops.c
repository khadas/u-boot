/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/nand/logic/aml_phyops.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "aml_nftl.h"

int  nand_erase_superblk(struct aml_nftl_part_t *part,_physic_op_par *p)
{
	struct amlnand_phydev *phydev = (struct amlnand_phydev *)aml_nftl_get_part_priv(part);
	struct phydev_ops *ops = &phydev->ops;

	memset(ops, 0, sizeof(struct phydev_ops));

	ops->addr = p->phy_page.blkNO_in_chip;
	ops->addr <<= phydev->erasesize_shift;
	ops->len = phydev->erasesize;

	return phydev->erase(phydev);
}

int nand_read_page(struct aml_nftl_part_t *part,_physic_op_par *p)
{

	struct amlnand_phydev *phydev = (struct amlnand_phydev *)aml_nftl_get_part_priv(part);
	struct phydev_ops *ops = &phydev->ops;
	int ret;
	uint32 temp;

	memset(ops, 0, sizeof(struct phydev_ops));

	ops->addr = p->phy_page.blkNO_in_chip;
	ops->addr <<= phydev->erasesize_shift;
	temp = p->phy_page.Page_NO;
	temp <<= phydev->writesize_shift;
	ops->addr += temp;

	ops->len = phydev->writesize;
	ops->ooblen = BYTES_OF_USER_PER_PAGE;
	ops->datbuf = p->main_data_addr;
	//aml_nftl_blk->nand_dev->ops.oobbuf = p->spare_data_addr;
	ops->oobbuf = p->spare_data_addr;//aml_nftl_blk->spare_buf;

	aml_nftl_add_part_total_read(part);
	//NPRINT("111aml_ops_read_page : %llx %d %d\n", aml_nftl_blk->nand_dev->ops.addr, p->phy_page.blkNO_in_chip, p->phy_page.Page_NO);
	//NPRINT("%s() addr %llx [%d, %d]\n", __func__, ops->addr, p->phy_page.blkNO_in_chip, p->phy_page.Page_NO);
	ret = phydev->read(phydev);
	//NPRINT("222aml_ops_read_page : %llx %d %d\n", aml_nftl_blk->nand_dev->ops.addr, p->phy_page.blkNO_in_chip, p->phy_page.Page_NO);

	if (ret == -EUCLEAN)
	{
		NPRINT("read reclaim\n");
	}

	/*if(p->spare_data_addr)
	    memcpy(p->spare_data_addr,aml_nftl_blk->spare_buf, BYTES_OF_USER_PER_PAGE);*/

	if ((ret!=0) &&(ret != -EUCLEAN)){
		NPRINT("aml_ops_read_page failed: %llx %d %d\n", ops->addr, p->phy_page.blkNO_in_chip, p->phy_page.Page_NO);
		ret  = -EBADMSG;
	}

	return ret;
}

int nand_write_page(struct aml_nftl_part_t *part, _physic_op_par *p)
{
	int ret;
	uint32 temp;
	struct amlnand_phydev *phydev = (struct amlnand_phydev *)aml_nftl_get_part_priv(part);
	struct phydev_ops *ops = &phydev->ops;

	memset(ops, 0, sizeof(struct phydev_ops));

	//    aml_nftl_blk->nand_dev->ops.addr =  aml_nftl_blk->nand_dev->erasesize;
	//    aml_nftl_blk->nand_dev->ops.addr *= p->phy_page.blkNO_in_chip;
	//    aml_nftl_blk->nand_dev->ops.addr += p->phy_page.Page_NO * aml_nftl_blk->nand_dev->writesize;

	ops->addr = p->phy_page.blkNO_in_chip;
	ops->addr <<= phydev->erasesize_shift;
	temp = p->phy_page.Page_NO;
	temp <<= phydev->writesize_shift;
	ops->addr += temp;

	ops->len = phydev->writesize;
	ops->ooblen = BYTES_OF_USER_PER_PAGE;
	ops->datbuf = p->main_data_addr;
	ops->oobbuf = p->spare_data_addr;

	aml_nftl_add_part_total_write(part);
	NFTL_LINE
	aml_nand_dbg("%s() addr %llx [%d, %d]\n", __func__, ops->addr, p->phy_page.blkNO_in_chip, p->phy_page.Page_NO);
	ret = phydev->write(phydev);

	return ret;
}

int nand_is_blk_good(struct aml_nftl_part_t *part,_physic_op_par *p)
{
	int ret;
	struct amlnand_phydev *phydev = (struct amlnand_phydev *)aml_nftl_get_part_priv(part);
	struct phydev_ops *ops = &phydev->ops;

	memset(ops, 0, sizeof(struct phydev_ops));

	ops->addr = phydev->erasesize;
	ops->addr *= p->phy_page.blkNO_in_chip;
	ops->addr <<= phydev->erasesize_shift;

	//    NPRINT("nand_is_blk_good %x,%x,%llx\n",p->phy_page.blkNO_in_chip,aml_nftl_blk->nand_dev->erasesize,aml_nftl_blk->nand_dev->ops.addr);

	ret = phydev->block_isbad(phydev);
	//    NPRINT("nand_is_blk_good 2\n");
	if(ret == 0)
	{
		return RET_YES;
	}
	else
	{
	      if(ret == 1)
			ret = RET_NO;
		return ret;
	}
}

int nand_mark_bad_blk(struct aml_nftl_part_t *part,_physic_op_par *p)
{
	int ret;
	struct amlnand_phydev *phydev = (struct amlnand_phydev *)aml_nftl_get_part_priv(part);
	struct phydev_ops *ops = &phydev->ops;

	memset(ops, 0, sizeof(struct phydev_ops));
	//    aml_nftl_blk->nand_dev->ops.addr =  aml_nftl_blk->nand_dev->erasesize;
	ops->addr = p->phy_page.blkNO_in_chip;
	ops->addr <<= phydev->erasesize_shift;
	ops->len = phydev->erasesize;

	ret = phydev->block_markbad(phydev);
	if(ret == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
