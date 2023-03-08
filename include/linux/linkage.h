/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * U-Boot - linkage.h
 *
 * Copyright (c) 2005-2007 Analog Devices Inc.
 */

#ifndef _LINUX_LINKAGE_H
#define _LINUX_LINKAGE_H

#include <asm/linkage.h>

/* Some toolchains use other characters (e.g. '`') to mark new line in macro */
#ifndef ASM_NL
#define ASM_NL		 ;
#endif

#ifdef __cplusplus
#define CPP_ASMLINKAGE		extern "C"
#else
#define CPP_ASMLINKAGE
#endif

#ifndef asmlinkage
#define asmlinkage CPP_ASMLINKAGE
#endif

#define SYMBOL_NAME_STR(X)	#X
#define SYMBOL_NAME(X)		X
#ifdef __STDC__
#define SYMBOL_NAME_LABEL(X)	X##:
#else
#define SYMBOL_NAME_LABEL(X)	X:
#endif

#ifndef __ALIGN
#define __ALIGN .align		4
#endif

#ifndef __ALIGN_STR
#define __ALIGN_STR		".align 4"
#endif

#ifdef __ASSEMBLY__

#define ALIGN			__ALIGN
#define ALIGN_STR		__ALIGN_STR

/* SYM_T_FUNC -- type used by assembler to mark functions */
#ifndef SYM_T_FUNC
#define SYM_T_FUNC				STT_FUNC
#endif

/* SYM_T_OBJECT -- type used by assembler to mark data */
#ifndef SYM_T_OBJECT
#define SYM_T_OBJECT				STT_OBJECT
#endif

/* SYM_T_NONE -- type used by assembler to mark entries of unknown type */
#ifndef SYM_T_NONE
#define SYM_T_NONE				STT_NOTYPE
#endif

/* SYM_A_* -- align the symbol? */
#define SYM_A_ALIGN				ALIGN
#define SYM_A_NONE				/* nothing */

/* SYM_L_* -- linkage of symbols */
#define SYM_L_GLOBAL(name)			.globl name
#define SYM_L_WEAK(name)			.weak name
#define SYM_L_LOCAL(name)			/* nothing */

#define LENTRY(name) \
	ALIGN ASM_NL \
	SYMBOL_NAME_LABEL(name)

#define ENTRY(name) \
	.globl SYMBOL_NAME(name) ASM_NL \
	LENTRY(name)

#define WEAK(name) \
	.weak SYMBOL_NAME(name) ASM_NL \
	LENTRY(name)

#ifndef END
#define END(name) \
	.size name, .-name
#endif

#ifndef ENDPROC
#define ENDPROC(name) \
	.type name STT_FUNC ASM_NL \
	END(name)
#endif

/* === generic annotations === */
/* SYM_ENTRY -- use only if you have to for non-paired symbols */
#ifndef SYM_ENTRY
#define SYM_ENTRY(name, linkage, align...)		\
	linkage(name) ASM_NL				\
	align ASM_NL					\
name :
#endif

/* SYM_START -- use only if you have to */
#ifndef SYM_START
#define SYM_START(name, linkage, align...)		\
	SYM_ENTRY(name, linkage, align)
#endif

/* SYM_END -- use only if you have to */
#ifndef SYM_END
#define SYM_END(name, sym_type)				\
	.type name sym_type ASM_NL			\
	.size name, . -name
#endif

/* SYM_FUNC_START -- use for global functions */
#ifndef SYM_FUNC_START
/*
 * The same as SYM_FUNC_START_ALIAS, but we will need to distinguish these two
 * later.
 */
#define SYM_FUNC_START(name)				\
	SYM_START(name, SYM_L_GLOBAL, SYM_A_ALIGN)
#endif

/* SYM_FUNC_START_WEAK -- use for weak functions */
#ifndef SYM_FUNC_START_WEAK
#define SYM_FUNC_START_WEAK(name)			\
	SYM_START(name, SYM_L_WEAK, SYM_A_ALIGN)
#endif

/* SYM_FUNC_END_ALIAS -- the end of LOCAL_ALIASed or ALIASed function */
#ifndef SYM_FUNC_END_ALIAS
#define SYM_FUNC_END_ALIAS(name)			\
	SYM_END(name, SYM_T_FUNC)
#endif

/*
 * SYM_FUNC_END -- the end of SYM_FUNC_START_LOCAL, SYM_FUNC_START,
 * SYM_FUNC_START_WEAK, ...
 */
#ifndef SYM_FUNC_END
/* the same as SYM_FUNC_END_ALIAS, see comment near SYM_FUNC_START */
#define SYM_FUNC_END(name)				\
	SYM_END(name, SYM_T_FUNC)
#endif

/* SYM_CODE_START -- use for non-C (special) functions */
#ifndef SYM_CODE_START
#define SYM_CODE_START(name)				\
	SYM_START(name, SYM_L_GLOBAL, SYM_A_ALIGN)
#endif

/* SYM_CODE_START_NOALIGN -- use for non-C (special) functions, w/o alignment */
#ifndef SYM_CODE_START_NOALIGN
#define SYM_CODE_START_NOALIGN(name)			\
	SYM_START(name, SYM_L_GLOBAL, SYM_A_NONE)
#endif

/* SYM_CODE_START_LOCAL -- use for local non-C (special) functions */
#ifndef SYM_CODE_START_LOCAL
#define SYM_CODE_START_LOCAL(name)			\
	SYM_START(name, SYM_L_LOCAL, SYM_A_ALIGN)
#endif

/*
 * SYM_CODE_START_LOCAL_NOALIGN -- use for local non-C (special) functions,
 * w/o alignment
 */
#ifndef SYM_CODE_START_LOCAL_NOALIGN
#define SYM_CODE_START_LOCAL_NOALIGN(name)		\
	SYM_START(name, SYM_L_LOCAL, SYM_A_NONE)
#endif

/* SYM_CODE_END -- the end of SYM_CODE_START_LOCAL, SYM_CODE_START, ... */
#ifndef SYM_CODE_END
#define SYM_CODE_END(name)				\
	SYM_END(name, SYM_T_NONE)
#endif

#endif

#endif
