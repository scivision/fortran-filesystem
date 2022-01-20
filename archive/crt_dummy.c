int utime_cf(const char *file) {
   /* NOOP for systems without utime.h or sys/utime.h */
   return 0;
}
