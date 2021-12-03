submodule (pathlib) pathlib_gcc

implicit none (type, external)

contains

module procedure is_directory
!! For GCC Gfortran, similar for other compilers
integer :: i, statb(13)
character(:), allocatable :: wk
type(path_t) :: w

w = self%expanduser()
wk = w%path_str

!! must not have trailing slash on Windows
i = len_trim(wk)
if (wk(i:i) == char(92) .or. wk(i:i) == '/') wk = wk(1:i-1)


inquire(file=wk, exist=is_directory)
if(.not.is_directory) return

i = stat(wk, statb)
if(i /= 0) then
  is_directory = .false.
  return
endif

i = iand(statb(3), O'0040000')
is_directory = i == 16384

! print '(O8)', statb(3)

end procedure is_directory



module procedure executable

integer :: s(13), iu, ig, ierr

executable = .false.

if (.not. self%is_file()) return

ierr = stat(self%path_str, s)
if(ierr /= 0) return

iu = iand(s(3), O'0000100')
ig = iand(s(3), O'0000010')
executable = (iu == 64 .or. ig == 8)

end procedure executable


end submodule pathlib_gcc
