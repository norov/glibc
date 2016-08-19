/* Basic fallocate test (no specific flags is checked).
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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static void do_prepare (void);
#define PREPARE(argc, argv)     do_prepare () 
static int do_test (void);
#define TEST_FUNCTION           do_test ()

#define TIMEOUT 20 /* sec.  */

#include <test-skeleton.c>

static char *temp_filename;
static int temp_fd;

void
do_prepare (void)
{
  temp_fd = create_temp_file ("tst-fallocate.", &temp_filename);
  if (temp_fd == -1)
    {
      printf ("cannot create temporary file: %m\n");
      exit (1);
    }
}

#define FAIL(str) \
  do { printf ("error: %s (line %d)\n", str, __LINE__); return 1; } while (0)

static int
do_test_with_offset (off_t offset)
{
  int ret;
  struct stat finfo;
  char bwrite[1024] = { 0xf0 };
  char bread[1024];

  /* It tries to fallocate 1024 bytes from 'offset' and then write 1024 bytes.
     After both operation rewind the file descriptor and read 1024 bytes
     and check if both buffer have the same contents.  */
  ret = fallocate (temp_fd, 0, offset, 1024);
  if (ret == -1)
    FAIL ("fallocate failed");

  ret = fstat (temp_fd, &finfo);
  if (ret == -1)
    FAIL ("fstat failed");

  if (finfo.st_size < (offset + 1024))
    FAIL ("size of first fallocate less than expected (1024)");

  if (lseek (temp_fd, offset, SEEK_SET) == (off_t) -1)
    FAIL ("fseek (0, SEEK_SET) failed");

  if (write (temp_fd, bwrite, 1024) != 1024)
    FAIL ("fail trying to write 1024 bytes");

  if (lseek (temp_fd, offset, SEEK_SET) == (off_t) -1)
    FAIL ("fseek (0, SEEK_SET) failed");

  if (read (temp_fd, bread, 1024) != 1024)
    FAIL ("fail trying to read 1024 bytes");

  if (memcmp (bwrite, bread, 1024) != 0)
    FAIL ("buffer writted different than buffer readed");

  return 0;
}
