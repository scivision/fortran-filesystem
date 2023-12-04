program test_binpath

use, intrinsic :: iso_fortran_env, only : stderr=>error_unit
use filesystem

implicit none

if(command_argument_count() < 1) stop "please specify command line parameters as in CMakeLists.txt"

call test_lib_path()

contains


subroutine test_lib_path()

character(:), allocatable :: binpath, bindir
character(256) :: name
integer :: i, L
character :: s
logical :: shared

call get_command_argument(1, s, length=L, status=i)
if(i/=0) error stop "ERROR:test_binpath:test_lib_path: get_command_argument failed"
if(L/=1) error stop "ERROR:test_binpath: expected argument 0 for static or 1 for shared"
shared = s == '1'

allocate(character(get_max_path()) :: binpath)
allocate(character(get_max_path()) :: bindir)

binpath = lib_path()
bindir = lib_dir()

if(.not. shared) then
  if (len(binpath) /= 0) error stop "ERROR:test_binpath: lib_path should be empty for static library: " // binpath
  if (len(bindir) /= 0) error stop "ERROR:test_binpath: lib_dir should be empty for static library: " // bindir
  print *, "SKIPPED: lib_path/lib_dir: static library"
  return
endif

call get_command_argument(2, name, length=L, status=i)
if(i/=0) error stop "ERROR:test_binpath:test_lib_path: get_command_argument failed"
if(L<1) error stop "ERROR:test_binpath: expected lib_name as third argument"

i = index(binpath, trim(name))
if (i<1) error stop "ERROR:test_binpath: lib_path not found correctly: " // binpath // ' with name ' // trim(name)

print *, "OK: lib_path: ", binpath

if(len_trim(bindir)==0 .and. is_cygwin()) then
  print *, "SKIPPED: lib_dir: cygwin does not support lib_dir"
  return
endif

if(.not. same_file(parent(binpath), bindir)) then
  write(stderr,*) "ERROR:test_binpath: lib_dir not found correctly: " // parent(binpath) // ' /= ' // bindir
  error stop
endif

print *, "OK: lib_dir: ", bindir

deallocate(binpath)
deallocate(bindir)

end subroutine

end program
