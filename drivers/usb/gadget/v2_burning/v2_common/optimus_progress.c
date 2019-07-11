/*
 * \file        optimus_report_progress.c
 * \brief       display and print progress info when burning a partition
 *
 * \version     1.0.0
 * \date        2013/6/23
 * \author      Sam.Wu <wuehui@allwinnertech.com>
 *
 * Copyright (c) 2013 Allwinner Technology. All Rights Reserved.
 *
 */
#include "../v2_burning_i.h"

#define OPTIMUS_PROMPT_SIZE_MIN     (4U<<20)//mininal size to prompt burning progress step

struct ProgressInfo{
    u32     itemSzLow;
    u32     itemSzHigh;

    u8      startStep;
    u8      endStep;
    u8      totalStepNum;
    u8      currentStep;

    u32     bytesToIncOneStep;
    u32     unReportSzInByte;

    u32     bytesToUpdateStep;
    u32     reserv;

};
static struct ProgressInfo _progressInfo = {0};

int optimus_progress_init(const unsigned itemSzHigh, const unsigned itemSzLow, const u32 startStep, const u32 endStep)
{
    _progressInfo.itemSzLow     = itemSzLow;
    _progressInfo.itemSzHigh    = itemSzHigh;

    _progressInfo.startStep     = startStep;
    _progressInfo.endStep       = endStep;
    _progressInfo.currentStep   = startStep;
    _progressInfo.totalStepNum  = endStep - startStep;

    _progressInfo.unReportSzInByte  = 0;//clear it

    //ATTENTION: as divisor / is lossy, _progressInfo.bytesToIncOneStep * _progressInfo.totalStepNum <= item size, so 100% is sometimes not exactly Burn Completed!!
    _progressInfo.bytesToIncOneStep = ((((u64)itemSzHigh)<<32) + itemSzLow) / _progressInfo.totalStepNum;
    _progressInfo.bytesToUpdateStep = (OPTIMUS_PROMPT_SIZE_MIN > _progressInfo.bytesToIncOneStep) ? OPTIMUS_PROMPT_SIZE_MIN : _progressInfo.bytesToIncOneStep;


    DWN_DBG("item size 0x[%x, %x], currentStep %d, totalStepNum %d, bytesToIncOneStep 0x%x\n",
            itemSzHigh, itemSzLow, _progressInfo.currentStep, _progressInfo.totalStepNum, _progressInfo.bytesToIncOneStep);
    return 0;
}

int optimus_progress_exit(void)
{
    return 0;
}

int optimus_update_progress(const unsigned thisBurnSz)
{
    _progressInfo.unReportSzInByte += thisBurnSz;

    if (_progressInfo.bytesToUpdateStep > _progressInfo.unReportSzInByte) {
        return 0;
    }

    //if it is first time to prompt UI progress steps
    if (_progressInfo.unReportSzInByte == thisBurnSz && _progressInfo.startStep + 1 == _progressInfo.currentStep)
    {
        printf("\n");
    }

    _progressInfo.currentStep       += _progressInfo.unReportSzInByte / _progressInfo.bytesToIncOneStep;
    _progressInfo.unReportSzInByte  %= _progressInfo.bytesToIncOneStep;

    printf("Downloading %%%d\r", _progressInfo.currentStep);
    if (_progressInfo.currentStep == _progressInfo.endStep) {
        printf("\n");
    }

    return 0;
}

//outStr will be null-terminater after format
int optimus_hex_data_2_ascii_str(const unsigned char* hexData, const unsigned nBytes, char* outStr, const unsigned strSz)
{
    int i = 1;
    if (strSz < 2 * nBytes + 1) {
        DWN_ERR("strSz(%d) > 2 * nBytes(%d)\n", strSz, nBytes);
        return __LINE__;
    }

    sprintf(outStr, "%02x", hexData[0]);
    for (; i < nBytes; ++i)
    {
        sprintf(outStr, "%s%02x", outStr, hexData[i]);
    }

    return 0;
}

unsigned add_sum(const void* pBuf, const unsigned size)
{
    unsigned sum		 =	0;
    const unsigned* data = (const unsigned*)pBuf;
    unsigned wordLen 	 = size>>2;
    unsigned rest 		 = size & 3;

    for (; wordLen/4; wordLen -= 4)
    {
        sum += *data++;
        sum += *data++;
        sum += *data++;
        sum += *data++;
    }
    while (wordLen--)
    {
        sum += *data++;
    }

    if (rest == 0)
    {
        ;
    }
    else if(rest == 1)
    {
        sum += (*data) & 0xff;
    }
    else if(rest == 2)
    {
        sum += (*data) & 0xffff;
    }
    else if(rest == 3)
    {
        sum += (*data) & 0xffffff;
    }

    return sum;
}

#ifndef SCPI_CMD_USB_BOOT
#define SCPI_CMD_USB_BOOT 		0xB0	//skip to wait pc with timeout
#define SCPI_CMD_USB_UNBOOT 	0xB1	//skip to wait pc forever
#define SCPI_CMD_SDCARD_BOOT 	0xB2
#define SCPI_CMD_CLEAR_BOOT 	0xB3
static void _erase_bootloader(uint64_t arg0)
{
    if (SCPI_CMD_CLEAR_BOOT == arg0) return;//dummy as not supported

    const char* bootName = "bootloader";
    const int bootCpyNum = store_boot_copy_num(bootName);

    printf("arg0[0x%llx]\n", arg0);
    printf("set_boot_first_timeout not defined so Really erase\n");
    int iCopy = 0;
    for (; iCopy < bootCpyNum; ++iCopy) {
        store_boot_erase(bootName, iCopy);
    }
}
extern void set_boot_first_timeout(uint64_t arg0) __attribute__((weak, alias("_erase_bootloader")));

#include <asm/arch/bl31_apis.h>
#include <amlogic/cpu_id.h>
#endif//#ifndef SCPI_CMD_USB_BOOT
//I assume that store_inited yet when "bootloader_is_old"!!!!
int optimus_erase_bootloader(const char* extBootDev)
{
    if (!strcmp("usb", extBootDev))
    {
        set_boot_first_timeout(SCPI_CMD_USB_UNBOOT);
        return 0;
    }

    if (!strcmp("sdc", extBootDev))
    {
        cpu_id_t cpuid = get_cpu_id();
        if (MESON_CPU_MAJOR_ID_C1 == cpuid.family_id && MESON_CPU_CHIP_REVISION_A == cpuid.chip_rev)
            _erase_bootloader(SCPI_CMD_SDCARD_BOOT);
        else 
            set_boot_first_timeout(SCPI_CMD_SDCARD_BOOT);
        return 0;
    }

    return 0;
}

void optimus_clear_ovd_register(void)
{
    set_boot_first_timeout(SCPI_CMD_CLEAR_BOOT);
}

