#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <sysdep.h>
#include <sys/syscall.h>

#define __lxstat64 __lxstat64_disable

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
__lxstat (int vers, const char *name, struct stat *buf)
{
  if (vers == _STAT_VER_KERNEL)
    {
    struct stat64 st64;
    int rc = INLINE_SYSCALL (fstatat64, 4, AT_FDCWD, name, &st64,
                           AT_SYMLINK_NOFOLLOW);
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

hidden_def (__lxstat)
weak_alias (__lxstat, _lxstat);
#ifdef XSTAT_IS_XSTAT64
#undef __lxstat64
strong_alias (__lxstat, __lxstat64);
hidden_ver (__lxstat, __lxstat64)
#endif
