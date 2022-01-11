/*
C Runtime Library convenience interface for Fortran
based on https://github.com/urbanjost/M_system/blob/master/src/C-M_system.c (Public domain)

https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/utime-utime32-utime64-wutime-wutime32-wutime64
*/
#include <time.h>
#include <sys/utime.h>

int utime_cf(const char *file) {
   struct _utimbuf ut;
   /* time_t ut[2]; */

   time_t epoch_sec = time(NULL);

   ut.actime  = epoch_sec;
   ut.modtime = epoch_sec;
   int ierr = _utime(file, &ut);

   return ierr;
}
