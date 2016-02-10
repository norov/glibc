//#include <sysdeps/unix/sysv/linux/generic/xstat.c>
//#include <sysdeps/unix/sysv/linux/i386/xstat.c>
//#include <sysdeps/unix/sysv/linux/xstat.c>
//#include <sysdeps/unix/sysv/linux/generic/wordsize-32/xstat.c>

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <sysdep.h>
#include <sys/syscall.h>

static int
xstat32_conv (struct stat64 *kbuf, struct stat *buf)
{
	buf->st_dev = kbuf->st_dev;
	buf->st_mode = kbuf->st_mode;
	buf->st_nlink = kbuf->st_nlink;
	buf->st_uid = kbuf->st_uid;
	buf->st_gid = kbuf->st_gid;
	buf->st_rdev = kbuf->st_rdev;
	buf->st_size = kbuf->st_size;
	buf->st_blksize = kbuf->st_blksize;
	buf->st_blocks = kbuf->st_blocks;
	buf->st_atim.tv_sec = kbuf->st_atim.tv_sec;
	buf->st_atim.tv_nsec = kbuf->st_atim.tv_nsec;
	buf->st_mtim.tv_sec = kbuf->st_mtim.tv_sec;
	buf->st_mtim.tv_nsec = kbuf->st_mtim.tv_nsec;
	buf->st_ctim.tv_sec = kbuf->st_ctim.tv_sec;
	buf->st_ctim.tv_nsec = kbuf->st_ctim.tv_nsec;
  return 0;
}

/* Get information about the file NAME in BUF.  */
int
__xstat (int vers, const char *name, struct stat *buf)
{
  struct stat64 st64;

  if (vers == _STAT_VER_KERNEL)
    {
      int rc = INLINE_SYSCALL (fstatat64, 4, AT_FDCWD, name, buf, 0);
      if (rc)
        {
          __set_errno (rc);
          return -1;
        }

      return xstat32_conv (&st64, buf);
    }

  errno = EINVAL;
  return -1;
}
hidden_def (__xstat)
weak_alias (__xstat, _xstat);
#ifdef XSTAT_IS_XSTAT64
#undef __xstat64
strong_alias (__xstat, __xstat64);
hidden_ver (__xstat, __xstat64)
#endif
