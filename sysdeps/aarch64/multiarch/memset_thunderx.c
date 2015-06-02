/* Copyright (C) 2015 Free Software Foundation, Inc.

   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <string.h>
#include <stdint.h>

#define vector16 __attribute__((vector_size(16)))

#if defined(SHARED) && IS_IN (libc)

#if !defined(PREFER_SIZE_OVER_SPEED) && !defined(__OPTIMIZE_SIZE__) && !defined(__mips16)
/* Optimized Cavium ThunderX cacheline memset routines.
   preconditions: s is cache line aligned, n is integral multiple of 128. */

/* Return if the dc zva instruction is available. */
static inline int inhibit_loop_to_libcall
is_zva_available(void)
{
  long long t;
  asm("mrs     %0, dczid_el0":"=r"(t));
  return !(t & 0x10);
}

static inline char inhibit_loop_to_libcall
cache_zero(void * s, size_t n)
{
  char *p = s;
  if (__glibc_unlikely(!is_zva_available ()))
    return 0;

  while (n > 0)
    {
      asm("dc  zva, %0"::"r"(p):"memory");
      /* Workaround IVOpts going funny and producing worse code. */
      asm("":"+r"(p));
      p += 128;
      n -= 128;
    }
  return 1;
}

static inline void inhibit_loop_to_libcall
cache_memset(void * s, vector16 long long pat, size_t n)
{
  vector16 long long * p = s;

  while (n > 0)
    {
      *p = pat;
      *(p+1) = pat;
      *(p+2) = pat;
      *(p+3) = pat;
      *(p+4) = pat;
      *(p+5) = pat;
      *(p+6) = pat;
      *(p+7) = pat;
      p += 128/sizeof(*p);
      n -= 128;
    }
}

#endif

void * inhibit_loop_to_libcall
__memset_thunderx (void *m, int c, size_t n)
{
#if defined(PREFER_SIZE_OVER_SPEED) || defined(__OPTIMIZE_SIZE__)
  char *s = (char *) m;

  while (n-- != 0)
    {
      *s++ = (char) c;
    }

  return m;
#else
  char *s = (char *) m;
  unsigned long long buffer;
  size_t iter;

  /* Duplicate the pattern across the whole vector register */
  vector16 char v = {c,c,c,c,c,c,c,c,c,c,c,c,c,c,c,c};

  /* Get a 64bit version of the splat/dup for use below. */
  vector16 long long vbuffer = (vector16 long long)v;

  /* Work around a register allocator issue in that vbuffer
     and v are not allocated in the same register */
  asm("":"+w"(vbuffer));

  if (n < 64)
    goto small;

  /* Align the array to 16 byte alignment if needed. */
  if (((((size_t)s) & 0xf)))
    {
      /* Note this uses an unaligned store but it is faster than
         having multiple if checks and stores. */
      *(vector16 long long *)s = vbuffer;
      size_t t = 16 - (((size_t)s) & 0xf);
      s += t;
      n -= t;
    }

  /* If more than two cache lines, optimize this by doing a cache line at a time. */
  if (n >= 256)
    {
      vector16 long long *aligned_addr = (vector16 long long *)s;

      /* Align it to the cache line if needed */
      if (((size_t)s) & 127)
	{
	  size_t t = 128 - (((size_t)s) & 127);
	  iter = t / sizeof(*aligned_addr);
	  n -= t;
	  while (iter > 0)
	    {
	      aligned_addr[0] = vbuffer;
	      aligned_addr++;
	      iter--;
	    }
	}
      size_t align = n & ~127;
      if ((c & 0xff) || !cache_zero (aligned_addr, align))
	cache_memset (aligned_addr, vbuffer, align);
      aligned_addr = aligned_addr + align/sizeof(*aligned_addr);
      n -= align;
      /* Pick up the remainder with a bytewise loop.  */
      s = (char*)aligned_addr;
    }

    /* Between 64 and 256 bytes, Store 64 bytes at a time */
    {
      vector16 long long *aligned_addr = (vector16 long long *)s;
      iter = n / (4*sizeof(*aligned_addr));
      n = n % (4*sizeof(*aligned_addr));
      while (iter > 0)
	{
	  aligned_addr[0] = vbuffer;
	  aligned_addr[1] = vbuffer;
	  aligned_addr[2] = vbuffer;
	  aligned_addr[3] = vbuffer;
	  aligned_addr += 4;
	  iter--;
	}

      /* Pick up the remainder with a bytewise loop.  */
      s = (char*)aligned_addr;
    }

small:
  /* < 64 bytes. */
  if (__glibc_unlikely (n & 32))
    {
      *(vector16 long long*)s = vbuffer;
      *(vector16 long long*)(s+16) = vbuffer;
      s += 32;
      n -= 32;
    }

  /* Note these might be unaligned stores but they are faster than doing other checks. */
  if (__glibc_unlikely (n&16))
    {
      *(vector16 long long*)s = vbuffer;
      s += 16;
    }

  buffer = vbuffer[0];
  if (__glibc_unlikely (n&8))
    {
      *(unsigned long long*)s = buffer;
      s += 8;
    }
  if (__glibc_unlikely (n&4))
    {
      *(unsigned int*)s = buffer;
      s += 4;
    }
  if (__glibc_unlikely (n&2))
    {
      *(unsigned short*)s = buffer;
      s += 2;
    }
  if (__glibc_unlikely (n&1))
    {
      *(unsigned char*)s = buffer;
      s += 1;
    }

  return m;

#endif /* not PREFER_SIZE_OVER_SPEED */
}

#endif /* IS_IN(libc) */
