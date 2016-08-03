#define __NR_stat		__NR_stat64
#define __NR_fstat		__NR_fstat64
#define __NR_newfstatat		__NR_fstatat64
#define __NR_lstat		__NR_lstat64

#define XSTAT_IS_XSTAT64	1
#define STAT_IS_KERNEL_STAT
