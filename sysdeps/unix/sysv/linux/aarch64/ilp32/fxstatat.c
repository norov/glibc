#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <sys/syscall.h>


struct __kernel_stat64 {
	uint64_t	st_dev;
	unsigned char   __pad0[4];

	uint32_t	__st_ino;
	uint32_t	st_mode;
	uint32_t	st_nlink;

	uint32_t	st_uid;
	uint32_t	st_gid;

	uint64_t	st_rdev;
	unsigned char   __pad3[4];

	int64_t		st_size;
	uint32_t	st_blksize;
	uint64_t	st_blocks;	/* Number of 512-byte blocks allocated. */

	uint32_t	st_atim;
	uint32_t	st_atim_nsec;

	uint32_t	st_mtim;
	uint32_t	st_mtim_nsec;

	uint32_t	st_ctim;
	uint32_t	st_ctim_nsec;

	uint64_t st_ino;
};

static int
xstat_conv (struct __kernel_stat64 *kbuf, struct stat *buf)
{
	/* Convert current kernel version of `struct stat64' to
           `struct stat'.  */
	buf->st_dev = kbuf->st_dev;
	buf->st_ino = kbuf->st_ino;
	buf->st_mode = kbuf->st_mode;
	buf->st_nlink = kbuf->st_nlink;
	buf->st_uid = kbuf->st_uid;
	buf->st_gid = kbuf->st_gid;
	buf->st_rdev = kbuf->st_rdev;
	buf->st_size = kbuf->st_size;
	buf->st_blksize = kbuf->st_blksize;
	buf->st_blocks = kbuf->st_blocks;
	buf->st_atim.tv_sec = kbuf->st_atim;
	buf->st_atim.tv_nsec = kbuf->st_atim_nsec;
	buf->st_mtim.tv_sec = kbuf->st_mtim;
	buf->st_mtim.tv_nsec = kbuf->st_mtim_nsec;
	buf->st_ctim.tv_sec = kbuf->st_ctim;
	buf->st_ctim.tv_nsec = kbuf->st_ctim_nsec;
  return 0;
}

/* Get information about the file NAME relative to FD in ST.  */
int
__fxstatat (int vers, int fd, const char *file, struct stat *st, int flag)
{
  int result;
  struct __kernel_stat64 st64;

  result = INLINE_SYSCALL (fstatat64, 4, fd, file, &st64, flag);
  if (result)
    {
      __set_errno (result);
      return -1;
    }

  return xstat_conv (&st64, st);
}
libc_hidden_def (__fxstatat)

#ifdef XSTAT_IS_XSTAT64
# undef __fxstatat64
strong_alias (__fxstatat, __fxstatat64);
libc_hidden_ver (__fxstatat, __fxstatat64)
#endif
