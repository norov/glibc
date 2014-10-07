/* Copyright (C) 2007-2015 Free Software Foundation, Inc.
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

#include <dirent.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <bits/wordsize.h>

#include <sysdep.h>
#include <sys/syscall.h>
#include <linux/posix_types.h>

struct kernel_dirent64
  {
    uint64_t		d_ino;
    int64_t		d_off;
    unsigned short int	d_reclen;
    unsigned char	d_type;
    char		d_name[256];
  };

static size_t conv (char *buf, size_t nbytes)
{
	char *end = buf + nbytes;
	struct dirent *u = (struct dirent *) buf;
	struct kernel_dirent64 *k = (struct kernel_dirent64 *) buf;
	size_t sizd = offsetof(struct kernel_dirent64, d_name)
		  - offsetof(struct dirent, d_name);

	while ((char*) k < end)
	{
	  struct kernel_dirent64 *nk = (char *) k + k->d_reclen;
	  size_t name_len = k->d_reclen - offsetof(struct kernel_dirent64, d_name);

	  u->d_ino = k->d_ino;
	  u->d_off = k->d_off;
	  u->d_reclen = k->d_reclen - sizd;
	  u->d_type = k->d_type;
	  memcpy (u->d_name, k->d_name, name_len);

	  u = (char *) u + u->d_reclen;
	  k = nk;
	}

	return (size_t) u - (size_t) buf;
}

ssize_t
__getdents (int fd, char *buf, size_t nbytes)
{

  int ret;

  ret = INLINE_SYSCALL (getdents64, 3, fd, buf, nbytes);
  if (ret == -1)
	  return ret;

  return conv(buf, ret);
}

