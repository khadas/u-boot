/*
 * \file        store_wrapper.c
 * \brief       wrapper for storage.c
 *
 * \version     1.0.0
 * \date        2019/5/30
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2019 Amlogic. All Rights Reserved.
 *
 */
#include <common.h>
#include <amlogic/storage.h>
#include <linux/mtd/mtd.h>

#define debugP(fmt...) //printf("Dbg[WRP]L%d:", __LINE__),printf(fmt)
#define errorP(fmt...) printf("Err[WRP]L%d:", __LINE__),printf(fmt)
#define wrnP(fmt...)   printf("wrn:"fmt)
#define MsgP(fmt...)   printf("[WRP]"fmt)

//change part logic offset to physical address for mtd, not changed if not MTD
static int mtd_find_phy_off_by_lgc_off(const char* partName, const loff_t logicAddr, loff_t* phyAddr)
{
	struct mtd_info * mtdPartInf = NULL;
	loff_t off = 0;
	static struct {
		loff_t lastblkPhyOff;
		loff_t lastblkLgcOff;
		char   partName[64];
	}_map4SpeedUp = {0};
	int canSpeedUp = 0;

	if (!(BOOT_NAND_MTD == store_get_type() || BOOT_SNAND == store_get_type())) {
		*phyAddr = logicAddr;
		return 0;
	}

	mtdPartInf = get_mtd_device_nm(partName);
	if (IS_ERR(mtdPartInf)) {
		errorP("device(%s) is err\n", partName);
		return CMD_RET_FAILURE;
	}
	const unsigned eraseSz = mtdPartInf->erasesize;
	const unsigned offsetInBlk = logicAddr & (eraseSz - 1);

	if ( !strcmp(partName, _map4SpeedUp.partName) && logicAddr >= _map4SpeedUp.lastblkLgcOff) {
		canSpeedUp = 1;
	} else {
		_map4SpeedUp.lastblkLgcOff = _map4SpeedUp.lastblkPhyOff = 0;
		strncpy(_map4SpeedUp.partName, partName, 63);
	}

	if ( canSpeedUp ) {
		if ( logicAddr >= _map4SpeedUp.lastblkLgcOff &&
				logicAddr < _map4SpeedUp.lastblkLgcOff + eraseSz) {
			*phyAddr = _map4SpeedUp.lastblkPhyOff + offsetInBlk;
			return 0;
		}
		_map4SpeedUp.lastblkPhyOff += eraseSz;
		_map4SpeedUp.lastblkLgcOff += eraseSz;
		off = _map4SpeedUp.lastblkPhyOff;
	}
	for (; off < mtdPartInf->size; off += eraseSz, _map4SpeedUp.lastblkPhyOff += eraseSz) {
		if (mtd_block_isbad(mtdPartInf, off)) {
			MsgP("bad blk at  %08llx\n", (unsigned long long)off);
		} else {
			if ( logicAddr >= _map4SpeedUp.lastblkLgcOff &&
					logicAddr < _map4SpeedUp.lastblkLgcOff + eraseSz) {
				*phyAddr = _map4SpeedUp.lastblkPhyOff + offsetInBlk;
				return 0;
			}
			_map4SpeedUp.lastblkLgcOff += eraseSz;
		}
	}

	return __LINE__;
}

int store_logic_write(const char *name, loff_t off, size_t size, void *buf)
{
	loff_t phyAddr = 0;
	if (mtd_find_phy_off_by_lgc_off(name, off, &phyAddr)) {
		errorP("Fail find phy addr\n");
		return -__LINE__;
	}
	return store_write(name, phyAddr, size, buf);
}

int store_logic_read(const char *name, loff_t off, size_t size, void *buf)
{
	loff_t phyAddr = 0;
	if (mtd_find_phy_off_by_lgc_off(name, off, &phyAddr)) {
		errorP("Fail find phy addr\n");
		return -__LINE__;
	}
	return store_read(name, phyAddr, size, buf);
}

//get partition logic size
u64 store_logic_cap(const char* partName)
{
	if (!(BOOT_NAND_MTD == store_get_type() || BOOT_SNAND == store_get_type())) {
		return store_part_size(partName);
	}
	//get mtd part logic size (i.e, not including the bad blocks)
	struct mtd_info * mtdPartInf = NULL;
	uint64_t partSzLgc = 0;
	loff_t off = 0;

	mtdPartInf = get_mtd_device_nm(partName);
	if (IS_ERR(mtdPartInf)) {
		errorP("device(%s) is err\n", partName);
		return CMD_RET_FAILURE;
	}
	const unsigned eraseSz   = mtdPartInf->erasesize;
	const uint64_t partSzPhy = mtdPartInf->size;

	partSzLgc = partSzPhy;
	for (; off < partSzPhy; off += eraseSz) {
		if (mtd_block_isbad(mtdPartInf, off)) {
			partSzLgc -= eraseSz;
		}
	}
	return partSzLgc;
}

