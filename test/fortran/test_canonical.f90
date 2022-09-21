program test_canon

use, intrinsic:: iso_fortran_env, only : stderr=>error_unit

use filesystem, only : path_t, get_cwd, same_file, canonical, is_dir, is_file

implicit none


call test_canonical()
print *, "OK: canonical full"

contains

subroutine test_canonical()

type(path_t) :: cur, par, file, p1, p2
character(*), parameter :: dummy = "nobody.txt"

integer :: L1, L2, L3

!> empty
if(canonical("") /= "") error stop "resolve('') /= ''"

! -- current directory  -- old MacOS doesn't handle "." or ".." alone
cur = path_t(".")
cur = cur%resolve()
L1 = cur%length()
if (L1 < 1) then
  write(stderr,*) "ERROR: canonical '.' " // cur%path()
  error stop
endif

if (cur%path() /= get_cwd()) then
  write(stderr,*) "ERROR: canonical('.') " // cur%path() // " /= get_cwd: " // get_cwd()
  error stop
endif

print *, "OK: current dir = ", cur%path()
! -- home directory
p1 = path_t("~")
p1 = p1%resolve()
if (p1%path(1,1) == "~") error stop "%resolve ~ did not expanduser: " // p1%path()
if (canonical("~") == "~") error stop "resolve('~') should not be '~'"
print *, "OK: home dir = ", p1%path()

p2 = path_t(p1%parent())
L1 = p2%length()
if (L1 >= p1%length()) error stop "parent home " // p2%path()
print *, "OK: parent home = ", p2%path()


! -- relative dir
par = path_t("~/..")
par = par%resolve()

L2 = par%length()
if (L2 /= L1) then
  write(stderr,*) 'ERROR:canonical:relative: up dir not canonicalized: ~/.. => ' // par%path()
  error stop
endif
print *, 'OK: canon_dir = ', par%path()

! -- relative file
file = path_t('~/../' // dummy)
file = file%resolve()
L3 = file%length()
if (L3 - L2 /= len(dummy) + 1) error stop 'file was not canonicalized: ' // file%path()

print *, 'OK: canon_file = ', file%path()

end subroutine test_canonical


end program
