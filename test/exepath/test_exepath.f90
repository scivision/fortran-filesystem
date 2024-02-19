program test_binpath

use, intrinsic :: iso_fortran_env, only : stderr=>error_unit
use filesystem

implicit none

if(command_argument_count() < 1) stop "please specify command line parameters as in CMakeLists.txt"

call test_exe_path()

contains



subroutine test_exe_path()

character(:), allocatable :: binpath, bindir
integer :: i, L
character(256) :: exe_name

call get_command_argument(1, exe_name, length=L, status=i)
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


end program
