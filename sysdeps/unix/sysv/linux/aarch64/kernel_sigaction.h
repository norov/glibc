
#define HAVE_SA_RESTORER

/* This is the sigaction structure in aarch64 kernel.
   Note the ILP32 struct uses the same struct as LP64
   which is why the fields are 64bit in size. */
struct kernel_sigaction {
	unsigned long long k_sa_handler;
	unsigned long long sa_flags;
	unsigned long long sa_restorer;
	sigset_t sa_mask;
};
