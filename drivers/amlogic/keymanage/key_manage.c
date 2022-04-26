// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include "key_manage_i.h"
#include <u-boot/sha1.h>
#include <linux/ctype.h>
#if defined(CONFIG_AML_SECURITY_KEY)
extern int _burn_key_in_type_hdcp2(const char* keyname,
        void* databuf, const unsigned bufLen, char* decryptBuf);
extern int _read_key_in_type_hdcp2(const char* keyname,
        void* databuf, const unsigned bufLen, char* decryptBuf);
extern int _km_hdcp2_size(void);
#else
static int _burn_key_in_type_hdcp2(const char* keyname,
        void* databuf, const unsigned bufLen, char* decryptBuf)
{
    KM_ERR("CONFIG_AML_SECURITY_KEY undefined\n");
    return -__LINE__;
}
static int _read_key_in_type_hdcp2(const char* keyname,
        void* databuf, const unsigned bufLen, char* decryptBuf)
{
    KM_ERR("CONFIG_AML_SECURITY_KEY undefined\n");
    return -__LINE__;
}
int _km_hdcp2_size(void)
{
    KM_ERR("CONFIG_AML_SECURITY_KEY undefined\n");
    return 0;
}
#endif// #if defined(CONFIG_AML_SECURITY_KEY)

enum _KmUsrKeyType{
    KM_USER_KEY_TYPE_MAC        = 0 ,//key format is all ascii, and splitted by :
    KM_USER_KEY_TYPE_SHA1           ,//key format is end with 20 bytes sha1sum
    KM_USER_KEY_TYPE_HDCP2          ,//special case, can only identified with name
    KM_USER_KEY_TYPE_RAW            ,//raw format which can burn to target directly
};

static const char* _cfgKeyTypes[] = {
    [KM_USER_KEY_TYPE_MAC]          = "mac"     ,
    [KM_USER_KEY_TYPE_SHA1]         = "sha1"    ,
    [KM_USER_KEY_TYPE_HDCP2]        = "hdcp2"   ,
    [KM_USER_KEY_TYPE_RAW]          = "raw"     ,
};
#define _SUPPORTED_CFG_TYPES_NUM    ( sizeof(_cfgKeyTypes) / sizeof(char*) )


static int km_get_user_key_format(const char* srcKeyName, int* key_type)
{
    int ret                 = 0;
    int   srcKeyType        = 0;
    const char* cfgType     = NULL;

    cfgType = keymanage_dts_get_key_type (srcKeyName) ;
    if (NULL == cfgType) {
        KM_ERR("Fail in get keytype cfg in dts for key[%s]\n", srcKeyName);
        return __LINE__;
    }

    for (srcKeyType = 0; srcKeyType < _SUPPORTED_CFG_TYPES_NUM; ++srcKeyType)
    {
        ret = strcmp(cfgType, _cfgKeyTypes[srcKeyType]);
        if (!ret) break;
    }
    if (srcKeyType == _SUPPORTED_CFG_TYPES_NUM) {
        KM_ERR("prop key-type[%s] unsupported in key[%s]\n", cfgType, srcKeyName);
        return __LINE__;
    }

    if (KM_USER_KEY_TYPE_RAW == srcKeyType)
    {
        do
        {
            ret = !strcmp(srcKeyName, "mac") || !strcmp(srcKeyName, "mac_bt") || !strcmp(srcKeyName, "mac_wifi");
            if (ret) { srcKeyType = KM_USER_KEY_TYPE_MAC; break; }

            /*ret = !strcmp(srcKeyName, "hdcp") ;*/
            /*if (ret) { srcKeyType = KM_USER_KEY_TYPE_SHA1; break; }*/

            ret = !strcmp(srcKeyName, "hdcp2") ;
            if (ret) { srcKeyType = KM_USER_KEY_TYPE_HDCP2; break; }

        }while(0);
    }

    *key_type = srcKeyType;
    return 0;
}

#if 1//START MAC
const int _UsrMacKeyLen = 17;

//key manager user interface: mac format check and change format if needed
static int _burn_key_in_type_mac(const char* keyname, const char* srcKeyVal, const unsigned srcKeyLen, void* decryptBuf)
{
    int ret = 0;
    int index = 0;
    ssize_t     targetKeyLen    = 0;
    char*       dstKeyVal       = (char*)decryptBuf;

    if (_UsrMacKeyLen != srcKeyLen) {
        KM_ERR("mac len %d is invalid, must be %d\n", srcKeyLen, _UsrMacKeyLen);
        return -EINVAL;
    }

    for (index = 0; index < _UsrMacKeyLen; index += 3)
    {
        int k = 0;
        const char* p = srcKeyVal + index;
        for (k = 0; k < 2; ++k) {
            const char c = *p++;
            if (!isxdigit(c)) {
                KM_ERR("mac str(%s) fmt err at index[%d]\n", srcKeyVal, index + k);
                return __LINE__;
            }
        }
        if (':' != *p && index + k < _UsrMacKeyLen) {
            KM_ERR("mac str(%s) fmt err at index[%d], must be :, but %c\n", srcKeyVal, index + 2, *p);
            return __LINE__;
        }
    }

    enum key_manager_dev_e keyDev = keymanage_dts_get_key_device(keyname);
    if (KEY_M_MAX_DEV == keyDev) {
        KM_ERR("Fail get key dev for key[%s]\n", keyname);
        return __LINE__;
    }
    if (KEY_M_EFUSE_NORMAL != keyDev) { targetKeyLen = _UsrMacKeyLen; }
    else
    {//efusekey, check configure size
        ret = key_unify_query_size(keyname, &targetKeyLen);
        if (ret) {
            KM_ERR("Fail at get key size, ret=%d\n", ret);
            return __LINE__;
        }

        if (6 != targetKeyLen && targetKeyLen != _UsrMacKeyLen) {
            KM_ERR("efuse key[%s] len %zd err\n", keyname, targetKeyLen);
            return __LINE__;
        }
    }

    if (_UsrMacKeyLen == targetKeyLen) {//say its target not efuse ?
        ret =  key_unify_write(keyname, srcKeyVal, srcKeyLen);
        return ret;
    }

    KM_DBG("targetKeyLen=%zd\n", targetKeyLen);
    for (index = 0; index < targetKeyLen; ++index) {
        char theByteStr[4] ;
        theByteStr[0] = srcKeyVal[index * 3 + 0];
        theByteStr[1] = srcKeyVal[index * 3 + 1];
        theByteStr[2] = '\0';
        unsigned byteSum = 0;

        byteSum = simple_strtoul(theByteStr, NULL, 16);
        KM_DBG("byteSum[%d]=0x%x\n", index, byteSum);
        if (byteSum > 0xff) {
            KM_ERR("theByteStr=%s err\n", theByteStr);
            return __LINE__;
        }
        dstKeyVal[index] = byteSum;
    }

    ret = key_unify_write(keyname, dstKeyVal, targetKeyLen);
    return ret;
}

//Return value: key size that user wanted, ok if > 0
static int _read_key_in_type_mac(const char* keyname, char* databuf, const unsigned bufLen, char* decryptBuf)
{
    int ret = 0;
    int index = 0;
    int n = 0;
    ssize_t keyDevSz = 0;

    if (_UsrMacKeyLen > bufLen) {
        KM_ERR("mac len %d is invalid, must be %d\n", bufLen, _UsrMacKeyLen);
        return -__LINE__;
    }

    ret = key_unify_query_size (keyname, &keyDevSz) ;
    if (ret) {
        KM_ERR("Fail in get key sz, ret=%d\n", ret);
        return -__LINE__;
    }

    ret = key_unify_read (keyname, decryptBuf, (unsigned)keyDevSz) ;
    if (ret) {
        KM_ERR("fail in read key[%s]\n", keyname);
        return -__LINE__;
    }

    if (_UsrMacKeyLen == keyDevSz) {
        memcpy(databuf, decryptBuf, keyDevSz);
        return 0;
    }

    n = 0;
    databuf[0] = '\0';
    for (index = 0; index < keyDevSz; ++index)
    {
        const unsigned byteSum = decryptBuf[index];

        //sprintf(databuf, "%s%02x:", databuf, byteSum);
        n += sprintf(&databuf[n], "%02x:", byteSum);
        if (n > bufLen) {
            break;
        }
    }

    return ret;
}
#endif//END MAC

#if 1//Start sha1sum
//key value which end up 20 bytes sha1sum
//check the sha1sum and remove it after checked ok
static int _burn_key_in_type_sha1(const char* keyname, void* databuf, const unsigned bufLen, char* decryptBuf)
{
    int ret = 0;
    const unsigned srcKeyLen    = bufLen;
    const char* srcKeyVal       = (char*)databuf;
    const unsigned shaSumLen = 20;
    const unsigned licLen = srcKeyLen - shaSumLen;
    const u8* orgSum = (u8*)srcKeyVal + licLen;
    u8 genSum[shaSumLen];

    if (srcKeyLen <= 20) {
        KM_ERR("Err key len %d for sha1 fmt\n", srcKeyLen);
        return __LINE__;
    }

    sha1_csum((u8*)srcKeyVal, licLen, genSum);

    ret = memcmp(orgSum, genSum, shaSumLen);
    if (ret) {
        const unsigned fmtStrLen = shaSumLen * 2 + 2;
        char org_sha1Str[fmtStrLen + 2];
        char gen_sha1Str[fmtStrLen + 2];
        int byteIndex = 0;
        int gen_n,org_n;

        gen_n = org_n = 0;
        org_sha1Str[0] = gen_sha1Str[0] = '\0';
        for (byteIndex = 0; byteIndex < shaSumLen; ++byteIndex)
        {
            //sprintf(org_sha1Str, "%s%02x", org_sha1Str, orgSum[byteIndex]);
            //sprintf(gen_sha1Str, "%s%02x", gen_sha1Str, genSum[byteIndex]);
            org_n += sprintf(&org_sha1Str[org_n], "%02x",  orgSum[byteIndex]);
            gen_n += sprintf(&gen_sha1Str[gen_n], "%02x",  genSum[byteIndex]);
            if ((org_n > fmtStrLen+2) || (gen_n > fmtStrLen+2)) {
                KM_ERR("%s:%d mem is overflow!!!n",__func__,__LINE__);
                break;
            }
        }
        KM_ERR("sha1sum, orgSum[%s] != genSum[%s]\n", org_sha1Str, gen_sha1Str);

        return __LINE__;
    }
    KM_MSG("Verify key with sha1sum OK\n");

    ret = key_unify_write(keyname, srcKeyVal, licLen);
    return ret;
}
#endif//END sha1sum
/* *
 * APIs of key manage
*/

int key_manage_init(const char* seednum, const char* dtbaddr)
{
    int ret = key_unify_init(seednum, dtbaddr);

    return ret;
}

int key_manage_exit(void)
{
    return key_unify_uninit();
}

int key_manage_write(const char* keyname, const void* keydata, const unsigned dataLen)
{
    int ret = 0;
    int srcKeyType;
    char* decryptBuf = NULL;
    const int DecryptBufMaxLen = 64<<10;

    ret = key_unify_query_key_has_configure(keyname);
    if (!ret) {
        KM_ERR ("There isn't cfg for key[%s]\n", keyname) ;
        return __LINE__;
    }

    ret = km_get_user_key_format(keyname, &srcKeyType);
    if (ret) {
        KM_ERR ("Fail in get user key type\n") ;
        return __LINE__;
    }

    decryptBuf = (char*)malloc(DecryptBufMaxLen);
    switch (srcKeyType)
    {
        case KM_USER_KEY_TYPE_MAC:
            {
                ret = _burn_key_in_type_mac(keyname, (char*)keydata, dataLen, decryptBuf);
            }
            break;

        case KM_USER_KEY_TYPE_SHA1:
            {
                ret = _burn_key_in_type_sha1(keyname, (char*)keydata, dataLen, decryptBuf);
            }
            break;

        case KM_USER_KEY_TYPE_HDCP2:
            {
                ret = _burn_key_in_type_hdcp2(keyname, (char*)keydata, dataLen, decryptBuf);
            }
            break;

        case KM_USER_KEY_TYPE_RAW:
            {
                ret = key_unify_write(keyname, (char*)keydata, dataLen);
            }
            break;
    }

    free(decryptBuf);
    return ret;
}

int key_manage_read(const char* keyname, void* keydata, const unsigned bufLen)
{
    int ret = 0;
    ssize_t keysize = 0;
    int srcKeyType = 0;
    char* decryptBuf = NULL;
    const int DecryptBufMaxLen = 64<<10;

    ret = key_manage_query_size(keyname, &keysize);
    if (ret) {
        KM_ERR ("Fail in query key size for key[%s]\n", keyname) ;
        return __LINE__;
    }
    if (keysize > bufLen) {
        KM_ERR ("keysize %zd > bufLen %d\n", keysize, bufLen) ;
        return __LINE__;
    }

    ret = km_get_user_key_format(keyname, &srcKeyType);
    if (ret) {
        KM_ERR ("Fail in get user key type\n") ;
        return __LINE__;
    }

    decryptBuf = (char*)malloc(DecryptBufMaxLen);
    switch (srcKeyType)
    {
        case KM_USER_KEY_TYPE_MAC:
            {
                ret = _read_key_in_type_mac(keyname, (char*)keydata, (unsigned)keysize, decryptBuf);
            }
            break;

        case KM_USER_KEY_TYPE_HDCP2:
            {
                ret = _read_key_in_type_hdcp2(keyname, keydata, (unsigned)keysize, decryptBuf);
            }
            break;

        case KM_USER_KEY_TYPE_SHA1:
        case KM_USER_KEY_TYPE_RAW:
        default:
            {
                ret = key_unify_read(keyname, keydata, (unsigned)keysize);
            }
            break;
    }

    free(decryptBuf);
    return ret;
}

int key_manage_query_size(const char* keyname, ssize_t* keysize)
{
    int ret = 0;
    int exist = 0;
    int srcKeyType = 0;

    ret = key_manage_query_exist(keyname, &exist);
    if (ret) {
        KM_ERR ("Fail in query key exist\n") ;
        return __LINE__;
    }
    if (!exist) {
        KM_ERR ("key[%s] not programed yet\n", keyname) ;
        return __LINE__;
    }

    ret = km_get_user_key_format(keyname, &srcKeyType);
    if (ret) {
        KM_ERR ("Fail in get user key type\n") ;
        return __LINE__;
    }

    switch (srcKeyType)
    {
        case KM_USER_KEY_TYPE_MAC:
            {
                *keysize = _UsrMacKeyLen;
            }
            break;

        case KM_USER_KEY_TYPE_HDCP2:
            {
                *keysize = _km_hdcp2_size();
            }
            break;

        case KM_USER_KEY_TYPE_RAW:
        case KM_USER_KEY_TYPE_SHA1:
        default:
            ret = key_unify_query_size(keyname, keysize);
    }

    return ret;
}

int key_manage_query_exist(const char* keyname, int* exist)
{
    int ret = 0;

    ret = key_unify_query_key_has_configure(keyname);
    if (!ret) {
        KM_ERR ("There isn't dts cfg for key[%s]\n", keyname) ;
        return __LINE__;
    }

    ret = key_unify_query_exist(keyname, exist);

    return ret;
}

int key_manage_query_secure(const char* keyname, int* isSecure)
{
    int ret = 0;
    int exist = 0;

    ret = key_manage_query_exist(keyname, &exist);
    if (ret) {
        KM_ERR ("Fail in query key exist, ret=%d\n", ret) ;
        return __LINE__;
    }
    if (!exist) {
        KM_ERR ("Key[%s] not programed yet\n", keyname) ;
        return __LINE__;
    }

    ret = key_unify_query_secure(keyname, isSecure);

    return ret;
}

int key_manage_query_canOverWrite(const char* keyname, int* canOverWrite)
{
    return key_unify_query_canOverWrite(keyname, canOverWrite);
}

static int do_keyman_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    const char* seedNum = argv[1];
    const char* dtbAddr = argc > 2 ? argv[2] : NULL;

    if (argc < 2) return CMD_RET_USAGE;

    ret = key_manage_init(seedNum, dtbAddr);
    return ret;
}

static int do_keyman_exit(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    return key_manage_exit();
}

//read keyname addr <fmt>
//fmt can be hex/str, if str, env keyname will be setted
static int do_keyman_read(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    const char* keyname = argv[1];
    char* dataBuf = NULL;
    const char* dataFmt = argc > 3 ? argv[3] : NULL;
    ssize_t keyLen = 0;
    if (argc < 3) return CMD_RET_USAGE;

    dataBuf = (char*)simple_strtoul(argv[2], NULL, 16);
    if (!dataBuf) {
        KM_ERR("Fail in parse argv[2] to dataBuf\n");
        return __LINE__;
    }

    ret = key_manage_query_size(keyname, &keyLen);
    if (ret) {
        KM_DBG("Fail get sz for[%s]\n", keyname);//here occure in booting if key not burned yet!
        return __LINE__;
    }
    ret = key_manage_read(keyname, dataBuf, keyLen);
    if (ret) {
        KM_ERR("Fail in read key[%s] at sz %zd\n", keyname, keyLen);
        return __LINE__;
    }
    if (dataFmt)
    {
        if (!strcmp("hex", dataFmt))
        {
            _keyman_buf_to_hex_ascii((uint8_t*)dataBuf, keyLen, NULL, 0);
            return 0;
        }
        else if(!strcmp("str", dataFmt))
        {
            int i = 0;

            dataBuf[keyLen] = '\0';
            for (; i < keyLen; ++i) {
                ret = isascii(dataBuf[i]);
                if (!ret) {
                    KM_MSG("key value has non ascii, can't pr\n");
                    return CMD_RET_FAILURE;
                }
            }
            env_set(keyname, dataBuf);//setenv for bootargs
            KM_DBG("env:%s=%s\n", keyname, dataBuf);
            return 0;
        }
        else KM_MSG("Err key dataFmt(%s)\n", dataFmt);
    }

    //return ret ? CMD_RET_FAILURE : CMD_RET_SUCCESS;
    return CMD_RET_SUCCESS;
}

//argv: 1       2       3
//write keyname size    addr
//write keyname hex/str value
static int do_keyman_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    const char* keyname     = argv[1];
    const char* databuf     = NULL;
    char*       tmpBuf      = NULL;
    const char* inputFmt    = argv[2];
    unsigned    dataLen     = 0;
    if (argc < 4) return CMD_RET_USAGE;

    if (!strcmp("hex", inputFmt))
    {
        databuf = argv[3];
        dataLen = strlen(databuf) / 2;

        tmpBuf = (char*)malloc(dataLen);
        ret = _keyman_hex_ascii_to_buf(databuf, tmpBuf, dataLen);
        if (ret) {
            KM_ERR("Fail in change hex argv[3] to bin, err=%d\n", ret);
            free(tmpBuf);
            return CMD_RET_FAILURE;
        }
        databuf = tmpBuf;
    }
    else if(!strcmp("str", inputFmt))
    {
        databuf = argv[3];
        dataLen = strlen(databuf);

        const char* p = databuf;
        for (; *p; ++p) {
            if (!isascii(*p)) {
                KM_ERR("inputFmt is %s, but argv[3] contain non ascii\n", inputFmt);
                return CMD_RET_FAILURE;
            }
        }
        KM_DBG("str:%s, len=%d\n", databuf, dataLen);
    }
    else
    {
        dataLen = simple_strtoul(argv[2], NULL, 0);
        if (!dataLen) {
            KM_ERR("dataLen err\n");
            return __LINE__;
        }
        if (dataLen > (64*1024)) {
            KM_ERR("keylen 0x%x too large!\n", dataLen);
            return __LINE__;
        }
        databuf = (char*)simple_strtoul(argv[3], NULL, 16);
    }

    ret = key_manage_write(keyname, databuf, dataLen);
    if (tmpBuf) free(tmpBuf) ;
    return ret ? CMD_RET_FAILURE : CMD_RET_SUCCESS;
}

//query: 1          2
//      exist       keyname
//      secure      keyname
//      size        keyname
static int do_keyman_query(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    const char* cmd     = argv[1];
    const char* keyname = argv[2];
    if (argc < 3) return CMD_RET_USAGE;

    if (!strcmp("exist", cmd))
    {
        int exist = 0;
        ret = key_manage_query_exist(keyname, &exist);
        if (ret) {
            KM_ERR("Fail in query key exist, err=%d", ret);
            return CMD_RET_FAILURE;
        }
        KM_MSG("key[%s] is %s Exist\n", keyname, exist ? "" : "NOT");
        ret = exist ? CMD_RET_SUCCESS : CMD_RET_FAILURE;
    }
    else if(!strcmp("secure", cmd))
    {
        int isSecure = 0;
        ret = key_manage_query_secure(keyname, &isSecure);
        if (ret) {
            KM_ERR("Fail in query key secure, err=%d\n", ret);
            return CMD_RET_FAILURE;
        }
        KM_MSG("key[%s] is %s Secure\n", keyname, isSecure ? "" : "NOT");
        ret = isSecure ? CMD_RET_SUCCESS : CMD_RET_FAILURE;
    }
    else if(!strcmp("size", cmd))
    {
        ssize_t keysize = 0;
        ret = key_manage_query_size(keyname, &keysize);
        if (ret) {
            KM_ERR("Fail in query key size, err=%d\n", ret);
            return CMD_RET_FAILURE;
        }
        KM_MSG("key[%s] size is %zd\n", keyname, keysize);
        ret = keysize ? CMD_RET_SUCCESS : CMD_RET_FAILURE;
    }
    else return CMD_RET_USAGE;

    return ret;
}

static cmd_tbl_t cmd_keyman_sub[] = {
    U_BOOT_CMD_MKENT(init,          3, 0, do_keyman_init, "", ""),
    U_BOOT_CMD_MKENT(exit,          2, 0, do_keyman_exit, "", ""),
    U_BOOT_CMD_MKENT(read,          4, 0, do_keyman_read, "", ""),
    U_BOOT_CMD_MKENT(write,         4, 0, do_keyman_write, "", ""),
    U_BOOT_CMD_MKENT(query,         3, 0, do_keyman_query, "", ""),
};

static int do_keymanage(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    cmd_tbl_t *c;

    if (argc < 2) return CMD_RET_USAGE;

    c = find_cmd_tbl(argv[1], cmd_keyman_sub, ARRAY_SIZE(cmd_keyman_sub));

	if (!c) { return CMD_RET_USAGE;}

    return	c->cmd(cmdtp, flag, --argc, ++argv);
}

U_BOOT_CMD(
   keyman,           //command name
   5,                   //maxargs
   0,                   //repeatable
   do_keymanage,   //command function
   "Unify key ops interfaces based dts cfg",           //description
   "    argv:  \n"   //usage
   "    init seedNum <dtbAddr>\n"
   "    read keyname addr <hex/str>\n"
   "    write keyname size addr \n"
   "    write keyname hex/str value\n"
   "    query exist/secure/size keyname\n"
   "    exit \n"
);

