/*
C Standard Library convenience interface for Fortran
based on https://github.com/urbanjost/M_system/blob/master/src/C-M_system.c (Public domain)
https://man7.org/linux/man-pages/man2/utime.2.html

Works with MacOS, Linux and MinGW/MSYS2, but NOT Intel oneAPI on Windows
*/
#include <time.h>
#include <utime.h>

int utime_cf(const char *file) {
   struct utimbuf ut;
   /* time_t ut[2]; */

   time_t epoch_sec = time(NULL);

   ut.actime  = epoch_sec;
   ut.modtime = epoch_sec;
   int ierr = utime(file, &ut);

   return ierr;
}
