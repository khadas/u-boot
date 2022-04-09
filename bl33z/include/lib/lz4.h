#ifndef __LZ4_H__
#define __LZ4_H__
/*
 * LZ4 Kernel Interface
 *
 * Copyright (C) 2013, LG Electronics, Kyungsik Lee <kyungsik.lee@lge.com>
 *
 */
#define LZ4_MEM_COMPRESS	(16384)
#define LZ4HC_MEM_COMPRESS	(262144 + (2 * sizeof(unsigned char *)))

/*
 * lz4_compressbound()
 * Provides the maximum size that LZ4 may output in a "worst case" scenario
 * (input data not compressible)
 */
static inline size_t lz4_compressbound(size_t isize)
{
	return isize + (isize / 255) + 16;
}

/*
 * lz4_compress()
 *	src     : source address of the original data
 *	src_len : size of the original data
 *	dst	: output buffer address of the compressed data
 *		This requires 'dst' of size LZ4_COMPRESSBOUND.
 *	dst_len : is the output size, which is returned after compress done
 *	workmem : address of the working memory.
 *		This requires 'workmem' of size LZ4_MEM_COMPRESS.
 *	return  : Success if return 0
 *		  Error if return (< 0)
 *	note :  Destination buffer and workmem must be already allocated with
 *		the defined size.
 */
int lz4_compress(const unsigned char *src, size_t src_len,
		unsigned char *dst, size_t *dst_len, void *wrkmem);

 /*
  * lz4hc_compress()
  *	 src	 : source address of the original data
  *	 src_len : size of the original data
  *	 dst	 : output buffer address of the compressed data
  *		This requires 'dst' of size LZ4_COMPRESSBOUND.
  *	 dst_len : is the output size, which is returned after compress done
  *	 workmem : address of the working memory.
  *		This requires 'workmem' of size LZ4HC_MEM_COMPRESS.
  *	 return  : Success if return 0
  *		   Error if return (< 0)
  *	 note :  Destination buffer and workmem must be already allocated with
  *		 the defined size.
  */
int lz4hc_compress(const unsigned char *src, size_t src_len,
		unsigned char *dst, size_t *dst_len, void *wrkmem);

#endif
