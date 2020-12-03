/*
 * \file        v3_tool_media.c
 * \brief       operations on media for v3tool
 *
 * \version     1.0.0
 * \date        2018/06/01
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2018 Amlogic. All Rights Reserved.
 *
 */
#include "../include/v3_tool_def.h"
#include <fdtdec.h>
#include <asm/arch/cpu_config.h>
#include <u-boot/sha256.h>
#define DWN_ERR FB_ERR
#define BOOTLOADER_MAX_SZ   (0x2<<20)
#define DTB_MAX_SZ          (256<<10)
#define DWN_DBG FB_DBG
#define DWN_ERR FB_ERR
#define DWN_MSG FB_MSG
#define DWN_WRN FB_WRN

DECLARE_GLOBAL_DATA_PTR;

//make sure logic part sz == dtb configured sz
static int _assert_logic_partition_cap(const char* thePartName, const uint64_t nandPartCap)
{
	extern struct partitions * part_table;

	int partIndex                   = 0;
	struct partitions * thePart     = NULL;
	if (NULL == part_table)
		return 0;
	for (thePart = part_table; partIndex < MAX_PART_NUM; ++thePart, ++partIndex)
	{
		const uint64_t partSzInBytes = thePart->size;
		if (memcmp(thePartName, thePart->name, strlen(thePartName))) continue;

		FB_DBG("cfg partSzInBytes %llx for part(%s)\n", partSzInBytes, thePartName);
		if (NAND_PART_SIZE_FULL == partSzInBytes) {return 0;}
		if (partSzInBytes > nandPartCap) {
			FB_EXIT("partSz of logic part(%s): sz dts %llx > Sz flash %llx\n",
					thePartName, partSzInBytes, nandPartCap);
		}

		return 0;
	}

	FB_EXIT("Can't find your download part(%s)\n", thePartName);
}

#if 1//storage wrapper
static int is_bootloader_discrte(bool* discreteMode)
{
	struct storage_info_t storeInfo;
	if (store_get_device_info(&storeInfo)) {
		FB_ERR("Fail get store dev info\n");
		return __LINE__;
	}
	*discreteMode = (DISCRETE_BOOTLOADER == storeInfo.mode);
	return 0;
}

static int bootloader_copy_sz(void)
{
	bool discreteMode = false;
	if (is_bootloader_discrte(&discreteMode)) {
		return 0;
	}

	return store_boot_copy_size("bootloader");
}

static int _bootloader_write(u8* dataBuf, unsigned off, unsigned binSz, const char* bootName)
{
	int iCopy = 0;
	const int bootCpyNum = store_boot_copy_num(bootName);
	const int bootCpySz  = (int)store_boot_copy_size(bootName);
	FB_MSG("[%s] CpyNum %d, bootCpySz 0x%x\n", bootName, bootCpyNum, bootCpySz);
	if (binSz + off > bootCpySz) FBS_EXIT(_ACK, "bootloader sz(0x%x) + off(0x%x) > bootCpySz 0x%x\n", binSz, off, bootCpySz);

	if (off) {
		FBS_ERR(_ACK, "current only 0 suuported!\n");
		return -__LINE__;
	}

	for (; iCopy < bootCpyNum; ++iCopy) {
		int ret = store_boot_write(bootName, iCopy, binSz, dataBuf);
		if (ret) FBS_EXIT(_ACK, "FAil in program[%s] at copy[%d]\n", bootName, iCopy);
	}

	return 0;
}

static p_payload_info_t _bl2x_mode_detect(u8* dataBuf)
{
	p_payload_info_t pInfo      = (p_payload_info_t)(dataBuf + BL2_SIZE);

	if (AML_MAGIC_HDR_L == pInfo->hdr.nMagicL && AML_MAGIC_HDR_R == pInfo->hdr.nMagicR) {
		FB_MSG("aml log : bootloader blxx mode!\n");
		return pInfo;
	}
	return NULL;
}

#ifdef CONFIG_SHA256
static int _bl2x_mode_check_header(p_payload_info_t pInfo)
{
	p_payload_info_hdr_t hdr    = &pInfo->hdr;
	uint8_t gensum[SHA256_SUM_LEN];
	const int nItemNum = hdr->byItemNum;

	printf("\naml log : info parse...\n");
	printf("\tsztimes : %s\n",hdr->szTimeStamp);
	printf("\tversion : %d\n",hdr->byVersion);
	printf("\tItemNum : %d\n",nItemNum);
	printf("\tSize    : %d(0x%x)\n",    hdr->nSize, hdr->nSize);
	if (nItemNum > 8 || nItemNum < 3) { FBS_EXIT(_ACK, "illegal nitem num %d\n", nItemNum); }

	const int nsz = sizeof(payload_info_hdr_t) + nItemNum * sizeof(payload_info_item_t) - SHA256_SUM_LEN;
	FB_MSG("nsz 0x%x\n", nsz);
	sha256_context ctx;
	sha256_starts(&ctx);
	sha256_update(&ctx, (u8*)&(hdr->nMagicL), nsz);
	sha256_finish(&ctx, gensum);
	int ret = memcmp(gensum, hdr->szSHA2, SHA256_SUM_LEN);
	if (ret) { FBS_EXIT(_ACK, "hdr info sha256sum not matched\n"); }
	FB_MSG("hdr info sha256sum DO matched\n");

	return 0;
}
#else
#define _bl2x_mode_check_header(...) 0
#endif// #ifdef CONFIG_SHA256

static const char* _flashPayload[] = {"bl2",  "bl2e", "bl2x", "ddrfip", "devfip"};
static p_payload_info_t _blxPayloadInf = NULL;
static int _payloadInfoSz = 0;
static int _discrete_bootloader_write(u8* dataBuf, unsigned off, unsigned binSz)
{
	int bl2CopySz  = BL2_SIZE/*(int)store_boot_copy_size("bl2")*/;
	FB_MSG("bl2CopySz 0x%x, binSz 0x%x\n", bl2CopySz, binSz);
	int ret = 0;

	if (binSz > bl2CopySz)
	{
		_blxPayloadInf = NULL;
		p_payload_info_t pInfo = _bl2x_mode_detect(dataBuf);
		if (!pInfo) {
			ret = _bootloader_write(dataBuf + bl2CopySz, 0, binSz - bl2CopySz, "tpl");
			if (ret) FBS_EXIT(_ACK, "Fail in burn tpl\n");
		} else
		{
			if (_bl2x_mode_check_header(pInfo)) {
				FBS_EXIT(_ACK, "Fail in check bl2x info\n");
			}
			char name[8];
			int nIndex = 0;
			p_payload_info_hdr_t hdr    = &pInfo->hdr;
			p_payload_info_item_t pItem = pInfo->arrItems;

			int offPayload = 0, szPayload = 0;

			memset(name, 0, 8);
			for (nIndex = 1, pItem += 1; nIndex < hdr->byItemNum; ++nIndex, ++pItem)
			{
				memcpy(name, &pItem->nMagic, sizeof(unsigned int));
				offPayload = pItem->nOffset;
				szPayload  = pItem->nPayLoadSize;
				FB_MSG("Item[%d]%4s offset 0x%08x sz 0x%x\n", nIndex, name, offPayload, szPayload);
				if (!szPayload) continue;
				ret = _bootloader_write(dataBuf + offPayload, 0, szPayload, _flashPayload[nIndex]);
				if (ret) FBS_EXIT(_ACK, "Fail in flash payload %s\n", name);
			}
			_blxPayloadInf = (p_payload_info_t)V3_DOWNLOAD_VERIFY_INFO;
			_payloadInfoSz = sizeof(payload_info_hdr_t) + pInfo->hdr.byItemNum * sizeof(payload_info_item_t);
			memcpy(_blxPayloadInf, pInfo, _payloadInfoSz);
		}
	}

	ret = _bootloader_write(dataBuf, 0, bl2CopySz, "bl2");
	if (ret) FBS_EXIT(_ACK, "Fail in program bl2\n");

	return 0;
}

int bootloader_write(u8* dataBuf, unsigned off, unsigned binSz)
{
	bool discreteMode = false;
	if (is_bootloader_discrte(&discreteMode)) {
		return -__LINE__;
	}
	if (!discreteMode) {
		return _bootloader_write(dataBuf, off, binSz, "bootloader");
	} else {
		return _discrete_bootloader_write(dataBuf, off, binSz);
	}
	return -__LINE__;
}

static int _bootloader_read(u8* pBuf, unsigned off, unsigned binSz, const char* bootName)
{
	int iCopy = 0;
	const int bootCpyNum = store_boot_copy_num(bootName);
	const int bootCpySz  = (int)store_boot_copy_size(bootName);

	if (binSz + off > bootCpySz) {
		FBS_ERR(_ACK, "bootloader sz(0x%x) + off(0x%x) > bootCpySz 0x%x\n", binSz, off, bootCpySz);
		return -__LINE__;
	}
	if (off) FBS_EXIT(_ACK, "current only 0 suuported!\n");

	for (iCopy = 0; iCopy < bootCpyNum; ++iCopy) {
		void* dataBuf = iCopy ? pBuf + binSz : pBuf;
		int ret = store_boot_read(bootName, iCopy, binSz, dataBuf);
		if (ret) FBS_EXIT("Fail to read boot[%s] at copy[%d]\n", bootName, iCopy);
		if (iCopy) {
			if (memcmp(pBuf, dataBuf, binSz))
				FBS_EXIT(_ACK, "[%s] copy[%d] content NOT the same as copy[0]\n", bootName, iCopy);
		}
	}

	return 0;
}

static int _discrete_bootloader_read(u8* dataBuf, unsigned off, unsigned binSz)
{
	int bl2CopySz  = BL2_SIZE/*(int)store_boot_copy_size("bl2")*/;
	FB_MSG("bl2CopySz 0x%x, binSz 0x%x\n", bl2CopySz, binSz);

	int ret = _bootloader_read(dataBuf, 0, bl2CopySz, "bl2");
	if (ret) FBS_EXIT(_ACK, "Fail in read bl2\n");
	if (binSz <= bl2CopySz) return 0;
	memset(dataBuf + bl2CopySz, 0, bl2CopySz);//clear 2k after bl2_size

	if (!_blxPayloadInf) {
		ret = _bootloader_read(dataBuf + bl2CopySz, 0, binSz - bl2CopySz, "tpl");
		if (ret) FBS_EXIT(_ACK, "Fail in read tpl\n");
	} else {
		char name[8];
		int nIndex = 0;
		if (!_blxPayloadInf) FBS_EXIT(_ACK, "exception, _blxPayloadInf null\n");

		p_payload_info_t pInfo      = _blxPayloadInf;
		p_payload_info_hdr_t hdr    = &pInfo->hdr;
		p_payload_info_item_t pItem = pInfo->arrItems;

		int offPayload = 0, szPayload = 0;

		memset(name, 0, 8);
		for (nIndex = 1, pItem +=1; nIndex < hdr->byItemNum; ++nIndex, ++pItem)
		{
			memcpy(name, &pItem->nMagic, sizeof(unsigned int));
			offPayload = pItem->nOffset;
			szPayload  = pItem->nPayLoadSize;
			FB_MSG("Item[%d]%4s offset 0x%08x sz 0x%x\n", nIndex, name, offPayload, szPayload);
			if (!szPayload) continue;
			ret = _bootloader_read(dataBuf + offPayload, 0, szPayload, _flashPayload[nIndex]);
			if (ret) FBS_EXIT(_ACK, "Fail in read payload %s\n", name);
		}
		memcpy(dataBuf + BL2_SIZE, _blxPayloadInf, _payloadInfoSz);
	}

	return 0;
}

int bootloader_read(u8* pBuf, unsigned off, unsigned binSz)
{
	bool discreteMode = false;
	if (is_bootloader_discrte(&discreteMode)) {
		return -__LINE__;
	}

	if (discreteMode)
		return _discrete_bootloader_read(pBuf, off, binSz);

	return _bootloader_read(pBuf, off, binSz, "bootloader");
}

//@rwFlag: 0---read, 1---write, 2---iread
int store_dtb_rw(void* buf, unsigned dtbSz, int rwFlag)
{
    int ret = 0;
    const unsigned dtbCap = store_rsv_size("dtb");
    if (dtbCap <= dtbSz)
        FBS_EXIT(_ACK, "dtb sz 0x%x > cap 0x%x\t", dtbSz, dtbCap);

    switch (rwFlag) {
        case 2: {//iread
            ret = store_rsv_read("dtb", dtbSz, buf);
            if (ret) FBS_EXIT(_ACK, "err(%d) in read dtb\t", ret);
        }
        case 0: {//read
            if ( 2 == rwFlag ) return 0;
            //TODO: add dtb parser
            FBS_EXIT(_ACK, "dtb parser not implemented yet\t");
        }break;
        case 1: {//write
            ret = store_rsv_erase("dtb");
            if (ret) FBS_EXIT(_ACK, "Fail erase dtb, ret %d\n", ret);
            ret = store_rsv_write("dtb", dtbSz, buf);
            if (ret) FBS_EXIT(_ACK, "Fail in dtb write, ret %d\t", ret);
        }break;
        default: FBS_EXIT(_ACK, "err dtb rwFlag %d\n", rwFlag);
    }

    return 0;
}
#endif// #if 1//storage wrapper


int v3tool_media_check_image_size(int64_t imgSz, const char* partName)
{
    int ret = 0;
    u64 partCap = 0;

    if (!strcmp("bootloader", partName)) {
        const unsigned bootSz = bootloader_copy_sz();
        if (imgSz > bootSz)
            FBS_EXIT(_ACK, "imgsz 0x%llx > copy sz 0x%x !\t", imgSz, bootSz);
        return 0;
    }

    if (!strcmp("_aml_dtb", partName)) {
        const unsigned dtbCap = store_rsv_size("dtb");
        if (imgSz >= dtbCap)
            FB_EXIT("imgsz 0x%llx >= max sz 0x%x\n", imgSz, dtbCap);
        return 0;
    }

    partCap = store_part_size(partName);
    if (!partCap) {
        DWN_ERR("Fail to get size for part %s\n", partName);
        return __LINE__;
    }
    DWN_MSG("flash LOGIC partCap 0x%llxB\n", partCap);
    if (imgSz > partCap) {
        DWN_ERR("imgSz 0x%llx out of cap 0x%llx\n", imgSz, partCap);
        return __LINE__;
    }
    ret = _assert_logic_partition_cap(partName, partCap);
    if (ret) {
        DWN_ERR("Fail in _assert_logic_partition_cap\n");
        return __LINE__;
    }

    return 0;
}

static int _optimusWorkMode = V3TOOL_WORK_MODE_NONE;

int v3tool_work_mode_get(void)
{
	return _optimusWorkMode;
}

int v3tool_work_mode_set(int workmode)
{
	_optimusWorkMode = workmode;
	return 0;
}


static int _disk_intialed_ok = 0;

int v3tool_is_flash_erased(void)
{
	return _disk_intialed_ok>>16;
}

static int should_load_env(void)
{
#ifdef CONFIG_OF_CONTROL
	return fdtdec_get_config_int(gd->fdt_blob, "load-environment", 1);
#elif defined CONFIG_DELAY_ENVIRONMENT
	return 0;
#else
	return 1;
#endif
}

static int initr_env(void)
{
	/* initialize environment */
	if (should_load_env()) {
		DWN_MSG("usb producing env_relocate\n");
		env_relocate();
	}
	return 0;
}

struct mtd_partition* __attribute__((weak)) get_partition_table(int *partitions)
{ FB_WRN("get_partition_table undefined\n"); return NULL;}

int __attribute__((weak)) sheader_need(void) { FB_WRN("sheader_need undefined\n"); return 0;}
void __attribute__((weak)) sheader_load(void *addr) { FB_WRN("sheader_load undefined\n"); return;}

#ifdef CONFIG_BACKUP_PART_NORMAL_ERASE
const char* BackupPart = (const char*)(CONFIG_BACKUP_PART_NORMAL_ERASE);
char* BackupPartAddr = (char*)(V3_DOWNLOAD_MEM_BASE);
#endif// #ifdef CONFIG_BACKUP_PART_NORMAL_ERASE

int v3tool_storage_init(const int eraseFlash, unsigned dtbImgSz)
{
	int ret = 0;
	unsigned char* dtbLoadedAddr = (unsigned char*)V3_DTB_LOAD_ADDR;
	int dtb_valid = 0;

	if (V3TOOL_WORK_MODE_USB_PRODUCE != v3tool_work_mode_get()) {//Already inited in other work mode
		/*DWN_MSG("Exit before re-init\n");*/
		/*store_exit();*/
	}

	if (dtbImgSz) {
#if defined(CONFIG_MTD) && defined(CONFIG_AML_MTDPART)
		extern int get_meson_mtd_partition_table(struct mtd_partition *partitions);
		int mtdParts = -1;
		struct mtd_partition *partitions;

		mtdParts = get_meson_mtd_partition_table(partitions);
		if (partitions) {//
			extern int check_valid_dts(unsigned char *buffer);
			ret = check_valid_dts(dtbLoadedAddr);
		} else
#endif // #if defined(CONFIG_MTD) && defined(CONFIG_AML_MTDPART)
			ret = get_partition_from_dts(dtbLoadedAddr);
		if (ret) FBS_EXIT(_ACK, "Failed at check dts\n");
		dtb_valid = 1;
	}

    if (sheader_need()) sheader_load((void*)V3_PAYLOAD_LOAD_ADDR);

	ret = store_init(1);
	if (ret <= 0)
		FBS_EXIT(_ACK, "Fail in store init %d, ret %d\n", 1, ret);

#ifdef CONFIG_BACKUP_PART_NORMAL_ERASE
	u32 backupPartSz = 0;
#endif//#ifdef CONFIG_BACKUP_PART_NORMAL_ERASE
	int initFlag = 0;
	switch (eraseFlash) {
		case 0://NO erase
		case 5://NO erase
			initFlag = 1;
			break;

		case 3://erase all(with key)
		case 1://normal erase, store init 3
			initFlag = 3;
			if (3 == eraseFlash) {
				if (store_rsv_protect("key", false))
					FBS_EXIT(_ACK, "Fail in disprotect key\n");
			} else {
#ifdef CONFIG_BACKUP_PART_NORMAL_ERASE
				//backup env to memory
				backupPartSz = (u32)store_part_size(BackupPart);
				FB_MSG("BackupPart %s sz 0x%x\n", BackupPart, backupPartSz);
				if (!backupPartSz) { FBS_EXIT(_ACK, " FAil in find BackupPart %s\n", BackupPart);}
				ret = store_read(BackupPart, 0, backupPartSz, BackupPartAddr);
				if (ret) { FBS_EXIT(_ACK, "FAil in backup important part %s to mem\n", BackupPart);}
#endif//#ifdef CONFIG_BACKUP_PART_NORMAL_ERASE
			}
			break;

		case 4: {//force erase all
					if (store_rsv_protect(NULL, false))
						FBS_EXIT(_ACK, "Fail in disprotect all rsv\n");
				}
		case 2:
				initFlag = 4;
				break;

		default:
				FBS_EXIT(_ACK, "Unsupported erase flag %d\n", eraseFlash);
	}

	FB_MSG("eraseFlash %d, initFlag %d\n", eraseFlash, initFlag);
	if (5 == eraseFlash) {//erase key only
		ret = store_rsv_erase("key");
		if (ret) FBS_EXIT(_ACK, "disk_initial 5, Fail in erase key\n");
	} else if (initFlag > 1) {
		ret = store_erase(NULL, 0, 0, 0);
		if (ret) FBS_EXIT(_ACK, "Fail in erase flash, ret[%d]\n", ret);
#ifdef CONFIG_BACKUP_PART_NORMAL_ERASE
		if (backupPartSz) {
			FB_MSG("restore BackupPart %s from mem\n", BackupPart);
			store_write(BackupPart, 0, backupPartSz, BackupPartAddr);
		}
#endif//#ifdef CONFIG_BACKUP_PART_NORMAL_ERASE
	}

	if (V3TOOL_WORK_MODE_USB_PRODUCE == v3tool_work_mode_get()) {
		if (!_disk_intialed_ok) {//last disk_initial also okay
			initr_env();//can only be called once
		}
	}
	_disk_intialed_ok  = 1;
	if (eraseFlash && eraseFlash < 5) _disk_intialed_ok += (1 <<16);

	FB_DBG("dtb_valid %d, dtbImgSz 0x%x\n", dtb_valid, dtbImgSz);
	if (dtb_valid)//for key init, or fail when get /unifykey
	{
		unsigned long fdtAddr = (unsigned long)dtbLoadedAddr;
#ifdef CONFIG_MULTI_DTB
		fdtAddr = get_multi_dt_entry(fdtAddr);
#endif// #ifdef CONFIG_MULTI_DTB
		ret = fdt_check_header((char*)fdtAddr);
		if (ret) FBS_EXIT(_ACK, "Fail in fdt check header\n");

		unsigned fdtsz    = fdt_totalsize((char*)fdtAddr);
		if (fdtAddr != (unsigned long)dtbLoadedAddr)
			memmove((char*)dtbLoadedAddr, (char*)fdtAddr, fdtsz);
	}

	return 0;
}

static int _v3tool_is_busy = 0;
static char* _v3tool_media_busy_info = "";
void v3tool_media_set_busy(const char* busyInfo)
{
	_v3tool_is_busy = 1;
	_v3tool_media_busy_info = fb_response_str - 4;
	strncpy(_v3tool_media_busy_info, "INFO", 4 + 1);//add terminated 0
	if (busyInfo)
		strncpy(_v3tool_media_busy_info + 4, busyInfo, 64);
}
void v3tool_media_set_free(const char* info)
{
	_v3tool_is_busy = 0;
	_v3tool_media_busy_info = fb_response_str - 4;
	strncpy(_v3tool_media_busy_info, "OKAY", 4 + 1);
	if (info)
		strncpy(_v3tool_media_busy_info + 4, info, 64);
}
int v3tool_media_is_busy(void)
{
    return _v3tool_is_busy;
}

