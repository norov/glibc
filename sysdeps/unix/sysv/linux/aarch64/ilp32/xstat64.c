#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <sysdep.h>
#include <sys/syscall.h>

int
__xstat64 (int vers, const char *name, struct stat64 *buf)
{
      int rc = INLINE_SYSCALL (fstatat64, 4, AT_FDCWD, name, buf, 0);
      if (rc)
          return -1;

	return 0;
}
hidden_def (__xstat64)
