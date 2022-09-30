program test_cpp_fs
!! test methods from C++ filesystem

use, intrinsic :: iso_fortran_env, only : stderr=>error_unit
use filesystem, only : path_t, get_cwd, exists, get_tempdir, get_homedir

implicit none

call test_exists()
print *, "OK fs: exists"

if (len_trim(get_homedir()) == 0) error stop "get_homedir failed"
print *, "OK: get_homedir"

if (len_trim(get_tempdir()) == 0) then
  write(stderr,*) "get_tempdir failed"
else
  print *, "OK: get_tempdir"
endif

contains


subroutine test_exists()

type(path_t) :: p1

if(exists("")) error stop "empty does not exist"

p1 = path_t(get_cwd())

if(.not. p1%exists()) error stop "%exists() failed"
if(.not. exists(get_cwd())) error stop "exists(get_cwd) failed"

end subroutine test_exists


end program
