// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

/*
 *
 * Copyright (C) 2018 Amlogic, Inc. All rights reserved.
*/

#include <common.h>
#include <asm/arch/io.h>
#include <amlogic/aml_efuse.h>
#include <asm/arch/secure_apb.h>

/*load license bit [0...127...255...511]*/
#define OTP_BIT_LOAD(feat) (readl(OTP_LIC00 + (((feat)/32)<<2)) & (1<<((feat)%32)))

#define FEAT_ENABLE_DEVICE_SCS_SIG_0             (10)
#define FEAT_ENABLE_DEVICE_SCS_SIG_1             (11)
#define IS_FEAT_EN_DEVICE_SCS_SIG()			        \
	((OTP_BIT_LOAD(FEAT_ENABLE_DEVICE_SCS_SIG_0) ||		\
	  OTP_BIT_LOAD(FEAT_ENABLE_DEVICE_SCS_SIG_1)) ? 1 : 0)
#define FEAT_ENABLE_DEVICE_PROT_0                (22)
#define FEAT_ENABLE_DEVICE_PROT_1                (23)
#define IS_FEAT_EN_DEVICE_PROT()			        \
	((OTP_BIT_LOAD(FEAT_ENABLE_DEVICE_PROT_0) ||	\
	  OTP_BIT_LOAD(FEAT_ENABLE_DEVICE_PROT_1)) ? 1 : 0)

/*check license bit*/
#define OTP_BIT_CHECK(feat) (OTP_BIT_LOAD((feat)) ? 1 : 0)

/* OTP_LIC block#3 ...*/
#define FEAT_DISABLE_NBL2_SNOR                   (384+44)
#define FEAT_DISABLE_NBL2_SNAND                  (384+45)
#define FEAT_ENABLE_4BL2_SNOR                    (384+46)
#define FEAT_ENABLE_8BL2_SNAND                   (384+47)
#define FEAT_DISABLE_EMMC_USER                   (384+53)
#define FEAT_DISABLE_EMMC_BOOT_0                 (384+54)
#define FEAT_DISABLE_EMMC_BOOT_1                 (384+55)
#define FEAT_DISABLE_NBL2_NAND                   (384+57)
#define FEAT_DISABLE_8BL2_NAND                   (384+58)

//function for EFUSE license query
int  IS_FEAT_BOOT_VERIFY(void)
{
	return IS_FEAT_EN_DEVICE_SCS_SIG();
}

int  IS_FEAT_BOOT_ENCRYPT(void)
{
	return IS_FEAT_EN_DEVICE_PROT();
}

int IS_FEAT_DIS_EMMC_USER(void)
{
	return OTP_BIT_CHECK(FEAT_DISABLE_EMMC_USER);
}

int IS_FEAT_DIS_EMMC_BOOT_0(void)
{
	return OTP_BIT_CHECK(FEAT_DISABLE_EMMC_BOOT_0);
}

int IS_FEAT_DIS_EMMC_BOOT_1(void)
{
	return OTP_BIT_CHECK(FEAT_DISABLE_EMMC_BOOT_1);
}

int IS_FEAT_EN_4BL2_SNOR(void)
{
	return OTP_BIT_CHECK(FEAT_ENABLE_4BL2_SNOR);
}

int IS_FEAT_DIS_NBL2_SNOR(void)
{
	return OTP_BIT_CHECK(FEAT_DISABLE_NBL2_SNOR);
}

int IS_FEAT_EN_8BL2_SNAND(void)
{
	return OTP_BIT_CHECK(FEAT_ENABLE_8BL2_SNAND);
}

int IS_FEAT_DIS_NBL2_SNAND(void)
{
	return OTP_BIT_CHECK(FEAT_DISABLE_NBL2_SNAND);
}

int IS_FEAT_DIS_8BL2_NAND(void)
{
	return OTP_BIT_CHECK(FEAT_DISABLE_8BL2_NAND);
}

int IS_FEAT_DIS_NBL2_NAND(void)
{
	return OTP_BIT_CHECK(FEAT_DISABLE_NBL2_NAND);
}
