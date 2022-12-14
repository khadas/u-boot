// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include "key_manage_i.h"
#include <amlogic/amlkey_if.h>
#include <u-boot/sha256.h>

int keymanage_securekey_init(const char* buf, int len)
{
	int encrypt_type;

	encrypt_type = unifykey_get_encrypt_type();
	return amlkey_init((uint8_t*)buf, len, encrypt_type);	//confirm
}

int keymanage_securekey_exit(void)
{
    return 0;
}

int keymanage_secukey_write(const char *keyname, const void* keydata, unsigned int datalen)
{
    int ret = 0;
    uint8_t origSum[SHA256_SUM_LEN];
    const int isSecure =  ( KEY_M_SECURE_KEY == keymanage_dts_get_key_device(keyname) ) ? 1 : 0;
    const int isEncrypt= strlen(keymanage_dts_get_enc_type(keyname)) ? 1 : 0;
    const unsigned int keyAttr = ( isSecure << 0 ) | ( isEncrypt << 8 );
    ssize_t writenLen = 0;

    if (isSecure)
    {
        sha256_context ctx;
        sha256_starts(&ctx);
        sha256_update(&ctx, keydata, datalen);
        sha256_finish(&ctx, origSum);
    }

    KM_MSG("isEncrypt=%s\n", keymanage_dts_get_enc_type(keyname));
    KM_DBG("%s, keyname=%s, keydata=%p, datalen=%d, isSecure=%d\n", __func__, keyname, keydata, datalen, isSecure);
    KM_MSG("keyAttr is 0x%08X\n", keyAttr);
    writenLen = amlkey_write((uint8_t*)keyname, (uint8_t*)keydata, datalen, keyAttr);
    if (writenLen != datalen) {
        KM_ERR("Want to write %u bytes, but only %zd Bytes\n", datalen, writenLen);
        return __LINE__;
    }

    if (isSecure)
    {
        uint8_t genSum[SHA256_SUM_LEN];

        ret = amlkey_hash_4_secure((uint8_t*)keyname, genSum);
        if (ret) {
            KM_ERR("Failed when gen hash for secure key[%s], ret=%d\n", keyname, ret);
            return __LINE__;
        }

        ret = memcmp(origSum, genSum, SHA256_SUM_LEN);
        if (ret)
        {
            int index = 0;
            char origSum_str[SHA256_SUM_LEN * 2 + 2];
            char genSum_str[SHA256_SUM_LEN * 2 + 2];
            int org_n = 0, gen_n = 0;

            origSum_str[0] = genSum_str[0] = '\0';
            for (index = 0; index < SHA256_SUM_LEN; ++index) {

                //sprintf(origSum_str, "%s%02x", origSum_str, origSum[index]);
                //sprintf(genSum_str, "%s%02x", genSum_str, genSum[index]);
                org_n += sprintf(&origSum_str[org_n], "%02x", origSum[index]);
                gen_n += sprintf(&genSum_str[gen_n], "%02x", genSum[index]);
            }

            KM_ERR("Failed in check hash, origSum[%s] != genSum[%s]\n", origSum_str, genSum_str);
            return __LINE__;
        }
        KM_MSG("OK in check sha1256 in burn key[%s]\n", keyname);
    }

    return ret;
}

ssize_t keymanage_secukey_size(const char* keyname)
{
	return amlkey_size((uint8_t*)keyname);	//actually size
}

int keymanage_secukey_exist(const char* keyname)
{
	return amlkey_isexsit((uint8_t*)keyname);	//exsit 1, non 0
}

int keymanage_secukey_can_read(const char* keyname)
{
	return !amlkey_issecure((uint8_t*)keyname);	//secure 1, non 0
}

int keymanage_secukey_read(const char* keyname, void* databuf,  unsigned buflen)
{
    int ret = 0;

    ret = keymanage_secukey_can_read(keyname);
    if (!ret) {
        KM_ERR("key[%s] can't read, is configured secured?\n", keyname);
        return __LINE__;
    }

	const ssize_t readLen = amlkey_read((uint8_t*)keyname, (uint8_t*)databuf, buflen);
    if (readLen != buflen) {
        KM_ERR("key[%s], want read %u Bytes, but %zd bytes\n", keyname, buflen, readLen);
        return __LINE__;
    }

    return 0;
}

#if 1//hdcp2 rx start
#define HDCP2_RX_LC128_LEN         (36)
#define HDCP2_RX_KEY_LEN           (862)
#pragma pack(push, 1)
typedef struct _Hdcp2RxKeyFmt{
    unsigned                version;
    char                    lc128[HDCP2_RX_LC128_LEN];
    char                    keyVal[HDCP2_RX_KEY_LEN];
}Hdcp2RxKeyFmt_t;
#pragma pack(pop)

#define HDCP2_RX_KEY_TOTAL_LEN        sizeof(Hdcp2RxKeyFmt_t)
#define HDCP2_RX_KEY_LC128_NAME       "hdcp2lc128"
#define HDCP2_RX_KEY_NAME             "hdcp2key"
#define HDCP2_RX_KEY_VERSION           (0x02000000U)
#define HDCP2_VERSION_LEN               ( 4 )

static char generalDataChange(const char input)
{
    int i;
    char result = 0;

    for (i=0; i<8; i++) {
        if ((input & (1<<i)) != 0)
            result |= (1<<(7-i));
        else
            result &= ~(1<<(7-i));
    }

    return result;
}

static void hdcp2DataEncryption(const unsigned len, const char *input, char *out)
{
    int i = 0;

    for (i=0; i<len; i++)
        *out++ = generalDataChange(*input++);
}

static void hdcp2DataDecryption(const unsigned len, const char *input, char *out)
{
    int i = 0;

    for (i=0; i<len; i++)
        *out++ = generalDataChange(*input++);
}

int _burn_key_in_type_hdcp2(const char* keyname, void* databuf, const unsigned bufLen, char* decryptBuf)
{
    Hdcp2RxKeyFmt_t* pHdcp2RxKey = (Hdcp2RxKeyFmt_t*)databuf;
    const int keyLen = HDCP2_RX_KEY_TOTAL_LEN;

    if (keyLen > bufLen) {
        KM_ERR("hdcp2 rx len unsupported. want %d but get %d\n", keyLen, bufLen);
        return __LINE__;
    }
    if (HDCP2_RX_KEY_VERSION != pHdcp2RxKey->version) {
        KM_ERR("Version value 0x%x is error, should be 0x%x\n", pHdcp2RxKey->version, HDCP2_RX_KEY_VERSION);
        return __LINE__;
    }

    hdcp2DataEncryption(keyLen, databuf, decryptBuf);
    KM_MSG("encrypt hdcp2 END.\n");
    pHdcp2RxKey = (Hdcp2RxKeyFmt_t*)decryptBuf;

    const uint8_t* tmpName      = (uint8_t*)HDCP2_RX_KEY_LC128_NAME;
    unsigned        tmpLen      = HDCP2_RX_LC128_LEN;
    unsigned        isSecure    = 0;
    ssize_t retLen = 0;
    retLen = amlkey_write(tmpName, (uint8_t*)&pHdcp2RxKey->lc128, tmpLen , isSecure);
    if (retLen != tmpLen) {
        KM_ERR ("Fail in write hdcp2 lc128, retLen %zd != want len %d\n", retLen, tmpLen) ;
        return __LINE__;
    }

    tmpName = (uint8_t*)HDCP2_RX_KEY_NAME;
    tmpLen = HDCP2_RX_KEY_LEN;
    retLen = amlkey_write(tmpName, (uint8_t*)&pHdcp2RxKey->keyVal, tmpLen , isSecure);
    if (retLen != tmpLen) {
        KM_ERR ("Fail in write hdcp2 key, retLen %zd != want len %d\n", retLen, tmpLen) ;
        return __LINE__;
    }

    tmpLen = HDCP2_VERSION_LEN;
    retLen = amlkey_write((uint8_t*)keyname, (uint8_t*)&pHdcp2RxKey->version, tmpLen, isSecure);
    if (retLen != tmpLen) {
        KM_ERR ("Fail in write hdcp2 key, retLen %zd != want len %d\n", retLen, tmpLen) ;
        return __LINE__;
    }

    return 0;
}

int _read_key_in_type_hdcp2(const char* keyname, void* databuf, const unsigned bufLen, char* decryptBuf)
{
    const unsigned srcKeyLen    = HDCP2_RX_KEY_TOTAL_LEN;
    Hdcp2RxKeyFmt_t* pHdcp2RxKey = (Hdcp2RxKeyFmt_t*)decryptBuf;
    const uint8_t* tmpName = NULL;
    int tmpLen = 0;

    if (bufLen < srcKeyLen) {
        KM_ERR("hdcp2 rx len unsupported. want %d but only %d\n", srcKeyLen, bufLen);
        return __LINE__;
    }

    ssize_t retLen = 0;
    tmpName = (uint8_t*)keyname;
    tmpLen = HDCP2_VERSION_LEN;
    retLen = amlkey_read(tmpName, (uint8_t*)&pHdcp2RxKey->version, tmpLen);
    if (retLen != tmpLen) {
        KM_ERR ("Fail in read key[%s] at len %d\n", tmpName, tmpLen) ;
        return __LINE__;
    }

    tmpName = (uint8_t*)HDCP2_RX_KEY_NAME;
    tmpLen  = HDCP2_RX_KEY_LEN;
    retLen = amlkey_read(tmpName, (uint8_t*)&pHdcp2RxKey->keyVal, tmpLen);
    if (retLen != tmpLen) {
        KM_ERR ("Fail in read key[%s] at len %d\n", tmpName, tmpLen) ;
        return __LINE__;
    }

    tmpName = (uint8_t*)HDCP2_RX_KEY_LC128_NAME;
    tmpLen  = HDCP2_RX_LC128_LEN;
    retLen = amlkey_read(tmpName, (uint8_t*)&pHdcp2RxKey->lc128, tmpLen);
    if (retLen != tmpLen) {
        KM_ERR ("Fail in read key[%s] at len %d\n", tmpName, tmpLen) ;
        return __LINE__;
    }

    hdcp2DataDecryption(srcKeyLen, (char*)pHdcp2RxKey, databuf);

    return 0;
}
int _km_hdcp2_size(void)
{
    return HDCP2_RX_KEY_TOTAL_LEN;
}
#endif//hdcp2 rx end


