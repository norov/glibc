/* Copyright (C) 2007-2014 Free Software Foundation, Inc.
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
#include <stdarg.h>
#include <sys/sem.h>
#include <ipc_priv.h>
#include <sysdep.h>

/* Define a `union semun' that is for glibc here.  */
union semun
{
  int val;			/* value for SETVAL */
  struct semid_ds *buf;		/* buffer for IPC_STAT & IPC_SET */
  unsigned short int *array;	/* array for GETALL & SETALL */
  struct seminfo *__buf;	/* buffer for IPC_INFO */
};

union semun_kernel
{
  int val;			/* value for SETVAL */
  unsigned long long kern_ptr;
};

int __semctl (int semid, int semnum, int cmd, ...);

int
__semctl (int semid, int semnum, int cmd, ...)
{
  union semun arg;
  union semun_kernel arg_real;
  va_list ap;

  va_start (ap, cmd);

  /* Get the argument.  */
  arg = va_arg (ap, union semun);

  va_end (ap);

  arg_real.kern_ptr = 0;
  if (cmd == SETVAL)
    arg_real.val = arg.val;
  else
    arg_real.kern_ptr = (unsigned long long)(uintptr_t)arg.array;

  return INLINE_SYSCALL (semctl, 4, semid, semnum, cmd,
			 arg_real.kern_ptr);
}

#include <shlib-compat.h>
versioned_symbol (libc, __semctl, semctl, GLIBC_2_0);
