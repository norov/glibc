/* __fxstatat64 () implementation.
   Copyright (C) 2011-2016 Free Software Foundation, Inc.
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
#define __fxstatat __fxstatat_disable

#include <sys/stat.h>
#undef _STAT_VER_LINUX
#define _STAT_VER_LINUX _STAT_VER_KERNEL

#include <sysdeps/unix/sysv/linux/fxstatat64.c>

#undef __fxstatat
#ifdef XSTAT_IS_XSTAT64
weak_alias (__fxstatat64, __fxstatat)
libc_hidden_ver (__fxstatat64, __fxstatat)
#endif
