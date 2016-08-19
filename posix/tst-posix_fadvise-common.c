/* Common posix_fadvise tests definitions.
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

#include <test-skeleton.c>

static char *temp_filename;
static int temp_fd;
static char fifoname[] = "/tmp/tst-posix_fadvise-fifo-XXXXXX";
static int fifofd;

static void
do_prepare (void)
{
  temp_fd = create_temp_file ("tst-posix_fadvise.", &temp_filename);
  if (temp_fd == -1)
    {
      printf ("cannot create temporary file: %m\n");
      exit (1);
    }

  if (mktemp (fifoname) == NULL)
    { 
      printf ("%s: cannot generate temp file name: %m\n", __func__);
      exit (1);
    }
  add_temp_file (fifoname);

  if (mkfifo (fifoname, S_IWUSR | S_IRUSR) != 0)
    {
      printf ("%s: cannot create fifo: %m\n", __func__);
      exit (1);
    }

  fifofd = open (fifoname, O_RDONLY | O_NONBLOCK);
  if (fifofd == -1)
    {
      printf ("%s: cannot open fifo: %m\n", __func__);
      exit (1);
    }
}

#define FAIL(str) \
  do { printf ("error: %s (line %d)\n", str, __LINE__); return 1; } while (0)

/* Effectivelly testing posix_fadvise is hard because side effects are not
   observed without checking either performance or any kernel specific
   supplied information.  Also, the syscall is meant to be an advisory,
   so kernel is free to use these information in which way it seems as
   fit (even ignoring it).
   
   This test check for some invalid returned operation to check argument
   passing and if implementation follows POSIX error definition.  */
static int
do_test_common (void)
{
  /* Add some data to file and ensure it is written down on disk.  */
  char buffer[2048] = { 0xcd };

  if (write (temp_fd, buffer, 2048) != 2048)
    FAIL ("write returned a value different than expected 2048");

  if (fsync (temp_fd) != 0)
    FAIL ("fsync failed");

  /* Test passing an invalid fd.  */
  if (posix_fadvise (-1, 0, 0, POSIX_FADV_NORMAL) != EBADF)
    FAIL ("posix_fadvise with invalid fd did not return EBADF");

  /* Test passing an invalid operation.  */
  if (posix_fadvise (temp_fd, 0, 0, -1) != EINVAL)
    FAIL ("posix_fadvise with invalid advise did not return EINVAL");

  /* Test passing a FIFO fd.  */
  if (posix_fadvise (fifofd, 0, 0, POSIX_FADV_NORMAL) != ESPIPE)
    FAIL ("posix_advise with PIPE fd did not return ESPIPE");

  /* Default fadvise on all file starting at initial position.  */
  if (posix_fadvise (temp_fd, 0, 0, POSIX_FADV_NORMAL) != 0)
    FAIL ("default posix_fadvise failed");

  if (posix_fadvise (temp_fd, 0, 4096, POSIX_FADV_NORMAL) != 0)
    FAIL ("posix_fadvise failed (offset = 0, len = 4096) failed");

  if (posix_fadvise (temp_fd, 4096, 0, POSIX_FADV_NORMAL) != 0)
    FAIL ("posix_fadvise failed (offset = 4096, len = 0) failed");

  return 0;
}
