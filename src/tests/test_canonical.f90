program test_canonical

use pathlib, only : path_t, cwd, same_file, resolve, mkdir

implicit none (type, external)

type(path_t) :: cur, parent, file, p1, p2
character(*), parameter :: dummy = "nobody.txt"

integer :: L1, L2, L3

! -- current directory  -- old MacOS doesn't handle "." or ".." alone
cur = path_t(".")
cur = cur%resolve()
L1 = cur%length()
if (L1 < 1) error stop "canonical '.' " // cur%path()

if (cur%path() /= cwd()) error stop "canonical('.') " // cur%path() // " /= cwd: " // cwd()

print *, "OK: current dir = ", cur%path()
! -- home directory
p1 = path_t("~")
p1 = p1%resolve()
if (p1%path(1,1) == "~") error stop "%resolve ~" // p1%path()
if (resolve("~") == "~") error stop "resolve('~')"
print *, "OK: home dir = ", p1%path()

p2 = path_t(p1%parent())
if (p2%length() >= p1%length()) error stop "parent home " // p2%path()

! -- relative dir
parent = path_t("~/..")
parent = parent%resolve()

L2 = parent%length()
if (L2 < 1) error stop 'directory was not canonicalized: ' // parent%path()

print *, 'OK: parent = ', parent%path()

! -- relative file
file = path_t('~/../' // dummy)
file = file%resolve()
L3 = file%length()
if (L3 - L2 /= len(dummy) + 1) error stop 'file was not canonicalized: ' // file%path()

print *, 'OK: canon_file = ', file%path()


call test_same_file()
print *, "OK: same_file"


contains


subroutine test_same_file()

type(path_t) :: p1, p2

call mkdir("test-a/b")

p1 = path_t("test-a/c")
call p1%touch()

p2 = path_t("test-a/b/../c")

if (.not. p1%same_file(p2)) error stop 'ERROR: %same_file'
if (.not. same_file(p1%path(), p2%path())) error stop 'ERROR: same_file()'

end subroutine test_same_file

end program
