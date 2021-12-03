program demo

use, intrinsic :: iso_c_binding, only : c_null_char
use pathlib, only : path_t, cwd

implicit none (type, external)

type(path_t) :: cur, parent, file, p1, p2
character(*), parameter :: dummy = "nobody.txt"

integer :: L1, L2, L3

! -- current directory  -- old MacOS doesn't handle "." or ".." alone
cur = path_t(".")
cur = cur%resolve()
L1 = cur%length()
if (L1 < 1) error stop "ERROR canonical '.' " // cur%path()

if (cur%path() /= cwd()) error stop "ERROR current directory " // cwd()

print *, "OK: current dir = ", cur%path()
! -- home directory
p1 = path_t("~")
p1 = p1%resolve()
if (p1%path(1,1) == "~") error stop "resolve ~" // p1%path()
print *, "OK: home dir = ", p1%path()

p2 = path_t(p1%parent())
if (p2%length() >= p1%length()) error stop "parent home " // p2%path()

! -- relative dir
parent = path_t("~/..")
parent = parent%resolve()

L2 = parent%length()
if (L2 < 1) error stop 'ERROR: directory was not canonicalized: ' // parent%path()

print *, 'OK: parent = ', parent%path()
! -- relative file
file = path_t('~/../' // dummy)
file = file%resolve()
L3 = file%length()
if (L3 - L2 /= len(dummy) + 1) error stop 'ERROR: file was not canonicalized: ' // file%path()

print *, 'OK: canon_file = ', file%path()

! --- same_file
p1 = path_t("a/c")
p2 = path_t("a/b/../c")

if (.not. p1%same_file(p2)) error stop 'ERROR: same_file'


print *, "OK: canonical"
end program
