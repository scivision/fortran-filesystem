program test_binpath

use, intrinsic :: iso_fortran_env, only : stderr=>error_unit
use filesystem, only : exe_path, exe_dir, lib_path, lib_dir, is_macos, is_windows, parent, same_file, get_max_path

implicit none

if(command_argument_count() < 1) stop "please specify command line parameters as in CMakeLists.txt"

call test_exe_path()

call test_lib_path()

contains


subroutine test_exe_path()

character(:), allocatable :: binpath, bindir
integer :: i, L
character(256) :: exe_name

call get_command_argument(2, exe_name, length=L, status=i)
if(i/=0) error stop "ERROR:test_binpath:test_exe_path: get_command_argument failed"
if(L<1) error stop "ERROR:test_binpath: expected exe_name as second argument"

allocate(character(get_max_path()) :: binpath)
allocate(character(get_max_path()) :: bindir)

binpath = exe_path()
bindir = exe_dir()

i = index(binpath, trim(exe_name))
if (i<1) then
  write(stderr, '(a,i3)') "ERROR:test_binpath: exe_path not found correctly: " // binpath // " " // trim(exe_name), i
  error stop
endif

if(.not. same_file(bindir, parent(binpath))) then
  write(stderr, '(a)') "ERROR:test_binpath: exe_dir not found correctly: " // bindir // " " // parent(binpath)
  error stop
endif

print *, "OK: exe_path: ", binpath
print *, "OK: exe_dir: ", bindir

deallocate(binpath)
deallocate(bindir)

end subroutine


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

call get_command_argument(3, name, length=L, status=i)
if(i/=0) error stop "ERROR:test_binpath:test_lib_path: get_command_argument failed"
if(L<1) error stop "ERROR:test_binpath: expected lib_name as third argument"

i = index(binpath, trim(name))
if (i<1) error stop "ERROR:test_binpath: lib_path not found correctly: " // binpath // ' with name ' // trim(name)

if(.not. same_file(parent(binpath), bindir)) then
  write(stderr,*) "ERROR:test_binpath: lib_dir not found correctly: " // parent(binpath) // ' /= ' // bindir
  error stop
endif

print *, "OK: lib_path: ", binpath
print *, "OK: lib_dir: ", bindir

deallocate(binpath)
deallocate(bindir)

end subroutine

end program
