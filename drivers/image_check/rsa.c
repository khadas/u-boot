// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <amlogic/image_check.h>

/**
 * a[] -= mod
 */
static void sub_mod(const struct rsa_public_key *key, u32 *a)
{
	s64 A = 0;
	u32 i;

	for (i = 0; i < key->len; ++i) {
		A += (u64)a[i] - key->n[i];
		a[i] = (u32)A;
		A >>= 32;
	}
}

/**
 * Return a[] >= mod
 */
static int ge_mod(const struct rsa_public_key *key, const u32 *a)
{
	u32 i;

	for (i = key->len; i;) {
		--i;
		if (a[i] < key->n[i])
			return 0;
		if (a[i] > key->n[i])
			return 1;
	}
	return 1;  /* equal */
}

/**
 * Montgomery c[] += a * b[] / R % mod
 */
static void mont_mul_add(const struct rsa_public_key *key,
			 u32 *c,
			 const u32 a,
			 const u32 *b)
{
	u64 A = (u64)a * b[0] + c[0];
	u32 d0 = (u32)A * key->n0inv;
	u64 B = (u64)d0 * key->n[0] + (u32)A;
	u32 i;

	for (i = 1; i < key->len; ++i) {
		A = (A >> 32) + (u64)a * b[i] + c[i];
		B = (B >> 32) + (u64)d0 * key->n[i] + (u32)A;
		c[i - 1] = (u32)B;
	}

	A = (A >> 32) + (B >> 32);

	c[i - 1] = (u32)A;

	if (A >> 32)
		sub_mod(key, c);
}

/**
 * Montgomery c[] = a[] * b[] / R % mod
 */
static void mont_mul(const struct rsa_public_key *key,
		     u32 *c,
		     const u32 *a,
		     const u32 *b)
{
	u32 i;

	for (i = 0; i < key->len; ++i)
		c[i] = 0;

	for (i = 0; i < key->len; ++i)
		mont_mul_add(key, c, a[i], b);
}

/**
 * In-place public exponentiation.
 *
 * @param key		Key to use in signing
 * @param inout		Input and output big-endian byte array
 * @param workbuf32	Work buffer; caller must verify this is
 *			4 x RSANUMWORDS elements long.
 */
static void mod_pow_F4(const struct rsa_public_key *key, u8 *inout,
		    u32 *workbuf32)
{
	u32 *a = workbuf32;
	u32 *aa = a + key->len;
	u32 *aaa = aa + key->len;
	u32 *m_r, *a_r, *aa_r;
	s32 i, len = 0;

	/* Convert from big endian byte array to little endian word array. */
	for (i = 0; i < key->len; ++i) {
		u32 tmp =
			(inout[((key->len - 1 - i) * 4) + 0] << 24) |
			(inout[((key->len - 1 - i) * 4) + 1] << 16) |
			(inout[((key->len - 1 - i) * 4) + 2] << 8) |
			(inout[((key->len - 1 - i) * 4) + 3] << 0);
		a[i] = tmp;
	}

	/* Count length of exponent */
	i = key->e;
	while (i > 0) {
		len++;
		i >>= 1;
	}

	/* Setup a_r and aa_r for first mont_mul operation */
	m_r = aaa;
	a_r = m_r;
	aa_r = aa;
	mont_mul(key, m_r, a, key->rr);  /* m_r = a * RR / R mod M */
	for (i = len - 2; i >= 0; i--) {
		mont_mul(key, aa_r, a_r, a_r); /* aa_r = a_r * a_r / R mod M */
		/* Only happens in first loop where a_r still points to m_r */
		if (a_r == m_r)
			a_r = a;
		if (key->e & (1 << i)) {
			mont_mul(key, a_r, aa_r, m_r);
		} else {
			/* Swap aa_r and a_r to avoid memcpy */
			aaa = a_r;
			a_r = aa_r;
			aa_r = aaa;
		}
	}

	/* Convert n-residue a_r to ordinary residue */
	memset(m_r, 0, key->len * 4);
	m_r[0] = 0x1;
	mont_mul(key, aa_r, a_r, m_r); /* aa_r = a_r * 1 / R mod M */
	aaa = aa_r;

	/* Make sure aaa < mod; aaa is at most 1x mod too large. */
	while (ge_mod(key, aaa))
		sub_mod(key, aaa);

	/* Convert to bigendian byte array */
	for (i = key->len - 1; i >= 0; --i) {
		u32 tmp = aaa[i];
		*inout++ = (u8)(tmp >> 24);
		*inout++ = (u8)(tmp >> 16);
		*inout++ = (u8)(tmp >>  8);
		*inout++ = (u8)(tmp >>  0);
	}
}

/*
 * PKCS#1 padding (from the RSA PKCS#1 v2.1 standard)
 *
 * The DER-encoded padding is defined as follows :
 * 0x00 || 0x01 || PS || 0x00 || T
 *
 * T: DER Encoded DigestInfo value which depends on the hash function used,
 * for SHA-256:
 * (0x)30 31 30 0d 06 09 60 86 48 01 65 03 04 02 01 05 00 04 20 || H.
 *
 * Length(T) = 51 octets for SHA-256
 *
 * PS: octet string consisting of {Length(RSA Key) - Length(T) - 3} 0xFF
 */
static const u8 sha256_tail[] = {
	0x00, 0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60,
	0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01,
	0x05, 0x00, 0x04, 0x20
};

//#define PKCS_PAD_SIZE (RSANUMBYTES - SHA256_DIGEST_SIZE)

/**
 * Check PKCS#1 padding bytes
 *
 * @param sig  Signature to verify
 * @param pkcs_pad_size  Length of padding
 * @return 0 if the padding is correct.
 */
static int check_padding(const u8 *sig, u32 pkcs_pad_size)
{
	u8 *ptr = (u8 *)sig;
	int result = 0;
	int i;

	/* First 2 bytes are always 0x00 0x01 */
	result |= *ptr++ ^ 0x00;
	result |= *ptr++ ^ 0x01;

	/* Then 0xff bytes until the tail */
	for (i = 0; i < pkcs_pad_size - sizeof(sha256_tail) - 2; i++)
		result |= *ptr++ ^ 0xff;

	/* Check the tail. */
	result |= memcmp(ptr, sha256_tail, sizeof(sha256_tail));

	return !!result;
}

/*
 * Verify a SHA256WithRSA PKCS#1 v1.5 signature against an expected
 * SHA256 hash.
 *
 * @param key           RSA public key
 * @param signature     RSA signature
 * @param sha           SHA-256 digest of the content to verify
 * @param workbuf32     Work buffer; caller must verify this is
 *                      3 x RSANUMWORDS elements long.
 * @return 0 on failure, 1 on success.
 */

int image_rsa_verify(const struct rsa_public_key *key, const u8 *signature,
	       const u8 *sha, u32 *workbuf32)
{
	u8 buf[MAX_RSANUMBYTES];
	u32 pkcs_pad_size;
	u32 key_bits = key->len * sizeof(u32) * 8;
	u32 key_bytes = key->len * sizeof(u32);

	/* Check key size */
	if (key_bits != 1024 && key_bits != 2048 && key_bits != 4096)
		return 0;

	/* Copy input to local workspace. */
	memcpy(buf, signature, key_bytes);
	mod_pow_F4(key, buf, workbuf32); /* In-place exponentiation. */

	/* Check the PKCS#1 padding */
	pkcs_pad_size = key_bytes - SHA256_DIGEST_SIZE;
	if (check_padding(buf, pkcs_pad_size) != 0)
		return 0;

	/* Check the digest. */
	if (memcmp(buf + pkcs_pad_size, sha, SHA256_DIGEST_SIZE) != 0)
		return 0;

	return 1;  /* All checked out OK. */
}
