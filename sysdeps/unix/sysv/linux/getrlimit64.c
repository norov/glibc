/* Copyright (C) 2010-2016 Free Software Foundation, Inc.
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

#define __getrlimit __getrlimit_undefined
#define getrlimit getrlimit_undefined

#include <errno.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sysdep.h>
#include <kernel-features.h>

#undef __getrlimit
#undef getrlimit

/* Put the soft and hard limits for RESOURCE in *RLIMITS.
   Returns 0 if successful, -1 if not (and sets errno).  */
int
__getrlimit64 (enum __rlimit_resource resource, struct rlimit64 *rlimits)
{
#ifdef __ASSUME_PRLIMIT64
  return INLINE_SYSCALL (prlimit64, 4, 0, resource, NULL, rlimits);
#else
# ifdef __NR_prlimit64
  int res = INLINE_SYSCALL (prlimit64, 4, 0, resource, NULL, rlimits);
  if (res == 0 || errno != ENOSYS)
    return res;
# endif

# if __RLIM_MATCHES_RLIM64
#  define rlimits32 (*rlimits)
# else
  struct rlimit rlimits32;
# endif

  if (INLINE_SYSCALL (getrlimit, 2, resource, &rlimits32) < 0)
    return -1;

# if !__RLIM_MATCHES_RLIM64
  if (rlimits32.rlim_cur == RLIM_INFINITY)
    rlimits->rlim_cur = RLIM64_INFINITY;
  else
    rlimits->rlim_cur = rlimits32.rlim_cur;
  if (rlimits32.rlim_max == RLIM_INFINITY)
    rlimits->rlim_max = RLIM64_INFINITY;
  else
    rlimits->rlim_max = rlimits32.rlim_max;
# endif

  return 0;

#endif
}
libc_hidden_def (__getrlimit64)
#ifndef getrlimit64
weak_alias (__getrlimit64, getrlimit64)
libc_hidden_weak (getrlimit64)
#endif

#if __RLIM_MATCHES_RLIM64
strong_alias (__getrlimit64, __getrlimit)

/* __getrlimit and getrlimit are made undefined to bypass type control,
   but the side effect of it is that libc_hidden_proto() doesn't work
   for that symbols correctly, So they are declared manually.  */
extern __typeof (__getrlimit64) __EI___getrlimit __asm__("__getrlimit");
extern __typeof (__getrlimit64) __getrlimit __asm__ ("__GI___getrlimit") __attribute__ ((visibility ("hidden")));
extern __typeof (__getrlimit64) __EI_getrlimit __asm__("getrlimit");
extern __typeof (__getrlimit64) getrlimit __asm__ ("__GI_getrlimit") __attribute__ ((visibility ("hidden")));

/* libc_hidden_weak (__getrlimit) declares weak symbol which is not global,
   but we need global one, so do it manually.  */
extern __typeof (__getrlimit) __EI___getrlimit __asm__("" "__getrlimit");
extern __typeof (__getrlimit) __EI___getrlimit __attribute__((alias ("__GI___getrlimit")));
weak_alias (__getrlimit64, getrlimit)
libc_hidden_weak (getrlimit)
#endif
