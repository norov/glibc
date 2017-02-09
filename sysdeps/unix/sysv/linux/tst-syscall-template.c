/* Basic tests for syscall template errno setup.

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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>

static int do_test (void);
#define TEST_FUNCTION           do_test ()

/* This defines the `main' function and some more.  */
#include <test-skeleton.c>

static int
do_test (void)
{
  const char str[] = "Hello wold!\n";
  int err;

  errno = 0xDEAD;

  if (errno != 0xDEAD)
    FAIL_EXIT1 ("Cannot access errno");

  /* Write at intectionally invalid file descriptor.
     Function should return -1, and set errno to EBADF.  */
  err = write (-1, str, sizeof (str));

  if (err != -1)
     FAIL_EXIT1 ("Unexpected write success");

  if (errno == 0xDEAD)
     FAIL_EXIT1 ("Errno is not updated");

  if (errno != EBADF)
     FAIL_EXIT1 ("Wrong error code: %d", errno);

  return 0;
}
