program test_binpath

use, intrinsic :: iso_fortran_env, only : stderr=>error_unit
use filesystem

implicit none

if(command_argument_count() < 1) stop "please specify command line parameters as in CMakeLists.txt"

call test_lib_path()

contains


subroutine test_lib_path()

character(:), allocatable :: binpath
character(256) :: name
integer :: i, L
character :: s
logical :: shared

call get_command_argument(1, s, length=L, status=i)
if(i/=0) error stop "ERROR:test_binpath:test_lib_path: get_command_argument failed"
if(L/=1) error stop "ERROR:test_binpath: expected argument 0 for static or 1 for shared"
shared = s == '1'

binpath = lib_path()

if(.not. shared) then
  if (len_trim(binpath) /= 0) error stop "ERROR:test_binpath: lib_path should be empty for static library: " // trim(binpath)
  write(stderr,'(a)') "SKIPPED: lib_path not available: static library"
  error stop 77
endif

call get_command_argument(2, name, length=L, status=i)
if(i/=0) error stop "ERROR:test_binpath:test_lib_path: get_command_argument failed"
if(L<1) error stop "ERROR:test_binpath: expected lib_name as third argument"

i = index(binpath, trim(name))
if (i<1) error stop "ERROR:test_binpath: lib_path not found correctly: " // trim(binpath) // ' with name ' // trim(name)

print *, "OK: lib_path: ", trim(binpath)

end subroutine

end program
