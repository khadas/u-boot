/*
 * \file        v3_tool_def.h
 * \brief       common definitions for v3 factory tools
 *
 * \version     1.0.0
 * \date        2018/04/11
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2018 Amlogic. All Rights Reserved.
 *
 */
#ifndef __V3_TOOL_DEF_H__
#define __V3_TOOL_DEF_H__
#include <config.h>
#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <environment.h>
#include <asm/string.h>
#include <asm/byteorder.h>
#include <cli.h>
#include <u-boot/sha1.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/io.h>
#include <asm/arch/bl31_apis.h>
#include <partition_table.h>
#include <amlogic/aml_efuse.h>
#include <amlogic/keyunify.h>
#include <amlogic/store_wrapper.h>

extern unsigned long get_multi_dt_entry(unsigned long fdt_addr);
extern void f_dwc_otg_pullup(int is_on);

extern char* fb_response_str;
#define _ACK fb_response_str

#undef FB_DBG
#undef FB_ERR
#undef FB_MSG
#define FB_ERR(fmt ...) printf("[ERR]%s:L%d:", __func__, __LINE__),printf(fmt)
#define FB_EXIT(fmt ...) do{FB_ERR(fmt);return -__LINE__;}while(0)
#define FB_MSG(fmt ...) printf("[MSG]"fmt)
#define FB_WRN(fmt ...) printf("[WRN]"fmt)
#define FB_DBG(fmt...)  //printf("[DBG]"fmt)
#define FB_HERE()    printf("f(%s)L%d\n", __func__, __LINE__)

#define RESPONSE_LEN	(128)
#define FBS_ERR(buf, fmt ...) do {\
    snprintf((buf),RESPONSE_LEN - 4, fmt);\
    printf("[ERR]%sL%d:%s\n", __func__, __LINE__, (buf));\
}while(0)
#define FBS_EXIT(buf, fmt ...) do {FBS_ERR(buf, fmt); return -__LINE__;} while(0)
#define _mymin(a,b) ( (a) > (b) ? (b) : (a) )

unsigned add_sum(const void* pBuf, const unsigned size);//Add-sum used for 64K transfer
int optimus_erase_bootloader(const char* extBootDev);
void optimus_clear_ovd_register(void);

//outStr will be null-terminater after format
int optimus_hex_data_2_ascii_str(const unsigned char* hexData, const unsigned nBytes, char* outStr, const unsigned strSz);

//for prompting step info
int optimus_progress_init(const unsigned itemSzHigh, const unsigned itemSzLow, const u32 startStep, const u32 endStep);
int optimus_progress_exit(void);
int optimus_update_progress(const unsigned thisBurnSz);

enum {
    V3TOOL_WORK_MODE_USB_UPDATE = (0xefe5),
    V3TOOL_WORK_MODE_USB_PRODUCE ,
    V3TOOL_WORK_MODE_SDC_UPDATE  ,
    V3TOOL_WORK_MODE_SDC_PRODUCE ,
    V3TOOL_WORK_MODE_UDISK_UPDATE,
    V3TOOL_WORK_MODE_SYS_RECOVERY,
};

#ifndef CONFIG_DTB_MEM_ADDR
#define CONFIG_DTB_MEM_ADDR     0x1000
#endif// #ifndef CONFIG_DTB_MEM_ADDR
#define V3_DTB_LOAD_ADDR    CONFIG_DTB_MEM_ADDR

#define EP_BUFFER_SIZE          4096	//temp buffer for command 'Download'
#define V3_DOWNLOAD_EP_DATA     ( 0x02<<20 ) //CONFIG_DTB_MEM_ADDR + 2M
#define V3_DOWNLOAD_EP_OUT      (V3_DOWNLOAD_EP_DATA)
#define V3_DOWNLOAD_EP_IN       (V3_DOWNLOAD_EP_OUT + EP_BUFFER_SIZE * 2)

#define V3_DOWNLOAD_MEM_BASE    (V3_DOWNLOAD_EP_DATA) //CONFIG_DTB_MEM_ADDR + 3M
#define V3_DOWNLOAD_MEM_SIZE    (0X8<<20)    //each mwrite size for sparse image
#define V3_DOWNLOAD_SPARE_SZ    (0X8<<10)    //8k for sparse spare
#define V3_DOWNLOAD_VERIFY_INFO (V3_DOWNLOAD_MEM_BASE + V3_DOWNLOAD_MEM_SIZE + V3_DOWNLOAD_SPARE_SZ)//1MB for temp use
#define V3_DOWNLOAD_VERIFY_INFO_SZ (0x1<<20)//

#define _RAW_IMG_TRANSFER_LEN (128<<10)	//each mwrite size for raw image
#define _UNIFYKEY_MAX_SZ       (256<<10)

#define V3_PAYLOAD_LOAD_ADDR    (CONFIG_DTB_MEM_ADDR + 0x100000) //sheader for sc2 nand

enum {
    V3TOOL_PART_IMG_FMT_RAW     = 0xabcd,
    V3TOOL_PART_IMG_FMT_SPARSE          ,
    V3TOOL_PART_IMG_FMT_UBIFS           ,
};

enum {
    V3TOOL_MEDIA_TYPE_STORE     = 0xefee,
    V3TOOL_MEDIA_TYPE_MEM               ,
    V3TOOL_MEDIA_TYPE_UNIFYKEY          ,
    V3TOOL_MEDIA_TYPE_MMC               ,//1-->emmc, 0-->sdcard
};

#define V3_PART_NAME_LEN   32
typedef struct {
    char    partName[V3_PART_NAME_LEN];
    int     mediaType;
    int64_t imgSzTotal;
    int64_t partStartOff;//
}ImgCommonPara;

typedef struct {
    ImgCommonPara commonInf;
    int             imgFmt;
    int             needVerify;

}ImgDownloadPara;

typedef struct {
    ImgCommonPara     commonInf;
}ImgUploadPara;

typedef union {
    ImgCommonPara   commonInf;
    ImgDownloadPara   download;
    ImgUploadPara   upload;

}ImgTransPara;

typedef struct {
    //data size from @fileOffset, END transfer if '0==@dataSize'
    int         dataSize;
    int64_t     fileOffset;
    char*       dataBuf;

}UsbDownInf;

typedef struct {
    int         dataSize;
    char*       dataBuf;
}UsbUpInf;

typedef union {
    UsbDownInf        downloadInfo;
    UsbUpInf          uploadInfo;

}TransferInfo;


int v3tool_buffman_img_init(ImgTransPara* imgInf, const int isDownload);
int v3tool_buffman_next_download_info(UsbDownInf** downloadInf);
int v3tool_buffman_data_complete_download(const UsbDownInf* downloadInf);
int v3tool_buffman_img_verify_sha1sum(unsigned char* vrySum);

int v3tool_buffman_next_upload_info(UsbUpInf** uploadInfo);
int v3tool_buffman_data_complete_upload(const UsbUpInf* uploadInf);

//for usb
#ifndef USE_FULL_SPEED
#define BULK_EP_MPS	(512)		//full speed
#define DWC_BLK_MAX_LEN         (8*BULK_EP_MPS)
#else
#define BULK_EP_MPS	(64)		//full speed
#define DWC_BLK_MAX_LEN         (6*BULK_EP_MPS)
#endif// #ifndef USE_FULL_SPEED
#define DWC_BLK_LEN(leftSz)     ((leftSz) >= DWC_BLK_MAX_LEN ? DWC_BLK_MAX_LEN : \
                                (leftSz >= BULK_EP_MPS ? ((leftSz/BULK_EP_MPS)*BULK_EP_MPS): leftSz))
#define DWC_BLK_NUM(totalTransLen)  ( (totalTransLen/DWC_BLK_MAX_LEN) + \
                                    ( (totalTransLen & (DWC_BLK_MAX_LEN-1)) >= BULK_EP_MPS ? 1 : 0 ) +\
                                    ( (totalTransLen & (BULK_EP_MPS-1)) ? 1 : 0 ) )

int v3tool_storage_init(int toErase, unsigned dtbImgSz);
int v3tool_storage_exit(void);
int is_v3tool_storage_inited(void);
int v3tool_is_flash_erased(void);

int bootloader_read(u8* pBuf, unsigned off, unsigned binSz);
int bootloader_write(u8* dataBuf, unsigned off, unsigned binSz);
int store_dtb_rw(void* buf, unsigned dtbSz, int rwFlag);

//for key opearations
//
#ifdef CONFIG_V3_KEY_BURNING_SUPPORT
int v2_key_command(const int argc, char * const argv[], char *info);

/*
 *This fucntion called by mread command, mread= bulkcmd "upload key .." + n * upload transfer, for key n==1
 *Attentions: return 0 if success, else failed
 *@keyName: key name in null-terminated c style string
 *@keyVal: the buffer to read back the key value
 *@keyValLen: keyVal len is strict when read, i.e, user must know the length of key he/she wnat to read!!
 *@errInfo: start it with success if burned ok, or format error info into it tell pc burned failed
 */
int v2_key_read(const char* keyName, u8* keyVal, const unsigned keyValLen, char* errInfo, unsigned* fmtLen);

/*
 *This fucntion called by mwrite command, mread= bulkcmd "download key .." + n * download transfer, for key n==1
 *Attentions: return value is the key length if burn sucess
 *@keyName: key name in null-terminated c style string
 *@keyVal: key value download from USB, "the value for sepecial keyName" may need de-encrypt by user code
 *@keyValLen: the key value downloaded from usb transfer!
 *@errInfo: start it with success if burned ok, or format error info into it tell pc burned failed
 */
unsigned v2_key_burn(const char* keyName, const u8* keyVal, const unsigned keyValLen, char* errInfo);
#endif//#ifdef CONFIG_V3_KEY_BURNING_SUPPORT


int v3tool_media_check_image_size(int64_t imgSz, const char* partName);
#define V3TOOL_WORK_MODE_NONE            0
#define V3TOOL_WORK_MODE_USB_UPDATE      (0xefe5)
#define V3TOOL_WORK_MODE_USB_PRODUCE     (0xefe6)
#define V3TOOL_WORK_MODE_SDC_UPDATE      (0xefe7)
#define V3TOOL_WORK_MODE_SDC_PRODUCE     (0xefe8)
#define V3TOOL_WORK_MODE_SYS_RECOVERY    (0xefe9)
int v3tool_work_mode_get(void);
int v3tool_work_mode_set(int workmode);

void v3tool_media_set_busy(const char* info);
void v3tool_media_set_free(const char* info);
int v3tool_media_is_busy(void);

#ifdef SYSCTRL_SEC_STATUS_REG4
//#define P_AO_SEC_SD_CFG9 	SYSCTRL_SEC_STATUS_REG1
#define P_AO_SEC_GP_CFG0 	SYSCTRL_SEC_STATUS_REG4
#define P_PREG_STICKY_REG2	SYSCTRL_SEC_STICKY_REG2
#endif// #ifndef P_AO_SEC_SD_CFG0

#endif//#ifndef __V3_TOOL_DEF_H__

