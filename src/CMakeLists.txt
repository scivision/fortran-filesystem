include(CheckIncludeFile)
include(CheckSymbolExists)

set_directory_properties(PROPERTIES LABELS pathlib)

target_sources(pathlib PRIVATE pathlib.f90 pure.f90 pure_iter.f90 impure.f90
io.f90 find.f90 envvar.f90)

check_include_file(utime.h HAVE_UTIME_H)
if(HAVE_UTIME_H)
  check_symbol_exists(utime utime.h HAVE_UTIME)
else()
  check_include_file(sys/utime.h HAVE_SYS_UTIME_H)
  if(HAVE_SYS_UTIME_H)
    if(WIN32)
      check_symbol_exists(_utime sys/utime.h HAVE_WIN32_UTIME)
    else()
      check_symbol_exists(utime sys/utime.h HAVE_UTIME)
    endif()
  endif()
endif()

if(HAVE_UTIME)
  target_sources(pathlib PRIVATE posix/crt.c)
elseif(HAVE_WIN32_UTIME)
  target_sources(pathlib PRIVATE windows/crt.c)
else()
  target_sources(pathlib PRIVATE crt_dummy.c)
endif()

target_compile_definitions(pathlib PRIVATE
$<$<BOOL:${HAVE_UTIME_H}>:HAVE_UTIME_H>
$<$<BOOL:${HAVE_SYS_UTIME_H}>:HAVE_SYS_UTIME_H>
)

if(WIN32)
  add_subdirectory(windows)
else()
  add_subdirectory(posix)
endif()

add_subdirectory(compiler)
