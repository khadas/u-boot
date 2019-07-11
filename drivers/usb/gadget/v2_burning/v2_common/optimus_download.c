/*
 * \file        optimu_download.c
 * \brief
 *
 * \version     1.0.0
 * \date        2013/4/25
 * \author      Sam.Wu <yihui.wu@amlogic.com>
 *
 * Copyright (c) 2013 Amlogic Inc. All Rights Reserved.
 *
 */
#include "../v2_burning_i.h"
#include <linux/libfdt.h>
#include <partition_table.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/bl31_apis.h>
#include <asm/io.h>
#include <asm/arch/mailbox.h>
#include <asm/arch/cpu_config.h>

extern unsigned int get_multi_dt_entry(unsigned long fdt_addr);
int is_optimus_storage_inited(void);

#if !defined(CONFIG_UNIFY_KEY_MANAGE)
int v2_key_read(const char* keyName, u8* keyVal, const unsigned keyValLen, char* errInfo, unsigned* fmtLen)
{
    DWN_ERR("burn key not supported as CONFIG_UNIFY_KEY_MANAGE undef!!");
    return OPT_DOWN_FAIL;
}

unsigned v2_key_burn(const char* keyName, const u8* keyVal, const unsigned keyValLen, char* errInfo)
{
    DWN_ERR("burn key not supported as CONFIG_UNIFY_KEY_MANAGE undef!!");
    return OPT_DOWN_FAIL;
}

int v2_key_command(const int argc, char * const argv[], char *info)
{

    DWN_ERR("burn key not supported as CONFIG_UNIFY_KEY_MANAGE undef!!");
    return OPT_DOWN_FAIL;
}
#endif//#ifndef CONFIG_UNIFY_KEY_MANAGE
#ifndef CONFIG_USB_BURNING_TOOL
#define platform_busy_increase_un_reported_size(sz)
#endif// #ifndef CONFIG_USB_BURNING_TOOL

#if 1//storage wrapper
#define FBS_ERR(a, fmt...) DWN_ERR(fmt)
#define FBS_EXIT(a, fmt...) do {DWN_ERR(fmt); return -__LINE__;} while(0)
#define FB_ERR  DWN_ERR
#define FB_MSG  DWN_MSG
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
    if (discreteMode) {
        return BL2_SIZE/*store_boot_copy_size("bl2")*/ + store_boot_copy_size("tpl");
    } else {
        return store_boot_copy_size("bootloader");
    }

    return 0;
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

static int _discrete_bootloader_write(u8* dataBuf, unsigned off, unsigned binSz)
{
    int bl2CopySz  = BL2_SIZE/*(int)store_boot_copy_size("bl2")*/;
    FB_MSG("bl2CopySz 0x%x, binSz 0x%x\n", bl2CopySz, binSz);

    int ret = _bootloader_write(dataBuf, 0, bl2CopySz, "bl2");
    if (ret) FBS_EXIT(_ACK, "Fail in program bl2\n");
    if (binSz <= bl2CopySz) return 0;

    ret = _bootloader_write(dataBuf + bl2CopySz, 0, binSz - bl2CopySz, "tpl");
    if (ret) FBS_EXIT(_ACK, "Fail in burn tpl\n");
    return 0;
}

static int bootloader_write(u8* dataBuf, unsigned off, unsigned binSz)
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
    if (ret) FBS_EXIT(_ACK, "Fail in program bl2\n");
    if (binSz <= bl2CopySz) return 0;

    ret = _bootloader_read(dataBuf + bl2CopySz, 0, binSz - bl2CopySz, "tpl");
    if (ret) FBS_EXIT(_ACK, "Fail in burn tpl\n");
    return 0;
}

static int bootloader_read(u8* pBuf, unsigned off, unsigned binSz)
{
    bool discreteMode = false;
    if (is_bootloader_discrte(&discreteMode)) {
        return -__LINE__;
    }

    if (discreteMode)
        return _discrete_bootloader_read(pBuf, off, binSz);

    return _bootloader_read(pBuf, off, binSz, "bootloader");
}
#endif// #if 1//storage wrapper

static unsigned long _dtb_is_loaded = 0;


#define IMG_VERIFY_ALG_NONE     0 //not need to veryfy
#define IMG_VERIFY_ALG_SHA1SUM  1
#define IMG_VERIFY_ALG_CRC32    2
#define IMG_VERIFY_ALG_ADDSUM   3

#define OPTIMUS_IMG_STA_EMPTY           0
#define OPTIMUS_IMG_STA_PRE_BURN        1 //has get tplcmd load
#define OPTIMUS_IMG_STA_BURN_ING        2
#define OPTIMUS_IMG_STA_BURN_COMPLETE   3
#define OPTIMUS_IMG_STA_BURN_FAILED     4
#define OPTIMUS_IMG_STA_VERIFY_ING      5
#define OPTIMUS_IMG_STA_VERIFY_END      6

enum {
    IMG_TYPE_SPARSE     = 0xae  ,
    IMG_TYPE_NORMAL             ,
    IMG_TYPE_BOOTLOADER         ,
    IMG_TYPE_DTB                ,
    IMG_TYPE_UBIFS		,
};

//Image info for burnning and verify
//FIXME: how to assert that image not larger than the partition
#define IMG_BURN_INFO_SZ    96
struct ImgBurnInfo{
    u8  imgType;    //0 normal, 1 sparse
    u8  verifyAlgorithm;//0--sha1sum, 1--crc32, 2--addsum
    u8  imgBurnSta;//
    u8  storageMediaType;//NAND default,
    u8  isDumpMode; //if dump mode
    u8  resrv4Align[3];

    u64 nextMediaOffset;//image size already  received
    u64 imgPktSz;//total size of the file image
    u64 imgSzDisposed;//total size alreay disposed
    u64 partBaseOffset;//start offset of this part

    void* devHdle;
    char partName[32];//

    u8  burnInfoPrivate[IMG_BURN_INFO_SZ - 32 - sizeof(void*) - sizeof(u64) * 5];//needed private info when verify, for example when we read ext4 to sparse file
};

static struct ImgBurnInfo OptimusImgBurnInfo = {0};
static const char*   _usbDownPartImgType = "";

struct imgBurnInfo_sparse{

};

struct imgBurnInfo_normal{
};

struct imgBurnInfo_bootloader{
    u32     transferBufAddr;
    u32     transferBufSzTotal;

};

COMPILE_TIME_ASSERT(IMG_BURN_INFO_SZ == sizeof(struct ImgBurnInfo));

#if CONFIG_AML_STORAGE
//asset logical partition size >= CFG size in storage.c
//nand often make mistake this size, emmc should always ok
static int _assert_logic_partition_cap(const char* thePartName, const uint64_t nandPartCap)
{
    extern struct partitions * part_table;

    int partIndex                   = 0;
    struct partitions * thePart     = NULL;
    if (NULL == part_table)
        return 0;
    for (thePart = part_table; partIndex < 36; ++thePart, ++partIndex)
    {
        if (memcmp(thePartName, thePart->name, strnlen(thePartName, MAX_PART_NAME_LEN))) continue;

        const uint64_t dtsPartSz = thePart->size;
        DWN_DBG("cfg dtsPartSz %llx for part(%s)\n", dtsPartSz, thePartName);
        if (NAND_PART_SIZE_FULL == dtsPartSz) {return 0;}
        if (dtsPartSz > nandPartCap) {
            DWN_ERR("partSz of logic part(%s): sz dts %llx > Sz flash %llx\n",
                    thePartName, dtsPartSz, nandPartCap);
            return __LINE__;
        }

        return 0;
    }

    DWN_ERR("Can't find your download part(%s)\n", thePartName);
    return __LINE__;
}
#else
#define _assert_logic_partition_cap(thePartName, nandPartCap) 0
#endif// #if CONFIG_AML_STORAGE

//return value is the actual size it write
static int optimus_download_dtb_image(struct ImgBurnInfo* pDownInfo, u32 dataSzReceived, const u8* data)
{
    int ret = 0;
    DWN_MSG("%s:dataSzReceived=0x%x\n", __func__, dataSzReceived);
    store_erase("dtb", 0, store_rsv_size("dtb"), 0);
    ret = store_rsv_write("dtb", dataSzReceived, (u8*)data);

    return ret ? 0 : dataSzReceived;
}

static int optimus_verify_dtb(struct ImgBurnInfo* pDownInfo, u8* genSum)
{
    int ret = OPT_DOWN_OK;
    unsigned char* pBuf = (unsigned char*)OPTIMUS_DOWNLOAD_TRANSFER_BUF_ADDR;
    uint64_t size = 0;

    size=pDownInfo->imgPktSz;
    ret = store_rsv_read("dtb", size, pBuf);
    if (ret) {
        DWN_ERR("Fail to read dtb\n");
        return __LINE__;
    }

    sha1_csum(pBuf, (u32)pDownInfo->imgPktSz, genSum);

    return ret;
}

//return value is the actual size it write
static int optimus_download_bootloader_image(struct ImgBurnInfo* pDownInfo, u32 dataSzReceived, const u8* data)
{
    int ret = OPT_DOWN_OK;
    int size = dataSzReceived;
    int iCopy = 0;

    if (dataSzReceived < pDownInfo->imgPktSz) {
        DWN_ERR("please write back bootloader after all data rx end.0x(%x, %x)\n", dataSzReceived, (u32)pDownInfo->imgPktSz);
        return 0;
    }

    if (size > bootloader_copy_sz()) {
        DWN_ERR("uboot.bin size 0x%x > 2M unsupported\n", size);
        return 0;
    }

    ret = bootloader_write((u8*)data, 0, size);
    if (ret) {
        DWN_ERR("FAil in program bootloader\n");
        return 0;
    }

    return ret ? 0 : dataSzReceived;
}

static int optimus_verify_bootloader(struct ImgBurnInfo* pDownInfo, u8* genSum)
{
    int ret = OPT_DOWN_OK;
    unsigned char* pBuf = (unsigned char*)OPTIMUS_DOWNLOAD_TRANSFER_BUF_ADDR;
    int size = 0;
    int iCopy = 0;
    int bootRealSz = pDownInfo->imgPktSz;

    size=bootRealSz;
    ret = bootloader_read(pBuf, 0, size);
    if (ret) {
        DWN_ERR("Fail to read bootloader\n");
        return __LINE__;
    }

    sha1_csum(pBuf, bootRealSz, genSum);
    return ret;
}


u32 optimus_cb_simg_write_media(const unsigned destAddrInSec, const unsigned dataSzInBy, const char* data)
{
    int ret = OPT_DOWN_OK;
    const char* partName = OptimusImgBurnInfo.partName;

    if (OPTIMUS_MEDIA_TYPE_STORE < OptimusImgBurnInfo.storageMediaType) {
        DWN_ERR("storage type %d not supported yet!\n", OptimusImgBurnInfo.storageMediaType);
        return OPT_DOWN_FAIL;
    }

    DWN_DBG("1addrOffset=0x%llx, dataSz=0x%x, data = 0x%p\t", (((u64)destAddrInSec)<<9), dataSzInBy, (void*)data);
    ret = store_logic_write(partName, (((u64)destAddrInSec)<<9), dataSzInBy, (void*)data);
    if (ret) {
        DWN_ERR("Fail to write to media, ret = %d\n", ret);
        return 0;
    }
    platform_busy_increase_un_reported_size(dataSzInBy);

    return dataSzInBy;
}

//return value: the data size disposed
static u32 optimus_download_sparse_image(struct ImgBurnInfo* pDownInfo, u32 dataSz, const u8* data)
{
    u32 unParsedDataLen = 0;
    int flashOffset = 0;
    const u64 addrOffset = pDownInfo->nextMediaOffset;

    flashOffset = optimus_simg_to_media((char*)data, dataSz, &unParsedDataLen, ((u32)(addrOffset>>9)));
    if (flashOffset < 0) {
        DWN_ERR("Fail in parse simg. src 0x%p, size 0x%x, unParsedDataLen 0x%x, ret %d\n", data, dataSz, unParsedDataLen, flashOffset);
        return 0;
    }
    pDownInfo->nextMediaOffset += ((u64)flashOffset)<<9;

    return dataSz - unParsedDataLen;
}

//Normal image can write directly to NAND, best aligned to 16K when write
//FIXME: check it aligned to 16K when called
//1, write to media     2 -- save the verify info
static u32 optimus_download_normal_image(struct ImgBurnInfo* pDownInfo, u32 dataSz, const u8* data)
{
    int ret = 0;
    u64 addrOrOffsetInBy = pDownInfo->nextMediaOffset;

    DWN_DBG("addrOffset=0x%llx, dataSz=0x%x, data = 0x%p\n", addrOrOffsetInBy, dataSz, data);

    ret = store_logic_write(pDownInfo->partName, addrOrOffsetInBy, dataSz, (void*)data);
    if (ret) {
        DWN_ERR("Fail to write to media\n");
        return 0;
    }
    platform_busy_increase_un_reported_size(dataSz);

    pDownInfo->nextMediaOffset += dataSz;

    return dataSz;
}

static int optimus_storage_open(struct ImgBurnInfo* pDownInfo, const u8* data, const u32 dataSz)
{
    int ret = OPT_DOWN_OK;
    const char* partName = (const char*)pDownInfo->partName;
    const int imgType = pDownInfo->imgType;
    const int MediaType = pDownInfo->storageMediaType;

    if (!pDownInfo->imgSzDisposed && OPTIMUS_IMG_STA_PRE_BURN == pDownInfo->imgBurnSta)
    {
        DWN_MSG("Burn Start...\n");
        pDownInfo->imgBurnSta = OPTIMUS_IMG_STA_BURN_ING;
    }
    else if(pDownInfo->imgSzDisposed == pDownInfo->imgPktSz && OPTIMUS_IMG_STA_BURN_COMPLETE == pDownInfo->imgBurnSta)
    {
        DWN_MSG("Verify Start...\n");
        pDownInfo->imgBurnSta = OPTIMUS_IMG_STA_VERIFY_ING;
    }

    switch (MediaType)
    {
        case OPTIMUS_MEDIA_TYPE_NAND:
        case OPTIMUS_MEDIA_TYPE_SDMMC:
        case OPTIMUS_MEDIA_TYPE_STORE:
            {
                if (IMG_TYPE_BOOTLOADER != pDownInfo->imgType && !pDownInfo->devHdle) //if not bootloader and device not open
                {
                    pDownInfo->devHdle = (void*)1;
                    if (!pDownInfo->devHdle) {
                        DWN_ERR("Fail to open nand part %s\n", partName);
                        return OPT_DOWN_FAIL;
                    }

                    if (IMG_TYPE_SPARSE == imgType)
                    {
                        ret = optimus_simg_probe(data, dataSz);
                        if (!ret) {
                            DWN_ERR("Fail in sparse format probe,ret=%d\n", ret);
                            return OPT_DOWN_FAIL;
                        }
                        return optimus_simg_parser_init(data);
                    }
                }
                else//is bootloader, than do nothing
                {
                    return OPT_DOWN_OK;
                }
            }
            break;

        case OPTIMUS_MEDIA_TYPE_KEY_UNIFY:
            break;

        case OPTIMUS_MEDIA_TYPE_MEM:
            break;

        default:
            DWN_MSG("Error MediaType %d\n", MediaType);
            return OPT_DOWN_FAIL;
    }

    return ret;
}

static int optimus_storage_close(struct ImgBurnInfo* pDownInfo)
{
    if (pDownInfo->imgSzDisposed == pDownInfo->imgPktSz && OPTIMUS_IMG_STA_BURN_ING == pDownInfo->imgBurnSta)
    {
        pDownInfo->imgBurnSta = OPTIMUS_IMG_STA_BURN_COMPLETE;
        DWN_MSG("Burn complete\n");

        return OPT_DOWN_OK;
    }

    if (!pDownInfo->imgSzDisposed && OPTIMUS_IMG_STA_VERIFY_ING == pDownInfo->imgBurnSta)
    {
        pDownInfo->imgBurnSta = OPTIMUS_IMG_STA_VERIFY_END;
        DWN_MSG("Verify End\n");
        return OPT_DOWN_OK;
    }

    return OPT_DOWN_OK;
}


//return value is the data size that actual dealed
static u32 optimus_storage_write(struct ImgBurnInfo* pDownInfo, u64 addrOrOffsetInBy, unsigned dataSz, const u8* data, char* errInfo)
{
    u32 burnSz = 0;
    const u32 imgType = pDownInfo->imgType;
    const int MediaType = pDownInfo->storageMediaType;

    addrOrOffsetInBy += pDownInfo->partBaseOffset;
    DWN_DBG("[0x]Data %p, addrOrOffsetInBy %llx, dataSzInBy %x\n", data, addrOrOffsetInBy, dataSz);

    if (OPTIMUS_IMG_STA_BURN_ING != pDownInfo->imgBurnSta) {
        sprintf(errInfo, "Error burn sta %d\n", pDownInfo->imgBurnSta);
        DWN_ERR(errInfo);
        return 0;
    }

    switch (MediaType)
    {
        case OPTIMUS_MEDIA_TYPE_NAND:
        case OPTIMUS_MEDIA_TYPE_SDMMC:
        case OPTIMUS_MEDIA_TYPE_STORE:
            {
                switch (imgType)
                {
                    case IMG_TYPE_NORMAL:
#if defined(UBIFS_IMG) || defined(CONFIG_CMD_UBIFS)
                    case IMG_TYPE_UBIFS:
#endif// #if defined(UBIFS_IMG) || defined(CONFIG_CMD_UBIFS)
                        burnSz = optimus_download_normal_image(pDownInfo, dataSz, data);
                        break;

                    case IMG_TYPE_BOOTLOADER:
                        burnSz = optimus_download_bootloader_image(pDownInfo, dataSz, data);
                        break;

                    case IMG_TYPE_SPARSE:
                        burnSz = optimus_download_sparse_image(pDownInfo, dataSz, data);
                        break;

                    case IMG_TYPE_DTB:
                        burnSz = optimus_download_dtb_image(pDownInfo, dataSz, data);
                        break;

                    default:
                        DWN_ERR("error image type %d\n", imgType);
                }
            }
            break;

        case OPTIMUS_MEDIA_TYPE_KEY_UNIFY:
            {
                burnSz = v2_key_burn(pDownInfo->partName, data, dataSz, errInfo);
                if (burnSz != dataSz) {//return value is write size
                    DWN_ERR("burn key failed\n");
                    return 0;
                }
            }
            break;

        case OPTIMUS_MEDIA_TYPE_MEM:
        {
            u8* buf = (u8*)addrOrOffsetInBy;
            if (buf != data) {
                DWN_ERR("buf(%llx) != data(%p)\n", addrOrOffsetInBy, data);
                return 0;
            }
            if (!strcmp("dtb", pDownInfo->partName)) //as memory write back size = min[fileSz, 2G], so reach here if downloaded ok!
            {
                unsigned char* dtbLoadAddr = (unsigned char*)OPTIMUS_DTB_LOAD_ADDR;
                unsigned char* srcDownDtb = (unsigned char*)data;

                memcpy(dtbLoadAddr, srcDownDtb, dataSz);
                DWN_MSG("load dt.img to 0x%p, sz=0x%x\n", dtbLoadAddr, dataSz);
                _dtb_is_loaded = dataSz;
            }

            burnSz = dataSz;
        }
        break;

        default:
            sprintf(errInfo, "Error MediaType %d\n", MediaType);
            DWN_ERR(errInfo);
    }

    return burnSz;
}

//TODO: to consist with optimus_storage_write, return value should be readSzInBy
static int optimus_storage_read(struct ImgBurnInfo* pDownInfo, u64 addrOrOffsetInBy,
                            unsigned readSzInBy, unsigned char* buff, char* errInfo)
{
    int ret = 0;
    const int MediaType = pDownInfo->storageMediaType;
    const char* partName = pDownInfo->partName;

    addrOrOffsetInBy += pDownInfo->partBaseOffset;

    switch (MediaType)
    {
        case OPTIMUS_MEDIA_TYPE_NAND:
        case OPTIMUS_MEDIA_TYPE_SDMMC:
        case OPTIMUS_MEDIA_TYPE_STORE:
            {
                if (IMG_TYPE_BOOTLOADER == pDownInfo->imgType)
                {
                    ret = bootloader_read(buff, 0, readSzInBy);
                }
                else if (IMG_TYPE_DTB == pDownInfo->imgType) {
                    ret = store_rsv_read("dtb", readSzInBy, buff);
                }
                else
                {
                    ret = store_logic_read(partName, addrOrOffsetInBy, readSzInBy, buff);
                    platform_busy_increase_un_reported_size(readSzInBy);
                }
                if (ret) {
                    if (errInfo) sprintf(errInfo, "Read failed\n") ;
                    DWN_ERR("Read failed\n");
                    return OPT_DOWN_FAIL;
                }

            }
            break;

        case OPTIMUS_MEDIA_TYPE_KEY_UNIFY:
            {
                unsigned fmtLen = 0;
                if (addrOrOffsetInBy) {
                    DWN_ERR("OH NO, IS key len > 64K!!? addrOrOffsetInBy is 0x%llx not 0\n", addrOrOffsetInBy);
                    return OPT_DOWN_FAIL;
                }
                ret = v2_key_read(pDownInfo->partName, buff, readSzInBy, errInfo, &fmtLen);
            }
            break;

        case OPTIMUS_MEDIA_TYPE_MEM:
        {
            u8* buf = (u8*)addrOrOffsetInBy;
            if (addrOrOffsetInBy >> 32) {
                DWN_ERR("mem addr 0x%llx too large\n", addrOrOffsetInBy);
            }
            if (buf != buff) {
                DWN_ERR("buf(%llx) != buff(%p)\n", addrOrOffsetInBy, buff);
            }
        }
        break;

        default:
            DWN_MSG("Error MediaType %d\n", MediaType);
            return OPT_DOWN_FAIL;
    }

    return ret;
}

//return value is the size actual write to media
//Paras: const char* partName, const char* imgType, const char* verifyAlgorithm
static u32 optimus_func_download_image(struct ImgBurnInfo* pDownInfo, u32 dataSz, const u8* data, char* errInfo)
{
    int burnSz = 0;
    int ret = 0;
    u64 nextMediaOffset = pDownInfo->nextMediaOffset;

    DWN_DBG("data=0x%p, sz=0x%x, offset=%llx\n", data, dataSz, nextMediaOffset);

    ret = optimus_storage_open(pDownInfo, data, dataSz);
    if (OPT_DOWN_OK != ret) {
        sprintf(errInfo, "Fail to open stoarge\n");
        DWN_ERR(errInfo);
        return 0;
    }

    burnSz = optimus_storage_write(pDownInfo, nextMediaOffset, dataSz, data, errInfo);
    if (!burnSz) {
        DWN_ERR("Fail in optimus_storage_write, data 0x%p, wantSz 0x%x\n", data, dataSz);
        goto _err;
    }
    pDownInfo->imgSzDisposed += burnSz;

    ret = optimus_storage_close(pDownInfo);
    if (ret) {
        DWN_ERR("Fail to close media\n");
        return 0;
    }

    return burnSz;

_err:
    optimus_storage_close(pDownInfo);
    pDownInfo->imgBurnSta = OPTIMUS_IMG_STA_BURN_FAILED;////
    return 0;
}

//TODO: add _errInfo as argument to pass more info
static int _parse_img_download_info(struct ImgBurnInfo* pDownInfo, const char* partName,
                                     const u64 imgSz, const char* imgType, const char* mediaType, const u64 partBaseOffset)
{
    int ret = 0;

    memset(pDownInfo, 0, sizeof(struct ImgBurnInfo));//clear burnning info

    //TODO: check format is normal/bootloader if upload!!

    _usbDownPartImgType = "";

    if (!strcmp("sparse", imgType))
    {
        pDownInfo->imgType = IMG_TYPE_SPARSE;
    }
    else if(!strcmp("bootloader", partName))
    {
        pDownInfo->imgType = (u8)IMG_TYPE_BOOTLOADER;
    }
    else if ( !strcmp("_aml_dtb", partName) ) {
        pDownInfo->imgType = (u8)IMG_TYPE_DTB;
    }
    else if(!strcmp("normal", imgType))
    {
        pDownInfo->imgType = (u8)IMG_TYPE_NORMAL;
    }
    else if(!strcmp("ubifs", imgType))
    {
        pDownInfo->imgType = IMG_TYPE_UBIFS;
        _usbDownPartImgType = "ubifs";
    }
    else{
        DWN_ERR("err image type %s\n", imgType);
        return __LINE__;
    }

    if (!strcmp("store", mediaType))
    {
        pDownInfo->storageMediaType = OPTIMUS_MEDIA_TYPE_STORE;
    }
    else if(!strcmp("nand", mediaType))
    {
        pDownInfo->storageMediaType = OPTIMUS_MEDIA_TYPE_NAND;
    }
    else if(!strcmp("sdmmc", mediaType))
    {
        pDownInfo->storageMediaType = OPTIMUS_MEDIA_TYPE_SDMMC;
    }
    else if(!strcmp("spiflash", mediaType))
    {
        pDownInfo->storageMediaType = OPTIMUS_MEDIA_TYPE_SPIFLASH;
    }
    else if(!strcmp("key", mediaType))
    {
        pDownInfo->storageMediaType = OPTIMUS_MEDIA_TYPE_KEY_UNIFY;

        if (OPTIMUS_DOWNLOAD_SLOT_SZ <= imgSz) {
            DWN_ERR("size (0x%llx) for key %s invalid!!\n", imgSz, partName);
            return __LINE__;
        }
    }
    else if(!strcmp("mem", mediaType))
    {
        pDownInfo->storageMediaType = OPTIMUS_MEDIA_TYPE_MEM;
    }
    else{
        DWN_ERR("error mediaType %s\n", mediaType);
        return __LINE__;
    }

    pDownInfo->partBaseOffset   = partBaseOffset;
    memcpy(pDownInfo->partName, partName, strlen(partName));

    if (OPTIMUS_MEDIA_TYPE_MEM > pDownInfo->storageMediaType) //if command for burning partition
    {
        if (strcmp("bootloader", partName) && strcmp("_aml_dtb", partName)) //get size if not bootloader
        {
            u64 partCap = store_part_size(partName);
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
        }
    }

    pDownInfo->nextMediaOffset  = pDownInfo->imgSzDisposed = 0;
    pDownInfo->imgPktSz         = imgSz;
    pDownInfo->imgBurnSta       = OPTIMUS_IMG_STA_PRE_BURN;

    DWN_MSG("Down(%s) part(%s) sz(0x%llx) fmt(%s)\n", mediaType, partName, pDownInfo->imgPktSz, imgType);

    return 0;
}

int optimus_download_init(void)
{
    memset(&OptimusImgBurnInfo, 0, sizeof(struct ImgBurnInfo));
    return 0;
}

int optimus_download_exit(void)
{
    return 0;
}

int optimus_parse_img_download_info(const char* partName, const u64 imgSz, const char* imgType, const char* mediaType, const u64 partBaseOffset)
{
    return _parse_img_download_info(&OptimusImgBurnInfo, partName, imgSz, imgType, mediaType, partBaseOffset);
}

static int _disk_intialed_ok = 0;

int is_optimus_storage_inited(void)
{
    return _disk_intialed_ok;
}

int optimus_save_loaded_dtb_to_flash(void)
{
    unsigned char* dtbLoadedAddr = (unsigned char*)OPTIMUS_DTB_LOAD_ADDR;

    if (!_dtb_is_loaded) return 0;

    //dtb erasing before write, or some NAND chip maybe cannot write real
    store_rsv_erase("dtb");
    return store_rsv_write("dtb", _dtb_is_loaded, dtbLoadedAddr);
}

int optimus_storage_init(int toErase)
{
    int ret = 0;
    char* cmd = NULL;
    unsigned char* dtbLoadedAddr = (unsigned char*)OPTIMUS_DTB_LOAD_ADDR;

    if (_disk_intialed_ok) {//To assert only actual disk intialed once
        DWN_MSG("Disk inited but init again!!!\n");
        /*return 0;*/
    }

    if (OPTIMUS_WORK_MODE_USB_PRODUCE != optimus_work_mode_get()) //Already inited in other work mode
    {
        /*DWN_MSG("Exit before re-init\n");*/
        /*store_exit1();*/
    }

    if (!_dtb_is_loaded) {
        DWN_WRN("dtb is not loaded yet\n");
    }
    else{
#ifdef CONFIG_AML_MTD
        if ( BOOT_NAND_MTD == store_get_type() ) {
            extern int check_valid_dts(unsigned char *buffer);
            ret =  check_valid_dts(dtbLoadedAddr);
        } else
#endif // #ifdef CONFIG_AML_MTD
        ret = get_partition_from_dts(dtbLoadedAddr);

        if (ret) {
            DWN_ERR("Failed at check dts\n");
            return __LINE__;
        }
    }

    ret = store_init(1);
    if (ret <= 0) {
        DWN_MSG("Fail in init storage,ret %d\n", ret);
        return -__LINE__;
    }

    switch (toErase)
    {
        case 0://NO erase
        case 1:
        case 2:
            break;

        case 3://erase all(with key)
            {
                cmd = "store disprotect key";
                DWN_MSG("run cmd [%s]\n", cmd);
                ret = run_command(cmd, 0);
                if (ret) {
                    DWN_ERR("Fail when run cmd[%s], ret %d\n", cmd, ret);
                    break;
                }
            } break;

        case 4://force erase all
            {
                cmd = "store disprotect key; store disprotect hynix";
                DWN_MSG("run cmd [%s]\n", cmd);
                ret = run_command(cmd, 0);
                if (ret) {
                    DWN_ERR("Fail when run cmd[%s], ret %d\n", cmd, ret);
                    break;
                }
            } break;

        default:
            DWN_ERR("Unsupported erase flag %d\n", toErase); ret = -__LINE__;
            break;
    }

    if (toErase > 0) {
        ret = store_erase(NULL, 0, 0, 0);
        if (ret) {
            DWN_ERR("Fail in erase flash,ret %d\n", ret);
            return -__LINE__;
        }
    }

    _disk_intialed_ok += toErase <<16;
    if (OPTIMUS_WORK_MODE_USB_PRODUCE == optimus_work_mode_get()) //env not relocated in this case
    {
        DWN_MSG("usb producing env_relocate\n");
        env_relocate();
    }

    if (_dtb_is_loaded)//for key init, or fail when get /unifykey
    {
        unsigned long fdtAddr = (unsigned long)dtbLoadedAddr;
#ifdef CONFIG_MULTI_DTB
        fdtAddr = get_multi_dt_entry(fdtAddr);
#endif// #ifdef CONFIG_MULTI_DTB
        ret = fdt_check_header((char*)fdtAddr);
        unsigned fdtsz    = fdt_totalsize((char*)fdtAddr);
        if (ret || !fdtsz ) {
            DWN_ERR("Fail in fdt check header\n");
            return __LINE__;
        }
        // if (fdtsz < _dtb_is_loaded)
        memmove((char*)dtbLoadedAddr, (char*)fdtAddr, fdtsz);
    }

    return 0;
}

int is_optimus_on_burn(void)//is now transfering image
{
    return (OPTIMUS_IMG_STA_BURN_ING == OptimusImgBurnInfo.imgBurnSta);
}

int is_optimus_pre_burn(void)    //is now has get "download command"
{
    return (OPTIMUS_IMG_STA_PRE_BURN == OptimusImgBurnInfo.imgBurnSta);
}

int is_optimus_to_burn_ready(void)
{
    return (OPTIMUS_IMG_STA_PRE_BURN == OptimusImgBurnInfo.imgBurnSta);
}

int is_optimus_burn_complete(void)
{
    int is_burn_completed = 0;

    is_burn_completed = (OPTIMUS_IMG_STA_BURN_COMPLETE == OptimusImgBurnInfo.imgBurnSta);
    if (!is_burn_completed) {
        DWN_MSG("imgSzDisposed 0x%llx != imgPktSz 0x%llx\n", OptimusImgBurnInfo.imgSzDisposed, OptimusImgBurnInfo.imgPktSz);
    }

    return is_burn_completed;
}

u32 optimus_download_img_data(const u8* data, const u32 size, char* errInfo)
{
    return optimus_func_download_image(&OptimusImgBurnInfo, size, data, errInfo);
}

static int optimus_sha1sum_verify_partition(const char* partName, const u64 verifyLen, const u8 imgType, u8* genSum)
{
    int ret = 0;
    u8* buff = (u8*) OPTIMUS_SHA1SUM_BUFFER_ADDR;
    const u32 buffSz = OPTIMUS_SHA1SUM_BUFFER_LEN;
    sha1_context ctx;
    u64 leftLen = verifyLen;

    if (strcmp(partName, OptimusImgBurnInfo.partName)) {
        DWN_ERR("partName %s err, must %s\n", partName, OptimusImgBurnInfo.partName);
        return OPT_DOWN_FAIL;
    }

    if (!is_optimus_burn_complete()) {
        return OPT_DOWN_FAIL;
    }

    memset(buff, 0xde, 1024);//clear 1kb data before verfiy, in case read buffer not overlapped
    if (IMG_TYPE_BOOTLOADER == imgType)
    {
        return optimus_verify_bootloader(&OptimusImgBurnInfo, genSum);
    }
    else if (IMG_TYPE_DTB == imgType) {
        return optimus_verify_dtb(&OptimusImgBurnInfo, genSum);
    }
    else if(IMG_TYPE_SPARSE == imgType)//sparse image
    {
        ret = optimus_sparse_back_info_probe();
        if (OPT_DOWN_TRUE != ret) {
            DWN_ERR("Fail to probe back sparse info\n");
            return OPT_DOWN_FAIL;
        }
    }

    ret = optimus_storage_open(&OptimusImgBurnInfo, NULL, 0);
    if (ret) {
        DWN_ERR("Fail to open storage for read\n");
        return OPT_DOWN_FAIL;
    }

    sha1_starts(&ctx);

    DWN_MSG("To verify part %s in fmt %s\n", partName, (IMG_TYPE_SPARSE == imgType) ? "sparse": "normal");
    if (IMG_TYPE_SPARSE == imgType) //sparse image
    {
        for (; leftLen;)
        {
            u32 spHeadSz   = 0;
            u32 chunkDataLen    = 0;
            u64 chunkDataOffset = 0;
            u8* head = NULL;

            ret = optimus_sparse_get_chunk_data(&head, &spHeadSz, &chunkDataLen, &chunkDataOffset);
            if (ret) {
                DWN_ERR("Fail to get chunk data\n");
                goto _finish;
            }

            sha1_update(&ctx, head, spHeadSz);

            leftLen -= spHeadSz + chunkDataLen;//update image read info

            for (;chunkDataLen;)
            {
                const int thisReadLen = (chunkDataLen > buffSz) ? buffSz : chunkDataLen;

                ret = optimus_storage_read(&OptimusImgBurnInfo, chunkDataOffset, thisReadLen, buff, NULL);
                if (ret) {
                    DWN_ERR("Fail to read at offset 0x[%x, %8x], len=0x%8x\n", ((u32)(chunkDataOffset>>32)), (u32)chunkDataOffset, thisReadLen);
                    goto _finish;
                }

                sha1_update(&ctx, buff, thisReadLen);

                chunkDataLen    -= thisReadLen;
                chunkDataOffset += thisReadLen;
            }

            if (leftLen && !spHeadSz) {
                DWN_ERR("Fail to read when pkt len left 0x%x\n", (u32)leftLen);
                break;
            }
        }
    }
    else//normal image
    {
        for (; leftLen;)
        {
            int thisReadLen = 0;
#if defined(UBIFS_IMG) || defined(CONFIG_CMD_UBIFS)
            if (!strcmp(_usbDownPartImgType, "ubifs")) {
                thisReadLen = leftLen;
            } else
#endif// #if defined(UBIFS_IMG) || defined(CONFIG_CMD_UBIFS)
            thisReadLen = (leftLen > buffSz) ? buffSz : ((u32)leftLen);

            u64 addrOffset = verifyLen - leftLen;

            ret = optimus_storage_read(&OptimusImgBurnInfo, addrOffset, thisReadLen, buff, NULL);
            if (ret) {
                DWN_ERR("Fail to read at offset 0x[%x, %8x], len=0x%8x\n", ((u32)(addrOffset>>32)), (u32)addrOffset, thisReadLen);
                goto _finish;
            }

            sha1_update(&ctx, buff, thisReadLen);

            leftLen -= thisReadLen;
        }

    }

_finish:
    OptimusImgBurnInfo.imgSzDisposed = leftLen;
    sha1_finish(&ctx, genSum);
    optimus_storage_close(&OptimusImgBurnInfo);

    return ret;
}

//usage: verify sha1sum nand srcSum part_name size imgType
int optimus_media_download_verify(const int argc, char * const argv[], char *info)
{
    const char* verifyType      = argv[1];
    const char* srcSum          = argv[2];
    static u8  verifyResult[20];
    static char sha1Result[42];
    const u8 srcImgType = OptimusImgBurnInfo.imgType;
    const char* partName = OptimusImgBurnInfo.partName;
    u64 verifyLen = OptimusImgBurnInfo.imgPktSz;
    int ret = 0;

    if (argc != 3) {
        strcpy(info, "failed:need 3 args\n");
        printf(info);
        return -1;
    }

    if (strcmp(verifyType, "sha1sum")) {
        ret = __LINE__;
        sprintf(info, "verifyType [%s] err, ret %d!\n", verifyType, ret);
        DWN_ERR(info);
        return ret;
    }

    ret = optimus_sha1sum_verify_partition(partName, verifyLen, srcImgType, verifyResult);
    if (ret) {
        DWN_ERR("Fail to gen check sum\n");
        return __LINE__;
    }

    ret = optimus_hex_data_2_ascii_str(verifyResult, 20, sha1Result, 42);
    if (ret) {
        DWN_ERR("Failed when format sha1 to string\n");
        return __LINE__;
    }

    /*DWN_MSG("%s %s\n", verifyType, sha1Result);*/
    ret = strcmp(sha1Result, srcSum);
    if (ret) {
        sprintf(info, "failed:Verify Failed with %s, origin sum \"%s\" != gen sum \"%s\"\n", verifyType, srcSum, sha1Result);
        DWN_ERR(info);
        return __LINE__;
    }

    DWN_MSG("VERIFY OK \n");
    return ret;
}

int optimus_key_burn_init(const char* keyType)
{
    int ret = 0;

    if (!strcmp("efuse", keyType))
    {
        return ret;
    }

    if (!strcmp("secure", keyType))
    {
        return ret;
    }

    DWN_ERR("unsported key type %s\n", keyType);
    return OPT_DOWN_FAIL;
}

//update tplcmd dev0 "download nand part_name imageType imgSz"
//update tplcmd dev0 "download get_status"
int optimus_parse_download_cmd(int argc, char* argv[])
{
    const int isUpload = !strcmp("upload", argv[0]);
    const char* mediaType   = argv[1];
    const char* part_name   = argv[2];
    const char* imgType     = argv[3];
    const char* imgSzStr    = argv[4];
    u64   imgSzInBy   = 0;
    u64   partBaseOffset = 0;
    int ret = 0;

    if (!strcmp("get_status", mediaType))
    {
        return !is_optimus_burn_complete();
    }

    if (!strcmp("is_ready", mediaType))
    {
        return !is_optimus_to_burn_ready();
    }

    if (5 > argc) {
        printf("argc[%d] too few, use \"download nand part_name imageType imgSz\"\n", argc);
        return  __LINE__;
    }

    imgSzInBy = simple_strtoull(imgSzStr, NULL, 0);

    if (!strcmp("mem", mediaType))
    {
        char* endp = NULL;
        partBaseOffset = simple_strtoull(part_name, &endp, 0);
        if (0 != *endp) //not a valid 0-terminated c string
        {
            if (!strcmp("dtb", part_name))
            {
                partBaseOffset = OPTIMUS_DOWNLOAD_TRANSFER_BUF_ADDR;
                DWN_DBG("dtb down to %llx\n", partBaseOffset);
            }
        }
    }

    ret = optimus_parse_img_download_info(part_name, imgSzInBy, imgType, mediaType, partBaseOffset);
    if (ret) {
        DWN_ERR("Fail in init download info\n");
        return __LINE__;
    }

    ret = optimus_buf_manager_tplcmd_init(mediaType, part_name, partBaseOffset, imgType, imgSzInBy, isUpload, 0);
    if (ret) {
        DWN_ERR("Fail in init download info\n");
        return __LINE__;
    }

    return OPT_DOWN_OK;
}

u32 optimus_dump_storage_data(u8* pBuf, const u32 wantSz, char* errInfo)
{
    struct ImgBurnInfo* pDownInfo = &OptimusImgBurnInfo;
    u64 nextMediaOffset = pDownInfo->nextMediaOffset;
    int ret = 0;

    DWN_DBG("pBuf=0x%p, wantSz=0x%x, nextMediaOffset=%x\n", pBuf, wantSz, (u32)nextMediaOffset);

    pDownInfo->isDumpMode = 1; //do not do any erase in dump mode
    ret = optimus_storage_open(pDownInfo, pBuf, wantSz);
    if (OPT_DOWN_OK != ret) {
        sprintf(errInfo, "Fail to open stoarge\n");
        DWN_ERR(errInfo);
        return 0;
    }

    ret = optimus_storage_read(pDownInfo, nextMediaOffset, wantSz, pBuf, errInfo);
    if (ret) {
        DWN_ERR("Failed \n");
        goto _err;
    }
    pDownInfo->imgSzDisposed    += wantSz;
    pDownInfo->nextMediaOffset  += wantSz;

    ret = optimus_storage_close(pDownInfo);
    if (ret) {
        DWN_ERR("Fail to close media\n");
        return 0;
    }

    return wantSz;

_err:
    optimus_storage_close(pDownInfo);
    pDownInfo->imgBurnSta = OPTIMUS_IMG_STA_BURN_FAILED;////
    return 0;
}

static int _optimusWorkMode = OPTIMUS_WORK_MODE_NONE;

int optimus_work_mode_get(void)
{
    return _optimusWorkMode;
}

int optimus_work_mode_set(int workmode)
{
    _optimusWorkMode = workmode;
    return 0;
}

int is_the_flash_first_burned(void)
{
    const char* s = getenv("upgrade_step");

    DWN_MSG("====>upgrade_step=%s<=====\n", s ? s : "<UNDEFINED>");

    return !strcmp(s, "0");//"0" indicate first boot
}

//FIXME: check whether 'saveenv' failed and exception when usb prodcing mode from code boot mode if without env_relocate
int optimus_set_burn_complete_flag(void)
{
    int rc = 0;
#if defined(CONFIG_CMD_SAVEENV) && !defined(CONFIG_ENV_IS_NOWHERE)
    const int IsTplLoadedFromBurningPackage = aml_burn_check_uboot_loaded_for_burn(0);
    char upgrade_step[8];

    if (IsTplLoadedFromBurningPackage)
    {
        /*rc = run_command("defenv", 0);//use new env directly if uboot is new !!!*/
        set_default_env("## save_setting ##\n", 0);//use new env directly if uboot is new !!!
#if 0
        const char* def_env_initargs = getenv("initargs");
        const char* def_env_bootargs = getenv("bootargs");
        if (!strstr(getenv("initargs"), "storage") && getenv("initargs") && 0) {
                rc = run_command("setenv initargs ${initargs} storage=${store}", 0);
                DWN_MSG("[initargs=%s]\n", getenv("initargs"));
        }
        else if(!strstr(getenv("bootargs"), "storage") && getenv("bootargs") && 0){//user not configure storage in 'bootargs' of default env
                rc = run_command("setenv bootargs ${bootargs} storage=${store}", 0);
                DWN_MSG("[bootargs=%s]\n", getenv("bootargs"));
        }
#endif
    }

    //Enable firstboot when flash empty to wipe_data/wipe_cache
    setenv("firstboot", "1");
    upgrade_step[0] = '1' + IsTplLoadedFromBurningPackage;
    upgrade_step[1] = '\0';
    DWN_MSG("Set upgrade_step to %s\n", upgrade_step);
    rc = setenv("upgrade_step", upgrade_step);
    if (rc) {
        DWN_ERR("Fail to set upgraded_step to 1\n");
    }
    rc = run_command("saveenv", 0);
    if (rc) {
        DWN_ERR("Fail to saveenv to flash\n");
    }
    udelay(200);
#endif//#if defined(CONFIG_CMD_SAVEENV) && !defined(CONFIG_ENV_IS_NOWHERE)

    return rc;
}

static int _optimus_set_reboot_mode(const int cfgFlag)
{
    char cmdbuf[96];
    const char* reboot_mode = NULL;
    switch (cfgFlag)
    {
        case OPTIMUS_BURN_COMPLETE__REBOOT_UPDATE:
            reboot_mode = "update";
            break;

        case OPTIMUS_BURN_COMPLETE__REBOOT_SDC_BURN:
            /*reboot_mode = MESON_SDC_BURNER_REBOOT;  */
            break;

        case OPTIMUS_BURN_COMPLETE__REBOOT_NORMAL:
        default:
            reboot_mode = "normal";
            break;
    }

    sprintf(cmdbuf, "reboot %s", reboot_mode);

    return run_command(cmdbuf, 0);
}

void optimus_reset(const int cfgFlag)
{
    unsigned i = 0x100;

    //set reboot mode
    _optimus_set_reboot_mode(cfgFlag);
    printf("Burn Reboot...\n");//Add printf to delay to save env
    while (--i > 0) {;}

    /*disable_interrupts();*/
	reset_cpu(0);

    while (i++)
    {
        unsigned ret = i;
        unsigned mask = 1U<<20;

        mask -= 1;
        ret &= mask;
        if (!ret) {
            printf("To reseting...\n");
        }
    }
}

void optimus_poweroff(void)
{
#ifndef CONFIG_POWER_KEY_SUPPORTED_FOR_BURN //default not support power key
    DWN_MSG("stop here as poweroff and powerkey not supported in platform!\n");
    DWN_MSG("You can <Ctrl-c> to reboot\n");
    while (!ctrlc()) continue;
    optimus_reset(OPTIMUS_BURN_COMPLETE__REBOOT_NORMAL);
#else
    printf("To poweroff\n");
    run_command("poweroff", 0);
    printf("!!!After run command poweroff!!\n");
#endif// #if CONFIG_POWER_KEY_SUPPORTED_FOR_BURN

    return;
}

//use choice = 0xfu to query is_burn_completed
int optimus_burn_complete(const int choice)
{
    static unsigned _isBurnComplete = 0;
    int rc = 0;

    switch (choice)
    {
        case OPTIMUS_BURN_COMPLETE__POWEROFF_AFTER_POWERKEY://wait power key to power off, for sdc_burn
            {
#ifndef CONFIG_POWER_KEY_SUPPORTED_FOR_BURN
                    optimus_poweroff();
#endif// #if CONFIG_POWER_KEY_SUPPORTED_FOR_BURN
                    DWN_MSG("PLS short-press power key to shut down\n");
                    do
                    {
                            rc = run_command("getkey", 0);
                    }while(rc);
            }
        case OPTIMUS_BURN_COMPLETE__POWEROFF_DIRECT:
            optimus_poweroff();
            break;

        case OPTIMUS_BURN_COMPLETE__POWEROFF_AFTER_DISCONNECT:
            DWN_MSG("Pls un-plug USB line to poweroff\n");
            _isBurnComplete = 0xefe;
            break;
        case OPTIMUS_BURN_COMPLETE__QUERY:
            return (0xefe == _isBurnComplete);

        case OPTIMUS_BURN_COMPLETE__REBOOT_UPDATE:
        case OPTIMUS_BURN_COMPLETE__REBOOT_NORMAL:
            {
                optimus_reset(choice);
            }
            break;


        default:
            rc = 1;
            DWN_ERR("Error burn_complete flag %d\n", choice);
    }

    return rc;
}

