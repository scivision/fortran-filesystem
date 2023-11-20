program test_expanduser

use, intrinsic:: iso_fortran_env, only : stderr=>error_unit
use filesystem, only : path_t, expanduser, get_homedir

implicit none


if(expanduser("") /= "") error stop "expanduser blank failed"
if(expanduser(".") /= ".") error stop "expanduser dot failed: " // expanduser(".")

if(expanduser("~P") /= "~P") error stop "expanduser ~P failed: " // expanduser("~P")

block
type(path_t) :: p2, p3

p2 = path_t("~")
p2 = p2%expanduser()

if(expanduser("~") /= p2%path()) then
   write(stderr,*) len(expanduser("~")), len(p2%path()), len_trim(expanduser("~")), len_trim(p2%path())
   write(stderr,'(a)') "expanduser() fcn /= method: " // expanduser("~") // " /= " // p2%path()
   error stop
endif

p3 = path_t(p2%path())
p3 = p3%expanduser()
if (p3%path() /= p2%path()) error stop "expanduser idempotent failed"

if(expanduser("~") /= get_homedir()) then
  write(stderr,*) "expanduser ~ failed: " // expanduser("~") // " /= " // get_homedir()
  error stop
endif

if (expanduser("~//") /= expanduser("~/")) error stop "expanduser double separator failed: " // &
   expanduser("~//") // " /= " // expanduser("~/")
end block
print *, "OK: filesystem: expanduser  ", expanduser("~")

end program
