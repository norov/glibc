/* A Generic Optimized memset implementation for AARCH64.
   Copyright (C) 2013-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <sysdep.h>

#if defined SHARED && IS_IN (libc)

#undef ENTRY_ALIGN
#define ENTRY_ALIGN(name, align)				\
  .globl C_SYMBOL_NAME(__memset_generic);			\
  .type C_SYMBOL_NAME(__memset_generic),%function;		\
  .p2align align;						\
  C_LABEL(__memset_generic)					\
  cfi_startproc;						\
  CALL_MCOUNT

#undef END

#define END(name)						\
  cfi_endproc;							\
  ASM_SIZE_DIRECTIVE(__memset_generic)


#undef libc_hidden_builtin_def
#define libc_hidden_builtin_def(name)

#endif

#include "../memset.S"
