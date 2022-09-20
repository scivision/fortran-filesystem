program test_canon

use, intrinsic:: iso_fortran_env, only : stderr=>error_unit

use filesystem, only : path_t, get_cwd, same_file, canonical, mkdir, is_dir, is_file

implicit none


call test_same_file()
print *, "OK: same_file"

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
if (L1 < 1) error stop "canonical '.' " // cur%path()

if (cur%path() /= get_cwd()) error stop "canonical('.') " // cur%path() // " /= get_cwd: " // get_cwd()

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
if (L2 /= L1) error stop 'up directory was not canonicalized: ~/.. => ' // par%path()

print *, 'OK: canon_dir = ', par%path()

! -- relative file
file = path_t('~/../' // dummy)
file = file%resolve()
L3 = file%length()
if (L3 - L2 /= len(dummy) + 1) error stop 'file was not canonicalized: ' // file%path()

print *, 'OK: canon_file = ', file%path()

end subroutine test_canonical


subroutine test_same_file()

type(path_t) :: p1, p2
integer :: i

call mkdir("test-a/b/", status=i)
if(i < 0) then
  write(stderr,'(A)') "mkdir not supported on this platform"
  return
endif

if(.not. is_dir("test-a/b")) error stop "mkdir test-a/b failed"

p1 = path_t("test-a/c")
call p1%touch()
if(.not. is_file("test-a/c")) error stop "touch test-a/c failed"

p2 = path_t("test-a/b/../c")

if (.not. p1%same_file(p2)) error stop 'ERROR: %same_file'
if (.not. same_file(p1%path(), p2%path())) error stop 'ERROR: same_file()'

end subroutine test_same_file

end program
