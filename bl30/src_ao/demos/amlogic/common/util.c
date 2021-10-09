/* Copyright (c) 2013 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Utility functions for Chrome EC */

#include "util.h"

unsigned int strlen(const char *s)
{
	int len = 0;

	while (*s++)
		len++;

	return len;
}


int isspace(int c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}


int isdigit(int c)
{
	return c >= '0' && c <= '9';
}


int isalpha(int c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int isprint(int c)
{
	return c >= ' ' && c <= '~';
}

int tolower(int c)
{
	return c >= 'A' && c <= 'Z' ? c + 'a' - 'A' : c;
}


int strcasecmp(const char *s1, const char *s2)
{
	int diff;
	do {
		diff = tolower(*s1) - tolower(*s2);
		if (diff)
			return diff;
	} while (*(s1++) && *(s2++));
	return 0;
}


int strncasecmp(const char *s1, const char *s2, size_t size)
{
	int diff;

	if (!size)
		return 0;

	do {
		diff = tolower(*s1) - tolower(*s2);
		if (diff)
			return diff;
	} while (*(s1++) && *(s2++) && --size);
	return 0;
}


int atoi(const char *nptr)
{
	int result = 0;
	int neg = 0;
	char c = '\0';

	while ((c = *nptr++) && isspace(c))
		;

	if (c == '-') {
		neg = 1;
		c = *nptr++;
	}

	while (isdigit(c)) {
		result = result * 10 + (c - '0');
		c = *nptr++;
	}

	return neg ? -result : result;
}


/* Like strtol(), but for integers */
int strtoi(const char *nptr, char **endptr, int base)
{
	int result = 0;
	int neg = 0;
	int c = '\0';

	if (endptr)
		*endptr = (char *)nptr;

	while ((c = *nptr++) && isspace(c))
		;

	if (c == '0' && *nptr == 'x') {
		base = 16;
		c = nptr[1];
		nptr += 2;
	} else if (base == 0) {
		base = 10;
		if (c == '-') {
			neg = 1;
			c = *nptr++;
		}
	}

	while (c) {
		if (c >= '0' && c < '0' + MIN(base, 10))
			result = result * base + (c - '0');
		else if (c >= 'A' && c < 'A' + base - 10)
			result = result * base + (c - 'A' + 10);
		else if (c >= 'a' && c < 'a' + base - 10)
			result = result * base + (c - 'a' + 10);
		else
			break;

		if (endptr)
			*endptr = (char *)nptr;
		c = *nptr++;
	}

	return neg ? -result : result;
}

int parse_bool(const char *s, int *dest)
{
	if (!strcasecmp(s, "off") || !strncasecmp(s, "dis", 3) ||
	    tolower(*s) == 'f' || tolower(*s) == 'n') {
		*dest = 0;
		return 1;
	} else if (!strcasecmp(s, "on") || !strncasecmp(s, "ena", 3) ||
	    tolower(*s) == 't' || tolower(*s) == 'y') {
		*dest = 1;
		return 1;
	} else {
		return 0;
	}
}

int memcmp(const void *s1, const void *s2, size_t len)
{
	const char *sa = s1;
	const char *sb = s2;

	int diff = 0;
	while (len-- > 0) {
		diff = *(sa++) - *(sb++);
		if (diff)
			return diff;
	}

	return 0;
}


void *memcpy(void *dest, const void *src, size_t len)
{
	char *d = (char *)dest;
	const char *s = (const char *)src;
	uint32_t *dw;
	const uint32_t *sw;
	char *head;
	char * const tail = (char *)dest + len;
	/* Set 'body' to the last word boundary */
	uint32_t * const body = (uint32_t *)((uintptr_t)tail & ~3);

	if (((uintptr_t)dest & 3) != ((uintptr_t)src & 3)) {
		/* Misaligned. no body, no tail. */
		head = tail;
	} else {
		/* Aligned */
		if ((uintptr_t)tail < (((uintptr_t)d + 3) & ~3))
			/* len is shorter than the first word boundary */
			head = tail;
		else
			/* Set 'head' to the first word boundary */
			head = (char *)(((uintptr_t)d + 3) & ~3);
	}

	/* Copy head */
	while (d < head)
		*(d++) = *(s++);

	/* Copy body */
	dw = (uint32_t *)d;
	sw = (uint32_t *)s;
	while (dw < body)
		*(dw++) = *(sw++);

	/* Copy tail */
	d = (char *)dw;
	s = (const char *)sw;
	while (d < tail)
		*(d++) = *(s++);

	return dest;
}


void *memset(void *dest, int c, size_t len)
{
	char *d = (char *)dest;
	uint32_t cccc;
	uint32_t *dw;
	char *head;
	char * const tail = (char *)dest + len;
	/* Set 'body' to the last word boundary */
	uint32_t * const body = (uint32_t *)((uintptr_t)tail & ~3);

	c &= 0xff;	/* Clear upper bits before ORing below */
	cccc = c | (c << 8) | (c << 16) | (c << 24);

	if ((uintptr_t)tail < (((uintptr_t)d + 3) & ~3))
		/* len is shorter than the first word boundary */
		head = tail;
	else
		/* Set 'head' to the first word boundary */
		head = (char *)(((uintptr_t)d + 3) & ~3);

	/* Copy head */
	while (d < head)
		*(d++) = c;

	/* Copy body */
	dw = (uint32_t *)d;
	while (dw < body)
		*(dw++) = cccc;

	/* Copy tail */
	d = (char *)dw;
	while (d < tail)
		*(d++) = c;

	return dest;
}


void *memmove(void *dest, const void *src, size_t len)
{
	if ((uintptr_t)dest <= (uintptr_t)src ||
	    (uintptr_t)dest >= (uintptr_t)src + len) {
		/* Start of destination doesn't overlap source, so just use
		 * memcpy(). */
		return memcpy(dest, src, len);
	} else {
		/* Need to copy from tail because there is overlap. */
		char *d = (char *)dest + len;
		const char *s = (const char *)src + len;
		uint32_t *dw;
		const uint32_t *sw;
		char *head;
		char * const tail = (char *)dest;
		/* Set 'body' to the last word boundary */
		uint32_t * const body = (uint32_t *)(((uintptr_t)tail+3) & ~3);

		if (((uintptr_t)dest & 3) != ((uintptr_t)src & 3)) {
			/* Misaligned. no body, no tail. */
			head = tail;
		} else {
			/* Aligned */
			if ((uintptr_t)tail > ((uintptr_t)d & ~3))
				/* Shorter than the first word boundary */
				head = tail;
			else
				/* Set 'head' to the first word boundary */
				head = (char *)((uintptr_t)d & ~3);
		}

		/* Copy head */
		while (d > head)
			*(--d) = *(--s);

		/* Copy body */
		dw = (uint32_t *)d;
		sw = (uint32_t *)s;
		while (dw > body)
			*(--dw) = *(--sw);

		/* Copy tail */
		d = (char *)dw;
		s = (const char *)sw;
		while (d > tail)
			*(--d) = *(--s);

		return dest;
	}
}


char *strzcpy(char *dest, const char *src, int len)
{
	char *d = dest;
	if (len <= 0)
		return dest;
	while (len > 1 && *src) {
		*(d++) = *(src++);
		len--;
	}
	*d = '\0';
	return dest;
}


int uint64divmod(uint64_t *n, int d)
{
	uint64_t q = 0, mask;
	int r = 0;

	/* Divide-by-zero returns zero */
	if (!d) {
		*n = 0;
		return 0;
	}

	/* Common powers of 2 = simple shifts */
	if (d == 2) {
		r = *n & 1;
		*n >>= 1;
		return r;
	} else if (d == 16) {
		r = *n & 0xf;
		*n >>= 4;
		return r;
	}

	/* If v fits in 32-bit, we're done. */
	if (*n <= 0xffffffff) {
		uint32_t v32 = *n;
		r = v32 % d;
		*n = v32 / d;
		return r;
	}

	/* Otherwise do integer division the slow way. */
	for (mask = (1ULL << 63); mask; mask >>= 1) {
		r <<= 1;
		if (*n & mask)
			r |= 1;
		if (r >= d) {
			r -= d;
			q |= mask;
		}
	}
	*n = q;
	return r;
}

int __clzsi2(int x)
{
	int r = 0;

	if (!x)
		return 32;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r += 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r += 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r += 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r += 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r += 1;
	}
	return r;
}

int get_next_bit(uint32_t *mask)
{
	int bit = 31 - __clzsi2(*mask);
	*mask &= ~(1 << bit);
	return bit;
}

char *strncpy (char *s1, const char *s2, size_t n)
{
	char c;
	char *s = s1;

	--s1;

	if (n >= 4)
	{
		size_t n4 = n >> 2;

		for (;;)
		{
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
			break;
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
			break;
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
			break;
			c = *s2++;
			*++s1 = c;
			if (c == '\0')
			break;
			if (--n4 == 0)
			goto last_chars;
		}
		n = n - (s1 - s) - 1;
		if (n == 0)
			return s;
		goto zero_fill;
	}

	last_chars:
	n &= 3;
	if (n == 0)
		return s;

	do
	{
		c = *s2++;
		*++s1 = c;
		if (--n == 0)
		return s;
	}
	while (c != '\0');

	zero_fill:
	do
		*++s1 = '\0';
	while (--n > 0);

	return s;
}


