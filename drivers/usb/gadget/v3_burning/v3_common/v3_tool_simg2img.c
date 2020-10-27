/*
 * \file        v3_tool_simg2img.c
 * \brief
 *
 * \version     1.0.0
 * \date        2018/04/20
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2018 Amlogic. All Rights Reserved.
 *
 */
#include "../include/v3_tool_def.h"
#include <sparse_format.h>
extern char* fb_response_str;
#define CHUNK_HEAD_SIZE sizeof(chunk_header_t)
#define  FILE_HEAD_SIZE sizeof(sparse_header_t)
#define  SPARSE_HEADER_MAJOR_VER 1
#define sperr(fmt...)   FBS_ERR(fb_response_str, fmt)
#define spmsg   FB_MSG

//states for a sparse packet, initialized when sparse packet probed
static struct
{
    unsigned        leftChunkNum;//chunks that not parsed yet
    unsigned        chunksBufLen;//>=OPTIMUS_DOWNLOAD_SPARSE_TRANSFER_SZ
    unsigned        sparseBlkSz;//block size of sparse format packet
    int             pktHeadLen;
    char*           chunkDataBuf;//buf for usb download

    //back up infomation for verify
    chunk_header_t* chunkInfoBackAddr;//file header and chunk info back address
    uint32_t        backChunkNum;      //chunk number backed
    int64_t         leftTransLen;
    int64_t         nextFlashAddr;
    int64_t         fileOffset; //fileOffset for usb download
    int64_t         imgTotalLen; //fileOffset for usb download
    chunk_header_t*  chunkInfo;

}_spPacketStates;
static int _downloadState = 0;
enum {
    SP_DOWNLOAD_START  = 0XF1,
    SP_DOWNLOAD_OK           ,
    SP_VERIFY_ING            ,
};

static int _simg_probe(const char* source, const u32 length)
{
    sparse_header_t *header = (sparse_header_t*) source;

    if (length < FILE_HEAD_SIZE) {
        sperr("length %d < sparse_header_t len %d\n", length, (int)FILE_HEAD_SIZE);
        return 0;
    }
    if (header->magic != SPARSE_HEADER_MAGIC) {
        spmsg("sparse bad magic, expect 0x%x but 0x%x\n", SPARSE_HEADER_MAGIC, header->magic);
        return 0;
    }

    if(!(SPARSE_HEADER_MAJOR_VER == header->major_version
                && FILE_HEAD_SIZE == header->file_hdr_sz
                && CHUNK_HEAD_SIZE == header->chunk_hdr_sz))
    {
        sperr("want 0x [%x, %x, %x], but [%x, %x, %x]\n",
                SPARSE_HEADER_MAJOR_VER,    (unsigned)FILE_HEAD_SIZE,             (unsigned)CHUNK_HEAD_SIZE,
                header->major_version,      header->file_hdr_sz,        header->chunk_hdr_sz);
        return 0;
    }


    return 1;
}

int v3tool_simg2img_init(const ImgDownloadPara* downPara)
{
    if ( V3TOOL_PART_IMG_FMT_SPARSE != downPara->imgFmt) {
        FBS_ERR(fb_response_str, "err img fmt %d for sparse", downPara->imgFmt);
        return -__LINE__;
    }
    memset(&_spPacketStates, 0, sizeof(_spPacketStates));
    _spPacketStates.chunkDataBuf = (char*)V3_DOWNLOAD_MEM_BASE;
    if ( downPara->needVerify )
        _spPacketStates.chunkInfoBackAddr = (chunk_header_t*)V3_DOWNLOAD_VERIFY_INFO;
    else FB_MSG("don't save inf for verify!\n");
    _spPacketStates.imgTotalLen = downPara->commonInf.imgSzTotal;
    _downloadState = SP_DOWNLOAD_START;

    return 0;
}

/*v3tool_simg2img_get_img: decide sparse img offset/size for download
 * if download info empty <==> first time download:
 *  then getSz = get header and one trunk info
 * if download info @leftTransLen == 0 <==> download ended
 * else leftSz =(chunk data len + chunkinfo)
 *      if leftSz > V3_DOWNLOAD_MEM_SIZE then getSz = V3_DOWNLOAD_MEM_SIZE
 *          if leftSz <= getSz + V3_DOWNLOAD_SPARE_SZ then getSz = leftSz
 *      else getSz = leftSz
 * *******************************
 * need support called more than once when usb-->ddr transfer addsum failed,
 * so don't update global var in get_img,
 * so update @leftTransLen in v3tool_simg2img_write_img, which is after usb->ddr check ok
 * */
/*
 * code flow: first time, then {sz = header + first chunkinfo, offset = 0}
 * 		   else {
 * 		   		offset = _spPacketStates.fileOffset;
 *				sz = V3_DOWNLOAD_MEM_SIZE ;
 *				if (sz + spare >= img left sz) then sz = img left sz
 *				??write img only dispose DATA + chunkinfo, so maybe 8M > this, but not matter??
 * 		   }
 */
int v3tool_simg2img_get_img(UsbDownInf* downInf)
{
    downInf->dataBuf  = (char*)V3_DOWNLOAD_MEM_BASE;
    downInf->fileOffset = _spPacketStates.fileOffset;
    int*    dataSize    = &(downInf->dataSize);
    //Following update downInf->dataSize

    //spmsg("L%d:leftTransLen 0x%llx\n", __LINE__, _spPacketStates.leftTransLen);
    //update leftTransLen if consumered by usb
    if (_spPacketStates.leftTransLen <= 0)
    {
        const int firstTime = !_spPacketStates.sparseBlkSz && !_spPacketStates.pktHeadLen;
        if ( firstTime ) {
            *dataSize   = CHUNK_HEAD_SIZE + FILE_HEAD_SIZE;
            return 0;
        }

        if (_spPacketStates.imgTotalLen == _spPacketStates.fileOffset) {
            *dataSize = 0;
            _downloadState = SP_DOWNLOAD_OK;
            spmsg("Finish sparse img\n");
            return 0;
        }
        FBS_ERR(fb_response_str, "Excep:NO leftTransLen but img not end, fileOffset %llx, leftChunkNum %d",
                _spPacketStates.fileOffset, _spPacketStates.leftChunkNum);
        return -__LINE__;
    }

    int thisLen = (_spPacketStates.leftTransLen > V3_DOWNLOAD_MEM_SIZE)
        ? V3_DOWNLOAD_MEM_SIZE : _spPacketStates.leftTransLen;
    //get all leftdata if left sz small than V3_DOWNLOAD_SPARE_SZ
    if (_spPacketStates.leftTransLen <= thisLen + V3_DOWNLOAD_SPARE_SZ) {
        thisLen = _spPacketStates.leftTransLen;
    }
    if (_spPacketStates.fileOffset + thisLen > _spPacketStates.imgTotalLen) {
        sperr("Exception, fileOffset 0x%llx + thisLen %x > total %llx\n",
                _spPacketStates.fileOffset , thisLen , _spPacketStates.imgTotalLen);
        return -__LINE__;
    }

    *dataSize = thisLen;
    return 0;
}

static int check_chunk_info(const chunk_header_t* chunk, unsigned* pDataLen, unsigned* pFlashAddr, unsigned* fillVal)
{
    unsigned chunkDataLen    = 0;
    unsigned nextFlashAddr   = 0;//update flash addr for next write

    nextFlashAddr = chunkDataLen = chunk->chunk_sz * _spPacketStates.sparseBlkSz;
    switch (chunk->chunk_type)
    {
        case CHUNK_TYPE_RAW:
            {
                if (CHUNK_HEAD_SIZE + chunkDataLen != chunk->total_sz) {
                    sperr("sparse: bad chunk size: head 0x%x + data 0x%x != total 0x%x\n",
                            (unsigned)CHUNK_HEAD_SIZE, chunkDataLen, chunk->total_sz);
                    return -__LINE__;
                }
            }break;
        case CHUNK_TYPE_DONT_CARE:
            {
                if (CHUNK_HEAD_SIZE != chunk->total_sz) {
                    sperr("bogus DONT CARE chunk\n");
                    return -__LINE__;
                }
                chunkDataLen = 0;
            }break;
        case CHUNK_TYPE_FILL:
            {
                if (CHUNK_HEAD_SIZE + 4 != chunk->total_sz) {
                    sperr("error FILL chunk\n");
                    return -__LINE__;
                }
                const int fillFieldLen = 4;
                if (fillVal) *fillVal = *(unsigned*)(chunk+1);
                chunkDataLen = fillFieldLen;
            }break;
        case CHUNK_TYPE_CRC32:
            sperr("CHUNK_TYPE_CRC32 unsupported yet!\n");
            return -__LINE__;
        default:
            sperr("unknown chunk ID 0x%x at %p, leftid %d\n", chunk->chunk_type, chunk,
                    _spPacketStates.leftChunkNum);
            return -__LINE__;
    }

    if (pDataLen) *pDataLen = chunkDataLen;
    if (pFlashAddr) *pFlashAddr = nextFlashAddr;
    return 0;
}

static int simg2img_fill_chunk_write(const char* partName, int fillLen, const unsigned* pFillVal, int64_t flashAddr)
{
    const int BufSz = 256*1024;//heard from LarsonJ, 128K is max burst in emmc control read/write
    u8* fillBuf = (u8*)pFillVal + sizeof(unsigned) + CHUNK_HEAD_SIZE;//fill trunk data format[4bytes body + nextDownInf]
    const unsigned fillVal = *pFillVal;
    int needFill = 1;

    if (v3tool_is_flash_erased()) {
        switch (store_get_type()) {
            case BOOT_EMMC:
            case BOOT_SD:
                needFill = (fillVal != 0);
                break;

            case BOOT_NAND_NFTL:
                needFill = (fillVal != 0XFFFFFFFFU);
                break;

            default: FBS_EXIT(_ACK, "unsupported boot dev %d\n", store_get_type());
        }
    } else {/* always need fill when flash NOT erased */}
   //for, emmc, if fillVal is 0, then needFill = false if "disk_inital > 0"
    if (!needFill) return 0;

    int LeftDataLen = fillLen;
    int i = 0;
    const int szFill = min(BufSz, fillLen);
    unsigned* tmpFill = (unsigned*)fillBuf;
    //spmsg("Fill start 0x%llx by value 0x%x\n", flashAddr, fillVal);
    for (; i < szFill; i += 4) {
        *tmpFill++ = fillVal;
    }
    while (LeftDataLen >0) {
        const unsigned thisWriteLen = min(LeftDataLen, BufSz);
        FB_DBG("fill off/sz/val 0x%08llx %x %x\n", flashAddr, thisWriteLen, fillVal);
        int ret = store_logic_write(partName, flashAddr, thisWriteLen, fillBuf);
        if (ret) {
            sperr("FILL_CHUNK:Want write 0x%x Bytes, but failed\n", thisWriteLen);
            return -__LINE__;
        }

        LeftDataLen -= thisWriteLen;
        flashAddr   += thisWriteLen;
    }

    return 0;
}


//v3tool_simg2img_write_img called after transfer (and checksum) ok
//Recevied data format: [file header] + [chunk info] or [chunk body >=0] + [next chunk info]
//                  <==> <[file header]> + <[chunkBody]> + <[nextChunkInfo]>
//                      //has [file header] if first download
//                      //[chunkBody] size 0 if first download or CHUNK_TYPE_DONT_CARE
//                      //has [nextChunkInfo] except that
//                          last chunk or [chunkBody] > (V3_DOWNLOAD_MEM_SIZE + V3_DOWNLOAD_SPARE_SZ)
//                  code part 1: check if [file header]
//                  code part 2: dispose chunk body
//                  code part 3: dispose next chunk info for next download
//update global var: _spPacketStates.[fileOffset, leftTransLen, leftChunkNum]
//_spPacketStates.chunkInfo: chunk info for rx chunk body
//              nextDownInf: chunk info for next download
int v3tool_simg2img_write_img(const UsbDownInf* downInf, const ImgDownloadPara* downPara)
{
    char* dataBuf = downInf->dataBuf;
    int dataSize  = downInf->dataSize;
    const chunk_header_t* nextDownInf = NULL;
    bool hasFileHeader      = false;
    bool hasChunkBody       = true;
    bool hasNextChunkInfo   = true;//nextChunkInfo

    //update globals not affected by rx data format
    _spPacketStates.fileOffset   += dataSize;
    _spPacketStates.leftTransLen -= dataSize;

    hasFileHeader = !_spPacketStates.sparseBlkSz && !_spPacketStates.pktHeadLen;
    if (hasFileHeader) //code part 1
    {
        hasChunkBody     = false;
        hasNextChunkInfo = true;
        if ( !_simg_probe(dataBuf, dataSize) )
            FBS_EXIT(_ACK, "err sparse img header");

        if (dataSize != FILE_HEAD_SIZE + CHUNK_HEAD_SIZE) {
            sperr("Excep:first download sz need %d, but rx %d\n", (int)(FILE_HEAD_SIZE + CHUNK_HEAD_SIZE), dataSize);
            return -__LINE__;
        }

        //2,
        sparse_header_t *header = (sparse_header_t*) dataBuf;
        _spPacketStates.leftChunkNum    = header->total_chunks - 1;//how many chunkinfo not parsed
        _spPacketStates.pktHeadLen      = header->file_hdr_sz;
        _spPacketStates.sparseBlkSz     = header->blk_sz;//often 4k
        spmsg("totalChunkNum %d, fileHeadSz 0x%x, chunkHeadSz 0x%zx, blk 0x%x\n", _spPacketStates.leftChunkNum + 1,
                _spPacketStates.pktHeadLen, CHUNK_HEAD_SIZE, _spPacketStates.sparseBlkSz);

        //3,
        memcpy(_spPacketStates.chunkInfoBackAddr, header, FILE_HEAD_SIZE);
        _spPacketStates.chunkInfoBackAddr =
            (chunk_header_t*)((sparse_header_t*)(_spPacketStates.chunkInfoBackAddr) + 1);
        dataBuf  += FILE_HEAD_SIZE;//to make same as other rx data format
        dataSize -= FILE_HEAD_SIZE;
    }

    const bool lastChunk    = !_spPacketStates.leftChunkNum;
    if (!hasFileHeader)
    {
        if (lastChunk) {//img ended
            hasNextChunkInfo = false;
            spmsg("last chunk\n");
            /*return 0;*/
        }
        if ( _spPacketStates.leftTransLen ) {//only data body
            hasNextChunkInfo = false;
            //spmsg("big chunk left 0x%llx\n", _spPacketStates.leftTransLen);
        } else {
            _spPacketStates.leftChunkNum -= 1;
        }
    }

    if (hasNextChunkInfo) dataSize   -= CHUNK_HEAD_SIZE;
    if (hasChunkBody)
    {
        const chunk_header_t* pChunk = _spPacketStates.chunkInfo;//this download info
        const char* part = downPara->commonInf.partName;
        int64_t flashAddr = _spPacketStates.nextFlashAddr + downPara->commonInf.partStartOff;
        unsigned flashWrLen = dataSize;
        const unsigned chunkFlashSpace = pChunk->chunk_sz * _spPacketStates.sparseBlkSz;;

        //spmsg("chunkInfo %p, %x\n", pChunk, pChunk->chunk_type);
        const unsigned chunkType = pChunk->chunk_type;
        if (CHUNK_TYPE_RAW == chunkType) {
            FB_DBG("raw wr: off/sz 0x%08llx flashWrLen %x\n", flashAddr, flashWrLen);
            if (store_logic_write(part, flashAddr, flashWrLen, dataBuf)) {
                sperr("Fail in flash raw trunk\n");
                return -__LINE__;
            }
            _spPacketStates.nextFlashAddr += dataSize;
        } else if (CHUNK_TYPE_FILL == chunkType) {
            if ( 4 != dataSize ) {
                sperr("fill chunk but dataSize(%d) != 4\n", dataSize);
                return -__LINE__;
            }
            const unsigned* fillVal = (unsigned*)dataBuf;
            FB_DBG("fill wr: off/sz 0x%08llx flashWrLen %x\n", _spPacketStates.nextFlashAddr, chunkFlashSpace);
            if (simg2img_fill_chunk_write((char*)part, chunkFlashSpace, fillVal, flashAddr)) {
                sperr("Fail in fill fill-chunk\n");
                return -__LINE__;
            }
            _spPacketStates.nextFlashAddr += chunkFlashSpace;
        } else if (CHUNK_TYPE_DONT_CARE == chunkType) {
            if (dataSize) {
                sperr("DONT_CARE trunk sz %x err, should be 0\n", dataSize);
                return -__LINE__;
            }
            FB_DBG("donnot care: off/sz 0x%08llx flashWrLen", _spPacketStates.nextFlashAddr, chunkFlashSpace);
            _spPacketStates.nextFlashAddr += chunkFlashSpace;
        } else {
            sperr("chunk(%x) should not be here", chunkType);
            return -__LINE__;
        }
    }

    //just update for next download, not doing flash writing even don't care trunk
    if (hasNextChunkInfo)
    {
        nextDownInf = (chunk_header_t*)(dataBuf + dataSize);
        _spPacketStates.chunkInfo = _spPacketStates.chunkInfoBackAddr;
        memcpy(_spPacketStates.chunkInfoBackAddr++, nextDownInf, CHUNK_HEAD_SIZE);
        //spmsg("back chunkInfo 0x%p, next %p, %x\n", _spPacketStates.chunkInfo, nextDownInf, _spPacketStates.chunkInfo->chunk_type);

        unsigned chunkDataLen = 0;
        if (check_chunk_info(nextDownInf, &chunkDataLen, NULL, NULL)) {
            sperr("Fail parse next chunk header\n");
            return -__LINE__;
        }
        _spPacketStates.leftTransLen = chunkDataLen;
        if (_spPacketStates.fileOffset + chunkDataLen < _spPacketStates.imgTotalLen) {
            _spPacketStates.leftTransLen += CHUNK_HEAD_SIZE;
        }
        //spmsg("update leftTransLen 0x%llx\n", _spPacketStates.leftTransLen);
    }

    return 0;
}

int v3tool_simg2img_verify_img(sha1_context* ctx, const char* partName, int64_t partBase)
{
    const sparse_header_t *header = (sparse_header_t*)V3_DOWNLOAD_VERIFY_INFO;
    const int nChunk = header->total_chunks;
    const unsigned vryBuffLen  = (1<<20);//small buffer to reply while verify long
    unsigned char* dataBuf = (unsigned char*)V3_DOWNLOAD_MEM_BASE;
    unsigned chunkDataLen = 0;
    unsigned long timePeriod = 0;
    const chunk_header_t* backInf = (chunk_header_t*)(header + 1);
    int ret = -__LINE__;

    static int _iChunk = 0;
    static int64_t _partOffset = 0;
    static unsigned long _lastReportTick = 0;
    static unsigned _dataChunkLeft = 0;//initial value or chunk verified end

    if (SP_DOWNLOAD_OK == _downloadState) {//first time
        _iChunk = 0;
        _partOffset = partBase;
        _dataChunkLeft = 0;
        _downloadState = SP_VERIFY_ING;
        if ( !_simg_probe((char*)header, FILE_HEAD_SIZE) ) {
            sperr("err backup header\n");
            return -__LINE__;
        }
        sha1_update(ctx, (u8*)header, FILE_HEAD_SIZE);
        _lastReportTick = get_timer(0);
        v3tool_media_set_free(NULL);
        FB_DBG("partBase 0x%llx\n", partBase);
    } else {
        backInf += _iChunk;
    }
    for ( ; _iChunk < nChunk; ++_iChunk, ++backInf )
    {
        if (!_dataChunkLeft) sha1_update(ctx, (u8*)backInf, CHUNK_HEAD_SIZE);
        const int64_t flashSpace = backInf->chunk_sz * _spPacketStates.sparseBlkSz;
        switch (backInf->chunk_type)
        {
            case CHUNK_TYPE_RAW:
                {
                    chunkDataLen = flashSpace;
                    if (CHUNK_HEAD_SIZE + chunkDataLen != backInf->total_sz) {
                        sperr("sparse: bad chunk size: head 0x%x + data 0x%x != total 0x%x\n",
                                (unsigned)CHUNK_HEAD_SIZE, chunkDataLen, backInf->total_sz);
                        goto _verify_end;
                    }
                }break;
            case CHUNK_TYPE_DONT_CARE:
                {
                    if (CHUNK_HEAD_SIZE != backInf->total_sz) {
                        sperr("bogus DONT CARE chunk\n");
                        goto _verify_end;
                    }
                    chunkDataLen = 0;
                }break;
            case CHUNK_TYPE_FILL:
                {
                    if (CHUNK_HEAD_SIZE + 4 != backInf->total_sz) {
                        sperr("error FILL chunk\n");
                        goto _verify_end;
                    }
                    //spmsg("fill chunk start 0x%llx\n", _partOffset);
                    const int fillFieldLen = 4;
                    chunkDataLen = fillFieldLen;
                }break;
            case CHUNK_TYPE_CRC32:
                sperr("CHUNK_TYPE_CRC32 unsupported yet!\n");
                goto _verify_end;
            default:
                sperr("unknown chunk ID 0x%x at %p, leftid %d\n", backInf->chunk_type, backInf, _iChunk);
                goto _verify_end;
        }
        if (!_dataChunkLeft)_dataChunkLeft = chunkDataLen;
        int64_t flashAddr = _partOffset + chunkDataLen - _dataChunkLeft;
        while ( _dataChunkLeft ) {
            const int thisLen = min(vryBuffLen, _dataChunkLeft);
            FB_DBG("rd chunk %x: off/sz 0x%08llx %x\n", backInf->chunk_type, flashAddr, thisLen);
            if (store_logic_read(partName, flashAddr, thisLen, dataBuf)) {
                sperr("Fail in read storage for verify\n");
                goto _verify_end;
            }
            sha1_update(ctx, dataBuf, thisLen);

            _dataChunkLeft     -= thisLen;
            flashAddr   += thisLen;

            if (_dataChunkLeft) {
                timePeriod = get_timer(_lastReportTick);
                if (timePeriod >= 2*1000) {//2Sec
                    _lastReportTick += timePeriod;
                    FB_DBG("cost %lu >= 2s\n", timePeriod);
                    FB_DBG("xx[%d]flashAddr 0x%llx, leftLen %x, thisLen %x\n",
                            _iChunk, flashAddr, _dataChunkLeft, thisLen);
                    char info[64];
                    sprintf(info, "%04d / %04d Chunks", _iChunk + 1, nChunk);
                    v3tool_media_set_busy(info);
                    return 0;
                }
            }
        }
        _partOffset += flashSpace;
    }

    ret = 0;
_verify_end:
    v3tool_media_set_free(NULL);
    _iChunk = 0;
    spmsg("verify end\n");
    return ret;
}

