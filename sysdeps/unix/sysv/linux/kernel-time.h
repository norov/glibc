/* Helpers to convert struct __timespec to user-visible timespec.

   Copyright (C) 1991-2016 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#if SUPPORT_64BIT_TIME_TYPES
# define conv_timespec(ts, _ts)					\
  do								\
    {								\
      (ts)->tv_sec = (_ts)->tv_sec;				\
      (ts)->tv_nsec = (_ts)->tv_nsec;				\
    }								\
  while (0)
# define stat_conv_timespecs(__stat)				\
  do								\
    {								\
      conv_timespec (&(__stat)->st_atim, &(__stat)->__st_atim);	\
      conv_timespec (&(__stat)->st_mtim, &(__stat)->__st_mtim);	\
      conv_timespec (&(__stat)->st_ctim, &(__stat)->__st_ctim);	\
    }								\
  while (0)
#else
# define conv_timespec(ts, _ts)		do {} while (0)
# define stat_conv_timespecs(__stat)	do {} while (0)
#endif

