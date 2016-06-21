#include <sysdeps/unix/sysv/linux/arm/pread64.c>

strong_alias (__libc_pread64, __libc_pread)
strong_alias (__libc_pread64, __pread)
weak_alias (__libc_pread64, pread)
