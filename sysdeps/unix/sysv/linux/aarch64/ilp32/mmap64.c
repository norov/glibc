#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#include <sysdep.h>
#include <sys/syscall.h>

/* mmap is provided by mmap as they are the same. */
void *__mmap (void *__addr, size_t __len, int __prot,
		     int __flags, int __fd, __off_t __offset)
{
   void *result;
   result = (void *)
     INLINE_SYSCALL (mmap2, 6, __addr,
                     __len, __prot, __flags, __fd, __offset);
   return result;
}
/* mmap64 is provided by mmap as they are the same. */
void *__mmap64 (void *__addr, size_t __len, int __prot,
		     int __flags, int __fd, __off64_t __offset)
{
   void *result;
   result = (void *)
     INLINE_SYSCALL (mmap2, 6, __addr,
                     __len, __prot, __flags, __fd, __offset);
   return result;
}
weak_alias (__mmap, mmap)
weak_alias (__mmap64, mmap64)
