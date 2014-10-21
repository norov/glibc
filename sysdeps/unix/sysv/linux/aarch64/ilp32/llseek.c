#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include <sysdep.h>
#include <sys/syscall.h>

#include "overflow.h"

extern loff_t __llseek (int fd, loff_t offset, int whence);

loff_t
__llseek (int fd, loff_t offset, int whence)
{
  return INLINE_SYSCALL (_llseek, 3, fd, offset, whence);
}
weak_alias (__llseek, llseek)
strong_alias (__llseek, __libc_lseek64)
strong_alias (__llseek, __lseek64)
weak_alias (__llseek, lseek64)

