#error "This file must be written based on the data type sizes of the target"

#define __WORDSIZE			/* Bits ber word (size of CPU register.  */
#define __WORDSIZE32_SIZE_ULONG		/* Set to 1 if port is working in 32-bit mode
					   on 64-bit CPU to define SIZE_MAX correctly.
					   Keep undefined for 64-bit ports.  */
#define __WORDSIZE32_PTRDIFF_LONG	/* Likewize for PTRDIFF_MIN and PTRDIFF_MAX.  */
#define __WORDSIZE_TIME64_COMPAT32	/* Set to 1 to force time types to be 32-bit
					   in struct lastlog and struct utmp{,x}
					   if your 64-bit port should be compatible to
					   32-bit old ports.  */
