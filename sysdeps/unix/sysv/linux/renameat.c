/* renameat() syscall
   Copyright (C) 2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Chris Metcalf <cmetcalf@tilera.com>, 2011.

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

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sysdep.h>

#if !defined (__NR_renameat) && !defined (__NR_renameat2)
# error "Both renameat and renameat2 not supported"
#endif

/* Rename the file OLD to NEW.  */
int
renameat (int oldfd, const char *old, int newfd, const char *new)
{
#ifdef __NR_renameat
  return INLINE_SYSCALL_CALL (renameat, oldfd, old, newfd, new);
#else
  return INLINE_SYSCALL_CALL (renameat2, oldfd, old, newfd, new, 0);
#endif
}
