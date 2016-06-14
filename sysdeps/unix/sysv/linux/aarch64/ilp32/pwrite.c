#include <sysdeps/unix/sysv/linux/generic/wordsize-32/pwrite.c>

weak_alias (__libc_pwrite, __pwrite64)
weak_alias (__libc_pwrite, __libc_pwrite64)
libc_hidden_weak (__pwrite64)
weak_alias (__libc_pwrite, pwrite64)
