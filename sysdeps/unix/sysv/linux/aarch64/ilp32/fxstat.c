//#include <sysdeps/unix/sysv/linux/generic/wordsize-32/fxstat.c>
/* Copyright (C) 2011-2016 Free Software Foundation, Inc.
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

#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <kernel_stat.h>

#include <sysdep.h>
#include <sys/syscall.h>

#include "overflow.h"

/* Get information about the file FD in BUF.  */
int
__fxstat (int vers, int fd, struct stat *buf)
{
  if (vers == _STAT_VER_KERNEL)
    {
      int rc = INLINE_SYSCALL (fstat64, 2, fd, buf);
      if (rc)
	      return rc;

      conv_timespec(&buf->st_atim, &buf->__st_atim);
      conv_timespec(&buf->st_mtim, &buf->__st_mtim);
      conv_timespec(&buf->st_ctim, &buf->__st_ctim);

      return stat_overflow (buf);
    }

  errno = EINVAL;
  return -1;
}

hidden_def (__fxstat)
weak_alias (__fxstat, _fxstat);
