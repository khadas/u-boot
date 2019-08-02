/*
 * \file        v3_tool_buff_manager.c
 * \brief       buffer manager for partition image
 *
 * \version     1.0.0
 * \date        2018/04/11
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2018 Amlogic. All Rights Reserved.
 *
 */
#include "../include/v3_tool_def.h"

int v3tool_simg2img_init(const ImgDownloadPara* downPara);
int v3tool_simg2img_get_img(UsbDownInf* downInf);
int v3tool_simg2img_write_img(const UsbDownInf* downInf, const ImgDownloadPara* downPara);
int v3tool_simg2img_verify_img(sha1_context* ctx, const char* partName, int64_t partBase);
#ifndef CONFIG_V3_KEY_BURNING_SUPPORT
static int v2_key_read(const char* keyName, u8* keyVal, const unsigned keyValLen, char* errInfo, unsigned* fmtLen)
{
    FB_ERR("burn key not supported as CONFIG_UNIFY_KEY_MANAGE undef!!");
    return -1;
}
static unsigned v2_key_burn(const char* keyName, const u8* keyVal, const unsigned keyValLen, char* errInfo)
{
    FB_ERR("burn key not supported as CONFIG_UNIFY_KEY_MANAGE undef!!");
    return -1;
}

static int v2_key_command(const int argc, char * const argv[], char *info)
{
    FB_ERR("burn key not supported as CONFIG_UNIFY_KEY_MANAGE undef!!");
    return -1;
}
#define key_manage_query_size(a,b) 1
#endif// //#ifdef CONFIG_V3_KEY_BURNING_SUPPORT

static struct {
    ImgTransPara        imgTransPara;//user para
    int                 isDownload;
    int                 dataBufCap;
    char*               dataBuf;
    char*               verifyInfo;//verifyInfo will saved while downloading, and used while verify
    int                 inited;

}_imgTransferInfo;

static UsbDownInf _usbDownInf = {0};
static UsbUpInf _usbUpInf = {0};
static int64_t _rawImgFileOffset = 0;
static int64_t _rawImgVryLen = 0;

//isn't pc get busy info and so still waiting
static int _dnl_check_if_PC_waiting(void)
{
    return !strncmp(fb_response_str - 4, "INFO", 4);
}

int v3tool_buffman_img_verify_sha1sum(unsigned char* vrySum)
{
    static sha1_context ctx;
    const ImgDownloadPara* imgDownPara = &_imgTransferInfo.imgTransPara.download;
    const ImgCommonPara*   imgCmnPara  = &imgDownPara->commonInf;
    const int              media       = imgCmnPara->mediaType;
    const int              imgFmt      = imgDownPara->imgFmt;
    const char*            part        = imgCmnPara->partName;
    const int64_t          imgTotalLen = imgCmnPara->imgSzTotal;
    const int64_t          partBase    = imgCmnPara->partStartOff;
    unsigned char*         vryBuff     = (unsigned char*)V3_DOWNLOAD_MEM_BASE;
    const int              vryBuffLen  = (2<<20);
    int64_t                vryLen      = _rawImgVryLen;
    int                     ret        = 0;
    if (!_imgTransferInfo.isDownload) {
        FBS_ERR(_ACK, "only download can support verify now.");
        return -__LINE__;
    }
    if ( V3TOOL_MEDIA_TYPE_STORE != media ) {
        FBS_ERR(_ACK, "verify can'tsupport media[%d]", media);
        return -__LINE__;
    }
    //report busy, then continue verify at next pc bulk-in request
    if ( !_dnl_check_if_PC_waiting() ) {
        sha1_starts(&ctx);
        FB_DBG("init ctx\n");
    }

    if ( !strcmp("bootloader", part) ) {
        ret = bootloader_read(vryBuff, 0, imgTotalLen);
        sha1_update(&ctx, vryBuff, imgTotalLen);
    } else if ( !strcmp("_aml_dtb", part) ) {
        ret = store_dtb_rw(vryBuff, imgTotalLen, 2);
        sha1_update(&ctx, vryBuff, imgTotalLen);
    } else {
        switch (imgFmt)
        {
            case V3TOOL_PART_IMG_FMT_RAW:
                {
                    int thisVryLen = 0;
                    static unsigned long _lastReportTick = 0;
                    if (!vryLen) _lastReportTick = get_timer(0);
                    for (; vryLen < imgTotalLen; vryLen += thisVryLen)
                    {
                        thisVryLen = imgTotalLen - vryLen;
                        thisVryLen = thisVryLen > vryBuffLen ? vryBuffLen : thisVryLen;
                        ret = store_logic_read(part, vryLen + partBase, thisVryLen, vryBuff);
                        if ( ret ) {
                            FBS_ERR(_ACK, "FAil in store read");
                            return -__LINE__;
                        }
                        sha1_update(&ctx, vryBuff, thisVryLen);
                        if (get_timer(_lastReportTick) >= 4000) {
                            _lastReportTick += get_timer(_lastReportTick);
                            _rawImgVryLen = vryLen + thisVryLen;
                            char info[64];
                            sprintf(info, "%06lld / %06lld MBytes", (vryLen>>20), (imgTotalLen>>20));
                            v3tool_media_set_busy(info);
                            return 0;
                        }
                    }

                }break;
            case V3TOOL_PART_IMG_FMT_SPARSE:
                {
                    ret = v3tool_simg2img_verify_img(&ctx, part, partBase);
                    if ( ret ) {
                        FB_ERR("fail in very img,ret %d\n", ret);
                        return -__LINE__;
                    }
                    if (v3tool_media_is_busy()) {
                        return 0;
                    }
                }break;
            default:
                FBS_ERR(_ACK, "imgFmt[%x] cannot verify", imgFmt);
                return -__LINE__;
        }
    }
    sha1_finish(&ctx, vrySum);
    v3tool_media_set_free(NULL);
    return 0;
}

//@imgPara:
//@isDownload: 1 if download, 0 if upload
//@needVerify: 1 if need verify, if need verify, will save some info while downloading
//              will ignored if not download mode
int v3tool_buffman_img_init(ImgTransPara* imgPara, const int isDownload)
{
    _imgTransferInfo.isDownload = isDownload;
    memcpy(&_imgTransferInfo.imgTransPara, imgPara, sizeof(ImgTransPara));
    _imgTransferInfo.dataBuf = (char*)V3_DOWNLOAD_MEM_BASE;
    _imgTransferInfo.dataBufCap = V3_DOWNLOAD_MEM_SIZE;
    _imgTransferInfo.verifyInfo = isDownload ? (char*)V3_DOWNLOAD_VERIFY_INFO : NULL;
    _imgTransferInfo.inited     = 1;
    ImgCommonPara*   commonInf = &imgPara->commonInf;
    const int64_t imgSize = commonInf->imgSzTotal;
    int ret = 0;

    switch (commonInf->mediaType)
    {
        case V3TOOL_MEDIA_TYPE_MEM:
            { //User should make sure mem address is able to access by himself if media is mem
                _imgTransferInfo.dataBuf = (char*)commonInf->partStartOff;
                _imgTransferInfo.dataBufCap = imgSize;
                _imgTransferInfo.verifyInfo = NULL;
            } break;
        case V3TOOL_MEDIA_TYPE_STORE:
            {
                ret = v3tool_media_check_image_size(imgSize, commonInf->partName);
                if ( ret ) {
                    FB_EXIT("Fail in check img sz\n");
                }
            }break;
        case V3TOOL_MEDIA_TYPE_UNIFYKEY:
            {
                if (!isDownload) {
                    const char* queryKey = commonInf->partName;
                    ssize_t keySz = 0;
                    ret = key_manage_query_size(queryKey,&keySz);
                    if (ret) {
                        FB_EXIT("Fail get sz for key[%s]\n", queryKey);
                    }
                    if (keySz != imgSize) {
                        FB_EXIT("key[%s] sz %zd != cmd key sz %lld\n", queryKey, keySz, imgSize);
                    }
                }
            } break;
        default:
            FB_EXIT("Exception, err media type 0x%x\n", commonInf->mediaType);
    }
    if (V3TOOL_PART_IMG_FMT_SPARSE == imgPara->download.imgFmt && isDownload) {
        imgPara->download.needVerify = 1;
        if (v3tool_simg2img_init(&imgPara->download)) {
            FB_ERR("Fail in simg init\n");
            return -__LINE__;
        }
    }
    //Init _usbDownInf for raw image
    _rawImgFileOffset  = 0;
    _rawImgVryLen      = 0;
    optimus_progress_init((unsigned)(imgSize>>32), (unsigned)imgSize, 0, 100);

    return 0;
}

static int _v3tool_buffman_next_download_info_rawimg(ImgDownloadPara* imgPara)
{
    ImgCommonPara* cmnInf = &imgPara->commonInf;
    switch (cmnInf->mediaType) {
        case V3TOOL_MEDIA_TYPE_MEM:
            {
                _usbDownInf.fileOffset = _rawImgFileOffset;
                int64_t leftLen        = cmnInf->imgSzTotal - _rawImgFileOffset;
                _usbDownInf.dataSize   = _mymin(leftLen, _RAW_IMG_TRANSFER_LEN);
                _usbDownInf.dataBuf    = _imgTransferInfo.dataBuf + _rawImgFileOffset;
                FB_DBG("dataBuf %p, dataSize %x\n", _usbDownInf.dataBuf, _usbDownInf.dataSize);
            }break;
        case V3TOOL_MEDIA_TYPE_STORE:
            {
                _usbDownInf.fileOffset = _rawImgFileOffset;
                int64_t leftLen        = cmnInf->imgSzTotal - _rawImgFileOffset;
                if (strcmp("bootloader", cmnInf->partName) && strcmp("_aml_dtb", cmnInf->partName))
                {_usbDownInf.dataSize   = _mymin(leftLen, _RAW_IMG_TRANSFER_LEN);}
                else _usbDownInf.dataSize   = leftLen;
                _usbDownInf.dataBuf    = (char*)V3_DOWNLOAD_MEM_BASE;
            }break;
        case V3TOOL_MEDIA_TYPE_UNIFYKEY:
            {
                _usbDownInf.fileOffset = _rawImgFileOffset;
                _usbDownInf.dataSize   = cmnInf->imgSzTotal - _rawImgFileOffset;
                _usbDownInf.dataBuf    = (char*)V3_DOWNLOAD_MEM_BASE;
            } break;
        default:
            {
                FB_EXIT("err media type %d\n", cmnInf->mediaType);
            }break;
    }

    //for next raw img download
    _rawImgFileOffset += _usbDownInf.dataSize;
    return 0;
}

int v3tool_buffman_next_download_info(UsbDownInf** downloadInf)
{
    int ret = 0;
    *downloadInf = NULL;
    if (!_imgTransferInfo.inited || !_imgTransferInfo.isDownload) {
        FBS_ERR(_ACK, "buffman %d, %d\n", _imgTransferInfo.inited ,_imgTransferInfo.isDownload);
        return -__LINE__;
    }

    ImgDownloadPara* imgPara = &_imgTransferInfo.imgTransPara.download;
    ImgCommonPara* cmnInf = &imgPara->commonInf;
    switch (imgPara->imgFmt) {
        case V3TOOL_PART_IMG_FMT_RAW:
            {
                ret = _v3tool_buffman_next_download_info_rawimg(imgPara);
                if ( ret ) FB_EXIT("FAil in get buf for raw img\n");
                *downloadInf = &_usbDownInf;
                return 0;
            }
        case V3TOOL_PART_IMG_FMT_SPARSE:
            {
                switch (cmnInf->mediaType) {
                    case V3TOOL_MEDIA_TYPE_STORE:
                        {
                            ret = v3tool_simg2img_get_img(&_usbDownInf);
                            if ( !ret ) *downloadInf = &_usbDownInf;
                            else {
                                FBS_ERR(_ACK, "fail in get next sp img data");
                                return -__LINE__;
                            }
                        }break;
                    default:
                        {
                            FBS_ERR(fb_response_str, "err media %d for sparse fmt", cmnInf->mediaType);
                        }break;
                }
            }break;
        default:
            FBS_ERR(_ACK, "unsupported imgFmt 0x%x", imgPara->imgFmt);
            return -__LINE__;
    }

    return ret;
}

int v3tool_buffman_data_complete_download(const UsbDownInf* downloadInf)
{
    ImgCommonPara* cmnInf = &_imgTransferInfo.imgTransPara.commonInf;
    ImgDownloadPara* downInf = &_imgTransferInfo.imgTransPara.download;
    const int imgFmt     = downInf->imgFmt;
    /*const int needVerify = downInf->needVerify;*/
    const int mediaType = cmnInf->mediaType;
    const char* partName = cmnInf->partName;
    int ret = 0;
    const int thisTransferLen = downloadInf->dataSize;
    u8* dataBuf = (u8*)downloadInf->dataBuf;

    switch ( imgFmt ) {
        case V3TOOL_PART_IMG_FMT_RAW:
            {
                switch ( mediaType ) {
                    case V3TOOL_MEDIA_TYPE_STORE:
                        {
                            if ( !strcmp("bootloader", partName) ) {
                                ret = bootloader_write(dataBuf, 0, thisTransferLen);
                            } else if ( !strcmp("_aml_dtb", partName) ) {
                                ret = store_dtb_rw(dataBuf, thisTransferLen, 1);
                            }
                            else {
                                ret = store_logic_write(partName, downloadInf->fileOffset, thisTransferLen, dataBuf);
                            }
                        } break;
                    case V3TOOL_MEDIA_TYPE_MEM:
                        break;
                    case V3TOOL_MEDIA_TYPE_UNIFYKEY:
                        ret = v2_key_burn(partName, dataBuf, thisTransferLen, _ACK);
                        if (ret != thisTransferLen) {
                            FBS_ERR(_ACK, "err in program key to media,ret %d", ret);
                            ret = -__LINE__;
                        } else ret = 0;
                        break;
                    default:
                        FBS_ERR(fb_response_str, "err media type %d for raw img", mediaType);
                        break;
                }
            }break;
        case V3TOOL_PART_IMG_FMT_SPARSE:
            {
                FB_DBG("buf %p, sz %d\n", downloadInf->dataBuf, thisTransferLen);
                ret = v3tool_simg2img_write_img(downloadInf, &_imgTransferInfo.imgTransPara.download);
            } break;
        case V3TOOL_PART_IMG_FMT_UBIFS:
            break;
        default:
            FBS_ERR(fb_response_str, "err media type %d for sp img", mediaType);
            break;
    }
    if ( !ret )optimus_update_progress(thisTransferLen);

    return ret;
}

int v3tool_buffman_data_complete_upload(const UsbUpInf* uploadInf)
{
    //for next download
    _rawImgFileOffset += uploadInf->dataSize;
    return 0;
}

int v3tool_buffman_next_upload_info(UsbUpInf** uploadInfo)
{
    int ret = 0;
    *uploadInfo = NULL;
    if (!_imgTransferInfo.inited || _imgTransferInfo.isDownload) {
        FB_ERR("buffman %d, %d\n", _imgTransferInfo.inited ,_imgTransferInfo.isDownload);
        return -__LINE__;
    }
    ImgUploadPara* upInf = &_imgTransferInfo.imgTransPara.upload;
    ImgCommonPara* cmnInf = &upInf->commonInf;
    char* partName = (char*)cmnInf->partName;

    int64_t leftLen        = cmnInf->imgSzTotal - _rawImgFileOffset;
    if ( !leftLen ) {
        _usbUpInf.dataSize = 0;
        *uploadInfo = &_usbUpInf;
        return 0;
    }

    _usbUpInf.dataSize   = _mymin(leftLen, _RAW_IMG_TRANSFER_LEN);
    _usbUpInf.dataBuf    = (char*)V3_DOWNLOAD_MEM_BASE;
    u8* dataBuf = (u8*)_usbUpInf.dataBuf;
    unsigned dataSize = _usbUpInf.dataSize;
    switch (cmnInf->mediaType) {
        case V3TOOL_MEDIA_TYPE_MEM:
            {
                _usbUpInf.dataBuf = (char*)cmnInf->partStartOff + _rawImgFileOffset;
            }break;
        case V3TOOL_MEDIA_TYPE_STORE:
            {
                if ( !strcmp("bootloader", partName) || !strcmp("_aml_dtb", partName)) {
                    dataSize = _usbUpInf.dataSize = leftLen;
                }
                if (!strcmp("bootloader", partName)) {
                    ret = bootloader_read(dataBuf, 0, dataSize);
                } else if (!strcmp("_aml_dtb", partName)) {
                    //'2' means using 'store dtb iread' rather than 'read'
                    ret = store_dtb_rw(dataBuf, dataSize, 2);
                }
                else  {
                    ret = store_logic_read(partName, _rawImgFileOffset, dataSize, dataBuf);
                }
                if (ret) {
                    FB_ERR("Fail in read store at offset %llx\n", _rawImgFileOffset);
                    return -__LINE__;
                }
            }break;
        case V3TOOL_MEDIA_TYPE_UNIFYKEY:
            {
                _usbUpInf.dataSize   = leftLen;
                unsigned keySz = 0;
                ret = v2_key_read(partName, (u8*)_usbUpInf.dataBuf, leftLen, _ACK, &keySz);
                if (ret || keySz != leftLen) {
                    FB_ERR("Fail in key read,ret %d\n", ret);
                    return -__LINE__;
                }
            }break;
        default:
            FB_ERR("unsupported media %d\n", cmnInf->mediaType);
            break;
    }

    FB_DBG("dataBuf %p, dataSize %x\n", _usbUpInf.dataBuf, _usbUpInf.dataSize);

    *uploadInfo = &_usbUpInf;
    return 0;
}

