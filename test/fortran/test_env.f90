program test_env

use, intrinsic :: iso_fortran_env, only : stderr=>error_unit
use filesystem, only : path_t, get_cwd, exists, get_tempdir, get_homedir, is_windows

implicit none

call test_exists()
print '(a)', "OK fs: exists"

call test_homedir()
print '(a)', "OK fs: homedir"

if (len_trim(get_tempdir()) == 0) error stop "get_tempdir failed"
print '(a)', "OK: get_tempdir: " // get_tempdir()


contains


subroutine test_exists()

type(path_t) :: p1

if(exists("")) error stop "empty does not exist"

p1 = path_t(get_cwd())

if(.not. p1%exists()) error stop "%exists() failed"
if(.not. exists(get_cwd())) error stop "exists(get_cwd) failed"

end subroutine


subroutine test_homedir()

character(:), allocatable :: h, k
character(1000) :: buf
integer :: i

if(is_windows()) then
  k = "USERPROFILE"
else
  k = "HOME"
end if

call get_environment_variable(k, buf, status=i)
if (i/=0) then
  print '(a)', "env var " // k // " not set"
else
  print '(a)', "get_environment_variable: " // k // " = " // trim(buf)
endif

h = get_homedir()

if (len_trim(h) == 0) error stop "get_homedir failed: zero length result. This can happen on to CI due to getpwuid() restrictions."
print '(a)', "get_homedir: " // h

end subroutine


end program
