#ifdef __WIN32
  include "windows/crt.f90.inc"
  include "windows/path.f90.inc"
  include "windows/sys.f90.inc"
#else
  include "posix/crt.f90.inc"
  include "posix/path.f90.inc"
  include "posix/sys.f90.inc"
#endif
