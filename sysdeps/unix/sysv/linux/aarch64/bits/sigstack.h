/* sigstack, sigaltstack definitions.
   Copyright (C) 1998, 1999 Free Software Foundation, Inc.
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

#ifndef _SIGNAL_H
# error "Never include this file directly.  Use <signal.h> instead"
#endif


/* Structure describing a signal stack (obsolete).  */
struct sigstack
  {
    void *ss_sp;		/* Signal stack pointer.  */
    int ss_onstack;		/* Nonzero if executing on this stack.  */
  };


/* Possible values for `ss_flags.'.  */
enum
{
  SS_ONSTACK = 1,
#define SS_ONSTACK	SS_ONSTACK
  SS_DISABLE
#define SS_DISABLE	SS_DISABLE
};

/* Minimum stack size for a signal handler.  */
#define MINSIGSTKSZ    5120

/* System default stack size.  */
#define SIGSTKSZ       16384

/* Alternate, preferred interface.
   This structure matches the same size and layout
   for both ILP32 and LP64.  */
typedef struct sigaltstack
  {
#if defined(__ILP32__) && defined(__AARCH64EB__)
    int __pad_ss_sp;
#endif
    void *ss_sp;
#if defined(__ILP32__) && !defined(__AARCH64EB__)
    int __pad_ss_sp;
#endif
    int ss_flags;
#if defined(__ILP32__)
    int __pad_after_ss_flags;
#endif
#if defined(__ILP32__) && defined(__AARCH64EB__)
    int __pad_ss_size;
#endif
    size_t ss_size;
#if defined(__ILP32__) && !defined(__AARCH64EB__)
    int __pad_ss_size;
#endif
  } stack_t;
