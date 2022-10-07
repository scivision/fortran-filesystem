program test_find

use, intrinsic :: iso_fortran_env, only : stderr => error_unit
use filesystem, only : remove, mkdir, make_absolute, is_windows, touch, is_file

implicit none

call test_make_absolute()
print *, "OK: make_absolute"

contains


subroutine test_make_absolute()

character(:), allocatable :: fn2

if (is_windows()) then
  fn2 = make_absolute("rel", "j:/foo")
  if (fn2 /= "j:/foo/rel") error stop "did not make_absolute Windows j:/foo/rel, got: " // fn2
else
  fn2 = make_absolute("rel", "/foo")
  if (fn2 /= "/foo/rel") error stop "did not make_absolute Unix /foo/rel, got: " // fn2
endif

if(make_absolute("rel", "") /= "rel") error stop "make_absolute empty root: " // make_absolute("rel", "")

if(make_absolute("", "") /= "") error stop "make_absolute empty both: " // make_absolute("", "")

if(make_absolute("", "rel") /= "rel") error stop "make_absolute empty base: " //make_absolute("", "rel")

end subroutine test_make_absolute

end program
