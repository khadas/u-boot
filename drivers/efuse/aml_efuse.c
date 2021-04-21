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

//weak function for EFUSE license query
//all following functions are defined with "weak" for customization of each SoC
//EFUSE_LICX	--> AO_SEC_SD_CFG10/9 --> EFUSE mirror
int  __attribute__((weak)) IS_FEAT_BOOT_VERIFY(void)
{
	#ifndef ADDR_IS_FEAT_BOOT_VERIFY
	  #ifdef EFUSE_LIC0
		  #define ADDR_IS_FEAT_BOOT_VERIFY (EFUSE_LIC0)
		  #define OSET_IS_FEAT_BOOT_VERIFY (0)
	  #elif defined(OTP_LIC0)
		  #define ADDR_IS_FEAT_BOOT_VERIFY (OTP_LIC0)
		  #define OSET_IS_FEAT_BOOT_VERIFY (0)
	  #else
		  #define ADDR_IS_FEAT_BOOT_VERIFY (AO_SEC_SD_CFG10)
		  #define OSET_IS_FEAT_BOOT_VERIFY (4)
	  #endif
	#endif

	return ((readl(ADDR_IS_FEAT_BOOT_VERIFY) >> OSET_IS_FEAT_BOOT_VERIFY) & 1);

	#undef ADDR_IS_FEAT_BOOT_VERIFY
	#undef OSET_IS_FEAT_BOOT_VERIFY
}
int  __attribute__((weak)) IS_FEAT_BOOT_ENCRYPT(void)
{
	#ifndef ADDR_IS_FEAT_BOOT_ENCRYPT
	  #ifdef EFUSE_LIC0
		#define ADDR_IS_FEAT_BOOT_ENCRYPT (EFUSE_LIC0)
		#define OSET_IS_FEAT_BOOT_ENCRYPT (1)
	  #elif defined(OTP_LIC0)
		#define ADDR_IS_FEAT_BOOT_ENCRYPT (OTP_LIC0)
		#define OSET_IS_FEAT_BOOT_ENCRYPT (1)
	  #else
		#define ADDR_IS_FEAT_BOOT_ENCRYPT (AO_SEC_SD_CFG10)
		#define OSET_IS_FEAT_BOOT_ENCRYPT (28)
	  #endif
	#endif

	return ((readl(ADDR_IS_FEAT_BOOT_ENCRYPT) >> OSET_IS_FEAT_BOOT_ENCRYPT) & 1);

	#undef ADDR_IS_FEAT_BOOT_ENCRYPT
	#undef OSET_IS_FEAT_BOOT_ENCRYPT
}

int  __attribute__((weak)) IS_FEAT_DIS_EMMC_USER(void)
{
	return 0;
}

int  __attribute__((weak)) IS_FEAT_DIS_EMMC_BOOT_0(void)
{
	return 0;
}

int  __attribute__((weak)) IS_FEAT_DIS_EMMC_BOOT_1(void)
{
	return 0;
}

int  __attribute__((weak)) IS_FEAT_EN_4BL2_SNOR(void)
{
	return 0;
}

int  __attribute__((weak)) IS_FEAT_DIS_NBL2_SNOR(void)
{
	return 0;
}

int  __attribute__((weak)) IS_FEAT_EN_8BL2_SNAND(void)
{
	return 0;
}

int  __attribute__((weak)) IS_FEAT_DIS_NBL2_SNAND(void)
{
	return 0;
}

int  __attribute__((weak)) IS_FEAT_DIS_8BL2_NAND(void)
{
	return 0;
}

int  __attribute__((weak)) IS_FEAT_DIS_NBL2_NAND(void)
{
	return 1;
}
