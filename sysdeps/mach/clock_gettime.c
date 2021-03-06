/* Copyright (C) 1991-2019 Free Software Foundation, Inc.
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
   <https://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <time.h>
#include <mach.h>
#include <shlib-compat.h>

/* Get the current time of day, putting it into *TS.
   Returns 0 on success, -1 on errors.  */
int
__clock_gettime (clockid_t clock_id, struct timespec *ts)
{
  if (clock_id != CLOCK_REALTIME)
    {
      errno = EINVAL;
      return -1;
    }

  /* __host_get_time can only fail if passed an invalid host_t.
     __mach_host_self could theoretically fail (producing an
     invalid host_t) due to resource exhaustion, but we assume
     this will never happen.  */
  time_value_t tv;
  __host_get_time (__mach_host_self (), &tv);
  TIME_VALUE_TO_TIMESPEC (&tv, ts);
  return 0;
}
libc_hidden_def (__clock_gettime)

versioned_symbol (libc, __clock_gettime, clock_gettime, GLIBC_2_17);
/* clock_gettime moved to libc in version 2.17;
   old binaries may expect the symbol version it had in librt.  */
#if SHLIB_COMPAT (libc, GLIBC_2_2, GLIBC_2_17)
strong_alias (__clock_gettime, __clock_gettime_2);
compat_symbol (libc, __clock_gettime_2, clock_gettime, GLIBC_2_2);
#endif
