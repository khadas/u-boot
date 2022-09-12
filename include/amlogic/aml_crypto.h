/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _AML_CRYPTO_H
#define _AML_CRYPTO_H

#include <u-boot/sha256.h>

#define CRYPTO_ERROR_NO_ERROR       (0)
#define CRYPTO_ERROR_BAD_PARAMETERS (-1)
#define CRYPTO_ERROR_BAD_PROCESS    (-2)

#define AES_KEY_SIZE_128         16
#define AES_KEY_SIZE_192         24
#define AES_KEY_SIZE_256         32

#define DES_KEY_SIZE            8
#define TDES_2K_KEY_SIZE        16
#define TDES_3K_KEY_SIZE        24

/*
 * aes_ecb_enc_keytbl - AES ECB encrypt with keytable
 *
 * @input - input pointer
 * @output - output pointer
 * @nbytes - data length
 * @key_slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t aes_ecb_enc_keytbl(uint32_t keylen, const void *input, void *output,
		uint8_t iv[16], size_t nbytes, uint32_t slot);
/*
 * aes_ecb_dec_keytbl - AES ECB decrypt with keytable
 *
 * @input - input pointer
 * @output - output pointer
 * @nbytes - data length
 * @key_slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t aes_ecb_dec_keytbl(uint32_t keylen, const void *input, void *output,
		uint8_t iv[16], size_t nbytes, uint32_t slot);
/*
 * aes_cbc_enc_keytbl - AES CBC encrypt with keytable
 *
 * @input - input pointer
 * @output - output pointer
 * @iv - AES IV
 * @nbytes - data length
 * @key_slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t aes_cbc_enc_keytbl(uint32_t keylen, const void *input, void *output,
		uint8_t iv[16], size_t nbytes, uint32_t slot);
/*
 * aes256cbc_dec_keytbl - AES CBC decrypt with keytable
 *
 * @input - input pointer
 * @output - output pointer
 * @iv - AES IV
 * @nbytes - data length
 * @key_slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t aes256cbc_dec_keytbl(const void *input, void *output, uint8_t iv[16],
		size_t nbytes, uint32_t key_slot);

/*
 * aes_cbc_dec_keytbl - AES CBC decrypt with keytable
 *
 * @keylen - key length
 * @input - input pointer
 * @output - output pointer
 * @iv - AES IV
 * @nbytes - data length
 * @slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t aes_cbc_dec_keytbl(uint32_t keylen, const void *input, void *output, uint8_t iv[16],
		size_t nbytes, uint32_t slot);
/*
 * aes_ctr_encrypt_keytbl - AES CTR encrypt with keytable
 *
 * @keylen - key length
 * @input - input pointer
 * @output - output pointer
 * @iv - AES initial counter
 * @nbytes - data length
 * @slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t aes_ctr_encrypt_keytbl(uint32_t keylen, const void *input, void *output,
		uint8_t iv[16], size_t nbytes, uint32_t slot);
/*
 * aes_ctr_decrypt_keytbl - AES CTR decrypt with keytable
 *
 * @keylen - key length
 * @input - input pointer
 * @output - output pointer
 * @iv - AES initial counter
 * @nbytes - data length
 * @slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t aes_ctr_decrypt_keytbl(uint32_t keylen, const void *input, void *output,
		uint8_t iv[16], size_t nbytes, uint32_t slot);
/*
 * aes256cbc_iv0_dec_keytbl - AES CBC decrypt with keytable and iv0
 *
 * @input - input pointer
 * @output - output pointer
 * @nbytes - data length
 * @slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t aes256cbc_iv0_dec_keytbl(const void *input, void *output, size_t nbytes,
		uint32_t slot);

/*
 * des_tdes_ecb_enc_keytbl - DES/TDES ECB encrypt with keytable
 *
 * @keylen - key length
 * @input - input pointer
 * @output - output pointer
 * @iv - TDES IV
 * @nbytes - data length
 * @slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t des_tdes_ecb_enc_keytbl(uint32_t keylen, const void *input,
		void *output, size_t nbytes, uint32_t slot);
/*
 * des_tdes_ecb_dec_keytbl - DES/TDES ECB decrypt with keytable
 *
 * @keylen - key length
 * @input - input pointer
 * @output - output pointer
 * @iv - TDES IV
 * @nbytes - data length
 * @slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t des_tdes_ecb_dec_keytbl(uint32_t keylen, const void *input,
		void *output, uint8_t iv[16], size_t nbytes, uint32_t slot);
/*
 * des_tdes_cbc_enc_keytbl - DES/TDES CBC encrypt with keytable
 *
 * @keylen - key length
 * @input - input pointer
 * @output - output pointer
 * @iv - TDES IV
 * @nbytes - data length
 * @slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t des_tdes_cbc_enc_keytbl(uint32_t keylen, const void *input,
		void *output, uint8_t iv[8], size_t nbytes, uint32_t slot);
/*
 * des_tdes_cbc_dec_keytbl - DES/TDES CBC decrypt with keytable
 *
 * @keylen - key length
 * @input - input pointer
 * @output - output pointer
 * @iv - TDES IV
 * @nbytes - data length
 * @slot - key table slot to be used
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t des_tdes_cbc_dec_keytbl(uint32_t keylen, const void *input,
		void *output, uint8_t iv[8], size_t nbytes, uint32_t slot);

/*
 * aml_hw_init - HW SHA Init
 *
 * @cur_ctx - SHA context
 * @is224 - SHA224 or SHA256
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t aml_hw_sha_init(sha2_ctx *cur_ctx, uint32_t is224);
/*
 * aml_hw_sha_update - HW SHA Update
 *
 * @cur_ctx - SHA context
 * @input - input pointer
 * @ilen - input size
 * @hash - output hash
 * @last_update - to finalize SHA context
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t aml_hw_sha_update(sha2_ctx *cur_ctx, const uint8_t *input,
		uint32_t ilen, uint8_t *hash, uint8_t last_update);
#endif
