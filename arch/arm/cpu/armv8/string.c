// SPDX-License-Identifier: GPL-2.0+

#include <common.h>

#include <asm/sections.h>
#include <linux/compiler.h>
#include <linux/err.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef __HAVE_ARCH_MEMSET
void *memset(void *s, int c, size_t count)
{
	unsigned long *sl = (unsigned long *) s;
	char *s8;

	if (gd->flags & GD_FLG_CACHE_EN)
		return __memset_cache(s, c, count);

#if !defined TINY_MEMSET
	unsigned long cl = 0;
	int i;

	/* do it one word at a time (32 bits or 64 bits) while possible */
	if ( ((ulong)s & (sizeof(*sl) - 1)) == 0) {
		for (i = 0; i < sizeof(*sl); i++) {
			cl <<= 8;
			cl |= c & 0xff;
		}
		while (count >= sizeof(*sl)) {
			*sl++ = cl;
			count -= sizeof(*sl);
		}
	}
#endif	/* fill 8 bits at a time */
	s8 = (char *)sl;
	while (count--)
		*s8++ = c;

	return s;
}
#endif

#ifdef __HAVE_ARCH_MEMCMP
/**
 * memcmp - Compare two areas of memory
 * @cs: One area of memory
 * @ct: Another area of memory
 * @count: The size of the area.
 */
int memcmp(const void *cs, const void *ct, size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	/* fast version after cache enable */
	if (gd->flags & GD_FLG_CACHE_EN) {
		return __memcmp(cs, ct, count);
	}

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}
#endif

#ifdef __HAVE_ARCH_STRLEN
/**
 * strlen - Find the length of a string
 * @s: The string to be sized
 */
size_t strlen(const char *s)
{
	const char *sc;

	/* fast version after cache enable */
	if (gd->flags & GD_FLG_CACHE_EN)
		return __strlen(s);

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}
#endif

#ifdef __HAVE_ARCH_STRCMP
/**
 * strcmp - Compare two strings
 * @cs: One string
 * @ct: Another string
 */
int strcmp(const char *cs, const char *ct)
{
	register signed char __res;

	/* fast version after cache enable */
	if (gd->flags & GD_FLG_CACHE_EN)
		return __strcmp(cs, ct);

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}
#endif

#ifdef __HAVE_ARCH_MEMCPY
/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
void *memcpy(void *dest, const void *src, size_t count)
{
	unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
	char *d8, *s8;

	if (src == dest)
		return dest;

	/* fast version after cache enable */
	if (gd->flags & GD_FLG_CACHE_EN)
		return __memcpy_cache(dest, src, count);

	/* while all data is aligned (common case), copy a word at a time */
	if ((((ulong)dest | (ulong)src) & (sizeof(*dl) - 1)) == 0) {
		while (count >= sizeof(*dl)) {
			*dl++ = *sl++;
			count -= sizeof(*dl);
		}
	}
	/* copy the reset one byte at a time */
	d8 = (char *)dl;
	s8 = (char *)sl;
	while (count--)
		*d8++ = *s8++;

	return dest;
}
#endif

#ifdef __HAVE_ARCH_MEMMOVE
/**
 * memmove - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * Unlike memcpy(), memmove() copes with overlapping areas.
 */
void *memmove(void *dest, const void *src, size_t count)
{
	char *tmp, *s;

	/* fast version after cache enable */
	if (gd->flags & GD_FLG_CACHE_EN)
		return __memmove_cache(dest, src, count);

	if (dest <= src) {
		memcpy(dest, src, count);
	} else {
		tmp = (char *)dest + count;
		s = (char *)src + count;
		while (count--)
			*--tmp = *--s;
	}

	return dest;
}
#endif
