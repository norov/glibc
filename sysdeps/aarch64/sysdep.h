/* Copyright (C) 1997-2015 Free Software Foundation, Inc.

   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _AARCH64_SYSDEP_H
#define _AARCH64_SYSDEP_H

#include <sysdeps/generic/sysdep.h>

#ifdef __LP64__
#define AARCH64_R(NAME)		R_AARCH64_ ## NAME
#define PTR_REG(n)	x##n
#define PTR_LOG_SIZE	3
#else
#define AARCH64_R(NAME)		R_AARCH64_P32_ ## NAME
#define PTR_REG(n)	w##n
#define PTR_LOG_SIZE	2
#endif

#define PTR_SIZE	(1<<PTR_LOG_SIZE)

#ifdef	__ASSEMBLER__

/* Syntactic details of assembler.  */

#define ASM_SIZE_DIRECTIVE(name) .size name,.-name

/* Define an entry point visible from C.  */
#define ENTRY(name)						\
  .globl C_SYMBOL_NAME(name);					\
  .type C_SYMBOL_NAME(name),%function;				\
  .align 4;							\
  C_LABEL(name)							\
  cfi_startproc;						\
  CALL_MCOUNT

/* Define an entry point visible from C.  */
#define ENTRY_ALIGN(name, align)				\
  .globl C_SYMBOL_NAME(name);					\
  .type C_SYMBOL_NAME(name),%function;				\
  .p2align align;						\
  C_LABEL(name)							\
  cfi_startproc;						\
  CALL_MCOUNT

/* Define an entry point visible from C with a specified alignment and
   pre-padding with NOPs.  This can be used to ensure that a critical
   loop within a function is cache line aligned.  Note this version
   does not adjust the padding if CALL_MCOUNT is defined. */

#define ENTRY_ALIGN_AND_PAD(name, align, padding)		\
  .globl C_SYMBOL_NAME(name);					\
  .type C_SYMBOL_NAME(name),%function;				\
  .p2align align;						\
  .rep padding;							\
  nop;								\
  .endr;							\
  C_LABEL(name)							\
  cfi_startproc;						\
  CALL_MCOUNT

#undef	END
#define END(name)						\
  cfi_endproc;							\
  ASM_SIZE_DIRECTIVE(name)

/* If compiled for profiling, call `mcount' at the start of each function.  */
#ifdef	PROF
# define CALL_MCOUNT						\
	str	x30, [sp, #-16]!;				\
	bl	mcount;						\
	ldr	x30, [sp], #16	;
#else
# define CALL_MCOUNT		/* Do nothing.  */
#endif

/* Local label name for asm code.  */
#ifndef L
# define L(name)         .L##name
#endif

/* Load or store to/from a pc-relative EXPR into/from R, using T.
   Note R and T are register numbers and not register names.  */
#define LDST_PCREL(OP, R, T, EXPR)			\
	adrp	x##T, EXPR;				\
	OP	PTR_REG (R), [x##T, #:lo12:EXPR];	\

/* Load or store to/from a got-relative EXPR into/from R, using T.
   Note R and T are register numbers and not register names.  */
#define LDST_GLOBAL(OP, R, T,  EXPR)			\
	adrp	x##T, :got:EXPR;			\
	ldr	PTR_REG (T), [x##T, #:got_lo12:EXPR];	\
	OP	x##R, [x##T];

/* Since C identifiers are not normally prefixed with an underscore
   on this system, the asm identifier `syscall_error' intrudes on the
   C name space.  Make sure we use an innocuous name.  */
#define syscall_error	__syscall_error
#define mcount		_mcount

#endif	/* __ASSEMBLER__ */

#endif  /* _AARCH64_SYSDEP_H */
