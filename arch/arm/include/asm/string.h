#ifndef __ASM_ARM_STRING_H
#define __ASM_ARM_STRING_H

#include <config.h>

/*
 * We don't do inline string functions, since the
 * optimised inline asm versions are not small.
 */

#undef __HAVE_ARCH_STRRCHR
extern char * strrchr(const char * s, int c);

#undef __HAVE_ARCH_STRCHR
extern char * strchr(const char * s, int c);

#if CONFIG_IS_ENABLED(USE_ARCH_MEMCPY)
#define __HAVE_ARCH_MEMCPY
#endif
extern void * memcpy(void *, const void *, __kernel_size_t);

#undef __HAVE_ARCH_MEMMOVE
extern void * memmove(void *, const void *, __kernel_size_t);

#undef __HAVE_ARCH_MEMCHR
extern void * memchr(const void *, int, __kernel_size_t);

#undef __HAVE_ARCH_MEMZERO
#if CONFIG_IS_ENABLED(USE_ARCH_MEMSET)
#define __HAVE_ARCH_MEMSET
#endif
extern void * memset(void *, int, __kernel_size_t);

#if 0
extern void __memzero(void *ptr, __kernel_size_t n);

#define memset(p,v,n)							\
	({								\
		if ((n) != 0) {						\
			if (__builtin_constant_p((v)) && (v) == 0)	\
				__memzero((p),(n));			\
			else						\
				memset((p),(v),(n));			\
		}							\
		(p);							\
	})

#define memzero(p,n) ({ if ((n) != 0) __memzero((p),(n)); (p); })
#else
extern void memzero(void *ptr, __kernel_size_t n);
#endif

void *__memset_cache(void *a, int b, __kernel_size_t c);
void *__memcpy_cache(void *a, const void *b, __kernel_size_t c);
void *__memmove_cache(void *a, const void *b, __kernel_size_t c);
int __memcmp(const void *cs, const void *ct, size_t count);
__kernel_size_t __strlen(const char *a);
int __strcmp(const char *a, const char *b);

#ifndef CONFIG_AML_UASAN
#ifdef CONFIG_ARM64
#ifndef __HAVE_ARCH_MEMCPY
#define __HAVE_ARCH_MEMCPY
void *memcpy(void *, const void *, __kernel_size_t);
#endif
#ifndef __HAVE_ARCH_MEMMOVE
#define __HAVE_ARCH_MEMMOVE
void *memmove(void *, const void *, __kernel_size_t);
#endif
#ifndef __HAVE_ARCH_MEMSET
#define __HAVE_ARCH_MEMSET
void *memset(void *, int, __kernel_size_t);
void *memset_non_cache(void *, int, __kernel_size_t);
#endif
#ifndef __HAVE_ARCH_MEMCMP
#define __HAVE_ARCH_MEMCMP
int memcmp(const void *, const void *, __kernel_size_t);
#endif
#ifndef __HAVE_ARCH_STRLEN
#define __HAVE_ARCH_STRLEN
__kernel_size_t strlen(const char *);
#endif
#ifndef __HAVE_ARCH_STRNLEN
#define __HAVE_ARCH_STRNLEN
__kernel_size_t strnlen(const char *, __kernel_size_t);
#endif
#ifndef __HAVE_ARCH_STRCMP
#define __HAVE_ARCH_STRCMP
int strcmp(const char *, const char *);
#endif
#ifndef __HAVE_ARCH_STRNCMP
#define __HAVE_ARCH_STRNCMP
int strncmp(const char *, const char *, __kernel_size_t);
#endif
#endif
#endif

#endif
