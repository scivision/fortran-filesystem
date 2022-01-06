#ifdef __GFORTRAN__
  include "compiler/gcc.f90.inc"
#elif defined(__INTEL_COMPILER)
  include "compiler/intel.f90.inc"
#else
  include "compiler/unknown.f90.inc"
#endif
