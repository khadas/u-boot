// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <amlogic/storage.h>
#include <linux/mtd/mtd.h>
#include <asm/arch/cpu_config.h>
#include <amlogic/store_wrapper.h>
#include <u-boot/sha256.h>

#define debugP(fmt...) //printf("Dbg[WRP]L%d:", __LINE__),printf(fmt)
#define errorP(fmt...) printf("Err[WRP]L%d:", __LINE__),printf(fmt)
#define wrnP(fmt...)   printf("wrn:"fmt)
#define MsgP(fmt...)   printf("[WRP]"fmt)

//change part logic offset to physical address for mtd, not changed if not MTD
static int mtd_find_phy_off_by_lgc_off(const char* partName, const loff_t logicAddr, loff_t* phyAddr)
{
	if (!(BOOT_NAND_MTD == store_get_type() || BOOT_SNAND == store_get_type())) {
		*phyAddr = logicAddr;
		return 0;
	}
#ifndef CONFIG_CMD_MTD
	MsgP("Exception, boottype is MTD or snand, BUT CMD_MTD not defined\n");
#else
	struct mtd_info * mtdPartInf = NULL;
	mtdPartInf = get_mtd_device_nm(partName);
	if (!mtdPartInf) {
		errorP("device(%s) is null\n", partName);
		return CMD_RET_FAILURE;
	}

	if (IS_ERR(mtdPartInf)) {
		errorP("device(%s) is err\n", partName);
		return CMD_RET_FAILURE;
	}

	const unsigned eraseSz = mtdPartInf->erasesize;
	const unsigned offsetInBlk = logicAddr & (eraseSz - 1);
	loff_t off = 0;
	int canSpeedUp = 0;
	static struct {
		loff_t lastblkPhyOff;
		loff_t lastblkLgcOff;
		char   partName[64];
	}_map4SpeedUp = {0};
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
#endif// #ifndef CONFIG_CMD_MTD
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

#ifndef CONFIG_CMD_MTD
	MsgP("Exception, boottype is MTD or snand, BUT CMD_MTD not defined\n");
	return 0;
#else
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
#endif// #ifndef CONFIG_CMD_MTD
}

int store_gpt_ops(size_t sz, void *buf, int is_wr)
{
	int ret = 0;

	if (!sz || sz >= 0x100000) {
		errorP("sz 0x%zx to large\n", sz);
		return -__LINE__;
	}

	if (is_wr)
		ret = store_gpt_write(buf);
	else
		ret = store_gpt_read(buf);

	return ret;
}

#ifndef CONFIG_AML_V3_FACTORY_BURN//storage wrapper
#define FB_ERR(fmt ...) printf("[ERR]%s:L%d:", __func__, __LINE__), printf(fmt)
#define FB_EXIT(fmt ...) do { FB_ERR(fmt); return -__LINE__; } while (0)
#define FB_MSG(fmt ...) printf("[MSG]" fmt)
#define FBS_EXIT(buf, fmt ...) do { FBS_ERR(buf, fmt); return -__LINE__; } while (0)

int bootloader_copy_sz(void)
{
	return store_boot_copy_size("bootloader");
}

static int _bootloader_write(u8 *dataBuf, unsigned off, unsigned binSz, const char *bootName)
{
	int iCopy = 0;
	const int bootCpyNum = store_boot_copy_num(bootName);
	const int bootCpySz  = (int)store_boot_copy_size(bootName);

	FB_MSG("[%s] CpyNum %d, bootCpySz 0x%x\n", bootName, bootCpyNum, bootCpySz);
	if (binSz + off > bootCpySz)
		FB_EXIT("bootloader sz(0x%x) + off(0x%x) > bootCpySz 0x%x\n",
				binSz, off, bootCpySz);

	if (off) {
		FB_ERR("current only 0 supported!\n");
		return -__LINE__;
	}

	for (; iCopy < bootCpyNum; ++iCopy) {
		int ret = store_boot_write(bootName, iCopy, binSz, dataBuf);

		if (ret)
			FB_EXIT("FAil in program[%s] at copy[%d]\n", bootName, iCopy);
	}

	return 0;
}

int bootloader_write(u8 *dataBuf, unsigned off, unsigned binSz)
{
	return _bootloader_write(dataBuf, off, binSz, "bootloader");
}

static int _bootloader_read(u8 *pBuf, unsigned off, unsigned binSz, const char *bootName)
{
	int iCopy = 0;
	const int bootCpyNum = store_boot_copy_num(bootName);
	const int bootCpySz  = (int)store_boot_copy_size(bootName);
	int validCpyNum = bootCpyNum;//at least valid cpy num
	int actVldCpyNum = 0;//actual valid copy num

	if (binSz + off > bootCpySz) {
		FB_ERR("bootloader sz(0x%x) + off(0x%x) > bootCpySz 0x%x\n", binSz, off, bootCpySz);
		return -__LINE__;
	}
	if (off)
		FB_EXIT("current only 0 supported!\n");

	for (iCopy = 0; iCopy < bootCpyNum; ++iCopy) {
		void *dataBuf = iCopy ? pBuf + binSz : pBuf;
		int ret = store_boot_read(bootName, iCopy, binSz, dataBuf);

		if (ret) {
			FB_ERR("Fail to read boot[%s] at copy[%d]\n", bootName, iCopy);
			continue;
		}
		if (iCopy) {
			if (!actVldCpyNum) {//NO valid cpy yet, so copy0 NOT valid also
				memcpy(pBuf, dataBuf, binSz);
			}
			if (memcmp(pBuf, dataBuf, binSz)) {
				FB_ERR("[%s] copy[%d] content NOT the same as copy[0]\n",
						bootName, iCopy);//maybe ddr err as nand not err
				memset(pBuf, 0, binSz);
				return -__LINE__;
			}
		}
		++actVldCpyNum;
	}
	if (actVldCpyNum < validCpyNum) {
		FB_ERR("[%s]actual valid copy num %d < configured num %d\n",
				bootName, actVldCpyNum, validCpyNum);
		memset(pBuf, 0, binSz);
		return -__LINE__;
	}

	return 0;
}

int bootloader_read(u8 *pBuf, unsigned off, unsigned binSz)
{
	return _bootloader_read(pBuf, off, binSz, "bootloader");
}
#endif// #ifndef CONFIG_AML_V3_FACTORY_BURN//storage wrapper

