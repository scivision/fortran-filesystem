function(c_check)

set(CMAKE_REQUIRED_DEFINITIONS -D__STDC_WANT_LIB_EXT1__=1)

check_symbol_exists(getenv_s "stdlib.h" HAVE_GETENV_S)

endfunction()
