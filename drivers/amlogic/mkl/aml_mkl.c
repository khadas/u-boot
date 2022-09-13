// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <amlogic/aml_mkl.h>

#include <malloc.h>
#include <string.h>
#include <dma.h>
#include <asm/io.h>
#include <asm/arch/regs.h>
#include <asm/arch/register.h>
#include <asm/arch/secure_apb.h>

// #define MKL_DMA_TEST

/* kl offset */
#define KL_PENDING_OFFSET   (31)
#define KL_PENDING_MASK	    (1)
#define KL_STATUS_OFFSET    (29)
#define KL_STATUS_MASK	    (3)
#define KL_ALGO_OFFSET	    (28)
#define KL_ALGO_MASK	    (1)
#define KL_MODE_OFFSET	    (26)
#define KL_MODE_MASK	    (3)
#define KL_FLAG_OFFSET	    (24)
#define KL_FLAG_MASK	    (3)
#define KL_KEYALGO_OFFSET   (20)
#define KL_KEYALGO_MASK	    (0xf)
#define KL_USERID_OFFSET    (16)
#define KL_USERID_MASK	    (0xf)
#define KL_KTE_OFFSET	    (8)
#define KL_KTE_MASK         (0xff)
#define KL_MRK_OFFSET	    (4)
#define KL_MRK_MASK	        (0xf)
#define KL_FUNC_ID_OFFSET   (0)
#define KL_FUNC_ID_MASK	    (0xf)
#define KL_MID_OFFSET	    (24)
#define KL_MID_MASK	        (0xff)
#define KL_MID_EXTRA_OFFSET (8)
#define KL_TEE_PRIV_OFFSET  (7)
#define KL_TEE_PRIV_MASK    (1)
#define KL_TEE_SEP_OFFSET   (2)
#define KL_TEE_SEP_MASK	    (3)
#define KL_LEVEL_OFFSET	    (4)
#define KL_LEVEL_MASK	    (7)
#define KL_STAGE_OFFSET	    (0)
#define KL_STAGE_MASK	    (3)

/* kl etc */
#define KT_KTE_MAX		    (256)
#define HANDLE_TO_KTE(h)	((h) & (KT_KTE_MAX - 1))
#define KL_PENDING_WAIT_TIMEOUT (20000)

/* kl status */
#define KL_STATUS_OK			 (0)
#define KL_STATUS_ERROR_PERMISSION_DINED (1)
#define KL_STATUS_ERROR_OTP		 (2)
#define KL_STATUS_ERROR_DEPOSIT		 (3)
#define KL_STATUS_ERROR_TIMEOUT		 (4)
#define KL_STATUS_ERROR_BAD_PARAM	 (5)
#define KL_STATUS_ERROR_BAD_STATE	 (6)

static int aml_mkl_program_key(uint32_t addr, const uint8_t *data, uint32_t len)
{
	int i = 0;
	const u32 *data32 = (const u32 *)data;

	if (!data32 || len != 16)
		return -1;

	for (i = 0; i < 4; i++)
		writel(data32[i], (uint32_t *)MKL_REE_EK + addr + i);

	return 0;
}

static int aml_mkl_lock(void)
{
	int cnt = 0;

	while (readl((uint32_t *)MKL_REE_RDY) != 1) {
		if (cnt++ > KL_PENDING_WAIT_TIMEOUT) {
			printf("Error: wait KT ready timeout\n");
			return KL_STATUS_ERROR_TIMEOUT;
		}
	}

	return KL_STATUS_OK;
}

static void aml_mkl_unlock(void)
{
	writel(1, (uint32_t *)MKL_REE_RDY);
}

int aml_mkl_run(struct amlkl_params *param)
{
	int ret;
	int i;
	u32 reg_val = 0;
	struct amlkl_usage *pu;

	if (!param)
		return KL_STATUS_ERROR_BAD_PARAM;

	pu = &param->usage;
	printf("kth:%d, levels:%d, kl_algo:%d, func_id:%d, mrk:%d\n",
			param->kt_handle, param->levels, param->kl_algo, param->func_id,
			param->mrk_cfg_index);
	printf("kt usage, crypto:%d, algo:%d, uid:%d\n",
			pu->crypto, pu->algo, pu->uid);

	if (pu->uid & ~KL_USERID_MASK ||
		(pu->uid > AML_KT_USER_M2M_5 && pu->uid < AML_KT_USER_TSD) ||
		(pu->uid > AML_KT_USER_TSE && pu->uid < KL_USERID_MASK) ||
		pu->algo & ~KL_KEYALGO_MASK ||
		(pu->algo > AML_KT_ALGO_DES && pu->algo < AML_KT_ALGO_NDL) ||
		(pu->algo > AML_KT_ALGO_CSA2 && pu->algo < AML_KT_ALGO_HMAC) ||
		(pu->algo > AML_KT_ALGO_HMAC && pu->algo < KL_KEYALGO_MASK) ||
		pu->crypto & ~KL_FLAG_MASK || param->levels != AML_KL_LEVEL_3 ||
		param->kl_algo > AML_KL_ALGO_AES) {
		return KL_STATUS_ERROR_BAD_PARAM;
	}

	/* 1. Read KL_REE_RDY to lock KL */
	if (aml_mkl_lock() != KL_STATUS_OK) {
		printf("key ladder not ready\n");
		return KL_STATUS_ERROR_BAD_STATE;
	}

	/* 2. Program Eks */
	for (i = 0; i < param->levels; i++) {
		ret = aml_mkl_program_key(i * 4, param->eks[param->levels - 1 - i],
						sizeof(param->eks[param->levels - 1 - i]));
		if (ret != 0) {
			printf("Error: Ek data has bad parameter\n");
			return KL_STATUS_ERROR_BAD_PARAM;
		}
	}

	/* 3. Program KL_REE_CMD */
	reg_val = 0;
	reg_val =
		(0 << KL_TEE_PRIV_OFFSET | 0 << KL_LEVEL_OFFSET
		| 0 << KL_STAGE_OFFSET | 0 << KL_TEE_SEP_OFFSET);
	writel(reg_val, (uint32_t *)MKL_REE_CMD);

	/* 4. Program KL_REE_CFG with KL_pending to 1 */
	reg_val = 0;
	reg_val =
		(1 << KL_PENDING_OFFSET
		 | param->kl_algo << KL_ALGO_OFFSET
		 | param->kl_mode << KL_MODE_OFFSET
		 | param->usage.crypto << KL_FLAG_OFFSET
		 | param->usage.algo << KL_KEYALGO_OFFSET
		 | param->usage.uid << KL_USERID_OFFSET
		 | (HANDLE_TO_KTE(param->kt_handle)) << KL_KTE_OFFSET
		 | param->mrk_cfg_index << KL_MRK_OFFSET
		 | (param->func_id << KL_FUNC_ID_OFFSET));
	writel(reg_val, (uint32_t *)MKL_REE_CFG);

	/* 5. Poll KL_REE_CFG till KL_pending is 0 */
	while ((ret = readl((uint32_t *)MKL_REE_CFG) &
			(1 << KL_PENDING_OFFSET)))
		;

	/* 6.	Write KL_REE_RDY to release KL */
	aml_mkl_unlock();

	/* 7. Get final status */
	ret = (ret >> KL_STATUS_OFFSET) & KL_STATUS_MASK;

	switch (ret) {
	case 0:
		break;
	case 1:
		printf("Permission Denied Error code: %d\n", ret);
		return KL_STATUS_ERROR_BAD_STATE;
	case 2:
	case 3:
	default:
		printf("OTP or KL Error code: %d\n", ret);
		return KL_STATUS_ERROR_BAD_STATE;
	}

	printf("aml key ladder run success\n");

	return ret;
}

#ifdef MKL_DMA_TEST
#include <amlogic/aml_crypto.h>

static int mkl_test_dvgk(void)
{
	int ret = 0;
	struct amlkl_params kl_param;
	uint32_t dvgk_test_key_slot = 77; //only test slot
	uint8_t dst[16] = {0};

	/* dvgk_func_0_bs_0_tsep_0.test.vector.txt */
	uint8_t dvgk_ek3[16] = {
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	};

	uint8_t dvgk_ek2[16] = {
		0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
		0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
	};

	uint8_t dvgk_ek1[16] = {
		0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
		0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f
	};

	uint8_t dvgk_plain[16] = {
		0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
		0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xb0
	};

	uint8_t dvgk_cipher[16] = {
		0x74, 0x1e, 0x39, 0x05, 0xc2, 0x53, 0x05, 0x15,
		0xfd, 0x5c, 0xb0, 0x07, 0xcc, 0x81, 0x3b, 0xaf
	};

	memset(&kl_param, 0, sizeof(kl_param));
	kl_param.kt_handle = dvgk_test_key_slot;
	kl_param.levels = AML_KL_LEVEL_3;
	kl_param.usage.crypto = AML_KT_FLAG_ENC_DEC;
	kl_param.usage.algo = AML_KT_ALGO_AES;
	kl_param.usage.uid = AML_KT_USER_M2M_0;
	kl_param.kl_algo = AML_KL_ALGO_AES;
	kl_param.kl_mode = AML_KL_MODE_AML;
	kl_param.mrk_cfg_index = AML_KL_MRK_DVGK;
	kl_param.func_id = AML_KL_FUNC_AES_0;
	memcpy(kl_param.eks[0], dvgk_ek1, 16);
	memcpy(kl_param.eks[1], dvgk_ek2, 16);
	memcpy(kl_param.eks[2], dvgk_ek3, 16);
	ret = aml_mkl_run(&kl_param);
	if (ret != 0) {
		printf("aml_mkl_run error[ret:%d]\n", ret);
		return -1;
	}

	ret = aes_ecb_enc_keytbl(AES_KEY_SIZE_128, dvgk_plain, dst,
			NULL, sizeof(dvgk_plain), dvgk_test_key_slot);
	if (!memcmp(dst, dvgk_cipher, sizeof(dvgk_cipher)))
		printf("MKL DMA TEST PASS\n");
	else
		printf("MKL DMA TEST FAIL\n");

	return ret;
}

static int do_mkl_dma_test(uint32_t test_case)
{
	switch (test_case) {
	case 0:
		mkl_test_dvgk();
		return 0;
	default:
		printf("Unknown test case\n");
		return 0;
	}
}

static int do_mkl_test(cmd_tbl_t *cmdtp, int flag, int argc,
				char *const argv[])
{
	uint32_t test_case = 0;
	char *endp = 0;

	if (argc != 2) {
		cmd_usage(cmdtp);
		return 1;
	}

	test_case = ustrtoul(argv[1], &endp, 0);

	do_mkl_dma_test(test_case);

	return 0;
}

U_BOOT_CMD(mkl, 2, 0, do_mkl_test,
	"mkl <test case>",
	"if case=0, run mkl dma test for dvgk\n"
	);
#endif
