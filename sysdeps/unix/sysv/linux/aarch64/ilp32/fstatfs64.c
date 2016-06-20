/* Return information about the filesystem on which FD resides.
   Copyright (C) 1996-2016 Free Software Foundation, Inc.
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
#define __fstatfs __statfs_disable
#define fstatfs statfs_disable

#include <errno.h>
#include <sys/statfs.h>
#include <stddef.h>

int
__fstatfs64 (int fd, struct statfs64 *buf)
{
  return INLINE_SYSCALL (fstatfs64, 2, fd, buf);
}

#undef __fstatfs
#undef fstatfs
strong_alias (__fstatfs64, __fstatfs)
weak_alias (__fstatfs64, fstatfs64)
weak_alias (__fstatfs64, fstatfs)

