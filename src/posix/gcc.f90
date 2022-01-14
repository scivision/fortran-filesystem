submodule (pathlib) posix_gcc
!! functions for GCC on POSIX systems

implicit none (type, external)

contains

module procedure is_symlink
!! https://www.intel.com/content/www/us/en/develop/documentation/fortran-compiler-oneapi-dev-guide-and-reference/top/language-reference/a-to-z-reference/s-1/stat.html

integer :: i, ftmode, statb(13)
character(:), allocatable :: wk

is_symlink = .false.

wk = expanduser(path)
if(len_trim(wk) == 0) return

inquire(file=wk, exist=is_symlink)
if(.not.is_symlink) return

i = lstat(wk, statb)
if(i /= 0) then
  is_symlink = .false.
  return
endif

ftmode = iand(statb(3), O'0170000') !< file type mode
! print '(a,O8)', "TRACE:is_symlink lstat(3) file type octal: ", ftmode

i = iand(ftmode, O'0120000')
is_symlink = i == 40960

end procedure is_symlink

end submodule posix_gcc
