/* Linux getrlimit implementation (32 bits rlim_t).
   Copyright (C) 2016 Free Software Foundation, Inc.
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
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <shlib-compat.h>

#ifndef __RLIM_T_MATCHES_RLIM64_T

/* The __NR_getrlimit compatibility implementation is required iff
   __NR_getrlimit is also defined (meaning an old broken RLIM_INFINITY
   definition).  */
# ifndef __NR_ugetrlimit
#  define __NR_ugetrlimit __NR_getrlimit
#  undef SHLIB_COMPAT
#  define SHLIB_COMPAT(a, b, c) 0
# endif

int
__new_getrlimit (enum __rlimit_resource resource, struct rlimit *rlim)
{
# ifdef __NR_prlimit64
  struct rlimit64 rlim64;
  int res = INLINE_SYSCALL_CALL (prlimit64, 0, resource, NULL, &rlim64);
  if (res == 0)
    {
      rlim->rlim_cur = rlim64.rlim_cur;
      if (rlim->rlim_cur != rlim64.rlim_cur)
	rlim->rlim_cur = RLIM_INFINITY;
      rlim->rlim_max = rlim64.rlim_max;
      if (rlim->rlim_max != rlim64.rlim_max)
        rlim->rlim_max = RLIM_INFINITY;
    }
  if (res == 0 || errno != ENOSYS)
    return res;
# endif
  return INLINE_SYSCALL_CALL (ugetrlimit, resource, rlim);
}
weak_alias (__new_getrlimit, __getrlimit)
hidden_weak (__getrlimit)

# if SHLIB_COMPAT (libc, GLIBC_2_0, GLIBC_2_2)
/* Back compatible 2Gig limited rlimit.  */
int
__old_getrlimit (enum __rlimit_resource resource, struct rlimit *rlim)
{
  return INLINE_SYSCALL_CALL (getrlimit, resource, rlim);
}
compat_symbol (libc, __old_getrlimit, getrlimit, GLIBC_2_0);
versioned_symbol (libc, __new_getrlimit, getrlimit, GLIBC_2_2);
# else
weak_alias (__new_getrlimit, getrlimit)
# endif

#endif /* __RLIM_T_MATCHES_RLIM64_T  */
